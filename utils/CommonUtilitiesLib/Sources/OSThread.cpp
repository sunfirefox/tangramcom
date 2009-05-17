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
File:       OSThread.cpp

Contains:   Thread abstraction implementation



*/

#include <stdio.h>
#include <stdlib.h>
#include "SafeStdLib.h"
#include <string.h>
#include <errno.h>


#ifndef __Win32__
#if __PTHREADS__
#include <pthread.h>
#if USE_THR_YIELD
#include <thread.h>
#endif
#else
#include <mach/mach.h>
#include <mach/cthreads.h>
#endif
#include <unistd.h>
#endif

#include "OSHeaders.h"
#include "OSThread.h"
#include "MyAssert.h"
#include "OSMutex.h"

#ifdef __sgi__ 
#include <time.h>
#endif

//
// OSThread.cp
//
void*   OSThread::sMainThreadData = NULL;

int		OSThread::sInitlized	= 0;
#ifdef __Win32__
DWORD   OSThread::sThreadStorageIndex = 0;
#elif __PTHREADS__
pthread_key_t OSThread::gMainKey = 0;
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
pthread_attr_t OSThread::sThreadAttr;
#endif
#endif

#if __linux__ ||  __MacOSX__
Bool16  OSThread::sWrapSleep = false;
#endif

void OSThread::Initialize()
{
	if(0 != sInitlized)
		return;

	OSGlobalCriticalSection _ogcs;
	if(0 != sInitlized)
		return;

#ifdef __Win32__
	sThreadStorageIndex = ::TlsAlloc();
	Assert(sThreadStorageIndex >= 0);
#elif __PTHREADS__
	pthread_key_create(&OSThread::gMainKey, NULL);
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING

	//
	// Added for Solaris...

	pthread_attr_init(&sThreadAttr);
	/* Indicate we want system scheduling contention scope. This
	thread is permanently "bound" to an LWP */
	pthread_attr_setscope(&sThreadAttr, PTHREAD_SCOPE_SYSTEM);
#endif

#endif

	sInitlized = 1;
}

OSThread::OSThread()
:   fStopRequested(false),
fJoined(false),
fThreadID(0),
#if __Win32__
fThreadHandle(NULL),
#endif
fThreadData(NULL)
{
}

OSThread::~OSThread()
{
    if(fThreadID != 0)
        this->StopAndWaitForThread();
}

void OSThread::Start()
{
	if(0 == sInitlized)
	{	//自动初始化线程私有变量
		OSThread::Initialize();
	}

    if(GetCurrentThreadID() == GetThreadID())
        return;
  
    //reset thread
    if(fStopRequested)
    {
        if(!fJoined) Join();
        fStopRequested = false;
        fJoined        = false;
    }

    if(fThreadID !=0)
        return;

    
#ifdef __Win32__
	//unsigned int theId = 0; // We don't care about the identifier
	fThreadHandle = (HANDLE)_beginthreadex( NULL,   // Inherit security
		0,      // Inherit stack size
		_Entry, // Entry function
		(void*)this,    // Entry arg
		0,      // Begin executing immediately
		(unsigned int*)&fThreadID );
	Assert(fThreadHandle != 0);
#elif __PTHREADS__
	pthread_attr_t* theAttrP;
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
	//theAttrP = &sThreadAttr;
	theAttrP = 0;
#else
	theAttrP = NULL;
#endif
	int err = pthread_create((pthread_t*)&fThreadID, theAttrP, _Entry, (void*)this);
	Assert(err == 0);
#else
	fThreadID = (UInt32)cthread_fork((cthread_fn_t)_Entry, (any_t)this);
#endif
}

void OSThread::StopAndWaitForThread()
{
	fStopRequested = true;
	if (!fJoined)
		Join();
}

void OSThread::Join()
{
    Assert(GetCurrentThreadID() != GetThreadID());
	// What we're trying to do is allow the thread we want to delete to complete
	// running. So we wait for it to stop.
	Assert(!fJoined);
	fJoined = true;
#ifdef __Win32__
	DWORD theErr = ::WaitForSingleObject((HANDLE)fThreadHandle, INFINITE);
	Assert(theErr == WAIT_OBJECT_0);
    fThreadHandle = NULL;
#elif __PTHREADS__
	void *retVal;
	pthread_join((pthread_t)fThreadID, &retVal);
#else
	cthread_join((cthread_t)fThreadID);
#endif
    
    fThreadID = 0;
}

