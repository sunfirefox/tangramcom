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

#ifndef _RPC_CLEINT_REQUEST_HHH
#define _RPC_CLEINT_REQUEST_HHH
#include "OSBaseTypes.h"
#include "OSMutex.h"
#include "OSCond.h"

#include "RpcComCommon.h"


namespace nsTgmRpcCom
{  

class ClientRequestObject:public CSmartObject
{
public:
	ClientRequestObject(ULONG inSeqNo,ULONG inTimeOut)
		: mSeqNo(inSeqNo),mTimeOutInMSecs(inTimeOut)
	{}

	virtual ~ClientRequestObject();

	virtual void SetResult(RpcComPacket* inPacket) = 0;

	virtual void SetResult(HRESULT inResult) = 0;//因为其他原因，调用失败

	ULONG		mSeqNo;
	ULONG		mTimeOutInMSecs;//超时
};


class ClientSyncRequestObject:public ClientRequestObject
{
public:
	ClientSyncRequestObject(ULONG inSeqNo,ULONG inTimeOut);
	virtual ~ClientSyncRequestObject();

	HRESULT WaitResult(RpcComPacket** inPacket);
	virtual void SetResult(RpcComPacket* inPacket);
	virtual void SetResult(HRESULT inResult);

private:

	CComSmartPtr<RpcComPacket> mPacket;
	BOOL	mbResult;
	HRESULT mResult;
	OSMutex mMutex;
	OSCond  mCond;
	
};


class ClientAsyncRequestObject:public ClientRequestObject
{
public:
	ClientAsyncRequestObject(ITgmRpcConnectionAsyncReqCallBack* inCallback,ULONG inSeqNo,ULONG inUserData,ULONG inTimeOut);
	virtual ~ClientAsyncRequestObject();

	virtual void SetResult(RpcComPacket* inPacket);
	virtual void SetResult(HRESULT inResult);

private:

	CComSmartPtr<ITgmRpcConnectionAsyncReqCallBack> m_Callback;
	ULONG m_UserData;
};


};

#endif //#ifndef _RPC_CLEINT_REQUEST_HHH
