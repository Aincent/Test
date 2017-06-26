
/*
 * DanLuMgr.cpp
 *
 *  Created on: 2014年8月7日
 *      Author: root
 */

#include "DanLuMgr.h"
#include "Log/Logger.h"
#include "./DanLuLoader.h"
#include "CharDefine.h"
#include "ServerMsgDefine.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "../Map/SceneManager.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "../ServerEventDefine.h"
#include "../FileLoader/MallLoader.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "../SysSetting/SysSettingLoader.h"
#include "FileLoader/GameParaLoader.h"
#include "math.h"
#include "../Attribute/AttributeCreature.h"
#include "../Fighting/Fighting.h"
#include "../Meridians/FileLoader/MeridianLoader.h"
#include "../Attribute/AttributeManage.h"
#include "../Mail/MailMessage.h"
#include "./FileLoader/InternationalLoader.h"

DanLuMgr::DanLuMgr(Player* player)
:m_owner(player)
{
	m_itemCntr.clear();

	ReInit();
}

DanLuMgr::~DanLuMgr()
{
	this->ReleaseDanLuMgr();
}

void DanLuMgr::ReInit()
{
	m_id = -1;
	m_exp = 0;
	m_point = 0;
	m_itemCntr.clear();

	if(!m_lvEvent.IsValid())
		m_lvEvent = RegEvent(m_owner, PLAYER_LEVEL_UP_EVENT_TYPE, this, &DanLuMgr::HandleCanEnterDanLu);//进入丹道系统
}

void DanLuMgr::ReleaseDanLuMgr()
{
	m_itemCntr.clear();

	if (m_lvEvent.IsValid())
	{
		m_lvEvent.SetNull();
	}
}

void DanLuMgr::InitDanLuFromDB(const PlayerInfo::DanLuInfo& info)
{
	m_id = info.danluid();
	m_exp = info.exp();
	m_point = info.point();

//	m_produceNum = info.producenum();

	DanLuInfo::ProduceItem item;
	for (int i = 0; i < info.itemid_size(); ++i)
	{
		item.Clear();
		item.m_itemID = info.itemid(i);
		if (0 < info.itemnum_size())
		{
			if (0 >= info.itemnum(i))
			{
				continue;
			}
			else
			{
				item.m_itemCount = info.itemnum(i);
			}
		}
		else
		{
			// 为了兼容 2015-7-22 之前的版本 -----
			// m_itemCntr[info.itempos(i)] = make_pair(info.itemid(i), 1);
			// ---------------------------
			// 兼容 2016-1-8 之前的版本
			item.m_itemCount = 1;
		}
		m_itemCntr.push_back(item);
	}

	// 熔炼值超过上限值的操作 主要是兼容老玩家
	HandlePointMoreCeil();

	/*上限 判断是否 有丹道信息*/
	this->HandleDanLu(true);

	SetVitality();

	m_owner->SetInitPartDataFlag(eDanLuInfo);
}

void DanLuMgr::SetVitality()
{
	if (DanLuLoader::GetInstance()->IsMaxDanLuID(m_id))
	{
		sVitality *vi = VitalityLoader::GetInstance()->GetVitalityInfoByID(eViDanLuUp);
		if(vi)
			m_owner->GetVitalityMgr()->FinshVitality(eViDanLuUp,vi->count);
	}
}


void DanLuMgr::SetDanLuToDB(PlayerInfo::DanLuInfo* info)
{
	info->set_danluid(m_id);
	info->set_exp(m_exp);
	info->set_point(m_point);
//	info->set_producenum(m_produceNum);

	list<DanLuInfo::ProduceItem>::iterator it = m_itemCntr.begin();
	for (; it != m_itemCntr.end();	it++)
	{
		// info->add_itempos(it->first);
		info->add_itemid(it->m_itemID);
		info->add_itemnum(it->m_itemCount);
	}
}

void DanLuMgr::HandlePointMoreCeil()
{
	int pointCeiling = DanLuLoader::GetInstance()->GetLianEquipCeiling();

	int produceNum = m_point / pointCeiling; // 产品数量 不是物品数量
	int suplusPoint = m_point % pointCeiling; // 剩余熔炼值
	vector<DanLuInfo::ProduceItem> vecItem;
	DanLuInfo::ProduceItem item;
	for (int i = 0; i < produceNum; ++i)
	{
		item.Clear();
		// 随机物品
		RandItem(item);
		if (item.IsInvalid() || NULL == GoodsLoader::GetInstance()->GetItemDataByID(item.m_itemID))
		{
			return;
		}

		vecItem.push_back(item);
	}

	if (0 < vecItem.size() && vecItem.size() == (uint)produceNum)
	{
		// 添加到容器
		m_itemCntr.insert(m_itemCntr.end(), vecItem.begin(), vecItem.end());
		m_point = suplusPoint;
	}
}

