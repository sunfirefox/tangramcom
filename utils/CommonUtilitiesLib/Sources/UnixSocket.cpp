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
	@file		UnixSocket.cpp

    @author     LinShaocheng
    @date       2009/04/07

	Description:
		
	Version:	1.0.0.1
	History:
	
	V1.0.0.1	LinShaocheng	2009/04/07		Create project
*/

#include "OSConfigure.h"

#ifndef __Win32__
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#include <errno.h>

#include "UnixSocket.h"
#include "SocketUtils.h"
#include "OS.h"

#ifdef USE_NETLOG
#include <netlog.h>
#endif

void UnixSocket::SnarfSocket( UnixSocket & fromSocket )
{
    // take the connection away from the other socket and use it as our own.
    Assert(fFileDesc == OSEventContext::kInvalidFileDesc);
    this->Set( fromSocket.fFileDesc, &fromSocket.m_fRemoteAddr );
    
    // clear the old socket so he doesn't close and the like
    struct  sockaddr_un  remoteaddr;
    
    ::memset( &remoteaddr, 0, sizeof( remoteaddr ) );

    fromSocket.Set( OSEventContext::kInvalidFileDesc, &remoteaddr );

    // get the event context too
    this->SnarfEventContext( fromSocket );

}

void UnixSocket::Set(int inSocket, struct sockaddr_un* remoteaddr)
{
    m_fRemoteAddr = *remoteaddr;
    fFileDesc = inSocket;
    
    if ( inSocket != OSEventContext::kInvalidFileDesc ) 
    {
        //make sure to find out what IP address this connection is actually occuring on. That
        //way, we can report correct information to clients asking what the connection's IP is
#if __Win32__ || __osf__ || __sgi__ || __hpux__	
        int nLen = sizeof(m_fLocalAddr);
#else
        size_t nLen = sizeof(m_fLocalAddr);
#endif
        int err = ::getsockname(fFileDesc, (struct sockaddr*)&m_fLocalAddr, &nLen);
        AssertV(err == 0, OSThread::GetErrno());
        fState |= kBound;
        fState |= kConnected;
    }
    else
        fState = 0;
}

OS_Error UnixSocket::Open()
{
	Assert(fFileDesc == OSEventContext::kInvalidFileDesc);
	fFileDesc = ::socket(PF_UNIX, SOCK_STREAM, 0);
	if (fFileDesc == OSEventContext::kInvalidFileDesc)
	{
		return (OS_Error)OSThread::GetErrno();
	}     

	// Setup this socket's event context
	if (fState & kNonBlockingSocketType)
	{
		this->InitNonBlocking(fFileDesc);   
	}

	return OS_NoErr;
}

OS_Error UnixSocket::Bind(const char* inPath)
{
#if __Win32__ || __osf__ || __sgi__ || __hpux__	
	int nLen;
#else
	socklen_t nLen;
#endif

	unlink(inPath);
	m_fLocalAddr.sun_family = AF_UNIX;
	strcpy(m_fLocalAddr.sun_path, inPath);
	nLen = sizeof(m_fLocalAddr.sun_family) + strlen(m_fLocalAddr.sun_path);

	int err = ::bind(fFileDesc, (sockaddr *)&m_fLocalAddr, nLen);

	if (err == -1)
	{
		m_fLocalAddr.sun_path[0] = 0;
		return (OS_Error)OSThread::GetErrno();
	}
	else 
	{
		//int len;
		::getsockname(fFileDesc, (sockaddr *)&m_fLocalAddr, &nLen); // get the kernel to fill in unspecified values
	}

	fState |= kBound;
	return OS_NoErr;
}

OS_Error  UnixSocket::Connect(const char* inPath)
{
	size_t nLen;

	m_fRemoteAddr.sun_family = AF_UNIX;
	strcpy(m_fRemoteAddr.sun_path, inPath);
	nLen = sizeof(m_fRemoteAddr.sun_family) + strlen(m_fRemoteAddr.sun_path);

	// don't forget to error check the connect()! 
	int err = ::connect(fFileDesc, (sockaddr *)&m_fRemoteAddr, nLen);
	fState |= kConnected;

	if (err == -1)
	{
		return (OS_Error)OSThread::GetErrno();
	}

	return OS_NoErr;
}

const char* UnixSocket::GetLocalPath()
{
	return m_fLocalAddr.sun_path;
}