void OSThread::ThreadYield()
{
	// on platforms who's threading is not pre-emptive yield 
	// to another thread
#if THREADING_IS_COOPERATIVE
#if __PTHREADS__
#if USE_THR_YIELD
	thr_yield();
#else
	sched_yield();
#endif
#endif
#endif
}

#include "OS.h"
void OSThread::Sleep(UInt32 inMsec)
{

#ifdef __Win32__
	::Sleep(inMsec);
#elif __linux__ ||  __MacOSX__

	if (inMsec == 0)
		return;

	SInt64 startTime = OS::Milliseconds();
	SInt64 timeLeft = inMsec;
	SInt64 timeSlept = 0;

	do {
		//qtss_printf("OSThread::Sleep = %qd request sleep=%qd\n",timeSlept, timeLeft);
		timeLeft = inMsec - timeSlept;
		if (timeLeft < 1)
			break;

		::usleep(timeLeft * 1000);

		timeSlept = (OS::Milliseconds() - startTime);
		if (timeSlept < 0) // system time set backwards
			break;

	} while (timeSlept < inMsec);

	//qtss_printf("total sleep = %qd request sleep=%lu\n", timeSlept,inMsec);

#elif defined(__osf__) || defined(__hpux__)
	if (inMsec < 1000)
		::usleep(inMsec * 1000); // useconds must be less than 1,000,000
	else
		::sleep((inMsec + 500) / 1000); // round to the nearest whole second
#elif defined(__sgi__) 
	struct timespec ts;

	ts.tv_sec = 0;
	ts.tv_nsec = inMsec * 1000000;

	nanosleep(&ts, 0);
#else
	::usleep(inMsec * 1000);
#endif

}

#ifdef __Win32__
unsigned int WINAPI OSThread::_Entry(LPVOID inThread)
#else
void* OSThread::_Entry(void *inThread)  //static
#endif
{
	OSThread* theThread = (OSThread*)inThread;
#ifdef __Win32__
	BOOL theErr = ::TlsSetValue(sThreadStorageIndex, theThread);
	Assert(theErr == TRUE);
#elif __PTHREADS__
	theThread->fThreadID = (pthread_t)pthread_self();
	pthread_setspecific(OSThread::gMainKey, theThread);
#else
	theThread->fThreadID = (UInt32)cthread_self();
	cthread_set_data(cthread_self(), (any_t)theThread);
#endif

	//
	// Run the thread
	theThread->Entry();
	return NULL;
}

OSThread*   OSThread::GetCurrent()
{
#ifdef __Win32__
	return (OSThread *)::TlsGetValue(sThreadStorageIndex);
#elif __PTHREADS__
	return (OSThread *)pthread_getspecific(OSThread::gMainKey);
#else
	return (OSThread*)cthread_data(cthread_self());
#endif
}

#ifdef __Win32__
int OSThread::GetErrno()
{
	int winErr = ::GetLastError();


	// Convert to a POSIX errorcode. The *major* assumption is that
	// the meaning of these codes is 1-1 and each Winsock, etc, etc
	// function is equivalent in errors to the POSIX standard. This is 
	// a big assumption, but the server only checks for a small subset of
	// the real errors, on only a small number of functions, so this is probably ok.
	switch (winErr)
	{

	case ERROR_FILE_NOT_FOUND: return ENOENT;

	case ERROR_PATH_NOT_FOUND: return ENOENT;       




	case WSAEINTR:      return EINTR;
	case WSAENETRESET:  return EPIPE;
	case WSAENOTCONN:   return ENOTCONN;
	case WSAEWOULDBLOCK:return EAGAIN;
	case WSAECONNRESET: return EPIPE;
	case WSAEADDRINUSE: return EADDRINUSE;
	case WSAEMFILE:     return EMFILE;
	case WSAEINPROGRESS:return EINPROGRESS;
	case WSAEADDRNOTAVAIL: return EADDRNOTAVAIL;
	case WSAECONNABORTED: return EPIPE;
	case 0:             return 0;

	default:            return ENOTCONN;
	}
}

