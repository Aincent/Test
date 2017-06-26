/*
 * Destination.cpp
 *
 *  Created on: 2014年11月27日
 *      Author: root
 */
#include "Destination.h"
#include "DestinationLoader.h"
#include "Log/Logger.h"
#include "../Container/ContainerBase.h"
#include "../Object/Player/Player.h"
#include "../Object/Monster/Monster.h"
#include "MessageCommonRet.h"
#include "../QuickMessage.h"
#include "../ServerEventDefine.h"
#include "../EventSet/EventDefine.h"
#include "../FileLoader/MallLoader.h"
#include "../Horse/HorseManager.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../DanLu/DanLuMgr.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "../Object/Creature.h"
#include "../Object/AI/AIBrain.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Fighting/Fighting.h"
#include "../HeartMagic/HeartMagicMgr.h"
#include "../Magic/SMagicMgr.h"
#include "../Object/GirlWar/GirlWarManager.h"
#include "../Container/ArticleEquipMgr.h"

Destination::Destination(Player* player):m_owner(player)
{
	ReInit();
}

Destination::~Destination()
{
	this->ReleaseDestination();
}

void Destination::ReInit()
{
	if(!m_lvEvent.IsValid())
		m_lvEvent = RegEvent(m_owner, PLAYER_LEVEL_UP_EVENT_TYPE, this, &Destination::HandlePlayerAdvanceLevel);

	if(!m_BuyItemFromMallEvent.IsValid())
		m_BuyItemFromMallEvent = RegEvent(m_owner, PLAYER_BUYITEM_FROMMALL, this, &Destination::HandlePlayerBuyItemFromMall);

	if(!m_HorseUpdateEvent.IsValid())
		m_HorseUpdateEvent = RegEvent(m_owner, PLAYER_HORSE_UPDATE, this, &Destination::HandlePlayerHorseUpdate);

	if(!m_MagicWeaponUpdateEvent.IsValid())
		m_MagicWeaponUpdateEvent = RegEvent(m_owner, PLAYER_WEAPON_MAGICWEAPON_ADVANCE, this, &Destination::HandlePlayerMagicWeaponUpdate);

	if(!m_MeridianUpdateEvent.IsValid())
		m_MeridianUpdateEvent = RegEvent(m_owner, PLAYER_MERIDIAN_UPDATE, this, &Destination::HandlePlayerMeridianUpdate);

	if(!m_ComposeEvent.IsValid())
		m_ComposeEvent = RegEvent(m_owner, PLAYER_SMELT_COMPOSE, this, &Destination::HandlePlayerCompose);

	if(!m_PassInstanceEvent.IsValid())
		m_PassInstanceEvent = RegEvent(m_owner, PLAYER_PASS_INSTANCE, this, &Destination::HandlePlayerPassInstance);

	if(!m_KillWorldBossEvent.IsValid())
		m_KillWorldBossEvent = RegEvent(m_owner, MONSTER_KILLED_EVENT_TYPE, this, &Destination::HandlePlayerKillWorldBoss);

	if(!m_RefreshMagicWeaponSkillEvent.IsValid())
		m_RefreshMagicWeaponSkillEvent = RegEvent(m_owner, PLAYER_REFRESH_MAGICWEAPON_SKILL, this, &Destination::HandlePlayerRefreshMagicWeaponSkill);

	if(!m_StrengthEquipEvent.IsValid())
		m_StrengthEquipEvent = RegEvent(m_owner, PLAYER_SMELT_STRONG, this, &Destination::HandlePlayerStrengthEquip);

	if(!m_FightPowerEvent.IsValid())
		m_FightPowerEvent = RegEvent(m_owner, PLAYER_FIGHT_POWER_CHANGE, this, &Destination::HandlePlayerFightPowerChange);

	if(!m_ExchangeJobluckyEvent.IsValid())
		m_ExchangeJobluckyEvent = RegEvent(m_owner, PLAYER_JOBLUCKY_EXCHANGE, this, &Destination::HandlePlayerExchangeJoblucky);

	if(!m_ChannelUpdateEvent.IsValid())
		m_ChannelUpdateEvent = RegEvent(m_owner, PLAYER_CHANNEL_UPDATE, this, &Destination::HandlePlayerChannelUpdate);

	if(!m_GenenateEscortEvent.IsValid())
		m_GenenateEscortEvent = RegEvent(m_owner, PLAYER_GENENATE_ESCORT, this, &Destination::HandlePlayerGenenateEscort);

	if(!m_TurnLuckTableEvent.IsValid())
		m_TurnLuckTableEvent = RegEvent(m_owner, PLAYER_TURN_LUCKY_TABLE, this, &Destination::HandlePlayerTurnLuckTable);

	if(!m_RankingChangeEvent.IsValid())
		m_RankingChangeEvent = RegEvent(m_owner, PLAYER_RANKING_CHANGE, this, &Destination::HandlePlayerRankingChange);

	if(!m_ClanWarEvent.IsValid())
		m_ClanWarEvent = RegEvent(m_owner, PLAYER_WIN_CLAN_WAR, this, &Destination::HandlePlayerClanWar);

	if(!m_SkillUpgradeEvent.IsValid())
		m_SkillUpgradeEvent = RegEvent(m_owner, PLAYER_SKILL_UPGRADE, this, &Destination::HandlePlayerSkillUpgrade);

	if(!m_VipEvent.IsValid())
		m_VipEvent = RegEvent(m_owner, PLAYER_GET_VIP_GIFT, this, &Destination::HandlePlayerGetVipGift);

	if(!m_areaOverEvent.IsValid())
		m_areaOverEvent = RegEvent(m_owner, PLAYER_ARENA_OVER, this, &Destination::HandleAreaOver);

	if(!m_SMagicWeaponUpdateEvent.IsValid())
		m_SMagicWeaponUpdateEvent = RegEvent(m_owner, PLAYER_WEAPON_SMAGICWEAPON_ADVANCE, this, &Destination::HandlePlayerSMagicWeaponUpdate);

	if(!m_HeartMagicUpdateEvent.IsValid())
		m_HeartMagicUpdateEvent = RegEvent(m_owner, PLAYER_WEAPON_HEARTMAGIC_ADVANCE, this, &Destination::HandlePlayerHeartMagicUpdate);

	if(!m_insertStoneEvent.IsValid())
		m_insertStoneEvent = RegEvent(m_owner, PLAYER_SMELT_INSERT, this, &Destination::HandlePlayerInsertStone);

	if(!m_AddGirlWarEvent.IsValid())
		m_AddGirlWarEvent = RegEvent(m_owner, PLAYER_GIRLWAR_ADD, this, &Destination::HandlePlayerAddGirlWar);

	if(!m_QualityGirlWarEvent.IsValid())
		m_QualityGirlWarEvent = RegEvent(m_owner, PLAYER_GIRLWAR_QUALITY, this, &Destination::HandlePlayerQualityGirlWar);

	if(!m_beKilledEvent.IsValid())
		m_beKilledEvent = RegEvent(m_owner, ADD_SIN_EVENT_TYPE, this,&Destination::HandlePlayerBeKilled);

	if(!m_wearEquipEvent.IsValid())
		m_wearEquipEvent = RegEvent(m_owner, PLAYER_WEAR_EQUIP, this, &Destination::HandlePlayerWearEquip);

	mDestinationIDMap.clear();
}

