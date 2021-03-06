/*** Autogenerated by WIDL 1.1.6 from htiframe.idl - Do not edit ***/
#include <rpc.h>
#include <rpcndr.h>

#ifndef __WIDL_HTIFRAME_H
#define __WIDL_HTIFRAME_H
#ifdef __cplusplus
extern "C" {
#endif

#include <objidl.h>
#include <oleidl.h>
#ifndef __ITargetNotify_FWD_DEFINED__
#define __ITargetNotify_FWD_DEFINED__
typedef interface ITargetNotify ITargetNotify;
#endif

typedef ITargetNotify *LPTARGETNOTIFY;
/*****************************************************************************
 * ITargetNotify interface
 */
#ifndef __ITargetNotify_INTERFACE_DEFINED__
#define __ITargetNotify_INTERFACE_DEFINED__

DEFINE_GUID(IID_ITargetNotify, 0x863a99a0, 0x21bc, 0x11d0, 0x82,0xb4, 0x00,0xa0,0xc9,0x0c,0x29,0xc5);
#if defined(__cplusplus) && !defined(CINTERFACE)
interface ITargetNotify : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE OnCreate(
        IUnknown *pUnkDestination,
        ULONG cbCookie) = 0;

    virtual HRESULT STDMETHODCALLTYPE OnReuse(
        IUnknown *pUnkDestination) = 0;

};
#else
typedef struct ITargetNotifyVtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        ITargetNotify* This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        ITargetNotify* This);

    ULONG (STDMETHODCALLTYPE *Release)(
        ITargetNotify* This);

    /*** ITargetNotify methods ***/
    HRESULT (STDMETHODCALLTYPE *OnCreate)(
        ITargetNotify* This,
        IUnknown *pUnkDestination,
        ULONG cbCookie);

    HRESULT (STDMETHODCALLTYPE *OnReuse)(
        ITargetNotify* This,
        IUnknown *pUnkDestination);

    END_INTERFACE
} ITargetNotifyVtbl;
interface ITargetNotify {
    CONST_VTBL ITargetNotifyVtbl* lpVtbl;
};

#ifdef COBJMACROS
/*** IUnknown methods ***/
#define ITargetNotify_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define ITargetNotify_AddRef(This) (This)->lpVtbl->AddRef(This)
#define ITargetNotify_Release(This) (This)->lpVtbl->Release(This)
/*** ITargetNotify methods ***/
#define ITargetNotify_OnCreate(This,pUnkDestination,cbCookie) (This)->lpVtbl->OnCreate(This,pUnkDestination,cbCookie)
#define ITargetNotify_OnReuse(This,pUnkDestination) (This)->lpVtbl->OnReuse(This,pUnkDestination)
#endif

#endif

HRESULT STDMETHODCALLTYPE ITargetNotify_OnCreate_Proxy(
    ITargetNotify* This,
    IUnknown *pUnkDestination,
    ULONG cbCookie);
void __RPC_STUB ITargetNotify_OnCreate_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ITargetNotify_OnReuse_Proxy(
    ITargetNotify* This,
    IUnknown *pUnkDestination);
void __RPC_STUB ITargetNotify_OnReuse_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);

#endif  /* __ITargetNotify_INTERFACE_DEFINED__ */

#ifndef __ITargetNotify2_FWD_DEFINED__
#define __ITargetNotify2_FWD_DEFINED__
typedef interface ITargetNotify2 ITargetNotify2;
#endif

typedef ITargetNotify2 *LPTARGETNOTIFY2;
/*****************************************************************************
 * ITargetNotify2 interface
 */
#ifndef __ITargetNotify2_INTERFACE_DEFINED__
#define __ITargetNotify2_INTERFACE_DEFINED__

