/*
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 *
 */
/*
    File:       Task.cpp

    Contains:   implements Task class
                    
    
*/

#include "Task.h"
#include "OS.h"
#include "OSMemory.h"
#include "OSAtomic.h"
#include "OSMutexRW.h"
//#include "OSLogger.h"
#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(lcmut);


class TaskThread : public OSThread
{
public:

	//Implementation detail: all tasks get run on TaskThreads.

	TaskThread(TaskThreadPoolBase	*inTaskThreadPool) :  OSThread()/*, fTaskThreadPoolElem()*/,fTaskThreadPool(inTaskThreadPool)
	{/*fTaskThreadPoolElem.SetEnclosingObject(this);*/}
	virtual         ~TaskThread() { this->StopAndWaitForThread(); }

protected:
	enum
	{
		kMinWaitTimeInMilSecs = 10  //UInt32
	};

	virtual void    Entry();

	virtual Task*   WaitForTask();

	void			ProcessTask(Task *inTask);

	void			AssignTask(Task *inTask){ fTaskQueue.EnQueue(&inTask->fTaskQueueElem);}

	UInt32			GetTaskQueueLen(){	return fTaskQueue.GetQueue()->GetLength(); }

protected:

	/*OSQueueElem			fTaskThreadPoolElem;*/

	OSHeap              fHeap;
	OSQueue_Blocking    fTaskQueue;

	TaskThreadPoolBase	*fTaskThreadPool;


	//friend class Task;
	friend class TaskThreadPool;
	friend class STQTaskThreadPool;
};




#define TASK_DEBUG 1

#if TASK_DEBUG
#define  TASKDBG_LOG TRACE
#else
#define TASKDBG_LOG(XXXX) do {} while(0)
#endif

unsigned int    Task::sThreadPicker = 0;

static char* sTaskStateStr="live_"; //Alive

Task::Task(TaskThreadPoolBase * inTaskThreadPool)
:   fEvents(0), fUseThisThread(NULL), fWriteLock(false), fTimerHeapElem(), fTaskQueueElem(),fTaskThreadPool(inTaskThreadPool)
{
#if DEBUG
    fInRunCount = 0;
#endif
    this->SetTaskName("unknown");

	fTaskQueueElem.SetEnclosingObject(this);
	fTimerHeapElem.SetEnclosingObject(this);

	if(NULL == fTaskThreadPool)
	{
		fTaskThreadPool = TaskThreadPool::GetInstance();
	}

}

void Task::SetTaskName(const char* name) 
{
    if (name == NULL) 
        return;
   
   ::strncpy(fTaskName,sTaskStateStr,sizeof(fTaskName));
   ::strncat(fTaskName,name,sizeof(fTaskName));
   fTaskName[sizeof(fTaskName) -1] = 0; //terminate in case it is longer than ftaskname.
   
}

Bool16 Task::Valid()
{
    if  (   (this->fTaskName == NULL)
         || (0 != ::strncmp(sTaskStateStr,this->fTaskName, 5))
         )
     {
#if TASK_DEBUG
		qtss_printf(" Task::Valid Found invalid task = %p\n", (void*)this);
#endif
        return false;
     }
    
    return true;
}

Task::EventFlags Task::GetEvents()
{
    //Mask off every event currently in the mask except for the alive bit, of course,
    //which should remain unaffected and unreported by this call.
    EventFlags events = fEvents & kAliveOff;
    (void)os_atomic_sub(&fEvents, events);
    return events;
}

void Task::Signal(EventFlags events)
{
    if (!this->Valid())
        return;
        
    //Fancy no mutex implementation. We atomically mask the new events into
    //the event mask. Because os_atomic_or returns the old state of the mask,
    //we only schedule this task once.
    events |= kAlive;
    EventFlags oldEvents = os_atomic_or(&fEvents, events);
    if ((!(oldEvents & kAlive))/* && (fTaskThreadPool->m_TaskThreadNum > 0)*/)
    {
		fTaskThreadPool->AssignTask(this);
	}
    else
	{
		TASKDBG_LOG("Task::Signal sent to dead or busy TaskName=%s  q elem=%p  enclosing=%p\n",  fTaskName, (void*)&fTaskQueueElem, (void*)this);
	}
}


