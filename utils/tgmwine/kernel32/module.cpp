/*
* Module API
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
#ifdef __linux__

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(modu);

#include "module.h"
#include <dlfcn.h>
#include "OSMutexRW.h"

#include <string>
#include <map>
#include <utility>
using namespace std;

static int string_unicode_to_ascii(char* dst,const WCHAR* src){
	char* p = dst;
	while (*p++ = *src++){}
	return p-dst;
}

typedef struct  
{
	string					dli_fname;	/* File name of defining object */
	void					*dli_fbase;	/* Load address of that object */
	void					*handle;
	int						dli_ref;
}agr_dll_infos_t;

static OSMutexRW						s_dll_mutex;
static map<void*, agr_dll_infos_t>		s_dll_infos;
typedef map<void*, agr_dll_infos_t>::iterator dll_iter_t;

typedef BOOL (WINAPI *__DllMain_t)(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID fImpLoad);

static FARPROC _get_process_address( HMODULE hModule, LPCSTR function );

HMODULE WINE_WINAPI LoadLibraryA(LPCSTR libname)
{
	void* hModule = dlopen(libname, RTLD_NOW | RTLD_GLOBAL);
	if(hModule)	{
		OSMutexWriteLocker wlocker(&s_dll_mutex);
		dll_iter_t iter = s_dll_infos.find(hModule);
		if(iter != s_dll_infos.end())	{
			TRACE("name:%s, hModule:%p, have been loaded \r\n",libname,hModule);
			++(*iter).second.dli_ref;

		}
		else{
			agr_dll_infos_t info;
			info.dli_fname	= libname;
			info.dli_ref	= 1;
			info.dli_fbase	= NULL;
			info.handle		= hModule;
			s_dll_infos.insert(make_pair(hModule,info));
			
			//__DllMain_t* __DllMain = (__DllMain_t*)_get_process_address((HMODULE)hModule,"DllMain");
			//if(__DllMain)	(*__DllMain)((HINSTANCE)hModule,DLL_PROCESS_ATTACH,NULL);
			//TRACE("name:%s, hModule:%p, dllmain:%p Insert to s_dll_infos \r\n",libname,hModule,__DllMain);
			TRACE("name:%s, hModule:%p, Insert to s_dll_infos \r\n",libname,hModule);
		}
	}

	if(NULL == hModule){
		WARN("name(%s) failed:%s\r\n",libname,dlerror());
	}
	
	return (HMODULE)hModule;
}

HMODULE WINE_WINAPI LoadLibraryExA(LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
{
	return LoadLibraryA(lpLibFileName);
}


HMODULE WINE_WINAPI LoadLibraryExW(LPCWSTR libnameW, HANDLE hfile, DWORD flags){
	char nameA[260];
	string_unicode_to_ascii(nameA,libnameW);
	return LoadLibraryA(nameA);
}


BOOL WINE_WINAPI FreeLibrary(HINSTANCE hLibModule)
{
	if (hLibModule == NULL)
		return FALSE;

	dlclose(hLibModule);

	if(hLibModule)	{
		OSMutexWriteLocker wlocker(&s_dll_mutex);
		dll_iter_t iter = s_dll_infos.find(hLibModule);
		if(iter != s_dll_infos.end())	{
			--(*iter).second.dli_ref;
			if(0 == (*iter).second.dli_ref)
				s_dll_infos.erase(iter);
		}

	}
	return TRUE;
}

static FARPROC _get_process_address( HMODULE hModule, LPCSTR function ){
	void * func = dlsym(hModule, function);
	if(func == NULL)	return NULL;

	dll_iter_t iter = s_dll_infos.find(hModule);
	if(iter != s_dll_infos.end() && NULL == (*iter).second.dli_fbase){
		Dl_info info;
		memset(&info,0,sizeof(info));
		dladdr(func,&info);
		(*iter).second.dli_fbase = info.dli_fbase;
		TRACE("hModule:%p, function:%s, and find dli_fbase:%p \r\n",hModule,function,info.dli_fbase);
	}

	return 	 (FARPROC)func;
}
FARPROC WINE_WINAPI GetProcAddress( HMODULE hModule, LPCSTR function )
{
	if (hModule == NULL)
		return NULL;
	void * func = dlsym(hModule, function);
	//printf("module.cpp GetProcAddress:%p,%s,%p\r\n",hModule,function,func);
	if(func == NULL)	return NULL;

	OSMutexReadLocker rlocker(&s_dll_mutex);
	dll_iter_t iter = s_dll_infos.find(hModule);
	if(iter != s_dll_infos.end() && NULL == (*iter).second.dli_fbase){
		Dl_info info;
		memset(&info,0,sizeof(info));
		dladdr(func,&info);
		(*iter).second.dli_fbase = info.dli_fbase;
		TRACE("hModule:%p, function:%s, and find dli_fbase:%p \r\n",hModule,function,info.dli_fbase);
	}
	
	return 	 (FARPROC)func;
}

DWORD WINE_WINAPI GetModuleFileNameA(
								HMODULE hModule,	/* [in] Module handle (32 bit) */
								LPSTR   lpFileName,	/* [out] Destination for file name */
								DWORD   size )		/* [in] Size of lpFileName in characters */
{
	int len = 0;
	if (hModule == NULL){
		WARN("WARN : hModule is null\r\n");
		return 0;
	}
	lpFileName[0] = 0;
	OSMutexReadLocker rlocker(&s_dll_mutex);
	dll_iter_t iter = s_dll_infos.find(hModule);
	if(iter != s_dll_infos.end())	{
		size_t theNameLen = strlen((*iter).second.dli_fname.c_str());
		len = min(size-1,theNameLen);
		strncpy(lpFileName,(*iter).second.dli_fname.c_str(),len);
		lpFileName[len] = 0;
		TRACE("Successed: hModule(%p), lpFileName:%s,size(%ld),len(%ld) \r\n",hModule,lpFileName,size,len);
	}
	else{
		WARN("Failed : hModule(%p)\r\n",hModule);
	}
	
	return len;
}


HMODULE WINE_WINAPI GetCurrentModule(void * func)
{
	Dl_info info;
	memset(&info,0,sizeof(info));
	dladdr(func,&info);
	if(info.dli_fbase == NULL){
		WARN("dladdr Failed : func(%p)\r\n",func);
		return NULL;
	}
	OSMutexReadLocker rlocker(&s_dll_mutex);
	for(dll_iter_t iter = s_dll_infos.begin();iter != s_dll_infos.end();iter++){
		if(info.dli_fbase == (*iter).second.dli_fbase){
			TRACE("Successed: func(%p),moudle(%p)\r\n",func,(HMODULE)(*iter).first);
			return (HMODULE)(*iter).first;
		}
	}
	WARN("Failed : func(%p),info.dli_fbase(%p)\r\n",func,info.dli_fbase);
	return NULL;
}


BOOL WINE_WINAPI DisableThreadLibraryCalls( HMODULE hModule )
{
	//NTSTATUS    nts = LdrDisableThreadCalloutsForDll( hModule );
	//if (nts == STATUS_SUCCESS) return TRUE;

	//SetLastError( RtlNtStatusToDosError( nts ) );
	//return FALSE;
	FIXME("DisableThreadLibraryCalls Not Implemented");
	return FALSE;
}

#endif

