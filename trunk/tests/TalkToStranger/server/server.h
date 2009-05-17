/*
* talk server
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
#ifndef SERVER_HH
#define SERVER_HH

#include "../talk.h"
#include "tgmtl/tgmtl.h"
#include "utl.h"
#include "OSMutex.h"
#include "OSCond.h"
#include "OSThread.h"
#include <list>
using namespace std;

class CMessage;
class CTalkSession;
class CTalkManager;



class CMsgThread:public OSThread
{
public:
	virtual     void            Entry();
	void PostMessage(CMessage *inMsg);

	CMsgThread();
private:
	list<CMessage *>	m_Msgs;
	OSMutex				m_Mutex;
	OSCond				m_Cond;
};


enum{
	STATE_IDLE,
	STATE_WAIT_CONN,
	STATE_CONN,
	STATE_EXIT
};

enum{
	MSG_UNKNOWN = -1,
	MSG_START,
	MSG_BYE,
	MSG_SAY,
	MSG_EXIT,

	MSG_PEER_CONN,
	MSG_PEER_BYE,
	MSG_PEER_SAY,
};

class CMessage{
public:
	CMessage(const WCHAR *pwszStatement);
	CMessage(ULONG inType,const WCHAR* statement);
	CMessage(ULONG inType,CTalkSession* inPeer);
	CMessage(ULONG inType);
	~CMessage();

private:
	ULONG type;
	WCHAR * statement;
	CTalkSession* peer;
	CTalkSession* target;

	friend class CMsgThread;
	friend class CTalkSession;
};


class CTalkSession:public tgmtl::cocalss_base<ITalkSession>
{
public:
	//////////////////////////////////////////////////////////////////////////
	virtual HRESULT STDMETHODCALLTYPE Say(const WCHAR *pwszStatement);

	virtual HRESULT STDMETHODCALLTYPE Advise(ITalkSessionEvents *pEventSink,DWORD *pdwReg);
	virtual HRESULT STDMETHODCALLTYPE Unadvise(DWORD dwReg);

	//////////////////////////////////////////////////////////////////////////
	void PostMessage(CMessage * inMsg,CTalkSession * inTarget = NULL);
	HRESULT ProcessMessage(CMessage* inMsg);

	CTalkSession();
	~CTalkSession();
private:
	HRESULT ChangeState(ULONG inState);

private:
	ULONG				m_state;
	ITalkSessionEvents*	m_Event;
	CTalkSession*		m_peer;
};


class CTalkManager:public tgmtl::cocalss_base<ITalkSessionManager>
{
public:
	CTalkManager();
	virtual ~CTalkManager();
	virtual HRESULT STDMETHODCALLTYPE CreateSession(ITalkSession **ppts)
	{
		ITalkSession *ts = new CTalkSession();
		ts->AddRef();
		*ppts = ts;
		return S_OK;
	}
};

#endif
