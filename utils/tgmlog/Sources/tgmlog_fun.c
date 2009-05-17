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

/* allocate some tmp string space */
/* FIXME: this is not 100% thread-safe */
static char *get_temp_buffer( size_t size )
{
	static char *list[32];
	static int pos;
	char *ret;
	int idx;

#ifdef WIN32
	idx = InterlockedExchangeAdd( &pos, 1 ) % (sizeof(list)/sizeof(list[0]));
#else
	idx = ++pos%32;//the arm linux is signal cpu
#endif

	if ((ret = realloc( list[idx], size ))) list[idx] = ret;
	return ret;
}


/* release unused part of the buffer */
static void release_temp_buffer( char *buffer, size_t size )
{
	/* don't bother doing anything */
	UNUSED(buffer);
	UNUSED(size);
}


/* default implementation of tgmlog_dbgstr_an */
static const char *default_dbgstr_an( const char *str, int n )
{
	static const char hex[16] = "0123456789abcdef";
	char *dst, *res;
	size_t size;

	if (!((ULONG_PTR)str >> 16))
	{
		if (!str) return "(null)";
		res = funcs.get_temp_buffer( 6 );
		sprintf( res, "#%04x", LOWORD(str) );
		return res;
	}
	if (n == -1) n = strlen(str);
	if (n < 0) n = 0;
	size = 10 + min( 300, n * 4 );
	dst = res = funcs.get_temp_buffer( size );
	*dst++ = '"';
	while (n-- > 0 && dst <= res + size - 9)
	{
		unsigned char c = *str++;
		switch (c)
		{
		case '\n': *dst++ = '\\'; *dst++ = 'n'; break;
		case '\r': *dst++ = '\\'; *dst++ = 'r'; break;
		case '\t': *dst++ = '\\'; *dst++ = 't'; break;
		case '"':  *dst++ = '\\'; *dst++ = '"'; break;
		case '\\': *dst++ = '\\'; *dst++ = '\\'; break;
		default:
			if (c >= ' ' && c <= 126)
				*dst++ = c;
			else
			{
				*dst++ = '\\';
				*dst++ = 'x';
				*dst++ = hex[(c >> 4) & 0x0f];
				*dst++ = hex[c & 0x0f];
			}
		}
	}
	*dst++ = '"';
	if (n > 0)
	{
		*dst++ = '.';
		*dst++ = '.';
		*dst++ = '.';
	}
	*dst++ = 0;
	funcs.release_temp_buffer( res, dst - res );
	return res;
}


/* default implementation of tgmlog_dbgstr_wn */
static const char *default_dbgstr_wn( const unsigned short *str, int n )
{
	char *dst, *res;
	size_t size;

	if (!((ULONG_PTR)str >> 16))
	{
		if (!str) return "(null)";
		res = funcs.get_temp_buffer( 6 );
		sprintf( res, "#%04x", LOWORD(str) );
		return res;
	}
	if (n == -1)
	{
		const unsigned short *end = str;
		while (*end) end++;
		n = end - str;
	}
	if (n < 0) n = 0;
	size = 12 + min( 300, n * 5 );
	dst = res = funcs.get_temp_buffer( size );
	*dst++ = 'L';
	*dst++ = '"';
	while (n-- > 0 && dst <= res + size - 10)
	{
		unsigned short c = *str++;
		switch (c)
		{
		case '\n': *dst++ = '\\'; *dst++ = 'n'; break;
		case '\r': *dst++ = '\\'; *dst++ = 'r'; break;
		case '\t': *dst++ = '\\'; *dst++ = 't'; break;
		case '"':  *dst++ = '\\'; *dst++ = '"'; break;
		case '\\': *dst++ = '\\'; *dst++ = '\\'; break;
		default:
			if (c >= ' ' && c <= 126)
				*dst++ = (char)(c&0xFF);
			else
			{
				*dst++ = '\\';
				sprintf(dst,"%04x",c);
				dst+=4;
			}
		}
	}
	*dst++ = '"';
	if (n > 0)
	{
		*dst++ = '.';
		*dst++ = '.';
		*dst++ = '.';
	}
	*dst++ = 0;
	funcs.release_temp_buffer( res, dst - res );
	return res;
}



//
#ifdef _WIN32
#include <crtdbg.h>
#endif
/* default implementation of tgmlog_dbg_vprintf */
static int default_dbg_vprintf( const char *format, va_list args )
{
#ifdef _WIN32
	char buf[1024];
	vsprintf( buf, format, args );
	_RPT0(_CRT_WARN, buf);
	return fprintf(stderr,buf);
#else
	return vfprintf( stderr, format, args );
#endif
}


/* default implementation of tgmlog_dbg_vlog */
static int default_dbg_vlog( enum __tgmlog_debug_class cls, struct __tgmlog_debug_channel *channel,
							const char *func, const char *format, va_list args )
{
	int ret = 0;

	//if (cls < sizeof(debug_classes)/sizeof(debug_classes[0]))
	//	ret += tgmlog_dbg_printf( "%s:%s:%s ", debug_classes[cls], channel->name, func );
	char sz_header[1024];
	ret = tgmlog_dbg_header_format_exec(sz_header,sizeof(sz_header) - 1, cls,channel, func);
	tgmlog_dbg_printf("%s",sz_header);

	if (format && format[0])
		ret += funcs.dbg_vprintf( format, args );
	return ret;
}

struct __tgmlog_debug_functions funcs =
{
	get_temp_buffer,
		release_temp_buffer,
		default_dbgstr_an,
		default_dbgstr_wn,
		default_dbg_vprintf,
		default_dbg_vlog
};

void __tgmlog_dbg_set_functions( const struct __tgmlog_debug_functions *new_funcs,
struct __tgmlog_debug_functions *old_funcs, size_t size )
{
	if (old_funcs) memcpy( old_funcs, &funcs, min(sizeof(funcs),size) );
	if (new_funcs) memcpy( &funcs, new_funcs, min(sizeof(funcs),size) );
}

