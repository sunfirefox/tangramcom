/*#!perl
MapHeaderToDll("objbase.h", "ole32.dll");
ActivateAroundFunctionCall("ole32.dll");
#IgnoreFunction("CreateDataAdviseHolder"); # this function occurs in ole2.h and objbase.h
                                           # The wrapped one is in objbase.h
IgnoreFunction("CoBuildVersion"); # deprecated
IgnoreFunction("CoGetCurrentProcess"); # never fails => hard to wrap well
IgnoreFunction("CoAddRefServerProcess"); # never fails => hard to wrap well
IgnoreFunction("CoReleaseServerProcess"); # never fails => hard to wrap well
IgnoreFunction("DebugCoGetRpcFault"); # not documented
IgnoreFunction("DebugCoSetRpcFault"); # not documented
IgnoreFunction("wIsEqualGUID");
DeclareFunctionErrorValue("CoLoadLibrary", "NULL");
DeclareFunctionErrorValue("StringFromGUID2" , "0");
DeclareFunctionErrorValue("CoTaskMemAlloc", "NULL");
DeclareFunctionErrorValue("CoTaskMemRealloc", "NULL");
IgnoreFunction("DllGetClassObject"); # client function prototyped (like WinMain)
IgnoreFunction("DllCanUnloadNow"); # client function prototyped (like WinMain)
*/

//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (c) Microsoft Corporation. All rights reserved.
//
//  File:       objbase.h
//
//  Contents:   Component object model defintions.
//
//----------------------------------------------------------------------------

//#include "windef.h"
#include <rpc.h>
#include <rpcndr.h>

#if !defined( _OBJBASE_H_ )
#define _OBJBASE_H_

#if _MSC_VER > 1000
#pragma once
#endif

//#include <pshpack8.h>

#ifdef _MAC
#ifndef _WLM_NOFORCE_LIBS

#ifdef _WLMDLL
        #ifdef _DEBUG
                #pragma comment(lib, "oledlgd.lib")
                #pragma comment(lib, "msvcoled.lib")
        #else
                #pragma comment(lib, "oledlg.lib")
                #pragma comment(lib, "msvcole.lib")
        #endif
#else
        #ifdef _DEBUG
                #pragma comment(lib, "wlmoled.lib")
                #pragma comment(lib, "ole2uid.lib")
        #else
                #pragma comment(lib, "wlmole.lib")
                #pragma comment(lib, "ole2ui.lib")
        #endif
        #pragma data_seg(".drectve")
        static char _gszWlmOLEUIResourceDirective[] = "/macres:ole2ui.rsc";
        #pragma data_seg()
#endif

#pragma comment(lib, "uuid.lib")

#ifdef _DEBUG
    #pragma comment(lib, "ole2d.lib")
    #pragma comment(lib, "ole2autd.lib")
#else
    #pragma comment(lib, "ole2.lib")
    #pragma comment(lib, "ole2auto.lib")
#endif

#endif // !_WLM_NOFORCE_LIBS
#endif // _MAC

#if defined(__MSC_VER) || defined(__MINGW32__) || defined(__CYGWIN__)
# define DECLSPEC_HIDDEN
#elif defined(__GNUC__) && ((__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3)))
# define DECLSPEC_HIDDEN __attribute__((visibility ("hidden")))
#else
# define DECLSPEC_HIDDEN
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef MIDL_PASS
# if defined(_MSC_VER)
#  define DECLSPEC_IMPORT __declspec(dllimport)
# elif defined(__MINGW32__) || defined(__CYGWIN__)
#  define DECLSPEC_IMPORT __attribute__((dllimport))
# else
#  define DECLSPEC_IMPORT DECLSPEC_HIDDEN
# endif
#else
# define DECLSPEC_IMPORT
#endif

#ifdef _OLE32_
#define WINOLEAPI        STDAPI
#define WINOLEAPI_(type) STDAPI_(type)
#else

#ifdef _68K_
#ifndef REQUIRESAPPLEPASCAL
#define WINOLEAPI        EXTERN_C DECLSPEC_IMPORT HRESULT PASCAL
#define WINOLEAPI_(type) EXTERN_C DECLSPEC_IMPORT type PASCAL
#else
#define WINOLEAPI        EXTERN_C DECLSPEC_IMPORT PASCAL HRESULT
#define WINOLEAPI_(type) EXTERN_C DECLSPEC_IMPORT PASCAL type
#endif
#else
#define WINOLEAPI        EXTERN_C DECLSPEC_IMPORT HRESULT STDAPICALLTYPE
#define WINOLEAPI_(type) EXTERN_C DECLSPEC_IMPORT type STDAPICALLTYPE
#endif

#endif


typedef void                                   *LPVOID;
typedef const void                             *LPCVOID;
#ifdef _MSC_VER
typedef long                                   *LPLONG;
typedef unsigned long   DWORD,      *PDWORD,   *LPDWORD;
typedef unsigned long   ULONG,      *PULONG;
#else
typedef int                                    *LPLONG;
typedef unsigned int    DWORD,      *PDWORD,   *LPDWORD;
typedef unsigned int    ULONG,      *PULONG;
#endif
/* register/revoke/get class objects */

//WINOLEAPI  CoGetClassObject(IN REFCLSID rclsid, IN DWORD dwClsContext, IN LPVOID pvReserved,IN REFIID riid, OUT LPVOID FAR* ppv);
WINOLEAPI  CoRegisterClassObject(IN REFCLSID rclsid, IN LPUNKNOWN pUnk,
                    IN DWORD dwClsContext, IN DWORD flags, OUT LPDWORD lpdwRegister);
WINOLEAPI  CoRevokeClassObject(IN DWORD dwRegister);
WINOLEAPI  CoResumeClassObjects(void);
WINOLEAPI  CoSuspendClassObjects(void);
WINOLEAPI_(ULONG) CoAddRefServerProcess(void);
WINOLEAPI_(ULONG) CoReleaseServerProcess(void);
WINOLEAPI  CoGetPSClsid(IN REFIID riid, OUT CLSID *pClsid);
WINOLEAPI  CoRegisterPSClsid(IN REFIID riid, IN REFCLSID rclsid);

// Registering surrogate processes
//WINOLEAPI  CoRegisterSurrogate(IN LPSURROGATE pSurrogate);

#endif     // __OBJBASE_H__
