#if 1
#include <WinSock2.h>
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
	if (tcpc == NULL)
		return NULL;
	tcpc->sock = -1;
	if (socketpair(PF_UNIX, SOCK_STREAM, 0, tcpc->cancel_fds) < 0)
	{
		ERR("socketpair() failed: %s\n", strerror(errno));
		HeapFree(GetProcessHeap(), 0, tcpc);
		return NULL;
	}
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
		ERR("getaddrinfo for %s:%s failed: %s\n", Connection->NetworkAddr,
			Connection->Endpoint, gai_strerror(ret));
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
		setsockopt(sock, SOL_TCP, TCP_NODELAY, &val, sizeof(val));
		fcntl(sock, F_SETFL, O_NONBLOCK); /* make socket nonblocking */

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
		ERR("getaddrinfo for port %s failed: %s\n", endpoint,
			gai_strerror(ret));
		if ((ret == EAI_SERVICE) || (ret == EAI_NONAME))
			return RPC_S_INVALID_ENDPOINT_FORMAT;
		return RPC_S_CANT_CREATE_ENDPOINT;
	}

	for (ai_cur = ai; ai_cur; ai_cur = ai_cur->ai_next)
	{
		RpcConnection_tcp *tcpc;
		RPC_STATUS create_status;

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
		ret = fcntl(sock, F_SETFL, O_NONBLOCK);
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

	addrsize = sizeof(address);
	ret = accept(server->sock, (struct sockaddr*) &address, &addrsize);
	if (ret < 0)
	{
		ERR("Failed to accept a TCP connection: error %d\n", ret);
		return RPC_S_OUT_OF_RESOURCES;
	}
	/* reset to blocking behaviour */
	fcntl(ret, F_SETFL, 0);
	client->sock = ret;
	TRACE("Accepted a new TCP connection\n");
	return RPC_S_OK;
}