void    Task::GlobalUnlock()    
{   
    if (this->fWriteLock){   
		this->fWriteLock = false;   
        fTaskThreadPool->GetRWMutex().Unlock();
    }                                               
}

void  Task::ForceSameThread()   {
	fUseThisThread = (TaskThread*)OSThread::GetCurrent();
	Assert(fUseThisThread != NULL);

#if TASK_DEBUG
	if (fTaskName[0] == 0) ::strcpy(fTaskName, " corrupt task");
	TASKDBG_LOG("Task::ForceSameThread fUseThisThread %p task %s enque elem=%p enclosing %p\n", (void*)fUseThisThread, fTaskName,(void*) &fTaskQueueElem,(void*) this);
#endif

}

SInt64  Task::CallLocked()        
{   
	/*example:            
	if (fModuleState.globalLockRequested) // call this request back locked
		return this->CallLocked();
	*/
	ForceSameThread();
	fWriteLock = true;
	return (SInt64) 10; // minimum of 10 milliseconds between locks
}


void TaskThread::Entry()
{
	OSThreadRunInfo _runinfo((OSTHREAD_ID_t)GetCurrentThreadID(),"TaskThread");
 
	Task* theTask = NULL;
    
    while (true) 
    {
        theTask = this->WaitForTask();

        //
        // WaitForTask returns NULL when it is time to quit
        if (theTask == NULL || false == theTask->Valid() )
            return;

		ProcessTask(theTask);
    }
}


