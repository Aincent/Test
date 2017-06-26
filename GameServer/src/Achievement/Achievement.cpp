/*
 * Achievement.cpp
 *
 *  Created on: 2014年6月19日
 *      Author: root
 */
#include <math.h>
#include "Achievement.h"
#include "util.h"
#include "Timer/TimerInterface.h"
#include "../Map/ConfigLoader/MapConfig.h"
#include "Log/Logger.h"
#include "../Container/ContainerBase.h"
#include "../Object/Player/Player.h"
#include "../Object/Player/PlayerMessage.h"
#include "MessageCommonRet.h"
#include "MessageStruct/AchievementProto.pb.h"
#include "../QuickMessage.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "../QuestNew/QuestLoader.h"
#include "../Skill/SkillLoader/SkillLoader.h"
#include "../ServerEventDefine.h"
#include "../EventSet/EventDefine.h"
#include "../Object/Monster/Monster.h"
#include "../Horse/HorseManager.h"
#include "FileLoader/GoodsLoader.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../MapInstance/InstanceMapLoader.h"
#include "../Container/ArticleEquipMgr.h"
#include "FightDefined.h"
#include "../Attribute/AttributeCreature.h"
#include "./FileLoader/GameParaLoader.h"
#include "../Fighting/Fighting.h"
#include "../StatisticMgr/StatisticHandle.h"

Achievement::Achievement(Player * player):m_time(0),m_owner(player)
{
	InitEvent();
}

Achievement::~Achievement()
{
	this->ReleaseAchievement();
}

void Achievement::InitEvent()
{
	if(!m_lvEvent.IsValid())
		m_lvEvent = RegEvent(m_owner, PLAYER_LEVEL_UP_EVENT_TYPE, this, &Achievement::HandlePlayerAdvanceLevel);

	if(!m_killEvent.IsValid())
		m_killEvent = RegEvent(m_owner, MONSTER_KILLED_EVENT_TYPE, this, &Achievement::HandlePlayerKillMonster);

	if(!m_submitQuestEvent.IsValid())
		m_submitQuestEvent = RegEvent(m_owner, PLAYER_QUEST_SUBMITQUEST, this, &Achievement::HandlePlayerSubmitTask);//提交任务

	if(!m_passInstanceEvent.IsValid())
		m_passInstanceEvent = RegEvent(m_owner, PLAYER_PASS_INSTANCE, this, &Achievement::HandlePlayerPassInstance);

	if(!m_channelUpdateEvent.IsValid())
		m_channelUpdateEvent = RegEvent(m_owner, PLAYER_CHANNEL_UPDATE, this, &Achievement::HandlePlayerChannelUpdate);

	if(!m_strengthEquipEvent.IsValid())
		m_strengthEquipEvent = RegEvent(m_owner, PLAYER_SMELT_STRONG, this, &Achievement::HandlePlayerStrengthEquip);

	//洗练
	if(!m_smeltEvent.IsValid())
		m_smeltEvent = RegEvent(m_owner, PLAYER_SMELT_SMELT, this, &Achievement::HandlePlayerSmelt);

	if(!m_beKilledEvent.IsValid())
		m_beKilledEvent = RegEvent(m_owner, ADD_SIN_EVENT_TYPE, this,&Achievement::HandlePlayerBeKilled);

	if(!m_fightPowerEvent.IsValid())
		m_fightPowerEvent = RegEvent(m_owner, PLAYER_FIGHT_POWER_CHANGE, this, &Achievement::HandlePlayerFightPowerChange);

	if(!m_skillUpgradeEvent.IsValid())
		m_skillUpgradeEvent = RegEvent(m_owner, PLAYER_SKILL_UPGRADE, this, &Achievement::HandlePlayerSkillUpgrade);

	if(!m_signInEvent.IsValid())
		m_signInEvent = RegEvent(m_owner, PLAYER_SIGNIN_EVENT_TYPE, this, &Achievement::HandlePlayerSignIn);

	if(!m_horseUpdateEvent.IsValid())
		m_horseUpdateEvent = RegEvent(m_owner, PLAYER_HORSE_UPDATE, this, &Achievement::HandlePlayerHorseUpdate);

	if(!m_enterClanEvent.IsValid())
		m_enterClanEvent = RegEvent(m_owner, PLAYER_CLAN_ENTERCLAN, this, &Achievement::HandlePlayerEnterClan);

	if(!m_clanWarEvent.IsValid())
		m_clanWarEvent = RegEvent(m_owner, PLAYER_WIN_CLAN_WAR, this, &Achievement::HandlePlayerClanWar);

	if(!m_insertStoneEvent.IsValid())
		m_insertStoneEvent = RegEvent(m_owner, PLAYER_SMELT_INSERT, this, &Achievement::HandlePlayerInsertStone);

	if(!m_wearEquipEvent.IsValid())
		m_wearEquipEvent = RegEvent(m_owner, PLAYER_WEAR_EQUIP, this, &Achievement::HandlePlayerWearEquip);

	if(!m_magicWeaponUpdateEvent.IsValid())
		m_magicWeaponUpdateEvent = RegEvent(m_owner, PLAYER_WEAPON_MAGICWEAPON_ADVANCE, this, &Achievement::HandlePlayerMagicWeaponUpdate);

	if(!m_refreshMagicWeaponSkillEvent.IsValid())
		m_refreshMagicWeaponSkillEvent = RegEvent(m_owner, PLAYER_REFRESH_MAGICWEAPON_SKILL, this, &Achievement::HandlePlayerRefreshMagicWeaponSkill);

	if(!m_clanUpdateEvent.IsValid())
		m_clanUpdateEvent = RegEvent(m_owner, PLAYER_CLAN_UPGRADE, this, &Achievement::HandlePlayerClanUpdate);

	if(!m_oneKeyCompleteEvent.IsValid())
		m_oneKeyCompleteEvent = RegEvent(m_owner, PLAYER_ONE_KEY_COMPLETE, this, &Achievement::HandlePlayerOneKeyComplete);
}

