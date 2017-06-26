/*
 * GMInterfaceAnswerMessage.h
 *
 *  Created on: 16/07/2015
 *      Author: root
 */

#ifndef GMINTERFACEANSWERMESSAGE_H_
#define GMINTERFACEANSWERMESSAGE_H_

#include "Singleton.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "Network/MessageHandler.h"

using namespace CommBaseOut;

class GMInterfaceAnswerMessage : public Singleton<GMInterfaceAnswerMessage>,public Message_Handler,public Request_Handler,public Ack_Handler
{
public:
	GMInterfaceAnswerMessage();

	~GMInterfaceAnswerMessage();

public:
	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);

	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);

	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

private:
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_BUCKLE_ATT);

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_GIVE_ANSWER_ITEM);

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_REQ_WS2GM_PLAYER_ANSWER);

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_BEGINANSWER);
};


#endif /* GMINTERFACEANSWERMESSAGE_H_ */
