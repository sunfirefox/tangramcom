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

	a new thread pool
	20090424 add by txf

*/
#include "OSConfigure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#ifndef __Win32__
#if __PTHREADS__
#include <pthread.h>
#if USE_THR_YIELD
#include <thread.h>
#endif
#else
#include <mach/mach.h>
#include <mach/cthreads.h>
#endif
#include <unistd.h>
#endif

#include <list>
#include <utility>
using std::list;
using std::pair;


#include "OSHeaders.h"
#include "MyAssert.h"
#include "OSMutex.h"
#include "OSCond.h"
#include "OSAtomic.h"
#include "OSThreadPool.h"
//#include "OSLogger.h"
#include "OS.h"
#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(lcmut);

#define OSTHRRPL_DEBUG

#ifdef	OSTHRRPL_DEBUG
#define OTP_DEBUG DTRACE

static ULONG	num_total_item = 0;
static ULONG	num_total_workers = 0;

#else
#define OTP_DEBUG(XXX) do {} while(0)
#endif

typedef struct  WorkItem_t
{
	thread_start_entry_t	func;
	void *					content;
	UInt32					flag;
	WorkItem_t(thread_start_entry_t inFunc, void* inContext,UInt32 inFlags)
		:func(inFunc),content(inContext),flag(inFlags)
	{}
}WorkItem_t;

static ULONG	num_work_items = 0;
static ULONG	num_workers = 0;			//总的线程数目
static ULONG	num_workers_busy = 0;		//正在忙的线程数目
static ULONG	num_workers_reside = 0;		//驻留的线程数目，最大为处理器数目的两倍
//static ULONG	num_workers_exclusive  = 0;	//专用的线程数目，为长时间执行的任务保留
static ULONG	num_processors = 1;
static BOOL		flag_system_exit = false;

//static OSThreadDataHolder& thread_data_holder()
//{	static OSThreadDataHolder ins;	return ins;};
//
//static inline bool workitem_from_same_pool()//来自同线程池的线程的调用
//{	OSThreadDataHolder& holder = thread_data_holder(); return &holder == holder.GetData(); }

static inline bool	thread_is_need_exit()
{	return (0 == num_work_items && num_workers > num_workers_reside) || flag_system_exit; }

static inline bool thread_is_need_start()
{
	bool ret;
	if(num_workers < num_processors * 2)
		ret = (num_workers == num_workers_busy || 0 == num_workers);
	else if(num_workers < num_processors * 3 )
		ret = (num_workers == num_workers_busy) && (num_work_items > num_workers*3 /*|| workitem_from_same_pool()*/);
	else
		ret = (num_workers == num_workers_busy) && (num_work_items > num_workers*5 /*|| workitem_from_same_pool()*/);

	//OTP_DEBUG("OSThreadPool:ret:%d,%lu,%lu,%lu\r\n",ret,num_workers,num_workers_busy,num_work_items));
	return ret;
}




class OSThreadPool
{
public:
	HRESULT AddWorkItem(thread_start_entry_t inFunc, void* inContext,UInt32 inFlags);

	HRESULT CreateWorker();

#ifdef __Win32__
	static unsigned int WINAPI _Entry(LPVOID inThreadPool)
#else
	static void* _Entry(void *inThreadPool)  //static
#endif
	{
		OSThreadPool* theThreadPool = (OSThreadPool*)inThreadPool;
		theThreadPool->Entry();

#ifdef __Win32__
		return 0;
#else
		return NULL;
#endif
	}

	void Entry();

	OSThreadPool()
	{
		//num_processors = OS::GetNumProcessors();
	}

	~OSThreadPool()	{
		flag_system_exit =  true;
		mCond.Broadcast();

		if(num_work_items){
			OTP_DEBUG("OSThreadPool:still have work items(%lu) not be excuted\r\n",(unsigned long)num_work_items);
		}

		if(num_workers){
			OTP_DEBUG("OSThreadPool:still have worker thread(%lu) not be exited\r\n",(unsigned long)num_workers);
		}
	}

	static OSThreadPool* GetInstance()
	{
		static OSThreadPool ins;
		return &ins;
	}
private:

	//typedef pair<thread_start_entry_t,void *> WorkItem_t;
	list< WorkItem_t > mWorkItems;
	OSMutex	mMutex;
	OSCond	mCond;


};


HRESULT OSThreadPool::AddWorkItem(thread_start_entry_t inFunc, void* inContext,UInt32 inFlags)
{
	//OTP_DEBUG((OSLL_TRACE,"OSThreadPool:AddWorkItem Start\r\n"));
	{
		OSMutexLocker _locker(&mMutex);
		mWorkItems.push_back(WorkItem_t(inFunc,inContext,inFlags));
		num_work_items++;
		//mCond.Broadcast();
#ifdef	OSTHRRPL_DEBUG
		num_total_item++;
#endif
	}
	
	//OTP_DEBUG((OSLL_TRACE,"OSThreadPool:AddWorkItem OK\r\n"));
	
	if(num_workers > num_workers_busy)
	{
		//OTP_DEBUG((OSLL_TRACE,"OSThreadPool:AddWorkItem Signal\r\n"));
		mCond.Signal();
	}

	if(thread_is_need_start())
	{
		//创建新的工作线程
		os_atomic_add(&num_workers,1);
		if(FAILED(CreateWorker()))
			os_atomic_sub(&num_workers,1);
		//OTP_DEBUG((OSLL_TRACE,"OSThreadPool:AddWorkItem CreateThread\r\n"));
	}

	//OTP_DEBUG((OSLL_TRACE,"OSThreadPool:AddWorkItem End\r\n"));
	return S_OK;
}

