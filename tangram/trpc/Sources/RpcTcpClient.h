/*
* Rpc Client ,TCP socket
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

#ifndef RPC_CLIENT_HH_TXF
#define RPC_CLIENT_HH_TXF

#include "RpcComCommon.h"
#include "RpcClientRequest.h"
#include "RpcTcpConnection.h"


#include <map>
using namespace std;

namespace nsTgmRpcCom{
	/*!
	* 服务器通知对象接口
	* 由ClientRpc实现
	*/
	class CRpcTCPClient:public ITgmRpcClient,CRpcTCPConnection
	{
	public:
		CRpcTCPClient(ULONG inAddr = 0, UInt16 inPort = 0, BOOL isSimple = FALSE);
		virtual ~CRpcTCPClient();

		virtual HRESULT STDMETHODCALLTYPE QueryInterface ( /* [in] */ REFIID riid,/* [iid_is][out] */ void **ppvObject)
		{	return CRpcTCPConnection::QueryInterface(riid,ppvObject);		}

		virtual ULONG  STDMETHODCALLTYPE AddRef ()
		{	return CRpcTCPConnection::AddRef();		}

		virtual ULONG  STDMETHODCALLTYPE Release()
		{	return CRpcTCPConnection::Release();		}

		virtual HRESULT STDMETHODCALLTYPE Connect()
		{	return CRpcTCPConnection::Connect(mAddr,mPort);	}

		virtual HRESULT STDMETHODCALLTYPE Disconnect()
		{	return CRpcTCPConnection::Disconnect();		}

		virtual HRESULT STDMETHODCALLTYPE Advise(ITgmRpcConnectionEvent * inEventHandler,ULONG inIsSvrConn) 
		{	return CRpcTCPConnection::Advise(inEventHandler,inIsSvrConn);	}

		virtual HRESULT STDMETHODCALLTYPE Unadvise()
		{	return CRpcTCPConnection::Unadvise();	}

		virtual HRESULT STDMETHODCALLTYPE SendSyncRequest(const void *inParamsPtr,ULONG inParmsSize,
			void *outParamsPtr/* = NULL*/,ULONG outParamsSize/* = 0*/,ULONG *outParamsLenPtr/* = NULL*/,
			ULONG inTimeOut /*= RpcCOM_INFINITE_TIMEOUT*/)
		{	return CRpcTCPConnection::SendSyncRequest(inParamsPtr,inParmsSize,outParamsPtr,outParamsSize,outParamsLenPtr,inTimeOut);	}

		virtual HRESULT STDMETHODCALLTYPE SendSyncRequestEx(const void *inParamsPtr,ULONG inParmsSize,
			void **outParamsPtr/* = NULL*/,ULONG *outParamsLenPtr/* = NULL*/,ipc_client_memory_free_fun_t * free_result_mem,
			ULONG inTimeOut /*= RpcCOM_INFINITE_TIMEOUT*/)
		{	return CRpcTCPConnection::SendSyncRequestEx(inParamsPtr,inParmsSize,outParamsPtr,outParamsLenPtr,free_result_mem,inTimeOut);	}

		virtual HRESULT STDMETHODCALLTYPE SendAsyncRequest(const void *inParamsPtr,ULONG inParmsSize,
			ITgmRpcConnectionAsyncReqCallBack *inCallback /*= NULL*/,
			ULONG *outReqNo/* = NULL*/,
			ULONG inUserData/* = 0*/,
			ULONG inTimeOut/* = RpcCOM_INFINITE_TIMEOUT*/)
		{	return CRpcTCPConnection::SendAsyncRequest(inParamsPtr,inParmsSize,inCallback,outReqNo,inUserData,inTimeOut);	}

		virtual HRESULT STDMETHODCALLTYPE SendNotify(const void *inParamsPtr,ULONG inParmsSize)
		{	return CRpcTCPConnection::SendNotify(inParamsPtr,inParmsSize);	}

		virtual HRESULT STDMETHODCALLTYPE GetResourceLocation(char* outBindString,ULONG inBingStringSize)
		{	return CRpcTCPConnection::GetResourceLocation(outBindString,inBingStringSize);		}

	
	private:

		ULONG							mAddr;
		UInt16							mPort;

		//CRpcTCPConnection				mConnection;
	};


}//RpcComm

#endif//#ifndef RPC_CLIENT_HH_TXF
