/*
 * ExtraAtributeMgr.cpp
 *
 *  Created on: 2015年4月16日
 *      Author: root
 */
#include "ExtraAtributeMgr.h"
#include "ExtraAtributeLoader.h"
#include "Log/Logger.h"
#include "../Container/ContainerBase.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "../ServerEventDefine.h"
#include "../Container/ArticleEquipMgr.h"
#include <map>
#include "../Fighting/Fighting.h"
#include "../Attribute/AttributeManage.h"

ExtraAtributeMgr::ExtraAtributeMgr(Player* player):m_owner(player)
{
	ReInit();
}

ExtraAtributeMgr::~ExtraAtributeMgr()
{
	Release();
}

void ExtraAtributeMgr::ReInit()
{
	m_extraStrengthLv = 0;
	m_extraRefineLv = 0;
	m_extraEquipLv = 0;
	m_extraStoneLv = 0;

	if(!m_StrengthEquipEvent.IsValid())
		m_StrengthEquipEvent = RegEvent(m_owner, PLAYER_SMELT_STRONG, this, &ExtraAtributeMgr::HandlePlayerStrengthEquip);

	if(!m_smeltEvent.IsValid())
		m_smeltEvent = RegEvent(m_owner, PLAYER_SMELT_SMELT, this, &ExtraAtributeMgr::HandlePlayerSmelt);

	if(!m_wearEquipEvent.IsValid())
		m_wearEquipEvent = RegEvent(m_owner, PLAYER_WEAR_EQUIP, this, &ExtraAtributeMgr::HandlePlayerWearEquip);

	if(!m_insertStoneEvent.IsValid())
		m_insertStoneEvent = RegEvent(m_owner, PLAYER_SMELT_INSERT, this, &ExtraAtributeMgr::HandlePlayerInsertStone);

	if(!m_takeOffStoneEvent.IsValid())
		m_takeOffStoneEvent = RegEvent(m_owner, PLAYER_TAKE_OFF_STONE, this, &ExtraAtributeMgr::HandlePlayerTakeOffStone);

	if(!m_equipUpgrade.IsValid())
		m_equipUpgrade = RegEvent(m_owner, PLAYER_EQUIP_UPGRADE, this, &ExtraAtributeMgr::HandlePlayerEquipUpgrade);

	if(!m_takeOffEquip.IsValid())
		m_takeOffEquip = RegEvent(m_owner, PLAYER_TAKE_OFF_EQUIP, this, &ExtraAtributeMgr::HandlePlayerTakeOffEquip);
}

void ExtraAtributeMgr::Release()
{
	if(m_StrengthEquipEvent.IsValid())
	{
		m_StrengthEquipEvent.SetNull();
	}
	if(m_smeltEvent.IsValid())
	{
		m_smeltEvent.SetNull();
	}
	if(m_wearEquipEvent.IsValid())
	{
		m_wearEquipEvent.SetNull();
	}
	if(m_insertStoneEvent.IsValid())
	{
		m_insertStoneEvent.SetNull();
	}
	if(m_takeOffStoneEvent.IsValid())
	{
		m_takeOffStoneEvent.SetNull();
	}
	if(m_equipUpgrade.IsValid())
	{
		m_equipUpgrade.SetNull();
	}
	if(m_takeOffEquip.IsValid())
	{
		m_takeOffEquip.SetNull();
	}
}

void ExtraAtributeMgr::InitExtraAtribute()
{
	m_extraStrengthLv = CalExtraAtributeLv(ExtraAtributeType_Strength);
	m_extraRefineLv = CalExtraAtributeLv(ExtraAtributeType_Refine);
	m_extraEquipLv = CalExtraAtributeLv(ExtraAtributeType_EquipLv);
	m_extraStoneLv = CalExtraAtributeLv(ExtraAtributeType_Stone);
}

//上线计算属性
void ExtraAtributeMgr::onlineCalculateAtt()
{
	map<ECharAttrType,int64> tempAttList;

	getAttByTypeAndNumber(ExtraAtributeType_Strength,CalExtraAtributeLv(ExtraAtributeType_Strength),tempAttList);
	getAttByTypeAndNumber(ExtraAtributeType_Refine,  CalExtraAtributeLv(ExtraAtributeType_Refine),  tempAttList);
	getAttByTypeAndNumber(ExtraAtributeType_EquipLv, CalExtraAtributeLv(ExtraAtributeType_EquipLv), tempAttList);
	getAttByTypeAndNumber(ExtraAtributeType_Stone,   CalExtraAtributeLv(ExtraAtributeType_Stone),   tempAttList);

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttExtra);

	m_owner->calculateResultAddAtt(tempAttList,tempValueType);

	calculateFight();

	return ;
}

