/*
* Management of the debugging channels
*
* Copyright 2000 Alexandre Julliard
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

#include "tgmlog_common.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define MAX_DEBUG_OPTIONS 256

#ifdef DEBUG_VERSE
static unsigned char	default_flags = 15;//(1 << __TGMLOG_DBCL_ERR) | (1 << __TGMLOG_DBCL_FIXME);
#else
static unsigned char	default_flags = (1 << __TGMLOG_DBCL_ERR) | (1 << __TGMLOG_DBCL_FIXME) | (1 << __TGMLOG_DBCL_WARN);
#endif
#ifdef WIN32
static const char*		default_header_format = "NcCF";//默认日志头输出格式 debug_usage
#else
static const char*		default_header_format = "NcCF";//默认日志头输出格式 debug_usage
#endif

/* print the usage message */
static void debug_usage(void)
{
    static const char usage[] =
        "---------------------------------------------------------\n"
        "Syntax of the TGMLOGDEBUG variable:\n"
        "	TGMLOGDEBUG=[class]+xxx,[class]-yyy,...\n\n"
        "Example: TGMLOGDEBUG=+all,warn-heap\n"
        "	turns on all messages except warning heap messages\n"
        "Available message classes: err, warn, fixme, trace,debug\n"
        "---------------------------------------------------------\n"
        "Syntax of the TGMLOGDEBUG_FORMAT variable:\n"
        "	TGMLOGDEBUG_FORMAT=[d][t][N][P][T][c][C][F]\n\n"
        "N process-name,P pid,T tid,t tim,d date,C channel,c class,F func,f funcTrace\n"
        "Example: TGMLOGDEBUG_FORMAT=NPcC\n"
        "	the log header prefix should is [process-name][pid][class][channel]\n"
        "---------------------------------------------------------\n";

    fprintf( stderr, usage);
    exit(1);
}

static int nb_debug_options = -1;
static struct __tgmlog_debug_channel debug_options[MAX_DEBUG_OPTIONS];

//static struct __tgmlog_debug_functions funcs;

static void debug_init(void);

static int cmp_name( const void *p1, const void *p2 )
{
	const char *name = p1;
	const struct __tgmlog_debug_channel *chan = p2;
	return strcmp( name, chan->name );
}

/* get the flags to use for a given channel, possibly setting them too in case of lazy init */
unsigned char __tgmlog_dbg_get_channel_flags( struct __tgmlog_debug_channel *channel )
{
	if (nb_debug_options == -1) debug_init();

	if (nb_debug_options)
	{
		struct __tgmlog_debug_channel *opt = bsearch( channel->name, debug_options, nb_debug_options,
			sizeof(debug_options[0]), cmp_name );
		if (opt) return opt->flags;
	}
	/* no option for this channel */
	if (channel->flags & (1 << __TGMLOG_DBCL_INIT)) channel->flags = default_flags;
	return default_flags;
}

/* set the flags to use for a given channel; return 0 if the channel is not available to set */
int __tgmlog_dbg_set_channel_flags( struct __tgmlog_debug_channel *channel,
								 unsigned char set, unsigned char clear )
{
	if (nb_debug_options == -1) debug_init();

	if (nb_debug_options)
	{
		struct __tgmlog_debug_channel *opt = bsearch( channel->name, debug_options, nb_debug_options,
			sizeof(debug_options[0]), cmp_name );
		if (opt)
		{
			opt->flags = (opt->flags & ~clear) | set;
			return 1;
		}
	}
	return 0;
}

/* add a new debug option at the end of the option list */
static void add_option( const char *name, unsigned char set, unsigned char clear )
{
	int min = 0, max = nb_debug_options - 1, pos, res;

	if (!name[0])  /* "all" option */
	{
		default_flags = (default_flags & ~clear) | set;
		return;
	}
	if (strlen(name) >= sizeof(debug_options[0].name)) return;

	while (min <= max)
	{
		pos = (min + max) / 2;
		res = strcmp( name, debug_options[pos].name );
		if (!res)
		{
			debug_options[pos].flags = (debug_options[pos].flags & ~clear) | set;
			return;
		}
		if (res < 0) max = pos - 1;
		else min = pos + 1;
	}
	if (nb_debug_options >= MAX_DEBUG_OPTIONS) return;

	pos = min;
	if (pos < nb_debug_options) memmove( &debug_options[pos + 1], &debug_options[pos],
		(nb_debug_options - pos) * sizeof(debug_options[0]) );
	strcpy( debug_options[pos].name, name );
	debug_options[pos].flags = (default_flags & ~clear) | set;
	nb_debug_options++;
}

