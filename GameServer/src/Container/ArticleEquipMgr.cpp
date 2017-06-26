/*
 * ArticleEquipMgr.cpp
 *
 *  Created on: 2014幄1�7�1�7旄1�7
 *      Author: helh
 */

#include "ArticleEquipMgr.h"
#include "../QuickMessage.h"
#include "Log/Logger.h"
#include "MessageCommonRet.h"
#include "../ServerEventDefine.h"
#include "../ExtraAtribute/ExtraAtributeMgr.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "../Fighting/Fighting.h"
#include "math.h"
#include "../Attribute/AttributeManage.h"
#include "ParcelMgr.h"
#include "FileLoader/MaterialSynthesisLoader.h"
#include "../Map/GameZone.h"
#include "../Object/Player/Player.h"
#include "../StatisticMgr/StatisticHandle.h"

ArticleEquipMgr::ArticleEquipMgr(Player* player, size_t MaxContainerSize):ArticleBaseMgr(player, MaxContainerSize)
{
	m_quick.resize(QUICK_SECTOR_COUNT);
}

ArticleEquipMgr::~ArticleEquipMgr()
{

}

void ArticleEquipMgr::ReInit()
{
	ArticleBaseMgr::ReInit();
	m_quick.resize(QUICK_SECTOR_COUNT);
}

void ArticleEquipMgr::Release()
{
	ArticleBaseMgr::Release();
	m_quick.clear();
}

void ArticleEquipMgr::SetArticleInfo(PlayerInfo::ContainerInfo *content)
{

}

void ArticleEquipMgr::GetArticleInfo(CharLogin::EquipQuickInfo *content)
{
	vector<Smart_Ptr<PropBase> >::iterator it = m_gridList.begin();
	for(; it != m_gridList.end(); ++it)
	{
		Smart_Ptr<EquipProp> ptr = *it;
		if(ptr.Get()!=NULL)
		{
			CharLogin::ClientItemInfo *itemInfo = content->add_equip();
			ptr->SetClientInfo(itemInfo);
		}
	}

	vector<QuickSector>::iterator itQuick = m_quick.begin();
	for(; itQuick != m_quick.end(); ++itQuick)
	{
		PlayerInfo::QuickContainer *qInfo = content->add_quick();

		qInfo->set_type((*itQuick).type);
		if((*itQuick).type > 0)
			qInfo->set_value((*itQuick).value);
	}

	m_player->getExtraAtributeMgr()->InitExtraAtribute();
}


void ArticleEquipMgr::SetArticleInfo(const PlayerInfo::EquipQuickInfo *content)
{
	for(int i=0; i<content->equip_size(); ++i)
	{
		PlayerInfo::ItemInfo iInfo = content->equip(i);
		InitEquipItem(&iInfo);
	}

	for(int j=0; j<content->quick_size(); ++j)
	{
		QuickSector tSec;

		m_quick[j].type = content->quick(j).type();
		if(m_quick[j].type > 0)
			m_quick[j].value = content->quick(j).value();
	}
	m_player->SetInitPartDataFlag(eEquipQuickInfo);
}

//上线计算属性
void ArticleEquipMgr::onlineCalculateAtt()
{
	map<ECharAttrType,int64> tempAttList;

	for(size_t i = 0;i < m_gridList.size();++i)
	{
		if(m_gridList[i].Get() == NULL)
			continue;

		getOneEquipAtt(i,m_gridList[i],tempAttList);
	}

	calculateAllSuitAtt(tempAttList);

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttEquip);

	m_player->calculateResultAddAtt(tempAttList,tempValueType);

	calculateFight();

	return ;
}

//计算一次战斗力
int ArticleEquipMgr::calculateFight()
{
	map<ECharAttrType,int64> tempAttList;

	for(size_t i = 0;i < m_gridList.size();++i)
	{
		if(m_gridList[i].Get() == NULL)
			continue;

		getOneEquipAtt(i,m_gridList[i],tempAttList);
	}

	calculateAllSuitAtt(tempAttList);

	int tempResult = m_player->getFightingManage()->CalculateFighting(tempAttList);
	return m_player->getFightingManage()->setFighting(eFightingEquit,tempResult);
}

//获得某件装备的所有属性，套装的不算
void ArticleEquipMgr::getOneEquipAtt(int nPos,Smart_Ptr<EquipProp> equip,map<ECharAttrType,int64>& attList)
{
	if(equip.Get()==NULL)
		return;

	const GoodsInfo* tempData =	GoodsLoader::GetInstance()->GetItemDataByID(equip->GetID());
	if(tempData != NULL)
	{
		attList[eCharHPMax] 	   += int64(tempData->basehp);
		attList[eCharMPMax] 	   += int64(tempData->basemp);
		attList[eCharPhyAttackMax] += int64(tempData->baphyackmax);
		attList[eCharPhyDefence]   += int64(tempData->baphydefmax);
		attList[eCharHitRate]      += int64(tempData->basehit);
		attList[eCharAvoidRate]    += int64(tempData->basedodge);
		attList[eCrackDownRate]    += int64(tempData->basecrit);
		attList[eAvoidCrackRate]   += int64(tempData->baseung);
	}

	attributeBonus tempAttrBonus = equip->GetAttrBonus();

	attList[eCharHPMax] 	   += int64(tempAttrBonus.HP);
	attList[eCharPhyAttackMax] += int64(tempAttrBonus.phyAttack);
	attList[eCharPhyDefence]   += int64(tempAttrBonus.phyDef);
	attList[eCharHitRate]      += int64(tempAttrBonus.hitRate);
	attList[eCharAvoidRate]    += int64(tempAttrBonus.avoidRate);
	attList[eCrackDownRate]    += int64(tempAttrBonus.downRate);
	attList[eAvoidCrackRate]   += int64(tempAttrBonus.crackRate);

	EquipSmithExLoader::GetInstance()->GetStrengthLevelData(nPos,equip->GetUpgrade(),attList);

	vector<int> tempStones = equip->GetStones();
	if(tempStones.size())
	{
		for(uint i = 0; i != tempStones.size(); i++)
		{

			if(tempStones[i] <= 0)
				continue;

			const GoodsInfo* tempToneInfo = 	GoodsLoader::GetInstance()->GetItemDataByID(tempStones[i]);
			if(tempToneInfo == NULL)
				continue;

			switch(tempToneInfo->stoneType)
			{
				case eHPStone:
				{
					attList[eCharHPMax] += tempToneInfo->GetStoneAttr();
					break;
				}
				case eMPStone:
				{
					attList[eCharMPMax] += tempToneInfo->GetStoneAttr();
					break;
				}
				case ePhyATTStone:
				{
					attList[eCharPhyAttackMax] += tempToneInfo->GetStoneAttr();
					break;
				}
				case ePhyDefStone:
				{
					attList[eCharPhyDefence] += tempToneInfo->GetStoneAttr();
					break;
				}
				case eHitStone:
				{
					attList[eCharHitRate] += tempToneInfo->GetStoneAttr();
					break;
				}
				case eAvoidStone:
				{
					attList[eCharAvoidRate] += tempToneInfo->GetStoneAttr();
					break;
				}
				case eCrackStone:
				{
					attList[eCrackDownRate] += tempToneInfo->GetStoneAttr();
					break;
				}
				case eAdCrackStone:
				{
					attList[eAvoidCrackRate] += tempToneInfo->GetStoneAttr();
					break;
				}
			}
		}
	}

	return ;
}