DEFINE_GUID(IID_ITargetNotify2, 0x3050f6b1, 0x98b5, 0x11cf, 0xbb,0x82, 0x00,0xaa,0x00,0xbd,0xce,0x0b);
#if defined(__cplusplus) && !defined(CINTERFACE)
interface ITargetNotify2 : public ITargetNotify
{
    virtual HRESULT STDMETHODCALLTYPE GetOptionString(
        BSTR *pbstrOptions) = 0;

};
#else
typedef struct ITargetNotify2Vtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        ITargetNotify2* This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        ITargetNotify2* This);

    ULONG (STDMETHODCALLTYPE *Release)(
        ITargetNotify2* This);

    /*** ITargetNotify methods ***/
    HRESULT (STDMETHODCALLTYPE *OnCreate)(
        ITargetNotify2* This,
        IUnknown *pUnkDestination,
        ULONG cbCookie);

    HRESULT (STDMETHODCALLTYPE *OnReuse)(
        ITargetNotify2* This,
        IUnknown *pUnkDestination);

    /*** ITargetNotify2 methods ***/
    HRESULT (STDMETHODCALLTYPE *GetOptionString)(
        ITargetNotify2* This,
        BSTR *pbstrOptions);

    END_INTERFACE
} ITargetNotify2Vtbl;
interface ITargetNotify2 {
    CONST_VTBL ITargetNotify2Vtbl* lpVtbl;
};

#ifdef COBJMACROS
/*** IUnknown methods ***/
#define ITargetNotify2_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define ITargetNotify2_AddRef(This) (This)->lpVtbl->AddRef(This)
#define ITargetNotify2_Release(This) (This)->lpVtbl->Release(This)
/*** ITargetNotify methods ***/
#define ITargetNotify2_OnCreate(This,pUnkDestination,cbCookie) (This)->lpVtbl->OnCreate(This,pUnkDestination,cbCookie)
#define ITargetNotify2_OnReuse(This,pUnkDestination) (This)->lpVtbl->OnReuse(This,pUnkDestination)
/*** ITargetNotify2 methods ***/
#define ITargetNotify2_GetOptionString(This,pbstrOptions) (This)->lpVtbl->GetOptionString(This,pbstrOptions)
#endif

#endif

HRESULT STDMETHODCALLTYPE ITargetNotify2_GetOptionString_Proxy(
    ITargetNotify2* This,
    BSTR *pbstrOptions);
void __RPC_STUB ITargetNotify2_GetOptionString_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);

#endif  /* __ITargetNotify2_INTERFACE_DEFINED__ */

#ifndef __ITargetFrame2_FWD_DEFINED__
#define __ITargetFrame2_FWD_DEFINED__
typedef interface ITargetFrame2 ITargetFrame2;
#endif

typedef ITargetFrame2 *LPTARGETFRAME2;
typedef enum __WIDL_htiframe_generated_name_00000000 {
    FINDFRAME_NONE = 0,
    FINDFRAME_JUSTTESTEXISTENCE = 1,
    FINDFRAME_INTERNAL = 0x80000000
} FINDFRAME_FLAGS;
typedef enum __WIDL_htiframe_generated_name_00000001 {
    FRAMEOPTIONS_SCROLL_YES = 0x1,
    FRAMEOPTIONS_SCROLL_NO = 0x2,
    FRAMEOPTIONS_SCROLL_AUTO = 0x4,
    FRAMEOPTIONS_NORESIZE = 0x8,
    FRAMEOPTIONS_NO3DBORDER = 0x10,
    FRAMEOPTIONS_DESKTOP = 0x20,
    FRAMEOPTIONS_BROWSERBAND = 0x40
} FRAMEOPTIONS_FLAGS;
/*****************************************************************************
 * ITargetFrame2 interface
 */
#ifndef __ITargetFrame2_INTERFACE_DEFINED__
#define __ITargetFrame2_INTERFACE_DEFINED__

