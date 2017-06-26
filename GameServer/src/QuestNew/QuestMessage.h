/*
 * QuestMessage.h
 *
 *  Created on: 2014年10月30日
 *      Author: root
 */

#ifndef QUESTMESSAGE_H_
#define QUESTMESSAGE_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"
#include "MessageStruct/Quest/QuestMessage.pb.h"

using namespace std;
using namespace CommBaseOut;

class QuestNewMessage : public Singleton<QuestNewMessage>
{
public:
	QuestNewMessage();
public:
	~QuestNewMessage();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_UPDATEQUEST);		//提交任务

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_FRESHSTAR);		//帮派和日常刷星

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ONEKEYCOM);		//一键完成日常和帮派任务

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_FINISH_ONE_QUEST);
	
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_REFLUSH_WEEK_TASK);
};



#endif /* QUESTMESSAGE_H_ */