void	TaskThread::ProcessTask(Task* inTask)
{
	Bool16 doneProcessingEvent = false;

	while (!doneProcessingEvent)
	{
		//If a task holds locks when it returns from its Run function,
		//that would be catastrophic and certainly lead to a deadlock
#if DEBUG
		Assert(this->GetNumLocksHeld() == 0);
#endif
#if TASK_DEBUG
		Assert(inTask->fInRunCount == 0);
		inTask->fInRunCount++;
#endif
		inTask->fUseThisThread = NULL; // Each invocation of Run must independently
		// request a specific thread.
		SInt64 theTimeout = 0;

		if (inTask->fWriteLock)
		{   
			OSMutexWriteLocker mutexLocker(&fTaskThreadPool->GetRWMutex());
			TASKDBG_LOG("TaskThread::Entry run global locked TaskName=%s CurMSec=%.3f thread=%p task=%p,fInRunCount=%d\n", 
				inTask->fTaskName, OS::StartTimeMilli_Float() ,(void*) this,(void*) inTask,inTask->fInRunCount);

			theTimeout = inTask->Run();
			inTask->fWriteLock = false;
		}
		else
		{
			OSMutexReadLocker mutexLocker(&fTaskThreadPool->GetRWMutex());
			TASKDBG_LOG("TaskThread::Entry run TaskName=%s CurMSec=%.3f thread=%p task=%p,fInRunCount=%d\n", 
				inTask->fTaskName, OS::StartTimeMilli_Float(), (void*) this,(void*) inTask,inTask->fInRunCount);

			theTimeout = inTask->Run();

		}
#if DEBUG
		Assert(this->GetNumLocksHeld() == 0);
#endif
#if TASK_DEBUG
		inTask->fInRunCount--;
		Assert(inTask->fInRunCount == 0);
#endif          
		if (theTimeout < 0)
		{
#if TASK_DEBUG
			qtss_printf("TaskThread::Entry delete TaskName=%s CurMSec=%.3f thread=%p task=%p\n", inTask->fTaskName, OS::StartTimeMilli_Float(), (void*) this, (void*) inTask);

			inTask->fUseThisThread = NULL;

			if (NULL != fHeap.Remove(&inTask->fTimerHeapElem)) 
				qtss_printf("TaskThread::Entry task still in heap before delete\n");

			if (NULL != inTask->fTaskQueueElem.InQueue())
				qtss_printf("TaskThread::Entry task still in queue before delete\n");

			inTask->fTaskQueueElem.Remove();

			if (inTask->fEvents &~ Task::kAlive)
				qtss_printf ("TaskThread::Entry flags still set  before delete\n");

			(void)os_atomic_sub(&inTask->fEvents, 0);

			::strncat (inTask->fTaskName, " deleted", sizeof(inTask->fTaskName) -1);

#endif
			inTask->fTaskName[0] = 'D'; //mark as dead
			delete inTask;
			inTask = NULL;
			doneProcessingEvent = true;

		}
		else if (theTimeout == 0)
		{
			//We want to make sure that 100% definitely the task's Run function WILL
			//be invoked when another thread calls Signal. We also want to make sure
			//that if an event sneaks in right as the task is returning from Run()
			//(via Signal) that the Run function will be invoked again.
			doneProcessingEvent = os_compare_and_store(Task::kAlive, 0, &inTask->fEvents);
			if (doneProcessingEvent)
				inTask = NULL; 
		}
		else
		{
			//note that if we get here, we don't reset inTask, so it will get passed into
			//WaitForTask
			TASKDBG_LOG("TaskThread::Entry insert TaskName=%s in timer heap thread=%p elem=%p task=%p timeout=%.2f\n", inTask->fTaskName,  (void*) this, (void*) &inTask->fTimerHeapElem,(void*) inTask, (float)theTimeout / (float) 1000);
			inTask->fTimerHeapElem.SetValue(OS::Milliseconds() + theTimeout);
			fHeap.Insert(&inTask->fTimerHeapElem);
			(void)os_atomic_or(&inTask->fEvents, Task::kIdleEvent);
			doneProcessingEvent = true;
		}


#if TASK_DEBUG
		SInt64  yieldStart = OS::Milliseconds();
#endif

		this->ThreadYield();

#if TASK_DEBUG
		SInt64  yieldDur = OS::Milliseconds() - yieldStart;
		static SInt64   numZeroYields;

		if ( yieldDur > 1 )
		{
			qtss_printf( "TaskThread::Entry time in Yield %ld, numZeroYields %ld\n", (long)yieldDur, (long)numZeroYields );
			numZeroYields = 0;
		}
		else
			numZeroYields++;
#endif

	}

}
Task* TaskThread::WaitForTask()
{
    while (!OSThread::GetCurrent()->IsStopRequested())
    {
        SInt64 theCurrentTime = OS::Milliseconds();
        
        if ((fHeap.PeekMin() != NULL) && (fHeap.PeekMin()->GetValue() <= theCurrentTime))
        {    
            TASKDBG_LOG("TaskThread::WaitForTask found timer-task=%s thread %p fHeap.CurrentHeapSize(%lu) taskElem = %p enclose=%p\n",
                ((Task*)fHeap.PeekMin()->GetEnclosingObject())->fTaskName, (void*) this, (unsigned long)fHeap.CurrentHeapSize(), (void*)fHeap.PeekMin(), (void*)fHeap.PeekMin()->GetEnclosingObject());
            return (Task*)fHeap.ExtractMin()->GetEnclosingObject();
        }
    
        //if there is an element waiting for a timeout, figure out how long we should wait.
        SInt64 theTimeout = 0;
        
		//当fHead中任务存在时，修改theTimeout,否则，theTimeout 为0，fTaskQueue.DeQueueBlocking将无限期等待
		if (fHeap.PeekMin() != NULL)
		{
			theTimeout = fHeap.PeekMin()->GetValue() - theCurrentTime;
			Assert(theTimeout >= 0);

			// Make sure we can't go to sleep for some ridiculously short
			// period of time
			if (theTimeout < kMinWaitTimeInMilSecs)
				theTimeout = kMinWaitTimeInMilSecs;
		}
            
	
        //wait...
#if TASK_DEBUG
		 qtss_printf("TaskThread::WaitForTask DeQueueBlocking,theTimeout:%lu\n", (unsigned long)(theTimeout&0XFFFFFFFFUL));
#endif

        OSQueueElem* theElem = fTaskQueue.DeQueueBlocking(this, (SInt32) theTimeout);
        if (theElem != NULL)
        {    
#if TASK_DEBUG
             qtss_printf("TaskThread::WaitForTask found signal-task=%s thread %p fTaskQueue.GetLength(%lu) taskElem = %p enclose=%p\n", ((Task*)theElem->GetEnclosingObject())->fTaskName,  (void*) this, (unsigned long)fTaskQueue.GetQueue()->GetLength(), (void*)theElem,  (void*)theElem->GetEnclosingObject() );
#endif
            return (Task*)theElem->GetEnclosingObject();
        }

        //
        // If we are supposed to stop, return NULL, which signals the caller to stop
        //if (OSThread::GetCurrent()->IsStopRequested())
        //    return NULL;
    } 
	
	return NULL;
}


