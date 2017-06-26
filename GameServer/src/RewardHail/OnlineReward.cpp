/*
 * OnlineReward.cpp
 *
 *  Created on: 2014年6月19日
 *      Author: root
 */


#include "OnlineReward.h"
#include "util.h"
#include "Timer/TimerInterface.h"
#include "../Map/ConfigLoader/MapConfig.h"
#include "Log/Logger.h"
#include "../Container/ContainerBase.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "FileLoader/OnlineRewardLoader.h"
#include "../QuickMessage.h"
#include "CounterDefine.h"
#include "../Counter/Counter.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "../ServerEventDefine.h"
#include "FileLoader/GameParaLoader.h"

OnlineReward::OnlineReward(Player * player):m_owner(player)
{

}

OnlineReward::~OnlineReward()
{

}

void OnlineReward::ReInit()
{
	m_onlineReward.Init();
	m_historyReward.Init();
}

void OnlineReward::Release()
{
	m_onlineReward.Release();
		m_historyReward.Release();
}

void OnlineReward::InitOnlineReward(int rewardRecord, int historyReward)
{
	unsigned char buf[4] = {0};

	if(m_owner->GetCounterService()->getResetDayType() != eDayResetToday)
	{
		rewardRecord = 0;
		historyReward = 0;
	}

	CUtil::SafeMemmove(buf, 4, &rewardRecord, 4);
	m_onlineReward.SetData(buf, 4);

	CUtil::SafeMemmove(buf, 4, &historyReward, 4);
	m_historyReward.SetData(buf, 4);
}

int OnlineReward::GetRecord()
{
	int len = 0;
	unsigned char *buf = 0;

	buf = const_cast<unsigned char *>(m_onlineReward.GetData(len));
	int onlineReward = 0;
	CUtil::SafeMemmove(&onlineReward, 4, buf, len);

	return onlineReward;
}

int OnlineReward::GetHistoryReward()
{
	int len = 0;
	unsigned char *buf = 0;

	buf = const_cast<unsigned char *>(m_historyReward.GetData(len));
	int historyReward = 0;
	CUtil::SafeMemmove(&historyReward, 4, buf, len);

	return historyReward;
}

void OnlineReward::GetOnlineRewardInfo(RewardHail::ClientOlineRewardInfo &info)
{
	info.set_rewardinfo(GetHistoryReward());
	int64 onLineTime = 0;
	int64 lastResetTime = m_owner->GetCounterService()->GetPlayerLastResetTime();

	//说明玩家在刷新之前已经在线了,那么就得以刷新的时刻作为玩家在线的起点
	if(m_owner->GetLoginTime() < lastResetTime)
		onLineTime = CUtil::GetNowSecond() - lastResetTime;
	else
		onLineTime = m_owner->CalOnlineTime();

	info.set_time(onLineTime);
}

int OnlineReward::GetOnlineReward(RewardHail::GetReward &info, vector<DWORD> &goods, vector<DWORD> &num)
{
	int i;
	int res = 0;

	vector<int> bglod;
	vector<int> money;
	vector<int> zhenqi;
	vector<int> vecPos;
	vector<DWORD> strengthLvVec;
	vector<WORD> bindVec;

	for(i = 0; i < info.list_size(); i++)
	{
		int mbit = info.list(i) -1;
		if(!m_onlineReward.GetBit(mbit))
		{
			return eNotGetReward;
		}
		if(m_historyReward.GetBit(mbit))
		{
			return eHaveGetReward;
		}
	}

	for(i = 0; i < info.list_size(); i++)
	{
		OnlineRewardStruct *onlineReward = OnlineRewardLoader::GetInstance()->GetOnlineRewardLoader(info.list(i));
		if(!onlineReward)
		{
			continue;
		}

		goods.insert(goods.end(), onlineReward->m_nAward.begin(), onlineReward->m_nAward.end());
		num.insert(num.end(), onlineReward->m_nAwardNum.begin(), onlineReward->m_nAwardNum.end());
		bglod.push_back(onlineReward->i_bglod);
		money.push_back(onlineReward->i_money);
		zhenqi.push_back(onlineReward->i_zhenqi);
	}

	vector<WORD> tempFlagList;
	tempFlagList.resize(goods.size(),1);

	res = m_owner->GetContainer(ePackageType)->IsBagEnought(goods,num,tempFlagList);

	if(!res)
	{
		vector<int> vecType;

		size_t t;
		for(t = 0; t < bglod.size(); t++)
		{
			m_owner->ChangeBindGolden(bglod[t], true);
		}
		for(t = 0; t < money.size(); t++)
		{
			m_owner->ChangeMoney(money[t], true);

//			StatisticInfo::CostAndGotInfo costAndGotInfo;
//			StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,money[t]);
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Money,eStaMinor_Money_OnLine,&costAndGotInfo);
		}
		for(t = 0; t < zhenqi.size(); t++)
		{
			m_owner->ChangeCurForce(zhenqi[t], true);
		}

		bindVec.resize(goods.size(), 1);
		strengthLvVec.resize(goods.size(), 0);
		m_owner->GetContainer(ePackageType)->AddItem(goods, num, vecPos, bindVec,strengthLvVec,true,npc_fly_type);


		vecType.push_back(eCharBindGolden);
		vecType.push_back(eCharMoney);
		vecType.push_back(eCharForce);

		for(i = 0; i < info.list_size(); i++)
		{
			int mbit = info.list(i) -1;
			m_historyReward.SetBit(mbit, true); //********************** 在线时长 于奖励 标志要一致
		}

		m_owner->SetDataFlag(eBaseInfo);
		m_owner->SetDataFlag(eBattleInfo);

		m_owner->SynCharAttribute(vecType);

//		eLogicRelevant fun_enum = eStaMinor_Reward_GetOnlinePrize;
//		if(info.list_size() >0)
//			fun_enum = eStaMinor_Reward_GetAllPrize;

		EventArgs tArgs;
		m_owner->FireEvent(PLAYER_ONLINE_GIFT,tArgs);

		//统计 获取 在线奖励
//		StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(), eStatic_Reward, fun_enum, 1);
		return 0;
	}
	else
	{
		return res;
	}
}

