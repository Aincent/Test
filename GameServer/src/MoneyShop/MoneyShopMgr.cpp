/*
 * MoneyShopMgr.cpp
 *
 *  Created on: 2014年8月9日
 *      Author: root
 */
#include "MoneyShopMgr.h"
#include "Log/Logger.h"
#include "CharDefine.h"
#include "MessageCommonRet.h"
#include "../Map/SceneManager.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "../ServerEventDefine.h"
#include "CounterDefine.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "../Daily/DailyLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "MessageStruct/StatisticInfo.pb.h"
#include "../ActiveUIHighLight/CActiveHighLightMgr.h"
#include "../StatisticMgr/StatisticHandle.h"

MoneyShopMgr::MoneyShopMgr()
{
	m_moneyShop = MoneyShopLoader::GetInstance()->GetGoMoneyShop();
}

MoneyShopMgr::~MoneyShopMgr()
{
}

int MoneyShopMgr::GiveMoney(int lv ,int freeCounts)
{
	int money = 0;
	int critValue = 0;
	MoneyShopLoader::GetInstance()->getMoney(lv,freeCounts,0,critValue,money);

	return money;
}

void MoneyShopMgr::GetMoneyShopInfo(const Smart_Ptr<Player>& player,
		MoneyShopInfo::AckMoneyShopInfo& info)
{
	if(!IsCanEnterMoneyShop(player))
	{
		info.set_ret(eLVNotEnough);
		return ;
	}
	info.set_freenum(player->GetCounterService()->GetNumById(EXCHANGFREE_DAY_MONEY));
	info.set_costnum(player->GetCounterService()->GetNumById(EXCHANGCOST_DAY_MONEY));
}

void MoneyShopMgr::ExchangeMoney(const Smart_Ptr<Player>& player,
		MoneyShopInfo::AckExchangeMoney* info)
{
	if(!IsCanEnterMoneyShop(player))
	{
		info->set_ret(eLVNotEnough);
		return ;
	}

	int type = JugeExchangeStaus(player);
	if(type != EXCHANGFREE_DAY_MONEY && type != EXCHANGCOST_DAY_MONEY)
	{
		info->set_ret(type);
		return ;
	}
	else
	{
		info->set_ret(0);
	}

	int tMoney 	   = 0;
	int tCritValue = 0;
	ExchangeMoney(player, type,tCritValue,tMoney);
	player->GetCounterService()->IncreasePlayerCounter(type, 1);

	int freeCount = player->GetCounterService()->GetNumById(EXCHANGFREE_DAY_MONEY);
	int costCount = player->GetCounterService()->GetNumById(EXCHANGCOST_DAY_MONEY);

	info->set_money(tMoney);
	info->set_freenum(freeCount);
	info->set_costnum(costCount);
	info->set_critvalue(tCritValue);

	EventArgs tempArgs;
	player->FireEvent(PLAYER_RECEIVE_MONEY_SHOP,tempArgs);

	//统计一天钱庄产生的铜钱数
//	if(eAccountType_User == player->GetAccountType())
//	{
//		StatisticMgr::GetInstance()->StatisticDailyTimes(eStatic_MoneyShop,eStaMinor_MoneyShop_GetMoney,tMoney);
	GLOBAL_STATISTIC_GM(player, GlobalType::Main::eMoneyShop, GlobalType::Minor::eMoneyShop_GetMoney, StatistSaveType::Int64_Data(tMoney));
//	}
}

