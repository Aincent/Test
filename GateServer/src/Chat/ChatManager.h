/*
 * ChatManager.h
 *
 *  Created on: 2014年5月19日
 *      Author: root
 */

#ifndef CHATMANAGER_H_
#define CHATMANAGER_H_
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"

using namespace CommBaseOut;


class ChatManager : public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	~ChatManager();

	static ChatManager *GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new ChatManager();
		}

		return m_instance;
	}

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_CHAT);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2WS_CHAT);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GT_CHAT);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GT_MARQUEE);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_CHAT);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

private:
	ChatManager();

private:
	static ChatManager * m_instance;

};




#endif /* CHATMANAGER_H_ */
