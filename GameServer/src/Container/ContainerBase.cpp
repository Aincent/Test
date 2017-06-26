/*
 * ContainerBase.cpp
 *
 *  Created on: 2014幄1�7�1�7旄1�7
 *      Author: helh
 */

#include "ContainerBase.h"
#include "../Object/Player/Player.h"
#include "Goods/PropDefine.h"
#include "Log/Logger.h"
#include "Network/Message_Facade.h"
#include "ServerMsgDefine.h"
#include "../ServerManager.h"
#include "Network/MessageManager.h"
#include "../MessageBuild.h"
#include "MessageCommonRet.h"
#include "FightDefined.h"
#include "../Object/FileLoader/NPCLoader.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "../Object/FileLoader/GirlWarLoader.h"
#include "ArticleEquipMgr.h"
#include "../ServerEventDefine.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "../QuickMessage.h"
#include "Goods/MakeProp.h"
#include "../EquipSmith/MaterialSynthesis/CMeterialSynMessage.h"
#include "ParcelMgr.h"
#include "../StatisticMgr/StatisticHandle.h"

ArticleBaseMgr::ArticleBaseMgr(Player * player, size_t MaxContainerSize):m_player(player),m_maxContainerSize(MaxContainerSize)
{
	ReInit();
}

ArticleBaseMgr::~ArticleBaseMgr()
{
	//Release();
}

void ArticleBaseMgr::ReInit()
{
	m_openFlag = false;
	m_curContainerSize = 0;
	m_openTime = 0;
}

void ArticleBaseMgr::Release()
{
	m_gridList.clear();
	m_lockGridList.clear();
}

//根据ClientItemInfo得到 ItemInfo
void ArticleBaseMgr::GetItemInfoByClItInfo(const CharLogin::ClientItemInfo *Item,PlayerInfo::ItemInfo &info)
{
	info.set_id(Item->id());
	info.set_flag(Item->flag());
	for(int i = 0 ; i < Item->newattr_size() ; i++)
	{
		info.add_newattr(Item->newattr(i));
	}
	for(int i = 0 ; i < Item->refineattrlv_size() ; i++)
	{
		info.add_refineattrlv(Item->refineattrlv(i));
	}
	for(int i = 0 ; i < Item->stoneid_size() ; i++)
	{
		info.add_stoneid(Item->stoneid(i));
	}
	for(int i = 0 ; i < Item->inlaypos_size() ; i++)
	{
		info.add_inlaypos(Item->inlaypos(i));
	}
	info.set_num(Item->num());
	info.set_endtime(Item->time());
}

//添加物品到背包根据CharLogin::ClientItemInfo紧限背包容器调用
bool ArticleBaseMgr::Mail_AddItems(const CharLogin::ClientItemInfo *Item,vector<DWORD> &vec)
{
	Smart_Ptr<PropBase> newProp;
	PlayerInfo::ItemInfo info;
	GetItemInfoByClItInfo(Item,info);
	int nullGrid = -1;
	DWORD i = 0;

	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(info.id());
	if(ginfo==NULL)
		return true;

	if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeMoneyItemId())//铜钱
	{
		m_player->MoneyChange(info.num(),true);

//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//		StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,info.num());
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_player->GetID(),eStatic_Money,eStaMinor_Money_Mail,&costAndGotInfo);
		return true;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeExpItemId())
	{
		m_player->AddExp(info.num());
		return true;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeForceItemId())
	{
		m_player->ChangeCurForce(info.num(),true);
		m_player->SynCharAttribute(eCharForce,m_player->getAttManage()->getValue<int>(eCharForce));
		return true;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeGoldenItemId())
	{
		m_player->ChangeGolden(info.num(),true);
		m_player->SynCharAttribute(eCharGolden,m_player->getAttManage()->getValue<int>(eCharGolden));
		return true;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeBindGoldenItemId())
	{
		m_player->ChangeBindGolden(info.num(),true);
		m_player->SynCharAttribute(eCharBindGolden,m_player->getAttManage()->getValue<int>(eCharBindGolden));
		return true;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeContributionItemId())
	{
		m_player->ChangeFContri(info.num(),true);
		m_player->SynCharAttribute(eFaContribution,m_player->getAttManage()->getValue<int>(eFaContribution));
		m_player->SynchCharAttributeToWS(eFaContribution, info.num());
		return true;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeCountryContributionItemId())
	{
		m_player->ChangeCountryContribution(info.num(),true);
		return true;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeCreditItemId())
	{
		m_player->ChangeCredit(info.num(),true);
		m_player->SynCharAttribute(eCredit,m_player->getAttManage()->getValue<int>(eCredit));

		m_player->AddPrestige(info.num());
		m_player->SynCharAttribute(ePrestige,m_player->getAttManage()->getValue<int>(ePrestige));

//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//		StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCredit,info.num());
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_player->GetID(),eStatic_Credit,eStaMinor_Credit_Mail,&costAndGotInfo);
		return true;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeVipGoldenId())
	{
		m_player->GetVipPart()->AddUpGoldAndSyn(info.num());
		return true;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeChallengeItem())
	{
		m_player->ChangeChallenge(info.num(), true);
		return true;
	}

	switch(ginfo->type)
	{
	case eWeapon:
	case eArmor:
	case eJewelry:
		{
			Smart_Ptr<EquipProp> tProp;

			tProp = new EquipProp(info.id());
			tProp->SetServerInfo(&info);
			newProp = tProp;

			break;
		}
	case eTime:
		{
			Smart_Ptr<TimeProp> tProp;
			tProp = new TimeProp(info.id(),info.flag() & 0xff);
			tProp->SetServerInfo(&info);
			newProp = tProp;
			break;
		}
	default :
		{
			Smart_Ptr<PropBase> tProp;
			tProp = new PropBase(info.id(),info.flag() & 0xff);
			tProp->SetServerInfo(&info);
			newProp = tProp;
		}
	}

	if(ginfo->stack>1)
	{
		for(; i<m_gridList.size(); ++i)
		{
			if(m_gridList[i]->IsSame(ginfo->itemID, newProp->GetFlag()))
			{//物品一样了，而且data非叠加后余下的物品
				if((m_gridList[i]->GetAmount() + newProp->GetAmount()) <= ginfo->stack)
				{//小于最大堆叠数
					vec.push_back(i);
					m_gridList[i]->SetAmount(m_gridList[i]->GetAmount() + newProp->GetAmount());
					newProp->SetEmpty();
					break;
				}
				else
				{//大于最大堆叠数，还得找个空位
					newProp->SetAmount(m_gridList[i]->GetAmount() + newProp->GetAmount() - ginfo->stack);
					m_gridList[i]->SetAmount(ginfo->stack);
					vec.push_back(i);
				}
			}
		}
	}

	if(newProp->GetAmount() > 0)
	{
		nullGrid = FindNullPos();

		if(nullGrid >= 0)
		{
			m_gridList[nullGrid] = newProp;
			vec.push_back(nullGrid);
		}
	}

	m_player->SetDataFlag(ePackageInfo);

	if (ePackageType == GetType())
	{
		if (0 < vec.size())
		{
			EventArgs args;
			m_player->FireEvent(PLYAER_PACKAGE_CHANGE, args);
		}
	}

	return true;
}

int ArticleBaseMgr::Base_AddSimpleItem(const GoodsInfo &ginfo, vector<int> &vec, int count, WORD flag, int strengthLv)
{
	//可以叠加
	if(ginfo.stack > 1)
	{
		for(size_t i = 0; i<m_gridList.size(); ++i)
		{
			if(m_gridList[i].Get() == NULL)
				continue;

			if(!m_gridList[i]->IsSame(ginfo.itemID,flag))
				continue;

			if(m_gridList[i]->GetAmount() >= ginfo.stack)
				continue;

			//物品一样了，而且data非叠加后余下的物品
			if((m_gridList[i]->GetAmount() + count) <= ginfo.stack)
			{
				BEHAVIOR_HANDLE_DO(m_player->GetMyself(), BehaviorHandType::AddItem(ginfo.itemID, count));

				//小于最大堆叠数
				m_gridList[i]->SetAmount(m_gridList[i]->GetAmount() + count);
				count = 0;
				vec.push_back(i);

				break;
			}
			else
			{
				int add = ginfo.stack - m_gridList[i]->GetAmount();
				BEHAVIOR_HANDLE_DO(m_player->GetMyself(),  BehaviorHandType::AddItem(ginfo.itemID, add));
//
				//大于最大堆叠数，还得找个空位
				count = m_gridList[i]->GetAmount() + count - ginfo.stack;
				m_gridList[i]->SetAmount(ginfo.stack);
				vec.push_back(i);
//
			}
		}
	}

	if(count <= 0)
		return 0;

	while(count > 0)
	{
		int tempPos = FindNullPos();

		if(tempPos == -1)
			return ePackageFull;

		vec.push_back(tempPos);

		Smart_Ptr<PropBase> tempRemain = makePropByType(ginfo.itemID,(eGoodsType)ginfo.type,strengthLv);

		tempRemain->SetFlag(flag);

		if(ginfo.stack > 1)
		{
			//大过于或等于ginfo.stack个
			if(count / ginfo.stack > 0)
			{
				tempRemain->SetAmount(ginfo.stack);
				BEHAVIOR_HANDLE_DO(m_player->GetMyself(), BehaviorHandType::AddItem(ginfo.itemID, ginfo.stack));
			}
			else
			{
				int add = count % ginfo.stack;
				tempRemain->SetAmount(add);
				BEHAVIOR_HANDLE_DO(m_player->GetMyself(), BehaviorHandType::AddItem(ginfo.itemID, add));
			}

			count -= tempRemain->GetAmount();
		}
		else
		{
			tempRemain->SetAmount(1);
			--count;
			BEHAVIOR_HANDLE_DO(m_player->GetMyself(), BehaviorHandType::DelItem(ginfo.itemID, 1));
		}


		m_gridList[tempPos] = tempRemain;
	}

	return 0;
}

