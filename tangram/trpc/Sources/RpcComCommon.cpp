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

#include "RpcComCommon.h"
#include "OSMutex.h"
#include "Socket.h"
#include "SocketUtils.h"

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(agrrpc);

namespace nsTgmRpcCom{

	static const char *string_binding_find_delimiter(
		const char *string_binding, char delim)
	{
		const char *next;
		for (next = string_binding; *next; next++) {
			if (*next == '\\') {
				next++;
				continue;
			}
			if (*next == delim)
				return next;
		}
		return NULL;
	}

	static char* unescape_string_binding_component(
		const char *string_binding, int len ,char * dst)
	{
		char *p;
		char* component = dst;

		if(len ==-1)	len = strlen((const char*)string_binding);
		if(len == 0) 	return NULL;
		if(dst== NULL)	return NULL;

		for (p = component; len > 0; string_binding++, len--) {
			if (*string_binding == '\\') {
				string_binding++;
				len--;
				*p++ = *string_binding;
			} else {
				*p++ = *string_binding;
			}
		}
		*p = '\0';
		return component;
	}

	static char* escape_string_binding_component(char* string_binding,
		const char *component)
	{
		for (; *component; component++) {
			switch (*component) {
	  case '@':
	  case ':':
	  case '[':
	  case ']':
	  case '\\':
		  *string_binding++ = '\\';
		  *string_binding++ = *component;
		  break;
	  default:
		  *string_binding++ = *component;
		  break;
			}
		}
		return string_binding;
	}



	HRESULT CRpcResourceLoaction::set_Binding(const char* StringBinding)//format: protocol:addr:endpoint[:option]
	{
		const char *data, *next;
		static const char ep_opt[] = "endpoint=";
		BOOL endpoint_already_found = FALSE;

		reset();
		mDirty = true;

		data = (const char *)StringBinding;

		next = string_binding_find_delimiter(data, '@');
		if (next) {
			//not support uuid
			data = next+1;
		}

		next = string_binding_find_delimiter(data, ':');
		if (next) {
			unescape_string_binding_component(data, next - data,mProtseq);
			data = next+1;
		}

		next = string_binding_find_delimiter(data, '[');
		if (next) {
			const char *close;
			char opt[RPCBINGING_MAX_SIZE];

			unescape_string_binding_component(data, next - data,mNetworkAddr);
			data = next+1;
			close = string_binding_find_delimiter(data, ']');
			if (!close) goto fail;

			/* tokenize options */
			while (data < close) {
				next = string_binding_find_delimiter(data, ',');
				if (!next || next > close) next = close;
				/* FIXME: this is kind of inefficient */
				unescape_string_binding_component(data, next - data,opt);
				data = next+1;

				/* parse option */
				next = string_binding_find_delimiter(opt, '=');
				if (!next) {
					/* not an option, must be an endpoint */
					if (endpoint_already_found) goto fail;
					strcpy((char*)mEndpoint,(const char*)opt);
					endpoint_already_found = TRUE;
				} else {
					if (strncmp((const char *)opt, (const char*)ep_opt, strlen((const char*)ep_opt)) == 0) {
						/* endpoint option */
						if (endpoint_already_found) goto fail;
						unescape_string_binding_component(next+1, -1,mEndpoint);
						endpoint_already_found = TRUE;
					} else {
						strcpy((char*)mEndpoint,(const char*)opt);
					}
				}
			}

			data = close+1;
			if (*data) goto fail;
		}
		else
			unescape_string_binding_component(data, -1,mNetworkAddr);

		return S_OK;

fail:
		reset();

		ERR("Failed E_INVALIDARG StringBinding(%s)\n",StringBinding);
		return E_INVALIDARG;

	}

	const char* CRpcResourceLoaction::get_Binding()
	{
		if(mDirty)
		{
			mDirty = false;
			this->compose();
		}

		return mRRL[0]?mRRL:NULL;
	}

