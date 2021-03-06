/*** Autogenerated by WIDL 1.1.6 from talk.idl - Do not edit ***/

#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif /* __REDQ_RPCPROXY_H_VERSION__ */

#define __midl_proxy
#include "objbase.h"
#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error This code needs a newer version of rpcproxy.h
#endif /* __RPCPROXY_H_VERSION__ */

#include "talk.h"

#define RpcExceptionInit(filter_func,finally_func) do {} while(0)
#define __DECL_EXCEPTION_FRAME

struct __proxy_frame
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    void             *This;
};

static int __proxy_filter( struct __proxy_frame *__frame )
{
    return (__frame->_StubMsg.dwStubPhase != PROXY_SENDRECEIVE);
}

#define TYPE_FORMAT_STRING_SIZE 59
#define PROC_FORMAT_STRING_SIZE 37

typedef struct _MIDL_TYPE_FORMAT_STRING
{
    short Pad;
    unsigned char Format[TYPE_FORMAT_STRING_SIZE];
} MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
{
    short Pad;
    unsigned char Format[PROC_FORMAT_STRING_SIZE];
} MIDL_PROC_FORMAT_STRING;


static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;

static const MIDL_STUB_DESC Object_StubDesc;

/*****************************************************************************
 * ITalkSession interface
 */
static void __finally_ITalkSession_Say_Proxy( struct __proxy_frame *__frame )
{
    NdrProxyFreeBuffer( __frame->This, &__frame->_StubMsg );
}

