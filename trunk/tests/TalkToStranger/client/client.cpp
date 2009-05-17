/*
* talk  client
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
#include <stdio.h>
#include <stdlib.h>
#include <initguid.h>
#include "objbase.h"
#include "../talk.h"


static const char* _wstr2astr(char * dst, const unsigned short * src)
{
	char *s = dst;
	if(dst == NULL || src == NULL)
		return NULL;
	while ((*dst = *src)!=0 ){dst++;src++;}
	*dst = 0;
	return s;
}

static const WCHAR* _astr2wstr(WCHAR * dst, const char * src)
{
	WCHAR *s = dst;
	if(dst == NULL || src == NULL)
		return NULL;
	while ((*dst = *src)!=0 ){dst++;src++;}
	*dst = 0;
	return s;
}

void Error(HRESULT hr, const char *psz)
{
	printf("%s failed and returned 0x%x\n", psz, hr);
}



class EventSink : public ITalkSessionEvents
{
public:
	STDMETHODIMP QueryInterface(REFIID riid, void**ppv)
	{
		if (riid == IID_IUnknown)
			*ppv = static_cast<ITalkSessionEvents*>(this);
		else if (riid == IID_ITalkSessionEvents)
			*ppv = static_cast<ITalkSessionEvents*>(this);
		else
			return (*ppv = 0), E_NOINTERFACE;
		reinterpret_cast<IUnknown*>(*ppv)->AddRef();
		return S_OK;
	}
	STDMETHODIMP_(ULONG) AddRef(void)
	{
		return 2;
	}
	STDMETHODIMP_(ULONG) Release(void)
	{
		return 1;
	}
	STDMETHODIMP OnStatement( const OLECHAR *pwszUser,
		const OLECHAR *pwszStmt)
	{
		char theStr0[256];
		char theStr1[1024];
		printf("\r%-14s: %s\n: ",_wstr2astr(theStr0,pwszUser),_wstr2astr(theStr1,pwszStmt));
		printf("\ryou           : ");
		return S_OK;
	}

};


int main(int argc, char **argv)
{
	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr))
	{
		// grab the requested session manager
		ITalkSessionManager *ptsm = 0;
		// hr = CoGetClassObject(CLSID_TalkSession, CLSCTX_ALL, pcsi, IID_ITalkSessionManager, (void**)&ptsm);
		hr = CoCreateInstance(CLSID_TalkToStranger,NULL,CLSCTX_ALL,IID_ITalkSessionManager,	(void**)&ptsm);
		if (SUCCEEDED(hr))
		{
			ITalkSession *pts =NULL;
			hr = ptsm->CreateSession(&pts);
			if(SUCCEEDED(hr))
			{
				// hook up event sink to receive new messages
				EventSink es;
				DWORD dwReg;
				hr = pts->Advise(&es, &dwReg);
				if (SUCCEEDED(hr))
				{
					// run UI loop to get statements from console and send them
					OLECHAR wszStmt[1024];
					char cszStmt[1024];
					printf("\nnote          :say \"/chat\" to start a new talk,say \"/bye\" to end the current talk,say \"/exit\" to exit room\n");
					printf("\ryou           : ");
					while (fgets(cszStmt,1024,stdin))
					{
						if(cszStmt[0] == 0){
							printf("\ryou           : ");
							continue;
						}
						if(cszStmt[strlen(cszStmt) -1] == '\n')
							cszStmt[strlen(cszStmt) -1] = 0;
						if(cszStmt[0] == 0){
							printf("\ryou           : ");
							continue;
						}
						
						_astr2wstr(wszStmt,cszStmt);
						hr = pts->Say(wszStmt);
						if (FAILED(hr))
							Error(hr, "Say");

						if(strcmp(cszStmt,"/exit") == 0)
							break;
						printf("\ryou           : ");
					}
					// tear down connection for event sink
					pts->Unadvise(dwReg);
				}
				else
					Error(hr, "Advise");
				// release chat session
				pts->Release();
			}

		}
		// release session manager
		ptsm->Release();

		CoUninitialize();
	}

return hr;
}