void Destination::ReleaseDestination()
{
	mDestinationIDMap.clear();

	if(m_lvEvent.IsValid())
	{
		m_lvEvent.SetNull();
	}
	if(m_BuyItemFromMallEvent.IsValid())
	{
		m_BuyItemFromMallEvent.SetNull();
	}
	if(m_HorseUpdateEvent.IsValid())
	{
		m_HorseUpdateEvent.SetNull();
	}
	if(m_MagicWeaponUpdateEvent.IsValid())
	{
		m_MagicWeaponUpdateEvent.SetNull();
	}
	if(m_MeridianUpdateEvent.IsValid())
	{
		m_MeridianUpdateEvent.SetNull();
	}

	if(m_ComposeEvent.IsValid())
	{
		m_ComposeEvent.SetNull();
	}
	if(m_PassInstanceEvent.IsValid())
	{
		m_PassInstanceEvent.SetNull();
	}
	if(m_KillWorldBossEvent.IsValid())
	{
		m_KillWorldBossEvent.SetNull();
	}
	if(m_RefreshMagicWeaponSkillEvent.IsValid())
	{
		m_RefreshMagicWeaponSkillEvent.SetNull();
	}
	if(m_StrengthEquipEvent.IsValid())
	{
		m_StrengthEquipEvent.SetNull();
	}

	if(m_FightPowerEvent.IsValid())
	{
		m_FightPowerEvent.SetNull();
	}
	if(m_ExchangeJobluckyEvent.IsValid())
	{
		m_ExchangeJobluckyEvent.SetNull();
	}
	if(m_ChannelUpdateEvent.IsValid())
	{
		m_ChannelUpdateEvent.SetNull();
	}
	if(m_GenenateEscortEvent.IsValid())
	{
		m_GenenateEscortEvent.SetNull();
	}
	if(m_TurnLuckTableEvent.IsValid())
	{
		m_TurnLuckTableEvent.SetNull();
	}

	if(m_RankingChangeEvent.IsValid())
	{
		m_RankingChangeEvent.SetNull();
	}
	if(m_ClanWarEvent.IsValid())
	{
		m_ClanWarEvent.SetNull();
	}
	if(m_SkillUpgradeEvent.IsValid())
	{
		m_SkillUpgradeEvent.SetNull();
	}
	if(m_VipEvent.IsValid())
	{
		m_VipEvent.SetNull();
	}
	if(m_areaOverEvent.IsValid())
	{
		m_areaOverEvent.SetNull();
	}
	if(m_SMagicWeaponUpdateEvent.IsValid())
	{
		m_SMagicWeaponUpdateEvent.SetNull();
	}
	if(m_HeartMagicUpdateEvent.IsValid())
	{
		m_HeartMagicUpdateEvent.SetNull();
	}
	if(m_insertStoneEvent.IsValid())
	{
		m_insertStoneEvent.SetNull();
	}
	if(m_AddGirlWarEvent.IsValid())
	{
		m_AddGirlWarEvent.SetNull();
	}
	if(m_QualityGirlWarEvent.IsValid())
	{
		m_QualityGirlWarEvent.SetNull();
	}
	if(m_beKilledEvent.IsValid())
	{
		m_beKilledEvent.SetNull();
	}
	if(m_wearEquipEvent.IsValid())
	{
		m_wearEquipEvent.SetNull();
	}
}

