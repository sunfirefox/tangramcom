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
 *
 *  History:
 *      11-Feb-1999 Umesh Vaishampayan (umeshv@apple.com)
 *          Added os_atomic_or().
 *
 *      26-Oct-1998 Umesh Vaishampayan (umeshv@apple.com)
 *          Made the header c++ friendly.
 *
 *      12-Oct-1998 Umesh Vaishampayan (umeshv@apple.com)
 *          Changed simple_ to spin_ so as to coexist with cthreads till
 *          the merge to the system framework.
 *
 *      8-Oct-1998  Umesh Vaishampayan (umeshv@apple.com)
 *          Created from the kernel code to be in a dynamic shared library.
 *          Kernel code created by: Bill Angell (angell@apple.com)
 */

#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#include "OSBaseTypes.h"

#ifdef __cplusplus
extern "C" {
#endif
TGMCOMUTI_API UInt32 __os_compare_and_store(UInt32 oval,UInt32 nval, UInt32 *area);

TGMCOMUTI_API UInt32 __os_atomic_add(UInt32 *area, UInt32 val);
TGMCOMUTI_API UInt32 __os_atomic_sub(UInt32 *area, UInt32 val);
TGMCOMUTI_API UInt32 __os_atomic_or(UInt32 *area, UInt32 mask);

#ifdef __cplusplus
}
#endif

#define os_compare_and_store(oval,nval,area) __os_compare_and_store((UInt32)oval,(UInt32)nval, (UInt32*)area)

#define os_atomic_add(area,val) __os_atomic_add((UInt32 *)area, (UInt32)val)
#define os_atomic_sub(area,val) __os_atomic_sub((UInt32 *)area, (UInt32)val)
#define os_atomic_or(area,val) __os_atomic_or((UInt32 *)area, (UInt32)val)



//unsigned long os_atomic_add(unsigned long *area, long val);
//unsigned long os_atomic_sub(unsigned long *area, long val);
//unsigned long os_atomic_or	(unsigned long *area, unsigned long inMask);


#endif /* _ATOMIC_H_ */
