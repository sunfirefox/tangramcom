/*
* Rpc common class
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

#ifndef RPCCOM_COMMON_HH
#define RPCCOM_COMMON_HH
#include "OSComUtils.h"
#include "RpcInterface.h"
#include "RpcComProtocol.h"
#include <errno.h>


class RpcComEnviorment
{
public:
	static void Initialize();
	static void Uninitialize();
};

class RpcResourceLoactionIf:public IUnknown
{
public:
	virtual HRESULT set_Binding(const char* inBinding) = 0;//format:RpcBindString protocol:addr[endpoint:option]
	virtual const char* get_Binding() = 0;

	virtual HRESULT set_ProtSequ(const char* inProtSequ) = 0;
	virtual const char* get_ProtSequ() = 0;

	virtual HRESULT set_NetworkAddr(const char* inAddr) = 0;
	virtual const char* get_NetworkAddr() = 0;

	virtual HRESULT set_Endpoint(const char* inEndpoint) = 0;
	virtual const char* get_Endpoint() = 0;

	virtual HRESULT set_Option(const char* inOption) = 0;
	virtual const char* get_Option() = 0;
};


namespace nsTgmRpcCom
{
	inline HRESULT map_oserror_to_hresult(OS_Error inError,HRESULT inDefHr)
	{
		if(inError == 0)
			return S_OK;

		switch(inError)
		{
#ifndef WIN32
        case ECONNRESET://对端已经关闭
            return RPCCOM_E_CONN_DISCONNECTED;
#endif
		case ENOTCONN://对端已经关闭
            return RPCCOM_E_CONN_DISCONNECTED;
        case EPIPE://EPIPE错误是：程序企图向已经关闭的读端写数据
            return RPCCOM_E_CONN_PIPE;
		case EADDRINUSE:
			return RPCCOM_E_CONN_ADDR_INUSE;
		case EINPROGRESS:
			return RPCCOM_E_SOCKET_INPROGRESS;
		case EADDRNOTAVAIL:
			return RPCCOM_E_CONN_ADDR_NOTAVAIL;
		case ENOBUFS:
			return E_OUTOFMEMORY;
		case EAGAIN:
			return RPCCOM_E_SOCKET_AGAIN;
		default:
			return inDefHr;
		}
	}

	class CRpcResourceLoaction:public IUnknownImp<RpcResourceLoactionIf>
	{
	public:
		CRpcResourceLoaction()
		{
			reset();
		}

		~CRpcResourceLoaction()
		{
			reset();
		}

		virtual HRESULT set_Binding(const char* inRRL);//format: protocol:addr:endpoint[:option]

		virtual const char* get_Binding();

		virtual HRESULT set_ProtSequ(const char* inProtSequ);

		virtual const char* get_ProtSequ()
		{	return mProtseq[0] ==0? NULL: mProtseq;}

		virtual HRESULT set_NetworkAddr(const char* inAddr);

		virtual const char* get_NetworkAddr()
		{	return mNetworkAddr[0] ==0? NULL: mNetworkAddr;}

		virtual HRESULT set_Endpoint(const char* inEndpoint);

		virtual const char* get_Endpoint()
		{	return mEndpoint[0] ==0? NULL: mEndpoint;}

		virtual HRESULT set_Option(const char* inOption);

		virtual const char* get_Option()
		{	return mOption[0] ==0? NULL: mOption;}

	private:
		void reset()
		{
			mProtseq[0] = 0;
			mNetworkAddr[0] = 0;
			mEndpoint[0] = 0;
			mOption[0] = 0;
			mRRL[0] = 0;
			mDirty = false;
		}

		char* compose();
	private:
		char mProtseq[64];
		char mNetworkAddr[64];
		char mEndpoint[128];
		char mOption[64];

		char mRRL[RPCBINGING_MAX_SIZE];
		bool mDirty;
	};
};





#endif//##ifndef RPCCOM_COMMON_HH

