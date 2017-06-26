/*
 * HorseManager.cpp
 *
 *  Created on: 2014年4月30日
 *      Author: helh
 */

#include "HorseManager.h"
#include "Log/Logger.h"
#include "CharDefine.h"
#include "MessageStruct/CharLogin/GoodsMessage.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "ServerMsgDefine.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "../Map/SceneManager.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "../ServerEventDefine.h"
#include "../EventSet/EventDefine.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "../QuickMessage.h"
#include "FileLoader/GameParaLoader.h"
#include "../FileLoader/MallLoader.h"
#include "../Fighting/Fighting.h"
#include "../Attribute/AttributeManage.h"


HorseMgr::HorseMgr(Player * player):
m_owner(player)
{
	ReInit();
}

HorseMgr::~HorseMgr()
{
	this->ReleaseHorse();
}

void HorseMgr::ReInit()
{
	m_id = 0,
	m_faceID = 0;
	m_exp = 0;

	m_skin_vec.clear();

	if(!m_lvEvent.IsValid())
		m_lvEvent = RegEvent(m_owner, PLAYER_LEVEL_UP_EVENT_TYPE, this, &HorseMgr::HandlePlayerAdvanceLevel);
}

void HorseMgr::ReleaseHorse()
{
	m_skin_vec.clear();

	if (m_lvEvent.IsValid())
	{
		m_lvEvent.SetNull();
	}

	if (m_endTimer.IsValid())
	{
		m_endTimer.SetNull();
	}
}

void HorseMgr::InitHorseFromDB(const PlayerInfo::HorseInfo &info)
{
	m_owner->SetInitPartDataFlag(eHorseInfo);

	m_id = info.id();
	m_faceID = info.mid();
	m_exp = info.exp();

	for (int i = 0;i < info.skinidlist_size(); ++i)
	{
		hSkinInfo item;
		item.skinid = info.skinidlist(i).skinid();
		item.endTime = info.skinidlist(i).endtime();

		if (item.endTime < 0)
			continue;

		if (item.endTime > 0)
		{
			int64 time = item.endTime - CUtil::GetNowSecond();
			if (time > 0)
			{
				if(!m_endTimer.IsValid())
					m_endTimer = TimerInterface::GetInstance()->RegRepeatedTime(&HorseMgr::UpDateSkin, this, 0, 0, 0, 0, 1000, 10*1000);
			}
			else
			{
				//上线的时候穿戴中的皮肤到期,扣除属性
				if(m_faceID == item.skinid)
				{
					if(m_id > 0)
						setModelID(m_id);
					else
						setModelID(0);
				}

				continue;
			}
		}

		m_skin_vec[item.skinid] = item;
	}

	SetVitality();
}

void HorseMgr::SetVitality()
{
	if (HorseLoader::GetInstance()->IsMaxHorseID(m_id))
	{
		sVitality *vi = VitalityLoader::GetInstance()->GetVitalityInfoByID(eViHorseUp);
		if(vi)
			m_owner->GetVitalityMgr()->FinshVitality(eViHorseUp,vi->count);
	}
}

void HorseMgr::SetHorseToDB(PlayerInfo::HorseInfo *info)
{
	info->set_id(m_id);
	info->set_mid(m_faceID);
	if (HorseLoader::GetInstance()->IsMaxHorseID(m_id))
		m_exp = 0;
	info->set_exp(m_exp);

	map<int,hSkinInfo >::iterator it = m_skin_vec.begin();
	for (;it != m_skin_vec.end();it++)
	{
		PlayerInfo::SkinInfo * skinInfo = info->add_skinidlist();
		skinInfo->set_skinid(it->second.skinid);
		skinInfo->set_endtime(it->second.endTime);
	}
}

//上线计算属性
void HorseMgr::onlineCalculateAtt()
{
	map<ECharAttrType,int64> tempAttList;

	getDesignationHorseAttList(m_id,tempAttList);

	map<int,hSkinInfo >::iterator tempOutIt = m_skin_vec.begin();
	for (;tempOutIt != m_skin_vec.end(); ++tempOutIt)
	{
		getDesignationSkinAttList(tempOutIt->first,tempAttList);
	}

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttHorse);

	m_owner->calculateResultAddAtt(tempAttList,tempValueType);

	calculateFight();

	return ;
}

//计算一次战斗力
int HorseMgr::calculateFight()
{
	map<ECharAttrType,int64> tempAttList;

	getDesignationHorseAttList(m_id,tempAttList);

	map<int,hSkinInfo >::iterator tempOutIt = m_skin_vec.begin();
	for (;tempOutIt != m_skin_vec.end(); ++tempOutIt)
	{
		getDesignationSkinAttList(tempOutIt->first,tempAttList);
	}

	int tempResult = m_owner->getFightingManage()->CalculateFighting(tempAttList);
	return m_owner->getFightingManage()->setFighting(eFightingHorse,tempResult);
}

