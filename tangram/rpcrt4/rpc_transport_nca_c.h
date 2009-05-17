
/**** ncacn_np support ****/

typedef struct _RpcConnection_np
{
	RpcConnection common;
	HANDLE pipe;
	OVERLAPPED ovl;
	BOOL listening;
} RpcConnection_np;

static RpcConnection *rpcrt4_conn_np_alloc(void)
{
	RpcConnection_np *npc = HeapAlloc(GetProcessHeap(), 0, sizeof(RpcConnection_np));
	if (npc)
	{
		npc->pipe = NULL;
		memset(&npc->ovl, 0, sizeof(npc->ovl));
		npc->listening = FALSE;
	}
	return &npc->common;
}

static RPC_STATUS rpcrt4_conn_listen_pipe(RpcConnection_np *npc)
{
	if (npc->listening)
		return RPC_S_OK;

	npc->listening = TRUE;
	for (;;)
	{
		if (ConnectNamedPipe(npc->pipe, &npc->ovl))
			return RPC_S_OK;

		switch(GetLastError())
		{
		case ERROR_PIPE_CONNECTED:
			SetEvent(npc->ovl.hEvent);
			return RPC_S_OK;
		case ERROR_IO_PENDING:
			/* will be completed in rpcrt4_protseq_np_wait_for_new_connection */
			return RPC_S_OK;
		case ERROR_NO_DATA_DETECTED:
			/* client has disconnected, retry */
			DisconnectNamedPipe( npc->pipe );
			break;
		default:
			npc->listening = FALSE;
			WARN("Couldn't ConnectNamedPipe (error was %d)\n", GetLastError());
			return RPC_S_OUT_OF_RESOURCES;
		}
	}
}

