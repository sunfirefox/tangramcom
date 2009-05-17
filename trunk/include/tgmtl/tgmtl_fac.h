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

#ifndef TGMTL_FAC_HHH
#define TGMTL_FAC_HHH

#ifndef TANGRAM_TEMPLATE_COMMON_HEADER
#error this file must include in tgmtl.h
#endif

namespace tgmtl{

	template<typename T> 
	class class_factory_base : public IClassFactory
	{
	public:
		STDMETHOD_(ULONG, AddRef)()
		{
#ifndef EXESVC
			ModuleAddRef();
#endif
			return 2;
		}

		STDMETHOD_(ULONG, Release)()
		{
#ifndef EXESVC
			ModuleRelease();
#endif
			return 1;
		}

		STDMETHOD(QueryInterface)(REFIID riid, void **pv)
		{
			if (!pv) return E_POINTER;
			*pv = 0;
			if (riid == IID_IClassFactory) *pv = static_cast<IClassFactory*>(this);
			if (riid == IID_IUnknown) *pv = this;

			if (*pv == NULL) return E_NOINTERFACE;

			AddRef();
			return S_OK;
		}

		STDMETHOD(LockServer)(BOOL bLock)
		{
			if (bLock)
				ModuleAddRef();
			else
				ModuleRelease();

			return S_OK;
		}
	};

	template<typename T> class class_factory : public class_factory_base<T>
	{
	public:
		STDMETHOD(CreateInstance)(IUnknown *pUnkOuter, REFIID riid, void **ppv)
		{
			if (pUnkOuter) return CLASS_E_NOAGGREGATION;
			*ppv = 0;

			T* t = new T;

			t->AddRef();
			HRESULT hr = t->QueryInterface(riid, ppv);
			t->Release();

			return hr;
		}

	};

};


#endif //#ifndef TGMTL_FAC_HHH