void Destination::GetDestinationInfo(int destinationtype,std::vector<ServerReturn::ServerThreeInt>& list)
{
	DestinationMap::iterator it = mDestinationIDMap.begin();

	for(; it != mDestinationIDMap.end(); ++it)
	{
		if(it->second.destinationType == destinationtype)
		{
			ServerReturn::ServerThreeInt ret;
			ret.set_retf(it->first);
			ret.set_rets((it->second).hadGet);
			ret.set_rett((it->second).num);
			list.push_back(ret);
		}
	}
}

void Destination::GetDestinationInfo(int destinationId,PlayerInfo::DestinationContent& content)
{
	DestinationMap::iterator it = mDestinationIDMap.find(destinationId);
	if(it != mDestinationIDMap.end())
	{
		it->second.SetProto(content);
	}
}

bool Destination::AddPassDestination(int destinationId, int param,int param1)
{
	DestinationMap::iterator it = mDestinationIDMap.find(destinationId);
	if(it == mDestinationIDMap.end())
		return false;
	sDestinationEntry *entry = DestinationLoader::GetInstance()->GetDestinationEntry(destinationId);
	if(!(bool)entry)
		return false;
	int destinationType = it->second.destinationType;
	if(it->second.isComplete)
		return true;
	bool iscomplete = false;
	//根据传入的param直接判断
	if(destinationType == DestinationType_Level ||
	   destinationType == DestinationType_HorseLevel ||
	   destinationType == DestinationType_MagicWeaponLevel ||
	   destinationType == DestinationType_DanLuLevel ||
	   destinationType == DestinationTyp_EquipStar ||
	   destinationType == DestinationTyp_TotalAttackValue ||
	   destinationType == DestinationTyp_Meridian ||
	   destinationType == DestinationType_SMagicWeaponLevel ||
	   destinationType == DestinationType_HeartMagicLevel ||
	   destinationType == DestinationType_AddGirlWar ||
	   destinationType == DestinationType_EquipWear)
	{
		if((DWORD)param >= entry->i_targetnumber[0])
		{
			iscomplete = true;
		}
	}
	//竞技场排名小于多少名
	else if(destinationType == DestinationType_ArenaRank)
	{
		if((DWORD)param <= entry->i_targetnumber[0])
		{
			iscomplete = true;
		}
	}
	//达到某个特定的目标
	else if(destinationType == DestinationType_Instance)
	{
		//此时param为副本ID
		if((DWORD)param == entry->i_targetnumber[1])
		{
			uint tempnum = it->second.num + 1;
			it->second.num = tempnum;
			m_owner->SetDataFlag(eDestinationInfo);
			if(tempnum >= entry->i_targetnumber[0])
			{
				iscomplete = true;
			}
		}
	}
	else if(destinationType == DestinationTyp_Escort) //X级的镖车
	{
		if((DWORD)param == entry->i_targetnumber[0])
		{
			iscomplete = true;
		}
	}
	else if (destinationType == DestinationType_Synthetic)
	{
		iscomplete = true;
	}
	else if(destinationType == DestinationType_SkillLevel ||
			destinationType == DestinationTyp_JobLuckyLeval ||
			destinationType == DestinationType_EquipInsertStone ||
			destinationType == DestinationType_GirlWarLevel)
	{
		//拥有param个到达param1级的技能 DestinationType_SkillLevel
		//拥有品质param，param1级的官印 DestinationTyp_JobLuckyLeval
		if((DWORD)param == entry->i_targetnumber[0] && (DWORD)param1 >= entry->i_targetnumber[1])
		{
			iscomplete = true;
		}
	}
	//累加的判断
	else
	{
		int tempnum = it->second.num + param;
		it->second.num = tempnum;
		m_owner->SetDataFlag(eDestinationInfo);
		if((DWORD)tempnum >= entry->i_targetnumber[0])
		{
			iscomplete = true;
		}
	}

	if(iscomplete)
	{
		it->second.num = entry->i_targetnumber[0];
		//推送游戏目标达成消息
		if(m_owner->IsOpenSystem(eStGameObject))
		{
			ServerReturn::ServerRetInt toClient;
			int mess = entry->i_achID;
			toClient.set_ret(mess);
			QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(),&toClient,MSG_SIM_DESTINATIONCOMPLETE);
		}

		it->second.destinationID = entry->i_achID;
		it->second.isComplete = true;
		it->second.hadGet = DestinationUseType_2;

		PassTargetArgs arg(m_owner->GetMyself(), entry->i_achID);
		m_owner->FireEvent(PLAYER_PASS_TARGET, arg);


		m_owner->SetDataFlag(eDestinationInfo);
		return true;
	}

	return false;
}