int HorseMgr::AttrChangeByHorse(const int &beforeID, const int  &afterID)
{
	HorseStaticInfo *before_ptr = NULL;
	HorseStaticInfo *after_ptr = NULL;
	vector<int> type;
	vector<int> typeBro;

	HorseStaticInfo tmpInfo;

	if (beforeID >0)
		before_ptr = HorseLoader::GetInstance()->GetHorseInfoByID(beforeID);
	else if(beforeID == 0)
		before_ptr = &tmpInfo;
	else
		return 0;  //数据有误

	if (afterID >0)
		after_ptr = HorseLoader::GetInstance()->GetHorseInfoByID(afterID);
	else if((afterID == 0))
		after_ptr = &tmpInfo;
	else
		return 0;  //数据有误

	if (!after_ptr || !before_ptr)
		return 0;

	HorseAttrInfo be_attInfo = before_ptr->attr[m_owner->getAttManage()->getValue<BYTE>(eCharProfession) - 1];
	HorseAttrInfo af_attInfo = after_ptr->attr[m_owner->getAttManage()->getValue<BYTE>(eCharProfession) - 1];
	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttHorse);
	int hp = af_attInfo.hp - be_attInfo.hp ;
	if (hp <0)
		m_owner->ChangeHPMax(0 - hp, false,tempValueType);
	else
		m_owner->ChangeHPMax(hp, true,tempValueType);

	typeBro.push_back(eCharHPMax);

//	int mp = af_attInfo.mp - be_attInfo.mp ;
//	if (mp <0)
//		m_owner->ChangeMPMax(0 - mp, false,tempValueType);
//	else
//		m_owner->ChangeMPMax(mp, true,tempValueType);

//	typeBro.push_back(eCharMPMax);

	int phyatt = af_attInfo.phyatt - be_attInfo.phyatt ;
	if (phyatt <0)
		m_owner->ChangePhyAttackMax(0 - phyatt, false,tempValueType);
	else
		m_owner->ChangePhyAttackMax(phyatt, true,tempValueType);

	type.push_back(eCharPhyAttackMax);

	int tempDefense = af_attInfo.m_Defense - be_attInfo.m_Defense ;
	if (tempDefense <0)
		m_owner->ChangePDefence(0 - tempDefense, false,tempValueType);
	else
		m_owner->ChangePDefence(tempDefense, true,tempValueType);

	type.push_back(eCharPhyDefence);

	int hitrate = af_attInfo.hitrate - be_attInfo.hitrate ;
	if (hitrate <0)
		m_owner->ChangeHRate(0 - hitrate, false,tempValueType);
	else
		m_owner->ChangeHRate(hitrate, true,tempValueType);

	type.push_back(eCharHitRate);

	int avoidrate = af_attInfo.avoidrate - be_attInfo.avoidrate ;
	if (avoidrate <0)
		m_owner->ChangeARate(0 - avoidrate, false,tempValueType);
	else
		m_owner->ChangeARate(avoidrate, true,tempValueType);

	type.push_back(eCharAvoidRate);

	int crackrate = af_attInfo.crackrate - be_attInfo.crackrate ;
	if (crackrate <0)
		m_owner->ChangeCRate(0 - crackrate, false,tempValueType);
	else
		m_owner->ChangeCRate(crackrate, true,tempValueType);

	type.push_back(eCrackDownRate);

	int avoidcrack = af_attInfo.avoidcrack - be_attInfo.avoidcrack ;
	if (avoidcrack <0)
		m_owner->ChangeBaserte(0 - avoidcrack, false,tempValueType);
	else
		m_owner->ChangeBaserte(avoidcrack, true,tempValueType);

	type.push_back(eAvoidCrackRate);

//	int movespeed = af_attInfo.movespeed - be_attInfo.movespeed ;
//	if (movespeed <0)
//		m_owner->ChangeMoveSpeed(0 - movespeed, false,tempValueType);
//	else
//		m_owner->ChangeMoveSpeed(movespeed, true,tempValueType);
//
//	type.push_back(eCharMoveSpeed);

	calculateFight();

	type.push_back(eFightPower);

	if (typeBro.size() > 0)
		m_owner->BroadcastCharAttribute(typeBro);

	if (type.size() > 0)
	{
		m_owner->SynCharAttribute(type);
	}
	return 0;
}

