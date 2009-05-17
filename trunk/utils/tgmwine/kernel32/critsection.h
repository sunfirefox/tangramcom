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
#ifndef _MY_CRITSECTION_HHH
#define _MY_CRITSECTION_HHH
#include <stdlib.h>
#include <stdarg.h>
#include	"windef.h"
#include	"winbase.h"

#ifdef __cplusplus
extern "C" {
#endif


void  WINE_WINAPI  InitializeCriticalSection(CRITICAL_SECTION *lpCrit);
void  WINE_WINAPI  EnterCriticalSection(CRITICAL_SECTION *lpCrit);
void  WINE_WINAPI  LeaveCriticalSection(CRITICAL_SECTION *lpCrit);
void  WINE_WINAPI  DeleteCriticalSection(CRITICAL_SECTION *lpCrit);

LPVOID	WINE_WINAPI HeapAlloc( HANDLE heap, DWORD flags, SIZE_T size );
BOOL	WINE_WINAPI HeapFree( HANDLE heap, DWORD flags, LPVOID ptr );
LPVOID	WINE_WINAPI HeapReAlloc( HANDLE heap, DWORD flags, LPVOID ptr, SIZE_T size );
SIZE_T	WINE_WINAPI HeapSize( HANDLE heap, DWORD flags, LPCVOID ptr );


HANDLE	WINE_WINAPI Agr_CreateEvent();
void	WINE_WINAPI Agr_SetEvent(HANDLE handle);
void	WINE_WINAPI Agr_WaitEvent(HANDLE handle);
void	WINE_WINAPI Agr_CloseEvent(HANDLE handle);

HANDLE WINE_WINAPI Agr_CreateMutex();
void WINE_WINAPI Agr_LockMutex(HANDLE handle);
void WINE_WINAPI Agr_UnlockMutex(HANDLE handle);
void WINE_WINAPI Agr_CloseMutex(HANDLE handle);

#ifdef __cplusplus
}
#endif


#endif
