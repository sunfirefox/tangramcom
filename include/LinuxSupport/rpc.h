#ifndef __RPC_H__TXF_78783
#define __RPC_H__TXF_78783

//
#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////



#ifndef __MIDL_USER_DEFINED
#define midl_user_allocate MIDL_user_allocate
#define midl_user_free     MIDL_user_free
#define __MIDL_USER_DEFINED
#endif

typedef void * I_RPC_HANDLE;
typedef long RPC_STATUS;

#if defined(__RPC_WIN32__) || defined(__RPC_WIN64__)
#define RPC_UNICODE_SUPPORTED
#endif


#define __RPC_API
#define __RPC_USER
#define __RPC_STUB
#define RPC_ENTRY


#define __RPC_FAR

#define STDMETHODCALLTYPE
#define STDMETHODVCALLTYPE

#define STDAPICALLTYPE
#define STDAPIVCALLTYPE

#define STDAPI                  EXTERN_C HRESULT STDAPICALLTYPE
#define STDAPI_(type)           EXTERN_C type STDAPICALLTYPE

#define STDMETHODIMP            HRESULT STDMETHODCALLTYPE
#define STDMETHODIMP_(type)     type STDMETHODCALLTYPE

#define BEGIN_INTERFACE
#define END_INTERFACE

typedef void __RPC_FAR * RPC_IF_HANDLE;

typedef char IRpcStubBuffer;
typedef char IRpcChannelBuffer;
typedef char RPC_MESSAGE,*PRPC_MESSAGE;


#ifdef __cplusplus
}
#endif


#endif //__RPC_H__TXF_78783