//上线计算属性
void DanLuMgr::onlineCalculateAtt()
{
	map<ECharAttrType,int64> tempAttList;

	getDataAttList(m_id,tempAttList);

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttDanlu);

	m_owner->calculateResultAddAtt(tempAttList,tempValueType);

	calculateFight();

	return ;
}

//计算一次战斗力
int DanLuMgr::calculateFight()
{
	map<ECharAttrType,int64> tempAttList;

	getDataAttList(m_id,tempAttList);

	int tempResult = m_owner->getFightingManage()->CalculateFighting(tempAttList);
	return m_owner->getFightingManage()->setFighting(eFightingDanLu,tempResult);
}

void DanLuMgr::GetDanLuInfo(DanLuInfo::AckPlayerDanLuInfo& info)
{
	if (!IsOpenDanLuSystem())
	{
		//等级不足时 不返回错误码 写死 初始丹道 仅为了 客户端显示 所需
		info.set_ret(0);
		info.set_id(GameParaLoader::GetInstance()->getInitDanLuId());
		info.set_exp(0);
		return ;
	}

	GetNowDanLuInfo(info);
}

void DanLuMgr::GetNowDanLuInfo(DanLuInfo::AckPlayerDanLuInfo &info)
{
	info.set_ret(0);
	info.set_id(m_id);
	info.set_exp(m_exp);

	info.set_point(m_point);

	list<DanLuInfo::ProduceItem>::iterator it = m_itemCntr.begin();
	for (; it != m_itemCntr.end();	++it)
	{
		DanLuInfo::SmeltItem* pItem = info.add_item();
		pItem->set_itemid(it->m_itemID);
		pItem->set_itemnum(it->m_itemCount);
//		pItem->set_itempos(it->first);
	}
}

void DanLuMgr::GetDanLuInfo(int *id, int *exp)
{
	*id = m_id;
	*exp = m_exp;
}

int DanLuMgr::CalDanLuFun(const LianLuInfo& info,eCommonRateType& type)
{
	int tempExp = info.m_BaseExp;
	switch(IsCrack(info.m_MaxCritRate,info.m_MinCritRate))
	{
	case eRateTypeBig:			//大爆击
		{
			tempExp = (int)(info.m_BaseExp * info.m_MinCrit);
			type    = eRateTypeBig;
			break;
		}
	case eRateTypeSmall:		//小爆击
		{
			tempExp = (int)(info.m_BaseExp * info.m_MinCrit);
			type    = eRateTypeSmall;
			break;
		}
	default:
		{
			type    = eRateTypeSmall;
			break;
		}
	}

	return tempExp;
}

ServerRet DanLuMgr::oneEatDanluBuckle(int& currentID,bool isBuy,bool isAuto,int& allExp,
		int& currentExp,map<int,int>& itemList,int& golden,int& money,map<eCommonRateType,DanLuExpStatistics>& resultExpList)
{
	LianLuInfo tempInfo;
	DanLuLoader::GetInstance()->GetLianLuInfo(currentID,tempInfo);

	if (tempInfo.id <= 0)
	{
		return eItemError;
	}

	if(tempInfo.m_LevelLimit > m_owner->getAttManage()->getValue<int>(eCharLv))
	{
		return eLVNotEnough;
	}

	if (DanLuLoader::GetInstance()->IsMaxDanLuID(currentID))
	{
		return eHighestLv;
	}

	if (m_owner->getAttManage()->getValue<int>(eCharMoney) < money + tempInfo.m_NeedMoney)
	{
		return eMoneyNotEnough;
	}

	int tempCount  = m_owner->GetContainer(ePackageType)->FindPropCount(tempInfo.m_ItemID);

	tempCount = tempCount - itemList[tempInfo.m_ItemID];
	if (tempCount < 0)
	{
		tempCount = 0;
	}

	if (tempInfo.upNeedNum > tempCount)
	{
		if (!isBuy)
		{
			return eNotEnogGoods;
		}

		itemList[tempInfo.m_ItemID] += tempCount;
		tempCount 	   				 = tempInfo.upNeedNum - tempCount;
		//花费元宝
		int tempGolden = MallLoader::GetInstance()->GetDanLuStoneCost(tempInfo.m_ItemID) * tempCount;

		Smart_Ptr<PlayerVipPart> tempVip = m_owner->GetVipPart();
		if (tempVip)
		{
			tempGolden = PlayerVipPart::CalBuyCostGolden(tempVip, tempGolden);
		}

		if (!m_owner->IsEnoughGolden(golden + tempGolden))
		{
			return eNotEnoughGold;
		}

		golden += tempGolden;
	}
	else
	{
		itemList[tempInfo.m_ItemID] += tempInfo.upNeedNum;
	}

	money += tempInfo.m_NeedMoney;

	eCommonRateType tempType;
	int tempGainExp = 0;
	int tempAddExp  = CalDanLuFun(tempInfo,tempType);

	ServerRet tempError = addExpResult(tempAddExp,currentID,currentExp,tempGainExp);

	resultExpList[tempType].m_Counts += 1;
	resultExpList[tempType].m_Exp 	 += tempGainExp;
	allExp += tempGainExp;

	m_owner->GetVitalityMgr()->FinshVitality(eViDanLuUp);

	return tempError;
}