void Achievement::ReInit()
{
	m_time = 0;
	m_shopInfo.clear();
	mAchievementIDMap.clear();
	mRecentAchievement.clear();

	InitEvent();
}

void Achievement::ReleaseAchievement()
{
	m_time = 0;
	mAchievementIDMap.clear();
	mRecentAchievement.clear();
	m_shopInfo.clear();

	if(m_lvEvent.IsValid())
	{
		m_lvEvent.SetNull();
	}
	if(m_killEvent.IsValid())
	{
		m_killEvent.SetNull();
	}
	if(m_submitQuestEvent.IsValid())
	{
		m_submitQuestEvent.SetNull();
	}
	if(m_passInstanceEvent.IsValid())
	{
		m_passInstanceEvent.SetNull();
	}
	if(m_channelUpdateEvent.IsValid())
	{
		m_channelUpdateEvent.SetNull();
	}

	if(m_strengthEquipEvent.IsValid())
	{
		m_strengthEquipEvent.SetNull();
	}
	if(m_smeltEvent.IsValid())
	{
		m_smeltEvent.SetNull();
	}
	if(m_beKilledEvent.IsValid())
	{
		m_beKilledEvent.SetNull();
	}
	if(m_fightPowerEvent.IsValid())
	{
		m_fightPowerEvent.SetNull();
	}
	if(m_skillUpgradeEvent.IsValid())
	{
		m_skillUpgradeEvent.SetNull();
	}

	if(m_signInEvent.IsValid())
	{
		m_signInEvent.SetNull();
	}
	if(m_horseUpdateEvent.IsValid())
	{
		m_horseUpdateEvent.SetNull();
	}
	if(m_enterClanEvent.IsValid())
	{
		m_enterClanEvent.SetNull();
	}
	if(m_clanWarEvent.IsValid())
	{
		m_clanWarEvent.SetNull();
	}
	if(m_insertStoneEvent.IsValid())
	{
		m_insertStoneEvent.SetNull();
	}

	if(m_wearEquipEvent.IsValid())
	{
		m_wearEquipEvent.SetNull();
	}
	if(m_magicWeaponUpdateEvent.IsValid())
	{
		m_magicWeaponUpdateEvent.SetNull();
	}
	if(m_refreshMagicWeaponSkillEvent.IsValid())
	{
		m_refreshMagicWeaponSkillEvent.SetNull();
	}
	if(m_clanUpdateEvent.IsValid())
	{
		m_clanUpdateEvent.SetNull();
	}
	if(m_oneKeyCompleteEvent.IsValid())
	{
		m_oneKeyCompleteEvent.SetNull();
	}
}

