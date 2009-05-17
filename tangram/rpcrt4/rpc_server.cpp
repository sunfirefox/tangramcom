/*
* RPC server API
*
* Copyright 2001 Ove K鍁en, TransGaming Technologies
* Copyright 2004 Filip Navara
* Copyright 2006-2008 Robert Shearman (for CodeWeavers)
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

#include "config.h"
#include "wine/port.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "windef.h"
#include "winbase.h"
#include "winerror.h"

#include "rpc.h"
#include "rpcndr.h"
#include "excpt.h"

#include "tgmlog.h"
#include "wine/exception.h"

#include "rpc_server.h"
//#include "rpc_assoc.h"
#include "rpc_message.h"
#include "rpc_defs.h"
#include "ncastatus.h"
#include "OSAtomic.h"
//#include "OSMutex.h"
//#include "OSCond.h"


TGMLOG_DEFAULT_DEBUG_CHANNEL(rpc);

static RpcServerInterface* RPCRT4_find_interface(UUID* object,
												 const RPC_SYNTAX_IDENTIFIER* if_id,
												 BOOL check_object);
static void RPCRT4_release_server_interface(RpcServerInterface *sif);

#ifdef __cplusplus
extern "C" {
#endif /* defined(__cplusplus) */
WINADVAPI HANDLE	WINAPI Agr_CreateEvent();
WINADVAPI void	WINAPI Agr_SetEvent(HANDLE handle);
WINADVAPI void	WINAPI Agr_WaitEvent(HANDLE handle);
WINADVAPI void	WINAPI Agr_CloseEvent(HANDLE handle);
#ifdef __cplusplus
} /* extern "C" */
#endif /* defined(__cplusplus) */

