/*
* call scm client
*
* Copyright 2009  Jokul for Tranzda
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

#include <stdarg.h>
#include <string.h>

#define COBJMACROS
#define NONAMELESSUNION
#define NONAMELESSSTRUCT


//#include "OSBaseTypes.h"



#include "windef.h"
#include "winbase.h"
#include "winuser.h"
//#include "winsvc.h"
#include "objbase.h"
#include "objidl.h"
#include "ole2.h"
#include "rpc.h"
//#include "winerror.h"
//#include "winreg.h"
//#include "wine/unicode.h"
#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(ole);


#include "compobj_private.h"
//
//#include "tgmlog.h"
//
//#include "../os_include/scm_api.h"

#include "objidl.h"
#include "objbase.h"
#include "scm_api.h"
#include "scm_api_i_c.inc"
/* starts listening for a local server */


HRESULT RPC_StartLocalServer(REFCLSID clsid, IStream *stream, BOOL multi_use, void **registration)
{
	ISCMClsFactoryResloveService * pService = NULL;
    HRESULT hr;
    FB_TRACE("> rclsid:%s\n",TGMLOG_STR_GUID(clsid));
	hr = CoCreateInstance(&CLSID_SCMClsFactoryResloveService,NULL,
		CLSCTX_INPROC_SERVER,&IID_ISCMClsFactoryResloveService,(LPVOID *)&pService);
	if(SUCCEEDED(hr))
	{
		LARGE_INTEGER	seekto;
		ULARGE_INTEGER	newpos;
		STATSTG			ststg;

		unsigned char	*buffer;
		int 			buflen;
		ULONG			res;

		hr = IStream_Stat(stream,&ststg,0);
		if (FAILED(hr)) return hr;

		seekto.u.LowPart = 0;
		seekto.u.HighPart = 0;
		hr = IStream_Seek(stream,seekto,STREAM_SEEK_SET,&newpos);
        if (FAILED(hr)) {
            FE_ERR("< hr:%x\n",hr);
            return hr;
        }

		buflen = ststg.cbSize.u.LowPart;
		buffer = (unsigned char	*)HeapAlloc(GetProcessHeap(),0,buflen);

		hr = IStream_Read(stream,buffer,buflen,&res);
		if (FAILED(hr))	{
			HeapFree(GetProcessHeap(),0,buffer);
            FE_ERR("< hr:%x\n",hr);
			return hr;
		}

		hr = ISCMClsFactoryResloveService_Register(pService,clsid,res,buffer);

		if(SUCCEEDED(hr))
			*registration = pService;//直到结束服务器时才释放此业务接口。
		else
			ISCMClsFactoryResloveService_Release(pService);

		HeapFree(GetProcessHeap(),0,buffer);
	}
    FE_TRACE("< hr:%x\n",hr);
	return hr;
}

/* stops listening for a local server */
void RPC_StopLocalServer(void *registration)
{
	ISCMClsFactoryResloveService * pService = (ISCMClsFactoryResloveService *) registration;
	//todo unregister

	//ISCMClsFactoryResloveService_Unregister(pService,..);
    FB_TRACE("> \n");
	ISCMClsFactoryResloveService_Release(pService);
    FE_TRACE("< \n");
}


