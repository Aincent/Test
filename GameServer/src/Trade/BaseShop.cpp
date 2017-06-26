/*
 * BaseShop.cpp
 *
 *  Created on: 2016年11月7日
 *      Author: root
 */

#include "BaseShop.h"
#include "MessageCommonRet.h"
#include "../Counter/Counter.h"
#include "../Container/ContainerBase.h"
#include "../Object/Player/Player.h"
#include "../Attribute/AttributeCreature.h"
#include "../Country/GsCountryMgr.h"
#include "util.h"
#include "Log/Logger.h"

BaseShopMgr::BaseShopMgr(Player * obj):m_nFlushTime(0),m_owner(obj)
{
	m_myster.clear();
	m_limitbuy.clear();

	if(m_reflushTime.IsValid())
		m_reflushTime.SetNull();

	time_t curtime=time(0);
	tm tim;
	localtime_r(&curtime,&tim);

	int curMin = tim.tm_min;
	int curSec = tim.tm_sec;

	int64 beginTime = 0;

	//每天的整点刷新一遍
	beginTime = 1 * 60 * 60 * 1000 - curMin * 60 * 1000 - curSec * 1000;
	m_reflushTime = TimerInterface::GetInstance()->RegRepeatedTime(&BaseShopMgr::FlushMyster, this, 0, 0, 0, 0, beginTime,1*60*60*1000);
}

BaseShopMgr::~BaseShopMgr()
{
	if(m_reflushTime.IsValid())
		m_reflushTime.SetNull();
}

void BaseShopMgr::InitBaseShopInfo(const PlayerInfo::BaseShopInfo& baseShopInfo)
{
	bool bFlushMyter = GetFlushState(baseShopInfo.flushtime());
	eDayResetType tempType = m_owner->GetCounterService()->getResetDayType();
	if(tempType == eDayResetToday)
	{
		for(int i = 0; i < baseShopInfo.list_size(); i++)
		{
			DWORD type = baseShopInfo.list(i).type();
			if(type == eMysteryShop && !bFlushMyter)
			{
				for(int j = 0; j < baseShopInfo.list(i).data_size(); j++)
				{
					const ShopEntry* pEnty = ShopLoader::GetInstance()->GetShopEntry(type,baseShopInfo.list(i).data(j).id());
					m_myster.push_back(*pEnty);
				}

				LOG_DEBUG(FILEINFO,"InitBaseShopInfo ............................. %d",m_myster.size());
			}
			else
			{
				for(int j = 0; j < baseShopInfo.list(i).data_size(); j++)
				{
					m_limitbuy[type][baseShopInfo.list(i).data(j).id()] =baseShopInfo.list(i).data(j).nums();
				}
			}
		}

		if(bFlushMyter)
		{
			FlushMyster();
			m_nFlushTime = CUtil::GetNowSecond();
			m_owner->SetDataFlag(eBaseShopInfo);
		}
	}
	else
	{
		ResetBaseInfo();
	}
}

void BaseShopMgr::SetBaseShopInfo(PlayerInfo::BaseShopInfo *baseShopInfo)
{
	baseShopInfo->set_flushtime(m_nFlushTime);

	map<DWORD,map<DWORD,DWORD> >::iterator itr = m_limitbuy.begin();
	for(; itr != m_limitbuy.end(); ++itr)
	{
		PlayerInfo::BaseShopInfoList* plist = baseShopInfo->add_list();
		plist->set_type(itr->first);
		map<DWORD,DWORD>::iterator it = itr->second.begin();
		for(; it != itr->second.end(); ++it)
		{
			PlayerInfo::BaseShopInfoData* pData = plist->add_data();
			pData->set_id(it->first);
			pData->set_nums(it->second);
		}
	}

	PlayerInfo::BaseShopInfoList* plist = baseShopInfo->add_list();
	plist->set_type(eMysteryShop);
	vector<ShopEntry>::iterator it = m_myster.begin();
	for(; it != m_myster.end(); ++it)
	{
		PlayerInfo::BaseShopInfoData* pData = plist->add_data();
		pData->set_id(it->m_nItemId);
		pData->set_nums(0);
	}

	LOG_DEBUG(FILEINFO,"SetBaseShopInfo ............................. %d",m_myster.size());
}

