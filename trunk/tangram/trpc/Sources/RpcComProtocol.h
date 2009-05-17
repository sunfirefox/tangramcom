/*
* Rpc Protocol
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

#ifndef RPCCOM_PROTOCOL_HH_45e6456
#define RPCCOM_PROTOCOL_HH_45e6456
#include "OSBaseTypes.h"
#include "RpcComCommon.h"
#include "OSMutex.h"
class Socket;

namespace nsTgmRpcCom
{  

	enum
	{
		RPCCOM_METHOD_INVALID = 0,
		RPCCOM_METHOD_REQUEST,
		RPCCOM_METHOD_RETURN,
		RPCCOM_METHOD_RETURN_RESULT,
		RPCCOM_METHOD_NOTIFY
	};


	class RpcComPacket:public CSmartObject
	{
	public:
		enum
		{
			RPCCOM_PACEKER_HEADER_SIZE = 16,
		};

#define RPCCOM_PACEKER_HEADER_PREFIX 0XFFFFFFFFUL

		ULONG	type;
		ULONG	sequ_no;
		ULONG	data_size;

		//ULONG	data_len;
		UInt8	*data_buf;

		RpcComPacket()
			: type(RPCCOM_METHOD_INVALID)
			, sequ_no(0)
			, data_size(0)
			, data_buf(NULL)
		{
		}

		~RpcComPacket()
		{
			if(data_buf)
				delete[] data_buf;
			data_buf = NULL;
		}
	};
	
	class RpcComProtocol
	{
	public:
		RpcComProtocol(Socket * inSocketPtr):mSocketPtr(inSocketPtr),mHeaderLen(0),mDataLen(0)
		{
		}

		~RpcComProtocol()
		{
			if(mPacketPtr) mPacketPtr = NULL;
		}

		HRESULT method_request(ULONG inSequNo,UInt8 * inData,	ULONG inLen)		
		{
			return method_invoke(RPCCOM_METHOD_REQUEST,inSequNo,inData,inLen);
		}

		HRESULT method_return(ULONG inSequNo,UInt8 * inData,	ULONG inLen)		
		{
			return method_invoke(RPCCOM_METHOD_RETURN,inSequNo,inData,inLen);
		}

		HRESULT method_return_result(ULONG inSequNo,UInt8 * inData,	ULONG inLen)	
		{
			return method_invoke(RPCCOM_METHOD_RETURN_RESULT,inSequNo,inData,inLen);
		}

		HRESULT method_notify(ULONG inSequNo,UInt8 * inData,	ULONG inLen)		
		{
			return method_invoke(RPCCOM_METHOD_NOTIFY,inSequNo,inData,inLen);
		}

		static ULONG	gen_server_sequno();
		static ULONG	gen_client_seqno();

		HRESULT method_receive(RpcComPacket ** outPacket);//

	private:
		HRESULT method_invoke(ULONG inType,ULONG inSequNo,UInt8 *inData,ULONG inLen);

	private:
		CComSmartPtr<RpcComPacket>	mPacketPtr;
		Socket*			mSocketPtr;

		UInt8			mHeaderBuf[RpcComPacket::RPCCOM_PACEKER_HEADER_SIZE];
		ULONG			mHeaderLen;
		ULONG			mDataLen;

		OSMutex			mMutex;
	};

};//namespace nsTgmRpcCom

#endif //#ifndef RPCCOM_PROTOCOL_HH

