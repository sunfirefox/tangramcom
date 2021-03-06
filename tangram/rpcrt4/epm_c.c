/*** Autogenerated by WIDL 1.1.6 from epm.idl - Do not edit ***/
#include <string.h>
#ifdef _ALPHA_
#include <stdarg.h>
#endif

#include "epm.h"

#ifndef USE_COMPILER_EXCEPTIONS

#include "wine/exception.h"
#undef RpcTryExcept
#undef RpcExcept
#undef RpcEndExcept
#undef RpcTryFinally
#undef RpcFinally
#undef RpcEndFinally
#undef RpcExceptionCode
#undef RpcAbnormalTermination

struct __exception_frame;
typedef int (*__filter_func)(EXCEPTION_RECORD *, struct __exception_frame *);
typedef void (*__finally_func)(struct __exception_frame *);

#define __DECL_EXCEPTION_FRAME \
    EXCEPTION_REGISTRATION_RECORD frame; \
    __filter_func                 filter; \
    __finally_func                finally; \
    sigjmp_buf                    jmp; \
    DWORD                         code; \
    unsigned char                 abnormal_termination; \
    unsigned char                 filter_level; \
    unsigned char                 finally_level;

struct __exception_frame
{
    __DECL_EXCEPTION_FRAME
};

static DWORD __widl_exception_handler( EXCEPTION_RECORD *record,
                                       EXCEPTION_REGISTRATION_RECORD *frame,
                                       CONTEXT *context,
                                       EXCEPTION_REGISTRATION_RECORD **pdispatcher )
{
    struct __exception_frame *exc_frame = (struct __exception_frame *)frame;

    if (record->ExceptionFlags & (EH_UNWINDING | EH_EXIT_UNWIND | EH_NESTED_CALL))
    {
        if (exc_frame->finally_level && (record->ExceptionFlags & (EH_UNWINDING | EH_EXIT_UNWIND)))
        {
            exc_frame->abnormal_termination = 1;
            exc_frame->finally( exc_frame );
        }
        return ExceptionContinueSearch;
    }
    exc_frame->code = record->ExceptionCode;
    if (exc_frame->filter_level && exc_frame->filter( record, exc_frame ) == EXCEPTION_EXECUTE_HANDLER)
    {
        __wine_rtl_unwind( frame, record );
        if (exc_frame->finally_level > exc_frame->filter_level)
        {
            exc_frame->abnormal_termination = 1;
            exc_frame->finally( exc_frame );
            __wine_pop_frame( frame );
        }
        exc_frame->filter_level = 0;
        siglongjmp( exc_frame->jmp, 1 );
    }
    return ExceptionContinueSearch;
}

#define RpcTryExcept \
    if (!sigsetjmp( __frame->jmp, 0 )) \
    { \
        if (!__frame->finally_level) \
            __wine_push_frame( &__frame->frame ); \
        __frame->filter_level = __frame->finally_level + 1;

#define RpcExcept(expr) \
        if (!__frame->finally_level) \
            __wine_pop_frame( &__frame->frame ); \
        __frame->filter_level = 0; \
    } \
    else \

#define RpcEndExcept

#define RpcExceptionCode() (__frame->code)

#define RpcTryFinally \
    if (!__frame->filter_level) \
        __wine_push_frame( &__frame->frame ); \
    __frame->finally_level = __frame->filter_level + 1;

#define RpcFinally \
    if (!__frame->filter_level) \
        __wine_pop_frame( &__frame->frame ); \
    __frame->finally_level = 0;

#define RpcEndFinally

#define RpcAbnormalTermination() (__frame->abnormal_termination)

#define RpcExceptionInit(filter_func,finally_func) \
    do { \
        __frame->frame.Handler = __widl_exception_handler; \
        __frame->filter = (__filter_func)(filter_func); \
        __frame->finally = (__finally_func)(finally_func); \
        __frame->abnormal_termination = 0; \
        __frame->filter_level = 0; \
        __frame->finally_level = 0; \
    } while (0)

#else /* USE_COMPILER_EXCEPTIONS */

#define RpcExceptionInit(filter_func,finally_func) do {} while(0)
#define __DECL_EXCEPTION_FRAME

#endif /* USE_COMPILER_EXCEPTIONS */

#define TYPE_FORMAT_STRING_SIZE 329
#define PROC_FORMAT_STRING_SIZE 137

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
 * epm interface
 */

