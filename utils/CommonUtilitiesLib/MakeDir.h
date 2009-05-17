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
*	History:
*		2009/04/01	Jokul for Tranzda
*			Add TGMCOMUTI_API declare
*/
 
#ifndef __makedir__hhh
#define __makedir__hhh
#include "OSBaseTypes.h"
#if (! __MACOS__)
    #include <sys/file.h>
    #include <sys/stat.h>
    #include <sys/types.h>
	//#ifndef __solaris__ || __hpux__ 
	#if !defined(__solaris__) || !defined(__hpux__)
        #include <sys/sysctl.h>
    #endif
    #include <sys/time.h>
#else
    #include "BogusDefs.h"
#endif

#ifndef S_IRWXU
    #define S_IRWXU 0
#endif


    #ifdef __cplusplus
    extern "C" {
    #endif


    int     TGMCOMUTI_API MakeDir( const char* path, int mode );
    int     TGMCOMUTI_API RecursiveMakeDir( const char*inPath, int mode);

    #ifdef __cplusplus
    }   
    #endif


#endif