TaskThreadPool::TaskThreadPool()
:m_TaskThreadNum(0)
{
	memset(&m_TaskThreadArray,0,sizeof(m_TaskThreadArray));
}

TaskThreadPool::~TaskThreadPool()
{
	RemoveThreads(m_TaskThreadNum);
}

#define MY_MIN(a,b)    (((a) < (b)) ? (a) : (b))

TaskThread* TaskThreadPool::CreateThread()
{
	return NEW TaskThread(this);
}

UInt32 TaskThreadPool::AddThreads(UInt32 numToAdd)
{
	numToAdd			= MY_MIN(numToAdd,sMaxNumTaskThreads - m_TaskThreadNum); 
	UInt32 pos			= m_TaskThreadNum;
	m_TaskThreadNum		+= numToAdd;
	
    for (UInt32 x = 0; x < numToAdd; x++)
    {
		m_TaskThreadArray[pos+x] = NEW TaskThread(this);
        m_TaskThreadArray[pos+x]->Start();
    }
    
    return numToAdd;
}


void TaskThreadPool::RemoveThreads(UInt32 numToRel)
{
	if(NULL == m_TaskThreadArray)
	{
		return;
	}

	numToRel		= MY_MIN(numToRel,m_TaskThreadNum);
	m_TaskThreadNum -= numToRel;
	UInt32 pos		= m_TaskThreadNum;
	

    //Tell all the threads to stop
    for (UInt32 x = 0; x < numToRel; x++)
        m_TaskThreadArray[pos+x]->SendStopRequest();

    //Because any (or all) threads may be blocked on the queue, cycle through
    //all the threads, signalling each one
    for (UInt32 y = 0; y < numToRel; y++)
	{
		m_TaskThreadArray[pos+y]->fTaskQueue.Disable();
        m_TaskThreadArray[pos+y]->fTaskQueue.GetCond()->Signal();//通知TaskThread 由事件队列的等待方法中退出
	}
    
    //Ok, now wait for the selected threads to terminate, deleting them and removing
    //them from the queue.
    for (UInt32 z = 0; z < numToRel; z++)
	{
        delete m_TaskThreadArray[pos+z];
		m_TaskThreadArray[pos+z] = NULL;
	}

}

TaskThreadPool* TaskThreadPool::GetInstance()//获取唯一实例。当然，客户也可以创建自己的线程池；
{
	static TaskThreadPool ins;
	return &ins;
}

