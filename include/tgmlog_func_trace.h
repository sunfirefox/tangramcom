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

#ifndef TGMLOG_FUNC_TRACE_HHHH
#define TGMLOG_FUNC_TRACE_HHHH

#ifdef __cplusplus
extern "C" {
#endif

TGMLOG_EXPORT int   tgmlog_func_trace_enter();
TGMLOG_EXPORT void  tgmlog_func_trace_exit();
TGMLOG_EXPORT int   tgmlog_func_trace_begin_printf( const char *format, ... );
TGMLOG_EXPORT int   tgmlog_func_trace_end_printf( const char *format, ... );

#ifdef __cplusplus
}
#endif


//__TGMLOG_DPRINTF BEGIN////////////////////////////////////////////////////////////////////////
#ifdef __GNUC__

#define __TGMLOG_DPRINTF_FUNC_BEN(dbcl,dbch) \
    do {tgmlog_func_trace_enter();\
    if(__TGMLOG_GET_DEBUGGING(dbcl,(dbch))) { \
    struct __tgmlog_debug_channel * const __dbch = (dbch); \
    const enum __tgmlog_debug_class __dbcl = __TGMLOG_DBCL##dbcl; \
    __TGMLOG_DBG_LOG_FUNC_BEN

#define __TGMLOG_DBG_LOG_FUNC_BEN(args...) \
    tgmlog_dbg_log( __dbcl, __dbch, __FUNCTION__, args); } } while(0)

#define __TGMLOG_DPRINTF_FUNC_END(dbcl,dbch) \
    do { if(__TGMLOG_GET_DEBUGGING(dbcl,(dbch))) { \
    struct __tgmlog_debug_channel * const __dbch = (dbch); \
    const enum __tgmlog_debug_class __dbcl = __TGMLOG_DBCL##dbcl; \
    __TGMLOG_DBG_LOG_FUNC_END

#define __TGMLOG_DBG_LOG_FUNC_END(args...) \
    tgmlog_dbg_log( __dbcl, __dbch, __FUNCTION__, args); } \
    tgmlog_func_trace_exit();} while(0)


#define __TGMLOG_PRINTF_ATTR(fmt,args) __attribute__((format (printf,fmt,args)))

#elif defined(__SUNPRO_C)

#define __TGMLOG_DPRINTF_FUNC_BEN(dbcl,dbch) \
    do { tgmlog_func_trace_enter();\
    if(__TGMLOG_GET_DEBUGGING(dbcl,(dbch))) { \
    struct __tgmlog_debug_channel * const __dbch = (dbch); \
    const enum __TGMLOG_DEBUG_CLASS __dbcl = __TGMLOG_DBCL##dbcl; \
    __TGMLOG_DBG_LOG_FUNC_BEN

#define __TGMLOG_DBG_LOG_FUNC_BEN(...) \
    tgmlog_dbg_log( __dbcl, __dbch, __func__, __VA_ARGS__); } } while(0)

#define __TGMLOG_DPRINTF_FUNC_END(dbcl,dbch) \
    do { if(__TGMLOG_GET_DEBUGGING(dbcl,(dbch))) { \
    struct __tgmlog_debug_channel * const __dbch = (dbch); \
    const enum __TGMLOG_DEBUG_CLASS __dbcl = __TGMLOG_DBCL##dbcl; \
    __TGMLOG_DBG_LOG_FUNC_END

#define __TGMLOG_DBG_LOG_FUNC_END(...) \
    tgmlog_dbg_log( __dbcl, __dbch, __func__, __VA_ARGS__); } \
    tgmlog_func_trace_exit();} while(0)

#define __TGMLOG_PRINTF_ATTR(fmt,args)

#else  /* WIN32 */