void ArticleEquipMgr::SendArticleInfo()
{

}

void ArticleEquipMgr::GetArticleInfoByPos(CharLogin::ClientContainerInfo *content,PlayerInfo::ContainerInfo *info)
{

}

void ArticleEquipMgr::GetArticleInfo(google::protobuf::Message *info)
{
	PlayerInfo::EquipQuickInfo *tInfo = static_cast<PlayerInfo::EquipQuickInfo *>(info);

	vector<Smart_Ptr<PropBase> >::iterator it = m_gridList.begin();
	for(; it != m_gridList.end(); ++it)
	{
		Smart_Ptr<EquipProp> ptr = *it;
		if(ptr.Get()!=NULL)
		{
			PlayerInfo::ItemInfo *itemInfo = tInfo->add_equip();
			ptr->SetInfo(itemInfo);
		}
	}

	vector<QuickSector>::iterator itQuick = m_quick.begin();
	for(; itQuick != m_quick.end(); ++itQuick)
	{
		PlayerInfo::QuickContainer *qInfo = tInfo->add_quick();

		qInfo->set_type((*itQuick).type);
		if((*itQuick).type > 0)
			qInfo->set_value((*itQuick).value);
	}
}

QuickSector ArticleEquipMgr::GetQuickSector(int gride)
{
	return m_quick[gride];
}


void ArticleEquipMgr::ChangeQuickSector(int srcGride, int destGride)
{
	if(srcGride < 0 || srcGride >= QUICK_SECTOR_COUNT)
	{
		return;
	}
	if(destGride < 0 || destGride >= QUICK_SECTOR_COUNT)
	{
		return;
	}

	QuickSector srcQuickSector = m_quick[srcGride];
	m_quick[srcGride] = m_quick[destGride];
	m_quick[destGride] = srcQuickSector;
}

void ArticleEquipMgr::SetQuickSector(int gride, int type, int value)
{
	if(gride < 0 || gride >= QUICK_SECTOR_COUNT)
	{
		return;
	}

#ifdef DEBUG
	if(type == e_QuickLan_skill)
	{
		LOG_ERROR(FILEINFO,"SetQuickSector playerID:%lld skillID:%d",m_player->GetID(),value);
	}
#endif

	QuickSector quickSector;
	quickSector.type = type;
	quickSector.value = value;
	m_quick[gride] = quickSector;
}

void ArticleEquipMgr::ClearQuickSector(int gride)
{
	if(gride < 0 || gride >= QUICK_SECTOR_COUNT)
	{
		return;
	}
	m_quick[gride].type = 0;
}

void ArticleEquipMgr::UpdateQuickSectorById(int goodsId, int type)
{
	DWORD pos = 0;

	if(goodsId == 0)
	{
		return;
	}

	for(pos = 0; pos < m_quick.size(); pos++)
	{
		if(m_quick[pos].type == 2 && m_quick[pos].value == goodsId)
		{
			ClearQuickSector(pos);
			break;
		}
	}

	Goods::UpdateQuickSector toClient;
	toClient.set_type(type);
	toClient.set_pos(pos);
	toClient.set_goodid(goodsId);
	QuickMessage::GetInstance()->AddSendMessage(m_player->GetID(), m_player->GetChannelID(), &toClient, MSG_SIM_GM2C_UPDATEQUICKSECTOR);
}

