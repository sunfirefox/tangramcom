/*
* COM stub (CStdStubBuffer) implementation
*
* Copyright 2001 Ove K�ven, TransGaming Technologies
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

#include "config.h"
#include "wine/port.h"

#include <stdarg.h>

#define COBJMACROS

#include "windef.h"
#include "winbase.h"
#include "winerror.h"
#include "excpt.h"

#include "objbase.h"
#include "rpcproxy.h"

#include "tgmlog.h"
#include "wine/exception.h"

#include "cpsf.h"

TGMLOG_DEFAULT_DEBUG_CHANNEL(ole);

#define STUB_HEADER(This) (((const CInterfaceStubHeader*)((This)->lpVtbl))[-1])

static LONG WINAPI stub_filter(EXCEPTION_POINTERS *eptr)
{
    if (eptr->ExceptionRecord->ExceptionFlags & EXCEPTION_NONCONTINUABLE)
        return EXCEPTION_CONTINUE_SEARCH;
    return EXCEPTION_EXECUTE_HANDLER;
}

typedef struct
{
    IUnknownVtbl *base_obj;
    IRpcStubBuffer *base_stub;
    CStdStubBuffer stub_buffer;
} cstdstubbuffer_delegating_t;

static inline cstdstubbuffer_delegating_t *impl_from_delegating( IRpcStubBuffer *iface )
{
    return (cstdstubbuffer_delegating_t*)((char *)iface - FIELD_OFFSET(cstdstubbuffer_delegating_t, stub_buffer));
}

HRESULT WINAPI CStdStubBuffer_Construct(REFIID riid,
                                        LPUNKNOWN pUnkServer,
                                        PCInterfaceName name,
                                        CInterfaceStubVtbl *vtbl,
                                        LPPSFACTORYBUFFER pPSFactory,
                                        LPRPCSTUBBUFFER *ppStub)
{
    CStdStubBuffer *This;
    IUnknown *pvServer;
    HRESULT r;
    FB_TRACE("> (%p,%p,%p,%p) %s\n", pUnkServer, vtbl, pPSFactory, ppStub, name);
    TRACE("iid=%s\n", TGMLOG_STR_GUID(vtbl->header.piid));
    TRACE("vtbl=%p\n", &vtbl->Vtbl);

    if (!IsEqualGUID(vtbl->header.piid, riid)) {
        ERR("IID mismatch during stub creation\n");
        return RPC_E_UNEXPECTED;
    }

    r = IUnknown_QueryInterface(pUnkServer, riid, (void**)&pvServer);
    if(FAILED(r)){
        FE_WARN("< hr=%d\n",r);
        return r;
    }

    This = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(CStdStubBuffer));
    if (!This) {
        IUnknown_Release(pvServer);
        FE_ERR("< hr=%d\n",E_OUTOFMEMORY);
        return E_OUTOFMEMORY;
    }

    This->lpVtbl = &vtbl->Vtbl;
    This->RefCount = 1;
    This->pvServerObject = pvServer;
    This->pPSFactory = pPSFactory;
    *ppStub = (LPRPCSTUBBUFFER)This;

    IPSFactoryBuffer_AddRef(pPSFactory);
    FE_TRACE("< hr=%d\n",r);
    return S_OK;
}

static CRITICAL_SECTION delegating_vtbl_section;
static CRITICAL_SECTION_DEBUG critsect_debug =
{
    0, 0, &delegating_vtbl_section,
    { &critsect_debug.ProcessLocksList, &critsect_debug.ProcessLocksList },
    0, 0, { (DWORD_PTR)(__FILE__ ": delegating_vtbl_section") }
};
static CRITICAL_SECTION delegating_vtbl_section = { &critsect_debug, -1, 0, 0, 0, 0 };

typedef struct
{
    DWORD ref;
    DWORD size;
    void **methods;
    IUnknownVtbl vtbl;
    /* remaining entries in vtbl */
} ref_counted_vtbl;

static struct
{
    ref_counted_vtbl *table;
} current_vtbl;


