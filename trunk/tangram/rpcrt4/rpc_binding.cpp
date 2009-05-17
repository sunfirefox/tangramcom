/*
* RPC binding API
*
* Copyright 2001 Ove Kåven, TransGaming Technologies
* Copyright 2003 Mike Hearn
* Copyright 2004 Filip Navara
* Copyright 2006 CodeWeavers
* Copyright 2009 Jokul for Tranzda
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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "windef.h"
#include "winbase.h"
#include "winnls.h"
#include "winerror.h"
#include "winternl.h"
#include "wine/unicode.h"

#include "rpc.h"
#include "rpcndr.h"

#include "tgmlog.h"


#include "rpc_binding.h"
#include "rpc_assoc.h"

TGMLOG_DEFAULT_DEBUG_CHANNEL(rpc);

extern "C" WINADVAPI INT WINAPI agr_strncpy_ctow(LPWSTR dst, LPCSTR src ,int n);
extern "C" WINADVAPI INT WINAPI agr_strncpy_wtoc(LPSTR dst, LPCWSTR src ,int n);

LPSTR RPCRT4_strndupA(LPCSTR src, INT slen)
{
    DWORD len;
    LPSTR s;
    if (!src) return NULL;
    if (slen == -1) slen = strlen(src);
    len = slen;
    s = (LPSTR)HeapAlloc(GetProcessHeap(), 0, len+1);
    memcpy(s, src, len);
    s[len] = 0;
    return s;
}

LPSTR RPCRT4_strdupWtoA(LPCWSTR src)
{
    DWORD len;
    LPSTR s;
    if (!src) return NULL;
    //len = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
    len = lstrlenW(src)+1;
    s = (LPSTR)HeapAlloc(GetProcessHeap(), 0, len);
    //WideCharToMultiByte(CP_ACP, 0, src, -1, s, len, NULL, NULL);
    agr_strncpy_wtoc(s,src,len);
    return s;
}

LPWSTR RPCRT4_strdupAtoW(LPCSTR src)
{
    DWORD len;
    LPWSTR s;
    if (!src) return NULL;
    //len = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
    len = strlen(src)+1;
    s = (LPWSTR)HeapAlloc(GetProcessHeap(), 0, len*sizeof(WCHAR));
    //MultiByteToWideChar(CP_ACP, 0, src, -1, s, len);
    agr_strncpy_ctow(s,src,len);
    return s;
}

static LPWSTR RPCRT4_strndupAtoW(LPCSTR src, INT slen)
{
    DWORD len;
    LPWSTR s;
    if (!src) return NULL;
    //len = MultiByteToWideChar(CP_ACP, 0, src, slen, NULL, 0);
    len =  strlen(src)+1;
    s = (LPWSTR)HeapAlloc(GetProcessHeap(), 0, len*sizeof(WCHAR));
    //MultiByteToWideChar(CP_ACP, 0, src, slen, s, len);
    agr_strncpy_ctow(s,src,len);
    return s;
}

LPWSTR RPCRT4_strndupW(LPCWSTR src, INT slen)
{
    DWORD len;
    LPWSTR s;
    if (!src) return NULL;
    if (slen == -1) slen = strlenW(src);
    len = slen;
    s = (LPWSTR)HeapAlloc(GetProcessHeap(), 0, (len+1)*sizeof(WCHAR));
    memcpy(s, src, len*sizeof(WCHAR));
    s[len] = 0;
    return s;
}

void RPCRT4_strfree(LPSTR src)
{
    HeapFree(GetProcessHeap(), 0, src);
}

static RPC_STATUS RPCRT4_AllocBinding(RpcBinding** Binding, BOOL server)
{
    RpcBinding* NewBinding;

    NewBinding = (RpcBinding*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(RpcBinding));
    NewBinding->refs = 1;
    NewBinding->server = server;

    *Binding = NewBinding;

    return RPC_S_OK;
}

static RPC_STATUS RPCRT4_CreateBindingA(RpcBinding** Binding, BOOL server, LPCSTR Protseq)
{
    RpcBinding* NewBinding;

    RPCRT4_AllocBinding(&NewBinding, server);
    NewBinding->Protseq = RPCRT4_strdupA(Protseq);

    TRACE("binding: %p\n", NewBinding);
    *Binding = NewBinding;

    return RPC_S_OK;
}

static RPC_STATUS RPCRT4_CreateBindingW(RpcBinding** Binding, BOOL server, LPCWSTR Protseq)
{
    RpcBinding* NewBinding;

    RPCRT4_AllocBinding(&NewBinding, server);
    NewBinding->Protseq = RPCRT4_strdupWtoA(Protseq);

    TRACE("binding: %p\n", NewBinding);
    *Binding = NewBinding;

    return RPC_S_OK;
}

static RPC_STATUS RPCRT4_CompleteBindingA(RpcBinding* Binding, LPCSTR NetworkAddr,
                                          LPCSTR Endpoint, LPCSTR NetworkOptions)
{
    RPC_STATUS status;

    FB_TRACE("> (RpcBinding == ^%p, NetworkAddr == %s, EndPoint == %s, NetworkOptions == %s)\n", Binding,
        TGMLOG_STR_A(NetworkAddr), TGMLOG_STR_A(Endpoint), TGMLOG_STR_A(NetworkOptions));

    RPCRT4_strfree(Binding->NetworkAddr);
    Binding->NetworkAddr = RPCRT4_strdupA(NetworkAddr);
    RPCRT4_strfree(Binding->Endpoint);
    Binding->Endpoint = RPCRT4_strdupA(Endpoint);
    HeapFree(GetProcessHeap(), 0, Binding->NetworkOptions);
    Binding->NetworkOptions = RPCRT4_strdupAtoW(NetworkOptions);

    /* only attempt to get an association if the binding is complete */
    if (Endpoint && Endpoint[0] != '\0')
    {
        status = RPCRT4_GetAssociation(Binding->Protseq, Binding->NetworkAddr,
            Binding->Endpoint, Binding->NetworkOptions,
            &Binding->Assoc);
        if (status != RPC_S_OK)
        {
            FE_WARN("< status:%ld \n",status);
            return status;
        }
    }

    FE_TRACE("< \n");
    return RPC_S_OK;
}

