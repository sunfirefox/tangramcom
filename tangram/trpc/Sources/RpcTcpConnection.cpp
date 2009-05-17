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

#include "RpcTcpConnection.h"

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(agrrpc);
#include <assert.h>

namespace nsTgmRpcCom
{	
	/*!
	* 客户请求对象接口
	* 由ServerRpc实现
	*/
	class CRpcTcpConnectionRequestObj:public IUnknownImp<ITgmRpcConnectionRequestObject>
	{
	public:
		/*!
		* 获取请求/通知序号
		*
		*/
		virtual ULONG STDMETHODCALLTYPE GetSequNo()
		{	return mPacketPtr->sequ_no;	}

		/*!
		* 获取请求调用参数
		*
		*/
		virtual const void* STDMETHODCALLTYPE GetParamsPtr()
		{   return mPacketPtr->data_buf;	}

		/*!
		* 获取请求调用参数大小
		*
		*/
		virtual ULONG STDMETHODCALLTYPE GetParamsSize()
		{	return mPacketPtr->data_size;	}

		/*!
		* 发送应答
		*
		*/
		virtual HRESULT STDMETHODCALLTYPE SendReply(const void *inParamsPtr,ULONG inParmsSize)
		{
			return mConnection->SendReply(mPacketPtr->sequ_no,(UInt8 *)inParamsPtr,inParmsSize);
		}

		virtual HRESULT STDMETHODCALLTYPE SendResult(HRESULT inResult)
		{
			return mConnection->SendResult(mPacketPtr->sequ_no,inResult);
		}

		//////////////////////////////////////////////////////////////////////////
		CRpcTcpConnectionRequestObj(RpcComPacket * inPacket,CRpcTCPConnection* inConn)
			:mPacketPtr(inPacket),mConnection(inConn)	
		{
			Assert(inPacket);
		}
        
		virtual ~CRpcTcpConnectionRequestObj(){}

	private:
		CComSmartPtr<RpcComPacket>		mPacketPtr;
		CComSmartPtr<CRpcTCPConnection>	mConnection;
	};
	/*!
	* 服务器通知对象接口
	* 由ClientRpc实现
	*/
	class RpcTcpConnectionNotifyObj:public IUnknownImp<ITgmRpcConnectionNotifyObject>
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		//! 获取请求/通知序号
		virtual ULONG STDMETHODCALLTYPE GetSequNo() 
		{return mPacketPtr->sequ_no;}

		//!  获取请求调用参数
		virtual const void* STDMETHODCALLTYPE GetParamsPtr()
		{ return mPacketPtr->data_buf;	}

		//! 获取请求调用参数大小
		virtual ULONG STDMETHODCALLTYPE GetParamsSize()
		{return mPacketPtr->data_size;}
		//////////////////////////////////////////////////////////////////////////
		RpcTcpConnectionNotifyObj(RpcComPacket *inPacket):mPacketPtr(inPacket)
		{

		}

		virtual ~RpcTcpConnectionNotifyObj()
		{
			//TRACE("~RpcTcpConnectionNotifyObj() %p,%X\r\n",this,GetRef());
		}

