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
		History:	2009/04/01 Jokul for Tranzda,	modify function name     
*/


#include <OSBaseTypes.h>
#include "OSAtomic.h"
#include "OSThread.h"

#if __Win32__

UInt32 __os_compare_and_store(UInt32 oval, UInt32 nval, UInt32 *area)
{
	return oval == InterlockedCompareExchange((volatile LONG*)area,(LONG)nval,(LONG)oval);
}

UInt32 __os_atomic_add(UInt32 *area, UInt32 val)
{
	return InterlockedExchangeAdd((volatile LONG*)area,(LONG)val) + val;
}

UInt32 __os_atomic_sub(UInt32 *area,UInt32 val)
{
	return InterlockedExchangeAdd((volatile LONG*)area,-((LONG)val)) - val;
}

UInt32 __os_atomic_or(UInt32 *area, UInt32 val)
{
	UInt32 oldval;
	do 	{
		oldval= *area;
	} while(oldval != InterlockedCompareExchange((volatile LONG*)area,(LONG)(oldval|val),(LONG)oldval));
	
	return oldval;
}


#elif __linux__

#include "OSMutex.h"

static OSMutex& GetAtomicMutex()
{
	static OSMutex sAtomicMutex;
	return sAtomicMutex;
}

UInt32 __os_compare_and_store(UInt32 oval, UInt32 nval, UInt32 *area)
{
	UInt32 rv;
	// OSGlobalCriticalSection _ogcs;
	OSMutexLocker _locker(&GetAtomicMutex());
	if( oval == *area )
	{
		rv=1;
		*area = nval;
	}
	else
		rv=0;
	return rv;
}

UInt32 __os_atomic_add(UInt32 *area, UInt32 val)
{
	//OSGlobalCriticalSection _ogcs;
	OSMutexLocker _locker(&GetAtomicMutex());
	*area += val;
	return *area;
}

UInt32 __os_atomic_sub(UInt32 *area,UInt32 val)
{
	//OSGlobalCriticalSection _ogcs;
	OSMutexLocker _locker(&GetAtomicMutex());
	*area -= val;
	return *area;
}

UInt32 __os_atomic_or(UInt32 *area, UInt32 val)
{
	UInt32 oldval;

	//OSGlobalCriticalSection _ogcs;
	OSMutexLocker _locker(&GetAtomicMutex());
	oldval=*area;
	*area = oldval | val;
	return oldval;
}


#endif
