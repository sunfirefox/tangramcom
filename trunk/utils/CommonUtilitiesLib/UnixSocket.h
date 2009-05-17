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
	@file		UnixSocket.h

	History:	2009/04/01 Jokul for Tranzda,	add  to project      
*/

#ifndef _UNIXSOCKET_H_
#define _UNIXSOCKET_H_

#include "OSEventContext.h"
#ifndef _WIN32
#include "sys/un.h"
#include <unistd.h>
#endif

#include <stdlib.h>
#include "SafeStdLib.h"
#include "Socket.h"
#include "Task.h"
#include "StrPtrLen.h"

#ifdef _WIN32
struct sockaddr_un
{
	int sun_family;
	char sun_path[128];
};
#endif // _WIN32

class TGMCOMUTI_API UnixSocket : public Socket
{
    public:
        UnixSocket(Task *notifytask, UInt32 inSocketType)
            : Socket(notifytask, inSocketType)
			, fRemoteStr(fRemoteBuffer, kIPAddrBufSize) 
		{}
        virtual ~UnixSocket() {}

		OS_Error Bind(const char* inPath);
		OS_Error Connect(const char* inPath);
        //Open
        OS_Error Open();

        // Basically a copy constructor for this object, also NULLs out the data
        // in tcpSocket.        
        void SnarfSocket( UnixSocket &fromSocket );

		const char*  GetLocalPath();

  protected:
        void Set(int inSocket, struct sockaddr_un* remoteaddr);
                            
        enum
        {
            kIPAddrBufSize = 20 //UInt32
        };

        struct sockaddr_un m_fLocalAddr;
		struct sockaddr_un m_fRemoteAddr;
        char fRemoteBuffer[kIPAddrBufSize];
        StrPtrLen fRemoteStr;
        
		friend class UnixListenerSocket; 
};

#endif	// End of _UNIXSOCKET_H_
