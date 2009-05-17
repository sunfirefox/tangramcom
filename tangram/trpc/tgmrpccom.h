/*
* rpc com
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

#ifndef AGRRPC_HHHH
#define AGRRPC_HHHH
//#include "OSBaseTypes.h"
#include "tgmrpc_if.h"

#ifdef WIN32
#if defined(ARGRPC_MAKEDLL)
#define AGRRPC_EXPORT  __declspec(dllexport)
#else 		/* use a DLL library */
#define AGRRPC_EXPORT  __declspec(dllimport)
#endif

#elif __linux__

#if defined(ARGRPC_MAKEDLL)
#define AGRRPC_EXPORT   __attribute__((visibility("default")))
#else
#define AGRRPC_EXPORT
#endif

#else
#error unsupportted_os
#endif

namespace nsTgmRpcCom
{
class AGRRPC_EXPORT CRpcComFactory
{
public:
	//!   
	/*!
	 * bindString  ProtocolSequence:NetworkAddress[Endpoint,Option]
	 */
	static HRESULT CreateRpcServer(ITgmRpcServer **ppOutIf,const char * bindString);
	
	//! 创建Rpc客户端  
	/*!
	 *  @param inIsOnlySynReqSup 是否为简化本客户端.简化版仅仅支持同步请求,不支持异步请求,通知和反向通知
	 */
	static HRESULT CreateRpcClient(ITgmRpcClient **ppOutIf,const char * bindString);
};

};

#endif

