/*
 * ActivityMessage.h
 *
 *  Created on: 2014年12月1日
 *      Author: root
 */

#ifndef ACTIVITYMESSAGE_H_
#define ACTIVITYMESSAGE_H_

#include "define.h"
#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include "../QuickMessage.h"
#include "ActivityMgr.h"

using namespace std;
using namespace CommBaseOut;

class Player;

class ActivityMessage : public Singleton<ActivityMessage>, public Message_Handler
{
public:
	ActivityMessage();
	~ActivityMessage();
protected:
	//是否帮派战宝箱采集
	bool IsClanWarGather(Smart_Ptr<Player>& player, int key, int &ret);

private:
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_PROTECTED_KING_INFO);	//请求守护秦王活动信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_REFLUSH_COUNT);	//请求重置秦王活动信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_DRIRECTED_FINISH); //请求扫荡
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GET_REWARD);  //请求获取奖励
	DEF_MSG_QUICK_SIMPLE_DECLARE_FUN_H(MSG_SIM_C2GM_GET_NODES_REWARD);	//获取闯关奖励

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_YELLOWVIP_INFO);  //获取黄钻信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_YELLOWVIP_NOVICEPACKS);  //新手礼包
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_YELLOWVIP_GROWPACKS);  //成长礼包
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_YELLOWVIP_DAILYPACKS);  //每日礼包
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_YELLOWVIP_YEARVIPPACKS);  //年费黄钻
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_YELLOWVIP_GIRLWAR);//战姬激活

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ASKBOX); //请求采集

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GET_ON_LINE_AT_TIME);

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_NOTICEACTIVITYSTATE); // 通知GM运营活动状态

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_DOUBLE_PROTECTEDKING); // WS通知GM开始双倍守护秦王活动

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ANSWER_INFO);//请求个人答题数据
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_CHOICE_ANSWER);//答题
	void Handle_Message(Safe_Smart_Ptr<Message> &message);
};


#endif /* ACTIVITYMESSAGE_H_ */
