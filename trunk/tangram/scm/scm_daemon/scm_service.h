/*
* scm service
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
#ifndef _SCM_SERVICE_HHH
#define _SCM_SERVICE_HHH

#include "OSBaseTypes.h"
#include "OSMutexRW.h"

#include "objbase.h"
#include <map>
#include <utility>
#include <string>
using namespace std;

namespace nsSCMD{

template <class T>
struct scmsvr_ltg
{
	bool operator()(const T& s1,const T& s2) const
	{		return memcmp(&s1,&s2,sizeof(T)) < 0;	}
};

class  scmsvr_objref_t
{
public:
	unsigned char objref[1024];
	int length;

	scmsvr_objref_t(){
		length = 0;
		objref[0] = 0;
	}
	scmsvr_objref_t(int in_size,const unsigned char* in_data)
	{
		memcpy(objref,in_data,in_size);
		length = in_size;
	};
};

class ClsFactoryResloveService
{
public:
	
	HRESULT Register(REFIID in_riid,int in_size,const unsigned char* in_data);
	HRESULT Unregister(REFIID in_riid);
	HRESULT Query(REFIID in_riid,int in_size,unsigned char* out_data,int *out_length);

private:
	typedef map<IID,scmsvr_objref_t, scmsvr_ltg<IID> > ClsFactoryContainer;
	ClsFactoryContainer	mClsFactorys;
	OSMutexRW			mClsFactoryMutexRW;

};

class ApartMentResloveService{
public:
	
	HRESULT Register(const OXID &in_oxid,const char* in_binding);
	HRESULT Unregister(const  OXID &in_oxid);
	HRESULT Query(const OXID &in_oxid,int in_binding_size,char * out_binding);

private:
	typedef map<OXID,string,scmsvr_ltg<OXID> > ApartMentContainer;
	ApartMentContainer	mApartMents;
	OSMutexRW			mApartMentsMutexRW;

};

};
#endif

