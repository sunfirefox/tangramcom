/*
 * RPC transport layer
 *
 * Copyright 2001 Ove Kåven, TransGaming Technologies
 * Copyright 2003 Mike Hearn
 * Copyright 2004 Filip Navara
 * Copyright 2006 Mike McCormack
 * Copyright 2006 Damjan Jovanovic
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
 *
 */


#include "config.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>

#include "objbase.h"
#define  COM_NO_WINDOWS_H
#include "tgmrpccom.h"

using namespace nsTgmRpcCom;

#include "OSComUtils.h"
#include "tgmlog.h"

#include "rpc_binding.h"
#include "rpc_message.h"
//#include "rpc_server.h"
#include "epm_towers.h"


TGMLOG_DEFAULT_DEBUG_CHANNEL(rpc);

/**** interface to rest of code ****/

RPC_STATUS RPCRT4_OpenClientConnection(RpcConnection* Connection)
{
	FB_TRACE("> (Connection == ^%p)\n", Connection);
	assert(!Connection->server);
	
	RPC_CSTR StringBinding = NULL;
	RpcStringBindingComposeA(NULL,(RPC_CSTR)Connection->Protsequ,(RPC_CSTR)Connection->NetworkAddr,
		(RPC_CSTR)Connection->Endpoint,(RPC_CSTR)Connection->NetworkOptions,&StringBinding);
	
	ITgmRpcClient * theClient = NULL;
	HRESULT hr = CRpcComFactory::CreateRpcClient(&theClient,(const char*)StringBinding);
	hr =  theClient->Connect();
	Connection->rpc_client	= theClient;
	Connection->rpc_connection = theClient;
    FE_TRACE("< hr=0x%x \n",hr);
	return hr;

}

RPC_STATUS RPCRT4_CloseConnection(RpcConnection* Connection)
{
  FB_TRACE("> (Connection == ^%p)\n", Connection);

  if(Connection->rpc_connection)
  {
	  ((ITgmRpcConnection*)(Connection->rpc_connection))->Disconnect();
	  ((ITgmRpcConnection*)(Connection->rpc_connection))->Release();
	  Connection->rpc_connection = NULL;

  }
  FE_TRACE("< \n");
  return RPC_S_OK;
}

RPC_STATUS RPCRT4_CreateConnection(RpcConnection** Connection, BOOL server,
    LPCSTR Protseq, LPCSTR NetworkAddr, LPCSTR Endpoint,
    LPCWSTR NetworkOptions, RpcAuthInfo* AuthInfo, RpcQualityOfService *QOS)
{
  
  RpcConnection* NewConnection = (RpcConnection*)malloc(sizeof(RpcConnection));
  memset(NewConnection,0,sizeof(RpcConnection));
  NewConnection->server			= server;
  NewConnection->NetworkAddr	= RPCRT4_strdupA(NetworkAddr);
  NewConnection->Endpoint		= RPCRT4_strdupA(Endpoint);
  NewConnection->Protsequ		= RPCRT4_strdupA(Protseq);
    
  TRACE("connection: %p\n", NewConnection);
  *Connection = NewConnection;

  return RPC_S_OK;
}



RPC_STATUS RPCRT4_DestroyConnection(RpcConnection* Connection)
{
  FB_TRACE("> connection: %p\n", Connection);

  RPCRT4_CloseConnection(Connection);
  RPCRT4_strfree(Connection->Endpoint);
  RPCRT4_strfree(Connection->NetworkAddr);
 
  /* server-only */
  //if (Connection->server_binding) RPCRT4_ReleaseBinding(Connection->server_binding);

  free(Connection);
  FE_TRACE("<\n");
  return RPC_S_OK;
}



