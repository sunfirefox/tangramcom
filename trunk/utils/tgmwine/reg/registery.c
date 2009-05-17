/*
* registry implement by tranzda
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
#ifdef  REGISTRERY_IMPLEMENT_DATABASE
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
//#undef __WINESRC__

#include "windef.h"
#include "winbase.h"
#include "winternl.h"
#include "winreg.h"

#include "tgmlog.h"
#include "tgmlog_util.h"

TGMLOG_DEFAULT_DEBUG_CHANNEL(reg);
#ifdef WIN32
#define _TRANZDA_VM_
#endif
#if 1


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif
typedef void * HREGKEY;
typedef signed int          Int32;
typedef unsigned int        UInt32;
extern signed int SS_RegKey_CreateKeyEx(IN HREGKEY hKey, IN const unsigned short * pSubKeyName, OUT HREGKEY * phkResult,
									  OUT UInt32 * puStatus);
extern signed int SS_RegKey_DeleteKey(IN HREGKEY hKey, IN const unsigned short * pSubKeyName);
extern signed int SS_RegKey_OpenKeyEx(IN HREGKEY hKey, IN const unsigned short * pSubKeyName, OUT HREGKEY * phkResult);
extern signed int SS_RegKey_CloseKey(IN HREGKEY hKey);
extern signed int SS_RegKey_SetValueEx(IN HREGKEY hKey, IN const unsigned short * pValueName, IN UInt32 uValueType,
									 IN const void * pValueData, IN signed int nValueDataLength);
extern signed int SS_RegKey_QueryValueEx(IN HREGKEY hKey, IN const unsigned short * pValueName, OUT UInt32 * puValueType,
									   OUT void * pValueData, IN OUT signed int * pnValueDataLength);
extern signed int SS_RegKey_DeleteValue(IN HREGKEY hKey, IN const unsigned short * pValueName);
extern signed int SS_RegKey_QueryInfoKey(IN HREGKEY hKey, OUT signed int * pnSubKeysCount, OUT signed int * pnMaxSubKeyNameLen,
									   OUT signed int * pnValuesCount, OUT signed int * pnMaxValueNameLen,
									   OUT signed int * pnMaxValueDataLen);
extern signed int SS_RegKey_EnumValue(IN HREGKEY hKey, IN signed int nIndex, OUT unsigned short * pValueName,
									IN OUT signed int * pnValueNameLength, OUT UInt32 * puValueType,
									OUT void * pValueData, IN OUT signed int * pnValueDataLength);
extern signed int SS_RegKey_EnumKeyEx(IN HREGKEY hKey, IN signed int nIndex, OUT unsigned short * pSubKeyName,
									IN OUT signed int * pnSubKeyNameLength);
#define REGHKEY_SYSKEY_CLASSES_ROOT         ((HREGKEY)0x00000001)

static int string_unicode_to_ascii(char* dst,const WCHAR* src){
	char* p = dst;
	while (*p++ = *src++){}
	return p-dst;
}

static int __lstrlenW( LPCWSTR str )
{
	const WCHAR *s = str;
	while (*s) s++;
	return s - str;
}


static LSTATUS convert_ssret_to_lstatus(int ssret){
	if(ssret >= 0)
		return ERROR_SUCCESS;
	switch(ssret)
	{
	case -4:
		return ERROR_FILE_NOT_FOUND;
	case -3:
		return ERROR_NO_MORE_ITEMS;
	}
	return ssret;
}



static HKEY convert_winkey_to_sskry(HKEY hkey){
	switch((DWORD)hkey)
	{
	case (DWORD)HKEY_CLASSES_ROOT:
		hkey = REGHKEY_SYSKEY_CLASSES_ROOT;
		break;
	}
	return hkey;
}

static LPCWSTR reg_net_next_key(LPCWSTR name,LPWSTR keyname)
{
	WCHAR* p = name;
	if(NULL == name) return NULL;
	while ((*keyname = *p) != 0 )	{
		if((*keyname) == '\\'){
			*keyname = '\0';
			return ++p;
		}
		++keyname;
		++p;
	}
	return NULL;
}

/******************************************************************************
* RegCreateKeyExW   [ADVAPI32.@]
*
* See RegCreateKeyExA.
*/
LSTATUS WINE_WINAPI RegCreateKeyExW( HKEY hkey, LPCWSTR name, DWORD reserved, LPWSTR class_,
							   DWORD options, REGSAM access, SECURITY_ATTRIBUTES *sa,
							   PHKEY retkey, LPDWORD dispos )
{
	UInt32 uStatus;
	int ret = 0;
	LPCWSTR nxtName;
	WCHAR	keyname[128];
	int		status;
	HKEY	key,subkey;
	UNICODE_STRING src;
	UNICODE_STRING dst;

	//TRACE(" >> RegCreateKeyExW(hkey:%p,name:%s\n",hkey,debugstr_w(name));

	hkey	= convert_winkey_to_sskry(hkey);
	key		= hkey;

	RtlInitUnicodeStringEx(&src,name);
	RtlUpcaseUnicodeString(&dst,&src,TRUE);

	nxtName = dst.Buffer;

	do 
	{
		nxtName = reg_net_next_key(nxtName,keyname);
		status = (SS_RegKey_CreateKeyEx( key, keyname, &subkey,&uStatus));
		if(nxtName != NULL){
			if(key != hkey)
				RegCloseKey(key);
			key = subkey;
			subkey = NULL;
		}

	} while(nxtName);

	RtlFreeUnicodeString(&dst);

	*retkey = subkey;

	TRACE("RegCreateKeyExW(hkey:%p, name:%s, retkey:%p, status:%ld\n",hkey,tgmlog_dbgstr_w(name),subkey,status);
	return convert_ssret_to_lstatus(status);
}




