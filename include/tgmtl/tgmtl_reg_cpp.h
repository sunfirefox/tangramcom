/*
* tangram template library
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

#include "tgmtl_reg.h"
#include <stdio.h>
//#include "windef.h"
//#include "winbase.h"
//#include "winuser.h"
//#include "winreg.h"
//#include "winerror.h"

#if WIN32
extern "C" WINADVAPI LONG   WINAPI RegDeleteTreeA(HKEY,LPCSTR);
#else
extern "C" LONG   RegDeleteTreeA(HKEY,LPCSTR);
#endif

/***********************************************************************
*		static string constants
*/
static char const interface_keyname[]		= "Interface";
static char const base_ifa_keyname[]		= "BaseInterface";
static char const num_methods_keyname[]		= "NumMethods";
static char const ps_clsid_keyname[]		= "ProxyStubClsid";
static char const ps_clsid32_keyname[]		= "ProxyStubClsid32";
static char const clsid_keyname[]			= "CLSID";
static char const curversion_keyname[]		= "CurVer";
static char const ips32_keyname[]			= "InprocServer32";
static char const progid_keyname[]			= "ProgID";
static char const progid_ver_keyname[]		= "VersionIndependentProgID";
static char const tmodel_valuename[]		= "ThreadingModel";

/***********************************************************************
*		static helper functions
*/
static LONG register_key_guid(HKEY base, char const *name, GUID const *guid);
static LONG register_key_defvalueA(HKEY base, char const *name,char const *value);
static LONG register_progid(char const *clsid, char const *progid,char const *name);
static LONG register_progidver(char const *progid_ver,char const *progid,char const *name);
static const char* register_get_path();


static HRESULT tgm_string_from_guid(const GUID* riid,char *outStr,ULONG inSize)
{
	if (inSize < 39)
		return E_FAIL;

	sprintf(outStr, "{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		riid->Data1, riid->Data2, riid->Data3,
		riid->Data4[0], riid->Data4[1],
		riid->Data4[2], riid->Data4[3],
		riid->Data4[4], riid->Data4[5],
		riid->Data4[6], riid->Data4[7]);
	return S_OK;
}

/***********************************************************************
*		register_interfaces
*/
HRESULT register_interfaces(struct regsvr_interface const *list)
{
	LONG res = ERROR_SUCCESS;
	HKEY interface_key;

	//res = RegCreateKeyExA(HKEY_CLASSES_ROOT, interface_keyname, 0, NULL, 0,
		//KEY_READ | KEY_WRITE, NULL, &interface_key, NULL);
	res = RegCreateKeyA(HKEY_CLASSES_ROOT, interface_keyname, &interface_key);
	if (res != ERROR_SUCCESS) goto error_return;

	for (; res == ERROR_SUCCESS && list->iid; ++list) {
		char buf[39];
		HKEY iid_key;

		tgm_string_from_guid(list->iid, buf, 39);
		//res = RegCreateKeyExA(interface_key, buf, 0, NULL, 0,
		//	KEY_READ | KEY_WRITE, NULL, &iid_key, NULL);
		res = RegCreateKeyA( interface_key, buf, &iid_key);
		if (res != ERROR_SUCCESS) goto error_close_interface_key;

		if (list->name) {
			res = RegSetValueExA(iid_key, NULL, 0, REG_SZ,
				(CONST BYTE*)(list->name),
				strlen(list->name) + 1);
			if (res != ERROR_SUCCESS) goto error_close_iid_key;
		}

		if (list->base_iid) {
			res = register_key_guid(iid_key, base_ifa_keyname, list->base_iid);
			if (res != ERROR_SUCCESS) goto error_close_iid_key;
		}

		if (0 <= list->num_methods) {
			static char const fmt[3] = { '%','d',0 };
			HKEY key;

			//res = RegCreateKeyExA(iid_key, num_methods_keyname, 0, NULL, 0,
				//KEY_READ | KEY_WRITE, NULL, &key, NULL);
			res = RegCreateKeyA(iid_key, num_methods_keyname, &key);
			if (res != ERROR_SUCCESS) goto error_close_iid_key;

			sprintf(buf, fmt, list->num_methods);
			res = RegSetValueExA(key, NULL, 0, REG_SZ,
				(CONST BYTE*)buf,
				(strlen(buf) + 1) * sizeof(char));
			RegCloseKey(key);

			if (res != ERROR_SUCCESS) goto error_close_iid_key;
		}

		if (list->ps_clsid) {
			res = register_key_guid(iid_key, ps_clsid_keyname, list->ps_clsid);
			if (res != ERROR_SUCCESS) goto error_close_iid_key;
		}

		if (list->ps_clsid32) {
			res = register_key_guid(iid_key, ps_clsid32_keyname, list->ps_clsid32);
			if (res != ERROR_SUCCESS) goto error_close_iid_key;
		}

error_close_iid_key:
		RegCloseKey(iid_key);
	}

error_close_interface_key:
	RegCloseKey(interface_key);
error_return:
	return res != ERROR_SUCCESS ? HRESULT_FROM_WIN32(res) : S_OK;
}

