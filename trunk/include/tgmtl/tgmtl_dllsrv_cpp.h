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

#include "tgmtl_reg_cpp.h"
#include "OSAtomic.h"


 
static LONG g_cLocks = 0;
void ModuleAddRef(void)
{
	os_atomic_add(&g_cLocks,1);
}

void ModuleRelease(void)
{
	os_atomic_sub(&g_cLocks,1);
}

BOOL ModuleIsStopping(void)
{
	return FALSE;
}

BOOL ModuleIsIdle(void)
{
	return g_cLocks == 0;
}

extern regsvr_coclass coclass_list[];

#include "tgmtl_reg.h"

TGMTL_DLLENTRY_API DllCanUnloadNow(void)
{
	return ModuleIsIdle() ? S_OK : S_FALSE;
}


TGMTL_DLLENTRY_API DllRegisterServer(void)
{
	HRESULT hr;

	hr = register_coclasses(coclass_list);
	return hr;
}


TGMTL_DLLENTRY_API DllUnregisterServer(void)
{
	HRESULT hr;

	hr = unregister_coclasses(coclass_list);

	return hr;
}