int HorseMgr::MoveSpeedChangeByHorse(const int &beforeID, const int  &afterID)
{
	HorseStaticInfo *before_ptr = NULL;
	HorseStaticInfo *after_ptr = NULL;
	vector<int> type;
	vector<int> typeBro;

	HorseStaticInfo tmpInfo;

	if (beforeID >0)
		before_ptr = HorseLoader::GetInstance()->GetHorseInfoByID(beforeID);
	else if(beforeID == 0)
		before_ptr = &tmpInfo;
	else
		return 0;  //数据有误

	if (afterID >0)
		after_ptr = HorseLoader::GetInstance()->GetHorseInfoByID(afterID);
	else if((afterID == 0))
		after_ptr = &tmpInfo;
	else
		return 0;  //数据有误

	if (!after_ptr || !before_ptr)
		return 0;

	HorseAttrInfo be_attInfo = before_ptr->attr[m_owner->getAttManage()->getValue<BYTE>(eCharProfession) - 1];
	HorseAttrInfo af_attInfo = after_ptr->attr[m_owner->getAttManage()->getValue<BYTE>(eCharProfession) - 1];
	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttHorse);

	int movespeed = af_attInfo.movespeed - be_attInfo.movespeed ;
	if (movespeed <0)
		m_owner->ChangeMoveSpeed(0 - movespeed, false,tempValueType);
	else
		m_owner->ChangeMoveSpeed(movespeed, true,tempValueType);

	type.push_back(eCharMoveSpeed);

	if (type.size() > 0)
	{
		m_owner->SynCharAttribute(type);
	}

	m_owner->SetDataFlag(eHorseInfo);

	return 0;
}

int HorseMgr::NewHandleHorse(const int& id, const bool &flag)
{
	int now_horseID = m_owner->GetHorseModelID();

	if (id >0)
	{
		//是否拥有这个坐骑
		if (!HorseLoader::GetInstance()->IsUseModel(m_id, id))
		{
			if (!IsHaveThisSkin(id))
				return eNotHaveHorse;
		}

		//是否换同一个坐骑
		if (id == now_horseID)
			return eSameHorse;


		MoveSpeedChangeByHorse(now_horseID,id);

		this->setModelID(id);

		m_owner->SetCreatureState(ERS_Horse,false);
	}
	else if (id == 0)
	{
		if(m_owner->GetHorseModelID() == id)
			return eNotOnHorse;

		MoveSpeedChangeByHorse(now_horseID,id);

		m_owner->SetHorseModelID(id);
		m_owner->SetDataFlag(eBaseInfo);

		m_owner->UnsetCreatureState(ERS_Horse,false);
	}
	else
	{
		return eClientSendDataError;
	}

	//广播给其他玩家
	ServerReturn::ServerRetInt ret;

	ret.set_ret(((int64)id << 32) | m_owner->GetKey());
	CSceneMgr::GetInstance()->SendSynchMessage(&ret, MSG_SIM_GT2C_HANDLEHORSE, m_owner->GetMapID(),
									m_owner->GetKey(), m_owner->GetXNewPos(), m_owner->GetYNewPos());

	return 0;
}

int HorseMgr::HandleHorse(int id)
{
	return NewHandleHorse(id, false);
}

