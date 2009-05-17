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

#ifndef __TGMLOG_TGMLOG_DEBUG_H
#define __TGMLOG_TGMLOG_DEBUG_H

#include <stdarg.h>

#ifdef WIN32

#if defined(TGMLOG_MAKEDLL)
#define TGMLOG_EXPORT  __declspec(dllexport)
#else 		/* use a DLL library */
#define TGMLOG_EXPORT  __declspec(dllimport)
#endif

#elif __linux__

#if defined(TGMLOG_MAKEDLL)
#define TGMLOG_EXPORT   __attribute__((visibility("default")))
#else
#define TGMLOG_EXPORT
#endif

#else
#error unsupportted_os
#endif

#ifdef __cplusplus
extern "C" {
#endif


    /*
    * Internal definitions (do not use these directly)
    */
    enum __tgmlog_debug_class
    {
        __TGMLOG_DBCL_FIXME,	//留待以后修正
        __TGMLOG_DBCL_ERR,		//严重错误
        __TGMLOG_DBCL_WARN,		//警告
        __TGMLOG_DBCL_TRACE,	//运行日志
        __TGMLOG_DBCL_DTRACE,	//运行日志，仅作调试用
        __TGMLOG_DBCL_INIT = 15  /* lazy init flag */
    };

    struct __tgmlog_debug_channel
    {
        unsigned char flags;
        char name[15];
    };



#define __TGMLOG_GET_DEBUGGING_DTRACE(dbch)		((dbch)->flags & (1 << __TGMLOG_DBCL_DTRACE))
#define __TGMLOG_GET_DEBUGGING_TRACE(dbch)		((dbch)->flags & (1 << __TGMLOG_DBCL_TRACE))
#define __TGMLOG_GET_DEBUGGING_WARN(dbch)		((dbch)->flags & (1 << __TGMLOG_DBCL_WARN))
#define __TGMLOG_GET_DEBUGGING_FIXME(dbch)		((dbch)->flags & (1 << __TGMLOG_DBCL_FIXME))
#define __TGMLOG_GET_DEBUGGING_ERR(dbch)		((dbch)->flags & (1 << __TGMLOG_DBCL_ERR))

#define __TGMLOG_GET_DEBUGGING(dbcl,dbch)  __TGMLOG_GET_DEBUGGING##dbcl(dbch)
#define __TGMLOG_IS_DEBUG_ON(dbcl,dbch) \
    (__TGMLOG_GET_DEBUGGING##dbcl(dbch) && (__tgmlog_dbg_get_channel_flags(dbch) & (1 << __TGMLOG_DBCL##dbcl)))

//__TGMLOG_DPRINTF BEGIN////////////////////////////////////////////////////////////////////////
#ifdef __GNUC__

    #define __TGMLOG_DPRINTF(dbcl,dbch) \
        do { if(__TGMLOG_GET_DEBUGGING(dbcl,(dbch))) { \
        struct __tgmlog_debug_channel * const __dbch = (dbch); \
        const enum __tgmlog_debug_class __dbcl = __TGMLOG_DBCL##dbcl; \
        __TGMLOG_DBG_LOG

    #define __TGMLOG_DBG_LOG(args...) \
        tgmlog_dbg_log( __dbcl, __dbch, __FUNCTION__, args); } } while(0)

    #define __TGMLOG_PRINTF_ATTR(fmt,args) __attribute__((format (printf,fmt,args)))

#elif defined(__SUNPRO_C)

    #define __TGMLOG_DPRINTF(dbcl,dbch) \
        do { if(__TGMLOG_GET_DEBUGGING(dbcl,(dbch))) { \
        struct __tgmlog_debug_channel * const __dbch = (dbch); \
        const enum __TGMLOG_DEBUG_CLASS __dbcl = __TGMLOG_DBCL##dbcl; \
        __TGMLOG_DBG_LOG

    #define __TGMLOG_DBG_LOG(...) \
        tgmlog_dbg_log( __dbcl, __dbch, __func__, __VA_ARGS__); } } while(0)

    #define __TGMLOG_PRINTF_ATTR(fmt,args)

#else  /* !__GNUC__ && !__SUNPRO_C */

    #define __TGMLOG_DPRINTF(dbcl,dbch) \
        (!__TGMLOG_GET_DEBUGGING(dbcl,(dbch)) || \
        (tgmlog_dbg_log(__TGMLOG_DBCL##dbcl,(dbch),__FUNCTION__,"","") == -1)) ? \
        (void)0 : (void)tgmlog_dbg_printf

    #define __TGMLOG_PRINTF_ATTR(fmt, args)

#endif  /* !__GNUC__ && !__SUNPRO_C */
//__TGMLOG_DPRINTF  END////////////////////////////////////////////////////////////////////////

#ifdef TGMLOG_NO_TRACE_MSGS
#ifdef __GNUC__
    #define TGMLOG_TRACE(args...)   do { } while(0)
#else
    static inline void TGMLOG_TRACE(const char* fmt,...){}
#endif
#define TGMLOG_TRACE_(ch)       TGMLOG_TRACE
#endif

#ifdef TGMLOG_NO_DEBUG_MSGS
#ifdef __GNUC__
    #define TGMLOG_DTRACE(args...)  do { } while(0)
    #define TGMLOG_WARN(args...)    do { } while(0)
    #define TGMLOG_FIXME(args...)   do { } while(0)
#else
    static inline void TGMLOG_DTRACE(const char* fmt,...){}
    #define TGMLOG_WARN       TGMLOG_DTRACE
    #define TGMLOG_FIXME      TGMLOG_DTRACE
#endif

#define TGMLOG_DTRACE_(ch)      TGMLOG_DTRACE
#define TGMLOG_WARN_(ch)        TGMLOG_WARN
#define TGMLOG_FIXME_(ch)       TGMLOG_FIXME
#endif

    //TGMLOG_EXPORT int tgmlog_dbg_init(const char* appname,unsigned char flags);
    TGMLOG_EXPORT int   tgmlog_dbg_printf( const char *format, ... ) __TGMLOG_PRINTF_ATTR(1,2);
    TGMLOG_EXPORT int   tgmlog_dbg_log( enum __tgmlog_debug_class cls, struct __tgmlog_debug_channel *ch, const char *func,
        const char *format, ... ) __TGMLOG_PRINTF_ATTR(4,5);
    TGMLOG_EXPORT int   tgmlog_dbg_assert_printf( const char *format, ... ) __TGMLOG_PRINTF_ATTR(1,2);



#ifndef TGMLOG_DTRACE
#define TGMLOG_DTRACE               __TGMLOG_DPRINTF(_DTRACE,__tgmlog_dbch___default)
#define TGMLOG_DTRACE_(ch)          __TGMLOG_DPRINTF(_DTRACE,&__tgmlog_dbch_##ch)
#endif
#define TGMLOG_DTRACE_ON(ch)        __TGMLOG_IS_DEBUG_ON(_DTRACE,&__tgmlog_dbch_##ch)

#ifndef TGMLOG_TRACE
#define TGMLOG_TRACE                __TGMLOG_DPRINTF(_TRACE,__tgmlog_dbch___default)
#define TGMLOG_TRACE_(ch)           __TGMLOG_DPRINTF(_TRACE,&__tgmlog_dbch_##ch)
#endif
#define TGMLOG_TRACE_ON(ch)         __TGMLOG_IS_DEBUG_ON(_TRACE,&__tgmlog_dbch_##ch)

#ifndef TGMLOG_WARN
#define TGMLOG_WARN                 __TGMLOG_DPRINTF(_WARN,__tgmlog_dbch___default)
#define TGMLOG_WARN_(ch)            __TGMLOG_DPRINTF(_WARN,&__tgmlog_dbch_##ch)
#endif
#define TGMLOG_WARN_ON(ch)          __TGMLOG_IS_DEBUG_ON(_WARN,&__tgmlog_dbch_##ch)

#ifndef TGMLOG_FIXME
#define TGMLOG_FIXME                __TGMLOG_DPRINTF(_FIXME,__tgmlog_dbch___default)
#define TGMLOG_FIXME_(ch)           __TGMLOG_DPRINTF(_FIXME,&__tgmlog_dbch_##ch)
#endif
#define TGMLOG_FIXME_ON(ch)         __TGMLOG_IS_DEBUG_ON(_FIXME,&__tgmlog_dbch_##ch)

#define TGMLOG_ERR                  __TGMLOG_DPRINTF(_ERR,__tgmlog_dbch___default)
#define TGMLOG_ERR_(ch)             __TGMLOG_DPRINTF(_ERR,&__tgmlog_dbch_##ch)
#define TGMLOG_ERR_ON(ch)           __TGMLOG_IS_DEBUG_ON(_ERR,&__tgmlog_dbch_##ch)

#define TGMLOG_COND_EXEC(cond,sentence)	do {if (cond) {sentence ; } }while(0)

    //example TGMLOG_ASSERT(a>0)
#define TGMLOG_ASSERT_C(cond)       TGMLOG_COND_EXEC(!(cond),tgmlog_dbg_assert_printf("ASSERT(%s),%s,%d\n",#cond,__FILE__,__LINE__))

    //example TGMLOG_ASSERT_M(a>0,("%d%d",1,1))
#define TGMLOG_ASSERT_M(cond,mesg)	do {if (!(cond)){\
    tgmlog_dbg_printf("ASSERT(%s),%s,%d  ",#cond,__FILE__,__LINE__);\
    tgmlog_dbg_assert_printf mesg ;\
}}while(0)

#define TGMLOG_DPRINTF                  tgmlog_dbg_printf
#define TGMLOG_MESSAGE                  tgmlog_dbg_printf
#define TGMLOG_QUOTE(XXX)			    XXX
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

#define TGMLOG_DECLARE_DEBUG_CHANNEL(ch) \
    static struct __tgmlog_debug_channel __tgmlog_dbch_##ch = { ~0, #ch }
#define TGMLOG_DEFAULT_DEBUG_CHANNEL(ch) \
    static struct __tgmlog_debug_channel __tgmlog_dbch_##ch = { ~0, #ch }; \
    static struct __tgmlog_debug_channel * const __tgmlog_dbch___default = &__tgmlog_dbch_##ch


#ifndef TGMLOG_DISABL_ABBREVIATE

#ifndef DTRACE//DEBUG TRACE 
#define DTRACE						TGMLOG_DTRACE
#define DTRACE_(ch)					TGMLOG_DTRACE_(ch)
#define DTRACE_ON(ch)				TGMLOG_DTRACE_ON(ch)

#define DTRACE_FBEN					TGMLOG_DTRACE_FUNC_BEN
#define DTRACE						TGMLOG_DTRACE
#endif

#ifndef TRACE
#define TRACE						TGMLOG_TRACE
#define TRACE_(ch)					TGMLOG_TRACE_(ch)
#define TRACE_ON(ch)				TGMLOG_TRACE_ON(ch)
#endif

#ifndef WARN
#define WARN						TGMLOG_WARN
#define WARN_(ch)					TGMLOG_WARN_(ch)
#define WARN_ON(ch)					TGMLOG_WARN_ON(ch)
#endif


#ifndef FIXME
#define FIXME						TGMLOG_FIXME
#define FIXME_(ch)					TGMLOG_FIXME_(ch)
#define FIXME_ON(ch)				TGMLOG_FIXME_ON(ch)
#endif

#undef	ERR  /* Solaris got an 'ERR' define in <sys/reg.h> */
#define ERR							TGMLOG_ERR
#define ERR_(ch)					TGMLOG_ERR_(ch)
#define ERR_ON(ch)					TGMLOG_ERR_ON(ch)

#define ASSERT_C					TGMLOG_ASSERT_C
#define ASSERT_M					TGMLOG_ASSERT_M
#define COND_EXEC					TGMLOG_COND_EXEC

#endif//TGMLOG_DISABL_ABBREVIATE


#ifdef __cplusplus
}
#endif


#include "tgmlog_func_trace.h"


#ifndef TGMLOG_DISABLE_SUP_UTIL
#include "tgmlog_util.h"
#endif


/*example 
#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(test);

int testfunc(int a,int b )//computer a/b,
{
    DTRACE("a=%d,b=%d\n",a,b);
    COND_EXEC(b==0,ERR("b is not be zero\n"));
    ASSERT_C(b!=0);
    ASSERT_M(b!=0,("b(%d) is not be zero\n",b));
    
    int c = a / b;
    TRACE("a=%d,b=%d,ret=%d\n",a,b,c);
    return c;
}
*/
#endif  /* __TGMLOG_TGMLOG_DEBUG_H */