void Achievement::SetPrestigeToDB(PlayerInfo::PrestigeInfoList *InfoList)
{
	PlayerInfo::PrestigeInfo *info = InfoList->mutable_info();
	if(!info)
	{
		return;
	}

	PlayerInfo::DBAchShopInfo *shop = InfoList->mutable_shop();

	if(!shop)
	{
		return;
	}

	shop->set_time(m_time);
	map<int,sAchShopInfo>::iterator itShop = m_shopInfo.begin();
	for(;itShop != m_shopInfo.end();itShop++)
	{
		PlayerInfo::DBAchShopItem *item = shop->add_item();
		item->set_count(itShop->second.count);
		item->set_id(itShop->second.id);
		for(uint i = 0 ; i < itShop->second.goodsid.size(); i++)
		{
			item->add_goodsid(itShop->second.goodsid[i]);
		}
	}

	AchievementMap::iterator itMap = mAchievementIDMap.begin();

	for(itMap = mAchievementIDMap.begin(); itMap != mAchievementIDMap.end(); ++itMap)
	{
		PlayerInfo::PrestigeContent * prestigeContent = info->add_prestigeinfos();
		prestigeContent->set_prestigetype(itMap->second.achievementType);
		prestigeContent->set_prestigeid(itMap->second.achievementId);
		prestigeContent->set_num(itMap->second.achievementPercent);
		prestigeContent->set_iscomplete(itMap->second.isComplete);
		prestigeContent->set_hadget(itMap->second.UseType);
	}

	//添加最近完成的成就
	AccumulateGetList::iterator itRecent = mRecentAchievement.begin();

	for(itRecent = mRecentAchievement.begin(); itRecent != mRecentAchievement.end(); ++itRecent)
	{
		PlayerInfo::PrestigeContent * prestigeContent = info->add_prestigeinfos();
		prestigeContent->set_prestigetype(itRecent->achievementType);
		prestigeContent->set_prestigeid(itRecent->achievementId);
		prestigeContent->set_num(itRecent->achievementPercent);
		prestigeContent->set_iscomplete(itRecent->isComplete);
		prestigeContent->set_hadget(itRecent->UseType);
	}
}

