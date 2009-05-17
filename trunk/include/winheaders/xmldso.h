/*** Autogenerated by WIDL 1.1.6 from xmldso.idl - Do not edit ***/
#include "rpc.h"
#include "rpcndr.h"

#ifndef __WIDL_XMLDSO_H
#define __WIDL_XMLDSO_H
#ifdef __cplusplus
extern "C" {
#endif

#include "wtypes.h"
#include <ocidl.h>
#ifndef __IXMLDOMDocument_FWD_DEFINED__
#define __IXMLDOMDocument_FWD_DEFINED__
typedef interface IXMLDOMDocument IXMLDOMDocument;
#endif

#ifndef __IXMLDSOControl_FWD_DEFINED__
#define __IXMLDSOControl_FWD_DEFINED__
typedef interface IXMLDSOControl IXMLDSOControl;
#endif

/*****************************************************************************
 * IXMLDSOControl interface
 */
#ifndef __IXMLDSOControl_INTERFACE_DEFINED__
#define __IXMLDSOControl_INTERFACE_DEFINED__

DEFINE_GUID(IID_IXMLDSOControl, 0x310afa62, 0x0575, 0x11d2, 0x9c,0xa9, 0x00,0x60,0xb0,0xec,0x3d,0x39);
#if defined(__cplusplus) && !defined(CINTERFACE)
interface IXMLDSOControl : public IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_XMLDocument(
        IXMLDOMDocument **ppDoc) = 0;

    virtual HRESULT STDMETHODCALLTYPE put_XMLDocument(
        IXMLDOMDocument *ppDoc) = 0;

    virtual HRESULT STDMETHODCALLTYPE get_JavaDSOCompatible(
        BOOL *fJavaDSOCompatible) = 0;

    virtual HRESULT STDMETHODCALLTYPE put_JavaDSOCompatible(
        BOOL fJavaDSOCompatible) = 0;

    virtual HRESULT STDMETHODCALLTYPE get_readyState(
        long *state) = 0;

};
#else
typedef struct IXMLDSOControlVtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        IXMLDSOControl* This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        IXMLDSOControl* This);

    ULONG (STDMETHODCALLTYPE *Release)(
        IXMLDSOControl* This);

    /*** IDispatch methods ***/
    HRESULT (STDMETHODCALLTYPE *GetTypeInfoCount)(
        IXMLDSOControl* This,
        UINT *pctinfo);

    HRESULT (STDMETHODCALLTYPE *GetTypeInfo)(
        IXMLDSOControl* This,
        UINT iTInfo,
        LCID lcid,
        ITypeInfo **ppTInfo);

    HRESULT (STDMETHODCALLTYPE *GetIDsOfNames)(
        IXMLDSOControl* This,
        REFIID riid,
        LPOLESTR *rgszNames,
        UINT cNames,
        LCID lcid,
        DISPID *rgDispId);

    HRESULT (STDMETHODCALLTYPE *Invoke)(
        IXMLDSOControl* This,
        DISPID dispIdMember,
        REFIID riid,
        LCID lcid,
        WORD wFlags,
        DISPPARAMS *pDispParams,
        VARIANT *pVarResult,
        EXCEPINFO *pExcepInfo,
        UINT *puArgErr);

    /*** IXMLDSOControl methods ***/
    HRESULT (STDMETHODCALLTYPE *get_XMLDocument)(
        IXMLDSOControl* This,
        IXMLDOMDocument **ppDoc);

    HRESULT (STDMETHODCALLTYPE *put_XMLDocument)(
        IXMLDSOControl* This,
        IXMLDOMDocument *ppDoc);

    HRESULT (STDMETHODCALLTYPE *get_JavaDSOCompatible)(
        IXMLDSOControl* This,
        BOOL *fJavaDSOCompatible);

    HRESULT (STDMETHODCALLTYPE *put_JavaDSOCompatible)(
        IXMLDSOControl* This,
        BOOL fJavaDSOCompatible);

    HRESULT (STDMETHODCALLTYPE *get_readyState)(
        IXMLDSOControl* This,
        long *state);

    END_INTERFACE
} IXMLDSOControlVtbl;
interface IXMLDSOControl {
    CONST_VTBL IXMLDSOControlVtbl* lpVtbl;
};

