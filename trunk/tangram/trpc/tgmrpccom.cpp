/*
* Rpc com
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

#include "OSBaseTypes.h"
#include "tgmrpccom.h"
#include "SocketUtils.h"
#include "RpcTcpServer.h"
#include "RpcTcpClient.h"
#include "RpcUnixServer.h"
#include "RpcUnixClient.h"

#include <memory.h>
#include "tgmrpccom.h"

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(agrrpc);

namespace nsTgmRpcCom
{
	HRESULT CRpcComFactory::CreateRpcServer(ITgmRpcServer **ppOutIf, const char * bindString)
	{
		*ppOutIf = NULL;

		CRpcResourceLoaction theRRL;
		theRRL.set_Binding(bindString);
		if(0 == strcmp(theRRL.get_ProtSequ(),RPCBINGING_PROTSEQU_RPCCOM_TCP))
		{
			int		thePort = 0;
			ULONG	theAddr = 0;

			if(theRRL.get_Endpoint())
			{
				sscanf(theRRL.get_Endpoint(),"%d",&thePort);
			}

			if(theRRL.get_NetworkAddr())
			{
				theAddr = SocketUtils::ConvertStringToAddr(theRRL.get_NetworkAddr());
			}
			else
			{
				theAddr = SocketUtils::ConvertStringToAddr("127.0.0.1");//默认为本机通讯
			}

			*ppOutIf = new CRpcTCPServer(theAddr,thePort);
			TRACE("bindString(%s),%s,theAddr(%X),thePort(%d)\n",bindString,RPCBINGING_PROTSEQU_RPCCOM_TCP,(unsigned int)theAddr,thePort);

		}
		else if (0 == strcmp(theRRL.get_ProtSequ(),RPCBINGING_PROTSEQU_RPCCOM_UNIX))
		{
			if (theRRL.get_NetworkAddr())
			{
				*ppOutIf = new CRpcUnixServer(theRRL.get_NetworkAddr());//"/home/ipc_unix_socket"
				TRACE("bindString(%s),%s,path(%s)\n",bindString,RPCBINGING_PROTSEQU_RPCCOM_UNIX, theRRL.get_NetworkAddr());
			}
		}

		if(*ppOutIf)
		{
			(*ppOutIf)->AddRef();
			return S_OK;
		}

		return E_FAIL;
	}

	HRESULT CRpcComFactory::CreateRpcClient(ITgmRpcClient **ppOutIf, const char * bindString)
	{
		*ppOutIf = NULL;

		CRpcResourceLoaction theRRL;
		theRRL.set_Binding(bindString);
		if(0 == strcmp(theRRL.get_ProtSequ(),RPCBINGING_PROTSEQU_RPCCOM_TCP))
		{
			int		thePort = 0;
			ULONG	theAddr = 0;

			if(theRRL.get_Endpoint()){
				sscanf(theRRL.get_Endpoint(),"%d",&thePort);
			}

			if(theRRL.get_NetworkAddr()){
				theAddr = SocketUtils::ConvertStringToAddr(theRRL.get_NetworkAddr());
			}
			else{
				theAddr = SocketUtils::ConvertStringToAddr("127.0.0.1");//默认为本机通讯
			}

			*ppOutIf = new CRpcTCPClient(theAddr,thePort);
			TRACE("bindString(%s),%s,theAddr(%X),thePort(%d)\n",bindString,RPCBINGING_PROTSEQU_RPCCOM_TCP,(unsigned int)theAddr,thePort);
		}
		else if (0 == strcmp(theRRL.get_ProtSequ(),RPCBINGING_PROTSEQU_RPCCOM_UNIX))
		{
			if (theRRL.get_NetworkAddr())
			{
				*ppOutIf = new CRpcUnixClient(theRRL.get_NetworkAddr(), TRUE);//"/home/ipc_unix_socket"
				TRACE("bindString(%s),%s,Path(%s)\n",bindString,RPCBINGING_PROTSEQU_RPCCOM_UNIX, theRRL.get_NetworkAddr());
			}
		}

		if(*ppOutIf)
		{
			(*ppOutIf)->AddRef();
			return S_OK;
		}
		else
			return E_FAIL;
	}

};
