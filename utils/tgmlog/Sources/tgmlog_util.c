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
#include "tgmlog.h"
#include "tgmlog_common.h"
/* wrappers to use the function pointers */

extern const char *tgmlog_dbg_sprintf( const char *format, ... ) __TGMLOG_PRINTF_ATTR(1,2);

const char *tgmlog_dbgstr_an( const char * s, int n )
{
	return funcs.dbgstr_an(s, n);
}

const char *tgmlog_dbgstr_wn( const unsigned short *s, int n )
{
	return funcs.dbgstr_wn(s, n);
}

const char *tgmlog_dbgstr_a( const char * s)
{
	return funcs.dbgstr_an(s, -1);
}

const char *tgmlog_dbgstr_w( const unsigned short *s)
{
	return funcs.dbgstr_wn(s, -1);
}

const char *tgmlog_dbgstr_guid( const GUID *id )
{
	if (!id) return "(null)";
	if (!((ULONG_PTR)id >> 16)) return tgmlog_dbg_sprintf( "<guid-0x%04lx>", (ULONG_PTR)id & 0xffff );
	return tgmlog_dbg_sprintf( "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
		id->Data1, id->Data2, id->Data3,
		id->Data4[0], id->Data4[1], id->Data4[2], id->Data4[3],
		id->Data4[4], id->Data4[5], id->Data4[6], id->Data4[7] );
}

//const char *tgmlog_dbgstr_point( const POINT *pt )
//{
//	if (!pt) return "(null)";
//	return tgmlog_dbg_sprintf( "(%d,%d)", pt->x, pt->y );
//}

//const char *tgmlog_dbgstr_size( const SIZE *size )
//{
//	if (!size) return "(null)";
//	return tgmlog_dbg_sprintf( "(%d,%d)", size->cx, size->cy );
//}
//
//const char *tgmlog_dbgstr_rect( const RECT *rect )
//{
//	if (!rect) return "(null)";
//	return tgmlog_dbg_sprintf( "(%d,%d)-(%d,%d)", rect->left, rect->top,
//		rect->right, rect->bottom );
//}

const char *tgmlog_dbgstr_longlong( ULONGLONG ll )
{
	if (sizeof(ll) > sizeof(unsigned long) && ll >> 32)
		return tgmlog_dbg_sprintf( "%lx%08lx", (unsigned long)(ll >> 32), (unsigned long)ll );
	else return tgmlog_dbg_sprintf( "%lx", (unsigned long)ll );
}


/* printf with temp buffer allocation */
const char *tgmlog_dbg_sprintf( const char *format, ... )
{
	static const int max_size = 200;
	char *ret;
	int len;
	va_list valist;

	va_start(valist, format);
	ret = funcs.get_temp_buffer( max_size );
	len = VSNPRINTF( ret, max_size, format, valist );
	if (len == -1 || len >= max_size) ret[max_size-1] = 0;
	else funcs.release_temp_buffer( ret, len + 1 );
	va_end(valist);
	return ret;
}


