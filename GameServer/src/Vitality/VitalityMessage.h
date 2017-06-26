/*
 * VitalityMessage.h
 *
 *  Created on: 2015年3月17日
 *      Author: root
 */

#ifndef VITALITYMESSAGE_H_
#define VITALITYMESSAGE_H_

#include "Singleton.h"
#include "ServerMsgDefine.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "../QuickMessage.h"

using namespace CommBaseOut;

class VitalityMessage : public Singleton<VitalityMessage>
{
public:
	VitalityMessage();
	~VitalityMessage();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GETVITALITYINFO); // 请求活跃信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GETVITALITYGIFT); // 请求领取活跃奖励

};
#endif /* VITALITYMESSAGE_H_ */
