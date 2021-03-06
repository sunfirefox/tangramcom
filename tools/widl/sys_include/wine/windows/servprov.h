/*** Autogenerated by WIDL 1.1.6 from servprov.idl - Do not edit ***/
#include <rpc.h>
#include <rpcndr.h>

#ifndef __WIDL_SERVPROV_H
#define __WIDL_SERVPROV_H
#ifdef __cplusplus
extern "C" {
#endif

#include <objidl.h>
#ifndef __IServiceProvider_FWD_DEFINED__
#define __IServiceProvider_FWD_DEFINED__
typedef interface IServiceProvider IServiceProvider;
#endif

typedef IServiceProvider *LPSERVICEPROVIDER;
/*****************************************************************************
 * IServiceProvider interface
 */
#ifndef __IServiceProvider_INTERFACE_DEFINED__
#define __IServiceProvider_INTERFACE_DEFINED__

DEFINE_GUID(IID_IServiceProvider, 0x6d5140c1, 0x7436, 0x11ce, 0x80,0x34, 0x00,0xaa,0x00,0x60,0x09,0xfa);
#if defined(__cplusplus) && !defined(CINTERFACE)
interface IServiceProvider : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE QueryService(
        REFGUID guidService,
        REFIID riid,
        void **ppvObject) = 0;

};
#else
typedef struct IServiceProviderVtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        IServiceProvider* This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        IServiceProvider* This);

    ULONG (STDMETHODCALLTYPE *Release)(
        IServiceProvider* This);

    /*** IServiceProvider methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryService)(
        IServiceProvider* This,
        REFGUID guidService,
        REFIID riid,
        void **ppvObject);

    END_INTERFACE
} IServiceProviderVtbl;
interface IServiceProvider {
    CONST_VTBL IServiceProviderVtbl* lpVtbl;
};

#ifdef COBJMACROS
/*** IUnknown methods ***/
#define IServiceProvider_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define IServiceProvider_AddRef(This) (This)->lpVtbl->AddRef(This)
#define IServiceProvider_Release(This) (This)->lpVtbl->Release(This)
/*** IServiceProvider methods ***/
#define IServiceProvider_QueryService(This,guidService,riid,ppvObject) (This)->lpVtbl->QueryService(This,guidService,riid,ppvObject)
#endif

#endif

HRESULT STDMETHODCALLTYPE IServiceProvider_RemoteQueryService_Proxy(
    IServiceProvider* This,
    REFGUID guidService,
    REFIID riid,
    IUnknown **ppvObject);
void __RPC_STUB IServiceProvider_RemoteQueryService_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT CALLBACK IServiceProvider_QueryService_Proxy(
    IServiceProvider* This,
    REFGUID guidService,
    REFIID riid,
    void **ppvObject);
HRESULT __RPC_STUB IServiceProvider_QueryService_Stub(
    IServiceProvider* This,
    REFGUID guidService,
    REFIID riid,
    IUnknown **ppvObject);

#endif  /* __IServiceProvider_INTERFACE_DEFINED__ */

/* Begin additional prototypes for all interfaces */


/* End additional prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __WIDL_SERVPROV_H */
