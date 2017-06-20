/*
 * CommonMessage.h
 *
 *  Created on: 2014年5月13日
 *      Author: helh
 */

#ifndef COMMONMESSAGE_H_
#define COMMONMESSAGE_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"


using namespace CommBaseOut;


class CommonMessage : public Singleton<CommonMessage>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	CommonMessage();
	~CommonMessage();

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GM2GT_QUICKMESSAGE);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2C_QUICKMESSAGE);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_C2GT_QUICKMESSAGE);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_C2WS_QUICKMESSAGE);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_QUICKMESSAGE);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2WS_QUICKMESSAGE);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_QUICKMESSAGE);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2WS_QUICKMESSAGE);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

};



#endif /* COMMONMESSAGE_H_ */