void HorseMgr::EatHorse(HorseInfo::HorseAdvance& requestMsg, HorseInfo::AddExpReturn *ret)
{
	bool tempIsBuy = requestMsg.isautobuy();

	int tempcurrentId    = m_id;
	int tempcurrentExp   = m_exp;
	int tempgolden	     = 0;
	int exp 			 = 0;
	int tempMoney 		 = 0;
	bool tempFirst		 = true;

	map<eCommonRateType,HorseExpStatistics> tempresultList;
	map<int,int> tempItemList;

	ServerRet tempResult   = eRetSuccess;
	ServerRet tempOneError = eRetSuccess;

	if(requestMsg.type() == eHorseTypeNormal)
	{
		tempResult = oneSimulationData(tempcurrentId,tempIsBuy,tempcurrentExp,exp,tempresultList,tempItemList,tempgolden,tempMoney);
	}
	else
	{
		if (!IsCanAutoAdvance())
		{ // VIP等级不足
			tempResult = eVIPLvNotEnough;
		}
		else
		{
			int tempMaxID = HorseLoader::GetInstance()->getMaxID();
			int tempEatID = requestMsg.id();
			while(tempEatID <= tempMaxID && tempcurrentId < tempEatID)
			{
				tempOneError = oneSimulationData(tempcurrentId,tempIsBuy,tempcurrentExp,exp,tempresultList,tempItemList,tempgolden,tempMoney);
				if(tempOneError != eRetSuccess)
				{
					if(tempFirst)
					{
						tempResult = tempOneError;
					}

					break;
				}

				tempFirst = false;
			}
		}
	}

	ret->set_ret(tempResult);
	ret->set_stoperror(tempOneError);

	if(tempResult != eRetSuccess)
	{
		return ;
	}

//	StatisticInfo::IDAndCostAndGotInfo idAndCostAndGotInfo;
//	idAndCostAndGotInfo.set_beforeid(m_id);
//	StatisticInfo::CostAndGotInfo * costAndGotInfo = idAndCostAndGotInfo.mutable_costandgotlist();

	vector<int> vec;
	//扣除元宝
	int tempbuckleGolden 	 = 0;
	int tempbuckleBindGolden = 0;
	//扣除金钱
	if (tempgolden >0)
	{
//		StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo->add_costlist();
//		StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,m_owner->GetMyself());

//		StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo->add_costlist();
//		StatisticMgr::SetItemCostBefore(costBGoldenInfo,eCharBindGolden,m_owner->GetMyself());

		m_owner->DecreaseGoldenCost(tempgolden,tempbuckleGolden,tempbuckleBindGolden);

		if(tempbuckleGolden > 0)
			vec.push_back(eCharGolden);

		if(tempbuckleBindGolden > 0)
			vec.push_back(eCharBindGolden);

//		StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,tempbuckleGolden,m_owner->GetMyself());
//		StatisticMgr::SetItemCostAfter(costBGoldenInfo,eCharBindGolden,tempbuckleBindGolden,m_owner->GetMyself());
	}

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

	if (tempMoney > 0)
	{
//		StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo->add_costlist();
//		StatisticMgr::SetItemCostBefore(costItemInfo,eCharMoney,tempMoney,m_owner->GetMyself());

		m_owner->MoneyChange(tempMoney,false);
		vec.push_back(eCharMoney);

//		StatisticMgr::SetItemCostAfter(costItemInfo,eCharMoney,m_owner->GetMyself());
	}

	m_owner->SynCharAttribute(vec);

	int tempBeforeID = m_id;
	m_id  = tempcurrentId;
	m_exp = tempcurrentExp;

	//改变属性
	if (m_id != tempBeforeID)
	{
		const HorseStaticInfo* tempCurrentData = HorseLoader::GetInstance()->GetHorseInfoByID(tempBeforeID);
		const HorseStaticInfo* tempLastData    = HorseLoader::GetInstance()->GetHorseInfoByID(m_id);
		if(tempCurrentData != NULL && tempLastData != NULL && tempCurrentData->advancelv != tempLastData->advancelv)
		{
			ChatMgr::GetInstance()->Marquee(eMarqueeHorse,m_owner->GetName(),
					ChatMgr::GetInstance()->GetStringByInt(tempCurrentData->id),ChatMgr::GetInstance()->GetStringByInt(tempLastData->id));
			m_owner->SynchCharAttributeToWS(eHorseQuality,tempLastData->advancelv);

//			idAndCostAndGotInfo.set_afterid(m_id);

//			if(eAccountType_User == m_owner->GetAccountType())
//				StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Horse,eStaMinor_Horse_LevelStarEvent,&idAndCostAndGotInfo);
		}
		this->AddAttrWhenChangeHorse(tempBeforeID,m_id);
	}

	ret->set_allexp(exp);
	map<eCommonRateType,HorseExpStatistics>::iterator tempIt = tempresultList.begin();
	for(;tempIt != tempresultList.end();++tempIt)
	{
		HorseInfo::HorseCritStatistics* tempStatistics = ret->add_statisticslist();
		tempStatistics->set_type(tempIt->first);
		tempStatistics->set_counts(tempIt->second.m_Counts);
		tempStatistics->set_exp(tempIt->second.m_Exp);
	}

	map<int,int>::iterator tempItemIt = tempItemList.begin();
	for(;tempItemIt != tempItemList.end();++tempItemIt)
	{
		if(tempItemIt->second == 0)
		{
			continue;
		}

		HorseInfo::HorseItemList* tempItemPtr = ret->add_itemlist();

		tempItemPtr->set_itemid(tempItemIt->first);
		tempItemPtr->set_counts(tempItemIt->second);
	}

	ret->set_exp(m_exp);
	ret->set_id(m_id);
	ret->set_type(requestMsg.type());
	ret->set_golden(tempbuckleGolden + tempbuckleBindGolden);
    ret->set_money(tempMoney);

	 if (!IsCanAdvance())
	 {
		 m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Role, eUIHLSmall_Role_HorseCanUpgrade), 1, false);
	 }
	m_owner->SetDataFlag(eHorseInfo);
}

int HorseMgr::getEatExpByRate(const HorseStaticInfo& info,eCommonRateType& type)
{
	float tempRate = (float)rand() / RAND_MAX;

	int tempExp = info.m_BaseExp;
	if (info.m_MaxCritRate >= tempRate)
	{
		tempExp = (int)(tempExp * info.m_MaxCrit);
		type = eRateTypeBig;
	}
	else if (info.m_MinCritRate >= tempRate)
	{
		tempExp = (int)(tempExp * info.m_MinCrit);
		type = eRateTypeSmall;
	}
	else
	{
		type = eRateTypeSimple;
	}

	return tempExp;
}