LSTATUS WINE_WINAPI RegCreateKeyExA( HKEY hkey, LPCSTR name, DWORD reserved, LPSTR class_,
							   DWORD options, REGSAM access, SECURITY_ATTRIBUTES *sa,
							   PHKEY retkey, LPDWORD dispos )
{

	UNICODE_STRING nameA;
	int ret = 0;

	RtlCreateUnicodeStringFromAsciiz(&nameA,name);
	ret = RegCreateKeyExW(hkey, nameA.Buffer, reserved,class_,options,access,sa,retkey,dispos);
	RtlFreeUnicodeString(&nameA);

	return ret;
}


LSTATUS WINE_WINAPI RegCreateKeyW( HKEY hkey, LPCWSTR lpSubKey, PHKEY phkResult )
{
	return RegCreateKeyExW(hkey,lpSubKey,0,NULL,0,0,NULL,phkResult,NULL);
}

LSTATUS WINE_WINAPI RegCreateKeyA( HKEY hkey, LPCSTR lpSubKey, PHKEY phkResult )
{
	return RegCreateKeyExA(hkey,lpSubKey,0,NULL,0,0,NULL,phkResult,NULL);
}


LSTATUS WINE_WINAPI RegOpenKeyExW( HKEY hkey, LPCWSTR name, DWORD reserved, REGSAM access, PHKEY retkey )
{
	WCHAR	keyname[128];
	HKEY	subkey;
	LPCWSTR nxtName;
	HKEY	key;
	LSTATUS status;
	UNICODE_STRING src;
	UNICODE_STRING dst;

	//TRACE(" >> RegOpenKeyExW(hkey:%p,name:%s\n",hkey,debugstr_w(name));

	 hkey	= convert_winkey_to_sskry(hkey);
	 key	= hkey;

	 RtlInitUnicodeStringEx(&src,name);
	 RtlUpcaseUnicodeString(&dst,&src,TRUE);
	 nxtName = dst.Buffer;
	
	do 
	{
		nxtName = reg_net_next_key(nxtName,keyname);
		status = convert_ssret_to_lstatus(SS_RegKey_OpenKeyEx( key, keyname, &subkey));
		if(nxtName != NULL){
			if(key != hkey)
				RegCloseKey(key);
			key = subkey;
			subkey = NULL;
		}

	} while(nxtName);

	RtlFreeUnicodeString(&dst);
	
	*retkey = subkey;

	TRACE("RegOpenKeyExW(hkey:%p, name:%s, retkey:%p, status:%ld)\n",hkey,tgmlog_dbgstr_w(name),subkey,status);
	return status;
}


LSTATUS WINE_WINAPI RegOpenKeyExA( HKEY hkey, LPCSTR name, DWORD reserved, REGSAM access, PHKEY retkey )
{	
	UNICODE_STRING uName;
	NTSTATUS status;

	RtlCreateUnicodeStringFromAsciiz(&uName,name);
	status =  RegOpenKeyExW( hkey, uName.Buffer, reserved, access,retkey);
	RtlFreeUnicodeString(&uName);
	
	return status;
}


