/*
* configure header
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

#ifndef TANGRAM_WINE_CONFIG_HHH
#define TANGRAM_WINE_CONFIG_HHH



#ifdef WIN32
	#define WINE_WINAPI WINAPI
	#define __i386__
	#define TANGRAM_REGISTERTABLE_PATH  "D:\\tangram_registery.xml";

#else
	#define WINE_WINAPI
	
	#ifndef __stdcall
	#define __stdcall
	#endif

	#define TANGRAM_REGISTERTABLE_PATH  "./tangram_registery.xml";
#endif

#endif





