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

#ifndef TGMLOG_COMMON_HHH
#define TGMLOG_COMMON_HHH

#include <stdio.h>
#include <stdarg.h>

#ifdef _WIN32
#include <time.h>
#define SNPRINTF	_snprintf	
#define VSNPRINTF	_vsnprintf
#else
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#define SNPRINTF	snprintf	
#define VSNPRINTF	vsnprintf
#endif

#include "tgmlog.h"
#include "tgmlog_util.h"

extern struct	__tgmlog_debug_functions funcs;

static const char * const debug_classes[] = { "fixme", "err", "warn", "trace","debug" };


/*
N name
P pid
T tid
t time
d date
C channel
c class
F func
[d][t][N][P][T][c][C][F]
*/
void			tgmlog_dbg_header_format_set(	const char* fmt);
int				tgmlog_dbg_header_format_exec(	char* header,int size,
												enum __tgmlog_debug_class cls,struct __tgmlog_debug_channel *channel,const char *func);

#ifndef UNUSED
#define UNUSED(x) ( void )(x)
#endif


#define TGMLOG_SUP_FUNC_TRACE
#ifdef  TGMLOG_SUP_FUNC_TRACE

void    tgmlog_func_trace_init();
int     tgmlog_func_trace_enter();
void    tgmlog_func_trace_exit();

typedef struct tgm_tls_func_t{
    int  level;
    char sz[64];
    //const char *name[64];
}tgm_tls_func_t;

tgm_tls_func_t* tgmlog_func_trace_level();

#endif //#ifdef TGMLOG_SUP_FUNC_TRACE


#endif//TGMLOG_COMMON_HHH