static RPC_STATUS RPCRT4_CompleteBindingW(RpcBinding* Binding, LPCWSTR NetworkAddr,
                                          LPCWSTR Endpoint, LPCWSTR NetworkOptions)
{
    RPC_STATUS status;

    FB_TRACE("> (RpcBinding == ^%p, NetworkAddr == %s, EndPoint == %s, NetworkOptions == %s)\n", Binding, 
        TGMLOG_STR_W(NetworkAddr), TGMLOG_STR_W(Endpoint), TGMLOG_STR_W(NetworkOptions));

    RPCRT4_strfree(Binding->NetworkAddr);
    Binding->NetworkAddr = RPCRT4_strdupWtoA(NetworkAddr);
    RPCRT4_strfree(Binding->Endpoint);
    Binding->Endpoint = RPCRT4_strdupWtoA(Endpoint);
    HeapFree(GetProcessHeap(), 0, Binding->NetworkOptions);
    Binding->NetworkOptions = RPCRT4_strdupW(NetworkOptions);

    /* only attempt to get an association if the binding is complete */
    if (Endpoint && Endpoint[0] != '\0')
    {
        status = RPCRT4_GetAssociation(Binding->Protseq, Binding->NetworkAddr,
            Binding->Endpoint, Binding->NetworkOptions,
            &Binding->Assoc);
        if (status != RPC_S_OK){
            FE_WARN("< status:%ld \n",status);
            return status;
        }
    }

    FE_TRACE("< \n");
    return RPC_S_OK;
}

//RPC_STATUS RPCRT4_ResolveBinding(RpcBinding* Binding, LPCSTR Endpoint)
//{
//  RPC_STATUS status;
//
//  TRACE("(RpcBinding == ^%p, EndPoint == \"%s\"\n", Binding, Endpoint);
//
//  RPCRT4_strfree(Binding->Endpoint);
//  Binding->Endpoint = RPCRT4_strdupA(Endpoint);
//
//  if (Binding->Assoc) RpcAssoc_Release(Binding->Assoc);
//  Binding->Assoc = NULL;
//  status = RPCRT4_GetAssociation(Binding->Protseq, Binding->NetworkAddr,
//                                 Binding->Endpoint, Binding->NetworkOptions,
//                                 &Binding->Assoc);
//  if (status != RPC_S_OK)
//      return status;
//
//  return RPC_S_OK;
//}

RPC_STATUS RPCRT4_SetBindingObject(RpcBinding* Binding, const UUID* ObjectUuid)
{
    TRACE("(*RpcBinding == ^%p, UUID == %s)\n", Binding, TGMLOG_STR_GUID(ObjectUuid)); 
    if (ObjectUuid) Binding->ObjectUuid = *ObjectUuid;
    else UuidCreateNil(&Binding->ObjectUuid);
    return RPC_S_OK;
}