void	TaskThreadPool::AssignTask(Task *inTask)
{
	TaskThread *theTaskThread  = NULL;
	static UInt32 sThreadPicker = 0;
	
	if(m_TaskThreadNum ==0 ) 
	{
#if TASK_DEBUG     
		qtss_printf("TaskThreadPool::AssignTaskl m_TaskThreadNum ==0");
#endif
		return;
	}
	
	if (inTask->fUseThisThread != NULL)// Task needs to be placed on a particular thread.
	{
		theTaskThread =  inTask->fUseThisThread;
	}
	else
	{
		unsigned int theThreadIdx = os_atomic_add(&sThreadPicker, 1) % m_TaskThreadNum;
		theTaskThread = m_TaskThreadArray[theThreadIdx];
	}

	theTaskThread->AssignTask(inTask);


#if TASK_DEBUG
	if (inTask->fTaskName[0] == 0) ::strcpy(inTask->fTaskName, " corrupt task");
	qtss_printf("Task::Signal enque TaskName=%s  q elem=%p enclosing=%p,"
		"thread=%p userthread=%p,m_TaskThreadNum=%lu,QueueLen=%lu\n", 
		inTask->fTaskName, (void*) &inTask->fTaskQueueElem,(void*)inTask,
		(void*)theTaskThread,(void*)inTask->fUseThisThread,(unsigned long)m_TaskThreadNum,(unsigned long)(theTaskThread->GetTaskQueueLen()));
#endif

}


//////////////////////////////////////////////////////////////////////////


class STQTaskThread : public TaskThread
{
public:
	//Implementation detail: all tasks get run on TaskThreads.
	STQTaskThread(STQTaskThreadPool	*inTaskThreadPool):
	  TaskThread((TaskThreadPoolBase*) inTaskThreadPool),fWorkTaskThreadPool(inTaskThreadPool)
	  {fPoolQueueElem.SetEnclosingObject(this);}

	  virtual ~STQTaskThread(){}

protected:

	virtual Task*   WaitForTask();

	void Entry()
	{
		OSThreadRunInfo _runinfo((OSTHREAD_ID_t)GetCurrentThreadID(),"TaskThread");

		Task* theTask = NULL;

		while (true) 
		{
			theTask = this->WaitForTask();

			//
			// WaitForTask returns NULL when it is time to quit
			if (theTask == NULL || false == theTask->Valid() )
			{
				OSMutexLocker _locker(&fWorkTaskThreadPool->fMutex);
				fWorkTaskThreadPool->SwicthThreadToDead(this);
				return;
			}

			ProcessTask(theTask);

		}
	}
private:
	STQTaskThreadPool*	fWorkTaskThreadPool;
	OSQueueElem			fPoolQueueElem;

	friend class STQScheduleTask;
	friend class STQTaskThreadPool;
};

class STQScheduleTask:public Task
{
public:
	virtual SInt64          Run()
	{
		EventFlags event;
		event = GetEvents();
		if (event & Task::kKillEvent){
			return 0;
		}

		OSQueue	& theTaskThreadIdleQueue = fWorkTaskThreadPool->fTaskThreadIdleQueue;
		OSQueue_Blocking & theTaskQueue = fWorkTaskThreadPool->fTaskQueue;
		if(theTaskThreadIdleQueue.GetLength() > 0 && theTaskQueue.GetQueue()->GetLength() > 0)
		{
			UInt32 i = 0;
			OSMutexLocker _locker(&fWorkTaskThreadPool->fMutex);
			for(OSQueueElem *theElem = theTaskThreadIdleQueue.GetHead();theElem && i++ < theTaskThreadIdleQueue.GetLength();theElem = theElem->Next())
			{
				STQTaskThread *theThread = (STQTaskThread *)theElem->GetEnclosingObject();
				Assert(theThread);
				theThread->fTaskQueue.GetCond()->Signal();
			}
		}

		fWorkTaskThreadPool->ClearDeadThreads();

		return 0;
	}

