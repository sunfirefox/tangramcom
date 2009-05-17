/*
* tangram com projecct confiure
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

#ifndef GLOBAL_CONFIG_HHH
#define GLOBAL_CONFIG_HHH

#ifdef WIN32
	#define USE_COMPILER_EXCEPTIONS		//使用编译器提供的异常机制
#endif

//for libtgmole32
#define TANGRAMCOM_RPC_WITH_IPCP_TCP

//for scm
#define TANGRAMCOM_SCM_CONN_BINDING RPCBINGING_PROTSEQU_RPCCOM_TCP":127.0.0.1[7892]"






#endif




