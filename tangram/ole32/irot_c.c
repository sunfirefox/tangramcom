/*** Autogenerated by WIDL 1.1.6 from irot.idl - Do not edit ***/
#include <string.h>
#ifdef _ALPHA_
#include <stdarg.h>
#endif

#include "irot.h"

#ifndef USE_COMPILER_EXCEPTIONS

#define RpcTryExcept if (1) {
#define RpcExcept(expr) } else {
#define RpcEndExcept }
#define RpcTryFinally
#define RpcFinally
#define RpcEndFinally
#define RpcExceptionCode() 0

#define RpcExceptionInit(filter_func,finally_func) do {} while(0)
#define __DECL_EXCEPTION_FRAME

#else /* USE_COMPILER_EXCEPTIONS */

#define RpcExceptionInit(filter_func,finally_func) do {} while(0)
#define __DECL_EXCEPTION_FRAME

#endif /* USE_COMPILER_EXCEPTIONS */

#define TYPE_FORMAT_STRING_SIZE 241
#define PROC_FORMAT_STRING_SIZE 103

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

/*****************************************************************************
 * Irot interface
 */

static const RPC_CLIENT_INTERFACE Irot___RpcClientInterface =
{
    sizeof(RPC_CLIENT_INTERFACE),
    {{0x7a98c254,0x6808,0x11cf,{0xb7,0x3b,0x00,0xaa,0x00,0xb6,0x77,0xa8}},{0,2}},
    {{0x8a885d04,0x1ceb,0x11c9,{0x9f,0xe8,0x08,0x00,0x2b,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0,
    0,
};
RPC_IF_HANDLE Irot_v0_2_c_ifspec = (RPC_IF_HANDLE)& Irot___RpcClientInterface;

static const MIDL_STUB_DESC Irot_StubDesc;

static RPC_BINDING_HANDLE Irot__MIDL_AutoBindHandle;

struct __frame_IrotRegister
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_BINDING_HANDLE _Handle;
};

static void __finally_IrotRegister( struct __frame_IrotRegister *__frame )
{
    NdrFreeBuffer(&__frame->_StubMsg);
}

HRESULT IrotRegister(
    IrotHandle h,
    const MonikerComparisonData *moniker_data,
    const InterfaceData *object,
    const InterfaceData *moniker,
    const FILETIME *time,
    DWORD grfFlags,
    IrotCookie *cookie,
    IrotContextHandle *ctxt_handle)
{
    struct __frame_IrotRegister __f, * const __frame = &__f;
    HRESULT _RetVal;
    RPC_MESSAGE _RpcMessage;
    __frame->_Handle = 0;

    RpcExceptionInit( 0, __finally_IrotRegister );
    if (!moniker_data)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!object)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!moniker)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!time)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!cookie)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!ctxt_handle)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    RpcTryFinally
    {
        NdrClientInitializeNew(&_RpcMessage, &__frame->_StubMsg, &Irot_StubDesc, 0);
        __frame->_Handle = h;

        __frame->_StubMsg.BufferLength = 48;

        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)moniker_data,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[20]);

        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)object,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[42]);

        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)moniker,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[54]);

        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)time,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[66]);


        NdrGetBuffer(&__frame->_StubMsg, __frame->_StubMsg.BufferLength, __frame->_Handle);


        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)moniker_data,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[20]);

        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)object,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[42]);

        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)moniker,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[54]);

        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)time,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[66]);

        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(DWORD *)__frame->_StubMsg.Buffer = grfFlags;
        __frame->_StubMsg.Buffer += sizeof(DWORD);

        NdrSendReceive(&__frame->_StubMsg, __frame->_StubMsg.Buffer);

        __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
        __frame->_StubMsg.BufferEnd = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

        if ((_RpcMessage.DataRepresentation & 0x0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert(&__frame->_StubMsg, (PFORMAT_STRING)&__MIDL_ProcFormatString.Format[0]);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&cookie,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[70],
            0);

        *ctxt_handle = 0;
        NdrClientContextUnmarshall(
            &__frame->_StubMsg,
            (NDR_CCONTEXT *)ctxt_handle,
            __frame->_Handle);

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
        __finally_IrotRegister( __frame );
    }
    RpcEndFinally

    return _RetVal;
}