void Destination::UpdateDestinationInfo(int destinationtype, int param,int param1)
{
	std::vector<DWORD> ids;
	DestinationLoader::GetInstance()->GetSpecifiedDestinationID(destinationtype,ids);
	for(size_t i = 0;i < ids.size();++i)
	{
		DWORD destinationID = ids[i];
		if(hasGetDestination(destinationID) == false)
		{
			AddPassDestination(destinationID,param,param1);
			//当前阶段没完成，后面的更高级的阶段就不用判断了，更容易的没完成，更难的肯定没完成 bool isComplete =
//			if(isComplete == false)
//				break;
		}
	}
}

bool Destination::hasGetDestination(uint destinationId)
{
	DestinationMap::iterator it = mDestinationIDMap.find(destinationId);
	if(it != mDestinationIDMap.end())
	{
		if(it->second.isComplete != 1)
			return false;
		else
			return true;
	}

	return false;
}

int Destination::DrawDestination(uint destinationId)
{
	DestinationMap::iterator it = mDestinationIDMap.find(destinationId);

	if(it != mDestinationIDMap.end())
	{
		if(it->second.isComplete == 0)
		{
			return eDestinationNotComplete;
		}
		if(it->second.hadGet != DestinationUseType_3)
		{
			int ret = -1;
			vector<int> vec;
			sDestinationEntry *entry = DestinationLoader::GetInstance()->GetDestinationEntry(destinationId);
			if(entry)
			{
				vector<int> vecPos;
				ret = m_owner->GetContainer(ePackageType)->AddItem(entry->i_rewardID, entry->i_rewardCount, vecPos,1);
			}

			LOG_DEBUG(FILEINFO,"DrawDestination playerID %lld error %d",m_owner->GetID(),ret);

			if(ret == 0)
			{
				it->second.hadGet = DestinationUseType_3;
				m_owner->SetDataFlag(eDestinationInfo);

//				StatisticInfo::CostAndGotInfo costAndGotInfo;
//				if(entry->i_rewardID == DWORD(GameParaLoader::GetInstance()->getExchangeForceItemId()) && entry->i_rewardCount > 0)
//				{
//					StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//					StatisticMgr::AddItemGotInfo(gotInfo,eCharForce,entry->i_rewardCount);
//					if(eAccountType_User == m_owner->GetAccountType())
//						StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Force,eStaMinor_Force_Destination,&costAndGotInfo);
//				}
//				else if(entry->i_rewardID == DWORD(GameParaLoader::GetInstance()->getExchangeMoneyItemId()) && entry->i_rewardCount > 0)
//				{
//					StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//					StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,entry->i_rewardCount);
//					if(eAccountType_User == m_owner->GetAccountType())
//						StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Money,eStaMinor_Money_Destination,&costAndGotInfo);
//				}
//				else if(entry->i_rewardID == DWORD(GameParaLoader::GetInstance()->getExchangeBindGoldenItemId()) && entry->i_rewardCount > 0)
//				{
//					StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//					StatisticMgr::AddItemGotInfo(gotInfo,eCharBindGolden,entry->i_rewardCount);
//					if(eAccountType_User == m_owner->GetAccountType())
//						StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_BindGolden,eStaMinor_BindGolden_Destination,&costAndGotInfo);
//				}

				DrawTargetArgs arg(m_owner->GetMyself(), entry->i_achID);
				m_owner->FireEvent(PLAYER_DRAW_TARGETREWARD, arg);
//				if (m_owner->IsOpenSystem(eStGameObject))
//				{
//					m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_GameTarget, eUIHLSmall_GameTarget_CanGetReward), 1, false);
//				}
			}
			else
				return ret;
		}
		else
			return eDestinationHasGotted;
	}
	else
		return eDestinationNotExist;

	return 0;
}