int ArticleBaseMgr::Base_AddItem(int id, vector<int> &vec, int count, WORD flag, int strengthLv)
{
	int ret = 0;
	if(count <=0)
		return -2;

	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(id);
	if(ginfo == NULL)
	{
		return -1;
	}

	if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeMoneyItemId())//铜钱
	{
		m_player->MoneyChange(count,true);
		return 0;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeExpItemId())
	{
		m_player->AddExp(count);
		return 0;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeForceItemId())
	{
		m_player->ChangeCurForce(count,true);
		m_player->SynCharAttribute(eCharForce,m_player->getAttManage()->getValue<int>(eCharForce));
		return 0;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeGoldenItemId())
	{
		m_player->ChangeGolden(count,true);
		m_player->SynCharAttribute(eCharGolden,m_player->getAttManage()->getValue<int>(eCharGolden));
		return 0;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeBindGoldenItemId())
	{
		m_player->ChangeBindGolden(count,true);
		m_player->SynCharAttribute(eCharBindGolden,m_player->getAttManage()->getValue<int>(eCharBindGolden));
		return 0;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeContributionItemId())
	{
		m_player->ChangeFContri(count,true);
		m_player->SynCharAttribute(eFaContribution,m_player->getAttManage()->getValue<int>(eFaContribution));
		m_player->SynchCharAttributeToWS(eFaContribution,count);
		return 0;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeCountryContributionItemId())
	{
		m_player->ChangeCountryContribution(count,true);
		return 0;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeCreditItemId())
	{
		m_player->ChangeCredit(count,true);
		m_player->SynCharAttribute(eCredit,m_player->getAttManage()->getValue<int>(eCredit));

		m_player->AddPrestige(count);
		m_player->SynCharAttribute(ePrestige,m_player->getAttManage()->getValue<int>(ePrestige));
		return 0;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeVipGoldenId())
	{
		m_player->GetVipPart()->AddUpGoldAndSyn(count);
		return 0;
	}
	else if(ginfo->itemID == GameParaLoader::GetInstance()->getExchangeChallengeItem())
	{
		m_player->ChangeChallenge(count,true);
		return 0;
	}

	ret = Base_AddSimpleItem(*ginfo,vec,count,flag,strengthLv);

	if(ret != 0)
		return ret;

	m_player->SetDataFlag(ePackageInfo);

	//统计获得 品质为紫色 或 橙色物品 个数
//	if(ginfo->itemQuality == eGoodsQualityPurple || ginfo->itemQuality == eGoodsQualityOrange)
//	{
//		StatisticMgr::GetInstance()->StatisticPlayerGetItems(m_player->GetMyself(), eStatic_Package, ginfo->itemID,count,count * ginfo->price);
//	}

	//响应事件往背包添加物品事件
	if(m_player->GetMyself())
	{
		BuyItemEventArgs e(m_player->GetMyself(),ginfo->itemID,count,0);
		m_player->FireEvent(PLAYER_PACKET_ADDITEM, e);
		if (ePackageType == GetType())
		{
			Smart_Ptr<ArticleParcelMgr> parcelMgr = m_player->GetContainer(ePackageType);
			m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Package, eUIHLSmall_Package_Full), parcelMgr->IsGirdFull());

		}
	}

	return 0;
}

int ArticleBaseMgr::AddItem(const vector<DWORD>& idList,const vector<DWORD> &numList, vector<int>& vecPos,const vector<WORD>& flagList,
		const vector<DWORD> &strengthLv,bool isSend,fightDefined fightType)
{
	if(IsBagEnought(idList,numList,flagList) != 0)
		return ePackageFull;

	int ret = 0;

	for(uint i = 0; i < idList.size(); i++)
	{
		ret = this->Base_AddItem(idList[i], vecPos, numList[i], flagList[i],strengthLv[i]);

		if(ret != 0)
			break;
	}

	if(isSend && vecPos.size() > 0)
	{
		vector<DWORD> tempSendList;;
		for(size_t i = 0;i < vecPos.size();++i)
		{
			tempSendList.push_back(ePackageType << 16 | vecPos[i]);
		}
		m_player->SendSectorChange(tempSendList,fightType);
	}

//	if (vecPos.size() > 0)
//	{
//		Smart_Ptr<CHighLightMgr> HLMgr = m_player->GetActiveHLMgr();
//		if ((bool)HLMgr)
//		{
//			int value = 0;
//			int isCan = CMeterialSynMessage::GetInstance()->GetParcelCanSynth(m_player->GetMyself());
//			HLMgr->GetValueByType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth, value);
//			m_player->GetActiveHLMgr()->GetValueByType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth, value);
//			if (isCan != value)
//			{
//				m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth), isCan);
//			}
//		}
//	}

	if (0 < vecPos.size())
	{
		EventArgs args;
		m_player->FireEvent(PLYAER_PACKAGE_CHANGE, args);
	}

	return ret;
}

void ArticleBaseMgr::InitEquipItem(PlayerInfo::ItemInfo *info){}

void ArticleBaseMgr::InitItem(const PlayerInfo::ItemInfo *info)
{
	Smart_Ptr<PropBase> newProp;
	const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(info->id());
	if(ginfo == NULL)
	{
		return;
	}

	switch(ginfo->type)
	{
	case eWeapon:
	case eArmor:
	case eJewelry:
		{
			Smart_Ptr<EquipProp> tProp;

			tProp = new EquipProp(info->id());
//			tProp->Init(ginfo.inlayNum);
			newProp = tProp;

			break;
		}
	case eTime:
		{
			//是否过期
			int64 cur=(int64)(CUtil::GetNowSecond()*0.001);
			if(info->endtime()<0 || cur>=info->endtime())
			{
				return;
			}
			newProp = new TimeProp(info->id());
			break;
		}
	default :
		{
			newProp = new PropBase(info->id());
		}
	}

	newProp->SetServerInfo(info);
	if(info->num() < (int)m_gridList.size())
	{
		m_gridList[info->num()] = newProp;
	}

	if(m_player->GetMyself())
	{
		BuyItemEventArgs e(m_player->GetMyself(),info->id(),1,0);
		m_player->FireEvent(PLAYER_PACKET_ADDITEM, e);
	}
}

int ArticleBaseMgr::AddItem(DWORD id, DWORD num, vector<int> &vecPos,WORD flag,DWORD strenght,bool isSend,fightDefined fightType)
{
	vector<DWORD> tempIDList;
	vector<DWORD> tempNumList;
	vector<WORD> tempFlagList;
	vector<DWORD> tempStrenghtList;

	tempIDList.push_back(id);
	tempNumList.push_back(num);
	tempFlagList.push_back(flag);
	tempStrenghtList.push_back(strenght);

	return AddItem(tempIDList,tempNumList,vecPos,tempFlagList,tempStrenghtList,isSend,fightType);
}

void ArticleBaseMgr::SaveGrideItemInfo(int gride, Smart_Ptr<PropBase> &propBase)
{
	if(IsValidPos(gride) == false)
		return ;

	m_gridList[gride] = propBase;
}

Smart_Ptr<PropBase> ArticleBaseMgr::NewGoods(Smart_Ptr<PropBase> & propBase)
{
	Smart_Ptr<PropBase> newProp = NULL;
	const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(propBase->GetID());
	if(ginfo==NULL)
		return newProp;

	switch(ginfo->type)
	{
	case eWeapon:
	case eArmor:
	case eJewelry:
	{
		newProp = Base_NewEquipProp(propBase);

		break;
	}
	case eTime:
	{
		newProp = Base_NewTimeProp(propBase);
		break;
	}
	default :
	{
		newProp = Base_NewPropBase(propBase);
	}
	}
	return newProp;
}

Smart_Ptr<PropBase> ArticleBaseMgr::Base_NewEquipProp(Smart_Ptr<EquipProp> equipProp)
{
	Smart_Ptr<EquipProp> newProp = NULL;

	newProp = new EquipProp(equipProp->GetID());
	newProp->SetAmount(equipProp->GetAmount());
	newProp->SetUpgrade(equipProp->GetUpgrade());						//强化
	vector<ReGetAttr> attrAttr;
	equipProp->GetAttrBaptizeList(attrAttr);
	newProp->SetAttrBaptizeList(attrAttr);								//洗练

	newProp->SetEndTime(equipProp->GetEndTime());
	newProp->SetStones(equipProp->GetStones());
	newProp->SetFlag(equipProp->GetFlag());

	return newProp;
}

Smart_Ptr<PropBase> ArticleBaseMgr::Base_NewTimeProp(Smart_Ptr<TimeProp> timeProp)
{
	Smart_Ptr<TimeProp> newProp =	NULL;
	newProp = new TimeProp(timeProp->GetID());
	newProp->SetAmount(timeProp->GetAmount());
	newProp->SetDwTime(timeProp->GetDwTime());
	newProp->SetFlag(timeProp->GetFlag());
	return newProp;
}

Smart_Ptr<PropBase> ArticleBaseMgr::Base_NewPropBase(Smart_Ptr<PropBase> &propBase)
{
	Smart_Ptr<PropBase> newProp = NULL;
	newProp = new PropBase(propBase->GetID());
	newProp->SetAmount(propBase->GetAmount());
	newProp->SetFlag(propBase->GetFlag());
	return newProp;
}

void ArticleBaseMgr::ClearGrideItemInfo(int gride)
{
	if((size_t)gride >= m_gridList.size())
		return;

	Smart_Ptr<PropBase> propBase = m_gridList[gride];
	if(!propBase)
	{
		return;
	}
	propBase->SetAmount(0);
	propBase->SetID(-1);
	m_gridList[gride] = propBase;
}

void ArticleBaseMgr::checkTime()
{
	for(size_t i=0; i<m_gridList.size(); ++i)
	{
		if(IsEmpty(i))
			continue;
		Smart_Ptr<PropBase>& ref = m_gridList[i];
		if(ref.Get()==NULL || ref->GetAmount()<=0 || ref->GetItemType()!=eTime)
			continue;

		int64 nowTime = (int64)(CUtil::GetNowSecond()*0.001);
		Smart_Ptr<TimeProp> & timeProp = (Smart_Ptr<TimeProp>&)(ref);
		if(timeProp->GetDwTime() > 0 && nowTime >= timeProp->GetDwTime())
		{
				ref->SetEmpty();
				m_player->SetDataFlag(ePackageInfo);

				Goods::GTAckToCUpdateGoodsRet info;
				Goods::UpdateGoods * item = info.add_updategoods();
				CharLogin::ClientItemInfo *clientInfo = item->mutable_iteminfo();
				info.set_status(0);
				item->set_container(this->GetType());
				item->set_pos(i);
				m_gridList[i]->SetClientInfo(clientInfo);
				SendAddItem(info);
		}
	}
}

bool ArticleBaseMgr::isTimeOut(int pos)
{
	Smart_Ptr<PropBase> ref = GetItemDataPtr(pos);
	if(ref.Get()==NULL||IsEmpty(pos))
	{
		return true;
	}

	if(ref->GetItemType()!=eTime)
		return false;

	int64 nowTime = (int64)(CUtil::GetNowSecond()*0.001);
	Smart_Ptr<TimeProp> timeProp = ref;
	if(timeProp->GetDwTime() > 0 && nowTime >= timeProp->GetDwTime())
	{
		return true;
	}

	return false;
}

