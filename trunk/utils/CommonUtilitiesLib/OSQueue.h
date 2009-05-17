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
    File:       OSQueue.h

    Contains:   implements OSQueue class
                    
	History:
				2009/04/01 Jokul for Tranzda
					add  TGMCOMUTI_API declare    
*/

#ifndef _OSQUEUE_H_
#define _OSQUEUE_H_

#include "MyAssert.h"
#include "OSBaseTypes.h"
#include "OSMutex.h"
#include "OSCond.h"
#include "OSThread.h"

#define OSQUEUETESTING 0

class OSQueue;

class TGMCOMUTI_API OSQueueElem {
    public:
        OSQueueElem(void* enclosingObject = NULL) : fNext(NULL), fPrev(NULL), fQueue(NULL),
                                                    fEnclosingObject(enclosingObject) {}
        virtual ~OSQueueElem() { Assert(fQueue == NULL); }

        Bool16 IsMember(const OSQueue& queue) { return (&queue == fQueue); }
        Bool16 IsMemberOfAnyQueue()     { return fQueue != NULL; }
        void* GetEnclosingObject()  { return fEnclosingObject; }
        void SetEnclosingObject(void* obj) { fEnclosingObject = obj; }

        OSQueueElem* Next() { return fNext; }
        OSQueueElem* Prev() { return fPrev; }
        OSQueue* InQueue()  { return fQueue; }
        inline void Remove();

    private:

        OSQueueElem*    fNext;
        OSQueueElem*    fPrev;
        OSQueue *       fQueue;
        void*           fEnclosingObject;

        friend class    OSQueue;
};

class TGMCOMUTI_API OSQueue {
    public:
        OSQueue();
        ~OSQueue() {}

        void            EnQueue(OSQueueElem* object);
        OSQueueElem*    DeQueue();

        OSQueueElem*    GetHead() { if (fLength > 0) return fSentinel.fPrev; return NULL; }
        OSQueueElem*    GetTail() { if (fLength > 0) return fSentinel.fNext; return NULL; }
        UInt32          GetLength() { return fLength; }
        
        void            Remove(OSQueueElem* object);

#if OSQUEUETESTING
        static Bool16       Test();
#endif

    protected:

        OSQueueElem     fSentinel;
        UInt32          fLength;
};

class TGMCOMUTI_API OSQueueIter
{
    public:
        OSQueueIter(OSQueue* inQueue) : fQueueP(inQueue), fCurrentElemP(inQueue->GetHead()) {}
        OSQueueIter(OSQueue* inQueue, OSQueueElem* startElemP ) : fQueueP(inQueue)
            {
                if ( startElemP )
                {   Assert( startElemP->IsMember(*inQueue ) );
                    fCurrentElemP = startElemP;
                
                }
                else
                    fCurrentElemP = NULL;
            }
        ~OSQueueIter() {}
        
        void            Reset() { fCurrentElemP = fQueueP->GetHead(); }
        
        OSQueueElem*    GetCurrent() { return fCurrentElemP; }
        void            Next();
        
        Bool16          IsDone() { return fCurrentElemP == NULL; }
        
    private:
    
        OSQueue*        fQueueP;
        OSQueueElem*    fCurrentElemP;
};

class TGMCOMUTI_API OSQueue_Blocking
{
    public:
		OSQueue_Blocking():fDisable(FALSE) {}
        ~OSQueue_Blocking() {}
        
        OSQueueElem*    DeQueueBlocking(OSThread* inCurThread, SInt32 inTimeoutInMilSecs);
        OSQueueElem*    DeQueue();//will not block
        void            EnQueue(OSQueueElem* obj);
		void			EnQueueBroadcast(OSQueueElem* obj);
        
        OSCond*         GetCond()   { return &fCond; }
        OSQueue*        GetQueue()  { return &fQueue; }

		void			Disable();
        
    private:

        OSCond              fCond;
        OSMutex             fMutex;
        OSQueue             fQueue;
		BOOL				fDisable;
};


void    OSQueueElem::Remove()
{
    if (fQueue != NULL)
        fQueue->Remove(this);
}
#endif //_OSQUEUE_H_
