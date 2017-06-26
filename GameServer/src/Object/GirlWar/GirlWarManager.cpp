/*
 * GirlWarManager.cpp
 *
 *  Created on: 2014年12月6日
 *      Author: root
 */

#include "GirlWarManager.h"
#include "Log/Logger.h"
#include "../ObjectFactory.h"
#include "../FileLoader/GirlWarLoader.h"
#include "MessageCommonRet.h"
#include "../../QuickMessage.h"
#include <cmath>
#include "../../Container/ContainerBase.h"
#include "../../ServerEventDefine.h"
#include "../../Chat/ChatMgr.h"
#include "../../Attribute/AttributeCreature.h"
#include "../../Attribute/AttributeManage.h"
#include "../../Fighting/Fighting.h"
#include "./FileLoader/GameParaLoader.h"
#include "../FileLoader/MallLoader.h"


GirlWarManager::GirlWarManager(Player * obj):m_owner(obj)
{
	Init();
}

GirlWarManager::~GirlWarManager()
{
	//m_collection.clear();
	ReleaseGirlWar();
}

void GirlWarManager::Init()
{
	if(!m_enterEvent.IsValid())
		m_enterEvent = RegEvent(m_owner,PLAYER_ENTER_MAP_EVENT_TYPE, this, &GirlWarManager::EnterPetData);

	if(!m_logoutEvent.IsValid())
		m_logoutEvent = RegEvent(m_owner,PLAYER_LEAVE_MAP_EVENT_TYPE, this,&GirlWarManager::LeavePetData);

	if(!m_outEvent.IsValid())
		m_outEvent = RegEvent(m_owner,PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, this, &GirlWarManager::HandlePlayerOut);

	//if(!m_lvEvent.IsValid())
	//	m_lvEvent = RegEvent(m_owner, PLAYER_LEVEL_UP_EVENT_TYPE, this, &GirlWarManager::HandleCanLevelReward);//是否 到了 可领取奖励

	//if(!m_signEvent.IsValid())
	//	m_signEvent = RegEvent(m_owner, PLAYER_SIGNIN_EVENT_TYPE, this, &GirlWarManager::HandleCanSignin);//是否可以 领取 签到奖励

	//if(!m_addItemEvent.IsValid())
	//	m_addItemEvent = RegEvent(m_owner, PLAYER_PACKET_ADDITEM, this, &GirlWarManager::HandleAddItem);//是否可以 领取物品奖励

	//if(!m_instancePassEvent.IsValid())
	//	m_instancePassEvent = RegEvent(m_owner, PLAYER_PASS_INSTANCE, this, &GirlWarManager::HandleInstancePass); // 通关副本

	//if(!m_guardPassNodeEvent.IsValid())
	//	m_guardPassNodeEvent = RegEvent(m_owner, PLAYER_PROTECTKING_PASSMAXNODE, this, &GirlWarManager::HandleGuardMaxNode);

	m_collection.clear();
	Smart_Ptr<GirlWar> girlWar;
	SetGirlWar(girlWar);
}

void GirlWarManager::ReInit()
{
	Init();
}

void GirlWarManager::ReleaseGirlWar()
{
	if(m_enterEvent.IsValid())
	{
		m_enterEvent.SetNull();
	}

	if(m_logoutEvent.IsValid())
	{
		m_logoutEvent.SetNull();
	}


	if(m_outEvent.IsValid())
	{
		m_outEvent.SetNull();
	}

	if(m_lvEvent.IsValid())
	{
		m_lvEvent.SetNull();
	}
	if(m_signEvent.IsValid())
	{
		m_signEvent.SetNull();
	}
	if(m_vipEvent.IsValid())
	{
		m_vipEvent.SetNull();
	}
	if(m_addItemEvent.IsValid())
	{
		m_addItemEvent.SetNull();
	}

	if(m_endTimer.IsValid())
	{
		m_endTimer.SetNull();
	}

	Smart_Ptr<GirlWar> obj;

	map<int64,Smart_Ptr<GirlWar> >::iterator itr = m_collection.begin();
	for(; itr != m_collection.end(); ++itr)
	{
		if(itr->second.Get()==NULL)
			continue;

		//回收对象
		//itr->second->Realese();

		itr->second->SetOwnerToBrain(obj);
		Smart_Ptr<Player> player;
		itr->second->SetOwner(player);
	}

	m_collection.clear();

	SetGirlWar(obj);
}

bool GirlWarManager::EnterPetData(const EventArgs & e)
{
	PlayerMapEventArgs *pArgs = (PlayerMapEventArgs *) (&e);

	Smart_Ptr<Player> player = pArgs->m_obj;
	if(player->GetID() == m_owner->GetID())
	{
		Smart_Ptr<GirlWar> girlWar =player->GetGirlWarManager()->GetGirlWar();
		if(girlWar)
		{
			girlWar->Fight();
		}
	}

	return true;
}

bool GirlWarManager::LeavePetData(const EventArgs & e)
{
	OneParmaEventArgs *pArgs = (OneParmaEventArgs *) (&e);

	if(pArgs->m_obj->GetType() != ePlayer)
		return true;

	Smart_Ptr<Player> player = pArgs->m_obj;
	if(player->GetID() == m_owner->GetID())
	{
		Smart_Ptr<GirlWar> girlWar = player->GetGirlWarManager()->GetGirlWar();
		if(girlWar)
		{
			girlWar->Rest();
		}
	}

	return true;
}

bool GirlWarManager::HandlePlayerOut(const EventArgs & e)
{
	return true;

	//战姬的释放在管理器释放时再释放

	/*PlayerRemoveMemberEventArgs *pArgs = (PlayerRemoveMemberEventArgs *) (&e);

	Smart_Ptr<Player> player = pArgs->m_pObj;

	Smart_Ptr<GirlWar> obj;
	if(player->GetID() == m_owner->GetID())
	{
		map<int64,Smart_Ptr<GirlWar> >::iterator itr = m_collection.begin();
		for(; itr != m_collection.end(); ++itr)
		{
			if(itr->second.Get()==NULL)
				continue;

			//回收对象
			itr->second->Realese();

			itr->second->SetOwnerToBrain(obj);
			Smart_Ptr<Player> player;
			itr->second->SetOwner(player);
		}

		SetGirlWar(obj);
	}

	return true;*/
}

bool  GirlWarManager::HandleCanLevelReward(const EventArgs & e)
{
	vector<int> vec;

	if(m_owner)
	{
		GirlWarLoader::GetInstance()->IsGirlWarNeed(eLv,m_owner->getAttManage()->getValue<int>(eCharLv),0,vec, 0);
		for(size_t i = 0 ; i < vec.size(); i++)
		{
			GreateGirlWar(vec[i]);
		}
	}
	return true;
}


bool GirlWarManager::HandleCanSignin(const EventArgs & e)
{
	vector<int> vec;

	if(m_owner)
	{
		GirlWarLoader::GetInstance()->IsGirlWarNeed(eSings,m_owner->GetCounterService()->GetNumById(TOTAL_SIGNIN_COUNT),0,vec, 0);
		for(size_t i = 0 ; i < vec.size(); i++)
		{
			GreateGirlWar(vec[i]);
		}
	}

	return true;
}


bool GirlWarManager::HandleUpdateVipLv(const EventArgs & e)
{
	vector<int> vec;
	PlayerAddPackSizeEventArgs * packSize = (PlayerAddPackSizeEventArgs*)(&e);

	if(m_owner && packSize)
	{
		GirlWarLoader::GetInstance()->IsGirlWarNeed(eVip,packSize->m_addLv,0,vec, 0);
		for(size_t i = 0 ; i < vec.size(); i++)
		{
			GreateGirlWar(vec[i]);
		}
	}

	return true;
}