HRESULT OSThreadPool::CreateWorker()
{
#ifdef __Win32__
	unsigned int theId = 0; // We don't care about the identifier
	HANDLE theHandle = (HANDLE)_beginthreadex( NULL,   // Inherit security
		0,      // Inherit stack size
		_Entry, // Entry function
		(void*)this,    // Entry arg
		0,      // Begin executing immediately
		&theId );
	if(theHandle)
	{
		//OTP_DEBUG((OSLL_TRACE,"OSThreadPool:Create(%lu,%lu)\r\n",fThreadID,theId));
		CloseHandle(theHandle);
		return S_OK;
	}
	return E_FAIL;
#elif __PTHREADS__
	pthread_t fThreadID;
	pthread_attr_t* theAttrP = NULL;
	int err = pthread_create((pthread_t*)&fThreadID, theAttrP, _Entry, (void*)this);
	//OTP_DEBUG((OSLL_TRACE,"OSThreadPool:Create(%lu,%lu)\r\n",((ULONG*)&fThreadID)[0],theId));
	return err == 0? S_OK:E_FAIL;
#endif
}



void OSThreadPool::Entry()
{
	//UInt32	id = os_atomic_add(&num_workers,1);
	//thread_data_holder().SetData(&thread_data_holder());
#ifdef OSTHRRPL_DEBUG
	UInt32 id = num_total_workers++;
	UInt32	theCompleteItemsNum = 0;
#endif
	//OTP_DEBUG((OSLL_TRACE,"OSThreadPool:Therad Start(%lu),num_workers(%lu),num_workers_busy(%lu),num_work_items(%lu)\r\n",id,num_workers,num_workers_busy,num_work_items));
	while (!flag_system_exit)
	{	
		WorkItem_t theItem(NULL,NULL,0);
		{
			OSMutexLocker _locker(&mMutex);
recheck:
			if(num_work_items >0)
			{
				theItem = mWorkItems.front();
				mWorkItems.pop_front();
				num_work_items --;
			}
			else
			{
				const ULONG wait_timeout = 30000;
				SInt64 strTime = OS::GetTickCount();
				SInt64 curTime = strTime;
				while((0 == num_work_items && (strTime + wait_timeout) >  curTime) && !flag_system_exit){
					mCond.Wait(&mMutex,(UInt32)(strTime + wait_timeout - curTime));
					curTime = OS::GetTickCount();
					//OTP_DEBUG((OSLL_TRACE,"OSThreadPool:Therad Wakeup exit:%d num_work_items:%lu,span:%lu\r\n",thread_is_need_exit(),num_work_items,curTime - strTime));
				};

				if(thread_is_need_exit())
					break;
				else 
					goto recheck;
			}
		}

		//OTP_DEBUG((OSLL_TRACE,"OSThreadPool:Entry()  theItem.func Start\r\n"));
		if((OSTP_EXECUTELONGFUNCTION | OSTP_EXECUTEINWAITTHREAD) & theItem.flag )
		{
			os_atomic_sub(&num_workers,1);//由于需要长时间执行，因此不再参与调度判断
			//OTP_DEBUG((OSLL_TRACE,"OSThreadPool:OSTP_EXECUTELONGFUNCTION Start\r\n"));
		}
		else
		{
			os_atomic_add(&num_workers_busy,1);
		}

		
		theItem.func(theItem.content);
		

		if((OSTP_EXECUTELONGFUNCTION | OSTP_EXECUTEINWAITTHREAD) & theItem.flag )
		{
			os_atomic_add(&num_workers,1);//执行完毕，归还给线程池
			//OTP_DEBUG((OSLL_TRACE,"OSThreadPool:OSTP_EXECUTELONGFUNCTION End\r\n"));
		}
		else
		{
			os_atomic_sub(&num_workers_busy,1);
		}
		//OTP_DEBUG((OSLL_TRACE,"OSThreadPool:Entry()  theItem.func End\r\n\n\n\n\n"));

#ifdef OSTHRRPL_DEBUG
		theCompleteItemsNum++;
#endif
	}
	os_atomic_sub(&num_workers,1);
	OTP_DEBUG("OSThreadPool:Therad Stoped(%lu),num_workers(%lu),num_workers_busy(%lu),num_work_items(%lu),theCompleteItemsNum(%lu),Total(%lu,%lu)\r\n",
		(unsigned long)id,(unsigned long)num_workers,(unsigned long)num_workers_busy,(unsigned long)num_work_items,(unsigned long)theCompleteItemsNum,
        (unsigned long)num_total_item,(unsigned long)num_total_workers);
	//thread_data_holder().SetData(NULL);
}

HRESULT OSTP_QueueUserWorkItem( thread_start_entry_t inFunc, void* inContext, ULONG Flags )
{
	return OSThreadPool::GetInstance()->AddWorkItem(inFunc,inContext,Flags);
}