/***********************************************************************
*		unregister_interfaces
*/
HRESULT unregister_interfaces(struct regsvr_interface const *list)
{
	LONG res = ERROR_SUCCESS;
	HKEY interface_key;

	res = RegOpenKeyExA(HKEY_CLASSES_ROOT, interface_keyname, 0,
		KEY_READ | KEY_WRITE, &interface_key);
	if (res == ERROR_FILE_NOT_FOUND) return S_OK;
	if (res != ERROR_SUCCESS) goto error_return;

	for (; res == ERROR_SUCCESS && list->iid; ++list) {
		char buf[39];

		tgm_string_from_guid(list->iid, buf, 39);
		res = RegDeleteTreeA(interface_key, buf);
		if (res == ERROR_FILE_NOT_FOUND) res = ERROR_SUCCESS;
	}

	RegCloseKey(interface_key);
error_return:
	return res != ERROR_SUCCESS ? HRESULT_FROM_WIN32(res) : S_OK;
}

/***********************************************************************
*		register_coclasses
*/
HRESULT register_coclasses(struct regsvr_coclass const *list)
{
	LONG res = ERROR_SUCCESS;
	HKEY coclass_key;

	//res = RegCreateKeyExA(HKEY_CLASSES_ROOT, clsid_keyname, 0, NULL, 0,
	//	KEY_READ | KEY_WRITE, NULL, &coclass_key, NULL);
	res = RegCreateKeyA(HKEY_CLASSES_ROOT, clsid_keyname, &coclass_key);
	if (res != ERROR_SUCCESS) goto error_return;

	for (; res == ERROR_SUCCESS && list->clsid; ++list) {
		char buf[39];
		HKEY clsid_key;

		tgm_string_from_guid(list->clsid, buf, 39);
		//res = RegCreateKeyExA(coclass_key, buf, 0, NULL, 0,
		//	KEY_READ | KEY_WRITE, NULL, &clsid_key, NULL);
		res = RegCreateKeyA(coclass_key, buf, &clsid_key);
		if (res != ERROR_SUCCESS) goto error_close_coclass_key;

		if (list->name) {
			res = RegSetValueExA(clsid_key, NULL, 0, REG_SZ,
				(CONST BYTE*)(list->name),
				strlen(list->name) + 1);
			if (res != ERROR_SUCCESS) goto error_close_clsid_key;
		}

		//if (list->ips) {
		//	res = register_key_defvalueA(clsid_key, ips_keyname, list->ips);
		//	if (res != ERROR_SUCCESS) goto error_close_clsid_key;
		//}

		const char* ips32 = list->ips32 ? list->ips32 : register_get_path();

		if (ips32) {
			HKEY ips32_key;

			//res = RegCreateKeyExA(clsid_key, ips32_keyname, 0, NULL, 0,
			//	KEY_READ | KEY_WRITE, NULL,&ips32_key, NULL);
			res = RegCreateKeyA(clsid_key, ips32_keyname, &ips32_key);
			if (res != ERROR_SUCCESS) goto error_close_clsid_key;

			res = RegSetValueExA(ips32_key, NULL, 0, REG_SZ,
				(CONST BYTE*)ips32,
				lstrlenA(ips32) + 1);
			if (res == ERROR_SUCCESS && list->ips32_tmodel)
				res = RegSetValueExA(ips32_key, tmodel_valuename, 0, REG_SZ,
				(CONST BYTE*)list->ips32_tmodel,
				strlen(list->ips32_tmodel) + 1);
			RegCloseKey(ips32_key);
			if (res != ERROR_SUCCESS) goto error_close_clsid_key;
		}

		if (list->progid) {
			res = register_key_defvalueA(clsid_key, progid_keyname,
				list->progid);
			if (res != ERROR_SUCCESS) goto error_close_clsid_key;

			res = register_progid(buf, list->progid, list->name);
			if (res != ERROR_SUCCESS) goto error_close_clsid_key;
		}

		if (list->progid_ver) {
			res = register_key_defvalueA(clsid_key, progid_ver_keyname,
				list->progid_ver);
			if (res != ERROR_SUCCESS) goto error_close_clsid_key;

			res = register_progidver(list->progid_ver, list->progid, list->name);
			if (res != ERROR_SUCCESS) goto error_close_clsid_key;
		}

error_close_clsid_key:
		RegCloseKey(clsid_key);
	}

error_close_coclass_key:
	RegCloseKey(coclass_key);
error_return:
	return res != ERROR_SUCCESS ? HRESULT_FROM_WIN32(res) : S_OK;
}