int ArticleEquipMgr::IsEmptyQuickSector(int gride)
{
	if(m_quick[gride].type == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int ArticleEquipMgr::GetQuickSectorSize()
{
	return m_quick.size();
}

int ArticleEquipMgr::MoveQuickSector(int type, int arg1, int arg2, int container)
{
	if(type == e_QuickLan_move)
	{
		ChangeQuickSector(arg1, arg2);
	}
	else if(type == e_QuickLan_skill)
	{
		SetQuickSector(arg2, type, arg1);
	}
	else if(type == e_QuickLan_good)
	{
		vector<size_t> epos;

		if(m_player->GetContainer(ePackageType)->HaveGoodsEnought(arg1, 1, epos))
		{
			const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(arg1);
			if(ginfo== NULL)
			{
				LOG_ERROR(FILEINFO, "item id is error");
				return eItemError;
			}

			//能否拖
			if(ginfo->telCD != GameParaLoader::GetInstance()->getHPCD() &&
					ginfo->telCD != GameParaLoader::GetInstance()->getMPCD())
			{
				return eCanPutDragSar;
			}

			for(int i = 0; i < GetQuickSectorSize(); i++)
			{
				if(GetQuickSector(i).type == e_QuickLan_good)
				{
					if(arg1 == GetQuickSector(i).value)
					{
						return eDragSarHaveSame;
					}
				}
			}
			SetQuickSector(arg2, type, arg1);
		}
		else
		{
			return eNotFindItem;
		}
	}
	else if(type == e_QuickLan_clear)
	{
		ClearQuickSector(arg1);
	}
	else
	{
		return eSystemError;
	}
	m_player->SetDataFlag(eEquipQuickInfo);
	m_player->SetDataFlag(ePackageInfo);
	return 0;
}

DWORD ArticleEquipMgr::GetEquipCountWeared(void)
{
	DWORD equipCount = 0;
	for(int i = 0;i < EQUIPITEM_SECTOR_COUNT;++i)
	{
		int size = m_gridList.size();
		if(i >= size)
			break;

		Smart_Ptr<PropBase> base_ptr = m_gridList[i];
		if(bool(base_ptr) == false)
			continue;

		const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(base_ptr->GetID());
		if(ginfo==NULL)
			continue;

		if(ginfo->type != eWeapon && ginfo->type != eArmor && ginfo->type != eJewelry)
			continue;

		equipCount++;
	}

	return equipCount;
}

DWORD ArticleEquipMgr::GetEquipCountStrength()
{
	DWORD StrengthEquipCount = 0;
	bool flag = true;
	for(int i = 0;i < EQUIPITEM_SECTOR_COUNT;++i)
	{
		int size = m_gridList.size();
		if(i >= size)
			break;

		Smart_Ptr<PropBase> base_ptr = m_gridList[i];
		if(bool(base_ptr) == false)
			continue;

		Smart_Ptr<EquipProp> tProp = base_ptr;
		if(tProp)
		{
			if(flag)
			{
				StrengthEquipCount = tProp->GetUpgrade();
				flag = false;
			}
			else
			{
				if(StrengthEquipCount > (DWORD)tProp->GetUpgrade())
				{
					StrengthEquipCount = tProp->GetUpgrade();
				}
			}
		}
	}

	return StrengthEquipCount;
}

DWORD ArticleEquipMgr::GetRefineCount()
{
	DWORD refineCount = 0;
	bool flag = true;
	for(int i = 0;i < EQUIPITEM_SECTOR_COUNT;++i)
	{
		int size = m_gridList.size();
		if(i >= size)
			break;

		Smart_Ptr<PropBase> base_ptr = m_gridList[i];
		if(bool(base_ptr) == false)
			continue;
		Smart_Ptr<EquipProp> tProp = base_ptr;
		if(tProp)
		{
			int temp = 0;
			vector<ReGetAttr> attrBaptizeList;
			tProp->GetAttrBaptizeList(attrBaptizeList);
			for(uint i = 0;i < attrBaptizeList.size();++i)
			{
//				refineCount += attrBaptizeList[i].level;
				if(i == 0)
				{
					temp = attrBaptizeList[i].level;
				}
				else
				{
					if(temp > attrBaptizeList[i].level)
						temp = attrBaptizeList[i].level;
				}
			}

			if(flag)
			{
				refineCount = temp;
				flag = false;
			}
			else
			{
				if(refineCount > (DWORD)temp)
					refineCount = temp;
			}
		}
	}

	return refineCount;
}

DWORD ArticleEquipMgr::GetEquipLvCount()
{
	DWORD quipCount = 0;
	bool flag = true;
	for(int i = 0;i < EQUIPITEM_SECTOR_COUNT;++i)
	{
		int size = m_gridList.size();
		if(i >= size)
			break;

		Smart_Ptr<PropBase> base_ptr = m_gridList[i];
		if(bool(base_ptr) == false)
			continue;

		const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(base_ptr->GetID());
		if(ginfo == NULL)
			continue;

		if(ginfo->type != eWeapon && ginfo->type != eArmor && ginfo->type != eJewelry)
			continue;

		if(flag)
		{
			quipCount = ginfo->useLvReq;
			flag = false;
		}
		else
		{
			if(quipCount > (DWORD)ginfo->useLvReq)
				quipCount = ginfo->useLvReq;
		}

//		quipCount += ginfo->useLvReq;
	}

	return quipCount;
}

DWORD ArticleEquipMgr::GetStoneCount()
{
	DWORD stoneCount = 0;
	bool flag = true;
	for(int i = 0;i < EQUIPITEM_SECTOR_COUNT;++i)
	{
		int size = m_gridList.size();
		if(i >= size)
			break;

		Smart_Ptr<PropBase> base_ptr = m_gridList[i];
		if(bool(base_ptr) == false)
			continue;

		Smart_Ptr<EquipProp> tProp = base_ptr;
		if(tProp)
		{
			bool bResult = true;
			int temp = 0;
			vector<int> stoneList = tProp->GetStones();
			for (uint i = 0; i < stoneList.size(); ++i)
			{
				if (stoneList[i] > 0)
				{
					const GoodsInfo* info =	GoodsLoader::GetInstance()->GetItemDataByID(stoneList[i]);
					if(info==NULL)
						continue;

					if(bResult)
					{
						temp = info->useLvReq;
						bResult = false;
					}
					else
					{
						if(temp < info->useLvReq)
						{
							temp = info->useLvReq;
						}
					}
//					stoneCount += info->useLvReq;
				}
			}

			if(flag)
			{
				stoneCount = temp;
				flag = false;
			}
			else
			{
				if(stoneCount > (DWORD)temp)
				{
					stoneCount = temp;
				}
			}
		}
	}

	return stoneCount;
}

DWORD ArticleEquipMgr::GetStoneCount(int nLv)
{
	DWORD stoneCount = 0;
	for(int i = 0;i < EQUIPITEM_SECTOR_COUNT;++i)
	{
		int size = m_gridList.size();
		if(i >= size)
			break;

		Smart_Ptr<PropBase> base_ptr = m_gridList[i];
		if(bool(base_ptr) == false)
			continue;

		Smart_Ptr<EquipProp> tProp = base_ptr;
		if(tProp)
		{
			vector<int> stoneList = tProp->GetStones();
			for (uint i = 0; i < stoneList.size(); ++i)
			{
				if (stoneList[i] > 0)
				{
					const GoodsInfo* info =	GoodsLoader::GetInstance()->GetItemDataByID(stoneList[i]);
					if(info==NULL)
						continue;
					if(info->useLvReq >= nLv)
						stoneCount ++ ;
				}
			}
		}
	}

	return stoneCount;
}

void ArticleEquipMgr::GetLegendEquipCount(int& legendEquipCount,int& legendAndFullStrengthEquipCount)
{
	for(int i = 0;i < EQUIPITEM_SECTOR_COUNT;++i)
	{
		int size = m_gridList.size();
		if(i >= size)
			break;

		Smart_Ptr<PropBase> base_ptr = m_gridList[i];
		if(bool(base_ptr) == false)
			continue;
		const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(base_ptr->GetID());
		if(ginfo == NULL)
			continue;
		if(ginfo->type != eWeapon && ginfo->type != eArmor && ginfo->type != eJewelry)
			continue;
		Smart_Ptr<EquipProp> tProp = base_ptr;
		if(ginfo->itemQuality >=4)
		{
			legendEquipCount++;
			if(tProp && tProp->GetUpgrade() >= 12)
				legendAndFullStrengthEquipCount++;
		}
	}
}

int ArticleEquipMgr::ChangeAttr_FromEquip(const int &pos,
		const int&before_ID, const int &after_ID)
{
	if(pos >= (int)m_gridList.size())
		return -1;

	const GoodsInfo *good_ptr = NULL;
	int after_hole = 0; //
	int before_hole = 0;
	bool isAdd = false;

	good_ptr = GoodsLoader::GetInstance()->GetItemDataByID(after_ID);
	if(good_ptr)
		after_hole = good_ptr->itemQuality;

	good_ptr = GoodsLoader::GetInstance()->GetItemDataByID(before_ID);
	if(good_ptr)
		before_hole = good_ptr->itemQuality;

	Smart_Ptr<EquipProp> equip_ptr = m_gridList[pos];

	int needhole = 0;
	if(after_hole >before_hole)
	{
		isAdd = true;
		needhole = after_hole - before_hole;
	}
	else if(after_hole == before_hole)
		return 0;
	else
	{
		isAdd = false;
		needhole = before_hole - after_hole;
	}

	vector<int> vecpos;
	equip_ptr->HaveStone_FunPos(needhole, vecpos);

	vector<int>  stone_ptr = equip_ptr->GetStones();
	if(stone_ptr.size())
	{
		for(uint i=0; i<vecpos.size(); ++i)
		{
			if ((int)stone_ptr.size() > vecpos[i])
			{
				int id = stone_ptr[vecpos[i]];

				this->ChangePlayerAttr(id, isAdd);
			}
		}
	}

	return 0;
}


void ArticleEquipMgr::ChangePlayerAttr(const int &stone_ID, bool isAdd)
{
	const GoodsInfo *good_ptr = GoodsLoader::GetInstance()->GetItemDataByID(stone_ID);
	if(!good_ptr)
	{
		return ;
	}

	this->ChangePlayerAttr(good_ptr->stoneType, good_ptr->GetStoneAttr(), isAdd);
}

void ArticleEquipMgr::ChangePlayerAttr(int type, int value, bool isAdd,std::vector<int> &vec,std::vector<int> &vecBro)
{
	if(value <= 0)
		return;

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttEquip);

	switch(type)
	{
	case eCharHPMax:
	{
		if(!isAdd)
		{
			if(m_player->getAttManage()->getValue<int>(eCharHP) + value > m_player->getAttManage()->getValue<int>(eCharHPMax))
				vecBro.push_back(eCharHP);
		}

		m_player->ChangeHPMax(value, isAdd,tempValueType);
		vecBro.push_back(eCharHPMax);
		break;
	}
	case eCharMPMax:
	{
		if(!isAdd)
		{
			if(m_player->getAttManage()->getValue<int>(eCharMP) + value > m_player->getAttManage()->getValue<int>(eCharMPMax))
				vecBro.push_back(eCharMP);
		}

		m_player->ChangeMPMax(value, isAdd,tempValueType);
		vecBro.push_back(eCharMPMax);
		break;
	}
	case eCharPhyAttackMax:
	{
		m_player->ChangePhyAttackMax(value, isAdd,tempValueType);
		vec.push_back(eCharPhyAttackMax);
		break;
	}
	case eCharPhyDefence:
	{
		m_player->ChangePDefence(value, isAdd,tempValueType);
		vec.push_back(eCharPhyDefence);
		break;
	}
	case eCharHitRate:
	{
		m_player->ChangeHRate(value, isAdd,tempValueType);
		vec.push_back(eCharHitRate);
		break;
	}
	case eCharAvoidRate:
	{
		m_player->ChangeARate(value, isAdd,tempValueType);
		vec.push_back(eCharAvoidRate);
		break;
	}
	case eCrackDownRate:
	{
		m_player->ChangeCRate(value, isAdd,tempValueType);
		vec.push_back(eCrackDownRate);
		break;
	}
	case eAvoidCrackRate:
	{
		m_player->ChangeBaserte(value, isAdd,tempValueType);
		vec.push_back(eAvoidCrackRate);
		break;
	}
	}
}

void ArticleEquipMgr::ChangePlayerAttr(int type, int value, bool isAdd)
{
	vector<int> vec;
	vector<int> vecBro;
	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttEquip);
	switch(type)
	{
	case eHPStone:
	{
		if(!isAdd)
		{
			if(m_player->getAttManage()->getValue<int>(eCharHP) + value > m_player->getAttManage()->getValue<int>(eCharHPMax))
				vecBro.push_back(eCharHP);
		}

		m_player->ChangeHPMax(value, isAdd,tempValueType);
		vecBro.push_back(eCharHPMax);
		break;
	}
	case eMPStone:
	{
		if(!isAdd)
		{
			if(m_player->getAttManage()->getValue<int>(eCharMP) + value > m_player->getAttManage()->getValue<int>(eCharMPMax))
				vecBro.push_back(eCharMP);
		}

		m_player->ChangeMPMax(value, isAdd,tempValueType);
		vecBro.push_back(eCharMPMax);
		break;
	}
	case ePhyATTStone:
	{
		m_player->ChangePhyAttackMax(value, isAdd,tempValueType);
		vec.push_back(eCharPhyAttackMax);
		break;
	}
	case ePhyDefStone:
	{
		m_player->ChangePDefence(value, isAdd,tempValueType);
		vec.push_back(eCharPhyDefence);
		break;
	}
	case eHitStone:
	{
		m_player->ChangeHRate(value, isAdd,tempValueType);
		vec.push_back(eCharHitRate);
		break;
	}
	case eAvoidStone:
	{
		m_player->ChangeARate(value, isAdd,tempValueType);
		vec.push_back(eCharAvoidRate);
		break;
	}
	case eCrackStone:
	{
		m_player->ChangeCRate(value, isAdd,tempValueType);
		vec.push_back(eCrackDownRate);
		break;
	}
	case eAdCrackStone:
	{
		m_player->ChangeBaserte(value, isAdd,tempValueType);
		vec.push_back(eAvoidCrackRate);
		break;
	}
	}

	if(isAdd)
	{
		//换装时计算一次战斗力
		calculateFight();

		vec.push_back(eFightPower);
	}

	m_player->BroadcastCharAttribute(vecBro);
	m_player->SynCharAttribute(vec);
}