static HRESULT WINAPI delegating_QueryInterface(IUnknown *pUnk, REFIID iid, void **ppv)
{
    *ppv = (void *)pUnk;
    return S_OK;
}

static ULONG WINAPI delegating_AddRef(IUnknown *pUnk)
{
    return 1;
}

static ULONG WINAPI delegating_Release(IUnknown *pUnk)
{
    return 1;
}

#if defined(__i386__)

/* The idea here is to replace the first param on the stack
ie. This (which will point to cstdstubbuffer_delegating_t)
with This->stub_buffer.pvServerObject and then jump to the
relevant offset in This->stub_buffer.pvServerObject's vtbl.
*/
#include "pshpack1.h"
typedef struct {
    DWORD mov1;    /* mov 0x4(%esp), %eax      8b 44 24 04 */
    WORD mov2;     /* mov 0x10(%eax), %eax     8b 40 */
    BYTE sixteen;  /*                          10   */
    DWORD mov3;    /* mov %eax, 0x4(%esp)      89 44 24 04 */
    WORD mov4;     /* mov (%eax), %eax         8b 00 */
    WORD mov5;     /* mov offset(%eax), %eax   8b 80 */
    DWORD offset;  /*                          xx xx xx xx */
    WORD jmp;      /* jmp *%eax                ff e0 */
    BYTE pad[3];   /* lea 0x0(%esi), %esi      8d 76 00 */
} vtbl_method_t;
#include "poppack.h"

static void fill_table(IUnknownVtbl *vtbl, void **methods, DWORD num)
{
    vtbl_method_t *method;
    void **entry;
    DWORD i;

    vtbl->QueryInterface = delegating_QueryInterface;
    vtbl->AddRef = delegating_AddRef;
    vtbl->Release = delegating_Release;

    method = (vtbl_method_t*)methods;
    entry = (void**)(vtbl + 1);

    for(i = 3; i < num; i++)
    {
        *entry = method;
        method->mov1 = 0x0424448b;
        method->mov2 = 0x408b;
        method->sixteen = 0x10;
        method->mov3 = 0x04244489;
        method->mov4 = 0x008b;
        method->mov5 = 0x808b;
        method->offset = i << 2;
        method->jmp = 0xe0ff;
        method->pad[0] = 0x8d;
        method->pad[1] = 0x76;
        method->pad[2] = 0x00;

        method++;
        entry++;
    }
}

#else  /* __i386__ */

typedef struct {int dummy;} vtbl_method_t;
static void fill_table(IUnknownVtbl *vtbl, void **methods, DWORD num)
{
    ERR("delegated stubs are not supported on this architecture\n");
}

#endif  /* __i386__ */

void create_delegating_vtbl(DWORD num_methods)
{
    FB_TRACE("> %d\n", num_methods);
    if(num_methods <= 3)
    {
        FE_ERR("< should have more than %d methods\n", num_methods);
        return;
    }

    EnterCriticalSection(&delegating_vtbl_section);
    if(!current_vtbl.table || num_methods > current_vtbl.table->size)
    {
        DWORD size;
        //DWORD old_protect;
        if(current_vtbl.table && current_vtbl.table->ref == 0)
        {
            TRACE("freeing old table\n");
            //VirtualFree(current_vtbl.table->methods, 0, MEM_RELEASE);
            HeapFree(GetProcessHeap(), 0,current_vtbl.table->methods);
            HeapFree(GetProcessHeap(), 0, current_vtbl.table);
        }
        size = (num_methods - 3) * sizeof(vtbl_method_t);
        current_vtbl.table = HeapAlloc(GetProcessHeap(), 0, FIELD_OFFSET(ref_counted_vtbl, vtbl) + num_methods * sizeof(void*));
        current_vtbl.table->ref = 0;
        current_vtbl.table->size = num_methods;
        //current_vtbl.table->methods = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        current_vtbl.table->methods = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
        fill_table(&current_vtbl.table->vtbl, current_vtbl.table->methods, num_methods);
        //VirtualProtect(current_vtbl.table->methods, size, PAGE_EXECUTE_READ, &old_protect);
    }
    LeaveCriticalSection(&delegating_vtbl_section);
    FE_TRACE("< \n");
}

