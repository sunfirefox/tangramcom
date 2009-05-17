/*
* STRING function
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
#include "config.h"
#include "wine/port.h"

#include <ctype.h>
#include <stdarg.h>
#include <string.h>


#define WINE_NO_INLINE_STRING
#include "windef.h"
#include "winbase.h"

#include "agrwine_string.h"

static INT (WINE_WINAPI *pLoadStringA)(HINSTANCE, UINT, LPSTR, INT);
static INT (WINE_WINAPI *pwvsprintfA)(LPSTR, LPCSTR, va_list);

LPWSTR WINE_WINAPI lstrcpynW( LPWSTR dst, LPCWSTR src, INT n )
{
	LPWSTR d = dst;
	LPCWSTR s = src;
	UINT count = n;

	while ((count > 1) && *s)	{
		count--;
		*d++ = *s++;
	}
	if (count) *d = 0;
	return dst;
}

LPWSTR WINE_WINAPI lstrcpyW( LPWSTR dst, LPCWSTR src)
{
	LPWSTR d = dst;
	LPCWSTR s = src;

	while (*s){		*d++ = *s++;	}
	*d = 0;
	return dst;
}

INT WINE_WINAPI lstrlenW( LPCWSTR str )
{
	const WCHAR *s = str;
	while (*s) s++;
	return s - str;
}

INT WINE_WINAPI lstrlenA( LPCSTR str )
{
	return strlen( str );
}

//INT WINE_WINAPI lstrlenW( LPCWSTR str )
//{
//	const WCHAR *s = str;
//	while (*s) s++;
//	return s - str;
//}


//////////////////////////////////////////////////////////////////////////
//WINADVAPI INT WINAPI agr_strcpy_ctow(LPWSTR dst, LPCSTR src );
INT WINE_WINAPI agr_strcpy_ctow(LPWSTR dst, LPCSTR src )
{
	WCHAR *s = dst;
	if(dst == NULL || src == NULL)
		return -1;
	while ((*dst = *src)!=0){dst++;src++;}
	*dst = 0;
	return dst - s;
}

INT WINE_WINAPI agr_strncpy_ctow(LPWSTR dst, LPCSTR src ,int n)
{
	WCHAR *s = dst;
	if(dst == NULL || src == NULL)
		return -1;
	while ((*dst = *src)!=0 && (dst -s) < n){dst++;src++;}
	*dst = 0;
	return dst - s;
}

INT WINE_WINAPI agr_strcpy_wtoc(LPSTR dst, LPCWSTR src )
{
	char *s = dst;
	if(dst == NULL || src == NULL)
		return -1;
	while ((*dst = *src)!=0){dst++;src++;}
	*dst = 0;
	return dst - s;
}


INT WINE_WINAPI agr_strncpy_wtoc(LPSTR dst, LPCWSTR src ,int n)
{
	char *s = dst;
	if(dst == NULL || src == NULL)
		return -1;
	while ((*dst = *src)!=0 && (dst -s) < n){dst++;src++;}
	*dst = 0;
	return dst - s;
}