//计算一次战斗力
int ExtraAtributeMgr::calculateFight()
{
	map<ECharAttrType,int64> tempAttList;

	getAttByTypeAndNumber(ExtraAtributeType_Strength,CalExtraAtributeLv(ExtraAtributeType_Strength),tempAttList);
	getAttByTypeAndNumber(ExtraAtributeType_Refine,  CalExtraAtributeLv(ExtraAtributeType_Refine),  tempAttList);
	getAttByTypeAndNumber(ExtraAtributeType_EquipLv, CalExtraAtributeLv(ExtraAtributeType_EquipLv), tempAttList);
	getAttByTypeAndNumber(ExtraAtributeType_Stone,   CalExtraAtributeLv(ExtraAtributeType_Stone),   tempAttList);

	int tempResult = m_owner->getFightingManage()->CalculateFighting(tempAttList);
	return m_owner->getFightingManage()->setFighting(eFightingExtra,tempResult);
}

void ExtraAtributeMgr::ChangeExtraAtribute(DWORD type, DWORD extraLv,bool isAdd)
{
	ExtraAtribute info;
	ExtraAtributeLoader::GetInstance()->GetExtraAtribute(type,extraLv,info);
	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttExtra);
	if(m_owner && info.extraAtriLv != 0)
	{
		std::vector<int> vec;

		if(info.basehp > 0)
		{
			m_owner->ChangeHPMax(info.basehp,isAdd,tempValueType);
			vec.push_back(eCharHPMax);
		}

		if(info.baphyackmax > 0)
		{
			m_owner->ChangePhyAttackMax(info.baphyackmax,isAdd,tempValueType);
			vec.push_back(eCharPhyAttackMax);
		}
		if(info.baphydefmax > 0)
		{
			m_owner->ChangePDefence(info.baphydefmax,isAdd,tempValueType);
			vec.push_back(eCharPhyDefence);
		}

		if(info.m_Dodge > 0)
		{
			m_owner->ChangeARate(info.m_Dodge,isAdd,tempValueType);
			vec.push_back(eCharAvoidRate);
		}

		if(info.m_Hit > 0)
		{
			m_owner->ChangeHRate(info.m_Hit,isAdd,tempValueType);
			vec.push_back(eCharHitRate);
		}

		if(info.m_Crit > 0)
		{
			m_owner->ChangeCRate(info.m_Crit,isAdd,tempValueType);
			vec.push_back(eCrackDownRate);
		}

		if(info.m_ResistCrit > 0)
		{
			m_owner->ChangeBaserte(info.m_ResistCrit,isAdd,tempValueType);
			vec.push_back(eAvoidCrackRate);
		}

		if (0 < calculateFight())
		{
			vec.push_back(eFightPower);
		}

		//回复属性没有
		m_owner->BroadcastCharAttribute(vec);
		m_owner->SynCharAttribute(vec);
	}
}

DWORD ExtraAtributeMgr::CalExtraAtributeLv(DWORD type)
{
	int extraAtributeLv = 0;
	std::map<DWORD,ExtraAtribute> infolist;
	ExtraAtributeLoader::GetInstance()->GetExtraAtributeList(type,infolist);
	std::map<DWORD,ExtraAtribute>::iterator it = infolist.begin();
	for(;it != infolist.end();++it)
	{
		if(IsExtraAtribute(it->second))
			extraAtributeLv = it->first;
		else
			break;
	}

	return extraAtributeLv;
}

bool ExtraAtributeMgr::IsExtraAtribute(ExtraAtribute info)
{
	switch(info.type)
	{
		case ExtraAtributeType_Strength:
			return IsExtraAtributeStrength(info);
		case ExtraAtributeType_Refine:
			return IsExtraAtributeRefine(info);
		case ExtraAtributeType_EquipLv:
			return IsExtraAtributeEquipLv(info);
		case ExtraAtributeType_Stone:
			return IsExtraAtributeStone(info);
		default:
			break;
	}

	return false;
}

bool ExtraAtributeMgr::IsExtraAtributeStrength(ExtraAtribute info)
{
	if(info.type != ExtraAtributeType_Strength)
		return false;

	Smart_Ptr<ArticleEquipMgr> equipMgr= m_owner->GetContainer(eEquipQuickType);
	if(bool(equipMgr) == false)
		return false;

	DWORD equipStrengthCount = equipMgr->GetEquipCountStrength();
	if(equipStrengthCount < info.num)
		return false;

	return true;
}