DEFINE_GUID(IID_ITargetFrame2, 0x86d52e11, 0x94a8, 0x11d0, 0x82,0xaf, 0x00,0xc0,0x4f,0xd5,0xae,0x38);
#if defined(__cplusplus) && !defined(CINTERFACE)
interface ITargetFrame2 : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE SetFrameName(
        LPCWSTR pszFrameName) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetFrameName(
        LPWSTR *ppszFrameName) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetParentFrame(
        IUnknown **ppunkParent) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetFrameSrc(
        LPCWSTR pszFrameSrc) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetFrameSrc(
        LPWSTR *ppszFrameSrc) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetFramesContainer(
        IOleContainer **ppContainer) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetFrameOptions(
        DWORD dwFlags) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetFrameOptions(
        DWORD *pdwFlags) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetFrameMargins(
        DWORD dwWidth,
        DWORD dwHeight) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetFrameMargins(
        DWORD *pdwWidth,
        DWORD *pdwHeight) = 0;

    virtual HRESULT STDMETHODCALLTYPE FindFrame(
        LPCWSTR pszTargetName,
        DWORD dwFlags,
        IUnknown **ppunkTargetFrame) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetTargetAlias(
        LPCWSTR pszTargetName,
        LPWSTR *ppszTargetAlias) = 0;

};
#else
typedef struct ITargetFrame2Vtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        ITargetFrame2* This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        ITargetFrame2* This);

    ULONG (STDMETHODCALLTYPE *Release)(
        ITargetFrame2* This);

    /*** ITargetFrame2 methods ***/
    HRESULT (STDMETHODCALLTYPE *SetFrameName)(
        ITargetFrame2* This,
        LPCWSTR pszFrameName);

    HRESULT (STDMETHODCALLTYPE *GetFrameName)(
        ITargetFrame2* This,
        LPWSTR *ppszFrameName);

    HRESULT (STDMETHODCALLTYPE *GetParentFrame)(
        ITargetFrame2* This,
        IUnknown **ppunkParent);

    HRESULT (STDMETHODCALLTYPE *SetFrameSrc)(
        ITargetFrame2* This,
        LPCWSTR pszFrameSrc);

    HRESULT (STDMETHODCALLTYPE *GetFrameSrc)(
        ITargetFrame2* This,
        LPWSTR *ppszFrameSrc);

    HRESULT (STDMETHODCALLTYPE *GetFramesContainer)(
        ITargetFrame2* This,
        IOleContainer **ppContainer);

    HRESULT (STDMETHODCALLTYPE *SetFrameOptions)(
        ITargetFrame2* This,
        DWORD dwFlags);

    HRESULT (STDMETHODCALLTYPE *GetFrameOptions)(
        ITargetFrame2* This,
        DWORD *pdwFlags);

    HRESULT (STDMETHODCALLTYPE *SetFrameMargins)(
        ITargetFrame2* This,
        DWORD dwWidth,
        DWORD dwHeight);

    HRESULT (STDMETHODCALLTYPE *GetFrameMargins)(
        ITargetFrame2* This,
        DWORD *pdwWidth,
        DWORD *pdwHeight);

    HRESULT (STDMETHODCALLTYPE *FindFrame)(
        ITargetFrame2* This,
        LPCWSTR pszTargetName,
        DWORD dwFlags,
        IUnknown **ppunkTargetFrame);

    HRESULT (STDMETHODCALLTYPE *GetTargetAlias)(
        ITargetFrame2* This,
        LPCWSTR pszTargetName,
        LPWSTR *ppszTargetAlias);

    END_INTERFACE
} ITargetFrame2Vtbl;
interface ITargetFrame2 {
    CONST_VTBL ITargetFrame2Vtbl* lpVtbl;
};