void ArticleEquipMgr::InitEquipItem(PlayerInfo::ItemInfo* info)
{
	Smart_Ptr<PropBase> newProp;
	Smart_Ptr<EquipProp> tProp;

	tProp = new EquipProp(info->id());
	newProp = tProp;
	tProp->SetServerInfo(info);
	m_gridList.push_back(newProp);

	if(m_player->GetMyself())
	{
		BuyItemEventArgs e(m_player->GetMyself(),info->id(),1,0);
		m_player->FireEvent(PLAYER_PACKET_ADDITEM, e);
	}
}

void ArticleEquipMgr::ChangeAttrbuteByEquip(bool isAdd,const Smart_Ptr<EquipProp> &propBase,int nPos)
{
	vector<int> vec;
	vector<int> vecBro;

	attributeBonus tempAttrBonus = propBase->GetAttrBonus();
	vector<int> stones = propBase->GetStones();
	map<ECharAttrType,int64> tempAttList;

	const GoodsInfo* goodsInfo = GoodsLoader::GetInstance()->GetItemDataByID(propBase->GetID());
	if(goodsInfo != NULL)
	{
		tempAttList[eCharHPMax] 	   += int64(goodsInfo->basehp);
		tempAttList[eCharMPMax] 	   += int64(goodsInfo->basemp);
		tempAttList[eCharPhyAttackMax] += int64(goodsInfo->baphyackmax);
		tempAttList[eCharPhyDefence]   += int64(goodsInfo->baphydefmax);
		tempAttList[eCharHitRate]      += int64(goodsInfo->basehit);
		tempAttList[eCharAvoidRate]    += int64(goodsInfo->basedodge);
		tempAttList[eCrackDownRate]    += int64(goodsInfo->basecrit);
		tempAttList[eAvoidCrackRate]   += int64(goodsInfo->baseung);
	}

	tempAttList[eCharHPMax] 	   += int64(tempAttrBonus.HP);
	tempAttList[eCharPhyAttackMax] += int64(tempAttrBonus.phyAttack);
	tempAttList[eCharPhyDefence]   += int64(tempAttrBonus.phyDef);
	tempAttList[eCharHitRate]      += int64(tempAttrBonus.hitRate);
	tempAttList[eCharAvoidRate]    += int64(tempAttrBonus.avoidRate);
	tempAttList[eCrackDownRate]    += int64(tempAttrBonus.downRate);
	tempAttList[eAvoidCrackRate]   += int64(tempAttrBonus.crackRate);

	EquipSmithExLoader::GetInstance()->GetStrengthLevelData(nPos,propBase->GetUpgrade(),tempAttList);

	if(stones.size())
	{
		for(uint i = 0; i != stones.size(); i++)
		{
			int stoneID = stones[i];
			if(stoneID >0)
			{
				const GoodsInfo* stoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(stoneID);
				if(stoneInfo==NULL)
					continue;
				switch(stoneInfo->stoneType)
				{
					case eHPStone:
					{
						tempAttList[eCharHPMax] += stoneInfo->GetStoneAttr();

						break;
					}
					case eMPStone:
					{
						tempAttList[eCharMPMax] += stoneInfo->GetStoneAttr();

						break;
					}
					case ePhyATTStone:
					{
						tempAttList[eCharPhyAttackMax] += stoneInfo->GetStoneAttr();

						break;
					}
					case ePhyDefStone:
					{
						tempAttList[eCharPhyDefence] += stoneInfo->GetStoneAttr();

						break;
					}
					case eHitStone:
					{
						tempAttList[eCharHitRate] += stoneInfo->GetStoneAttr();

						break;
					}
					case eAvoidStone:
					{
						tempAttList[eCharAvoidRate] += stoneInfo->GetStoneAttr();

						break;
					}
					case eCrackStone:
					{
						tempAttList[eCrackDownRate]  += stoneInfo->GetStoneAttr();

						break;
					}
					case eAdCrackStone:
					{
						tempAttList[eAvoidCrackRate] += stoneInfo->GetStoneAttr();

						break;
					}
					default:
					{
						break;
					}
				}
			}
		}
	}

	getSuitAttList(!isAdd,propBase,tempAttList);

	map<ECharAttrType,int64>::iterator tempIt = tempAttList.begin();
	for(;tempIt != tempAttList.end();++tempIt)
	{
		ChangePlayerAttr(tempIt->first,tempIt->second,isAdd,vec,vecBro);
	}

	//目前ChangeAttrbuteByEquip都是成对出现，先有false，再有true,所以为true时才算装备模块的战半力，如果有哪个地方不是成对的，则要另外调用战半力函数
	if(isAdd)
	{
		//换装时计算一次战斗力
		calculateFight();

		vec.push_back(eFightPower);
	}

	m_player->BroadcastCharAttribute(vecBro);
	m_player->SynCharAttribute(vec);
}



