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
*			move some declare to OSBaseTypes.h
*/

#ifndef OSHeaders_H
#define OSHeaders_H

#include "OSBaseTypes.h"

/* Platform-specific components */
#if __linux__ || __linuxppc__ || __FreeBSD__ || __MacOSX__


#elif __Win32__

/* Includes */
	#if _MSC_VER >= 1301

	#include <windows.h>
	
	#else

	#include <windows.h>
	#include <winsock2.h>
	#include <mswsock.h>
	#include <ws2tcpip.h>

	#endif
	#include <tchar.h>
	#include <process.h>
    #include <io.h>
    #include <direct.h>
    #include <errno.h>

	// Winsock does not use iovecs
	struct iovec {
		u_long  iov_len; // this is not the POSIX definition, it is rather defined to be
		char FAR*   iov_base; // equivalent to a WSABUF for easy integration into Win32
	};

#endif


#endif /* OSHeaders_H */


