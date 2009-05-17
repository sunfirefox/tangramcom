/*
* Rpc Server,Unix socket 
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
#include "RpcUnixServer.h"


#include <algorithm>

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(agrrpc);

namespace nsTgmRpcCom
{  
	class CRpcServerUnixConnection:public CRpcUnixConnection
	{
	public:
		CRpcServerUnixConnection(CRpcUnixServer* inServerPtr)
			: CRpcUnixConnection(TRUE)
			, mServerPtr(inServerPtr)
		{
			char theTaskName[128];
			sprintf(theTaskName,"CRpcServerUnixConnection%p",this);
			SetTaskName(theTaskName);

			TRACE(" nsTgmRpcCom::CRpcServerUnixConnection() this:%p\n",this);
			mIsConnected = TRUE;
		}

		virtual HRESULT STDMETHODCALLTYPE Disconnect()
		{
			HRESULT hr = CRpcUnixConnection::Disconnect();
			if(SUCCEEDED(hr))
			{
				mServerPtr->RemoveClient(this);
			}
			return hr;
		}

	protected:
		virtual HRESULT	DisconnectUnexpected()
		{
			HRESULT hr = CRpcUnixConnection::DisconnectUnexpected();
			if(SUCCEEDED(hr))
			{
				mServerPtr->RemoveClient(this);
			}
			return hr;
		}
	private:
		CRpcUnixServer* mServerPtr;
	};

	Task* RSUnixServerListenerSocket::GetSessionTask(UnixSocket** outSocket)
	{
		TRACE(">>outSocket:%p\n",outSocket);
		CComSmartPtr<CRpcServerUnixConnection> theConn = new CRpcServerUnixConnection(mServerPtr);
		
		ITgmRpcServerEvent* theEvt = (ITgmRpcServerEvent*)(mServerPtr->mEvtHandler);
		theEvt->On_ClientConnection_Created((CRpcServerUnixConnection*)theConn);
				
		*outSocket = &(theConn->get_Socket());
		mServerPtr->AddClient(theConn);
		
		TRACE("<<outSocket:%p,mServerPtr->mEvtHandler(%p),theConn(%p)\n",outSocket,theEvt,(CRpcServerUnixConnection*)theConn);
		return (Task*)((CRpcServerUnixConnection*)theConn);
	}

	CRpcUnixServer::CRpcUnixServer(const char* inAddr /* = NULL */)
		: mIsListening(FALSE)
	{
		memset(m_szPath, 0, sizeof(m_szPath));
		memset(m_szPathAddr, 0, sizeof(m_szPathAddr));

		if (inAddr)
		{
			strcpy(m_szPathAddr, inAddr);
		}
		
		RpcComEnviorment::Initialize();
		m_ListenSocketServer.set_Server(this);
	}

	CRpcUnixServer::~CRpcUnixServer()
	{
		Close();
		DisconnectAll();

		RpcComEnviorment::Uninitialize();
		TRACE("CRpcUnixServer::~CRpcUnixServer(%p)",this);
	}

	HRESULT CRpcUnixServer::set_Path(const char* inPath)
	{
		if(mIsListening)
			return RPCCOM_E_CONN_LISTENING;

		if (inPath)
		{
			strcpy(m_szPath, inPath);
		}		

		return S_OK;
	}

	HRESULT CRpcUnixServer::set_Addr(const char* inAddr)
	{
		if(mIsListening)
			return RPCCOM_E_CONN_LISTENING;

		if (inAddr)
		{
			strcpy(m_szPathAddr, inAddr);
		}		

		return S_OK;
	}

	void CRpcUnixServer::AddClient(CRpcUnixConnection * inClient)
	{
		OSMutexWriteLocker _locker(&mMutexRW);
		inClient->AddRef();
		mClients.push_back(inClient);
	}
	
	void CRpcUnixServer::RemoveClient(CRpcUnixConnection * inClient)
	{
		////关闭linkId指定的连接，删除相应的底层客户连接对象
		OSMutexWriteLocker _locker(&mMutexRW);
		vector<CRpcUnixConnection*>::iterator iter =  std::find(mClients.begin(),mClients.end(),inClient);
		//取消运行

		if(iter != mClients.end())
		{
			//(*iter)->Signal(Task::kKillEvent);
			(*iter)->Release();
			mClients.erase(iter);
		}
		else
		{
			WARN("CRpcUnixServer::RemoveClient find client (%p) failed\n",inClient);
		}
		
	}

	HRESULT CRpcUnixServer::Listen(/*const OLECHAR *in_listenCh*/)
	{		
		if (0 == strlen(m_szPathAddr))
		{
			GenerateSocketPath();
		}
		
		OS_Error theErr = m_ListenSocketServer.Initialize(this->m_szPathAddr);
		
		if(OS_NoErr != theErr)	
		{
			return RPCCOM_E_CONN_LISTEN_FAILED;
		}

		mIsListening = TRUE;
		m_ListenSocketServer.RequestEvent(OSEventContext::EV_RE);

		return S_OK;
	}

	/*!
	* 关闭所有Rpc客户连接对象
	*
	*/
	HRESULT CRpcUnixServer::DisconnectAll()
	{
		OSMutexWriteLocker _locker(&mMutexRW);

		vector<CRpcUnixConnection*>::iterator iter;
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
	HRESULT CRpcUnixServer::Close()
	{
		m_ListenSocketServer.Cleanup();
		mIsListening = FALSE;
		return S_OK;
	}

	/*!
	* 注册事件处理接口
	*
	*/
	HRESULT CRpcUnixServer::Advise(ITgmRpcServerEvent * inEventHandler)
	{
		TRACE(" CRpcUnixServer::Advise set inEventHandler(%p)\n",inEventHandler);
		mEvtHandler = inEventHandler;
		return S_OK;
	}

	/*!取消注册事件处理接口
	* 
	*
	*/
	HRESULT CRpcUnixServer::Unadvise()
	{
		TRACE("\n");
		mEvtHandler = NULL;
		return S_OK;
	}

	/*!获取Endpoint信息
	* 
	*
	*/
	HRESULT CRpcUnixServer::GetResourceLocation(char* outBindString,ULONG inBingStringSize)
	{
		if(!mRRL)
		{
			mRRL = new CRpcResourceLoaction();
			mRRL->set_ProtSequ((const char*)RPCBINGING_PROTSEQU_RPCCOM_UNIX);
			mRRL->set_NetworkAddr(m_ListenSocketServer.GetLocalPath());
		}
		
		if(mRRL)
		{
			size_t theLen = strlen((const char*)mRRL->get_Binding())+1;
			if(inBingStringSize < theLen)
			{
				ERR("Failed E_INVALIDARG inBingStringSize(%lu),theLen(%lu)\n",(unsigned long)inBingStringSize,(unsigned long)theLen);
				return E_INVALIDARG;
			}
			strcpy((char*)outBindString,(const char*)mRRL->get_Binding());
			return S_OK;
		}
		
		return E_FAIL;
	}

	static ULONG GetProcessID()
	{
#ifdef WIN32
		return GetCurrentProcessId();
#else
		return getpid();
#endif
	}

	void CRpcUnixServer::GenerateSocketPath()
	{
		char szAddr[128];
		sprintf(szAddr, "ipc_unix_socket_PID%8X_%8X", GetProcessID, rand());
		if (strlen(m_szPath))
		{
			strcpy(m_szPathAddr, m_szPath);

			if (m_szPath[strlen(m_szPath) - 1] != '/')
			{
				strcat(m_szPathAddr, "/");
			}			
		}
		
		strcat(m_szPathAddr, szAddr);
	}

};

