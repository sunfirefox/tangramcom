/*
* scm daemon
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
#ifndef _SCM_DAEMON_HHH
#define _SCM_DAEMON_HHH



#include "tgmrpccom.h"
using namespace nsTgmRpcCom;

#include "scm_api.h"
#include "scm_protocol.h"

#include "scm_service.h"
#include "OSComUtils.h"

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(agrscm);

namespace nsSCMD{
	class ScmServer;

	static int client_connection_num = 0;
	class CRpcServerConnectionEvent:public IUnknownImp<ITgmRpcConnectionEvent>
	{
	public:
		virtual void STDMETHODCALLTYPE On_Connected();
		virtual void STDMETHODCALLTYPE On_Disconnected();
		virtual void STDMETHODCALLTYPE On_DisconnectUnexpected();
		virtual void STDMETHODCALLTYPE On_Notify(ITgmRpcConnectionNotifyObject *inNtyPtr);
		virtual void STDMETHODCALLTYPE On_Request(ITgmRpcConnectionRequestObject *inReqPtr);

		//////////////////////////////////////////////////////////////////////////
		CRpcServerConnectionEvent(ScmServer *inServerPtr)
			:mServerPtr(inServerPtr) {
				TRACE("--SCM Construct:%p,inServerPtr(%p),j0(%d),i0(%d)\n",this,mServerPtr,j0,i0);
		}

		virtual ~CRpcServerConnectionEvent() {
				TRACE("--SCM Deconstruct:%p,inServerPtr(%p)\n",this,mServerPtr);
		}

		
	private:
		int j0;
		ScmServer* mServerPtr;
		int i0;

	};

	class CRpcServerEvent:public IUnknownImp<ITgmRpcServerEvent>
	{
	public:
		/*!
		*　处理请求：创建客户连接对象。
		*　ppOutCliObj返回客户创建的新客户连接对象
		*/
		virtual HRESULT  STDMETHODCALLTYPE On_ClientConnection_Created(ITgmRpcConnection *inConn);
						 
		CRpcServerEvent(ScmServer *inServerPtr):mServerPtr(inServerPtr)	{
			TRACE("SCM Construct:%p,inServerPtr(%p)\n",this,mServerPtr);
		}

		virtual ~CRpcServerEvent(){
			TRACE("SCM Deconstruct:%p,inServerPtr(%p)\n",this,mServerPtr);
		}

	private:
		ScmServer * mServerPtr;

	};	


	class ScmServer
	{
	public:
		ScmServer();
		~ScmServer();

	private:
		CComSmartPtr<ITgmRpcServer>	mServerIf;

		ClsFactoryResloveService	mClsFactoryService;
		ApartMentResloveService		mApartMentService;

		friend class CRpcServerConnectionEvent;

	};

};
#endif