static IUnknownVtbl *get_delegating_vtbl(void)
{
    IUnknownVtbl *ret;

    EnterCriticalSection(&delegating_vtbl_section);
    current_vtbl.table->ref++;
    ret = &current_vtbl.table->vtbl;
    LeaveCriticalSection(&delegating_vtbl_section);
    return ret;
}

static void release_delegating_vtbl(IUnknownVtbl *vtbl)
{
    ref_counted_vtbl *table = (ref_counted_vtbl*)((DWORD *)vtbl - 1);

    EnterCriticalSection(&delegating_vtbl_section);
    table->ref--;
    TRACE("ref now %d\n", table->ref);
    if(table->ref == 0 && table != current_vtbl.table)
    {
        TRACE("... and we're not current so free'ing\n");
        //VirtualFree(current_vtbl.table->methods, 0, MEM_RELEASE);
        HeapFree(GetProcessHeap(), 0,current_vtbl.table->methods);
        HeapFree(GetProcessHeap(), 0, table);
    }
    LeaveCriticalSection(&delegating_vtbl_section);
}

HRESULT WINAPI CStdStubBuffer_Delegating_Construct(REFIID riid,
                                                   LPUNKNOWN pUnkServer,
                                                   PCInterfaceName name,
                                                   CInterfaceStubVtbl *vtbl,
                                                   REFIID delegating_iid,
                                                   LPPSFACTORYBUFFER pPSFactory,
                                                   LPRPCSTUBBUFFER *ppStub)
{
    cstdstubbuffer_delegating_t *This;
    IUnknown *pvServer;
    HRESULT r;

    FB_TRACE("> (%p,%p,%p,%p) %s\n", pUnkServer, vtbl, pPSFactory, ppStub, name);
    TRACE("iid=%s delegating to %s\n", TGMLOG_STR_GUID(vtbl->header.piid), TGMLOG_STR_GUID(delegating_iid));
    TRACE("vtbl=%p\n", &vtbl->Vtbl);

    if (!IsEqualGUID(vtbl->header.piid, riid))
    {
        FE_ERR("< IID mismatch during stub creation\n");
        return RPC_E_UNEXPECTED;
    }

    r = IUnknown_QueryInterface(pUnkServer, riid, (void**)&pvServer);
    if(FAILED(r)) 
    {
        FE_WARN("< hr = 0x%x \n",r);
        return r;
    }

    This = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*This));
    if (!This)
    {
        IUnknown_Release(pvServer);
        FE_ERR("< hr \n");
        return E_OUTOFMEMORY;
    }

    This->base_obj = get_delegating_vtbl();
    r = create_stub(delegating_iid, (IUnknown*)&This->base_obj, &This->base_stub);
    if(FAILED(r))
    {
        release_delegating_vtbl(This->base_obj);
        HeapFree(GetProcessHeap(), 0, This);
        IUnknown_Release(pvServer);
        FE_WARN("< hr = 0x%x \n",r);
        return r;
    }

    This->stub_buffer.lpVtbl = &vtbl->Vtbl;
    This->stub_buffer.RefCount = 1;
    This->stub_buffer.pvServerObject = pvServer;
    This->stub_buffer.pPSFactory = pPSFactory;
    *ppStub = (LPRPCSTUBBUFFER)&This->stub_buffer;

    IPSFactoryBuffer_AddRef(pPSFactory);

    FE_TRACE("< \n");
    return S_OK;
}

HRESULT WINAPI CStdStubBuffer_QueryInterface(LPRPCSTUBBUFFER iface,
                                             REFIID riid,
                                             LPVOID *obj)
{
    CStdStubBuffer *This = (CStdStubBuffer *)iface;
    FB_TRACE("> (%p)->QueryInterface(%s,%p)\n",This,TGMLOG_STR_GUID(riid),obj);

    if (IsEqualIID(&IID_IUnknown, riid) ||
        IsEqualIID(&IID_IRpcStubBuffer, riid))
    {
        IUnknown_AddRef(iface);
        *obj = iface;

        FE_TRACE("< S_OK\n");
        return S_OK;
    }
    *obj = NULL;

    FE_TRACE("< E_NOINTERFACE\n");
    return E_NOINTERFACE;
}

