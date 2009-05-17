/*
* Wine debugging interface
*
* Copyright 1999 Patrik Stridvall
* Copyright 2009 Jokul for Tranzda
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

#ifndef tgmlog_util_h
#define tgmlog_util_h

#include "string.h"
#include "wtypes.h"

#ifndef GUID_DEFINED
#include "guiddef.h"
#endif

#ifndef _ULONGLONG_
# define _ULONGLONG_
# ifdef _MSC_VER
typedef signed __int64   LONGLONG,  *PLONGLONG;
typedef unsigned __int64 ULONGLONG, *PULONGLONG;
# elif defined(__GNUC__)
typedef long long LONGLONG,   *PLONGLONG;
typedef unsigned long long ULONGLONG,  *PULONGLONG;
# else
typedef signed __int64   DECLSPEC_ALIGN(8) LONGLONG,   *PLONGLONG;
typedef unsigned __int64 DECLSPEC_ALIGN(8) ULONGLONG,  *PULONGLONG;
#error asfd
# endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
TGMLOG_EXPORT unsigned char	__tgmlog_dbg_get_channel_flags( struct __tgmlog_debug_channel *channel );
TGMLOG_EXPORT int			__tgmlog_dbg_set_channel_flags( struct __tgmlog_debug_channel *channel,unsigned char set, unsigned char clear );

//////////////////////////////////////////////////////////////////////////
struct __tgmlog_debug_functions
{
	char * (*get_temp_buffer)( size_t n );
	void   (*release_temp_buffer)( char *buffer, size_t n );
	const char * (*dbgstr_an)( const char * s, int n );
	const char * (*dbgstr_wn)( const unsigned short *s, int n );
	int (*dbg_vprintf)( const char *format, va_list args );
	int (*dbg_vlog)( enum __tgmlog_debug_class cls, struct __tgmlog_debug_channel *channel,
		const char *function, const char *format, va_list args );
};

TGMLOG_EXPORT void			 __tgmlog_dbg_set_functions( const struct __tgmlog_debug_functions *new_funcs,
											struct __tgmlog_debug_functions *old_funcs, size_t size );


//////////////////////////////////////////////////////////////////////////


TGMLOG_EXPORT const char *  tgmlog_dbgstr_an( const char * s, int n );
TGMLOG_EXPORT const char * tgmlog_dbgstr_wn( const unsigned short *s, int n );

TGMLOG_EXPORT const char * tgmlog_dbgstr_a( const char *s );
TGMLOG_EXPORT const char * tgmlog_dbgstr_w( const unsigned short *s );
TGMLOG_EXPORT const char * tgmlog_dbgstr_guid( const GUID *id );
TGMLOG_EXPORT const char * tgmlog_dbgstr_longlong( ULONGLONG ll );

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
#define TGMLOG_CHANFLAG_GET(ch)				__tgmlog_dbg_get_channel_flags((ch))
#define TGMLOG_CHANFLAG_SET(ch,set,clr)		__tgmlog_dbg_set_channel_flags((ch),(set),(clr))
#define TGMLOG_FUNCS_SET(newfun,oldfun,size) __tgmlog_dbg_set_functions((newfun),(oldfun),(size))


#define TGMLOG_STR_AN(as,n)		tgmlog_dbgstr_an(as,n)
#define TGMLOG_STR_WN(ws,n)		tgmlog_dbgstr_an(ws,n)
#define TGMLOG_STR_A(as)		tgmlog_dbgstr_a(as)
#define TGMLOG_STR_W(ws)		tgmlog_dbgstr_w(ws)
#define TGMLOG_STR_GUID(iid)	tgmlog_dbgstr_guid(iid)

#ifdef __cplusplus
}
#endif


#endif//#ifndef tgmlog_util_h


