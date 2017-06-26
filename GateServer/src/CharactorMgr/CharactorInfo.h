/*
 * CharactorInfo.h
 *
 *  Created on: 2013��10��26��
 *      Author: helh
 */

#ifndef CHARACTORINFO_H_
#define CHARACTORINFO_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"

using namespace CommBaseOut;

class CharactorInfo : public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	~CharactorInfo();

	static CharactorInfo * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new CharactorInfo();
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

	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_CHARATTR);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_HORSEINFO);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_HANDLEHORSE);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_HORSEADD);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_CHARATTR);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_HORSEINFO);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_HANDLEHORSE);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_HORSEADD);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);

	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);

	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

private:
	CharactorInfo();

private:
	static CharactorInfo* m_instance;
};


#endif /* CHARACTORINFO_H_ */
