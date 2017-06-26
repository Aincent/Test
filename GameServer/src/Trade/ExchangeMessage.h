/*
 * ExchangeMessage.h
 *
 *  Created on: 2014.5.15
 *      Author: cz
 */

/******************************************/
//说明：交易的通讯模块
/*****************************************/

#ifndef EXCHANGEMESSAGE_H_
#define EXCHANGEMESSAGE_H_

#include "Network/MessageHandler.h"
#include "Singleton.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include "../Object/Player/Player.h"


using namespace std;
using namespace CommBaseOut;


class ExchangeMessage : public Message_Handler, public Request_Handler, public Ack_Handler, public Singleton<ExchangeMessage>
{
public:
	ExchangeMessage();
	~ExchangeMessage();
	//
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_INITEXCHANGE);		//请求交易
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_OTHERSTATUS);			//trader的反馈请求
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_CANCELEXCHANGE);		//取消交易
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_EXCHANGELOCK);		//锁定交易
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_VERIFYEXCHANGE);		//确认交易


	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_INITEXCHANGE);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_OTHERSTATUS);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_CANCELEXCHANGE);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_EXCHANGELOCK);		//锁定交易
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_VERIFYEXCHANGE);		//确认交易

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

};


#endif /* EXCHANGEMESSAGE_H_ */