	private:
		CComSmartPtr<RpcComPacket> mPacketPtr;
	};

    static int __tcpconn_count__ = 0;
	CRpcTCPConnection::CRpcTCPConnection(BOOL isServer /* = FALSE */, BOOL isSimple /* = FALSE */)
		:Task(DEFAULT_TASK_THREAD_POOL_INS)
        , mIsSvrConn(FALSE)
		, mSocket(NULL, Socket::kBlockingSocketType)
		, mProtseq(&mSocket)
		, mIsConnected(FALSE)
		, mIsServer(isServer)
		, mIsSimple(isSimple)
	{
		char theTaskName[128];
		sprintf(theTaskName,"CRpcTCPConnection%p",this);
		SetTaskName(theTaskName);
        
        __tcpconn_count__++;
		if (!mIsSimple)
		{
			mSocket.SetTask(this);
		}		
	}

	CRpcTCPConnection::~CRpcTCPConnection()
	{
		TRACE("~CRpcTCPConnection() %p,%X\r\n",this,GetRef());
		Assert(0 == GetRef());
		Disconnect();

        __tcpconn_count__--;
	}

	HRESULT CRpcTCPConnection::Connect(ULONG inAddr,UInt16 inPort)
	{
		OS_Error theErr = mSocket.Open();
		HRESULT hr = map_oserror_to_hresult(theErr,E_FAIL);
		if(FAILED(hr)){
			return hr;
		}

		theErr = mSocket.Connect(inAddr,inPort);
		hr = map_oserror_to_hresult(theErr,E_FAIL);
		if(SUCCEEDED(hr))	
		{
			mSocket.RequestEvent(OSEventContext::EV_RE);
			mIsConnected = TRUE;
			if(mEvtHandler)
			{
				mEvtHandler->On_Connected();
			}
		} 
		else
		{			
			WARN("CRpcTCPClient::Connect Failed (%s)\n",strerror(theErr) );
			mSocket.Cleanup();
		}


		return hr;
	}


	/*! 断开连接 
	*  
	*  
	*/
	HRESULT CRpcTCPConnection::Disconnect()
	{
		if(mIsConnected)
		{
			mSocket.Cleanup();
			mIsConnected = FALSE;
			if(mEvtHandler)
			{
				mEvtHandler->On_Disconnected();
			}
		}

		if (!mIsSimple)
		{
			CancelAllRequest(RPCCOM_E_CONN_DISCONNECTED);//取消所有请求
		}
		
		return S_OK;
	}

	HRESULT	CRpcTCPConnection::DisconnectUnexpected()
	{
		if(mIsConnected)
		{
			mSocket.Cleanup();
			mIsConnected = FALSE;
			if(mEvtHandler)
			{
				mEvtHandler->On_DisconnectUnexpected();
			}
		}

		if (!mIsSimple)
		{
			CancelAllRequest(RPCCOM_E_CONN_DISCONNECTED);//取消所有请求
		}
		
		return S_OK;
	}

	/*!
	* 注册事件处理接口
	*
	*/
	HRESULT CRpcTCPConnection::Advise(ITgmRpcConnectionEvent * inEventHandler,ULONG inIsSvrConn)
	{
		TRACE("CRpcTCPConnection::Advise() mEvtHandler:%p,inEventHandler:%p,inIsSvrConn(%d)\n",
            (ITgmRpcConnectionEvent*)mEvtHandler,inEventHandler,inIsSvrConn);
		if(mEvtHandler)	
		{
			mEvtHandler->On_Disconnected();
            mIsSvrConn = FALSE;
		}
				
		mEvtHandler = inEventHandler;
		
		if(mEvtHandler)
		{
            if(inIsSvrConn) mIsSvrConn = TRUE;
			mEvtHandler->On_Connected();
		}

		DTRACE("call end\r\n");
		return S_OK;
	}

	/*!取消注册事件处理接口
	* 
	*
	*/
	HRESULT CRpcTCPConnection::Unadvise()
	{
		mEvtHandler = NULL;
		return S_OK;
	}


	HRESULT CRpcTCPConnection::WaitResult(RpcComPacket** ioPacket)
	{
		int nSelRet;
		fd_set fdsRead;
		struct timeval tv, *tvPtr = NULL;
		int nTimeOut = 3000;
		HRESULT hr = E_FAIL;

		if (nTimeOut > 0)
		{
			tv.tv_usec = nTimeOut * 1000;  //tv_usec单位微秒
			tv.tv_sec = 0;
			tvPtr = &tv;
		}			

		while (TRUE)
		{
			FD_ZERO(&fdsRead);
			FD_SET(mSocket.GetFileDesc(), &fdsRead);
			nSelRet = ::select(mSocket.GetFileDesc() + 1, &fdsRead, NULL, NULL, tvPtr);
			if (nSelRet == 0)
			{
				// Time out.
				return E_FAIL;
			}
			else if (nSelRet < 0)
			{
				int err = OSThread::GetErrno();

				// Select error
				return E_FAIL;
			}

			CComSmartPtr<RpcComPacket> thePacket;
			hr = mProtseq.method_receive(&thePacket);
			if (SUCCEEDED(hr))
			{
				*ioPacket = thePacket;
				(*ioPacket)->AddRef();
				return hr;
			}
			else
			{
				switch(hr)
				{
				case RPCCOM_E_CONN_DISCONNECTED:
				case RPCCOM_E_CONN_PIPE:
					DisconnectUnexpected();//断开所有链接
					WARN("RPCCOM_E_CONN_DISCONNECTED:0x%X\n",hr);
					break;
				case RPCCOM_E_SOCKET_AGAIN:
					mSocket.RequestEvent(OSEventContext::EV_RE);//继续去读数据
					break;
				default:
					//mSocket.RequestEvent(EV_RE);//继续去读数据
					ASSERT_M(0,("Failed:0x%X\n",hr));
					break;
				}
            }

		}

		return hr;
	}

	/*! 同步方法调用  
	*  当调用返回时，将直接填充各个输出参数。
	*  
	*/
	HRESULT CRpcTCPConnection::SendSyncRequest(const void *inParamsPtr,ULONG inParmsSize,
		void *outParamsPtr/* = NULL*/,ULONG outParamsSize/* */,ULONG *outParamsLenPtr/* = NULL*/,
		ULONG inTimeOut /*= RpcCOM_INFINITE_TIMEOUT*/)
	{
		ULONG theSeqNo = mIsServer?RpcComProtocol::gen_server_sequno():RpcComProtocol::gen_client_seqno();
		CComSmartPtr<RpcComPacket> thePacketPtr = NULL;
		HRESULT hr = E_FAIL;

		if (mIsSimple)
		{
			hr = mProtseq.method_request(theSeqNo,( UInt8* )inParamsPtr,inParmsSize);
			if(FAILED(hr))
			{
				if (RPCCOM_E_CONN_DISCONNECTED == hr)
				{
					DisconnectUnexpected();//断开所有链接
					WARN("RPCCOM_E_CONN_DISCONNECTED:0x%X\n",hr);
				}
				return(hr);
			}

			hr = WaitResult(&thePacketPtr);
		}
		else
		{
			CComSmartPtr<ClientSyncRequestObject> theReqPtr = new ClientSyncRequestObject(theSeqNo, inTimeOut);
			hr = AddRequest(theSeqNo,theReqPtr);
			if(FAILED(hr))	return(hr);

			hr = mProtseq.method_request(theSeqNo,( UInt8* )inParamsPtr,inParmsSize);
			if(FAILED(hr))
			{
				CancelRequest(theSeqNo);
				return(hr);
			}

			OSThread::ThreadYield();
			hr  = theReqPtr->WaitResult(&thePacketPtr);
		}

		if(SUCCEEDED(hr) && thePacketPtr && outParamsPtr)
		{
			ULONG len = min(outParamsSize,thePacketPtr->data_size);
			COND_EXEC(outParamsSize < thePacketPtr->data_size,
				WARN("out params buf is too small,outParamsSize(%lu) thePacketPtr->data_size(%lu)\n",
				(unsigned long)outParamsSize,(unsigned long)thePacketPtr->data_size));

			memcpy(outParamsPtr,thePacketPtr->data_buf, len);
			if(outParamsLenPtr) *outParamsLenPtr = len;
		}
		else
		{
			if(outParamsLenPtr) *outParamsLenPtr = 0;
		}

		return hr;
	}

	static void ipc_protocol_packet_memory_free_fun(void* inMem)
	{
		delete[] (UInt8*)inMem;
	}

	HRESULT CRpcTCPConnection::SendSyncRequestEx(const void *inParamsPtr,ULONG inParmsSize,
		void **outParamsPtr/* = NULL*/,ULONG *outParamsLenPtr/* = NULL*/,ipc_client_memory_free_fun_t * free_fun,
		ULONG inTimeOut /*= RpcCOM_INFINITE_TIMEOUT*/)
	{
		ULONG theSeqNo = mIsServer?RpcComProtocol::gen_server_sequno():RpcComProtocol::gen_client_seqno();
		CComSmartPtr<RpcComPacket> thePacketPtr = NULL;
		HRESULT hr = E_FAIL;

		if (mIsSimple)
		{
			hr = mProtseq.method_request(theSeqNo,( UInt8* )inParamsPtr,inParmsSize);
			if(FAILED(hr))	
			{
				if (RPCCOM_E_CONN_DISCONNECTED == hr)
				{
					DisconnectUnexpected();//断开所有链接
					WARN("RPCCOM_E_CONN_DISCONNECTED:0x%X\n",hr);
				}
				WARN("20 FAILED:0x%X\n",hr);
				return(hr);
			}

			hr = WaitResult(&thePacketPtr);
		}
		else
		{
			CComSmartPtr<ClientSyncRequestObject> theReqPtr = new ClientSyncRequestObject(theSeqNo, inTimeOut);
			hr = AddRequest(theSeqNo,theReqPtr);
			if(FAILED(hr))
			{
				WARN("10 FAILED:0x%X\n",hr);
				return(hr);
			}

			hr = mProtseq.method_request(theSeqNo,( UInt8* )inParamsPtr,inParmsSize);
			if(FAILED(hr))	
			{
				WARN("20 FAILED:0x%X\n",hr);
				CancelRequest(theSeqNo);
				return(hr);
			}

			OSThread::ThreadYield();
			hr  = theReqPtr->WaitResult(&thePacketPtr);
		}

		if(SUCCEEDED(hr))
		{
			*outParamsPtr		= thePacketPtr->data_buf;
			*outParamsLenPtr	= thePacketPtr->data_size;
			*free_fun			= ipc_protocol_packet_memory_free_fun;	
			thePacketPtr->data_buf = NULL;
			thePacketPtr->data_size = 0;
		}
		else
		{
			WARN("30 FAILED:0x%X\n",hr);
			return(hr);
		}

		return hr;
	}

	/*! 异步方法调用
	*  当请求发送成功或者失败，调用立刻返回。
	*  当服务器端执行完相应的请求，调用指定的回调接口
	*/
	HRESULT CRpcTCPConnection::SendAsyncRequest(const void *inParamsPtr,ULONG inParmsSize,
		ITgmRpcConnectionAsyncReqCallBack *inCallback /*= NULL*/,
		ULONG *outReqNo/* = NULL*/,
		ULONG inUserData/* */,
		ULONG inTimeOut/* = RpcCOM_INFINITE_TIMEOUT*/)
	{
		ULONG theSeqNo = mIsServer?RpcComProtocol::gen_server_sequno():RpcComProtocol::gen_client_seqno();
		HRESULT hr  = E_FAIL;

		if (mIsSimple)
		{
			// Simple mode not support Asynchronous operations.
			return hr;
		}

		if(inCallback)
		{
			ClientAsyncRequestObject * theReqPtr = new ClientAsyncRequestObject(inCallback, theSeqNo, inUserData, inTimeOut);
			hr = AddRequest(theSeqNo,theReqPtr);
		}

		hr = mProtseq.method_request(theSeqNo,( UInt8* )inParamsPtr,inParmsSize);
		if(FAILED(hr))	
		{
			WARN("10 FAILED:0x%X\n",hr);
			CancelRequest(theSeqNo);
			return(hr);
		}

		if(outReqNo)
			*outReqNo = theSeqNo;

		if(FAILED(hr))
		{
			WARN("20 FAILED:0x%X\n",hr);
			return(hr);
		}

		return hr;
	}

	/*! 通知
	*  
	*  
	*/
	HRESULT CRpcTCPConnection::SendNotify(const void *inParamsPtr,ULONG inParmsSize)
	{
		ULONG theSeqNo = mIsServer?RpcComProtocol::gen_server_sequno():RpcComProtocol::gen_client_seqno();
		HRESULT hr = mProtseq.method_notify(theSeqNo,( UInt8* )inParamsPtr,inParmsSize);

		if (mIsSimple)
		{
			if (RPCCOM_E_CONN_DISCONNECTED == hr)
			{
				DisconnectUnexpected();//断开所有链接
				WARN("RPCCOM_E_CONN_DISCONNECTED:0x%X\n",hr);
			}
		}
		
		return hr;
	}

	HRESULT CRpcTCPConnection::SendReply(ULONG inSequNo,const void *inParamsPtr,ULONG inParmsSize)
	{	
		HRESULT hr =  mProtseq.method_return(inSequNo,(UInt8 *)inParamsPtr,inParmsSize);	
		if (mIsSimple)
		{
			if (RPCCOM_E_CONN_DISCONNECTED == hr)
			{
				DisconnectUnexpected();//断开所有链接
				WARN("RPCCOM_E_CONN_DISCONNECTED:0x%X\n",hr);
			}
		}
		return hr;
	}

	HRESULT CRpcTCPConnection::SendResult(ULONG inSequNo,HRESULT inResult)
	{	
		HRESULT hr = mProtseq.method_return_result(inSequNo,(UInt8 *)&inResult,sizeof(HRESULT));	
		if (mIsSimple)
		{
			if (RPCCOM_E_CONN_DISCONNECTED == hr)
			{
				DisconnectUnexpected();//断开所有链接
				WARN("RPCCOM_E_CONN_DISCONNECTED:0x%X\n",hr);
			}
		}
		return hr;
	}


	HRESULT CRpcTCPConnection::GetResourceLocation(char* outBindString,ULONG inBingStringSize)
	{
		CComSmartPtr<RpcResourceLoactionIf> theRRL = new CRpcResourceLoaction();
		if(theRRL)
		{
			theRRL->set_ProtSequ((const char*)RPCBINGING_PROTSEQU_RPCCOM_TCP);
			theRRL->set_NetworkAddr((const char*)mSocket.GetLocalAddrStr()->GetAsCString());
			theRRL->set_Endpoint((const char*)mSocket.GetLocalPortStr()->GetAsCString());

			size_t theLen = strlen((const char*)theRRL->get_Binding())+1;
			if(inBingStringSize < theLen)
			{
				ERR("Failed E_INVALIDARG inBingStringSize(%lu),theLen(%lu)\n",(unsigned long)inBingStringSize,(unsigned long)theLen);
				return E_INVALIDARG;
			}
			strcpy((char*)outBindString,(const char*)theRRL->get_Binding());
			return S_OK;
		}

		return E_FAIL;
	}

	SInt64 CRpcTCPConnection::Run()
	{
		//TRACE(" start\n");
		EventFlags event = GetEvents();
		if (event & Task::kKillEvent)
		{
			TRACE("receviced kKillEvent\n");
			return -1;
		}

		if (event & Task::kReadEvent)
		{
			CComSmartPtr<RpcComPacket> thePacket;
			HRESULT hr = mProtseq.method_receive(&thePacket);

			if(SUCCEEDED(hr))	
			{
				assert(thePacket);
				switch(thePacket->type)
				{
				case RPCCOM_METHOD_REQUEST:
					if(mEvtHandler)
					{
						CComSmartPtr<ITgmRpcConnectionRequestObject> theReqPtr = new CRpcTcpConnectionRequestObj(thePacket,this);
						mEvtHandler->On_Request(theReqPtr);
					}
					break;

				case RPCCOM_METHOD_RETURN:
					{
						CComSmartPtr<ClientRequestObject> theReqPtr;
						hr = GetRequest(thePacket->sequ_no,&theReqPtr);
						if(FAILED(hr)) break;
						assert(theReqPtr);
						theReqPtr->SetResult(thePacket);
					}
					break;

				case RPCCOM_METHOD_RETURN_RESULT:
					{
						CComSmartPtr<ClientRequestObject> theReqPtr;
						hr = GetRequest(thePacket->sequ_no,&theReqPtr);
						if(FAILED(hr)) break;
						assert(theReqPtr);
						theReqPtr->SetResult(*((HRESULT*)thePacket->data_buf));
					}
					break;

				case RPCCOM_METHOD_NOTIFY:
					if(mEvtHandler)
					{
						CComSmartPtr<ITgmRpcConnectionNotifyObject> theNtyPtr =  new RpcTcpConnectionNotifyObj(thePacket);
						mEvtHandler->On_Notify(theNtyPtr);
					}
					break;
				default:
					{
						WARN("Unknown packet type:0x%X\n",thePacket->type);
					}
					break;
				}

				mSocket.RequestEvent(OSEventContext::EV_RE);//继续去读数据
			}
			else
			{
				switch(hr)
				{
				case RPCCOM_E_CONN_DISCONNECTED:
				case RPCCOM_E_CONN_PIPE:
					DisconnectUnexpected();//断开所有链接
					TRACE("RPCCOM_E_CONN_DISCONNECTED:0x%X\n",hr);
					break;
				case RPCCOM_E_SOCKET_AGAIN:
				case RPCCOM_E_CONN_NO_PACKET:
					mSocket.RequestEvent(OSEventContext::EV_RE);//继续去读数据
					break;
				default:
					//mSocket.RequestEvent(EV_RE);//继续去读数据
					ASSERT_M(0,("Failed:0x%X\n",hr));
					break;
				}
            }
		}

		
		//TRACE(" end\n");
		return 0;
	}


	HRESULT CRpcTCPConnection::AddRequest(ULONG inSeqNo,ClientRequestObject *inReqPtr)
	{
		OSMutexLocker _locker(&mMutex);
		if(!mSocket.IsConnected())
			return RPCCOM_E_CONN_DISCONNECTED;
		
		inReqPtr->AddRef();
		mRequestObjs.insert(std::make_pair(inSeqNo, inReqPtr));
		return S_OK;
	}

	HRESULT CRpcTCPConnection::GetRequest(ULONG inSeqNo,ClientRequestObject** outObj)
	{
		OSMutexLocker _locker(&mMutex);
		ReqObjIter_t iter = mRequestObjs.find(inSeqNo);

		if(mRequestObjs.end() == iter)	{
			//CLL_ASSERT(CLDBG_LIBID_RPCCOM, m_RequestObjs.end() != iter);
			ERR("Failed E_INVALIDARG inSeqNo(%lu)\n",(unsigned long)inSeqNo);
			return E_INVALIDARG;
		}

		ClientRequestObject* theReqPtr = iter->second;
		mRequestObjs.erase(iter);

		if(outObj)
		{
			*outObj = theReqPtr;
			(*outObj)->AddRef();
		}

		theReqPtr->Release();

		return S_OK;
	}

	void CRpcTCPConnection::CancelRequest(ULONG inSeqNo)
	{
		GetRequest(inSeqNo,NULL);
	}

	void	CRpcTCPConnection::CancelAllRequest(HRESULT inResult)
	{
		OSMutexLocker _locker(&mMutex);
		for(ReqObjIter_t iter = mRequestObjs.begin();iter != mRequestObjs.end(); iter++)
		{
			ClientRequestObject* theReqPtr = iter->second;
			theReqPtr->SetResult(inResult);
			theReqPtr->Release();
		}
		mRequestObjs.clear();	
	}

};//RpcComm