void DanLuMgr::EatDanLu(DanLuInfo::AckPlayerDanLuInfo* info,DanLuInfo::ClientDanLuAddExp& req)
{
	if (!IsOpenDanLuSystem())
	{
		info->set_ret(eLVNotEnough);
		return ;
	}

	//说明是自动进阶 //2016-09-26 临时取消
//	if (req.type() == eDanLuAdvanceAuto)
//	{
//		if (!IsCanFastDanLu())
//		{
//			info->set_ret(eVipLevelNotEnough);
//			return;
//		}
//	}

	int tempcurrentID   = m_id;
	int tempallExp      = 0;
	int tempcurrentExp  = m_exp;
	int tempGgolden 	= 0;
	int tempMoney		= 0;
	ServerRet tempResultCode = eRetSuccess;
	int tempResultCounts 	 = 0;
	map<eCommonRateType,DanLuExpStatistics> tempResultExpList;
	map<int,int> tempItemList;

	bool tempIsFirst = true;

	ServerRet tempOneError = eRetSuccess;

	eDanLudvanceType tempType = (eDanLudvanceType)req.type();

	if(tempType == eDanLuAdvanceNormal)
	{
		tempResultCode =  oneEatDanluBuckle(tempcurrentID,req.isautobuy(),
				req.type() == eDanLuAdvanceAuto,tempallExp,tempcurrentExp,tempItemList,tempGgolden,tempMoney,tempResultExpList);
	}
	else
	{
		int tempMaxID = DanLuLoader::GetInstance()->getMaxID();
		int tempEatID = req.id();
		while(tempEatID <= tempMaxID && tempcurrentID < tempEatID)
		{
			tempOneError = oneEatDanluBuckle(tempcurrentID,req.isautobuy(),
					req.type() == eDanLuAdvanceAuto,tempallExp,tempcurrentExp,tempItemList,tempGgolden,tempMoney,tempResultExpList);

			if (tempOneError != eRetSuccess)
			{
				if(tempIsFirst)
				{
					tempResultCode = tempOneError;
				}
				break;
			}

			tempIsFirst = false;

			++tempResultCounts;
		}
	}

	info->set_stoperror(tempOneError);
	info->set_ret(tempResultCode);

	if (tempResultCode != eRetSuccess)
	{
		return ;
	}

//	StatisticInfo::IDAndCostAndGotInfo idAndCostAndGotInfo;
//	idAndCostAndGotInfo.set_beforeid(m_id);
//	StatisticInfo::CostAndGotInfo * costAndGotInfo = idAndCostAndGotInfo.mutable_costandgotlist();

	//扣物品
	if(tempItemList.size() > 0)
	{
		map<int,int>::iterator tempItemIt = tempItemList.begin();
		for(;tempItemIt != tempItemList.end();++tempItemIt)
		{
			if(tempItemIt->second == 0)
			{
				continue;
			}
			BaseArgs tempArgs;
			tempArgs.GoodsID = tempItemIt->first;
			tempArgs.Num     = tempItemIt->second;
			m_owner->GetContainer(ePackageType)->DeleteGoods(&tempArgs,true);

//			StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo->add_costlist();
//			StatisticMgr::SetItemCost(costItemInfo,tempItemIt->first,tempItemIt->second);
		}
	}

	vector<int> tempAtt;
	if (tempMoney > 0)
	{
//		StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo->add_costlist();
//		StatisticMgr::SetItemCostBefore(costItemInfo,eCharMoney,tempMoney,m_owner->GetMyself());

		m_owner->MoneyChange(tempMoney,false);
		tempAtt.push_back(eCharMoney);

//		StatisticMgr::SetItemCostAfter(costItemInfo,eCharMoney,m_owner->GetMyself());
	}

	//扣除元宝
	int tempbuckleGolden 	 = 0;
	int tempbuckleBindGolden = 0;
	if (tempGgolden > 0)
	{
//		StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo->add_costlist();
//		StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,m_owner->GetMyself());
//
//		StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo->add_costlist();
//		StatisticMgr::SetItemCostBefore(costBGoldenInfo,eCharBindGolden,m_owner->GetMyself());

		m_owner->DecreaseGoldenCost(tempGgolden,tempbuckleGolden,tempbuckleBindGolden);

		if(tempbuckleGolden > 0)
			tempAtt.push_back(eCharGolden);

		if(tempbuckleBindGolden > 0)
			tempAtt.push_back(eCharBindGolden);

//		StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,tempbuckleGolden,m_owner->GetMyself());
//
//		StatisticMgr::SetItemCostAfter(costBGoldenInfo,eCharBindGolden,tempbuckleBindGolden,m_owner->GetMyself());
	}

	m_owner->SynCharAttribute(tempAtt);

	LianLuInfo infof;
	LianLuInfo infos;
	DanLuLoader::GetInstance()->GetLianLuInfo(m_id,infof);
	DanLuLoader::GetInstance()->GetLianLuInfo(tempcurrentID,infos);

	if (infos.id > 0 && infof.quality != infos.quality)
	{
		ChatMgr::GetInstance()->Marquee(eMarqueeDanLu,m_owner->GetName(),ChatMgr::GetInstance()->GetStringByInt(m_id),ChatMgr::GetInstance()->GetStringByInt(tempcurrentID));
	}

	int tempBeforeID = m_id;

	m_id  = tempcurrentID;
	m_exp = tempcurrentExp;

	//加属性
	if (tempBeforeID != m_id)
	{
		AddDanLuFun(tempBeforeID,m_id);
	}

	m_owner->SetDataFlag(eDanLuInfo);

	GetNowDanLuInfo(*info);
	info->set_ret(tempResultCode);

	map<int,int>::iterator tempIt = tempItemList.begin();
	for(;tempIt != tempItemList.end();++tempIt)
	{
		if(tempIt->second == 0)
		{
			continue;
		}
		DanLuInfo::DanLuItemList* tempItemPtr = info->add_itemlist();

		tempItemPtr->set_itemid(tempIt->first);
		tempItemPtr->set_counts(tempIt->second);
	}

	info->set_addallexp(tempallExp);
	info->set_golden(tempbuckleGolden + tempbuckleBindGolden);
	info->set_money(tempMoney);
	info->set_type(tempType);

//	idAndCostAndGotInfo.set_afterid(m_id);

	map<eCommonRateType,DanLuExpStatistics>::iterator tempResultIt = tempResultExpList.begin();
	for (;tempResultIt != tempResultExpList.end();++tempResultIt)
	{
		DanLuInfo::DanluStatistics* tempStatistics = info->add_statisticslist();
		tempStatistics->set_multiple(tempResultIt->first);
		tempStatistics->set_counts(tempResultIt->second.m_Counts);
		tempStatistics->set_exp(tempResultIt->second.m_Exp);
	}

//	if(eAccountType_User == m_owner->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Danlu,eStaMinor_Danlu_LevelStarEvent,&idAndCostAndGotInfo);

	m_owner->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Role, eUIHLSmall_Role_DanLuCanAdvance), IsCanAdvance());

	return ;
}

