/*** Autogenerated by WIDL 1.1.6 from comcat.idl - Do not edit ***/
#include <rpc.h>
#include <rpcndr.h>

#ifndef __WIDL_COMCAT_H
#define __WIDL_COMCAT_H
#ifdef __cplusplus
extern "C" {
#endif

#include <unknwn.h>
typedef GUID CATID;
typedef REFGUID REFCATID;
#define CATID_NULL GUID_NULL
#define IsEqualCATID(rcatid1, rcatid2) IsEqualGUID(rcatid1, rcatid2)
#define IEnumCATID IEnumGUID
#define IID_IEnumCATID IID_IEnumGUID
#define LPENUMCATID LPENUMGUID
#define IEnumCLSID IEnumGUID
#define IID_IEnumCLSID IID_IEnumGUID
#define LPENUMCLSID LPENUMGUID
#ifndef __IEnumGUID_FWD_DEFINED__
#define __IEnumGUID_FWD_DEFINED__
typedef interface IEnumGUID IEnumGUID;
#endif

typedef IEnumGUID *LPENUMGUID;
/*****************************************************************************
 * IEnumGUID interface
 */
#ifndef __IEnumGUID_INTERFACE_DEFINED__
#define __IEnumGUID_INTERFACE_DEFINED__

DEFINE_GUID(IID_IEnumGUID, 0x0002e000, 0x0000, 0x0000, 0xc0,0x00, 0x00,0x00,0x00,0x00,0x00,0x46);
#if defined(__cplusplus) && !defined(CINTERFACE)
interface IEnumGUID : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE Next(
        ULONG celt,
        GUID *rgelt,
        ULONG *pceltFetched) = 0;

    virtual HRESULT STDMETHODCALLTYPE Skip(
        ULONG celt) = 0;

    virtual HRESULT STDMETHODCALLTYPE Reset(
        ) = 0;

    virtual HRESULT STDMETHODCALLTYPE Clone(
        IEnumGUID **ppenum) = 0;

};
#else
typedef struct IEnumGUIDVtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        IEnumGUID* This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        IEnumGUID* This);

    ULONG (STDMETHODCALLTYPE *Release)(
        IEnumGUID* This);

    /*** IEnumGUID methods ***/
    HRESULT (STDMETHODCALLTYPE *Next)(
        IEnumGUID* This,
        ULONG celt,
        GUID *rgelt,
        ULONG *pceltFetched);

    HRESULT (STDMETHODCALLTYPE *Skip)(
        IEnumGUID* This,
        ULONG celt);

    HRESULT (STDMETHODCALLTYPE *Reset)(
        IEnumGUID* This);

    HRESULT (STDMETHODCALLTYPE *Clone)(
        IEnumGUID* This,
        IEnumGUID **ppenum);

    END_INTERFACE
} IEnumGUIDVtbl;
interface IEnumGUID {
    CONST_VTBL IEnumGUIDVtbl* lpVtbl;
};

#ifdef COBJMACROS
/*** IUnknown methods ***/
#define IEnumGUID_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define IEnumGUID_AddRef(This) (This)->lpVtbl->AddRef(This)
#define IEnumGUID_Release(This) (This)->lpVtbl->Release(This)
/*** IEnumGUID methods ***/
#define IEnumGUID_Next(This,celt,rgelt,pceltFetched) (This)->lpVtbl->Next(This,celt,rgelt,pceltFetched)
#define IEnumGUID_Skip(This,celt) (This)->lpVtbl->Skip(This,celt)
#define IEnumGUID_Reset(This) (This)->lpVtbl->Reset(This)
#define IEnumGUID_Clone(This,ppenum) (This)->lpVtbl->Clone(This,ppenum)
#endif

#endif

HRESULT STDMETHODCALLTYPE IEnumGUID_Next_Proxy(
    IEnumGUID* This,
    ULONG celt,
    GUID *rgelt,
    ULONG *pceltFetched);