static const RPC_CLIENT_INTERFACE epm___RpcClientInterface =
{
    sizeof(RPC_CLIENT_INTERFACE),
    {{0xe1af8308,0x5d1f,0x11c9,{0x91,0xa4,0x08,0x00,0x2b,0x14,0xa0,0xfa}},{3,0}},
    {{0x8a885d04,0x1ceb,0x11c9,{0x9f,0xe8,0x08,0x00,0x2b,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0,
    0,
};
RPC_IF_HANDLE epm_v3_0_c_ifspec = (RPC_IF_HANDLE)& epm___RpcClientInterface;

static const MIDL_STUB_DESC epm_StubDesc;

static RPC_BINDING_HANDLE epm__MIDL_AutoBindHandle;

struct __frame_ept_insert
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_BINDING_HANDLE _Handle;
};

static void __finally_ept_insert( struct __frame_ept_insert *__frame )
{
    NdrFreeBuffer(&__frame->_StubMsg);
}

void ept_insert(
    handle_t h,
    unsigned32 num_ents,
    ept_entry_t entries[],
    boolean32 replace,
    error_status_t *status)
{
    struct __frame_ept_insert __f, * const __frame = &__f;
    RPC_MESSAGE _RpcMessage;
    __frame->_Handle = 0;

    RpcExceptionInit( 0, __finally_ept_insert );
    if (!status)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    RpcTryFinally
    {
        NdrClientInitializeNew(&_RpcMessage, &__frame->_StubMsg, &epm_StubDesc, 0);
        __frame->_Handle = h;

        __frame->_StubMsg.BufferLength = 24;


        __frame->_StubMsg.MaxCount = (unsigned long)num_ents;

        NdrComplexArrayBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)entries,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[64]);


        NdrGetBuffer(&__frame->_StubMsg, __frame->_StubMsg.BufferLength, __frame->_Handle);


        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(unsigned32 *)__frame->_StubMsg.Buffer = num_ents;
        __frame->_StubMsg.Buffer += sizeof(unsigned32);

        __frame->_StubMsg.MaxCount = (unsigned long)num_ents;

        NdrComplexArrayMarshall(
            &__frame->_StubMsg,
            (unsigned char *)entries,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[64]);

        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(boolean32 *)__frame->_StubMsg.Buffer = replace;
        __frame->_StubMsg.Buffer += sizeof(boolean32);

        NdrSendReceive(&__frame->_StubMsg, __frame->_StubMsg.Buffer);

        __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
        __frame->_StubMsg.BufferEnd = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

        if ((_RpcMessage.DataRepresentation & 0x0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert(&__frame->_StubMsg, (PFORMAT_STRING)&__MIDL_ProcFormatString.Format[0]);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&status,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[82],
            0);

    }
    RpcFinally
    {
        __finally_ept_insert( __frame );
    }
    RpcEndFinally
}

struct __frame_ept_delete
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_BINDING_HANDLE _Handle;
};

static void __finally_ept_delete( struct __frame_ept_delete *__frame )
{
    NdrFreeBuffer(&__frame->_StubMsg);
}

void ept_delete(
    handle_t h,
    unsigned32 num_ents,
    ept_entry_t entries[],
    error_status_t *status)
{
    struct __frame_ept_delete __f, * const __frame = &__f;
    RPC_MESSAGE _RpcMessage;
    __frame->_Handle = 0;

    RpcExceptionInit( 0, __finally_ept_delete );
    if (!status)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    RpcTryFinally
    {
        NdrClientInitializeNew(&_RpcMessage, &__frame->_StubMsg, &epm_StubDesc, 1);
        __frame->_Handle = h;

        __frame->_StubMsg.BufferLength = 16;


        __frame->_StubMsg.MaxCount = (unsigned long)num_ents;

        NdrComplexArrayBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)entries,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[86]);

        NdrGetBuffer(&__frame->_StubMsg, __frame->_StubMsg.BufferLength, __frame->_Handle);


        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(unsigned32 *)__frame->_StubMsg.Buffer = num_ents;
        __frame->_StubMsg.Buffer += sizeof(unsigned32);

        __frame->_StubMsg.MaxCount = (unsigned long)num_ents;

        NdrComplexArrayMarshall(
            &__frame->_StubMsg,
            (unsigned char *)entries,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[86]);

        NdrSendReceive(&__frame->_StubMsg, __frame->_StubMsg.Buffer);

        __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
        __frame->_StubMsg.BufferEnd = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

        if ((_RpcMessage.DataRepresentation & 0x0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert(&__frame->_StubMsg, (PFORMAT_STRING)&__MIDL_ProcFormatString.Format[16]);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&status,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[104],
            0);

    }
    RpcFinally
    {
        __finally_ept_delete( __frame );
    }
    RpcEndFinally
}