ServerRet DanLuMgr::addExpResult(int addExp,int& currentID,int& currentExp,int& resultAddExp)
{
	ServerRet tempError = eRetSuccess;
	const map<int, LianLuInfo>& tempFileList = DanLuLoader::GetInstance()->getFileData();
	map<int, LianLuInfo>::const_iterator tempIt = tempFileList.find(currentID);
	while(addExp > 0)
	{
		//可以升到下一阶
		if(currentExp + addExp >= tempIt->second.needexp)
		{
			//计算剩余经验
			DWORD tempExp = tempIt->second.needexp;

			++tempIt;

			//安全判断
			if(tempIt == tempFileList.end())
			{
				tempError = eSystemError;
				break;
			}
			else if(tempIt->second.m_LevelLimit > m_owner->getAttManage()->getValue<int>(eCharLv))
			{
				tempError = eLVNotEnough;
				break;
			}
			//达到最大阶,该位置要返回
			else if (DanLuLoader::GetInstance()->IsMaxDanLuID(tempIt->first))
			{
				resultAddExp += tempExp - currentExp;
				currentID  = tempIt->first;
				currentExp = 0;
				tempError = eRetSuccess;

				break;
			}

			addExp = currentExp + addExp - tempExp;
			//本次加的经验
			resultAddExp += tempExp - currentExp;
			currentID  = tempIt->first;
			currentExp = 0;
		}
		else
		{
			currentExp += addExp;
			resultAddExp += addExp;
			tempError = eRetSuccess;
			break;
		}
	}

	return tempError;
}

//增加熔炼值
void DanLuMgr::AddDanLuExp(int exp)
{
	if (m_id <= 0)
		return ;

	if (exp < 0 || exp > 9999999)
		exp=9999999;

	int id = m_id;
	int addExp = m_exp;
	int tempGainExp = 0;

	addExpResult(exp,id,addExp,tempGainExp);

	SetDanLuInfo(&id, &addExp);

	DanLuInfo::AckPlayerDanLuInfo tMsg;
	GetDanLuInfo(tMsg);
	QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(),&tMsg,MSG_REQ_C2GT_DANLUINFO);
}

bool DanLuMgr::IsCanFastDanLu()
{
	//要有VIP等级
	return (m_owner->GetVipPart()->GetValueByRight(eVipAutoSmeltingDanLu) != VIP_NOT_HAVE_THIS_RIGHT);
}

bool DanLuMgr::IsOpenDanLuSystem()
{
	return m_owner->IsOpenSystem(eStDanLu);
}