void __RPC_STUB IEnumGUID_Next_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IEnumGUID_Skip_Proxy(
    IEnumGUID* This,
    ULONG celt);
void __RPC_STUB IEnumGUID_Skip_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IEnumGUID_Reset_Proxy(
    IEnumGUID* This);
void __RPC_STUB IEnumGUID_Reset_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IEnumGUID_Clone_Proxy(
    IEnumGUID* This,
    IEnumGUID **ppenum);
void __RPC_STUB IEnumGUID_Clone_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);

#endif  /* __IEnumGUID_INTERFACE_DEFINED__ */

#ifndef __IEnumCATEGORYINFO_FWD_DEFINED__
#define __IEnumCATEGORYINFO_FWD_DEFINED__
typedef interface IEnumCATEGORYINFO IEnumCATEGORYINFO;
#endif

typedef IEnumCATEGORYINFO *LPENUMCATEGORYINFO;
typedef struct tagCATEGORYINFO {
    CATID catid;
    LCID lcid;
    OLECHAR szDescription[128];
} CATEGORYINFO;
typedef struct tagCATEGORYINFO *LPCATEGORYINFO;
/*****************************************************************************
 * IEnumCATEGORYINFO interface
 */
#ifndef __IEnumCATEGORYINFO_INTERFACE_DEFINED__
#define __IEnumCATEGORYINFO_INTERFACE_DEFINED__

DEFINE_GUID(IID_IEnumCATEGORYINFO, 0x0002e011, 0x0000, 0x0000, 0xc0,0x00, 0x00,0x00,0x00,0x00,0x00,0x46);
#if defined(__cplusplus) && !defined(CINTERFACE)
interface IEnumCATEGORYINFO : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE Next(
        ULONG celt,
        CATEGORYINFO *rgelt,
        ULONG *pceltFetched) = 0;

    virtual HRESULT STDMETHODCALLTYPE Skip(
        ULONG celt) = 0;

    virtual HRESULT STDMETHODCALLTYPE Reset(
        ) = 0;

    virtual HRESULT STDMETHODCALLTYPE Clone(
        IEnumCATEGORYINFO **ppenum) = 0;

};
#else
typedef struct IEnumCATEGORYINFOVtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        IEnumCATEGORYINFO* This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        IEnumCATEGORYINFO* This);

    ULONG (STDMETHODCALLTYPE *Release)(
        IEnumCATEGORYINFO* This);

    /*** IEnumCATEGORYINFO methods ***/
    HRESULT (STDMETHODCALLTYPE *Next)(
        IEnumCATEGORYINFO* This,
        ULONG celt,
        CATEGORYINFO *rgelt,
        ULONG *pceltFetched);

    HRESULT (STDMETHODCALLTYPE *Skip)(
        IEnumCATEGORYINFO* This,
        ULONG celt);

    HRESULT (STDMETHODCALLTYPE *Reset)(
        IEnumCATEGORYINFO* This);

    HRESULT (STDMETHODCALLTYPE *Clone)(
        IEnumCATEGORYINFO* This,
        IEnumCATEGORYINFO **ppenum);

    END_INTERFACE
} IEnumCATEGORYINFOVtbl;
interface IEnumCATEGORYINFO {
    CONST_VTBL IEnumCATEGORYINFOVtbl* lpVtbl;
};

#ifdef COBJMACROS
/*** IUnknown methods ***/
#define IEnumCATEGORYINFO_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define IEnumCATEGORYINFO_AddRef(This) (This)->lpVtbl->AddRef(This)
#define IEnumCATEGORYINFO_Release(This) (This)->lpVtbl->Release(This)
/*** IEnumCATEGORYINFO methods ***/
#define IEnumCATEGORYINFO_Next(This,celt,rgelt,pceltFetched) (This)->lpVtbl->Next(This,celt,rgelt,pceltFetched)
#define IEnumCATEGORYINFO_Skip(This,celt) (This)->lpVtbl->Skip(This,celt)
#define IEnumCATEGORYINFO_Reset(This) (This)->lpVtbl->Reset(This)
#define IEnumCATEGORYINFO_Clone(This,ppenum) (This)->lpVtbl->Clone(This,ppenum)
#endif