HRESULT STDMETHODCALLTYPE ITalkSession_Say_Proxy(
    ITalkSession* This,
    const OLECHAR *pwszStatement)
{
    struct __proxy_frame __f, * const __frame = &__f;
    HRESULT     _RetVal;
    RPC_MESSAGE _RpcMessage;

    RpcExceptionInit( __proxy_filter, __finally_ITalkSession_Say_Proxy );
    __frame->This = This;
    RpcTryExcept
    {
        NdrProxyInitialize(This, &_RpcMessage, &__frame->_StubMsg, &Object_StubDesc, 3);
        if(!pwszStatement)
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
        RpcTryFinally
        {
            __frame->_StubMsg.BufferLength = 6;
            NdrConformantStringBufferSize(
                &__frame->_StubMsg,
                (unsigned char *)pwszStatement,
                (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[4]);

            NdrProxyGetBuffer(This, &__frame->_StubMsg);
            NdrConformantStringMarshall(
                &__frame->_StubMsg,
                (unsigned char *)pwszStatement,
                (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[4]);

            NdrProxySendReceive(This, &__frame->_StubMsg);

            __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
            __frame->_StubMsg.BufferEnd   = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

            if ((_RpcMessage.DataRepresentation & 0xffff) != NDR_LOCAL_DATA_REPRESENTATION)
                NdrConvert( &__frame->_StubMsg, &__MIDL_ProcFormatString.Format[0]);

            __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
            if (__frame->_StubMsg.Buffer + sizeof(HRESULT) > __frame->_StubMsg.BufferEnd)
            {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
            }
            _RetVal = *(HRESULT *)__frame->_StubMsg.Buffer;
            __frame->_StubMsg.Buffer += sizeof(HRESULT);

        }
        RpcFinally
        {
            __finally_ITalkSession_Say_Proxy( __frame );
        }
        RpcEndFinally
    }
    RpcExcept(__frame->_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
    {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
    }
    RpcEndExcept
    return _RetVal;
}

struct __frame_ITalkSession_Say_Stub
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    ITalkSession * _This;
    HRESULT _RetVal;
    const OLECHAR * pwszStatement;
};

static void __finally_ITalkSession_Say_Stub( struct __frame_ITalkSession_Say_Stub *__frame )
{
    NdrPointerFree(
        &__frame->_StubMsg,
        (unsigned char *)__frame->pwszStatement,
        (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[2]);

}

void __RPC_STUB ITalkSession_Say_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD* _pdwStubPhase)
{
    struct __frame_ITalkSession_Say_Stub __f, * const __frame = &__f;

    __frame->_This = (ITalkSession*)((CStdStubBuffer*)This)->pvServerObject;

    NdrStubInitialize(_pRpcMessage, &__frame->_StubMsg, &Object_StubDesc, _pRpcChannelBuffer);

    RpcExceptionInit( 0, __finally_ITalkSession_Say_Stub );
    __frame->pwszStatement = 0;

    RpcTryFinally
    {
        if ((_pRpcMessage->DataRepresentation & 0xffff) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert( &__frame->_StubMsg, &__MIDL_ProcFormatString.Format[0]);

        NdrConformantStringUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&__frame->pwszStatement,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[4],
            0);


        *_pdwStubPhase = STUB_CALL_SERVER;

        __frame->_RetVal = __frame->_This->lpVtbl->Say(__frame->_This, __frame->pwszStatement);

        *_pdwStubPhase = STUB_MARSHAL;

        __frame->_StubMsg.BufferLength = 8;

        NdrStubGetBuffer(This, _pRpcChannelBuffer, &__frame->_StubMsg);

        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(HRESULT *)__frame->_StubMsg.Buffer = __frame->_RetVal;
        __frame->_StubMsg.Buffer += sizeof(HRESULT);

    }
    RpcFinally
    {
        __finally_ITalkSession_Say_Stub( __frame );
    }
    RpcEndFinally
    _pRpcMessage->BufferLength = __frame->_StubMsg.Buffer - (unsigned char *)_pRpcMessage->Buffer;
}

static void __finally_ITalkSession_Advise_Proxy( struct __proxy_frame *__frame )
{
    NdrProxyFreeBuffer( __frame->This, &__frame->_StubMsg );
}

HRESULT STDMETHODCALLTYPE ITalkSession_Advise_Proxy(
    ITalkSession* This,
    ITalkSessionEvents *pEventSink,
    DWORD *pdwReg)
{
    struct __proxy_frame __f, * const __frame = &__f;
    HRESULT     _RetVal;
    RPC_MESSAGE _RpcMessage;

    RpcExceptionInit( __proxy_filter, __finally_ITalkSession_Advise_Proxy );
    __frame->This = This;
    if(pdwReg)
        MIDL_memset( pdwReg, 0, sizeof( *pdwReg ));
    RpcTryExcept
    {
        NdrProxyInitialize(This, &_RpcMessage, &__frame->_StubMsg, &Object_StubDesc, 4);
        if(!pEventSink)
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
        if(!pdwReg)
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
        RpcTryFinally
        {
            __frame->_StubMsg.BufferLength = 8;
            NdrInterfacePointerBufferSize(
                &__frame->_StubMsg,
                (unsigned char *)pEventSink,
                (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[6]);

            NdrProxyGetBuffer(This, &__frame->_StubMsg);
            NdrInterfacePointerMarshall(
                &__frame->_StubMsg,
                (unsigned char *)pEventSink,
                (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[6]);

            NdrProxySendReceive(This, &__frame->_StubMsg);

            __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
            __frame->_StubMsg.BufferEnd   = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

            if ((_RpcMessage.DataRepresentation & 0xffff) != NDR_LOCAL_DATA_REPRESENTATION)
                NdrConvert( &__frame->_StubMsg, &__MIDL_ProcFormatString.Format[6]);

            NdrPointerUnmarshall(
                &__frame->_StubMsg,
                (unsigned char **)&pdwReg,
                (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[24],
                0);

            __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
            if (__frame->_StubMsg.Buffer + sizeof(HRESULT) > __frame->_StubMsg.BufferEnd)
            {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
            }
            _RetVal = *(HRESULT *)__frame->_StubMsg.Buffer;
            __frame->_StubMsg.Buffer += sizeof(HRESULT);

        }
        RpcFinally
        {
            __finally_ITalkSession_Advise_Proxy( __frame );
        }
        RpcEndFinally
    }
    RpcExcept(__frame->_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
    {
        /* FIXME: free_variable code for pdwReg type 17 missing */

        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
    }
    RpcEndExcept
    return _RetVal;
}

struct __frame_ITalkSession_Advise_Stub
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    ITalkSession * _This;
    HRESULT _RetVal;
    ITalkSessionEvents * pEventSink;
    DWORD _W0;
    DWORD * pdwReg;
};

static void __finally_ITalkSession_Advise_Stub( struct __frame_ITalkSession_Advise_Stub *__frame )
{
    NdrInterfacePointerFree(
        &__frame->_StubMsg,
        (unsigned char *)__frame->pEventSink,
        (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[6]);

    NdrPointerFree(
        &__frame->_StubMsg,
        (unsigned char *)__frame->pdwReg,
        (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[24]);

}

void __RPC_STUB ITalkSession_Advise_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD* _pdwStubPhase)
{
    struct __frame_ITalkSession_Advise_Stub __f, * const __frame = &__f;

    __frame->_This = (ITalkSession*)((CStdStubBuffer*)This)->pvServerObject;

    NdrStubInitialize(_pRpcMessage, &__frame->_StubMsg, &Object_StubDesc, _pRpcChannelBuffer);

    RpcExceptionInit( 0, __finally_ITalkSession_Advise_Stub );
    __frame->pEventSink = 0;
    __frame->pdwReg = 0;

    RpcTryFinally
    {
        if ((_pRpcMessage->DataRepresentation & 0xffff) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert( &__frame->_StubMsg, &__MIDL_ProcFormatString.Format[6]);

        NdrInterfacePointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&__frame->pEventSink,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[6],
            0);


        __frame->pdwReg = &__frame->_W0;

        *_pdwStubPhase = STUB_CALL_SERVER;

        __frame->_RetVal = __frame->_This->lpVtbl->Advise(__frame->_This, __frame->pEventSink, __frame->pdwReg);

        *_pdwStubPhase = STUB_MARSHAL;

        __frame->_StubMsg.BufferLength = 8;
        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)__frame->pdwReg,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[24]);


        NdrStubGetBuffer(This, _pRpcChannelBuffer, &__frame->_StubMsg);
        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)__frame->pdwReg,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[24]);


        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(HRESULT *)__frame->_StubMsg.Buffer = __frame->_RetVal;
        __frame->_StubMsg.Buffer += sizeof(HRESULT);

    }
    RpcFinally
    {
        __finally_ITalkSession_Advise_Stub( __frame );
    }
    RpcEndFinally
    _pRpcMessage->BufferLength = __frame->_StubMsg.Buffer - (unsigned char *)_pRpcMessage->Buffer;
}

static void __finally_ITalkSession_Unadvise_Proxy( struct __proxy_frame *__frame )
{
    NdrProxyFreeBuffer( __frame->This, &__frame->_StubMsg );
}

HRESULT STDMETHODCALLTYPE ITalkSession_Unadvise_Proxy(
    ITalkSession* This,
    DWORD dwReg)
{
    struct __proxy_frame __f, * const __frame = &__f;
    HRESULT     _RetVal;
    RPC_MESSAGE _RpcMessage;

    RpcExceptionInit( __proxy_filter, __finally_ITalkSession_Unadvise_Proxy );
    __frame->This = This;
    RpcTryExcept
    {
        NdrProxyInitialize(This, &_RpcMessage, &__frame->_StubMsg, &Object_StubDesc, 5);
        RpcTryFinally
        {
            __frame->_StubMsg.BufferLength = 8;

            NdrProxyGetBuffer(This, &__frame->_StubMsg);
            MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
            __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
            *(DWORD *)__frame->_StubMsg.Buffer = dwReg;
            __frame->_StubMsg.Buffer += sizeof(DWORD);

            NdrProxySendReceive(This, &__frame->_StubMsg);

            __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
            __frame->_StubMsg.BufferEnd   = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

            if ((_RpcMessage.DataRepresentation & 0xffff) != NDR_LOCAL_DATA_REPRESENTATION)
                NdrConvert( &__frame->_StubMsg, &__MIDL_ProcFormatString.Format[16]);

            __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
            if (__frame->_StubMsg.Buffer + sizeof(HRESULT) > __frame->_StubMsg.BufferEnd)
            {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
            }
            _RetVal = *(HRESULT *)__frame->_StubMsg.Buffer;
            __frame->_StubMsg.Buffer += sizeof(HRESULT);

        }
        RpcFinally
        {
            __finally_ITalkSession_Unadvise_Proxy( __frame );
        }
        RpcEndFinally
    }
    RpcExcept(__frame->_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
    {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
    }
    RpcEndExcept
    return _RetVal;
}

struct __frame_ITalkSession_Unadvise_Stub
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    ITalkSession * _This;
    HRESULT _RetVal;
    DWORD dwReg;
};

static void __finally_ITalkSession_Unadvise_Stub( struct __frame_ITalkSession_Unadvise_Stub *__frame )
{

}

void __RPC_STUB ITalkSession_Unadvise_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD* _pdwStubPhase)
{
    struct __frame_ITalkSession_Unadvise_Stub __f, * const __frame = &__f;

    __frame->_This = (ITalkSession*)((CStdStubBuffer*)This)->pvServerObject;

    NdrStubInitialize(_pRpcMessage, &__frame->_StubMsg, &Object_StubDesc, _pRpcChannelBuffer);

    RpcExceptionInit( 0, __finally_ITalkSession_Unadvise_Stub );

    RpcTryFinally
    {
        if ((_pRpcMessage->DataRepresentation & 0xffff) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert( &__frame->_StubMsg, &__MIDL_ProcFormatString.Format[16]);

        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        if (__frame->_StubMsg.Buffer + sizeof(DWORD) > __frame->_StubMsg.BufferEnd)
        {
            RpcRaiseException(RPC_X_BAD_STUB_DATA);
        }
        __frame->dwReg = *(DWORD *)__frame->_StubMsg.Buffer;
        __frame->_StubMsg.Buffer += sizeof(DWORD);


        *_pdwStubPhase = STUB_CALL_SERVER;

        __frame->_RetVal = __frame->_This->lpVtbl->Unadvise(__frame->_This, __frame->dwReg);

        *_pdwStubPhase = STUB_MARSHAL;

        __frame->_StubMsg.BufferLength = 8;

        NdrStubGetBuffer(This, _pRpcChannelBuffer, &__frame->_StubMsg);

        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(HRESULT *)__frame->_StubMsg.Buffer = __frame->_RetVal;
        __frame->_StubMsg.Buffer += sizeof(HRESULT);

    }
    RpcFinally
    {
        __finally_ITalkSession_Unadvise_Stub( __frame );
    }
    RpcEndFinally
    _pRpcMessage->BufferLength = __frame->_StubMsg.Buffer - (unsigned char *)_pRpcMessage->Buffer;
}

static const CINTERFACE_PROXY_VTABLE(6) _ITalkSessionProxyVtbl =
{
    {
        &IID_ITalkSession,
    },
    {
        IUnknown_QueryInterface_Proxy,
        IUnknown_AddRef_Proxy,
        IUnknown_Release_Proxy,
        ITalkSession_Say_Proxy,
        ITalkSession_Advise_Proxy,
        ITalkSession_Unadvise_Proxy
    }
};


static const PRPC_STUB_FUNCTION ITalkSession_table[] =
{
    ITalkSession_Say_Stub,
    ITalkSession_Advise_Stub,
    ITalkSession_Unadvise_Stub
};

static const CInterfaceStubVtbl _ITalkSessionStubVtbl =
{
    {
        &IID_ITalkSession,
        0,
        6,
        &ITalkSession_table[-3],
    },
    {
        CStdStubBuffer_METHODS
    }
};

/*****************************************************************************
 * ITalkSessionEvents interface
 */
static void __finally_ITalkSessionEvents_OnStatement_Proxy( struct __proxy_frame *__frame )
{
    NdrProxyFreeBuffer( __frame->This, &__frame->_StubMsg );
}

HRESULT STDMETHODCALLTYPE ITalkSessionEvents_OnStatement_Proxy(
    ITalkSessionEvents* This,
    const OLECHAR *pwszUser,
    const OLECHAR *pwszStmnt)
{
    struct __proxy_frame __f, * const __frame = &__f;
    HRESULT     _RetVal;
    RPC_MESSAGE _RpcMessage;

    RpcExceptionInit( __proxy_filter, __finally_ITalkSessionEvents_OnStatement_Proxy );
    __frame->This = This;
    RpcTryExcept
    {
        NdrProxyInitialize(This, &_RpcMessage, &__frame->_StubMsg, &Object_StubDesc, 3);
        if(!pwszUser)
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
        if(!pwszStmnt)
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
        RpcTryFinally
        {
            __frame->_StubMsg.BufferLength = 12;
            NdrConformantStringBufferSize(
                &__frame->_StubMsg,
                (unsigned char *)pwszUser,
                (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[30]);

            NdrConformantStringBufferSize(
                &__frame->_StubMsg,
                (unsigned char *)pwszStmnt,
                (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[34]);

            NdrProxyGetBuffer(This, &__frame->_StubMsg);
            NdrConformantStringMarshall(
                &__frame->_StubMsg,
                (unsigned char *)pwszUser,
                (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[30]);

            NdrConformantStringMarshall(
                &__frame->_StubMsg,
                (unsigned char *)pwszStmnt,
                (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[34]);

            NdrProxySendReceive(This, &__frame->_StubMsg);

            __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
            __frame->_StubMsg.BufferEnd   = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

            if ((_RpcMessage.DataRepresentation & 0xffff) != NDR_LOCAL_DATA_REPRESENTATION)
                NdrConvert( &__frame->_StubMsg, &__MIDL_ProcFormatString.Format[20]);

            __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
            if (__frame->_StubMsg.Buffer + sizeof(HRESULT) > __frame->_StubMsg.BufferEnd)
            {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
            }
            _RetVal = *(HRESULT *)__frame->_StubMsg.Buffer;
            __frame->_StubMsg.Buffer += sizeof(HRESULT);

        }
        RpcFinally
        {
            __finally_ITalkSessionEvents_OnStatement_Proxy( __frame );
        }
        RpcEndFinally
    }
    RpcExcept(__frame->_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
    {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
    }
    RpcEndExcept
    return _RetVal;
}

struct __frame_ITalkSessionEvents_OnStatement_Stub
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    ITalkSessionEvents * _This;
    HRESULT _RetVal;
    const OLECHAR * pwszUser;
    const OLECHAR * pwszStmnt;
};

static void __finally_ITalkSessionEvents_OnStatement_Stub( struct __frame_ITalkSessionEvents_OnStatement_Stub *__frame )
{
    NdrPointerFree(
        &__frame->_StubMsg,
        (unsigned char *)__frame->pwszUser,
        (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[28]);

    NdrPointerFree(
        &__frame->_StubMsg,
        (unsigned char *)__frame->pwszStmnt,
        (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[32]);

}

void __RPC_STUB ITalkSessionEvents_OnStatement_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD* _pdwStubPhase)
{
    struct __frame_ITalkSessionEvents_OnStatement_Stub __f, * const __frame = &__f;

    __frame->_This = (ITalkSessionEvents*)((CStdStubBuffer*)This)->pvServerObject;

    NdrStubInitialize(_pRpcMessage, &__frame->_StubMsg, &Object_StubDesc, _pRpcChannelBuffer);

    RpcExceptionInit( 0, __finally_ITalkSessionEvents_OnStatement_Stub );
    __frame->pwszUser = 0;
    __frame->pwszStmnt = 0;

    RpcTryFinally
    {
        if ((_pRpcMessage->DataRepresentation & 0xffff) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert( &__frame->_StubMsg, &__MIDL_ProcFormatString.Format[20]);

        NdrConformantStringUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&__frame->pwszUser,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[30],
            0);

        NdrConformantStringUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&__frame->pwszStmnt,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[34],
            0);


        *_pdwStubPhase = STUB_CALL_SERVER;

        __frame->_RetVal = __frame->_This->lpVtbl->OnStatement(__frame->_This, __frame->pwszUser, __frame->pwszStmnt);

        *_pdwStubPhase = STUB_MARSHAL;

        __frame->_StubMsg.BufferLength = 8;

        NdrStubGetBuffer(This, _pRpcChannelBuffer, &__frame->_StubMsg);

        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(HRESULT *)__frame->_StubMsg.Buffer = __frame->_RetVal;
        __frame->_StubMsg.Buffer += sizeof(HRESULT);

    }
    RpcFinally
    {
        __finally_ITalkSessionEvents_OnStatement_Stub( __frame );
    }
    RpcEndFinally
    _pRpcMessage->BufferLength = __frame->_StubMsg.Buffer - (unsigned char *)_pRpcMessage->Buffer;
}

static const CINTERFACE_PROXY_VTABLE(4) _ITalkSessionEventsProxyVtbl =
{
    {
        &IID_ITalkSessionEvents,
    },
    {
        IUnknown_QueryInterface_Proxy,
        IUnknown_AddRef_Proxy,
        IUnknown_Release_Proxy,
        ITalkSessionEvents_OnStatement_Proxy
    }
};


static const PRPC_STUB_FUNCTION ITalkSessionEvents_table[] =
{
    ITalkSessionEvents_OnStatement_Stub
};

static const CInterfaceStubVtbl _ITalkSessionEventsStubVtbl =
{
    {
        &IID_ITalkSessionEvents,
        0,
        4,
        &ITalkSessionEvents_table[-3],
    },
    {
        CStdStubBuffer_METHODS
    }
};

/*****************************************************************************
 * ITalkSessionManager interface
 */
static void __finally_ITalkSessionManager_CreateSession_Proxy( struct __proxy_frame *__frame )
{
    NdrProxyFreeBuffer( __frame->This, &__frame->_StubMsg );
}

HRESULT STDMETHODCALLTYPE ITalkSessionManager_CreateSession_Proxy(
    ITalkSessionManager* This,
    ITalkSession **ppts)
{
    struct __proxy_frame __f, * const __frame = &__f;
    HRESULT     _RetVal;
    RPC_MESSAGE _RpcMessage;

    RpcExceptionInit( __proxy_filter, __finally_ITalkSessionManager_CreateSession_Proxy );
    __frame->This = This;
    if(ppts)
        MIDL_memset( ppts, 0, sizeof( *ppts ));
    RpcTryExcept
    {
        NdrProxyInitialize(This, &_RpcMessage, &__frame->_StubMsg, &Object_StubDesc, 3);
        if(!ppts)
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
        RpcTryFinally
        {
            __frame->_StubMsg.BufferLength = 0;
            NdrProxyGetBuffer(This, &__frame->_StubMsg);
            NdrProxySendReceive(This, &__frame->_StubMsg);

            __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
            __frame->_StubMsg.BufferEnd   = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

            if ((_RpcMessage.DataRepresentation & 0xffff) != NDR_LOCAL_DATA_REPRESENTATION)
                NdrConvert( &__frame->_StubMsg, &__MIDL_ProcFormatString.Format[30]);

            NdrPointerUnmarshall(
                &__frame->_StubMsg,
                (unsigned char **)&ppts,
                (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[54],
                0);

            __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
            if (__frame->_StubMsg.Buffer + sizeof(HRESULT) > __frame->_StubMsg.BufferEnd)
            {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
            }
            _RetVal = *(HRESULT *)__frame->_StubMsg.Buffer;
            __frame->_StubMsg.Buffer += sizeof(HRESULT);

        }
        RpcFinally
        {
            __finally_ITalkSessionManager_CreateSession_Proxy( __frame );
        }
        RpcEndFinally
    }
    RpcExcept(__frame->_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
    {
        /* FIXME: free_variable code for ppts type 17 missing */

        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
    }
    RpcEndExcept
    return _RetVal;
}

struct __frame_ITalkSessionManager_CreateSession_Stub
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    ITalkSessionManager * _This;
    HRESULT _RetVal;
    ITalkSession *_W0;
    ITalkSession ** ppts;
};

static void __finally_ITalkSessionManager_CreateSession_Stub( struct __frame_ITalkSessionManager_CreateSession_Stub *__frame )
{
    NdrPointerFree(
        &__frame->_StubMsg,
        (unsigned char *)__frame->ppts,
        (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[54]);

}

void __RPC_STUB ITalkSessionManager_CreateSession_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD* _pdwStubPhase)
{
    struct __frame_ITalkSessionManager_CreateSession_Stub __f, * const __frame = &__f;

    __frame->_This = (ITalkSessionManager*)((CStdStubBuffer*)This)->pvServerObject;

    NdrStubInitialize(_pRpcMessage, &__frame->_StubMsg, &Object_StubDesc, _pRpcChannelBuffer);

    RpcExceptionInit( 0, __finally_ITalkSessionManager_CreateSession_Stub );
    __frame->ppts = 0;

    RpcTryFinally
    {
        if ((_pRpcMessage->DataRepresentation & 0xffff) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert( &__frame->_StubMsg, &__MIDL_ProcFormatString.Format[30]);


        __frame->ppts = &__frame->_W0;
        __frame->_W0 = 0;

        *_pdwStubPhase = STUB_CALL_SERVER;

        __frame->_RetVal = __frame->_This->lpVtbl->CreateSession(__frame->_This, __frame->ppts);

        *_pdwStubPhase = STUB_MARSHAL;

        __frame->_StubMsg.BufferLength = 8;
        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)__frame->ppts,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[54]);


        NdrStubGetBuffer(This, _pRpcChannelBuffer, &__frame->_StubMsg);
        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)__frame->ppts,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[54]);


        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(HRESULT *)__frame->_StubMsg.Buffer = __frame->_RetVal;
        __frame->_StubMsg.Buffer += sizeof(HRESULT);

    }
    RpcFinally
    {
        __finally_ITalkSessionManager_CreateSession_Stub( __frame );
    }
    RpcEndFinally
    _pRpcMessage->BufferLength = __frame->_StubMsg.Buffer - (unsigned char *)_pRpcMessage->Buffer;
}

static const CINTERFACE_PROXY_VTABLE(4) _ITalkSessionManagerProxyVtbl =
{
    {
        &IID_ITalkSessionManager,
    },
    {
        IUnknown_QueryInterface_Proxy,
        IUnknown_AddRef_Proxy,
        IUnknown_Release_Proxy,
        ITalkSessionManager_CreateSession_Proxy
    }
};


static const PRPC_STUB_FUNCTION ITalkSessionManager_table[] =
{
    ITalkSessionManager_CreateSession_Stub
};

static const CInterfaceStubVtbl _ITalkSessionManagerStubVtbl =
{
    {
        &IID_ITalkSessionManager,
        0,
        4,
        &ITalkSessionManager_table[-3],
    },
    {
        CStdStubBuffer_METHODS
    }
};

static const MIDL_STUB_DESC Object_StubDesc =
{
    0,
    NdrOleAllocate,
    NdrOleFree,
    {0}, 0, 0, 0, 0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x10001, /* Ndr library version */
    0,
    0x50100a4, /* MIDL Version 5.1.164 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    1,  /* Flags */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
};

#if !defined(__RPC_WIN32__)
#error Currently only Wine and WIN32 are supported.
#endif

static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
{
    0,
    {
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x4),
        0x53,    /* FC_RETURN_PARAM_BASETYPE */
        0x08,    /* FC_LONG */
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x6),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x18),
        0x53,    /* FC_RETURN_PARAM_BASETYPE */
        0x08,    /* FC_LONG */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x09,    /* FC_ULONG */
        0x53,    /* FC_RETURN_PARAM_BASETYPE */
        0x08,    /* FC_LONG */
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x1e),
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x22),
        0x53,    /* FC_RETURN_PARAM_BASETYPE */
        0x08,    /* FC_LONG */
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x36),
        0x53,    /* FC_RETURN_PARAM_BASETYPE */
        0x08,    /* FC_LONG */
        0x0
    }
};

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
{
    0,
    {
        NdrFcShort(0x0),
/* 2 (const OLECHAR *) */
        0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */
        0x25,    /* FC_C_WSTRING */
        0x5c, /* FC_PAD */
/* 6 (ITalkSessionEvents *) */
        0x2f,	/* FC_IP */
        0x5a,	/* FC_CONSTANT_IID */
        NdrFcLong(0xa7ccbb09),
        NdrFcShort(0x7d0b),
        NdrFcShort(0x4318),
        0xba,
        0xe6,
        0x4b,
        0xcc,
        0x8a,
        0xb3,
        0x0c,
        0xa7,

        0x11, 0xc,    /* FC_RP [allocated_on_stack] [simple_pointer] */
        0x09,    /* FC_ULONG */
        0x5c,          /* FC_PAD */
/* 28 (const OLECHAR *) */
        0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 30 */
        0x25,    /* FC_C_WSTRING */
        0x5c, /* FC_PAD */
/* 32 (const OLECHAR *) */
        0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 34 */
        0x25,    /* FC_C_WSTRING */
        0x5c, /* FC_PAD */
/* 36 (ITalkSession *) */
        0x2f,	/* FC_IP */
        0x5a,	/* FC_CONSTANT_IID */
        NdrFcLong(0xa7ccbb08),
        NdrFcShort(0x7d0b),
        NdrFcShort(0x4318),
        0xba,
        0xe6,
        0x4b,
        0xcc,
        0x8a,
        0xb3,
        0x0c,
        0xa7,

/* 54 */
        0x11, 0x14,		/* FC_RP [allocated_on_stack] [pointer_deref] */
        NdrFcShort(0xffffffec),	/* 36 */
        0x0
    }
};

