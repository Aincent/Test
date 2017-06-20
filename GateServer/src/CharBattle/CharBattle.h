/*
 * CharBattle.h
 *
 *  Created on: 2013��11��12��
 *      Author: helh
 */

#ifndef CHARBATTLE_H_
#define CHARBATTLE_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"

using namespace CommBaseOut;

class CharBattleMgr : public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	~CharBattleMgr();

	static CharBattleMgr *GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new CharBattleMgr();
		}

		return m_instance;
	}

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_SIM_C2GT_ATTACK);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_C2GT_REVIVAL);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_C2GT_MONITOR);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_UPGRADE_SKILL);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_UPGRADE_SKILL);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_SIM_GT2GM_ATTACK);

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
	CharBattleMgr();

private:
	static CharBattleMgr * m_instance;

};


#endif /* CHARBATTLE_H_ */
