//#include "OSComUtils.h"
//
//template <class CT,class InParams,class OutParams,UInt32 APINO,class CAPINO_STR>
//class CExcutorBase
//{
//private:
//	HRESULT _ExecSyncReq()
//	{
//		ULONG	len = 0;
//
//		if(NULL == mClientIf)		
//			return E_FAIL;
//
//		return mClientIf->SendSyncRequest(&mIn,sizeof(mIn),&mOut,sizeof(mOut),&len,RPCCOM_INFINITE_TIMEOUT);
//	}
//public:
//	//////////////////////////////////////////////////////////////////////////
//	template<class Param1>
//		HRESULT ExecSyncReq(Param1 p1)
//	{
//		CT *_this = static_cast<CT *>(this);
//		_this->SetInParams(p1);
//		HRESULT hr = _ExecSyncReq();
//		if(SUCCEEDED(hr))
//		{
//			this->GetOutParams(p1);
//		}
//
//		return hr;
//	}
//
//	template<class Param1,class Param2>
//		HRESULT ExecSyncReq(Param1 p1,Param2 p2)
//	{
//		CT *_this = static_cast<CT *>(this);
//		_this->SetInParams(p1,p2);
//		HRESULT hr = _ExecSyncReq();
//		if(SUCCEEDED(hr))
//		{
//			_this->GetOutParams(p1,p2);
//		}
//		return hr;
//	}
//
//	template<class Param1,class Param2,class Param3>
//		HRESULT ExecSyncReq(Param1 p1,Param2 p2,Param3 p3)
//	{
//		CT *_this = static_cast<CT *>(this);
//		_this->SetInParams(p1,p2,p3);
//		HRESULT hr = _ExecSyncReq();
//		if(SUCCEEDED(hr))
//		{
//			_this->GetOutParams(p1,p2,p3);
//		}
//		return hr;
//	}
//
//	template<class Param1,class Param2,class Param3,class Param4>
//		HRESULT ExecSyncReq(Param1 p1,Param2 p2,Param3 p3,Param4 p4)
//	{
//		CT *_this = static_cast<CT *>(this);
//		_this->SetInParams(p1,p2,p3,p4);
//		HRESULT hr = _ExecSyncReq();
//		if(SUCCEEDED(hr))
//		{
//			_this->GetOutParams(p1,p2,p3,p4);
//		}
//		return hr;
//	}
//
//
//	CExcutorBase():mClientIf(NULL)
//	{
//		memset(&mIn,0,sizeof(mIn));
//		memset(&mOut,0,sizeof(mOut));
//		mIn.apino = APINO;
//	}
//
//	void Init(RpcClientIf *inIf)
//	{
//		mClientIf = inIf;
//	}
//
//protected:
//	InParams	mIn;
//	OutParams	mOut;
//	CComSmartPtr<RpcClientIf> mClientIf;
//};
//
//template <class CT,class InParams,UInt32 APINO,class CAPINO_STR>
//class CExcutorBaseNoReply
//{
//private:
//	HRESULT _ExecSyncReq()
//	{
//		ULONG	len = 0;
//
//		if(NULL == mClientIf)		
//			return E_FAIL;
//
//		return mClientIf->SendSyncRequest(&mIn,sizeof(mIn),NULL,0,&len,RPCCOM_INFINITE_TIMEOUT);
//	}
//public:
//	//////////////////////////////////////////////////////////////////////////
//	template<class Param1>
//		HRESULT ExecSyncReq(Param1 p1)
//	{
//		CT *_this = static_cast<CT *>(this);
//		_this->SetInParams(p1);
//		HRESULT hr = _ExecSyncReq();
//		return hr;
//	}
//
//	template<class Param1,class Param2>
//		HRESULT ExecSyncReq(Param1 p1,Param2 p2)
//	{
//		CT *_this = static_cast<CT *>(this);
//		_this->SetInParams(p1,p2);
//		HRESULT hr = _ExecSyncReq();
//		return hr;
//	}
//
//	template<class Param1,class Param2,class Param3>
//		HRESULT ExecSyncReq(Param1 p1,Param2 p2,Param3 p3)
//	{
//		CT *_this = static_cast<CT *>(this);
//		_this->SetInParams(p1,p2,p3);
//		HRESULT hr = _ExecSyncReq();
//		return hr;
//	}
//
//	template<class Param1,class Param2,class Param3,class Param4>
//		HRESULT ExecSyncReq(Param1 p1,Param2 p2,Param3 p3,Param4 p4)
//	{
//		CT *_this = static_cast<CT *>(this);
//		_this->SetInParams(p1,p2,p3,p4);
//		HRESULT hr = _ExecSyncReq();
//		return hr;
//	}
//
//
//	CExcutorBaseNoReply():mClientIf(NULL)
//	{
//		memset(&mIn,0,sizeof(mIn));
//		mIn.apino = APINO;
//	}
//
//	void Init(RpcClientIf *inIf)
//	{
//		mClientIf = inIf;
//	}
//
//
//protected:
//	InParams	mIn;
//	CComSmartPtr<RpcClientIf> mClientIf;
//};
