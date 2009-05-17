/*
* COM proxy/stub factory (CStdPSFactory) implementation
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
#include <stdio.h>
#include <string.h>

#ifdef _WIN32//txf
#include <crtdbg.h>
#include "wtypes.h"
#include <time.h>
#define SNPRINTF	_snprintf	
#define VSNPRINTF	_vsnprintf
#else
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#define SNPRINTF	snprintf	
#define VSNPRINTF	vsnprintf
#endif

#define COBJMACROS

#include "windef.h"
#include "winbase.h"
#include "winerror.h"
#include "winreg.h"

#include "objbase.h"

#include "rpcproxy.h"

#include "tgmlog.h"

#include "cpsf.h"

TGMLOG_DEFAULT_DEBUG_CHANNEL(ole);

static BOOL FindProxyInfo(const ProxyFileInfo **pProxyFileList, REFIID riid, const ProxyFileInfo **pProxyInfo, int *pIndex)
{

    while (*pProxyFileList) {
        if ((*pProxyFileList)->pIIDLookupRtn(riid, pIndex)) {
            *pProxyInfo = *pProxyFileList;
            TRACE("found: ProxyInfo %p Index %d\n", *pProxyInfo, *pIndex);
            return TRUE;
        }
        pProxyFileList++;
    }
    TRACE("not found\n");
    return FALSE;
}

static HRESULT WINAPI CStdPSFactory_QueryInterface(LPPSFACTORYBUFFER iface,
                                                   REFIID riid,
                                                   LPVOID *obj)
{
    CStdPSFactoryBuffer *This = (CStdPSFactoryBuffer *)iface;
    TRACE("(%p)->QueryInterface(%s,%p)\n",iface,TGMLOG_STR_GUID(riid),obj);
    if (IsEqualGUID(&IID_IUnknown,riid) ||
        IsEqualGUID(&IID_IPSFactoryBuffer,riid)) {
            *obj = This;
            InterlockedIncrement( &This->RefCount );
            return S_OK;
        }
        return E_NOINTERFACE;
}

static ULONG WINAPI CStdPSFactory_AddRef(LPPSFACTORYBUFFER iface)
{
    CStdPSFactoryBuffer *This = (CStdPSFactoryBuffer *)iface;
    TRACE("(%p)->AddRef()\n",iface);
    return InterlockedIncrement( &This->RefCount );
}

static ULONG WINAPI CStdPSFactory_Release(LPPSFACTORYBUFFER iface)
{
    CStdPSFactoryBuffer *This = (CStdPSFactoryBuffer *)iface;
    TRACE("(%p)->Release()\n",iface);
    return InterlockedDecrement( &This->RefCount );
}

static HRESULT WINAPI CStdPSFactory_CreateProxy(LPPSFACTORYBUFFER iface,
                                                LPUNKNOWN pUnkOuter,
                                                REFIID riid,
                                                LPRPCPROXYBUFFER *ppProxy,
                                                LPVOID *ppv)
{
    CStdPSFactoryBuffer *This = (CStdPSFactoryBuffer *)iface;
    const ProxyFileInfo *ProxyInfo;
    int Index;
    HRESULT hr = E_FAIL;
    FB_TRACE("> (%p)->CreateProxy(%p,%s,%p,%p)\n",iface,pUnkOuter,
        TGMLOG_STR_GUID(riid),ppProxy,ppv);
    if (!FindProxyInfo(This->pProxyFileList,riid,&ProxyInfo,&Index))
        return E_NOINTERFACE;
    hr =  StdProxy_Construct(riid, pUnkOuter, ProxyInfo, Index, iface, ppProxy, ppv);
    FE_TRACE("< hr =0x%x\n",hr);
    return hr;
}

static HRESULT WINAPI CStdPSFactory_CreateStub(LPPSFACTORYBUFFER iface,
                                               REFIID riid,
                                               LPUNKNOWN pUnkServer,
                                               LPRPCSTUBBUFFER *ppStub)
{
    CStdPSFactoryBuffer *This = (CStdPSFactoryBuffer *)iface;
    const ProxyFileInfo *ProxyInfo;
    int Index;
    HRESULT hr;
    FB_TRACE("> (%p)->CreateStub(%s,%p,%p)\n",iface,TGMLOG_STR_GUID(riid),
        pUnkServer,ppStub);

    if (!FindProxyInfo(This->pProxyFileList,riid,&ProxyInfo,&Index))  {
        FE_TRACE("< hr =%x\n",E_NOINTERFACE);
        return E_NOINTERFACE;
    }

    if(ProxyInfo->pDelegatedIIDs && ProxyInfo->pDelegatedIIDs[Index]) {
        hr = CStdStubBuffer_Delegating_Construct(riid, pUnkServer, ProxyInfo->pNamesArray[Index],
            ProxyInfo->pStubVtblList[Index], ProxyInfo->pDelegatedIIDs[Index],
            iface, ppStub);
        FE_TRACE("< hr =%x\n",hr);
        return hr;
    }

    hr =  CStdStubBuffer_Construct(riid, pUnkServer, ProxyInfo->pNamesArray[Index],
        ProxyInfo->pStubVtblList[Index], iface, ppStub);
    FE_TRACE("< hr =%x\n",hr);
    return hr;
}

static const IPSFactoryBufferVtbl CStdPSFactory_Vtbl =
{
    CStdPSFactory_QueryInterface,
        CStdPSFactory_AddRef,
        CStdPSFactory_Release,
        CStdPSFactory_CreateProxy,
        CStdPSFactory_CreateStub
};

/***********************************************************************
*           NdrDllGetClassObject [RPCRT4.@]
*/
HRESULT WINAPI NdrDllGetClassObject(REFCLSID rclsid, REFIID iid, LPVOID *ppv,
                                    const ProxyFileInfo **pProxyFileList,
                                    const CLSID *pclsid,
                                    CStdPSFactoryBuffer *pPSFactoryBuffer)
{
    HRESULT hr = E_FAIL;
    FB_TRACE("> (%s, %s, %p, %p, %s, %p)\n", TGMLOG_STR_GUID(rclsid),
        TGMLOG_STR_GUID(iid), ppv, pProxyFileList, TGMLOG_STR_GUID(pclsid),
        pPSFactoryBuffer);

    *ppv = NULL;
    if (!pPSFactoryBuffer->lpVtbl) {
        const ProxyFileInfo **pProxyFileList2;
        int max_delegating_vtbl_size = 0;
        pPSFactoryBuffer->lpVtbl = &CStdPSFactory_Vtbl;
        pPSFactoryBuffer->RefCount = 0;
        pPSFactoryBuffer->pProxyFileList = pProxyFileList;
        for (pProxyFileList2 = pProxyFileList; *pProxyFileList2; pProxyFileList2++) {
            int i;
            for (i = 0; i < (*pProxyFileList2)->TableSize; i++) {
                /* FIXME: i think that different vtables should be copied for
                * async interfaces */
                void * const *pSrcRpcStubVtbl = (void * const *)&CStdStubBuffer_Vtbl;
                void **pRpcStubVtbl = (void **)&(*pProxyFileList2)->pStubVtblList[i]->Vtbl;
                int j;

                if ((*pProxyFileList2)->pDelegatedIIDs && (*pProxyFileList2)->pDelegatedIIDs[i]) {
                    pSrcRpcStubVtbl = (void * const *)&CStdStubBuffer_Delegating_Vtbl;
                    if ((*pProxyFileList2)->pStubVtblList[i]->header.DispatchTableCount > max_delegating_vtbl_size)
                        max_delegating_vtbl_size = (*pProxyFileList2)->pStubVtblList[i]->header.DispatchTableCount;
                }

                for (j = 0; j < sizeof(IRpcStubBufferVtbl)/sizeof(void *); j++)
                    if (!pRpcStubVtbl[j])
                        pRpcStubVtbl[j] = pSrcRpcStubVtbl[j];
            }
        }
        if(max_delegating_vtbl_size > 0)
            create_delegating_vtbl(max_delegating_vtbl_size);
    }

    if (pclsid && IsEqualGUID(rclsid, pclsid)){
        hr =  IPSFactoryBuffer_QueryInterface((LPPSFACTORYBUFFER)pPSFactoryBuffer, iid, ppv);
        FE_TRACE("< hr:0x%x\n",hr);
        return hr;
    }
    else {
        const ProxyFileInfo *info;
        int index;
        /* otherwise, the dll may be using the iid as the clsid, so
        * search for it in the proxy file list */
        if (FindProxyInfo(pProxyFileList, rclsid, &info, &index))
            return IPSFactoryBuffer_QueryInterface((LPPSFACTORYBUFFER)pPSFactoryBuffer, iid, ppv);

        FE_WARN("< class %s not available\n", TGMLOG_STR_GUID(rclsid));
        return CLASS_E_CLASSNOTAVAILABLE;
    }
}

