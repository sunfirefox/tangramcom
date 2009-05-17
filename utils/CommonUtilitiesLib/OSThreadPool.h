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
	History:
		2009/04/01 Jokul for Tranzda
			add  TGMCOMUTI_API declare 
*/
#ifndef OSTHREAD_POOLL_HHH
#define OSTHREAD_POOLL_HHH
#include "OSBaseTypes.h"

#define OSTP_EXECUTELONGFUNCTION			0x10
#define OSTP_EXECUTEINWAITTHREAD			0x00000004	//可能等待另外一个线程的执行结果


typedef DWORD (*thread_start_entry_t)(void* lpThreadParameter);

//!将工作任务压入队列
HRESULT OSTP_QueueUserWorkItem	(thread_start_entry_t Function, void* Context, ULONG Flags );
//!设置常驻线程，通常不超过CPU数目的两倍
ULONG	OSTP_SetResideWorkerNum	(ULONG inNum);


#endif//#ifndef OSTHREAD_POOLL_HHH