eCommonRateType DanLuMgr::IsCrack(float bigRate,float smallRate)
{
	float tempRate = (float)rand() / RAND_MAX;

	if (bigRate >= tempRate)
		return eRateTypeBig;

	else if (smallRate >= tempRate)
		return eRateTypeSmall;

	return eRateTypeSimple;
}

int DanLuMgr::SetDanLuInfo(int *id, int *exp)
{
	if (*id <=0)
	{
		return -1;
	}

	int tempTempID = m_id;
	SetNowDanLuInfo(*id, *exp);

	//加属性
	if (tempTempID < *id)
		AddDanLuFun(tempTempID, *id);

	return 0;
}

void DanLuMgr::SetNowDanLuInfo(int id, int exp)
{
	LianLuInfo infof;
	LianLuInfo infos;
	DanLuLoader::GetInstance()->GetLianLuInfo(id,infof);
	DanLuLoader::GetInstance()->GetLianLuInfo(m_id,infos);

	if (infos.id > 0 && infof.quality != infos.quality)
	{
		ChatMgr::GetInstance()->Marquee(eMarqueeDanLu,m_owner->GetName(),ChatMgr::GetInstance()->GetStringByInt(m_id),ChatMgr::GetInstance()->GetStringByInt(id));
	}

	m_id = id;
	if (id==DanLuLoader::GetInstance()->GetLianLuMaxLv())
		exp=0;
	m_exp = exp;
	m_owner->SetDataFlag(eDanLuInfo);
}

void DanLuMgr::AddDanLuFun(int beforeID,int currentID)
{
	map<ECharAttrType,int64> tempMinusAttList;
	map<ECharAttrType,int64> tempAddAttList;

	getDataAttList(beforeID,tempMinusAttList);
	getDataAttList(currentID,tempAddAttList);

	map<ECharAttrType,int64>::iterator tempIt;
	map<ECharAttrType,int64> tempSendAtt;

	tempIt = tempMinusAttList.begin();

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttDanlu);

	for (;tempIt != tempMinusAttList.end(); ++tempIt)
	{
		m_owner->addAtt(tempIt->first,tempIt->second,false,tempValueType);
		tempSendAtt[tempIt->first] = 0;
	}

	tempIt = tempAddAttList.begin();
	for (;tempIt != tempAddAttList.end(); ++tempIt)
	{
		m_owner->addAtt(tempIt->first,tempIt->second,true,tempValueType);
		tempSendAtt[tempIt->first] = 0;
	}

	vector<int> tempMyselfVec;
	vector<int> tempBroadcast;
	tempIt = tempSendAtt.begin();
	for (;tempIt != tempSendAtt.end();++tempIt)
	{
		if(tempIt->first == eCharHPMax)
			tempBroadcast.push_back(tempIt->first);

		tempMyselfVec.push_back(tempIt->first);
	}

	if(tempBroadcast.size()>0)
	{
		m_owner->BroadcastCharAttribute(tempBroadcast);
	}

	if(tempMyselfVec.size() > 0)
	{
		calculateFight();
		tempMyselfVec.push_back(eFightPower);
		m_owner->SynCharAttribute(tempMyselfVec);
	}

//	StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(), eStatic_Danlu, eStaMinor_Danlu_LevelStar, 1);

	return ;
}

int DanLuMgr::GetDanluEquip(int& itemID, int& itemNum)
{
	if (0 >= m_itemCntr.size())
	{
		return -1;
	}

	DanLuInfo::ProduceItem item = m_itemCntr.front();

	if (item.IsInvalid())
	{
		return eItemError;
	}

	if (0 != m_owner->GetContainer(ePackageType)->IsBagEnought(item.m_itemID, item.m_itemCount, 1))
	{
		return ePackageFull;
	}
	vector<int> vecPos;
	m_owner->GetContainer(ePackageType)->AddItem(item.m_itemID, item.m_itemCount, vecPos, 1);

	m_itemCntr.pop_front();

	itemID = item.m_itemID;
	itemNum = item.m_itemCount;

	m_owner->SetDataFlag(eDanLuInfo);

	return 0;

}

bool DanLuMgr::HandleCanEnterDanLu(const EventArgs& e)
{
	return this->HandleDanLu(false);
}

bool DanLuMgr::HandleDanLu(bool isOnline)
{
	if (m_owner->getAttManage()->getValue<int>(eCharLv) >= SysSettingLoader::GetInstance()->GetLvByType(eStDanLu)
			&& m_id<=0)
	{
		SetNowDanLuInfo(INITDANLUID, 0);

		//上线不添加属性
		if (!isOnline)
			AddDanLuFun(0,INITDANLUID);
	}

	return true;
}

bool DanLuMgr::IsDanLuMaxByLv()
{
	if (DanLuLoader::GetInstance()->getMaxID() <= m_id)
	{
		return true;
	}

	LianLuInfo* info = DanLuLoader::GetInstance()->GetLianLuPtr(m_id + 1);


	if (NULL == info)
	{
		return true;
	}

	return info->m_LevelLimit > m_owner->getAttManage()->getValue<int>(eCharLv);

}