int ArticleBaseMgr::SaveClientItemInfo(PlayerInfo::ItemInfo *info,vector<int> &vec)
{
	WORD i=0;
	int nullGrid = -1;
	Smart_Ptr<PropBase> newProp = NULL;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(info->id());
	if(ginfo == NULL)
	{
		return eItemError;
	}

	switch(ginfo->type)
	{
	case eWeapon:
	case eArmor:
	case eJewelry:
	{
		Smart_Ptr<EquipProp> tProp;
		tProp = new EquipProp(ginfo->itemID);
		newProp = tProp;

		break;
	}
	case eTime:
	{
		newProp = new TimeProp(ginfo->itemID);

		break;
	}
	default:
	{
		newProp = new PropBase(ginfo->itemID);
	}
	}

	newProp->SetServerInfo(info);


	if(ginfo->stack>1)
	{
		for(; i<m_gridList.size(); ++i)
		{
			if(m_gridList[i]->IsSame(ginfo->itemID, newProp->GetFlag()))
			{//物品一样了，而且data非叠加后余下的物品
				if((m_gridList[i]->GetAmount() + newProp->GetAmount()) <= ginfo->stack)
				{//小于最大堆叠数
					vec.push_back(i);
					m_gridList[i]->SetAmount(m_gridList[i]->GetAmount() + newProp->GetAmount());
					newProp->SetEmpty();
					break;
				}
				else
				{//大于最大堆叠数，还得找个空位
					newProp->SetAmount(m_gridList[i]->GetAmount() + newProp->GetAmount() - ginfo->stack);
					m_gridList[i]->SetAmount(ginfo->stack);
					vec.push_back(i);
				}
			}
		}
	}

	if(newProp->GetAmount() > 0)
	{
		nullGrid = FindNullPos();

		if(nullGrid >= 0)
		{
			m_gridList[nullGrid] = newProp;
			vec.push_back(nullGrid);
		}
		else
		{
			return ePackageFull;
		}
	}

	return 0;
}

void ArticleBaseMgr::GetClientItemInfo(int gride, CharLogin::ClientItemInfo *clientItemInfo)
{
	if((size_t)gride >= m_gridList.size())
		return;

	m_gridList[gride]->SetClientInfo(clientItemInfo);
}

void ArticleBaseMgr::SubmitGrideItem(int pos)
{
	if (IsValidPos(pos) == false)
		return ;

	m_gridList[pos]->SubAmount();
	m_player->SetDataFlag(ePackageInfo);

	Goods::GTAckToCUpdateGoodsRet info;
	Goods::UpdateGoods * item = info.add_updategoods();
	CharLogin::ClientItemInfo *clientInfo = item->mutable_iteminfo();

	info.set_status(0);
	item->set_container(this->GetType());
	item->set_pos(pos);
	m_gridList[pos]->SetClientInfo(clientInfo);

	SendAddItem(info);
}

void ArticleBaseMgr::DeleteGrideItem(int pos,int num)
{
	if (IsValidPos(pos) == false)
		return ;

	m_gridList[pos]->SubAmount(num);
	m_player->SetDataFlag(ePackageInfo);

	Goods::GTAckToCUpdateGoodsRet info;
	Goods::UpdateGoods * item = info.add_updategoods();
	CharLogin::ClientItemInfo *clientInfo = item->mutable_iteminfo();

	info.set_status(0);
	item->set_container(this->GetType());
	item->set_pos(pos);
	m_gridList[pos]->SetClientInfo(clientInfo);

	SendAddItem(info);
}

// 寻找指定物品ID的所在位置
int ArticleBaseMgr::FindPosByPropID(int id, int isBind)
{
	vector<int> epos;

	for (size_t i = 0; i< m_gridList.size(); i++)
	{
		Smart_Ptr<PropBase> propBase = m_gridList[i];
		if(!propBase)
		{
			continue;
		}
		if (propBase->GetID() == id)
		{
			if(propBase->GetBindType() == isBind)
			{
				return i;
			}
			epos.push_back(i);
		}
	}

	//如果有对应的物品
	if(epos.size() > 0)
	{
		return epos[0];
	}

	return -1;
}

//得到该物品的所有格子号，返回总数量
int ArticleBaseMgr::FindPosByPropID(int id , vector<int> &vec , bool flag)
{
	int num = 0;

	for (size_t i = 0; i< m_gridList.size(); i++)
	{
		Smart_Ptr<PropBase> propBase = m_gridList[i];
		if(!propBase)
		{
			continue;
		}
		if (propBase->GetID() == id && (propBase->IsBind() == flag))
		{
			num += propBase->GetAmount();
			vec.push_back(i);
		}
	}

	return num;
}

//得到物品数量，通过ID
int ArticleBaseMgr::FindPropCount(int id)
{
		int num = 0;

		for (size_t i = 0; i< m_gridList.size(); i++)
		{
			Smart_Ptr<PropBase> propBase = m_gridList[i];
			if(!propBase)
			{
				continue;
			}
			if (propBase->GetID() == id)
			{
				num += propBase->GetAmount();
			}
		}

		return num;
}

void ArticleBaseMgr::SendAddItem(Goods::GTAckToCUpdateGoodsRet &info)
{
	QuickMessage::GetInstance()->AddSendMessage(m_player->GetID(), m_player->GetChannelID(),&info,MSG_SIM_GT2C_SAVEGOODS);
}

bool ArticleBaseMgr::Synch_AddItem(Smart_Ptr<PropBase> &data, int isFly)
{
	vector<int> tempPosList;
	if(AddItem(data->GetID(),data->GetAmount(),tempPosList,data->GetFlag(),0,false) != 0)
		return false;

	switch((eGoodsType)data->GetItemType())
	{
	case eWeapon:
	case eArmor:
	case eJewelry:
		{
			Smart_Ptr<EquipProp> tempSoure = data;
			vector<ReGetAttr> tempAttList;

			tempSoure->GetAttrBaptizeList(tempAttList);

			for(size_t i = 0;i < tempPosList.size();++i)
			{
				Smart_Ptr<EquipProp> tempTarget = GetItemDataPtr(tempPosList[i]);;
				if(tempTarget.Get() == NULL)
					continue;

				if(tempTarget->GetItemType() != tempSoure->GetItemType())
					continue;

				tempTarget->SetAttrBaptizeList(tempAttList);
				tempTarget->SetUpgrade(tempSoure->GetUpgrade());
				tempTarget->SetStones(tempSoure->GetStones());
			}

			break;
		}
	case eTime:
		{
			Smart_Ptr<TimeProp> tempSoure  = data;

			for(size_t i = 0;i < tempPosList.size();++i)
			{
				Smart_Ptr<TimeProp> tempTarget = GetItemDataPtr(tempPosList[i]);

				if(tempTarget.Get() == NULL)
					continue;

				if(tempSoure->GetItemType() != tempTarget->GetItemType())
					continue;

				tempTarget->SetDwTime(tempSoure->GetDwTime());
			}

			break;
		}
	default:
		{
			break;
		}
	}

	if(tempPosList.size() > 0)
	{
		vector<DWORD> tempSendList;;
		for(size_t i = 0;i < tempPosList.size();++i)
		{
			tempSendList.push_back(ePackageType << 16 | tempPosList[i]);
		}
		m_player->SendSectorChange(tempSendList,isFly);
	}

	m_player->SetDataFlag(ePackageInfo);

	return true;
}

void ArticleBaseMgr::SortBag()
{
	for(size_t i = 1; i < m_gridList.size(); i++)
	{
		for(size_t j = 0; j < i; j++)
		{
			Smart_Ptr<PropBase> Src = m_gridList[i];
			Smart_Ptr<PropBase> Dest = m_gridList[j];
			if(!Src || !Dest)
			{
				continue;
			}
			if(Src->IsValid())
			{
				continue;
			}
			if(Dest->IsValid())
			{
				m_gridList[i] = Dest;
				m_gridList[j] = Src;
			}
			else
			{
				if(int(Src->GetItemType()) < int(Dest->GetItemType()))
				{
					m_gridList[i] = Dest;
					m_gridList[j] = Src;
				}
				else if(int(Src->GetItemType()) == int(Dest->GetItemType()))
				{
					if(Src->GetItemQuality() > Dest->GetItemQuality())
					{
						m_gridList[i] = Dest;
						m_gridList[j] = Src;
					}
					else if(Src->GetItemQuality() == Dest->GetItemQuality())
					{
						if(Src->GetID() < Dest->GetID())
						{
							m_gridList[i] = Dest;
							m_gridList[j] = Src;
						}
					}
				}
			}
		}
	}
}

int ArticleBaseMgr::GetGoodsNum(int goodsId)
{
	int allNum = 0;
	for(DWORD i = 0; i < m_gridList.size(); i++)
	{
		Smart_Ptr<PropBase> propBase = m_gridList[i];
		if(!propBase)
		{
			continue;
		}
		if(propBase->GetID() > 0 && propBase->GetID() == goodsId)
		{
			allNum += propBase->GetAmount();
		}
	}
	return allNum;
}

bool ArticleBaseMgr::HaveGoodsEnought(int goodsId, int num, vector<size_t> &epos)
{
	int allNum = 0;
	for(size_t i = 0; i < m_gridList.size(); i++)
	{
		Smart_Ptr<PropBase> propBase = m_gridList[i];
		if(!propBase)
		{
			continue;
		}

		if(!propBase->IsValid() && propBase->GetID() == goodsId)
		{
			allNum += propBase->GetAmount();

			epos.push_back(i);
		}
	}

	if(allNum >= num)
	{
		return true;
	}
	return false;
}