DWORD        OSThread::GetCurrentThreadID() { return ::GetCurrentThreadId()  ; }
#elif __PTHREADS__
int			OSThread::GetErrno() { return errno; }
pthread_t	OSThread::GetCurrentThreadID() { return ::pthread_self(); }
#else
int         OSThread::GetErrno() { return cthread_errno();    }   
cthread_t   OSThread::GetCurrentThreadID() { return cthread_self(); }
#endif


#include <vector>
#include <utility>
using std::vector;
using std::pair;
#include <stdio.h>
#include "OSMutex.h"

typedef struct  {
	char str[64];
}OSThread_Info_Str_t;

typedef vector< pair<OSThread::OSTHREAD_ID_t,OSThread_Info_Str_t> > OSThread_RunInfoUnitArray_t;

static OSThread_RunInfoUnitArray_t& ___get_current_all_running_thread_infos___()
{
	static OSThread_RunInfoUnitArray_t ins;
	return ins;
}



BOOL OSThreadRunInfo::m_IsEnable = FALSE;

void OSThreadRunInfo::Initialize()
{
	OSThreadRunInfo::m_IsEnable = TRUE;
}

OSThreadRunInfo::OSThreadRunInfo(OSThread::OSTHREAD_ID_t inThreadId,const char * inName)
:m_ThreadId(inThreadId)
{
	if(FALSE == m_IsEnable)
	{
		return;
	}

	OSThread_RunInfoUnitArray_t& theInfos = ___get_current_all_running_thread_infos___();

	if(theInfos.size()%10 == 0 )
	{
		theInfos.reserve(theInfos.size()+10);
	}

	OSThread_Info_Str_t theStr;
	sprintf(theStr.str,"0X%X,%s\n",(unsigned int)inThreadId,inName);

	OSGlobalCriticalSection _ogcs;
	theInfos.push_back(pair<OSThread::OSTHREAD_ID_t,OSThread_Info_Str_t>(inThreadId,theStr));
}


OSThreadRunInfo::~OSThreadRunInfo()
{
	if(FALSE == m_IsEnable)
	{
		return;
	}

	OSThread_RunInfoUnitArray_t& theInfos = ___get_current_all_running_thread_infos___();

	OSGlobalCriticalSection _ogcs;

	OSThread_RunInfoUnitArray_t::iterator iter = theInfos.begin();
	while(iter != theInfos.end())
	{
		if((*iter).first == m_ThreadId)
		{
			theInfos.erase(iter);
			return;
		}
		iter++;
	}

}

SInt32 OSThreadRunInfo::GetAllThreadRunInfos(char *outStr,UInt32 /*inSize*/)
{
	OSThread_RunInfoUnitArray_t& theInfos = ___get_current_all_running_thread_infos___();

	OSGlobalCriticalSection _ogcs;

	for(unsigned int i = 0 ; i < theInfos.size() ; i++)
	{
		strcat(outStr+strlen(outStr),theInfos[i].second.str);
	}

	return OS_NoErr;
}

void*    OSThread::GetMainThreadData()     { return sMainThreadData; }
void     OSThread::SetMainThreadData(void* inData) { sMainThreadData = inData; }



OSThreadDataHolder::OSThreadDataHolder()
{
#ifdef __Win32__
	mThreadStorageIndex = ::TlsAlloc();
	Assert(mThreadStorageIndex >= 0);
#elif __PTHREADS__
	pthread_key_create(&mMainKey, NULL);
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
	// Added for Solaris...

	pthread_attr_init(&mThreadAttr);
	/* Indicate we want system scheduling contention scope. This
	thread is permanently "bound" to an LWP */
	pthread_attr_setscope(&mThreadAttr, PTHREAD_SCOPE_SYSTEM);
#endif

#endif
}

OSThreadDataHolder::~OSThreadDataHolder()
{

}

void OSThreadDataHolder::SetData(void* data)
{
#ifdef __Win32__
	BOOL theErr = ::TlsSetValue(mThreadStorageIndex, data);
	Assert(theErr == TRUE);
#elif __PTHREADS__
	pthread_setspecific(mMainKey, data);
#else
	cthread_set_data(cthread_self(), (any_t)data);
#endif
}

void* OSThreadDataHolder::GetData()
{
#ifdef __Win32__
	return (OSThread *)::TlsGetValue(mThreadStorageIndex);
#elif __PTHREADS__
	return (OSThread *)pthread_getspecific(mMainKey);
#else
	return (OSThread*)cthread_data(cthread_self());
#endif
}


