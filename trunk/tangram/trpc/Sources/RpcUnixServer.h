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

#ifndef _RPCUNIXSERVER_H_
#define _RPCUNIXSERVER_H_

#include "OSBaseTypes.h"
#include "OSAtomic.h"
#include "UnixListenerSocket.h"
#include "UnixSocket.h"
#include "Task.h"

#include "RpcComCommon.h"
#include "RpcUnixConnection.h"

#include <vector>
using namespace std;



namespace nsTgmRpcCom
{
	class CRpcUnixServer;
	class RSUnixServerListenerSocket:public UnixListenerSocket
	{
	public:
		RSUnixServerListenerSocket(CRpcUnixServer* inServer = NULL)
			: mServerPtr(inServer)
		{	}

		void set_Server(CRpcUnixServer* inServer){ mServerPtr = inServer; }

		virtual Task* GetSessionTask(UnixSocket** outSocket);

	private:
		CRpcUnixServer* mServerPtr;
	};	

	class CRpcUnixServer : public IUnknownImp<ITgmRpcServer>
	{
	public:
		CRpcUnixServer(const char* inAddr = NULL);
		virtual ~CRpcUnixServer();

		HRESULT set_Path(const char* inPath);
		HRESULT set_Addr(const char* inAddr);

		/*! ��ʼ�����ͻ�����
		*  
		*  @param[in] in_listenCh����ͨ��
		*/
		virtual HRESULT STDMETHODCALLTYPE Listen(/*const OLECHAR *in_listenCh*/);

		/*! ֹͣ����
		*  
		*  
		*/
		virtual HRESULT STDMETHODCALLTYPE Close();

		/*!
		* �ر����пͻ����Ӷ���
		*
		*/
		virtual HRESULT STDMETHODCALLTYPE DisconnectAll();

		/*!
		* ע���¼�����ӿ�
		*
		*/
		virtual HRESULT STDMETHODCALLTYPE Advise(ITgmRpcServerEvent * inEventHandler);

		/*!ȡ��ע���¼�����ӿ�
		* 
		*
		*/
		virtual HRESULT STDMETHODCALLTYPE Unadvise();

		/*!��ȡResourceLocation��Ϣ
		* 
		*
		*/
		virtual HRESULT STDMETHODCALLTYPE GetResourceLocation(char* outBindString,ULONG inBingStringSize);

	public:
		void RemoveClient(CRpcUnixConnection * inClient);
	private:
		void AddClient(CRpcUnixConnection * inClient);
		
		void GenerateSocketPath();

	private:
		char m_szPath[128];
		char m_szPathAddr[128];
		BOOL								mIsListening;
		RSUnixServerListenerSocket			m_ListenSocketServer;
		CComSmartPtr<ITgmRpcServerEvent>	mEvtHandler;
		OSMutexRW							mMutexRW;
		vector<CRpcUnixConnection *>		mClients;
		CComSmartPtr<RpcResourceLoactionIf> mRRL;

		friend class RSUnixServerListenerSocket;
	};

}//RpcComm


#endif	// End of _RPCUNIXSERVER_H_