#ifdef COBJMACROS
/*** IUnknown methods ***/
#define ITargetFrame2_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define ITargetFrame2_AddRef(This) (This)->lpVtbl->AddRef(This)
#define ITargetFrame2_Release(This) (This)->lpVtbl->Release(This)
/*** ITargetFrame2 methods ***/
#define ITargetFrame2_SetFrameName(This,pszFrameName) (This)->lpVtbl->SetFrameName(This,pszFrameName)
#define ITargetFrame2_GetFrameName(This,ppszFrameName) (This)->lpVtbl->GetFrameName(This,ppszFrameName)
#define ITargetFrame2_GetParentFrame(This,ppunkParent) (This)->lpVtbl->GetParentFrame(This,ppunkParent)
#define ITargetFrame2_SetFrameSrc(This,pszFrameSrc) (This)->lpVtbl->SetFrameSrc(This,pszFrameSrc)
#define ITargetFrame2_GetFrameSrc(This,ppszFrameSrc) (This)->lpVtbl->GetFrameSrc(This,ppszFrameSrc)
#define ITargetFrame2_GetFramesContainer(This,ppContainer) (This)->lpVtbl->GetFramesContainer(This,ppContainer)
#define ITargetFrame2_SetFrameOptions(This,dwFlags) (This)->lpVtbl->SetFrameOptions(This,dwFlags)
#define ITargetFrame2_GetFrameOptions(This,pdwFlags) (This)->lpVtbl->GetFrameOptions(This,pdwFlags)
#define ITargetFrame2_SetFrameMargins(This,dwWidth,dwHeight) (This)->lpVtbl->SetFrameMargins(This,dwWidth,dwHeight)
#define ITargetFrame2_GetFrameMargins(This,pdwWidth,pdwHeight) (This)->lpVtbl->GetFrameMargins(This,pdwWidth,pdwHeight)
#define ITargetFrame2_FindFrame(This,pszTargetName,dwFlags,ppunkTargetFrame) (This)->lpVtbl->FindFrame(This,pszTargetName,dwFlags,ppunkTargetFrame)
#define ITargetFrame2_GetTargetAlias(This,pszTargetName,ppszTargetAlias) (This)->lpVtbl->GetTargetAlias(This,pszTargetName,ppszTargetAlias)
#endif

#endif

HRESULT STDMETHODCALLTYPE ITargetFrame2_SetFrameName_Proxy(
    ITargetFrame2* This,
    LPCWSTR pszFrameName);
void __RPC_STUB ITargetFrame2_SetFrameName_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ITargetFrame2_GetFrameName_Proxy(
    ITargetFrame2* This,
    LPWSTR *ppszFrameName);
void __RPC_STUB ITargetFrame2_GetFrameName_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ITargetFrame2_GetParentFrame_Proxy(
    ITargetFrame2* This,
    IUnknown **ppunkParent);
void __RPC_STUB ITargetFrame2_GetParentFrame_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ITargetFrame2_SetFrameSrc_Proxy(
    ITargetFrame2* This,
    LPCWSTR pszFrameSrc);
void __RPC_STUB ITargetFrame2_SetFrameSrc_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ITargetFrame2_GetFrameSrc_Proxy(
    ITargetFrame2* This,
    LPWSTR *ppszFrameSrc);
void __RPC_STUB ITargetFrame2_GetFrameSrc_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ITargetFrame2_GetFramesContainer_Proxy(
    ITargetFrame2* This,
    IOleContainer **ppContainer);
void __RPC_STUB ITargetFrame2_GetFramesContainer_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ITargetFrame2_SetFrameOptions_Proxy(
    ITargetFrame2* This,
    DWORD dwFlags);
void __RPC_STUB ITargetFrame2_SetFrameOptions_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ITargetFrame2_GetFrameOptions_Proxy(
    ITargetFrame2* This,
    DWORD *pdwFlags);
void __RPC_STUB ITargetFrame2_GetFrameOptions_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ITargetFrame2_SetFrameMargins_Proxy(
    ITargetFrame2* This,
    DWORD dwWidth,
    DWORD dwHeight);
void __RPC_STUB ITargetFrame2_SetFrameMargins_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ITargetFrame2_GetFrameMargins_Proxy(
    ITargetFrame2* This,
    DWORD *pdwWidth,
    DWORD *pdwHeight);
