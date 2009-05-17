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
    File:       IdleTask.cpp

    Contains:   IdleTasks are identical to normal tasks (see task.h) with one exception:
    
                You can schedule them for timeouts. If you call SetIdleTimer
                on one, after the time has elapsed the task object will receive an
                OS_IDLE event. 
                
    

*/

#include "IdleTask.h"
#include "OSMemory.h"
#include "OS.h"

//IDLETASKTHREAD IMPLEMENTATION:
IdleTaskThread*     IdleTask::sIdleThread = NULL;

void IdleTaskThread::SetIdleTimer(IdleTask *activeObj, SInt64 msec)
{
    //note: OSHeap doesn't support a random remove, so this function
    //won't change the timeout value if there is already one set
    if (activeObj->fIdleElem.IsMemberOfAnyHeap())
        return;
    activeObj->fIdleElem.SetValue(OS::Milliseconds() + msec);
    
    {
        OSMutexLocker locker(&fHeapMutex);
        fIdleHeap.Insert(&activeObj->fIdleElem);
    }
    fHeapCond.Signal();
}

void IdleTaskThread::CancelTimeout(IdleTask* idleObj)
{
    Assert(idleObj != NULL);
    OSMutexLocker locker(&fHeapMutex);
    fIdleHeap.Remove(&idleObj->fIdleElem);  
}

void
IdleTaskThread::Entry()
{
	OSThreadRunInfo _runinfo((OSTHREAD_ID_t)GetCurrentThreadID(),"IdleTaskThread");

    OSMutexLocker locker(&fHeapMutex);
    
    ////while (true)
    while(!IsStopRequested())
    {
        //if there are no events to process, block.
        if (fIdleHeap.CurrentHeapSize() == 0)
            fHeapCond.Wait(&fHeapMutex);
        SInt64 msec = OS::Milliseconds();
        
        //pop elements out of the heap as long as their timeout time has arrived
        while ((fIdleHeap.CurrentHeapSize() > 0) && (fIdleHeap.PeekMin()->GetValue() <= msec))
        {
            IdleTask* elem = (IdleTask*)fIdleHeap.ExtractMin()->GetEnclosingObject();
            Assert(elem != NULL);
            elem->Signal(Task::kIdleEvent);
        }
                        
        //we are done sending idle events. If there is a lowest tick count, then
        //we need to sleep until that time.
        if (fIdleHeap.CurrentHeapSize() > 0)
        {
            SInt64 timeoutTime = fIdleHeap.PeekMin()->GetValue();
            //because sleep takes a 32 bit number
            timeoutTime -= msec;
            Assert(timeoutTime > 0);
            UInt32 smallTime = (UInt32)timeoutTime;
            fHeapCond.Wait(&fHeapMutex, smallTime);
        }
    }   
}

void IdleTask::Initialize()
{
    if (sIdleThread == NULL)
    {
        sIdleThread = NEW IdleTaskThread();
        sIdleThread->Start();
    }
}

void IdleTask::Uninitialize()
{
    if (sIdleThread)
    {
        sIdleThread->SendStopRequest();
        sIdleThread->fHeapCond.Signal();
        sIdleThread->StopAndWaitForThread();        
        delete sIdleThread;
        sIdleThread = NULL;
    }  
}

IdleTask::~IdleTask()
{
	if (!fIdleElem.IsMemberOfAnyHeap())
    {
        return;
    }
    //clean up stuff used by idle thread routines
    Assert(sIdleThread != NULL);
    
    OSMutexLocker locker(&sIdleThread->fHeapMutex);

    //Check to see if there is a pending timeout. If so, get this object
    //out of the heap
    if (fIdleElem.IsMemberOfAnyHeap())
        sIdleThread->CancelTimeout(this);
}


void IdleTask::SetIdleTimer(SInt64 msec) { sIdleThread->SetIdleTimer(this, msec); }

//CancelTimeout
//If there is a pending timeout for this object, this function cancels it.
//If there is no pending timeout, this function does nothing.
//Currently not supported because OSHeap doesn't support random remove
void IdleTask::CancelTimeout() { sIdleThread->CancelTimeout(this); }