#define __TGMLOG_DPRINTF_FUNC_BEN(dbcl,dbch) \
    (!__TGMLOG_GET_DEBUGGING(dbcl,(dbch)) || \
    (tgmlog_dbg_log(__TGMLOG_DBCL##dbcl,(dbch),__FUNCTION__,"","") == -1)) ? \
    tgmlog_func_trace_enter(): (void)tgmlog_func_trace_begin_printf

#define __TGMLOG_DPRINTF_FUNC_END(dbcl,dbch) \
    (!__TGMLOG_GET_DEBUGGING(dbcl,(dbch)) || \
    (tgmlog_dbg_log(__TGMLOG_DBCL##dbcl,(dbch),__FUNCTION__,"","") == -1)) ? \
    tgmlog_func_trace_exit() : (void)tgmlog_func_trace_end_printf

#define __TGMLOG_PRINTF_ATTR(fmt, args)

#endif  
//__TGMLOG_DPRINTF  END////////////////////////////////////////////////////////////////////////

#ifndef TGMLOG_DISABLE_SUP_FUNC_TRACE
#define TGMLOG_DTRACE_FUNC_BEN      __TGMLOG_DPRINTF_FUNC_BEN(_DTRACE,__tgmlog_dbch___default)
#define TGMLOG_DTRACE_FUNC_END      __TGMLOG_DPRINTF_FUNC_END(_DTRACE,__tgmlog_dbch___default)
#define TGMLOG_TRACE_FUNC_BEN       __TGMLOG_DPRINTF_FUNC_BEN(_TRACE,__tgmlog_dbch___default)
#define TGMLOG_TRACE_FUNC_END       __TGMLOG_DPRINTF_FUNC_END(_TRACE,__tgmlog_dbch___default)
#define TGMLOG_WARN_FUNC_BEN        __TGMLOG_DPRINTF_FUNC_BEN(_WARN,__tgmlog_dbch___default)
#define TGMLOG_WARN_FUNC_END        __TGMLOG_DPRINTF_FUNC_END(_WARN,__tgmlog_dbch___default)
#define TGMLOG_FIXME_FUNC_BEN       __TGMLOG_DPRINTF_FUNC_BEN(_FIXME,__tgmlog_dbch___default)
#define TGMLOG_FIXME_FUNC_END       __TGMLOG_DPRINTF_FUNC_END(_FIXME,__tgmlog_dbch___default)
#define TGMLOG_ERR_FUNC_BEN         __TGMLOG_DPRINTF_FUNC_BEN(_ERR,__tgmlog_dbch___default)
#define TGMLOG_ERR_FUNC_END         __TGMLOG_DPRINTF_FUNC_END(_ERR,__tgmlog_dbch___default)

#else   //TGMLOG_DISABLE_SUP_FUNC_TRACE

#define TGMLOG_DTRACE_FUNC_BEN      TGMLOG_DTRACE
#define TGMLOG_DTRACE_FUNC_END      TGMLOG_DTRACE
#define TGMLOG_TRACE_FUNC_BEN       TGMLOG_TRACE
#define TGMLOG_TRACE_FUNC_END       TGMLOG_TRACE
#define TGMLOG_WARN_FUNC_BEN        TGMLOG_WARN
#define TGMLOG_WARN_FUNC_END        TGMLOG_WARN
#define TGMLOG_FIXME_FUNC_BEN       TGMLOG_FIXME
#define TGMLOG_FIXME_FUNC_END       TGMLOG_FIXME
#define TGMLOG_ERR_FUNC_BEN         TGMLOG_ERR
#define TGMLOG_ERR_FUNC_END         TGMLOG_ERR 

#endif //TGMLOG_DISABLE_SUP_FUNC_TRACE

#ifndef TGMLOG_DISABL_ABBREVIATE

#define FB_DTRACE                   TGMLOG_DTRACE_FUNC_BEN
#define FE_DTRACE                   TGMLOG_DTRACE_FUNC_END
#define FB_TRACE                    TGMLOG_TRACE_FUNC_BEN       
#define FE_TRACE                    TGMLOG_TRACE_FUNC_END       
#define FB_WARN                     TGMLOG_WARN_FUNC_BEN        
#define FE_WARN                     TGMLOG_WARN_FUNC_END        
#define FB_FIXME                    TGMLOG_FIXME_FUNC_BEN       
#define FE_FIXME                    TGMLOG_FIXME_FUNC_END       
#define FB_ERR                      TGMLOG_ERR_FUNC_BEN         
#define FE_ERR                      TGMLOG_ERR_FUNC_END   

#endif// TGMLOG_DISABL_ABBREVIATE

#endif //#ifndef TGMLOG_FUNC_TRACE_HHHH


