/*
* Thread info
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
#if __linux__
#include "config.h"
#include "wine/port.h"

#include <ctype.h>
#include <stdarg.h>
#include <string.h>

#define WINE_NO_INLINE_STRI
#include "windef.h"
#include "winbase.h"
#include "wine/winbase16.h"
#include "wine/unicode.h"
//#include "wine/exception.h"

#include <pthread.h>

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(agrwine);

DWORD WINE_WINAPI GetCurrentProcessId(void)
{
	DWORD id = (DWORD)getpid();
	TRACE("%lu\n",id);
	return id;
}
DWORD WINE_WINAPI GetCurrentThreadId(void)
{
	DWORD id =  (DWORD)pthread_self();
	TRACE("%lu\n",id);
	return id;
}

#endif//__linux__

