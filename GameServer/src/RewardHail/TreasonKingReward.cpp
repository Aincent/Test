/*
 * TreasonKingReward.cpp
 *
 *  Created on: 2014年11月27日
 *      Author: root
 */

#include "TreasonKingReward.h"
#include "../Activity/FileLoader/TreasonKingLoader.h"
#include "MessageCommonRet.h"
#include "../Container/ContainerBase.h"
#include "../Object/Player/Player.h"

TreasonKingReward::TreasonKingReward(Player * player):m_owner(player)
{
//	setDateTime(0, 0, 0, 0, -1, 0, TRIGGER_BY_MONTH_ID +1);
}

TreasonKingReward::~TreasonKingReward()
{
	Release();
}

void TreasonKingReward::ReInit()
{
	m_Reward.Init();
}

void TreasonKingReward::Release()
{
	m_Reward.Release();
}

void TreasonKingReward::InitTreasonKingReward(int reward)
{
	unsigned char buf[4] = {0};

	if(m_owner->GetCounterService()->getResetDayType() != eDayResetToday)
	{
		reward = 0;
	}

	CUtil::SafeMemmove(buf, 4, &reward, 4);
	m_Reward.SetData(buf, 4);
}

int TreasonKingReward::GetRewardInfo()
{
	int len = 0;
	unsigned char *buf = 0;

	buf = const_cast<unsigned char *>(m_Reward.GetData(len));
	int reward = 0;
	CUtil::SafeMemmove(&reward, 4, buf, len);
	return reward;
}

int TreasonKingReward::GetLevelsReward(int nLevels)
{
	int res = 0;
	QuantityArticle qa;

	if(m_Reward.GetBit(nLevels-1))
	{
		res =  eHaveGetReward;
	}
	else
	{
		TreasonKingLoader::GetInstance()->GetReward(nLevels-1,qa);
		if(qa.nArticle > 0)
		{
			//判断背包是否已满
			res = m_owner->GetContainer(ePackageType)->IsBagEnought(qa.nArticle, qa.nNum);
			if(res)
			{
				return res;
			}

			vector<int> tempPosList;
			int bindType = TreasonKingLoader::GetInstance()->getGoRewardBindType();
			if(m_owner->GetContainer(ePackageType)->AddItem(qa.nArticle,qa.nNum,tempPosList,bindType) != 0)
			{
				res = eContainerFull;
			}
			else
			{
				m_Reward.SetBit(nLevels-1, true);
			}
		}
	}

	return res;
}

void TreasonKingReward::ResetTreasonKingReward()
{
	unsigned char signinBuf[4] = {0};
	m_Reward.SetData(signinBuf, 4);
}
