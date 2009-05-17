/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Fri Apr 17 18:55:31 2009
 */
/* Compiler settings for .\agrrpc.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __agrrpc_h_h__
#define __agrrpc_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITgmRpcConnectionNotifyObject_FWD_DEFINED__
#define __ITgmRpcConnectionNotifyObject_FWD_DEFINED__
typedef interface ITgmRpcConnectionNotifyObject ITgmRpcConnectionNotifyObject;
#endif 	/* __ITgmRpcConnectionNotifyObject_FWD_DEFINED__ */


#ifndef __ITgmRpcConnectionRequestObject_FWD_DEFINED__
#define __ITgmRpcConnectionRequestObject_FWD_DEFINED__
typedef interface ITgmRpcConnectionRequestObject ITgmRpcConnectionRequestObject;
#endif 	/* __ITgmRpcConnectionRequestObject_FWD_DEFINED__ */


#ifndef __ITgmRpcConnectionAsyncReqCallBack_FWD_DEFINED__
#define __ITgmRpcConnectionAsyncReqCallBack_FWD_DEFINED__
typedef interface ITgmRpcConnectionAsyncReqCallBack ITgmRpcConnectionAsyncReqCallBack;
#endif 	/* __ITgmRpcConnectionAsyncReqCallBack_FWD_DEFINED__ */


#ifndef __ITgmRpcConnectionEvent_FWD_DEFINED__
#define __ITgmRpcConnectionEvent_FWD_DEFINED__
typedef interface ITgmRpcConnectionEvent ITgmRpcConnectionEvent;
#endif 	/* __ITgmRpcConnectionEvent_FWD_DEFINED__ */


#ifndef __ITgmRpcConnection_FWD_DEFINED__
#define __ITgmRpcConnection_FWD_DEFINED__
typedef interface ITgmRpcConnection ITgmRpcConnection;
#endif 	/* __ITgmRpcConnection_FWD_DEFINED__ */


#ifndef __ITgmRpcClient_FWD_DEFINED__
#define __ITgmRpcClient_FWD_DEFINED__
typedef interface ITgmRpcClient ITgmRpcClient;
#endif 	/* __ITgmRpcClient_FWD_DEFINED__ */


#ifndef __ITgmRpcServerEvent_FWD_DEFINED__
#define __ITgmRpcServerEvent_FWD_DEFINED__
typedef interface ITgmRpcServerEvent ITgmRpcServerEvent;
#endif 	/* __ITgmRpcServerEvent_FWD_DEFINED__ */


#ifndef __ITgmRpcServer_FWD_DEFINED__
#define __ITgmRpcServer_FWD_DEFINED__
typedef interface ITgmRpcServer ITgmRpcServer;
#endif 	/* __ITgmRpcServer_FWD_DEFINED__ */


#ifndef __ITgmRpcFactory_FWD_DEFINED__
#define __ITgmRpcFactory_FWD_DEFINED__
typedef interface ITgmRpcFactory ITgmRpcFactory;
#endif 	/* __ITgmRpcFactory_FWD_DEFINED__ */


#ifndef __TangramRpcCOMLib_FWD_DEFINED__
#define __TangramRpcCOMLib_FWD_DEFINED__

#ifdef __cplusplus
typedef class TangramRpcCOMLib TangramRpcCOMLib;
#else
typedef struct TangramRpcCOMLib TangramRpcCOMLib;
#endif /* __cplusplus */

#endif 	/* __TangramRpcCOMLib_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_agrrpc_0000 */
/* [local] */ 

#define RPCCOM_E_REQUEST_EXEC_TIMEOUT  ((HRESULT)0x80040044L)	//执行请求超时，MTAPI_E_TIMEOUT
#define RPCCOM_E_CONN_DISCONNECTED     ((HRESULT)0x80040601L)		//客户不存在或者已经异常断开，MTAPI_E_RPCCOM_CLIENT_CONN_NOT_EXIST
#define RPCCOM_E_CONN_SEND_FAILED      ((HRESULT)0x80040602L)
#define RPCCOM_E_CONN_INVALID          ((HRESULT)0x80040603L)
#define RPCCOM_E_CONN_NO_PACKET        ((HRESULT)0x80040604L)
#define RPCCOM_E_CONN_LISTENING        ((HRESULT)0x80040605L)
#define RPCCOM_E_CONN_LISTEN_FAILED    ((HRESULT)0x80040606L)
#define RPCCOM_E_CONN_ADDR_INUSE       ((HRESULT)0x80040607L)
#define RPCCOM_E_SOCKET_INPROGRESS     ((HRESULT)0x80040608L)
#define RPCCOM_E_CONN_ADDR_NOTAVAIL    ((HRESULT)0x80040609L)		//客户端地址无效。例如，connect函数地址参数为空
#define RPCCOM_E_SOCKET_AGAIN          ((HRESULT)0x8004060bL)
#define RPCCOM_E_CONN_PIPE             ((HRESULT)0x8004060cL)
#define RPCBINGING_PROTSEQU_RPCCOM_TCP "ipcp_tcp" 
#define RPCBINGING_PROTSEQU_RPCCOM_UNIX "ipcp_unix"
#define RPCBINGING_MAX_SIZE 128
#define RPCCOM_INFINITE_TIMEOUT 200000	//!< 最长等待200秒 