static int rpcrt4_conn_tcp_read(RpcConnection *Connection,
								void *buffer, unsigned int count)
{
	RpcConnection_tcp *tcpc = (RpcConnection_tcp *) Connection;
	int bytes_read = 0;
	do
	{
		int r = recv(tcpc->sock, (char *)buffer + bytes_read, count - bytes_read, 0);
		if (!r)
			return -1;
		else if (r > 0)
			bytes_read += r;
		else if (errno != EAGAIN)
		{
			WARN("recv() failed: %s\n", strerror(errno));
			return -1;
		}
		else
		{
			struct pollfd pfds[2];
			pfds[0].fd = tcpc->sock;
			pfds[0].events = POLLIN;
			pfds[1].fd = tcpc->cancel_fds[0];
			pfds[1].events = POLLIN;
			if (poll(pfds, 2, -1 /* infinite */) == -1 && errno != EINTR)
			{
				ERR("poll() failed: %s\n", strerror(errno));
				return -1;
			}
			if (pfds[1].revents & POLLIN) /* canceled */
			{
				char dummy;
				read(pfds[1].fd, &dummy, sizeof(dummy));
				return -1;
			}
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
	do
	{
		int r = send(tcpc->sock, (const char *)buffer + bytes_written, count - bytes_written, 0);
		if (r >= 0)
			bytes_written += r;
		else if (errno != EAGAIN)
			return -1;
		else
		{
			struct pollfd pfd;
			pfd.fd = tcpc->sock;
			pfd.events = POLLOUT;
			if (poll(&pfd, 1, -1 /* infinite */) == -1 && errno != EINTR)
			{
				ERR("poll() failed: %s\n", strerror(errno));
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
	close(tcpc->cancel_fds[0]);
	close(tcpc->cancel_fds[1]);
	return 0;
}

static void rpcrt4_conn_tcp_cancel_call(RpcConnection *Connection)
{
	RpcConnection_tcp *tcpc = (RpcConnection_tcp *) Connection;
	char dummy = 1;

	TRACE("%p\n", Connection);

	write(tcpc->cancel_fds[1], &dummy, 1);
}

static int rpcrt4_conn_tcp_wait_for_incoming_data(RpcConnection *Connection)
{
	RpcConnection_tcp *tcpc = (RpcConnection_tcp *) Connection;
	struct pollfd pfds[2];

	TRACE("%p\n", Connection);

	pfds[0].fd = tcpc->sock;
	pfds[0].events = POLLIN;
	pfds[1].fd = tcpc->cancel_fds[0];
	pfds[1].events = POLLIN;
	if (poll(pfds, 2, -1 /* infinite */) == -1 && errno != EINTR)
	{
		ERR("poll() failed: %s\n", strerror(errno));
		return -1;
	}
	if (pfds[1].revents & POLLIN) /* canceled */
	{
		char dummy;
		read(pfds[1].fd, &dummy, sizeof(dummy));
		return -1;
	}

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
			ERR("getaddrinfo failed: %s\n", gai_strerror(ret));
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

static RPC_STATUS rpcrt4_ncacn_ip_tcp_parse_top_of_tower(const unsigned char *tower_data,
														 size_t tower_size,
														 char **networkaddr,
														 char **endpoint)
{
	const twr_tcp_floor_t *tcp_floor = (const twr_tcp_floor_t *)tower_data;
	const twr_ipv4_floor_t *ipv4_floor;
	struct in_addr in_addr;

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
		if (!inet_ntop(AF_INET, &in_addr, *networkaddr, INET_ADDRSTRLEN))
		{
			ERR("inet_ntop: %s\n", strerror(errno));
			I_RpcFree(*networkaddr);
			*networkaddr = NULL;
			if (endpoint)
			{
				I_RpcFree(*endpoint);
				*endpoint = NULL;
			}
			return EPT_S_NOT_REGISTERED;
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
	if (ps)
	{
		int fds[2];
		if (!socketpair(PF_UNIX, SOCK_DGRAM, 0, fds))
		{
			fcntl(fds[0], F_SETFL, O_NONBLOCK);
			fcntl(fds[1], F_SETFL, O_NONBLOCK);
			ps->mgr_event_rcv = fds[0];
			ps->mgr_event_snd = fds[1];
		}
		else
		{
			ERR("socketpair failed with error %s\n", strerror(errno));
			HeapFree(GetProcessHeap(), 0, ps);
			return NULL;
		}
	}
	return &ps->common;
}

static void rpcrt4_protseq_sock_signal_state_changed(RpcServerProtseq *protseq)
{
	RpcServerProtseq_sock *sockps = CONTAINING_RECORD(protseq, RpcServerProtseq_sock, common);
	char dummy = 1;
	write(sockps->mgr_event_snd, &dummy, sizeof(dummy));
}

static void *rpcrt4_protseq_sock_get_wait_array(RpcServerProtseq *protseq, void *prev_array, unsigned int *count)
{
	struct pollfd *poll_info = prev_array;
	RpcConnection_tcp *conn;
	RpcServerProtseq_sock *sockps = CONTAINING_RECORD(protseq, RpcServerProtseq_sock, common);

	EnterCriticalSection(&protseq->cs);

	/* open and count connections */
	*count = 1;
	conn = (RpcConnection_tcp *)protseq->conn;
	while (conn) {
		if (conn->sock != -1)
			(*count)++;
		conn = (RpcConnection_tcp *)conn->common.Next;
	}

	/* make array of connections */
	if (poll_info)
		poll_info = HeapReAlloc(GetProcessHeap(), 0, poll_info, *count*sizeof(*poll_info));
	else
		poll_info = HeapAlloc(GetProcessHeap(), 0, *count*sizeof(*poll_info));
	if (!poll_info)
	{
		ERR("couldn't allocate poll_info\n");
		LeaveCriticalSection(&protseq->cs);
		return NULL;
	}

	poll_info[0].fd = sockps->mgr_event_rcv;
	poll_info[0].events = POLLIN;
	*count = 1;
	conn =  CONTAINING_RECORD(protseq->conn, RpcConnection_tcp, common);
	while (conn) {
		if (conn->sock != -1)
		{
			poll_info[*count].fd = conn->sock;
			poll_info[*count].events = POLLIN;
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
	struct pollfd *poll_info = wait_array;
	int ret, i;
	RpcConnection *cconn;
	RpcConnection_tcp *conn;

	if (!poll_info)
		return -1;

	ret = poll(poll_info, count, -1);
	if (ret < 0)
	{
		ERR("poll failed with error %d\n", ret);
		return -1;
	}

	for (i = 0; i < count; i++)
		if (poll_info[i].revents & POLLIN)
		{
			/* RPC server event */
			if (i == 0)
			{
				char dummy;
				read(poll_info[0].fd, &dummy, sizeof(dummy));
				return 0;
			}

			/* find which connection got a RPC */
			EnterCriticalSection(&protseq->cs);
			conn = CONTAINING_RECORD(protseq->conn, RpcConnection_tcp, common);
			while (conn) {
				if (poll_info[i].fd == conn->sock) break;
				conn = CONTAINING_RECORD(conn->common.Next, RpcConnection_tcp, common);
			}
			cconn = NULL;
			if (conn)
				RPCRT4_SpawnConnection(&cconn, &conn->common);
			else
				ERR("failed to locate connection for fd %d\n", poll_info[i].fd);
			LeaveCriticalSection(&protseq->cs);
			if (cconn)
				RPCRT4_new_client(cconn);
			else
				return -1;
		}

		return 1;
}

#endif//_TXF_MODIFY_

