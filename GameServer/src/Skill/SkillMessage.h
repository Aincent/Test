/*
 * SkillMessage.h
 *
 *  Created on: 2014年1月14日
 *      Author: root
 */

#ifndef SKILLMESSAGE_H_
#define SKILLMESSAGE_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"
#include "../MsgCommonClass.h"
#include "../Object/Player/Player.h"

using namespace std;
using namespace CommBaseOut;


class SkillMessage : public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	~SkillMessage();

	static SkillMessage * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new SkillMessage();
		}

		return m_instance;
	}

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_SIM_GT2GM_ATTACK);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_UPGRADE_SKILL);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2WS_UPGRADE_CLAN_SKILL);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2WS_CLAN_SKILLS);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

	//明于buff抵抗和免疫
	bool isCanAddBuff(const BuffInfo& info,Smart_Ptr<CreatureObj> owner,Smart_Ptr<CreatureObj> target);

private:
	SkillMessage();

private:
	static SkillMessage * m_instance;
};




#endif /* SKILLMESSAGE_H_ */