#endif

HRESULT STDMETHODCALLTYPE IEnumCATEGORYINFO_Next_Proxy(
    IEnumCATEGORYINFO* This,
    ULONG celt,
    CATEGORYINFO *rgelt,
    ULONG *pceltFetched);
void __RPC_STUB IEnumCATEGORYINFO_Next_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IEnumCATEGORYINFO_Skip_Proxy(
    IEnumCATEGORYINFO* This,
    ULONG celt);
void __RPC_STUB IEnumCATEGORYINFO_Skip_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IEnumCATEGORYINFO_Reset_Proxy(
    IEnumCATEGORYINFO* This);
void __RPC_STUB IEnumCATEGORYINFO_Reset_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IEnumCATEGORYINFO_Clone_Proxy(
    IEnumCATEGORYINFO* This,
    IEnumCATEGORYINFO **ppenum);
void __RPC_STUB IEnumCATEGORYINFO_Clone_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);

#endif  /* __IEnumCATEGORYINFO_INTERFACE_DEFINED__ */

#ifndef __ICatInformation_FWD_DEFINED__
#define __ICatInformation_FWD_DEFINED__
typedef interface ICatInformation ICatInformation;
#endif

typedef ICatInformation *LPCATINFORMATION;
/*****************************************************************************
 * ICatInformation interface
 */
#ifndef __ICatInformation_INTERFACE_DEFINED__
#define __ICatInformation_INTERFACE_DEFINED__

DEFINE_GUID(IID_ICatInformation, 0x0002e013, 0x0000, 0x0000, 0xc0,0x00, 0x00,0x00,0x00,0x00,0x00,0x46);
#if defined(__cplusplus) && !defined(CINTERFACE)
interface ICatInformation : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE EnumCategories(
        LCID lcid,
        IEnumCATEGORYINFO **ppenumCategoryInfo) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetCategoryDesc(
        REFCATID rcatid,
        LCID lcid,
        LPWSTR *pszDesc) = 0;

    virtual HRESULT STDMETHODCALLTYPE EnumClassesOfCategories(
        ULONG cImplemented,
        CATID rgcatidImpl[],
        ULONG cRequired,
        CATID rgcatidReq[],
        IEnumGUID **ppenumClsid) = 0;

    virtual HRESULT STDMETHODCALLTYPE IsClassOfCategories(
        REFCLSID rclsid,
        ULONG cImplemented,
        CATID rgcatidImpl[],
        ULONG cRequired,
        CATID rgcatidReq[]) = 0;

    virtual HRESULT STDMETHODCALLTYPE EnumImplCategoriesOfClass(
        REFCLSID rclsid,
        IEnumGUID **ppenumCatid) = 0;

    virtual HRESULT STDMETHODCALLTYPE EnumReqCategoriesOfClass(
        REFCLSID rclsid,
        IEnumGUID **ppenumCatid) = 0;

};
#else
typedef struct ICatInformationVtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        ICatInformation* This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        ICatInformation* This);

    ULONG (STDMETHODCALLTYPE *Release)(
        ICatInformation* This);

    /*** ICatInformation methods ***/
    HRESULT (STDMETHODCALLTYPE *EnumCategories)(
        ICatInformation* This,
        LCID lcid,
        IEnumCATEGORYINFO **ppenumCategoryInfo);

    HRESULT (STDMETHODCALLTYPE *GetCategoryDesc)(
        ICatInformation* This,
        REFCATID rcatid,
        LCID lcid,
        LPWSTR *pszDesc);

    HRESULT (STDMETHODCALLTYPE *EnumClassesOfCategories)(
        ICatInformation* This,
        ULONG cImplemented,
        CATID rgcatidImpl[],
        ULONG cRequired,
        CATID rgcatidReq[],
        IEnumGUID **ppenumClsid);

    HRESULT (STDMETHODCALLTYPE *IsClassOfCategories)(
        ICatInformation* This,
        REFCLSID rclsid,
        ULONG cImplemented,
        CATID rgcatidImpl[],
        ULONG cRequired,
        CATID rgcatidReq[]);

    HRESULT (STDMETHODCALLTYPE *EnumImplCategoriesOfClass)(
        ICatInformation* This,
        REFCLSID rclsid,
        IEnumGUID **ppenumCatid);

    HRESULT (STDMETHODCALLTYPE *EnumReqCategoriesOfClass)(
        ICatInformation* This,
        REFCLSID rclsid,
        IEnumGUID **ppenumCatid);

    END_INTERFACE
} ICatInformationVtbl;
interface ICatInformation {
    CONST_VTBL ICatInformationVtbl* lpVtbl;
};

