库的初始化过程：

/*如果使用了时间服务*/
OS::Initialize();

/*如果使用了OSThread，确切地说是使用了其中的OSThread::GetCurrent*/
OSThread::Initialize();

/*如果使用了OSMemory*/
OSMemory::SetMemoryError(ENOMEM);

/*如果使用了Socket*/
#ifdef __Win32__        
    //
    // Start Win32 DLLs
    WORD wsVersion = MAKEWORD(1, 1);
    WSADATA wsData;
    (void)::WSAStartup(wsVersion, &wsData);
#endif

/*如果使用了Socket以及Task作为Socket数据的接收者 */
Socket::Initialize();

/*如果使用了SocketUtils*/
SocketUtils::Initialize(false);

例如，MTAPIServer需要使用以下初始化函数
OS::Initialize();
OSThread::Initialize();