//RPC_STATUS RPCRT4_MakeBinding(RpcBinding** Binding, RpcConnection* Connection)
//{
//  RpcBinding* NewBinding;
//  TRACE("(RpcBinding == ^%p, Connection == ^%p)\n", Binding, Connection);
//
//  RPCRT4_AllocBinding(&NewBinding, Connection->server);
//  NewBinding->Protseq = NULL;
//  NewBinding->NetworkAddr = RPCRT4_strdupA(Connection->NetworkAddr);
//  NewBinding->Endpoint = RPCRT4_strdupA(Connection->Endpoint);
//  NewBinding->FromConn = Connection;
//
//  TRACE("binding: %p\n", NewBinding);
//  *Binding = NewBinding;
//
//  return RPC_S_OK;
//}

void RPCRT4_AddRefBinding(RpcBinding* Binding)
{
    InterlockedIncrement(&Binding->refs);
}

RPC_STATUS RPCRT4_ReleaseBinding(RpcBinding* Binding)
{
    if (InterlockedDecrement(&Binding->refs))
        return RPC_S_OK;

    TRACE("binding: %p\n", Binding);
    if (Binding->Assoc) RpcAssoc_Release(Binding->Assoc);
    RPCRT4_strfree(Binding->Endpoint);
    RPCRT4_strfree(Binding->NetworkAddr);
    RPCRT4_strfree(Binding->Protseq);
    HeapFree(GetProcessHeap(), 0, Binding->NetworkOptions);

    HeapFree(GetProcessHeap(), 0, Binding);
    return RPC_S_OK;
}

RPC_STATUS RPCRT4_OpenBinding(RpcBinding* Binding, RpcConnection** Connection,
                              const RPC_SYNTAX_IDENTIFIER *TransferSyntax,
                              const RPC_SYNTAX_IDENTIFIER *InterfaceId)
{
    FB_TRACE("> (Binding == ^%p)\n", Binding);

    if (!Binding->server) {
        FE_TRACE("< \n");
        return RpcAssoc_GetClientConnection(Binding->Assoc, InterfaceId,
            TransferSyntax, Binding->AuthInfo, Binding->QOS, Connection);
    } else {
        /* we already have a connection with acceptable binding, so use it. RPCRT4_MakeBinding*/
        if (Binding->FromConn) {//
            *Connection = Binding->FromConn;
            FE_TRACE("< \n");
            return RPC_S_OK;
        } else {
            FE_ERR("< no connection in binding\n");
            return RPC_S_INTERNAL_ERROR;
        }
    }
}

RPC_STATUS RPCRT4_CloseBinding(RpcBinding* Binding, RpcConnection* Connection)
{
    RPC_STATUS status =RPC_E_FAIL;
    FB_TRACE("> (Binding == ^%p)\n", Binding);

    if (!Connection) return RPC_S_OK;
    if (Binding->server) {
        /* don't destroy a connection that is cached in the binding */
        if (Binding->FromConn == Connection){
            FE_TRACE("< Binding->server \n");
            return RPC_S_OK;
        }
        status = RPCRT4_DestroyConnection(Connection);
        FE_TRACE("< Binding->server status:%ld\n",status);
        return status;
    }
    else {
        RpcAssoc_ReleaseIdleConnection(Binding->Assoc, Connection);
        FE_TRACE("< client \n");
        return RPC_S_OK;
    }
}

static LPSTR RPCRT4_strconcatA(LPSTR dst, LPCSTR src)
{
    DWORD len = strlen(dst), slen = strlen(src);
    LPSTR ndst = (LPSTR)HeapReAlloc(GetProcessHeap(), 0, dst, (len+slen+2)*sizeof(CHAR));
    if (!ndst)
    {
        HeapFree(GetProcessHeap(), 0, dst);
        return NULL;
    }
    ndst[len] = ',';
    memcpy(ndst+len+1, src, slen+1);
    return ndst;
}

static LPWSTR RPCRT4_strconcatW(LPWSTR dst, LPCWSTR src)
{
    DWORD len = strlenW(dst), slen = strlenW(src);
    LPWSTR ndst = (LPWSTR)HeapReAlloc(GetProcessHeap(), 0, dst, (len+slen+2)*sizeof(WCHAR));
    if (!ndst) 
    {
        HeapFree(GetProcessHeap(), 0, dst);
        return NULL;
    }
    ndst[len] = ',';
    memcpy(ndst+len+1, src, (slen+1)*sizeof(WCHAR));
    return ndst;
}

