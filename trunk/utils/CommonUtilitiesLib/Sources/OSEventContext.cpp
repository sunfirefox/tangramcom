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
File:       OSCond.cpp

Contains:   Implementation of OSEventContext class

20090424 txf

*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "OSConfigure.h"
#include "OSEventContext.h"

#if __linux__
#include <unistd.h>
#endif

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(evloop);

#define EVENT_CONTEXT_DEBUG 0


OSEventContext::OSEventContext(int inFileDesc, OSEventThread* inThread)
:   fFileDesc(inFileDesc),
    fEventThread(inThread),
    //fWatchEventCalled(false),
    fAutoCleanup(true)
{
    OSEventThread::SetEvent(&fEventReq,fFileDesc,false,NULL,NULL);
}

//如果旧的描述符无效，设置新的描述符
int OSEventContext::SetFileDesc(int inFileDesc)
{
	if(inFileDesc == kInvalidFileDesc)
	{
		qtss_printf("OSEventContext::SetFileDesc FAULT inFileDesc == kInvalidFileDesc\n"); 
		Assert(inFileDesc != kInvalidFileDesc);
		return -1;
	}

	if(fFileDesc != kInvalidFileDesc)
	{
		qtss_printf("OSEventContext::SetFileDesc FAULT fFileDesc != kInvalidFileDesc\n"); 
		Assert(fFileDesc == kInvalidFileDesc);
		return -1;
	}
	
	fFileDesc = inFileDesc;
	return 0;
}
//如果旧的OSEventThread无效，设置新的OSEventThread
int OSEventContext::SetEventThread(OSEventThread* inThread)
{
	if(inThread == NULL)
	{
		qtss_printf("OSEventContext::SetEventThread FAULT inThread == NULL\n"); 
		Assert(inThread != NULL);
		return -1;
	}

	Assert(fEventThread == NULL);
	Assert(inThread != NULL);
	fEventThread = inThread;
	return 0;
}

void OSEventContext::InitNonBlocking(int inFileDesc)
{
    fFileDesc = inFileDesc;
    
#ifdef __Win32__
    u_long one = 1;
    int err = ::ioctlsocket(fFileDesc, FIONBIO, &one);
#else
    int flag = ::fcntl(fFileDesc, F_GETFL, 0);
    int err = ::fcntl(fFileDesc, F_SETFL, flag | O_NONBLOCK);
#endif
    AssertV(err == 0, OSThread::GetErrno());
}

OS_Error OSEventContext::CloseFileDesc(int inFileDesc)
{
	OS_Error err;
#ifdef __Win32__
	err = ::closesocket(inFileDesc);
#else
	err = ::close(inFileDesc);
#endif
    return err;
}

void OSEventContext::Cleanup()
{
    int err = 0;
    int theFileDesc = fFileDesc;
    fFileDesc = kInvalidFileDesc;
    if (theFileDesc != kInvalidFileDesc)
    {
        fEventThread->DelEvent(&fEventReq);
        OSThread::ThreadYield();
       
        CloseFileDesc(theFileDesc);
    }
    //fWatchEventCalled = false;							
    AssertV(err == 0, OSThread::GetErrno());//we don't really care if there was an error, but it's nice to know
}


void OSEventContext::CancelRequest()
{
    int err = 0;

    if (fFileDesc != kInvalidFileDesc)
    {
        fEventThread->DelEvent(&fEventReq);
    }
    //fWatchEventCalled = false;							
    AssertV(err == 0, OSThread::GetErrno());//we don't really care if there was an error, but it's nice to know
}



void OSEventContext::SnarfEventContext( OSEventContext &fromContext )
{ 
    FIXME("not implement\n");
}

void OSEventContext::EventProcessCallBack(int fd, short events, void *userdata){
    OSEventContext* This = (OSEventContext*)userdata;
	if(This && This->fFileDesc > 0){
        This->ProcessEvent(EV_RE);
	}
}

void OSEventContext::RequestEvent(int theMask)
{
    ASSERT_C(theMask == EV_RE);
    fEventThread->SetEvent(&fEventReq,fFileDesc,false,OSEventContext::EventProcessCallBack,this);
    fEventThread->AddEvent(&fEventReq);
}

void  OSEventContext::SetTask(Task* inTask)
{  
	fTask = inTask; 
	if (EVENTCONTEXT_DEBUG)
	{
		if (fTask== NULL)  
			qtss_printf("OSEventContext::SetTask context=%p task= NULL\n",  this); 
		else 
			qtss_printf("OSEventContext::SetTask context=%p task= %p name=%s\n", this,fTask, fTask->fTaskName); 
	}
}

void OSEventContext::ProcessEvent(int eventBits) 
{   
    if (EVENTCONTEXT_DEBUG)
	{
		if (fTask== NULL)  
			qtss_printf("OSEventContext::ProcessEvent context=%p task=NULL\n",this); 
		else 
			qtss_printf("OSEventContext::ProcessEvent context=%p task=%p TaskName=%s\n",this,fTask, fTask->fTaskName); 
	}

	if (fTask != NULL)
		fTask->Signal(Task::kReadEvent); 
}


