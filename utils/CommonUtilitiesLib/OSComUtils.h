/*
*
* @APPLE_LICENSE_HEADER_START@
* 
* Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
* 
* This file contains Original Code and/or Modifications of Original Code
* as defined in and that are subject to the Apple Public Source License
* Version 2.0 (the 'License'). You may not use this file except in
* compliance with the License. Please obtain a copy of the License at
* http://www.opensource.apple.com/apsl/ and read it before using this
* file.
* 
* The Original Code and all software distributed under the License are
* distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
* EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
* INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
* Please see the License for the specific language governing rights and
* limitations under the License.
* 
* @APPLE_LICENSE_HEADER_END@
*
*/
/*
File:       OSComUtils.h

Contains:   COM template
*/
#ifndef OS_COM_UITILIS_HHH
#define OS_COM_UITILIS_HHH
#include "OSBaseTypes.h"
#include "OSAtomic.h"
#include "MyAssert.h"
#include "unknwn.h"
//#define STDMETHODCALLTYPE

/*!
本模板提供了IUnknown的基本实现。用户可以继承这个模板，减少代码的编写。
如果某些场合不需要支持QueryInterface,仅仅利用AddRef/Release,这也是一个好选择；否则需要重新实现QueryInterface
本模板适用于直接或者间接从IUnknown继承的接口,不支持多继承.
*/
template<class IfClass>
class IUnknownImp:public IfClass
{
public:
	HRESULT STDMETHODCALLTYPE QueryInterface ( /* [in] */ REFIID riid,/* [iid_is][out] */ void **ppvObject)
	{
		UNUSED(riid);UNUSED(ppvObject);
		return E_NOTIMPL;
	}

	ULONG  STDMETHODCALLTYPE AddRef ()
	{
		Assert(m_Ref <1000);
		//printf("IUnknownImp AddRef %p,%X\r\n",this,m_Ref+1);
		return os_atomic_add((ULONG*)&m_Ref,1);
	}

	ULONG  STDMETHODCALLTYPE Release()
	{
		ULONG ref = os_atomic_sub(&m_Ref,1);
		//printf("IUnknownImp Release %p,%X\r\n",this,ref);
		Assert(ref <1000);

		if(0 == ref)	
		{
			//delete 和 OSMutexLocker _Locker(&mMutex)不处于同一个代码区域。这样，析构本对象时，没人使用mMutex
			delete this;
		}

		return ref;
	}

	inline ULONG GetRef()	{	
		return m_Ref;	
	}

	virtual ~IUnknownImp()
	{
	}
protected:
	IUnknownImp():m_Ref(0)
	{
	}

	inline ULONG& get_Reference(){	
		return m_Ref;	
	}
private:
	ULONG	m_Ref;
};

class CSmartObject
{
public:
	inline ULONG AddRef()
	{
		Assert(m_Ref <1000);
		return os_atomic_add((ULONG*)&m_Ref,1);
	}

	inline ULONG Release()
	{
		ULONG ref = os_atomic_sub((ULONG*)&m_Ref,1);
		//OSLOG_LOG((OSLL_LOG,"CSmartObject Release %p,%X\r\n",this,ref));
        Assert(ref <1000);

		if(0 == ref)
		{
			delete this;
		}
		return ref;
	}

	inline ULONG GetRef()	{	
		return m_Ref;	
	}

	CSmartObject():m_Ref(0){}

	virtual ~CSmartObject(){}

private:
	ULONG m_Ref;

	/*OSMutex* getMutex()
	{
	static OSMutex mMutex;
	return &mMutex;
	}*/

};


//CComPtrBase provides the basis for all other smart pointers
//The other smartpointers add their own constructors and operators
template <class T>
class CComSmartPtrBase
{
protected:
	CComSmartPtrBase()
	{
		mPtr = NULL;
	}
	CComSmartPtrBase(T* lp)
	{
		mPtr = lp;
		if (mPtr != NULL)
			mPtr->AddRef();
	}
public:
	typedef T _PtrClass;
	~CComSmartPtrBase()
	{
		if (mPtr){
			mPtr->Release();
			mPtr = NULL;
		}
	}
	operator T*() const
	{
		return mPtr;
	}
	T& operator*() const
	{
		Assert(mPtr!=NULL);
		return *mPtr;
	}
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the mPtr member explicitly.
	T** operator&() 
	{
		Assert(mPtr==NULL);
		return &mPtr;
	}

	bool operator!() const 
	{
		return (mPtr == NULL);
	}
	bool operator<(T* pT) const 
	{
		return mPtr < pT;
	}
	bool operator==(T* pT) const 
	{
		return mPtr == pT;
	}

	// Release the interface and set to NULL
	void Release() 
	{
		T* pTemp = mPtr;
		if (pTemp)
		{
			mPtr = NULL;
			pTemp->Release();
		}
	}

	// Attach to an existing interface (does not AddRef)
	void Attach(T* p2) 
	{
		if (mPtr)
			mPtr->Release();
		mPtr = p2;
	}
	// Detach the interface (does not Release)
	T* Detach() 
	{
		T* pt = mPtr;
		mPtr = NULL;
		return pt;
	}
	HRESULT CopyTo(T** ppT) 
	{
		Assert(ppT != NULL);
		if (ppT == NULL)
			return E_POINTER;
		*ppT = mPtr;
		if (mPtr)
			mPtr->AddRef();
		return S_OK;
	}

	T* mPtr;
};

template <class T>
class CComSmartPtr : public CComSmartPtrBase<T>
{
public:
	CComSmartPtr() 
	{
	}

	CComSmartPtr(T* lp):CComSmartPtrBase<T>(lp)
	{
	}

	CComSmartPtr(const CComSmartPtr<T>& lp):CComSmartPtrBase<T>(lp.mPtr)
	{
	}

	T* operator=(T* lp) 
	{
		if (lp)
			lp->AddRef();
		if (this->mPtr)
			(this->mPtr)->Release();
		this->mPtr = lp;
		return lp;
	}

	T* operator=(const CComSmartPtr<T>& lp) 
	{
		if (lp.mPtr)
			lp->AddRef();
		if (this->mPtr)
			(this->mPtr)->Release();
		this->mPtr = lp.mPtr;
		return this->mPtr;
	}

	T* operator->() const 
	{ 
		return this->mPtr; 
	}
};

#endif//#ifndef OS_COM_UITILIS_HHH