struct __frame_IrotRevoke
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_BINDING_HANDLE _Handle;
};

static void __finally_IrotRevoke( struct __frame_IrotRevoke *__frame )
{
    NdrFreeBuffer(&__frame->_StubMsg);
}

HRESULT IrotRevoke(
    IrotHandle h,
    IrotCookie cookie,
    IrotContextHandle *ctxt_handle,
    PInterfaceData *object,
    PInterfaceData *moniker)
{
    struct __frame_IrotRevoke __f, * const __frame = &__f;
    HRESULT _RetVal;
    RPC_MESSAGE _RpcMessage;
    __frame->_Handle = 0;

    RpcExceptionInit( 0, __finally_IrotRevoke );
    if (!ctxt_handle)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!object)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!moniker)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    RpcTryFinally
    {
        NdrClientInitializeNew(&_RpcMessage, &__frame->_StubMsg, &Irot_StubDesc, 1);
        __frame->_Handle = h;

        __frame->_StubMsg.BufferLength = 32;



        NdrGetBuffer(&__frame->_StubMsg, __frame->_StubMsg.BufferLength, __frame->_Handle);


        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(IrotCookie *)__frame->_StubMsg.Buffer = cookie;
        __frame->_StubMsg.Buffer += sizeof(IrotCookie);

        NdrClientContextMarshall(
            &__frame->_StubMsg,
            (NDR_CCONTEXT)*ctxt_handle,
            1);

        NdrSendReceive(&__frame->_StubMsg, __frame->_StubMsg.Buffer);

        __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
        __frame->_StubMsg.BufferEnd = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

        if ((_RpcMessage.DataRepresentation & 0x0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert(&__frame->_StubMsg, (PFORMAT_STRING)&__MIDL_ProcFormatString.Format[30]);

        NdrClientContextUnmarshall(
            &__frame->_StubMsg,
            (NDR_CCONTEXT *)ctxt_handle,
            __frame->_Handle);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&object,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[94],
            0);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&moniker,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[110],
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
        __finally_IrotRevoke( __frame );
    }
    RpcEndFinally

    return _RetVal;
}

struct __frame_IrotIsRunning
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_BINDING_HANDLE _Handle;
};

static void __finally_IrotIsRunning( struct __frame_IrotIsRunning *__frame )
{
    NdrFreeBuffer(&__frame->_StubMsg);
}

HRESULT IrotIsRunning(
    IrotHandle h,
    const MonikerComparisonData *moniker_data)
{
    struct __frame_IrotIsRunning __f, * const __frame = &__f;
    HRESULT _RetVal;
    RPC_MESSAGE _RpcMessage;
    __frame->_Handle = 0;

    RpcExceptionInit( 0, __finally_IrotIsRunning );
    if (!moniker_data)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    RpcTryFinally
    {
        NdrClientInitializeNew(&_RpcMessage, &__frame->_StubMsg, &Irot_StubDesc, 2);
        __frame->_Handle = h;

        __frame->_StubMsg.BufferLength = 0;

        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)moniker_data,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[122]);

        NdrGetBuffer(&__frame->_StubMsg, __frame->_StubMsg.BufferLength, __frame->_Handle);


        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)moniker_data,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[122]);

        NdrSendReceive(&__frame->_StubMsg, __frame->_StubMsg.Buffer);

        __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
        __frame->_StubMsg.BufferEnd = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

        if ((_RpcMessage.DataRepresentation & 0x0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert(&__frame->_StubMsg, (PFORMAT_STRING)&__MIDL_ProcFormatString.Format[48]);

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
        __finally_IrotIsRunning( __frame );
    }
    RpcEndFinally

    return _RetVal;
}

struct __frame_IrotGetObject
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_BINDING_HANDLE _Handle;
};

static void __finally_IrotGetObject( struct __frame_IrotGetObject *__frame )
{
    NdrFreeBuffer(&__frame->_StubMsg);
}