void Achievement::InitPrestigeInfo(const PlayerInfo::PrestigeInfoList &infoList)
{
	mAchievementIDMap.clear();
	mRecentAchievement.clear();
	PlayerInfo::PrestigeInfo info = infoList.info();
	PlayerInfo::DBAchShopInfo shop = infoList.shop();
	mAchievementIDMap = AchievementLoader::GetInstance()->getAchievementIdMap();
	for(int i = 0;i < info.prestigeinfos_size();++i)
	{
		PlayerInfo::PrestigeContent *prestigeInfo = info.mutable_prestigeinfos(i);
		if(!prestigeInfo)
			continue;

		AchievementMap::iterator it = mAchievementIDMap.find(prestigeInfo->prestigeid());
		if(it != mAchievementIDMap.end())
		{
			sPrestigeContent item(prestigeInfo);
			//获取最近获得的成就
			if(prestigeInfo->prestigetype() == AchievementType_5)
			{
				mRecentAchievement.push_back(item);
				item.achievementType = it->second.achievementType;
			}

			mAchievementIDMap[prestigeInfo->prestigeid()] = item;
		}
	}

	m_owner->SetDataFlag(ePrestigeInfo);

	if(shop.time() < PlayerMessage::GetInstance()->GetEightHoursTime() || shop.item_size() == 0)
	{
		AchievementLoader::GetInstance()->UpdataShop(m_shopInfo);
		m_time = PlayerMessage::GetInstance()->GetEightHoursTime();
	}
	else
	{
		bool isKey = CUtil::IsSameDay(m_owner->GetOffLineTime());
		m_time = shop.time();
		for(int i = 0 ;i < shop.item_size() ; i++)
		{
			sAchShopInfo item;

			if(isKey)
				item.count = shop.item(i).count();
			else
				item.count = 0;
			item.id = shop.item(i).id();
			for(int j = 0 ; j < shop.item(i).goodsid_size() ; j++)
			{
				item.goodsid.push_back(shop.item(i).goodsid(j));
			}
			m_shopInfo[item.id] = item;
		}
	}
}
void Achievement::SetRetAchShopInfo(AchievementProto::RetAchShopInfo *info)
{

	info->set_time(m_time);
	map<int,sAchShopInfo>::iterator it = m_shopInfo.begin();
	for(;it!=m_shopInfo.end();it++)
	{
		AchievementProto::RetAchShopItem *item = info->add_item();
		item->set_count(it->second.count);
		item->set_id(it->second.id);
		for(uint i = 0 ; i < it->second.goodsid.size() ; i++)
		{
			item->add_goodsid(it->second.goodsid[i]);
		}
	}
}
bool Achievement::AddPassPrestige(int prestigeId, int param)
{
	AchievementMap::iterator it = mAchievementIDMap.find(prestigeId);
	if(it == mAchievementIDMap.end())
		return false;

	if(it->second.isComplete)
		return true;

	bool ret = false;

	sAchievementEntry *entry = AchievementLoader::GetInstance()->GetAchievementEntry(prestigeId);
	if(entry)
	{


		bool isComplete = false;
		//根据param直接判断是否完成成就
		if(entry->i_target == AchievementType_Level ||
		   entry->i_target == AchievementType_ChannelUpdate ||
		   entry->i_target == AchievementType_Strength ||
		   entry->i_target == AchievementType_SinValue ||
		   entry->i_target == AchievementType_FightPower ||
		   entry->i_target == AchievementType_HorseUpgrade ||
		   entry->i_target == AchievementType_WearLegendEquip ||
		   entry->i_target == AchievementType_WearStrengthLegendEquip ||
		   entry->i_target == AchievementType_ChallengeInstance ||
		   entry->i_target == AchievementType_MagicWeaponAdvance ||
		   entry->i_target == AchievementType_ClanUpgrade ||
		   entry->i_target == AchievementType_SetStoneCount)
		{
			if(param >= entry->i_targetnumber)
				isComplete = true;
			else
			{
				if((DWORD)param > it->second.achievementPercent)
				{
					it->second.achievementPercent = param;
					m_owner->SetDataFlag(ePrestigeInfo);
				}
			}
		}
		//根据param直接相等判断
		else if(entry->i_target == AchievementType_CompleteQuest)
		{
			if(param == entry->i_targetnumber)
				isComplete = true;
		}
		//根据param来累积判断否完成成就
		else
		{
			if(it->second.achievementPercent < (DWORD)entry->i_targetnumber)
			{
				int tempnum = it->second.achievementPercent;
				if(param != 0)
				{
					tempnum += param;
				}
				else
				{
					tempnum += 1;
				}

				it->second.achievementPercent = tempnum;
				m_owner->SetDataFlag(ePrestigeInfo);
				if(tempnum >= entry->i_targetnumber)
				{
					isComplete = true;
				}
			}
		}

		if(isComplete)
		{
			BEHAVIOR_MONI(m_owner->GetMyself(), BehaviorType::Main::eAchievement, BehaviorType::Minor::eAchieve_Pass, MONITOR);

			it->second.achievementPercent = entry->i_targetnumber;
			//推送成就达成消息
			ServerReturn::ServerRetInt toClient;
			int mess = entry->i_achID;
			toClient.set_ret(mess);
			//toClient.set_completeid(entry->i_achID);
			//toClient.set_completetype(0);			//完成的类型 0成就 1称号
			QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(),&toClient,MSG_SIM_PRESTIGECOMPLETE);

			it->second.achievementId = entry->i_achID;
			it->second.isComplete = true;
			it->second.UseType = AchievementUseType_2;

			//添加最近完成的
			sPrestigeContent recent;
//			if(mRecentAchievement.size() >= 3)
//			{
//				mRecentAchievement.pop_back();
//			}
			recent = it->second;
			recent.achievementType = AchievementType_5;
			mRecentAchievement.insert(mRecentAchievement.begin(),recent);

			//添加声望
			if(entry->i_prestige > 0)
			{
				std::vector<int> vecType;
				m_owner->AddPrestige((int)entry->i_prestige);
				vecType.push_back(ePrestige);

				m_owner->ChangeCredit((int)entry->i_prestige,true);
				vecType.push_back(eCredit);

//				StatisticInfo::CostAndGotInfo costAndGotInfo;
//				StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo,eCredit,(int)entry->i_prestige);
//				StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Credit,eStaMinor_Credit_Achievement,&costAndGotInfo);

				m_owner->SynCharAttribute(vecType);
			}
			m_owner->SetDataFlag(eBaseInfo);

			//判断成就是否有称号
			if(entry->i_titleID > 0)
			{
				//激活称号
				m_owner->GetTitle()->addTitle(entry->i_titleID);
				//客户端会根据成就ID寻找对应的称号ID，所以不需要再推送
//				if(0 == ret)
//				{
//					//推送获得称号消息
//					ServerReturn::ServerRetInt title;
//					int64 i = 1;
//					int64 vaule = i<<32 | entry->i_titleID;
//					title.set_ret(vaule);
//
//					QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(),&title,MSG_SIM_PRESTIGECOMPLETE);
//				}
			}
			//判断成就值触发的称号
			//UPdatePrestigeValueTitle(m_owner->GetPrestige());

			m_owner->SetDataFlag(ePrestigeInfo);

			//统计获得 成就点
//			this->StatisticAchievement();

			m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Achivement, eUIHLSmall_Acheivement_HaveUnGetReward),
					1, true);

			ret = true;

//			
		}


	}


	return ret;
}