/***********************************************************************
*		unregister_coclasses
*/
HRESULT unregister_coclasses(struct regsvr_coclass const *list)
{
	LONG res = ERROR_SUCCESS;
	HKEY coclass_key;

	//res = RegOpenKeyExA(HKEY_CLASSES_ROOT, clsid_keyname, 0,
		//KEY_READ | KEY_WRITE, &coclass_key);
	res = RegOpenKeyA(HKEY_CLASSES_ROOT, clsid_keyname,&coclass_key);
	if (res == ERROR_FILE_NOT_FOUND) return S_OK;
	if (res != ERROR_SUCCESS) goto error_return;

	for (; res == ERROR_SUCCESS && list->clsid; ++list) {
		char buf[39];

		tgm_string_from_guid(list->clsid, buf, 39);
		res = RegDeleteTreeA(coclass_key, buf);
		if (res == ERROR_FILE_NOT_FOUND) res = ERROR_SUCCESS;
		if (res != ERROR_SUCCESS) goto error_close_coclass_key;

		if (list->progid) {
			res = RegDeleteTreeA(HKEY_CLASSES_ROOT, list->progid);
			if (res == ERROR_FILE_NOT_FOUND) res = ERROR_SUCCESS;
			if (res != ERROR_SUCCESS) goto error_close_coclass_key;
		}

		if (list->progid_ver) {
			res = RegDeleteTreeA(HKEY_CLASSES_ROOT, list->progid_ver);
			if (res == ERROR_FILE_NOT_FOUND) res = ERROR_SUCCESS;
			if (res != ERROR_SUCCESS) goto error_close_coclass_key;
		}
	}

error_close_coclass_key:
	RegCloseKey(coclass_key);
error_return:
	return res != ERROR_SUCCESS ? HRESULT_FROM_WIN32(res) : S_OK;
}

/***********************************************************************
*		regsvr_key_guid
*/
static LONG register_key_guid(HKEY base, char const *name, GUID const *guid)
{
	char buf[39];

	tgm_string_from_guid(guid, buf, 39);
	return register_key_defvalueA(base, name, buf);
}

