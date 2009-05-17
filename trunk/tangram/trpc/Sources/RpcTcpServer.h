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

#ifndef RPC_SERVER_HH_TXF
#define RPC_SERVER_HH_TXF

#include "OSBaseTypes.h"
#include "OSAtomic.h"
#include "TCPListenerSocket.h"
#include "TCPSocket.h"
#include "Task.h"

#include "RpcComCommon.h"
#include "RpcTcpConnection.h"

#include <vector>
using namespace std;



namespace nsTgmRpcCom
{  

	class CRpcTCPServer;

	class RSTCPServerListenerSocket:public TCPListenerSocket
	{
	public:

		RSTCPServerListenerSocket(CRpcTCPServer* inServer = NULL):mServerPtr(inServer)
		{	}

		void	set_Server(CRpcTCPServer* inServer){ mServerPtr = inServer; }

		virtual Task*	GetSessionTask(TCPSocket** outSocket);

	private:

		CRpcTCPServer* mServerPtr;
	};

	

	class CRpcTCPServer: public IUnknownImp<ITgmRpcServer>
	{
	public:
		CRpcTCPServer(ULONG inAddr = 0,UInt16 inPort = 0);
		virtual ~CRpcTCPServer();

		HRESULT set_Addr(ULONG inAddr,UInt16 inPort);

		
		/*! 开始监听客户请求
		*  
		*  @param[in] in_listenCh下行通道
		*/
		virtual HRESULT STDMETHODCALLTYPE Listen(/*const OLECHAR *in_listenCh*/);

		/*! 停止监听
		*  
		*  
		*/
		virtual HRESULT STDMETHODCALLTYPE Close();

		/*!
		* 关闭所有客户连接对象
		*
		*/
		virtual HRESULT STDMETHODCALLTYPE DisconnectAll();



		/*!
		* 注册事件处理接口
		*
		*/
		virtual HRESULT STDMETHODCALLTYPE Advise(ITgmRpcServerEvent * inEventHandler);

		/*!取消注册事件处理接口
		* 
		*
		*/
		virtual HRESULT STDMETHODCALLTYPE Unadvise();



		/*!获取ResourceLocation信息
		* 
		*
		*/
		virtual HRESULT STDMETHODCALLTYPE GetResourceLocation(char* outBindString,ULONG inBingStringSize);

		

	public:
		void RemoveClient(CRpcTCPConnection * inClient);
	private:
		void AddClient(CRpcTCPConnection * inClient);
		

		friend class RSTCPServerListenerSocket;

	private:
		ULONG							mAddr;
		UInt16							mPort;

		BOOL							mIsListening;

		RSTCPServerListenerSocket		mListenSocket;
		CComSmartPtr<ITgmRpcServerEvent>	mEvtHandler;
		OSMutexRW						mMutexRW;

		vector<CRpcTCPConnection *>			mClients;

		CComSmartPtr<RpcResourceLoactionIf> mRRL;
	};

}//RpcComm


#endif//#ifndef RPC_SERVER_HH_TXF


