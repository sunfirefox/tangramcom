/*
 * RPC server API
 *
 * Copyright 2001 Ove Kåven, TransGaming Technologies
 * Copyright 2009 Jokul for Tranzda
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

#ifndef __WINE_RPC_SERVER_H
#define __WINE_RPC_SERVER_H



#include "rpc_binding.h"
#include "wine/list.h"


typedef struct _RpcServerInterface
{
  struct list entry;
  RPC_SERVER_INTERFACE* If;
  UUID MgrTypeUuid;
  RPC_MGR_EPV* MgrEpv;
  UINT Flags;
  UINT MaxCalls;
  UINT MaxRpcSize;
  RPC_IF_CALLBACK_FN* IfCallbackFn;
  LONG CurrentCalls; /* number of calls currently executing */
  /* set when unregistering interface to let the caller of
   * RpcServerUnregisterIf* know that all calls have finished */
  HANDLE CallsCompletedEvent;
  BOOL Delete; /* delete when the last call finishes */
} RpcServerInterface;


#include "objbase.h"
#define  COM_NO_WINDOWS_H
#include "tgmrpccom.h"
#include "OSComUtils.h"
using namespace nsTgmRpcCom;

#include "rpc_defs.h"
#include "rpc_packet_merger.h"

namespace nsrpcrt4{
class CRpcServerEvent;
class AppRpcServer;


class CRpcServerConnectionEvent:public IUnknownImp<ITgmRpcConnectionEvent>
{
public:
	virtual void STDMETHODCALLTYPE On_Connected();

	virtual void STDMETHODCALLTYPE On_Disconnected();

	virtual void STDMETHODCALLTYPE On_DisconnectUnexpected();

	virtual void STDMETHODCALLTYPE On_Notify(ITgmRpcConnectionNotifyObject *inNtyPtr);

	virtual void STDMETHODCALLTYPE On_Request(ITgmRpcConnectionRequestObject *inReqPtr);

	//////////////////////////////////////////////////////////////////////////
	CRpcServerConnectionEvent(AppRpcServer &inServer);

	~CRpcServerConnectionEvent();
private:
	RPC_STATUS process_bind_packet(RpcPktBindHdr *hdr, RPC_MESSAGE *msg,ITgmRpcConnectionRequestObject *inReqPtr);
	RPC_STATUS process_request_packet(RpcPktRequestHdr *hdr, RPC_MESSAGE *msg,ITgmRpcConnectionRequestObject *inReqPtr,CRpcPacker& inPacket);

private:
	AppRpcServer & mServer;
	RPC_SYNTAX_IDENTIFIER	mActiveInterface;

	RpcBinding	*mBindingPtr;
};

class CRpcServerEvent:public IUnknownImp<ITgmRpcServerEvent>
{
public:
	virtual HRESULT  STDMETHODCALLTYPE On_ClientConnection_Created(ITgmRpcConnection *inConn);
	CRpcServerEvent(AppRpcServer &inServer)
		:mServer(inServer)
	{}
private:
	AppRpcServer & mServer;

};	

class AppRpcServer
{
public:
	HRESULT Start();
	void Stop();

	AppRpcServer(const char * inProtocol,const char* inAddr,const char * inEndPoint
		,rpc_server_status_changed_callback inCallBack= NULL,void *inUserData = NULL);
	~AppRpcServer();

	const char* get_EndPoint(){	return mEndpoint;}

private:
	CComSmartPtr<ITgmRpcServer>	mServerIf;
	 char						mProtsequ[64];
	 char						mNetworkAddr[64];
	 char						mEndpoint[64];
	 char						mNetworkOptions[64];

	rpc_server_status_changed_callback		mCallBack;
	void*									mUserData;
	friend class CRpcServerConnectionEvent;

};

};


#endif  /* __WINE_RPC_SERVER_H */
