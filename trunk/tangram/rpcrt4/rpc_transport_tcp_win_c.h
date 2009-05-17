#if 1


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
static int is_wsa_started = FALSE;
#define GAI_STRERROR_BUFFER_SIZE 1024
static char * _gai_strerror(
							IN int ecode)
{
	DWORD dwMsgLen;
	static char buff[GAI_STRERROR_BUFFER_SIZE + 1];

	dwMsgLen = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM
		|FORMAT_MESSAGE_IGNORE_INSERTS
		|FORMAT_MESSAGE_MAX_WIDTH_MASK,
		NULL,
		ecode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)buff,
		GAI_STRERROR_BUFFER_SIZE,
		NULL);

	return buff;
}

//#ifndef _TXF_MODIFY_
/**** ncacn_ip_tcp support ****/

typedef struct _RpcConnection_tcp
{
	RpcConnection common;
	int sock;
	int cancel_fds[2];
} RpcConnection_tcp;

static RpcConnection *rpcrt4_conn_tcp_alloc(void)
{
	RpcConnection_tcp *tcpc;
	tcpc = HeapAlloc(GetProcessHeap(), 0, sizeof(RpcConnection_tcp));

	if(0 == is_wsa_started)
	{
		WSADATA wsaData;	
		WSAStartup( MAKEWORD(2, 2), &wsaData );

		is_wsa_started=1;
	}
	if (tcpc == NULL)
		return NULL;
	tcpc->sock = -1;
#ifdef _BREAK_WITH_SOCKET_
	if (socketpair(PF_UNIX, SOCK_STREAM, 0, tcpc->cancel_fds) < 0)
	{
		ERR("socketpair() failed: %s\n", strerror(errno));
		HeapFree(GetProcessHeap(), 0, tcpc);
		return NULL;
	}
#else
	tcpc->cancel_fds[0] = 0;
#endif
	return &tcpc->common;
}

static RPC_STATUS rpcrt4_ncacn_ip_tcp_open(RpcConnection* Connection)
{
	RpcConnection_tcp *tcpc = (RpcConnection_tcp *) Connection;
	int sock;
	int ret;
	struct addrinfo *ai;
	struct addrinfo *ai_cur;
	struct addrinfo hints;

	TRACE("(%s, %s)\n", Connection->NetworkAddr, Connection->Endpoint);

	if (tcpc->sock != -1)
		return RPC_S_OK;

	hints.ai_flags          = 0;
	hints.ai_family         = PF_UNSPEC;
	hints.ai_socktype       = SOCK_STREAM;
	hints.ai_protocol       = IPPROTO_TCP;
	hints.ai_addrlen        = 0;
	hints.ai_addr           = NULL;
	hints.ai_canonname      = NULL;
	hints.ai_next           = NULL;

	ret = getaddrinfo(Connection->NetworkAddr, Connection->Endpoint, &hints, &ai);
	if (ret)
	{
		ERR("getaddrinfo for %s:%s failed: %s\n", Connection->NetworkAddr,Connection->Endpoint, _gai_strerror(ret));
		return RPC_S_SERVER_UNAVAILABLE;
	}

	for (ai_cur = ai; ai_cur; ai_cur = ai_cur->ai_next)
	{
		int val;

		if (TRACE_ON(rpc))
		{
			char host[256];
			char service[256];
			getnameinfo(ai_cur->ai_addr, ai_cur->ai_addrlen,
				host, sizeof(host), service, sizeof(service),
				NI_NUMERICHOST | NI_NUMERICSERV);
			TRACE("trying %s:%s\n", host, service);
		}

		sock = socket(ai_cur->ai_family, ai_cur->ai_socktype, ai_cur->ai_protocol);
		if (sock == -1)
		{
			WARN("socket() failed: %s\n", strerror(errno));
			continue;
		}

		if (0>connect(sock, ai_cur->ai_addr, ai_cur->ai_addrlen))
		{
			WARN("connect() failed: %s\n", strerror(errno));
			closesocket(sock);
			continue;
		}

		/* RPC depends on having minimal latency so disable the Nagle algorithm */
		val = 1;
		setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&val, sizeof(int));
		val = 1;
		ioctlsocket(sock, FIONBIO, &val);
				
		tcpc->sock = sock;

		freeaddrinfo(ai);
		TRACE("connected\n");
		return RPC_S_OK;
	}

	freeaddrinfo(ai);
	ERR("couldn't connect to %s:%s\n", Connection->NetworkAddr, Connection->Endpoint);
	return RPC_S_SERVER_UNAVAILABLE;
}