/***********************************************************************
*           NdrDllCanUnloadNow [RPCRT4.@]
*/
HRESULT WINAPI NdrDllCanUnloadNow(CStdPSFactoryBuffer *pPSFactoryBuffer)
{
    return !(pPSFactoryBuffer->RefCount);
}

/***********************************************************************
*           NdrDllRegisterProxy [RPCRT4.@]
*/
HRESULT WINAPI NdrDllRegisterProxy(HMODULE hDll,
                                   const ProxyFileInfo **pProxyFileList,
                                   const CLSID *pclsid)
{
    LPSTR clsid;
    char keyname[120], module[MAX_PATH];
    HKEY key, subkey;
    DWORD len;

    FB_TRACE("> (%p,%p,%s)\n", hDll, pProxyFileList, TGMLOG_STR_GUID(pclsid));
    UuidToStringA((UUID*)pclsid, (unsigned char**)&clsid);

    /* register interfaces to point to clsid */
    while (*pProxyFileList) {
        unsigned u;
        for (u=0; u<(*pProxyFileList)->TableSize; u++) {
            CInterfaceStubVtbl *proxy = (*pProxyFileList)->pStubVtblList[u];
            PCInterfaceName name = (*pProxyFileList)->pNamesArray[u];
            LPSTR iid;

            TRACE("registering %s %s => %s\n", name, TGMLOG_STR_GUID(proxy->header.piid), clsid);

            UuidToStringA((UUID*)proxy->header.piid, (unsigned char**)&iid);
            SNPRINTF(keyname, sizeof(keyname), "Interface\\{%s}", iid);
            RpcStringFreeA((unsigned char**)&iid);
            if (RegCreateKeyExA(HKEY_CLASSES_ROOT, keyname, 0, NULL, 0,
                KEY_WRITE, NULL, &key, NULL) == ERROR_SUCCESS) {
                    if (name)
                        RegSetValueExA(key, NULL, 0, REG_SZ, (const BYTE *)name, strlen(name));
                    if (RegCreateKeyExA(key, "ProxyStubClsid32", 0, NULL, 0,
                        KEY_WRITE, NULL, &subkey, NULL) == ERROR_SUCCESS) {
                            SNPRINTF(module, sizeof(module), "{%s}", clsid);
                            RegSetValueExA(subkey, NULL, 0, REG_SZ, (LPBYTE)module, strlen(module));
                            RegCloseKey(subkey);
                        }
                        RegCloseKey(key);
                }
        }
        pProxyFileList++;
    }

    /* register clsid to point to module */
    SNPRINTF(keyname, sizeof(keyname), "CLSID\\{%s}", clsid);
    len = GetModuleFileNameA(hDll, module, sizeof(module));
    if (len && len < sizeof(module)) {
        TRACE("registering CLSID %s => %s\n", clsid, module);
        if (RegCreateKeyExA(HKEY_CLASSES_ROOT, keyname, 0, NULL, 0,
            KEY_WRITE, NULL, &key, NULL) == ERROR_SUCCESS) {
                // RegSetValueExA(subkey, NULL, 0, REG_SZ, (const BYTE *)"PSFactoryBuffer", strlen("PSFactoryBuffer"));//comment by txf
                if (RegCreateKeyExA(key, "InprocServer32", 0, NULL, 0,
                    KEY_WRITE, NULL, &subkey, NULL) == ERROR_SUCCESS) {
                        RegSetValueExA(subkey, NULL, 0, REG_SZ, (LPBYTE)module, strlen(module));
                        RegSetValueExA(subkey, "ThreadingModel", 0, REG_SZ, (const BYTE *)"Both", strlen("Both"));
                        RegCloseKey(subkey);
                    }
                    RegCloseKey(key);
            }
    }

    /* done */
    RpcStringFreeA((unsigned char**)&clsid);

    FE_TRACE("<\n");
    return S_OK;
}