static RPC_STATUS rpcrt4_conn_create_pipe(RpcConnection *Connection, LPCSTR pname)
{
	RpcConnection_np *npc = (RpcConnection_np *) Connection;
	TRACE("listening on %s\n", pname);
#ifdef _TXF_MODIFY_
	npc->pipe = CreateNamedPipeA(pname, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
#else
	npc->pipe = CreateNamedPipeA(pname, PIPE_ACCESS_DUPLEX ,
#endif
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
		PIPE_UNLIMITED_INSTANCES,
		RPC_MAX_PACKET_SIZE, RPC_MAX_PACKET_SIZE, 5000, NULL);
	if (npc->pipe == INVALID_HANDLE_VALUE) {
		WARN("CreateNamedPipe failed with error %d\n", GetLastError());
		if (GetLastError() == ERROR_FILE_EXISTS)
			return RPC_S_DUPLICATE_ENDPOINT;
		else
			return RPC_S_CANT_CREATE_ENDPOINT;
	}

	memset(&npc->ovl, 0, sizeof(npc->ovl));
	npc->ovl.hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

	/* Note: we don't call ConnectNamedPipe here because it must be done in the
	* server thread as the thread must be alertable */
	return RPC_S_OK;
}

static RPC_STATUS rpcrt4_conn_open_pipe(RpcConnection *Connection, LPCSTR pname, BOOL wait)
{
	RpcConnection_np *npc = (RpcConnection_np *) Connection;
	HANDLE pipe;
	DWORD err, dwMode;

	TRACE("connecting to %s\n", pname);

	while (TRUE) {
		DWORD dwFlags = 0;
		if (Connection->QOS)
		{
			dwFlags = SECURITY_SQOS_PRESENT;
			switch (Connection->QOS->qos->ImpersonationType)
			{
			case RPC_C_IMP_LEVEL_DEFAULT:
				/* FIXME: what to do here? */
				break;
			case RPC_C_IMP_LEVEL_ANONYMOUS:
				dwFlags |= SECURITY_ANONYMOUS;
				break;
			case RPC_C_IMP_LEVEL_IDENTIFY:
				dwFlags |= SECURITY_IDENTIFICATION;
				break;
			case RPC_C_IMP_LEVEL_IMPERSONATE:
				dwFlags |= SECURITY_IMPERSONATION;
				break;
			case RPC_C_IMP_LEVEL_DELEGATE:
				dwFlags |= SECURITY_DELEGATION;
				break;
			}
			if (Connection->QOS->qos->IdentityTracking == RPC_C_QOS_IDENTIFY_DYNAMIC)
				dwFlags |= SECURITY_CONTEXT_TRACKING;
		}
		pipe = CreateFileA(pname, GENERIC_READ|GENERIC_WRITE, 0, NULL,
			OPEN_EXISTING, dwFlags, 0);
		if (pipe != INVALID_HANDLE_VALUE) break;
		err = GetLastError();
		if (err == ERROR_PIPE_BUSY) {
			TRACE("connection failed, error=%x\n", err);
			return RPC_S_SERVER_TOO_BUSY;
		}
		if (!wait || !WaitNamedPipeA(pname, NMPWAIT_WAIT_FOREVER)) {
			err = GetLastError();
			WARN("connection failed, error=%x\n", err);
			return RPC_S_SERVER_UNAVAILABLE;
		}
	}

	/* success */
	memset(&npc->ovl, 0, sizeof(npc->ovl));
	/* pipe is connected; change to message-read mode. */
	dwMode = PIPE_READMODE_MESSAGE;
	SetNamedPipeHandleState(pipe, &dwMode, NULL, NULL);
	npc->ovl.hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
	npc->pipe = pipe;

	return RPC_S_OK;
}

static RPC_STATUS rpcrt4_ncalrpc_open(RpcConnection* Connection)
{
	RpcConnection_np *npc = (RpcConnection_np *) Connection;
	static const char prefix[] = "\\\\.\\pipe\\lrpc\\";
	RPC_STATUS r;
	LPSTR pname;

	/* already connected? */
	if (npc->pipe)
		return RPC_S_OK;

	/* protseq=ncalrpc: supposed to use NT LPC ports,
	* but we'll implement it with named pipes for now */
	pname = I_RpcAllocate(strlen(prefix) + strlen(Connection->Endpoint) + 1);
	strcat(strcpy(pname, prefix), Connection->Endpoint);
	r = rpcrt4_conn_open_pipe(Connection, pname, TRUE);
	I_RpcFree(pname);

	return r;
}

static RPC_STATUS rpcrt4_protseq_ncalrpc_open_endpoint(RpcServerProtseq* protseq, LPSTR endpoint)
{
	static const char prefix[] = "\\\\.\\pipe\\lrpc\\";
	RPC_STATUS r;
	LPSTR pname;
	RpcConnection *Connection;

	r = RPCRT4_CreateConnection(&Connection, TRUE, protseq->Protseq, NULL,
		endpoint, NULL, NULL, NULL);
	if (r != RPC_S_OK)
		return r;

	/* protseq=ncalrpc: supposed to use NT LPC ports,
	* but we'll implement it with named pipes for now */
	pname = I_RpcAllocate(strlen(prefix) + strlen(Connection->Endpoint) + 1);
	strcat(strcpy(pname, prefix), Connection->Endpoint);
	r = rpcrt4_conn_create_pipe(Connection, pname);
	I_RpcFree(pname);

	EnterCriticalSection(&protseq->cs);
	Connection->Next = protseq->conn;
	protseq->conn = Connection;
	LeaveCriticalSection(&protseq->cs);

	return r;
}

static RPC_STATUS rpcrt4_ncacn_np_open(RpcConnection* Connection)
{
	RpcConnection_np *npc = (RpcConnection_np *) Connection;
	static const char prefix[] = "\\\\.";
	RPC_STATUS r;
	LPSTR pname;

	/* already connected? */
	if (npc->pipe)
		return RPC_S_OK;

	/* protseq=ncacn_np: named pipes */
	pname = I_RpcAllocate(strlen(prefix) + strlen(Connection->Endpoint) + 1);
	strcat(strcpy(pname, prefix), Connection->Endpoint);
	r = rpcrt4_conn_open_pipe(Connection, pname, FALSE);
	I_RpcFree(pname);

	return r;
}

static RPC_STATUS rpcrt4_protseq_ncacn_np_open_endpoint(RpcServerProtseq *protseq, LPSTR endpoint)
{
	static const char prefix[] = "\\\\.";
	RPC_STATUS r;
	LPSTR pname;
	RpcConnection *Connection;

	r = RPCRT4_CreateConnection(&Connection, TRUE, protseq->Protseq, NULL,
		endpoint, NULL, NULL, NULL);
	if (r != RPC_S_OK)
		return r;

	/* protseq=ncacn_np: named pipes */
	pname = I_RpcAllocate(strlen(prefix) + strlen(Connection->Endpoint) + 1);
	strcat(strcpy(pname, prefix), Connection->Endpoint);
	r = rpcrt4_conn_create_pipe(Connection, pname);
	I_RpcFree(pname);

	EnterCriticalSection(&protseq->cs);
	Connection->Next = protseq->conn;
	protseq->conn = Connection;
	LeaveCriticalSection(&protseq->cs);

	return r;
}

static void rpcrt4_conn_np_handoff(RpcConnection_np *old_npc, RpcConnection_np *new_npc)
{    
	/* because of the way named pipes work, we'll transfer the connected pipe
	* to the child, then reopen the server binding to continue listening */

	new_npc->pipe = old_npc->pipe;
	new_npc->ovl = old_npc->ovl;
	old_npc->pipe = 0;
	memset(&old_npc->ovl, 0, sizeof(old_npc->ovl));
	old_npc->listening = FALSE;
}

static RPC_STATUS rpcrt4_ncacn_np_handoff(RpcConnection *old_conn, RpcConnection *new_conn)
{
	RPC_STATUS status;
	LPSTR pname;
	static const char prefix[] = "\\\\.";

	rpcrt4_conn_np_handoff((RpcConnection_np *)old_conn, (RpcConnection_np *)new_conn);

	pname = I_RpcAllocate(strlen(prefix) + strlen(old_conn->Endpoint) + 1);
	strcat(strcpy(pname, prefix), old_conn->Endpoint);
	status = rpcrt4_conn_create_pipe(old_conn, pname);
	I_RpcFree(pname);

	return status;
}

static RPC_STATUS rpcrt4_ncalrpc_handoff(RpcConnection *old_conn, RpcConnection *new_conn)
{
	RPC_STATUS status;
	LPSTR pname;
	static const char prefix[] = "\\\\.\\pipe\\lrpc\\";

	TRACE("%s\n", old_conn->Endpoint);

	rpcrt4_conn_np_handoff((RpcConnection_np *)old_conn, (RpcConnection_np *)new_conn);

	pname = I_RpcAllocate(strlen(prefix) + strlen(old_conn->Endpoint) + 1);
	strcat(strcpy(pname, prefix), old_conn->Endpoint);
	status = rpcrt4_conn_create_pipe(old_conn, pname);
	I_RpcFree(pname);

	return status;
}

static int rpcrt4_conn_np_read(RpcConnection *Connection,
							   void *buffer, unsigned int count)
{
	RpcConnection_np *npc = (RpcConnection_np *) Connection;
	char *buf = buffer;
	BOOL ret = TRUE;
	DWORD err = 0;
	unsigned int bytes_left = count;

	memset((char*)buffer,0,count);

	while (bytes_left)
	{
		DWORD bytes_read = 0;
		ret = ReadFile(npc->pipe, buf, bytes_left, &bytes_read, NULL);
#ifdef _TXF_MODIFY_
		if(!ret)
		{
			err = GetLastError();
			if(err != ERROR_MORE_DATA)
			{
				break;
			}
			ret =  1;//表示读成功
		};
		if (!bytes_read)
			break;
		if(bytes_read > bytes_left)
		{
			return -1;
		}
#else
		if (!ret || !bytes_read)
			break;
#endif

		bytes_left -= bytes_read;
		buf += bytes_read;
	}
	return ret ? count : -1;
}

static int rpcrt4_conn_np_write(RpcConnection *Connection,
								const void *buffer, unsigned int count)
{
	RpcConnection_np *npc = (RpcConnection_np *) Connection;
	const char *buf = buffer;
	BOOL ret = TRUE;
	unsigned int bytes_left = count;

	while (bytes_left)
	{
		DWORD bytes_written;
		ret = WriteFile(npc->pipe, buf, bytes_left, &bytes_written, NULL);
		if (!ret || !bytes_written)
			break;
		bytes_left -= bytes_written;
		buf += bytes_written;
	}
	return ret ? count : -1;
}

static int rpcrt4_conn_np_close(RpcConnection *Connection)
{
	RpcConnection_np *npc = (RpcConnection_np *) Connection;
	if (npc->pipe) {
		FlushFileBuffers(npc->pipe);
		CloseHandle(npc->pipe);
		npc->pipe = 0;
	}
	if (npc->ovl.hEvent) {
		CloseHandle(npc->ovl.hEvent);
		npc->ovl.hEvent = 0;
	}
	return 0;
}

static void rpcrt4_conn_np_cancel_call(RpcConnection *Connection)
{
	/* FIXME: implement when named pipe writes use overlapped I/O */
}

static int rpcrt4_conn_np_wait_for_incoming_data(RpcConnection *Connection)
{
	/* FIXME: implement when named pipe writes use overlapped I/O */
	return -1;
}

static size_t rpcrt4_ncacn_np_get_top_of_tower(unsigned char *tower_data,
											   const char *networkaddr,
											   const char *endpoint)
{
	twr_empty_floor_t *smb_floor;
	twr_empty_floor_t *nb_floor;
	size_t size;
	size_t networkaddr_size;
	size_t endpoint_size;

	TRACE("(%p, %s, %s)\n", tower_data, networkaddr, endpoint);

	networkaddr_size = networkaddr ? strlen(networkaddr) + 1 : 1;
	endpoint_size = endpoint ? strlen(endpoint) + 1 : 1;
	size = sizeof(*smb_floor) + endpoint_size + sizeof(*nb_floor) + networkaddr_size;

	if (!tower_data)
		return size;

	smb_floor = (twr_empty_floor_t *)tower_data;

	tower_data += sizeof(*smb_floor);

	smb_floor->count_lhs = sizeof(smb_floor->protid);
	smb_floor->protid = EPM_PROTOCOL_SMB;
	smb_floor->count_rhs = endpoint_size;

	if (endpoint)
		memcpy(tower_data, endpoint, endpoint_size);
	else
		tower_data[0] = 0;
	tower_data += endpoint_size;

	nb_floor = (twr_empty_floor_t *)tower_data;

	tower_data += sizeof(*nb_floor);

	nb_floor->count_lhs = sizeof(nb_floor->protid);
	nb_floor->protid = EPM_PROTOCOL_NETBIOS;
	nb_floor->count_rhs = networkaddr_size;

	if (networkaddr)
		memcpy(tower_data, networkaddr, networkaddr_size);
	else
		tower_data[0] = 0;
	tower_data += networkaddr_size;

	return size;
}

static RPC_STATUS rpcrt4_ncacn_np_parse_top_of_tower(const unsigned char *tower_data,
													 size_t tower_size,
													 char **networkaddr,
													 char **endpoint)
{
	const twr_empty_floor_t *smb_floor = (const twr_empty_floor_t *)tower_data;
	const twr_empty_floor_t *nb_floor;

	TRACE("(%p, %d, %p, %p)\n", tower_data, (int)tower_size, networkaddr, endpoint);

	if (tower_size < sizeof(*smb_floor))
		return EPT_S_NOT_REGISTERED;

	tower_data += sizeof(*smb_floor);
	tower_size -= sizeof(*smb_floor);

	if ((smb_floor->count_lhs != sizeof(smb_floor->protid)) ||
		(smb_floor->protid != EPM_PROTOCOL_SMB) ||
		(smb_floor->count_rhs > tower_size) ||
		(tower_data[smb_floor->count_rhs - 1] != '\0'))
		return EPT_S_NOT_REGISTERED;

	if (endpoint)
	{
		*endpoint = I_RpcAllocate(smb_floor->count_rhs);
		if (!*endpoint)
			return RPC_S_OUT_OF_RESOURCES;
		memcpy(*endpoint, tower_data, smb_floor->count_rhs);
	}
	tower_data += smb_floor->count_rhs;
	tower_size -= smb_floor->count_rhs;

	if (tower_size < sizeof(*nb_floor))
		return EPT_S_NOT_REGISTERED;

	nb_floor = (const twr_empty_floor_t *)tower_data;

	tower_data += sizeof(*nb_floor);
	tower_size -= sizeof(*nb_floor);

	if ((nb_floor->count_lhs != sizeof(nb_floor->protid)) ||
		(nb_floor->protid != EPM_PROTOCOL_NETBIOS) ||
		(nb_floor->count_rhs > tower_size) ||
		(tower_data[nb_floor->count_rhs - 1] != '\0'))
		return EPT_S_NOT_REGISTERED;

	if (networkaddr)
	{
		*networkaddr = I_RpcAllocate(nb_floor->count_rhs);
		if (!*networkaddr)
		{
			if (endpoint)
			{
				I_RpcFree(*endpoint);
				*endpoint = NULL;
			}
			return RPC_S_OUT_OF_RESOURCES;
		}
		memcpy(*networkaddr, tower_data, nb_floor->count_rhs);
	}

	return RPC_S_OK;
}

typedef struct _RpcServerProtseq_np
{
	RpcServerProtseq common;
	HANDLE mgr_event;
} RpcServerProtseq_np;

static RpcServerProtseq *rpcrt4_protseq_np_alloc(void)
{
	RpcServerProtseq_np *ps = HeapAlloc(GetProcessHeap(), 0, sizeof(*ps));
	if (ps)
		ps->mgr_event = CreateEventW(NULL, FALSE, FALSE, NULL);
	return &ps->common;
}

static void rpcrt4_protseq_np_signal_state_changed(RpcServerProtseq *protseq)
{
	RpcServerProtseq_np *npps = CONTAINING_RECORD(protseq, RpcServerProtseq_np, common);
	SetEvent(npps->mgr_event);
}

static void *rpcrt4_protseq_np_get_wait_array(RpcServerProtseq *protseq, void *prev_array, unsigned int *count)
{
	HANDLE *objs = prev_array;
	RpcConnection_np *conn;
	RpcServerProtseq_np *npps = CONTAINING_RECORD(protseq, RpcServerProtseq_np, common);

	EnterCriticalSection(&protseq->cs);

	/* open and count connections */
	*count = 1;
	conn = CONTAINING_RECORD(protseq->conn, RpcConnection_np, common);
	while (conn) {
		rpcrt4_conn_listen_pipe(conn);
		if (conn->ovl.hEvent)
			(*count)++;
		conn = CONTAINING_RECORD(conn->common.Next, RpcConnection_np, common);
	}

	/* make array of connections */
	if (objs)
		objs = HeapReAlloc(GetProcessHeap(), 0, objs, *count*sizeof(HANDLE));
	else
		objs = HeapAlloc(GetProcessHeap(), 0, *count*sizeof(HANDLE));
	if (!objs)
	{
		ERR("couldn't allocate objs\n");
		LeaveCriticalSection(&protseq->cs);
		return NULL;
	}

	objs[0] = npps->mgr_event;
	*count = 1;
	conn = CONTAINING_RECORD(protseq->conn, RpcConnection_np, common);
	while (conn) {
		if ((objs[*count] = conn->ovl.hEvent))
			(*count)++;
		conn = CONTAINING_RECORD(conn->common.Next, RpcConnection_np, common);
	}
	LeaveCriticalSection(&protseq->cs);
	return objs;
}

static void rpcrt4_protseq_np_free_wait_array(RpcServerProtseq *protseq, void *array)
{
	HeapFree(GetProcessHeap(), 0, array);
}

static int rpcrt4_protseq_np_wait_for_new_connection(RpcServerProtseq *protseq, unsigned int count, void *wait_array)
{
	HANDLE b_handle;
	HANDLE *objs = wait_array;
	DWORD res;
	RpcConnection *cconn;
	RpcConnection_np *conn;

	if (!objs)
		return -1;

	do
	{
		/* an alertable wait isn't strictly necessary, but due to our
		* overlapped I/O implementation in Wine we need to free some memory
		* by the file user APC being called, even if no completion routine was
		* specified at the time of starting the async operation */
		res = WaitForMultipleObjectsEx(count, objs, FALSE, INFINITE, TRUE);
	} while (res == WAIT_IO_COMPLETION);

	if (res == WAIT_OBJECT_0)
		return 0;
	else if (res == WAIT_FAILED)
	{
		ERR("wait failed with error %d\n", GetLastError());
		return -1;
	}
	else
	{
		b_handle = objs[res - WAIT_OBJECT_0];
		/* find which connection got a RPC */
		EnterCriticalSection(&protseq->cs);
		conn = CONTAINING_RECORD(protseq->conn, RpcConnection_np, common);
		while (conn) {
			if (b_handle == conn->ovl.hEvent) break;
			conn = CONTAINING_RECORD(conn->common.Next, RpcConnection_np, common);
		}
		cconn = NULL;
		if (conn)
			RPCRT4_SpawnConnection(&cconn, &conn->common);
		else
			ERR("failed to locate connection for handle %p\n", b_handle);
		LeaveCriticalSection(&protseq->cs);
		if (cconn)
		{
			RPCRT4_new_client(cconn);
			return 1;
		}
		else return -1;
	}
}

static size_t rpcrt4_ncalrpc_get_top_of_tower(unsigned char *tower_data,
											  const char *networkaddr,
											  const char *endpoint)
{
	twr_empty_floor_t *pipe_floor;
	size_t size;
	size_t endpoint_size;

	TRACE("(%p, %s, %s)\n", tower_data, networkaddr, endpoint);

	endpoint_size = strlen(endpoint) + 1;
	size = sizeof(*pipe_floor) + endpoint_size;

	if (!tower_data)
		return size;

	pipe_floor = (twr_empty_floor_t *)tower_data;

	tower_data += sizeof(*pipe_floor);

	pipe_floor->count_lhs = sizeof(pipe_floor->protid);
	pipe_floor->protid = EPM_PROTOCOL_PIPE;
	pipe_floor->count_rhs = endpoint_size;

	memcpy(tower_data, endpoint, endpoint_size);
	tower_data += endpoint_size;

	return size;
}

static RPC_STATUS rpcrt4_ncalrpc_parse_top_of_tower(const unsigned char *tower_data,
													size_t tower_size,
													char **networkaddr,
													char **endpoint)
{
	const twr_empty_floor_t *pipe_floor = (const twr_empty_floor_t *)tower_data;

	TRACE("(%p, %d, %p, %p)\n", tower_data, (int)tower_size, networkaddr, endpoint);

	if (tower_size < sizeof(*pipe_floor))
		return EPT_S_NOT_REGISTERED;

	tower_data += sizeof(*pipe_floor);
	tower_size -= sizeof(*pipe_floor);

	if ((pipe_floor->count_lhs != sizeof(pipe_floor->protid)) ||
		(pipe_floor->protid != EPM_PROTOCOL_PIPE) ||
		(pipe_floor->count_rhs > tower_size) ||
		(tower_data[pipe_floor->count_rhs - 1] != '\0'))
		return EPT_S_NOT_REGISTERED;

	if (networkaddr)
		*networkaddr = NULL;

	if (endpoint)
	{
		*endpoint = I_RpcAllocate(pipe_floor->count_rhs);
		if (!*endpoint)
			return RPC_S_OUT_OF_RESOURCES;
		memcpy(*endpoint, tower_data, pipe_floor->count_rhs);
	}

	return RPC_S_OK;
}