LSTATUS WINE_WINAPI RegOpenKeyW( HKEY hkey, LPCWSTR name, PHKEY retkey )
{
	return RegOpenKeyExW(hkey,name,0,0,retkey);
}


LSTATUS WINE_WINAPI RegOpenKeyA( HKEY hkey, LPCSTR name, PHKEY retkey )
{
	return RegOpenKeyExA(hkey,name,0,0,retkey);
}

LSTATUS WINE_WINAPI RegEnumKeyExW( HKEY hkey, DWORD index, LPWSTR name, LPDWORD name_len,
							 LPDWORD reserved, LPWSTR class_, LPDWORD class_len, FILETIME *ft )
{
	NTSTATUS status;
	UNICODE_STRING src;
	UNICODE_STRING dst;

	//name[0] = 0;
	//RtlInitUnicodeStringEx(&src,name);
	//RtlUpcaseUnicodeString(&dst,&src,TRUE);

	name[0]=0;
	hkey = convert_winkey_to_sskry(hkey);
	//status =  SS_RegKey_EnumKeyEx(hkey,index,dst.Buffer,name_len);
	status =  SS_RegKey_EnumKeyEx(hkey,index,name,name_len);
	if(status == -4)
		status = -3;
	
	//RtlFreeUnicodeString(&dst);

	TRACE("RegEnumKeyExW(hkey:%p, index:%lu, name:%s, status:%ld)\n",hkey,index,tgmlog_dbgstr_w(name),status);
	return convert_ssret_to_lstatus(status);
}

LSTATUS WINE_WINAPI RegEnumKeyExA( HKEY hkey, DWORD index, LPSTR name, LPDWORD name_len,
							 LPDWORD reserved, LPSTR class_, LPDWORD class_len, FILETIME *ft )
{
	NTSTATUS status;
	WCHAR uName[128];

	uName[0] = 0;
	status =  RegEnumKeyExW(hkey,index,uName,name_len,reserved,class_,class_len,ft);
	if(status ==  ERROR_SUCCESS ){
		string_unicode_to_ascii(name,uName);
	}


	
	return status;
}


LSTATUS WINE_WINAPI RegEnumKeyW( HKEY hkey, DWORD index, LPWSTR name, DWORD name_len )
{
	return RegEnumKeyExW( hkey, index, name, &name_len, NULL, NULL, NULL, NULL );
}


LSTATUS WINE_WINAPI RegEnumKeyA( HKEY hkey, DWORD index, LPSTR name, DWORD name_len )
{
	return RegEnumKeyExA( hkey, index, name, &name_len, NULL, NULL, NULL, NULL );
}


LSTATUS WINE_WINAPI RegEnumValueW( HKEY hkey, DWORD index, LPWSTR value, LPDWORD val_count,
							 LPDWORD reserved, LPDWORD type, LPBYTE data, LPDWORD count )
{
	hkey = convert_winkey_to_sskry(hkey);
	return convert_ssret_to_lstatus(SS_RegKey_EnumValue(hkey,index,value,val_count,type,data,count));
}


LSTATUS WINE_WINAPI RegCloseKey( HKEY hkey )
{
	hkey = convert_winkey_to_sskry(hkey);
	return convert_ssret_to_lstatus(SS_RegKey_CloseKey(hkey));
}


/******************************************************************************
* RegDeleteKeyW   [ADVAPI32.@]
*
* See RegDeleteKeyA.
*/
LSTATUS WINE_WINAPI RegDeleteKeyW( HKEY hkey, LPCWSTR name )
{
	int status;
	UNICODE_STRING src;
	UNICODE_STRING dst;

	RtlInitUnicodeStringEx(&src,name);
	RtlUpcaseUnicodeString(&dst,&src,TRUE);

	hkey = convert_winkey_to_sskry(hkey);
	status = SS_RegKey_DeleteValue(hkey,dst.Buffer);
	RtlFreeUnicodeString(&dst);

	return convert_ssret_to_lstatus(status);
}


LSTATUS WINE_WINAPI RegDeleteKeyA( HKEY hkey, LPCSTR name )
{
	UNICODE_STRING uName;
	NTSTATUS status;

	RtlCreateUnicodeStringFromAsciiz(&uName,name);
	status =  RegDeleteKeyW(hkey,uName.Buffer);
	RtlFreeUnicodeString(&uName);

	return status;
}

