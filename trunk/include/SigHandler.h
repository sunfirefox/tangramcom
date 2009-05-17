
#include <iostream>
#include <signal.h>
#include "OSBaseTypes.h"
#include "OSMutex.h"
static OSMutex	sExitMutex;
static OSCond	sExitCond;
static bool		sExitFlag = false;
/* ========================================================================
* Signal and error handler.
*/
#if __Win32__
#include "wincon.h"
static BOOL SignalEventHandler( DWORD signalID )
{   
	if ( (signalID != SIGINT) && (signalID != SIGTERM) ) 
	{
		Beep(1000, 1000); 
		std::cout << "SignalEventHandler try to stop server!!!" << std::endl;
		sExitFlag = true;
		sExitCond.Signal();
		return TRUE;
	}

	return FALSE;

}

#else
static void SignalEventHandler( int signalID )
{   
	if ( (signalID == SIGALRM) || (signalID == SIGHUP) )    // unexpected SIGALRM || SIGHUP
	{
		return;
	}

	if (signalID == SIGINT) 
	{
		std::cout << "SignalEventHandler try to stop server!!!" << std::endl;
		sExitFlag = true;
		sExitCond.Signal();
		return;
	}
	::exit(-1);
}
#endif



static void RegisterEventHandlers()
{
#if  __Win32__
	BOOL bRet = SetConsoleCtrlHandler( (PHANDLER_ROUTINE) SignalEventHandler, true);
	UNUSED(bRet);
	return;

#else  //linux

	struct sigaction act;

	//#if defined(sun) || defined(i386) || defined(__MacOSX__) || defined(__sgi__) || defined(__osf__) || defined(__hpux__) 
#if 1
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = (void(*)(int))&SignalEventHandler;
#else
	act.sa_mask = 0;
	act.sa_flags = 0;
	act.sa_handler = (void(*)(...))&SignalEventHandler;
#endif

	if ( ::signal(SIGTERM, SIG_IGN) != SIG_IGN) 
	{   // from kill...
		if ( ::sigaction(SIGTERM, &act, NULL) != 0 )
		{   //MT_OUTPUT(( "- RegisterEventHandlers: System error (%i).\n", (int)SIG_ERR ));
		}
	}

	if ( ::signal(SIGINT, SIG_IGN) != SIG_IGN) 
	{   // ^C signal
		if ( ::sigaction(SIGINT, &act, NULL)  != 0 )
		{   //MT_OUTPUT(( "- RegisterEventHandlers: System error (%i).\n", (int)SIG_ERR ));
		}

	}

	if ( ::signal(SIGPIPE, SIG_IGN) != SIG_IGN) 
	{   // broken pipe probably from a failed RTSP session (the server went down?)
		if ( ::sigaction(SIGPIPE, &act, NULL)   != 0 )
		{   //MT_OUTPUT(( "- RegisterEventHandlers: System error (%i).\n", (int)SIG_ERR ));
		}

	}

	if ( ::signal(SIGHUP, SIG_IGN) != SIG_IGN) 
	{   // catch any SIGHUP
		if ( ::sigaction(SIGHUP, &act, NULL)  != 0)
		{   //MT_OUTPUT(( "- RegisterEventHandlers: System error (%i).\n", (int)SIG_ERR ));
		}

	}

	if ( ::signal(SIGALRM, SIG_IGN) != SIG_IGN) 
	{   // catch any SIGALRM
		if ( ::sigaction(SIGALRM, &act, NULL)  != 0)
		{   //MT_OUTPUT(( "- RegisterEventHandlers: System error (%i).\n", (int)SIG_ERR ));
		}

	}

#endif
}
