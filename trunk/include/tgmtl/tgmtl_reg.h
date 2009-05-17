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

#ifndef _TGMTL_REG_HHH
#define _TGMTL_REG_HHH

#ifndef TANGRAM_TEMPLATE_COMMON_HEADER
#error this file must include in tgmtl.h
#endif

struct regsvr_interface
{
	IID const *iid;		/* NULL for end of list */
	const char* name;		/* can be NULL to omit */
	IID const *base_iid;	/* can be NULL to omit */
	int num_methods;		/* can be <0 to omit */
	CLSID const *ps_clsid;	/* can be NULL to omit */
	CLSID const *ps_clsid32;	/* can be NULL to omit */
};


struct regsvr_coclass
{
	CLSID const* clsid;			/* NULL for end of list */
	const char* name;			/* can be NULL to omit,friendly name */
	const char* ips32;			/* can be NULL to omit,file path*/	
	const char* ips32_tmodel;	/* can be NULL to omit */	
	const char* progid;			/* can be NULL to omit */	
	const char* progid_ver;		/* can be NULL to omit */
};

#ifdef __cplusplus
extern "C" {
#endif


	#define COCLASS_LIST_BEGIN 	struct regsvr_coclass coclass_list[] = {
	#define COCLASS_LIST_ENTRY(clsid,classname) {&clsid,#classname,NULL,"Both",NULL,NULL},
	#define COCLASS_LIST_END	{NULL,NULL,NULL,NULL,NULL,NULL}};

	#define COCLASS_MAP_BEGIN 	TGMTL_DLLENTRY_API DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv){\
								HRESULT hr = S_OK;\

	#define COCLASS_MAP_ENTRY(clsid,classname) if(0 == memcmp(&rclsid,&clsid,sizeof(rclsid))){\
								static tgmtl::class_factory<classname> cf;\
								return cf.QueryInterface(riid,ppv);\
								}

	#define COCLASS_MAP_END		return CLASS_E_CLASSNOTAVAILABLE;\
								}


	HRESULT register_interfaces(struct regsvr_interface const *list);
	HRESULT unregister_interfaces(struct regsvr_interface const *list);
	HRESULT register_coclasses(struct regsvr_coclass const *list);
	HRESULT unregister_coclasses(struct regsvr_coclass const *list);

#ifdef __cplusplus
};
#endif


#endif//_TGMTL_REG_HHH