static WCHAR sz_default_val_w[] = {'D','e','f','a','u','l','t','\0'};
static CHAR sz_default_val_a[] = {'D','e','f','a','u','l','t','\0'};

LSTATUS WINE_WINAPI RegSetValueExW( HKEY hkey, LPCWSTR name, DWORD reserved,
							  DWORD type, CONST BYTE *data, DWORD count )
{
	NTSTATUS status;
	hkey = convert_winkey_to_sskry(hkey);
	if(NULL == name || 0 == name[0]) name = sz_default_val_w;
	status = SS_RegKey_SetValueEx(hkey,name,type,data,type == 1 ? count/2 :count);

	TRACE("RegSetValueExW(hkey:%p, name:%s, type:%lu,status:%ld)\n",hkey,tgmlog_dbgstr_w(name),type,status);
	return  convert_ssret_to_lstatus(status);
}


LSTATUS WINE_WINAPI RegSetValueExA( HKEY hkey, LPCSTR name, DWORD reserved, DWORD type,
							  CONST BYTE *data, DWORD count )
{
	UNICODE_STRING uName;
	UNICODE_STRING uData;
	NTSTATUS status;

	RtlCreateUnicodeStringFromAsciiz( &uName, name );
	if(type == 1)	{
		RtlCreateUnicodeStringFromAsciiz( &uData, data );
		status =  RegSetValueExW(hkey,uName.Buffer,reserved,type,uData.Buffer,uData.Length);
		RtlFreeUnicodeString(&uData);
	}
	else{
		status =  RegSetValueExW(hkey,uName.Buffer,reserved,type,data,count);
	}

	RtlFreeUnicodeString(&uName);
	
	return status;
}


LSTATUS WINE_WINAPI RegSetValueW( HKEY hkey, LPCWSTR name, DWORD type, LPCWSTR data, DWORD count )
{
	HKEY subkey = hkey;
	DWORD ret;

	if (type != REG_SZ || !data) return ERROR_INVALID_PARAMETER;

	if (name && name[0])  /* need to create the subkey */
	{
		if ((ret = RegCreateKeyW( hkey, name, &subkey )) != ERROR_SUCCESS) return ret;
	}

	ret = RegSetValueExW( subkey, NULL, 0, REG_SZ, (const BYTE*)data,
		(__lstrlenW( data ) + 1) * sizeof(WCHAR) );
	if (subkey != hkey) RegCloseKey( subkey );
	return ret;
}


LSTATUS WINE_WINAPI RegSetValueA( HKEY hkey, LPCSTR name, DWORD type, LPCSTR data, DWORD count )
{
	HKEY subkey = hkey;
	DWORD ret;

	if (type != REG_SZ || !data) return ERROR_INVALID_PARAMETER;

	if (name && name[0])  /* need to create the subkey */
	{
		if ((ret = RegCreateKeyA( hkey, name, &subkey )) != ERROR_SUCCESS) return ret;
	}
	ret = RegSetValueExA( subkey, NULL, 0, REG_SZ, (const BYTE*)data, strlen(data)+1 );
	if (subkey != hkey) RegCloseKey( subkey );
	return ret;
}







LSTATUS WINE_WINAPI RegQueryValueW( HKEY hkey, LPCWSTR name, LPWSTR data, LPLONG count ){
	UInt32 uValueType;
	return RegQueryValueExW(hkey,name,0,&uValueType,(LPSTR)data,count);

}

LSTATUS WINE_WINAPI RegQueryValueA( HKEY hkey, LPCSTR name, LPSTR data, LPLONG count ){
	UInt32 uValueType;
	return RegQueryValueExA(hkey,name,0,&uValueType,data,count);
}

LSTATUS WINE_WINAPI RegQueryValueExW( HKEY hkey, LPCWSTR name, LPDWORD reserved, LPDWORD type,
								LPBYTE data, LPDWORD count )
{
	int	status;
	hkey = convert_winkey_to_sskry(hkey);
	if(NULL == name || 0 == name[0]) name = sz_default_val_w;
	status =  SS_RegKey_QueryValueEx(hkey, name,type,data, count);
	if(*type ==1){
		*count = *count *2;
	}

	TRACE("RegQueryValueExW(hkey:%p, name:%s, type:%lu,status:%ld)\n",hkey,tgmlog_dbgstr_w(name),type,status);
	return convert_ssret_to_lstatus(status);
}

LSTATUS WINE_WINAPI RegQueryValueExA( HKEY hkey, LPCSTR name, LPDWORD reserved, LPDWORD type,
								LPBYTE data, LPDWORD count )