static RPC_STATUS rpcrt4_protseq_ncacn_ip_tcp_open_endpoint(RpcServerProtseq *protseq, LPSTR endpoint)
{
	RPC_STATUS status = RPC_S_CANT_CREATE_ENDPOINT;
	int sock;
	int ret;
	struct addrinfo *ai;
	struct addrinfo *ai_cur;
	struct addrinfo hints;
	RpcConnection *first_connection = NULL;

	TRACE("(%p, %s)\n", protseq, endpoint);

	hints.ai_flags          = AI_PASSIVE /* for non-localhost addresses */;
	hints.ai_family         = PF_UNSPEC;
	hints.ai_socktype       = SOCK_STREAM;
	hints.ai_protocol       = IPPROTO_TCP;
	hints.ai_addrlen        = 0;
	hints.ai_addr           = NULL;
	hints.ai_canonname      = NULL;
	hints.ai_next           = NULL;

	ret = getaddrinfo(NULL, endpoint, &hints, &ai);
	if (ret)
	{
		ERR("getaddrinfo for port %s failed: %s\n", endpoint,_gai_strerror(ret));

		if ((ret == EAI_SERVICE) || (ret == EAI_NONAME))
			return RPC_S_INVALID_ENDPOINT_FORMAT;
		return RPC_S_CANT_CREATE_ENDPOINT;
	}

	for (ai_cur = ai; ai_cur; ai_cur = ai_cur->ai_next)
	{
		RpcConnection_tcp *tcpc;
		RPC_STATUS create_status;
		u_long one = 1;

		if (TRACE_ON(rpc))
		{
			char host[256];
			char service[256];
			getnameinfo(ai_cur->ai_addr, ai_cur->ai_addrlen,
				host, sizeof(host), service, sizeof(service),
				NI_NUMERICHOST | NI_NUMERICSERV);
			TRACE("trying %s:%s\n", host, service);
		}

		sock = socket(ai_cur->ai_family, ai_cur->ai_socktype, ai_cur->ai_protocol);
		if (sock == -1)
		{
			WARN("socket() failed: %s\n", strerror(errno));
			status = RPC_S_CANT_CREATE_ENDPOINT;
			continue;
		}

		ret = bind(sock, ai_cur->ai_addr, ai_cur->ai_addrlen);
		if (ret < 0)
		{
			WARN("bind failed: %s\n", strerror(errno));
			closesocket(sock);
			if (errno == EADDRINUSE)
				status = RPC_S_DUPLICATE_ENDPOINT;
			else
				status = RPC_S_CANT_CREATE_ENDPOINT;
			continue;
		}
		create_status = RPCRT4_CreateConnection((RpcConnection **)&tcpc, TRUE,
			protseq->Protseq, NULL,
			endpoint, NULL, NULL, NULL);
		if (create_status != RPC_S_OK)
		{
			closesocket(sock);
			status = create_status;
			continue;
		}

		tcpc->sock = sock;
		ret = listen(sock, protseq->MaxCalls);
		if (ret < 0)
		{
			WARN("listen failed: %s\n", strerror(errno));
			RPCRT4_DestroyConnection(&tcpc->common);
			status = RPC_S_OUT_OF_RESOURCES;
			continue;
		}
		/* need a non-blocking socket, otherwise accept() has a potential
		* race-condition (poll() says it is readable, connection drops,
		* and accept() blocks until the next connection comes...)
		*/
		
		ret = ioctlsocket(sock, FIONBIO, &one);
		if (ret < 0)
		{
			WARN("couldn't make socket non-blocking, error %d\n", ret);
			RPCRT4_DestroyConnection(&tcpc->common);
			status = RPC_S_OUT_OF_RESOURCES;
			continue;
		}

		tcpc->common.Next = first_connection;
		first_connection = &tcpc->common;
	}

	freeaddrinfo(ai);

	/* if at least one connection was created for an endpoint then
	* return success */
	if (first_connection)
	{
		RpcConnection *conn;

		/* find last element in list */
		for (conn = first_connection; conn->Next; conn = conn->Next)
			;

		EnterCriticalSection(&protseq->cs);
		conn->Next = protseq->conn;
		protseq->conn = first_connection;
		LeaveCriticalSection(&protseq->cs);

		TRACE("listening on %s\n", endpoint);
		return RPC_S_OK;
	}

	ERR("couldn't listen on port %s\n", endpoint);
	return status;
}

