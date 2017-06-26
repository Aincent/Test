/*
 * FestivalUIMessage.h
 *
 *  Created on: 16/12/2015
 *      Author: root
 */

#ifndef CHRISTMASMESSAGE_H_
#define CHRISTMASMESSAGE_H_

#include "define.h"
#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"

class FestivalUIMessage : public Singleton<FestivalUIMessage>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	FestivalUIMessage();

	~FestivalUIMessage();

public:
	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);

	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);

	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

public:
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2G_GET_BREAK_EGG_INFO);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2G_GET_CHRISTMAS_LOGIN_INFO);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2G_GET_LIMIT_BUY_INFO);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2G_EGG_RESLUSH);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2G_BREAK_EGG);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2G_RECEIVE_REWARD);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2G_LIMIT_BUY_REFLUSH);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2G_LIMIT_BUY_BUY);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2G_CHRISTMAS_EXCHANGE);

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_W2GM_CHECK_PACKAGE);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_NEW_YEAR_LOGIN_DATA);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_RECEIVE_NEW_YEAR);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2W_BUY_HORSE_SKIN);

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_W2GM_GET_NEW_YEAR_DATA);
};


#endif /* CHRISTMASMESSAGE_H_ */