ServerRet HorseMgr::oneSimulationData(int& currentId,bool isBuy,int& currentExp,int& allExp,
		map<eCommonRateType,HorseExpStatistics>& resultList,map<int,int>& itemList,int& golden,int& money)
{
	const map<int, HorseStaticInfo>& tempDataList    = HorseLoader::GetInstance()->getFileData();
	map<int, HorseStaticInfo>::const_iterator tempIt = tempDataList.find(currentId);
	if(tempIt == tempDataList.end())
	{
		return eItemError;
	}

	if(HorseLoader::GetInstance()->IsMaxHorseID(currentId))
	{
		return eAlreadyMaxHorse;
	}

	if (m_owner->getAttManage()->getValue<int>(eCharMoney) < money + tempIt->second.m_NeedMoney)
	{
		return eMoneyNotEnough;
	}

	int tempItemCounts = m_owner->GetContainer(ePackageType)->GetGoodsNum(tempIt->second.m_ItemID);
	tempItemCounts    -= itemList[tempIt->second.m_ItemID];

	if (tempItemCounts < 0)
	{
		tempItemCounts = 0;
	}

	if (tempItemCounts < tempIt->second.needNum)
	{
		if(!isBuy)
		{
			return eItemNotEnough;
		}

		int tempGolden = MallLoader::GetInstance()->GetDanLuStoneCost(tempIt->second.m_ItemID);
		tempGolden    *= tempIt->second.needNum - tempItemCounts;

		Smart_Ptr<PlayerVipPart> tempVip = m_owner->GetVipPart();
		if (tempVip)
		{
			tempGolden = PlayerVipPart::CalBuyCostGolden(tempVip, tempGolden);
		}

		itemList[tempIt->second.m_ItemID] += tempItemCounts;

		if (!m_owner->IsEnoughGolden(tempGolden + golden))
		{
			return eNotEnoughGold;
		}

		golden += tempGolden;
	}
	else
	{
		itemList[tempIt->second.m_ItemID] += tempIt->second.needNum;
	}

	money += tempIt->second.m_NeedMoney;

	eCommonRateType tempType = eRateTypeNUll;
	int tempGainExp = 0;
	int tempExp = getEatExpByRate(tempIt->second,tempType);
	ServerRet tempError = eRetSuccess;

	while(tempExp > 0 && tempIt->second.exp > 0)
	{
		if(currentExp + tempExp >= tempIt->second.exp)
		{
			int tempThisExp = tempIt->second.exp;

			++tempIt;

			if(tempIt == tempDataList.end())
			{
				tempError = eSystemError;
				break;
			}
			else if(tempIt->second.m_LevelLimit > m_owner->getAttManage()->getValue<int>(eCharLv))
			{
				tempError = eLVNotEnough;
				break;
			}
			else if(HorseLoader::GetInstance()->IsMaxHorseID(tempIt->second.id))
			{
				currentId   = tempIt->second.id;
				allExp	   += tempThisExp - currentExp;
				tempGainExp += tempThisExp - currentExp;
				currentExp  = 0;
				tempError = eRetSuccess;
				break;
			}

			currentId  = tempIt->second.id;
			tempExp    = currentExp + tempExp - tempThisExp;
			tempGainExp += tempThisExp - currentExp;
			allExp    += tempThisExp - currentExp;
			currentExp = 0;
		}
		else
		{
			currentExp += tempExp;
			allExp     += tempExp;
			tempGainExp += tempExp;
			tempError = eRetSuccess;
			break;
		}
	}

	m_owner->GetVitalityMgr()->FinshVitality(eViHorseUp);

	resultList[tempType].m_Counts += 1;
	resultList[tempType].m_Exp += tempGainExp;

	return tempError;
}

void HorseMgr::AddAttrWhenChangeHorse(const int &before_ID, const int &after_ID)
{
	AttrChangeByHorse(before_ID, after_ID);

	if (m_owner->GetHorseModelID() > 0)
	{
		//如果穿戴了购买的皮肤则坐骑升阶后不改变皮肤
		if (m_owner->GetHorseModelID() != after_ID
				&& IsHaveThisSkin(m_faceID) == false)
		{
			setModelID(after_ID);

			//广播给其他玩家
			ServerReturn::ServerRetInt ret;
			ret.set_ret(((int64)m_faceID << 32) | m_owner->GetKey());
			CSceneMgr::GetInstance()->SendSynchMessage(&ret, MSG_SIM_GT2C_HANDLEHORSE, m_owner->GetMapID(), m_owner->GetKey(), m_owner->GetXNewPos(), m_owner->GetYNewPos());
		}
	}

//	this->StatisticHorseData(before_ID, m_id);
}