/***********************************************************************
*		regsvr_key_defvalueW
*/
//static LONG register_key_defvalueW(
//								   HKEY base,
//								   char const *name,
//								   char const *value)
//{
//	LONG res;
//	HKEY key;
//
//	res = RegCreateKeyExW(base, name, 0, NULL, 0,
//		KEY_READ | KEY_WRITE, NULL, &key, NULL);
//	if (res != ERROR_SUCCESS) return res;
//	res = RegSetValueExW(key, NULL, 0, REG_SZ, (CONST BYTE*)value,
//		(lstrlenW(value) + 1) * sizeof(char));
//	RegCloseKey(key);
//	return res;
//}

/***********************************************************************
*		regsvr_key_defvalueA
*/
static LONG register_key_defvalueA(
								   HKEY base,
								   char const *name,
								   char const *value)
{
	LONG res;
	HKEY key;

	res = RegCreateKeyExA(base, name, 0, NULL, 0,
		KEY_READ | KEY_WRITE, NULL, &key, NULL);
	if (res != ERROR_SUCCESS) return res;
	res = RegSetValueExA(key, NULL, 0, REG_SZ, (CONST BYTE*)value,
		lstrlenA(value) + 1);
	RegCloseKey(key);
	return res;
}

/***********************************************************************
*		regsvr_progid
*/
static LONG register_progid(
							char const *clsid,
							char const *progid,
							char const *name)
{
	LONG res;
	HKEY progid_key;

	res = RegCreateKeyExA(HKEY_CLASSES_ROOT, progid, 0,
		NULL, 0, KEY_READ | KEY_WRITE, NULL,
		&progid_key, NULL);
	if (res != ERROR_SUCCESS) return res;

	if (name) {
		res = RegSetValueExA(progid_key, NULL, 0, REG_SZ,
			(CONST BYTE*)name, strlen(name) + 1);
		if (res != ERROR_SUCCESS) goto error_close_progid_key;
	}

	if (clsid) {
		res = register_key_defvalueA(progid_key, clsid_keyname, clsid);
		if (res != ERROR_SUCCESS) goto error_close_progid_key;
	}

error_close_progid_key:
	RegCloseKey(progid_key);
	return res;
}

/***********************************************************************
*		regsvr_progid
*/
static LONG register_progidver(
							char const *progid_ver,
							char const *progid,
							char const *name)
{
	LONG res;
	HKEY progid_key;

	res = RegCreateKeyExA(HKEY_CLASSES_ROOT, progid_ver, 0,
		NULL, 0, KEY_READ | KEY_WRITE, NULL,
		&progid_key, NULL);
	if (res != ERROR_SUCCESS) return res;

	if (name) {
		res = RegSetValueExA(progid_key, NULL, 0, REG_SZ,
			(CONST BYTE*)name, strlen(name) + 1);
		if (res != ERROR_SUCCESS) goto error_close_progid_key;
	}

	if (progid) {
		res = register_key_defvalueA(progid_key, curversion_keyname, progid);
		if (res != ERROR_SUCCESS) goto error_close_progid_key;
	}

error_close_progid_key:
	RegCloseKey(progid_key);
	return res;
}





#ifdef WIN32
static HINSTANCE tgm_get_this_instance(void)
{
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery(tgm_get_this_instance, &mbi, sizeof(mbi));
	return (HINSTANCE)mbi.AllocationBase;
}

#else

#define GUN_SOURCE
#include <dlfcn.h>
static void __get_cur_module_func_addr__(){};
static HINSTANCE tgm_get_this_instance(void)
{
	HINSTANCE hins =  GetCurrentModule((void*)__get_cur_module_func_addr__);
	return hins;
}

#endif

static const char* register_get_path(){
	HINSTANCE hInstance = tgm_get_this_instance();
	static char szModuleName[YACL_MAX_PATH] = {0};
	GetModuleFileNameA(hInstance, szModuleName, YACL_MAX_PATH);
	return szModuleName;
}

