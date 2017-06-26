/*
 * RewardMgr.h
 *
 *  Created on: 2014年6月17日
 *      Author: root
 */

#ifndef REWARDMGR_H_
#define REWARDMGR_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../Object/Player/Player.h"
#include "../QuickMessage.h"
#include "../MessageBuild.h"
#include "Smart_Ptr.h"
#include "MessageStruct/RewardHail/RewardHail.pb.h"

using namespace std;
using namespace CommBaseOut;

enum RewardType
{
	SIGIN_REWARD = 1,
	ONLINE_REWARD = 2,
	LEVEL_REWARD = 3,
	TOTALLOGIN_REWARD = 4,  //累积登录
	FIRSTCHARGE_REWARD =5,   //首充
	MICROCLIENT_REWARD = 6, // 微端奖励
};

const int RewardDaTing_OpenLv = 10;  //奖励大厅开启等级

class Player;

class RewardMgr : public Singleton<RewardMgr>
{
public:
	RewardMgr();
public:
	~RewardMgr();

	int GetReward(Smart_Ptr<Player> player, RewardHail::GetReward &info);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_SIGNIN);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_OPENSIGNIN);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_ONLINEREWARDINFO);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_LEVELREWARDINFO);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_GETREWARD);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_TOTALLOGINREWARDINFO);
};


#endif /* REWARDMGR_H_ */
