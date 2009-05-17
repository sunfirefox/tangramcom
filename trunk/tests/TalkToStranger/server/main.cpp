/*
* talk server
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

#include <windows.h>
#include <initguid.h>
#define EXESVC
#include "server.h"

int main(int argc,const char* agrv[])
{
	if(argc < 2) {
		printf("Usage: talk_server /AUTOMTION | /REGSERVER | /UNREGSERVER\n");
		return -1;
	}
	const char* szCmdParam = agrv[1];
	
	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr))
	{
		if(0 == u_strcasecmp(agrv[1],"/REGSERVER") || 0 == u_strcasecmp(agrv[1],"-REGSERVER"))
		{
			printf("/REGSERVER\n");
		}
		else if(0 == u_strcasecmp(agrv[1],"/UNREGSERVER") || 0 == u_strcasecmp(agrv[1],"-UNREGSERVER"))
		{
			printf("/UNREGSERVER\n");

		}else if(0 == u_strcasecmp(agrv[1],"/AUTOMATION") || 0 == u_strcasecmp(agrv[1],"-AUTOMATION") 
			|| 0 == u_strcasecmp(agrv[1],"/EMBEDDING") || 0 == u_strcasecmp(agrv[1],"-EMBEDDING") )
		{
			IUnknown *pUnk = NULL;
			DWORD dwReg = 0;
			static tgmtl::class_factory<CTalkManager> cf;
			hr = cf.QueryInterface(IID_IUnknown,(void **)&pUnk);
			if (SUCCEEDED(hr))
			{
				hr = CoRegisterClassObject(CLSID_TalkToStranger,pUnk,CLSCTX_LOCAL_SERVER,REGCLS_MULTIPLEUSE| REGCLS_SUSPENDED,&dwReg);
				if (SUCCEEDED(hr))
					hr = CoResumeClassObjects();

				if(SUCCEEDED(hr)){
					printf("=============================================\n");
					printf("Server is Ready!\n");

					ModuleAddRef();//this line ensure the server running forever

					ModuleServerListen();
				}

				CoRevokeClassObject(dwReg);
			}
			//pUnk->Release();
		}
		else
		{
#ifdef WIN32
			MessageBoxA(NULL,"UNKNOWN PARAMS","Test",MB_OK);
#endif
		}
		
		CoUninitialize();
	}
	return hr;
}

#include "tgmtl/tgmtl_exesrv_cpp.h"