	STQScheduleTask(STQTaskThreadPool	*inTaskThreadPool):Task(inTaskThreadPool),fWorkTaskThreadPool(inTaskThreadPool)
	{
		SetTaskName("Schedule");
	}

	void Schedule()
	{
		if((fWorkTaskThreadPool->fTaskThreadDeadQueue.GetQueue()->GetLength() > 0)
			||(fWorkTaskThreadPool->fTaskThreadIdleQueue.GetLength() > 0 && fWorkTaskThreadPool->fTaskQueue.GetQueue()->GetLength() > 0)
			)
		{
			this->Signal(Task::kStartEvent);
		}
	}
private:
	STQTaskThreadPool*	fWorkTaskThreadPool;
};

Task*   STQTaskThread::WaitForTask()
{
	while (!OSThread::GetCurrent()->IsStopRequested())
	{
		SInt64 theCurrentTime = OS::Milliseconds();

		if ((fHeap.PeekMin() != NULL) && (fHeap.PeekMin()->GetValue() <= theCurrentTime))
		{    
#if TASK_DEBUG
			qtss_printf("TaskThread::WaitForTask found timer-task=%s thread %p fHeap.CurrentHeapSize(%lu) taskElem = %p enclose=%p\n",
                ((Task*)fHeap.PeekMin()->GetEnclosingObject())->fTaskName, (void*) this, (unsigned long)fHeap.CurrentHeapSize(), (void*)fHeap.PeekMin(), (void*)fHeap.PeekMin()->GetEnclosingObject());
#endif
			return (Task*)fHeap.ExtractMin()->GetEnclosingObject();
		}

		if(OSQueueElem* theElem = fTaskQueue.DeQueue())
		{
			return (Task*)theElem->GetEnclosingObject();
		}

		if(OSQueueElem *theElem = fWorkTaskThreadPool->fTaskQueue.DeQueue())
		{
			fWorkTaskThreadPool->fScheduleTask->Schedule();//看来可能还有活啊，调度
			return (Task*)theElem->GetEnclosingObject();
		}

		//if there is an element waiting for a timeout, figure out how long we should wait.
		SInt64 theTimeout = 0;

		//当fHead中任务存在时，修改theTimeout,否则，theTimeout 为0，fTaskQueue.DeQueueBlocking将无限期等待
		if (fHeap.PeekMin() != NULL)
		{
			theTimeout = fHeap.PeekMin()->GetValue() - theCurrentTime;
			Assert(theTimeout >= 0);

			// Make sure we can't go to sleep for some ridiculously short
			// period of time
			if (theTimeout < kMinWaitTimeInMilSecs)
				theTimeout = kMinWaitTimeInMilSecs;
		}
		else if(fWorkTaskThreadPool->isOverFlowTaskThread())
		{
			theTimeout = 1000;
		}

		//wait...
		TASKDBG_LOG("TaskThread::WaitForTask DeQueueBlocking,theTimeout:%lu\n", (unsigned long)(theTimeout&0XFFFFFFFFUL));
		
		/*if(0 == theTimeout)*/{
			OSMutexLocker _locker(&fWorkTaskThreadPool->fMutex);
			fWorkTaskThreadPool->SwicthThreadToIdle(this);
		}
		
		OSQueueElem* theElem = fTaskQueue.DeQueueBlocking(this, (SInt32) theTimeout);

		/*if(0 == theTimeout)*/{
			OSMutexLocker _locker(&fWorkTaskThreadPool->fMutex);
			fWorkTaskThreadPool->SwicthThreadToActive(this);
		}


		if (theElem != NULL)
		{    
			TASKDBG_LOG("TaskThread::WaitForTask found signal-task=%s thread %p fTaskQueue.GetLength(%lu) taskElem = %p enclose=%p\n",
				((Task*)theElem->GetEnclosingObject())->fTaskName,  (void*) this, (unsigned long)fTaskQueue.GetQueue()->GetLength(), (void*)theElem,  (void*)theElem->GetEnclosingObject() );
			return (Task*)theElem->GetEnclosingObject();
		}
		else if(NULL == fHeap.PeekMin())
		{
			fWorkTaskThreadPool->CheckOverflowThread(this);
		}
		//
		// If we are supposed to stop, return NULL, which signals the caller to stop
		//if (OSThread::GetCurrent()->IsStopRequested())
		//    return NULL;
	} 

	return NULL;

}