bool DanLuMgr::IsCanAdvance()
{
	return 0 == CheckAdvanceCond();
}

bool DanLuMgr::IsCanSmelting()
{
	bool bResult = false;
	int size =  m_owner->GetContainer(ePackageType)->GetSize();
	for(int pos = 0; pos < size; ++pos)
	{
		int goodID = 0;
		const Smart_Ptr<PropBase> item = m_owner->GetContainer(ePackageType)->GetItemDataPtr(pos);
		if (!item)
		{
			continue;
		}

		goodID = item->GetID();
		const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodID);
		if(ginfo == NULL)
		{
			break;
		}

		//物品  不是 武器
		if ((ginfo->type == eWeapon || ginfo->type == eArmor || ginfo->type == eJewelry) && ginfo->itemQuality < 4 && ginfo->suitID <= 0)
		{
				bResult = true;
				break;
		}
	}

	return bResult;
}

void DanLuMgr::DanluEquip_Exp(int exp)
{
	if (DanLuLoader::GetInstance()->IsMaxDanLuID(m_id))
		return ;

	int id = m_id;
	int addExp = m_exp;
	int tempGainExp = 0;

	addExpResult(exp,id,addExp,tempGainExp);

	SetDanLuInfo(&id, &addExp);

	DanLuInfo::AckPlayerDanLuInfo tMsg;
	GetDanLuInfo(tMsg);
	QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(),&tMsg,MSG_REQ_C2GT_DANLUINFO);
}

int DanLuMgr::DanluEquip_Point(const DanLuInfo::ClientDanLuEquip &req)
{
	LianEquipParm* lianEquipParam = DanLuLoader::GetInstance()->GetLianEquipParmPtr();
	if (NULL == lianEquipParam)
	{
		return eItemError;
	}

	if ((int)m_itemCntr.size() >= lianEquipParam->awardStack)
	{
		return eDanLuEquipOverAwardStack;
	}

	int oldPoint = m_point;
	m_point += CalHowPoint(req);

	int pointCeiling = lianEquipParam->pointCeil;
	if (pointCeiling <= oldPoint)
	{ // 原来的熔炼值高于熔炼值上限是错误的  规则不成立
		LOG_ERROR(FILEINFO, "DanLuEquip oldPoint greater than the pointCeiling, oldPoint number error");
	}

	int produceNum = m_point / pointCeiling; // 产品数量 不是物品数量
	int suplusPoint = m_point % pointCeiling; // 剩余熔炼值
	vector<DanLuInfo::ProduceItem> vecItem;
	DanLuInfo::ProduceItem item;
	for (int i = 0; i < produceNum; ++i)
	{
		item.Clear();
		// 随机物品
		int err = RandItem(item);
		if (0 != err)
		{
			return err;
		}
		if (item.IsInvalid() || NULL == GoodsLoader::GetInstance()->GetItemDataByID(item.m_itemID))
		{
			return eItemError;
		}

		vecItem.push_back(item);
	}




	if (0 < vecItem.size() && vecItem.size() == (uint)produceNum)
	{
		// 添加到容器
		m_itemCntr.insert(m_itemCntr.end(), vecItem.begin(), vecItem.end());

		m_point = suplusPoint;

		const int BASEADDEXPLULIAN = GameParaLoader::GetInstance()->getBaseAddExpLuLian();

		// 熔炼有增加丹炉经验
		DanluEquip_Exp(BASEADDEXPLULIAN * vecItem.size());
	}


	return 0;
}

void DanLuMgr::DanLuEquip(DanLuInfo::RetClientDanLuEquip* info,const DanLuInfo::ClientDanLuEquip& req)
{
	if (!IsOpenDanLuSystem())
	{
		info->set_ret(eLVNotEnough);
		return ;
	}

	int ret = CheckDanluItem(req);
	if (ret != 0)
	{
		info->set_ret(ret);
		return ;
	}

	ret = this->DanluEquip_Point(req);
	if (ret != 0)
	{
		info->set_ret(ret);
		return;
	}

	ReclaimDanLu(info,req);

	DeleteDanluItem(req);

	info->set_ret(0);
	info->set_point(m_point);
	list<DanLuInfo::ProduceItem>::iterator it = m_itemCntr.begin();
	for (; it != m_itemCntr.end();	++it)
	{
		DanLuInfo::SmeltItem* pItem = info->add_item();
		pItem->set_itemid(it->m_itemID);
		pItem->set_itemnum(it->m_itemCount);
	}


	//完成活跃次数
	m_owner->GetVitalityMgr()->FinshVitality(eViDanLuSmelt,req.goodid_size());
	m_owner->SetDataFlag(eDanLuInfo);

	m_owner->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Role, eUIHLSmall_Role_DanLuCanSmelting), IsCanSmelting());
}

