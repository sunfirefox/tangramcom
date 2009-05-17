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
File:       OSCond.cpp

Contains:   Implementation of OSEventThread class

20090424 txf

*/
/*
问题0：如果所查找的EV目前处于pendinglist,ev所属对象调用DelEvent立刻析构，那么，很有可能造成段错误
问题1：没有DelTimer
*/
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <errno.h>
#include <string.h>

#include "OSConfigure.h"

#ifdef __Win32__
#include <winsock2.h>
#define USE_SOCKET_PAIR
#else
#include <unistd.h>
#include <sys/time.h>
#endif

#include <time.h>

#include "OS.h"
#include "OSEventThread.h"
#include "OSEventContext.h"

extern int dumb_socketpair(int socks[2], int make_overlapped);

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(evloop);
#undef TRACE
static void TRACE(const char* fmt, ...){}

//#define dump_event(x)
#define dump_event(ev) TRACE("Event %p,watchNode(next= %p,prev= %p),activeNode(next= %p,prev= %p),fd= %d,pers= %d,timeout = %lu,func= %p,param= %p",\
    ev,ev->watchNode.next,ev->watchNode.prev,ev->activeNode.next,ev->activeNode.prev,ev->fd,ev->persist,(unsigned long)ev->timeout,ev->func,ev->param)


bool  OSEventThread::addWatch(event_req_t * ev)
{
    TRACE("~~~~ +addWatch (%d) ~~~~\n",ev->fd);
    event_req_t * tev = UT_LIST_GET_FIRST(mWatchList);
    event_req_t * prev;

    FD_SET(ev->fd, &mReadFds);
    if (ev->fd >= mMaxfFd) mMaxfFd = ev->fd+1;

    if(tev == NULL || tev->fd > ev->fd){
        UT_LIST_ADD_FIRST(watchNode,mWatchList,ev);
        TRACE("~~~~ -addWatch ,mMaxfFd(%d),count(%d)~~~~\n",mMaxfFd,mWatchList.count);
        return true;
    }

    do{ 
        prev  = tev;
        tev = UT_LIST_GET_NEXT(watchNode,prev);  
    }while (tev && tev->fd < ev->fd);
    
    UT_LIST_INSERT_AFTER(watchNode,mWatchList,prev,ev);
    TRACE("~~~~ -addWatch ,mMaxfFd(%d),count(%d)~~~~\n",mMaxfFd,mWatchList.count);
    return true;
}

void OSEventThread::removeWatch(event_req_t * ev)
{
    TRACE("~~~~ +removeWatch (%d) ~~~~\n",ev->fd);
    FD_CLR(ev->fd, &mReadFds);
    UT_LIST_REMOVE(watchNode,mWatchList,ev);

    event_req_t * rev = UT_LIST_GET_LAST(mWatchList);
    mMaxfFd = rev?rev->fd+1 : 0;

    TRACE("~~~~ -removeWatch ,mMaxfFd(%d),count(%d)~~~~\n",mMaxfFd,mWatchList.count);
}

void OSEventThread::addTimer(event_timer_t * ev)
{
    //event_timer_t * list;
    //if (ev->timespan > 0) {
    //    // add to timer list
    //    list = UT_LIST_GET_FIRST(mTimerList);
    //    ev->timeout = ev->timespan + OS::GetTickCount();

    //    // keep list sorted
    //    while ( (list) && (list->timeout < ev->timeout )) {
    //            list = UT_LIST_GET_NEXT(watchNode,ev);
    //        }
    //    // list now points to the first event older than ev
    //    UT_LIST_INSERT_AFTER(watchNode,mTimerList,list,ev);
    //} 

    TRACE("~~~~ +addTimer (%d) ~~~~\n",ev->timespan);
    event_timer_t * tev = UT_LIST_GET_FIRST(mTimerList);
    event_timer_t * prev;

    ev->timeout = ev->timespan + OS::GetTickCount();

    if(tev == NULL || tev->timeout > ev->timeout){
        UT_LIST_ADD_FIRST(watchNode,mTimerList,ev);
        TRACE("~~~~ -addWatch ,timeout(%d),count(%d)~~~~\n",ev->timeout,mTimerList.count);
        return;
        //return true;
    }

    do{ 
        prev  = tev;
        tev = UT_LIST_GET_NEXT(watchNode,prev);  
    }while (tev && tev->timeout < ev->timeout);

    UT_LIST_INSERT_AFTER(watchNode,mTimerList,prev,ev);
    TRACE("~~~~ -addTimer ,timeout(%d),count(%d)~~~~\n",ev->timeout,mTimerList.count);
   // return true;
    
}