STQTaskThreadPool::STQTaskThreadPool():fMinThreadNum(0),fMaxThreadNum(0),fTotalThreadNum(0),fIsEnable(TRUE)
{
	fScheduleTask = new STQScheduleTask(this);
}


STQTaskThreadPool::~STQTaskThreadPool()
{
	Uninitialize();

}

void STQTaskThreadPool::CheckOverflowThread(STQTaskThread * inThread)
{
	if(isOverFlowTaskThread() && (NULL == inThread->fHeap.PeekMin() || !fIsEnable))
	{	
		OSMutexLocker _locker(&fMutex);
		if(isOverFlowTaskThread() && (NULL == inThread->fHeap.PeekMin() || !fIsEnable))
		{
			inThread->SendStopRequest();
			inThread->fTaskQueue.Disable();
			inThread->fTaskQueue.GetCond()->Signal();
		}
	}
}

void STQTaskThreadPool::Initialize(ULONG min,ULONG max)
{
	fMinThreadNum = min;
	fMaxThreadNum = max;
}

void STQTaskThreadPool::Uninitialize()
{
	fIsEnable = FALSE;

    ////通知所有线程退出 
	{
		OSMutexLocker __locker(&fMutex);
		while (OSQueueElem* theElem = fTaskThreadIdleQueue.DeQueue())
		{
			STQTaskThread* theThread = ((STQTaskThread*) theElem->GetEnclosingObject());
			theThread->SendStopRequest();
			theThread->fTaskQueue.Disable();
			theThread->fTaskQueue.GetCond()->Signal();
		}

		while (OSQueueElem* theElem = fTaskThreadActiveQueue.DeQueue())
		{
			STQTaskThread* theThread = ((STQTaskThread*) theElem->GetEnclosingObject());
			theThread->SendStopRequest();
			theThread->fTaskQueue.Disable();
			theThread->fTaskQueue.GetCond()->Signal();
		}
	}

	////等待所有线程退出
	while(fTotalThreadNum >0)
	{
		ClearDeadThreads();
		OSThread::Sleep(10);
	}

	delete fScheduleTask;
	fScheduleTask = NULL;
}

void STQTaskThreadPool::AssignTask(Task * inTask)
{
	if(!fIsEnable) 
	{
		TASKDBG_LOG("reject new task because the pool id disabled\n");
		return;
	}

	if(fTaskThreadIdleQueue.GetLength()>0)
	{
		OSMutexLocker _locker(&fMutex);
		if(OSQueueElem *theElem = fTaskThreadIdleQueue.DeQueue())
		{
			STQTaskThread * theThread = (STQTaskThread * )theElem->GetEnclosingObject();
			theThread->AssignTask(inTask);
			return;
		}
	}

	fTaskQueue.EnQueue(&inTask->fTaskQueueElem); 
	if(fTaskQueue.GetQueue()->GetLength() >0){
		UInt32 i = 0;
		OSMutexLocker _locker(&fMutex);
		for (OSQueueElem *theElem = fTaskThreadIdleQueue.GetHead();
			theElem != NULL && i < fTaskQueue.GetQueue()->GetLength(); theElem = theElem->Next(),i++)
		{
			STQTaskThread * theThread = (STQTaskThread * )theElem->GetEnclosingObject();
			Assert(theThread);
			theThread->fTaskQueue.GetCond()->Signal();
		}
	}

	//add new thread when need
	if( isLackTaskThread())
	{
		OSMutexLocker _locker(&fMutex);
		if(isLackTaskThread())
		{
			STQTaskThread * theTaskThread = new STQTaskThread(this);
			Assert(theTaskThread);
			os_atomic_add(&fTotalThreadNum,1);
			
			OSMutexLocker _locker(&fMutex);
			SwicthThreadToActive(theTaskThread);
			theTaskThread->Start();
		}
	}
}