/* FIXME: should call to rpcss instead */
HRESULT RPC_GetLocalClassObject(REFCLSID rclsid, REFIID iid, LPVOID *ppv)
{
	ISCMClsFactoryResloveService * pService = NULL;
	LARGE_INTEGER	seekto;
	ULARGE_INTEGER	newpos;

	unsigned char	buffer[512];
	int 			buflen = 0;
	ULONG			res;

	IStream *pStm = NULL;
    HRESULT hr;

    FB_TRACE("> rclsid:%s\n",TGMLOG_STR_GUID(rclsid));
	hr = CoCreateInstance(&CLSID_SCMClsFactoryResloveService,NULL,
		CLSCTX_INPROC_SERVER,&IID_ISCMClsFactoryResloveService,(LPVOID *)&pService);

	if(SUCCEEDED(hr))
	{
		hr = ISCMClsFactoryResloveService_Query(pService,rclsid,512,buffer,&buflen);
	}

	hr = CreateStreamOnHGlobal(0,TRUE,&pStm);
	if (FAILED(hr)) goto out;
	hr = IStream_Write(pStm,buffer,buflen,&res);
	if (FAILED(hr)) goto out;
	seekto.u.LowPart = 0;seekto.u.HighPart = 0;
	hr = IStream_Seek(pStm,seekto,STREAM_SEEK_SET,&newpos);

	//20 unmarshalling classfactory
	TRACE("unmarshalling classfactory\n");
	hr = CoUnmarshalInterface(pStm,&IID_IClassFactory,ppv);
out:
	if(pStm)		IStream_Release(pStm);
	if(pService)	ISCMClsFactoryResloveService_Release(pService);
    FE_TRACE("< \n");
	return hr;
}

HRESULT RPC_QueryOXIDBindingHandle(const OXID *oxid,const IPID *ipid,RPC_BINDING_HANDLE  *bind)
{
	ISCMApartMentResloveService * pService = NULL;
	unsigned char	bindingString[512];
	RPC_STATUS		status =  -1;

	HRESULT hr ;

    FB_TRACE("> oxid: %s,ipid: %s\n",tgmlog_dbgstr_longlong(oxid),TGMLOG_STR_GUID(ipid));
    hr = CoCreateInstance(&CLSID_SCMApartMentResloveService,NULL,
		CLSCTX_INPROC_SERVER,&IID_ISCMApartMentResloveService,(LPVOID *)&pService);

	if(SUCCEEDED(hr))
	{
		hr = ISCMApartMentResloveService_Query(pService,*oxid,512,bindingString);
	
		if(SUCCEEDED(hr))
		{
			status = RpcBindingFromStringBindingA(bindingString,bind);

			if (status == RPC_S_OK)
			{
				IPID ipid2 = *ipid; /* why can't RpcBindingSetObject take a const? */
				status = RpcBindingSetObject(*bind, &ipid2);
				if (status != RPC_S_OK)
					RpcBindingFree(*bind);
			}
			else
			{
				//				ERR("Couldn't get binding for endpoint %s, status = %ld\n", TGMLOG_STR_W(endpoint), status);
				hr = HRESULT_FROM_WIN32(status);
			}
		}

		ISCMClsFactoryResloveService_Release(pService);
	}
    FE_TRACE("< hr:%x\n",hr);
	return hr;
}

HRESULT RPC_RegisterOXIDBindingHandle(const OXID *oxid,const char* bindingString)
{
	ISCMApartMentResloveService * pService = NULL;

	HRESULT hr = CoCreateInstance(&CLSID_SCMApartMentResloveService,NULL,
		CLSCTX_INPROC_SERVER,&IID_ISCMApartMentResloveService,(LPVOID *)&pService);

	if(SUCCEEDED(hr))
	{
		hr = ISCMApartMentResloveService_Register(pService,*oxid,bindingString);
		ISCMClsFactoryResloveService_Release(pService);
	}

	return hr;
}

HRESULT RPC_UnegisterOXIDBindingHandle(const OXID *oxid)
{
	ISCMApartMentResloveService * pService = NULL;

	HRESULT hr = CoCreateInstance(&CLSID_SCMApartMentResloveService,NULL,
		CLSCTX_INPROC_SERVER,&IID_ISCMApartMentResloveService,(LPVOID *)&pService);

	if(SUCCEEDED(hr))
	{
		hr = ISCMApartMentResloveService_Unregister(pService,*oxid);
		ISCMClsFactoryResloveService_Release(pService);
	}

	return hr;
}

void scm_rpc_server_status_changed_callback(int status,const char* bindString,void * userdata)
{
	switch(status)
	{
	case 1:
		RPC_RegisterOXIDBindingHandle((OXID*)userdata,bindString);
		break;
	case 0:
		RPC_UnegisterOXIDBindingHandle((OXID*)userdata);
		break;
	default:
	    break;
	}
	
}