void DanLuMgr::ReclaimDanLu(DanLuInfo::RetClientDanLuEquip *info,const DanLuInfo::ClientDanLuEquip &req)
{
	vector<PropItem> items;
	map<int,int> reclaimGoods;
	for(int i = 0; i < req.goodid_size(); ++i)
	{
		if(req.num(i) <= 0)
			return;

		int pos = req.goodid(i);
		int goodID = 0;
		const Smart_Ptr<PropBase> item = m_owner->GetContainer(ePackageType)->GetItemDataPtr(pos);
		if (!item)
		{
			return;
		}

		goodID = item->GetID();
		const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodID);
		if(ginfo == NULL)
		{
			return ;
		}

		//物品  不是 武器
		if (!(ginfo->type == eWeapon
				|| ginfo->type == eArmor
				|| ginfo->type == eJewelry))
			return;

		Smart_Ptr<EquipProp> equipItem = item;

		//强化
		const DanLuReclaim* pStrengthen = DanLuLoader::GetInstance()->GetDanLuReclaim(eReclaim_Strengthen,equipItem->GetItemQuality(),equipItem->GetUpgrade());
		if(pStrengthen)
		{
			map<int,int>::iterator itr = reclaimGoods.find(pStrengthen->nItemId);
			if(itr != reclaimGoods.end())
			{
				itr->second += pStrengthen->nItemNum;
			}
			else
			{
				reclaimGoods[pStrengthen->nItemId] = pStrengthen->nItemNum;
			}
		}

		//洗练
		const DanLuReclaim* pPurification = DanLuLoader::GetInstance()->GetDanLuReclaim(eReclaim_Purification,equipItem->GetItemQuality());
		const vector<ReGetAttr>& vTemp = equipItem->GetAttrBaptizeList();
		if(pPurification && vTemp.size() > 0)
		{
			int temp = 0;
			int num = 0;
			vector<ReGetAttr>::const_iterator vitr = vTemp.begin();
			for(; vitr != vTemp.end(); ++vitr)
			{
				temp += vitr->level;
			}

			num = temp/vTemp.size();

			if(num > 0)
			{
				map<int,int>::iterator itr = reclaimGoods.find(pPurification->nItemId);
				if(itr != reclaimGoods.end())
				{
					itr->second += num;
				}
				else
				{
					reclaimGoods[pPurification->nItemId] = num;
				}
			}
		}
	}

	if(reclaimGoods.size() > 0)
	{
		map<int,int>::iterator itr = reclaimGoods.begin();
		for(; itr != reclaimGoods.end(); ++itr)
		{
			int64 temp = itr->second;
			const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(itr->first);
			if(ginfo == NULL)
			{
				return;
			}

			while(temp > 0)
			{
				PropItem item;
				item.id = itr->first;
				if (temp > ginfo->stack)
				{
					item.num = ginfo->stack;
					temp -= ginfo->stack;
				}
				else
				{
					item.num = temp;
					temp = 0;
				}

				items.push_back(item);
			}

			DanLuInfo::DanLuItemList* tempItemPtr = info->add_itemlist();
			tempItemPtr->set_itemid(itr->first);
			tempItemPtr->set_counts(itr->second);
		}

		string warning = InternationalLoader::GetInstance()->GetInterStr(60);
		string warning1 = InternationalLoader::GetInstance()->GetInterStr(61);
		MailMessageMgr::GetInstance()->SendMail(m_owner->GetID(),items,warning1.c_str(),warning.c_str());
	}
}

int DanLuMgr::CheckDanluItem(const DanLuInfo::ClientDanLuEquip& info)
{
	//个数大过一个
//	if (info.goodid_size() > 1
//			&& m_owner->GetVipPart()->GetValueByRight(eVipOneKeyMelting) != VIP_HAVE_THIS_RIGHT)
//		return eVIPLvNotEnough;

	if(info.goodid_size() <=0)
		return eClientSendDataError;

	for(int i = 0; i < info.goodid_size(); ++i)
	{
		if(info.num(i) <= 0)
			return eClientSendDataError;

		int pos = info.goodid(i);
		int goodID = 0;
		const Smart_Ptr<PropBase> item = m_owner->GetContainer(ePackageType)->GetItemDataPtr(pos);
		if (!item)
		{
			return eNotFindItem;
		}

		if (item->GetAmount() < info.num(i))
		{
			return eItemNotEnough;
		}

		goodID = item->GetID();
		const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodID);
		if(ginfo == NULL)
		{
			return eItemError;
		}

		//物品  不是 武器
		if (!(ginfo->type == eWeapon
				|| ginfo->type == eArmor
				|| ginfo->type == eJewelry))
			return eCanNotDanluEquip;

		if(ginfo->itemQuality == 4 && ginfo->suitID > 0)
		{
			return eCanNotDanluEquip;
		}
	}

	return 0;
}