struct __frame_ept_lookup
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_BINDING_HANDLE _Handle;
};

static void __finally_ept_lookup( struct __frame_ept_lookup *__frame )
{
    NdrFullPointerXlatFree(__frame->_StubMsg.FullPtrXlatTables);

    NdrFreeBuffer(&__frame->_StubMsg);
}

void ept_lookup(
    handle_t h,
    unsigned32 inquiry_type,
    uuid_p_t object,
    rpc_if_id_p_t interface_id,
    unsigned32 vers_option,
    ept_lookup_handle_t *entry_handle,
    unsigned32 max_ents,
    unsigned32 *num_ents,
    ept_entry_t entries[],
    error_status_t *status)
{
    struct __frame_ept_lookup __f, * const __frame = &__f;
    RPC_MESSAGE _RpcMessage;
    __frame->_Handle = 0;

    RpcExceptionInit( 0, __finally_ept_lookup );
    __frame->_StubMsg.FullPtrXlatTables = NdrFullPointerXlatInit(0,XLAT_CLIENT);

    if (!entry_handle)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!num_ents)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!status)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    RpcTryFinally
    {
        NdrClientInitializeNew(&_RpcMessage, &__frame->_StubMsg, &epm_StubDesc, 2);
        _RpcMessage.RpcFlags = ( RPC_NCA_FLAGS_DEFAULT | RPC_NCA_FLAGS_IDEMPOTENT );

        __frame->_Handle = h;

        __frame->_StubMsg.BufferLength = 64;


        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)object,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[126]);

        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)interface_id,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[142]);




        NdrGetBuffer(&__frame->_StubMsg, __frame->_StubMsg.BufferLength, __frame->_Handle);


        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(unsigned32 *)__frame->_StubMsg.Buffer = inquiry_type;
        __frame->_StubMsg.Buffer += sizeof(unsigned32);

        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)object,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[126]);

        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)interface_id,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[142]);

        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(unsigned32 *)__frame->_StubMsg.Buffer = vers_option;
        __frame->_StubMsg.Buffer += sizeof(unsigned32);

        NdrClientContextMarshall(
            &__frame->_StubMsg,
            (NDR_CCONTEXT)*entry_handle,
            1);

        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(unsigned32 *)__frame->_StubMsg.Buffer = max_ents;
        __frame->_StubMsg.Buffer += sizeof(unsigned32);

        NdrSendReceive(&__frame->_StubMsg, __frame->_StubMsg.Buffer);

        __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
        __frame->_StubMsg.BufferEnd = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

        if ((_RpcMessage.DataRepresentation & 0x0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert(&__frame->_StubMsg, (PFORMAT_STRING)&__MIDL_ProcFormatString.Format[30]);

        NdrClientContextUnmarshall(
            &__frame->_StubMsg,
            (NDR_CCONTEXT *)entry_handle,
            __frame->_Handle);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&num_ents,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[150],
            0);

        NdrComplexArrayUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&entries,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[154],
            0);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&status,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[172],
            0);

    }
    RpcFinally
    {
        __finally_ept_lookup( __frame );
    }
    RpcEndFinally
}

struct __frame_ept_map
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_BINDING_HANDLE _Handle;
};

static void __finally_ept_map( struct __frame_ept_map *__frame )
{
    NdrFullPointerXlatFree(__frame->_StubMsg.FullPtrXlatTables);

    NdrFreeBuffer(&__frame->_StubMsg);
}

