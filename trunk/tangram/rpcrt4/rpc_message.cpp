/*
 * RPC messages
 *
 * Copyright 2001-2002 Ove Kåven, TransGaming Technologies
 * Copyright 2004 Filip Navara
 * Copyright 2006 CodeWeavers
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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "windef.h"
#include "winbase.h"
#include "winerror.h"
#include "winuser.h"

#include "rpc.h"
#include "rpcndr.h"
#include "rpcdcep.h"

#include "tgmlog.h"

#include "rpc_binding.h"
#include "rpc_defs.h"
#include "rpc_message.h"
#include "ncastatus.h"

#define  COM_NO_WINDOWS_H
#include "tgmrpc_if.h"
//using namespace nsTgmRpcCom;

TGMLOG_DEFAULT_DEBUG_CHANNEL(rpc);

#include "rpc_packet_merger.h"

/* note: the DCE/RPC spec says the alignment amount should be 4, but
 * MS/RPC servers seem to always use 16 */
#define AUTH_ALIGNMENT 16

/* gets the amount needed to round a value up to the specified alignment */
#define ROUND_UP_AMOUNT(value, alignment) \
    (((alignment) - (((value) % (alignment)))) % (alignment))
#define ROUND_UP(value, alignment) (((value) + ((alignment) - 1)) & ~((alignment)-1))

enum secure_packet_direction
{
  SECURE_PACKET_SEND,
  SECURE_PACKET_RECEIVE
};

//static RPC_STATUS I_RpcReAllocateBuffer(PRPC_MESSAGE pMsg);

static DWORD RPCRT4_GetHeaderSize(const RpcPktHdr *Header)
{
  static const DWORD header_sizes[] = {
    sizeof(Header->request), 0, sizeof(Header->response),
    sizeof(Header->fault), 0, 0, 0, 0, 0, 0, 0, sizeof(Header->bind),
    sizeof(Header->bind_ack), sizeof(Header->bind_nack),
    0, 0, 0, 0, 0
  };
  ULONG ret = 0;
  
  if (Header->common.ptype < sizeof(header_sizes) / sizeof(header_sizes[0])) {
    ret = header_sizes[Header->common.ptype];
    if (ret == 0)
      FIXME("unhandled packet type\n");
    if (Header->common.flags & RPC_FLG_OBJECT_UUID)
      ret += sizeof(UUID);
  } else {
    TRACE("invalid packet type\n");
  }

  return ret;
}

static int packet_has_body(const RpcPktHdr *Header)
{
    return (Header->common.ptype == PKT_FAULT) ||
           (Header->common.ptype == PKT_REQUEST) ||
           (Header->common.ptype == PKT_RESPONSE);
}

static int packet_has_auth_verifier(const RpcPktHdr *Header)
{
    return !(Header->common.ptype == PKT_BIND_NACK) &&
           !(Header->common.ptype == PKT_SHUTDOWN);
}

static VOID RPCRT4_BuildCommonHeader(RpcPktHdr *Header, unsigned char PacketType,
                              unsigned long DataRepresentation)
{
  Header->common.rpc_ver = RPC_VER_MAJOR;
  Header->common.rpc_ver_minor = RPC_VER_MINOR;
  Header->common.ptype = PacketType;
  Header->common.drep[0] = LOBYTE(LOWORD(DataRepresentation));
  Header->common.drep[1] = HIBYTE(LOWORD(DataRepresentation));
  Header->common.drep[2] = LOBYTE(HIWORD(DataRepresentation));
  Header->common.drep[3] = HIBYTE(HIWORD(DataRepresentation));
  Header->common.auth_len = 0;
  Header->common.call_id = 1;
  Header->common.flags = 0;
  /* Flags and fragment length are computed in RPCRT4_Send. */
}                              

static RpcPktHdr *RPCRT4_BuildRequestHeader(unsigned long DataRepresentation,
                                     unsigned long BufferLength,
                                     unsigned short ProcNum,
                                     UUID *ObjectUuid)
{
  RpcPktHdr *header;
  BOOL has_object;
  RPC_STATUS status;

  has_object = (ObjectUuid != NULL && !UuidIsNil(ObjectUuid, &status));
  header = (RpcPktHdr *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                     sizeof(header->request) + (has_object ? sizeof(UUID) : 0));
  if (header == NULL) {
    return NULL;
  }

  RPCRT4_BuildCommonHeader(header, PKT_REQUEST, DataRepresentation);
  header->common.frag_len = sizeof(header->request);
  header->request.alloc_hint = BufferLength;
  header->request.context_id = 0;
  header->request.opnum = ProcNum;
  if (has_object) {
    header->common.flags |= RPC_FLG_OBJECT_UUID;
    header->common.frag_len += sizeof(UUID);
    memcpy(&header->request + 1, ObjectUuid, sizeof(UUID));
  }

  return header;
}