int DanLuMgr::CalHowPoint(const DanLuInfo::ClientDanLuEquip& info)
{
	int retpoint = 0;
	const GoodsInfo *	ptr =	NULL;
	Smart_Ptr<PropBase> item = NULL;
	int itemID = 0;

	for (int i=0; i<info.goodid_size(); ++i)
	{
		// goodsid 被用为pos位置
		item = m_owner->GetContainer(ePackageType)->GetItemDataPtr(info.goodid(i));
		if (false == bool(item))
			continue;

		itemID = item->GetID();
		ptr = GoodsLoader::GetInstance()->GetItemDataByID(itemID);
		if (!ptr)
			continue; /*数据有误*/

		retpoint += (DanLuLoader::GetInstance()->CalExchangePoint(ptr->useLvReq, ptr->itemQuality) * info.num(i));
	}

	return retpoint;
}

int DanLuMgr::CalHowExp(const DanLuInfo::ClientDanLuEquip &info)
{
	int ret_exp = 0;
	const GoodsInfo *	ptr =	NULL;

	for (int i=0; i<info.goodid_size(); ++i)
	{
		ptr = GoodsLoader::GetInstance()->GetItemDataByID(info.goodid(i));
		if (!ptr)
			continue; /*数据有误*/

		ret_exp += (DanLuLoader::GetInstance()->CalExchangeExp(ptr->useLvReq, ptr->itemQuality) * info.num(i));
	}

	return ret_exp;
}

int DanLuMgr::RandItem(DanLuInfo::ProduceItem& item)
{
	LianLuInfo *ptr = DanLuLoader::GetInstance()->GetLianLuPtr(m_id);

	if (NULL == ptr)
	{
		return eItemError;
	}
	ProbabilityVector<DanLuInfo::Produce> randHandle(ptr->m_produceItem);
	int resIndex = randHandle.calculation();
	if (INVALID_INDEX == resIndex)
		return eItemError;

	item = ptr->m_produceItem[resIndex].m_produceItem;

	return 0;

}

void DanLuMgr::DeleteDanluItem(const DanLuInfo::ClientDanLuEquip& info)
{
//	StatisticInfo::CostAndGotInfo costAndGotInfo;

	vector<BaseArgs> vec;
	for (int i=0; i<info.goodid_size(); ++i)
	{
		BaseArgs deleArg;

		deleArg.GoodsID = -1;
		deleArg.Num = info.num(i);
		// goodsid字段被应用成pos位置
		deleArg.Pos = info.goodid(i);

		vec.push_back(deleArg);

//		Smart_Ptr<PropBase> item = m_owner->GetContainer(ePackageType)->GetItemDataPtr(info.goodid(i));
//		if(item)
//		{
//			StatisticInfo::CostItemInfo * costInfo = costAndGotInfo.add_costlist();
//			StatisticMgr::SetItemCost(costInfo,item->GetID(),info.num(i));
//		}
	}

//	if(eAccountType_User == m_owner->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Danlu,eStaMinor_Danlu_Eat,&costAndGotInfo);

	m_owner->GetContainer(ePackageType)->DeleteGoods(vec, false);
}

int DanLuMgr::CheckAdvanceCond()
{
	int curID = m_id;
	LianLuInfo tempInfo;
	DanLuLoader::GetInstance()->GetLianLuInfo(curID,tempInfo);

	if (tempInfo.id <= 0)
	{
		return eItemError;
	}

	if(tempInfo.m_LevelLimit > m_owner->getAttManage()->getValue<int>(eCharLv))
	{
		return eLVNotEnough;
	}

	if (DanLuLoader::GetInstance()->IsMaxDanLuID(curID))
	{
		return eHighestLv;
	}

	if (m_owner->getAttManage()->getValue<int>(eCharMoney) < tempInfo.m_NeedMoney)
	{
		return eMoneyNotEnough;
	}

	int tempCount  = m_owner->GetContainer(ePackageType)->FindPropCount(tempInfo.m_ItemID);

	if (tempCount < 0)
	{
		tempCount = 0;
	}

	if (tempInfo.upNeedNum > tempCount)
	{
		return eNotEnogGoods;
	}

	return 0;
}

void DanLuMgr::getDataAttList(int id,map<ECharAttrType,int64>& attList)
{
	LianLuInfo* tempInfo = DanLuLoader::GetInstance()->GetLianLuPtr(id);
	if (tempInfo == NULL)
		return ;

	map<int,DanLuFunInfo>::iterator tempIt = tempInfo->m_danluFun.find(m_owner->getAttManage()->getValue<BYTE>(eCharProfession));
	if (tempIt == tempInfo->m_danluFun.end())
		return ;

	attList[eCharHPMax] 	  	  += tempIt->second.blood;
	attList[eCharPhyAttackMax] += tempIt->second.phyatk;
	attList[eCharPhyDefence]   += tempIt->second.phydefend;
	attList[eCharHitRate]      += tempIt->second.m_Hit;
	attList[eCharAvoidRate]    += tempIt->second.m_Dodge;
	attList[eCrackDownRate]    += tempIt->second.crackrate;
	attList[eAvoidCrackRate]   += tempIt->second.crackdefend;
}