HRESULT IrotGetObject(
    IrotHandle h,
    const MonikerComparisonData *moniker_data,
    PInterfaceData *obj,
    IrotCookie *cookie)
{
    struct __frame_IrotGetObject __f, * const __frame = &__f;
    HRESULT _RetVal;
    RPC_MESSAGE _RpcMessage;
    __frame->_Handle = 0;

    RpcExceptionInit( 0, __finally_IrotGetObject );
    if (!moniker_data)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!obj)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!cookie)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    RpcTryFinally
    {
        NdrClientInitializeNew(&_RpcMessage, &__frame->_StubMsg, &Irot_StubDesc, 3);
        __frame->_Handle = h;

        __frame->_StubMsg.BufferLength = 8;

        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)moniker_data,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[134]);

        NdrGetBuffer(&__frame->_StubMsg, __frame->_StubMsg.BufferLength, __frame->_Handle);


        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)moniker_data,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[134]);

        NdrSendReceive(&__frame->_StubMsg, __frame->_StubMsg.Buffer);

        __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
        __frame->_StubMsg.BufferEnd = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

        if ((_RpcMessage.DataRepresentation & 0x0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert(&__frame->_StubMsg, (PFORMAT_STRING)&__MIDL_ProcFormatString.Format[56]);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&obj,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[150],
            0);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&cookie,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[154],
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
        __finally_IrotGetObject( __frame );
    }
    RpcEndFinally

    return _RetVal;
}

struct __frame_IrotNoteChangeTime
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_BINDING_HANDLE _Handle;
};

static void __finally_IrotNoteChangeTime( struct __frame_IrotNoteChangeTime *__frame )
{
    NdrFreeBuffer(&__frame->_StubMsg);
}

HRESULT IrotNoteChangeTime(
    IrotHandle h,
    IrotCookie cookie,
    const FILETIME *time)
{
    struct __frame_IrotNoteChangeTime __f, * const __frame = &__f;
    HRESULT _RetVal;
    RPC_MESSAGE _RpcMessage;
    __frame->_Handle = 0;

    RpcExceptionInit( 0, __finally_IrotNoteChangeTime );
    if (!time)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    RpcTryFinally
    {
        NdrClientInitializeNew(&_RpcMessage, &__frame->_StubMsg, &Irot_StubDesc, 4);
        __frame->_Handle = h;

        __frame->_StubMsg.BufferLength = 16;


        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)time,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[166]);

        NdrGetBuffer(&__frame->_StubMsg, __frame->_StubMsg.BufferLength, __frame->_Handle);


        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(IrotCookie *)__frame->_StubMsg.Buffer = cookie;
        __frame->_StubMsg.Buffer += sizeof(IrotCookie);

        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)time,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[166]);

        NdrSendReceive(&__frame->_StubMsg, __frame->_StubMsg.Buffer);

        __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
        __frame->_StubMsg.BufferEnd = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

        if ((_RpcMessage.DataRepresentation & 0x0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert(&__frame->_StubMsg, (PFORMAT_STRING)&__MIDL_ProcFormatString.Format[72]);

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
        __finally_IrotNoteChangeTime( __frame );
    }
    RpcEndFinally

    return _RetVal;
}

struct __frame_IrotGetTimeOfLastChange
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_BINDING_HANDLE _Handle;
};

static void __finally_IrotGetTimeOfLastChange( struct __frame_IrotGetTimeOfLastChange *__frame )
{
    NdrFreeBuffer(&__frame->_StubMsg);
}

HRESULT IrotGetTimeOfLastChange(
    IrotHandle h,
    const MonikerComparisonData *moniker_data,
    FILETIME *time)
{
    struct __frame_IrotGetTimeOfLastChange __f, * const __frame = &__f;
    HRESULT _RetVal;
    RPC_MESSAGE _RpcMessage;
    __frame->_Handle = 0;

    RpcExceptionInit( 0, __finally_IrotGetTimeOfLastChange );
    if (!moniker_data)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!time)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    RpcTryFinally
    {
        NdrClientInitializeNew(&_RpcMessage, &__frame->_StubMsg, &Irot_StubDesc, 5);
        __frame->_Handle = h;

        __frame->_StubMsg.BufferLength = 8;

        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)moniker_data,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[178]);

        NdrGetBuffer(&__frame->_StubMsg, __frame->_StubMsg.BufferLength, __frame->_Handle);


        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)moniker_data,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[178]);

        NdrSendReceive(&__frame->_StubMsg, __frame->_StubMsg.Buffer);

        __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
        __frame->_StubMsg.BufferEnd = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

        if ((_RpcMessage.DataRepresentation & 0x0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert(&__frame->_StubMsg, (PFORMAT_STRING)&__MIDL_ProcFormatString.Format[82]);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&time,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[190],
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
        __finally_IrotGetTimeOfLastChange( __frame );
    }
    RpcEndFinally

    return _RetVal;
}

