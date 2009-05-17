/*
* rpc com
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
#include "OSBaseTypes.h"
#include "OSMutex.h"
#include "tgmrpccom.h"
#include "scm_client.h"
#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(agrscm);

static ITgmRpcClient*		scm_rpc_client;
static int				scm_rpc_client_count;
#define	scm_rpc_client_mutex _scm_rpc_client_mutex()
static OSMutex& _scm_rpc_client_mutex()
{
	static OSMutex ins;
	return ins;
}

CScmClient::CScmClient()
{
	HRESULT hr = S_OK;
	OSMutexLocker _locker(&scm_rpc_client_mutex);
	if(scm_rpc_client_count++ == 0)
	{
		hr = nsTgmRpcCom::CRpcComFactory::CreateRpcClient(&scm_rpc_client,TANGRAMCOM_SCM_CONN_BINDING);
		if(SUCCEEDED(hr))
		{
//re_listen:
			hr = scm_rpc_client->Connect();
			if(hr == RPCCOM_E_CONN_DISCONNECTED)//start scm server
			{
				FIXME("try to start scm daemon\n");
				//system("scm_daemon.exe");
				//goto re_listen;
			}
		}
	}
	COND_EXEC(FAILED(hr),ERR("create scm client faile:%X,%s\n",hr,TANGRAMCOM_SCM_CONN_BINDING));
}

CScmClient::~CScmClient()
{
	OSMutexLocker _locker(&scm_rpc_client_mutex);
	if(--scm_rpc_client_count == 0)
	{
		scm_rpc_client->Release();
		scm_rpc_client = NULL;
	}
}

ITgmRpcClient* CScmClient::GetClient()
{
	return scm_rpc_client;
}




//////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE Proxy_ISCMClsFactoryResloveService::Register(REFIID in_clsid,int in_length,const unsigned char* in_data)
{
	TRACE("ClsFactoryResloveService >>:in_clsid(%s),in_length(%d),in_data(%s)\n",
		TGMLOG_STR_GUID(&in_clsid),in_length,TGMLOG_STR_AN((const char*)in_data,in_length));

	SCM_INCOKE_PARAM(ISCMClsFactoryResloveService,Register);
	theInParams.clsid	= in_clsid;
	theInParams.length	= in_length;
	memcpy(theInParams.objref,in_data,in_length);

	HRESULT hr =mClient.GetClient()->SendSyncRequest(&theInParams,sizeof(theInParams),&theOutParams,sizeof(theOutParams),NULL,0);
	if(SUCCEEDED(hr))
	{
		hr = theOutParams.result;
	}

	TRACE("ClsFactoryResloveService <<:hr(0x%X)\n",hr);
	return hr;
}

HRESULT STDMETHODCALLTYPE Proxy_ISCMClsFactoryResloveService::Unregister(REFIID in_clsid)
{
	TRACE("ClsFactoryResloveService >>:in_clsid(%s)\n",TGMLOG_STR_GUID(&in_clsid));

	SCM_INCOKE_PARAM(ISCMClsFactoryResloveService,Unregister);
	theInParams.clsid	= in_clsid;

	HRESULT hr =mClient.GetClient()->SendSyncRequest(&theInParams,sizeof(theInParams),&theOutParams,sizeof(theOutParams),NULL,0);
	if(SUCCEEDED(hr))
	{
		hr = theOutParams.result;
	}

	TRACE("ClsFactoryResloveService <<:hr(0x%X)\n",hr);
	return hr;
}

HRESULT STDMETHODCALLTYPE Proxy_ISCMClsFactoryResloveService::Query(REFIID in_clsid,int in_size,unsigned char* out_data,int *out_length)
{
	TRACE("ClsFactoryResloveService >>:in_clsid(%s),in_size(%d)\n",TGMLOG_STR_GUID(&in_clsid),in_size);
	
	SCM_INCOKE_PARAM(ISCMClsFactoryResloveService,Query);
	int length = 0;
	theInParams.clsid	= in_clsid;

	HRESULT hr =mClient.GetClient()->SendSyncRequest(&theInParams,sizeof(theInParams),&theOutParams,sizeof(theOutParams),NULL,0);
    if(SUCCEEDED(hr))
    {
        hr = theOutParams.result;
        if(SUCCEEDED(hr)){
            length = theOutParams.length;
            memcpy(out_data,theOutParams.objref,theOutParams.length);
        }
    }
	
	if(out_length)	*out_length = length;

	TRACE("ClsFactoryResloveService <<:hr(0x%X),*out_length(%d),%s\n",
		hr,length,(length > 0) ? TGMLOG_STR_AN((const char*)out_data,length):"(nill)");

	return hr;
}



HRESULT STDMETHODCALLTYPE Proxy_ISCMApartMentResloveService::Register(OXID in_oxid,const char* in_binding)	
{
	TRACE("ApartMentResloveService>>:in_oxid(%s),in_binding(%s)\n",tgmlog_dbgstr_longlong(in_oxid),in_binding);

	SCM_INCOKE_PARAM(ISCMApartMentResloveService,Register);
	theInParams.oxid	= in_oxid;
	if(NULL == in_binding && in_binding[0] == 0 && (strlen(in_binding)+1 > RPCBINGING_MAX_SIZE))
		return E_INVALIDARG;
	strcpy(theInParams.rrl,in_binding);

	HRESULT hr =mClient.GetClient()->SendSyncRequest(&theInParams,sizeof(theInParams),&theOutParams,sizeof(theOutParams),NULL,0);
	if(SUCCEEDED(hr))
	{
		hr = theOutParams.result;
	}
	
	TRACE("ApartMentResloveService<<:hr(0x%X)\n",hr);
	return hr;
}

HRESULT STDMETHODCALLTYPE Proxy_ISCMApartMentResloveService::Unregister(OXID in_oxid)
{
	TRACE("ApartMentResloveService>>:in_oxid(%s)\n",tgmlog_dbgstr_longlong(in_oxid));

	SCM_INCOKE_PARAM(ISCMApartMentResloveService,Unregister);
	theInParams.oxid	= in_oxid;

	HRESULT hr =mClient.GetClient()->SendSyncRequest(&theInParams,sizeof(theInParams),&theOutParams,sizeof(theOutParams),NULL,0);
	if(SUCCEEDED(hr))
	{
		hr = theOutParams.result;
	}
	TRACE("ApartMentResloveService<<:hr(0x%X)\n",hr);
	return hr;
}

HRESULT STDMETHODCALLTYPE Proxy_ISCMApartMentResloveService::Query(OXID in_oxid,int in_binding_size,char * out_binding)
{
	TRACE("ApartMentResloveService>>:in_oxid(%s),in_binding_size(%d)\n",tgmlog_dbgstr_longlong(in_oxid),in_binding_size);

	SCM_INCOKE_PARAM(ISCMApartMentResloveService,Query);
	theInParams.oxid	= in_oxid;
	out_binding[0] = 0;
	HRESULT hr =mClient.GetClient()->SendSyncRequest(&theInParams,sizeof(theInParams),&theOutParams,sizeof(theOutParams),NULL,0);
	if(SUCCEEDED(hr))
	{
		hr = theOutParams.result;
        if(SUCCEEDED(hr)){
            if(in_binding_size < (int)strlen(theOutParams.rrl)){
                WARN("E_OUTOFMEMORY\n");
                hr =  E_OUTOFMEMORY;
            }
            strcpy(out_binding,theOutParams.rrl);
        }
	}

	TRACE("ApartMentResloveService<<:hr(0x%X),out_binding(%s)\n",hr,out_binding);
	return hr;
}