void HorseMgr::AddHorse()
{
	HorseStaticInfo * info = NULL;

	PlayerInfo::HorseInfo toClient;

	info = HorseLoader::GetInstance()->GetFirstHorse();
	if (info == 0)
	{
		return;
	}

	m_id = info->id;
	m_faceID = info->id;
	m_exp = 0;
	m_owner->SetDataFlag(eHorseInfo);

	AttrChangeByHorse(0, info->id);

	toClient.set_id(m_id);
	toClient.set_mid(m_faceID);
	toClient.set_exp(m_exp);

	QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(),m_owner->GetChannelID(),&toClient,MSG_SIM_GT2C_GETHORSE);
}

bool HorseMgr::HandlePlayerAdvanceLevel(const EventArgs & e)
{
	if (m_owner->IsOpenSystem(eStHorse) && m_id <= 0)
	{
		AddHorse();
	}

	return true;
}

//void HorseMgr::StatisticHorseData(int before_id, int after_id)
//{
//	//统计 坐骑升星次数
////	StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(), eStatic_Horse, eStaMinor_Horse_LevelStar, 1);
//
//	int before_advnceID =	HorseLoader::GetInstance()->GetHorseAdvanceLvByID(before_id);
//	int now_advnceID = HorseLoader::GetInstance()->GetHorseAdvanceLvByID(after_id);
//	if (now_advnceID >before_advnceID)
//	{
//		StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(), eStatic_Horse, eStaMinor_Horse_LevelUp, 1);
//	}
//}

void HorseMgr::setModelID(const int& model_id)
{
	m_faceID = model_id;
	m_owner->SetHorseModelID(model_id);

	m_owner->SetDataFlag(eHorseInfo);
	m_owner->SetDataFlag(eBaseInfo);
}

int HorseMgr::BuyHorseSkin(const int& skinId)
{
	if (m_id <= 0)
		return eConNotMatch;

	const HorseSkinData *tempSkinData = HorseLoader::GetInstance()->GetHorseSkinInfo(skinId);
	if (tempSkinData == NULL)
		return eClientSendDataError;

	if (!m_owner->IsEnoughGolden(tempSkinData->m_BuyGolden))
		return eNotEnoughGold;

	int ret = AddHorseSkin(skinId);
	if (ret != 0)
		return ret;

//	StatisticInfo::CostAndGotInfo costAndGotInfo;
//	StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
//	StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,tempSkinData->m_BuyGolden,m_owner->GetMyself());
//
//	StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo.add_costlist();
//	StatisticMgr::SetItemCostBefore(costBGoldenInfo,eCharBindGolden,m_owner->GetMyself());

	int costGolden = 0;
	int costbGolden = 0;
	vector<int> vec;

	m_owner->DecreaseGoldenCost(tempSkinData->m_BuyGolden, costGolden, costbGolden);
	if(costGolden > 0)
		vec.push_back(eCharGolden);
	if(costbGolden > 0)
		vec.push_back(eCharBindGolden);

//	StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,m_owner->GetMyself());
//
//	StatisticMgr::SetItemCostAfter(costBGoldenInfo,eCharBindGolden,costbGolden,m_owner->GetMyself());
//
//	StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo.add_gotlist();
//	StatisticMgr::AddItemGotInfo(gotItemInfo,skinId,1);

	m_owner->SynCharAttribute(vec);

	m_owner->SetDataFlag(eHorseInfo);
	m_owner->SetDataFlag(eBaseInfo);

//	if(eAccountType_User == m_owner->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Horse,eStaMinor_Horse_BugHorseSkinEvent,&costAndGotInfo);
//	// 统计元宝花费
//	StatisticMgr::GetInstance()->StatisticPlayerGetItems(m_owner->GetMyself(),eStatic_AboutGolden,skinId,1,tempSkinData->m_BuyGolden);

	return 0;
}

//使用道具获得皮肤
int HorseMgr::GetHorseSkin(const int& skinId , int64 time,int num)
{
	int ret = AddHorseSkin(skinId,time,num);
	if (ret != 0)
		return ret;

	m_owner->SetDataFlag(eHorseInfo);

	PlayerInfo::SkinInfo toPlayer;

	hSkinInfo skinInfo;
	GetHorseSkinInfo(skinId,skinInfo);

	toPlayer.set_skinid(skinInfo.skinid);
	toPlayer.set_endtime(skinInfo.endTime);

	QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(), &toPlayer, MSG_SIM_GM2C_GETSKIN);

//	m_owner->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Horse, eUIHLSmall_Horse_GetNewSkin), 1);

	return 0;
}