void MoneyShopMgr::ExchangeMoney(const Smart_Ptr<Player>& player, int type,int& critValue,int& money)
{
//	StatisticInfo::CostAndGotInfo costAndGotInfo;
//	StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
//	StatisticMgr::SetItemCost(costItemInfo,0,0);
//	StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo.add_gotlist();


	int tLevel = player->getAttManage()->getValue<int>(eCharLv);
	int costNum = player->GetCounterService()->GetNumById(EXCHANGCOST_DAY_MONEY);
	int freeCount = player->GetCounterService()->GetNumById(EXCHANGFREE_DAY_MONEY);

	vector<int> vec;
	if(type == EXCHANGCOST_DAY_MONEY)
	{
		int cost = MoneyShopLoader::GetInstance()->getComsune(tLevel,costNum);

		//优先扣除绑定元宝
		int bindGolden = player->getAttManage()->getValue<int>(eCharBindGolden);
		if(bindGolden >= cost)
		{
			player->ChangeBindGolden(cost, false);
			vec.push_back(eCharBindGolden);
		}
		else
		{
			player->ChangeBindGolden(bindGolden,false);
			vec.push_back(eCharBindGolden);

//			StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,cost-bindGolden,player->GetMyself());

			player->ChangeGolden(cost-bindGolden,false);
			vec.push_back(eCharGolden);

//			StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,player->GetMyself());

			if(eAccountType_User == player->GetAccountType())
			{
				//统计兑换次数
//				StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(player, eStatic_MoneyShop,	eStaMinor_MoneyShop_ExchangeTimes);

				//统计每日钱庄 兑换消耗 元宝
//				StatisticMgr::GetInstance()->StatisticDailyTimes(eStatic_MoneyShop,	eStaMinor_MoneyShop_GoldIncome,	cost-bindGolden);
				GLOBAL_STATISTIC_GM(player, GlobalType::Main::eMoneyShop, GlobalType::Minor::eMoneyShop_BuyMoneyCostGolden, StatistSaveType::Int64_Data(bindGolden));
			}
		}
	}

    MoneyShopLoader::GetInstance()->getMoney(tLevel,freeCount,costNum,critValue,money);
	player->ChangeMoney(money, true);

//	StatisticMgr::AddItemGotInfo(gotItemInfo,eCharMoney,money);
//	if(eAccountType_User == player->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_MoneyShop,eStaMinor_MoneyShop_GetMoneyEvent,&costAndGotInfo);

	vec.push_back(eCharMoney);
	player->SynCharAttribute(vec);
	return ;
}

int MoneyShopMgr::JugeExchangeStaus(const Smart_Ptr<Player>& player)
{
	int vipTimes = 0;
	if(player->GetVipPart())
	{
		double times = player->GetVipPart()->GetValueByRight(eVipMoneyShopAdd);
		if (times != VIP_NOT_HAVE_THIS_RIGHT)
		{
			vipTimes = (int)times;
		}
	}

	int freeNum = player->GetCounterService()->GetNumById(EXCHANGFREE_DAY_MONEY);
	if(freeNum <m_moneyShop.freeMax)
	{
		return EXCHANGFREE_DAY_MONEY;
	}

	int costNum = player->GetCounterService()->GetNumById(EXCHANGCOST_DAY_MONEY);
	if(costNum >= (m_moneyShop.costMax + vipTimes))
	{
		return eMoneyShopCostNumNotEnough;
	}

	//绑定元宝和元宝是否足够
	int cost = MoneyShopLoader::GetInstance()->getComsune(player->getAttManage()->getValue<int>(eCharLv),costNum);
	int total = player->getAttManage()->getValue<int>(eCharGolden) + player->getAttManage()->getValue<int>(eCharBindGolden);
	if(total < cost)
	{
		return eNotEnoughGold;
	}

	return EXCHANGCOST_DAY_MONEY;
}

bool MoneyShopMgr::IsCanEnterMoneyShop(const Smart_Ptr<Player>& player)
{
	if(!player)
		return false;

	return player->IsOpenSystem(eStBank);
}

int MoneyShopMgr::MoneyShop_IsHaveFreeNum(const Smart_Ptr<Player>& player)
{
	if(!player)
		return 0;

	if(!IsCanEnterMoneyShop(player))
		return 0;

	int freeNum = player->GetCounterService()->GetNumById(EXCHANGFREE_DAY_MONEY);
	return  m_moneyShop.freeMax - freeNum;
}

int MoneyShopMgr::CalRecoreMoney(const Smart_Ptr<Player> &player, int num,Daily::RecoverResourceInfo *info)
{
	if(!IsCanEnterMoneyShop(player))
		return eLVNotEnough;

	int havenum = m_moneyShop.freeMax - num;
	if(havenum <=0)
		return eMoneyShopCostNumNotEnough;

	sRecorceSourceParam *ptr = DailyLoader::GetInstance()->GetRecorceSourceParamPtr(eRecorceSourceParamType_moneyShop);
	if(!ptr)
		return 0;

	int money = havenum * GiveMoney(player->getAttManage()->getValue<int>(eCharLv),havenum);

	Daily::RecoverResourceReward * reward = info->mutable_reward();
	reward->set_money(money);
	info->set_restcount(havenum);
	info->set_id(eRecorceSourceParamType_moneyShop);

	return 0;
}

int MoneyShopMgr::GetMoneyShopFreeMax()
{
	return m_moneyShop.freeMax;
}









