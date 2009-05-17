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
    File:       EventContext.h

    Contains:   An event context provides the intelligence to take an event
                generated from a UNIX file descriptor (usually EV_RE or EV_WR)
                and signal a Task. 
                    

    
    
*/
/*
* History:
*	2009/04/01	Jokul for Tranzda
*		Add TGMCOMUTI_API declare,Add EventThreadPool,Modify EventThread
*/


#ifndef __EVENT_CONTEXT_H__
#define __EVENT_CONTEXT_H__

#include "OSThread.h"
#include "Task.h"
#include "OSRef.h"
#include "OSEv.h"

//enable to trace event context execution and the task associated with the context
#define EVENTCONTEXT_DEBUG 0

class EventThread;

class TGMCOMUTI_API EventContext
{
public:

    //
    // Constructor. Pass in the EventThread you would like to receive
    // events for this context, and the fd that this context applies to
    EventContext(int inFileDesc, EventThread* inThread);
    virtual ~EventContext() { if (fAutoCleanup) this->Cleanup(); }

	//如果旧的描述符无效(kInvalidFileDesc)，设置新的描述符
	int		SetFileDesc(int inFileDesc);

	//如果旧的EventThread无效(NULL)，设置新的EventThread
	int		SetEventThread(EventThread* inThread);

	
    
    //
    // InitNonBlocking
    //
    // Sets inFileDesc to be non-blocking. Once this is called, the
    // EventContext object "owns" the file descriptor, and will close it
    // when Cleanup is called. This is necessary because of some weird
    // select() behavior. DON'T CALL CLOSE ON THE FD ONCE THIS IS CALLED!!!!
    void            InitNonBlocking(int inFileDesc);

    //
    // Cleanup. Will be called by the destructor, but can be called earlier
    void            Cleanup();

	//
    // Arms this EventContext. Pass in the events you would like to receive
    void            RequestEvent(int theMask = EV_RE);

	// CancelRequest 仅仅将自身从select列表中移除，并不关闭套接字；这是其和CleanUp的区别
	void			CancelRequest();


    
    //
    // Provide the task you would like to be notified
    void            SetTask(Task* inTask);

    // when the HTTP Proxy tunnels takes over a TCPSocket, we need to maintain this context too
    void            SnarfEventContext( EventContext &fromContext );
    
    // Don't cleanup this socket automatically
    void            DontAutoCleanup() { fAutoCleanup = false; }
    
    // Direct access to the FD is not recommended, but is needed for modules
    // that want to use the Socket classes and need to request events on the fd.
    int             GetSocketFD()       { return fFileDesc; }
	int             GetFileDesc()       { return fFileDesc; }
    
    enum
    {
        kInvalidFileDesc = -1   //int
    };

protected:

    //
    // ProcessEvent
    //
    // When an event occurs on this file descriptor, this function
    // will get called. Default behavior is to Signal the associated
    // task, but that behavior may be altered / overridden.
    //
    // Currently, we always generate a Task::kReadEvent
	virtual void ProcessEvent(int /*eventBits*/);

	//////////////////////////////////////////////////////////////////////////
	//关闭描述符
	//对于windows,由于EventContext以前默认用于socket,所以调用的是scoketclose
	//对于其他句柄，不能这样处理
	virtual OS_Error CloseFileDesc(int inFileDesc);

    int             fFileDesc;

private:

    struct eventreq fEventReq;
    
    OSRef           fRef;
    //PointerSizedInt fUniqueID;
    //StrPtrLen       fUniqueIDStr;
    EventThread*    fEventThread;
    Bool16          fWatchEventCalled;
    int             fEventBits;
    Bool16          fAutoCleanup;

    Task*           fTask;
#if DEBUG
    Bool16          fModwatched;
#endif
    
    static unsigned int sUniqueID;
    
    friend class EventThread;
};

class  EventThread : public OSThread
{
public:

	TGMCOMUTI_API EventThread();
	TGMCOMUTI_API virtual ~EventThread();

	TGMCOMUTI_API void StartSelect();
	TGMCOMUTI_API void StopSelect();

	TGMCOMUTI_API static OSMutex* GetMutex();

private:

    TGMCOMUTI_API virtual void Entry();
    
private:

	OSRefTable      fRefTable;
    //event_selector* fEvtSelector;
	
	LONG			fRunCount;
    
	friend class EventContext;
};

//!本类目前维护一个单件EventThread，用于处理串口读事件，Modem信号等等
/*!
对于Socket,依然使用单独的EventThread
*/
class TGMCOMUTI_API EventThreadFactory
{
public:
	//初始化并自动启动唯一实例。可以多次调用，但必须和Uninitialize对称调用
	//不需要调用StartSelect
	static void Initialize();
	
	//停止并反初始化唯一实例
	//不需要调用StopSelect
	static HRESULT Uninitialize();

	//返回唯一实例
	static EventThread* GetInstance();
private:
	static ULONG m_RefCount;
	static EventThread* m_EventThread;
};

#endif //__EVENT_CONTEXT_H__
