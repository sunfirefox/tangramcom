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
File:       OSEventContext.h

Contains:   An event thread

*/
/*
*	History:
*		2009/04/01	Jokul for Tranzda
*			add to project
*/

#ifndef OSEVENTREACTOR___HH_THREAD
#define OSEVENTREACTOR___HH_THREAD
#include "OSMutex.h"
#include "OSThread.h"

#include "utdlist.h"

typedef void (*event_req_cb_t)(int fd, short events, void *userdata);
typedef void (*event_timer_cb_t)(unsigned long timespan, void *userdata);

struct event_req_t;
typedef struct event_req_t event_req_t;
struct event_req_t {
    UT_LIST_NODE_T(event_req_t) watchNode;
    UT_LIST_NODE_T(event_req_t) activeNode;

    bool persist;
    event_req_cb_t func;
    void *param;
    volatile int status;

    int fd;
};

struct event_timer_t;
typedef struct event_timer_t event_timer_t;
struct event_timer_t {
    UT_LIST_NODE_T(event_timer_t) watchNode;
    UT_LIST_NODE_T(event_timer_t) activeNode;

    bool persist;
    event_timer_cb_t func;
    void *param;
    volatile int status;

    unsigned long timeout;
    unsigned long timespan;

};

class TGMCOMUTI_API OSEventThread:public OSThread
{
public:
    // Add event to watch list
    void AddEvent(event_req_t * ev);//fd ±ÿ–Î”––ß

    // Remove event from watch list
    void DelEvent(event_req_t * ev);

    // Initialize an event 
    static void SetEvent(event_req_t * ev, int fd, bool persist, event_req_cb_t func, void * param);


    // Add timer event
    void AddTimer(event_timer_t * ev);

    // Remove timer from watch list
    void DelTimer(event_timer_t * ev);

    // Initialize an timer 
    static void SetTimer(event_timer_t * ev, unsigned long timespan, bool persist, event_timer_cb_t func, void * param);




    // Initialize internal data structs
    OSEventThread();

    // Uninitialize internal data structs
    ~OSEventThread();

    //set auto-stop option,in milliseconds.<0 not stop; ==0 stop imediate ; >0 stop after idle x milliseconds
    long SetIdleTimeOut(long idleTimeout);

    //get the default instance
    static OSEventThread* GetDefaultEventThread();
    
    
    
    // set socket fd nonblocking
    static int  SetFileDescNonBlocking(int inFileDesc);
protected:
    // Event loop
    void Entry();

private:
    void    removeWatch(event_req_t * ev);
    bool    addWatch(event_req_t * ev);
    void    processReadReadies(fd_set * rfds, int n);

    void    addTimer(event_timer_t * ev);
    void    processTimeouts();
    void    rollback_timer(ULONG now);
    int     calcNextTimeout(struct timeval * tv,ULONG now);
   
    void    firePending();
   
    void    init_wakeuper();
    void    uninit_wakeuper();
    void    wakeup();

    void    IncreaseWatchNum();
    void    DecreaseWatchNum();

    static void idleTimeOut(unsigned long timespan, void *userdata);

private:

    OSMutex mMutex;
    long    mIdleTimeout;
    //bool    mAutoStop;
    
    //////////////////////////////////////////////////////////////////////////
    fd_set mReadFds;
    int    mMaxfFd;
    int    mWatchNum;

    UT_LIST_BASE_NODE_T(event_req_t) mWatchList;
    UT_LIST_BASE_NODE_T(event_req_t) mPendingList;
    event_req_t mWakeupFdEvent;

    UT_LIST_BASE_NODE_T(event_timer_t) mTimerList;
    UT_LIST_BASE_NODE_T(event_timer_t) mTimeoutList;
    event_timer_t mIdleTimer;

    //////////////////////////////////////////////////////////////////////////
    int mFdWakeupRead;
    int mFdWakeupWrite;
    
};

#endif