struct __frame_IrotEnumRunning
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_BINDING_HANDLE _Handle;
};

static void __finally_IrotEnumRunning( struct __frame_IrotEnumRunning *__frame )
{
    NdrFreeBuffer(&__frame->_StubMsg);
}

HRESULT IrotEnumRunning(
    IrotHandle h,
    PInterfaceList *list)
{
    struct __frame_IrotEnumRunning __f, * const __frame = &__f;
    HRESULT _RetVal;
    RPC_MESSAGE _RpcMessage;
    __frame->_Handle = 0;

    RpcExceptionInit( 0, __finally_IrotEnumRunning );
    if (!list)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    RpcTryFinally
    {
        NdrClientInitializeNew(&_RpcMessage, &__frame->_StubMsg, &Irot_StubDesc, 6);
        __frame->_Handle = h;

        __frame->_StubMsg.BufferLength = 0;

        NdrGetBuffer(&__frame->_StubMsg, __frame->_StubMsg.BufferLength, __frame->_Handle);


        NdrSendReceive(&__frame->_StubMsg, __frame->_StubMsg.Buffer);

        __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
        __frame->_StubMsg.BufferEnd = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

        if ((_RpcMessage.DataRepresentation & 0x0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert(&__frame->_StubMsg, (PFORMAT_STRING)&__MIDL_ProcFormatString.Format[94]);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&list,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[236],
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
        __finally_IrotEnumRunning( __frame );
    }
    RpcEndFinally

    return _RetVal;
}

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

static const MIDL_STUB_DESC Irot_StubDesc =
{
    (void *)& Irot___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    {
        &Irot__MIDL_AutoBindHandle,
    },
    0,
    0,
    0,
    0,
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


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
{
    0,
    {
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x0f,    /* FC_IGNORE */
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x14),
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x2a),
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x36),
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x42),
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x09,    /* FC_ULONG */
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x46),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x4a),
        0x53,    /* FC_RETURN_PARAM_BASETYPE */
        0x08,    /* FC_LONG */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x0f,    /* FC_IGNORE */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x09,    /* FC_ULONG */
        0x50,    /* FC_IN_OUT_PARAM */
        0x01,
        NdrFcShort(0x4e),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x5e),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x6e),
        0x53,    /* FC_RETURN_PARAM_BASETYPE */
        0x08,    /* FC_LONG */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x0f,    /* FC_IGNORE */
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x7a),
        0x53,    /* FC_RETURN_PARAM_BASETYPE */
        0x08,    /* FC_LONG */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x0f,    /* FC_IGNORE */
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x86),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x96),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x9a),
        0x53,    /* FC_RETURN_PARAM_BASETYPE */
        0x08,    /* FC_LONG */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x0f,    /* FC_IGNORE */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x09,    /* FC_ULONG */
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0xa6),
        0x53,    /* FC_RETURN_PARAM_BASETYPE */
        0x08,    /* FC_LONG */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x0f,    /* FC_IGNORE */
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0xb2),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0xbe),
        0x53,    /* FC_RETURN_PARAM_BASETYPE */
        0x08,    /* FC_LONG */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x0f,    /* FC_IGNORE */
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0xec),
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
/* 2 (BYTE[]) */
        0x1b,	/* FC_CARRAY */
        0x0,	/* 0 */
        NdrFcShort(0x1),	/* 1 */
        0x9, /* Corr desc: FC_ULONG */
        0x0, /* no operators */
        NdrFcShort(0xfffffffc), /* offset = -4 */
        0x02,	/* FC_CHAR */
        0x5b,		/* FC_END */
