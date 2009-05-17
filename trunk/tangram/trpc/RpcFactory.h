/*
* Rpc class factory
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


#ifndef Rpc_FACTORY_H_TXF_5466
#define Rpc_FACTORY_H_TXF_5466
#include "RpcInterface.h"

//#include "winreg.h"
//#include "yacl/combook.h"
#include "tgmtl/tgmtl.h"
using namespace tgmtl;

namespace nsTgmRpcCom
{

	class CTgmRpcFactory:public cocalss_base<ITgmRpcFactory>
	{
	public:

		//!   
		/*!
		*  @param inParams "udp:63".MTAPIServerĿǰʹ��6688
		*/
		HRESULT STDMETHODCALLTYPE CreateRpcServer(ITgmRpcServer **ppOutIf, const char * bindString);

		//! ����Rpc�ͻ���  
		/*!
		*  @param inIsOnlySynReqSup �Ƿ�Ϊ�򻯱��ͻ���.�򻯰����֧��ͬ������,��֧���첽����,֪ͨ�ͷ���֪ͨ
		*/
		HRESULT STDMETHODCALLTYPE CreateRpcClient(ITgmRpcClient **ppOutIf, const char * bindString);

		//////////////////////////////////////////////////////////////////////////

		CTgmRpcFactory():cocalss_base<ITgmRpcFactory>(IID_ITgmRpcFactory){}

		virtual ~CTgmRpcFactory(){}
	};


};
#endif


