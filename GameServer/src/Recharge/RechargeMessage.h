/*
 * RechargeMessage.h
 *
 *  Created on: 2015年6月18日
 *      Author: root
 */

#ifndef RECHARGEMESSAGE_H_
#define RECHARGEMESSAGE_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include "../QuickMessage.h"
#include "../MsgCommonClass.h"

using namespace std;
using namespace CommBaseOut;

class RechargeMessage : public Singleton<RechargeMessage>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	RechargeMessage();
	virtual ~RechargeMessage();

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_SYSPAYINFO);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_SYSMARKETINFO);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_SYS58PLAYPAYINFO);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_SYS360PLAYPAYINFO);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_SYSCONNECTINGINFO);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);
};


#endif /* RECHARGEMESSAGE_H_ */
