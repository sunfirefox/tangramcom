/*
* CriticalSection
*
* Copyright 2009  Jokul for Tranzda
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
*/

#include "critsection.h"

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(csec);

//////////////////////////////////////////////////////////////////////////
#include "OSMutex.h"
void  WINE_WINAPI  InitializeCriticalSection(CRITICAL_SECTION *lpCrit)
{
	TRACE("\n");
	lpCrit->LockSemaphore = new OSMutex();
	lpCrit->DebugInfo = (PRTL_CRITICAL_SECTION_DEBUG)malloc(sizeof(*lpCrit->DebugInfo));
	memset(lpCrit->DebugInfo,0,sizeof(*lpCrit->DebugInfo));

	TRACE(" lpCrit(%p),LockSemaphore(%p),DebugInfo(%p)\n",lpCrit,lpCrit->LockSemaphore,lpCrit->DebugInfo);
}

void  WINE_WINAPI  EnterCriticalSection(CRITICAL_SECTION *lpCrit)
{
	if(lpCrit->LockSemaphore == NULL){
		TRACE("auto init CriticalSection\n");
		InitializeCriticalSection(lpCrit);
	}

	OSMutex * theMutex = (OSMutex *)lpCrit->LockSemaphore;
	TRACE("  lpCrit(%p),LockSemaphore(%p)\n",lpCrit,lpCrit->LockSemaphore);
	if(theMutex) {
		theMutex->Lock();
	}
}

void  WINE_WINAPI  LeaveCriticalSection(CRITICAL_SECTION *lpCrit)
{
	OSMutex * theMutex = (OSMutex *)lpCrit->LockSemaphore;
	TRACE(" lpCrit(%p),LockSemaphore(%p)\n",lpCrit,lpCrit->LockSemaphore);
	if(theMutex) {
		theMutex->Unlock();
	}
}
void  WINE_WINAPI  DeleteCriticalSection(CRITICAL_SECTION *lpCrit)
{
	OSMutex * theMutex = (OSMutex *)lpCrit->LockSemaphore;
	TRACE(" lpCrit(%p),LockSemaphore(%p)\n",lpCrit,lpCrit->LockSemaphore);
	if(theMutex) {
		delete theMutex;
	}
	
	if(lpCrit->DebugInfo) {
		free(lpCrit->DebugInfo);
	}
}





//////////////////////////////////////////////////////////////////////////



#include "OSMutex.h"
#include "OSCond.h"

struct  AgrEvent_t
{
	OSMutex mMutex;
	OSCond  mCond;
	bool	mSignaled;
};

HANDLE WINE_WINAPI Agr_CreateEvent()
{
	AgrEvent_t * evt = new AgrEvent_t;
	evt->mSignaled = false;
	return evt;
}

void WINE_WINAPI Agr_SetEvent(HANDLE handle)
{
	AgrEvent_t * evt = (AgrEvent_t*)handle;
	OSMutexLocker locker(&evt->mMutex);
	evt->mSignaled = true;
	evt->mCond.Signal();
}

void WINE_WINAPI Agr_WaitEvent(HANDLE handle)
{
	AgrEvent_t * evt = (AgrEvent_t*)handle;
	OSMutexLocker locker(&evt->mMutex);
	if(!evt->mSignaled)
		evt->mCond.Wait(&evt->mMutex,0);
}

void WINE_WINAPI Agr_CloseEvent(HANDLE handle){
	AgrEvent_t * evt = (AgrEvent_t*)handle;
	delete evt;
}



HANDLE WINE_WINAPI Agr_CreateMutex(){
	OSMutex * mtx = new OSMutex();
	return (HANDLE)mtx;
}

void WINE_WINAPI Agr_LockMutex(HANDLE handle ){
	OSMutex * mtx = (OSMutex*)handle;
	if(mtx)
	mtx->Lock();
}
void WINE_WINAPI Agr_UnlockMutex(HANDLE handle){
	OSMutex * mtx = (OSMutex*)handle;
	if(mtx)
	mtx->Unlock();
}
void WINE_WINAPI Agr_CloseMutex(HANDLE handle){
	OSMutex * mtx = (OSMutex*)handle;
	if(mtx)
	delete mtx;
}