int HorseMgr::AddHorseSkin(const int& skinId ,int64 time,int num)
{
	const HorseSkinData *tempSkinData = HorseLoader::GetInstance()->GetHorseSkinInfo(skinId);
	if (tempSkinData == NULL)
		return eClientSendDataError;

	int64 nowSecond = CUtil::GetNowSecond();

	hSkinInfo item;
	item.skinid = skinId;

	if (0 == time)
	{
		//永久皮肤
		if (0 == tempSkinData->m_LastTime)
			item.endTime = 0;
		else
			item.endTime = nowSecond + tempSkinData->m_LastTime * num;
	}
	else
		item.endTime = nowSecond + time * num;

	bool tempIsAdd = false;
	std::map<int,hSkinInfo>::iterator it = m_skin_vec.find(skinId);
	if (it!=m_skin_vec.end())
	{
		if (item.endTime == 0)
		{
			//两个均为永久皮肤
			if (it->second.endTime == 0)
				return eBuySameHorseSkin;
			else
				it->second.endTime = 0;
		}
		else
		{
			if(it->second.endTime == 0)
				return eHaveSkinForever;
			else
				//时间累积
				it->second.endTime += (item.endTime - nowSecond);
		}
	}
	else
	{
		tempIsAdd = true;
		m_skin_vec[skinId] = item;
	}

	if (it->second.endTime > 0 && !m_endTimer.IsValid())
		m_endTimer = TimerInterface::GetInstance()->RegRepeatedTime(&HorseMgr::UpDateSkin, this, 0, 0, 0, 0, 1000, 10*1000);

	if (tempIsAdd)
	{
		std::vector<int> tempAttList;
		addSkinAtt(skinId,true,tempAttList);

		if(tempAttList.size() > 0)
			m_owner->SynCharAttribute(tempAttList);
	}

	return 0;
}

bool HorseMgr::DeleteHorseSkin(const int& skinId)
{
	map<int,hSkinInfo>::iterator it = m_skin_vec.find(skinId);
	if (it != m_skin_vec.end())
	{
		//正在穿的皮肤被删除需要同步属性
		if (m_faceID == it->first)
		{
			setModelID(m_id);

			//广播给其他玩家
			ServerReturn::ServerRetInt ret;

			ret.set_ret(((int64)m_id << 32) | m_owner->GetKey());
			CSceneMgr::GetInstance()->SendSynchMessage(&ret, MSG_SIM_GT2C_HANDLEHORSE, m_owner->GetMapID(),
									m_owner->GetKey(), m_owner->GetXNewPos(), m_owner->GetYNewPos(),true);
		}

		std::vector<int> tempAttList;
		addSkinAtt(m_faceID,false,tempAttList);
		if (tempAttList.size() > 0)
			m_owner->SynCharAttribute(tempAttList);

		ServerReturn::ServerRetInt toPlayer;
		toPlayer.set_ret(it->first);
		QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(), &toPlayer,MSG_SIM_GM2C_SKINEND);

		m_skin_vec.erase(it);
		m_owner->SetDataFlag(eHorseInfo);
		m_owner->SetDataFlag(eBaseInfo);
		return true;
	}

	return false;
}

bool HorseMgr::IsHaveThisSkin(const int& skinId)
{
	map<int,hSkinInfo>::iterator it = m_skin_vec.begin();
	for (; it != m_skin_vec.end(); it++)
	{
		if(it->second.skinid == skinId)
			return true;
	}

	return false;
}

int HorseMgr::GetHourseQuality()
{
	return HorseLoader::GetInstance()->GetHorseAdvanceLvByID(m_id);
}

void HorseMgr::GetHorseSkinInfo(const int& skinId,hSkinInfo & skinInfo)
{
	map<int,hSkinInfo >::iterator it = m_skin_vec.find(skinId);
	if(it != m_skin_vec.end())
	{
		skinInfo = it->second;
	}
}

void HorseMgr::UpDateSkin(void * arg)
{
	std::vector<int> tempDeleteId;

	int64 nowTime = CUtil::GetNowSecond();

	map<int,hSkinInfo>::iterator it = m_skin_vec.begin();
	for (;it != m_skin_vec.end();++it)
	{
		//永久皮肤
		if(it->second.endTime == 0)
			continue;
		//没到限时时间
		if(nowTime < it->second.endTime)
			continue;

		tempDeleteId.push_back(it->first);
	}

	for (size_t i = 0;i < tempDeleteId.size(); ++i)
	{
		DeleteHorseSkin(tempDeleteId[i]);
	}
}

