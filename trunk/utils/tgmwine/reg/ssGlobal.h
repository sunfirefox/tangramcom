/**************************************************************************/
/*                                                                        */
/* Copyright (c) 2005 Tranzda Technologies Co.,Ltd.                       */
/*                    深圳市全智达科技有限公司  版权所有 2005             */
/*                                                                        */
/*  PROPRIETARY RIGHTS of Tranzda Technologies Co.,Ltd. are involved in   */
/*  the subject matter of this material. All manufacturing, reproduction, */
/*  use, and sales rights pertaining to this subject matter are governed  */
/*  by the license agreement.  The recipient of this software implicitly  */ 
/*  accepts the terms of the license.                                     */
/*  本软件文档资料是深圳市全智达科技有限公司的合法资产，任何人士阅读和使  */
/*  用本资料必须获得相应的书面授权,承担保密责任和接受相应的法律约束。     */
/*                                                                        */
/**************************************************************************/

#ifndef __SS_GLOBAL_H__
#define __SS_GLOBAL_H__

#ifdef _WIN32

    #include <windows.h>

#else

    #include <pthread.h>
    #include <sys/types.h>

#if !(defined(DWORD_DEFINE) || defined(DWORD))
#define DWORD_DEFINE
    typedef unsigned long       DWORD;
	typedef int                 BOOL;
	typedef unsigned char       BYTE;
	typedef unsigned short      WORD;
	typedef float               FLOAT;
	typedef int                 INT;
	typedef unsigned int        UINT;
	typedef unsigned int        *PUINT;
#endif

#endif

#include "ssTypes.h"

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef _WIN32
	typedef DWORD       SS_PID;
	typedef DWORD       SS_TID;
	typedef HANDLE      SS_MSGHANDLE;
    typedef HMODULE     DLL_HANDLE;
#define SS_MSGHANDLE_NULL	NULL
	typedef  LPTHREAD_START_ROUTINE	THREAD_ROUTINE_ENTRY;
#else
	typedef pid_t       SS_PID;
	typedef pthread_t   SS_TID;
	typedef int         SS_MSGHANDLE;
    typedef void *      DLL_HANDLE;
#define SS_MSGHANDLE_NULL	-1
	typedef void * (*THREAD_ROUTINE_ENTRY)(void *);
#endif

    //定义全局线程唯一ID类型
    typedef struct
    {
        SS_PID  pid;
        SS_TID  tid;
    }SS_GTID;

#ifdef __cplusplus
    }
#endif

#endif  //__SS_GLOBAL_H__