static RPC_STATUS rpcrt4_conn_tcp_handoff(RpcConnection *old_conn, RpcConnection *new_conn)
{
	int ret;
	struct sockaddr_in address;
	socklen_t addrsize;
	RpcConnection_tcp *server = (RpcConnection_tcp*) old_conn;
	RpcConnection_tcp *client = (RpcConnection_tcp*) new_conn;
	u_long one = 0;

	addrsize = sizeof(address);
	ret = accept(server->sock, (struct sockaddr*) &address, &addrsize);
	if (ret < 0)
	{
		ERR("Failed to accept a TCP connection: error %d\n", ret);
		return RPC_S_OUT_OF_RESOURCES;
	}
	client->sock = ret;

	/* reset to blocking behaviour */
	ret = ioctlsocket(server->sock, FIONBIO, &one);
	
	TRACE("Accepted a new TCP connection\n");
	return RPC_S_OK;
}

static int rpcrt4_conn_tcp_read(RpcConnection *Connection,
								void *buffer, unsigned int count)
{
	RpcConnection_tcp *tcpc = (RpcConnection_tcp *) Connection;
	int bytes_read = 0;
	int ret;
	int err = 0;
	do
	{
		int r = recv(tcpc->sock, (char *)buffer + bytes_read, count - bytes_read, 0);

		if (!r)
			return -1;
		else if (r > 0)
		{
			if(r > count - bytes_read)
			{
				assert(0);
			}
			bytes_read += r;
		}
		else if ((err = WSAGetLastError()) != WSAEWOULDBLOCK)
		{
			WARN("recv() failed: %s\n", _gai_strerror(err));
			return -1;
		}
		else
		{
#ifdef _BREAK_WITH_SOCKET_
			int maxfd = (tcpc->sock > tcpc->cancel_fds[0])?(tcpc->sock):(tcpc->cancel_fds[0]);
			fd_set r_set;
			FD_ZERO(&r_set);
			FD_SET(tcpc->sock,&r_set);
			FD_SET(tcpc->cancel_fds[0],&r_set);
			
			if (select(maxfd+1, &r_set,NULL,NULL, 0 /* infinite */) == -1 )
			{
				err = WSAGetLastError();
				ERR("poll() failed: %s\n", _gai_strerror(err));
				return -1;
			}
			if (FD_ISSET(tcpc->cancel_fds[0],&r_set)) /* canceled */
			{
				char dummy;
				read(tcpc->cancel_fds[0], &dummy, sizeof(dummy));
				return -1;
			}
#else
			
			fd_set r_set;
		
			do 
			{			
				struct timeval  tv;
				tv.tv_usec = 20;
				tv.tv_sec = 0;

				FD_ZERO(&r_set);
				FD_SET(tcpc->sock,&r_set);
				
				ret = select(tcpc->sock+1, &r_set,NULL,NULL, &tv);
			} while(	(ret == 0 && tcpc->cancel_fds[0] == 0));

			if(tcpc->cancel_fds[0] != 0)
			{
				tcpc->cancel_fds[0] = 0;
				return -1;
			}
			
			if (ret == -1 )
			{
				err = WSAGetLastError();
				ERR("poll() failed: %s\n", _gai_strerror(err));
				return -1;
			}

#endif
		}
	} while (bytes_read != count);
	TRACE("%d %p %u -> %d\n", tcpc->sock, buffer, count, bytes_read);
	return bytes_read;
}