void Achievement::GetPrestigeInfo(AchievementType prestigetype, std::vector<ServerReturn::ServerThreeInt>& list)
{
	AchievementMap::iterator it = mAchievementIDMap.begin();

	if(prestigetype == AchievementType_5)
	{
		AccumulateGetList::iterator itRecent = mRecentAchievement.begin();
		for(;itRecent != mRecentAchievement.end();itRecent++)
		{
			if((*itRecent).UseType != AchievementUseType_2)
				continue;

			ServerReturn::ServerThreeInt ret;
			ret.set_retf((*itRecent).achievementId);
			ret.set_rets((*itRecent).UseType);
			ret.set_rett((*itRecent).achievementPercent);
			list.push_back(ret);
		}
		return;
	}

	for(it = mAchievementIDMap.begin(); it != mAchievementIDMap.end(); ++it)
	{
		if(it->second.achievementType == prestigetype)
		{
			ServerReturn::ServerThreeInt ret;
			ret.set_retf(it->first);
			ret.set_rets((it->second).UseType);
			ret.set_rett((it->second).achievementPercent);
			list.push_back(ret);
		}
	}

	ServerReturn::ServerThreeInt retType;
	retType.set_retf(prestigetype);
	retType.set_rets(0);
	retType.set_rett(0);
	list.push_back(retType);
}

bool Achievement::hasGetAchievement(uint achieveId)
{
	AchievementMap::iterator it = mAchievementIDMap.find(achieveId);
	if(it != mAchievementIDMap.end())
	{
		if(it->second.isComplete != 1)
			return false;
		else
			return true;
	}

	return false;
}

bool Achievement::DrawAchievement(uint achieveId, int &errorType)
{
	AchievementMap::iterator it = mAchievementIDMap.find(achieveId);
	AccumulateGetList::iterator recentIt = mRecentAchievement.begin();


	if(it != mAchievementIDMap.end())
	{
		if(it->second.UseType != AchievementUseType_3)
		{
			it->second.UseType = AchievementUseType_3;

			for(;recentIt != mRecentAchievement.end();recentIt++)
			{
				uint recentId = recentIt->achievementId;
				if(recentId == achieveId)
				{
					*recentIt = it->second;
					recentIt->achievementType = AchievementType_5;
					//recentIt->set_hadget(AchievementUseType_3);
				}
			}

			vector<int> vec;
			sAchievementEntry *entry = AchievementLoader::GetInstance()->GetAchievementEntry(achieveId);
			if(entry)
			{
				m_owner->ChangeMoney(entry->i_money,true);
				vec.push_back(eCharMoney);

//				StatisticInfo::CostAndGotInfo costAndGotInfo;
//				if(entry->i_money > 0)
//				{
//					StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//					StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,entry->i_money);
//				}

				m_owner->AddExp(entry->i_exp);

//				if(entry->i_exp > 0)
//				{
//					StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//					StatisticMgr::AddItemGotInfo(gotInfo,eCharExp,entry->i_exp);
//				}

				m_owner->ChangeCurForce(entry->i_zhenqi,true);
				vec.push_back(eCharForce);

//				if(entry->i_zhenqi > 0)
//				{
//					StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//					StatisticMgr::AddItemGotInfo(gotInfo,eCharForce,entry->i_zhenqi);
//				}

				m_owner->ChangeBindGolden(entry->i_BGolden,true);
				vec.push_back(eCharBindGolden);

//				if(entry->i_BGolden > 0)
//				{
//					StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//					StatisticMgr::AddItemGotInfo(gotInfo,eCharBindGolden,entry->i_BGolden);
//				}

//				if(eAccountType_User == m_owner->GetAccountType())
//					StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Reward,eStaMinor_Reward_Achievement,&costAndGotInfo);
			}

			if(vec.size() > 0)
			{
				m_owner->SynCharAttribute(vec);
			}

			m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Achivement, eUIHLSmall_Acheivement_HaveUnGetReward), 1, false);

			m_owner->SetDataFlag(ePrestigeInfo);
			errorType = 0;
			return true;
		}

		if(it->second.UseType == AchievementUseType_3)
		{
			errorType = ePrestigeHadGet;
			return false;
		}
	}

	errorType = ePrestigeIDError;
	return false;
}

void Achievement::GetAchievementTypePercent(std::vector<ServerReturn::ServerRetInt> &info)
{
	uint achiTotal[AchievementType_4] = {0,0,0,0};			//
	uint achiComplete[AchievementType_4] = {0,0,0,0};

	AchievementMap::iterator it = mAchievementIDMap.begin();
	for(; it != mAchievementIDMap.end(); ++it)
	{
		achiTotal[it->second.achievementType]++;
		if(it->second.isComplete > 0)
		{
			achiComplete[it->second.achievementType-1]++;
		}
	}

	for(uint j = 0; j < AchievementType_4; j++)
	{
		int64 achiInfo = achiComplete[j];
		achiInfo = (achiInfo << 32) | (j+1);
		ServerReturn::ServerRetInt ret;
		ret.set_ret(achiInfo);
		info.push_back(ret);
	}
}

//void Achievement::StatisticAchievement()
//{
//	if (m_owner->GetMyself())
//	{
//		StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(), eStatic_Achievement, eStaMinor_Achievement_GetPrestige, 1);
//	}
//}

