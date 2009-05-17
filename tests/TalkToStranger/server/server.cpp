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

#include "server.h"

CMsgThread msg_thread;
static	std::list<CTalkSession *>	wait_sessions;
static	OSMutex						wait_sessions_mutex;


void  CMsgThread::Entry()
{
	while(!IsStopRequested()){
	OSMutexLocker _locker(&m_Mutex);
	
	if(m_Msgs.empty())
		m_Cond.Wait(&m_Mutex);
	else{
		CMessage * msg = m_Msgs.front();
		m_Msgs.pop_front();
		CTalkSession *ts = msg->target;
		ts->ProcessMessage(msg);
		ts->Release();
		delete msg;

	}
	}
}

void CMsgThread::PostMessage(CMessage *inMsg){
	OSMutexLocker _locker(&m_Mutex);
	if(inMsg){
		inMsg->target->AddRef();
		m_Msgs.push_back(inMsg);
	}
	m_Cond.Signal();
}

CMsgThread::CMsgThread(){

}

CMessage::CMessage(const WCHAR *pwszStatement):type(MSG_UNKNOWN),statement(NULL),peer(NULL)
{
	if(pwszStatement[0] == '/'){
		if(0 == u_strcasecmp("/exit",pwszStatement)){
			type = MSG_EXIT;
		}else if(0 == u_strcasecmp("/start",pwszStatement)){
			type = MSG_START;
		}else if(0 == u_strcasecmp("/bye",pwszStatement)){
			type = MSG_BYE;
		}

	}else{
		type = MSG_SAY;
		statement = new WCHAR[1024];
		q_ustrcpy(statement,pwszStatement);
	}
}

CMessage::CMessage(ULONG inType,const WCHAR* inStatement):type(inType),statement(NULL),peer(NULL)
{
	statement = new WCHAR[1024];
	q_ustrcpy(statement,inStatement);
}


CMessage::CMessage(ULONG inType,CTalkSession* inPeer):type(inType),statement(NULL),peer(inPeer)
{

}

CMessage::CMessage(ULONG inType):type(inType),statement(NULL),peer(NULL)
{
	
}


CMessage::~CMessage()
{
	if(peer)
		peer->Release();

	if(statement)
		delete [] statement;

}