/* Copies the escaped version of a component into a string binding.
* Note: doesn't nul-terminate the string */
static RPC_CSTR escape_string_binding_component(RPC_CSTR string_binding,
                                                const unsigned char *component)
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

static RPC_WSTR escape_string_binding_componentW(RPC_WSTR string_binding,
                                                 const WCHAR *component)
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

static const unsigned char *string_binding_find_delimiter(
    const unsigned char *string_binding, unsigned char delim)
{
    const unsigned char *next;
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

static const WCHAR *string_binding_find_delimiterW(
    const WCHAR *string_binding, WCHAR delim)
{
    const WCHAR *next;
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

static RPC_CSTR unescape_string_binding_component(
    const unsigned char *string_binding, int len)
{
    RPC_CSTR component, p;

    if (len == -1) len = strlen((const char *)string_binding);

    component = (RPC_CSTR)HeapAlloc(GetProcessHeap(), 0, (len + 1) * sizeof(*component));
    if (!component) return NULL;
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

static RPC_WSTR unescape_string_binding_componentW(
    const WCHAR *string_binding, int len)
{
    RPC_WSTR component, p;

    if (len == -1) len = strlenW(string_binding);

    component = (RPC_WSTR)HeapAlloc(GetProcessHeap(), 0, (len + 1) * sizeof(*component));
    if (!component) return NULL;
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

/***********************************************************************
*             RpcStringBindingComposeA (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcStringBindingComposeA(RPC_CSTR ObjUuid, RPC_CSTR Protseq,
                                           RPC_CSTR NetworkAddr, RPC_CSTR Endpoint,
                                           RPC_CSTR Options, RPC_CSTR *StringBinding )
{
    DWORD len = 1;
    RPC_CSTR data;

    FB_TRACE("> (%s,%s,%s,%s,%s,%p)\n",
        TGMLOG_STR_A( (char*)ObjUuid ), TGMLOG_STR_A( (char*)Protseq ),
        TGMLOG_STR_A( (char*)NetworkAddr ), TGMLOG_STR_A( (char*)Endpoint ),
        TGMLOG_STR_A( (char*)Options ), StringBinding );

    /* overestimate for each component for escaping of delimiters */
    if (ObjUuid && *ObjUuid) len += strlen((char*)ObjUuid) * 2 + 1;
    if (Protseq && *Protseq) len += strlen((char*)Protseq) * 2 + 1;
    if (NetworkAddr && *NetworkAddr) len += strlen((char*)NetworkAddr) * 2;
    if (Endpoint && *Endpoint) len += strlen((char*)Endpoint) * 2 + 2;
    if (Options && *Options) len += strlen((char*)Options) * 2 + 2;

    data = (RPC_CSTR)HeapAlloc(GetProcessHeap(), 0, len);
    *StringBinding = data;

    if (ObjUuid && *ObjUuid) {
        data = escape_string_binding_component(data, ObjUuid);
        *data++ = '@';
    }
    if (Protseq && *Protseq) {
        data = escape_string_binding_component(data, Protseq);
        *data++ = ':';
    }
    if (NetworkAddr && *NetworkAddr)
        data = escape_string_binding_component(data, NetworkAddr);

    if ((Endpoint && *Endpoint) ||
        (Options && *Options)) {
            *data++ = '[';
            if (Endpoint && *Endpoint) {
                data = escape_string_binding_component(data, Endpoint);
                if (Options && *Options) *data++ = ',';
            }
            if (Options && *Options) {
                data = escape_string_binding_component(data, Options);
            }
            *data++ = ']';
        }
        *data = 0;

        FE_TRACE("< \n");
        return RPC_S_OK;
}

/***********************************************************************
*             RpcStringBindingComposeW (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcStringBindingComposeW( RPC_WSTR ObjUuid, RPC_WSTR Protseq,
                                           RPC_WSTR NetworkAddr, RPC_WSTR Endpoint,
                                           RPC_WSTR Options, RPC_WSTR* StringBinding )
{
    DWORD len = 1;
    RPC_WSTR data;

    FB_TRACE("> (%s,%s,%s,%s,%s,%p)\n",
        TGMLOG_STR_W( ObjUuid ), TGMLOG_STR_W( Protseq ),
        TGMLOG_STR_W( NetworkAddr ), TGMLOG_STR_W( Endpoint ),
        TGMLOG_STR_W( Options ), StringBinding);

    /* overestimate for each component for escaping of delimiters */
    if (ObjUuid && *ObjUuid) len += strlenW(ObjUuid) * 2 + 1;
    if (Protseq && *Protseq) len += strlenW(Protseq) * 2 + 1;
    if (NetworkAddr && *NetworkAddr) len += strlenW(NetworkAddr) * 2;
    if (Endpoint && *Endpoint) len += strlenW(Endpoint) * 2 + 2;
    if (Options && *Options) len += strlenW(Options) * 2 + 2;

    data = (RPC_WSTR)HeapAlloc(GetProcessHeap(), 0, len*sizeof(WCHAR));
    *StringBinding = data;

    if (ObjUuid && *ObjUuid) {
        data = escape_string_binding_componentW(data, ObjUuid);
        *data++ = '@';
    }
    if (Protseq && *Protseq) {
        data = escape_string_binding_componentW(data, Protseq);
        *data++ = ':';
    }
    if (NetworkAddr && *NetworkAddr) {
        data = escape_string_binding_componentW(data, NetworkAddr);
    }
    if ((Endpoint && *Endpoint) ||
        (Options && *Options)) {
            *data++ = '[';
            if (Endpoint && *Endpoint) {
                data = escape_string_binding_componentW(data, Endpoint);
                if (Options && *Options) *data++ = ',';
            }
            if (Options && *Options) {
                data = escape_string_binding_componentW(data, Options);
            }
            *data++ = ']';
        }
        *data = 0;

        FE_TRACE("< \n");
        return RPC_S_OK;
}


/***********************************************************************
*             RpcStringBindingParseA (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcStringBindingParseA( RPC_CSTR StringBinding, RPC_CSTR *ObjUuid,
                                         RPC_CSTR *Protseq, RPC_CSTR *NetworkAddr,
                                         RPC_CSTR *Endpoint, RPC_CSTR *Options)
{
    const unsigned char *data, *next;
    static const char ep_opt[] = "endpoint=";
    BOOL endpoint_already_found = FALSE;

    FB_TRACE("> (%s,%p,%p,%p,%p,%p)\n", TGMLOG_STR_A((char*)StringBinding),
        ObjUuid, Protseq, NetworkAddr, Endpoint, Options);

    if (ObjUuid) *ObjUuid = NULL;
    if (Protseq) *Protseq = NULL;
    if (NetworkAddr) *NetworkAddr = NULL;
    if (Endpoint) *Endpoint = NULL;
    if (Options) *Options = NULL;

    data = StringBinding;

    next = string_binding_find_delimiter(data, '@');
    if (next) {
        UUID uuid;
        RPC_STATUS status;
        RPC_CSTR str_uuid = unescape_string_binding_component(data, next - data);
        status = UuidFromStringA(str_uuid, &uuid);
        if (status != RPC_S_OK) {
            HeapFree(GetProcessHeap(), 0, str_uuid);
            return status;
        }
        if (ObjUuid)
            *ObjUuid = str_uuid;
        else
            HeapFree(GetProcessHeap(), 0, str_uuid);
        data = next+1;
    }

    next = string_binding_find_delimiter(data, ':');
    if (next) {
        if (Protseq) *Protseq = unescape_string_binding_component(data, next - data);
        data = next+1;
    }

    next = string_binding_find_delimiter(data, '[');
    if (next) {
        const unsigned char *close;
        RPC_CSTR opt;

        if (NetworkAddr) *NetworkAddr = unescape_string_binding_component(data, next - data);
        data = next+1;
        close = string_binding_find_delimiter(data, ']');
        if (!close) goto fail;

        /* tokenize options */
        while (data < close) {
            next = string_binding_find_delimiter(data, ',');
            if (!next || next > close) next = close;
            /* FIXME: this is kind of inefficient */
            opt = unescape_string_binding_component(data, next - data);
            data = next+1;

            /* parse option */
            next = string_binding_find_delimiter(opt, '=');
            if (!next) {
                /* not an option, must be an endpoint */
                if (endpoint_already_found) goto fail;
                if (Endpoint) *Endpoint = opt;
                else HeapFree(GetProcessHeap(), 0, opt);
                endpoint_already_found = TRUE;
            } else {
                if (strncmp((const char *)opt, ep_opt, strlen(ep_opt)) == 0) {
                    /* endpoint option */
                    if (endpoint_already_found) goto fail;
                    if (Endpoint) *Endpoint = unescape_string_binding_component(next+1, -1);
                    HeapFree(GetProcessHeap(), 0, opt);
                    endpoint_already_found = TRUE;
                } else {
                    /* network option */
                    if (Options) {
                        if (*Options) {
                            /* FIXME: this is kind of inefficient */
                            *Options = (unsigned char*) RPCRT4_strconcatA( (char*)*Options, (char *)opt);
                            HeapFree(GetProcessHeap(), 0, opt);
                        } else
                            *Options = opt;
                    } else
                        HeapFree(GetProcessHeap(), 0, opt);
                }
            }
        }

        data = close+1;
        if (*data) goto fail;
    }
    else if (NetworkAddr) 
        *NetworkAddr = unescape_string_binding_component(data, -1);

    FE_TRACE("< \n");
    return RPC_S_OK;

fail:
    if (ObjUuid) RpcStringFreeA((unsigned char**)ObjUuid);
    if (Protseq) RpcStringFreeA((unsigned char**)Protseq);
    if (NetworkAddr) RpcStringFreeA((unsigned char**)NetworkAddr);
    if (Endpoint) RpcStringFreeA((unsigned char**)Endpoint);
    if (Options) RpcStringFreeA((unsigned char**)Options);
    FE_WARN("< RPC_S_INVALID_STRING_BINDING \n");
    return RPC_S_INVALID_STRING_BINDING;
}