	HRESULT CRpcResourceLoaction::set_ProtSequ(const char* inProtSequ)
	{
		if(strlen((const char*)inProtSequ)+1 > OS_DIM(mProtseq)){
			ERR("Failed E_INVALIDARG inProtSequ(%s)\n",inProtSequ);
			return E_INVALIDARG;
		}
		strcpy((char*)mProtseq,(const char*)inProtSequ);
		mDirty = true;
		return S_OK;
	}

	HRESULT CRpcResourceLoaction::set_NetworkAddr(const char* inAddr)
	{
		if(strlen((const char*)inAddr)+1 > OS_DIM(mNetworkAddr)){
			ERR("Failed E_INVALIDARG inAddr(%s)\n",inAddr);
			return E_INVALIDARG;
		}
		strcpy((char*)mNetworkAddr,(const char*)inAddr);
		mDirty = true;
		return S_OK;
	}


	HRESULT CRpcResourceLoaction::set_Endpoint(const char* inEndpoint)
	{
		if(strlen((const char*)inEndpoint)+1 > OS_DIM(mEndpoint)){
			ERR("Failed E_INVALIDARG inEndpoint(%s)\n",inEndpoint);
			return E_INVALIDARG;
		}
		strcpy((char*)mEndpoint,(const char*)inEndpoint);
		mDirty = true;
		return S_OK;
	}

	HRESULT CRpcResourceLoaction::set_Option(const char* inOption)
	{
		if(strlen((const char*)inOption)+1 > OS_DIM(mOption))
			return E_INVALIDARG;
		strcpy((char*)mOption,(const char*)inOption);
		mDirty = true;
		return S_OK;
	}

	char* CRpcResourceLoaction::compose()
	{
		//DWORD len = 1;
		char* data;
		char* StringBinding;

		///* overestimate for each component for escaping of delimiters */
		////if (*ObjUuid) len += strlen((char*)ObjUuid) * 2 + 1;
		//if (mProtseq && *mProtseq) len += strlen((char*)mProtseq) * 2 + 1;
		//if (mNetworkAddr && *mNetworkAddr) len += strlen((char*)mNetworkAddr) * 2;
		//if (mEndpoint && *mEndpoint) len += strlen((char*)mEndpoint) * 2 + 2;
		//if (mOption && *mOption) len += strlen((char*)mOption) * 2 + 2;

		//data = (char*)malloc(len);
		mRRL[0] = 0;
		StringBinding = data = mRRL;

		/*if (ObjUuid && *ObjUuid) {
		data = escape_string_binding_component(data, ObjUuid);
		*data++ = '@';
		}*/
		if (mProtseq && *mProtseq) {
			data = escape_string_binding_component(data, mProtseq);
			*data++ = ':';
		}
		if (mNetworkAddr && *mNetworkAddr)
			data = escape_string_binding_component(data, mNetworkAddr);

		if ((mEndpoint && *mEndpoint) ||
			(mOption && *mOption)) {
				*data++ = '[';
				if (mEndpoint && *mEndpoint) {
					data = escape_string_binding_component(data, mEndpoint);
					if (mOption && *mOption) *data++ = ',';
				}
				if (mOption && *mOption) {
					data = escape_string_binding_component(data, mOption);
				}
				*data++ = ']';
			}
			*data = 0;
			return StringBinding;
	}


};




static int ipccom_env_ref_count = 0;
void RpcComEnviorment::Initialize()
{
	OSGlobalCriticalSection osgcs;
	if(ipccom_env_ref_count++ == 0)
	{
		OSThread::Initialize();
#ifdef __Win32__
		WSADATA wsaData;	
		WSAStartup( MAKEWORD(2, 2), &wsaData );
#endif  
		SocketUtils::Initialize();
		//创建并启动EventThread
		//Socket::Initialize();
		//Socket::StartThread();
	}
}

void RpcComEnviorment::Uninitialize()
{
	OSGlobalCriticalSection osgcs;
	if(--ipccom_env_ref_count == 0)
	{
//		Socket::StopThread();
//		Socket::Uninitialize();
	}

}

