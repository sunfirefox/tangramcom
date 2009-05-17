/*
* XML registry table
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

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "windef.h"
#include "winbase.h"

#include "winternl.h"
#include "winreg.h"

#include "XMLParser.h"

#ifdef  REGISTRERY_IMPLEMENT_XML
#include "OSMutex.h"
#include "winreg.h"

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(reg);

#ifdef WIN32
#define _TRANZDA_VM_
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

typedef void * HREGKEY;

#define REGHKEY_SYSKEY_CLASSES_ROOT         ((HREGKEY)0x00000001)

static XMLParser *_xmlparser = NULL;
static int _xmlparser_ref_count = 0;
static int _xmlparser_dirty_flag = 0;
static OSMutex* _xmlparser_mutex(){
    static  OSMutex mutex;  return &mutex;
}
//#ifdef WIN32
//static char com_registery_path[]  =  "D:\\NEWPLUS\\TDA_DATA\\Data\\SysData\\agrcom_registery.xml";
//#else
//static char com_registery_path[]  =  "/NEWPLUS/TDA_DATA/Data/SysData/agrcom_registery.xml";
//#endif

static char com_registery_path[] = TANGRAM_REGISTERTABLE_PATH;



static  char sz_default_val_a[]                = "__defaulr_value__";
static  char sz_val_key[]                      = "__value__";
static  char sz_val_key_attr_name[]            = "name";
static  char sz_val_key_attr_type[]            = "type";


static char * s_FileHeader[] =
{
    "<?xml version =\"1.0\"?>",
        NULL
};

static const char* get_next_tag_name(const char* name,char* keyname)
{
    const char* p = name;
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


static XMLParser* get_xmlparser(){

    OSMutexLocker _locker(_xmlparser_mutex());
    if(!_xmlparser){
        TRACE("%s\n",com_registery_path);
        _xmlparser = new XMLParser(com_registery_path);
        if(false == _xmlparser->ParseFile())
            _xmlparser->SetRootTag(new XMLTag("ROOT"));
    }
    return _xmlparser;
}



static XMLTag*  convert_winkey_to_tag(HKEY hKey){

    if (hKey == HKEY_CLASSES_ROOT)
    {
        XMLParser * xmlparser = get_xmlparser();
        XMLTag* rootTag = xmlparser->GetRootTag();
        XMLTag* clsTag = rootTag->GetEmbeddedTagByName("CLASSES");
        if(!clsTag){
            clsTag = new XMLTag("CLASSES");
            rootTag->AddEmbeddedTag(clsTag);
        }
        return clsTag;
    }
    else
        return (XMLTag*)hKey;
}

static inline int __lstrlenW( LPCWSTR str )
{
    const WCHAR *s = str;
    while (*s) s++;
    return s - str;
}

static inline int __string_unicode_to_ascii(char* dst,const WCHAR* src){
    char* p = dst;
    while (*p++ = *src++){}
    return p-dst;
}

static inline int __string_ascii_to_unicode(WCHAR* dst,const char* src){
    WCHAR* p = dst;
    while (*p++ = *src++){}
    return p-dst;
}

class _str_warp{
public:
    _str_warp(LPCWSTR inStr,int  len = -1):str(NULL)
    {
        if(inStr == NULL)    return;
        if(len = -1)
            len = __lstrlenW(inStr);
        str =  new char[len+1];
        __string_unicode_to_ascii(str,inStr);

    }

    ~_str_warp(){
        delete [] str;
    }

    char* get_str(){return str;}

private:
    char * str;
};

#define WSTR2A(XXX) _str_warp(XXX).get_str()


/******************************************************************************
* RegCreateKeyExW   [ADVAPI32.@]
*
* See RegCreateKeyExA.
*/
LSTATUS WINE_WINAPI RegCreateKeyExW( HKEY hKey, LPCWSTR name, DWORD reserved, LPWSTR class_,
                                    DWORD options, REGSAM access, SECURITY_ATTRIBUTES *sa,
                                    PHKEY retkey, LPDWORD dispos )
{
    return RegCreateKeyExA(hKey,WSTR2A(name),reserved, WSTR2A(class_), options,access,sa,retkey,dispos );
}


