/*
* Rpc Connection,TCP Socket
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

#ifndef RPC_CONNECTION_HH_TXF
#define RPC_CONNECTION_HH_TXF

#include "OSBaseTypes.h"
#include "OSAtomic.h"
#include "TCPListenerSocket.h"
#include "TCPSocket.h"
#include "Task.h"

#include "RpcComCommon.h"
#include "RpcClientRequest.h"

#include <map>
using namespace std;

namespace nsTgmRpcCom
{
	class CRpcTCPConnection:public IUnknownImp<ITgmRpcConnection>,public Task
	{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
	public:
		CRpcTCPConnection(BOOL isServer = FALSE, BOOL isSimple = FALSE);
		virtual ~CRpcTCPConnection();

		//ULONG  STDMETHODCALLTYPE AddRef ()
		//{
		//	return os_atomic_add((ULONG*)&get_Reference(),1);
		//}

		ULONG  STDMETHODCALLTYPE Release()
		{
			ULONG ref = os_atomic_sub(&get_Reference(),1);
			Assert(ref <1000);
			if(0 == ref)	
				Signal(Task::kKillEvent);
			return ref;
		}

		/*! 断开连接 
		*  
		*  
		*/
		virtual HRESULT STDMETHODCALLTYPE Disconnect();

		/*!
		* 注册事件处理接口
		*
		*/
		virtual HRESULT STDMETHODCALLTYPE Advise(ITgmRpcConnectionEvent * inEventHandler,ULONG inIsSvrConn);

		/*!取消注册事件处理接口
		* 
		*
		*/
		virtual HRESULT STDMETHODCALLTYPE Unadvise();



		/*! 同步方法调用  
		*  当调用返回时，将直接填充各个输出参数。
		*  
		*/
		virtual HRESULT STDMETHODCALLTYPE SendSyncRequest(const void *inParamsPtr,ULONG inParmsSize,
			void *outParamsPtr/* = NULL*/,ULONG outParamsSize/* */,ULONG *outParamsLenPtr/* = NULL*/,
			ULONG inTimeOut /*= RpcCOM_INFINITE_TIMEOUT*/) ;

		virtual HRESULT STDMETHODCALLTYPE SendSyncRequestEx(const void *inParamsPtr,ULONG inParmsSize,
			void **outParamsPtr/* = NULL*/,ULONG *outParamsLenPtr/* = NULL*/,ipc_client_memory_free_fun_t * free_fun,
			ULONG inTimeOut /*= RpcCOM_INFINITE_TIMEOUT*/);

		/*! 异步方法调用
		*  当请求发送成功或者失败，调用立刻返回。
		*  当服务器端执行完相应的请求，调用指定的回调接口
		*/
		virtual HRESULT STDMETHODCALLTYPE SendAsyncRequest(const void *inParamsPtr,ULONG inParmsSize,
			ITgmRpcConnectionAsyncReqCallBack *inCallback /*= NULL*/,
			ULONG *outReqNo/* = NULL*/,
			ULONG inUserData/* */,
			ULONG inTimeOut/* = RpcCOM_INFINITE_TIMEOUT*/) ;

		/*! 通知,可以作为SendRequest的简化版  
		*  
		*  
		*/
		virtual HRESULT STDMETHODCALLTYPE SendNotify(const void *inParamsPtr,ULONG inParmsSize);

		/*!获取ResourceLocation
		* 
		*
		*/
		virtual HRESULT STDMETHODCALLTYPE GetResourceLocation(char* outBindString,ULONG inBingStringSize);

	public:

		virtual SInt64          Run();

		virtual HRESULT			SendReply(ULONG inSequNo,const void *inParamsPtr,ULONG inParmsSize);
		virtual HRESULT			SendResult(ULONG inSequNo,HRESULT inResult);

		TCPSocket&				get_Socket(){return mSocket;}

		HRESULT					Connect(ULONG inAddr,UInt16 inPort);//just for client connecting to server

		//HRESULT SendNotify(ULONG inSequNo,const void *inParamsPtr,ULONG inParmsSize)
		//{	return mProtseq.method_notify(inSequNo,(UInt8 *)inParamsPtr,inParmsSize);	}

	protected:
		HRESULT	AddRequest(ULONG inSeqNo,ClientRequestObject *inReqPtr);

		HRESULT GetRequest(ULONG inSeqNo,ClientRequestObject** outObj);

		void	CancelRequest(ULONG inSeqNo);

		void	CancelAllRequest(HRESULT inHr);

		virtual HRESULT	DisconnectUnexpected();

		// For simple client mode Synchronous operation.
		HRESULT WaitResult(RpcComPacket** ioPacket);
    protected:
        BOOL							mIsConnected;
        BOOL							mIsServer;
		BOOL							mIsSimple;

    private:
		CComSmartPtr<ITgmRpcConnectionEvent>	mEvtHandler;
        BOOL                                    mIsSvrConn;

		typedef map<ULONG, ClientRequestObject*> ReqObjMap_t;
		typedef ReqObjMap_t::iterator	ReqObjIter_t;
		ReqObjMap_t						mRequestObjs;

		TCPSocket						mSocket;
		RpcComProtocol					mProtseq;

		OSMutex							mMutex;

	};
}//RpcComm

#endif//#ifndef RPC_CLIENT_HH_TXF
