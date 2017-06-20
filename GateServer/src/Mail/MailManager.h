/*
 * MailManager.h
 *
 *  Created on: 2014年6月4日
 *      Author: root
 */

#ifndef MAILMANAGER_H_
#define MAILMANAGER_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"

using namespace CommBaseOut;

class MailManager : public Singleton<MailManager>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	MailManager();
	~MailManager();
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GT_SENDMAIL);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

};


#endif /* MAILMANAGER_H_ */