void ept_map(
    handle_t h,
    uuid_p_t object,
    twr_p_t map_tower,
    ept_lookup_handle_t *entry_handle,
    unsigned32 max_towers,
    unsigned32 *num_towers,
    twr_p_t *towers,
    error_status_t *status)
{
    struct __frame_ept_map __f, * const __frame = &__f;
    RPC_MESSAGE _RpcMessage;
    __frame->_Handle = 0;

    RpcExceptionInit( 0, __finally_ept_map );
    __frame->_StubMsg.FullPtrXlatTables = NdrFullPointerXlatInit(0,XLAT_CLIENT);

    if (!entry_handle)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!num_towers)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!status)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    RpcTryFinally
    {
        NdrClientInitializeNew(&_RpcMessage, &__frame->_StubMsg, &epm_StubDesc, 3);
        _RpcMessage.RpcFlags = ( RPC_NCA_FLAGS_DEFAULT | RPC_NCA_FLAGS_IDEMPOTENT );

        __frame->_Handle = h;

        __frame->_StubMsg.BufferLength = 48;

        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)object,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[194]);

        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)map_tower,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[206]);



        NdrGetBuffer(&__frame->_StubMsg, __frame->_StubMsg.BufferLength, __frame->_Handle);


        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)object,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[194]);

        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)map_tower,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[206]);

        NdrClientContextMarshall(
            &__frame->_StubMsg,
            (NDR_CCONTEXT)*entry_handle,
            1);

        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(unsigned32 *)__frame->_StubMsg.Buffer = max_towers;
        __frame->_StubMsg.Buffer += sizeof(unsigned32);

        NdrSendReceive(&__frame->_StubMsg, __frame->_StubMsg.Buffer);

        __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
        __frame->_StubMsg.BufferEnd = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

        if ((_RpcMessage.DataRepresentation & 0x0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert(&__frame->_StubMsg, (PFORMAT_STRING)&__MIDL_ProcFormatString.Format[64]);

        NdrClientContextUnmarshall(
            &__frame->_StubMsg,
            (NDR_CCONTEXT *)entry_handle,
            __frame->_Handle);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&num_towers,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[214],
            0);

        NdrConformantVaryingArrayUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&towers,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[218],
            0);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&status,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[252],
            0);

    }
    RpcFinally
    {
        __finally_ept_map( __frame );
    }
    RpcEndFinally
}

struct __frame_ept_lookup_handle_free
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_BINDING_HANDLE _Handle;
};

static void __finally_ept_lookup_handle_free( struct __frame_ept_lookup_handle_free *__frame )
{
    NdrFreeBuffer(&__frame->_StubMsg);
}

void ept_lookup_handle_free(
    handle_t h,
    ept_lookup_handle_t *entry_handle,
    error_status_t *status)
{
    struct __frame_ept_lookup_handle_free __f, * const __frame = &__f;
    RPC_MESSAGE _RpcMessage;
    __frame->_Handle = 0;

    RpcExceptionInit( 0, __finally_ept_lookup_handle_free );
    if (!entry_handle)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!status)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    RpcTryFinally
    {
        NdrClientInitializeNew(&_RpcMessage, &__frame->_StubMsg, &epm_StubDesc, 4);
        __frame->_Handle = h;

        __frame->_StubMsg.BufferLength = 32;


        NdrGetBuffer(&__frame->_StubMsg, __frame->_StubMsg.BufferLength, __frame->_Handle);


        NdrClientContextMarshall(
            &__frame->_StubMsg,
            (NDR_CCONTEXT)*entry_handle,
            1);

        NdrSendReceive(&__frame->_StubMsg, __frame->_StubMsg.Buffer);

        __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
        __frame->_StubMsg.BufferEnd = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

        if ((_RpcMessage.DataRepresentation & 0x0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert(&__frame->_StubMsg, (PFORMAT_STRING)&__MIDL_ProcFormatString.Format[94]);

        NdrClientContextUnmarshall(
            &__frame->_StubMsg,
            (NDR_CCONTEXT *)entry_handle,
            __frame->_Handle);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&status,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[260],
            0);

    }
    RpcFinally
    {
        __finally_ept_lookup_handle_free( __frame );
    }
    RpcEndFinally
}

struct __frame_ept_inq_object
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_BINDING_HANDLE _Handle;
};

static void __finally_ept_inq_object( struct __frame_ept_inq_object *__frame )
{
    NdrFreeBuffer(&__frame->_StubMsg);
}

void ept_inq_object(
    handle_t h,
    GUID *ept_object,
    error_status_t *status)
{
    struct __frame_ept_inq_object __f, * const __frame = &__f;
    RPC_MESSAGE _RpcMessage;
    __frame->_Handle = 0;

    RpcExceptionInit( 0, __finally_ept_inq_object );
    if (!ept_object)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    if (!status)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    RpcTryFinally
    {
        NdrClientInitializeNew(&_RpcMessage, &__frame->_StubMsg, &epm_StubDesc, 5);
        _RpcMessage.RpcFlags = ( RPC_NCA_FLAGS_DEFAULT | RPC_NCA_FLAGS_IDEMPOTENT );

        __frame->_Handle = h;

        __frame->_StubMsg.BufferLength = 24;

        NdrGetBuffer(&__frame->_StubMsg, __frame->_StubMsg.BufferLength, __frame->_Handle);


        NdrSendReceive(&__frame->_StubMsg, __frame->_StubMsg.Buffer);

        __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
        __frame->_StubMsg.BufferEnd = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

        if ((_RpcMessage.DataRepresentation & 0x0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert(&__frame->_StubMsg, (PFORMAT_STRING)&__MIDL_ProcFormatString.Format[106]);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&ept_object,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[282],
            0);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&status,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[286],
            0);

    }
    RpcFinally
    {
        __finally_ept_inq_object( __frame );
    }
    RpcEndFinally
}

