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
	@file		UnixListenerSocket.h
	History:	2009/04/01 Jokul for Tranzda,	add  to project
*/

#ifndef _UNIXLISTENERSOCKET_H_
#define _UNIXLISTENERSOCKET_H_

#include "UnixSocket.h"
#include "IdleTask.h"

class TGMCOMUTI_API UnixListenerSocket : public UnixSocket, public IdleTask
{
    public:

        UnixListenerSocket() 
			: UnixSocket(NULL, UnixSocket::kNonBlockingSocketType)
			, IdleTask()
			, fOutOfDescriptors(false)
			, fSleepBetweenAccepts(false) 
		{
			this->SetTaskName("UnixListenerSocket");
		}

        virtual ~UnixListenerSocket() {}
        
        OS_Error Initialize(const char* inPath);

        //You can query the listener to see if it is failing to accept
        //connections because the OS is out of descriptors.
        Bool16      IsOutOfDescriptors()
		{ return fOutOfDescriptors; }

        void SlowDown() { fSleepBetweenAccepts = true; }
        void RunNormal() { fSleepBetweenAccepts = false; }
        //derived object must implement a way of getting tasks & sockets to this object 
        virtual Task*   GetSessionTask(UnixSocket** outSocket) = 0;
        
        virtual SInt64  Run();
            
    private:
    
        enum
        {
			kListenSocketRecvBufSize	= 8*1024,	//监听套接字接收缓冲区大小
            kTimeBetweenAcceptsInMsec	= 1000,		//UInt32
            kListenQueueLength			= 128       //UInt32
        };

        virtual void ProcessEvent(int eventBits);
        OS_Error    Listen(UInt32 queueLength);

        Bool16          fOutOfDescriptors;
        Bool16          fSleepBetweenAccepts;
};
#endif	// End of _UNIXLISTENERSOCKET_H_