int ArticleEquipMgr::CheckWearEquip(int goodsId, const GoodsInfo &ginfo)
{
	if(m_player->getAttManage()->getValue<int>(eCharLv) < ginfo.useLvReq)
	{
		return eLVNotEnough;
	}

	//现在0为通用穿戴物品
	if(ginfo.wearJob != 0 && ((m_player->getAttManage()->getValue<BYTE>(eCharProfession) & 0xffff) != ginfo.wearJob))
	{
		return eProfessionNotMatch;
	} 	//性别为2时表示男女通用

	if(ginfo.type != eWeapon && ginfo.type != eArmor && ginfo.type != eJewelry)
		return eGoodsTypeError;

	return 0;
}

/*
 *
 * */

int ArticleEquipMgr::WearEquip(int type, int gride, int goodsId)
{
	vector<DWORD> vec;
	int pos = 0;

	if(m_player->IsDead())
	{
		return eCreatureDead;
	}

	const	GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsId);
	if(ginfo == NULL)
		return eItemError;

//	StatisticInfo::IDAndCostAndGotInfo idAndCostAndGotInfo;
//	StatisticInfo::CostAndGotInfo * costAndGotInfo = idAndCostAndGotInfo.mutable_costandgotlist();

	if(type == e_EquipOperation_Wear)
	{

		int res = 0;
		res = CheckWearEquip(goodsId, *ginfo);
		if(!res)
		{

			Smart_Ptr<EquipProp> propBase = m_player->GetContainer(ePackageType)->GetItemDataPtr(gride);
			if(!propBase || propBase->GetID() != goodsId)
			{
				return eItemError;
			}
			if(propBase->IsValid())
			{
				return eItemError;
			}
			else
			{
				BEHAVIOR_MONI(m_player->GetMyself(), BehaviorType::Main::eEquip, BehaviorType::Minor::eEquip_Wear, MONITOR);

				if(ginfo->bindTyle == eUseBind)
				{
					propBase->SetBind(true);
				}

				int ItemPlace = ginfo->itemPlace - 1;

				Smart_Ptr<EquipProp> changePropBase = m_player->GetContainer(eEquipQuickType)->GetItemDataPtr(ItemPlace);

				//如果本位置有装备了,那么就要进行替换
				if(changePropBase && !changePropBase->IsValid())
				{
						this->ChangeAttrbuteByEquip(false,changePropBase,ItemPlace);
				}

				this->ChangeEquipPos(ePackageType, gride, eEquipQuickType, ItemPlace, vec);

				this->ChangeAttrbuteByEquip(true,propBase,ItemPlace);

				if (m_player->GetMyself())
				{
					WearEquipArgs e(m_player->GetMyself(), goodsId);
					m_player->FireEvent(PLAYER_WEAR_EQUIP,e);
				}

				pos = ItemPlace;

//				idAndCostAndGotInfo.set_beforeid(ePackageType);
//				idAndCostAndGotInfo.set_afterid(eEquipQuickType);
//				StatisticInfo::CostItemInfo * costInfo = costAndGotInfo->add_costlist();
//				StatisticMgr::SetItemCost(costInfo,propBase->GetID(),1);
				
			}


		}
		else
		{
			return res;
		}
	}
	else if(type == e_EquipOperation_Drop)
	{
		Smart_Ptr<EquipProp> propBase = m_player->GetContainer(eEquipQuickType)->GetItemDataPtr(gride-1);
		if(!propBase || propBase->GetID() != goodsId)
		{
			return eItemError;
		}
		if(propBase->IsValid())
		{
			return eItemError;
		}
		else
		{
			int newGride = m_player->GetContainer(ePackageType)->FindNullPos();
			if(newGride == -1)
			{
				return ePackageFull;
			}
			else
			{
				BEHAVIOR_MONI(m_player->GetMyself(), BehaviorType::Main::eEquip, BehaviorType::Minor::eEquip_Drop, MONITOR);

				this->ChangeAttrbuteByEquip(false,propBase,gride-1);

				this->ChangeEquipPos(eEquipQuickType, gride-1, ePackageType, newGride, vec);

				//脱掉装备时ChangeAttrbuteByEquip不是成对出现
				calculateFight();
				vector<int> tempPower;
				tempPower.push_back(eFightPower);
				m_player->SynCharAttribute(tempPower);

				pos = gride -1;

				if (m_player->GetMyself())
				{
					WearEquipArgs e(m_player->GetMyself(), goodsId);
					m_player->FireEvent(PLAYER_TAKE_OFF_EQUIP,e);
				}

//				idAndCostAndGotInfo.set_beforeid(eEquipQuickType);
//				idAndCostAndGotInfo.set_afterid(ePackageType);
//				StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo->add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo,propBase->GetID(),1);


			}
		}
	}
	else
	{
		return eClientSendDataError;
	}