bool GirlWarManager::HandleAddItem(const EventArgs & e)
{
	vector<int> vec;
	BuyItemEventArgs* data = (BuyItemEventArgs*)(&e);

	if(!data)
	{
		return true;
	}
	Smart_Ptr<Player>  player = data->m_pObj;

	if(player && player->GetID() == m_owner->GetID())
	{
		Smart_Ptr<ArticleBaseMgr> pack = player->GetContainer(ePackageType);
		if(!pack)
		{
			return true;
		}

		GirlWarLoader::GetInstance()->IsGirlWarNeed(eProp,data->m_id,pack->GetGoodsNum(data->m_id),vec, 0);
		for(size_t i = 0 ; i < vec.size(); i++)
		{
			GreateGirlWar(vec[i]);
		}
	}

	return true;
}

bool GirlWarManager::HandleInstancePass(const EventArgs& e)
{
	vector<int> vec;
	PlayerInstancePassEventArgs* data = (PlayerInstancePassEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		GirlWarLoader::GetInstance()->IsGirlWarNeed(eInstancePass, 0, 0, vec, data->m_mapId);
		for(size_t i = 0 ; i < vec.size(); i++)
		{
			GreateGirlWar(vec[i]);
		}
	}

	return true;
}

bool GirlWarManager::HandleGuardMaxNode(const EventArgs& e)
{
	vector<int> vec;
	PlayerProtectKingPassMaxArgs* data = (PlayerProtectKingPassMaxArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		GirlWarLoader::GetInstance()->IsGirlWarNeed(eGuardInstancePass, data->m_node, 0, vec, 0);
		for(size_t i = 0 ; i < vec.size(); i++)
		{
			GreateGirlWar(vec[i]);
		}
	}

	return true;
}

void GirlWarManager::InitGirlWarInfo(const PlayerInfo::GirlWarInfo &info)
{
	for(int i = 0 ; i < info.girlwar_size(); i++)
	{
		PlayerInfo::GirlWar girlWar = info.girlwar(i);

		const sGirlWarInfo *pInfo = GirlWarLoader::GetInstance()->GetGirlWarInfo(girlWar.id());
		if(!pInfo)
		{
			return;
		}

		Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(ePet);
		Smart_Ptr<GirlWar> gw = obj;

		gw->SetID(girlWar.id());
		gw->SetName(girlWar.name());
		gw->SetModle(pInfo->nModle);
		gw->getAttManage()->setValue(eCharExp,girlWar.exp());
		gw->getAttManage()->setValue(eCharLv,girlWar.level());
		gw->SetHPMax(girlWar.hpmax());
		gw->SetCurHP(girlWar.hp());
		gw->getAttManage()->setValue(eCharPhyAttackMax,girlWar.phyattackmax());
		gw->getAttManage()->setValue(eCharPhyDefence,girlWar.phydefence());
		gw->getAttManage()->setValue(eCharHitRate,girlWar.hitrate());
		gw->getAttManage()->setValue(eCharAvoidRate,girlWar.avoidrate());
		gw->getAttManage()->setValue(eCrackDownRate,girlWar.crit());
		gw->getAttManage()->setValue(eAvoidCrackRate,girlWar.ung());
		gw->getAttManage()->setValue(eCharMoveSpeed,girlWar.movespeed());
		gw->SetDeadTime(girlWar.deadtime());
		gw->getAttManage()->setValue(eFightPower,girlWar.fightpower());
		gw->SetPhyAttackTime(pInfo->nAckInterval);
		gw->SetSkill1(girlWar.skill1());
		gw->SetSkill2(girlWar.skill2());
		gw->SetSkill3(girlWar.skill3());

		int starrank = girlWar.starrank() > 0 ? girlWar.starrank() : 1;
		gw->SetStarrank(starrank);

		gw->SetQuality(girlWar.quality());
		for(int i = 0; i < girlWar.index_size(); i++)
		{
			gw->SetEquip(girlWar.index(i),girlWar.equip(i));
		}

		gw->SetOwnerToBrain(gw);
		gw->SetOwner(m_owner->GetMyself());

		if(girlWar.iswar())
		{
			gw->SetIsWar(girlWar.iswar());
			SetGirlWar(gw);
		}
		else
		{
			gw->SetIsWar(false);
		}

		AddGirlWar(gw);
	}

	SetVitality();
}

void GirlWarManager::SetVitality()
{
	bool isGotAllTopLvGirls = true;
	map<int,sGirlWarInfo> allGirlInfo;

	GirlWarLoader::GetInstance()->GetAllGirlWarInfo(allGirlInfo);
	//说明拥有说有的战姬
	if(allGirlInfo.size() == m_collection.size())
	{
		map<int64 ,Smart_Ptr<GirlWar> >::iterator it = m_collection.begin();
		for(;it!=m_collection.end();++it)
		{
			const sGirlWarInfo *pInfo = GirlWarLoader::GetInstance()->GetGirlWarInfo(it->second->GetID());
			int currentLv = it->second->getAttManage()->getValue<int>(eCharLv);
			if(pInfo && currentLv < pInfo->nLimitLv)
			{
				isGotAllTopLvGirls = false;
				break;
			}
		}
	}
	else
		isGotAllTopLvGirls = false;

	if(isGotAllTopLvGirls)
	{
		sVitality *vi = VitalityLoader::GetInstance()->GetVitalityInfoByID(eViGirlWarLingering);
		if(vi)
			m_owner->GetVitalityMgr()->FinshVitality(eViGirlWarLingering,vi->count);
	}
}

//上线计算属性
void GirlWarManager::onlineCalculateAtt()
{
	map<ECharAttrType,int64> tempAttList;
	map<int64 ,Smart_Ptr<GirlWar> >::iterator tempIt =  m_collection.begin();

	for(;tempIt != m_collection.end(); ++tempIt)
	{
			getDesignationLingeringAttList(tempIt->second,tempAttList);
	}

    //去掉总星阶加成 2016－4－4
//	const sGirlWarCollection* pColl = GirlWarLoader::GetInstance()->GetGirlWarCollection(GetStarrank());
//	if(pColl)
//	{
//		int nProfession = m_owner->getAttManage()->getValue<int>(eCharProfession);
//		tempAttList[eCharHPMax] 	   += (int64)(pColl->GetAddHp(nProfession));
//		tempAttList[eCharPhyAttackMax] += (int64)(pColl->GetAddPhyAck(nProfession));
//		tempAttList[eCharPhyDefence]   += (int64)(pColl->GetAddPhydef(nProfession));
//		tempAttList[eCharAvoidRate]   += (int64)(pColl->GetAddDodge(nProfession));
//		tempAttList[eCrackDownRate]   += (int64)(pColl->GetAddCrit(nProfession));
//		tempAttList[eAvoidCrackRate]   += (int64)(pColl->GetAddUng(nProfession));
//	}

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttGirlWar);

	m_owner->calculateResultAddAtt(tempAttList,tempValueType);

	calculateFight();

	return ;
}

//计算一次战斗力
int GirlWarManager::calculateFight()
{
	map<ECharAttrType,int64> tempAttList;
	map<int64 ,Smart_Ptr<GirlWar> >::iterator tempIt =  m_collection.begin();

	for(;tempIt != m_collection.end(); ++tempIt)
	{
			getDesignationLingeringAttList(tempIt->second,tempAttList);
	}


       //去掉总星阶加成 2016－4－4
//	int nProfession = m_owner->getAttManage()->getValue<int>(eCharProfession);
//	const sGirlWarCollection* pColl = GirlWarLoader::GetInstance()->GetGirlWarCollection(GetStarrank());
//	if(pColl)
//	{
//		tempAttList[eCharHPMax] 	   += (int64)(pColl->GetAddHp(nProfession));
//		tempAttList[eCharPhyAttackMax] += (int64)(pColl->GetAddPhyAck(nProfession));
//		tempAttList[eCharPhyDefence]   += (int64)(pColl->GetAddPhydef(nProfession));
//		tempAttList[eCharAvoidRate]   += (int64)(pColl->GetAddDodge(nProfession));
//		tempAttList[eCrackDownRate]   += (int64)(pColl->GetAddCrit(nProfession));
//		tempAttList[eAvoidCrackRate]   += (int64)(pColl->GetAddUng(nProfession));
//	}

	int tempResult = m_owner->getFightingManage()->CalculateFighting(tempAttList);
	return m_owner->getFightingManage()->setFighting(eFightingGirlWar,tempResult);
}

