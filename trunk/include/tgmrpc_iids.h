

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Thu Mar 12 19:38:55 2009
 */
/* Compiler settings for agrrpc.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_ITgmRpcConnectionNotifyObject,0x9BABEF25,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcConnectionRequestObject,0x9BABEF26,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcConnectionAsyncReqCallBack,0x9BABEF27,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcConnectionEvent,0x9BABEF28,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcConnection,0x9BABEF29,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcClient,0x9BABEF2A,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcServerEvent,0x9BABEF2B,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcServer,0x9BABEF2C,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcFactory,0x9BABEF2D,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(CLSID, CLSID_TangramRpcCOMLib,0x1E7D0791,0x0359,0x43af,0x83,0x12,0x94,0xD1,0x53,0x9B,0xF1,0x68);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/



/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Thu Mar 12 19:38:55 2009
 */
/* Compiler settings for agrrpc.idl:
    Oicf, W1, Zp8, env=Win64 (32b run,appending)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if defined(_M_IA64) || defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_ITgmRpcConnectionNotifyObject,0x9BABEF25,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcConnectionRequestObject,0x9BABEF26,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcConnectionAsyncReqCallBack,0x9BABEF27,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcConnectionEvent,0x9BABEF28,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcConnection,0x9BABEF29,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcClient,0x9BABEF2A,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcServerEvent,0x9BABEF2B,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcServer,0x9BABEF2C,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(IID, IID_ITgmRpcFactory,0x9BABEF2D,0x8B75,0x4e14,0x83,0xA3,0xE4,0xE5,0x99,0xEA,0xFD,0x68);


MIDL_DEFINE_GUID(CLSID, CLSID_TangramRpcCOMLib,0x1E7D0791,0x0359,0x43af,0x83,0x12,0x94,0xD1,0x53,0x9B,0xF1,0x68);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* defined(_M_IA64) || defined(_M_AMD64)*/