void Achievement::UpdatePrestigeInfo(int type, int param)
{
	std::vector<DWORD> ids;
	AchievementLoader::GetInstance()->GetAchievementTypeIDs(type,ids);
	for(uint i = 0;i < ids.size();++i)
	{
		DWORD id = ids[i];
		if(hasGetAchievement(id) == false)
		{

			AddPassPrestige(id,param);
			//当前阶段没完成，后面的更高级的阶段就不用判断了，更容易的没完成，更难的肯定没完成
//			if(isComplete == false)
//				break;


		}
	}
}

void Achievement::UPdatePrestigeValueTitle(int pretigeValue)
{
	if(pretigeValue <= 1)
		return;

	map<DWORD, sTitleEntry> allTitleInfo = AchievementLoader::GetInstance()->GetTitleInfo();
	map<DWORD, sTitleEntry>::iterator it = allTitleInfo.begin();
	for(;it!=allTitleInfo.end();++it)
	{
		sTitleEntry entry = it->second;
		if(entry.i_prestige <= 1)
			continue;

		if((DWORD)pretigeValue >= entry.i_prestige)
		{
			//title模块会判断是否已经拥有此称号
			int ret = m_owner->GetTitle()->addTitle(entry.i_titleID);
			if(0 == ret)
			{
				//推送获得称号消息
				ServerReturn::ServerRetInt title;
				int64 i = 1;
				int64 vaule = i<<32 | entry.i_titleID;
				title.set_ret(vaule);

				QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(),&title,MSG_SIM_PRESTIGECOMPLETE);
			}
		}
	}
}

void Achievement::GetPrestigeInfo(int prestigeId,PlayerInfo::PrestigeContent& content)
{
}

bool Achievement::HandlePlayerAdvanceLevel(const EventArgs& e)
{
	UpdatePrestigeInfo(AchievementType_Level,m_owner->getAttManage()->getValue<int>(eCharLv));
	return true;
}

bool Achievement::HandlePlayerKillMonster(const EventArgs& e)
{
	TwoParmaEventArgs* data = (TwoParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	if(data->m_other->GetType() != ePlayer && data->m_other->GetType() != ePet)
		return true;

	Smart_Ptr<Player>  player;
	Smart_Ptr<GirlWar>  girlwar;

	if(data->m_other->GetType() == ePet)
	{
		girlwar = data->m_other;

		player  = girlwar->GetOwner();
	}
	else
	{
		player = data->m_other;
	}

	Smart_Ptr<Monster> mon = data->m_myself;
	if(player && mon && player->GetID() == m_owner->GetID())
	{
		if(mon->GetQuality() == eWorldMonster)
		{
			UpdatePrestigeInfo(AchievementTyp_KillBoss);
		}
		else if(mon->GetQuality() == eEscortCar)
		{
			UpdatePrestigeInfo(AchievementType_RobEscort);
		}
		UpdatePrestigeInfo(AchievementType_KillMonster);
	}

	return true;
}

bool Achievement::HandlePlayerSubmitTask(const EventArgs& e)
{
	PlayerAcceptQuestEventArgs* data = (PlayerAcceptQuestEventArgs*)(&e);
	if(!data)
	{
		return false;
	}

	Smart_Ptr<Player> player = data->m_obj;
	Entry_Quest *entry = QuestLoader::GetInstance()->GetQuestInfo(data->m_questid);
	if(entry && player)
	{
		UpdatePrestigeInfo(AchievementType_Quest);
		UpdatePrestigeInfo(AchievementType_CompleteQuest,data->m_questid);
		if(entry->m_nMainType == QuestType_Functional)
			UpdatePrestigeInfo(AchievementType_DailyQuest);
		if(entry->m_nMainType == QuestType_Clan)
			UpdatePrestigeInfo(AchievementType_ClanQuest);
	}

	return true;
}

bool Achievement::HandlePlayerPassInstance(const EventArgs& e)
{
	PlayerInstancePassEventArgs* data = (PlayerInstancePassEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdatePrestigeInfo(AchievementType_Instance);
		const int First_Challenge_MapID = GameParaLoader::GetInstance()->getFirstChallengeMapId();
		if(data->m_mapId >= First_Challenge_MapID && data->m_mapId <= First_Challenge_MapID + 14)
		{
			int instanceIndex = data->m_mapId - First_Challenge_MapID + 1;
			UpdatePrestigeInfo(AchievementType_ChallengeInstance,instanceIndex);
		}
	}

	return true;
}

bool Achievement::HandlePlayerChannelUpdate(const EventArgs& e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdatePrestigeInfo(AchievementType_ChannelUpdate,player->GetAcupoint());
	}

	return true;
}