void __RPC_STUB ITargetFrame2_GetFrameMargins_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ITargetFrame2_FindFrame_Proxy(
    ITargetFrame2* This,
    LPCWSTR pszTargetName,
    DWORD dwFlags,
    IUnknown **ppunkTargetFrame);
void __RPC_STUB ITargetFrame2_FindFrame_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ITargetFrame2_GetTargetAlias_Proxy(
    ITargetFrame2* This,
    LPCWSTR pszTargetName,
    LPWSTR *ppszTargetAlias);
void __RPC_STUB ITargetFrame2_GetTargetAlias_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);

#endif  /* __ITargetFrame2_INTERFACE_DEFINED__ */

#ifndef __ITargetContainer_FWD_DEFINED__
#define __ITargetContainer_FWD_DEFINED__
typedef interface ITargetContainer ITargetContainer;
#endif

typedef ITargetContainer *LPTARGETCONTAINER;
/*****************************************************************************
 * ITargetContainer interface
 */
#ifndef __ITargetContainer_INTERFACE_DEFINED__
#define __ITargetContainer_INTERFACE_DEFINED__

DEFINE_GUID(IID_ITargetContainer, 0x7847ec01, 0x2bec, 0x11d0, 0x82,0xb4, 0x00,0xa0,0xc9,0x0c,0x29,0xc5);
#if defined(__cplusplus) && !defined(CINTERFACE)
interface ITargetContainer : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE GetFrameUrl(
        LPWSTR *ppszFrameSrc) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetFramesContainer(
        IOleContainer **ppContainer) = 0;

};
#else
typedef struct ITargetContainerVtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        ITargetContainer* This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        ITargetContainer* This);

    ULONG (STDMETHODCALLTYPE *Release)(
        ITargetContainer* This);

    /*** ITargetContainer methods ***/
    HRESULT (STDMETHODCALLTYPE *GetFrameUrl)(
        ITargetContainer* This,
        LPWSTR *ppszFrameSrc);

    HRESULT (STDMETHODCALLTYPE *GetFramesContainer)(
        ITargetContainer* This,
        IOleContainer **ppContainer);

    END_INTERFACE
} ITargetContainerVtbl;
interface ITargetContainer {
    CONST_VTBL ITargetContainerVtbl* lpVtbl;
};

#ifdef COBJMACROS
/*** IUnknown methods ***/
#define ITargetContainer_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define ITargetContainer_AddRef(This) (This)->lpVtbl->AddRef(This)
#define ITargetContainer_Release(This) (This)->lpVtbl->Release(This)
/*** ITargetContainer methods ***/
#define ITargetContainer_GetFrameUrl(This,ppszFrameSrc) (This)->lpVtbl->GetFrameUrl(This,ppszFrameSrc)
#define ITargetContainer_GetFramesContainer(This,ppContainer) (This)->lpVtbl->GetFramesContainer(This,ppContainer)
#endif

#endif

HRESULT STDMETHODCALLTYPE ITargetContainer_GetFrameUrl_Proxy(
    ITargetContainer* This,
    LPWSTR *ppszFrameSrc);
void __RPC_STUB ITargetContainer_GetFrameUrl_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ITargetContainer_GetFramesContainer_Proxy(
    ITargetContainer* This,
    IOleContainer **ppContainer);
void __RPC_STUB ITargetContainer_GetFramesContainer_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);

#endif  /* __ITargetContainer_INTERFACE_DEFINED__ */

/* Begin additional prototypes for all interfaces */

ULONG           __RPC_USER BSTR_UserSize     (ULONG *, ULONG, BSTR *);
unsigned char * __RPC_USER BSTR_UserMarshal  (ULONG *, unsigned char *, BSTR *);
unsigned char * __RPC_USER BSTR_UserUnmarshal(ULONG *, unsigned char *, BSTR *);
void            __RPC_USER BSTR_UserFree     (ULONG *, BSTR *);

/* End additional prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __WIDL_HTIFRAME_H */