void BaseShopMgr::ReleaseBaseShopMgr()
{
	m_myster.clear();
	m_limitbuy.clear();

	if(m_reflushTime.IsValid())
		m_reflushTime.SetNull();
}

void BaseShopMgr::ResetBaseInfo()
{
	m_limitbuy.clear();
	FlushMyster();
	m_nFlushTime = CUtil::GetNowSecond();
	m_owner->SetDataFlag(eBaseShopInfo);
}

void BaseShopMgr::BuyItem(BaseShopInfo::ClientReqBuyItem& req,BaseShopInfo::AckBuyItem& toClient)
{
	//判断背包是否已满
	vector<DWORD> itemVec;
	vector<DWORD> numVec;
	vector<DWORD> strengthLvVec;
	vector<WORD> bindVec;
	vector<int> vecLock;
	int errNum = 0;

	const ShopEntry* pEnty = ShopLoader::GetInstance()->GetShopEntry(req.type(),req.id());
	if(!pEnty)
	{
		toClient.set_ret(eMallGoodsNull);
		return;
	}

	if(req.num() <= 0)
	{
		toClient.set_ret(eClientSendDataError);
		return;
	}

	itemVec.push_back(pEnty->m_nItemId);
	numVec.push_back(req.num() * pEnty->m_nItemCount);
	errNum = m_owner->GetContainer(ePackageType)->IsBagEnought(pEnty->m_nItemId, req.num() * pEnty->m_nItemCount,1);
	if(errNum != 0)
	{
		toClient.set_ret(errNum);
		return;
	}

	switch(req.type())
	{
	case eCarryShop:
		{
			int needMoney = pEnty->m_nPrice * req.num();
			int money = m_owner->getAttManage()->getValue<int>(eCharMoney);
			money -= needMoney;
			if(money<0)
			{
				toClient.set_ret(eNotEnoghMoney);
				return;
			}

			m_owner->MoneyChange(needMoney,false);

			strengthLvVec.resize(itemVec.size(), 0);
			bindVec.resize(itemVec.size(),1);

			m_owner->GetContainer(ePackageType)->AddItem(itemVec, numVec, vecLock, bindVec,strengthLvVec,true,shop_buy_type);
			break;
		}
	case eMysteryShop:
		{
			int needPrice = pEnty->m_nPrice * req.num();

			if(!FindMysterItemId(pEnty->m_nItemId))
			{
				toClient.set_ret(eClientSendDataError);
				return;
			}

			if(pEnty->m_nCurrencyType == 2)
			{
				int money = m_owner->getAttManage()->getValue<int>(eCharMoney);
				money -= needPrice;
				if(money<0)
				{
					toClient.set_ret(eNotEnoghMoney);
					return;
				}

				m_owner->MoneyChange(needPrice,false);
			}
			else
			{
				int golden = m_owner->getAttManage()->getValue<int>(eCharGolden);
				golden -= needPrice;
				if(golden<0)
				{
					toClient.set_ret(eNotEnogGolden);
					return;
				}

				m_owner->ChangeGolden(needPrice,false);
			}

			strengthLvVec.resize(itemVec.size(), 0);
			bindVec.resize(itemVec.size(),1);

			m_owner->GetContainer(ePackageType)->AddItem(itemVec, numVec, vecLock, bindVec,strengthLvVec,true,shop_buy_type);

			break;
		}
	case eClanShop:
		{
			int limit = GetLimitbuy(eClanShop,pEnty->m_nItemId);
			if(pEnty->m_nDailyLimit < (DWORD)(limit + req.num()))
			{
				toClient.set_ret(eItemNotEnough);
				return;
			}

			int contr = m_owner->getAttManage()->getValue<int>(eFaContribution);
			int needPrice = pEnty->m_nPrice * req.num();
			contr -= needPrice;
			if(contr<0)
			{
				toClient.set_ret(eNotEnoughFContri);
				return;
			}

			int totalContr = m_owner->ChangeFContri(needPrice, false);
			m_owner->SynCharAttribute(eFaContribution,totalContr);
			m_owner->SynchCharAttributeToWS(eFaContribution,0-needPrice);

			strengthLvVec.resize(itemVec.size(), 0);
			bindVec.resize(itemVec.size(),1);

			m_owner->GetContainer(ePackageType)->AddItem(itemVec, numVec, vecLock, bindVec,strengthLvVec,true,shop_buy_type);
			toClient.set_num(pEnty->m_nDailyLimit -(limit + req.num()));

			AddLimitbuy(eClanShop,pEnty->m_nItemId,req.num());
			break;
		}
	case eCampShop:
		{
			int limit = GetLimitbuy(eCampShop,pEnty->m_nItemId);
			if(pEnty->m_nDailyLimit < (DWORD)(limit + req.num()))
			{
				toClient.set_ret(eItemNotEnough);
				return;
			}

			if((DWORD)GsCountryMgr::GetInstance()->getCampGrade(m_owner->getAttManage()->getValue<DWORD>(eCharCountry)) < pEnty->m_nCampLevel)
			{
				toClient.set_ret(eLVNotMatch);
				return;
			}

			int contribute = m_owner->getAttManage()->getValue<int>(eCurCountryContribute);
			int needPrice = pEnty->m_nPrice * req.num();
			contribute -= needPrice;
			if(contribute<0)
			{
				toClient.set_ret(eNotEnogCountryContribute);
				return;
			}

			m_owner->ChangeCountryContribution(needPrice, false);

			strengthLvVec.resize(itemVec.size(), 0);
			bindVec.resize(itemVec.size(),1);

			m_owner->GetContainer(ePackageType)->AddItem(itemVec, numVec, vecLock, bindVec,strengthLvVec,true,shop_buy_type);
			toClient.set_num(pEnty->m_nDailyLimit -(limit + req.num()));
			AddLimitbuy(eCampShop,pEnty->m_nItemId,req.num());

			break;
		}
	case eCreditShop:
		{
			int limit = GetLimitbuy(eCreditShop,pEnty->m_nItemId);
			if(pEnty->m_nDailyLimit < (DWORD)(limit + req.num()))
			{
				toClient.set_ret(eItemNotEnough);
				return;
			}

			int credit = m_owner->getAttManage()->getValue<int>(eCredit);
			int needPrice = pEnty->m_nPrice * req.num();
			credit -= needPrice;
			if(credit<0)
			{
				toClient.set_ret(eNotEnoughCredit);
				return;
			}

			int totalCredit = m_owner->ChangeCredit(needPrice, false);
			m_owner->SynCharAttribute(eCredit, totalCredit);
			m_owner->SynchCharAttributeToWS(eCredit, totalCredit);

			strengthLvVec.resize(itemVec.size(), 0);
			bindVec.resize(itemVec.size(),1);

			m_owner->GetContainer(ePackageType)->AddItem(itemVec, numVec, vecLock, bindVec,strengthLvVec,true,shop_buy_type);
			toClient.set_num(pEnty->m_nDailyLimit -(limit + req.num()));

			AddLimitbuy(eCreditShop,pEnty->m_nItemId,req.num());
			break;
		}
	case eHonorShop:
		{
			int limit = GetLimitbuy(eHonorShop,pEnty->m_nItemId);
			if(pEnty->m_nDailyLimit < (DWORD)(limit + req.num()))
			{
				toClient.set_ret(eItemNotEnough);
				return;
			}

			int needMoney = pEnty->m_nPrice * req.num();
			int money = m_owner->getAttManage()->getValue<int>(eCharMoney);
			money -= needMoney;
			if(money<0)
			{
				toClient.set_ret(eNotEnoghMoney);
				return;
			}

			m_owner->MoneyChange(needMoney,false);

			strengthLvVec.resize(itemVec.size(), 0);
			bindVec.resize(itemVec.size(),1);

			m_owner->GetContainer(ePackageType)->AddItem(itemVec, numVec, vecLock, bindVec,strengthLvVec,true,shop_buy_type);
			toClient.set_num(pEnty->m_nDailyLimit -(limit + req.num()));
			AddLimitbuy(eHonorShop,pEnty->m_nItemId,req.num());

			break;
		}
	default:
		{
			break;
		}
	}

	toClient.set_ret(0);
	toClient.set_id(req.id());
	toClient.set_type(req.type());
}