bool ExtraAtributeMgr::IsExtraAtributeRefine(ExtraAtribute info)
{
	if(info.type != ExtraAtributeType_Refine)
		return false;

	Smart_Ptr<ArticleEquipMgr> equipMgr= m_owner->GetContainer(eEquipQuickType);
	if(bool(equipMgr) == false)
		return false;

	DWORD refineCount = equipMgr->GetRefineCount();
	if(refineCount < info.num)
		return false;

	return true;
}

bool ExtraAtributeMgr::IsExtraAtributeEquipLv(ExtraAtribute info)
{
	if(info.type != ExtraAtributeType_EquipLv)
		return false;

	Smart_Ptr<ArticleEquipMgr> equipMgr= m_owner->GetContainer(eEquipQuickType);
	if(bool(equipMgr) == false)
		return false;

//	DWORD equipCount = equipMgr->GetEquipCountWeared();
//	if(equipCount < info.num)
//		return false;

	DWORD equipLvCount = equipMgr->GetEquipLvCount();
	if(equipLvCount < info.num)
		return false;

	return true;
}

bool ExtraAtributeMgr::IsExtraAtributeStone(ExtraAtribute info)
{
	if(info.type != ExtraAtributeType_Stone)
		return false;

	Smart_Ptr<ArticleEquipMgr> equipMgr= m_owner->GetContainer(eEquipQuickType);
	if(bool(equipMgr) == false)
		return false;

	DWORD stoneCount = equipMgr->GetStoneCount();
	if(stoneCount < info.num)
		return false;

	return true;
}

bool ExtraAtributeMgr::HandlePlayerStrengthEquip(const EventArgs & e)
{
	StrengthEquipArgs* data = (StrengthEquipArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		//扣除原来的属性加成
		ChangeExtraAtribute(ExtraAtributeType_Strength,m_extraStrengthLv,false);
		//计算现在的档数
		m_extraStrengthLv = CalExtraAtributeLv(ExtraAtributeType_Strength);
		//加上现在的档数对应的属性
		ChangeExtraAtribute(ExtraAtributeType_Strength,m_extraStrengthLv,true);

		m_owner->SynCharAttribute();
	}

	return true;
}

bool ExtraAtributeMgr::HandlePlayerSmelt(const EventArgs& e)
{
	SmeltArgs* data = (SmeltArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		//扣除原来的属性加成
		ChangeExtraAtribute(ExtraAtributeType_Refine,m_extraRefineLv,false);
		//计算现在的档数
		m_extraRefineLv = CalExtraAtributeLv(ExtraAtributeType_Refine);
		//加上现在的档数对应的属性
		ChangeExtraAtribute(ExtraAtributeType_Refine,m_extraRefineLv,true);
	}

	return true;
}

bool ExtraAtributeMgr::HandlePlayerWearEquip(const EventArgs& e)
{
	WearEquipArgs* data = (WearEquipArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		//扣除原来的属性加成
		ChangeExtraAtribute(ExtraAtributeType_Strength,m_extraStrengthLv,false);
		ChangeExtraAtribute(ExtraAtributeType_Refine,m_extraRefineLv,false);
		ChangeExtraAtribute(ExtraAtributeType_EquipLv,m_extraEquipLv,false);
		ChangeExtraAtribute(ExtraAtributeType_Stone,m_extraStoneLv,false);
		//计算现在的档数
		m_extraStrengthLv = CalExtraAtributeLv(ExtraAtributeType_Strength);
		m_extraRefineLv = CalExtraAtributeLv(ExtraAtributeType_Refine);
		m_extraEquipLv = CalExtraAtributeLv(ExtraAtributeType_EquipLv);
		m_extraStoneLv = CalExtraAtributeLv(ExtraAtributeType_Stone);
		//加上现在的档数对应的属性
		ChangeExtraAtribute(ExtraAtributeType_Strength,m_extraStrengthLv,true);
		ChangeExtraAtribute(ExtraAtributeType_Refine,m_extraRefineLv,true);
		ChangeExtraAtribute(ExtraAtributeType_EquipLv,m_extraEquipLv,true);
		ChangeExtraAtribute(ExtraAtributeType_Stone,m_extraStoneLv,true);
	}
	return true;
}