static int rpcrt4_conn_tcp_write(RpcConnection *Connection,
								 const void *buffer, unsigned int count)
{
	RpcConnection_tcp *tcpc = (RpcConnection_tcp *) Connection;
	int bytes_written = 0;
	int err= 0;
	do
	{
		int r = send(tcpc->sock, (const char *)buffer + bytes_written, count - bytes_written, 0);
		if (r >= 0)
			bytes_written += r;
		else if ((err = WSAGetLastError()) != WSAEWOULDBLOCK)
			return -1;
		else
		{
			fd_set wset;
			FD_ZERO(&wset);
			FD_SET(tcpc->sock,&wset);
			if (select(tcpc->sock+1,NULL, &wset,NULL, 0 /* infinite */) == -1 )
			{
				err = WSAGetLastError();
				ERR("poll() failed: %s\n", err);
				return -1;
			}
		}
	} while (bytes_written != count);
	TRACE("%d %p %u -> %d\n", tcpc->sock, buffer, count, bytes_written);
	return bytes_written;
}

static int rpcrt4_conn_tcp_close(RpcConnection *Connection)
{
	RpcConnection_tcp *tcpc = (RpcConnection_tcp *) Connection;

	TRACE("%d\n", tcpc->sock);

	if (tcpc->sock != -1)
		closesocket(tcpc->sock);
	tcpc->sock = -1;
#ifdef _BREAK_WITH_SOCKET_
	close(tcpc->cancel_fds[0]);
	close(tcpc->cancel_fds[1]);
#else
#endif
	return 0;
}

static void rpcrt4_conn_tcp_cancel_call(RpcConnection *Connection)
{
	RpcConnection_tcp *tcpc = (RpcConnection_tcp *) Connection;
#ifdef _BREAK_WITH_SOCKET_
	char dummy = 1;

	TRACE("%p\n", Connection);

	write(tcpc->cancel_fds[1], &dummy, 1);
#else
	tcpc->cancel_fds[0] = 1;
#endif
}

static int rpcrt4_conn_tcp_wait_for_incoming_data(RpcConnection *Connection)
{
	RpcConnection_tcp *tcpc = (RpcConnection_tcp *) Connection;
	int err = -1;
#ifdef _BREAK_WITH_SOCKET_	
	int maxfd = (tcpc->sock > tcpc->cancel_fds[0])?(tcpc->sock):(tcpc->cancel_fds[0]);
	fd_set r_set;

	TRACE("%p\n", Connection);

	FD_ZERO(&r_set);
	FD_SET(tcpc->sock,&r_set);
	FD_SET(tcpc->cancel_fds[0],&r_set);

	if (select(maxfd+1, &r_set,NULL,NULL, 0 /* infinite */) == -1 )
	{
		err = WSAGetLastError();
		ERR("poll() failed: %s\n", _gai_strerror(err));
		return -1;
	}
	if (FD_ISSET(tcpc->cancel_fds[0],&r_set)) /* canceled */
	{
		char dummy;
		read(tcpc->cancel_fds[0], &dummy, sizeof(dummy));
		return -1;
	}
#else
	int ret = -1;
	fd_set r_set;

	TRACE("%p\n", Connection);


	
	do 
	{
		struct timeval  tv;
		tv.tv_usec = 20;
		tv.tv_sec = 0;

		FD_ZERO(&r_set);
		FD_SET(tcpc->sock,&r_set);

		ret = select(tcpc->sock+1, &r_set,NULL,NULL, &tv /* infinite */);
	} while( (ret == 0 && tcpc->cancel_fds[0] == 0) );

	if(tcpc->cancel_fds[0] != 0)
	{
		tcpc->cancel_fds[0] = 0;
		return -1;
	}
	
	if (ret == -1 )
	{
		err = WSAGetLastError();
		ERR("poll() failed: %s\n", _gai_strerror(err));
		return -1;
	}

#endif
	return 0;
}

