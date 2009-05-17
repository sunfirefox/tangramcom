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
#include "scm_daemon.h"
#include "tgmlog.h"

#define API_DISPATCH_BEGIN(CLASS_NAME,METHOD_NAME) \
	case SCM_GET_API_NO(API_INTERFACE_##CLASS_NAME,Params_##CLASS_NAME##_##METHOD_NAME::ProcNum):{\
		Params_##CLASS_NAME##_##METHOD_NAME::InParams &	theInParams = * (Params_##CLASS_NAME##_##METHOD_NAME::InParams*)inReqPtr->GetParamsPtr();\
		Params_##CLASS_NAME##_##METHOD_NAME::OutParams	theOutParams

#define API_DISPATCH_END(CLASS,METHOD) \
		inReqPtr->SendReply(&theOutParams,sizeof(theOutParams));\
	}return


static inline const char *dbgstr_guid( const GUID *id,char* buf)
{
	if (!id) return "(null)";
	sprintf(buf,"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
		id->Data1, id->Data2, id->Data3,
		id->Data4[0], id->Data4[1], id->Data4[2], id->Data4[3],
		id->Data4[4], id->Data4[5], id->Data4[6], id->Data4[7] );
	return buf;
}

//static inline const char *dbgstr_oxid( const OXID& oxid,char* buf)
//{
//	//if (!id) return "(null)";
//	sprintf(buf,"{%08x-%08x}",
//		(unsigned int)(oxid >> 8), (unsigned int)(oxid & 0xFFFFFFFF));
//	return buf;
//}

namespace nsSCMD{


	 void STDMETHODCALLTYPE CRpcServerConnectionEvent::On_Connected()
	{
		os_atomic_add(&client_connection_num,1);
		TRACE("SCM_DAEMON SCONN(%p): Connectted,num(%d)\r\n",this,client_connection_num);
		//TRACE("Call end\r\n");
	}

	 void STDMETHODCALLTYPE CRpcServerConnectionEvent::On_Disconnected()
	{
		os_atomic_sub(&client_connection_num,1);
		TRACE("SCM_DAEMON SCONN(%p): Disconnectted,num(%d)\r\n",this,client_connection_num);
	}

	 void STDMETHODCALLTYPE CRpcServerConnectionEvent::On_DisconnectUnexpected()
	{
		os_atomic_sub(&client_connection_num,1);
		TRACE("SCM_DAEMON SCONN(%p): On_DisconnectUnexpected,num(%d)\r\n",this,client_connection_num);
	}

