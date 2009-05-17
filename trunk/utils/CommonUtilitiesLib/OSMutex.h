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
    File:       OSMutex.h

    Contains:   Platform - independent mutex header. The implementation of this object
                is platform - specific. Each platform must define an independent
                OSMutex.h & OSMutex.cpp file.
                
                This file is for Mac OS X Server only


	History:
				2009/04/01 Jokul for Tranzda
					add  TGMCOMUTI_API declare   

*/

#ifndef _OSMUTEX_H_
#define _OSMUTEX_H_

#include <stdlib.h>
#include "SafeStdLib.h"
#ifndef __Win32__
#include <sys/errno.h>
    #if __PTHREADS_MUTEXES__
        #if __MacOSX__
            #ifndef _POSIX_PTHREAD_H
                #include <pthread.h>
            #endif
        #else
            #include <pthread.h>
        #endif
        
    #else
        #include "mymutex.h"
    #endif
#endif

#include "OSBaseTypes.h"
#include "OSThread.h"
#include "MyAssert.h"

class OSCond;

class TGMCOMUTI_API OSMutex
{
    public:

        OSMutex();
        ~OSMutex();

        void Lock();
        void Unlock();
        
        // Returns true on successful grab of the lock, false on failure
        Bool16 TryLock();

    private:

#ifdef __Win32__
        CRITICAL_SECTION fMutex;
        
        DWORD       fHolder;
        UInt32      fHolderCount;
        
#elif !__PTHREADS_MUTEXES__
        mymutex_t fMutex;
#else
        pthread_mutex_t fMutex;
        // These two platforms don't implement pthreads recursive mutexes, so
        // we have to do it manually
        pthread_t   fHolder;
        UInt32      fHolderCount;
#endif

#if __PTHREADS_MUTEXES__ || __Win32__       
        void        RecursiveLock();
        void        RecursiveUnlock();
        Bool16      RecursiveTryLock();
#endif
        friend class OSCond;
};

class   TGMCOMUTI_API OSMutexLocker
{
    public:

        OSMutexLocker(OSMutex *inMutexP) : fMutex(inMutexP) { if (fMutex != NULL) fMutex->Lock(); }
        ~OSMutexLocker() {  if (fMutex != NULL) fMutex->Unlock(); }
        
        void Lock()         { if (fMutex != NULL) fMutex->Lock(); }
        void Unlock()       { if (fMutex != NULL) fMutex->Unlock(); }
        
    private:

        OSMutex*    fMutex;
};



//提供一个全局的关键段，保证其原子操作
//OSGlobalCriticalSection 不容许嵌套
//其作用仅仅是提供一个全局互斥锁，减少OSMutex的实例数目
//用法 OSGlobalCriticalSection _ofcs;
class TGMCOMUTI_API OSGlobalCriticalSection
{
public:
	OSGlobalCriticalSection();
	~OSGlobalCriticalSection();
};

#endif //_OSMUTEX_H_