void Destination::SetDestinationToDB(PlayerInfo::DestinationInfoList* InfoList)
{
	PlayerInfo::DestinationInfo *info = InfoList->mutable_info();
	if(!info)
	{
		return;
	}

	DestinationMap::iterator itMap = mDestinationIDMap.begin();

	for(; itMap != mDestinationIDMap.end(); ++itMap)
	{
		PlayerInfo::DestinationContent * destinationContent = info->add_destinationinfos();
		itMap->second.SetProto(*destinationContent);
	}
}

int Destination::GetUnGetRewardNum()
{
	int count = 0;
	for (DestinationMap::iterator it = mDestinationIDMap.begin(); it != mDestinationIDMap.end(); ++it)
	{
		sDestinationContent& dest = it->second;
		if (0 == dest.isComplete)
		{
			continue;
		}
		if (dest.hadGet != DestinationUseType_3)
		{
			count++;
		}
	}
	return count;
}

void Destination::GetStageDestinationInfo(int destinationStage,std::vector<ServerReturn::ServerThreeInt>& list)
{
	std::vector<DWORD> ids;
	DestinationLoader::GetInstance()->GetStageDestinationID(destinationStage,ids);

	for(DWORD i = 0;i < ids.size();++i)
	{
		DestinationMap::iterator it = mDestinationIDMap.find(ids[i]);
		if(it != mDestinationIDMap.end())
		{
			ServerReturn::ServerThreeInt ret;
			ret.set_retf(it->first);
			ret.set_rets((it->second).hadGet);
			ret.set_rett((it->second).num);
			list.push_back(ret);
		}
	}
}