int ArticleBaseMgr::BaseArgs_DeleteGoodsById(int goodsId, int num,bool isSend)
{
	Goods::GTAckToCUpdateGoodsRet ret;
	vector<size_t> epos;
	int leftNum = num;

	if(HaveGoodsEnought(goodsId, num, epos))
	{
		for(size_t i = 0; i < epos.size(); i++)
		{
			Smart_Ptr<PropBase> propBase = GetItemDataPtr(epos[i]);
			if(propBase->IsValid())
			{
				continue;
			}
			if(propBase->GetID() != goodsId)
			{
				continue;
			}
			if(propBase->GetAmount() >= leftNum)
			{
				BEHAVIOR_HANDLE_DO(m_player->GetMyself(), BehaviorHandType::DelItem(goodsId, leftNum));
//
				propBase->SubAmount(leftNum);
				Goods::UpdateGoods *updateGoods = ret.add_updategoods();
				CharLogin::ClientItemInfo *info = updateGoods->mutable_iteminfo();
				m_player->GetContainer(ePackageType)->GetClientItemInfo(epos[i], info);
				updateGoods->set_container(ePackageType);
				updateGoods->set_pos(epos[i]);
//
				break;
			}
			else
			{
				int delNum = propBase->GetAmount();
				BEHAVIOR_HANDLE_DO(m_player->GetMyself(), BehaviorHandType::DelItem(goodsId, delNum));

				leftNum -= propBase->GetAmount();
				propBase->SubAmount(propBase->GetAmount());
				Goods::UpdateGoods *updateGoods = ret.add_updategoods();
				CharLogin::ClientItemInfo *info = updateGoods->mutable_iteminfo();
				m_player->GetContainer(ePackageType)->GetClientItemInfo(epos[i], info);
				updateGoods->set_container(ePackageType);
				updateGoods->set_pos(epos[i]);
//
			}
		}

		ret.set_status(0);
		m_player->SetDataFlag(ePackageInfo);

		if(isSend)
		{
			SendAddItem(ret);
		}

		if (ePackageType == GetType())
		{
//			Smart_Ptr<ArticleParcelMgr> parcelMgr = m_player->GetContainer(ePackageType);
//			m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Package, eUIHLSmall_Package_Full), parcelMgr->IsGirdFull());
			if (0 < ret.updategoods_size())
			{
				EventArgs args;
				m_player->FireEvent(PLYAER_PACKAGE_CHANGE, args);
			}

			// 神工坊
//			Smart_Ptr<CHighLightMgr> HLMgr = m_player->GetActiveHLMgr();
//			if ((bool)HLMgr)
//			{
//				int value = 0;
//				int isCan = CMeterialSynMessage::GetInstance()->GetParcelCanSynth(m_player->GetMyself());
//				HLMgr->GetValueByType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth, value);
//				m_player->GetActiveHLMgr()->GetValueByType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth, value);
//				if (isCan != value)
//				{
//					m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth), isCan);
//				}
//			}
		}

		return 0;
	}
	else
	{
		return eItemNotEnough;
	}
}
//此方法有多少扣多少（适用于不足元宝代替）返回帮定类型 有一个帮定就返回帮定
bool ArticleBaseMgr::DeleteGoodsById(int goodsId, int num , bool idBind)
{
	Goods::GTAckToCUpdateGoodsRet ret;
	vector<size_t> epos;
	int leftNum = num;
	bool bind = false;

	HaveGoodsEnought(goodsId, num, epos);

	for(size_t i = 0; i < epos.size(); i++)
	{
		Smart_Ptr<PropBase> propBase = GetItemDataPtr(epos[i]);
		if(leftNum == 0 )
		{
			break;
		}
		if(propBase->IsValid())
		{
			continue;
		}
		if(propBase->GetID() != goodsId)
		{
			continue;
		}
		if(propBase->GetBindType() != idBind)
		{
			continue;
		}
		if(propBase->GetAmount() >= leftNum)
		{
			if(bind == false)
				bind = (propBase->GetBindType() == true);

			propBase->SubAmount(leftNum);
			leftNum = 0;
			Goods::UpdateGoods *updateGoods = ret.add_updategoods();
			CharLogin::ClientItemInfo *info = updateGoods->mutable_iteminfo();
			m_player->GetContainer(ePackageType)->GetClientItemInfo(epos[i], info);
			updateGoods->set_container(ePackageType);
			updateGoods->set_pos(epos[i]);
			break;
		}
		else
		{
			if(bind == false)
					bind = (propBase->GetBindType() == true);

			leftNum -= propBase->GetAmount();
			propBase->SubAmount(propBase->GetAmount());
			Goods::UpdateGoods *updateGoods = ret.add_updategoods();
			CharLogin::ClientItemInfo *info = updateGoods->mutable_iteminfo();
			m_player->GetContainer(ePackageType)->GetClientItemInfo(epos[i], info);
			updateGoods->set_container(ePackageType);
			updateGoods->set_pos(epos[i]);
		}
	}

	for(size_t i = 0; i < epos.size(); i++)
	{
		Smart_Ptr<PropBase> propBase = GetItemDataPtr(epos[i]);
		if(leftNum == 0 )
		{
			break;
		}
		if(propBase->IsValid())
		{
			continue;
		}
		if(propBase->GetID() != goodsId)
		{
			continue;
		}
		if(propBase->GetAmount() >= leftNum)
		{
			if(bind == false)
					bind = (propBase->GetBindType() == true);

			propBase->SubAmount(leftNum);
			leftNum = 0;
			Goods::UpdateGoods *updateGoods = ret.add_updategoods();
			CharLogin::ClientItemInfo *info = updateGoods->mutable_iteminfo();
			m_player->GetContainer(ePackageType)->GetClientItemInfo(epos[i], info);
			updateGoods->set_container(ePackageType);
			updateGoods->set_pos(epos[i]);
			break;
		}
		else
		{
			if(bind == false)
					bind = (propBase->GetBindType() == true);

			leftNum -= propBase->GetAmount();
			propBase->SubAmount(propBase->GetAmount());
			Goods::UpdateGoods *updateGoods = ret.add_updategoods();
			CharLogin::ClientItemInfo *info = updateGoods->mutable_iteminfo();
			m_player->GetContainer(ePackageType)->GetClientItemInfo(epos[i], info);
			updateGoods->set_container(ePackageType);
			updateGoods->set_pos(epos[i]);
		}
	}
	ret.set_status(0);
	m_player->SetDataFlag(ePackageInfo);
	SendAddItem(ret);

	if (0 < ret.updategoods_size())
	{
		EventArgs args;
		m_player->FireEvent(PLYAER_PACKAGE_CHANGE, args);

//		// 神工坊
//		Smart_Ptr<CHighLightMgr> HLMgr = m_player->GetActiveHLMgr();
//		if ((bool)HLMgr)
//		{
//			int value = 0;
//			int isCan = CMeterialSynMessage::GetInstance()->GetParcelCanSynth(m_player->GetMyself());
//			HLMgr->GetValueByType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth, value);
//			m_player->GetActiveHLMgr()->GetValueByType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth, value);
//			if (isCan != value)
//			{
//				m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth), isCan);
//			}
//		}

	}

	return bind;
}

int ArticleBaseMgr::BaseArgs_DeleteGoodsByPos(int goodsId, int pos, int num,bool isSend)
{
	Goods::GTAckToCUpdateGoodsRet ret;
	Smart_Ptr<PropBase> propBase;

	if(pos < 0)
	{
		return eBagPosError;
	}
	propBase = m_gridList[pos];
	if(!propBase)
	{
		return eItemError;
	}
	if(propBase->IsValid())
	{
		return eItemError;
	}
	if(propBase->GetID() != goodsId)
	{
		return eItemError;
	}
	if(propBase->GetAmount() < num)
	{
		return eItemNotEnough;
	}
	if(propBase->SubAmount(num))
	{
		BEHAVIOR_HANDLE_DO(m_player->GetMyself(), BehaviorHandType::DelItem(goodsId, num));
//
		Goods::UpdateGoods *updateGoods = ret.add_updategoods();
		CharLogin::ClientItemInfo *info = updateGoods->mutable_iteminfo();

		m_player->GetContainer(ePackageType)->GetClientItemInfo(pos, info);

		updateGoods->set_container(ePackageType);
		updateGoods->set_pos(pos);
		ret.set_status(0);

		if(isSend)
		{
			SendAddItem(ret);
		}

		m_player->SetDataFlag(ePackageInfo);
		if (ePackageType == GetType())
		{
//			Smart_Ptr<ArticleParcelMgr> parcelMgr = m_player->GetContainer(ePackageType);
//			m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Package, eUIHLSmall_Package_Full), parcelMgr->IsGirdFull());
			if (0 < ret.updategoods_size())
			{
				EventArgs args;
				m_player->FireEvent(PLYAER_PACKAGE_CHANGE, args);
			}
//			// 神工坊
//			Smart_Ptr<CHighLightMgr> HLMgr = m_player->GetActiveHLMgr();
//			if ((bool)HLMgr)
//			{
//				int value = 0;
//				int isCan = CMeterialSynMessage::GetInstance()->GetParcelCanSynth(m_player->GetMyself());
//				HLMgr->GetValueByType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth, value);
//				m_player->GetActiveHLMgr()->GetValueByType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth, value);
//				if (isCan != value)
//				{
//					m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth), isCan);
//				}
//			}

		}

	}
	return 0;
}

int ArticleBaseMgr::BaseArgs_DeleteGoodsByPos(vector<BaseArgs>& checkArgs)
{
	Goods::GTAckToCUpdateGoodsRet ret;
	Smart_Ptr<PropBase> propBase;

	int pos = 0;
	int num = 0;
	for (uint i = 0; i < checkArgs.size(); ++i)
	{
		pos = checkArgs[i].Pos;
		num = checkArgs[i].Num;

		if (0 > pos)
		{
			return eBagPosError;
		}
		propBase = m_gridList[pos];
		if (!propBase)
		{
			return eItemError;
		}
		if (propBase->IsValid())
		{
			return eItemError;
		}
		if (propBase->GetAmount() < num)
		{
			return eItemNotEnough;
		}
		if (propBase->SubAmount(num))
		{
			BEHAVIOR_HANDLE_DO(m_player->GetMyself(), BehaviorHandType::DelItem(propBase->GetID(), num));
			Goods::UpdateGoods *updateGoods = ret.add_updategoods();
			CharLogin::ClientItemInfo *info = updateGoods->mutable_iteminfo();

			m_player->GetContainer(ePackageType)->GetClientItemInfo(pos, info);

			updateGoods->set_container(ePackageType);
			updateGoods->set_pos(pos);
		}
	}
	ret.set_status(0);
	SendAddItem(ret);
	m_player->SetDataFlag(ePackageInfo);

	if (ePackageType == GetType())
	{
//		Smart_Ptr<ArticleParcelMgr> parcelMgr = m_player->GetContainer(ePackageType);
//		m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Package, eUIHLSmall_Package_Full), parcelMgr->IsGirdFull());
		if (0 < ret.updategoods_size())
		{
			EventArgs args;
			m_player->FireEvent(PLYAER_PACKAGE_CHANGE, args);
		}

//		// 神工坊
//		Smart_Ptr<CHighLightMgr> HLMgr = m_player->GetActiveHLMgr();
//		if ((bool)HLMgr)
//		{
//			int value = 0;
//			int isCan = CMeterialSynMessage::GetInstance()->GetParcelCanSynth(m_player->GetMyself());
//			HLMgr->GetValueByType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth, value);
//			m_player->GetActiveHLMgr()->GetValueByType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth, value);
//			if (isCan != value)
//			{
//				m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth), isCan);
//			}
//		}
	}



	return 0;
}