struct __frame_ept_mgmt_delete
{
    __DECL_EXCEPTION_FRAME
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_BINDING_HANDLE _Handle;
};

static void __finally_ept_mgmt_delete( struct __frame_ept_mgmt_delete *__frame )
{
    NdrFullPointerXlatFree(__frame->_StubMsg.FullPtrXlatTables);

    NdrFreeBuffer(&__frame->_StubMsg);
}

void ept_mgmt_delete(
    handle_t h,
    boolean32 object_speced,
    uuid_p_t object,
    twr_p_t tower,
    error_status_t *status)
{
    struct __frame_ept_mgmt_delete __f, * const __frame = &__f;
    RPC_MESSAGE _RpcMessage;
    __frame->_Handle = 0;

    RpcExceptionInit( 0, __finally_ept_mgmt_delete );
    __frame->_StubMsg.FullPtrXlatTables = NdrFullPointerXlatInit(0,XLAT_CLIENT);

    if (!status)
    {
        RpcRaiseException(RPC_X_NULL_REF_POINTER);
    }

    RpcTryFinally
    {
        NdrClientInitializeNew(&_RpcMessage, &__frame->_StubMsg, &epm_StubDesc, 6);
        __frame->_Handle = h;

        __frame->_StubMsg.BufferLength = 16;


        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)object,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[308]);

        NdrPointerBufferSize(
            &__frame->_StubMsg,
            (unsigned char *)tower,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[320]);

        NdrGetBuffer(&__frame->_StubMsg, __frame->_StubMsg.BufferLength, __frame->_Handle);


        MIDL_memset(__frame->_StubMsg.Buffer, 0, (0x4 - (long)__frame->_StubMsg.Buffer) & 0x3);
        __frame->_StubMsg.Buffer = (unsigned char *)(((long)__frame->_StubMsg.Buffer + 3) & ~0x3);
        *(boolean32 *)__frame->_StubMsg.Buffer = object_speced;
        __frame->_StubMsg.Buffer += sizeof(boolean32);

        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)object,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[308]);

        NdrPointerMarshall(
            &__frame->_StubMsg,
            (unsigned char *)tower,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[320]);

        NdrSendReceive(&__frame->_StubMsg, __frame->_StubMsg.Buffer);

        __frame->_StubMsg.BufferStart = _RpcMessage.Buffer;
        __frame->_StubMsg.BufferEnd = __frame->_StubMsg.BufferStart + _RpcMessage.BufferLength;

        if ((_RpcMessage.DataRepresentation & 0x0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION)
            NdrConvert(&__frame->_StubMsg, (PFORMAT_STRING)&__MIDL_ProcFormatString.Format[118]);

        NdrPointerUnmarshall(
            &__frame->_StubMsg,
            (unsigned char **)&status,
            (PFORMAT_STRING)&__MIDL_TypeFormatString.Format[324],
            0);

    }
    RpcFinally
    {
        __finally_ept_mgmt_delete( __frame );
    }
    RpcEndFinally
}

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