//WINADVAPI LSTATUS   WINE_WINAPI RegQueryValueExA(HKEY,LPCSTR,LPDWORD,LPDWORD,LPBYTE,LPDWORD);
{
	UNICODE_STRING	uName;
	NTSTATUS status;

	RtlCreateUnicodeStringFromAsciiz( &uName, name );

	status = RegQueryValueExW(hkey,uName.Buffer,reserved,type,data,count);
    RtlFreeUnicodeString(&uName);

	if(status ==  ERROR_SUCCESS && *type ==1){
		char	astring[128];
		WCHAR	*pw =  data;
		char	*pa = astring;
		while (*pa++ = *pw++){	}
		//*pa = 0;
		strcpy(data,astring);
		*count = pa - astring;
	}

	return status;
}


LSTATUS   WINAPI RegDeleteValueW(HKEY hkey,LPCWSTR name){
	int	status;
	hkey = convert_winkey_to_sskry(hkey);
	status = SS_RegKey_DeleteValue(hkey,name);
	return convert_ssret_to_lstatus(status);
}

#endif

LSTATUS WINE_WINAPI RegDeleteTreeW(HKEY hKey, LPCWSTR lpszSubKey)
{
	LONG ret;
	//DWORD dwMaxSubkeyLen, dwMaxValueLen;
	DWORD dwMaxLen, dwSize;
	WCHAR szNameBuf[MAX_PATH], *lpszName = szNameBuf;
	HKEY hSubKey = hKey;

	//TRACE("(hkey=%p,%p %s)\n", hKey, lpszSubKey, debugstr_w(lpszSubKey));

	if(lpszSubKey)
	{
		ret = RegOpenKeyExW(hKey, lpszSubKey, 0, KEY_READ, &hSubKey);
		if (ret) return ret;
	}

	/* Get highest length for keys, values */
	//ret = RegQueryInfoKeyW(hSubKey, NULL, NULL, NULL, NULL,
	//	&dwMaxSubkeyLen, NULL, NULL, &dwMaxValueLen, NULL, NULL, NULL);
	//if (ret) goto cleanup;

	//dwMaxSubkeyLen++;
	//dwMaxValueLen++;
	//dwMaxLen = max(dwMaxSubkeyLen, dwMaxValueLen);
    dwMaxLen = 256;
	if (dwMaxLen > sizeof(szNameBuf)/sizeof(WCHAR))
	{
		/* Name too big: alloc a buffer for it */
		if (!(lpszName = HeapAlloc( GetProcessHeap(), 0, dwMaxLen*sizeof(WCHAR))))
		{
			ret = ERROR_NOT_ENOUGH_MEMORY;
			goto cleanup;
		}
	}


	/* Recursively delete all the subkeys */
	while (TRUE)
	{
		dwSize = dwMaxLen;
		if (RegEnumKeyExW(hSubKey, 0, lpszName, &dwSize, NULL,
			NULL, NULL, NULL)) break;

		ret = RegDeleteTreeW(hSubKey, lpszName);
		if (ret) goto cleanup;
	}

	if (lpszSubKey)
		ret = RegDeleteKeyW(hKey, lpszSubKey);
	else
		while (TRUE)
		{
			dwSize = dwMaxLen;
			if (RegEnumValueW(hKey, 0, lpszName, &dwSize,
				NULL, NULL, NULL, NULL)) break;

			ret = RegDeleteValueW(hKey, lpszName);
			if (ret) goto cleanup;
		}

cleanup:
		/* Free buffer if allocated */
		if (lpszName != szNameBuf)
			HeapFree( GetProcessHeap(), 0, lpszName);
		if(lpszSubKey)
			RegCloseKey(hSubKey);
		return ret;
}


LSTATUS WINE_WINAPI RegDeleteTreeA(HKEY hKey, LPCSTR lpszSubKey)
{
	LONG ret;
	UNICODE_STRING lpszSubKeyW;

	if (lpszSubKey) RtlCreateUnicodeStringFromAsciiz( &lpszSubKeyW, lpszSubKey);
	else lpszSubKeyW.Buffer = NULL;
	ret = RegDeleteTreeW( hKey, lpszSubKeyW.Buffer);
	RtlFreeUnicodeString( &lpszSubKeyW );
	return ret;
}

#endif//#ifdef  REGISTRERY_IMPLEMENT_DATABASE