int ArticleBaseMgr::BaseArgs_DeleteGoodsByID(vector<BaseArgs>& checkArgs)
{
	Goods::GTAckToCUpdateGoodsRet ret;

	if (0 >= checkArgs.size())
	{
		return eItemError;
	}

	int goodsID = 0, num = 0;

	for (uint i = 0; i < checkArgs.size(); ++i)
	{
		goodsID = checkArgs[i].GoodsID;
		num = checkArgs[i].Num;

		vector<size_t> epos;
		if (HaveGoodsEnought(goodsID, num, epos))
		{
			for (size_t i = 0; i < epos.size(); ++i)
			{
				Smart_Ptr<PropBase> propBase = GetItemDataPtr(epos[i]);
				if(propBase->IsValid())
				{
					continue;
				}
				if(propBase->GetID() != goodsID)
				{
					continue;
				}
				if(propBase->GetAmount() >= num)
				{
					propBase->SubAmount(num);
					BEHAVIOR_HANDLE_DO(m_player->GetMyself(), BehaviorHandType::DelItem(goodsID, num));
					Goods::UpdateGoods *updateGoods = ret.add_updategoods();
					CharLogin::ClientItemInfo *info = updateGoods->mutable_iteminfo();
					m_player->GetContainer(ePackageType)->GetClientItemInfo(epos[i], info);
					updateGoods->set_container(ePackageType);
					updateGoods->set_pos(epos[i]);
					break;
				}
				else
				{
					num -= propBase->GetAmount();
					propBase->SubAmount(propBase->GetAmount());
					BEHAVIOR_HANDLE_DO(m_player->GetMyself(), BehaviorHandType::DelItem(goodsID, propBase->GetAmount()));
					Goods::UpdateGoods *updateGoods = ret.add_updategoods();
					CharLogin::ClientItemInfo *info = updateGoods->mutable_iteminfo();
					m_player->GetContainer(ePackageType)->GetClientItemInfo(epos[i], info);
					updateGoods->set_container(ePackageType);
					updateGoods->set_pos(epos[i]);
				}
			}
		}
	}

	ret.set_status(0);
	m_player->SetDataFlag(ePackageInfo);

	SendAddItem(ret);

	if (ePackageType == GetType())
	{
//		Smart_Ptr<ArticleParcelMgr> parcelMgr = m_player->GetContainer(ePackageType);
//		m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Package, eUIHLSmall_Package_Full), parcelMgr->IsGirdFull());

		if (0 < ret.updategoods_size())
		{
			EventArgs args;
			m_player->FireEvent(PLYAER_PACKAGE_CHANGE, args);
		}

//		// 神工坊
//		Smart_Ptr<CHighLightMgr> HLMgr = m_player->GetActiveHLMgr();
//		if ((bool)HLMgr)
//		{
//			int value = 0;
//			int isCan = CMeterialSynMessage::GetInstance()->GetParcelCanSynth(m_player->GetMyself());
//			HLMgr->GetValueByType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth, value);
//			m_player->GetActiveHLMgr()->GetValueByType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth, value);
//			if (isCan != value)
//			{
//				m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth), isCan);
//			}
//		}
	}

	return 0;
}

int ArticleBaseMgr::DeleteGoods(const BaseArgs *checkArgs,bool isSend)
{
	if(checkArgs==NULL)
		return eItemError;

	if(checkArgs->Pos != -1)
	{
		return BaseArgs_DeleteGoodsByPos(checkArgs->GoodsID, checkArgs->Pos, checkArgs->Num,isSend);
	}
	else
	{
		return BaseArgs_DeleteGoodsById(checkArgs->GoodsID, checkArgs->Num,isSend);
	}
}

int ArticleBaseMgr::DeleteGoods(vector<BaseArgs>& checkArgs, bool byIDOrPos)
{
	if (0 >= checkArgs.size())
	{
		return false;
	}

	if (byIDOrPos)
	{
		return BaseArgs_DeleteGoodsByID(checkArgs);
	}
	else
	{
		return BaseArgs_DeleteGoodsByPos(checkArgs);
	}
}

//寻找一个空位置
int ArticleBaseMgr::FindNullPos()
{
	for(WORD i = 0; i < m_gridList.size(); i++)
	{
		Smart_Ptr<PropBase> propBase = m_gridList[i];
		if(propBase->GetID() == -1)
		{
			return i;
		}
	}
	return -1;
}

int ArticleBaseMgr::GetFreeSize()
{
	int freesize = 0;
	for(DWORD i = 0; i < m_gridList.size(); i++)
	{
		Smart_Ptr<PropBase> propBase = m_gridList[i];
		if(propBase->GetID() <= 0)
		{
			freesize++;
		}
	}

	return freesize;

}

int ArticleBaseMgr::AddBackPackSize(int container, int num, Goods::GTAckToCOpenGoodsRet &toGate)
{
//	Goods::GTAckToCOpenGoodsRet *ret = toGate.mutable_ret();
	if((int)(GetSize() + num) > GetMaxContainerSize())
	{
		toGate.set_status(1);
		toGate.set_container(0);
		toGate.add_pos(0);
	}
	else
	{
		for(int i = 1; i<= num; i++)
		{
			toGate.add_pos(GetSize());

			Smart_Ptr<PropBase> newProp = new PropBase(-1);
			m_gridList.push_back(newProp);
		}

		toGate.set_status(0);
		toGate.set_container(container);

		m_curContainerSize = GetSize();
		m_openFlag = true;
	}

//		toGate.set_charid(m_player->GetID());
		m_player->SetDataFlag(container+5);
		return 0;
}

int ArticleBaseMgr::DiscardEquipOrGoods(int goodsId,int container,Goods::DiscardUpdateGoodsRet * Ret)
{
	int res = 0;
	int gride = FindPosByPropID(goodsId);

	if(!IsValidPos(gride))
	{
		return eSystemError;
	}
	else
	{
			Smart_Ptr<PropBase> propBase = m_gridList[gride];

			const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(goodsId);
			if(ginfo == NULL)
			{
				return eItemError;
			}

			if(ginfo->type == eTask)
			{
				return eCantDiscardTaskGoods;
			}

			//格子号太大了
			if(!propBase)
			{
				return eItemError;
			}
			if(propBase->IsValid())
			{
				return eItemError;
			}
			else
			{
				Ret->set_goodsid(goodsId);
				Ret->set_discardnum(propBase->GetAmount());
				propBase->SetEmpty();
				Goods::UpdateGoods *updateGoods = Ret->mutable_updategoods();
				CharLogin::ClientItemInfo *info = updateGoods->mutable_iteminfo();
				propBase->SetClientInfo(info);
				updateGoods->set_container(container);
				updateGoods->set_pos(gride);

				if(container == ePackageType)
					m_player->SetDataFlag(ePackageInfo);
				else if(container == eEquipQuickType)
					m_player->SetDataFlag(eEquipQuickType);
			}
	}
	Ret->set_status(res);
	return 0;
}

//int ArticleBaseMgr::DiscardGOODS(int container, int gride, Goods::DiscardUpdateGoodsRet &Ret)
//{
//	int res = 0;
//	int goodsId = 0;
//
//	if(!IsValidPos(gride))
//	{
//		return eSystemError;
//	}
//	else
//	{
//		if(container != ePackageType)
//		{
//			return eCantDiscard;
//		}
//		else
//		{
//			Smart_Ptr<PropBase> propBase = m_gridList[gride];
//			goodsId = propBase->GetID();
//
//			const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(propBase->GetID());
//			if(ginfo == NULL)
//			{
//				return eItemError;
//			}
//
//			if(ginfo->type == eTask)
//			{
//				return eCantDiscardTaskGoods;
//			}
//
//			//格子号太大了
//			if(!propBase)
//			{
//				return eItemError;
//			}
//			if(propBase->IsValid())
//			{
//				return eItemError;
//			}
//			else
//			{
//				StatisticInfo::CostAndGotInfo costAndGotInfo;
//				StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
//				StatisticMgr::SetItemCost(costItemInfo,propBase->GetID(),propBase->GetAmount());
//
//				if(eAccountType_User == m_player->GetAccountType())
//					StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_player->GetID(),eStatic_Package, eStaMinor_Package_DiscardItemEvent,&costAndGotInfo);
//
//				Ret.set_goodsid(propBase->GetID());
//				Ret.set_discardnum(propBase->GetAmount());
//				propBase->SetEmpty();
//				Goods::UpdateGoods *updateGoods = Ret.mutable_updategoods();
//				CharLogin::ClientItemInfo *info = updateGoods->mutable_iteminfo();
//				propBase->SetClientInfo(info);
//				updateGoods->set_container(container);
//				updateGoods->set_pos(gride);
//				m_player->SetDataFlag(ePackageInfo);
//
//				//统计丢弃 品质高的物品次数
//				if(ginfo->itemQuality > eGoodsQualityGreen)
//					StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_player->GetMyself(), eStatic_Package, eStaMinor_Package_DiscardItem, 1);
//
//				EventArgs args;
//				m_player->FireEvent(PLYAER_PACKAGE_CHANGE, args);
//			}
//		}
//	}
//	Ret.set_status(res);
//	return 0;
//}


int ArticleBaseMgr::IsCanMoveToHouseHandle(const GoodsInfo *good_ptr)
{
	/*目前没有对 仓库 只作如果是任务 物品所需的 限制*/
	if(!good_ptr)
	{
		return eNull;
	}

	if(good_ptr->type == eTask)
	{
		return eNotCanMoveToHouse;
	}

	return 0;
}

int ArticleBaseMgr::CalCanDoMove(const int &startContainer, const int &endContainer)
{
	if(startContainer < ePackageType || startContainer >= eArticleMax)
	{
		return ePlayerPackageFail;
	}

	if(endContainer < ePackageType || endContainer >= eArticleMax)
	{
		return ePlayerPackageFail;
	}

	//判断不能 移动到装备栏
	if(endContainer == eEquipQuickType || startContainer == eEquipQuickType)
		return eCanPutDragSar;

	return 0;
}

