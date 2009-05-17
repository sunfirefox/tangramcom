/*
* Rpc interface
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
#include "tgmrpc_if.h"

//#if 0
//#ifndef Rpc_COM_IF_TXF_2343
//#define Rpc_COM_IF_TXF_2343
//#include "OSDataTypes.h"
//#include "objbase.h"
//#include "unknwn.h"
//
//
//typedef void (*ipc_client_memory_free_fun_t)(void* inMem);
//
//enum
//{
//	//RpcCOM_MESG_DATA_MAXSIZE = 4096,	//!< Rpc COM每个请求或者通知的最大尺寸
//	RPCCOM_INFINITE_TIMEOUT = 200*1000	//!< 最长等待200秒
//};
//
////#define RpcCOM_E_REQUEST_EXEC_TIMEOUT				((HRESULT)0x80040044L)	//执行请求超时，MTAPI_E_TIMEOUT
////#define RpcCOM_E_CLIENT_CONN_NOT_EXIST			((HRESULT)0x80040601L)	//客户不存在或者已经异常断开，MTAPI_E_RpcCOM_CLIENT_CONN_NOT_EXIST
////#define RpcCOM_E_REQUEST_EXEC_GET_RESPONSE_FAILED	((HRESULT)0x80040602L) //获取客户RESPONSE失败
//
//#define RPCCOM_E_REQUEST_EXEC_TIMEOUT	((HRESULT)0x80040044L)	//执行请求超时，MTAPI_E_TIMEOUT
//
//#define RPCCOM_E_CONN_DISCONNECTED	((HRESULT)0x80040601L)		//客户不存在或者已经异常断开，MTAPI_E_RpcCOM_CLIENT_CONN_NOT_EXIST
//#define RPCCOM_E_CONN_SEND_FAILED	((HRESULT)0x80040602L)
//#define RPCCOM_E_CONN_INVALID		((HRESULT)0x80040603L)
//#define RPCCOM_E_CONN_NO_PACKET		((HRESULT)0x80040604L)
//#define RPCCOM_E_CONN_LISTENING		((HRESULT)0x80040605L)
//#define RPCCOM_E_CONN_LISTEN_FAILED	((HRESULT)0x80040606L)
//#define RPCCOM_E_CONN_ADDR_INUSE	((HRESULT)0x80040607L)
//#define RPCCOM_E_SOCKET_INPROGRESS	((HRESULT)0x80040608L)
//#define RPCCOM_E_CONN_ADDR_NOTAVAIL	((HRESULT)0x80040609L)		//客户端地址无效。例如，connect函数地址参数为空
//#define RPCCOM_E_SOCKET_AGAIN		((HRESULT)0x8004060bL)
//
//#define RPCBINGING_PROTSEQU_RPCCOM_TCP "rpcp_tcp"
//#define RPCBINGING_PROTSEQU_RPCCOM_UNIX "rpcp_unix"
//
//#define RPCBINGING_MAX_SIZE 128
//
//
//
//
///*!
//* Rpc所有相关接口，不要求实现QueryInterface接口
//*
//*/
//
///*
//|Client APP|	<----------------->   |Server APP|
//--------------------------------------------------
//|Client Rpc|	<----------------->	  |Server Rpc|
//*/
//
//
//
///*!
//* 连接通知接口
//* 由被调用方(ServerRpc/ClientRpc)实现
//*/
//class ITgmRpcConnectionNotifyObject:public IUnknown
//{
//public:
//	//! 获取请求/通知序号
//	virtual ULONG GetSequNo() = 0;
//
//	//!获取请求调用参数
//	virtual const void* GetParamsPtr() =0;
//
//	//! 获取请求调用参数大小
//	virtual ULONG GetParamsSize() = 0;
//};
//
///*!
//* 连接请求接口
//* 由被调用方(ServerRpc/ClientRpc)实现
//*/
//class ITgmRpcConnectionRequestObject:public ITgmRpcConnectionNotifyObject
//{
//public:
//	/*!
//	* 发送成功的应答
//	* 注意,如果返回RPCCOM_E_CONN_DISCONNECTED,表示客户端可能已经异常断开
//	* 
//	*/
//	virtual HRESULT SendReply(const void *inParamsPtr,ULONG inParmsSize) = 0;
//
//	/*发送应答，不需要数据区*/
//	virtual HRESULT SendResult(HRESULT inResult) = 0;
//
//	
//};
//
//
///*!连接异步请求结果
//* 由异步请求的调用方(ServerApp/ClientAPP)实现
//*
//*/
//class ITgmRpcConnectionAsyncReqCallBack:public IUnknown
//{
//public:
//	/*!
//	* 处理异步请求应答
//	*
//	*/
//	virtual void On_Reply(HRESULT inResult, const void* outParamsPtr,ULONG outParamsSize,ULONG inSeqNo,ULONG inUserData) = 0;
//};
//
///*!连接事件处理接口
//* 
//* 由ServerAPP/ClientAPP实现
//*/
//class ITgmRpcConnectionEvent:public IUnknown
//{
//public:
//	/*! 
//	*  本客户连接对象被创建
//	*  @param inSerNtyCBPtr 用以发送服务器事件的接口
//	*/
//	virtual void On_Connected()	= 0;
//
//	/*! 
//	*  本客户连接对象即将被销毁
//	*  
//	*/
//	virtual void On_Disconnected()	= 0;
//
//	/*!   
//	* 本客户连接对象异常断开 
//	*  
//	*/	
//	virtual void On_DisconnectUnexpected() = 0;
//
//	/*! 
//	*  本客户连接对象收到Notify
//	*  
//	*/
//	virtual void On_Notify(ITgmRpcConnectionNotifyObject *inNtyPtr)	= 0;
//
//	/*! 
//	*  本客户连接对象收到Request
//	*  
//	*/
//	virtual void On_Request(ITgmRpcConnectionRequestObject *inReqPtr)	= 0;
//};	
//
///*!连接操作接口
//* 
//* 由RpcCom实现
//*/
//class ITgmRpcConnection:public IUnknown
//{
//public:
//	/*! 断开连接 
//	*  
//	*  
//	*/
//	virtual HRESULT Disconnect() = 0;
//
//	/*!
//	* 注册事件处理接口
//	*
//	*/
//	virtual HRESULT Advise(ITgmRpcConnectionEvent * inEventHandler) = 0;
//
//	/*!取消注册事件处理接口
//	* 
//	*
//	*/
//	virtual HRESULT Unadvise() = 0;
//
//
//	/*! 同步方法调用  
//	*  当调用返回时，将直接填充各个输出参数。
//	*/
//	virtual HRESULT SendSyncRequest(const void *inParamsPtr,ULONG inParmsSize,
//		void *outParamsPtr/* = NULL*/,ULONG outParamsSize/* = 0*/,ULONG *outParamsLenPtr/* = NULL*/,
//		ULONG inTimeOut /*= RpcCOM_INFINITE_TIMEOUT*/) = 0;
//
//	/*! 同步方法调用  
//	*  当调用返回时，由RpcCom分配内存并返回结果，用户需要调用free_result_mem来释放返回的内存。
//	*/
//	virtual HRESULT SendSyncRequestEx(const void *inParamsPtr,ULONG inParmsSize,
//		void **outParamsPtr/* = NULL*/,ULONG *outParamsLenPtr/* = NULL*/,ipc_client_memory_free_fun_t * free_result_mem,
//		ULONG inTimeOut /*= RpcCOM_INFINITE_TIMEOUT*/) = 0;
//
//	/*! 异步方法调用
//	*  当请求发送成功或者失败，调用立刻返回。
//	*  当服务器端执行完相应的请求，调用指定的回调接口
//	*/
//	virtual HRESULT SendAsyncRequest(const void *inParamsPtr,ULONG inParmsSize,
//		ITgmRpcConnectionAsyncReqCallBack *inCallback /*= NULL*/,
//		ULONG *outReqNo/* = NULL*/,
//		ULONG inUserData/* = 0*/,
//		ULONG inTimeOut/* = RpcCOM_INFINITE_TIMEOUT*/) = 0;
//
//	/*! 通知
//	*/
//	virtual HRESULT SendNotify(const void *inParamsPtr,ULONG inParmsSize) = 0;
//
//	/*!获取ResourceLocation
//	* 
//	*
//	*/
//	virtual HRESULT GetResourceLocation(char* outBindString,ULONG inBingStringSize) = 0;
//};
//
//
//
//
////////////////////////////////////////////////////////////////////////////
//// Rpc客户组件接口                                                         
////////////////////////////////////////////////////////////////////////////
//
//
//
///*!Rpc客户组件
// * 由ClientRpc实现
// *
// */
//class ITgmRpcClient:public ITgmRpcConnection{
//public:
//	/*! 连接 
//	*  
//	*  
//	*/
//	virtual HRESULT Connect() = 0;
//};
//
//
//
//
////////////////////////////////////////////////////////////////////////////
////Rpc 服务组件接口
////////////////////////////////////////////////////////////////////////////
//
///*!Rpc 服务器事件处理接口
// * 
// * 由ServerAPP实现
// */
//class ITgmRpcServerEvent:public IUnknown
//{
//public:
//	/*!
//	*　处理请求：创建客户连接对象。
//	*　ppOutCliObj返回客户创建的新客户连接对象
//	*/
//	virtual HRESULT  STDMETHODCALLTYPE On_ClientConnection_Created(ITgmRpcConnection *ppOutCliObj) = 0;
//};	
//
//
///*!Rpc 服务器组件
//* 
//* 由ServerRpc实现
//*/
//class ITgmRpcServer:public IUnknown
//{
//public:
//	/*! 开始监听客户请求
//	*  
//	*  @param[in] in_listenCh下行通道
//	*/
//	virtual HRESULT Listen(/*const OLECHAR *in_listenCh*/) = 0;
//
//	/*! 停止监听
//	*  
//	*  
//	*/
//	virtual HRESULT Close() = 0;
//
//	/*!
//	* 关闭所有客户连接对象
//	*
//	*/
//	virtual HRESULT DisconnectAll() = 0;
//
//
//	/*!
//	 * 注册事件处理接口
//	 *
//	 */
//	virtual HRESULT Advise(ITgmRpcServerEvent * inEventHandler) = 0;
//
//	/*!取消注册事件处理接口
//	 * 
//	 *
//	 */
//	virtual HRESULT Unadvise() = 0;
//
//	/*!获取Endpoint信息
//	* 
//	*
//	*/
//	virtual HRESULT GetResourceLocation(char* outBindString,ULONG inBingStringSize) = 0;
//};	
//
//
//#endif
//
//#endif
//