static const MIDL_STUB_DESC epm_StubDesc =
{
    (void *)& epm___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    {
        &epm__MIDL_AutoBindHandle,
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
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x09,    /* FC_ULONG */
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x40),
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x09,    /* FC_ULONG */
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x52),
        0x5b,    /* FC_END */
        0x5c,    /* FC_PAD */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x0f,    /* FC_IGNORE */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x09,    /* FC_ULONG */
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x56),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x68),
        0x5b,    /* FC_END */
        0x5c,    /* FC_PAD */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x0f,    /* FC_IGNORE */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x09,    /* FC_ULONG */
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x7e),
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x8e),
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x09,    /* FC_ULONG */
        0x50,    /* FC_IN_OUT_PARAM */
        0x01,
        NdrFcShort(0x92),
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x09,    /* FC_ULONG */
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x96),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x9a),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0xac),
        0x5b,    /* FC_END */
        0x5c,    /* FC_PAD */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x0f,    /* FC_IGNORE */
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0xc2),
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0xce),
        0x50,    /* FC_IN_OUT_PARAM */
        0x01,
        NdrFcShort(0xd2),
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x09,    /* FC_ULONG */
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0xd6),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0xda),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0xfc),
        0x5b,    /* FC_END */
        0x5c,    /* FC_PAD */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x0f,    /* FC_IGNORE */
        0x50,    /* FC_IN_OUT_PARAM */
        0x01,
        NdrFcShort(0x100),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x104),
        0x5b,    /* FC_END */
        0x5c,    /* FC_PAD */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x0f,    /* FC_IGNORE */
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x11a),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x11e),
        0x5b,    /* FC_END */
        0x5c,    /* FC_PAD */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x0f,    /* FC_IGNORE */
        0x4e,    /* FC_IN_PARAM_BASETYPE */
        0x09,    /* FC_ULONG */
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x134),
        0x4d,    /* FC_IN_PARAM */
        0x01,
        NdrFcShort(0x140),
        0x51,    /* FC_OUT_PARAM */
        0x01,
        NdrFcShort(0x144),
        0x5b,    /* FC_END */
        0x5c,    /* FC_PAD */
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
/* 8 (GUID) */
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
/* 20 (byte[]) */
        0x1b,	/* FC_CARRAY */
        0x0,	/* 0 */
        NdrFcShort(0x1),	/* 1 */
        0x9, /* Corr desc: FC_ULONG */
        0x0, /* no operators */
        NdrFcShort(0xfffffffc), /* offset = -4 */
        0x01,	/* FC_BYTE */
        0x5b,		/* FC_END */
/* 30 (twr_t) */
        0x17,	/* FC_CSTRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x4),	/* 4 */
        NdrFcShort(0xfff2),	/* Offset= -14 (20) */
        0x09,	/* FC_ULONG */
        0x5b,		/* FC_END */
/* 38 */
        0x26,    /* FC_CSTRING */
        0x5c, /* FC_PAD */
        NdrFcShort(0x40), /* 64 */
/* 42 (ept_entry_t) */
        0x1a,	/* FC_BOGUS_STRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x54),	/* 84 */
        NdrFcShort(0x0),
        NdrFcShort(0xc),	/* Offset= 12 (60) */
        0x4c,	/* FC_EMBEDDED_COMPLEX */
        0x0,	/* FIXME: padding */
        NdrFcShort(0xffd4),	/* Offset= -44 (8) */
        0x36,	/* FC_POINTER */
        0x4c,	/* FC_EMBEDDED_COMPLEX */
        0x0,	/* FIXME: padding */
        NdrFcShort(0xffed),	/* Offset= -19 (38) */
        0x5b,		/* FC_END */
/* 60 (twr_t *) */
        0x11, 0x0,	/* FC_RP */
        NdrFcShort(0xffe0),	/* Offset= -32 (30) */
/* 64 (ept_entry_t[]) */
        0x21,	/* FC_BOGUS_ARRAY */
        0x3,	/* 3 */
        NdrFcShort(0x0),	/* 0 */
        0x20,	/* Corr desc: parameter */
        0x0,
        NdrFcShort(0x0),
        NdrFcLong(0xffffffff),	/* -1 */
        0x4c,	/* FC_EMBEDDED_COMPLEX */
        0x0,	/* FIXME: padding */
        NdrFcShort(0xffdc),	/* Offset= -36 (42) */
        0x5c,		/* FC_PAD */
        0x5b,		/* FC_END */
        0x11, 0xc,    /* FC_RP [allocated_on_stack] [simple_pointer] */
        0x10,    /* FC_ERROR_STATUS_T */
        0x5c,          /* FC_PAD */
/* 86 (ept_entry_t[]) */
        0x21,	/* FC_BOGUS_ARRAY */
        0x3,	/* 3 */
        NdrFcShort(0x0),	/* 0 */
        0x20,	/* Corr desc: parameter */
        0x0,
        NdrFcShort(0x0),
        NdrFcLong(0xffffffff),	/* -1 */
        0x4c,	/* FC_EMBEDDED_COMPLEX */
        0x0,	/* FIXME: padding */
        NdrFcShort(0xffc6),	/* Offset= -58 (42) */
        0x5c,		/* FC_PAD */
        0x5b,		/* FC_END */
        0x11, 0xc,    /* FC_RP [allocated_on_stack] [simple_pointer] */
        0x10,    /* FC_ERROR_STATUS_T */
        0x5c,          /* FC_PAD */