int ArticleBaseMgr::MoveGoods(int type, int startContainer, int startGride, int endContainer, int endGride, vector<DWORD> &vec)
{
	int jugeret = CalCanDoMove(startContainer, endContainer);
	if(jugeret != 0)
	{
		return jugeret;
	}

	Smart_Ptr<PropBase> SrcPropBase = m_player->GetContainer(startContainer)->GetItemDataPtr(startGride);
	Smart_Ptr<PropBase> DestPropBase = m_player->GetContainer(endContainer)->GetItemDataPtr(endGride);
	if(!SrcPropBase || !DestPropBase)
	{
		return eItemError;
	}

	const GoodsInfo* srcInfo = GoodsLoader::GetInstance()->GetItemDataByID(SrcPropBase->GetID());
	if(srcInfo == NULL)
	{
		return eItemError;
	}

	if(endContainer == eStoreHouseType )
	{
		jugeret = this->IsCanMoveToHouseHandle(srcInfo);  //判定要移动仓库中的物品是否是任务物品
		if(jugeret != 0)
		{
			return jugeret;
		}
	}

	if(type == 1)
	{
		if(m_player->GetContainer(endContainer)->IsLock(endGride))
		{
			return eGridNotOpen;
		}
		if(SrcPropBase->IsSame(DestPropBase->GetID(), DestPropBase->GetFlag()))
		{
			if(srcInfo->stack <= 1)
			{
				return eGoodsNotPile;
			}
			MergeGoods(startContainer, startGride, endContainer, endGride, vec);
		}
		else
		{
			ChangeGoods(startContainer, startGride, endContainer, endGride, vec);
		}
	}
	else
	{
		for(size_t i = 0; i < m_player->GetContainer(endContainer)->GetSize(); i++)
		{
			if(m_player->GetContainer(endContainer)->GetItemDataPtr(i)->IsSame(SrcPropBase->GetID(), SrcPropBase->GetFlag()))
			{
				MergeGoods(startContainer, startGride, endContainer, i, vec);
			}
		}
		for(size_t i = 0; i < m_player->GetContainer(endContainer)->GetSize(); i++)
		{
			if(m_player->GetContainer(endContainer)->IsEmpty(i) && !m_player->GetContainer(endContainer)->IsLock(i))
			{
				ChangeGoods(startContainer, startGride, endContainer, i, vec);
			}
		}
		return eContainerFull;
	}

	m_player->SetDataFlag(startContainer+5);
	m_player->SetDataFlag(endContainer+5);

//	Smart_Ptr<ArticleParcelMgr> parcelMgr = m_player->GetContainer(ePackageType);
//	m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Package, eUIHLSmall_Package_Full), parcelMgr->IsGirdFull());

	EventArgs args;
	m_player->FireEvent(PLYAER_PACKAGE_CHANGE, args);

//	if (endContainer == eStoreHouseType || endContainer == ePackageType )
//	{
//		Smart_Ptr<CHighLightMgr> HLMgr = m_player->GetActiveHLMgr();
//		if ((bool)HLMgr)
//		{
//			int isCan = CMeterialSynMessage::GetInstance()->GetParcelCanSynth(m_player->GetMyself());
//			m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth), isCan);
//		}
//	}

	return 0;
}



int ArticleBaseMgr::MoveGoods(int type, int startContainer, int startGride, int endContainer, vector<DWORD> &vec)
{
	int ret = 0;
	bool flag = true;

	int jugeret = CalCanDoMove(startContainer, endContainer);
	if(jugeret != 0)
		return jugeret;

	Smart_Ptr<PropBase> SrcPropBase = m_player->GetContainer(startContainer)->GetItemDataPtr(startGride);
	if(!SrcPropBase)
	{
		return eItemError;
	}

	const GoodsInfo* srcInfo = GoodsLoader::GetInstance()->GetItemDataByID(SrcPropBase->GetID());
	if(srcInfo == NULL)
		return eItemError;

	if(endContainer == eStoreHouseType)
	{
		int jugeret = this->IsCanMoveToHouseHandle(srcInfo);  //判定要移动仓库中的物品是否是任务物品
		if(jugeret != 0)
		{
			return jugeret;
		}
	}

//	StatisticInfo::IDAndCostAndGotInfo idAndCostAndGotInfo;
//	StatisticInfo::CostAndGotInfo * costAndGotInfo = idAndCostAndGotInfo.mutable_costandgotlist();

	ret = CanMoveGoods(startContainer, startGride, endContainer);
	if(ret == 0)
	{
//		idAndCostAndGotInfo.set_beforeid(startContainer);
//		StatisticInfo::CostItemInfo * costInfo = costAndGotInfo->add_costlist();
//		int propID = SrcPropBase->GetID();
//		int amount = SrcPropBase->GetAmount();
//		StatisticMgr::SetItemCost(costInfo,propID,amount);

		for(size_t i = 0; i < m_player->GetContainer(endContainer)->GetSize(); i++)
		{
			if(m_player->GetContainer(endContainer)->GetItemDataPtr(i)->IsSame(SrcPropBase->GetID(), SrcPropBase->GetFlag()))
			{
				int ret = MergeGoods(startContainer, startGride, endContainer, i, vec);
				if(0 == ret)
				{
					//成功了
//					idAndCostAndGotInfo.set_afterid(endContainer);
//					StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo->add_gotlist();
//					StatisticMgr::AddItemGotInfo(gotInfo,propID,amount);
				}
				flag = false;
				break;
			}
		}

		if(flag)
		{
			for(size_t i = 0; i < m_player->GetContainer(endContainer)->GetSize(); i++)
			{
				if(m_player->GetContainer(endContainer)->IsEmpty(i) && !m_player->GetContainer(endContainer)->IsLock(i))
				{
					Smart_Ptr<PropBase> NewSrcPropBase = m_player->GetContainer(endContainer)->NewGoods(SrcPropBase);

					m_player->GetContainer(endContainer)->SaveGrideItemInfo(i, NewSrcPropBase);
					m_player->GetContainer(startContainer)->ClearGrideItemInfo(startGride);

					vec.push_back((startContainer << 16) | startGride);
					vec.push_back((endContainer << 16) | i);

//					idAndCostAndGotInfo.set_afterid(endContainer);
//					StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo->add_gotlist();
//					StatisticMgr::AddItemGotInfo(gotInfo,NewSrcPropBase->GetID(),NewSrcPropBase->GetAmount());

					break;
				}
			}
		}

		m_player->SetDataFlag(startContainer+5);
		m_player->SetDataFlag(endContainer+5);

//		if(eAccountType_User == m_player->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_player->GetID(),eStatic_Package,eStaMinor_Package_Move,&idAndCostAndGotInfo);

//		Smart_Ptr<ArticleParcelMgr> parcelMgr = m_player->GetContainer(ePackageType);
//		m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Package, eUIHLSmall_Package_Full), parcelMgr->IsGirdFull());

		EventArgs args;
		m_player->FireEvent(PLYAER_PACKAGE_CHANGE, args);

//		if (endContainer == eStoreHouseType || endContainer == ePackageType )
//		{
//			Smart_Ptr<CHighLightMgr> HLMgr = m_player->GetActiveHLMgr();
//			if ((bool)HLMgr)
//			{
//				int isCan = CMeterialSynMessage::GetInstance()->GetParcelCanSynth(m_player->GetMyself());
//				m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth), isCan);
//			}
//		}
	}

	return ret;
}


int ArticleBaseMgr::CanMoveGoods(int startContainer, int startGride, int endContainer)
{
	int totalAmont = 0;
	int ret = eContainerFull;

	Smart_Ptr<PropBase> SrcPropBase = m_player->GetContainer(startContainer)->GetItemDataPtr(startGride);
	if(!SrcPropBase)
	{
		return eItemError;
	}

	totalAmont = SrcPropBase->GetAmount();

	for(size_t i = 0; i < m_player->GetContainer(endContainer)->GetSize(); i++)
	{
		Smart_Ptr<PropBase> DestPropBase = m_player->GetContainer(endContainer)->GetItemDataPtr(i);

		if(!DestPropBase)
		{
			return eItemError;
		}

		if(m_player->GetContainer(endContainer)->GetItemDataPtr(i)->IsSame(SrcPropBase->GetID(), SrcPropBase->GetFlag()))
		{
			int LeftAmount = DestPropBase->GetLeftPileNum();
			if(totalAmont <= LeftAmount)
			{
				break;
			}
			else
			{
				totalAmont -= LeftAmount;
				if(totalAmont <= 0)
				{
					break;
				}
			}
		}
	}

	if(totalAmont > 0)
	{
		for(size_t i = 0; i < m_player->GetContainer(endContainer)->GetSize(); i++)
		{
			if(m_player->GetContainer(endContainer)->IsEmpty(i) && !m_player->GetContainer(endContainer)->IsLock(i))
			{
				ret = 0;
				break;
			}
		}
	}

	return ret;
}


int ArticleBaseMgr::ChangeGoods(int startContainer, int startGride, int endContainer, int endGride, vector<DWORD> &vec)
{
	Smart_Ptr<PropBase> SrcPropBase = m_player->GetContainer(startContainer)->GetItemDataPtr(startGride);
	Smart_Ptr<PropBase> DestPropBase = m_player->GetContainer(endContainer)->GetItemDataPtr(endGride);
	if(!SrcPropBase || !DestPropBase)
	{
		return eItemError;
	}

	m_player->GetContainer(startContainer)->SaveGrideItemInfo(startGride, DestPropBase);
	m_player->GetContainer(endContainer)->SaveGrideItemInfo(endGride, SrcPropBase);

	vec.push_back((startContainer << 16) | startGride);
	vec.push_back((endContainer << 16) | endGride);

	return 0;
}

