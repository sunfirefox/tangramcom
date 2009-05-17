/*
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * Copyright (c) 2009 Jokul for Tranzda .All Rights Reserved.
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

// Build flags. How do you want your server built?
#define DEBUG 0
#define ASSERT 1
#define MEMORY_DEBUGGING  0 //enable this to turn on really fancy debugging of memory leaks, etc...
#define QTFILE_MEMORY_DEBUGGING 0

#ifndef __CL_DEBUG__
#define __CL_DEBUG__
#endif

#if __MacOSX__
    #define PLATFORM_SERVER_BIN_NAME "QuickTimeStreamingServer"
    #define PLATFORM_SERVER_TEXT_NAME "QuickTime Streaming Server"
#else
    #define PLATFORM_SERVER_BIN_NAME "DarwinStreamingServer"
    #define PLATFORM_SERVER_TEXT_NAME "Darwin Streaming Server"
#endif



// Platform-specific switches
#if __MacOSX__

#define USE_ATOMICLIB 0
#define MACOSXEVENTQUEUE 1
#define __PTHREADS__    1
#define __PTHREADS_MUTEXES__    1

#include <machine/endian.h>
#include <machine/limits.h>
#if BYTE_ORDER == BIG_ENDIAN
    #define BIGENDIAN      1
#else
    #define BIGENDIAN      0
#endif

#define ALLOW_NON_WORD_ALIGN_ACCESS 1
#define USE_THREAD      0 //Flag used in QTProxy
#define THREADING_IS_COOPERATIVE        0 
#define USE_THR_YIELD   0
#define kPlatformNameString     "MacOSX"
#define EXPORT
#define MACOSX_PUBLICBETA 0
#define USE_DEFAULT_STD_LIB 1

#elif __Win32__
#define USE_ATOMICLIB 0
#define MACOSXEVENTQUEUE 0
#define __PTHREADS__    0
#define __PTHREADS_MUTEXES__    0
//#define BIGENDIAN     0   // Defined equivalently inside windows
#define ALLOW_NON_WORD_ALIGN_ACCESS 1
#define USE_THREAD      0 //Flag used in QTProxy
#define THREADING_IS_COOPERATIVE        0
#define USE_THR_YIELD   0
#define kPlatformNameString     "Win32"
#define EXPORT  __declspec(dllexport)
#ifndef USE_DEFAULT_STD_LIB
    #define USE_DEFAULT_STD_LIB 1
#endif

#elif __linux__ 

#define USE_ATOMICLIB 0
#define MACOSXEVENTQUEUE 0
#define __PTHREADS__    1
#define __PTHREADS_MUTEXES__    1
#define BIGENDIAN       0
#define ALLOW_NON_WORD_ALIGN_ACCESS 1
#define USE_THREAD      0 //Flag used in QTProxy
#define THREADING_IS_COOPERATIVE        0 
#define USE_THR_YIELD   0
#define kPlatformNameString     "Linux"
#define EXPORT
#define _REENTRANT 1

#ifndef USE_DEFAULT_STD_LIB
	#define USE_DEFAULT_STD_LIB 1
#endif

#elif __linuxppc__ 

#define USE_ATOMICLIB 0
#define MACOSXEVENTQUEUE 0
#define __PTHREADS__    1
#define __PTHREADS_MUTEXES__    1
#define BIGENDIAN       1
#define ALLOW_NON_WORD_ALIGN_ACCESS 1
#define USE_THREAD      0 //Flag used in QTProxy
#define THREADING_IS_COOPERATIVE        0 
#define USE_THR_YIELD   0
#define kPlatformNameString     "LinuxPPC"
#define EXPORT
#define _REENTRANT 1

#ifndef USE_DEFAULT_STD_LIB
	#define USE_DEFAULT_STD_LIB 1
#endif

#elif __FreeBSD__ 

#define USE_ATOMICLIB 0
#define MACOSXEVENTQUEUE 0
#define __PTHREADS__    1
#define __PTHREADS_MUTEXES__    1
#define BIGENDIAN       0
#define ALLOW_NON_WORD_ALIGN_ACCESS 1
#define USE_THREAD      1 //Flag used in QTProxy
#define THREADING_IS_COOPERATIVE        1 
#define USE_THR_YIELD   0
#define kPlatformNameString     "FreeBSD"
#define EXPORT
#define _REENTRANT 1

#elif __solaris__ 

#define USE_ATOMICLIB 0
#define MACOSXEVENTQUEUE 0
#define __PTHREADS__    1
#define __PTHREADS_MUTEXES__    1
#define BIGENDIAN       1
#define ALLOW_NON_WORD_ALIGN_ACCESS 0
#define USE_THREAD      1 //Flag used in QTProxy
#define THREADING_IS_COOPERATIVE        0
#define USE_THR_YIELD   0
#define kPlatformNameString     "Solaris"
#define EXPORT
#define _REENTRANT 1

#elif __sgi__ 

#define USE_ATOMICLIB 0
#define MACOSXEVENTQUEUE 0
#define __PTHREADS__    1
#define __PTHREADS_MUTEXES__    1
#define BIGENDIAN               1
#define ALLOW_NON_WORD_ALIGN_ACCESS 0
#define USE_THREAD              1 //Flag used in QTProxy
#define THREADING_IS_COOPERATIVE                0
#define USE_THR_YIELD   0
#define kPlatformNameString     "IRIX"
#define EXPORT
#define _REENTRANT 1

#elif __hpux__ 

#define USE_ATOMICLIB 0
#define MACOSXEVENTQUEUE 0
#define __PTHREADS__    1
#define __PTHREADS_MUTEXES__    1
#define BIGENDIAN               1
#define ALLOW_NON_WORD_ALIGN_ACCESS 0
#define USE_THREAD              1 //Flag used in QTProxy
#define THREADING_IS_COOPERATIVE                0
#define USE_THR_YIELD   0
#define kPlatformNameString     "HP-UX"
#define EXPORT
#define _REENTRANT 1

#elif defined(__osf__)

#define __osf__ 1
#define USE_ATOMICLIB 0
#define MACOSXEVENTQUEUE 0
#define __PTHREADS__    1
#define __PTHREADS_MUTEXES__    1
#define BIGENDIAN       0
#define ALLOW_NON_WORD_ALIGN_ACCESS 0
#define USE_THREAD      1 //Flag used in QTProxy
#define THREADING_IS_COOPERATIVE        0
#define USE_THR_YIELD   0
#define kPlatformNameString     "Tru64UNIX"
#define EXPORT

#endif