bool Achievement::HandlePlayerStrengthEquip(const EventArgs& e)
{
	StrengthEquipArgs* data = (StrengthEquipArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdatePrestigeInfo(AchievementType_Strength,data->m_nEquipUpgrade);
	}

	return true;
}

bool Achievement::HandlePlayerBeKilled(const EventArgs& e)
{
	OneParmaEventArgs *Args = (OneParmaEventArgs *) (&e);
	Smart_Ptr<CreatureObj> obj = Args->m_obj;//杀人的玩家
	vector<int> vecType;
	if(obj)
	{
		if(obj->GetType() == ePlayer)
		{
			Smart_Ptr<Player> player = obj;

			/*增加  在某些地图中  杀人 是否  增加 红名的 判断*/
			int mapID = GET_MAP_ID(player->GetMapID());
			if(InstanceLoader::GetInstance()->IsNotAddSinValueMapID(mapID))
			{
				return true;
			}

			player->GetAchievement()->UpdatePrestigeInfo(AchievementType_KillPlayers);

			if(m_owner->GetSinState() != ERS_RED_NAME)
			{
				player->GetAchievement()->UpdatePrestigeInfo(AchievementType_SinValue,int(player->getAttManage()->getValue<float>(eSinValue))+100);
			}
		}
	}
	return true;
}

bool Achievement::HandlePlayerFightPowerChange(const EventArgs& e)
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
		UpdatePrestigeInfo(AchievementType_FightPower,player->getAttManage()->getValue<int>(eFightPower));
	}

	return true;
}

bool Achievement::HandlePlayerSkillUpgrade(const EventArgs& e)
{
	SkillUpgradeArgs* data = (SkillUpgradeArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID() && 10 == data->m_nLv)
	{
		SkillInfo * skillInfo = SkillLoader::GetInstance()->GetSkillInfoByID(data->m_nSkillId);
		if(0 == skillInfo)
			return true;
		bool isActiveSkill = false;
		if(skillInfo->fType != ePassiveSkill)
			isActiveSkill = true;
		if(isActiveSkill)
			UpdatePrestigeInfo(AchievementType_ActiveSKill);
		else
			UpdatePrestigeInfo(AchievementType_PassiveSkill);
	}

	return true;
}

bool Achievement::HandlePlayerSignIn(const EventArgs& e)
{
	if(m_owner)
	{
		UpdatePrestigeInfo(AchievementType_SignIn);
	}
	return true;
}

bool Achievement::HandlePlayerHorseUpdate(const EventArgs& e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdatePrestigeInfo(AchievementType_HorseUpgrade,m_owner->GetHorseManager()->GetId());
	}

	return true;
}

bool Achievement::HandlePlayerEnterClan(const EventArgs& e)
{
	OneParmaEventArgs *tArgs = (OneParmaEventArgs *)(&e);
	Smart_Ptr<Player> player = tArgs->m_obj;
	if(!player)
		return true;

	if(player->GetID() == m_owner->GetID())
	{
		UpdatePrestigeInfo(AchievementType_ClanPeopleCount);
	}
	return true;
}

bool Achievement::HandlePlayerClanWar(const EventArgs& e)
{
	ClanWarArgs* data = (ClanWarArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID() && data->m_bWin == 1)
	{
		UpdatePrestigeInfo(AchievementType_ClanWarWinCount);
	}

	return true;
}

bool Achievement::HandlePlayerInsertStone(const EventArgs& e)
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

		DWORD stoneCount = equipMgr->GetStoneCount(0);
		if(stoneCount <= 0)
			return true;

		UpdatePrestigeInfo(AchievementType_SetStoneCount,stoneCount);
	}

	return true;
}

bool Achievement::HandlePlayerWearEquip(const EventArgs& e)
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
			UpdatePrestigeInfo(AchievementType_WearLegendEquip,legendEquipCount);
			UpdatePrestigeInfo(AchievementType_WearStrengthLegendEquip,legendAndFullStrengthEquip);
		}
	}
	return true;
}

bool Achievement::HandlePlayerMagicWeaponUpdate(const EventArgs& e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdatePrestigeInfo(AchievementType_MagicWeaponAdvance,m_owner->GetMagicWeapon()->GetMagicWeapon());
	}

	return true;
}

bool Achievement::HandlePlayerRefreshMagicWeaponSkill(const EventArgs& e)
{
	ReflushMagicWeaponSkillArgs* data = (ReflushMagicWeaponSkillArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		SkillInfo * skillInfo = SkillLoader::GetInstance()->GetSkillInfoByID(data->m_nSkillId);
		if(0 == skillInfo)
			return true;
		if(skillInfo->quality >= 4)
			UpdatePrestigeInfo(AchievementType_ReflushMagicWeaponSkill);
	}

	return true;
}