int ArticleBaseMgr::MergeGoods(int startContainer, int startGride, int endContainer, int endGride, vector<DWORD> &vec)
{
	Smart_Ptr<PropBase> SrcPropBase = m_player->GetContainer(startContainer)->GetItemDataPtr(startGride);
	Smart_Ptr<PropBase> DestPropBase = m_player->GetContainer(endContainer)->GetItemDataPtr(endGride);
	if(!SrcPropBase || !DestPropBase)
	{
		return eItemError;
	}

	if(startContainer == endContainer)
	{
		int LeftAmount = DestPropBase->GetLeftPileNum();
		if(LeftAmount == 0)
		{
			return eItemCountNull;
		}
		if(SrcPropBase->GetAmount() >= LeftAmount)
		{
			DestPropBase->SetAmount(DestPropBase->GetMaxPileNum());
			SrcPropBase->SubAmount(LeftAmount);
			m_player->GetContainer(endContainer)->SaveGrideItemInfo(endGride, DestPropBase);
			m_player->GetContainer(startContainer)->SaveGrideItemInfo(startGride, SrcPropBase);
		}
		else
		{
			DestPropBase->SetAmount(SrcPropBase->GetAmount() + DestPropBase->GetAmount());
			m_player->GetContainer(endContainer)->SaveGrideItemInfo(endGride, DestPropBase);
			m_player->GetContainer(startContainer)->ClearGrideItemInfo(startGride);
		}
	}
	else
	{
		int LeftAmount = DestPropBase->GetLeftPileNum();
		if(SrcPropBase->GetAmount() <= LeftAmount)
		{
			DestPropBase->SetAmount(SrcPropBase->GetAmount() + DestPropBase->GetAmount());
			m_player->GetContainer(endContainer)->SaveGrideItemInfo(endGride, DestPropBase);
			m_player->GetContainer(startContainer)->ClearGrideItemInfo(startGride);
		}
		else
		{
			size_t i;
			DestPropBase->SetAmount(DestPropBase->GetMaxPileNum());
			m_player->GetContainer(endContainer)->SaveGrideItemInfo(endGride, DestPropBase);
			SrcPropBase->SubAmount(LeftAmount);
			for(i = 0; i < m_player->GetContainer(endContainer)->GetSize(); i++)
			{
				if(!m_player->GetContainer(endContainer)->IsEmpty(i) && !m_player->GetContainer(endContainer)->IsLock(i))
				{
					if(SrcPropBase->GetAmount() == 0)
					{
						break;
					}
					Smart_Ptr<PropBase> FindPropBase = m_player->GetContainer(endContainer)->GetItemDataPtr(i);
					if(FindPropBase->IsSame(SrcPropBase->GetID(), SrcPropBase->GetFlag()))
					{
						if(FindPropBase->GetAmount() >= FindPropBase->GetMaxPileNum())
						{
							continue;
						}
						int TempAmount = FindPropBase->GetLeftPileNum();
						if(SrcPropBase->GetAmount() <= TempAmount)
						{
							FindPropBase->SetAmount(SrcPropBase->GetAmount() + FindPropBase->GetAmount());
							m_player->GetContainer(endContainer)->SaveGrideItemInfo(i, FindPropBase);
							m_player->GetContainer(startContainer)->ClearGrideItemInfo(startGride);
							vec.push_back((endContainer << 16) | i);
							break;
						}
						else
						{
							FindPropBase->SetAmount(FindPropBase->GetMaxPileNum());
							m_player->GetContainer(endContainer)->SaveGrideItemInfo(i, FindPropBase);
							SrcPropBase->SubAmount(TempAmount);
							vec.push_back((endContainer << 16) | i);
							}
						}
					}
				}
				if(SrcPropBase->GetAmount() > 0)
				{
					for(i = 0; i < m_player->GetContainer(endContainer)->GetSize(); i++)
					{
						if(m_player->GetContainer(endContainer)->IsEmpty(i) && !m_player->GetContainer(endContainer)->IsLock(i))
						{
							Smart_Ptr<PropBase> NewSrcPropBase = m_player->GetContainer(endContainer)->NewGoods(SrcPropBase);
							m_player->GetContainer(endContainer)->SaveGrideItemInfo(i, NewSrcPropBase);
							m_player->GetContainer(startContainer)->ClearGrideItemInfo(startGride);
							vec.push_back((endContainer << 16) | i);
							break;
						}
					}
					if(i == m_player->GetContainer(endContainer)->GetSize())
					{
						DestPropBase->SetAmount(DestPropBase->GetMaxPileNum());
						m_player->GetContainer(endContainer)->SaveGrideItemInfo(endGride, DestPropBase);
						SrcPropBase->SubAmount(LeftAmount);
						m_player->GetContainer(startContainer)->SaveGrideItemInfo(startGride, SrcPropBase);
					}
				}
			}
		}
		vec.push_back((startContainer << 16) | startGride);
		vec.push_back((endContainer << 16) | endGride);
		return 0;
}

int ArticleBaseMgr::ArrangeContainer(int type)
{
	vector<DWORD> vec;
	SortBag();
	for(size_t i = 0; i < GetSize(); i++)
	{
		for(size_t j = i+1; j < GetSize(); j++)
		{
			Smart_Ptr<PropBase> SrcPropBase = GetItemDataPtr(j);
			Smart_Ptr<PropBase> DestPropBase = GetItemDataPtr(i);
			if(!SrcPropBase || !DestPropBase)
			{
				return eItemError;
			}
			if(DestPropBase->IsValid() && SrcPropBase->IsValid())
			{
				continue;
			}
			else
			{
				if(SrcPropBase->IsSame(DestPropBase->GetID(), DestPropBase->GetFlag()))
				{
					if(DestPropBase->GetLeftPileNum() <= 0)
					{
						break;
					}
					else
					{
						int status = MergeGoods(type, j, type, i, vec);
						if(status)
						{
							break;
						}
					}
				}
				else if(DestPropBase->IsValid())
				{
					ChangeGoods(type, j, type, i, vec);
				}
				else
				{
					continue;
				}
			}
		}
	}
	m_player->SetDataFlag(type+5);
	return 0;
}

int ArticleBaseMgr::UseGoods(BaseArgs *checkArgs)
{
	if(m_player->IsDead())
	{
		return eCreatureDead;
	}

	if(checkArgs==NULL)
	{
		return eItemError;
	}

	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(checkArgs->GoodsID);
	if(ginfo==NULL)
	{
		return eItemError;
	}

	if(ginfo->type != eConsume && ginfo->type != eTask)
	{
		return eItemCantUseInBag;
	}

	int res = CheckGoods(checkArgs, *ginfo);
	vector<int> vecType;

	if(!res)
	{
		res = DeleteGoods(checkArgs);
		if(0 == res)
		{
			switch(ginfo->effect)
			{
			case eBindGold:
			{
				USE_GOODS(use_glod, m_player, checkArgs);
				vecType.push_back(eCharMoney);
				vecType.push_back(eCharGolden);
				vecType.push_back(eCharBindGolden);
				break;
			}
			case eDoubleMry:
			{
				USE_GOODS(use_double_mry, m_player, checkArgs);
				break;
			}
			case eCottageProp:
			{
				USE_GOODS(use_cottage_prop, m_player, checkArgs);
				break;
			}
			case eSendProp:
			{
				USE_GOODS(use_send_prop, m_player, checkArgs);
				break;
			}
			case eMegaphoneProp:
			{
				USE_GOODS(use_megaphone_prop, m_player, checkArgs);
				break;
			}
			case eStrengthenProp:
			{
				USE_GOODS(use_strengthen_prop, m_player, checkArgs);
				break;
			}
			case eSuccinctProp:
			{
				USE_GOODS(use_succinct_prop, m_player, checkArgs);
				break;
			}
			case eProtectProp:
			{
				USE_GOODS(use_protect_prop, m_player, checkArgs);
				break;
			}
			case ePrestigeProp:
			{
				USE_GOODS(use_prestige_prop, m_player, checkArgs);
				break;
			}
			case eDoubleExp:
			case eBuffProp:
			{
				USE_GOODS(use_buff_prop, m_player, checkArgs);
				Smart_Ptr<ArticleEquipMgr> equipMgr = m_player->GetContainer(eEquipQuickType);
				if(equipMgr)
				{
					equipMgr->UpdateQuickSectorById(checkArgs->GoodsID, REMOVE_QUICK_SECTOR);
				}
				break;
			}
			case eHorseProp:
			{
				USE_GOODS(use_horse_prop, m_player, checkArgs);
				break;
			}
			case eGiftProp:
			{
				USE_GOODS(use_gift_prop, m_player, checkArgs);
				break;
			}
			case eTaskProp:
			{
				USE_GOODS(use_task_prop, m_player, checkArgs);
				break;
			}
			case eDecreaseSinProp:
			{
				USE_GOODS(use_decrease_sin_prop, m_player, checkArgs);
				break;
			}
			case	eGirlProp://	19	获得战姬道具
			{
				USE_GOODS(use_girl_prop, m_player, checkArgs);
				break;
			}
			case eHorseSkinProp://	20	获得坐骑皮肤道具
			{
				USE_GOODS(use_skin_prop, m_player, checkArgs);
				break;
			}
			case eGiftSProp:// 21 概率礼包类
			{
				USE_GOODS(use_gifts_prop, m_player, checkArgs);
				break;
			}
//			case eFashions://	22	时装
//			{
//				USE_GOODS(use_fashions_prop, m_player, checkArgs);
//				break;
//			}
//			case eQuantityProp://	23	时装碎片
//			{
//				USE_GOODS(use_quantity_prop, m_player, checkArgs);
//				break;
//			}
			case eVipCardProp://Vip体验卡
			{
				USE_GOODS(use_vip_card_prop, m_player, checkArgs);
				break;
			}
			case eBoxProp://天降宝箱
			{
				USE_GOODS(use_box_prop, m_player, checkArgs);
				break;
			}
			case eCountryBossBox://阵营BOSS战宝箱
			{
				USE_GOODS(use_countryBossBox_prop, m_player, checkArgs);
				break;
			}
			case eTitleProp://称号物品
			{
				USE_GOODS(use_title_prop, m_player, checkArgs);
				break;
			}
			default:
			{
				break;
			}
			}
			m_player->SynCharAttribute(vecType);
		}
		else
		{
			return res;
		}
	}
	return res;
}

