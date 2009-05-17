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
File:       OSConfigure

Contains:   Configure this library


*/
#ifndef OS_CONFIGURE_HHH_123
#define OS_CONFIGURE_HHH_123

#ifdef		WIN32
#undef		__Win32__
#define		__Win32__ 1
#endif

#if	__Win32__

	//#include "WinNTSupport/Win32header.h"
	//#define __Win32__ 1

#define _MT 1
	/* Defines needed to compile windows headers */

	#ifndef _X86_
	#define _X86_ 1
	#endif

	/* Pro4 compilers should automatically define this to appropriate value */
	#ifndef _M_IX86
	#define _M_IX86 500
	#endif

	#ifndef WIN32
	/* same as definition in OLE2.h where 100 implies WinNT version 1.0 */
	#define WIN32 100
	#endif

	#ifndef _WIN32
	#define _WIN32 1   //Defined for applications for Win32 and Win64. Always defined.
	#endif

	#ifndef _MSC_VER
	#define _MSC_VER 1301	/* 当前使用的编译器为MSVC7.1 = 1301 MSVC7 1300 MSVC6 1200*/
	#endif

	#ifndef _CRTAPI1
	#define _CRTAPI1 __cdecl
	#endif

	#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0502	 //Windows Server 2003 family
	#endif

	#ifndef _WIN32_IE
	#define _WIN32_IE 0x0600	//_WIN32_IE>=0x0600
	#endif

	#ifndef _UNICODE
	#define _UNICODE 1
	#endif

	#ifndef UNICODE
	#define UNICODE 1
	#endif

	#include "PlatformHeader.h"
	#include <fcntl.h>

#elif __linux__
	#include "PlatformHeader.h"

#else	//__Win32__
	#error you must define os enviroment:WIN32 __Win32__=1 or __linux__=1

#endif //__Win32__

//////////////////////////////////////////////////////////////////////////
//DLL 导出符号
//TGMCOMUTI_API适合于WINDOWS，LINUX都生成动态库的项目
#ifdef TGMCOMUTI_API
#undef TGMCOMUTI_API	
#endif

#if __Win32__ || defined(WIN32)

#if defined(TGMCOMUTI_MAKEDLL)
#define TGMCOMUTI_API  __declspec(dllexport)
#elif defined(TGMCOMUTI_IGNORE_EXPORT)
#define TGMCOMUTI_API
#else 		/* use a DLL library */
#define TGMCOMUTI_API  __declspec(dllimport)
#endif


#elif __linux__

#if defined(TGMCOMUTI_MAKEDLL)
#define TGMCOMUTI_API   __attribute__((visibility("default")))
#elif defined(TGMCOMUTI_IGNORE_EXPORT)
#define TGMCOMUTI_API
#else
#define TGMCOMUTI_API
#endif



#else
#error unsupportted_os
#endif


#endif