/* parse a set of debugging option specifications and add them to the option list */
static void parse_options( const char *str )
{
	char *opt, *next, *options;
	unsigned int i;

	if (!(options = strdup(str))) return;
	for (opt = options; opt; opt = next)
	{
		const char *p;
		unsigned char set = 0, clear = 0;

		if ((next = strchr( opt, ',' ))) *next++ = 0;

		p = opt + strcspn( opt, "+-" );
		if (!p[0]) p = opt;  /* assume it's a debug channel name */

		if (p > opt)
		{
			for (i = 0; i < sizeof(debug_classes)/sizeof(debug_classes[0]); i++)
			{
				int len = strlen(debug_classes[i]);
				if (len != (p - opt)) continue;
				if (!memcmp( opt, debug_classes[i], len ))  /* found it */
				{
					if (*p == '+') set |= 1 << i;
					else clear |= 1 << i;
					break;
				}
			}
			if (i == sizeof(debug_classes)/sizeof(debug_classes[0])) /* bad class name, skip it */
				continue;
		}
		else
		{
			if (*p == '-') clear = ~0;
			else set = ~0;
		}
		if (*p == '+' || *p == '-') p++;
		if (!p[0]) continue;

		if (!strcmp( p, "all" ))
			default_flags = (default_flags & ~clear) | set;
		else
			add_option( p, set, clear );
	}
	free( options );
}





/* initialize all options at startup */
static void debug_init(void)
{
	char *tgmlog_debug;
	char *tgmlog_conf_file;
	
	//env TGMLOGDEBUG
	if (nb_debug_options != -1) return;  /* already initialized */
	nb_debug_options = 0;
	if ((tgmlog_debug = getenv("TGMLOGDEBUG")))
	{
		if (!strcmp( tgmlog_debug, "help" )) debug_usage();
		parse_options( tgmlog_debug );
	}
	
	//env TGMLOGDEBUG_FORMAT
	tgmlog_dbg_header_format_set(getenv("TGMLOGDEBUG_FORMAT")?getenv("TGMLOGDEBUG_FORMAT"):default_header_format);

	//env TGMLOGDEBUG_CONFFILE
	if ((tgmlog_conf_file = getenv("TGMLOGDEBUG")))
	{
		//todo ...
		//parse_options_file( tgmlog_conf_file );
	}

}

/* varargs wrapper for funcs.dbg_vprintf */
int tgmlog_dbg_printf( const char *format, ... )
{
	int ret;
	va_list valist;

	va_start(valist, format);
	ret = funcs.dbg_vprintf( format, valist );
	va_end(valist);
	return ret;
}



int tgmlog_dbg_assert_printf( const char *format, ... )
{
	int ret;
	va_list valist;

	va_start(valist, format);
	ret = funcs.dbg_vprintf( format, valist );
	va_end(valist);

//#if !defined(_AFX_NO_DEBUG_CRT) && defined(__Win32__)	&& defined(_TRANZDA_VM_)
//	::MessageBoxA(NULL, szMsg, "警告", MB_OK);
//#endif

#if WIN32
	DebugBreak();
#else
	(*(long*)0) = 0;
#endif

	return ret;
}


/* varargs wrapper for funcs.dbg_vlog */
int tgmlog_dbg_log( enum __tgmlog_debug_class cls, struct __tgmlog_debug_channel *channel,
				 const char *func, const char *format, ... )
{
	int ret;
	va_list valist;

	if (!(__tgmlog_dbg_get_channel_flags( channel ) & (1 << cls))) return -1;

	va_start(valist, format);
	ret = funcs.dbg_vlog( cls, channel, func, format, valist );
	va_end(valist);
	return ret;
}