ULONG WINAPI CStdStubBuffer_AddRef(LPRPCSTUBBUFFER iface)
{
    CStdStubBuffer *This = (CStdStubBuffer *)iface;
    TRACE("(%p)->AddRef()\n",This);
    return InterlockedIncrement(&This->RefCount);
}

ULONG WINAPI NdrCStdStubBuffer_Release(LPRPCSTUBBUFFER iface,
                                       LPPSFACTORYBUFFER pPSF)
{
    CStdStubBuffer *This = (CStdStubBuffer *)iface;
    ULONG refs;

    FB_TRACE("> (%p)->Release()\n",This);

    refs = InterlockedDecrement(&This->RefCount);
    if (!refs)
    {
        /* test_Release shows that native doesn't call Disconnect here.
        We'll leave it in for the time being. */
        IRpcStubBuffer_Disconnect(iface);

        IPSFactoryBuffer_Release(pPSF);
        HeapFree(GetProcessHeap(),0,This);
    }

    FE_TRACE("< refs:%lu\n",(unsigned long)refs);
    return refs;
}

ULONG WINAPI NdrCStdStubBuffer2_Release(LPRPCSTUBBUFFER iface,
                                        LPPSFACTORYBUFFER pPSF)
{
    cstdstubbuffer_delegating_t *This = impl_from_delegating( iface );
    ULONG refs;

    TRACE("(%p)->Release()\n", This);

    refs = InterlockedDecrement(&This->stub_buffer.RefCount);
    if (!refs)
    {
        /* Just like NdrCStdStubBuffer_Release, we shouldn't call
        Disconnect here */
        IRpcStubBuffer_Disconnect((IRpcStubBuffer *)&This->stub_buffer);

        IRpcStubBuffer_Release(This->base_stub);
        release_delegating_vtbl(This->base_obj);

        IPSFactoryBuffer_Release(pPSF);
        HeapFree(GetProcessHeap(), 0, This);
    }

    FE_TRACE("< refs:%lu\n",(unsigned long)refs);
    return refs;
}

HRESULT WINAPI CStdStubBuffer_Connect(LPRPCSTUBBUFFER iface,
                                      LPUNKNOWN lpUnkServer)
{
    CStdStubBuffer *This = (CStdStubBuffer *)iface;
    HRESULT r;
    IUnknown *new = NULL;

    FB_TRACE("> (%p)->Connect(%p)\n",This,lpUnkServer);

    r = IUnknown_QueryInterface(lpUnkServer, STUB_HEADER(This).piid, (void**)&new);
    new = InterlockedExchangePointer((void**)&This->pvServerObject, new);
    if(new)
        IUnknown_Release(new);
    FE_TRACE("< new:%p\n",new);
    return r;
}

void WINAPI CStdStubBuffer_Disconnect(LPRPCSTUBBUFFER iface)
{
    CStdStubBuffer *This = (CStdStubBuffer *)iface;
    IUnknown *old = NULL;
    FB_TRACE("> (%p)->Disconnect(%p,%p)\n",This,This->pvServerObject,old);

    old = InterlockedExchangePointer((void**)&This->pvServerObject, NULL);

    if(old)
        IUnknown_Release(old);
    FE_TRACE("< old:%p\n",old);
}