#ifdef COBJMACROS
/*** IUnknown methods ***/
#define ICatInformation_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define ICatInformation_AddRef(This) (This)->lpVtbl->AddRef(This)
#define ICatInformation_Release(This) (This)->lpVtbl->Release(This)
/*** ICatInformation methods ***/
#define ICatInformation_EnumCategories(This,lcid,ppenumCategoryInfo) (This)->lpVtbl->EnumCategories(This,lcid,ppenumCategoryInfo)
#define ICatInformation_GetCategoryDesc(This,rcatid,lcid,pszDesc) (This)->lpVtbl->GetCategoryDesc(This,rcatid,lcid,pszDesc)
#define ICatInformation_EnumClassesOfCategories(This,cImplemented,rgcatidImpl,cRequired,rgcatidReq,ppenumClsid) (This)->lpVtbl->EnumClassesOfCategories(This,cImplemented,rgcatidImpl,cRequired,rgcatidReq,ppenumClsid)
#define ICatInformation_IsClassOfCategories(This,rclsid,cImplemented,rgcatidImpl,cRequired,rgcatidReq) (This)->lpVtbl->IsClassOfCategories(This,rclsid,cImplemented,rgcatidImpl,cRequired,rgcatidReq)
#define ICatInformation_EnumImplCategoriesOfClass(This,rclsid,ppenumCatid) (This)->lpVtbl->EnumImplCategoriesOfClass(This,rclsid,ppenumCatid)
#define ICatInformation_EnumReqCategoriesOfClass(This,rclsid,ppenumCatid) (This)->lpVtbl->EnumReqCategoriesOfClass(This,rclsid,ppenumCatid)
#endif

#endif

HRESULT STDMETHODCALLTYPE ICatInformation_EnumCategories_Proxy(
    ICatInformation* This,
    LCID lcid,
    IEnumCATEGORYINFO **ppenumCategoryInfo);
void __RPC_STUB ICatInformation_EnumCategories_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ICatInformation_GetCategoryDesc_Proxy(
    ICatInformation* This,
    REFCATID rcatid,
    LCID lcid,
    LPWSTR *pszDesc);
void __RPC_STUB ICatInformation_GetCategoryDesc_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ICatInformation_RemoteEnumClassesOfCategories_Proxy(
    ICatInformation* This,
    ULONG cImplemented,
    CATID rgcatidImpl[],
    ULONG cRequired,
    CATID rgcatidReq[],
    IEnumGUID **ppenumClsid);
void __RPC_STUB ICatInformation_RemoteEnumClassesOfCategories_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ICatInformation_RemoteIsClassOfCategories_Proxy(
    ICatInformation* This,
    REFCLSID rclsid,
    ULONG cImplemented,
    CATID rgcatidImpl[],
    ULONG cRequired,
    CATID rgcatidReq[]);
void __RPC_STUB ICatInformation_RemoteIsClassOfCategories_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ICatInformation_EnumImplCategoriesOfClass_Proxy(
    ICatInformation* This,
    REFCLSID rclsid,
    IEnumGUID **ppenumCatid);
