/*
* Rpc Client Request
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

#include "RpcClientRequest.h"

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(agrrpc);

namespace nsTgmRpcCom
{  
	ClientRequestObject::~ClientRequestObject()
	{
		Assert(0 == GetRef());
		//TRACE("this:%p,ref:%X\r\n",(CSmartObject*)this,GetRef());
	}

	ClientSyncRequestObject::ClientSyncRequestObject(ULONG inSeqNo, ULONG inTimeOut)
		:ClientRequestObject(inSeqNo,inTimeOut),
		mPacket(NULL),mbResult(FALSE),mResult(RPCCOM_E_REQUEST_EXEC_TIMEOUT)
	{
		//TRACE(" this:%p ,inSeqNo:0x%X,inTimeOut:%lu\r\n",(CSmartObject*)this,(unsigned int)inSeqNo,(unsigned long)inTimeOut);
	}

	ClientSyncRequestObject::~ClientSyncRequestObject()
	{

	}

	HRESULT ClientSyncRequestObject::WaitResult(RpcComPacket** inPacket)
	{
		if(FALSE == mbResult){
			OSMutexLocker _Locker(&mMutex);
			if(FALSE == mbResult){
				mCond.Wait(&mMutex, mTimeOutInMSecs);
			}
		}
		
		if(mbResult && SUCCEEDED(mResult))	{
			if(mPacket)	{
				*inPacket = mPacket;
				(*inPacket)->AddRef();
			}
		}

		return mResult;
	}

	void ClientSyncRequestObject::SetResult(RpcComPacket* inPacket)
	{
		OSMutexLocker _locker(&mMutex);

		mPacket		= inPacket;
		mResult		= S_OK;
		mbResult	= TRUE;
		
		mCond.Signal();
	}

	void ClientSyncRequestObject::SetResult(HRESULT inResult)
	{
		OSMutexLocker _locker(&mMutex);
		
		mResult		= inResult;
		mbResult	= TRUE;
		mCond.Signal();
	}

	///////////////////ClientAsyncRequestObject
	ClientAsyncRequestObject::ClientAsyncRequestObject(ITgmRpcConnectionAsyncReqCallBack* inCallback, ULONG inSeqNo, ULONG inUserData, ULONG inTimeOut)
		:ClientRequestObject(inSeqNo, inTimeOut)
		,m_Callback(inCallback)
		,m_UserData(inUserData)
	{
		//char * dfa = new char[23];
	}

	ClientAsyncRequestObject::~ClientAsyncRequestObject()
	{

	}

	void ClientAsyncRequestObject::SetResult(RpcComPacket* inPacket)
	{
		if(m_Callback == NULL)
		{
			return;
		}

		//CLL_ASSERT(CLDBG_LIBID_RpcCOM, inResData);
		//CLL_ASSERT(CLDBG_LIBID_RpcCOM, inResDataLen);
		m_Callback->On_Reply(S_OK,inPacket->data_buf, inPacket->data_size, mSeqNo, m_UserData);
	}

	void ClientAsyncRequestObject::SetResult(HRESULT inResult)
	{
		m_Callback->On_Reply(inResult,NULL, 0, mSeqNo, m_UserData);
	}

};