HRESULT WINAPI CStdStubBuffer_Invoke(LPRPCSTUBBUFFER iface,
                                     PRPCOLEMESSAGE pMsg,
                                     LPRPCCHANNELBUFFER pChannel)
{
    CStdStubBuffer *This = (CStdStubBuffer *)iface;
    DWORD dwPhase = STUB_UNMARSHAL;
    HRESULT hr = S_OK;

    FB_TRACE("> (%p)->Invoke(%p,%p)\n",This,pMsg,pChannel);

    __TRY
    {
        if (STUB_HEADER(This).pDispatchTable)
            STUB_HEADER(This).pDispatchTable[pMsg->iMethod](iface, pChannel, (PRPC_MESSAGE)pMsg, &dwPhase);
        else /* pure interpreted */
            NdrStubCall2(iface, pChannel, (PRPC_MESSAGE)pMsg, &dwPhase);
    }
    __EXCEPT(stub_filter)
    {
#ifdef USE_COMPILER_EXCEPTIONS
        DWORD dwExceptionCode = GetExceptionCode();
#else
        DWORD dwExceptionCode = -1;
#endif
        WARN("a stub call failed with exception 0x%08x (%d)\n", dwExceptionCode, dwExceptionCode);
        if (FAILED(dwExceptionCode))
            hr = dwExceptionCode;
        else
            hr = HRESULT_FROM_WIN32(dwExceptionCode);
    }
    __ENDTRY

        FE_TRACE("< hr:0x%x\n",hr);
    return hr;
}

LPRPCSTUBBUFFER WINAPI CStdStubBuffer_IsIIDSupported(LPRPCSTUBBUFFER iface,
                                                     REFIID riid)
{
    CStdStubBuffer *This = (CStdStubBuffer *)iface;
    TRACE("(%p)->IsIIDSupported(%s)\n",This,TGMLOG_STR_GUID(riid));
    return IsEqualGUID(STUB_HEADER(This).piid, riid) ? iface : NULL;
}

ULONG WINAPI CStdStubBuffer_CountRefs(LPRPCSTUBBUFFER iface)
{
    CStdStubBuffer *This = (CStdStubBuffer *)iface;
    TRACE("(%p)->CountRefs()\n",This);
    return This->RefCount;
}

HRESULT WINAPI CStdStubBuffer_DebugServerQueryInterface(LPRPCSTUBBUFFER iface,
                                                        LPVOID *ppv)
{
    CStdStubBuffer *This = (CStdStubBuffer *)iface;
    TRACE("(%p)->DebugServerQueryInterface(%p)\n",This,ppv);
    return S_OK;
}

void WINAPI CStdStubBuffer_DebugServerRelease(LPRPCSTUBBUFFER iface,
                                              LPVOID pv)
{
    CStdStubBuffer *This = (CStdStubBuffer *)iface;
    TRACE("(%p)->DebugServerRelease(%p)\n",This,pv);
}

const IRpcStubBufferVtbl CStdStubBuffer_Vtbl =
{
    CStdStubBuffer_QueryInterface,
        CStdStubBuffer_AddRef,
        NULL,
        CStdStubBuffer_Connect,
        CStdStubBuffer_Disconnect,
        CStdStubBuffer_Invoke,
        CStdStubBuffer_IsIIDSupported,
        CStdStubBuffer_CountRefs,
        CStdStubBuffer_DebugServerQueryInterface,
        CStdStubBuffer_DebugServerRelease
};

static HRESULT WINAPI CStdStubBuffer_Delegating_Connect(LPRPCSTUBBUFFER iface,
                                                        LPUNKNOWN lpUnkServer)
{
    cstdstubbuffer_delegating_t *This = impl_from_delegating(iface);
    HRESULT r;
    FB_TRACE("> (%p)->Connect(%p)\n", This, lpUnkServer);

    r = CStdStubBuffer_Connect(iface, lpUnkServer);
    if(SUCCEEDED(r))
        r = IRpcStubBuffer_Connect(This->base_stub, (IUnknown*)&This->base_obj);

    FE_TRACE("< hr:0x%x\n",r);
    return r;
}

static void WINAPI CStdStubBuffer_Delegating_Disconnect(LPRPCSTUBBUFFER iface)
{
    cstdstubbuffer_delegating_t *This = impl_from_delegating(iface);
    FB_TRACE("> (%p)->Disconnect()\n", This);

    IRpcStubBuffer_Disconnect(This->base_stub);
    CStdStubBuffer_Disconnect(iface);
    FE_TRACE("< \n");
}