void __RPC_STUB ICatInformation_EnumImplCategoriesOfClass_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ICatInformation_EnumReqCategoriesOfClass_Proxy(
    ICatInformation* This,
    REFCLSID rclsid,
    IEnumGUID **ppenumCatid);
void __RPC_STUB ICatInformation_EnumReqCategoriesOfClass_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT CALLBACK ICatInformation_EnumClassesOfCategories_Proxy(
    ICatInformation* This,
    ULONG cImplemented,
    CATID rgcatidImpl[],
    ULONG cRequired,
    CATID rgcatidReq[],
    IEnumGUID **ppenumClsid);
HRESULT __RPC_STUB ICatInformation_EnumClassesOfCategories_Stub(
    ICatInformation* This,
    ULONG cImplemented,
    CATID rgcatidImpl[],
    ULONG cRequired,
    CATID rgcatidReq[],
    IEnumGUID **ppenumClsid);
HRESULT CALLBACK ICatInformation_IsClassOfCategories_Proxy(
    ICatInformation* This,
    REFCLSID rclsid,
    ULONG cImplemented,
    CATID rgcatidImpl[],
    ULONG cRequired,
    CATID rgcatidReq[]);
HRESULT __RPC_STUB ICatInformation_IsClassOfCategories_Stub(
    ICatInformation* This,
    REFCLSID rclsid,
    ULONG cImplemented,
    CATID rgcatidImpl[],
    ULONG cRequired,
    CATID rgcatidReq[]);

#endif  /* __ICatInformation_INTERFACE_DEFINED__ */

#ifndef __ICatRegister_FWD_DEFINED__
#define __ICatRegister_FWD_DEFINED__
typedef interface ICatRegister ICatRegister;
#endif

typedef ICatRegister *LPCATREGISTER;
/*****************************************************************************
 * ICatRegister interface
 */
#ifndef __ICatRegister_INTERFACE_DEFINED__
#define __ICatRegister_INTERFACE_DEFINED__

DEFINE_GUID(IID_ICatRegister, 0x0002e012, 0x0000, 0x0000, 0xc0,0x00, 0x00,0x00,0x00,0x00,0x00,0x46);
#if defined(__cplusplus) && !defined(CINTERFACE)
interface ICatRegister : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE RegisterCategories(
        ULONG cCategories,
        CATEGORYINFO rgCategoryInfo[]) = 0;

    virtual HRESULT STDMETHODCALLTYPE UnRegisterCategories(
        ULONG cCategories,
        CATID rgcatid[]) = 0;

    virtual HRESULT STDMETHODCALLTYPE RegisterClassImplCategories(
        REFCLSID rclsid,
        ULONG cCategories,
        CATID rgcatid[]) = 0;

    virtual HRESULT STDMETHODCALLTYPE UnRegisterClassImplCategories(
        REFCLSID rclsid,
        ULONG cCategories,
        CATID rgcatid[]) = 0;

    virtual HRESULT STDMETHODCALLTYPE RegisterClassReqCategories(
        REFCLSID rclsid,
        ULONG cCategories,
        CATID rgcatid[]) = 0;

    virtual HRESULT STDMETHODCALLTYPE UnRegisterClassReqCategories(
        REFCLSID rclsid,
        ULONG cCategories,
        CATID rgcatid[]) = 0;

};
#else
typedef struct ICatRegisterVtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        ICatRegister* This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        ICatRegister* This);

    ULONG (STDMETHODCALLTYPE *Release)(
        ICatRegister* This);

    /*** ICatRegister methods ***/
    HRESULT (STDMETHODCALLTYPE *RegisterCategories)(
        ICatRegister* This,
        ULONG cCategories,
        CATEGORYINFO rgCategoryInfo[]);

    HRESULT (STDMETHODCALLTYPE *UnRegisterCategories)(
        ICatRegister* This,
        ULONG cCategories,
        CATID rgcatid[]);

    HRESULT (STDMETHODCALLTYPE *RegisterClassImplCategories)(
        ICatRegister* This,
        REFCLSID rclsid,
        ULONG cCategories,
        CATID rgcatid[]);

    HRESULT (STDMETHODCALLTYPE *UnRegisterClassImplCategories)(
        ICatRegister* This,
        REFCLSID rclsid,
        ULONG cCategories,
        CATID rgcatid[]);

    HRESULT (STDMETHODCALLTYPE *RegisterClassReqCategories)(
        ICatRegister* This,
        REFCLSID rclsid,
        ULONG cCategories,
        CATID rgcatid[]);

    HRESULT (STDMETHODCALLTYPE *UnRegisterClassReqCategories)(
        ICatRegister* This,
        REFCLSID rclsid,
        ULONG cCategories,
        CATID rgcatid[]);

    END_INTERFACE
} ICatRegisterVtbl;
interface ICatRegister {
    CONST_VTBL ICatRegisterVtbl* lpVtbl;
};

