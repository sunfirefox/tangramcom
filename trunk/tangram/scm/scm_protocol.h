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
#ifndef _SCM_PROTOCOL_H
#define _SCM_PROTOCOL_H

#include "tgmrpc_if.h"

//#define TANGRAMCOM_SCM_CONN_BINDING RPCBINGING_PROTSEQU_RPCCOM_TCP":127.0.0.1[7892]"

//#include "dcom.h"

enum
{
	API_INTERFACE_ISCMClsFactoryResloveService	= 0x1,
	API_INTERFACE_ISCMApartMentResloveService	= 0x2
};

#define SCM_GET_API_INTERFACE(APINO) ((APINO) >> 8)
#define SCM_GET_API_PROCNUM(APINO) ((APINO) >& 0xFF)
#define SCM_GET_API_NO(APIINTERFACE,APIPROCNUM) ((APIINTERFACE  << 8) + APIPROCNUM)

typedef struct Params_ISCMClsFactoryResloveService_Register
{
	typedef struct  
	{
		ULONG	apino;
		IID		clsid;
		unsigned char objref[1024];
		int		length;
	}InParams;

	typedef struct  
	{
		HRESULT result;
	}OutParams;

	static const int ProcNum = 1;
}Params_ISCMClsFactoryResloveService_Register;

typedef struct Params_ISCMClsFactoryResloveService_Unregister
{
	typedef struct  
	{
		ULONG	apino;
		IID		clsid;
	}InParams;

	typedef struct  
	{
		HRESULT result;
	}OutParams;

	static const int ProcNum = 2;
}Params_ISCMClsFactoryResloveService_Unregister;

typedef struct Params_ISCMClsFactoryResloveService_Query
{
	typedef struct  
	{
		ULONG	apino;
		IID		clsid;
	}InParams;

	typedef struct  
	{
		HRESULT result;
		unsigned char objref[1024];
		int		length;
	}OutParams;

	static const int ProcNum = 3;
}Params_ISCMClsFactoryResloveService_Query;

//////////////////////////////////////////////////////////////////////////
typedef struct Params_ISCMApartMentResloveService_Register
{
	typedef struct  
	{
		ULONG	apino;
		OXID	oxid;
		char	rrl[RPCBINGING_MAX_SIZE];
	}InParams;

	typedef struct  
	{
		HRESULT result;
		ULONG	cookie;
	}OutParams;

	static const int ProcNum = 1;
}Params_ISCMApartMentResloveService_Register;

typedef struct Params_ISCMApartMentResloveService_Unregister
{
	typedef struct  
	{
		ULONG	apino;
		OXID	oxid;
	}InParams;
	
	typedef struct  
	{
		HRESULT result;
	}OutParams;


	static const int ProcNum = 2;
}Params_ISCMApartMentResloveService_Unregister;

typedef struct Params_ISCMApartMentResloveService_Query
{
	typedef struct  
	{
		ULONG	apino;
		OXID	oxid;
	}InParams;

	typedef struct  
	{
		HRESULT result;
		char	rrl[RPCBINGING_MAX_SIZE];
	}OutParams;

	static const int ProcNum = 3;
}Params_ISCMApartMentResloveService_Query;

#endif 