HRESULT STDMETHODCALLTYPE CTalkSession::Say(const WCHAR *pwszStatement)
{
	CMessage* msg = new CMessage(pwszStatement);
	if(msg->type != MSG_UNKNOWN){
		PostMessage(msg);
	}
	else if(m_Event){
		m_Event->OnStatement(wstr_from_astr("note"),wstr_from_astr("unknown command"));

	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CTalkSession::Advise(ITalkSessionEvents *pEventSink,DWORD *pdwReg)
{
	pEventSink->AddRef();
	m_Event = pEventSink;
	*pdwReg = (DWORD)((void*)m_Event);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CTalkSession::Unadvise(DWORD dwReg){
	m_Event->Release();
	m_Event = NULL;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CTalkSession::ChangeState(ULONG inState){
	
	printf("seesion(%p) change state:%d => %d\n",this,m_state,inState);
	HRESULT hr = S_OK;
	switch(inState)
	{
	case STATE_IDLE:
		if(m_Event) {
			if(m_state == STATE_CONN)
				if(m_Event) {
					m_Event->OnStatement(wstr_from_astr("note"),wstr_from_astr("the talk session is end"));
				}
			hr = m_Event->OnStatement(wstr_from_astr("note"),wstr_from_astr("say \"/start\" to say a new talk,enter \"/exit\" to exit the program"));
		}
		break;
	case STATE_WAIT_CONN:
		if(m_Event) {
			hr = m_Event->OnStatement(wstr_from_astr("note"),wstr_from_astr("waitting connect ....\n""say \"/bye\" to cancel ,say \"/exit\" to exit the program"));
		}
		break;
	case STATE_CONN:
		if(m_Event) {
			hr = m_Event->OnStatement(wstr_from_astr("note"),wstr_from_astr("you is talking with stranger,say hi to he/she.\n"
																			"say \"/bye\" to end the current talk,say \"/exit\" to exit the program"));
		}
		break;
	}

	m_state = inState;
	return hr;

}

void CTalkSession::PostMessage(CMessage * inMsg,CTalkSession * inTarget)
{
	if(inTarget == NULL)
		inTarget = this;
	inMsg->target = inTarget;
	msg_thread.PostMessage(inMsg);
}

HRESULT CTalkSession::ProcessMessage(CMessage* inMsg)
{
	bool handle = false;
	switch(m_state)
	{
	case STATE_IDLE:
		switch(inMsg->type)
		{
		case MSG_START:
			{
				ChangeState(STATE_WAIT_CONN);

				OSMutexLocker _loker(&wait_sessions_mutex);
				if(!wait_sessions.empty()){
					m_peer = wait_sessions.front();
					m_peer->AddRef();
					wait_sessions.pop_front();
					CMessage* msg = new CMessage(MSG_PEER_CONN,this);
					PostMessage(msg,m_peer);
					ChangeState(STATE_CONN);
				}
				else{
					wait_sessions.push_back(this);
				}
			}
			handle = true;
			break;
		case MSG_EXIT:
			{
				ChangeState(STATE_EXIT);
			}
			handle = true;
			break;
		case MSG_SAY:
			{
				if(m_Event)
					m_Event->OnStatement(wstr_from_astr("note"),wstr_from_astr("you are not in talking now.\n"));
			}
			handle = true;
			break;
		}
		break;
	case STATE_WAIT_CONN:
		switch(inMsg->type)
		{
		case MSG_PEER_CONN:
			{
				m_peer = inMsg->peer;
				m_peer->AddRef();
				HRESULT hr = ChangeState(STATE_CONN);
				if(hr == 0x80040601L/*RPCCOM_E_CONN_DISCONNECTED*/)
				{
					printf("session(%p): has disconnect!\n",this);
					CMessage* msg = new CMessage(MSG_PEER_BYE);
					PostMessage(msg,m_peer);
					m_peer->Release();
					m_peer = NULL;
				}
			}
			handle = true;
			break;
		case MSG_BYE:
			{
				OSMutexLocker _loker(&wait_sessions_mutex);
				wait_sessions.remove(this);
				ChangeState(STATE_IDLE);
			}
			handle = true;
			break;
		case MSG_EXIT:
			{
				OSMutexLocker _loker(&wait_sessions_mutex);
				wait_sessions.remove(this);
				ChangeState(STATE_EXIT);
			}
			handle = true;
			break;
		case MSG_SAY:
			{
				if(m_Event) m_Event->OnStatement(wstr_from_astr("note"),wstr_from_astr("you are not in talking now.\n"));
			}
			handle = true;
			break;
		}
		break;
	case STATE_CONN:
		switch(inMsg->type)
		{
		case MSG_SAY:
			{
				CMessage* msg = new CMessage(MSG_PEER_SAY,inMsg->statement);
				PostMessage(msg,m_peer);
			}
			handle = true;
			break;
		case MSG_PEER_SAY:
			m_Event->OnStatement(wstr_from_astr("Stranger\t"),inMsg->statement);
			handle = true;
			break;
		case MSG_PEER_BYE:
			m_peer->Release();
			m_peer = NULL;
			ChangeState(STATE_IDLE);
			handle = true;
			break;
		case MSG_BYE:
		case MSG_EXIT:
			{
				CMessage* msg = new CMessage(MSG_PEER_BYE);
				PostMessage(msg,m_peer);
				m_peer->Release();
				m_peer = NULL;
				ChangeState(MSG_BYE == inMsg->type?STATE_IDLE:STATE_EXIT);
			}
			handle = true;
			break;
		}
		break;
	}

	if(handle == false){
		if(inMsg->type != MSG_SAY && inMsg->type != MSG_PEER_SAY)
		{
			if(m_Event) m_Event->OnStatement(wstr_from_astr("note"),wstr_from_astr("unknown command"));
		}
	}

	return S_OK;
}

CTalkSession::CTalkSession():tgmtl::cocalss_base<ITalkSession>(IID_ITalkSession)
,m_state(STATE_EXIT),m_Event(NULL),m_peer(NULL)
{
	ChangeState(STATE_IDLE);
}

CTalkSession::~CTalkSession()
{
	if(m_Event)
		m_Event->Release();
	if(m_peer)
		m_peer->Release();
}

CTalkManager::CTalkManager():tgmtl::cocalss_base<ITalkSessionManager>(IID_ITalkSessionManager)
{
	msg_thread.Start();
}

CTalkManager::~CTalkManager(){
	msg_thread.SendStopRequest();
	msg_thread.PostMessage(NULL);
	msg_thread.StopAndWaitForThread();
}

