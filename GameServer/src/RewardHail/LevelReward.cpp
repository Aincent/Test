/*
 * LevelReward.cpp
 *
 *  Created on: 2014年6月20日
 *      Author: root
 */

#include "LevelReward.h"
#include "util.h"
#include "../Container/ContainerBase.h"
#include "../Object/Player/Player.h"
#include "FileLoader/LevelRewardLoader.h"
#include "Log/Logger.h"
#include "MessageCommonRet.h"
#include "../ServerEventDefine.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "../Attribute/AttributeCreature.h"


LevelReward::LevelReward(Player * player):m_owner(player)
{

}

LevelReward::~LevelReward()
{
	Release();
}

void LevelReward::ReInit()
{
	m_levelReward.Init();
}

void LevelReward::Release()
{
	m_levelReward.Release();
}

void LevelReward::InitLevelReward(int levelRecord)
{
//	m_levelReward.SetSize(8);

	unsigned char buf[4] = {0};

	CUtil::SafeMemmove(buf, 4, &levelRecord, 4);
	m_levelReward.SetData(buf, 4);
}

int LevelReward::GetRecord()
{
	int len = 0;
	unsigned char *buf = 0;

	buf = const_cast<unsigned char *>(m_levelReward.GetData(len));
	int levelReward = 0;
	CUtil::SafeMemmove(&levelReward, 4, buf, len);
	return levelReward;
}

int LevelReward::GetLevelReward(RewardHail::GetReward &reward, vector<DWORD> &goods, vector<DWORD> &num)
{
	int res = 0;
	vector<int> vecPos;
	vector<DWORD> strengthLvVec;
	vector<WORD> bindVec;
	int type = reward.list(0);

	if(m_levelReward.GetBit(type-1))
	{
		return eHaveGetReward;
	}

	vector<int> vecType;
	LevelRewardStruct *info = LevelRewardLoader::GetInstance()->GetLevelRewardLoader(type);

	if(!info)
	{
		LOG_ERROR(FILEINFO, "level reward config [type = %d] is error", type);
		return eRewardHailConfigError;
	}

	if(m_owner->getAttManage()->getValue<int>(eCharLv) < info->i_lv)
	{
		return eLVNotEnough;
	}

	vector<WORD> tempFlagList;
	tempFlagList.resize(info->m_nAward.size(),1);

	res = m_owner->GetContainer(ePackageType)->IsBagEnought(info->m_nAward, info->m_nAwardNum,tempFlagList);

	goods.insert(goods.end(), info->m_nAward.begin(), info->m_nAward.end());
	num.insert(num.end(), info->m_nAwardNum.begin(), info->m_nAwardNum.end());

//	if (m_owner->GetVipPart())
//	{
//		int right = (int)m_owner->GetVipPart()->GetValueByRight(eVipLevelRewardExtraAward);
//		if (VIP_HAVE_THIS_RIGHT == right)
//		{
//			// VIP奖励
//			goods.insert(goods.end(), info->m_nVipAward.begin(), info->m_nVipAward.end());
//			num.insert(num.end(), info->m_nVipAwardNum.begin(), info->m_nVipAwardNum.end());
//		}
//	}

	if(!res)
	{
		m_levelReward.SetBit(type-1, true);

		m_owner->ChangeBindGolden(info->i_bglod, true);
		m_owner->ChangeMoney(info->i_money, true);
		m_owner->ChangeCurForce(info->i_zhenqi, true);
		bindVec.resize(goods.size(), 1);
		strengthLvVec.resize(goods.size(), 0);
		m_owner->GetContainer(ePackageType)->AddItem(goods, num, vecPos, bindVec,strengthLvVec,true,npc_fly_type);


		vecType.push_back(eCharBindGolden);
		vecType.push_back(eCharMoney);
		vecType.push_back(eCharForce);
		m_owner->SetDataFlag(eBaseInfo);
		m_owner->SetDataFlag(eBattleInfo);
		m_owner->SynCharAttribute(vecType);

		PlayerLevelGift tempArgs(m_owner->GetMyself(),type);
		m_owner->FireEvent(PLAYER_LEVEL_GIFT,tempArgs);

//		//统计 领取在线奖励
//		StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(), eStatic_Reward, eStaMinor_Reward_GrowUpPrize, 1);
//
//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//
//		for(uint i = 0;i < goods.size();++i)
//		{
//			StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotInfo,goods[i],num[i]);
//		}
//
//		if(info->i_bglod > 0)
//		{
//			StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotInfo,eCharBindGolden,info->i_bglod);
//		}
//
//		if(info->i_money > 0)
//		{
//			StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,info->i_money);
//		}
//
//		if(info->i_zhenqi > 0)
//		{
//			StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotInfo,eCharForce,info->i_zhenqi);
//		}
//
//		if(eAccountType_User == m_owner->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Reward,eStaMinor_Reward_Lv,&costAndGotInfo);

		return 0;
	}
	return res;
}

bool LevelReward::LevelReward_IsHaveReward()
{
	int lv = m_owner->getAttManage()->getValue<int>(eCharLv);
	lv = lv /10 ;
	for(int i=0; i< lv; ++i)
	{
		if(!m_levelReward.GetBit(i))
			return true;
	}
	return false;
}

int LevelReward::GetLRCanGetAward()
{
	int lv = m_owner->getAttManage()->getValue<int>(eCharLv);
	lv = lv / 10;
	int count = 0;


	const map<int, LevelRewardStruct>& levelData = LevelRewardLoader::GetInstance()->getLevelDataList();

	uint rewardCnt = levelData.size();
	for(uint i=0; i< rewardCnt; ++i)
	{
		LevelRewardStruct *info = LevelRewardLoader::GetInstance()->GetLevelRewardLoader(i+1);
		if (NULL == info)
		{
			continue;
		}

		if(info->i_lv <=  lv*10 &&!m_levelReward.GetBit(i))
		{
			++count;
		}
	}
	return count;
}
//
//bool LevelReward::HandleCanLevelReward(const EventArgs& e)
//{
//	int lv = m_owner->getAttManage()->getValue<int>(eCharLv);
//	lv = lv /10 ;
//	if(lv >=1)
//	{
//		m_owner->SynchActiveHighLight();
//	}
//	return true;
//}

//参数指定ID的等级礼包是否已经领取
bool LevelReward::isSomeLevelRewardReceive(int id)
{
	return m_levelReward.GetBit(id - 1);
}

//是否已经领取过任意一个等级礼包
bool LevelReward::isAnyLevelRwardReceive()
{
	const map<int, LevelRewardStruct>& tempDataList   = LevelRewardLoader::GetInstance()->getLevelDataList();
	map<int,LevelRewardStruct>::const_iterator tempIt = tempDataList.begin();

	for(;tempIt != tempDataList.end();++tempIt)
	{
		if(m_levelReward.GetBit(tempIt->first - 1))
			return true;
	}

	return false;
}