void Destination::GetHaveRewardDestination(std::vector<int> &list)
{
	DestinationMap::iterator it = mDestinationIDMap.begin();
	for(;it!=mDestinationIDMap.end();++it)
	{
		if(it->second.isComplete == 1 && it->second.hadGet == 1)
		{
			list.push_back(it->second.destinationID);
		}
	}
}

// 获取每阶段可领取个数
void Destination::GetPerStageCanGetAward(std::map<int, int>& get)
{
	for (DestinationMap::const_iterator it = mDestinationIDMap.begin(); it != mDestinationIDMap.end(); ++it)
	{
		sDestinationEntry* entry = DestinationLoader::GetInstance()->GetDestinationEntry(it->first);
		if (NULL == entry)
		{
			continue;
		}

		if (DestinationUseType_2 == it->second.hadGet)
		{
			get[entry->i_stage]++;
		}
	}
}

void Destination::InitDestinationInfo(const PlayerInfo::DestinationInfoList& infoList)
{
	m_owner->SetInitPartDataFlag(eDestinationInfo);

	mDestinationIDMap.clear();
	PlayerInfo::DestinationInfo info = infoList.info();
	//玩家创建时数据库还没有游戏目标数据，用表格初始化
	std::vector<DWORD> ids;
	DestinationLoader::GetInstance()->GetAllDestinationID(ids);
	for(uint i = 0; i < ids.size(); i++)
	{
		sDestinationContent destinationInfo;
		sDestinationEntry *entry = DestinationLoader::GetInstance()->GetDestinationEntry(ids[i]);
		if(!entry)
		{
			LOG_ERROR(FILEINFO, "Get Player Destination Form error");
			continue;
		}
		if(ids[i] <= 0)
			continue;

		destinationInfo.destinationID = ids[i];							//游戏目标ID
		destinationInfo.destinationType = entry->i_target;	//游戏目标类型
		destinationInfo.isComplete = 0;									//初始化还没有完成
		destinationInfo.hadGet = DestinationUseType_1;				//游戏目标领取状态初始化
		destinationInfo.num = 0;											//目标完成量
		mDestinationIDMap[ids[i]] = destinationInfo;
	}

	int dbsize = info.destinationinfos_size();

	for(int i = 0;i < dbsize;++i)
	{
		PlayerInfo::DestinationContent *destinationInfo = info.mutable_destinationinfos(i);
		int destinationId = destinationInfo->destinationid();
		//如果删除了表中的某个游戏目标，该游戏目标将舍弃不再存入数据库
		DestinationMap::iterator it = mDestinationIDMap.find(destinationId);
		if(it != mDestinationIDMap.end())
		{
			//如果修改了游戏目标的类型,将舍弃已有的数据，用最新的表里的数据初始化
			if(it->second.destinationType == destinationInfo->destinationtype())
			{
				sDestinationContent item = sDestinationContent(destinationInfo);
				mDestinationIDMap[destinationInfo->destinationid()] = item;
			}
		}
	}
}

bool Destination::HandlePlayerAdvanceLevel(const EventArgs & e)
{
	this->UpdateDestinationInfo(DestinationType_Level,m_owner->getAttManage()->getValue<int>(eCharLv),0);
	return true;
}

bool Destination::HandlePlayerBuyItemFromMall(const EventArgs & e)
{
	BuyItemEventArgs* data = (BuyItemEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_pObj;
	if(player && player->GetID() == m_owner->GetID())
	{
//		if(data->m_type == MallType_Bind)
			UpdateDestinationInfo(DestinationTyp_ShoppingCount,1,0);
	}

	return true;
}

bool Destination::HandlePlayerHorseUpdate(const EventArgs & e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationType_HorseLevel,m_owner->GetHorseManager()->GetId());
	}

	return true;
}

bool Destination::HandlePlayerMagicWeaponUpdate(const EventArgs & e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationType_MagicWeaponLevel,m_owner->GetMagicWeapon()->GetMagicWeapon());
	}

	return true;
}

bool Destination::HandlePlayerMeridianUpdate(const EventArgs & e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		int duLuId = -1;
		int exp = 0;
		m_owner->GetDanLuManager()->GetDanLuInfo(&duLuId,&exp);
		UpdateDestinationInfo(DestinationType_DanLuLevel,duLuId);
	}

	return true;
}