static size_t rpcrt4_ncacn_ip_tcp_get_top_of_tower(unsigned char *tower_data,
												   const char *networkaddr,
												   const char *endpoint)
{
	twr_tcp_floor_t *tcp_floor;
	twr_ipv4_floor_t *ipv4_floor;
	struct addrinfo *ai;
	struct addrinfo hints;
	int ret;
	size_t size = sizeof(*tcp_floor) + sizeof(*ipv4_floor);

	TRACE("(%p, %s, %s)\n", tower_data, networkaddr, endpoint);

	if (!tower_data)
		return size;

	tcp_floor = (twr_tcp_floor_t *)tower_data;
	tower_data += sizeof(*tcp_floor);

	ipv4_floor = (twr_ipv4_floor_t *)tower_data;

	tcp_floor->count_lhs = sizeof(tcp_floor->protid);
	tcp_floor->protid = EPM_PROTOCOL_TCP;
	tcp_floor->count_rhs = sizeof(tcp_floor->port);

	ipv4_floor->count_lhs = sizeof(ipv4_floor->protid);
	ipv4_floor->protid = EPM_PROTOCOL_IP;
	ipv4_floor->count_rhs = sizeof(ipv4_floor->ipv4addr);

	hints.ai_flags          = AI_NUMERICHOST;
	/* FIXME: only support IPv4 at the moment. how is IPv6 represented by the EPM? */
	hints.ai_family         = PF_INET;
	hints.ai_socktype       = SOCK_STREAM;
	hints.ai_protocol       = IPPROTO_TCP;
	hints.ai_addrlen        = 0;
	hints.ai_addr           = NULL;
	hints.ai_canonname      = NULL;
	hints.ai_next           = NULL;

	ret = getaddrinfo(networkaddr, endpoint, &hints, &ai);
	if (ret)
	{
		ret = getaddrinfo("0.0.0.0", endpoint, &hints, &ai);
		if (ret)
		{
			ERR("getaddrinfo failed: %s\n", _gai_strerror(ret));
			return 0;
		}
	}

	if (ai->ai_family == PF_INET)
	{
		const struct sockaddr_in *sin = (const struct sockaddr_in *)ai->ai_addr;
		tcp_floor->port = sin->sin_port;
		ipv4_floor->ipv4addr = sin->sin_addr.s_addr;
	}
	else
	{
		ERR("unexpected protocol family %d\n", ai->ai_family);
		return 0;
	}

	freeaddrinfo(ai);

	return size;
}
#ifndef INET_ADDRSTRLEN
# define INET_ADDRSTRLEN        16
#endif /* INET_ADDRSTRLEN */

