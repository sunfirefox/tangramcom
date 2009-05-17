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

#include "tgmlog_common.h"

//char		__tgmlog_debug_szProcessName[256] = "unknown";;

#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <time.h>

#ifdef WIN32
#include <windows.h>
#include "psapi.h"
#define INLINE __inline
#else
#include <sys/types.h>  
#include <unistd.h>  
#include <pthread.h>
#define INLINE inline 
#endif


static INLINE int	tgmlog_dbg_get_process_name(char * processname,int size)
{
#ifdef WIN32
	int len;
	HMODULE hMod;
	DWORD cbNeeded;
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,	FALSE, GetCurrentProcessId() );
	if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
		&cbNeeded) )
	{
		GetModuleBaseName( hProcess, hMod, processname,size);
		len = strlen(processname);
		while(--len >0){
			if('.' == processname[len]){
				processname[len] = 0;
				break;
			}
		}
	}
#else
	//todo... need optimize
	FILE   *stream = NULL;
	char	szcmd[64];
	int		len;
//#if 0
//	sprintf(szcmd,"ps -p %d -o args=",getpid());
//	stream = popen( szcmd, "r"); 
//	len = fread( processname, 1, size,  stream);
//	pclose(stream);
//#else
	sprintf(szcmd,"/proc/%d/cmdline",getpid());
	stream = fopen( szcmd, "r"); 
	len = fread( processname, 1, size,  stream);
	fclose(stream);
//#endif

	if(len >0 && processname[len-1] == '\n'){
		processname[--len] = 0;
	}

	while(--len >0){
		if('/' == processname[len]){
			strcpy(processname,processname+len+1);
			break;
		}
	}
#endif
	return strlen(processname);
}

static INLINE ULONG	tgmlog_dbg_get_process_id()
{
#ifdef WIN32
	return GetCurrentProcessId();
#else
	return getpid();
#endif
}

static INLINE ULONG	tgmlog_dbg_get_thread_id()
{
#ifdef WIN32
	return GetCurrentThreadId();
#else
	return (ULONG)pthread_self();
#endif
}



//////////////////////////////////////////////////////////////////////////
/*
N name
P pid
T tid
t time
d date
[d][t][N][P][T]
*/
static char sz_header_fix[256]	= {0};
static char sz_header_fmt[32]	= {0};
void tgmlog_dbg_header_format_set(const char* fmt)
{
	char f;
	char* p = sz_header_fix;
	char* m = sz_header_fmt;
	while((f =*fmt++) != 0){
		switch(f){
		case 'N':
			p += tgmlog_dbg_get_process_name(p,sizeof(sz_header_fix) - (p - sz_header_fix));
			*m++ = f;
			break;

        case 'P':
			p += sprintf(p,"(%lu)",(unsigned long)tgmlog_dbg_get_process_id());
			break;
        
		case 'T':
		case 't':
		case 'd':
		case 'c':
		case 'C':
		case 'F':
            *m++ = f;
            break;
#ifdef TGMLOG_SUP_FUNC_TRACE
        case 'f':
            tgmlog_func_trace_init();
            *m++ = f;
            break;
#endif
	
		}
	}
	*m = 0;
}

int	tgmlog_dbg_header_format_exec(char* header,int size,
								  enum __tgmlog_debug_class cls,struct __tgmlog_debug_channel *channel,const char *func)
{
	time_t tTime;
	struct tm* pTime = NULL;

	char * fmt = sz_header_fmt;
	char f;
    char * h = header;
	while((f =*fmt++) != 0){
		switch(f){
		case 'N':
			h += SNPRINTF(h,size-(h - header),"[%s]",sz_header_fix);
			break;
		case 'T':
			h += SNPRINTF(h,size-(h - header),"[%08X]",(unsigned long)tgmlog_dbg_get_thread_id());
			break;
		case 't':
			if(!pTime){
				time(&tTime);
				pTime = localtime(&tTime);
			}
			if(pTime){
				h += SNPRINTF(h,size-(h - header),"[%02d:%02d:%02d]",pTime->tm_hour, pTime->tm_min, pTime->tm_sec);
			}
			break;
		case 'd':
			if(!pTime){
				time(&tTime);
				pTime = localtime(&tTime);
			}
			if(pTime){
				h += SNPRINTF(h,size-(h - header),"[%02d/%02d/%02d]",pTime->tm_year%100, pTime->tm_mon, pTime->tm_mday);
			}
			break;
		case 'c':
			if (cls < sizeof(debug_classes)/sizeof(debug_classes[0])){
				h += SNPRINTF(h,size-(h - header),"[%-5s]",debug_classes[cls]);
			}
			break;
		case 'C':
			if (channel){
				h += SNPRINTF(h,size-(h - header),"[%-6s]",channel->name);
			}
			break;
		case 'F':
			if (func){
				h += SNPRINTF(h,size-(h - header),"[%s]",func);
			}
            break;
#ifdef TGMLOG_SUP_FUNC_TRACE
        case 'f':
            {
                tgm_tls_func_t * fun_tls = tgmlog_func_trace_level();
                strncpy(h,fun_tls->sz,fun_tls->level);
                h += fun_tls->level;
            }
			break;
#endif
		}
	}
	*h = 0;
	return h - header;
}


