/*** Autogenerated by WIDL 1.1.6 from ole32_unknwn.idl - Do not edit ***/

#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif /* __REDQ_RPCPROXY_H_VERSION__ */

#define __midl_proxy
#include "objbase.h"
#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error This code needs a newer version of rpcproxy.h
#endif /* __RPCPROXY_H_VERSION__ */

#include "ole32_unknwn.h"

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(comps);
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

#define TYPE_FORMAT_STRING_SIZE 35
#define PROC_FORMAT_STRING_SIZE 15

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
 * IClassFactory interface
 */
static void __finally_IClassFactory_RemoteCreateInstance_Proxy( struct __proxy_frame *__frame )
{
    NdrProxyFreeBuffer( __frame->This, &__frame->_StubMsg );
}

HRESULT STDMETHODCALLTYPE IClassFactory_RemoteCreateInstance_Proxy(
    IClassFactory* This,
    REFIID riid,
    IUnknown **ppvObject)
{
    struct __proxy_frame __f, * const __frame = &__f;
    HRESULT     _RetVal;
    RPC_MESSAGE _RpcMessage;

    RpcExceptionInit( __proxy_filter, __finally_IClassFactory_RemoteCreateInstance_Proxy );
    __frame->This = This;
    FB_TRACE("> \n");
    if(ppvObject)
        MIDL_memset( ppvObject, 0, sizeof( *ppvObject ));
    RpcTryExcept
    {
        NdrProxyInitialize(This, &_RpcMessage, &__frame->_StubMsg, &Object_StubDesc, 3);
        if(!riid)
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
        if(!ppvObject)
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
        RpcTryFinally
        {
            __frame->_StubMsg.BufferLength = 16;
            NdrPointerBufferSize(
                &__frame->_StubMsg,
                (unsigned char *)riid,
                (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[20]);

            NdrProxyGetBuffer(This, &__frame->_StubMsg);
            NdrPointerMarshall(
                &__frame->_StubMsg,
                (unsigned char *)riid,
                (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[20]);

            NdrProxySendReceive(This, &__frame->_StubMsg);

            __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
            __frame->_StubMsg.BufferEnd   = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

            if ((_RpcMessage.DataRepresentation & 0xffff) != NDR_LOCAL_DATA_REPRESENTATION)
                NdrConvert( &__frame->_StubMsg, &__MIDL_ProcFormatString.Format[0]);

            NdrPointerUnmarshall(
                &__frame->_StubMsg,
                (unsigned char **)&ppvObject,
                (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[30],
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
            __finally_IClassFactory_RemoteCreateInstance_Proxy( __frame );
        }
        RpcEndFinally
    }
    RpcExcept(__frame->_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
    {
        /* FIXME: free_variable code for ppvObject type 17 missing */

        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
    }
    RpcEndExcept
    FE_TRACE("< \n");
    return _RetVal;
}

struct __frame_IClassFactory_RemoteCreateInstance_Stub
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    IClassFactory * _This;
    HRESULT _RetVal;
    REFIID riid;
    IUnknown *_W0;
    IUnknown ** ppvObject;
};

static void __finally_IClassFactory_RemoteCreateInstance_Stub( struct __frame_IClassFactory_RemoteCreateInstance_Stub *__frame )
{
    NdrPointerFree(
        &__frame->_StubMsg,
        (unsigned char *)__frame->riid,
        (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[20]);

    __frame->_StubMsg.MaxCount = (unsigned long) __frame->riid;

    NdrPointerFree(
        &__frame->_StubMsg,
        (unsigned char *)__frame->ppvObject,
        (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[30]);

}

void __RPC_STUB IClassFactory_RemoteCreateInstance_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD* _pdwStubPhase)
{
    struct __frame_IClassFactory_RemoteCreateInstance_Stub __f, * const __frame = &__f;

    __frame->_This = (IClassFactory*)((CStdStubBuffer*)This)->pvServerObject;

    FB_TRACE("< \n");
    NdrStubInitialize(_pRpcMessage, &__frame->_StubMsg, &Object_StubDesc, _pRpcChannelBuffer);

    RpcExceptionInit( 0, __finally_IClassFactory_RemoteCreateInstance_Stub );
    __frame->riid = 0;
    __frame->ppvObject = 0;

    RpcTryFinally
    {
        if ((_pRpcMessage->DataRepresentation & 0xffff) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert( &__frame->_StubMsg, &__MIDL_ProcFormatString.Format[0]);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&__frame->riid,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[20],
            0);


        __frame->ppvObject = &__frame->_W0;
        __frame->_W0 = 0;

        *_pdwStubPhase = STUB_CALL_SERVER;

        __frame->_RetVal = IClassFactory_CreateInstance_Stub(__frame->_This, __frame->riid, __frame->ppvObject);

        *_pdwStubPhase = STUB_MARSHAL;

        __frame->_StubMsg.BufferLength = 8;
        __frame->_StubMsg.MaxCount = (unsigned long) __frame->riid;

        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)__frame->ppvObject,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[30]);


        NdrStubGetBuffer(This, _pRpcChannelBuffer, &__frame->_StubMsg);
        __frame->_StubMsg.MaxCount = (unsigned long) __frame->riid;

        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)__frame->ppvObject,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[30]);


        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(HRESULT *)__frame->_StubMsg.Buffer = __frame->_RetVal;
        __frame->_StubMsg.Buffer += sizeof(HRESULT);

    }
    RpcFinally
    {
        __finally_IClassFactory_RemoteCreateInstance_Stub( __frame );
    }
    RpcEndFinally
    _pRpcMessage->BufferLength = __frame->_StubMsg.Buffer - (unsigned char *)_pRpcMessage->Buffer;
    FE_TRACE("> \n");
}

static void __finally_IClassFactory_RemoteLockServer_Proxy( struct __proxy_frame *__frame )
{
    NdrProxyFreeBuffer( __frame->This, &__frame->_StubMsg );
}

HRESULT STDMETHODCALLTYPE IClassFactory_RemoteLockServer_Proxy(
    IClassFactory* This,
    BOOL fLock)
{
    struct __proxy_frame __f, * const __frame = &__f;
    HRESULT     _RetVal;
    RPC_MESSAGE _RpcMessage;

    RpcExceptionInit( __proxy_filter, __finally_IClassFactory_RemoteLockServer_Proxy );
    __frame->This = This;
    FB_TRACE("> \n");
    RpcTryExcept
    {
        NdrProxyInitialize(This, &_RpcMessage, &__frame->_StubMsg, &Object_StubDesc, 4);
        RpcTryFinally
        {
            __frame->_StubMsg.BufferLength = 8;

            NdrProxyGetBuffer(This, &__frame->_StubMsg);
            MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
            __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
            *(BOOL *)__frame->_StubMsg.Buffer = fLock;
            __frame->_StubMsg.Buffer += sizeof(BOOL);

            NdrProxySendReceive(This, &__frame->_StubMsg);

            __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
            __frame->_StubMsg.BufferEnd   = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

            if ((_RpcMessage.DataRepresentation & 0xffff) != NDR_LOCAL_DATA_REPRESENTATION)
                NdrConvert( &__frame->_StubMsg, &__MIDL_ProcFormatString.Format[10]);

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
            __finally_IClassFactory_RemoteLockServer_Proxy( __frame );
        }
        RpcEndFinally
    }
    RpcExcept(__frame->_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
    {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
    }
    RpcEndExcept
    FE_TRACE("< \n");
    return _RetVal;
}

struct __frame_IClassFactory_RemoteLockServer_Stub
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    IClassFactory * _This;
    HRESULT _RetVal;
    BOOL fLock;
};

static void __finally_IClassFactory_RemoteLockServer_Stub( struct __frame_IClassFactory_RemoteLockServer_Stub *__frame )
{

}

void __RPC_STUB IClassFactory_RemoteLockServer_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD* _pdwStubPhase)
{
    struct __frame_IClassFactory_RemoteLockServer_Stub __f, * const __frame = &__f;

    __frame->_This = (IClassFactory*)((CStdStubBuffer*)This)->pvServerObject;

    FB_TRACE("< \n");
    NdrStubInitialize(_pRpcMessage, &__frame->_StubMsg, &Object_StubDesc, _pRpcChannelBuffer);

    RpcExceptionInit( 0, __finally_IClassFactory_RemoteLockServer_Stub );

    RpcTryFinally
    {
        if ((_pRpcMessage->DataRepresentation & 0xffff) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert( &__frame->_StubMsg, &__MIDL_ProcFormatString.Format[10]);

        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        if (__frame->_StubMsg.Buffer + sizeof(BOOL) > __frame->_StubMsg.BufferEnd)
        {
            RpcRaiseException(RPC_X_BAD_STUB_DATA);
        }
        __frame->fLock = *(BOOL *)__frame->_StubMsg.Buffer;
        __frame->_StubMsg.Buffer += sizeof(BOOL);


        *_pdwStubPhase = STUB_CALL_SERVER;

        __frame->_RetVal = IClassFactory_LockServer_Stub(__frame->_This, __frame->fLock);

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
        __finally_IClassFactory_RemoteLockServer_Stub( __frame );
    }
    RpcEndFinally
    _pRpcMessage->BufferLength = __frame->_StubMsg.Buffer - (unsigned char *)_pRpcMessage->Buffer;
    FE_TRACE("> \n");
}

static const CINTERFACE_PROXY_VTABLE(5) _IClassFactoryProxyVtbl =
{
    {
        &IID_IClassFactory,
    },
    {
        IUnknown_QueryInterface_Proxy,
        IUnknown_AddRef_Proxy,
        IUnknown_Release_Proxy,
        IClassFactory_CreateInstance_Proxy,
        IClassFactory_LockServer_Proxy
    }
};


static const PRPC_STUB_FUNCTION IClassFactory_table[] =
{
    IClassFactory_RemoteCreateInstance_Stub,
    IClassFactory_RemoteLockServer_Stub
};

static const CInterfaceStubVtbl _IClassFactoryStubVtbl =
{
    {
        &IID_IClassFactory,
        0,
        5,
        &IClassFactory_table[-3],
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
        NdrFcShort(0x14),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x1e),
        0x53,    /* FC_RETURN_PARAM_BASETYPE */
        0x08,    /* FC_LONG */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x08,    /* FC_LONG */
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
/* 2 (unsigned char[8]) */
        0x1d,	/* FC_SMFARRAY */
        0x0,	/* 0 */
        NdrFcShort(0x8),	/* 8 */
        0x02,	/* FC_CHAR */
        0x5b,		/* FC_END */
/* 8 (IID) */
        0x15,	/* FC_STRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x10),	/* 16 */
        0x09,	/* FC_ULONG */
        0x07,	/* FC_USHORT */
        0x07,	/* FC_USHORT */
        0x4c,	/* FC_EMBEDDED_COMPLEX */
        0x0,	/* FIXME: padding */
        NdrFcShort(0xfff1),	/* Offset= -15 (2) */
        0x5b,		/* FC_END */
/* 20 */
        0x11, 0x0,		/* FC_RP */
        NdrFcShort(0xfffffff2),	/* 8 */
        0x2f,  /* FC_IP */
        0x5c,  /* FC_PAD */
        0x20,	/* Corr desc: parameter */
        0x0,
        NdrFcShort(0x0),
/* 30 */
        0x11, 0x14,		/* FC_RP [allocated_on_stack] [pointer_deref] */
        NdrFcShort(0xfffffff8),	/* 24 */
        0x0
    }
};

