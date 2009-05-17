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
	@file		UnixListenerSocket.cpp

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
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#endif	// __Win32__

#include <errno.h>
#include "OSHeaders.h"
#include "UnixListenerSocket.h"
#include "Task.h"

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(lcmut);

OS_Error UnixListenerSocket::Listen(UInt32 queueLength)
{
    if (fFileDesc == OSEventContext::kInvalidFileDesc)
        return EBADF;
        
    int err = ::listen(fFileDesc, queueLength);
    if (err != 0)
        return (OS_Error)OSThread::GetErrno();
    return OS_NoErr;
}

OS_Error UnixListenerSocket::Initialize(const char* inPath)
{
    OS_Error err = this->UnixSocket::Open();
	if (0 == err) 
	{
		do
		{   
			// set SO_REUSEADDR socket option before calling bind.
#ifndef __Win32__
			// this causes problems on NT (multiple processes can bind simultaneously),
			// so don't do it on NT.
			this->ReuseAddr();
#endif
			err = this->Bind(inPath);
			if (err != 0) break; // don't assert this is just a port already in use.

			//
			// Unfortunately we need to advertise a big buffer because our TCP sockets
			// can be used for incoming broadcast data. This could force the server
			// to run out of memory faster if it gets bogged down, but it is unavoidable.
			//this->SetSocketRcvBufSize(96 * 1024);       
			this->SetSocketRcvBufSize(kListenSocketRecvBufSize);
			err = this->Listen(kListenQueueLength);
			AssertV(err == 0, OSThread::GetErrno()); 
			if (err != 0) break;

		} while (false);
	}
    return err;
}

void UnixListenerSocket::ProcessEvent(int /*eventBits*/)
{
    //we are executing on the same thread as every other
    //socket, so whatever you do here has to be fast.
    
#if __Win32__ || __osf__ || __sgi__ || __hpux__	
    int sin_size = sizeof(m_fLocalAddr);
#else
    socklen_t sin_size = sizeof(struct sockaddr_un);
#endif
    Task* theTask = NULL;
    UnixSocket* theSocket = NULL;
    
    //fSocket data member of UnixSocket.
	int nNewSockFd = accept(fFileDesc, (struct sockaddr*)&m_fLocalAddr, &sin_size);

	TRACE("UnixListenerSocket::ProcessEvent : newSockFd(%d)", nNewSockFd);

	if (nNewSockFd == -1)
	{
        //take a look at what this error is.
        int acceptError = OSThread::GetErrno();
        if (acceptError == EAGAIN)
        { 
            //If it's EAGAIN, there's nothing on the listen queue right now, so modwatch and return
            this->RequestEvent(EV_RE);
            return;
        }

		//if these error gets returned, we're out of file desciptors, 
        //the server is going to be failing on sockets, logs, qtgroups and qtuser auth file accesses and movie files. The server is not functional.
		if (acceptError == EMFILE || acceptError == ENFILE)
        {           
			exit (EXIT_FAILURE);	
        }
        else
        {   
            char errStr[256];
            errStr[sizeof(errStr) -1] = 0;
            qtss_snprintf(errStr, sizeof(errStr) -1, "accept error = %d '%s' on socket. Clean up and continue.", acceptError, strerror(acceptError)); 
            WarnV( (acceptError == 0), errStr);
            
            theTask = this->GetSessionTask(&theSocket);
            if (theTask == NULL)
            {   
                close(nNewSockFd);
            }
            else
            {  
                theTask->Signal(Task::kKillEvent); // just clean up the task
            }
            
            if (theSocket)
			{
				theSocket->fState &= ~kConnected; // turn off connected state
			}

            return;
        }
	}
	
    theTask = this->GetSessionTask(&theSocket);
    if (theTask == NULL)
    {    //this should be a disconnect. do an ioctl call?
        close(nNewSockFd);
        if (theSocket)
		{
			theSocket->fState &= ~kConnected; // turn off connected state
		}
    }
    else
    {   
        Assert(nNewSockFd != OSEventContext::kInvalidFileDesc);
        
        //set options on the socket
        //we are a server, always disable nagle algorithm
        int one = 1;
		int err;
        //err = ::setsockopt(nNewSockFd, IPPROTO_TCP, TCP_NODELAY, (char*)&one, sizeof(int));
        //AssertV(err == 0, OSThread::GetErrno());
        
        err = ::setsockopt(nNewSockFd, SOL_SOCKET, SO_KEEPALIVE, (char*)&one, sizeof(int));
        AssertV(err == 0, OSThread::GetErrno());
    
        //int sndBufSize = 96L * 1024L;
		int sndBufSize = 16L * 1024L;
        err = ::setsockopt(nNewSockFd, SOL_SOCKET, SO_SNDBUF, (char*)&sndBufSize, sizeof(int));
        AssertV(err == 0, OSThread::GetErrno());
    
        //setup the socket. When there is data on the socket,
        //theTask will get an kReadEvent event
        theSocket->Set(nNewSockFd, &m_fLocalAddr);
        theSocket->InitNonBlocking(nNewSockFd);
        theSocket->SetTask(theTask);
        theSocket->RequestEvent(EV_RE);
    }
    
    if (fSleepBetweenAccepts)
    { 	
        // We are at our maximum supported sockets
        // slow down so we have time to process the active ones (we will respond with errors or service).
        // wake up and execute again after sleeping. The timer must be reset each time through
        //qtss_printf("UnixListenerSocket slowing down\n");
        this->SetIdleTimer(kTimeBetweenAcceptsInMsec); //sleep 1 second
    }
    else
    { 	
        // sleep until there is a read event outstanding (another client wants to connect)
        //qtss_printf("UnixListenerSocket normal speed\n");
        this->RequestEvent(EV_RE);
    }

    fOutOfDescriptors = false; // always false for now  we don't properly handle this elsewhere in the code
}

SInt64 UnixListenerSocket::Run()
{
    EventFlags events = this->GetEvents();
    
    //
    // ProcessEvent cannot be going on when this object gets deleted, because
    // the resolve / release mechanism of OSEventContext will ensure this thread
    // will block before destructing stuff.
    if (events & Task::kKillEvent)
	{
		return -1;
	}   
        
    //This function will get called when we have run out of file descriptors.
    //All we need to do is check the listen queue to see if the situation has
    //cleared up.
    (void)this->GetEvents();
    this->ProcessEvent(Task::kReadEvent);
    return 0;
}