int BaseShopMgr::GetLimitbuy(int nType,int nId)
{
	map<DWORD,map<DWORD,DWORD> >::iterator itr = m_limitbuy.find(nType);
	if(itr != m_limitbuy.end())
	{
		map<DWORD,DWORD>::iterator it = itr->second.find(nId);
		if(it != itr->second.end())
		{
			return it->second;
		}
	}

	return 0;
}

void BaseShopMgr::AddLimitbuy(int nType,int nId,int nNum)
{
	map<DWORD,map<DWORD,DWORD> >::iterator itr = m_limitbuy.find(nType);
	if(itr != m_limitbuy.end())
	{
		map<DWORD,DWORD>::iterator it = itr->second.find(nId);
		if(it != itr->second.end())
		{
			it->second += nNum;
		}
		else
		{
			itr->second[nId] = nNum;
		}
	}
	else
	{
		m_limitbuy[nType][nId] = nNum;
	}

	m_owner->SetDataFlag(eBaseShopInfo);
}

void BaseShopMgr::GetMyster(BaseShopInfo::AckMysterInfo& toClient)
{
	vector<ShopEntry>::iterator itr = m_myster.begin();
	for(; itr != m_myster.end(); ++itr)
	{
		toClient.add_id(itr->m_nId);
	}
}