/* 12 (const MonikerComparisonData) */
        0x17,	/* FC_CSTRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x4),	/* 4 */
        NdrFcShort(0xfff2),	/* Offset= -14 (2) */
        0x09,	/* FC_ULONG */
        0x5b,		/* FC_END */
/* 20 */
        0x11, 0x0,		/* FC_RP */
        NdrFcShort(0xfffffff6),	/* 12 */
/* 24 (BYTE[]) */
        0x1b,	/* FC_CARRAY */
        0x0,	/* 0 */
        NdrFcShort(0x1),	/* 1 */
        0x9, /* Corr desc: FC_ULONG */
        0x0, /* no operators */
        NdrFcShort(0xfffffffc), /* offset = -4 */
        0x02,	/* FC_CHAR */
        0x5b,		/* FC_END */
/* 34 (const InterfaceData) */
        0x17,	/* FC_CSTRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x4),	/* 4 */
        NdrFcShort(0xfff2),	/* Offset= -14 (24) */
        0x09,	/* FC_ULONG */
        0x5b,		/* FC_END */
/* 42 */
        0x11, 0x0,		/* FC_RP */
        NdrFcShort(0xfffffff6),	/* 34 */
/* 46 (const InterfaceData) */
        0x17,	/* FC_CSTRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x4),	/* 4 */
        NdrFcShort(0xffe6),	/* Offset= -26 (24) */
        0x09,	/* FC_ULONG */
        0x5b,		/* FC_END */
/* 54 */
        0x11, 0x0,		/* FC_RP */
        NdrFcShort(0xfffffff6),	/* 46 */
/* 58 (const FILETIME) */
        0x15,	/* FC_STRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x8),	/* 8 */
        0x09,	/* FC_ULONG */
        0x09,	/* FC_ULONG */
        0x5c,		/* FC_PAD */
        0x5b,		/* FC_END */
/* 66 */
        0x11, 0x0,		/* FC_RP */
        NdrFcShort(0xfffffff6),	/* 58 */
        0x11, 0xc,    /* FC_RP [allocated_on_stack] [simple_pointer] */
        0x09,    /* FC_ULONG */
        0x5c,          /* FC_PAD */
/* 74 */
        0x30,    /* FC_BIND_CONTEXT */
        0xa8,	/* Context flags: strict, out, via ptr, */
        0, /* FIXME: rundown routine index*/
        0, /* FIXME: param num */
/* 78 */
        0x30,    /* FC_BIND_CONTEXT */
        0xe8,	/* Context flags: strict, out, in, via ptr, */
        0, /* FIXME: rundown routine index*/
        0, /* FIXME: param num */
/* 82 (InterfaceData) */
        0x17,	/* FC_CSTRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x4),	/* 4 */
        NdrFcShort(0xffc2),	/* Offset= -62 (24) */
        0x09,	/* FC_ULONG */
        0x5b,		/* FC_END */
/* 90 */
        0x12, 0x0,		/* FC_UP */
        NdrFcShort(0xfffffff6),	/* 82 */
/* 94 */
        0x11, 0x14,		/* FC_RP [allocated_on_stack] [pointer_deref] */
        NdrFcShort(0xfffffffa),	/* 90 */
/* 98 (InterfaceData) */
        0x17,	/* FC_CSTRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x4),	/* 4 */
        NdrFcShort(0xffb2),	/* Offset= -78 (24) */
        0x09,	/* FC_ULONG */
        0x5b,		/* FC_END */
/* 106 */
        0x12, 0x0,		/* FC_UP */
        NdrFcShort(0xfffffff6),	/* 98 */
/* 110 */
        0x11, 0x14,		/* FC_RP [allocated_on_stack] [pointer_deref] */
        NdrFcShort(0xfffffffa),	/* 106 */
/* 114 (const MonikerComparisonData) */
        0x17,	/* FC_CSTRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x4),	/* 4 */
        NdrFcShort(0xff8c),	/* Offset= -116 (2) */
        0x09,	/* FC_ULONG */
        0x5b,		/* FC_END */
/* 122 */
        0x11, 0x0,		/* FC_RP */
        NdrFcShort(0xfffffff6),	/* 114 */