void GirlWarManager::SetGirlWarInfo(PlayerInfo::GirlWarInfo *info)
{
	map<int64,Smart_Ptr<GirlWar> >::iterator itr = m_collection.begin();
	for(; itr != m_collection.end(); ++itr)
	{
		PlayerInfo::GirlWar* gw = info->add_girlwar();

		gw->set_id(itr->second->GetID());
		gw->set_name(itr->second->GetName());
		//根据策划要求战鸡战斗力为给主人的战力加成，以此为证
		gw->set_fightpower(m_owner->getFightingManage()->getFighting(eFightingGirlWar));
		gw->set_exp(itr->second->getAttManage()->getValue<int64>(eCharExp));
		gw->set_level(itr->second->getAttManage()->getValue<int>(eCharLv));
		gw->set_hpmax(itr->second->getAttManage()->getValue<int>(eCharHPMax));
		gw->set_hp(itr->second->getAttManage()->getValue<int>(eCharHP));
		gw->set_phyattackmax(itr->second->getAttManage()->getValue<int>(eCharPhyAttackMax));
		gw->set_phydefence(itr->second->getAttManage()->getValue<int>(eCharPhyDefence));
		gw->set_hitrate(itr->second->getAttManage()->getValue<int>(eCharHitRate));
		gw->set_avoidrate(itr->second->getAttManage()->getValue<int>(eCharAvoidRate));
		gw->set_crit(itr->second->getAttManage()->getValue<int>(eCrackDownRate));
		gw->set_ung(itr->second->getAttManage()->getValue<int>(eAvoidCrackRate));
		gw->set_movespeed(itr->second->getAttManage()->getValue<int>(eCharMoveSpeed));
		gw->set_deadtime(itr->second->GetDeadTime());
		gw->set_iswar(itr->second->GetIsWar());
		gw->set_starrank(itr->second->GetStarrank());
		gw->set_quality(itr->second->GetQuality());
		gw->set_skill1(itr->second->GetSkill1());
		gw->set_skill2(itr->second->GetSkill2());
		gw->set_skill3(itr->second->GetSkill3());

		map<int,int> &mapEquip = itr->second->GetEquip();
		map<int,int>::iterator mitr = mapEquip.begin();
		for(; mitr != mapEquip.end(); ++mitr)
		{
			gw->add_index(mitr->first);
			gw->add_equip(mitr->second);
		}
	}
}

void GirlWarManager::AddGirlWar(Smart_Ptr<GirlWar>& girlWar)
{
	if( !girlWar)
	{
		return;
	}

	m_collection[girlWar->GetID()] = girlWar;
	m_owner->SetDataFlag(eGirlWarInfo);

	//cout<<endl<<"添加战姬："<<girlWar->GetID()<<"拥有战姬数量："<<m_collection.size()<<endl;
}

void GirlWarManager::GetGirlWar(int key,Smart_Ptr<GirlWar>& girlWar)
{
	map<int64 ,Smart_Ptr<GirlWar> >::iterator it = m_collection.begin();
	for(;it != m_collection.end(); it++)
	{
		if(it->second->GetKey() == key)
		{
			girlWar = it->second;
		}
	}
}

void GirlWarManager::GetGirlWarById(int nId,Smart_Ptr<GirlWar>& girlWar)
{
	map<int64 ,Smart_Ptr<GirlWar> >::iterator it = m_collection.find(nId);
	if(it != m_collection.end())
		girlWar = it->second;
}


void GirlWarManager::GetAllGirlWarInfo(GirlWarMessageInfo::ToClientGirlWarInfo& info)
{
	map<int,sGirlWarInfo> allGirlWarInfo;
	GirlWarLoader::GetInstance()->GetAllGirlWarInfo(allGirlWarInfo);

	map<int,sGirlWarInfo>::iterator itrEnd = allGirlWarInfo.end();
	for(map<int,sGirlWarInfo>::iterator itr = allGirlWarInfo.begin(); itr != itrEnd; ++itr)
	{
		Smart_Ptr<GirlWar> girlWar;
		GetGirlWarById(itr->first,girlWar);

		if(girlWar)
		{
			info.add_id(itr->first);
			info.add_ret(0);
			info.add_lv(girlWar->getAttManage()->getValue<int>(eCharLv));
			info.add_hp(girlWar->getAttManage()->getValue<int>(eCharHP));
			info.add_deadtime(girlWar->GetDeadTime());
			info.add_starrank(girlWar->GetStarrank());
			info.add_quality(girlWar->GetQuality());
			info.add_exp(girlWar->getAttManage()->getValue<int64>(eCharExp));
			GirlWarMessageInfo::ToClentEquip* equip = info.add_equip();
			map<int,int>::iterator itr = girlWar->GetEquip().begin();
			for(; itr != girlWar->GetEquip().end(); ++itr)
			{
				equip->add_index(itr->first);
				equip->add_id(itr->second);
			}

			info.add_skill1(girlWar->GetSkill1());
			info.add_skill2(girlWar->GetSkill2());
			info.add_skill3(girlWar->GetSkill3());
			GirlWarMessageInfo::ToClentNotHaveGirlWar* need = info.add_need();
			need->add_type(0);
			need->add_ref(0);
		}
		else
		{
			info.add_id(itr->first);
			info.add_ret(1);
			info.add_lv(0);
			info.add_hp(0);
			info.add_deadtime(0);
			info.add_starrank(0);
			info.add_quality(0);
			info.add_exp(0);
			info.add_skill1(0);
			info.add_skill2(0);
			info.add_skill3(0);
			info.add_equip();
			GetGirlWarCondition(itr->first,info.add_need());
		}
	}
}

void GirlWarManager::GetGirlWarCondition(int nId,GirlWarMessageInfo::ToClentNotHaveGirlWar* info)
{
	vector<int> vec;
	GirlWarLoader::GetInstance()->GetGirlWarType(nId,vec);

	for(size_t i = 0; i < vec.size(); i++)
	{
		switch(vec[i])
		{
		case eSings:
			{
				info->add_type((int)eSings);
				Smart_Ptr<CounterService> counterService = m_owner->GetCounterService();
				if(counterService)
				{
					info->add_ref(counterService->GetNumById(TOTAL_SIGNIN_COUNT));
				}
				break;
			}
		case eVip:
			{
				info->add_type((int)eVip);
				info->add_ref(m_owner->getAttManage()->getValue<DWORD>(eVIPLv));
				break;
			}
		case eLv:
			{
				info->add_type((int)eLv);
				info->add_ref(m_owner->getAttManage()->getValue<int>(eCharLv));
				break;
			}
		case eProp:
			{
				info->add_type((int)eProp);

				Smart_Ptr<ArticleBaseMgr> abm = m_owner->GetContainer(ePackageType);
				if(abm)
				{
					info->add_ref(abm->GetGoodsNum(GirlWarLoader::GetInstance()->GetGirlWarNeedPropId(nId)));
				}

				break;
			}
		case eMicro:
			{
				info->add_type((int)eMicro);
				info->add_ref(0);
				break;
			}
		case eMPVerification:
			{
				info->add_type((int)eMPVerification);
				info->add_ref(0);
				break;
			}
		}
	}
}

void GirlWarManager::ReseatGirlWar(int nId, GirlWarMessageInfo::ToClentFight& info)
{
	int ret = 0;
	Smart_Ptr<GirlWar> girlWar;

	GetGirlWarById(nId,girlWar);

	if(!girlWar)
	{
		ret = ePetNotExist;
	}
	else if(girlWar->IsDead())
	{
		ret = ePetIsDead;
	}
	else if(!GetGirlWar())
	{
		ret = ePetNotHadFight;
	}
	else if(GetGirlWar()->GetID() != nId)
	{
		ret = ePetIdError;
	}
	else
	{
		Smart_Ptr<GirlWar> girlWarInfo;
		SetGirlWar(girlWarInfo);
		//2016-4-4 取消出战星阶加成
		//girlWar->ChangeAttribute(false);
		girlWar->Rest();
		girlWar->SetIsWar(false);
		info.set_id(nId);
	}

	info.set_ret(ret);
}

