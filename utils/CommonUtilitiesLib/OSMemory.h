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
    File:       OSMemory.h

    Contains:   Prototypes for overridden new & delete, definition of OSMemory
                class which implements some memory leak debugging features.
	
	History:
				2009/04/01 Jokul for Tranzda
					add "delete" declare
                    
    
*/

#ifndef __OS_MEMORY_H__
#define __OS_MEMORY_H__

#include "OSBaseTypes.h"
#include "OSQueue.h"
#include "OSMutex.h"

class OSMemory
{
    public:
    
#if MEMORY_DEBUGGING
        //If memory debugging is on, clients can get access to data structures that give
        //memory status.
        static OSQueue* GetTagQueue() { return &sTagQueue; }
        static OSMutex* GetTagQueueMutex() { return &sMutex;    }
        static UInt32   GetAllocatedMemory() { return sAllocatedBytes; }

        static void*    DebugNew(size_t size, char* inFile, int inLine, Bool16 sizeCheck);
        static void     DebugDelete(void *mem);
        static Bool16       MemoryDebuggingTest();
        static void     ValidateMemoryQueue();

        enum
        {
            kMaxFileNameSize = 48
        };
        
        struct TagElem
        {
            OSQueueElem elem;
            char fileName[kMaxFileNameSize];
            int     line;
            UInt32 tagSize; //how big are objects of this type?
            UInt32 totMemory; //how much do they currently occupy
            UInt32 numObjects;//how many are there currently?
        };
#endif

        // Provides non-debugging behaviour for new and delete
        static void*    New(size_t inSize);
        static void     Delete(void* inMemory);
        
        //When memory allocation fails, the server just exits. This sets the code
        //the server exits with
        static void SetMemoryError(SInt32 inErr);
        
#if MEMORY_DEBUGGING
    private:
            
        struct MemoryDebugging
        {
            OSQueueElem elem;
            TagElem* tagElem;
            UInt32 size;
        };
        static OSQueue sMemoryQueue;
        static OSQueue sTagQueue;
        static UInt32  sAllocatedBytes;
        static OSMutex sMutex;
        
#endif
};


// NEW MACRO
// When memory debugging is on, this macro transparently uses the memory debugging
// overridden version of the new operator. When memory debugging is off, it just compiles
// down to the standard new.

#if MEMORY_DEBUGGING

#ifdef  NEW
#error Conflicting Macro "NEW"
#endif

#define NEW new (__FILE__, __LINE__)

#else

#ifdef  NEW
#error Conflicting Macro "NEW"
#endif

#define NEW new

#endif


// 
// PLACEMENT NEW OPERATOR
#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE
inline void* operator new(size_t, void* ptr) 
{ return ptr;}
inline void operator delete(void *, void *)
{	// delete if placement new fails
}
#endif



#if MEMORY_DEBUGGING

// These versions of the new operator with extra arguments provide memory debugging
// features.

void* operator new(size_t s, char* inFile, int inLine);
void* operator new[](size_t s, char* inFile, int inLine);

void operator delete(void* mem, char* inFile, int inLine);
void operator delete[](void* mem, char* inFile, int inLine);

#endif

// When memory debugging is not on, these are overridden so that if new fails,
// the process will exit.
void* operator new (size_t s);
void* operator new[](size_t s);

void operator delete(void* mem);
void operator delete[](void* mem);


#endif //__OS_MEMORY_H__