bool Destination::HandlePlayerCompose(const EventArgs & e)
{
	ComposeArgs* data = (ComposeArgs*)(&e);

	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;

	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationType_Synthetic,data->m_nItemQuality);
	}

	return true;
}

bool Destination::HandlePlayerPassInstance(const EventArgs & e)
{
	PlayerInstancePassEventArgs* data = (PlayerInstancePassEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationType_Instance,data->m_mapId);
	}

	return true;
}

bool Destination::HandlePlayerKillWorldBoss(const EventArgs & e)
{
	TwoParmaEventArgs* data = (TwoParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Monster> mon = data->m_myself;
	if(mon)
	{
		if(mon->GetQuality() == eWorldMonster)
		{
			map<int64, int> enemyValue;
			mon->GetBrain()->GetEnemyValue(enemyValue);
			map<int64, int>::iterator it = enemyValue.begin();
			for(; it != enemyValue.end(); ++it)
			{
				Smart_Ptr<Player> player;
				PlayerMessage::GetInstance()->GetPlayerByCharid(it->first,player);
				if(player)
				{
						player->GetDestination()->UpdateDestinationInfo(DestinationTyp_KillBoss,1);
				}
			}
		}
	}

	return true;
//	TwoParmaEventArgs* data = (TwoParmaEventArgs*)(&e);
//	if(!data)
//	{
//		return true;
//	}
//
//	if(data->m_other->GetType() != ePlayer && data->m_other->GetType() != ePet)
//		return true;
//
//	Smart_Ptr<Player>  player;
//	Smart_Ptr<GirlWar>  girlwar;
//
//	if(data->m_other->GetType() == ePet)
//	{
//		girlwar = data->m_other;
//
//		player  = girlwar->GetOwner();
//	}
//	else
//	{
//		player = data->m_other;
//	}
//
//	Smart_Ptr<Monster> mon = data->m_myself;
//	if(player && mon && player->GetID() == m_owner->GetID())
//	{
//		if(mon->GetQuality() == eWorldMonster)
//		{
//			UpdateDestinationInfo(DestinationTyp_KillBoss,1);
//		}
//	}

//	return true;
}

bool Destination::HandlePlayerRefreshMagicWeaponSkill(const EventArgs & e)
{
	ReflushMagicWeaponSkillArgs* data = (ReflushMagicWeaponSkillArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationTyp_RefreshMagicWeaponCount,1);
	}

	return true;
}

bool Destination::HandlePlayerStrengthEquip(const EventArgs & e)
{
	StrengthEquipArgs* data = (StrengthEquipArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationTyp_EquipStar,data->m_nEquipUpgrade);
	}

	return true;
}

bool Destination::HandlePlayerFightPowerChange(const EventArgs & e)
{
	PowerParmaEventArgs* data = (PowerParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	//铜钱副本的战力改变不算成就
	if(eFightMoneyInstance == data->m_type)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationTyp_TotalAttackValue,player->getAttManage()->getValue<int>(eFightPower));
	}

	return true;
}

bool Destination::HandlePlayerExchangeJoblucky(const EventArgs & e)
{
	ExchangeJobluckyArgs* data = (ExchangeJobluckyArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationTyp_JobLuckyLeval,data->m_nQuality,data->m_nLv);
	}

	return true;
}

bool Destination::HandlePlayerChannelUpdate(const EventArgs & e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationTyp_Meridian,player->GetAcupoint());
	}

	return true;
}

bool Destination::HandlePlayerGenenateEscort(const EventArgs & e)
{
	GenenateEscortArgs* data = (GenenateEscortArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationTyp_Escort,data->m_nQuality);
	}

	return true;
}

bool Destination::HandlePlayerTurnLuckTable(const EventArgs & e)
{
	TurnLuckyTableArgs* data = (TurnLuckyTableArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationType_TurnedLuckyTableCount,data->m_nNum);
	}

	return true;
}

bool Destination::HandlePlayerRankingChange(const EventArgs & e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationType_ArenaRank,player->getAttManage()->getValue<int>(eRanking));
	}

	return true;
}