void GirlWarManager::FightGirlWar(int nId, GirlWarMessageInfo::ToClentFight& info)
{
	int ret = 0;
	Smart_Ptr<GirlWar> girlWar;

	if(GetGirlWar())
	{
		if(GetGirlWar()->GetID() == nId)
		{
			ret = ePetHadFight;
			info.set_ret(ret);
			return;
		}
	}

	GetGirlWarById(nId,girlWar);

	if(!girlWar)
	{
		ret = ePetNotExist;
	}
	else if(girlWar->IsDead())
	{
		ret = ePetIsDead;
	}
	else
	{
		if(GetGirlWar())
		{
			//2016-4-4 取消出战星阶加成
			//GetGirlWar()->ChangeAttribute(false);
			GetGirlWar()->Rest();
			GetGirlWar()->SetIsWar(false);
		}

		SetGirlWar(girlWar);
		//2016-4-4 取消出战星阶加成
		//girlWar->ChangeAttribute(true);
		girlWar->Fight();

		girlWar->SetIsWar(true);

		info.set_id(nId);

		PlayerGirlwarFight tempArgs(GetGirlWar());
		m_owner->FireEvent(PLAYER_GIRLWAR_FIGHT,tempArgs);

		m_owner->SetDataFlag(eGirlWarInfo);
	}
	info.set_ret(ret);
}

void GirlWarManager::ReviveGirlWar(int nId, GirlWarMessageInfo::ToClentFight& info)
{
	int ret = 0;
	vector<int> vecType;
	Smart_Ptr<GirlWar> girlWar;

	GetGirlWarById(nId,girlWar);

	if(!girlWar)
	{
		ret = ePetNotExist;
	}
	else if(!girlWar->IsDead())
	{
		ret = ePetIsNotDead;
	}
	else
	{
//		sGirlWarInfo girlWarInfo;
//		GirlWarLoader::GetInstance()->GetGirlWarInfo(nId, girlWarInfo);
//
//		int64 nowSecond = CUtil::GetNowSecond();
//		int64 deadTime = girlWar->GetDeadTime();
//		int interval = nowSecond - deadTime;
//		//int nGolden = ((int)(CUtil::GetNowSecond() - girlWar->GetDeadTime()))/60000 * 5;
//		int nGolden = (girlWarInfo.nFlushTime - interval + 60000) / 60000 * 5;
//		if(m_owner->getAttManage()->getValue<int>(eCharGolden) < nGolden)
//		{
//			ret = eNotEnogGolden;
//		}
//		else
		{
			if(GetGirlWar() && GetGirlWar()->GetID() == nId)
			{
				girlWar->Reflush();
			}
			else
			{
				girlWar->onReviveTime(NULL);
			}

//			m_owner->ChangeGolden(nGolden,false, eStatic_AboutGolden, eStaMinor_AboutGolden_ReviveGirlWar);
//			vecType.push_back(eCharGolden);
//			m_owner->SynCharAttribute(vecType);
			info.set_id(nId);

			m_owner->SetDataFlag(eGirlWarInfo);

			// 统计元宝花费 兑换铜钱
//			StatisticMgr::GetInstance()->StatisticPlayerGetItems(m_owner->GetMyself(), eStatic_AboutGolden, eStaMinor_AboutGolden_ReviveGirlWar,1, nGolden);

		}
	}

	info.set_ret(ret);
}

int GirlWarManager::GreateGirlWar(int nId)
{
	int ret = 0;
	const GirlWarNeed *pNeed = GirlWarLoader::GetInstance()->GetGirlWarNeed(nId);
	const sGirlWarInfo *pInfo = GirlWarLoader::GetInstance()->GetGirlWarInfo(nId);
	const sGirlWarStarrank *pStarInfo= GirlWarLoader::GetInstance()->GetGirlWarStarrank(nId);
	const sGirlWarQuality *pQuality = GirlWarLoader::GetInstance()->GetGirlWarQuality(nId);
	const sGirlWarAttribute *pAttribute = GirlWarLoader::GetInstance()->GetGirlWarAttribute(nId);
	if(!pInfo || !pStarInfo || !pQuality || !pNeed || !pAttribute)
	{
		return eGirlDataFail;
	}

	ret = IsCanGetGirlWar(nId);
	if(ret != 0)
	{
		return ret;
	}
	else
	{
		Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(ePet);
		Smart_Ptr<GirlWar> gw = obj;

		gw->SetID(nId);
		gw->SetName(pInfo->sName);
		gw->SetModle(pInfo->nModle);
		gw->getAttManage()->setValue(eCharCountry,m_owner->getAttManage()->getValue<DWORD>(eCharCountry));
		gw->getAttManage()->setValue(eCharMoveSpeed,pInfo->nMoveSpeed);
		gw->getAttManage()->setValue(eCharHitRate,pInfo->nBasehit);
		gw->SetStarrank(1);
		gw->SetQuality(1);
		gw->SetDeadTime(0);
		gw->getAttManage()->setValue(eCharLv,1);

		gw->SetPhyAttackTime(pInfo->nAckInterval);
		gw->SetSkill1(pInfo->nSkill1);

		gw->SetOwnerToBrain(gw);
		gw->SetOwner(m_owner->GetMyself());

		AddGirlWar(gw);

		OneParmaEventArgs tempArgs(m_owner->GetMyself());
		m_owner->FireEvent(PLAYER_GIRLWAR_ADD,tempArgs);

		gw->ChangeAttribute(pStarInfo->nAddHp,pStarInfo->nAddPhyAck,pStarInfo->nAddPhydef,pStarInfo->nAddCrit,pStarInfo->nAddUng,pStarInfo->nAddDodge);
		gw->ChangeAttribute(pAttribute->nAddHp,pAttribute->nAddPhyAck,pAttribute->nAddPhydef,pAttribute->nAddCrit,pAttribute->nAddUng,pAttribute->nAddDodge);


		gw->ChangeOwerAttribute(true);
	       //去掉总星阶加成 2016－4－4
//		GetStarCollection();

		string str = ChatMgr::GetInstance()->GetStringByInt(gw->GetID());
		ChatMgr::GetInstance()->Marquee(eMarqueeGirl,m_owner->GetName(),str,"");

		m_owner->SetDataFlag(eGirlWarInfo);
		m_owner->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_GirlWar, eUIHLSmall_GirlWar_GetNew), 1);

		if(pNeed->nPropId > 0)
		{
			m_owner->GetContainer(ePackageType)->DeleteGoodsById(pNeed->nPropId,pNeed->nPropLv,true);
		}
	}

	return 0;
}