int ArticleBaseMgr::CheckGoods(BaseArgs *checkArgs, const GoodsInfo &ginfo)
{
	int goodsID = checkArgs->GoodsID;
	if(ginfo.itemID <= 0)
	{
		LOG_ERROR(FILEINFO, "use goods item[id = %d] is error", goodsID);
		return eItemError;
	}
	if(m_player->getAttManage()->getValue<int>(eCharLv) < ginfo.useLvReq)
	{
		return eLVNotEnough;
	}
	int res = 0;
	switch(ginfo.effect)
	{
	case eBindGold:
	{
		res = CHECK_USE_GOODS(check_glod, m_player, checkArgs);
		break;
	}
	case eDoubleExp:
	{
		res = CHECK_USE_GOODS(check_double_exp, m_player, checkArgs);
		break;
	}
	case eDoubleMry:
	{
		res = CHECK_USE_GOODS(check_double_mry, m_player, checkArgs);
		break;
	}
	case eCottageProp:
	{
		res = CHECK_USE_GOODS(check_cottage_prop, m_player, checkArgs);
		break;
	}
//	case eSendProp:
//	{
//		res = CHECK_USE_GOODS(check_send_prop, m_player, checkArgs);
//		break;
//	}
//	case eMegaphoneProp:
//	{
//		res = CHECK_USE_GOODS(check_megaphone_prop, m_player, checkArgs);
//		break;
//	}
//	case eStrengthenProp:
//	{
//		res = CHECK_USE_GOODS(check_strengthen_prop, m_player, checkArgs);
//		break;
//	}
//	case eSuccinctProp:
//	{
//		res = CHECK_USE_GOODS(check_succinct_prop, m_player, checkArgs);
//		break;
//	}
//	case eProtectProp:
//	{
//		res = CHECK_USE_GOODS(check_protect_prop, m_player, checkArgs);
//		break;
//	}
	case ePrestigeProp:
	{
		res = CHECK_USE_GOODS(check_prestige_prop, m_player, checkArgs);
		break;
	}
	case eBuffProp:
	{
		res = CHECK_USE_GOODS(check_buff_prop, m_player, checkArgs);
		break;
	}
	case eHorseProp:
	{
		res = CHECK_USE_GOODS(check_horse_prop, m_player, checkArgs);
		break;
	}
	case eGiftProp:
	{
		res = CHECK_USE_GOODS(check_gift_prop, m_player, checkArgs);
		break;
	}
	case eTaskProp:
	{
		res = CHECK_USE_GOODS(check_task_prop, m_player, checkArgs);
		break;
	}
	case eGirlProp://	19	获得战姬道具
	{
		res = CHECK_USE_GOODS(check_girl_prop, m_player, checkArgs);
		break;
	}
	case eHorseSkinProp://	20	获得坐骑皮肤道具
	{
		res = CHECK_USE_GOODS(check_skin_prop, m_player, checkArgs);
		break;
	}
	case eGiftSProp:// 21 概率礼包类
	{
		res = CHECK_USE_GOODS(check_gifts_prop, m_player, checkArgs);
		break;
	}
//	case eFashions://	22	时装
//	{
//		res = CHECK_USE_GOODS(check_fashions_prop, m_player, checkArgs);
//		break;
//	}
//	case eQuantityProp://	23	时装碎片
//	{
//		res = CHECK_USE_GOODS(check_quantity_prop, m_player, checkArgs);
//		break;
//	}
	case eDecreaseSinProp:
	{
		res = CHECK_USE_GOODS(check_sin_prop, m_player, checkArgs);
		break;
	}
	case eExpProp:
	{
		res = CHECK_USE_GOODS(use_exp_prop, m_player, checkArgs);
		break;
	}
	case eForceProp:
	{
		res = CHECK_USE_GOODS(use_force_prop, m_player, checkArgs);
		break;
	}
	case eGoldenProp:
	{
		res = CHECK_USE_GOODS(use_golden_prop, m_player, checkArgs);
		break;
	}
	case eVipCardProp:
	{
		res = CHECK_USE_GOODS(check_vip_card_prop, m_player, checkArgs);
		break;
	}
	case eBoxProp:
	{
		res = CHECK_USE_GOODS(check_box_prop, m_player, checkArgs);
		break;
	}
	case eCountryBossBox:// 30
	{
		res = CHECK_USE_GOODS(check_countryBossBox_prop, m_player, checkArgs);
		break;
	}
	case eTitleProp:
	{
		res = CHECK_USE_GOODS(check_title_prop, m_player, checkArgs);
		break;
	}
	default:
	{
		res = eGoodsTypeError;
	}
	}
	return res;
}


int ArticleBaseMgr::SplitGoods(int container, int gride, int num, vector<DWORD> &vec)
{
	int sparePos = m_player->GetContainer(container)->FindNullPos();
	if(sparePos >= 0)
	{
		Smart_Ptr<PropBase> SrcPropBase = m_player->GetContainer(container)->GetItemDataPtr(gride);
		Smart_Ptr<PropBase> DestPropBase = NULL;
		if(!SrcPropBase)
		{
			return eItemError;
		}
		if(SrcPropBase->IsValid())
		{
			return eItemError;
		}
		if(SrcPropBase->GetAmount() <= num)
		{
			//拆分超出数量
			return eSplitItemTooMuch;
		}
		DestPropBase = NewGoods(SrcPropBase);
		if(!DestPropBase)
		{
			return eItemError;
		}
		m_player->GetContainer(container)->SaveGrideItemInfo(sparePos, DestPropBase);
		if(num > 0)
		{
			SrcPropBase->SubAmount(num);
			DestPropBase->SubAmount(DestPropBase->GetAmount() - num);
		}
		else
		{
			int div = SrcPropBase->GetAmount() / 2;
			int yu = SrcPropBase->GetAmount() % 2;
			SrcPropBase->SubAmount(div);
			DestPropBase->SubAmount(div+yu);
		}

		vec.push_back((container << 16) | gride);
		vec.push_back((container << 16) | sparePos);
		m_player->SetDataFlag(container+5);
		return 0;
	}
	else
	{
		return eBagEmpty;
	}
}

Smart_Ptr<PropBase> ArticleBaseMgr::GetItemDataPtrByID(int &id)
{
	Smart_Ptr<PropBase> nbind;
	int index = 0;

	vector<Smart_Ptr<PropBase> >::iterator it = m_gridList.begin();
	for(; it!=m_gridList.end(); ++it)
	{
		index++;
		if(id == (*it)->GetID())
		{
			if((*it)->IsBind())
			{
				id = index - 1;
				return (*it);
			}

			if(!nbind)
			{
				id = index - 1;
				nbind = *it;
			}
		}
	}

	return nbind;
}

int ArticleBaseMgr::IsBagEnought(const vector<DWORD>& idList,const vector<DWORD>& numList,const vector<WORD>& flagList)
{
	if(idList.size() == 0)
		return 0;

	if(idList.size() != numList.size())
		return eRewardItemError;

	if(numList.size() != flagList.size())
		return eRewardItemError;

	vector<DWORD> newIdList;
	vector<DWORD> newNumList;
	vector<WORD> newflagList;
	size_t i = 0;
	size_t t = 0;

	for(i = 0; i < idList.size(); i++)
	{
		//铜钱 经验 真气 元宝 帮定元宝不占格子
		if(idList[i] == (DWORD)GameParaLoader::GetInstance()->getExchangeMoneyItemId() ||
			idList[i] == (DWORD)GameParaLoader::GetInstance()->getExchangeExpItemId() ||
			idList[i] == (DWORD)GameParaLoader::GetInstance()->getExchangeForceItemId() ||
			idList[i] == (DWORD)GameParaLoader::GetInstance()->getExchangeGoldenItemId() ||
			idList[i] == (DWORD)GameParaLoader::GetInstance()->getExchangeBindGoldenItemId() ||
			idList[i] == (DWORD)GameParaLoader::GetInstance()->getExchangeContributionItemId() ||
			idList[i] == (DWORD)GameParaLoader::GetInstance()->getExchangeCountryContributionItemId() ||
			idList[i] == (DWORD)GameParaLoader::GetInstance()->getExchangeCreditItemId() ||
			idList[i] == (DWORD)GameParaLoader::GetInstance()->getExchangeVipGoldenId() ||
			idList[i] == (DWORD)GameParaLoader::GetInstance()->getExchangeChallengeItem())
		{
			continue;
		}

		if(newIdList.size() <= 0)
		{
			newIdList.push_back(idList[i]);
			newNumList.push_back(numList[i]);
			newflagList.push_back(flagList[i]);
		}
		else
		{
			bool put = true;
			for(t = 0; t < newIdList.size(); t++)
			{
				if(idList[i] == newIdList[t])
				{
					newNumList[t] = newNumList[t] + numList[i];
					put = false;
					break;
				}
			}
			if(put)
			{
				newIdList.push_back(idList[i]);
				newNumList.push_back(numList[i]);
				newflagList.push_back(flagList[i]);
			}
		}
	}


	int freeSize = GetFreeSize();
	int leftPixNum = 0;
	int tempPixNum = 0;

	for(i = 0; i < newIdList.size(); i++)
	{
		if(newIdList[i] <= 0)
		{
			continue;
		}
		tempPixNum = newNumList[i];
		for(t = 0; t < m_gridList.size(); t++)
		{
			Smart_Ptr<PropBase> propBase = m_gridList[t];
			if(propBase.Get() == NULL)
				continue;

			if(propBase->GetID() <= 0)
				continue;

			if(!propBase->IsSame(newIdList[i],newflagList[i]))
				continue;

			leftPixNum = propBase->GetLeftPileNum();
			if(leftPixNum <= 0)
				continue;

			tempPixNum = tempPixNum - leftPixNum;
			if(tempPixNum <= 0)
			{
				break;
			}

		}

		const GoodsInfo* tempInfo = GoodsLoader::GetInstance()->GetItemDataByID(newIdList[i]);
		if(tempInfo == NULL)
			return eItemError;

		if(tempPixNum > 0)
		{
			if(tempInfo->stack <= 1)
			{
				freeSize -= tempPixNum;
			}
			else
			{
				freeSize -= tempPixNum / tempInfo->stack;
				if(tempPixNum % tempInfo->stack != 0)
					--freeSize;
			}
		}

		if(freeSize < 0)
		{
			return ePackageFull;
		}
	}

	return 0;
}

int ArticleBaseMgr::IsBagEnought(DWORD id, DWORD num,WORD flag)
{
	vector<DWORD> tempIDList;
	vector<DWORD> tempNumList;
	vector<WORD>  tempFlagList;

	tempIDList.push_back(id);
	tempNumList.push_back(num);
	tempFlagList.push_back(flag);

	return IsBagEnought(tempIDList,tempNumList,tempFlagList);
}

int ArticleBaseMgr::ChangeAttr_FromEquip(const int &pos, const int &goodID, bool isAdd){ return 0;}

void ArticleBaseMgr::ChangePlayerAttr(int type, int value, bool isAdd){}

int ArticleBaseMgr::CheckWearEquip(int goodsId, GoodsInfo &ginfo){return 0;}

int ArticleBaseMgr::WearEquip(int type, int gride, int goodsId){return 0;}

void ArticleBaseMgr::SetOpenTime(int64 time)
{
	m_openTime = time;
}
int64 ArticleBaseMgr::GetOpenTimeStamp()
{
	int64 time = 0;
	time = m_openTime - CUtil::GetNowSecond();
	time = time>0?time:0;

	return time;
}