STQTaskThreadPool* STQTaskThreadPool::GetInstance()//获取唯一实例。当然，客户也可以创建自己的线程池；
{
	static STQTaskThreadPool ins;
	return &ins;
}


void STQTaskThreadPool::ClearDeadThreads()
{
	while (OSQueueElem* theElem = fTaskThreadDeadQueue.DeQueue())
	{
		STQTaskThread* theThread =(STQTaskThread*) theElem->GetEnclosingObject();

		theThread->StopAndWaitForThread();
		delete theThread;
		os_atomic_sub(&fTotalThreadNum,1);
		
		TASKDBG_LOG("STQTaskThreadPool::ClearDeadThreads:(%p) fAvaThreadNum(%d),fTotalThreadNum(%d)\r\n",
			theThread,fTaskThreadIdleQueue.GetLength()+ fTaskThreadActiveQueue.GetLength() ,fTotalThreadNum);
	}
}

void  STQTaskThreadPool::SwicthThreadToActive(STQTaskThread * inTaskThread)
{
	inTaskThread->fPoolQueueElem.Remove();
	fTaskThreadActiveQueue.EnQueue(&inTaskThread->fPoolQueueElem);
	TASKDBG_LOG("TaskThread(%p) Switch Active\r\n",this);
}

void  STQTaskThreadPool::SwicthThreadToIdle(STQTaskThread * inTaskThread)
{
	inTaskThread->fPoolQueueElem.Remove();
	Assert(inTaskThread->fPoolQueueElem.GetEnclosingObject());
	fTaskThreadIdleQueue.EnQueue(&inTaskThread->fPoolQueueElem);
	TASKDBG_LOG("TaskThread(%p) Switch Idle\r\n",this);
}

void  STQTaskThreadPool::SwicthThreadToDead(STQTaskThread * inTaskThread)
{
    inTaskThread->fPoolQueueElem.Remove();
	fTaskThreadDeadQueue.EnQueue(&inTaskThread->fPoolQueueElem);
	TASKDBG_LOG("TaskThread(%p) Switch Dead\r\n",this);
}


//////////////////////////////////////////////////////////////////////////
DWORD WorkTaskThreadPool::worktaskthread_start_entry_t(void* lpThreadParameter)
{
	Task * inTask = (Task*)lpThreadParameter;
	Bool16 doneProcessingEvent = false;
	while (!doneProcessingEvent)
	{
		SInt64 theTimeout = 0;

		theTimeout = inTask->Run();
		        
		if (theTimeout < 0)
		{
			inTask->fTaskName[0] = 'D'; //mark as dead
			delete inTask;
			inTask = NULL;
			doneProcessingEvent = true;
		}
		else if (theTimeout == 0)
		{
			doneProcessingEvent = os_compare_and_store(Task::kAlive, 0, &inTask->fEvents);
			if (doneProcessingEvent)
				inTask = NULL; 
		}
	}
	return 0;
}

void WorkTaskThreadPool::AssignTask(Task *inTask)
{
	
	OSTP_QueueUserWorkItem(worktaskthread_start_entry_t,inTask,inTask->fEvents & Task::kWouldBlock?OSTP_EXECUTEINWAITTHREAD:NULL);
}

WorkTaskThreadPool* WorkTaskThreadPool::GetInstance()//获取唯一实例。当然，客户也可以创建自己的线程池；
{
	static WorkTaskThreadPool ins;
	return &ins;
}