//2016-4-4 战姬创建修改
//void GirlWarManager::GreateGirlWar(int  nId)
//{
//	GirlWarMessageInfo::ToClientActivate toClient;
//
//	const sGirlWarInfo *pInfo = GirlWarLoader::GetInstance()->GetGirlWarInfo(nId);
//	const sGirlWarStarrank *pStarInfo= GirlWarLoader::GetInstance()->GetGirlWarStarrank(nId);
//	const sGirlWarQuality *pQuality = GirlWarLoader::GetInstance()->GetGirlWarQuality(nId);
//
//	if(!pInfo || !pStarInfo || !pQuality)
//	{
//		return;
//	}
//	else if(IsHave(nId))
//	{
//		return;
//	}
//	else
//	{
//
//		Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(ePet);
//		Smart_Ptr<GirlWar> gw = obj;
//
//		gw->SetID(nId);
//		gw->SetName(pInfo->sName);
//		gw->SetModle(pInfo->nModle);
//		gw->getAttManage()->setValue(eCharCountry,m_owner->getAttManage()->getValue<DWORD>(eCharCountry));
//		gw->SetHPMax(pStarInfo->nAddHp);
//		gw->SetCurHP(pStarInfo->nAddHp);
//		gw->getAttManage()->setValue(eCharLv,1);
//		gw->getAttManage()->setValue(eCharPhyAttackMax,pStarInfo->nAddPhyAck);
//		gw->getAttManage()->setValue(eCharPhyDefence,pStarInfo->nAddPhydef);
//		gw->getAttManage()->setValue(eCharHitRate,pInfo->nBasehit);
//		gw->getAttManage()->setValue(eCharLuckyNULL,pInfo->nLuck);
//		gw->getAttManage()->setValue(eCharAvoidRate,pStarInfo->nAddDodge);
//		gw->getAttManage()->setValue(eCrackDownRate,pStarInfo->nAddCrit);
//		gw->getAttManage()->setValue(eAvoidCrackRate,pStarInfo->nAddUng);
//		gw->getAttManage()->setValue(eCharMoveSpeed,pInfo->nMoveSpeed);
//		gw->SetStarrank(1);
//		gw->SetQuality(1);
//		gw->SetDeadTime(0);
//
//		int fightpower = BattleInfoLoader::GetInstance()->GetAllFight(
//				gw->getAttManage()->getValue<int>(eCharPhyAttackMax),
//				gw->getAttManage()->getValue<int>(eCharPhyDefence),
//				gw->getAttManage()->getValue<int>(eCharHPMax),
//				gw->getAttManage()->getValue<int>(eCharHitRate),
//				gw->getAttManage()->getValue<int>(eCharAvoidRate),
//				gw->getAttManage()->getValue<int>(eCrackDownRate),
//				gw->getAttManage()->getValue<int>(eAvoidCrackRate));
//
//		gw->getAttManage()->setValue(eFightPower,fightpower);
//
//		gw->SetPhyAttackTime(pInfo->nAckInterval);
//		gw->SetSkill1(pInfo->nSkill1);
//
//		gw->SetOwnerToBrain(gw);
//		gw->SetOwner(m_owner->GetMyself());
//
//		AddGirlWar(gw);
//
//	       //去掉总星阶加成 2016－4－4
////		GetStarCollection();
//
//		toClient.set_id(gw->GetID());
//		string str = ChatMgr::GetInstance()->GetStringByInt(gw->GetID());
//		ChatMgr::GetInstance()->Marquee(eMarqueeGirl,m_owner->GetName(),str,"");
//
//		m_owner->SetDataFlag(eGirlWarInfo);
//
//		QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(), &toClient, MSG_SIM_GM2C_GIRLWARACTIVATE);
//
//		m_owner->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_GirlWar, eUIHLSmall_GirlWar_GetNew), 1);
//	}
//}

bool GirlWarManager::IsHave(int nId)
{
	map<int64 ,Smart_Ptr<GirlWar> >::iterator it = m_collection.find(nId);
	if(it != m_collection.end())
		return true;
	return false;
}

//是否有战姬大过指定等级
bool GirlWarManager::isMoreThanLevel(int level)
{
	map<int64 ,Smart_Ptr<GirlWar> >::iterator tempIt = m_collection.begin();
	for(;tempIt != m_collection.end();++tempIt)
	{
		if(tempIt->second->getAttManage()->getValue<int>(eCharLv) >= level)
			return true;
	}

	return false;
}

void GirlWarManager::GirlWarUpLv(GirlWarMessageInfo::FromClentUplv& req,GirlWarMessageInfo::ToClentUplv& info)
{
	vector<int> vecType;
	Smart_Ptr<GirlWar> girlWar;
	int curLv = 0;
	int primaryCount = 0;
	int needCount = 0;
	const GoodsInfo* ginfo = NULL;
	int ret = 0;
	GetGirlWarById(req.id(),girlWar);
	if(!girlWar)
	{
		ret = ePetNotExist;
	}
	else
	{
		curLv = girlWar->getAttManage()->getValue<int64>(eCharLv);

		if(curLv >= GirlWarLoader::GetInstance()->GetLimitLv(girlWar->GetID()))
		{
			info.set_ret(ePetLvMax);
			return;
		}

		if(curLv >= m_owner->getAttManage()->getValue<int64>(eCharLv))
		{
			info.set_ret(ePetLvOverPlayerLv);
			return;
		}

		primaryCount = m_owner->GetContainer(ePackageType)->FindPropCount(GirlWarLoader::GetInstance()->GetCommonExp());
		if(primaryCount <= 0 && !req.isbuy())
		{
			info.set_ret(eItemNotEnough);
			return;
		}

		ginfo = GoodsLoader::GetInstance()->GetItemDataByID(GirlWarLoader::GetInstance()->GetCommonExp());

		int tempGolden = MallLoader::GetInstance()->GetDanLuStoneCost(GirlWarLoader::GetInstance()->GetCommonExp());
		Smart_Ptr<PlayerVipPart> tempVip = m_owner->GetVipPart();
		if (tempVip)
		{
			tempGolden = PlayerVipPart::CalBuyCostGolden(tempVip, tempGolden);
		}

		//0:普通升级， 1：一键升级
		if(req.type() == 0)
		{
			if (primaryCount <= 0 && req.isbuy())
			{
				needCount = 1;

				if (!m_owner->IsEnoughGolden(tempGolden))
				{
					info.set_ret(eNotEnoughGold);
					return;
				}
			}
			else
			{
				m_owner->GetContainer(ePackageType)->DeleteGoodsById(GirlWarLoader::GetInstance()->GetCommonExp(),1,true);
			}

			girlWar->AddExp(ginfo->buffID);
		}
		else
		{
			int64 tempExp = girlWar->getAttManage()->getValue<int64>(eCharExp);
			int tempLevel = girlWar->getAttManage()->getValue<int>(eCharLv);
			tempExp = GirlWarLoader::GetInstance()->GetUpdateLv(girlWar->GetID(),tempLevel)- tempExp;
			int temp = (tempExp % ginfo->buffID) ? 1 : 0;
			needCount = tempExp/ginfo->buffID + temp;
			if(primaryCount < needCount && req.isbuy())
			{
				needCount -= primaryCount;
				if (!m_owner->IsEnoughGolden(tempGolden*needCount))
				{
					info.set_ret(eNotEnoughGold);
					return;
				}
			}
			else
			{
				needCount = 0;
			}

			for(int i = 0; i < primaryCount; i++)
			{
				m_owner->GetContainer(ePackageType)->DeleteGoodsById(GirlWarLoader::GetInstance()->GetCommonExp(),1,true);
				girlWar->AddExp(ginfo->buffID);
				if(curLv < girlWar->getAttManage()->getValue<int64>(eCharLv))
				{
					break;
				}
			}

			for(int j = 0; j < needCount; j++)
			{
				girlWar->AddExp(ginfo->buffID);
				if(curLv < girlWar->getAttManage()->getValue<int64>(eCharLv))
				{
					break;
				}
			}
		}

		int tempbuckleGolden 	 = 0;
		int tempbuckleBindGolden = 0;

		if (tempGolden* needCount > 0)
		{
			m_owner->DecreaseGoldenCost(tempGolden* needCount,tempbuckleGolden,tempbuckleBindGolden);

			if(tempbuckleGolden > 0)
				vecType.push_back(eCharGolden);

			if(tempbuckleBindGolden > 0)
				vecType.push_back(eCharBindGolden);
		}

		m_owner->SynCharAttribute(vecType);

		if(curLv < girlWar->getAttManage()->getValue<int64>(eCharLv))
		{
			const sGirlWarAttribute *pAttribute = GirlWarLoader::GetInstance()->GetGirlWarAttribute(girlWar->GetID(),curLv);
			const sGirlWarAttribute *pAttributeInfo = GirlWarLoader::GetInstance()->GetGirlWarAttribute(girlWar->GetID(),girlWar->getAttManage()->getValue<int64>(eCharLv));
			if(pAttribute && pAttributeInfo)
			{
				girlWar->ChangeAttribute(pAttribute->nAddHp,pAttribute->nAddPhyAck,pAttribute->nAddPhydef,pAttribute->nAddCrit,pAttribute->nAddUng,pAttribute->nAddDodge,false);
				girlWar->ChangeAttribute(pAttributeInfo->nAddHp,pAttributeInfo->nAddPhyAck,pAttributeInfo->nAddPhydef,pAttributeInfo->nAddCrit,pAttributeInfo->nAddUng,pAttributeInfo->nAddDodge);
			}
		}

		info.set_exp(girlWar->getAttManage()->getValue<int64>(eCharExp));
		info.set_lv(girlWar->getAttManage()->getValue<int64>(eCharLv));
		info.set_id(girlWar->GetID());
	}

	info.set_ret(ret);
}

