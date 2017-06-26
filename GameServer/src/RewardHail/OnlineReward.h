/*
 * OnlineReward.h
 *
 *  Created on: 2014年6月19日
 *      Author: root
 */

#ifndef ONLINEREWARD_H_
#define ONLINEREWARD_H_

#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "Smart_Ptr.h"
#include "define.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/RewardHail/RewardHail.pb.h"
#include "BitArray.h"

using namespace std;
using namespace CommBaseOut;

class Player;

const int OnlineBitCounter = 10;  //在线奖励总共奖励 数目

class OnlineReward
{
public:
	OnlineReward(Player * player);
	~OnlineReward();

	void ReInit();
	void Release();

	void ResetOnlineReward();

	void InitOnlineReward(int rewardRecord, int historyReward);
	int GetRecord();

	void GetOnlineRewardInfo(RewardHail::ClientOlineRewardInfo &info);

	int GetOnlineReward(RewardHail::GetReward &info, vector<DWORD> &goods, vector<DWORD> &num);

	int GetHistoryReward();

	static void OneTime(void * obj, void * arg);

	//是否还有在线奖励未领完
	bool OnlineReward_IsHaveReward();

	int GetOLCanGetAward();

	void CalOnlineReward();

	int CalOnlineNum(int min);

	bool isHaveReceiveOneReward();

private:

	Player * m_owner;
	CBitArray m_onlineReward;
	CBitArray m_historyReward;
};


#endif /* ONLINEREWARD_H_ */