static const CInterfaceProxyVtbl* const _ole32_unknwn_ProxyVtblList[] =
{
    (const CInterfaceProxyVtbl*)&_IClassFactoryProxyVtbl,
    0
};

static const CInterfaceStubVtbl* const _ole32_unknwn_StubVtblList[] =
{
    (const CInterfaceStubVtbl*)&_IClassFactoryStubVtbl,
    0
};

static PCInterfaceName const _ole32_unknwn_InterfaceNamesList[] =
{
    "IClassFactory",
    0
};

#define _ole32_unknwn_CHECK_IID(n) IID_GENERIC_CHECK_IID(_ole32_unknwn, pIID, n)

int __stdcall _ole32_unknwn_IID_Lookup(const IID* pIID, int* pIndex)
{
    if (!_ole32_unknwn_CHECK_IID(0))
    {
        *pIndex = 0;
        return 1;
    }
    return 0;
}

const ExtendedProxyFileInfo ole32_unknwn_ProxyFileInfo =
{
    (const PCInterfaceProxyVtblList*)&_ole32_unknwn_ProxyVtblList,
    (const PCInterfaceStubVtblList*)&_ole32_unknwn_StubVtblList,
    _ole32_unknwn_InterfaceNamesList,
    0,
    &_ole32_unknwn_IID_Lookup,
    1,
    1,
    0,
    0,
    0,
    0
};
