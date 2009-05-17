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
#include "dcom.h"
#include "scm_service.h"

#include "tgmlog.h"
TGMLOG_DEFAULT_DEBUG_CHANNEL(agrscm);

namespace nsSCMD{

	//////////////////////////////////////////////////////////////////////////
	HRESULT ClsFactoryResloveService::Register(REFIID in_riid,int in_size,const unsigned char* in_data)
	{
		OSMutexWriteLocker wlocker(&mClsFactoryMutexRW);
		scmsvr_objref_t theObj(in_size,in_data);
		mClsFactorys[in_riid] = theObj;
		//pair< ClsFactoryContainer::iterator,bool> ret = mClsFactorys.insert(make_pair(in_riid,scmsvr_objref_t(in_size,in_data)));
		//if(!ret.second)
		//{
		//	return CO_E_OBJISREG;
		//}
		return S_OK;
	}

	HRESULT ClsFactoryResloveService::Unregister(REFIID in_riid)
	{
		OSMutexWriteLocker wlocker(&mClsFactoryMutexRW);
		size_t ret =  mClsFactorys.erase(in_riid);
		return ret ? S_OK:CO_E_OBJNOTREG;
	}

	HRESULT ClsFactoryResloveService::Query(REFIID in_riid,int in_size,unsigned char* out_data,int *out_length)
	{
		OSMutexReadLocker rlocker(&mClsFactoryMutexRW);
		ClsFactoryContainer::iterator iter = mClsFactorys.find(in_riid);
		if(iter != mClsFactorys.end())
		{
			if(in_size < (*iter).second.length){
				WARN( "in_size is not enough\n");
				return  E_OUTOFMEMORY;
			}

			*out_length = (*iter).second.length;
			memcpy(out_data,(*iter).second.objref,*out_length);
			return S_OK;
		}
		return CO_E_OBJNOTREG;
	}




	//////////////////////////////////////////////////////////////////////////
	HRESULT ApartMentResloveService::Register(const OXID &in_oxid,const char* in_binding)
	{
		OSMutexWriteLocker wlocker(&mApartMentsMutexRW);
		mApartMents[in_oxid] = string(in_binding);
		//pair< ApartMentContainer::iterator,bool> ret = mApartMents.insert(make_pair(in_oxid,string(in_binding)));
		//if(!ret.second)
		//{
		//	return CO_E_OBJISREG;
		//}
		return S_OK;
	}

	HRESULT ApartMentResloveService::Unregister(const  OXID &in_oxid)	
	{
		OSMutexWriteLocker wlocker(&mApartMentsMutexRW);
		size_t ret =  mApartMents.erase(in_oxid);
		return ret ? S_OK:CO_E_OBJNOTREG;
	}

	HRESULT ApartMentResloveService::Query(const OXID &in_oxid,int in_binding_size,char * out_binding)
	{
		OSMutexReadLocker rlocker(&mApartMentsMutexRW);
		ApartMentContainer::iterator iter = mApartMents.find(in_oxid);
		if(iter != mApartMents.end())
		{
			string & rrl = (*iter).second;
			if(in_binding_size < rrl.length()+1 )
				return E_OUTOFMEMORY;
			strcpy(out_binding,rrl.c_str());
			return S_OK;
		}
		return CO_E_OBJNOTREG;
	}


};


