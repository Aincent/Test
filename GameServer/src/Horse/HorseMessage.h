/*
 * HorseMessage.h
 *
 *  Created on: 2014年4月30日
 *      Author: helh
 */

#ifndef HORSEMESSAGE_H_
#define HORSEMESSAGE_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"

using namespace CommBaseOut;


class HorseMessage : public Singleton<HorseMessage>, public Message_Handler, public Request_Handler
{
public:
	HorseMessage();
	~HorseMessage();

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_HORSEINFO);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_HANDLEHORSE);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_HORSEADD);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_C2GM_BUYHORSESKIN);

	DEF_MSG_QUICK_SIMPLE_DECLARE_FUN_H(MSG_SIM_C2GM_ALTERHORSESKINUIHL);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
};


#endif /* HORSEMESSAGE_H_ */
