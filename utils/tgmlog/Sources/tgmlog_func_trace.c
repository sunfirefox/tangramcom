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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//////////////////////////////////////////////////////////////////////////

#ifndef WIN32
#define __PTHREADS__ 1
#include <pthread.h>
#endif


#ifdef WIN32
DWORD   sThreadStorageIndex = 0;
#elif __PTHREADS__
pthread_key_t sThreadStorageKey = 0;
#endif

static int tgmlog_func_trace_sup = 0;

void tgmlog_func_trace_init()
{
    if(tgmlog_func_trace_sup) 
        return;

    tgmlog_func_trace_sup = 1;
#ifdef WIN32
    sThreadStorageIndex = TlsAlloc();
    //Assert(sThreadStorageIndex >= 0);
#elif __PTHREADS__
    pthread_key_create(&sThreadStorageKey, NULL);
#endif

}

static void tgmlog_tls_set(void * data){
#ifdef WIN32
    BOOL theErr = TlsSetValue(sThreadStorageIndex, data);
    //Assert(theErr == TRUE);
#elif __PTHREADS__
    pthread_setspecific(sThreadStorageKey, data);
#else
    #error sadf
#endif
}

static void*  tgmlog_tls_get()
{
#ifdef WIN32
    return (void *)TlsGetValue(sThreadStorageIndex);
#elif __PTHREADS__
    return (void *)pthread_getspecific(sThreadStorageKey);
#else
    return NULL;
#endif
}



int tgmlog_func_trace_enter()
{
    tgm_tls_func_t * tls;
    
    if(!tgmlog_func_trace_sup) 
        return 0;
    
    tls = (tgm_tls_func_t*)tgmlog_tls_get();

    if(!tls){
        tls = (tgm_tls_func_t*)malloc(sizeof(tgm_tls_func_t));
        memset(tls,0,64);
        tgmlog_tls_set(tls);
    }

    //tls->sz[tls->level++]=tls->level%20/2+'0';
    tls->sz[tls->level++]='\t';
    return tls->level;

}

void tgmlog_func_trace_exit()
{
    tgm_tls_func_t * tls;

    if(!tgmlog_func_trace_sup) 
        return ;
    
    tls = (tgm_tls_func_t*)tgmlog_tls_get();

    /* if(0 != strcmp(func_name,tls->name[tls->level])){
    assert(0);
    }*/

    assert(tls->level >0 );
    //tls->name[tls->level]= NULL;
    //tls->sz[tls->level--]='\0';
    tls->sz[tls->level--]='\0';
}

tgm_tls_func_t* tgmlog_func_trace_level()
{
    tgm_tls_func_t * tls;

    if(!tgmlog_func_trace_sup) 
        return NULL;
    
    tls = (tgm_tls_func_t*)tgmlog_tls_get();
    if(!tls){
        tls = (tgm_tls_func_t*)malloc(sizeof(tgm_tls_func_t));
        memset(tls,0,64);
        tgmlog_tls_set(tls);
    }
    return tls;
}

/* varargs wrapper for funcs.dbg_vprintf */
int tgmlog_func_trace_end_printf( const char *format, ... )
{
    int ret;
    va_list valist;

    va_start(valist, format);
    ret = funcs.dbg_vprintf( format, valist );
    va_end(valist);

    tgmlog_func_trace_exit();
    return ret;
}

/* varargs wrapper for funcs.dbg_vprintf */
int tgmlog_func_trace_begin_printf( const char *format, ... )
{
    int ret;
    va_list valist;

    va_start(valist, format);
    ret = funcs.dbg_vprintf( format, valist );
    va_end(valist);

    tgmlog_func_trace_enter();
    return ret;
}

//#endif// TGMLOG_SUP_FUNC_TRACE
