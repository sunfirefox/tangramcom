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
//	//RpcCOM_MESG_DATA_MAXSIZE = 4096,	//!< Rpc COMÿ���������֪ͨ�����ߴ�
//	RPCCOM_INFINITE_TIMEOUT = 200*1000	//!< ��ȴ�200��
//};
//
////#define RpcCOM_E_REQUEST_EXEC_TIMEOUT				((HRESULT)0x80040044L)	//ִ������ʱ��MTAPI_E_TIMEOUT
////#define RpcCOM_E_CLIENT_CONN_NOT_EXIST			((HRESULT)0x80040601L)	//�ͻ������ڻ����Ѿ��쳣�Ͽ���MTAPI_E_RpcCOM_CLIENT_CONN_NOT_EXIST
////#define RpcCOM_E_REQUEST_EXEC_GET_RESPONSE_FAILED	((HRESULT)0x80040602L) //��ȡ�ͻ�RESPONSEʧ��
//
//#define RPCCOM_E_REQUEST_EXEC_TIMEOUT	((HRESULT)0x80040044L)	//ִ������ʱ��MTAPI_E_TIMEOUT
//
//#define RPCCOM_E_CONN_DISCONNECTED	((HRESULT)0x80040601L)		//�ͻ������ڻ����Ѿ��쳣�Ͽ���MTAPI_E_RpcCOM_CLIENT_CONN_NOT_EXIST
//#define RPCCOM_E_CONN_SEND_FAILED	((HRESULT)0x80040602L)
//#define RPCCOM_E_CONN_INVALID		((HRESULT)0x80040603L)
//#define RPCCOM_E_CONN_NO_PACKET		((HRESULT)0x80040604L)
//#define RPCCOM_E_CONN_LISTENING		((HRESULT)0x80040605L)
//#define RPCCOM_E_CONN_LISTEN_FAILED	((HRESULT)0x80040606L)
//#define RPCCOM_E_CONN_ADDR_INUSE	((HRESULT)0x80040607L)
//#define RPCCOM_E_SOCKET_INPROGRESS	((HRESULT)0x80040608L)
//#define RPCCOM_E_CONN_ADDR_NOTAVAIL	((HRESULT)0x80040609L)		//�ͻ��˵�ַ��Ч�����磬connect������ַ����Ϊ��
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
//* Rpc������ؽӿڣ���Ҫ��ʵ��QueryInterface�ӿ�
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
//* ����֪ͨ�ӿ�
//* �ɱ����÷�(ServerRpc/ClientRpc)ʵ��
//*/
//class ITgmRpcConnectionNotifyObject:public IUnknown
//{
//public:
//	//! ��ȡ����/֪ͨ���
//	virtual ULONG GetSequNo() = 0;
//
//	//!��ȡ������ò���
//	virtual const void* GetParamsPtr() =0;
//
//	//! ��ȡ������ò�����С
//	virtual ULONG GetParamsSize() = 0;
//};
//
///*!
//* ��������ӿ�
//* �ɱ����÷�(ServerRpc/ClientRpc)ʵ��
//*/
//class ITgmRpcConnectionRequestObject:public ITgmRpcConnectionNotifyObject
//{
//public:
//	/*!
//	* ���ͳɹ���Ӧ��
//	* ע��,�������RPCCOM_E_CONN_DISCONNECTED,��ʾ�ͻ��˿����Ѿ��쳣�Ͽ�
//	* 
//	*/
//	virtual HRESULT SendReply(const void *inParamsPtr,ULONG inParmsSize) = 0;
//
//	/*����Ӧ�𣬲���Ҫ������*/
//	virtual HRESULT SendResult(HRESULT inResult) = 0;
//
//	
//};
//
//
///*!�����첽������
//* ���첽����ĵ��÷�(ServerApp/ClientAPP)ʵ��
//*
//*/
//class ITgmRpcConnectionAsyncReqCallBack:public IUnknown
//{
//public:
//	/*!
//	* �����첽����Ӧ��
//	*
//	*/
//	virtual void On_Reply(HRESULT inResult, const void* outParamsPtr,ULONG outParamsSize,ULONG inSeqNo,ULONG inUserData) = 0;
//};
//
///*!�����¼�����ӿ�
//* 
//* ��ServerAPP/ClientAPPʵ��
//*/
//class ITgmRpcConnectionEvent:public IUnknown
//{
//public:
//	/*! 
//	*  ���ͻ����Ӷ��󱻴���
//	*  @param inSerNtyCBPtr ���Է��ͷ������¼��Ľӿ�
//	*/
//	virtual void On_Connected()	= 0;
//
//	/*! 
//	*  ���ͻ����Ӷ��󼴽�������
//	*  
//	*/
//	virtual void On_Disconnected()	= 0;
//
//	/*!   
//	* ���ͻ����Ӷ����쳣�Ͽ� 
//	*  
//	*/	
//	virtual void On_DisconnectUnexpected() = 0;
//
//	/*! 
//	*  ���ͻ����Ӷ����յ�Notify
//	*  
//	*/
//	virtual void On_Notify(ITgmRpcConnectionNotifyObject *inNtyPtr)	= 0;
//
//	/*! 
//	*  ���ͻ����Ӷ����յ�Request
//	*  
//	*/
//	virtual void On_Request(ITgmRpcConnectionRequestObject *inReqPtr)	= 0;
//};	
//
///*!���Ӳ����ӿ�
//* 
//* ��RpcComʵ��
//*/
//class ITgmRpcConnection:public IUnknown
//{
//public:
//	/*! �Ͽ����� 
//	*  
//	*  
//	*/
//	virtual HRESULT Disconnect() = 0;
//
//	/*!
//	* ע���¼�����ӿ�
//	*
//	*/
//	virtual HRESULT Advise(ITgmRpcConnectionEvent * inEventHandler) = 0;
//
//	/*!ȡ��ע���¼�����ӿ�
//	* 
//	*
//	*/
//	virtual HRESULT Unadvise() = 0;
//
//
//	/*! ͬ����������  
//	*  �����÷���ʱ����ֱ�����������������
//	*/
//	virtual HRESULT SendSyncRequest(const void *inParamsPtr,ULONG inParmsSize,
//		void *outParamsPtr/* = NULL*/,ULONG outParamsSize/* = 0*/,ULONG *outParamsLenPtr/* = NULL*/,
//		ULONG inTimeOut /*= RpcCOM_INFINITE_TIMEOUT*/) = 0;
//
//	/*! ͬ����������  
//	*  �����÷���ʱ����RpcCom�����ڴ沢���ؽ�����û���Ҫ����free_result_mem���ͷŷ��ص��ڴ档
//	*/
//	virtual HRESULT SendSyncRequestEx(const void *inParamsPtr,ULONG inParmsSize,
//		void **outParamsPtr/* = NULL*/,ULONG *outParamsLenPtr/* = NULL*/,ipc_client_memory_free_fun_t * free_result_mem,
//		ULONG inTimeOut /*= RpcCOM_INFINITE_TIMEOUT*/) = 0;
//
//	/*! �첽��������
//	*  �������ͳɹ�����ʧ�ܣ��������̷��ء�
//	*  ����������ִ������Ӧ�����󣬵���ָ���Ļص��ӿ�
//	*/
//	virtual HRESULT SendAsyncRequest(const void *inParamsPtr,ULONG inParmsSize,
//		ITgmRpcConnectionAsyncReqCallBack *inCallback /*= NULL*/,
//		ULONG *outReqNo/* = NULL*/,
//		ULONG inUserData/* = 0*/,
//		ULONG inTimeOut/* = RpcCOM_INFINITE_TIMEOUT*/) = 0;
//
//	/*! ֪ͨ
//	*/
//	virtual HRESULT SendNotify(const void *inParamsPtr,ULONG inParmsSize) = 0;
//
//	/*!��ȡResourceLocation
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
//// Rpc�ͻ�����ӿ�                                                         
////////////////////////////////////////////////////////////////////////////
//
//
//
///*!Rpc�ͻ����
// * ��ClientRpcʵ��
// *
// */
//class ITgmRpcClient:public ITgmRpcConnection{
//public:
//	/*! ���� 
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
////Rpc ��������ӿ�
////////////////////////////////////////////////////////////////////////////
//
///*!Rpc �������¼�����ӿ�
// * 
// * ��ServerAPPʵ��
// */
//class ITgmRpcServerEvent:public IUnknown
//{
//public:
//	/*!
//	*���������󣺴����ͻ����Ӷ���
//	*��ppOutCliObj���ؿͻ��������¿ͻ����Ӷ���
//	*/
//	virtual HRESULT  STDMETHODCALLTYPE On_ClientConnection_Created(ITgmRpcConnection *ppOutCliObj) = 0;
//};	
//
//
///*!Rpc ���������
//* 
//* ��ServerRpcʵ��
//*/
//class ITgmRpcServer:public IUnknown
//{
//public:
//	/*! ��ʼ�����ͻ�����
//	*  
//	*  @param[in] in_listenCh����ͨ��
//	*/
//	virtual HRESULT Listen(/*const OLECHAR *in_listenCh*/) = 0;
//
//	/*! ֹͣ����
//	*  
//	*  
//	*/
//	virtual HRESULT Close() = 0;
//
//	/*!
//	* �ر����пͻ����Ӷ���
//	*
//	*/
//	virtual HRESULT DisconnectAll() = 0;
//
//
//	/*!
//	 * ע���¼�����ӿ�
//	 *
//	 */
//	virtual HRESULT Advise(ITgmRpcServerEvent * inEventHandler) = 0;
//
//	/*!ȡ��ע���¼�����ӿ�
//	 * 
//	 *
//	 */
//	virtual HRESULT Unadvise() = 0;
//
//	/*!��ȡEndpoint��Ϣ
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