void OSEventThread::processTimeouts()
{
    TRACE("~~~~ +processTimeouts ~~~~\n");
    ULONG  now = OS::GetTickCount();

    OSMutexLocker _locker(&mMutex);
    event_timer_t * tev = UT_LIST_GET_FIRST(mTimerList);
    event_timer_t * next;

    // walk list, see if now >= ev->timeout for any events
    TRACE("~~~~ Looking for timers <= %lu ~~~~\n", (unsigned long)now);
    while ((tev) && (now > tev->timeout)) {
        // Timer expired
        TRACE("~~~~ firing timer ~~~~\n");
        next = UT_LIST_GET_NEXT(watchNode,tev);
        
        tev->status = 1;
        UT_LIST_ADD_FIRST(activeNode,mTimeoutList,tev);

        if (tev->persist == false){
            UT_LIST_REMOVE(watchNode,mTimerList,tev);
            //if(tev != &mIdleTimer)
            DecreaseWatchNum();
        }
        
        tev = next;
    }

    TRACE("~~~~ -processTimeouts ~~~~\n");
}

void OSEventThread::processReadReadies(fd_set * rfds, int n)
{
    TRACE("~~~~ +processReadReadies (%d) ~~~~\n", n);

    OSMutexLocker _locker(&mMutex);
    event_req_t* rev = UT_LIST_GET_FIRST(mWatchList);
    event_req_t* next;
    
    while((NULL != rev) && (n > 0)){
        next = UT_LIST_GET_NEXT(watchNode,rev);
        if (FD_ISSET(rev->fd, rfds)) {
            rev->status = 1;
            UT_LIST_ADD_FIRST(activeNode,mPendingList,rev);
            
            if (rev->persist == false) {
                removeWatch(rev);
                DecreaseWatchNum();
            }
            n--;
        }
        rev = next;
    }

    TRACE("~~~~ -processReadReadies (%d) ~~~~\n", n);
}

void OSEventThread::firePending()
{
    TRACE("~~~~ +firePending ~~~~\n");
    event_req_t * ev;
    while (ev = UT_LIST_GET_FIRST(mPendingList)) {
        UT_LIST_REMOVE(activeNode,mPendingList,ev);
        try
        {
			ev->func(ev->fd, OSEventContext::EV_RE, ev->param); 
            ev->status = 0;
        }
        catch (...) {
            ERR("Falut req = %p\n",ev);
            ASSERT_C(0);
        }
    }

    event_timer_t * tev;
    while (tev = UT_LIST_GET_FIRST(mTimeoutList)) {
        UT_LIST_REMOVE(activeNode,mTimeoutList,tev);
        try
        {
            tev->func(tev->timeout, tev->param); 
            tev->status = 0;
        }
        catch (...) {
            ERR("Falut timer = %p\n",tev);
            ASSERT_C(0);
        }
    }

    TRACE("~~~~ -firePending ~~~~\n");
}

