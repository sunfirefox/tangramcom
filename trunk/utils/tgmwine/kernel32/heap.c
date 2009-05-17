/*
* Memory
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
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <stdarg.h>


#include "windef.h"
#include "winbase.h"

//////////////////////////////////////////////////////////////////////////
#ifdef __linux__
static SIZE_T heap_alloc_count = 0;

//#define WINE_WINAPI __attribute ((visibility))

LPVOID WINE_WINAPI HeapAlloc( HANDLE heap, DWORD flags, SIZE_T size )
{
	long * lptr =  (long*)malloc( size+4);
	if(lptr)	{
		heap_alloc_count	+=size;
		*lptr++				= size;

		if(flags & HEAP_ZERO_MEMORY){
			memset(lptr,0,size);
		}
	}
	return (LPVOID)lptr;
}

BOOL WINE_WINAPI HeapFree( HANDLE heap, DWORD flags, LPVOID ptr )
{
	if(ptr){
		long* lptr = (long*)ptr;
		heap_alloc_count -= *(--lptr);
		free(lptr);
		return 1;
	}
	return 0;
}

LPVOID WINE_WINAPI HeapReAlloc( HANDLE heap, DWORD flags, LPVOID ptr, SIZE_T size )
{
	if(NULL == ptr) return NULL;
	long* lptr = (long*)ptr;
	SIZE_T oldsize = *(lptr - 1);
	if(oldsize >= size){
		return (lptr);
	}

	lptr = (long*)realloc(--lptr,size+4);
	if(lptr)	{
		heap_alloc_count	+=size - (*lptr);
		*lptr++				= size;

		if(flags & HEAP_ZERO_MEMORY && size - oldsize ){
			memset(lptr,0,size);
		}
	}

	return lptr;
}

SIZE_T WINE_WINAPI HeapSize( HANDLE heap, DWORD flags, LPCVOID ptr )
{
	long* lptr = (long*)ptr;
	lptr--;
	return *lptr;
}

/***********************************************************************
*           GetProcessHeap    (KERNEL32.@)
*/
HANDLE WINE_WINAPI GetProcessHeap(void)
{
	return NULL;
}


HGLOBAL WINE_WINAPI GlobalAlloc(UINT uFlags,SIZE_T dwBytes){
	return (HGLOBAL)HeapAlloc(NULL,HEAP_ZERO_MEMORY,dwBytes);
}

HGLOBAL WINE_WINAPI GlobalReAlloc(HGLOBAL hMem,SIZE_T dwBytes,UINT uFlags){
	return (HGLOBAL)HeapReAlloc(NULL,uFlags,hMem,dwBytes);
}

SIZE_T	WINE_WINAPI	GlobalSize(HGLOBAL hMem	){
	return HeapSize(NULL,0,hMem);
}

LPVOID	WINE_WINAPI	GlobalLock(HGLOBAL hMem  ){
	return (LPVOID)hMem;
}

BOOL	WINE_WINAPI	GlobalUnlock(HGLOBAL hMem){
	return TRUE;
}

HGLOBAL	WINE_WINAPI	GlobalFree(HGLOBAL hMem ){
	HeapFree(NULL,0,hMem);
	return NULL;
}

HLOCAL WINE_WINAPI LocalAlloc(UINT flags, /* [in] Allocation attributes */ SIZE_T size /* [in] Number of bytes to allocate */			 ) 
{
	return GlobalAlloc( flags, size );
}

HLOCAL WINE_WINAPI LocalReAlloc(HLOCAL handle, SIZE_T size, UINT flags){
	return GlobalReAlloc(handle,size,flags);
}

#endif//__linux__