RpcPktHdr *RPCRT4_BuildResponseHeader(unsigned long DataRepresentation,
                                      unsigned long BufferLength)
{
  RpcPktHdr *header;

  header = (RpcPktHdr *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(header->response));
  if (header == NULL) {
    return NULL;
  }

  RPCRT4_BuildCommonHeader(header, PKT_RESPONSE, DataRepresentation);
  header->common.frag_len = sizeof(header->response);
  header->response.alloc_hint = BufferLength;

  return header;
}

RpcPktHdr *RPCRT4_BuildFaultHeader(unsigned long DataRepresentation,
                                   RPC_STATUS Status)
{
  RpcPktHdr *header;

  header = (RpcPktHdr *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(header->fault));
  if (header == NULL) {
    return NULL;
  }

  RPCRT4_BuildCommonHeader(header, PKT_FAULT, DataRepresentation);
  header->common.frag_len = sizeof(header->fault);
  header->fault.status = Status;

  return header;
}

RpcPktHdr *RPCRT4_BuildBindHeader(unsigned long DataRepresentation,
                                  unsigned short MaxTransmissionSize,
                                  unsigned short MaxReceiveSize,
                                  unsigned long  AssocGroupId,
                                  const RPC_SYNTAX_IDENTIFIER *AbstractId,
                                  const RPC_SYNTAX_IDENTIFIER *TransferId)
{
  RpcPktHdr *header;

  header = (RpcPktHdr *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(header->bind));
  if (header == NULL) {
    return NULL;
  }

  RPCRT4_BuildCommonHeader(header, PKT_BIND, DataRepresentation);
  header->common.frag_len = sizeof(header->bind);
  header->bind.max_tsize = MaxTransmissionSize;
  header->bind.max_rsize = MaxReceiveSize;
  header->bind.assoc_gid = AssocGroupId;
  header->bind.num_elements = 1;
  header->bind.num_syntaxes = 1;
  header->bind.abstract = *AbstractId;
  header->bind.transfer = *TransferId;

  return header;
}

static RpcPktHdr *RPCRT4_BuildAuthHeader(unsigned long DataRepresentation)
{
  RpcPktHdr *header;

  header = (RpcPktHdr *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                     sizeof(header->common) + 12);
  if (header == NULL)
    return NULL;

  RPCRT4_BuildCommonHeader(header, PKT_AUTH3, DataRepresentation);
  header->common.frag_len = 0x14;
  header->common.auth_len = 0;

  return header;
}

RpcPktHdr *RPCRT4_BuildBindNackHeader(unsigned long DataRepresentation,
                                      unsigned char RpcVersion,
                                      unsigned char RpcVersionMinor)
{
  RpcPktHdr *header;

  header = (RpcPktHdr *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(header->bind_nack));
  if (header == NULL) {
    return NULL;
  }

  RPCRT4_BuildCommonHeader(header, PKT_BIND_NACK, DataRepresentation);
  header->common.frag_len = sizeof(header->bind_nack);
  header->bind_nack.reject_reason = REJECT_REASON_NOT_SPECIFIED;
  header->bind_nack.protocols_count = 1;
  header->bind_nack.protocols[0].rpc_ver = RpcVersion;
  header->bind_nack.protocols[0].rpc_ver_minor = RpcVersionMinor;

  return header;
}

