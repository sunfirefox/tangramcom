/*
* String fucntion
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
#ifndef _MY_AGR_STRING_HHH
#define _MY_AGR_STRING_HHH
#include <stdlib.h>
#include <stdarg.h>
#include	"windef.h"
#include	"winbase.h"

#ifdef __cplusplus
extern "C" {
#endif

	INT WINE_WINAPI agr_strcpy_ctow(LPWSTR dst, LPCSTR src );
	INT WINE_WINAPI agr_strncpy_ctow(LPWSTR dst, LPCSTR src ,int n);
	INT WINE_WINAPI agr_strcpy_wtoc(LPSTR dst, LPCWSTR src );
	INT WINE_WINAPI agr_strncpy_wtoc(LPSTR dst, LPCWSTR src ,int n);

#ifdef __cplusplus
}
#endif


#endif