void BaseShopMgr::GetFlushMyster(BaseShopInfo::AckFlushMysterInfo& toClient)
{
	int price = 10;
	if(!m_owner->IsEnoughGolden(price))
	{
		toClient.set_ret(eNotEnogGolden);
		return ;
	}

	m_owner->ChangeGolden(price,false);

	FlushMyster();

	vector<ShopEntry>::iterator itr = m_myster.begin();
	for(; itr != m_myster.end(); ++itr)
	{
		toClient.add_id(itr->m_nId);
	}
}

void BaseShopMgr::FlushMyster()
{
	m_myster.clear();
	ShopLoader::GetInstance()->GetFlushMyster(m_myster);
	LOG_DEBUG(FILEINFO,"FlushMyster ............................. %d",m_myster.size());
	m_owner->SetDataFlag(eBaseShopInfo);
}

void BaseShopMgr::FlushMyster(void* arg)
{
	struct tm local;
	time_t t = 0;

	time(&t);
	localtime_r(&t,&local);

	if(local.tm_hour == 4 || local.tm_hour == 8 || local.tm_hour == 12 || local.tm_hour == 16 || local.tm_hour == 20 || local.tm_hour == 0)
	{
		FlushMyster();

		m_nFlushTime = CUtil::GetNowSecond();
	}
}

bool BaseShopMgr::GetFlushState(int64 nTime)
{
	if(nTime == 0)
	{
		return true;
	}

	int nHour = 0;
	struct tm *p;
	time_t t = nTime/1000;
	p = gmtime(&t);
	nHour = p->tm_hour;
	if(p)
	{
		struct tm local;
		time_t t = 0;
		time(&t);
		localtime_r(&t,&local);

		if(p->tm_hour == local.tm_hour)
		{
			return false;
		}

		if((p->tm_hour < 4 && local.tm_hour > 4) || (p->tm_hour < 8 && local.tm_hour > 8) || (p->tm_hour < 12 && local.tm_hour > 12) ||
				(p->tm_hour < 16 && local.tm_hour > 16) || (p->tm_hour < 20 && local.tm_hour > 20))
		{
			return true;
		}
	}

	return false;
}

void BaseShopMgr::GetBaseShopInfo(int nType,BaseShopInfo::AckBaseShopInfo& toClient)
{
	map<DWORD,map<DWORD,DWORD> >::iterator itr = m_limitbuy.find(nType);
	if(itr != m_limitbuy.end())
	{
		map<DWORD,DWORD>::iterator it = itr->second.begin();
		for(;it != itr->second.end();++it)
		{
			toClient.add_id(it->first);
			toClient.add_num(it->second);
		}
	}

	toClient.set_type(nType);
}

bool BaseShopMgr::FindMysterItemId(int nItemId)
{
	vector<ShopEntry>::iterator itr = m_myster.begin();
	for(; itr != m_myster.end(); ++itr)
	{
		if(itr->m_nItemId == (DWORD)nItemId)
		{
			return true;
		}
	}

	return false;
}