RpcPktHdr *RPCRT4_BuildBindAckHeader(unsigned long DataRepresentation,
                                     unsigned short MaxTransmissionSize,
                                     unsigned short MaxReceiveSize,
                                     unsigned long AssocGroupId,
                                     LPCSTR ServerAddress,
                                     unsigned long Result,
                                     unsigned long Reason,
                                     const RPC_SYNTAX_IDENTIFIER *TransferId)
{
  RpcPktHdr *header;
  unsigned long header_size;
  RpcAddressString *server_address;
  RpcResults *results;
  RPC_SYNTAX_IDENTIFIER *transfer_id;

  header_size = sizeof(header->bind_ack) +
                ROUND_UP(FIELD_OFFSET(RpcAddressString, string[strlen(ServerAddress) + 1]), 4) +
                sizeof(RpcResults) +
                sizeof(RPC_SYNTAX_IDENTIFIER);

  header = (RpcPktHdr *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, header_size);
  if (header == NULL) {
    return NULL;
  }

  RPCRT4_BuildCommonHeader(header, PKT_BIND_ACK, DataRepresentation);
  header->common.frag_len = header_size;
  header->bind_ack.max_tsize = MaxTransmissionSize;
  header->bind_ack.max_rsize = MaxReceiveSize;
  header->bind_ack.assoc_gid = AssocGroupId;
  server_address = (RpcAddressString*)(&header->bind_ack + 1);
  server_address->length = strlen(ServerAddress) + 1;
  strcpy(server_address->string, ServerAddress);
  /* results is 4-byte aligned */
  results = (RpcResults*)((ULONG_PTR)server_address + ROUND_UP(FIELD_OFFSET(RpcAddressString, string[server_address->length]), 4));
  results->num_results = 1;
  results->results[0].result = Result;
  results->results[0].reason = Reason;
  transfer_id = (RPC_SYNTAX_IDENTIFIER*)(results + 1);
  *transfer_id = *TransferId;

  return header;
}

VOID RPCRT4_FreeHeader(RpcPktHdr *Header)
{
  HeapFree(GetProcessHeap(), 0, Header);
}

NCA_STATUS RPC2NCA_STATUS(RPC_STATUS status)
{
    switch (status)
    {
    case ERROR_INVALID_HANDLE:              return NCA_S_FAULT_CONTEXT_MISMATCH;
    case ERROR_OUTOFMEMORY:                 return NCA_S_FAULT_REMOTE_NO_MEMORY;
    case RPC_S_NOT_LISTENING:               return NCA_S_SERVER_TOO_BUSY;
    case RPC_S_UNKNOWN_IF:                  return NCA_S_UNK_IF;
    case RPC_S_SERVER_TOO_BUSY:             return NCA_S_SERVER_TOO_BUSY;
    case RPC_S_CALL_FAILED:                 return NCA_S_FAULT_UNSPEC;
    case RPC_S_CALL_FAILED_DNE:             return NCA_S_MANAGER_NOT_ENTERED;
    case RPC_S_PROTOCOL_ERROR:              return NCA_S_PROTO_ERROR;
    case RPC_S_UNSUPPORTED_TYPE:            return NCA_S_UNSUPPORTED_TYPE;
    case RPC_S_INVALID_TAG:                 return NCA_S_FAULT_INVALID_TAG;
    case RPC_S_INVALID_BOUND:               return NCA_S_FAULT_INVALID_BOUND;
    case RPC_S_PROCNUM_OUT_OF_RANGE:        return NCA_S_OP_RNG_ERROR;
    case RPC_X_SS_HANDLES_MISMATCH:         return NCA_S_FAULT_CONTEXT_MISMATCH;
    case RPC_S_CALL_CANCELLED:              return NCA_S_FAULT_CANCEL;
    case RPC_S_COMM_FAILURE:                return NCA_S_COMM_FAILURE;
    case RPC_X_WRONG_PIPE_ORDER:            return NCA_S_FAULT_PIPE_ORDER;
    case RPC_X_PIPE_CLOSED:                 return NCA_S_FAULT_PIPE_CLOSED;
    case RPC_X_PIPE_DISCIPLINE_ERROR:       return NCA_S_FAULT_PIPE_DISCIPLINE;
    case RPC_X_PIPE_EMPTY:                  return NCA_S_FAULT_PIPE_EMPTY;
    case STATUS_FLOAT_DIVIDE_BY_ZERO:       return NCA_S_FAULT_FP_DIV_ZERO;
    case STATUS_FLOAT_INVALID_OPERATION:    return NCA_S_FAULT_FP_ERROR;
    case STATUS_FLOAT_OVERFLOW:             return NCA_S_FAULT_FP_OVERFLOW;
    case STATUS_FLOAT_UNDERFLOW:            return NCA_S_FAULT_FP_UNDERFLOW;
    case STATUS_INTEGER_DIVIDE_BY_ZERO:     return NCA_S_FAULT_INT_DIV_BY_ZERO;
    case STATUS_INTEGER_OVERFLOW:           return NCA_S_FAULT_INT_OVERFLOW;
    default:                                return status;
    }
}

