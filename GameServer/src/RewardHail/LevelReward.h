/*
 * LevelReward.h
 *
 *  Created on: 2014年6月20日
 *      Author: root
 */

#ifndef LEVELREWARD_H_
#define LEVELREWARD_H_

#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "Smart_Ptr.h"
#include "define.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/RewardHail/RewardHail.pb.h"
#include "BitArray.h"
#include "../EventSet/EventDefine.h"

using namespace std;
using namespace CommBaseOut;

class Player;

class LevelReward
{
public:
	LevelReward(Player * player);
	~LevelReward();

	void ReInit();
	void Release();

	void InitLevelReward(int levelRecord);

	int GetRecord();

	int GetLevelReward(RewardHail::GetReward &info, vector<DWORD> &goods, vector<DWORD> &num);

	//是否还有等级奖励没领取
	bool LevelReward_IsHaveReward();
	int GetLRCanGetAward();
	//角色 升级 是否到了可领取等级奖励
	bool HandleCanLevelReward(const EventArgs & e);

	//参数指定ID的等级礼包是否已经领取
	bool isSomeLevelRewardReceive(int id);

	//是否已经领取过任意一个等级礼包
	bool isAnyLevelRwardReceive();

private:

	Player * m_owner;
	CBitArray m_levelReward;
};


#endif /* LEVELREWARD_H_ */