#ifdef COBJMACROS
/*** IUnknown methods ***/
#define ICatRegister_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define ICatRegister_AddRef(This) (This)->lpVtbl->AddRef(This)
#define ICatRegister_Release(This) (This)->lpVtbl->Release(This)
/*** ICatRegister methods ***/
#define ICatRegister_RegisterCategories(This,cCategories,rgCategoryInfo) (This)->lpVtbl->RegisterCategories(This,cCategories,rgCategoryInfo)
#define ICatRegister_UnRegisterCategories(This,cCategories,rgcatid) (This)->lpVtbl->UnRegisterCategories(This,cCategories,rgcatid)
#define ICatRegister_RegisterClassImplCategories(This,rclsid,cCategories,rgcatid) (This)->lpVtbl->RegisterClassImplCategories(This,rclsid,cCategories,rgcatid)
#define ICatRegister_UnRegisterClassImplCategories(This,rclsid,cCategories,rgcatid) (This)->lpVtbl->UnRegisterClassImplCategories(This,rclsid,cCategories,rgcatid)
#define ICatRegister_RegisterClassReqCategories(This,rclsid,cCategories,rgcatid) (This)->lpVtbl->RegisterClassReqCategories(This,rclsid,cCategories,rgcatid)
#define ICatRegister_UnRegisterClassReqCategories(This,rclsid,cCategories,rgcatid) (This)->lpVtbl->UnRegisterClassReqCategories(This,rclsid,cCategories,rgcatid)
#endif

#endif

HRESULT STDMETHODCALLTYPE ICatRegister_RegisterCategories_Proxy(
    ICatRegister* This,
    ULONG cCategories,
    CATEGORYINFO rgCategoryInfo[]);
void __RPC_STUB ICatRegister_RegisterCategories_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ICatRegister_UnRegisterCategories_Proxy(
    ICatRegister* This,
    ULONG cCategories,
    CATID rgcatid[]);
void __RPC_STUB ICatRegister_UnRegisterCategories_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ICatRegister_RegisterClassImplCategories_Proxy(
    ICatRegister* This,
    REFCLSID rclsid,
    ULONG cCategories,
    CATID rgcatid[]);
void __RPC_STUB ICatRegister_RegisterClassImplCategories_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ICatRegister_UnRegisterClassImplCategories_Proxy(
    ICatRegister* This,
    REFCLSID rclsid,
    ULONG cCategories,
    CATID rgcatid[]);
void __RPC_STUB ICatRegister_UnRegisterClassImplCategories_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ICatRegister_RegisterClassReqCategories_Proxy(
    ICatRegister* This,
    REFCLSID rclsid,
    ULONG cCategories,
    CATID rgcatid[]);
void __RPC_STUB ICatRegister_RegisterClassReqCategories_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ICatRegister_UnRegisterClassReqCategories_Proxy(
    ICatRegister* This,
    REFCLSID rclsid,
    ULONG cCategories,
    CATID rgcatid[]);
void __RPC_STUB ICatRegister_UnRegisterClassReqCategories_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);

#endif  /* __ICatRegister_INTERFACE_DEFINED__ */

