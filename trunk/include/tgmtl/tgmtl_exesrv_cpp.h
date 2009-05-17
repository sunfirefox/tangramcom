#ifndef tgmtl_exesrv_cpp_h
#define tgmtl_exesrv_cpp_h

/*
* tangram template librarym
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

#include "OSMutex.h"
#include "OSCond.h"

#define IMPLEMENT_MTAEXE_MODULE_ROUTINES()
static OSMutex g_heventmutex;
static OSCond	g_heventCond;
static bool	g_heventDone = false;
void ModuleAddRef(void)
{
	CoAddRefServerProcess();
}

void ModuleRelease(void)
{
	if (CoReleaseServerProcess() == 0){
		OSMutexLocker _locker(&g_heventmutex);
		g_heventDone = true;
		g_heventCond.Signal();
	}
}

BOOL ModuleIsStopping(void)
{
	OSMutexLocker _locker(&g_heventmutex);
	return g_heventDone;
}

void ModuleServerListen(void)
{
	OSMutexLocker _locker(&g_heventmutex);
	if(g_heventDone) return;
	g_heventCond.Wait(&g_heventmutex);
}
#endif