namespace nsrpcrt4{
void CRpcServerConnectionEvent::On_Connected()
{
	//TRACE("SCONN(%p): Connectted,num(%d)\r\n",this,client_connection_num);
}

void CRpcServerConnectionEvent::On_Disconnected()
{
	//TRACE("SCONN(%p): Disconnectted\r\n",this);
	//if(mBindingPtr->FromConn){
	//	RPCRT4_DestroyConnection(mBindingPtr->FromConn);
	//	mBindingPtr->FromConn = NULL;
	//}
}

void CRpcServerConnectionEvent::On_DisconnectUnexpected()
{
	TRACE("SCONN(%p): On_DisconnectUnexpected\r\n",this);
	if(mBindingPtr->FromConn){
		RPCRT4_DestroyConnection(mBindingPtr->FromConn);
		mBindingPtr->FromConn = NULL;
	}
}

void CRpcServerConnectionEvent::On_Notify(ITgmRpcConnectionNotifyObject *inNtyPtr)
{
	//TRACE("SCONN(%p): Notify SeqNo(%lu),Len(%lu),Data(%s)\r\n",
	//	this,inNtyPtr->GetSequNo(),inNtyPtr->GetParamsSize(),(const unsigned char*)inNtyPtr->GetParamsPtr());
}

void CRpcServerConnectionEvent::On_Request(ITgmRpcConnectionRequestObject *inReqPtr)
{
	CRpcUnpacker theUnPacker(inReqPtr->GetParamsPtr(),inReqPtr->GetParamsSize());
	RpcPktHdr * theHdr = theUnPacker.get_Header();
	RPC_MESSAGE theMsg;
	memset(&theMsg,0,sizeof(theMsg));
	switch(theHdr->common.ptype)
	{
	case PKT_BIND:
		process_bind_packet((RpcPktBindHdr *)theHdr,NULL,inReqPtr);
		break;
	case PKT_REQUEST:
		{
			CRpcPacker theMerger;
			theMsg.BufferLength = theUnPacker.get_BufferLength();
			theMsg.Buffer = HeapAlloc(GetProcessHeap(),0,theMsg.BufferLength);
			memcpy(theMsg.Buffer,theUnPacker.get_BufferPtr(),theUnPacker.get_BufferLength());
			theMsg.Handle = mBindingPtr;
			process_request_packet((RpcPktRequestHdr *)theHdr,&theMsg,inReqPtr,theMerger);
		}

		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
CRpcServerConnectionEvent::CRpcServerConnectionEvent(AppRpcServer &inServer)
:mServer(inServer),mBindingPtr(NULL)
{
	memset(&mActiveInterface,0,sizeof(mActiveInterface));
}

CRpcServerConnectionEvent::~CRpcServerConnectionEvent()
{
	if(mBindingPtr) HeapFree(GetProcessHeap(),0,mBindingPtr);
}


HRESULT  CRpcServerEvent::On_ClientConnection_Created(ITgmRpcConnection *inConn)
{
	if(inConn)
	{
		CComSmartPtr<CRpcServerConnectionEvent> theEvent = new CRpcServerConnectionEvent(mServer);
		inConn->Advise(theEvent,1);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT AppRpcServer::Start()
{
	HRESULT hr = S_OK;
	if(!mServerIf)
	{
		RPC_CSTR StringBinding = NULL;
		RpcStringBindingComposeA(NULL,(RPC_CSTR)mProtsequ,(RPC_CSTR)mNetworkAddr,
			(RPC_CSTR)mEndpoint,(RPC_CSTR)mNetworkOptions,&StringBinding);
		
		HRESULT hr = nsTgmRpcCom::CRpcComFactory::CreateRpcServer(&mServerIf,(const char *)StringBinding);
		if(FAILED(hr))
			TRACE("SCONN Create CreateIpcServer failed(%X)\r\n",hr);

		hr = mServerIf->Advise(new CRpcServerEvent(*this));
		if(FAILED(hr))
			TRACE("SCONN Create Advise failed(%X)\r\n",hr);


		hr = mServerIf->Listen();

		if(FAILED(hr))
		{
			TRACE("SCONN Create Listen failed(%X)\r\n",hr);
		}
		else if(mCallBack)
		{
			char bindingString[RPCBINGING_MAX_SIZE];
			mServerIf->GetResourceLocation(bindingString,RPCBINGING_MAX_SIZE);
			if(mCallBack)
                (mCallBack)(1,bindingString,mUserData);
		}

		TRACE("SCONN Create(%p)\r\n",this);

		
	}

	return hr;
}

void AppRpcServer::Stop()
{
	if(mServerIf)
	{
        if(mCallBack)
            (mCallBack)(0,0,mUserData);
		mServerIf->DisconnectAll();
		mServerIf->Close();
		//mServerIf->Unadvise();
		mServerIf->Unadvise();
		mServerIf = NULL;
	}

	TRACE("SCONN Destroy(%p)\r\n",this);
}

AppRpcServer::AppRpcServer(const char * inProtocol,const char* inAddr,const char * inEndPoint
						   ,rpc_server_status_changed_callback inCallBack,void *inUserData )
						   :mCallBack(inCallBack),mUserData(inUserData)
{
	mNetworkAddr[0] = 0;
	mEndpoint[0] = 0;
	mNetworkOptions[0] = 0;
	if(inProtocol) 	strcpy(mProtsequ,inProtocol);
	if(inAddr) 		strcpy(mNetworkAddr,inAddr);
	if(inEndPoint)	strcpy(mEndpoint,inEndPoint);
}

AppRpcServer::~AppRpcServer()
{
}



RPC_STATUS CRpcServerConnectionEvent::process_bind_packet(RpcPktBindHdr *hdr, RPC_MESSAGE *msg,ITgmRpcConnectionRequestObject *inReqPtr)
{
	RPC_STATUS status;
	RpcServerInterface* sif;
	RpcPktHdr *response = NULL;

	/* FIXME: do more checks! */
	if (hdr->max_tsize < RPC_MIN_PACKET_SIZE ||
		!UuidIsNil(&mActiveInterface.SyntaxGUID, &status)) {
			TRACE("packet size less than min size, or active interface syntax guid non-null\n");
			sif = NULL;
		} else {
			mBindingPtr = (RpcBinding*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(RpcBinding));
			mBindingPtr->server = true;
			sif = RPCRT4_find_interface(NULL, &hdr->abstract, FALSE);
		}

		if (sif == NULL) {
			TRACE("rejecting bind request on connection %p\n", this);
			/* Report failure to client. */
			response = RPCRT4_BuildBindNackHeader(NDR_LOCAL_DATA_REPRESENTATION,
				RPC_VER_MAJOR, RPC_VER_MINOR);
		} else {
			TRACE("accepting bind request on connection %p for %s\n", this,
				TGMLOG_STR_GUID(&hdr->abstract.SyntaxGUID));

			static ULONG s_assoc_gid = 0;
			ULONG assoc_gid = hdr->assoc_gid ? hdr->assoc_gid :os_atomic_add(&s_assoc_gid,1);//如果为0，那么是此客户的第一次Binding，所以分配新的ID
			char binding_string[RPCBINGING_MAX_SIZE];
			mServer.mServerIf->GetResourceLocation(binding_string,RPCBINGING_MAX_SIZE);

			/* accept. */
			response = RPCRT4_BuildBindAckHeader(NDR_LOCAL_DATA_REPRESENTATION,
				RPC_MAX_PACKET_SIZE,
				RPC_MAX_PACKET_SIZE,
				assoc_gid,
				//mServer.mEndpoint,must modify to current mEndpoint
				binding_string,
				RESULT_ACCEPT, REASON_NONE,
				&sif->If->TransferSyntax);

			/* save the interface for later use */
			mActiveInterface = hdr->abstract;
			RPCRT4_release_server_interface(sif);
		}

		if (response)
			status = inReqPtr->SendReply(response, response->common.frag_len);
		else
			status = ERROR_OUTOFMEMORY;
		RPCRT4_FreeHeader(response);

		return status;

}


RPC_STATUS CRpcServerConnectionEvent::process_request_packet(RpcPktRequestHdr *hdr, RPC_MESSAGE *msg,ITgmRpcConnectionRequestObject *inReqPtr,CRpcPacker& inPacket)
{
	RPC_STATUS status;
	RpcPktHdr *response = NULL;
	RpcServerInterface* sif;
	RPC_DISPATCH_FUNCTION func = NULL;
	BOOL exception;
	UUID *object_uuid;
	void *buf = msg->Buffer;
    FB_TRACE("> \n");

	if (hdr->common.flags & RPC_FLG_OBJECT_UUID) {
		object_uuid = (UUID*)(hdr + 1);
	} else {
		object_uuid = NULL;
	}

	sif = RPCRT4_find_interface(object_uuid, &mActiveInterface, TRUE);
	if (!sif) {
		WARN("interface %s no longer registered, returning fault packet\n", TGMLOG_STR_GUID(&mActiveInterface.SyntaxGUID));
		response = RPCRT4_BuildFaultHeader(NDR_LOCAL_DATA_REPRESENTATION,
			NCA_S_UNK_IF);

		inPacket.BuildPacket(response);
		inReqPtr->SendReply(inPacket.get_DataPtr(), inPacket.get_DataLen());
		RPCRT4_FreeHeader(response);
        FE_WARN("< RPCRT4_find_interface failed\n");
		return RPC_S_OK;
	}

	msg->RpcInterfaceInformation = sif->If;
	/* copy the endpoint vector from sif to msg so that midl-generated code will use it */
	msg->ManagerEpv = sif->MgrEpv;
	if (object_uuid != NULL) {
		RPCRT4_SetBindingObject((RpcBinding*)msg->Handle, object_uuid);
	}

	/* find dispatch function */
	msg->ProcNum = hdr->opnum;
	if (sif->Flags & RPC_IF_OLE) {
		/* native ole32 always gives us a dispatch table with a single entry
		* (I assume that's a wrapper for IRpcStubBuffer::Invoke) */
		func = *sif->If->DispatchTable->DispatchTable;
	} else {
		if (msg->ProcNum >= sif->If->DispatchTable->DispatchTableCount) {
			WARN("invalid procnum (%d/%d)\n", msg->ProcNum, sif->If->DispatchTable->DispatchTableCount);
			response = RPCRT4_BuildFaultHeader(NDR_LOCAL_DATA_REPRESENTATION,
				NCA_S_OP_RNG_ERROR);

			inPacket.BuildPacket(response);
			inReqPtr->SendReply(inPacket.get_DataPtr(), inPacket.get_DataLen());
			RPCRT4_FreeHeader(response);
		}
		else
			func = sif->If->DispatchTable->DispatchTable[msg->ProcNum];
	}

	/* put in the drep. FIXME: is this more universally applicable?
	perhaps we should move this outward... */
	msg->DataRepresentation =
		MAKELONG( MAKEWORD(hdr->common.drep[0], hdr->common.drep[1]),
		MAKEWORD(hdr->common.drep[2], hdr->common.drep[3]));

	exception = FALSE;

	/* dispatch */
	//RPCRT4_SetThreadCurrentCallHandle(msg->Handle);
//#if 0
//	__TRY 
//	{
//		if (func) func(msg);
//	}
//	__EXCEPT_ALL 
//	{
//		WARN("exception caught with code 0x%08x = %d\n", GetExceptionCode(), GetExceptionCode());
//		exception = TRUE;
//		if (GetExceptionCode() == STATUS_ACCESS_VIOLATION)
//			status = ERROR_NOACCESS;
//		else
//			status = GetExceptionCode();
//		response = RPCRT4_BuildFaultHeader(msg->DataRepresentation,
//			RPC2NCA_STATUS(status));
//	}
//	__ENDTRY
//#endif
		if (func) func(msg);
	
		//RPCRT4_SetThreadCurrentCallHandle(NULL);

		/* release any unmarshalled context handles */
		//while ((context_handle = RPCRT4_PopThreadContextHandle()) != NULL)
		//  RpcServerAssoc_ReleaseContextHandle(conn->server_binding->Assoc, context_handle, TRUE);

		if (!exception)
			response = RPCRT4_BuildResponseHeader(msg->DataRepresentation, msg->BufferLength);

	/* send response packet */
	if (response) {
		inPacket.BuildPacket(response, exception ? NULL : msg->Buffer,  exception ? 0 : msg->BufferLength);
		HRESULT hr =  inReqPtr->SendReply(inPacket.get_DataPtr(), inPacket.get_DataLen());
		inPacket.Reset();
		status = HRESULT_MAPTO_RPC_STATUS(hr);
		RPCRT4_FreeHeader(response);

	} else  {
		ERR("out of memory\n");
	}

	msg->RpcInterfaceInformation = NULL;
	RPCRT4_release_server_interface(sif);

	if (msg->Buffer == buf) buf = NULL;
	TRACE("freeing Buffer=%p\n", buf);
	I_RpcFree(buf);

    FE_TRACE("< \n");
	return status;

}

};//namespace 


static nsrpcrt4::AppRpcServer * sServerPtr = NULL;



#if 1


typedef struct _RpcObjTypeMap
{
	/* FIXME: a hash table would be better. */
	struct _RpcObjTypeMap *next;
	UUID Object;
	UUID Type;
} RpcObjTypeMap;

static RpcObjTypeMap *RpcObjTypeMaps;

/* list of type RpcServerProtseq */

static struct list server_interfaces = LIST_INIT(server_interfaces);

static CRITICAL_SECTION server_cs = { NULL, -1, 0, 0, 0, 0 };


static UUID uuid_nil;

static inline RpcObjTypeMap *LookupObjTypeMap(UUID *ObjUuid)
{
	RpcObjTypeMap *rslt = RpcObjTypeMaps;
	RPC_STATUS dummy;

	while (rslt) {
		if (! UuidCompare(ObjUuid, &rslt->Object, &dummy)) break;
		rslt = rslt->next;
	}

	return rslt;
}

static inline UUID *LookupObjType(UUID *ObjUuid)
{
	RpcObjTypeMap *map = LookupObjTypeMap(ObjUuid);
	if (map)
		return &map->Type;
	else
		return &uuid_nil;
}

static RpcServerInterface* RPCRT4_find_interface(UUID* object,
												 const RPC_SYNTAX_IDENTIFIER* if_id,
												 BOOL check_object)
{
	UUID* MgrType = NULL;
	RpcServerInterface* cif;
	RPC_STATUS status;

	if (check_object)
		MgrType = LookupObjType(object);
	EnterCriticalSection(&server_cs);
	LIST_FOR_EACH_ENTRY(cif, &server_interfaces, RpcServerInterface, entry) {
		if (!memcmp(if_id, &cif->If->InterfaceId, sizeof(RPC_SYNTAX_IDENTIFIER)) &&
			(check_object == FALSE || UuidEqual(MgrType, &cif->MgrTypeUuid, &status))) {
				InterlockedIncrement(&cif->CurrentCalls);
				break;
			}
	}
	LeaveCriticalSection(&server_cs);
	if (&cif->entry == &server_interfaces) cif = NULL;
	TRACE("returning %p for object %s, if_id { %d.%d %s }\n", cif,
		TGMLOG_STR_GUID(object), if_id->SyntaxVersion.MajorVersion,
		if_id->SyntaxVersion.MinorVersion, TGMLOG_STR_GUID(&if_id->SyntaxGUID));
	return cif;
}

static void RPCRT4_release_server_interface(RpcServerInterface *sif)
{
	if (!InterlockedDecrement(&sif->CurrentCalls) &&
		sif->Delete) {
			/* sif must have been removed from server_interfaces before  
			* CallsCompletedEvent is set */
			if (sif->CallsCompletedEvent)
				Agr_SetEvent(sif->CallsCompletedEvent);
			HeapFree(GetProcessHeap(), 0, sif);
		}
}


static CRITICAL_SECTION listen_cs = { NULL, -1, 0, 0, 0, 0 };
/* whether the server is currently listening */
static BOOL std_listen;
/* number of manual listeners (calls to RpcServerListen) */
static LONG manual_listen_count;
/* total listeners including auto listeners */
static LONG listen_count;

static RPC_STATUS RPCRT4_start_listen(BOOL auto_listen)
{
	RPC_STATUS status = RPC_S_ALREADY_LISTENING;
	FB_TRACE("> auto_listen:%d sServerPtr:%p\n",auto_listen,sServerPtr);
	EnterCriticalSection(&listen_cs);
	if (auto_listen || (manual_listen_count++ == 0))
	{
		status = RPC_S_OK;
		if (++listen_count == 1)
			std_listen = TRUE;
	}
	LeaveCriticalSection(&listen_cs);

	if (std_listen)
	{
		if(auto_listen){
			if(sServerPtr){
				sServerPtr->Start();
			}
			else{
				FE_ERR("sServerPtr Not existed\n");
				assert(0);
			}
		}
	}
    FE_TRACE("< \n");
	return status;
}

static void RPCRT4_stop_listen(BOOL auto_listen)
{
    FB_TRACE("> autolisten:%d \n",auto_listen);
	EnterCriticalSection(&listen_cs);
	if (auto_listen || (--manual_listen_count == 0))
	{
		if (listen_count != 0 && --listen_count == 0) {
            std_listen = FALSE;
			LeaveCriticalSection(&listen_cs);

			if(!std_listen)
				sServerPtr->Stop();
			return;
		}
		assert(listen_count >= 0);
	}
	LeaveCriticalSection(&listen_cs);
    FE_TRACE("< \n");
}





/***********************************************************************
*             RpcServerUseProtseqEpW (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcServerUseProtseqEpW( RPC_WSTR Protseq, UINT MaxCalls, RPC_WSTR Endpoint, LPVOID SecurityDescriptor )
{
	LPSTR EndpointA;LPSTR ProtseqA;

    FB_TRACE("> \n");
	EndpointA = RPCRT4_strdupWtoA(Endpoint);
	ProtseqA = RPCRT4_strdupWtoA(Protseq);
	
	sServerPtr =new nsrpcrt4::AppRpcServer(ProtseqA,NULL,(const char*)EndpointA);
    TRACE("Create AppRpcServer:%p,std_listen:%d",sServerPtr,std_listen);
	if (std_listen)
	{
		sServerPtr->Start();
	}

    FE_TRACE("< \n");
	return RPC_S_OK;
}

RPC_STATUS WINAPI RpcServerUseProtseqWithCallBackW( RPC_WSTR Protseq, unsigned int MaxCalls, RPC_WSTR Endpoint, void *SecurityDescriptor,
								 rpc_server_status_changed_callback callback,void * userdata )
{
	LPSTR EndpointA;LPSTR ProtseqA;
    
    FB_TRACE("> \n");
	EndpointA = RPCRT4_strdupWtoA(Endpoint);
	ProtseqA = RPCRT4_strdupWtoA(Protseq);

	sServerPtr = new nsrpcrt4::AppRpcServer(ProtseqA,NULL,(const char*)EndpointA,callback,userdata);
	TRACE("Create AppRpcServer:%p,std_listen:%d",sServerPtr,std_listen);
	if (std_listen)
	{
		sServerPtr->Start();
	}
    FE_TRACE("< \n");
	return RPC_S_OK;
}





/***********************************************************************
*             RpcServerRegisterIf (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcServerRegisterIf( RPC_IF_HANDLE IfSpec, UUID* MgrTypeUuid, RPC_MGR_EPV* MgrEpv )
{
	TRACE("(%p,%s,%p)\n", IfSpec, TGMLOG_STR_GUID(MgrTypeUuid), MgrEpv);
	return RpcServerRegisterIf2( IfSpec, MgrTypeUuid, MgrEpv, 0, RPC_C_LISTEN_MAX_CALLS_DEFAULT, (UINT)-1, NULL );
}

/***********************************************************************
*             RpcServerRegisterIfEx (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcServerRegisterIfEx( RPC_IF_HANDLE IfSpec, UUID* MgrTypeUuid, RPC_MGR_EPV* MgrEpv,
										UINT Flags, UINT MaxCalls, RPC_IF_CALLBACK_FN* IfCallbackFn )
{
	TRACE("(%p,%s,%p,%u,%u,%p)\n", IfSpec, TGMLOG_STR_GUID(MgrTypeUuid), MgrEpv, Flags, MaxCalls, IfCallbackFn);
	return RpcServerRegisterIf2( IfSpec, MgrTypeUuid, MgrEpv, Flags, MaxCalls, (UINT)-1, IfCallbackFn );
}

/***********************************************************************
*             RpcServerRegisterIf2 (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcServerRegisterIf2( RPC_IF_HANDLE IfSpec, UUID* MgrTypeUuid, RPC_MGR_EPV* MgrEpv,
									   UINT Flags, UINT MaxCalls, UINT MaxRpcSize, RPC_IF_CALLBACK_FN* IfCallbackFn )
{
	PRPC_SERVER_INTERFACE If = (PRPC_SERVER_INTERFACE)IfSpec;
	RpcServerInterface* sif;
	unsigned int i;

    FB_TRACE("> \n");
	TRACE("(%p,%s,%p,%u,%u,%u,%p)\n", IfSpec, TGMLOG_STR_GUID(MgrTypeUuid), MgrEpv, Flags, MaxCalls,
		MaxRpcSize, IfCallbackFn);
	TRACE(" interface id: %s %d.%d\n", TGMLOG_STR_GUID(&If->InterfaceId.SyntaxGUID),
		If->InterfaceId.SyntaxVersion.MajorVersion,
		If->InterfaceId.SyntaxVersion.MinorVersion);
	TRACE(" transfer syntax: %s %d.%d\n", TGMLOG_STR_GUID(&If->TransferSyntax.SyntaxGUID),
		If->TransferSyntax.SyntaxVersion.MajorVersion,
		If->TransferSyntax.SyntaxVersion.MinorVersion);
	TRACE(" dispatch table: %p\n", If->DispatchTable);
	if (If->DispatchTable) {
		TRACE("  dispatch table count: %d\n", If->DispatchTable->DispatchTableCount);
		for (i=0; i<If->DispatchTable->DispatchTableCount; i++) {
			TRACE("   entry %d: %p\n", i, If->DispatchTable->DispatchTable[i]);
		}
		TRACE("  reserved: %ld\n", If->DispatchTable->Reserved);
	}
	TRACE(" protseq endpoint count: %d\n", If->RpcProtseqEndpointCount);
	TRACE(" default manager epv: %p\n", If->DefaultManagerEpv);
	TRACE(" interpreter info: %p\n", If->InterpreterInfo);

	sif = (RpcServerInterface*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(RpcServerInterface));
	sif->If           = If;
	if (MgrTypeUuid) {
		sif->MgrTypeUuid = *MgrTypeUuid;
		sif->MgrEpv       = MgrEpv;
	} else {
		memset(&sif->MgrTypeUuid, 0, sizeof(UUID));
		sif->MgrEpv       = If->DefaultManagerEpv;
	}
	sif->Flags        = Flags;
	sif->MaxCalls     = MaxCalls;
	sif->MaxRpcSize   = MaxRpcSize;
	sif->IfCallbackFn = IfCallbackFn;

	EnterCriticalSection(&server_cs);
	list_add_head(&server_interfaces, &sif->entry);
	LeaveCriticalSection(&server_cs);

	if (sif->Flags & RPC_IF_AUTOLISTEN)
		RPCRT4_start_listen(TRUE);

    FE_TRACE("< \n");
	return RPC_S_OK;
}

/***********************************************************************
*             RpcServerUnregisterIf (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcServerUnregisterIf( RPC_IF_HANDLE IfSpec, UUID* MgrTypeUuid, UINT WaitForCallsToComplete )
{
	PRPC_SERVER_INTERFACE If = (PRPC_SERVER_INTERFACE)IfSpec;
	HANDLE event = NULL;
	BOOL found = FALSE;
	BOOL completed = TRUE;
	RpcServerInterface *cif;
	RPC_STATUS status;

	FB_TRACE("> (IfSpec == (RPC_IF_HANDLE)^%p (%s), MgrTypeUuid == %s, WaitForCallsToComplete == %u)\n",
		IfSpec, TGMLOG_STR_GUID(&If->InterfaceId.SyntaxGUID), TGMLOG_STR_GUID(MgrTypeUuid), WaitForCallsToComplete);

	EnterCriticalSection(&server_cs);
	LIST_FOR_EACH_ENTRY(cif, &server_interfaces, RpcServerInterface, entry) {
		if ((!IfSpec || !memcmp(&If->InterfaceId, &cif->If->InterfaceId, sizeof(RPC_SYNTAX_IDENTIFIER))) &&
			UuidEqual(MgrTypeUuid, &cif->MgrTypeUuid, &status)) {
				list_remove(&cif->entry);
				TRACE("unregistering cif %p\n", cif);
				if (cif->CurrentCalls) {
					completed = FALSE;
					cif->Delete = TRUE;
					if (WaitForCallsToComplete){
						//cif->CallsCompletedEvent = event = CreateEventW(NULL, FALSE, FALSE, NULL);
						cif->CallsCompletedEvent = event = Agr_CreateEvent();
					}
				}
                
                //add by txf
                if (cif->Flags & RPC_IF_AUTOLISTEN)
                    RPCRT4_stop_listen(TRUE);

				found = TRUE;
				break;
			}
	}
	LeaveCriticalSection(&server_cs);

	if (!found) {
		FE_ERR("< not found for object %s\n", TGMLOG_STR_GUID(MgrTypeUuid));
		return RPC_S_UNKNOWN_IF;
	}

	if (completed){
		HeapFree(GetProcessHeap(), 0, cif);
	}
	else if (event) {
		/* sif will be freed when the last call is completed, so be careful not to
		* touch that memory here as that could happen before we get here */
		//WaitForSingleObject(event, INFINITE);
		//CloseHandle(event);
		Agr_WaitEvent(event);
		Agr_CloseEvent(event);
	}


    FE_TRACE("< \n");
	return RPC_S_OK;
}

/***********************************************************************
*             RpcServerUnregisterIfEx (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcServerUnregisterIfEx( RPC_IF_HANDLE IfSpec, UUID* MgrTypeUuid, int RundownContextHandles )
{
	FIXME("(IfSpec == (RPC_IF_HANDLE)^%p, MgrTypeUuid == %s, RundownContextHandles == %d): stub\n",
		IfSpec, TGMLOG_STR_GUID(MgrTypeUuid), RundownContextHandles);

	return RPC_S_OK;
}

/***********************************************************************
*             RpcServerRegisterAuthInfoA (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcServerRegisterAuthInfoA( RPC_CSTR ServerPrincName, ULONG AuthnSvc, RPC_AUTH_KEY_RETRIEVAL_FN GetKeyFn,
											 LPVOID Arg )
{
	FIXME( "(%s,%u,%p,%p): stub\n", ServerPrincName, AuthnSvc, GetKeyFn, Arg );

	return RPC_S_UNKNOWN_AUTHN_SERVICE; /* We don't know any authentication services */
}

/***********************************************************************
*             RpcServerRegisterAuthInfoW (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcServerRegisterAuthInfoW( RPC_WSTR ServerPrincName, ULONG AuthnSvc, RPC_AUTH_KEY_RETRIEVAL_FN GetKeyFn,
											 LPVOID Arg )
{
	FIXME( "(%s,%u,%p,%p): stub\n", TGMLOG_STR_W( ServerPrincName ), AuthnSvc, GetKeyFn, Arg );

	return RPC_S_UNKNOWN_AUTHN_SERVICE; /* We don't know any authentication services */
}

/***********************************************************************
*             RpcServerListen (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcServerListen( UINT MinimumCallThreads, UINT MaxCalls, UINT DontWait )
{
	return RPC_S_OK;
}

#endif 