bool Achievement::HandlePlayerSmelt(const EventArgs& e)
{
	SmeltArgs* data = (SmeltArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		if(data->m_nLegendAttrCount > 0)
			UpdatePrestigeInfo(AchievementType_Xilian,data->m_nLegendAttrCount);
	}

	return true;
}

bool Achievement::HandlePlayerClanUpdate(const EventArgs& e)
{
	ClanUpgradeArgs* data = (ClanUpgradeArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		UpdatePrestigeInfo(AchievementType_ClanUpgrade,data->m_nClanLv);
	}

	return true;
}

bool Achievement::HandlePlayerOneKeyComplete(const EventArgs& e)
{
	OneKeyCompleteArgs* data = (OneKeyCompleteArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		Entry_Quest *info = QuestLoader::GetInstance()->GetQuestInfo(data->m_eQuestID);
		if(!info)
			return true;

		UpdatePrestigeInfo(AchievementType_Quest,data->m_nTimes);
		if(info->m_nMainType == QuestType_Functional)
			UpdatePrestigeInfo(AchievementType_DailyQuest,data->m_nTimes);
		else if(info->m_nMainType == QuestType_Clan)
			UpdatePrestigeInfo(AchievementType_ClanQuest,data->m_nTimes);
	}

	return true;
}
//购买成就商品
int Achievement::BugAchShopGoods(int type,int goodsid)
{
	sAchShopItem item;
	vector<int> vecType;
	vector<int> vecLock;

	if(m_owner->getAttManage()->getValue<int>(ePrestige) < AchievementLoader::GetInstance()->GetAchByType(type))
	{
		return eConNotMatch;
	}

	if(m_owner->GetContainer(ePackageType)->GetFreeSize() < 1)
	{
		return ePackageIsFull;
	}

	map<int,sAchShopInfo>::iterator it = m_shopInfo.find(type);

	if(it == m_shopInfo.end())
	{
		return eConNotMatch;
	}

	AchievementLoader::GetInstance()->GetsAchShopItem(type , item);

	if((uint)it->second.count >= (m_owner->getAttManage()->getValue<DWORD>(eVIPLv)/2)*2 + item.count)
	{
		return eAchievementCountsFull;
	}

	map<int,sAchGoodsItem>::iterator itGoods = item.goodsItem.find(goodsid);

	if(itGoods == item.goodsItem.end())
	{
		return eCant_find_item;
	}

	if(m_owner->getAttManage()->getValue<int>(eCharMoney) < itGoods->second.money)
	{
		return eNotEnoghMoney;
	}

	it->second.count++;

//	StatisticInfo::CostAndGotInfo costAndGotInfo;
//	StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
//	StatisticMgr::SetItemCostBefore(costItemInfo,eCharMoney,itGoods->second.money,m_owner->GetMyself());

	m_owner->ChangeMoney(itGoods->second.money, false);
	vecType.push_back(eCharMoney);
	m_owner->SynCharAttribute(vecType);

//	StatisticMgr::SetItemCostAfter(costItemInfo,eCharMoney,m_owner->GetMyself());

	vector<WORD> flagVec;
	vector<DWORD> itemVec;
	vector<DWORD> numVec;
	vector<DWORD> strengthLv;

	itemVec.push_back(itGoods->second.goodsid);
	numVec.push_back(1);
	strengthLv.push_back(0);
	flagVec.push_back(0);

	m_owner->GetContainer(ePackageType)->AddItem(itemVec,numVec,vecLock,flagVec,strengthLv,true,npc_fly_type);

//	StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo.add_gotlist();
//	StatisticMgr::AddItemGotInfo(gotItemInfo,itGoods->second.goodsid);
//	if(eAccountType_User == m_owner->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Achievement,eStaMinor_Achievement_MallBuyEvent,&costAndGotInfo);

	m_owner->SetDataFlag(ePrestigeInfo);
	return 0;
}
void Achievement::Updata()
{
	vector<int> vecType;
	if(m_owner->getAttManage()->getValue<int>(eCharGolden) < AchievementLoader::GetInstance()->GetNeedGolden())
	{
		return;
	}

	m_owner->ChangeGolden(AchievementLoader::GetInstance()->GetNeedGolden(), false);
	vecType.push_back(eCharGolden);
	m_owner->SynCharAttribute(vecType);


	AchievementLoader::GetInstance()->UpdataShop(m_shopInfo);

	m_owner->SetDataFlag(ePrestigeInfo);
}

int Achievement::GetAllUnGetReward()
{
	int count = 0;
	for (AchievementMap::iterator it = mAchievementIDMap.begin(); mAchievementIDMap.end() != it; ++it)
	{
		if (it->second.UseType == AchievementUseType_2)
		{
			count++;
		}
	}

	return count;
}