/* 126 (const MonikerComparisonData) */
        0x17,	/* FC_CSTRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x4),	/* 4 */
        NdrFcShort(0xff80),	/* Offset= -128 (2) */
        0x09,	/* FC_ULONG */
        0x5b,		/* FC_END */
/* 134 */
        0x11, 0x0,		/* FC_RP */
        NdrFcShort(0xfffffff6),	/* 126 */
/* 138 (InterfaceData) */
        0x17,	/* FC_CSTRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x4),	/* 4 */
        NdrFcShort(0xff8a),	/* Offset= -118 (24) */
        0x09,	/* FC_ULONG */
        0x5b,		/* FC_END */
/* 146 */
        0x12, 0x0,		/* FC_UP */
        NdrFcShort(0xfffffff6),	/* 138 */
/* 150 */
        0x11, 0x14,		/* FC_RP [allocated_on_stack] [pointer_deref] */
        NdrFcShort(0xfffffffa),	/* 146 */
        0x11, 0xc,    /* FC_RP [allocated_on_stack] [simple_pointer] */
        0x09,    /* FC_ULONG */
        0x5c,          /* FC_PAD */
/* 158 (const FILETIME) */
        0x15,	/* FC_STRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x8),	/* 8 */
        0x09,	/* FC_ULONG */
        0x09,	/* FC_ULONG */
        0x5c,		/* FC_PAD */
        0x5b,		/* FC_END */
/* 166 */
        0x11, 0x0,		/* FC_RP */
        NdrFcShort(0xfffffff6),	/* 158 */
/* 170 (const MonikerComparisonData) */
        0x17,	/* FC_CSTRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x4),	/* 4 */
        NdrFcShort(0xff54),	/* Offset= -172 (2) */
        0x09,	/* FC_ULONG */
        0x5b,		/* FC_END */
/* 178 */
        0x11, 0x0,		/* FC_RP */
        NdrFcShort(0xfffffff6),	/* 170 */
/* 182 (FILETIME) */
        0x15,	/* FC_STRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x8),	/* 8 */
        0x09,	/* FC_ULONG */
        0x09,	/* FC_ULONG */
        0x5c,		/* FC_PAD */
        0x5b,		/* FC_END */
/* 190 */
        0x11, 0x4,		/* FC_RP [allocated_on_stack] */
        NdrFcShort(0xfffffff6),	/* 182 */
/* 194 (PInterfaceData[]) */
        0x1b,	/* FC_CARRAY */
        0x3,	/* 3 */
        NdrFcShort(0x4),	/* 4 */
        0x9, /* Corr desc: FC_ULONG */
        0x0, /* no operators */
        NdrFcShort(0xfffffffc), /* offset = -4 */
        0x8,	/* FC_LONG */
        0x5b,		/* FC_END */
/* 204 (InterfaceList) */
        0x18,	/* FC_CPSTRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x4),	/* 4 */
        NdrFcShort(0xfff2),	/* Offset= -14 (194) */
        0x4b, /* FC_PP */
        0x5c, /* FC_PAD */
        0x48, /* FC_VARIABLE_REPEAT */
        0x49, /* FC_FIXED_OFFSET */
        NdrFcShort(0x4), /* Increment = 4 */
        NdrFcShort(0x4), /* Offset to array = 4 */
        NdrFcShort(0x1), /* Number of pointers = 1 */
        NdrFcShort(0x4), /* Memory offset = 4 */
        NdrFcShort(0x4), /* Buffer offset = 4 */
/* 224 (PInterfaceData) */
        0x12, 0x0,	/* FC_UP */
        NdrFcShort(0xffa8),	/* Offset= -88 (138) */
        0x5b, /* FC_END */
        0x09,	/* FC_ULONG */
        0x5c,		/* FC_PAD */
        0x5b,		/* FC_END */
/* 232 */
        0x12, 0x0,		/* FC_UP */
        NdrFcShort(0xffffffe2),	/* 204 */
/* 236 */
        0x11, 0x14,		/* FC_RP [allocated_on_stack] [pointer_deref] */
        NdrFcShort(0xfffffffa),	/* 232 */
        0x0
    }
};