static RPC_STATUS rpcrt4_ncacn_ip_tcp_parse_top_of_tower(const unsigned char *tower_data,
														 size_t tower_size,
														 char **networkaddr,
														 char **endpoint)
{
	const twr_tcp_floor_t *tcp_floor = (const twr_tcp_floor_t *)tower_data;
	const twr_ipv4_floor_t *ipv4_floor;
	struct in_addr in_addr;
	int err= 0;

	TRACE("(%p, %d, %p, %p)\n", tower_data, (int)tower_size, networkaddr, endpoint);

	if (tower_size < sizeof(*tcp_floor))
		return EPT_S_NOT_REGISTERED;

	tower_data += sizeof(*tcp_floor);
	tower_size -= sizeof(*tcp_floor);

	if (tower_size < sizeof(*ipv4_floor))
		return EPT_S_NOT_REGISTERED;

	ipv4_floor = (const twr_ipv4_floor_t *)tower_data;

	if ((tcp_floor->count_lhs != sizeof(tcp_floor->protid)) ||
		(tcp_floor->protid != EPM_PROTOCOL_TCP) ||
		(tcp_floor->count_rhs != sizeof(tcp_floor->port)) ||
		(ipv4_floor->count_lhs != sizeof(ipv4_floor->protid)) ||
		(ipv4_floor->protid != EPM_PROTOCOL_IP) ||
		(ipv4_floor->count_rhs != sizeof(ipv4_floor->ipv4addr)))
		return EPT_S_NOT_REGISTERED;

	if (endpoint)
	{
		*endpoint = I_RpcAllocate(6 /* sizeof("65535") + 1 */);
		if (!*endpoint)
			return RPC_S_OUT_OF_RESOURCES;
		sprintf(*endpoint, "%u", ntohs(tcp_floor->port));
	}

	if (networkaddr)
	{
		*networkaddr = I_RpcAllocate(INET_ADDRSTRLEN);
		if (!*networkaddr)
		{
			if (endpoint)
			{
				I_RpcFree(*endpoint);
				*endpoint = NULL;
			}
			return RPC_S_OUT_OF_RESOURCES;
		}
		in_addr.s_addr = ipv4_floor->ipv4addr;
		//if (!inet_ntop(AF_INET, &in_addr, *networkaddr, INET_ADDRSTRLEN))
		{
			char* addr = inet_ntoa(in_addr);
			if(NULL == addr)
			{
				err = WSAGetLastError();
				ERR("inet_ntop: %s\n", _gai_strerror(err));
				I_RpcFree(*networkaddr);
				*networkaddr = NULL;
				if (endpoint)
				{
					I_RpcFree(*endpoint);
					*endpoint = NULL;
				}
				return EPT_S_NOT_REGISTERED;
			}
			memcpy(*networkaddr,addr,INET_ADDRSTRLEN);
		}
	}

	return RPC_S_OK;
}

typedef struct _RpcServerProtseq_sock
{
	RpcServerProtseq common;
	int mgr_event_rcv;
	int mgr_event_snd;
} RpcServerProtseq_sock;

static RpcServerProtseq *rpcrt4_protseq_sock_alloc(void)
{
	RpcServerProtseq_sock *ps = HeapAlloc(GetProcessHeap(), 0, sizeof(*ps));
	int err = 0;

	if(0 == is_wsa_started)
	{
		WSADATA wsaData;	
		WSAStartup( MAKEWORD(2, 2), &wsaData );

		is_wsa_started=1;
	}

	if (ps)
	{
#ifdef _BREAK_WITH_SOCKET_
		int fds[2];
		if (!socketpair(PF_UNIX, SOCK_DGRAM, 0, fds))
		{
			u_long one = 1;
			ioctlsocket(fds[0], FIONBIO, &one);
			ioctlsocket(fds[1], FIONBIO, &one);
			ps->mgr_event_rcv = fds[0];
			ps->mgr_event_snd = fds[1];
		}
		else
		{
			ERR("socketpair failed with error %s\n", strerror(errno));
			HeapFree(GetProcessHeap(), 0, ps);
			return NULL;
		}
#else
		ps->mgr_event_snd = 0;
#endif
	}
	return &ps->common;
}

static void rpcrt4_protseq_sock_signal_state_changed(RpcServerProtseq *protseq)
{
	RpcServerProtseq_sock *sockps = CONTAINING_RECORD(protseq, RpcServerProtseq_sock, common);
#ifdef _BREAK_WITH_SOCKET_
	char dummy = 1;
	write(sockps->mgr_event_snd, &dummy, sizeof(dummy));
#else
	sockps->mgr_event_snd = 1;
#endif
}