int GirlWarManager::WareEquip(GirlWarMessageInfo::GirlWarEquip& req,GirlWarMessageInfo::RetGirlWarEquip& info)
{
	Smart_Ptr<GirlWar> girlWar;
	vector<int> vecType;
	int ret = 0;
	GetGirlWarById(req.id(),girlWar);
	if(!girlWar)
	{
		ret = ePetNotExist;
	}
	else if(m_owner->GetContainer(ePackageType)->FindPropCount(req.equipid()) < 1)
	{
		ret = eItemNotEnough;
	}
	else if(girlWar->IsHaveEquip(req.equipid()))
	{
		ret = eGirlHaveWare;
	}
	else if(!girlWar->IsPosEmpty(req.index()))
	{
		ret = eGirlPosFull;
	}
	else if(!GirlWarLoader::GetInstance()->IsHavaEquip(girlWar->GetID(),girlWar->GetQuality(),req.equipid()))
	{
		ret = eGirlDataFail;
	}
	else
	{
			m_owner->GetContainer(ePackageType)->DeleteGoodsById(req.equipid(),1,true);
			girlWar->SetEquip(req.index(),req.equipid());
			const map<int,int>* pAttr = GirlWarLoader::GetInstance()->GetEquipAtt(girlWar->GetID(),girlWar->GetQuality(),req.equipid());
			if(pAttr)
			{
				map<int,int>::const_iterator itr = pAttr->begin();
				for(;itr != pAttr->end(); ++itr)
				{
					girlWar->getAttManage()->setValue((ECharAttrType)itr->first,itr->second);
					vecType.push_back(itr->first);
				}

				girlWar->SynCharAttribute(vecType);
			}

			info.set_id(req.id());
			GirlWarMessageInfo::ToClentEquip* pEquip = info.mutable_equip();
			map<int,int>::iterator mitr = girlWar->GetEquip().begin();
			for(; mitr != girlWar->GetEquip().end(); ++mitr)
			{
				pEquip->add_index(mitr->first);
				pEquip->add_id(mitr->second);
			}

			m_owner->SetDataFlag(eGirlWarInfo);
	}

	return ret;
}

int GirlWarManager::OneKeyWareEquip(int nId,GirlWarMessageInfo::RetGirlWarOneKeyEquip& info)
{
	Smart_Ptr<GirlWar> girlWar;
	vector<int> vecType;
	int ret = 0;
	GetGirlWarById(nId,girlWar);
	if(!girlWar)
	{
		ret = ePetNotExist;
	}
	else if(girlWar->GetEquip().size() >= 4)
	{
		ret = eGirlHaveWare;
	}
	else
	{
		const sGirlWarQuality *pQuality = GirlWarLoader::GetInstance()->GetGirlWarQuality(girlWar->GetID(),girlWar->GetQuality());
		if(pQuality)
		{
			GetWareEquip(girlWar->GetID(),pQuality->m_nEquip1);
			GetWareEquip(girlWar->GetID(),pQuality->m_nEquip2);
			GetWareEquip(girlWar->GetID(),pQuality->m_nEquip3);
			GetWareEquip(girlWar->GetID(),pQuality->m_nEquip4);

			info.set_id(nId);
			GirlWarMessageInfo::ToClentEquip* pEquip = info.mutable_equip();
			map<int,int>::iterator mitr = girlWar->GetEquip().begin();
			for(; mitr != girlWar->GetEquip().end(); ++mitr)
			{
				pEquip->add_index(mitr->first);
				pEquip->add_id(mitr->second);
			}

			m_owner->SetDataFlag(eGirlWarInfo);
		}
	}

	return ret;
}

void GirlWarManager::GetWareEquip(int nId,int nEquipId)
{
	Smart_Ptr<GirlWar> girlWar;
	vector<int> vecType;
	GetGirlWarById(nId,girlWar);
	if(!girlWar)
	{
		return;
	}
	else if(girlWar->GetEquip().size() >= 4)
	{
		return;
	}
	else if(m_owner->GetContainer(ePackageType)->FindPropCount(nEquipId) < 1)
	{
		return;
	}
	else if(girlWar->IsHaveEquip(nEquipId))
	{
		return;
	}
	else if(!GirlWarLoader::GetInstance()->IsHavaEquip(girlWar->GetID(),girlWar->GetQuality(),nEquipId))
	{
		return;
	}
	else
	{
		for(int i = 1; i < 5; i++)
		{
			if(girlWar->IsPosEmpty(i))
			{
				m_owner->GetContainer(ePackageType)->DeleteGoodsById(nEquipId,1,true);
				girlWar->SetEquip(i,nEquipId);
				const map<int,int>* pAttr = GirlWarLoader::GetInstance()->GetEquipAtt(girlWar->GetID(),girlWar->GetQuality(),nEquipId);
				if(pAttr)
				{
					map<int,int>::const_iterator itr = pAttr->begin();
					for(;itr != pAttr->end(); ++itr)
					{
						girlWar->getAttManage()->setValue((ECharAttrType)itr->first,itr->second);
						vecType.push_back(itr->first);
					}

					girlWar->SynCharAttribute(vecType);
				}

				break;
			}
		}
	}
}

//升阶
int GirlWarManager::UpQuality(int nId,GirlWarMessageInfo::RetGirlWarUpQuality& info)
{
	Smart_Ptr<GirlWar> girlWar;
	vector<int> vecType;
	int ret = 0;
	GetGirlWarById(nId,girlWar);
	const sGirlWarInfo *pInfo = GirlWarLoader::GetInstance()->GetGirlWarInfo(nId);
	if(!girlWar || !pInfo)
	{
		ret = ePetNotExist;
	}
	else if(girlWar->GetEquip().size() < 4)
	{
		ret = eGirlEquipNotFull;
	}
	else
	{
		const sGirlWarQuality *pQuality = GirlWarLoader::GetInstance()->GetGirlWarQuality(nId,girlWar->GetQuality());
		const sGirlWarQuality *pQualityNext = GirlWarLoader::GetInstance()->GetGirlWarQuality(nId,girlWar->GetQuality()+1);
		if(pQuality && pQualityNext)
		{
			if(pQuality->nQualityLv > girlWar->getAttManage()->getValue<int>(eCharLv))
			{
				ret = eLVNotEnough;
			}
			else
			{
				girlWar->SetQuality(girlWar->GetQuality() + 1);

				GetPetEventArgs tempArgs(m_owner->GetMyself(),girlWar->GetID(),girlWar->GetQuality(),0);
				m_owner->FireEvent(PLAYER_GIRLWAR_QUALITY,tempArgs);

				if(girlWar->GetQuality() == 3)
				{
					girlWar->SetSkill(2,pInfo->nSkill2);
				}
				else if(girlWar->GetQuality() == 5)
				{
					girlWar->SetSkill(3,pInfo->nSkill3);
				}

				(girlWar->GetEquip()).clear();

				info.set_id(nId);
				info.set_quality(girlWar->GetQuality());
				info.set_skill1(girlWar->GetSkill1());
				info.set_skill2(girlWar->GetSkill2());
				info.set_skill3(girlWar->GetSkill3());

				m_owner->SetDataFlag(eGirlWarInfo);
			}
		}
		else
		{
			ret = eGirlQualityFull;
		}
	}

	return ret;
}