RPC_STATUS NCA2RPC_STATUS(NCA_STATUS status)
{
    switch (status)
    {
    case NCA_S_COMM_FAILURE:            return RPC_S_COMM_FAILURE;
    case NCA_S_OP_RNG_ERROR:            return RPC_S_PROCNUM_OUT_OF_RANGE;
    case NCA_S_UNK_IF:                  return RPC_S_UNKNOWN_IF;
    case NCA_S_YOU_CRASHED:             return RPC_S_CALL_FAILED;
    case NCA_S_PROTO_ERROR:             return RPC_S_PROTOCOL_ERROR;
    case NCA_S_OUT_ARGS_TOO_BIG:        return ERROR_NOT_ENOUGH_SERVER_MEMORY;
    case NCA_S_SERVER_TOO_BUSY:         return RPC_S_SERVER_TOO_BUSY;
    case NCA_S_UNSUPPORTED_TYPE:        return RPC_S_UNSUPPORTED_TYPE;
    case NCA_S_FAULT_INT_DIV_BY_ZERO:   return RPC_S_ZERO_DIVIDE;
    case NCA_S_FAULT_ADDR_ERROR:        return RPC_S_ADDRESS_ERROR;
    case NCA_S_FAULT_FP_DIV_ZERO:       return RPC_S_FP_DIV_ZERO;
    case NCA_S_FAULT_FP_UNDERFLOW:      return RPC_S_FP_UNDERFLOW;
    case NCA_S_FAULT_FP_OVERFLOW:       return RPC_S_FP_OVERFLOW;
    case NCA_S_FAULT_INVALID_TAG:       return RPC_S_INVALID_TAG;
    case NCA_S_FAULT_INVALID_BOUND:     return RPC_S_INVALID_BOUND;
    case NCA_S_RPC_VERSION_MISMATCH:    return RPC_S_PROTOCOL_ERROR;
    case NCA_S_UNSPEC_REJECT:           return RPC_S_CALL_FAILED_DNE;
    case NCA_S_BAD_ACTID:               return RPC_S_CALL_FAILED_DNE;
    case NCA_S_WHO_ARE_YOU_FAILED:      return RPC_S_CALL_FAILED;
    case NCA_S_MANAGER_NOT_ENTERED:     return RPC_S_CALL_FAILED_DNE;
    case NCA_S_FAULT_CANCEL:            return RPC_S_CALL_CANCELLED;
    case NCA_S_FAULT_ILL_INST:          return RPC_S_ADDRESS_ERROR;
    case NCA_S_FAULT_FP_ERROR:          return RPC_S_FP_OVERFLOW;
    case NCA_S_FAULT_INT_OVERFLOW:      return RPC_S_ADDRESS_ERROR;
    case NCA_S_FAULT_UNSPEC:            return RPC_S_CALL_FAILED;
    case NCA_S_FAULT_PIPE_EMPTY:        return RPC_X_PIPE_EMPTY;
    case NCA_S_FAULT_PIPE_CLOSED:       return RPC_X_PIPE_CLOSED;
    case NCA_S_FAULT_PIPE_ORDER:        return RPC_X_WRONG_PIPE_ORDER;
    case NCA_S_FAULT_PIPE_DISCIPLINE:   return RPC_X_PIPE_DISCIPLINE_ERROR;
    case NCA_S_FAULT_PIPE_COMM_ERROR:   return RPC_S_COMM_FAILURE;
    case NCA_S_FAULT_PIPE_MEMORY:       return ERROR_OUTOFMEMORY;
    case NCA_S_FAULT_CONTEXT_MISMATCH:  return ERROR_INVALID_HANDLE;
    case NCA_S_FAULT_REMOTE_NO_MEMORY:  return ERROR_NOT_ENOUGH_SERVER_MEMORY;
    default:                            return status;
    }
}


    




/* validates version and frag_len fields */
RPC_STATUS RPCRT4_ValidateCommonHeader(const RpcPktCommonHdr *hdr)
{
  DWORD hdr_length;

  /* verify if the header really makes sense */
  if (hdr->rpc_ver != RPC_VER_MAJOR ||
      hdr->rpc_ver_minor != RPC_VER_MINOR)
  {
    WARN("unhandled packet version\n");
    return RPC_S_PROTOCOL_ERROR;
  }

  hdr_length = RPCRT4_GetHeaderSize((const RpcPktHdr*)hdr);
  if (hdr_length == 0)
  {
    WARN("header length == 0\n");
    return RPC_S_PROTOCOL_ERROR;
  }

  if (hdr->frag_len < hdr_length)
  {
    WARN("bad frag length %d\n", hdr->frag_len);
    return RPC_S_PROTOCOL_ERROR;
  }

  return RPC_S_OK;
}

