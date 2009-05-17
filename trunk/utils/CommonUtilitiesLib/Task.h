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
    File:       Task.h

    Contains:   Tasks are objects that can be scheduled. To schedule a task, you call its
                signal method, and pass in an event (events are bits and all events are defined
                below).
                
                Once Signal() is called, the task object will be scheduled. When it runs, its
                Run() function will get called. In order to clear the event, the derived task
                object must call GetEvents() (which returns the events that were sent).
                
                Calling GetEvents() implicitly "clears" the events returned. All events must
                be cleared before the Run() function returns, or Run() will be invoked again
                immediately.
                    
    

	History:
				2009/04/01 Jokul for Tranzda
					add  TGMCOMUTI_API declare ,add some class


    
*/

#ifndef __TASK_H__
#define __TASK_H__
#include "OSBaseTypes.h"
#include "OSQueue.h"
#include "OSHeap.h"
#include "OSThread.h"
#include "OSMutexRW.h"



class  TaskThread;
class  TaskThreadPoolBase;

class  TaskThreadPool;

class  STQTaskThread;
class  STQTaskThreadPool;
class  STQScheduleTask;

class	WorkTaskThreadPool;

//define USE_STQ_TASK_THREAD_POOL
#ifdef USE_STQ_TASK_THREAD_POOL
#define DEFAULT_TASK_THREAD_POOL_INS STQTaskThreadPool::GetInstance()
#else 
#define DEFAULT_TASK_THREAD_POOL_INS WorkTaskThreadPool::GetInstance()
#endif

class TGMCOMUTI_API Task
{
    public:
        
        typedef unsigned int EventFlags;

        //EVENTS
        //here are all the events that can be sent to a task
        enum
        {
            kKillEvent		=   0x1 << 0x0, //these are all of type "EventFlags"
            kIdleEvent		=   0x1 << 0x1,
            kStartEvent		=	0x1 << 0x2,
            kTimeoutEvent	=	0x1 << 0x3,
       
			//socket events
            kReadEvent		=	0x1 << 0x4, //All of type "EventFlags"
            kWriteEvent		=	0x1 << 0x5,
           
			//update event
            kUpdateEvent	=	0x1 << 0x6,
        };

		enum
		{
			//flag
			kWouldBlock		=	0x1 << 0x16
		};
		
        
        //CONSTRUCTOR / DESTRUCTOR
        //You must assign priority at create time.
								Task(TaskThreadPoolBase * inTaskThreadPool = NULL);

        virtual                 ~Task() {}

        //return:
        // >0-> invoke me after this number of MilSecs with a kIdleEvent
        // 0 don't reinvoke me at all.
        //-1 delete me
        //Suggested practice is that any task should be deleted by returning true from the
        //Run function. That way, we know that the Task is not running at the time it is
        //deleted. This object provides no protection against calling a method, such as Signal,
        //at the same time the object is being deleted (because it can't really), so watch
        //those dangling references!
        virtual SInt64          Run() = 0;
        
        //Send an event to this task.
        void                    Signal(EventFlags eventFlags);
        void                    GlobalUnlock();     
        Bool16                  Valid(); // for debugging
		
		//TaskName
		char					fTaskName[48];
		void					SetTaskName(const char* name);
        
    protected:
    
        //Only the tasks themselves may find out what events they have received
        EventFlags              GetEvents();
        
        // ForceSameThread
        //
        // A task, inside its run function, may want to ensure that the same task thread
        // is used for subsequent calls to Run(). This may be the case if the task is holding
        // a mutex between calls to run. By calling this function, the task ensures that the
        // same task thread will be used for the next call to Run(). It only applies to the
        // next call to run.
        void                    ForceSameThread();
        SInt64                  CallLocked();

    private:

        enum
        {
            kAlive =            0x80000000, //EventFlags, again. 
											//txf: if this flag is set,this task should rescheduled to original thread
            kAliveOff =         0x7fffffff
        };

        //void            SetTaskThread(TaskThread *thread);
        
        EventFlags      fEvents;
        TaskThread*     fUseThisThread;
        Bool16          fWriteLock;

//#if TASK_DEBUG
        //The whole premise of a task is that the Run function cannot be re-entered.
        //This debugging variable ensures that that is always the case
        volatile UInt32 fInRunCount;
//#endif

        //This could later be optimized by using a timing wheel instead of a heap,
        //and that way we wouldn't need both a heap elem and a queue elem here (just queue elem)
        OSHeapElem      fTimerHeapElem;
        OSQueueElem     fTaskQueueElem;
        
        //Variable used for assigning tasks to threads in a round-robin fashion
        static unsigned int sThreadPicker;

		TaskThreadPoolBase	*fTaskThreadPool;
        
        friend class    TaskThread; 
		friend class	TaskThreadPool;
		friend class	STQTaskThreadPool;
		friend class	WorkTaskThreadPool;

};