LSTATUS WINE_WINAPI RegCreateKeyExA( HKEY hKey, LPCSTR name, DWORD reserved, LPSTR class_,
                                    DWORD options, REGSAM access, SECURITY_ATTRIBUTES *sa,
                                    PHKEY retkey, LPDWORD dispos )
{
    char	keyname[128];
    const char*   nxtName = name;
    XMLTag*	key = NULL;
    XMLTag* subkey = NULL;
    LSTATUS status = ERROR_SUCCESS;

    key	= convert_winkey_to_tag(hKey);

    do 
    {
        nxtName = get_next_tag_name(nxtName,keyname);

        subkey = key->GetEmbeddedTagByName(keyname);
        if(NULL == subkey){
            subkey = new XMLTag(keyname);
            key->AddEmbeddedTag(subkey);
            _xmlparser_dirty_flag = 1;
        }

        if(nxtName != NULL){
            key = subkey;
        }
    } while(nxtName);


    *retkey = (HKEY)subkey;

    if(*retkey) {  _xmlparser_ref_count++;  }

    TRACE("hKey:%p, name:%s, retkey:%p, status:%ld\n",hKey,name,subkey,(long)status);
    return status;;
}


LSTATUS WINE_WINAPI RegCreateKeyW( HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult )
{
    return RegCreateKeyExW(hKey,lpSubKey,0,NULL,0,0,NULL,phkResult,NULL);
}

LSTATUS WINE_WINAPI RegCreateKeyA( HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult )
{
    return RegCreateKeyExA(hKey,lpSubKey,0,NULL,0,0,NULL,phkResult,NULL);
}


LSTATUS WINE_WINAPI RegOpenKeyExW( HKEY hKey, LPCWSTR name, DWORD reserved, REGSAM access, PHKEY retkey )
{
    return RegOpenKeyExA(  hKey, WSTR2A(name),  reserved,  access,  retkey );
}


LSTATUS WINE_WINAPI RegOpenKeyExA( HKEY hKey, LPCSTR name, DWORD reserved, REGSAM access, PHKEY retkey )
{	
    char	keyname[128];
    const char*   nxtName = name;
    XMLTag*	key = NULL,*subkey = NULL;
    LSTATUS status = ERROR_SUCCESS;

    key	= convert_winkey_to_tag(hKey);

    do 
    {
        nxtName = get_next_tag_name(nxtName,keyname);

        subkey = key->GetEmbeddedTagByName(keyname);
        if(NULL == subkey){
            status = ERROR_FILE_NOT_FOUND;
            break;
        }

        if(nxtName != NULL){
            key = subkey;
        }

    } while(nxtName);

    *retkey = (HKEY)subkey;

    if(*retkey) _xmlparser_ref_count++;

    TRACE(" hKey:%p, name:%s, retkey:%p, status:%ld\n",hKey,name,subkey,(long)status);
    return status;
}


LSTATUS WINE_WINAPI RegOpenKeyW( HKEY hKey, LPCWSTR name, PHKEY retkey )
{
    return RegOpenKeyExW(hKey,name,0,0,retkey);
}


LSTATUS WINE_WINAPI RegOpenKeyA( HKEY hKey, LPCSTR name, PHKEY retkey )
{
    return RegOpenKeyExA(hKey,name,0,0,retkey);
}

LSTATUS WINE_WINAPI RegEnumKeyExW( HKEY hKey, DWORD index, LPWSTR name, LPDWORD name_len,
                                  LPDWORD reserved, LPWSTR class_, LPDWORD class_len, FILETIME *ft )
{
    return RegEnumKeyExA(  hKey,  index, WSTR2A(name),  name_len, reserved, WSTR2A(class_),  class_len,  ft );
}

LSTATUS WINE_WINAPI RegEnumKeyExA( HKEY hKey, DWORD index, LPSTR name, LPDWORD name_len,
                                  LPDWORD reserved, LPSTR class_, LPDWORD class_len, FILETIME *ft )
{

    XMLTag*	key = NULL,*subkey = NULL;
    LSTATUS status = ERROR_SUCCESS;
    key = convert_winkey_to_tag(hKey);

    subkey = key->GetEmbeddedTag(index);
    if(NULL == subkey){
        name[0]=0;
        status = ERROR_NO_MORE_ITEMS;
    }
    else{
        strcpy(name,subkey->GetTagName());
        *name_len = strlen(name);
    }

    TRACE("hKey:%p, index:%lu, name:%s, status:%ld\n",hKey,(unsigned long)index,name,(long)status);
    return status;
}


LSTATUS WINE_WINAPI RegEnumKeyW( HKEY hKey, DWORD index, LPWSTR name, DWORD name_len )
{
    return RegEnumKeyExW( hKey, index, name, &name_len, NULL, NULL, NULL, NULL );
}