/* 108 (unsigned char[8]) */
        0x1d,	/* FC_SMFARRAY */
        0x0,	/* 0 */
        NdrFcShort(0x8),	/* 8 */
        0x02,	/* FC_CHAR */
        0x5b,		/* FC_END */
/* 114 (GUID) */
        0x15,	/* FC_STRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x10),	/* 16 */
        0x09,	/* FC_ULONG */
        0x07,	/* FC_USHORT */
        0x07,	/* FC_USHORT */
        0x4c,	/* FC_EMBEDDED_COMPLEX */
        0x0,	/* FIXME: padding */
        NdrFcShort(0xfff1),	/* Offset= -15 (108) */
        0x5b,		/* FC_END */
/* 126 */
        0x14, 0x0,		/* FC_FP */
        NdrFcShort(0xfffffff2),	/* 114 */
/* 130 (rpc_if_id_t) */
        0x15,	/* FC_STRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x14),	/* 20 */
        0x4c,	/* FC_EMBEDDED_COMPLEX */
        0x0,	/* FIXME: padding */
        NdrFcShort(0xffea),	/* Offset= -22 (114) */
        0x07,	/* FC_USHORT */
        0x07,	/* FC_USHORT */
        0x5c,		/* FC_PAD */
        0x5b,		/* FC_END */
/* 142 */
        0x12, 0x0,		/* FC_UP */
        NdrFcShort(0xfffffff2),	/* 130 */
/* 146 */
        0x30,    /* FC_BIND_CONTEXT */
        0xe0,	/* Context flags: out, in, via ptr, */
        0, /* FIXME: rundown routine index*/
        0, /* FIXME: param num */
        0x11, 0xc,    /* FC_RP [allocated_on_stack] [simple_pointer] */
        0x09,    /* FC_ULONG */
        0x5c,          /* FC_PAD */
/* 154 (ept_entry_t[]) */
        0x21,	/* FC_BOGUS_ARRAY */
        0x3,	/* 3 */
        NdrFcShort(0x0),	/* 0 */
        0x20,	/* Corr desc: parameter */
        0x0,
        NdrFcShort(0x0),
        0x20,	/* Corr desc: parameter */
        0x0,
        NdrFcShort(0x0),
        0x4c,	/* FC_EMBEDDED_COMPLEX */
        0x0,	/* FIXME: padding */
        NdrFcShort(0xff82),	/* Offset= -126 (42) */
        0x5c,		/* FC_PAD */
        0x5b,		/* FC_END */
        0x11, 0xc,    /* FC_RP [allocated_on_stack] [simple_pointer] */
        0x10,    /* FC_ERROR_STATUS_T */
        0x5c,          /* FC_PAD */
/* 176 (unsigned char[8]) */
        0x1d,	/* FC_SMFARRAY */
        0x0,	/* 0 */
        NdrFcShort(0x8),	/* 8 */
        0x02,	/* FC_CHAR */
        0x5b,		/* FC_END */
/* 182 (GUID) */
        0x15,	/* FC_STRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x10),	/* 16 */
        0x09,	/* FC_ULONG */
        0x07,	/* FC_USHORT */
        0x07,	/* FC_USHORT */
        0x4c,	/* FC_EMBEDDED_COMPLEX */
        0x0,	/* FIXME: padding */
        NdrFcShort(0xfff1),	/* Offset= -15 (176) */
        0x5b,		/* FC_END */
/* 194 */
        0x14, 0x0,		/* FC_FP */
        NdrFcShort(0xfffffff2),	/* 182 */
/* 198 (twr_t) */
        0x17,	/* FC_CSTRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x4),	/* 4 */
        NdrFcShort(0xff4a),	/* Offset= -182 (20) */
        0x09,	/* FC_ULONG */
        0x5b,		/* FC_END */
/* 206 */
        0x14, 0x0,		/* FC_FP */
        NdrFcShort(0xfffffff6),	/* 198 */
/* 210 */
        0x30,    /* FC_BIND_CONTEXT */
        0xe0,	/* Context flags: out, in, via ptr, */
        0, /* FIXME: rundown routine index*/
        0, /* FIXME: param num */
        0x11, 0xc,    /* FC_RP [allocated_on_stack] [simple_pointer] */
        0x09,    /* FC_ULONG */
        0x5c,          /* FC_PAD */