/***********************************************************************
 *           I_RpcNegotiateTransferSyntax [RPCRT4.@]
 *
 * Negotiates the transfer syntax used by a client connection by connecting
 * to the server.
 *
 * PARAMS
 *  pMsg   [I] RPC Message structure.
 *  pAsync [I] Asynchronous state to set.
 *
 * RETURNS
 *  Success: RPC_S_OK.
 *  Failure: Any error code.
 */
RPC_STATUS WINAPI I_RpcNegotiateTransferSyntax(PRPC_MESSAGE pMsg)//just service for client
{
  RpcBinding* bind = (RpcBinding*)pMsg->Handle;
  RpcConnection* conn;
  RPC_STATUS status = RPC_S_OK;

  FB_TRACE("> (%p)\n", pMsg);

  if (!bind || bind->server)
  {
    FE_ERR("< no binding\n");
    return RPC_S_INVALID_BINDING;
  }

  /* if we already have a connection, we don't need to negotiate again */
  if (!pMsg->ReservedForRuntime)
  {
    RPC_CLIENT_INTERFACE *cif = (RPC_CLIENT_INTERFACE *)pMsg->RpcInterfaceInformation;
    if (!cif) return RPC_S_INTERFACE_NOT_FOUND;

    if (!bind->Endpoint || !bind->Endpoint[0])
    {
      //TRACE("automatically resolving partially bound binding\n");
      //status = RpcEpResolveBinding(bind, cif);
      //if (status != RPC_S_OK) return status;
    }

    status = RPCRT4_OpenBinding(bind, &conn, &cif->TransferSyntax,&cif->InterfaceId);
	//status =  RpcAssoc_GetClientConnection(bind->Assoc, &cif->InterfaceId,
	//	&cif->TransferSyntax, NULL, NULL, &conn);

    if (status == RPC_S_OK)
    {
      pMsg->ReservedForRuntime = conn;
      RPCRT4_AddRefBinding(bind);
    }
  }

  FE_TRACE("< status:%lu\n",status);
  return status;
}

/***********************************************************************
 *           I_RpcGetBuffer [RPCRT4.@]
 *
 * Allocates a buffer for use by I_RpcSend or I_RpcSendReceive and binds to the
 * server interface.
 *
 * PARAMS
 *  pMsg [I/O] RPC message information.
 *
 * RETURNS
 *  Success: RPC_S_OK.
 *  Failure: RPC_S_INVALID_BINDING if pMsg->Handle is invalid.
 *           RPC_S_SERVER_UNAVAILABLE if unable to connect to server.
 *           ERROR_OUTOFMEMORY if buffer allocation failed.
 *
 * NOTES
 *  The pMsg->BufferLength field determines the size of the buffer to allocate,
 *  in bytes.
 *
 *  Use I_RpcFreeBuffer() to unbind from the server and free the message buffer.
 *
 * SEE ALSO
 *  I_RpcFreeBuffer(), I_RpcSend(), I_RpcReceive(), I_RpcSendReceive().
 */
RPC_STATUS WINAPI I_RpcGetBuffer(PRPC_MESSAGE pMsg)
{
  RPC_STATUS status;
  RpcBinding* bind = (RpcBinding*)pMsg->Handle;

  FB_TRACE("> (%p): BufferLength=%d\n", pMsg, pMsg->BufferLength);

  if (!bind)
  {
    FE_ERR("< no binding\n");
    return RPC_S_INVALID_BINDING;
  }

  pMsg->Buffer = I_RpcAllocate(pMsg->BufferLength);
  TRACE("Buffer=%p\n", pMsg->Buffer);

  if (!pMsg->Buffer)
    return ERROR_OUTOFMEMORY;

  if (!bind->server)
  {
    status = I_RpcNegotiateTransferSyntax(pMsg);
    if (status != RPC_S_OK)
      I_RpcFree(pMsg->Buffer);
  }
  else
    status = RPC_S_OK;

  FE_TRACE("< status:%lu\n",status);
  return status;
}

/***********************************************************************
 *           I_RpcReAllocateBuffer (internal)
 */
//static RPC_STATUS I_RpcReAllocateBuffer(PRPC_MESSAGE pMsg)
//{
//  TRACE("(%p): BufferLength=%d\n", pMsg, pMsg->BufferLength);
//  pMsg->Buffer = HeapReAlloc(GetProcessHeap(), 0, pMsg->Buffer, pMsg->BufferLength);
//
//  TRACE("Buffer=%p\n", pMsg->Buffer);
//  return pMsg->Buffer ? RPC_S_OK : ERROR_OUTOFMEMORY;
//}