bool ExtraAtributeMgr::HandlePlayerTakeOffEquip(const EventArgs& e)
{
	WearEquipArgs* data = (WearEquipArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		//扣除原来的属性加成
		ChangeExtraAtribute(ExtraAtributeType_Strength,m_extraStrengthLv,false);
		ChangeExtraAtribute(ExtraAtributeType_Refine,m_extraRefineLv,false);
		ChangeExtraAtribute(ExtraAtributeType_EquipLv,m_extraEquipLv,false);
		ChangeExtraAtribute(ExtraAtributeType_Stone,m_extraStoneLv,false);
		//计算现在的档数
		m_extraStrengthLv = CalExtraAtributeLv(ExtraAtributeType_Strength);
		m_extraRefineLv = CalExtraAtributeLv(ExtraAtributeType_Refine);
		m_extraEquipLv = CalExtraAtributeLv(ExtraAtributeType_EquipLv);
		m_extraStoneLv = CalExtraAtributeLv(ExtraAtributeType_Stone);
		//加上现在的档数对应的属性
		ChangeExtraAtribute(ExtraAtributeType_Strength,m_extraStrengthLv,true);
		ChangeExtraAtribute(ExtraAtributeType_Refine,m_extraRefineLv,true);
		ChangeExtraAtribute(ExtraAtributeType_EquipLv,m_extraEquipLv,true);
		ChangeExtraAtribute(ExtraAtributeType_Stone,m_extraStoneLv,true);
	}
	return true;
}


bool ExtraAtributeMgr::HandlePlayerInsertStone(const EventArgs& e)
{
	UseItemEventArgs* data = (UseItemEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_pObj;
	if(player && player->GetID() == m_owner->GetID())
	{
		//扣除原来的属性加成
		ChangeExtraAtribute(ExtraAtributeType_Stone,m_extraStoneLv,false);
		//计算现在的档数
		m_extraStoneLv = CalExtraAtributeLv(ExtraAtributeType_Stone);
		//加上现在的档数对应的属性
		ChangeExtraAtribute(ExtraAtributeType_Stone,m_extraStoneLv,true);
	}

	return true;
}


bool ExtraAtributeMgr::HandlePlayerTakeOffStone(const EventArgs& e)
{
	UseItemEventArgs* data = (UseItemEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_pObj;
	if(player && player->GetID() == m_owner->GetID())
	{
		//扣除原来的属性加成
		ChangeExtraAtribute(ExtraAtributeType_Stone,m_extraStoneLv,false);
		//计算现在的档数
		m_extraStoneLv = CalExtraAtributeLv(ExtraAtributeType_Stone);
		//加上现在的档数对应的属性
		ChangeExtraAtribute(ExtraAtributeType_Stone,m_extraStoneLv,true);
	}

	return true;
}

bool ExtraAtributeMgr::HandlePlayerEquipUpgrade(const EventArgs& e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		//扣除原来的属性加成
		ChangeExtraAtribute(ExtraAtributeType_EquipLv,m_extraEquipLv,false);
		//装备升级，强化星级会降低，所以得从新算
		ChangeExtraAtribute(ExtraAtributeType_Strength,m_extraStrengthLv,false);
		//计算现在的档数
		m_extraEquipLv = CalExtraAtributeLv(ExtraAtributeType_EquipLv);
		m_extraStrengthLv = CalExtraAtributeLv(ExtraAtributeType_Strength);
		//加上现在的档数对应的属性
		ChangeExtraAtribute(ExtraAtributeType_EquipLv,m_extraEquipLv,true);
		ChangeExtraAtribute(ExtraAtributeType_Strength,m_extraStrengthLv,true);
	}

	return true;
}

//根据档数是类型获得增加的属性
void ExtraAtributeMgr::getAttByTypeAndNumber(ExtraAtributeType type,int number,map<ECharAttrType,int64>& attList)
{
	ExtraAtribute tempInfo;
	ExtraAtributeLoader::GetInstance()->GetExtraAtribute(type,number,tempInfo);

	attList[eCharHPMax] 		+= tempInfo.basehp;
	attList[eCharPhyAttackMax]  += tempInfo.baphyackmax;
	attList[eCharPhyDefence] 	+= tempInfo.baphydefmax;
	attList[eCharAvoidRate] 	+= tempInfo.m_Dodge;
	attList[eCharHitRate] 		+= tempInfo.m_Hit;
	attList[eCrackDownRate] 	+= tempInfo.m_Crit;
	attList[eAvoidCrackRate] 	+= tempInfo.m_ResistCrit;

	return ;
}