class TGMCOMUTI_API TaskThreadPoolBase
{
public:
	virtual ~TaskThreadPoolBase(){};
	
	virtual void AssignTask(Task *inTask) = 0;
	
	virtual OSMutexRW& GetRWMutex() = 0;
};




//Because task threads share a global queue of tasks to execute,
//there can only be one pool of task threads. That is why this object
//is static.
class TGMCOMUTI_API TaskThreadPool:public TaskThreadPoolBase
{
public:

	TaskThreadPool();
	virtual ~TaskThreadPool();

    //Adds some threads to the pool
    UInt32	AddThreads(UInt32 numToAdd);
	void	RemoveThreads(UInt32 numToRel);

	void	SwitchPersonality( char *user = NULL, char *group = NULL);

	static TaskThreadPool* GetInstance();//获取唯一实例。当然，客户也可以创建自己的线程池；

	void	AssignTask(Task *inTask);
	
	virtual OSMutexRW& GetRWMutex(){ return m_MutexRW; }

protected:
	virtual	TaskThread* CreateThread();

private:
	enum{
		sMaxNumTaskThreads = 10
	};
    TaskThread*		m_TaskThreadArray[10];
    UInt32			m_TaskThreadNum;
    OSMutexRW		m_MutexRW;
    
    //friend class Task;
    friend class TaskThread;
};





class TGMCOMUTI_API STQTaskThreadPool:public TaskThreadPoolBase
{
public:
	STQTaskThreadPool();

	~STQTaskThreadPool();

	//! configure thread num
	void Initialize(ULONG min = 0/*常驻线程数目，最小为0*/,ULONG max = 0/*最大线程数目，0：无限*/);

	void Uninitialize();

	//! assign a new task
	virtual void AssignTask(Task * inTask);

	//! get globle rw mutex that need by task
	virtual OSMutexRW& GetRWMutex(){ return fMutexRW; }

	//! check and remove thread 
	void	CheckOverflowThread(STQTaskThread * inThread);

	//! Get the num of task
	UInt32	GetTaskNum(){return fTaskQueue.GetQueue()->GetLength();}

	//! get the num of activity threads 
	UInt32	GetThreadNum(){return fTaskThreadIdleQueue.GetLength()+ fTaskThreadActiveQueue.GetLength();}

	static STQTaskThreadPool* GetInstance();//获取唯一实例。当然，客户也可以创建自己的线程池；
	
		
private:

	//! cheek if need more thread
	BOOL isLackTaskThread(){ return (/*0 == fTaskThreadIdleQueue.GetLength() &&*/
								 fTaskQueue.GetQueue()->GetLength() >0 
								&& ( 0 == fMaxThreadNum || fTaskThreadIdleQueue.GetLength()+ fTaskThreadActiveQueue.GetLength() < fMaxThreadNum) && fIsEnable);
							}

	//! check that threads is too more
	BOOL isOverFlowTaskThread(){
		return (fTaskQueue.GetQueue()->GetLength() == 0 &&  (fTaskThreadIdleQueue.GetLength()+ fTaskThreadActiveQueue.GetLength() > fMinThreadNum || !fIsEnable));
	}

	//! stop and destroy all task thread
	void DestroyAllThread();

	void ClearDeadThreads();

	void SwicthThreadToActive(STQTaskThread * inTaskThread);
	void SwicthThreadToIdle(STQTaskThread * inTaskThread);
	void SwicthThreadToDead(STQTaskThread * inTaskThread);

private:
	OSMutexRW			fMutexRW;

	ULONG				fMinThreadNum;
	ULONG				fMaxThreadNum;
	ULONG				fTotalThreadNum;
	
	OSMutex				fMutex;
	OSQueue				fTaskThreadActiveQueue; //所有线程队列
	OSQueue				fTaskThreadIdleQueue;	//空闲的线程队列

	OSQueue_Blocking	fTaskThreadDeadQueue;	//等待停止的线程队列
	OSQueue_Blocking    fTaskQueue;

	BOOL				fIsEnable;

	STQScheduleTask		*fScheduleTask;

	friend class STQTaskThread;
	friend class STQScheduleTask;
};

#include "OSThreadPool.h"

class  WorkTaskThreadPool:public TaskThreadPoolBase
{
public:
	TGMCOMUTI_API virtual void AssignTask(Task *inTask);
	TGMCOMUTI_API virtual OSMutexRW& GetRWMutex(){	return mMutexRW; };

	//!获取唯一实例
	TGMCOMUTI_API static WorkTaskThreadPool* GetInstance();
private:
	WorkTaskThreadPool(){};//!防止创建实例
	static DWORD worktaskthread_start_entry_t(void* lpThreadParameter);
private:
	OSMutexRW mMutexRW;
}; 



#endif
