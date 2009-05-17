/*
* Rpc Server,TCP socket
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

#include "OSEventContext.h"
#include "RpcTcpServer.h"


#include <algorithm>

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(agrrpc);

namespace nsTgmRpcCom
{  
	class CRpcServerTCPConnection:public CRpcTCPConnection
	{
	public:
		CRpcServerTCPConnection(CRpcTCPServer* inServerPtr)
			: CRpcTCPConnection(true)
			, mServerPtr(inServerPtr)
		{
			char theTaskName[128];
			sprintf(theTaskName,"CRpcServerTCPConnection%p",this);
			SetTaskName(theTaskName);

			TRACE(" nsTgmRpcCom::CRpcServerTCPConnection() this:%p\n",this);
			mIsConnected = TRUE;
		}

		virtual HRESULT STDMETHODCALLTYPE Disconnect()
		{
			HRESULT hr = CRpcTCPConnection::Disconnect();
			if(SUCCEEDED(hr))
				mServerPtr->RemoveClient(this);
			return hr;
		}

	protected:
		virtual HRESULT	DisconnectUnexpected()
		{
			HRESULT hr = CRpcTCPConnection::DisconnectUnexpected();
			if(SUCCEEDED(hr))
				mServerPtr->RemoveClient(this);
			return hr;
		}
	private:
		CRpcTCPServer* mServerPtr;
	};


	Task* RSTCPServerListenerSocket::GetSessionTask(TCPSocket** outSocket)
	{
		TRACE(">>outSocket:%p\n",outSocket);
		CComSmartPtr<CRpcServerTCPConnection> theConn = new CRpcServerTCPConnection(mServerPtr);
		
		ITgmRpcServerEvent* theEvt = (ITgmRpcServerEvent*)(mServerPtr->mEvtHandler);
		theEvt->On_ClientConnection_Created((CRpcServerTCPConnection*)theConn);
				
		*outSocket = &(theConn->get_Socket());
		mServerPtr->AddClient(theConn);
		
		TRACE("<<outSocket:%p,mServerPtr->mEvtHandler(%p),theConn(%p)\n",outSocket,theEvt,(CRpcServerTCPConnection*)theConn);
		return (Task*)((CRpcServerTCPConnection*)theConn);
	}



	CRpcTCPServer::CRpcTCPServer(ULONG inAddr,UInt16 inPort):mAddr(inAddr),mPort(inPort),mIsListening(FALSE)
	{
		RpcComEnviorment::Initialize();
		mListenSocket.set_Server(this);
	}

	CRpcTCPServer::~CRpcTCPServer()
	{
		Close();
		DisconnectAll();

		RpcComEnviorment::Uninitialize();
		TRACE("CRpcTCPServer::~CRpcTCPServer(%p)",this);
	}

	HRESULT CRpcTCPServer::set_Addr(ULONG inAddr,UInt16 inPort)
	{
		if(mIsListening)
			return RPCCOM_E_CONN_LISTENING;

		mAddr = inAddr;
		mPort = inPort;

		return S_OK;
	}

	void CRpcTCPServer::AddClient(CRpcTCPConnection * inClient)
	{
		OSMutexWriteLocker _locker(&mMutexRW);
		inClient->AddRef();
		mClients.push_back(inClient);
	}
	
	void CRpcTCPServer::RemoveClient(CRpcTCPConnection * inClient)
	{
		////关闭linkId指定的连接，删除相应的底层客户连接对象
		OSMutexWriteLocker _locker(&mMutexRW);
		vector<CRpcTCPConnection*>::iterator iter =  std::find(mClients.begin(),mClients.end(),inClient);
		//取消运行

		if(iter != mClients.end())
		{
			//(*iter)->Signal(Task::kKillEvent);
			(*iter)->Release();
			mClients.erase(iter);
		}
		else
		{
			WARN("CRpcTCPServer::RemoveClient find client (%p) failed\n",inClient);
		}
		
	}

	HRESULT CRpcTCPServer::Listen(/*const OLECHAR *in_listenCh*/){
		
		OS_Error theErr = mListenSocket.Initialize(mAddr,mPort);
		
		if(OS_NoErr != theErr)	{
			return RPCCOM_E_CONN_LISTEN_FAILED;
		}

		mIsListening = TRUE;
		mListenSocket.RequestEvent(OSEventContext::EV_RE);

		return S_OK;
	}

	/*!
	* 关闭所有Rpc客户连接对象
	*
	*/
	HRESULT CRpcTCPServer::DisconnectAll()
	{
		OSMutexWriteLocker _locker(&mMutexRW);

		vector<CRpcTCPConnection*>::iterator iter;
		for(iter = mClients.begin();iter != mClients.end();iter++)
		{
			//(*iter)->Signal(Task::kKillEvent);
			(*iter)->Release();
		}
		mClients.clear();

		return S_OK;
	}

	/*! 关闭Rpc服务组件
	*  
	*  
	*/
	HRESULT CRpcTCPServer::Close(){
		mListenSocket.Cleanup();
		mIsListening = FALSE;
		return S_OK;
	}

	/*!
	* 注册事件处理接口
	*
	*/
	HRESULT CRpcTCPServer::Advise(ITgmRpcServerEvent * inEventHandler)
	{
		TRACE(" CRpcTCPServer::Advise set inEventHandler(%p)\n",inEventHandler);
		mEvtHandler = inEventHandler;
		return S_OK;
	}

	/*!取消注册事件处理接口
	* 
	*
	*/
	HRESULT CRpcTCPServer::Unadvise()
	{
		TRACE("\n");
		mEvtHandler = NULL;
		return S_OK;
	}

	/*!获取Endpoint信息
	* 
	*
	*/
	HRESULT CRpcTCPServer::GetResourceLocation(char* outBindString,ULONG inBingStringSize)
	{
		if(!mRRL)
		{
			mRRL = new CRpcResourceLoaction();
			mRRL->set_ProtSequ((const char*)RPCBINGING_PROTSEQU_RPCCOM_TCP);
			mRRL->set_NetworkAddr((const char*)mListenSocket.GetLocalAddrStr()->GetAsCString());
			mRRL->set_Endpoint((const char*)mListenSocket.GetLocalPortStr()->GetAsCString());
		}
		
		if(mRRL)
		{
			size_t theLen = strlen((const char*)mRRL->get_Binding())+1;
			if(inBingStringSize < theLen){
				ERR("Failed E_INVALIDARG inBingStringSize(%lu),theLen(%lu)\n",(unsigned long)inBingStringSize,(unsigned long)theLen);
				return E_INVALIDARG;
			}
			strcpy((char*)outBindString,(const char*)mRRL->get_Binding());
			return S_OK;
		}
		
		return E_FAIL;
	}

};