extern RPC_IF_HANDLE __MIDL_itf_agrrpc_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_agrrpc_0000_v0_0_s_ifspec;

#ifndef __ITgmRpcConnectionNotifyObject_INTERFACE_DEFINED__
#define __ITgmRpcConnectionNotifyObject_INTERFACE_DEFINED__

/* interface ITgmRpcConnectionNotifyObject */
/* [uuid][local][object] */ 


EXTERN_C const IID IID_ITgmRpcConnectionNotifyObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9BABEF25-8B75-4e14-83A3-E4E599EAFD68")
    ITgmRpcConnectionNotifyObject : public IUnknown
    {
    public:
        virtual ULONG STDMETHODCALLTYPE GetSequNo( void) = 0;
        
        virtual const void *STDMETHODCALLTYPE GetParamsPtr( void) = 0;
        
        virtual ULONG STDMETHODCALLTYPE GetParamsSize( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITgmRpcConnectionNotifyObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITgmRpcConnectionNotifyObject * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITgmRpcConnectionNotifyObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITgmRpcConnectionNotifyObject * This);
        
        ULONG ( STDMETHODCALLTYPE *GetSequNo )( 
            ITgmRpcConnectionNotifyObject * This);
        
        const void *( STDMETHODCALLTYPE *GetParamsPtr )( 
            ITgmRpcConnectionNotifyObject * This);
        
        ULONG ( STDMETHODCALLTYPE *GetParamsSize )( 
            ITgmRpcConnectionNotifyObject * This);
        
        END_INTERFACE
    } ITgmRpcConnectionNotifyObjectVtbl;

    interface ITgmRpcConnectionNotifyObject
    {
        CONST_VTBL struct ITgmRpcConnectionNotifyObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITgmRpcConnectionNotifyObject_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITgmRpcConnectionNotifyObject_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITgmRpcConnectionNotifyObject_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITgmRpcConnectionNotifyObject_GetSequNo(This)	\
    (This)->lpVtbl -> GetSequNo(This)

#define ITgmRpcConnectionNotifyObject_GetParamsPtr(This)	\
    (This)->lpVtbl -> GetParamsPtr(This)

#define ITgmRpcConnectionNotifyObject_GetParamsSize(This)	\
    (This)->lpVtbl -> GetParamsSize(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



ULONG STDMETHODCALLTYPE ITgmRpcConnectionNotifyObject_GetSequNo_Proxy( 
    ITgmRpcConnectionNotifyObject * This);


void __RPC_STUB ITgmRpcConnectionNotifyObject_GetSequNo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


const void *STDMETHODCALLTYPE ITgmRpcConnectionNotifyObject_GetParamsPtr_Proxy( 
    ITgmRpcConnectionNotifyObject * This);


void __RPC_STUB ITgmRpcConnectionNotifyObject_GetParamsPtr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


ULONG STDMETHODCALLTYPE ITgmRpcConnectionNotifyObject_GetParamsSize_Proxy( 
    ITgmRpcConnectionNotifyObject * This);


void __RPC_STUB ITgmRpcConnectionNotifyObject_GetParamsSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITgmRpcConnectionNotifyObject_INTERFACE_DEFINED__ */


#ifndef __ITgmRpcConnectionRequestObject_INTERFACE_DEFINED__
#define __ITgmRpcConnectionRequestObject_INTERFACE_DEFINED__

/* interface ITgmRpcConnectionRequestObject */
/* [uuid][local][object] */ 


EXTERN_C const IID IID_ITgmRpcConnectionRequestObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9BABEF26-8B75-4e14-83A3-E4E599EAFD68")
    ITgmRpcConnectionRequestObject : public ITgmRpcConnectionNotifyObject
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SendReply( 
            /* [in] */ const void *inParamsPtr,
            /* [in] */ ULONG inParmsSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SendResult( 
            /* [in] */ HRESULT inResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITgmRpcConnectionRequestObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITgmRpcConnectionRequestObject * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITgmRpcConnectionRequestObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITgmRpcConnectionRequestObject * This);
        
        ULONG ( STDMETHODCALLTYPE *GetSequNo )( 
            ITgmRpcConnectionRequestObject * This);
        
        const void *( STDMETHODCALLTYPE *GetParamsPtr )( 
            ITgmRpcConnectionRequestObject * This);
        
        ULONG ( STDMETHODCALLTYPE *GetParamsSize )( 
            ITgmRpcConnectionRequestObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *SendReply )( 
            ITgmRpcConnectionRequestObject * This,
            /* [in] */ const void *inParamsPtr,
            /* [in] */ ULONG inParmsSize);
        
        HRESULT ( STDMETHODCALLTYPE *SendResult )( 
            ITgmRpcConnectionRequestObject * This,
            /* [in] */ HRESULT inResult);
        
        END_INTERFACE
    } ITgmRpcConnectionRequestObjectVtbl;

    interface ITgmRpcConnectionRequestObject
    {
        CONST_VTBL struct ITgmRpcConnectionRequestObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITgmRpcConnectionRequestObject_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITgmRpcConnectionRequestObject_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITgmRpcConnectionRequestObject_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITgmRpcConnectionRequestObject_GetSequNo(This)	\
    (This)->lpVtbl -> GetSequNo(This)

#define ITgmRpcConnectionRequestObject_GetParamsPtr(This)	\
    (This)->lpVtbl -> GetParamsPtr(This)

#define ITgmRpcConnectionRequestObject_GetParamsSize(This)	\
    (This)->lpVtbl -> GetParamsSize(This)


#define ITgmRpcConnectionRequestObject_SendReply(This,inParamsPtr,inParmsSize)	\
    (This)->lpVtbl -> SendReply(This,inParamsPtr,inParmsSize)

#define ITgmRpcConnectionRequestObject_SendResult(This,inResult)	\
    (This)->lpVtbl -> SendResult(This,inResult)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ITgmRpcConnectionRequestObject_SendReply_Proxy( 
    ITgmRpcConnectionRequestObject * This,
    /* [in] */ const void *inParamsPtr,
    /* [in] */ ULONG inParmsSize);


void __RPC_STUB ITgmRpcConnectionRequestObject_SendReply_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITgmRpcConnectionRequestObject_SendResult_Proxy( 
    ITgmRpcConnectionRequestObject * This,
    /* [in] */ HRESULT inResult);


void __RPC_STUB ITgmRpcConnectionRequestObject_SendResult_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITgmRpcConnectionRequestObject_INTERFACE_DEFINED__ */


#ifndef __ITgmRpcConnectionAsyncReqCallBack_INTERFACE_DEFINED__
#define __ITgmRpcConnectionAsyncReqCallBack_INTERFACE_DEFINED__

/* interface ITgmRpcConnectionAsyncReqCallBack */
/* [uuid][local][object] */ 


EXTERN_C const IID IID_ITgmRpcConnectionAsyncReqCallBack;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9BABEF27-8B75-4e14-83A3-E4E599EAFD68")
    ITgmRpcConnectionAsyncReqCallBack : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE On_Reply( 
            /* [in] */ HRESULT inResult,
            /* [in] */ const void *outParamsPtr,
            /* [in] */ ULONG outParamsSize,
            /* [in] */ ULONG inSeqNo,
            /* [in] */ ULONG inUserData) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITgmRpcConnectionAsyncReqCallBackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITgmRpcConnectionAsyncReqCallBack * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITgmRpcConnectionAsyncReqCallBack * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITgmRpcConnectionAsyncReqCallBack * This);
        
        void ( STDMETHODCALLTYPE *On_Reply )( 
            ITgmRpcConnectionAsyncReqCallBack * This,
            /* [in] */ HRESULT inResult,
            /* [in] */ const void *outParamsPtr,
            /* [in] */ ULONG outParamsSize,
            /* [in] */ ULONG inSeqNo,
            /* [in] */ ULONG inUserData);
        
        END_INTERFACE
    } ITgmRpcConnectionAsyncReqCallBackVtbl;

    interface ITgmRpcConnectionAsyncReqCallBack
    {
        CONST_VTBL struct ITgmRpcConnectionAsyncReqCallBackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITgmRpcConnectionAsyncReqCallBack_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITgmRpcConnectionAsyncReqCallBack_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITgmRpcConnectionAsyncReqCallBack_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITgmRpcConnectionAsyncReqCallBack_On_Reply(This,inResult,outParamsPtr,outParamsSize,inSeqNo,inUserData)	\
    (This)->lpVtbl -> On_Reply(This,inResult,outParamsPtr,outParamsSize,inSeqNo,inUserData)

#endif /* COBJMACROS */


#endif 	/* C style interface */



void STDMETHODCALLTYPE ITgmRpcConnectionAsyncReqCallBack_On_Reply_Proxy( 
    ITgmRpcConnectionAsyncReqCallBack * This,
    /* [in] */ HRESULT inResult,
    /* [in] */ const void *outParamsPtr,
    /* [in] */ ULONG outParamsSize,
    /* [in] */ ULONG inSeqNo,
    /* [in] */ ULONG inUserData);


void __RPC_STUB ITgmRpcConnectionAsyncReqCallBack_On_Reply_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITgmRpcConnectionAsyncReqCallBack_INTERFACE_DEFINED__ */


#ifndef __ITgmRpcConnectionEvent_INTERFACE_DEFINED__
#define __ITgmRpcConnectionEvent_INTERFACE_DEFINED__

/* interface ITgmRpcConnectionEvent */
/* [uuid][local][object] */ 


EXTERN_C const IID IID_ITgmRpcConnectionEvent;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9BABEF28-8B75-4e14-83A3-E4E599EAFD68")
    ITgmRpcConnectionEvent : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE On_Connected( void) = 0;
        
        virtual void STDMETHODCALLTYPE On_Disconnected( void) = 0;
        
        virtual void STDMETHODCALLTYPE On_DisconnectUnexpected( void) = 0;
        
        virtual void STDMETHODCALLTYPE On_Notify( 
            /* [in] */ ITgmRpcConnectionNotifyObject *inNtyPtr) = 0;
        
        virtual void STDMETHODCALLTYPE On_Request( 
            /* [in] */ ITgmRpcConnectionRequestObject *inReqPtr) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITgmRpcConnectionEventVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITgmRpcConnectionEvent * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITgmRpcConnectionEvent * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITgmRpcConnectionEvent * This);
        
        void ( STDMETHODCALLTYPE *On_Connected )( 
            ITgmRpcConnectionEvent * This);
        
        void ( STDMETHODCALLTYPE *On_Disconnected )( 
            ITgmRpcConnectionEvent * This);
        
        void ( STDMETHODCALLTYPE *On_DisconnectUnexpected )( 
            ITgmRpcConnectionEvent * This);
        
        void ( STDMETHODCALLTYPE *On_Notify )( 
            ITgmRpcConnectionEvent * This,
            /* [in] */ ITgmRpcConnectionNotifyObject *inNtyPtr);
        
        void ( STDMETHODCALLTYPE *On_Request )( 
            ITgmRpcConnectionEvent * This,
            /* [in] */ ITgmRpcConnectionRequestObject *inReqPtr);
        
        END_INTERFACE
    } ITgmRpcConnectionEventVtbl;

    interface ITgmRpcConnectionEvent
    {
        CONST_VTBL struct ITgmRpcConnectionEventVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITgmRpcConnectionEvent_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITgmRpcConnectionEvent_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITgmRpcConnectionEvent_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITgmRpcConnectionEvent_On_Connected(This)	\
    (This)->lpVtbl -> On_Connected(This)

#define ITgmRpcConnectionEvent_On_Disconnected(This)	\
    (This)->lpVtbl -> On_Disconnected(This)

#define ITgmRpcConnectionEvent_On_DisconnectUnexpected(This)	\
    (This)->lpVtbl -> On_DisconnectUnexpected(This)

#define ITgmRpcConnectionEvent_On_Notify(This,inNtyPtr)	\
    (This)->lpVtbl -> On_Notify(This,inNtyPtr)

#define ITgmRpcConnectionEvent_On_Request(This,inReqPtr)	\
    (This)->lpVtbl -> On_Request(This,inReqPtr)

#endif /* COBJMACROS */


#endif 	/* C style interface */



void STDMETHODCALLTYPE ITgmRpcConnectionEvent_On_Connected_Proxy( 
    ITgmRpcConnectionEvent * This);


void __RPC_STUB ITgmRpcConnectionEvent_On_Connected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


void STDMETHODCALLTYPE ITgmRpcConnectionEvent_On_Disconnected_Proxy( 
    ITgmRpcConnectionEvent * This);


void __RPC_STUB ITgmRpcConnectionEvent_On_Disconnected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


void STDMETHODCALLTYPE ITgmRpcConnectionEvent_On_DisconnectUnexpected_Proxy( 
    ITgmRpcConnectionEvent * This);


void __RPC_STUB ITgmRpcConnectionEvent_On_DisconnectUnexpected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


void STDMETHODCALLTYPE ITgmRpcConnectionEvent_On_Notify_Proxy( 
    ITgmRpcConnectionEvent * This,
    /* [in] */ ITgmRpcConnectionNotifyObject *inNtyPtr);


void __RPC_STUB ITgmRpcConnectionEvent_On_Notify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


void STDMETHODCALLTYPE ITgmRpcConnectionEvent_On_Request_Proxy( 
    ITgmRpcConnectionEvent * This,
    /* [in] */ ITgmRpcConnectionRequestObject *inReqPtr);


void __RPC_STUB ITgmRpcConnectionEvent_On_Request_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITgmRpcConnectionEvent_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_agrrpc_0012 */
/* [local] */ 

typedef void ( *ipc_client_memory_free_fun_t )( 
    void *inMem);



extern RPC_IF_HANDLE __MIDL_itf_agrrpc_0012_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_agrrpc_0012_v0_0_s_ifspec;

#ifndef __ITgmRpcConnection_INTERFACE_DEFINED__
#define __ITgmRpcConnection_INTERFACE_DEFINED__

/* interface ITgmRpcConnection */
/* [uuid][local][object] */ 


EXTERN_C const IID IID_ITgmRpcConnection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9BABEF29-8B75-4e14-83A3-E4E599EAFD68")
    ITgmRpcConnection : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Disconnect( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Advise( 
            /* [in] */ ITgmRpcConnectionEvent *inEventHandler,
            /* [in] */ ULONG inIsSvrConn) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Unadvise( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SendSyncRequest( 
            /* [in] */ const void *inParamsPtr,
            /* [in] */ ULONG inParmsSize,
            /* [out] */ void *outParamsPtr,
            /* [in] */ ULONG outParamsSize,
            /* [out] */ ULONG *outParamsLenPtr,
            /* [in] */ ULONG inTimeOut) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SendSyncRequestEx( 
            /* [in] */ const void *inParamsPtr,
            /* [in] */ ULONG inParmsSize,
            /* [out] */ void **outParamsPtr,
            /* [out] */ ULONG *outParamsLenPtr,
            /* [out] */ ipc_client_memory_free_fun_t *free_result_mem,
            /* [in] */ ULONG inTimeOut) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SendAsyncRequest( 
            /* [in] */ const void *inParamsPtr,
            /* [in] */ ULONG inParmsSize,
            /* [in] */ ITgmRpcConnectionAsyncReqCallBack *inCallback,
            /* [out] */ ULONG *outReqNo,
            /* [in] */ ULONG inUserData,
            /* [in] */ ULONG inTimeOut) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SendNotify( 
            /* [in] */ const void *inParamsPtr,
            /* [in] */ ULONG inParmsSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetResourceLocation( 
            /* [in] */ unsigned char *outBindString,
            /* [in] */ ULONG inBingStringSize) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITgmRpcConnectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITgmRpcConnection * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITgmRpcConnection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITgmRpcConnection * This);
        
        HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ITgmRpcConnection * This);
        
        HRESULT ( STDMETHODCALLTYPE *Advise )( 
            ITgmRpcConnection * This,
            /* [in] */ ITgmRpcConnectionEvent *inEventHandler,
            /* [in] */ ULONG inIsSvrConn);
        
        HRESULT ( STDMETHODCALLTYPE *Unadvise )( 
            ITgmRpcConnection * This);
        
        HRESULT ( STDMETHODCALLTYPE *SendSyncRequest )( 
            ITgmRpcConnection * This,
            /* [in] */ const void *inParamsPtr,
            /* [in] */ ULONG inParmsSize,
            /* [out] */ void *outParamsPtr,
            /* [in] */ ULONG outParamsSize,
            /* [out] */ ULONG *outParamsLenPtr,
            /* [in] */ ULONG inTimeOut);
        
        HRESULT ( STDMETHODCALLTYPE *SendSyncRequestEx )( 
            ITgmRpcConnection * This,
            /* [in] */ const void *inParamsPtr,
            /* [in] */ ULONG inParmsSize,
            /* [out] */ void **outParamsPtr,
            /* [out] */ ULONG *outParamsLenPtr,
            /* [out] */ ipc_client_memory_free_fun_t *free_result_mem,
            /* [in] */ ULONG inTimeOut);
        
        HRESULT ( STDMETHODCALLTYPE *SendAsyncRequest )( 
            ITgmRpcConnection * This,
            /* [in] */ const void *inParamsPtr,
            /* [in] */ ULONG inParmsSize,
            /* [in] */ ITgmRpcConnectionAsyncReqCallBack *inCallback,
            /* [out] */ ULONG *outReqNo,
            /* [in] */ ULONG inUserData,
            /* [in] */ ULONG inTimeOut);
        
        HRESULT ( STDMETHODCALLTYPE *SendNotify )( 
            ITgmRpcConnection * This,
            /* [in] */ const void *inParamsPtr,
            /* [in] */ ULONG inParmsSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetResourceLocation )( 
            ITgmRpcConnection * This,
            /* [in] */ unsigned char *outBindString,
            /* [in] */ ULONG inBingStringSize);
        
        END_INTERFACE
    } ITgmRpcConnectionVtbl;

    interface ITgmRpcConnection
    {
        CONST_VTBL struct ITgmRpcConnectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITgmRpcConnection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITgmRpcConnection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITgmRpcConnection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITgmRpcConnection_Disconnect(This)	\
    (This)->lpVtbl -> Disconnect(This)

#define ITgmRpcConnection_Advise(This,inEventHandler,inIsSvrConn)	\
    (This)->lpVtbl -> Advise(This,inEventHandler,inIsSvrConn)

#define ITgmRpcConnection_Unadvise(This)	\
    (This)->lpVtbl -> Unadvise(This)

#define ITgmRpcConnection_SendSyncRequest(This,inParamsPtr,inParmsSize,outParamsPtr,outParamsSize,outParamsLenPtr,inTimeOut)	\
    (This)->lpVtbl -> SendSyncRequest(This,inParamsPtr,inParmsSize,outParamsPtr,outParamsSize,outParamsLenPtr,inTimeOut)

#define ITgmRpcConnection_SendSyncRequestEx(This,inParamsPtr,inParmsSize,outParamsPtr,outParamsLenPtr,free_result_mem,inTimeOut)	\
    (This)->lpVtbl -> SendSyncRequestEx(This,inParamsPtr,inParmsSize,outParamsPtr,outParamsLenPtr,free_result_mem,inTimeOut)

#define ITgmRpcConnection_SendAsyncRequest(This,inParamsPtr,inParmsSize,inCallback,outReqNo,inUserData,inTimeOut)	\
    (This)->lpVtbl -> SendAsyncRequest(This,inParamsPtr,inParmsSize,inCallback,outReqNo,inUserData,inTimeOut)

#define ITgmRpcConnection_SendNotify(This,inParamsPtr,inParmsSize)	\
    (This)->lpVtbl -> SendNotify(This,inParamsPtr,inParmsSize)

#define ITgmRpcConnection_GetResourceLocation(This,outBindString,inBingStringSize)	\
    (This)->lpVtbl -> GetResourceLocation(This,outBindString,inBingStringSize)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ITgmRpcConnection_Disconnect_Proxy( 
    ITgmRpcConnection * This);


void __RPC_STUB ITgmRpcConnection_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITgmRpcConnection_Advise_Proxy( 
    ITgmRpcConnection * This,
    /* [in] */ ITgmRpcConnectionEvent *inEventHandler,
    /* [in] */ ULONG inIsSvrConn);


void __RPC_STUB ITgmRpcConnection_Advise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITgmRpcConnection_Unadvise_Proxy( 
    ITgmRpcConnection * This);


void __RPC_STUB ITgmRpcConnection_Unadvise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITgmRpcConnection_SendSyncRequest_Proxy( 
    ITgmRpcConnection * This,
    /* [in] */ const void *inParamsPtr,
    /* [in] */ ULONG inParmsSize,
    /* [out] */ void *outParamsPtr,
    /* [in] */ ULONG outParamsSize,
    /* [out] */ ULONG *outParamsLenPtr,
    /* [in] */ ULONG inTimeOut);


void __RPC_STUB ITgmRpcConnection_SendSyncRequest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITgmRpcConnection_SendSyncRequestEx_Proxy( 
    ITgmRpcConnection * This,
    /* [in] */ const void *inParamsPtr,
    /* [in] */ ULONG inParmsSize,
    /* [out] */ void **outParamsPtr,
    /* [out] */ ULONG *outParamsLenPtr,
    /* [out] */ ipc_client_memory_free_fun_t *free_result_mem,
    /* [in] */ ULONG inTimeOut);


void __RPC_STUB ITgmRpcConnection_SendSyncRequestEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITgmRpcConnection_SendAsyncRequest_Proxy( 
    ITgmRpcConnection * This,
    /* [in] */ const void *inParamsPtr,
    /* [in] */ ULONG inParmsSize,
    /* [in] */ ITgmRpcConnectionAsyncReqCallBack *inCallback,
    /* [out] */ ULONG *outReqNo,
    /* [in] */ ULONG inUserData,
    /* [in] */ ULONG inTimeOut);


void __RPC_STUB ITgmRpcConnection_SendAsyncRequest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITgmRpcConnection_SendNotify_Proxy( 
    ITgmRpcConnection * This,
    /* [in] */ const void *inParamsPtr,
    /* [in] */ ULONG inParmsSize);


void __RPC_STUB ITgmRpcConnection_SendNotify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITgmRpcConnection_GetResourceLocation_Proxy( 
    ITgmRpcConnection * This,
    /* [in] */ unsigned char *outBindString,
    /* [in] */ ULONG inBingStringSize);


void __RPC_STUB ITgmRpcConnection_GetResourceLocation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITgmRpcConnection_INTERFACE_DEFINED__ */


#ifndef __ITgmRpcClient_INTERFACE_DEFINED__
#define __ITgmRpcClient_INTERFACE_DEFINED__

/* interface ITgmRpcClient */
/* [uuid][local][object] */ 


EXTERN_C const IID IID_ITgmRpcClient;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9BABEF2A-8B75-4e14-83A3-E4E599EAFD68")
    ITgmRpcClient : public ITgmRpcConnection
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Connect( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITgmRpcClientVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITgmRpcClient * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITgmRpcClient * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITgmRpcClient * This);
        
        HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ITgmRpcClient * This);
        
        HRESULT ( STDMETHODCALLTYPE *Advise )( 
            ITgmRpcClient * This,
            /* [in] */ ITgmRpcConnectionEvent *inEventHandler,
            /* [in] */ ULONG inIsSvrConn);
        
        HRESULT ( STDMETHODCALLTYPE *Unadvise )( 
            ITgmRpcClient * This);
        
        HRESULT ( STDMETHODCALLTYPE *SendSyncRequest )( 
            ITgmRpcClient * This,
            /* [in] */ const void *inParamsPtr,
            /* [in] */ ULONG inParmsSize,
            /* [out] */ void *outParamsPtr,
            /* [in] */ ULONG outParamsSize,
            /* [out] */ ULONG *outParamsLenPtr,
            /* [in] */ ULONG inTimeOut);
        
        HRESULT ( STDMETHODCALLTYPE *SendSyncRequestEx )( 
            ITgmRpcClient * This,
            /* [in] */ const void *inParamsPtr,
            /* [in] */ ULONG inParmsSize,
            /* [out] */ void **outParamsPtr,
            /* [out] */ ULONG *outParamsLenPtr,
            /* [out] */ ipc_client_memory_free_fun_t *free_result_mem,
            /* [in] */ ULONG inTimeOut);
        
        HRESULT ( STDMETHODCALLTYPE *SendAsyncRequest )( 
            ITgmRpcClient * This,
            /* [in] */ const void *inParamsPtr,
            /* [in] */ ULONG inParmsSize,
            /* [in] */ ITgmRpcConnectionAsyncReqCallBack *inCallback,
            /* [out] */ ULONG *outReqNo,
            /* [in] */ ULONG inUserData,
            /* [in] */ ULONG inTimeOut);
        
        HRESULT ( STDMETHODCALLTYPE *SendNotify )( 
            ITgmRpcClient * This,
            /* [in] */ const void *inParamsPtr,
            /* [in] */ ULONG inParmsSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetResourceLocation )( 
            ITgmRpcClient * This,
            /* [in] */ unsigned char *outBindString,
            /* [in] */ ULONG inBingStringSize);
        
        HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ITgmRpcClient * This);
        
        END_INTERFACE
    } ITgmRpcClientVtbl;

    interface ITgmRpcClient
    {
        CONST_VTBL struct ITgmRpcClientVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITgmRpcClient_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITgmRpcClient_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITgmRpcClient_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITgmRpcClient_Disconnect(This)	\
    (This)->lpVtbl -> Disconnect(This)

#define ITgmRpcClient_Advise(This,inEventHandler,inIsSvrConn)	\
    (This)->lpVtbl -> Advise(This,inEventHandler,inIsSvrConn)

#define ITgmRpcClient_Unadvise(This)	\
    (This)->lpVtbl -> Unadvise(This)

#define ITgmRpcClient_SendSyncRequest(This,inParamsPtr,inParmsSize,outParamsPtr,outParamsSize,outParamsLenPtr,inTimeOut)	\
    (This)->lpVtbl -> SendSyncRequest(This,inParamsPtr,inParmsSize,outParamsPtr,outParamsSize,outParamsLenPtr,inTimeOut)

#define ITgmRpcClient_SendSyncRequestEx(This,inParamsPtr,inParmsSize,outParamsPtr,outParamsLenPtr,free_result_mem,inTimeOut)	\
    (This)->lpVtbl -> SendSyncRequestEx(This,inParamsPtr,inParmsSize,outParamsPtr,outParamsLenPtr,free_result_mem,inTimeOut)

#define ITgmRpcClient_SendAsyncRequest(This,inParamsPtr,inParmsSize,inCallback,outReqNo,inUserData,inTimeOut)	\
    (This)->lpVtbl -> SendAsyncRequest(This,inParamsPtr,inParmsSize,inCallback,outReqNo,inUserData,inTimeOut)

#define ITgmRpcClient_SendNotify(This,inParamsPtr,inParmsSize)	\
    (This)->lpVtbl -> SendNotify(This,inParamsPtr,inParmsSize)

#define ITgmRpcClient_GetResourceLocation(This,outBindString,inBingStringSize)	\
    (This)->lpVtbl -> GetResourceLocation(This,outBindString,inBingStringSize)


#define ITgmRpcClient_Connect(This)	\
    (This)->lpVtbl -> Connect(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ITgmRpcClient_Connect_Proxy( 
    ITgmRpcClient * This);


void __RPC_STUB ITgmRpcClient_Connect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITgmRpcClient_INTERFACE_DEFINED__ */


#ifndef __ITgmRpcServerEvent_INTERFACE_DEFINED__
#define __ITgmRpcServerEvent_INTERFACE_DEFINED__

/* interface ITgmRpcServerEvent */
/* [uuid][local][object] */ 


EXTERN_C const IID IID_ITgmRpcServerEvent;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9BABEF2B-8B75-4e14-83A3-E4E599EAFD68")
    ITgmRpcServerEvent : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE On_ClientConnection_Created( 
            /* [out] */ ITgmRpcConnection *ppOutCliObj) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITgmRpcServerEventVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITgmRpcServerEvent * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITgmRpcServerEvent * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITgmRpcServerEvent * This);
        
        HRESULT ( STDMETHODCALLTYPE *On_ClientConnection_Created )( 
            ITgmRpcServerEvent * This,
            /* [out] */ ITgmRpcConnection *ppOutCliObj);
        
        END_INTERFACE
    } ITgmRpcServerEventVtbl;

    interface ITgmRpcServerEvent
    {
        CONST_VTBL struct ITgmRpcServerEventVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITgmRpcServerEvent_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITgmRpcServerEvent_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITgmRpcServerEvent_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITgmRpcServerEvent_On_ClientConnection_Created(This,ppOutCliObj)	\
    (This)->lpVtbl -> On_ClientConnection_Created(This,ppOutCliObj)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ITgmRpcServerEvent_On_ClientConnection_Created_Proxy( 
    ITgmRpcServerEvent * This,
    /* [out] */ ITgmRpcConnection *ppOutCliObj);


void __RPC_STUB ITgmRpcServerEvent_On_ClientConnection_Created_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITgmRpcServerEvent_INTERFACE_DEFINED__ */


#ifndef __ITgmRpcServer_INTERFACE_DEFINED__
#define __ITgmRpcServer_INTERFACE_DEFINED__

/* interface ITgmRpcServer */
/* [uuid][local][object] */ 


EXTERN_C const IID IID_ITgmRpcServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9BABEF2C-8B75-4e14-83A3-E4E599EAFD68")
    ITgmRpcServer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Listen( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DisconnectAll( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Advise( 
            /* [in] */ ITgmRpcServerEvent *inEventHandler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Unadvise( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetResourceLocation( 
            /* [size_is][out] */ unsigned char *outBindString,
            /* [in] */ ULONG inBingStringSize) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITgmRpcServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITgmRpcServer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITgmRpcServer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITgmRpcServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *Listen )( 
            ITgmRpcServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *Close )( 
            ITgmRpcServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *DisconnectAll )( 
            ITgmRpcServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *Advise )( 
            ITgmRpcServer * This,
            /* [in] */ ITgmRpcServerEvent *inEventHandler);
        
        HRESULT ( STDMETHODCALLTYPE *Unadvise )( 
            ITgmRpcServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetResourceLocation )( 
            ITgmRpcServer * This,
            /* [size_is][out] */ unsigned char *outBindString,
            /* [in] */ ULONG inBingStringSize);
        
        END_INTERFACE
    } ITgmRpcServerVtbl;

    interface ITgmRpcServer
    {
        CONST_VTBL struct ITgmRpcServerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITgmRpcServer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITgmRpcServer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITgmRpcServer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITgmRpcServer_Listen(This)	\
    (This)->lpVtbl -> Listen(This)

#define ITgmRpcServer_Close(This)	\
    (This)->lpVtbl -> Close(This)

#define ITgmRpcServer_DisconnectAll(This)	\
    (This)->lpVtbl -> DisconnectAll(This)

#define ITgmRpcServer_Advise(This,inEventHandler)	\
    (This)->lpVtbl -> Advise(This,inEventHandler)

#define ITgmRpcServer_Unadvise(This)	\
    (This)->lpVtbl -> Unadvise(This)

#define ITgmRpcServer_GetResourceLocation(This,outBindString,inBingStringSize)	\
    (This)->lpVtbl -> GetResourceLocation(This,outBindString,inBingStringSize)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ITgmRpcServer_Listen_Proxy( 
    ITgmRpcServer * This);


void __RPC_STUB ITgmRpcServer_Listen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITgmRpcServer_Close_Proxy( 
    ITgmRpcServer * This);


void __RPC_STUB ITgmRpcServer_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITgmRpcServer_DisconnectAll_Proxy( 
    ITgmRpcServer * This);


void __RPC_STUB ITgmRpcServer_DisconnectAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITgmRpcServer_Advise_Proxy( 
    ITgmRpcServer * This,
    /* [in] */ ITgmRpcServerEvent *inEventHandler);


void __RPC_STUB ITgmRpcServer_Advise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITgmRpcServer_Unadvise_Proxy( 
    ITgmRpcServer * This);


void __RPC_STUB ITgmRpcServer_Unadvise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITgmRpcServer_GetResourceLocation_Proxy( 
    ITgmRpcServer * This,
    /* [size_is][out] */ unsigned char *outBindString,
    /* [in] */ ULONG inBingStringSize);


void __RPC_STUB ITgmRpcServer_GetResourceLocation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITgmRpcServer_INTERFACE_DEFINED__ */


#ifndef __ITgmRpcFactory_INTERFACE_DEFINED__
#define __ITgmRpcFactory_INTERFACE_DEFINED__

/* interface ITgmRpcFactory */
/* [uuid][local][object] */ 


EXTERN_C const IID IID_ITgmRpcFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9BABEF2D-8B75-4e14-83A3-E4E599EAFD68")
    ITgmRpcFactory : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateRpcServer( 
            /* [out] */ ITgmRpcServer **ppOutIf,
            /* [string][in] */ const unsigned char *bindString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateRpcClient( 
            /* [out] */ ITgmRpcClient **ppOutIf,
            /* [string][in] */ const unsigned char *bindString) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITgmRpcFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITgmRpcFactory * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITgmRpcFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITgmRpcFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateRpcServer )( 
            ITgmRpcFactory * This,
            /* [out] */ ITgmRpcServer **ppOutIf,
            /* [string][in] */ const unsigned char *bindString);
        
        HRESULT ( STDMETHODCALLTYPE *CreateRpcClient )( 
            ITgmRpcFactory * This,
            /* [out] */ ITgmRpcClient **ppOutIf,
            /* [string][in] */ const unsigned char *bindString);
        
        END_INTERFACE
    } ITgmRpcFactoryVtbl;

    interface ITgmRpcFactory
    {
        CONST_VTBL struct ITgmRpcFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITgmRpcFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITgmRpcFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITgmRpcFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITgmRpcFactory_CreateRpcServer(This,ppOutIf,bindString)	\
    (This)->lpVtbl -> CreateRpcServer(This,ppOutIf,bindString)

#define ITgmRpcFactory_CreateRpcClient(This,ppOutIf,bindString)	\
    (This)->lpVtbl -> CreateRpcClient(This,ppOutIf,bindString)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ITgmRpcFactory_CreateRpcServer_Proxy( 
    ITgmRpcFactory * This,
    /* [out] */ ITgmRpcServer **ppOutIf,
    /* [string][in] */ const unsigned char *bindString);


void __RPC_STUB ITgmRpcFactory_CreateRpcServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITgmRpcFactory_CreateRpcClient_Proxy( 
    ITgmRpcFactory * This,
    /* [out] */ ITgmRpcClient **ppOutIf,
    /* [string][in] */ const unsigned char *bindString);


void __RPC_STUB ITgmRpcFactory_CreateRpcClient_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITgmRpcFactory_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