#ifdef COBJMACROS
/*** IUnknown methods ***/
#define IXMLDSOControl_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define IXMLDSOControl_AddRef(This) (This)->lpVtbl->AddRef(This)
#define IXMLDSOControl_Release(This) (This)->lpVtbl->Release(This)
/*** IDispatch methods ***/
#define IXMLDSOControl_GetTypeInfoCount(This,pctinfo) (This)->lpVtbl->GetTypeInfoCount(This,pctinfo)
#define IXMLDSOControl_GetTypeInfo(This,iTInfo,lcid,ppTInfo) (This)->lpVtbl->GetTypeInfo(This,iTInfo,lcid,ppTInfo)
#define IXMLDSOControl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) (This)->lpVtbl->GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)
#define IXMLDSOControl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) (This)->lpVtbl->Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)
/*** IXMLDSOControl methods ***/
#define IXMLDSOControl_get_XMLDocument(This,ppDoc) (This)->lpVtbl->get_XMLDocument(This,ppDoc)
#define IXMLDSOControl_put_XMLDocument(This,ppDoc) (This)->lpVtbl->put_XMLDocument(This,ppDoc)
#define IXMLDSOControl_get_JavaDSOCompatible(This,fJavaDSOCompatible) (This)->lpVtbl->get_JavaDSOCompatible(This,fJavaDSOCompatible)
#define IXMLDSOControl_put_JavaDSOCompatible(This,fJavaDSOCompatible) (This)->lpVtbl->put_JavaDSOCompatible(This,fJavaDSOCompatible)
#define IXMLDSOControl_get_readyState(This,state) (This)->lpVtbl->get_readyState(This,state)
#endif

#endif

HRESULT STDMETHODCALLTYPE IXMLDSOControl_get_XMLDocument_Proxy(
    IXMLDSOControl* This,
    IXMLDOMDocument **ppDoc);
void __RPC_STUB IXMLDSOControl_get_XMLDocument_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IXMLDSOControl_put_XMLDocument_Proxy(
    IXMLDSOControl* This,
    IXMLDOMDocument *ppDoc);
void __RPC_STUB IXMLDSOControl_put_XMLDocument_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IXMLDSOControl_get_JavaDSOCompatible_Proxy(
    IXMLDSOControl* This,
    BOOL *fJavaDSOCompatible);
void __RPC_STUB IXMLDSOControl_get_JavaDSOCompatible_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IXMLDSOControl_put_JavaDSOCompatible_Proxy(
    IXMLDSOControl* This,
    BOOL fJavaDSOCompatible);
void __RPC_STUB IXMLDSOControl_put_JavaDSOCompatible_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IXMLDSOControl_get_readyState_Proxy(
    IXMLDSOControl* This,
    long *state);
void __RPC_STUB IXMLDSOControl_get_readyState_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);

#endif  /* __IXMLDSOControl_INTERFACE_DEFINED__ */

/*****************************************************************************
 * XMLDSOControl coclass
 */

DEFINE_GUID(CLSID_XMLDSOControl, 0x550dda30, 0x0541, 0x11d2, 0x9c,0xa9, 0x00,0x60,0xb0,0xec,0x3d,0x39);

#ifndef __XMLDSOControl_FWD_DEFINED__
#define __XMLDSOControl_FWD_DEFINED__
typedef struct XMLDSOControl XMLDSOControl;
#endif /* defined __XMLDSOControl_FWD_DEFINED__ */

/* Begin additional prototypes for all interfaces */


/* End additional prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __WIDL_XMLDSO_H */