DEFINE_GUID( CATID_Insertable, 0x40fc6ed3, 0x2438, 0x11cf, 0xa3, 0xdb, 0x08, 0x00, 0x36, 0xf1, 0x25, 0x02);
DEFINE_GUID( CATID_Control, 0x40fc6ed4, 0x2438, 0x11cf, 0xa3, 0xdb, 0x08, 0x00, 0x36, 0xf1, 0x25, 0x02);
DEFINE_GUID( CATID_Programmable, 0x40fc6ed5, 0x2438, 0x11cf, 0xa3, 0xdb, 0x08, 0x00, 0x36, 0xf1, 0x25, 0x02);
DEFINE_GUID( CATID_IsShortcut, 0x40fc6ed6, 0x2438, 0x11cf, 0xa3, 0xdb, 0x08, 0x00, 0x36, 0xf1, 0x25, 0x02);
DEFINE_GUID( CATID_NeverShowExt, 0x40fc6ed7, 0x2438, 0x11cf, 0xa3, 0xdb, 0x08, 0x00, 0x36, 0xf1, 0x25, 0x02);
DEFINE_GUID( CATID_DocObject, 0x40fc6ed8, 0x2438, 0x11cf, 0xa3, 0xdb, 0x08, 0x00, 0x36, 0xf1, 0x25, 0x02);
DEFINE_GUID( CATID_Printable, 0x40fc6ed9, 0x2438, 0x11cf, 0xa3, 0xdb, 0x08, 0x00, 0x36, 0xf1, 0x25, 0x02);
DEFINE_GUID( CATID_RequiresDataPathHost, 0x0de86a50, 0x2baa, 0x11cf, 0xa2, 0x29, 0x00, 0xaa, 0x00, 0x3d, 0x73, 0x52);
DEFINE_GUID( CATID_PersistsToMoniker, 0x0de86a51, 0x2baa, 0x11cf, 0xa2, 0x29, 0x00, 0xaa, 0x00, 0x3d, 0x73, 0x52);
DEFINE_GUID( CATID_PersistsToStorage, 0x0de86a52, 0x2baa, 0x11cf, 0xa2, 0x29, 0x00, 0xaa, 0x00, 0x3d, 0x73, 0x52);
DEFINE_GUID( CATID_PersistsToStreamInit, 0x0de86a53, 0x2baa, 0x11cf, 0xa2, 0x29, 0x00, 0xaa, 0x00, 0x3d, 0x73, 0x52);
DEFINE_GUID( CATID_PersistsToStream, 0x0de86a54, 0x2baa, 0x11cf, 0xa2, 0x29, 0x00, 0xaa, 0x00, 0x3d, 0x73, 0x52);
DEFINE_GUID( CATID_PersistsToMemory, 0x0de86a55, 0x2baa, 0x11cf, 0xa2, 0x29, 0x00, 0xaa, 0x00, 0x3d, 0x73, 0x52);
DEFINE_GUID( CATID_PersistsToFile, 0x0de86a56, 0x2baa, 0x11cf, 0xa2, 0x29, 0x00, 0xaa, 0x00, 0x3d, 0x73, 0x52);
DEFINE_GUID( CATID_PersistsToPropertyBag, 0x0de86a57, 0x2baa, 0x11cf, 0xa2, 0x29, 0x00, 0xaa, 0x00, 0x3d, 0x73, 0x52);
DEFINE_GUID( CATID_InternetAware, 0x0de86a58, 0x2baa, 0x11cf, 0xa2, 0x29, 0x00, 0xaa, 0x00, 0x3d, 0x73, 0x52);
DEFINE_GUID( CATID_DesignTimeUIActivatableControl, 0xf2bb56d1, 0xdb07, 0x11d1, 0xaa, 0x6b, 0x00, 0x60, 0x97, 0xdb, 0x95, 0x39);
DEFINE_GUID(CLSID_StdComponentCategoriesMgr, 0x0002e005, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);
/* Begin additional prototypes for all interfaces */


/* End additional prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __WIDL_COMCAT_H */