//技能升级
int GirlWarManager::UpSkill(GirlWarMessageInfo::GirlWarUpSkill& req,GirlWarMessageInfo::RetGirlWarUpSkill& info)
{
	Smart_Ptr<GirlWar> girlWar;
	SkillInfo skillInfo;
	vector<int> vecType;
	int ret = 0;
	GetGirlWarById(req.id(),girlWar);
	SkillLoader::GetInstance()->GetSkillInfoByID(req.skillid(), skillInfo);
	if(!girlWar)
	{
		ret = ePetNotExist;
	}
	else if(skillInfo.skillID == -1)
	{
		ret = eSkillIsNull;
	}
	else if(req.skillid() != girlWar->GetSkill(req.index()))
	{
		ret = eSystemError;
	}
	else
	{
		//升级技能
		if(skillInfo.nextSkillID <= 0)
		{
			return eHighestLv;
		}

		SkillInfo nextInfo;
		SkillLoader::GetInstance()->GetSkillInfoByID(skillInfo.nextSkillID, nextInfo);
		if(nextInfo.skillID == -1)
		{
			return eSkillIsNull;
		}

		if(nextInfo.nowLv > (girlWar->GetStarrank() * 10))
		{
			return eGirlStarNotEnough;
		}

		if(nextInfo.useMoney > m_owner->getAttManage()->getValue<int>(eCharMoney))
		{
			return eNotEnoghMoney;
		}

		if(nextInfo.useForce > m_owner->getAttManage()->getValue<int>(eCharForce))
		{
			return eNotEnoughForce;
		}

		girlWar->SetSkill(req.index(),skillInfo.nextSkillID);

		m_owner->ChangeMoney(nextInfo.useMoney, false);
		m_owner->ChangeCurForce(nextInfo.useForce, false);

		if(nextInfo.useMoney > 0)
		{
			vecType.push_back(eCharMoney);
		}

		if(nextInfo.useForce > 0)
		{
			vecType.push_back(eCharForce);
		}

		if(vecType.size() > 0)
		{
			m_owner->SynCharAttribute(vecType);
		}

		m_owner->SetDataFlag(eGirlWarInfo);

		info.set_id(req.id());
		info.set_skillid(nextInfo.skillID);
		info.set_index(req.index());
	}

	return ret;
}

int GirlWarManager::UpStar(int nId,GirlWarMessageInfo::ToClientUpStar& info)
{
	Smart_Ptr<GirlWar> girlWar;
	vector<int> vecType;
	int ret = 0;
	int tempValue = 0;
	int tempResult = 0;
	int tempfightpower = 0;
	GetGirlWarById(nId,girlWar);

	if(!girlWar)
	{
		ret = ePetNotExist;
	}
	else
	{
		const sGirlWarStarrank *pStar = GirlWarLoader::GetInstance()->GetGirlWarStarrank(nId, girlWar->GetStarrank());
		const sGirlWarStarrank *pStarInfo = GirlWarLoader::GetInstance()->GetGirlWarStarrank(nId, girlWar->GetStarrank()+1);
		if(!pStar)
		{
			return eSystemError;
		}

		if(!pStarInfo)
		{
			return eGirlStarFull;
		}

		if(m_owner->GetContainer(ePackageType)->FindPropCount(pStarInfo->nDebrisId) < pStarInfo->nDebrisNum)
		{
			return eItemNotEnough;
		}

		m_owner->GetContainer(ePackageType)->DeleteGoodsById(pStarInfo->nDebrisId,pStarInfo->nDebrisNum,true);

//		girlWar->SetHPMax(pStarInfo->nAddHp);
//		girlWar->SetCurHP(pStarInfo->nAddHp);
//		girlWar->getAttManage()->setValue(eCharPhyAttackMax,pStarInfo->nAddPhyAck);
//		girlWar->getAttManage()->setValue(eCharPhyDefence,pStarInfo->nAddPhydef);
//		girlWar->getAttManage()->setValue(eCharAvoidRate,pStarInfo->nAddDodge);
//		girlWar->getAttManage()->setValue(eCrackDownRate,pStarInfo->nAddCrit);
//		girlWar->getAttManage()->setValue(eAvoidCrackRate,pStarInfo->nAddUng);
//		int fightpower = BattleInfoLoader::GetInstance()->GetAllFight(
//				girlWar->getAttManage()->getValue<int>(eCharPhyAttackMax),
//				girlWar->getAttManage()->getValue<int>(eCharPhyDefence),
//				girlWar->getAttManage()->getValue<int>(eCharHPMax),
//				girlWar->getAttManage()->getValue<int>(eCharHitRate),
//				girlWar->getAttManage()->getValue<int>(eCharAvoidRate),
//				girlWar->getAttManage()->getValue<int>(eCrackDownRate),
//				girlWar->getAttManage()->getValue<int>(eAvoidCrackRate));
//
//		girlWar->getAttManage()->setValue(eFightPower,fightpower);
//
//		vecType.push_back(eCharHP);
//		vecType.push_back(eCharHPMax);
//		vecType.push_back(eCharPhyAttackMax);
//		vecType.push_back(eCharPhyDefence);
//		vecType.push_back(eCharAvoidRate);
//		vecType.push_back(eCrackDownRate);
//		vecType.push_back(eAvoidCrackRate);
//		vecType.push_back(eFightPower);
//
//		girlWar->SynCharAttribute(vecType);

		girlWar->ChangeAttribute(pStar->nAddHp,pStar->nAddPhyAck,pStar->nAddPhydef,pStar->nAddCrit,pStar->nAddUng,pStar->nAddDodge,false);

		girlWar->ChangeAttribute(pStarInfo->nAddHp,pStarInfo->nAddPhyAck,pStarInfo->nAddPhydef,pStarInfo->nAddCrit,pStarInfo->nAddUng,pStarInfo->nAddDodge);

		girlWar->SetStarrank(girlWar->GetStarrank() + 1);

		m_owner->SetDataFlag(eGirlWarInfo);

		//2016-4-4 取消出战星阶加成
//		if(girlWar->GetIsWar())
//		{
			m_owner->ChangeHPMax(pStar->nAddOwerHp,false);
			m_owner->ChangePhyAttackMax(pStar->nAddOwerPhyAck,false);
			m_owner->ChangePDefence(pStar->nAddOwerPhydef,false);
			m_owner->ChangeARate(pStar->nAddOwerDodge,false);
			m_owner->ChangeCRate(pStar->nAddOwerCrit,false);
			m_owner->ChangeBaserte(pStar->nAddOwerUng,false);


			tempfightpower = BattleInfoLoader::GetInstance()->GetAllFight(
					pStar->nAddOwerPhyAck,pStar->nAddOwerPhydef,pStar->nAddOwerHp,0,pStar->nAddOwerDodge,pStar->nAddOwerCrit,pStar->nAddOwerUng);

			tempValue = m_owner->getFightingManage()->getFighting(eFightingGirlWar);
			tempResult =  tempValue - tempfightpower;
			m_owner->getFightingManage()->setFighting(eFightingGirlWar,tempResult);

			m_owner->ChangeHPMax(pStarInfo->nAddOwerHp);
			m_owner->ChangePhyAttackMax(pStarInfo->nAddOwerPhyAck);
			m_owner->ChangePDefence(pStarInfo->nAddOwerPhydef);
			m_owner->ChangeARate(pStarInfo->nAddOwerDodge);
			m_owner->ChangeCRate(pStarInfo->nAddOwerCrit);
			m_owner->ChangeBaserte(pStarInfo->nAddOwerUng);

			tempfightpower = BattleInfoLoader::GetInstance()->GetAllFight(
					pStarInfo->nAddOwerPhyAck,pStarInfo->nAddOwerPhydef,pStarInfo->nAddOwerHp,0,pStarInfo->nAddOwerDodge,pStarInfo->nAddOwerCrit,pStarInfo->nAddOwerUng);
			tempValue = m_owner->getFightingManage()->getFighting(eFightingGirlWar);
			tempResult =  tempValue + tempfightpower;
			m_owner->getFightingManage()->setFighting(eFightingGirlWar,tempResult);

			vecType.push_back(eCharHP);
			vecType.push_back(eCharHPMax);
			vecType.push_back(eCharPhyAttackMax);
			vecType.push_back(eCharPhyDefence);
			vecType.push_back(eCharAvoidRate);
			vecType.push_back(eCrackDownRate);
			vecType.push_back(eAvoidCrackRate);
			vecType.push_back(eFightPower);

			m_owner->SynCharAttribute(vecType);
//		}

	       //去掉总星阶加成 2016－4－4
//		GetStarCollection();

		info.set_starrank(girlWar->GetStarrank());
		info.set_id(girlWar->GetID());
	}

	return ret;
}

