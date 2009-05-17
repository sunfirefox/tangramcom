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

#ifndef tgmtl_unk_h_345
#define tgmtl_unk_h_345

#ifndef TANGRAM_TEMPLATE_COMMON_HEADER
#error this file must include in tgmtl.h
#endif

#ifdef _TCOM_UTILITY_THREAD_NOT_SUPPORT
#define os_atomic_add(REFADDR,Y) (++(*REFADDR))
#define os_atomic_sub(REFADDR,Y) (--(*REFADDR))
#else
#include "OSAtomic.h"
#endif

namespace tgmtl{

template <class IF_NAME>
class cocalss_base:public IF_NAME
{
public:
	STDMETHODIMP QueryInterface ( /* [in] */ REFIID riid,/* [iid_is][out] */ void **ppvObject)
	{
		if(riid == IID_IUnknown){
			*ppvObject = static_cast<IUnknown*>(this);
		}else if(riid == m_IID) {
			*ppvObject = static_cast<IF_NAME*>(this);
		}else {
			*ppvObject = NULL;
			return E_NOINTERFACE ;
		}

		((IUnknown*)*ppvObject)->AddRef();
		return S_OK;
	}

	STDMETHODIMP_(ULONG)  AddRef ()
	{
		ULONG ref =  os_atomic_add(&m_Ref,1);
		if(ref == 1)
		{
			ModuleAddRef();
		}
		return ref;
	}

	STDMETHODIMP_(ULONG)  Release()
	{
		ULONG ref = os_atomic_sub(&m_Ref,1);
		if(0 == ref)	
		{
			delete this;
			ModuleRelease();
		}
		return ref;
	}

	cocalss_base(REFIID inRIID):m_Ref(0),m_IID(inRIID)
	{
		ModuleAddRef();

	}

	virtual ~cocalss_base()
	{
		ModuleRelease();
	}

private:
	ULONG m_Ref;
	IID  m_IID;
};


};//tgmtl

#endif//tgmtl_unk_h_345
