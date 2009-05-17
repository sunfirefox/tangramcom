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

#include "windows.h"
#include "scm_client.h"

#include "tgmtl/tgmtl.h"

COCLASS_LIST_BEGIN
COCLASS_LIST_ENTRY(CLSID_SCMClsFactoryResloveService,Proxy_ISCMClsFactoryResloveService)
COCLASS_LIST_ENTRY(CLSID_SCMApartMentResloveService,Proxy_ISCMApartMentResloveService)
COCLASS_LIST_END

COCLASS_MAP_BEGIN
COCLASS_MAP_ENTRY(CLSID_SCMClsFactoryResloveService,Proxy_ISCMClsFactoryResloveService)
COCLASS_MAP_ENTRY(CLSID_SCMApartMentResloveService,Proxy_ISCMApartMentResloveService)
COCLASS_MAP_END



#include "tgmtl/tgmtl_dllsrv_cpp.h"


#include "scm_api_i_c.inc"