	 void STDMETHODCALLTYPE CRpcServerConnectionEvent::On_Notify(ITgmRpcConnectionNotifyObject *inNtyPtr)
	{
		TRACE("SCM_DAEMON SCONN(%p): Notify SeqNo(%lu),Len(%lu)\n",/*,Data(%s)\r\n",*/
			this,(unsigned long)inNtyPtr->GetSequNo(),(unsigned long)inNtyPtr->GetParamsSize()/*,(const char*)inNtyPtr->GetParamsPtr()*/);
	}

void STDMETHODCALLTYPE CRpcServerConnectionEvent::On_Request(ITgmRpcConnectionRequestObject *inReqPtr)
{
	TRACE("SCONN(%p): Request SeqNo(%lu),Len(%lu)\n",//,Data(%s)\r\n",
		this,(unsigned long)inReqPtr->GetSequNo(),(unsigned long)inReqPtr->GetParamsSize()/*,(const char*)inReqPtr->GetParamsPtr()*/);
	TGMLOG_QUOTE(char sz_debug_buf[64]);

	ULONG apino = *(ULONG*)inReqPtr->GetParamsPtr();
	switch(apino)
	{
		API_DISPATCH_BEGIN(ISCMClsFactoryResloveService,Register);
		theOutParams.result = mServerPtr->mClsFactoryService.Register(theInParams.clsid,theInParams.length,theInParams.objref);
		TRACE("ClsFactoryService Register cls(%s),length(%d),result(%X),%s\r\n",
					dbgstr_guid(&theInParams.clsid,sz_debug_buf),theInParams.length,theOutParams.result,TGMLOG_STR_AN((const char*)theInParams.objref,theInParams.length));
		API_DISPATCH_END(ISCMClsFactoryResloveService,Register);
		
		API_DISPATCH_BEGIN(ISCMClsFactoryResloveService,Unregister);
		theOutParams.result = mServerPtr->mClsFactoryService.Unregister(theInParams.clsid);
		TRACE("ClsFactoryService Unregister cls(%s),result(%X)\r\n",
					dbgstr_guid(&theInParams.clsid,sz_debug_buf),theOutParams.result);
		API_DISPATCH_END(ISCMClsFactoryResloveService,Unregister);

		API_DISPATCH_BEGIN(ISCMClsFactoryResloveService,Query);
		theOutParams.result = mServerPtr->mClsFactoryService.Query(theInParams.clsid,OS_DIM(theOutParams.objref),theOutParams.objref,&theOutParams.length);
		TRACE("ClsFactoryService Query cls(%s),length(%d),result(%X),%s\r\n",
					dbgstr_guid(&theInParams.clsid,sz_debug_buf),theOutParams.length,theOutParams.result,TGMLOG_STR_AN((const char*)theOutParams.objref,theOutParams.length));
		API_DISPATCH_END(ISCMClsFactoryResloveService,Query);

		API_DISPATCH_BEGIN(ISCMApartMentResloveService,Register);
		theOutParams.result = mServerPtr->mApartMentService.Register(theInParams.oxid,theInParams.rrl);
		TRACE("ApartMentService Register oxid(%s),rrl(%s),result(%X)\r\n",
					 tgmlog_dbgstr_longlong(theInParams.oxid),theInParams.rrl,theOutParams.result);
		API_DISPATCH_END(ISCMApartMentResloveService,Register);

		API_DISPATCH_BEGIN(ISCMApartMentResloveService,Unregister);
		theOutParams.result = mServerPtr->mApartMentService.Unregister(theInParams.oxid);
		TRACE("ApartMentService Unregister oxid(%s),result(%X)\r\n",
			tgmlog_dbgstr_longlong(theInParams.oxid),theOutParams.result);
		API_DISPATCH_END(ISCMApartMentResloveService,Unregister);

		API_DISPATCH_BEGIN(ISCMApartMentResloveService,Query);
		theOutParams.rrl[0] = 0;
		theOutParams.result = mServerPtr->mApartMentService.Query(theInParams.oxid,OS_DIM(theOutParams.rrl),theOutParams.rrl);
		TRACE("ApartMentService Query oxid(%s),rrl(%s),result(%X)\r\n",
			tgmlog_dbgstr_longlong(theInParams.oxid),theOutParams.rrl,theOutParams.result);
		API_DISPATCH_END(ISCMApartMentResloveService,Query);
	default:
		{
			ULONG hr = E_NOTIMPL;
			WARN("unknown apino:%lu\n",(unsigned long)apino);
			inReqPtr->SendResult(hr);
		}
		break;
	}
}


HRESULT  STDMETHODCALLTYPE CRpcServerEvent::On_ClientConnection_Created(ITgmRpcConnection *inConn)
{
	TRACE("CRpcServerEvent this:%p,ITgmRpcServerEvent:%p,inConn:%p,mServerPtr:%p\n",this,(ITgmRpcServerEvent*)this,inConn,mServerPtr);
	if(inConn)
	{
		CComSmartPtr<CRpcServerConnectionEvent> theEvent = new CRpcServerConnectionEvent(mServerPtr);
		inConn->Advise(theEvent,1);
		return S_OK;
	}
	return E_INVALIDARG;
}

ScmServer::ScmServer()
{
	HRESULT hr = nsTgmRpcCom::CRpcComFactory::CreateRpcServer(&mServerIf,TANGRAMCOM_SCM_CONN_BINDING);

	if(SUCCEEDED(hr))	{
		CRpcServerEvent * theSvrEvt = new CRpcServerEvent(this);
		theSvrEvt->AddRef();
		hr = mServerIf->Advise(theSvrEvt);
		if(SUCCEEDED(hr))	{
			hr = mServerIf->Listen();
		}
		theSvrEvt->Release();
	}

	TRACE("ScmServer Construct Successed(%p,0x%X)\r\n",this,hr);
}

ScmServer::~ScmServer()
{
	TRACE("ScmServer Disconstruct(%p)!!!\r\n",this);
	mServerIf->DisconnectAll();
	mServerIf->Close();
	mServerIf->Unadvise();
	mServerIf->Unadvise();
}

};


#include "SigHandler.h"
int main()
{	
	RegisterEventHandlers();
	nsSCMD::ScmServer *theScm = new nsSCMD::ScmServer();
	

	while (!sExitFlag)
	{
		sExitCond.Wait(&sExitMutex);
	}

	delete theScm;
	return 0;

}

