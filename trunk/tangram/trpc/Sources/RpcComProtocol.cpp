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

#include "OSBaseTypes.h"
#include "RpcComCommon.h"
#include "RpcComProtocol.h"
#include "OSMutex.h"
#include "OSAtomic.h"
#include "Socket.h"

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(agrrpc);

namespace nsTgmRpcCom
{
	HRESULT RpcComProtocol::method_invoke(ULONG inType,ULONG inSequNo,UInt8 *inData,ULONG inLen)
	{
		ULONG		sendlen1 = 0,sendlen2 = 0;;
		OS_Error	theErr	= OS_NoErr;
		HRESULT hr = E_FAIL;
		if(NULL == mSocketPtr)
		{
			return RPCCOM_E_CONN_INVALID;
		}
#define __SEND_ONE_MEM__
#ifndef __SEND_ONE_MEM__
		inLen = (inLen+3) & 0xFFFFFFFC;
		UInt8 theHeaderBuf[RpcComPacket::RPCCOM_PACEKER_HEADER_SIZE];
		((ULONG*)theHeaderBuf)[0] = RPCCOM_PACEKER_HEADER_PREFIX;
		((ULONG*)theHeaderBuf)[1] = inType;
		((ULONG*)theHeaderBuf)[2] = inSequNo;
		((ULONG*)theHeaderBuf)[3] = inLen;

		OSMutexLocker _locker(&mMutex);

		theErr	= mSocketPtr->Send((const char *)&theHeaderBuf,sizeof(theHeaderBuf),(UInt32*)&sendlen1);
		hr = map_oserror_to_hresult(theErr,RPCCOM_E_CONN_SEND_FAILED);
		if(FAILED(hr)) {
			ERR("Send header failed:0x%X\n",hr);
			return hr;
		}
		
		theErr	= mSocketPtr->Send((const char *)inData,inLen,(UInt32*)&sendlen2);
		hr = map_oserror_to_hresult(theErr,RPCCOM_E_CONN_SEND_FAILED);

		COND_EXEC(FAILED(hr),ERR("Send body failed:0x%X\n",hr));

		//TRACE("Send over:hr(0x%X),sendlen1(%lu),sendlen2(%lu)\r\n",hr,(unsigned long)sendlen1,(unsigned long)sendlen2);
#else
		OSMutexLocker _locker(&mMutex);
		UInt8* pBuf = new UInt8[RpcComPacket::RPCCOM_PACEKER_HEADER_SIZE + inLen];
		
		((ULONG*)pBuf)[0] = RPCCOM_PACEKER_HEADER_PREFIX;
		((ULONG*)pBuf)[1] = inType;
		((ULONG*)pBuf)[2] = inSequNo;
		((ULONG*)pBuf)[3] = inLen;

		memcpy((void*)&pBuf[RpcComPacket::RPCCOM_PACEKER_HEADER_SIZE], (void*)inData, inLen);
		
		theErr	= mSocketPtr->Send((const char *)pBuf, RpcComPacket::RPCCOM_PACEKER_HEADER_SIZE + inLen, (UInt32*)&sendlen1);
		hr = map_oserror_to_hresult(theErr,RPCCOM_E_CONN_SEND_FAILED);
		delete pBuf;
		if(FAILED(hr)) 
		{
			ERR("Send header failed:0x%X\n",hr);
			return hr;
		}
#endif

		return hr;
	}

	
	HRESULT RpcComProtocol::method_receive(RpcComPacket ** outPacket)
	{
        ULONG theLen = 0;
		OS_Error theErr;
		
		//不太可能重入
		
		if(!mPacketPtr)
		{
			mPacketPtr = new RpcComPacket();
		}

		if(mHeaderLen < RpcComPacket::RPCCOM_PACEKER_HEADER_SIZE)
		{
			theErr = mSocketPtr->Read(mHeaderBuf+mHeaderLen, RpcComPacket::RPCCOM_PACEKER_HEADER_SIZE - mHeaderLen,(UInt32*)&theLen);
			mHeaderLen += theLen;
			//DTRACE("method_receive : mHeaderLen(%d), theErr(%d)\r\n", mHeaderLen, theErr);

			HRESULT hr = map_oserror_to_hresult(theErr,E_FAIL);
			if(FAILED(hr) )
				return hr;
			if(theLen <= 0)
				return RPCCOM_E_SOCKET_AGAIN;

			if(RPCCOM_PACEKER_HEADER_PREFIX != ((ULONG*)mHeaderBuf)[0])
			{
				((ULONG*)mHeaderBuf)[0] = ((ULONG*)mHeaderBuf)[1];
				((ULONG*)mHeaderBuf)[1] = ((ULONG*)mHeaderBuf)[2];
				((ULONG*)mHeaderBuf)[2] = ((ULONG*)mHeaderBuf)[3];
				mHeaderLen -= sizeof(UInt32);
			}

			if(mHeaderLen < RpcComPacket::RPCCOM_PACEKER_HEADER_SIZE)
				return RPCCOM_E_CONN_NO_PACKET;
			
			mPacketPtr->type		= ((ULONG*)mHeaderBuf)[1];
			mPacketPtr->sequ_no		= ((ULONG*)mHeaderBuf)[2];
			mPacketPtr->data_size	= ((ULONG*)mHeaderBuf)[3];			

			if(mPacketPtr->data_size >0)
			{
				mPacketPtr->data_buf = new UInt8[mPacketPtr->data_size];
			}
		}

		if(mPacketPtr->data_size > mDataLen)
		{
			theLen = 0;
			theErr = mSocketPtr->Read(mPacketPtr->data_buf + mDataLen,mPacketPtr->data_size - mDataLen,(UInt32*)&theLen);
			mDataLen += theLen;

			HRESULT hr = map_oserror_to_hresult(theErr,E_FAIL);
			if(FAILED(hr) /*&& theLen <= 0*/)
				return hr;
			if(theLen <= 0)
				return RPCCOM_E_SOCKET_AGAIN;

			if(mPacketPtr->data_size > mDataLen)
				return RPCCOM_E_CONN_NO_PACKET;
		}

		*outPacket = mPacketPtr;
		(*outPacket)->AddRef();
		
		mPacketPtr = NULL;
		mHeaderLen = 0;
		mDataLen = 0;
		return S_OK;
	}

	ULONG	RpcComProtocol::gen_server_sequno()
	{
		static ULONG sequ = 0;
		return os_atomic_add(&sequ,2);
	}

	ULONG RpcComProtocol::gen_client_seqno()
	{
		static ULONG sequ = 1;
		return os_atomic_add(&sequ,2);
	}

};