//	if(eAccountType_User == m_player->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_player->GetID(),eStatic_Package,eStaMinor_Package_Move,&idAndCostAndGotInfo);

	//装备主武器
	vector<eItemPlace> vecType;
	if(pos == (eItemPlaceWeapon-1))
		vecType.push_back(eItemPlaceWeapon);
	else if(pos == (eItemPlaceArmor-1))
		vecType.push_back(eItemPlaceArmor);

	syncEquip(vecType);

	m_player->SendSectorChange(vec);

	m_player->SetDataFlag(ePackageInfo);
	m_player->SetDataFlag(eEquipQuickInfo);

//	UpdateEquipUI();
	Smart_Ptr<ArticleParcelMgr> parcelMgr = m_player->GetContainer(ePackageType);
	m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Package, eUIHLSmall_Package_Full), parcelMgr->IsGirdFull());


	return 0;
}

int ArticleEquipMgr::ChangeEquipPos(const int& startContainer,
		const int& startGride, const int& endContainer, const int& endGride,
		vector<DWORD>& vec)
{
	/*由于 装备 物品  变成  物品  宝石 清除其他属性  保留   后续 注意 宝石 要 保留到 相应 部位
	 * 查看 背包 穿戴  EuipStoneWhenDrop  比较 特殊
	*/

	Smart_Ptr<EquipProp> srcEquip;
	Smart_Ptr<EquipProp> DestEquip;
	Smart_Ptr<PropBase> SrcPropBase;
	Smart_Ptr<PropBase> DestPropBase;


	//脱下
	if(startContainer == eEquipQuickType)
	{
		srcEquip = m_player->GetContainer(eEquipQuickType)->GetItemDataPtr(startGride);
		if(srcEquip.Get()==NULL)
				return -1;

		DestEquip = NewEquipPropStone(srcEquip);

		srcEquip->Relasses();

		SrcPropBase = srcEquip;

		DestPropBase = DestEquip;
	}

	//穿上
	if(endContainer == eEquipQuickType)
	{
		srcEquip = m_player->GetContainer(startContainer)->GetItemDataPtr(startGride);
		SrcPropBase = srcEquip;

		DestEquip = m_player->GetContainer(eEquipQuickType)->GetItemDataPtr(endGride);
		if(srcEquip.Get()==NULL || DestEquip.Get()==NULL)
			return -1;

		vector<int> vec = DestEquip->GetStones();
		if (0 < vec.size())
		{
			srcEquip->SetStones(vec);
		}

		vector<ReGetAttr> attrBaptizeList;
		DestEquip->GetAttrBaptizeList(attrBaptizeList);
		srcEquip->SetAttrBaptizeList(attrBaptizeList);

		srcEquip->SetUpgrade(DestEquip->GetUpgrade());

		DestEquip->Relasses();

		DestPropBase = DestEquip;
	}

	if(!DestPropBase || !SrcPropBase)
		return -1;

	m_player->GetContainer(startContainer)->SaveGrideItemInfo(startGride, DestPropBase);
	m_player->GetContainer(endContainer)->SaveGrideItemInfo(endGride, SrcPropBase);

	vec.push_back((startContainer << 16) | startGride);
	vec.push_back((endContainer << 16) | endGride);

	return 0;
}

Smart_Ptr<EquipProp> ArticleEquipMgr::NewEquipPropStone(Smart_Ptr<EquipProp> equipProp)
{
	Smart_Ptr<EquipProp> newProp;

	newProp = new EquipProp(-1);
	newProp->SetStones(equipProp->GetStones());

	vector<ReGetAttr> attrBaptizeList;
	equipProp->GetAttrBaptizeList(attrBaptizeList);
	newProp->SetAttrBaptizeList(attrBaptizeList);

	newProp->SetUpgrade(equipProp->GetUpgrade());
	return newProp;
}

void ArticleEquipMgr::IsAllEquipCanStrength(int& can, vector<bool>* canIndex)
{
	can = 0;
	Smart_Ptr<ArticleParcelMgr> parcel = m_player->GetContainer(ePackageType);
	if (!parcel)
	{
		return;
	}

	for (uint i = 0; i < EQUIPITEM_SECTOR_COUNT; ++i)
	{
		Smart_Ptr<EquipProp> equip = GetItemDataPtr(i);

		if ((bool)equip && (uint)equip->GetUpgrade() < Strength_Max_Lv)
		{
			const StrengthBaseParam* pStrength = EquipSmithExLoader::GetInstance()->GetStrengthBaseParam(equip->GetUpgrade());
			if(pStrength)
			{

				const GoodsInfo* strengthItemInfo = GoodsLoader::GetInstance()->GetItemDataByID(pStrength->nItemId);

				if (pStrength->nItemNum > 0 && NULL != strengthItemInfo)
				{
					int stoneCount = 0;
					stoneCount = parcel->FindPropCount(pStrength->nItemId);
					if(stoneCount >= pStrength->nItemNum && m_player->getAttManage()->getValue<int>(eCharMoney) >= pStrength->nMoney)
					{
						can++;
						if (NULL != canIndex)
						{
							canIndex->push_back(true);
						}
						continue;
					}
				}
			}
		}
		if (NULL != canIndex)
		{
			canIndex->push_back(false);
		}
	}
}