static void *rpcrt4_protseq_sock_get_wait_array(RpcServerProtseq *protseq, void *prev_array, unsigned int *count)
{
	fd_set *poll_info = prev_array;
	RpcConnection_tcp *conn;
	RpcServerProtseq_sock *sockps = CONTAINING_RECORD(protseq, RpcServerProtseq_sock, common);

	EnterCriticalSection(&protseq->cs);

	/* make array of connections */
	if (!poll_info)
	{
		poll_info = HeapAlloc(GetProcessHeap(), 0, sizeof(fd_set));
		FD_ZERO(poll_info);
	}

	if (!poll_info)
	{
		ERR("couldn't allocate poll_info\n");
		LeaveCriticalSection(&protseq->cs);
		return NULL;
	}
#ifdef _BREAK_WITH_SOCKET_
	FD_SET(sockps->mgr_event_rcv,poll_info);
	*count = 1;
#else
	*count = 0;
#endif
	conn =  CONTAINING_RECORD(protseq->conn, RpcConnection_tcp, common);
	while (conn) {
		if (conn->sock != -1)
		{
			FD_SET(conn->sock,poll_info);
			(*count)++;
		}
		conn = CONTAINING_RECORD(conn->common.Next, RpcConnection_tcp, common);
	}
	LeaveCriticalSection(&protseq->cs);
	return poll_info;
}

static void rpcrt4_protseq_sock_free_wait_array(RpcServerProtseq *protseq, void *array)
{
	HeapFree(GetProcessHeap(), 0, array);
}

static int rpcrt4_protseq_sock_wait_for_new_connection(RpcServerProtseq *protseq, unsigned int count, void *wait_array)
{
	fd_set *poll_info = wait_array;
	fd_set r_set;
	int ret;
	int err = -1;
	RpcConnection *cconn;
	RpcConnection_tcp *conn;
	RpcServerProtseq_sock *sockps = CONTAINING_RECORD(protseq, RpcServerProtseq_sock, common);

	if (!poll_info)
		return -1;
#ifdef _BREAK_WITH_SOCKET_
	ret = select(1,poll_info, NULL,NULL,0);
#else
	conn = CONTAINING_RECORD(protseq->conn, RpcConnection_tcp, common);
	if(conn)
	{
		do{
			
			struct timeval  tv;
			tv.tv_usec = 20;
			tv.tv_sec = 0;
			r_set = *poll_info;

			ret = select(1,&r_set, NULL,NULL,&tv);
		}while( (ret == 0 && sockps->mgr_event_snd == 0));
	}
	else
	{
		assert(0);
	}

	if(sockps->mgr_event_snd != 0)
	{
		sockps->mgr_event_snd = 0;
		return 0;
	}

#endif
	if (ret < 0)
	{
		int err =  WSAGetLastError();
		ERR("poll failed with error %d,%s\n", ret,_gai_strerror(err));
		return -1;
	}

	if(ret > 0)
	{
#ifdef _BREAK_WITH_SOCKET_
		if (FD_ISSET(sockps->mgr_event_rcv,&r_set))
		{
			char dummy;
			read(sockps->mgr_event_rcv, &dummy, sizeof(dummy));
			return 0;
		}
#endif

		/* find which connection got a RPC */
		EnterCriticalSection(&protseq->cs);
		conn = CONTAINING_RECORD(protseq->conn, RpcConnection_tcp, common);
		while (conn) {
			if (FD_ISSET(conn->sock,&r_set)) break;
			conn = CONTAINING_RECORD(conn->common.Next, RpcConnection_tcp, common);
		}
		cconn = NULL;
		if (conn)
			RPCRT4_SpawnConnection(&cconn, &conn->common);
		else
			ERR("failed to locate connection for fd %d\n", conn->sock);
		LeaveCriticalSection(&protseq->cs);
		if (cconn)
			RPCRT4_new_client(cconn);
		else
			return -1;
	}
	return 1;
}


#endif//_TXF_MODIFY_

