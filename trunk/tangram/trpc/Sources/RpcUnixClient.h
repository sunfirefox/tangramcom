/*
* Rpc Client,Unix scoket
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

#ifndef _RPCUNIXCLIENT_H_
#define _RPCUNIXCLIENT_H_

#include "RpcComCommon.h"
#include "RpcClientRequest.h"
#include "RpcUnixConnection.h"


#include <map>
using namespace std;

namespace nsTgmRpcCom{
	/*!
	* 服务器通知对象接口
	* 由ClientRpc实现
	*/
	class CRpcUnixClient:public ITgmRpcClient,CRpcUnixConnection
	{
	public:
		CRpcUnixClient(const char* inPath, BOOL isSimple = FALSE);
		virtual ~CRpcUnixClient();

		virtual HRESULT STDMETHODCALLTYPE QueryInterface ( /* [in] */ REFIID riid,/* [iid_is][out] */ void **ppvObject)
		{	return CRpcUnixConnection::QueryInterface(riid,ppvObject);		}

		virtual ULONG  STDMETHODCALLTYPE AddRef ()
		{	return CRpcUnixConnection::AddRef();		}

		virtual ULONG  STDMETHODCALLTYPE Release()
		{	return CRpcUnixConnection::Release();		}

		virtual HRESULT STDMETHODCALLTYPE Connect()
		{	return CRpcUnixConnection::Connect(m_szPath);	}

		virtual HRESULT STDMETHODCALLTYPE Disconnect()
		{	return CRpcUnixConnection::Disconnect();		}

		virtual HRESULT STDMETHODCALLTYPE Advise(ITgmRpcConnectionEvent * inEventHandler,ULONG inIsSvrConn) 
		{	return CRpcUnixConnection::Advise(inEventHandler,inIsSvrConn);	}

		virtual HRESULT STDMETHODCALLTYPE Unadvise()
		{	return CRpcUnixConnection::Unadvise();	}

		virtual HRESULT STDMETHODCALLTYPE SendSyncRequest(const void *inParamsPtr,ULONG inParmsSize,
			void *outParamsPtr/* = NULL*/,ULONG outParamsSize/* = 0*/,ULONG *outParamsLenPtr/* = NULL*/,
			ULONG inTimeOut /*= RpcCOM_INFINITE_TIMEOUT*/)
		{	return CRpcUnixConnection::SendSyncRequest(inParamsPtr,inParmsSize,outParamsPtr,outParamsSize,outParamsLenPtr,inTimeOut);	}

		virtual HRESULT STDMETHODCALLTYPE SendSyncRequestEx(const void *inParamsPtr,ULONG inParmsSize,
			void **outParamsPtr/* = NULL*/,ULONG *outParamsLenPtr/* = NULL*/,ipc_client_memory_free_fun_t * free_result_mem,
			ULONG inTimeOut /*= RpcCOM_INFINITE_TIMEOUT*/)
		{	return CRpcUnixConnection::SendSyncRequestEx(inParamsPtr,inParmsSize,outParamsPtr,outParamsLenPtr,free_result_mem,inTimeOut);	}

		virtual HRESULT STDMETHODCALLTYPE SendAsyncRequest(const void *inParamsPtr,ULONG inParmsSize,
			ITgmRpcConnectionAsyncReqCallBack *inCallback /*= NULL*/,
			ULONG *outReqNo/* = NULL*/,
			ULONG inUserData/* = 0*/,
			ULONG inTimeOut/* = RpcCOM_INFINITE_TIMEOUT*/)
		{	return CRpcUnixConnection::SendAsyncRequest(inParamsPtr,inParmsSize,inCallback,outReqNo,inUserData,inTimeOut);	}

		virtual HRESULT STDMETHODCALLTYPE SendNotify(const void *inParamsPtr,ULONG inParmsSize)
		{	return CRpcUnixConnection::SendNotify(inParamsPtr,inParmsSize);	}

		virtual HRESULT STDMETHODCALLTYPE GetResourceLocation(char* outBindString,ULONG inBingStringSize)
		{	return CRpcUnixConnection::GetResourceLocation(outBindString,inBingStringSize);		}
	
	private:
		char m_szPath[128];
	};
}//RpcComm

#endif	// End of _RPCUNIXCLIENT_H_