/***********************************************************************
*             RpcStringBindingParseW (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcStringBindingParseW( RPC_WSTR StringBinding, RPC_WSTR *ObjUuid,
                                         RPC_WSTR *Protseq, RPC_WSTR *NetworkAddr,
                                         RPC_WSTR *Endpoint, RPC_WSTR *Options)
{
    const WCHAR *data, *next;
    static const WCHAR ep_opt[] = {'e','n','d','p','o','i','n','t','=',0};
    BOOL endpoint_already_found = FALSE;

    FB_TRACE("> (%s,%p,%p,%p,%p,%p)\n", TGMLOG_STR_W(StringBinding),
        ObjUuid, Protseq, NetworkAddr, Endpoint, Options);

    if (ObjUuid) *ObjUuid = NULL;
    if (Protseq) *Protseq = NULL;
    if (NetworkAddr) *NetworkAddr = NULL;
    if (Endpoint) *Endpoint = NULL;
    if (Options) *Options = NULL;

    data = StringBinding;

    next = string_binding_find_delimiterW(data, '@');
    if (next) {
        UUID uuid;
        RPC_STATUS status;
        RPC_WSTR str_uuid = unescape_string_binding_componentW(data, next - data);
        status = UuidFromStringW(str_uuid, &uuid);
        if (status != RPC_S_OK) {
            HeapFree(GetProcessHeap(), 0, str_uuid);
            FE_WARN("< status:%ld\n",status);
            return status;
        }
        if (ObjUuid)
            *ObjUuid = str_uuid;
        else
            HeapFree(GetProcessHeap(), 0, str_uuid);
        data = next+1;
    }

    next = string_binding_find_delimiterW(data, ':');
    if (next) {
        if (Protseq) *Protseq = unescape_string_binding_componentW(data, next - data);
        data = next+1;
    }

    next = string_binding_find_delimiterW(data, '[');
    if (next) {
        const WCHAR *close;
        RPC_WSTR opt;

        if (NetworkAddr) *NetworkAddr = unescape_string_binding_componentW(data, next - data);
        data = next+1;
        close = string_binding_find_delimiterW(data, ']');
        if (!close) goto fail;

        /* tokenize options */
        while (data < close) {
            next = string_binding_find_delimiterW(data, ',');
            if (!next || next > close) next = close;
            /* FIXME: this is kind of inefficient */
            opt = unescape_string_binding_componentW(data, next - data);
            data = next+1;

            /* parse option */
            next = string_binding_find_delimiterW(opt, '=');
            if (!next) {
                /* not an option, must be an endpoint */
                if (endpoint_already_found) goto fail;
                if (Endpoint) *Endpoint = opt;
                else HeapFree(GetProcessHeap(), 0, opt);
                endpoint_already_found = TRUE;
            } else {
                if (strncmpW(opt, ep_opt, strlenW(ep_opt)) == 0) {
                    /* endpoint option */
                    if (endpoint_already_found) goto fail;
                    if (Endpoint) *Endpoint = unescape_string_binding_componentW(next+1, -1);
                    HeapFree(GetProcessHeap(), 0, opt);
                    endpoint_already_found = TRUE;
                } else {
                    /* network option */
                    if (Options) {
                        if (*Options) {
                            /* FIXME: this is kind of inefficient */
                            *Options = RPCRT4_strconcatW(*Options, opt);
                            HeapFree(GetProcessHeap(), 0, opt);
                        } else
                            *Options = opt;
                    } else
                        HeapFree(GetProcessHeap(), 0, opt);
                }
            }
        }

        data = close+1;
        if (*data) goto fail;
    } else if (NetworkAddr) 
        *NetworkAddr = unescape_string_binding_componentW(data, -1);

    FE_TRACE("< \n");
    return RPC_S_OK;

