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
#include "OSBaseTypes.h"
#include "tgmrpc_if.h"
#include "OSComUtils.h"


#include "scm_api.h"
#include "scm_protocol.h"

#include "tgmtl/tgmtl.h"
using namespace tgmtl;


class CScmClient
{
public:
	CScmClient();
	~CScmClient();
	ITgmRpcClient* GetClient();
};

#define  SCM_INCOKE_PARAM(CLASS_NAME,METHOD_NAME) \
	Params_##CLASS_NAME##_##METHOD_NAME::InParams	theInParams;\
	Params_##CLASS_NAME##_##METHOD_NAME::OutParams	theOutParams;\
	theInParams.apino =  SCM_GET_API_NO(API_INTERFACE_##CLASS_NAME,Params_##CLASS_NAME##_##METHOD_NAME::ProcNum)

class Proxy_ISCMClsFactoryResloveService:public cocalss_base<ISCMClsFactoryResloveService>
{
public:

	//////////////////////////////////////////////////////////////////////////

	virtual HRESULT STDMETHODCALLTYPE Register(REFIID in_clsid,int in_size,const unsigned char* in_data);
	virtual HRESULT STDMETHODCALLTYPE Unregister(REFIID in_clsid);
	virtual HRESULT STDMETHODCALLTYPE Query(REFIID in_clsid,int in_size,unsigned char* out_data,int *out_length);

	//////////////////////////////////////////////////////////////////////////
	Proxy_ISCMClsFactoryResloveService():cocalss_base<ISCMClsFactoryResloveService>(IID_ISCMClsFactoryResloveService){

	}
private:
	CScmClient mClient;
};

class Proxy_ISCMApartMentResloveService:public cocalss_base<ISCMApartMentResloveService>
{
public:
	//////////////////////////////////////////////////////////////////////////
	virtual HRESULT STDMETHODCALLTYPE Register(OXID in_oxid,const char* in_binding)	;
	virtual HRESULT STDMETHODCALLTYPE Unregister(OXID in_oxid);
	virtual HRESULT STDMETHODCALLTYPE Query(OXID in_oxid,int in_binding_size,char * out_binding);

	//////////////////////////////////////////////////////////////////////////
	Proxy_ISCMApartMentResloveService():cocalss_base<ISCMApartMentResloveService>(IID_ISCMApartMentResloveService){

	}
private:
	CScmClient mClient;
};