int OSEventThread::calcNextTimeout(struct timeval * tv,ULONG now)
{
    event_timer_t * tev = UT_LIST_GET_FIRST(mTimerList);

    // Sorted list, so calc based on first node
    if (NULL == tev) {
        // no pending timers
        return -1;
    }

    TRACE("~~~~ now = %lu ms~~~~\n", (unsigned long)now);
    TRACE("~~~~ next = %lu ms ~~~~\n",(unsigned long)tev->timeout);
    if (tev->timeout > now) {
        ULONG span = tev->timeout - now;
        tv->tv_sec = span / 1000;
        tv->tv_usec = (span % 1000)*1000;
    } else {
        // timer already expired.
        tv->tv_sec = tv->tv_usec = 0;
    }
    return 0;
}

void OSEventThread::rollback_timer(ULONG now){
    TRACE("~~~~ +rollback_timer ~~~~\n");
    OSMutexLocker _locker(&mMutex);

    //event_req_t * tev = mTimerList.prev;
    event_timer_t * tev = UT_LIST_GET_LAST(mTimerList);
    event_timer_t * prev;

    // walk list, see if now >= ev->timeout for any events
    TRACE("~~~~ Looking for timers <= %lu ~~~~\n", (unsigned long)now);
    while ((tev) && (now < tev->timeout) && (now + 0xFFFFFF) < tev->timeout) {
        // Timer expired
        TRACE("~~~~ firing timer ~~~~\n");
        prev = UT_LIST_GET_PREV(watchNode,tev);
        UT_LIST_REMOVE(watchNode,mTimerList,tev);
        UT_LIST_ADD_FIRST(activeNode,mTimeoutList,tev);
        tev = prev;
    }
    TRACE("~~~~ -rollback_timer ~~~~\n");
}

// Initialize internal data structs
OSEventThread::OSEventThread()
:mIdleTimeout(-1),mMaxfFd(0),mFdWakeupRead(-1),mFdWakeupWrite(-1)
{
    FD_ZERO(&mReadFds);
    UT_LIST_INIT(mTimerList);
    UT_LIST_INIT(mPendingList);
    UT_LIST_INIT(mWatchList);
}

OSEventThread::~OSEventThread(){
    SendStopRequest();
    wakeup();
}

long OSEventThread::SetIdleTimeOut(long idleTimeout)
{
    TRACE("old:%d,new:%d\n",mIdleTimeout,idleTimeout);
    long old = mIdleTimeout;
    mIdleTimeout = idleTimeout;
    return old;
}

// Initialize an event
void OSEventThread::SetEvent(event_req_t * ev, int fd, bool persist, event_req_cb_t func, void * param)
{
    TRACE("~~~~ SetEvent %x ~~~~\n", (unsigned int)ev);
    memset(ev, 0, sizeof(event_req_t));
    ev->fd = fd;
    ev->persist = persist;
    ev->func = func;
    ev->param = param;

}

// Initialize an timer 
void OSEventThread::SetTimer(event_timer_t * ev, unsigned long timespan, bool persist, event_timer_cb_t func, void * param)
{
    TRACE("~~~~ SetTimer %x ~~~~\n", (unsigned int)ev);
    memset(ev, 0, sizeof(event_req_t));
    ev->timespan = timespan;
    ev->persist = persist;
    ev->func = func;
    ev->param = param;
}

int  OSEventThread::SetFileDescNonBlocking(int inFileDesc)
{
#ifdef __Win32__
    u_long one = 1;
    int err = ::ioctlsocket(inFileDesc, FIONBIO, &one);
#else
    int flag = ::fcntl(inFileDesc, F_GETFL, 0);
    int err = ::fcntl(inFileDesc, F_SETFL, flag | O_NONBLOCK);
#endif
    AssertV(err == 0, OSThread::GetErrno());
    return err;
}

// Add event to watch list
void OSEventThread::AddEvent(event_req_t * ev)
{
    TRACE("~~~~ +AddEvent ~~~~\n");
    if(!ev) return;

    {
        OSMutexLocker _locker(&mMutex);
        if(FD_ISSET(ev->fd,&mReadFds)) 
            return;
        if(addWatch(ev)){
            IncreaseWatchNum();
        }
    }
    wakeup();
    TRACE("~~~~ -AddEvent ~~~~\n");
}