fail:
    if (ObjUuid) RpcStringFreeW(ObjUuid);
    if (Protseq) RpcStringFreeW(Protseq);
    if (NetworkAddr) RpcStringFreeW(NetworkAddr);
    if (Endpoint) RpcStringFreeW(Endpoint);
    if (Options) RpcStringFreeW(Options);

    FE_WARN("< RPC_S_INVALID_STRING_BINDING \n");
    return RPC_S_INVALID_STRING_BINDING;
}



/***********************************************************************
*             RpcBindingVectorFree (RPCRT4.@)
*/
//RPC_STATUS WINAPI RpcBindingVectorFree( RPC_BINDING_VECTOR** BindingVector )
//{
//  RPC_STATUS status;
//  unsigned long c;
//
//  TRACE("(%p)\n", BindingVector);
//  for (c=0; c<(*BindingVector)->Count; c++) {
//    status = RpcBindingFree(&(*BindingVector)->BindingH[c]);
//  }
//  HeapFree(GetProcessHeap(), 0, *BindingVector);
//  *BindingVector = NULL;
//  return RPC_S_OK;
//}

/***********************************************************************
*             RpcBindingInqObject (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcBindingInqObject( RPC_BINDING_HANDLE Binding, UUID* ObjectUuid )
{
    RpcBinding* bind = (RpcBinding*)Binding;

    TRACE("(%p,%p) = %s\n", Binding, ObjectUuid, TGMLOG_STR_GUID(&bind->ObjectUuid));
    *ObjectUuid = bind->ObjectUuid;
    return RPC_S_OK;
}

/***********************************************************************
*             RpcBindingSetObject (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcBindingSetObject( RPC_BINDING_HANDLE Binding, UUID* ObjectUuid )
{
    RpcBinding* bind = (RpcBinding*)Binding;

    TRACE("(%p,%s)\n", Binding, TGMLOG_STR_GUID(ObjectUuid));
    if (bind->server) return RPC_S_WRONG_KIND_OF_BINDING;
    return RPCRT4_SetBindingObject(bind, ObjectUuid);
}

/***********************************************************************
*             RpcBindingFromStringBindingA (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcBindingFromStringBindingA( RPC_CSTR StringBinding, RPC_BINDING_HANDLE* Binding )
{
    RPC_STATUS ret;
    RpcBinding* bind = NULL;
    RPC_CSTR ObjectUuid, Protseq, NetworkAddr, Endpoint, Options;
    UUID Uuid;

    FB_TRACE("> (%s,%p)\n", TGMLOG_STR_A((char*)StringBinding), Binding);

    ret = RpcStringBindingParseA(StringBinding, &ObjectUuid, &Protseq,
        &NetworkAddr, &Endpoint, &Options);
    if (ret != RPC_S_OK) return ret;

    ret = UuidFromStringA(ObjectUuid, &Uuid);

    if (ret == RPC_S_OK)
        ret = RPCRT4_CreateBindingA(&bind, FALSE, (char*)Protseq);
    if (ret != RPC_S_OK) return ret;
    ret = RPCRT4_SetBindingObject(bind, &Uuid);
    if (ret == RPC_S_OK)
        ret = RPCRT4_CompleteBindingA(bind, (char*)NetworkAddr, (char*)Endpoint, (char*)Options);

    RpcStringFreeA((unsigned char**)&Options);
    RpcStringFreeA((unsigned char**)&Endpoint);
    RpcStringFreeA((unsigned char**)&NetworkAddr);
    RpcStringFreeA((unsigned char**)&Protseq);
    RpcStringFreeA((unsigned char**)&ObjectUuid);

    if (ret == RPC_S_OK) 
        *Binding = (RPC_BINDING_HANDLE)bind;
    else 
        RPCRT4_ReleaseBinding(bind);

    FE_TRACE("< \n");
    return ret;
}

/***********************************************************************
*             RpcBindingFromStringBindingW (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcBindingFromStringBindingW( RPC_WSTR StringBinding, RPC_BINDING_HANDLE* Binding )
{
    RPC_STATUS ret;
    RpcBinding* bind = NULL;
    RPC_WSTR ObjectUuid, Protseq, NetworkAddr, Endpoint, Options;
    UUID Uuid;

    FB_TRACE("> (%s,%p)\n", TGMLOG_STR_W(StringBinding), Binding);

    ret = RpcStringBindingParseW(StringBinding, &ObjectUuid, &Protseq,
        &NetworkAddr, &Endpoint, &Options);
    if (ret != RPC_S_OK) return ret;

    ret = UuidFromStringW(ObjectUuid, &Uuid);

    if (ret == RPC_S_OK)
        ret = RPCRT4_CreateBindingW(&bind, FALSE, Protseq);
    if (ret != RPC_S_OK) return ret;
    ret = RPCRT4_SetBindingObject(bind, &Uuid);
    if (ret == RPC_S_OK)
        ret = RPCRT4_CompleteBindingW(bind, NetworkAddr, Endpoint, Options);

    RpcStringFreeW(&Options);
    RpcStringFreeW(&Endpoint);
    RpcStringFreeW(&NetworkAddr);
    RpcStringFreeW(&Protseq);
    RpcStringFreeW(&ObjectUuid);

    if (ret == RPC_S_OK)
        *Binding = (RPC_BINDING_HANDLE)bind;
    else
        RPCRT4_ReleaseBinding(bind);
    
    FE_TRACE("< \n");
    return ret;
}





/***********************************************************************
*             RpcBindingCopy (RPCRT4.@)
*/
RPC_STATUS RPC_ENTRY RpcBindingCopy(
                                    RPC_BINDING_HANDLE SourceBinding,
                                    RPC_BINDING_HANDLE* DestinationBinding)
{
    RpcBinding *DestBinding;
    RpcBinding *SrcBinding = (RpcBinding*)SourceBinding;
    RPC_STATUS status;

    TRACE("(%p, %p)\n", SourceBinding, DestinationBinding);

    status = RPCRT4_AllocBinding(&DestBinding, SrcBinding->server);
    if (status != RPC_S_OK) return status;

    DestBinding->ObjectUuid = SrcBinding->ObjectUuid;
    //DestBinding->BlockingFn = SrcBinding->BlockingFn;
    DestBinding->Protseq = RPCRT4_strndupA(SrcBinding->Protseq, -1);
    DestBinding->NetworkAddr = RPCRT4_strndupA(SrcBinding->NetworkAddr, -1);
    DestBinding->Endpoint = RPCRT4_strndupA(SrcBinding->Endpoint, -1);
    DestBinding->NetworkOptions = RPCRT4_strdupW(SrcBinding->NetworkOptions);
    if (SrcBinding->Assoc) SrcBinding->Assoc->refs++;
    DestBinding->Assoc = SrcBinding->Assoc;



    *DestinationBinding = DestBinding;
    return RPC_S_OK;
}

///***********************************************************************
// *             RpcBindingReset (RPCRT4.@)
// */
//RPC_STATUS RPC_ENTRY RpcBindingReset(RPC_BINDING_HANDLE Binding)
//{
//    RpcBinding *bind = (RpcBinding *)Binding;
//
//    TRACE("(%p)\n", Binding);
//
//    RPCRT4_strfree(bind->Endpoint);
//    bind->Endpoint = NULL;
//    if (bind->Assoc) RpcAssoc_Release(bind->Assoc);
//    bind->Assoc = NULL;
//
//    return RPC_S_OK;
//}


/***********************************************************************
*             RpcBindingFree (RPCRT4.@)
*/
RPC_STATUS WINAPI RpcBindingFree( RPC_BINDING_HANDLE* Binding )
{
    RPC_STATUS status;
    TRACE("(%p) = %p\n", Binding, *Binding);
    status = RPCRT4_ReleaseBinding((RpcBinding*)*Binding);
    if (status == RPC_S_OK) *Binding = 0;
    return status;
}