/***********************************************************************
 *           I_RpcFreeBuffer [RPCRT4.@]
 *
 * Frees a buffer allocated by I_RpcGetBuffer or I_RpcReceive and unbinds from
 * the server interface.
 *
 * PARAMS
 *  pMsg [I/O] RPC message information.
 *
 * RETURNS
 *  RPC_S_OK.
 *
 * SEE ALSO
 *  I_RpcGetBuffer(), I_RpcReceive().
 */
RPC_STATUS WINAPI I_RpcFreeBuffer(PRPC_MESSAGE pMsg)
{
  RpcBinding* bind = (RpcBinding*)pMsg->Handle;

  FB_TRACE("> (%p) Buffer=%p\n", pMsg, pMsg->Buffer);

  if (!bind)
  {
    FE_ERR("< no binding\n");
    return RPC_S_INVALID_BINDING;
  }

  if (pMsg->ReservedForRuntime)
  {
    RpcConnection *conn = (RpcConnection *)pMsg->ReservedForRuntime;
    RPCRT4_CloseBinding(bind, conn);
    RPCRT4_ReleaseBinding(bind);
    pMsg->ReservedForRuntime = NULL;
  }
  I_RpcFree(pMsg->Buffer);

  FE_TRACE("< \n");
  return RPC_S_OK;
}



/* is this status something that the server can't recover from? */
static inline BOOL is_hard_error(RPC_STATUS status)
{
    switch (status)
    {
    case 0: /* user-defined fault */
    case ERROR_ACCESS_DENIED:
    case ERROR_INVALID_PARAMETER:
    case RPC_S_PROTOCOL_ERROR:
    case RPC_S_CALL_FAILED:
    case RPC_S_CALL_FAILED_DNE:
    case RPC_S_SEC_PKG_ERROR:
        return TRUE;
    default:
        return FALSE;
    }
}



/***********************************************************************
 *           I_RpcSendReceive [RPCRT4.@]
 *
 * Sends a message to the server and receives the response.
 *
 * PARAMS
 *  pMsg [I/O] RPC message information.
 *
 * RETURNS
 *  Success: RPC_S_OK.
 *  Failure: Any error code.
 *
 * NOTES
 *  The buffer must have been allocated with I_RpcGetBuffer().
 *
 * SEE ALSO
 *  I_RpcGetBuffer(), I_RpcSend(), I_RpcReceive().
 */
RPC_STATUS WINAPI I_RpcSendReceive(PRPC_MESSAGE pMsg)
{
	RpcBinding* bind = (RpcBinding*)pMsg->Handle;
	RpcConnection * conn = (RpcConnection *)pMsg->ReservedForRuntime;
	ITgmRpcConnection* theConn = conn->rpc_connection;
	void* res_buf = 0;
	ULONG res_len = 0;
	ipc_client_memory_free_fun_t res_free = NULL;


	RpcPktHdr *hdr = RPCRT4_BuildRequestHeader(pMsg->DataRepresentation,
		pMsg->BufferLength,
		pMsg->ProcNum & ~RPC_FLAGS_VALID_BIT,
		&bind->ObjectUuid);

	hdr->common.frag_len += pMsg->BufferLength;

	char* pkt =  (char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,hdr->common.frag_len+1);
	memcpy(pkt,hdr,hdr->common.frag_len-pMsg->BufferLength);
	memcpy(pkt+hdr->common.frag_len-pMsg->BufferLength,pMsg->Buffer,pMsg->BufferLength);


	HRESULT hr = theConn->SendSyncRequestEx(pkt,hdr->common.frag_len,&res_buf,&res_len,&res_free,0);
	HeapFree(GetProcessHeap(),0,pkt);
	HeapFree(GetProcessHeap(),0,hdr);
	HeapFree(GetProcessHeap(),0,pMsg->Buffer);

	if(SUCCEEDED(hr))
	{
		CRpcUnpacker theUnPacket(res_buf,res_len);
		pMsg->BufferLength = theUnPacket.get_BufferLength();
		pMsg->Buffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,pMsg->BufferLength);
		memcpy(pMsg->Buffer,theUnPacket.get_BufferPtr(),pMsg->BufferLength);
		res_free(res_buf);
	}else {
		RPCRT4_DestroyConnection(conn);
		pMsg->ReservedForRuntime = NULL;
	}

	return RPC_S_OK;
}


