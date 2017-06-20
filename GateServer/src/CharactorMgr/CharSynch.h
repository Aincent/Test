/*
 * CharSynch.h
 *
 *  Created on: 2013��10��11��
 *      Author: helh
 */

#ifndef CHARSYNCH_H_
#define CHARSYNCH_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "CharDefine.h"

using namespace CommBaseOut;

class CharSynch : public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	~CharSynch();

	static CharSynch * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new CharSynch();
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

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_C2GT_SYNCH);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_C2GT_HEARTBEAT);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GM2GT_SYNPATH);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GT2GM_SYNCHOBJECTATTR);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GM2GT_DELETEDROP);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GM2GT_SYNWCHATTR);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_SYNCHPOS);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_PICKITEM);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_FINDPATH);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_PICKITEM);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);

	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);

	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

private:
	CharSynch();

private:
	static CharSynch* m_instance;
};


#endif /* CHARSYNCH_H_ */