// Add timer event
void OSEventThread::AddTimer(event_timer_t * ev)
{
    TRACE("~~~~ +AddTimer ~~~~\n");
    if (ev->timespan <= 0)
        return;
    {
        OSMutexLocker _locker(&mMutex);
        addTimer(ev);
        IncreaseWatchNum();
    }
    wakeup();

    TRACE("~~~~ -AddTimer ~~~~\n");
}

// Remove event from watch or timer list
void OSEventThread::DelEvent(event_req_t * ev)
{
    TRACE("~~~~ +DelEvent ~~~~\n");
    if(!ev) return;

    {
        OSMutexLocker _locker(&mMutex);
        if(NULL != UT_LIST_GET_NEXT(watchNode,ev) || NULL != UT_LIST_GET_PREV(watchNode,ev))
        {
            removeWatch(ev);
            DecreaseWatchNum();
        }
    }
    wakeup();

    while (ev->status != 0)//just for single thread
    {
       WARN("wait exit from pending list \n");
       OSThread::Sleep(1);
    }
    TRACE("~~~~ -DelEvent ~~~~\n");
}

// Remove timer from watch list
void OSEventThread::DelTimer(event_timer_t * ev)
{
    TRACE("~~~~ +DelTimer ~~~~\n");
    if(!ev) return;

    {
        OSMutexLocker _locker(&mMutex);
        if(NULL == UT_LIST_GET_NEXT(watchNode,ev) || NULL == UT_LIST_GET_PREV(watchNode,ev))
        {
            UT_LIST_REMOVE(watchNode,mTimerList,ev);
            DecreaseWatchNum();
        }
    }
    wakeup();

    while (ev->status != 0)//just for single thread
    {
        WARN("wait exit from pending list \n");
        OSThread::Sleep(1);
    }
    TRACE("~~~~ -DelTimer ~~~~\n");
}

void OSEventThread::Entry()
{
    int n;
    fd_set rfds;
    struct timeval tv;
    struct timeval * ptv = NULL;

    ULONG lasttime = OS::GetTickCount();
    ULONG currtime = lasttime;

    init_wakeuper();

    while (!IsStopRequested()) {

        //10 check time rollback
        lasttime = currtime;
        currtime = OS::GetTickCount();
        if(currtime < lasttime) {
            rollback_timer(currtime);
            firePending();
        }

        //20 calc timeout
        if (-1 == calcNextTimeout(&tv,currtime)) {
            // no pending timers; block indefinitely
            TRACE("~~~~ no timers; blocking indefinitely ~~~~\n");
            ptv = NULL;
        } else {
            TRACE("~~~~ blocking for %ds + %dus ~~~~\n", (int)tv.tv_sec, (int)tv.tv_usec);
            ptv = &tv;
        }

        //30 make local copy of read fd_set
        memcpy(&rfds, &mReadFds, sizeof(fd_set));

        //40 select
        n = select(mMaxfFd, &rfds, NULL, NULL, ptv);

        TRACE("~~~~ %d events fired ~~~~\n", n);
        
        if(n > 0){

            //50 Check for read-ready
            processReadReadies(&rfds, n);

        }else if(n == 0){

            //60 Check for timeouts
            processTimeouts();

        }else{

            //70 check error
            int err = OSThread::GetErrno();
            switch(err)
            {
            case EINTR:
                continue;
            case EBADF:
                WARN("event_req: EBADF\n");
				continue;
            case ENOTCONN:
                ERR("event_req: ENOTCONN\n");
                continue;
            default:
                ERR("event_req: select error (%d)\n", err);
                ASSERT_C(0);
                return;
            }
        }


        //80 Fire away
        firePending();
    }

    uninit_wakeuper();

    TRACE("~~~~ Thread exit:%x ~~~~\n",(unsigned int)GetCurrentThreadID());
}

