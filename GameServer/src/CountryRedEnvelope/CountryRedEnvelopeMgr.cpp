/*
 * CountryRedEnvelopeMgr.cpp
 *
 *  Created on: 2015年4月9日
 *      Author: root
 */
#include "CountryRedEnvelopeMgr.h"
#include "MessageCommonRet.h"
#include "util.h"
#include "CountryDefine.h"
#include "../Object/Player/Player.h"
#include "FightDefined.h"
#include "RedEnvelopeDefine.h"
#include "CountryRedEnvelopeLoader.h"
#include "./FileLoader/GoodsLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "CharDefine.h"
#include "./FileLoader/GameParaLoader.h"

CountryRedEnvelopeMgr::CountryRedEnvelopeMgr(Player* player):m_owner(player),m_maxEnvelopeToday(0)
{
}

CountryRedEnvelopeMgr::~CountryRedEnvelopeMgr()
{
	Release();
}

void CountryRedEnvelopeMgr::ReInit()
{
	m_maxEnvelopeToday = 0;
}

void CountryRedEnvelopeMgr::Release()
{
	m_maxEnvelopeToday = 0;
}

int CountryRedEnvelopeMgr::GetCanGetEnvelopeCountMax(int lv)
{
	return CountryRedEnvelopeLoader::GetInstance()->GetCanGetEnvelopeCountMax(lv);
}

int CountryRedEnvelopeMgr::GetCurrentCanGetEnvelopeCount()
{
	int maxCount = m_owner->getMaxCanGetEnvelopeCount();
	if(maxCount <= 0)
		return 0;

	int haveGetEnvelopCount = m_owner->GetCounterService()->GetNumById(GET_ENVELOP_COUNT);

	int restCount = maxCount - haveGetEnvelopCount;
	if(restCount <= 0)
		return 0;

	return restCount;
}

int CountryRedEnvelopeMgr::GetEnvelopeReward(const RedEnvelopeProto::ReqGMGetEnvelopeReward& req)
{
	int op = req.op();
	if(op < GetEnvelopeRewardType_count || op >= GetEnvelopeRewardType_max)
		return eGetEnvelopeTypeError;

	int num = req.num();

	if(GetEnvelopeRewardType_bindGolden == op)
	{
		m_owner->ChangeBindGolden(num,true);

		vector<int> vec;
		vec.push_back(eCharBindGolden);
		m_owner->SynCharAttribute(vec);

//		if (num > 0)
//		{
//			StatisticInfo::CostAndGotInfo costAndGotInfo;
//			StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotInfo,eCharBindGolden,num);
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_BindGolden,eStaMinor_BindGolden_Envelope,&costAndGotInfo);
//		}

		return 0;
	}

	int itemID = 0;
	if(GetEnvelopeRewardType_count == op)
		itemID = CountryRedEnvelopeLoader::GetInstance()->getGetCountGotRewardItemId();
	else if(GetEnvelopeRewardType_acc == op)
		itemID = CountryRedEnvelopeLoader::GetInstance()->getAccCountGotRewardId();

	const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(itemID);
	if(ginfo==NULL)
	{
		return eRewardItemError;
	}

	vector<DWORD> tempItemIDList;
	vector<DWORD> tempNumList;
	vector<WORD> tempFlagList;
	vector<int> vecPos;
	vector<DWORD> strengthLvVec;

	ginfo->m_ContainData.getItemList(tempItemIDList,tempNumList,(ECharProf)m_owner->getAttManage()->getValue<BYTE>(eCharProfession));
	for(size_t i = 0;i < tempNumList.size();++i)
	{
		tempNumList[i] *= num;

		if(tempItemIDList[i] == DWORD(GameParaLoader::GetInstance()->getExchangeMoneyItemId()))
		{
			if (tempNumList[i] > 0)
			{
//				StatisticInfo::CostAndGotInfo costAndGotInfo;
//				StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,tempNumList[i]);
//				if(eAccountType_User == m_owner->GetAccountType())
//					StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Money,eStaMinor_Money_Envelope,&costAndGotInfo);
			}
		}
	}
	tempFlagList.resize(tempItemIDList.size(),ginfo->getFlag());
	strengthLvVec.resize(tempItemIDList.size(),ginfo->equipStrengthLv);

	m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Country, eUIHLSmall_Country_HaveUnGetReward), num, false);

	int ret = m_owner->GetContainer(ePackageType)->AddItem(tempItemIDList,tempNumList, vecPos, tempFlagList,strengthLvVec);

	return ret;
}
