/*
 * RPC endpoint mapper
 *
 * Copyright 2002 Greg Turner
 * Copyright 2001 Ove Kåven, TransGaming Technologies
 * Copyright 2008 Robert Shearman (for CodeWeavers)
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

#include "windef.h"
#include "winbase.h"
#include "winerror.h"

#include "rpc.h"

#include "tgmlog.h"
#include "wine/exception.h"

#include "rpc_binding.h"
#include "epm.h"
#include "epm_towers.h"

TGMLOG_DEFAULT_DEBUG_CHANNEL(ole);



void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t len)
{
    return HeapAlloc(GetProcessHeap(), 0, len);
}

void __RPC_USER MIDL_user_free(void __RPC_FAR * ptr)
{
    HeapFree(GetProcessHeap(), 0, ptr);
}
