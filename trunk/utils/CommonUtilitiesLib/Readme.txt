��ĳ�ʼ�����̣�

/*���ʹ����ʱ�����*/
OS::Initialize();

/*���ʹ����OSThread��ȷ�е�˵��ʹ�������е�OSThread::GetCurrent*/
OSThread::Initialize();

/*���ʹ����OSMemory*/
OSMemory::SetMemoryError(ENOMEM);

/*���ʹ����Socket*/
#ifdef __Win32__        
    //
    // Start Win32 DLLs
    WORD wsVersion = MAKEWORD(1, 1);
    WSADATA wsData;
    (void)::WSAStartup(wsVersion, &wsData);
#endif

/*���ʹ����Socket�Լ�Task��ΪSocket���ݵĽ����� */
Socket::Initialize();

/*���ʹ����SocketUtils*/
SocketUtils::Initialize(false);

���磬MTAPIServer��Ҫʹ�����³�ʼ������
OS::Initialize();
OSThread::Initialize();