void OnlineReward::ResetOnlineReward()
{
	unsigned char signinBuf[4] = {0};
	m_onlineReward.SetData(signinBuf, 4);

	unsigned char historyBuf[4] = {0};
	m_historyReward.SetData(historyBuf, 4);

	m_owner->SetDataFlag(eBaseInfo);

	RewardHail::ClientOlineRewardInfo info;
	info.set_rewardinfo(GetHistoryReward());
	info.set_time(0);
	QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(), &info, MSG_REQ_C2GT_ONLINEREWARDINFO);
}

bool OnlineReward::OnlineReward_IsHaveReward()
{
	for(int i =0; i < OnlineBitCounter; ++i)
	{
		bool onFlag = m_onlineReward.GetBit(i);
		bool reFlag = m_historyReward.GetBit(i);
		if( onFlag && !reFlag)
			return true;
	}
	return false;
}

int OnlineReward::GetOLCanGetAward()
{
	int count = 0;
	for (int i = 0; i < OnlineBitCounter; ++i)
	{
		bool onFlag = m_onlineReward.GetBit(i);
		bool reFlag = m_historyReward.GetBit(i);
		if( onFlag && !reFlag)
		{
			++count;
		}
	}
	return count;
}

void OnlineReward::CalOnlineReward()
{
	int64 tm = m_owner->CalOnlineTime();
	tm = tm + 10000; // 10秒误差

	int canNum = CalOnlineNum(tm);
	bool Flag = false;
	int cnt = 0;

	for(int i=0; i<=canNum; ++i)
	{
		if(!m_onlineReward.GetBit(i))
		{
			m_onlineReward.SetBit(i, true);
			Flag = true;
			++cnt;
		}
	}

	//在线时长 活动图标更新
	if(Flag)
	{
		m_owner->SetDataFlag(eBaseInfo);
		if (m_owner->IsOpenSystem(eStOnlineRewardLv) && 0 < cnt)
		{
			m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanGetOnlineAward), cnt, true);
		}
	}
}

int OnlineReward::CalOnlineNum(int min)
{
	const map<int, OnlineRewardStruct>& onLineInfo = OnlineRewardLoader::GetInstance()->getOnLineRewardData();

	int num = -1;
	for (map<int, OnlineRewardStruct>::const_iterator it = onLineInfo.begin(); it != onLineInfo.end(); ++it)
	{
		if (min >= it->second.i_online)
		{
			++num;
		}
		else
		{
			break;
		}
	}

	return num;
}

bool OnlineReward::isHaveReceiveOneReward()
{
	const map<int,OnlineRewardStruct>& tempDataList    = OnlineRewardLoader::GetInstance()->getOnLineRewardData();
	map<int,OnlineRewardStruct>::const_iterator tempIt = tempDataList.begin();

	for(;tempIt != tempDataList.end();++tempIt)
	{
		if(m_historyReward.GetBit(tempIt->first - 1))
			return true;
	}

	return false;
}