static ULONG WINAPI CStdStubBuffer_Delegating_CountRefs(LPRPCSTUBBUFFER iface)
{
    cstdstubbuffer_delegating_t *This = impl_from_delegating(iface);
    ULONG ret;
    FB_TRACE("> (%p)->CountRefs()\n", This);

    ret = CStdStubBuffer_CountRefs(iface);
    ret += IRpcStubBuffer_CountRefs(This->base_stub);

    FE_TRACE("< ret:0x%x\n",ret);
    return ret;
}

const IRpcStubBufferVtbl CStdStubBuffer_Delegating_Vtbl =
{
    CStdStubBuffer_QueryInterface,
        CStdStubBuffer_AddRef,
        NULL,
        CStdStubBuffer_Delegating_Connect,
        CStdStubBuffer_Delegating_Disconnect,
        CStdStubBuffer_Invoke,
        CStdStubBuffer_IsIIDSupported,
        CStdStubBuffer_Delegating_CountRefs,
        CStdStubBuffer_DebugServerQueryInterface,
        CStdStubBuffer_DebugServerRelease
};

const MIDL_SERVER_INFO *CStdStubBuffer_GetServerInfo(IRpcStubBuffer *iface)
{
    CStdStubBuffer *This = (CStdStubBuffer *)iface;
    return STUB_HEADER(This).pServerInfo;
}

/************************************************************************
*           NdrStubForwardingFunction [RPCRT4.@]
*/
void __RPC_STUB NdrStubForwardingFunction( IRpcStubBuffer *iface, IRpcChannelBuffer *pChannel,
                                          PRPC_MESSAGE pMsg, DWORD *pdwStubPhase )
{
    /* Note pMsg is passed intact since RPCOLEMESSAGE is basically a RPC_MESSAGE. */

    cstdstubbuffer_delegating_t *This = impl_from_delegating(iface);
    HRESULT r = IRpcStubBuffer_Invoke(This->base_stub, (RPCOLEMESSAGE*)pMsg, pChannel);
    if(FAILED(r)) RpcRaiseException(r);
    return;
}

/***********************************************************************
*           NdrStubInitialize [RPCRT4.@]
*/
void WINAPI NdrStubInitialize(PRPC_MESSAGE pRpcMsg,
                              PMIDL_STUB_MESSAGE pStubMsg,
                              PMIDL_STUB_DESC pStubDescriptor,
                              LPRPCCHANNELBUFFER pRpcChannelBuffer)
{
    TRACE("(%p,%p,%p,%p)\n", pRpcMsg, pStubMsg, pStubDescriptor, pRpcChannelBuffer);
    NdrServerInitializeNew(pRpcMsg, pStubMsg, pStubDescriptor);
    pStubMsg->pRpcChannelBuffer = pRpcChannelBuffer;
    IRpcChannelBuffer_GetDestCtx(pStubMsg->pRpcChannelBuffer,
        &pStubMsg->dwDestContext,
        &pStubMsg->pvDestContext);
}

/***********************************************************************
*           NdrStubGetBuffer [RPCRT4.@]
*/
void WINAPI NdrStubGetBuffer(LPRPCSTUBBUFFER iface,
                             LPRPCCHANNELBUFFER pRpcChannelBuffer,
                             PMIDL_STUB_MESSAGE pStubMsg)
{
    CStdStubBuffer *This = (CStdStubBuffer *)iface;
    HRESULT hr;

    FB_TRACE("> (%p, %p, %p)\n", This, pRpcChannelBuffer, pStubMsg);

    pStubMsg->RpcMsg->BufferLength = pStubMsg->BufferLength;
    hr = IRpcChannelBuffer_GetBuffer(pRpcChannelBuffer,
        (RPCOLEMESSAGE *)pStubMsg->RpcMsg, STUB_HEADER(This).piid);
    if (FAILED(hr))
    {
        RpcRaiseException(hr);
        FE_WARN("< hr:0x%x\n",hr);
        return;
    }

    pStubMsg->Buffer = pStubMsg->RpcMsg->Buffer;
    FE_TRACE("< \n");
}