/**
* A write on the wakeup fd is done just to pop us out of select()
* We empty the buffer here and then event_req will reset the timers on the
* way back down
*/
static void processWakeupCallback(int fd, short flags, void *param)
{
    char buff[16];
    int ret;
    UNUSED(flags);
    UNUSED(param);
    TRACE("processWakeupCallback\n");

    /* empty our wakeup socket out */
    do {
#ifdef USE_SOCKET_PAIR
        ret = ::recv(fd, buff, sizeof(buff),0);
#else
        ret = ::read(fd, buff, sizeof(buff));
#endif
    } while (ret > 0 || (ret < 0 && OSThread::GetErrno() == EINTR)); 
}

void    OSEventThread::wakeup()
{
    int ret;
    if (mFdWakeupWrite >0 &&  this->GetThreadID() != this->GetCurrentThreadID() ) {
        /* trigger event loop to wakeup 
        No reason to to this if we're in the event loop thread */
        do {  
#ifdef USE_SOCKET_PAIR
            ret = ::send(mFdWakeupWrite, " ", 1, 0);
#else
            ret = ::write (mFdWakeupWrite, " ", 1);
#endif
        } while (ret < 0 && OSThread::GetErrno() == EINTR);
    }    
}

void    OSEventThread::init_wakeuper()
{
    int ret;
    int filedes[2];

#ifdef USE_SOCKET_PAIR
    ret = dumb_socketpair(filedes, 0);
#else
    ret = pipe(filedes);
#endif

    if (ret < 0) {
        ERR("Error in pipe() errno:%d\n", errno);
    }

    mFdWakeupRead = filedes[0];
    mFdWakeupWrite = filedes[1];

    SetFileDescNonBlocking(mFdWakeupRead);
    SetEvent (&mWakeupFdEvent, mFdWakeupRead, true,
        processWakeupCallback, NULL);

    {
        OSMutexLocker _locker(&mMutex);
        addWatch(&mWakeupFdEvent);
    }
}

void    OSEventThread::uninit_wakeuper(){
    DelEvent(&mWakeupFdEvent);
#if __Win32__ && defined(USE_SOCKET_PAIR) 
    closesocket(mFdWakeupRead);
    closesocket(mFdWakeupWrite);
#else
    close(mFdWakeupRead);
    close(mFdWakeupWrite);
#endif

    mFdWakeupRead = -1;
    mFdWakeupWrite= -1;
}



void    OSEventThread::IncreaseWatchNum()
{
    int n = ++mWatchNum;
    if(n == 1 && GetCurrentThreadID() != GetThreadID()){
        //if(NULL != UT_LIST_GET_NEXT(watchNode,&mIdleTimer) || NULL != UT_LIST_GET_PREV(watchNode,&mIdleTimer) )
        //    UT_LIST_REMOVE(watchNode,mTimerList,&mIdleTimer);
        this->Start();
    }
}

void OSEventThread::idleTimeOut(unsigned long timespan, void *userdata)
{
    OSEventThread * This = (OSEventThread*)userdata;
    OSMutexLocker _locker(&This->mMutex);
    
    TRACE("\n");
    if(This->mWatchNum == 0 && This->mIdleTimeout >=0 ){
        This->SendStopRequest();
    }
}

void    OSEventThread::DecreaseWatchNum()
{
    int n = --mWatchNum;
    if(n == 0 ){
        if(mIdleTimeout == 0) {
            this->SendStopRequest();
        }
        //else if(mIdleTimeout > 0) {
        //    SetTimer(&mIdleTimer,mIdleTimeout,false,idleTimeOut,this);
        //    addTimer(&mIdleTimer);
        //}
    }
}


OSEventThread* OSEventThread::GetDefaultEventThread(){
    static OSEventThread ins;
    return &ins;
}