void ArticleEquipMgr::IsAllEquipCanInlay(int& canInlay, int& quickUp, vector<bool>* canIndex)
{
	canInlay = 0;
	quickUp = 0;
	map<int, int> itemList;
	Smart_Ptr<ArticleParcelMgr> parce = m_player->GetContainer(ePackageType);
	parce->GetAllIem(itemList);

	for (uint i = 0; i < EQUIPITEM_SECTOR_COUNT; ++i)
	{
		Smart_Ptr<EquipProp> equip = GetItemDataPtr(i);

		if ((bool)equip)
		{
			{
				bool flag = false;
				for ( uint j = 0; j < Com_FillStone_HoleNum; j++)
				{
					map<int, int>::iterator item = itemList.begin();
					map<int, int>::iterator itemEnd = itemList.end();
					int curStoneID = equip->IsHaveStone(j);
					if (0 == curStoneID)
					{
						for ( ; item != itemEnd; ++item)
						{
							const GoodsInfo* goods = GoodsLoader::GetInstance()->GetItemDataByID(item->first);
							if (NULL != goods && goods->type == eStone && goods->stoneType == EquipSmithExLoader::GetInstance()->GetInlayStoneType(i) && 0 == equip->IsAddStoneFull())
							{
								if (m_player->getAttManage()->getValue<int>(eCharLv) < EquipSmithExLoader::GetInstance()->GetInlayPosLv(i,j))
								{
									canInlay++;
									flag = true;
									if (NULL != canIndex)
									{
										canIndex->push_back(true);
									}

									break;
								}
							}
						}
					}
					else
					{
						// 达到快速宝石升级条件有多少个
						vector<MeterialSynthesisItem> canSynthItem;
						CMeterialSynthesisLoader::GetInstance()->GetInfoBySrcMaterialID(curStoneID, canSynthItem);
						if (0 < canSynthItem.size())
						{
							int money = m_player->getAttManage()->getValue<int>(eCharMoney);

							for (uint j = 0; j < canSynthItem.size(); ++j)
							{
								MeterialSynthesisItem& info = canSynthItem[j];

								const GoodsInfo* destStoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(info.destMeterialID);

								if (NULL != destStoneInfo && destStoneInfo->type == eStone  && info.costMoney <= money)
								{
									vector<pair<int, int> >& srcMeterialID = info.srcMeterialID;
									for (uint i = 0; i < srcMeterialID.size(); ++i)
									{
										map<int, int>::iterator itTemp = itemList.find(srcMeterialID[i].first);
										if (itemList.end() != itTemp && srcMeterialID[i].second - 1 <= itTemp->second)
										{
											quickUp++;
											flag = true;
											if (NULL != canIndex)
											{
												canIndex->push_back(false);
											}

											break;
										}
									}
									if (flag)
									{
										break;
									}
								}
							}
						}
					}
					if (flag)
					{
						break;
					}
				}
				if (flag)
				{
					continue;
				}
			}
		}
		if (NULL != canIndex)
		{
			canIndex->push_back(false);
		}
	}

}

void ArticleEquipMgr::IsAllEquipCanRefine(int& can, vector<bool>* canIndex)
{
	can = 0;
	Smart_Ptr<ArticleParcelMgr> parcelContainer = m_player->GetContainer(ePackageType);
	if (!parcelContainer)
	{
		return;
	}

	for (uint i = 0; i < EQUIPITEM_SECTOR_COUNT; ++i)
	{
		//得到装备指针
		Smart_Ptr<EquipProp> equip = GetItemDataPtr(i);
		if ((bool)equip)
		{
			//得到装备配置表
			const RefineBaseParam* pRefine = EquipSmithExLoader::GetInstance()->GetRefineBaseParam();
			if(NULL != pRefine && m_player->getAttManage()->getValue<int>(eCharMoney) >= pRefine->nMoney)
			{
				//得到宝石位置
				int cnt = parcelContainer->FindPropCount(pRefine->nItemId);
				if (pRefine->nItemNum <= cnt)
				{
					can++;
					if (NULL != canIndex)
					{
						canIndex->push_back(true);
					}
					continue;

				}
			}
		}
		if (NULL != canIndex)
		{
			canIndex->push_back(false);
		}
	}
}

void ArticleEquipMgr::IsAllEquipCanUpgrade(int& can, vector<bool>* canIndex)
{
	can = 0;
	Smart_Ptr<ArticleParcelMgr> parcel = m_player->GetContainer(ePackageType);
	if (!parcel)
	{
		return;
	}

	for (uint i = 0; i < EQUIPITEM_SECTOR_COUNT; ++i)
	{
		//得到装备指针
		Smart_Ptr<EquipProp> equip = GetItemDataPtr(i);

		if ((bool)equip && -1 != equip->GetID())
		{
			const GoodsInfo* equipInfo = GoodsLoader::GetInstance()->GetItemDataByID(equip->GetID());
			const GoodsInfo* laterEquipInfo = GoodsLoader::GetInstance()->GetItemDataByID(equip->GetID()+1);


			if (NULL != equipInfo && NULL != laterEquipInfo && (eWeapon == laterEquipInfo->type  ||
					eArmor == laterEquipInfo->type || eJewelry == laterEquipInfo->type) &&
					equipInfo->useLvReq != GameParaLoader::GetInstance()->getEquipMaxLv() &&
					laterEquipInfo->useLvReq <= m_player->getAttManage()->getValue<int>(eCharLv))
			{
				int nEquipType = equipInfo->suitID > 0?2:1;
				const UpgradeBaseParam* pEquiplv = EquipSmithExLoader::GetInstance()->GetUpgradeBaseParam(nEquipType,equipInfo->itemQuality,equipInfo->useLvReq);
				if(pEquiplv && m_player->getAttManage()->getValue<int>(eCharMoney) >= pEquiplv->nMoney)
				{

//					vector<int> notUpItemID;
//					// 获取不能升级的装备ID
//					EquipSmithLoader::GetInstance()->GetNotUpItemID(notUpItemID);
//					bool flag = false;
//					for (uint i = 0; i < notUpItemID.size(); ++i)
//					{
//						if (equipInfo->itemID == notUpItemID[i])
//						{
//							flag = true;
//							break;
//						}
//					}

//					if (!flag)
//					{
						int haveStoneCount = parcel->FindPropCount(pEquiplv->nItemId);
						if(haveStoneCount >= pEquiplv->nItemNum)
						{
							can++;
							if (NULL != canIndex)
							{
								canIndex->push_back(true);
							}
							continue;
						}
//					}
				}
			}
		}
		if (NULL != canIndex)
		{
			canIndex->push_back(false);
		}
	}
}

void ArticleEquipMgr::initQuickSkill(int skillID)
{
	m_quick[0].type = e_QuickLan_skill;	
	m_quick[0].value = skillID; 

	return ;
}

