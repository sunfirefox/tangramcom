/*
*  RPCRT4
*
* Copyright 2009 Jokul for Tranzda
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
* 
*/

#ifndef RPC_PACKET_MERGER_HHHH
#define RPC_PACKET_MERGER_HHHH
#include "rpc_defs.h"
#include "tgmlog.h"
class CRpcPacker
{
public:
	void* get_DataPtr(){return buf;}
	unsigned int get_DataLen(){return len;}

	CRpcPacker():buf(NULL),len(0),isMalloc(false)
	{}

	CRpcPacker(RpcPktHdr *Header):buf(NULL),len(0),isMalloc(false){
		BuildPacket(Header);
	}

	CRpcPacker(RpcPktHdr *Header,void *Buffer, unsigned int BufferLength):buf(NULL),len(0),isMalloc(false){
		BuildPacket(Header,Buffer,BufferLength);
	}

	void BuildPacket(RpcPktHdr *Header,void *Buffer = NULL, unsigned int BufferLength = 0);

	void Reset();

	~CRpcPacker(){
		Reset();
	}

private:
	void* buf;
	unsigned int len;
	bool	isMalloc;

};

class CRpcUnpacker
{
public:
	RpcPktHdr*		get_Header(){return hdr;}
	void*			get_BufferPtr(){return (void*)data;}
	unsigned int	get_BufferLength(){return len;}
	PRPC_MESSAGE	get_RpcMessage(PRPC_MESSAGE pMsg)
	{pMsg->Buffer = (void*)data;pMsg->BufferLength = len;return pMsg;}

	CRpcUnpacker(const void * inDataPtr,unsigned int inDataSize);

private:
	static DWORD GetHeaderSize(const RpcPktHdr *Header);
private:
	RpcPktHdr * hdr;
	const void* data;
	unsigned int len;

};

#endif
