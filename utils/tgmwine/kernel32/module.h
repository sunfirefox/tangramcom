/*
* Module API
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
#ifndef _MY_WINE_MODULE_HHH
#define _MY_WINE_MODULE_HHH
#include <stdlib.h>
#include <stdarg.h>
#include	"windef.h"

#ifdef __cplusplus
extern "C" {
#endif

	HMODULE  WINAPI LoadLibraryA(LPCSTR libname);
	HMODULE  WINAPI LoadLibraryExA(LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
	HMODULE	WINAPI LoadLibraryExW(LPCWSTR libnameW, HANDLE hfile, DWORD flags);
	BOOL	 WINAPI FreeLibrary(HINSTANCE hLibModule);
	FARPROC  WINAPI GetProcAddress( HMODULE hModule, LPCSTR function );
	DWORD	 WINAPI GetModuleFileNameA(HMODULE hModule,LPSTR lpFileName,DWORD size );
	HMODULE  WINAPI GetCurrentModule(void * func);
	BOOL	WINAPI DisableThreadLibraryCalls( HMODULE hModule );
	
#ifdef __cplusplus
}
#endif


#endif

