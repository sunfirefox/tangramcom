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
*			change name form ev.h
*/

#ifndef _SYS_EV_H_
#define _SYS_EV_H_

#include "OSConfigure.h"

#if !defined(__Win32__) && !defined(__solaris__) && !defined(__sgi__) && !defined(__osf__) && !defined(__hpux__)
    #include <sys/queue.h>
#endif

//struct eventreq {
//  int      er_type;
////#define EV_FD 1    // file descriptor
//  int      er_handle;
//  void    *er_data;
////  int      er_rcnt;
////  int      er_wcnt;
////  int      er_ecnt;
////  int      er_eventbits;
//#define EV_RE  1
//#define EV_WR  2
//#define EV_EX  4
//#define EV_RM  8
//#define EV_BD  16 //bad file
//#define EV_NC  32 //file disconnect
//
//};

//typedef struct eventreq *er_t;

//#ifdef _KERNEL
//
//#define EV_RBYTES 0x1
//#define EV_WBYTES 0x2
//#define EV_RWBYTES (EV_RBYTES|EV_WBYTES)
//#define EV_RCLOSED 0x4
//#define EV_RCONN  0x8
//#define EV_ERRORS  0x10
//#define EV_WCLOSED 0x20
//#define EV_WCONN   0x40
//#define EV_OOBD    0x80
//#define EV_OOBM    0x100
//
//struct eventqelt {
//  TAILQ_ENTRY(eventqelt)  ee_slist;
//  TAILQ_ENTRY(eventqelt)  ee_plist;
//  struct eventreq  ee_req;
//  struct proc *    ee_proc;
//  u_int            ee_flags;
//#define EV_QUEUED 1
//  u_int            ee_eventmask;
//  struct socket   *ee_sp;
//};
//
//#endif /* _KERNEL */



#if __linux__
enum
{
	EEVENTTHREADEXIT = 0xFFFF
};
#endif

#if !MACOSXEVENTQUEUE

//class event_selector
//{
//public:
//	virtual int select_watchevent(struct eventreq *req, int which) = 0;
//	virtual int select_modwatch(struct eventreq *req, int which) = 0;
//	virtual int select_waitevent(struct eventreq *req, void* onlyForMOSX) = 0;
//
//	virtual int select_removeevent(int which) = 0;
//	virtual int select_removeevent_notclean(int which) = 0;//主要针对linux，将其从select中移除，但是不要求自动关闭描述符
//
//	virtual void select_startevents() = 0;
//	virtual void select_stopevents() = 0;
//
//	virtual void select_clearevents() = 0;
//
//	virtual ~event_selector(){};
//};

//class TGMCOMUTI_API event_selector_factory
//{
//public:
//	static event_selector * Create();
//	static void Destroy(event_selector* inIns);
//};

#endif

#endif /* _SYS_EV_H_ */
