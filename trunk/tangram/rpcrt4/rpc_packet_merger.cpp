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
#include "rpc.h"
#include "rpcdce.h"
#include "guiddef.h"
#include "rpcdcep.h"
#include "rpc_packet_merger.h"


TGMLOG_DEFAULT_DEBUG_CHANNEL(rpc);

void CRpcPacker::BuildPacket(RpcPktHdr *Header,void *Buffer, unsigned int BufferLength)
{
	if(BufferLength == 0)
	{
		buf = Header;
		len = Header->common.frag_len;
		isMalloc = false;
	}
	else
	{
		unsigned int hdr_size = Header->common.frag_len;
		Header->common.frag_len = len = hdr_size + BufferLength;
		buf = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, len);
		memcpy(buf,Header,hdr_size);
		memcpy((char*)buf+hdr_size,Buffer,BufferLength);
		isMalloc = true;
	}
}

void CRpcPacker::Reset()
{
	if(isMalloc)
	{
		HeapFree(GetProcessHeap(), 0,buf);
		isMalloc = false;
	}
	buf = NULL;
	len = 0;
}


CRpcUnpacker::CRpcUnpacker(const void * inDataPtr,unsigned int inDataSize)
{
	hdr = (RpcPktHdr*)inDataPtr;
	unsigned hdr_len = GetHeaderSize(hdr);

	/* read packet body */
	data = (const char*)inDataPtr+hdr_len;
	switch (hdr->common.ptype) {
case PKT_RESPONSE:
	len = hdr->response.alloc_hint;
	break;
case PKT_REQUEST:
	len = hdr->request.alloc_hint;
	break;
default:
	len = hdr->common.frag_len - hdr_len;
	}
}

DWORD CRpcUnpacker::GetHeaderSize(const RpcPktHdr *Header)
{
	static const DWORD header_sizes[] = {
		sizeof(Header->request), 0, sizeof(Header->response),
			sizeof(Header->fault), 0, 0, 0, 0, 0, 0, 0, sizeof(Header->bind),
			sizeof(Header->bind_ack), sizeof(Header->bind_nack),
			0, 0, 0, 0, 0
	};
	ULONG ret = 0;

	if (Header->common.ptype < sizeof(header_sizes) / sizeof(header_sizes[0])) {
		ret = header_sizes[Header->common.ptype];
		if (ret == 0)
			FIXME("unhandled packet type\n");
		if (Header->common.flags & RPC_FLG_OBJECT_UUID)
			ret += sizeof(UUID);
	} else {
		TRACE("invalid packet type\n");
	}

	return ret;
}