LSTATUS WINE_WINAPI RegEnumKeyA( HKEY hKey, DWORD index, LPSTR name, DWORD name_len )
{
    return RegEnumKeyExA( hKey, index, name, &name_len, NULL, NULL, NULL, NULL );
}


LSTATUS WINE_WINAPI RegCloseKey( HKEY hKey )
{
    OSMutexLocker _locker(_xmlparser_mutex());
    TRACE(" hKey:%p, _xmlparser_ref_count:%d,dirty:%d\n",hKey,_xmlparser_ref_count,_xmlparser_dirty_flag);
    if(hKey &&  --_xmlparser_ref_count == 0 && _xmlparser )
    {
        //XMLParser* theParser = get_xmlparser();
        if(_xmlparser_dirty_flag){
            TRACE("WriteToFile \n");
            _xmlparser->WriteToFile(s_FileHeader);
        }
        delete _xmlparser;
        _xmlparser = NULL;
        _xmlparser_dirty_flag = 0;
    }
    
    return ERROR_SUCCESS;
}


/******************************************************************************
* RegDeleteKeyW   [ADVAPI32.@]
*
* See RegDeleteKeyA.
*/
LSTATUS WINE_WINAPI RegDeleteKeyW( HKEY hKey, LPCWSTR name )
{
    return RegDeleteKeyA(hKey,WSTR2A(name));
}


LSTATUS WINE_WINAPI RegDeleteKeyA( HKEY hKey, LPCSTR name )
{
    XMLTag*	key = NULL,*subkey = NULL;
    LSTATUS status = ERROR_SUCCESS;
    key = convert_winkey_to_tag(hKey);

    subkey = key->GetEmbeddedTagByName(name);
    if(subkey){
        key->RemoveEmbeddedTag(subkey);
        _xmlparser_dirty_flag = 1;
    }
    else
        status = ERROR_FILE_NOT_FOUND;

    TRACE(" hKey:%p, name:%s, status:%ld\n",hKey,name,(long)status);
    return status;
}



LSTATUS WINE_WINAPI RegSetValueExW( HKEY hKey, LPCWSTR name, DWORD reserved,
                                   DWORD type, CONST BYTE *data, DWORD count )
{
    return RegSetValueExA(  hKey,  WSTR2A(name),  reserved,  type,  (BYTE*)WSTR2A((LPCWSTR)data),  count/2 );
}


LSTATUS WINE_WINAPI RegSetValueExA( HKEY hKey, LPCSTR name, DWORD reserved, DWORD type,
                                   CONST BYTE *data, DWORD count )
{
    LSTATUS status = ERROR_SUCCESS;
    XMLTag*	key = convert_winkey_to_tag(hKey);

    if(NULL == name || 0 == name[0]) name = sz_default_val_a;

    //    assert(type == 1);
    XMLTag* valkey = key->GetEmbeddedTagByNameAndAttr(sz_val_key,sz_val_key_attr_name,name);
    if(!valkey){
        valkey = new XMLTag(sz_val_key);
        key->AddEmbeddedTag(valkey);
        valkey->SetAttribute(sz_val_key_attr_name,(char*)name);
    }
    valkey->SetAttribute(sz_val_key_attr_type,"REG_SZ");
    valkey->SetValue((char*)data);
    //valkey->SetAttribute("size",count);
    _xmlparser_dirty_flag = 1;

    TRACE(" hKey:%p, name:%s, type:%lu, data:%s, count:%lu,status:%ld\n",hKey,name,(unsigned long)type,(char*)data,(unsigned long) count,(long)status);
    return status;
}


LSTATUS WINE_WINAPI RegSetValueW( HKEY hKey, LPCWSTR name, DWORD type, LPCWSTR data, DWORD count )
{
    HKEY subkey = hKey;
    DWORD ret;

    if (type != REG_SZ || !data) return ERROR_INVALID_PARAMETER;

    if (name && name[0])  /* need to create the subkey */
    {
        if ((ret = RegCreateKeyW( hKey, name, &subkey )) != ERROR_SUCCESS) return ret;
    }

    ret = RegSetValueExW( subkey, NULL, 0, REG_SZ, (const BYTE*)data,
        (__lstrlenW( data ) + 1) * sizeof(WCHAR) );
    if (subkey != hKey) RegCloseKey( subkey );
    return ret;
}


