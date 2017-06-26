/*
 * DestinationMessage.h
 *
 *  Created on: 2014年11月27日
 *      Author: root
 */

#ifndef DESTINATIONMESSAGE_H_
#define DESTINATIONMESSAGE_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include <vector>
#include "../Object/Player/Player.h"
#include "../QuickMessage.h"
#include "MessageStruct/DestinationProto.pb.h"

using namespace std;
using namespace CommBaseOut;

class DestinationMessage : public Singleton<DestinationMessage>
{
public:
	DestinationMessage();
public:
	~DestinationMessage();
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_GETDESTINATIONTYPE);		//查看某个游戏目标阶段
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_GETDESTINATIONPRIZE);		//获取游戏目标奖励
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_GETHAVEREWARDDESTINATION);		//获取有奖励可领的游戏目标
};


#endif /* DESTINATIONMESSAGE_H_ */
