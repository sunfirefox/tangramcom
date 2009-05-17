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
    File:       OSThread.h

    Contains:   A thread abstraction
	
	History:
				2009/04/01 Jokul for Tranzda
					add  TGMCOMUTI_API declare ,add class OSThreadRunInfo,add class OSThreadDataHolder
    

*/

// OSThread.h
#ifndef __OSTHREAD__
#define __OSTHREAD__
#include "OSConfigure.h"
#ifndef __Win32__
#if __PTHREADS__
#if __solaris__ || __sgi__ || __hpux__
    #include <errno.h>
#else
    #include <sys/errno.h>
#endif
    #include <pthread.h>
#else
    #include <mach/mach.h>
    #include <mach/cthreads.h>
#endif
#endif

#include "OSBaseTypes.h"
#include "DateTranslator.h"

#ifdef Sleep	//解决TG3定义Sleep 问题
#undef Sleep
#endif

class TGMCOMUTI_API OSThread
{
public:

#ifdef __Win32__
	typedef DWORD           OSTHREAD_ID_t;
#elif __PTHREADS__
	typedef pthread_t       OSTHREAD_ID_t;
#else
	typedef UInt32          OSTHREAD_ID_t;
#endif

public:
                //
                // Call before calling any other OSThread function
                static void     Initialize();//! 在Start线程前，必须调用此接口，否则可能在ARM上退出时崩溃
                
                                OSThread();
    virtual                     ~OSThread();
    
    //
    // Derived classes must implement their own entry function
    virtual     void            Entry() = 0;
                void            Start();
                
                static void     ThreadYield();
                static void     Sleep(UInt32 inMsec);
                
                void            Join();
                void            SendStopRequest() { fStopRequested = true; }
                Bool16          IsStopRequested() { return fStopRequested; }
                void            StopAndWaitForThread();

                void*           GetThreadData()         { return fThreadData; }
                void            SetThreadData(void* inThreadData) { fThreadData = inThreadData; }

                OSTHREAD_ID_t   GetThreadID() { return fThreadID; }
                
                // As a convienence to higher levels, each thread has its own date buffer
                DateBuffer*     GetDateBuffer()         { return &fDateBuffer; }
                
                static void*    GetMainThreadData();//     { return sMainThreadData; }
                static void     SetMainThreadData(void* inData);// { sMainThreadData = inData; }
#if DEBUG
                UInt32          GetNumLocksHeld() { return 0; }
                void            IncrementLocksHeld() {}
                void            DecrementLocksHeld() {}
#endif

#if __linux__ ||  __MacOSX__
                static void     WrapSleep( Bool16 wrapSleep) {sWrapSleep = wrapSleep; }
#endif

#ifdef __Win32__
    static  int                 GetErrno();
    static  OSTHREAD_ID_t       GetCurrentThreadID();
#elif __PTHREADS__
    static  int                 GetErrno();
    static  OSTHREAD_ID_t       GetCurrentThreadID();
#else
    static  int                 GetErrno();
    static  OSTHREAD_ID_t       GetCurrentThreadID();
#endif

    static  OSThread*   GetCurrent();
    
private:

#ifdef __Win32__
    static DWORD    sThreadStorageIndex;
#elif __PTHREADS__
    static pthread_key_t    gMainKey;
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
    static pthread_attr_t sThreadAttr;
#endif
	
#endif
	static int		sInitlized;//是否已经做过静态初始化，即是否已经调用过OSThread::Initialize()

    Bool16 fStopRequested;
    Bool16 fJoined;

    OSTHREAD_ID_t   fThreadID;
#ifdef __Win32__
    HANDLE          fThreadHandle;
#endif

	void*           fThreadData;
    DateBuffer      fDateBuffer;
    
    static void*    sMainThreadData;
#ifdef __Win32__
    static unsigned int WINAPI _Entry(LPVOID inThread);
#else
    static void*    _Entry(void* inThread);
#endif

#if __linux__ || __MacOSX__
    static Bool16 sWrapSleep;
#endif
};

class OSThreadDataSetter
{
    public:
    
        OSThreadDataSetter(void* inInitialValue, void* inFinalValue) : fFinalValue(inFinalValue)
            { OSThread::GetCurrent()->SetThreadData(inInitialValue); }
            
        ~OSThreadDataSetter() { OSThread::GetCurrent()->SetThreadData(fFinalValue); }
        
    private:
    
        void*   fFinalValue;
};


  
/*! 本Class 建议在Entry实现函数中实例化
 *  
 */
class OSThreadRunInfo
{
public:
	OSThreadRunInfo(OSThread::OSTHREAD_ID_t inThreadId,const char * inName/*不要超过48字节*/);
	~OSThreadRunInfo();

	//激活记录线程运行信息记录功能
	static void Initialize();

	//因为不想在CommonUtility中使用CommonLib中的字符串操作函数，所以这里的接口是char*的
	static SInt32 GetAllThreadRunInfos(char *outStr,UInt32 inSize/*建议不低于1024字节*/);
private:
	OSThread::OSTHREAD_ID_t m_ThreadId;
	static BOOL m_IsEnable;
};

class OSThreadDataHolder
{
public:
	OSThreadDataHolder();
	
	~OSThreadDataHolder();

	void SetData(void* data);
	
	void* GetData();

private:
#ifdef __Win32__
	DWORD    mThreadStorageIndex;
#elif __PTHREADS__
	pthread_key_t    mMainKey;
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
	pthread_attr_t	 mThreadAttr;
#endif
#endif
};


#endif