bool Destination::HandlePlayerClanWar(const EventArgs & e)
{
	ClanWarArgs* data = (ClanWarArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID() && data->m_bWin == 1)
	{
		UpdateDestinationInfo(DestinationType_GroupWinCount,1);
	}

	return true;
}

bool Destination::HandlePlayerSkillUpgrade(const EventArgs & e)
{
	SkillUpgradeArgs* data = (SkillUpgradeArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		int count = player->GetSkillMgr()->GetspecifiedSkillLvCount(data->m_nLv);
		UpdateDestinationInfo(DestinationType_SkillLevel,count,data->m_nLv);
	}

	return true;
}

bool Destination::HandlePlayerGetVipGift(const EventArgs & e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationType_GetVipGiftCount,1);
	}

	return true;
}

bool Destination::HandleAreaOver(const EventArgs & e)
{
	ArenaOverArgs* data = (ArenaOverArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_Owner;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationType_ArenaOver,1);
	}

	return true;
}

bool Destination::HandlePlayerSMagicWeaponUpdate(const EventArgs & e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationType_SMagicWeaponLevel,m_owner->GetSMagic()->GetMagicWeapon());
	}

	return true;
}

bool Destination::HandlePlayerHeartMagicUpdate(const EventArgs & e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationType_HeartMagicLevel,m_owner->GetHeartMagicWeapon()->GetMagicWeapon());
	}

	return true;
}

bool Destination::HandlePlayerInsertStone(const EventArgs & e)
{
	UseItemEventArgs* data = (UseItemEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_pObj;
	if(player && player->GetID() == m_owner->GetID())
	{
		Smart_Ptr<ArticleEquipMgr> equipMgr= m_owner->GetContainer(eEquipQuickType);
		if(bool(equipMgr) == false)
			return true;

		const GoodsInfo* info =	GoodsLoader::GetInstance()->GetItemDataByID(data->m_id);
		if(info==NULL)
			return true;

		DWORD stoneCount = equipMgr->GetStoneCount(info->useLvReq);
		if(stoneCount <= 0)
			return true;

		UpdateDestinationInfo(DestinationType_EquipInsertStone,info->useLvReq,stoneCount);
	}

	return true;
}

bool Destination::HandlePlayerAddGirlWar(const EventArgs & e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationType_AddGirlWar,player->GetGirlWarManager()->GetGirlWarNum());
	}

	return true;
}

bool Destination::HandlePlayerQualityGirlWar(const EventArgs & e)
{
	GetPetEventArgs* data = (GetPetEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_pObj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdateDestinationInfo(DestinationType_GirlWarLevel,data->m_value,player->GetGirlWarManager()->GetGirlWarQuality(data->m_value));
	}

	return true;
}

bool Destination::HandlePlayerBeKilled(const EventArgs& e)
{
	OneParmaEventArgs *Args = (OneParmaEventArgs *) (&e);
	Smart_Ptr<CreatureObj> obj = Args->m_obj;//杀人的玩家
	vector<int> vecType;
	if(obj)
	{
		if(obj->GetType() == ePlayer)
		{
			Smart_Ptr<Player> player = obj;

			if(player && player->GetID() != m_owner->GetID())
			{
				return true;
			}

			/*增加  在某些地图中  杀人 是否  增加 红名的 判断*/
			int mapID = GET_MAP_ID(player->GetMapID());
			if(InstanceLoader::GetInstance()->IsNotAddSinValueMapID(mapID))
			{
				return true;
			}

			UpdateDestinationInfo(DestinationType_PlayerDead,1);
		}
	}
	return true;
}

bool Destination::HandlePlayerWearEquip(const EventArgs& e)
{
	WearEquipArgs* data = (WearEquipArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(data->m_nEquipId);
		if(ginfo!=NULL && ginfo->itemQuality >=4)
		{
			int legendEquipCount = 0;
			int legendAndFullStrengthEquip = 0;
			Smart_Ptr<ArticleEquipMgr> equipMgr= m_owner->GetContainer(eEquipQuickType);
			if(bool(equipMgr) == false)
				return true;
			equipMgr->GetLegendEquipCount(legendEquipCount,legendAndFullStrengthEquip);
			UpdateDestinationInfo(DestinationType_EquipWear,legendEquipCount);
		}
	}
	return true;
}
