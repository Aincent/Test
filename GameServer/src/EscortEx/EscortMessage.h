/*
 * EscortMessage.h
 *
 *  Created on: 2014年7月21日
 *      Author: root
 */

#ifndef ESCORTMESSAGE_H_
#define ESCORTMESSAGE_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"
#include "MessageStruct/Escort/EscortProto.pb.h"
#include "Timer/TimerInterface.h"

using namespace std;
using namespace CommBaseOut;

class EscortMessage : public Singleton<EscortMessage>, public Message_Handler
{
public:

	EscortMessage();

	~EscortMessage();

	void Handle_Message(Safe_Smart_Ptr<Message> &message);

public:

	//领取皇纲品质
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ESCORTGETQUALITY);
	//刷新皇纲
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ESCORTFRESH);
	//开始运镖
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ESCORTSTART);
	//立即完成
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ESCORTIMEDCOMPLETE);
	//加速
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ESCORTSPEEDUP);
	//放弃
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ESCORTABANDON);
	//领取奖励
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ESCORTGETRPIZE);
	//获取镖车信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_OPENESCORT);
	// WS通知GM双倍押镖活动状态
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_DOUBLEESCORTBEGIN);
};


#endif /* ESCORTMESSAGE_H_ */
