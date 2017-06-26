/*
 * VIPMessage.h
 *
 *  Created on: 2014年8月11日
 *      Author: root
 */

#ifndef VIPMESSAGE_H_
#define VIPMESSAGE_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include <vector>
#include "../Object/Player/Player.h"
#include "../QuickMessage.h"

using namespace std;
using namespace CommBaseOut;

class VIPMessage : public Singleton<VIPMessage>
{
public:
	VIPMessage();
public:
	~VIPMessage();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GETVIPGIFT);								//领取VIP奖励
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GETGIFTINFO);							//VIP礼物领取状态
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_PLAYERRECHARGEINFO);		//领取VIP奖励
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GETRECHARGEGIFT);				//VIP礼物领取状态
};



#endif /* VIPMESSAGE_H_ */