void ArticleEquipMgr::syncEquip(vector<eItemPlace>& poss)
{
	if(poss.size() == 0)
	{
		return ;
	}
	PlayerSynch::AlterPlayerEquip tEquipMsg;	
	for(size_t i = 0;i < poss.size();++i)
	{
		for(size_t j = 0;j < m_gridList.size();++j)
		{
			if((size_t)poss[i] != j + 1)
			{
				continue;
			}

			Smart_Ptr<EquipProp> tEquip = m_gridList[j];
			PlayerSynch::RemotePlayerEquip* tOne = tEquipMsg.add_equips();
			if(!tEquip || tEquip->IsValid())
			{
				tOne->set_id(0);
				tOne->set_type(poss[i]);
				tOne->set_strenggrade(0);	
			}
			else
			{
				tOne->set_id(tEquip->GetID());
				tOne->set_type(poss[i]);
				tOne->set_strenggrade(tEquip->GetUpgrade());	
			}
		}
	}

	tEquipMsg.set_playerid(m_player->GetKey());

	if(m_player->GetGameZone())
	{
		m_player->GetGameZone()->SendSynchMessage(&tEquipMsg,MSG_SIM_GM2C_REMOTE_PLAYER_EQUIP,m_player->GetKey(),m_player->GetXNewPos(),m_player->GetYNewPos(),false);
	}

	return ;
}

//void ArticleEquipMgr::UpdateEquipUI()
//{
//	int count = 0;
//	vector<E_UIHL_HANDLE> someHandle;
//	vector<E_UIHL_BIGTYPE> someBigType;
//	vector<E_UIHL_SMALLTYPE> someSmallType;
//	vector<bool> someFlag;
//	vector<int> counts;
//
//	if (m_player->IsOpenSystem(eEquipStrengtLv))
//	{
//		IsAllEquipCanStrength(count);
//	}
//
//	someSmallType.push_back(eUIHLSmall_EquipSmith_StrengthUnLvCeil);
//	counts.push_back(count);
//
//	count = 0;
//	if (m_player->IsOpenSystem(eEquipInlayLv))
//	{
//		IsAllEquipCanInlay(count);
//	}
//	someSmallType.push_back(eUIHLSmall_EquipSmith_StrengthUnLvCeil);
//	counts.push_back(count);
//
//	count = 0;
//	if (m_player->IsOpenSystem(eEquipRefineLv))
//	{
//		IsAllEquipCanRefine(count);
//	}
//	someSmallType.push_back(eUIHLSmall_EquipSmith_StrengthUnLvCeil);
//	counts.push_back(count);
//
//	count = 0;
//	if (m_player->IsOpenSystem(eEquipUpgradeLv) )
//	{
//		IsAllEquipCanUpgrade(count);
//	}
//	someSmallType.push_back(eUIHLSmall_EquipSmith_StrengthUnLvCeil);
//	counts.push_back(count);
//
//
//	someHandle.resize(someSmallType.size(), eUIHLHandle_ModAndSyn);
//	someBigType.resize(someSmallType.size(), eUIHLBig_EquipSmith);
//	someFlag.resize(someSmallType.size(), false);
//	m_player->UpdateUIHighLight(someHandle, someBigType, someSmallType, counts, someFlag);
//
//}

//获得穿上或脱下一件装备的套装属性
void ArticleEquipMgr::getSuitAttList(bool isDrop,const Smart_Ptr<EquipProp> equipPtr,map<ECharAttrType,int64>& attList)
{
	if(!equipPtr)
		return ;

	const GoodsInfo* tempInfo = GoodsLoader::GetInstance()->GetItemDataByID(equipPtr->GetID());
	if(tempInfo == NULL)
		return ;

	//装备部位
	short tempPos = tempInfo->itemPlace - 1;
	if(tempPos >= (short)m_gridList.size())
		return ;

	if(tempInfo->suitID == 0)
		return ;

	//获取穿上或脱下后该装备的套装件数
	int tempCounts = getSuitIDCounts(tempInfo->suitID,tempPos,!isDrop);

	map<ECharAttrType,int> tempCurrentAttList = GoodsLoader::GetInstance()->getSuitAttListByIDAndCounts(tempInfo->suitID,tempCounts);
	map<ECharAttrType,int>::iterator tempIt;

	//取未脱下的属性
	if(isDrop)
	{
		map<ECharAttrType,int> tempAttList = GoodsLoader::GetInstance()->getSuitAttListByIDAndCounts(tempInfo->suitID,tempCounts + 1);
		tempIt = tempAttList.begin();

		for(;tempIt != tempAttList.end();++tempIt)
		{
			attList[tempIt->first] += tempIt->second;
		}

		tempIt = tempCurrentAttList.begin();
		for(;tempIt != tempCurrentAttList.end();++tempIt)
		{
			attList[tempIt->first] -= tempIt->second;
		}
	}
	//获取未穿上的属性
	else
	{
		map<ECharAttrType,int> tempAttList = GoodsLoader::GetInstance()->getSuitAttListByIDAndCounts(tempInfo->suitID,tempCounts - 1);
		tempIt = tempAttList.begin();

		for(;tempIt != tempAttList.end();++tempIt)
		{
			attList[tempIt->first] -= tempIt->second;
		}

		tempIt = tempCurrentAttList.begin();
		for(;tempIt != tempCurrentAttList.end();++tempIt)
		{
			attList[tempIt->first] += tempIt->second;
		}
	}



	return ;
}

//获得身上某套装件数
int ArticleEquipMgr::getSuitIDCounts(int suitID,int pos,bool calculate)
{
	if(suitID == 0)
		return 0;

	short tempCounts 		  = 0;
	const GoodsInfo* tempInfo = NULL;

	for(size_t i = 0;i < m_gridList.size();++i)
	{
		//不计算该部位
		if((size_t)pos == i && !calculate)
			continue;

		if(m_gridList[i]->GetID() <= 0)
			continue;

		tempInfo = GoodsLoader::GetInstance()->GetItemDataByID(m_gridList[i]->GetID());
		if(tempInfo == NULL)
			continue;

		if(tempInfo->suitID != suitID)
			continue;

		++tempCounts;
	}

	return tempCounts;
}

//计算身上所有套装属性
void ArticleEquipMgr::calculateAllSuitAtt(map<ECharAttrType,int64>& attList)
{
	const GoodsInfo* tempInfo = NULL;
	map<int,int> tempSuitList;

	for(size_t i = 0;i < m_gridList.size();++i)
	{
		if(m_gridList[i]->GetID() <= 0)
			continue;

		tempInfo = GoodsLoader::GetInstance()->GetItemDataByID(m_gridList[i]->GetID());
		if(tempInfo == NULL)
			continue;

		if(tempInfo->suitID <= 0)
			continue;

		++tempSuitList[tempInfo->suitID];

		tempInfo = NULL;
	}

	map<int,int>::iterator tempOutIt = tempSuitList.begin();
	map<ECharAttrType,int> tempAttList;
	map<ECharAttrType,int>::iterator tempInIt;

	for(;tempOutIt != tempSuitList.end();++tempOutIt)
	{
		tempAttList = GoodsLoader::GetInstance()->getSuitAttListByIDAndCounts(tempOutIt->first,tempOutIt->second);
		tempInIt    =  tempAttList.begin();

		for(;tempInIt != tempAttList.end();++tempInIt)
		{
			attList[tempInIt->first] += tempInIt->second;
		}
	}


	return ;
}