static const CInterfaceProxyVtbl* const _talk_ProxyVtblList[] =
{
    (const CInterfaceProxyVtbl*)&_ITalkSessionProxyVtbl,
    (const CInterfaceProxyVtbl*)&_ITalkSessionEventsProxyVtbl,
    (const CInterfaceProxyVtbl*)&_ITalkSessionManagerProxyVtbl,
    0
};

static const CInterfaceStubVtbl* const _talk_StubVtblList[] =
{
    (const CInterfaceStubVtbl*)&_ITalkSessionStubVtbl,
    (const CInterfaceStubVtbl*)&_ITalkSessionEventsStubVtbl,
    (const CInterfaceStubVtbl*)&_ITalkSessionManagerStubVtbl,
    0
};

static PCInterfaceName const _talk_InterfaceNamesList[] =
{
    "ITalkSession",
    "ITalkSessionEvents",
    "ITalkSessionManager",
    0
};

#define _talk_CHECK_IID(n) IID_GENERIC_CHECK_IID(_talk, pIID, n)

int __stdcall _talk_IID_Lookup(const IID* pIID, int* pIndex)
{
    if (!_talk_CHECK_IID(0))
    {
        *pIndex = 0;
        return 1;
    }
    if (!_talk_CHECK_IID(1))
    {
        *pIndex = 1;
        return 1;
    }
    if (!_talk_CHECK_IID(2))
    {
        *pIndex = 2;
        return 1;
    }
    return 0;
}

const ExtendedProxyFileInfo talk_ProxyFileInfo =
{
    (const PCInterfaceProxyVtblList*)&_talk_ProxyVtblList,
    (const PCInterfaceStubVtblList*)&_talk_StubVtblList,
    _talk_InterfaceNamesList,
    0,
    &_talk_IID_Lookup,
    3,
    1,
    0,
    0,
    0,
    0
};