void GirlWarManager::GetStarCollection()
{
	vector<int> vecType;
	int tempValue = 0;
	int tempResult = 0;
	int tempfightpower = 0;
	int nProfession = m_owner->getAttManage()->getValue<int>(eCharProfession);
	const sGirlWarCollection* pColl = GirlWarLoader::GetInstance()->GetGirlWarCollection(GetStarrank());
	if(pColl)
	{
		if(pColl->nId == 1)
		{
			m_owner->ChangeHPMax(pColl->GetAddHp(nProfession));
			m_owner->ChangePhyAttackMax(pColl->GetAddPhyAck(nProfession));
			m_owner->ChangePDefence(pColl->GetAddPhydef(nProfession));
			m_owner->ChangeARate(pColl->GetAddDodge(nProfession));
			m_owner->ChangeCRate(pColl->GetAddCrit(nProfession));
			m_owner->ChangeBaserte(pColl->GetAddUng(nProfession));
		}

		const sGirlWarCollection* pTempColl = GirlWarLoader::GetInstance()->GetGirlWarCollectionById(pColl->nId -1);
		if(pTempColl && (pTempColl->nStarrankUp + 1) == GetStarrank())
		{
			m_owner->ChangeHPMax(pTempColl->GetAddHp(nProfession),false);
			m_owner->ChangePhyAttackMax(pTempColl->GetAddPhyAck(nProfession),false);
			m_owner->ChangePDefence(pTempColl->GetAddPhydef(nProfession),false);
			m_owner->ChangeARate(pTempColl->GetAddDodge(nProfession),false);
			m_owner->ChangeCRate(pTempColl->GetAddCrit(nProfession),false);
			m_owner->ChangeBaserte(pTempColl->GetAddUng(nProfession),false);

			tempfightpower = BattleInfoLoader::GetInstance()->GetAllFight(
					pTempColl->GetAddPhyAck(nProfession),pTempColl->GetAddPhydef(nProfession),pTempColl->GetAddHp(nProfession),0,pTempColl->GetAddDodge(nProfession),pTempColl->GetAddCrit(nProfession),pTempColl->GetAddUng(nProfession));

			tempValue = m_owner->getFightingManage()->getFighting(eFightingGirlWar);
			tempResult =  tempValue - tempfightpower;
			m_owner->getFightingManage()->setFighting(eFightingGirlWar,tempResult);


			m_owner->ChangeHPMax(pColl->GetAddHp(nProfession));
			m_owner->ChangePhyAttackMax(pColl->GetAddPhyAck(nProfession));
			m_owner->ChangePDefence(pColl->GetAddPhydef(nProfession));
			m_owner->ChangeARate(pColl->GetAddDodge(nProfession));
			m_owner->ChangeCRate(pColl->GetAddCrit(nProfession));
			m_owner->ChangeBaserte(pColl->GetAddUng(nProfession));

			tempfightpower = BattleInfoLoader::GetInstance()->GetAllFight(
					pColl->GetAddPhyAck(nProfession),pColl->GetAddPhydef(nProfession),pColl->GetAddHp(nProfession),0,pColl->GetAddDodge(nProfession),pColl->GetAddCrit(nProfession),pColl->GetAddUng(nProfession));

			tempValue = m_owner->getFightingManage()->getFighting(eFightingGirlWar);
			tempResult =  tempValue + tempfightpower;
			m_owner->getFightingManage()->setFighting(eFightingGirlWar,tempResult);
		}

		vecType.push_back(eCharHP);
		vecType.push_back(eCharHPMax);
		vecType.push_back(eCharPhyAttackMax);
		vecType.push_back(eCharPhyDefence);
		vecType.push_back(eCharAvoidRate);
		vecType.push_back(eCrackDownRate);
		vecType.push_back(eAvoidCrackRate);
		vecType.push_back(eFightPower);

		m_owner->SynCharAttribute(vecType);
	}
}

//获得指定战姬緾绵所增加的属性
void GirlWarManager::getDesignationLingeringAttList(Smart_Ptr<GirlWar> grilWar,map<ECharAttrType,int64>& attList)
{
	//2016-4-4 取消出战星阶加成
//	if(grilWar->GetIsWar())
//	{
		const sGirlWarStarrank *pStar = GirlWarLoader::GetInstance()->GetGirlWarStarrank(grilWar->GetID(), grilWar->GetStarrank());
		if(pStar)
		{
			attList[eCharHPMax] 	   += (int64)(pStar->nAddOwerHp);
			attList[eCharPhyAttackMax] += (int64)(pStar->nAddOwerPhyAck);
			attList[eCharPhyDefence]   += (int64)(pStar->nAddOwerPhydef);
			attList[eCharAvoidRate]   += (int64)(pStar->nAddOwerDodge);
			attList[eCrackDownRate]   += (int64)(pStar->nAddOwerCrit);
			attList[eAvoidCrackRate]   += (int64)(pStar->nAddOwerUng);
		}
//	}

	return ;
}


int GirlWarManager::GetStarrank()
{
	int ret = 0;
	map<int64 ,Smart_Ptr<GirlWar> >::iterator itr = m_collection.begin();
	for(; itr != m_collection.end(); itr++)
	{
		ret += itr->second->GetStarrank();
	}

	return ret;
}

void GirlWarManager::SetStarrankByOrder(int nId,int nStar)
{
	Smart_Ptr<GirlWar> girlWar;
	GetGirlWarById(nId,girlWar);
	if(!girlWar)
	{
		return;
	}

	girlWar->SetStarrank(nStar);
}

void GirlWarManager::SetQualityByOrder(int nId,int Quality)
{
	Smart_Ptr<GirlWar> girlWar;
	GetGirlWarById(nId,girlWar);
	if(!girlWar)
	{
		return;
	}

	girlWar->SetQuality(Quality);
}

void GirlWarManager::SetLvByOrder(int nId,int nLv)
{
	Smart_Ptr<GirlWar> girlWar;
	GetGirlWarById(nId,girlWar);
	if(!girlWar)
	{
		return;
	}

	girlWar->getAttManage()->setValue(eCharLv,nLv);
}

//给某个战姬加或减属性
void GirlWarManager::alterClanAtt(CreatureObj& object,bool isAdd)
{
	const vector<int> tSkillList = m_owner->GetSkillMgr()->getClanSkillID();
	for(size_t i = 0;i < tSkillList.size();++i)
	{
		SkillInfo* tInfo = SkillLoader::GetInstance()->GetSkillInfoByID(tSkillList[i]);
		if(tInfo == NULL)
		{
			continue;
		}

		if(tInfo->limitPro != ClanSkillType_GirlWar)
		{
			continue;
		}

		SkillMgr::ResultEffect(&object,tInfo,isAdd,false);
	}
	
	return ;
}

int GirlWarManager::IsCanGetGirlWar(int nId)
{
	const GirlWarNeed *pNeed = GirlWarLoader::GetInstance()->GetGirlWarNeed(nId);
	if(!pNeed)
	{
		return eGirlDataFail;
	}

	if(IsHave(nId))
	{
		return ePetExist;
	}

	if(pNeed->nLv > 0 && pNeed->nLv > m_owner->getAttManage()->getValue<int>(eCharLv))
	{
		return eLVNotEnough;
	}

	Smart_Ptr<ArticleBaseMgr> pack = m_owner->GetContainer(ePackageType);
	if(!pack)
	{
		return ePlayerPackageFail;
	}

	if(pNeed->nPropId > 0 && pNeed->nPropLv > pack->GetGoodsNum(pNeed->nPropId))
	{
		return eItemNotEnough;
	}

	return 0;
}

int GirlWarManager::GetGirlWarQuality(int nQuality)
{
	int ret = 0;
	map<int64 ,Smart_Ptr<GirlWar> >::iterator itr = m_collection.begin();
	for(; itr != m_collection.end(); itr++)
	{
		if(itr->second->GetQuality() >= nQuality)
		{
			ret++;
		}
	}

	return ret;
}