/***********************************************************************
*           NdrDllUnregisterProxy [RPCRT4.@]
*/
HRESULT WINAPI NdrDllUnregisterProxy(HMODULE hDll,
                                     const ProxyFileInfo **pProxyFileList,
                                     const CLSID *pclsid)
{
    LPSTR clsid;
    char keyname[120], module[MAX_PATH];
    DWORD len;

    FB_TRACE("> (%p,%p,%s)\n", hDll, pProxyFileList, TGMLOG_STR_GUID(pclsid));
    UuidToStringA((UUID*)pclsid, (unsigned char**)&clsid);

    /* unregister interfaces */
    while (*pProxyFileList) {
        unsigned u;
        for (u=0; u<(*pProxyFileList)->TableSize; u++) {
            CInterfaceStubVtbl *proxy = (*pProxyFileList)->pStubVtblList[u];
            PCInterfaceName name = (*pProxyFileList)->pNamesArray[u];
            LPSTR iid;

            TRACE("unregistering %s %s <= %s\n", name, TGMLOG_STR_GUID(proxy->header.piid), clsid);

            UuidToStringA((UUID*)proxy->header.piid, (unsigned char**)&iid);
            SNPRINTF(keyname, sizeof(keyname), "Interface\\{%s}", iid);
            RpcStringFreeA((unsigned char**)&iid);
            RegDeleteKeyA(HKEY_CLASSES_ROOT, keyname);
        }
        pProxyFileList++;
    }

    /* unregister clsid */
    SNPRINTF(keyname, sizeof(keyname), "CLSID\\{%s}", clsid);
    len = GetModuleFileNameA(hDll, module, sizeof(module));
    if (len && len < sizeof(module)) {
        TRACE("unregistering CLSID %s <= %s\n", clsid, module);
        RegDeleteKeyA(HKEY_CLASSES_ROOT, keyname);
    }

    /* done */
    RpcStringFreeA((unsigned char**)&clsid);

    FE_TRACE("< \n");
    return S_OK;
}