/* 218 (twr_p_t *) */
        0x1c,	/* FC_CVARRAY */
        0x3,	/* 3 */
        NdrFcShort(0x4),	/* 4 */
        0x20,	/* Corr desc: parameter */
        0x0,
        NdrFcShort(0x0),
        0x20,	/* Corr desc: parameter */
        0x0,
        NdrFcShort(0x0),
        0x4b, /* FC_PP */
        0x5c, /* FC_PAD */
        0x48, /* FC_VARIABLE_REPEAT */
        0x4a, /* FC_VARIABLE_OFFSET */
        NdrFcShort(0x4), /* Increment = 4 */
        NdrFcShort(0x0), /* Offset to array = 0 */
        NdrFcShort(0x1), /* Number of pointers = 1 */
        NdrFcShort(0x0), /* Memory offset = 0 */
        NdrFcShort(0x0), /* Buffer offset = 0 */
/* 244 (twr_p_t) */
        0x14, 0x0,	/* FC_FP */
        NdrFcShort(0xffd0),	/* Offset= -48 (198) */
        0x5b, /* FC_END */
        0x8,	/* FC_LONG */
        0x5c,		/* FC_PAD */
        0x5b,		/* FC_END */
        0x11, 0xc,    /* FC_RP [allocated_on_stack] [simple_pointer] */
        0x10,    /* FC_ERROR_STATUS_T */
        0x5c,          /* FC_PAD */
/* 256 */
        0x30,    /* FC_BIND_CONTEXT */
        0xe0,	/* Context flags: out, in, via ptr, */
        0, /* FIXME: rundown routine index*/
        0, /* FIXME: param num */
        0x11, 0xc,    /* FC_RP [allocated_on_stack] [simple_pointer] */
        0x10,    /* FC_ERROR_STATUS_T */
        0x5c,          /* FC_PAD */
/* 264 (unsigned char[8]) */
        0x1d,	/* FC_SMFARRAY */
        0x0,	/* 0 */
        NdrFcShort(0x8),	/* 8 */
        0x02,	/* FC_CHAR */
        0x5b,		/* FC_END */
/* 270 (GUID) */
        0x15,	/* FC_STRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x10),	/* 16 */
        0x09,	/* FC_ULONG */
        0x07,	/* FC_USHORT */
        0x07,	/* FC_USHORT */
        0x4c,	/* FC_EMBEDDED_COMPLEX */
        0x0,	/* FIXME: padding */
        NdrFcShort(0xfff1),	/* Offset= -15 (264) */
        0x5b,		/* FC_END */
/* 282 */
        0x11, 0x4,		/* FC_RP [allocated_on_stack] */
        NdrFcShort(0xfffffff2),	/* 270 */
        0x11, 0xc,    /* FC_RP [allocated_on_stack] [simple_pointer] */
        0x10,    /* FC_ERROR_STATUS_T */
        0x5c,          /* FC_PAD */
/* 290 (unsigned char[8]) */
        0x1d,	/* FC_SMFARRAY */
        0x0,	/* 0 */
        NdrFcShort(0x8),	/* 8 */
        0x02,	/* FC_CHAR */
        0x5b,		/* FC_END */
/* 296 (GUID) */
        0x15,	/* FC_STRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x10),	/* 16 */
        0x09,	/* FC_ULONG */
        0x07,	/* FC_USHORT */
        0x07,	/* FC_USHORT */
        0x4c,	/* FC_EMBEDDED_COMPLEX */
        0x0,	/* FIXME: padding */
        NdrFcShort(0xfff1),	/* Offset= -15 (290) */
        0x5b,		/* FC_END */
/* 308 */
        0x14, 0x0,		/* FC_FP */
        NdrFcShort(0xfffffff2),	/* 296 */
/* 312 (twr_t) */
        0x17,	/* FC_CSTRUCT */
        0x3,	/* 3 */
        NdrFcShort(0x4),	/* 4 */
        NdrFcShort(0xfed8),	/* Offset= -296 (20) */
        0x09,	/* FC_ULONG */
        0x5b,		/* FC_END */
/* 320 */
        0x14, 0x0,		/* FC_FP */
        NdrFcShort(0xfffffff6),	/* 312 */
        0x11, 0xc,    /* FC_RP [allocated_on_stack] [simple_pointer] */
        0x10,    /* FC_ERROR_STATUS_T */
        0x5c,          /* FC_PAD */
        0x0
    }
};