bool HorseMgr::IsHorseMaxByLv()
{
	if (HorseLoader::GetInstance()->getMaxID() <= m_id)
	{
		return true;
	}

	const map<int, HorseStaticInfo>& tempDataList = HorseLoader::GetInstance()->getFileData();
	map<int, HorseStaticInfo>::const_iterator tempIt = tempDataList.find(m_id + 1);
	if(tempIt == tempDataList.end())
	{
		return true;
	}

	return tempIt->second.m_LevelLimit > m_owner->getAttManage()->getValue<int>(eCharLv);
}

bool HorseMgr::IsCanAdvance()
{
	return 0 == CheckAdvanceCond();
}

int HorseMgr::CheckAdvanceCond()
{
	int curID = m_id;

	if (!m_owner->IsOpenSystem(eStHorse))
	{
		return eNotOpen;
	}

	const map<int, HorseStaticInfo>& tempDataList    = HorseLoader::GetInstance()->getFileData();
	map<int, HorseStaticInfo>::const_iterator it = tempDataList.find(curID);
	if(it == tempDataList.end())
	{
		return eItemError;
	}

	if(HorseLoader::GetInstance()->IsMaxHorseID(curID))
	{
		return eAlreadyMaxHorse;
	}

	if (m_owner->getAttManage()->getValue<int>(eCharMoney) < it->second.m_NeedMoney)
	{
		return eMoneyNotEnough;
	}

	int itemCount = m_owner->GetContainer(ePackageType)->GetGoodsNum(it->second.m_ItemID);
	if (itemCount < 0)
	{
		itemCount = 0;
	}

	if (itemCount < it->second.needNum)
	{
		return eItemNotEnough;
	}

	return 0;
}



void HorseMgr::addSkinAtt(int id,bool isAdd,vector<int>& attList)
{
	const HorseSkinData* tempData = HorseLoader::GetInstance()->GetHorseSkinInfo(id);
	if(tempData == NULL)
		return ;

	//用于多次调用，合并属性，防止多次发送同一个属性
	map<int,BYTE> tempAttList;
	for(size_t i = 0;i < attList.size();++i)
	{
		tempAttList[attList[i]] = 0;
	}

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttHorse);

	map<ECharAttrType,int64>::const_iterator tempIt = tempData->m_AttList.begin();
	for (;tempIt != tempData->m_AttList.end(); ++tempIt)
	{
		if(tempIt->second == 0)
			continue;

		m_owner->addAtt(tempIt->first,tempIt->second,isAdd,tempValueType);
		tempAttList[tempIt->first] = 0;
	}

	attList.clear();

	map<int,BYTE>::iterator tempMapIt = tempAttList.begin();
	for (;tempMapIt != tempAttList.end();++tempMapIt)
	{
		attList.push_back(tempMapIt->first);
	}

	if (attList.size() > 0)
	{
		calculateFight();
		attList.push_back(eFightPower);
	}

	return;
}

//获得指定ID坐骑增加的属性
void HorseMgr::getDesignationHorseAttList(int id,map<ECharAttrType,int64>& attList)
{
	HorseStaticInfo* tempData = HorseLoader::GetInstance()->GetHorseInfoByID(id);
	if (tempData == NULL)
		return;

	HorseAttrInfo tempCurrentInfo = tempData->attr[m_owner->getAttManage()->getValue<BYTE>(eCharProfession) - 1];

	attList[eCharHPMax] 	   += tempCurrentInfo.hp;
//	attList[eCharMPMax] 	   += tempCurrentInfo.mp;
	attList[eCharPhyAttackMax] += tempCurrentInfo.phyatt;
	attList[eCharPhyDefence]   += tempCurrentInfo.m_Defense;
	attList[eCharHitRate] 	   += tempCurrentInfo.hitrate;
	attList[eCharAvoidRate]    += tempCurrentInfo.avoidrate;
	attList[eCrackDownRate]    += tempCurrentInfo.crackrate;
	attList[eAvoidCrackRate]   += tempCurrentInfo.avoidcrack;
	attList[eCharMoveSpeed]    += tempCurrentInfo.movespeed;

	return;
}

//获得指定ID皮肤增加的属性
void HorseMgr::getDesignationSkinAttList(int id,map<ECharAttrType,int64>& attList)
{
	const HorseSkinData * tempData = HorseLoader::GetInstance()->GetHorseSkinInfo(id);
	if (tempData == NULL)
		return ;

	map<ECharAttrType,int64>::const_iterator tempInIt = tempData->m_AttList.begin();
	for (;tempInIt != tempData->m_AttList.end();++tempInIt)
	{
		attList[tempInIt->first] += tempInIt->second;
	}
}

// 是否达到自动进阶条件
bool HorseMgr::IsCanAutoAdvance()
{
	if (m_owner->GetVipPart())
	{
		return VIP_HAVE_THIS_RIGHT == m_owner->GetVipPart()->GetValueByRight(eVipHorseAutoAdvance);
	}

	return false;
}