LSTATUS WINE_WINAPI RegSetValueA( HKEY hKey, LPCSTR name, DWORD type, LPCSTR data, DWORD count )
{
    HKEY subkey = hKey;
    DWORD ret;

    if (type != REG_SZ || !data) return ERROR_INVALID_PARAMETER;

    if (name && name[0])  /* need to create the subkey */
    {
        if ((ret = RegCreateKeyA( hKey, name, &subkey )) != ERROR_SUCCESS) return ret;
    }
    ret = RegSetValueExA( subkey, NULL, 0, REG_SZ, (const BYTE*)data, strlen(data)+1 );
    if (subkey != hKey) RegCloseKey( subkey );
    return ret;
}



LSTATUS WINE_WINAPI RegQueryValueExW( HKEY hKey, LPCWSTR name, LPDWORD reserved, LPDWORD type,
                                     LPBYTE data, LPDWORD count )
{
    char* _data = new char[*count+1];
    _data[0] = 0;
    LSTATUS status =  RegQueryValueExA(  hKey,  WSTR2A(name),  reserved,  type,  (LPBYTE)_data,  count );
    if(status == ERROR_SUCCESS){
        __string_ascii_to_unicode((WCHAR*)data,_data);
    }
    return status;
}

LSTATUS WINE_WINAPI RegQueryValueExA( HKEY hKey, LPCSTR name, LPDWORD reserved, LPDWORD type,
                                     LPBYTE data, LPDWORD count )
{
    LSTATUS status = ERROR_SUCCESS;
    XMLTag*	key = convert_winkey_to_tag(hKey);

    if(NULL == name || 0 == name[0]) name = sz_default_val_a;

    //    assert(type == 1);
    XMLTag* valkey = key->GetEmbeddedTagByNameAndAttr(sz_val_key,sz_val_key_attr_name,name);
    if(!valkey){
        status = ERROR_FILE_NOT_FOUND;
    } else{
        *type = REG_SZ;
        strcpy((char*)data,valkey->GetValue());
        *count = strlen((char*)data)+1;
    }

    TRACE(" hKey:%p, name:%s, data:%s, status:%ld\n",hKey,name,(char*)data, (long)status);
    return status;
}

LSTATUS WINE_WINAPI RegQueryValueW( HKEY hKey, LPCWSTR name, LPWSTR data, LPLONG count ){
    DWORD uValueType;
    return RegQueryValueExW(hKey,name,0,&uValueType,(LPBYTE)data,(LPDWORD)count);

}

LSTATUS WINE_WINAPI RegQueryValueA( HKEY hKey, LPCSTR name, LPSTR data, LPLONG count ){
    DWORD uValueType;
    return RegQueryValueExA(hKey,name,0,&uValueType,(LPBYTE)data,(LPDWORD)count);
}


LSTATUS   WINAPI RegDeleteValueW(HKEY hKey,LPCWSTR name){
    return RegDeleteValueA(hKey,WSTR2A(name));
}

LSTATUS   WINAPI RegDeleteValueA(HKEY hKey,LPCSTR name){
    LSTATUS status = ERROR_SUCCESS;
    XMLTag*	key = convert_winkey_to_tag(hKey);

    if(NULL == name || 0 == name[0]) name = sz_default_val_a;

    //    assert(type == 1);
    XMLTag* valkey = key->GetEmbeddedTagByNameAndAttr(sz_val_key,sz_val_key_attr_name,name);
    if(!valkey){
        status = ERROR_FILE_NOT_FOUND;
    } else{
        key->RemoveEmbeddedTag(valkey);
        _xmlparser_dirty_flag = 1;
    }

    TRACE(" hKey:%p, name:%s, status:%ld\n",hKey,name,(long)status);
    return status;
}


LSTATUS WINE_WINAPI RegDeleteTreeW(HKEY hKey, LPCWSTR lpszSubKey)
{
    return RegDeleteTreeA(hKey,WSTR2A(lpszSubKey));
}


LSTATUS WINE_WINAPI RegDeleteTreeA(HKEY hKey, LPCSTR lpszSubKey)
{
    LSTATUS status = ERROR_SUCCESS;
    XMLTag*	key = convert_winkey_to_tag(hKey);

    XMLTag* subKey = key->GetEmbeddedTagByName(lpszSubKey);
    if(subKey){
        key->RemoveEmbeddedTag(subKey);
        _xmlparser_dirty_flag = 1;
    }

    TRACE(" hKey:%p, lpszSubKey:%s, status:%ld\n",hKey,lpszSubKey,(long)status);
    return status;
}




#endif //REGISTRERY_IMPLEMENT_XML
