/*
 * VitalityMgr.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: root
 */

#include "VitalityMgr.h"
#include "Log/Logger.h"
#include "VitalityLoader/VitalityLoader.h"
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
#include "FightDefined.h"
#include "../Object/Creature.h"
#include "../Object/AI/AIBrain.h"
#include "../Object/Player/PlayerMessage.h"

VitalityMgr::VitalityMgr(Player * player):m_owner(player)
{
	ReInit();
}

VitalityMgr::~VitalityMgr()
{
	Release();
}

void VitalityMgr::ReInit()
{
	m_Score = 0;
	m_time = 0;
	m_canGetMaxID = 0;

	m_allVitalityInfo.clear();
	m_isGetAward.clear();

	if(!m_signInEvent.IsValid())
		m_signInEvent = RegEvent(m_owner, PLAYER_SIGNIN_EVENT_TYPE, this, &VitalityMgr::HandlePlayerSignIn);

	if(!m_BuyItemFromMallEvent.IsValid())
		m_BuyItemFromMallEvent = RegEvent(m_owner, PLAYER_BUYITEM_FROMMALL, this, &VitalityMgr::HandlePlayerBuyItemFromMall);

	if(!m_equipInheritEvent.IsValid())
		m_equipInheritEvent = RegEvent(m_owner,PLAYER_EQUIP_INHERIT,this,&VitalityMgr::HandlePlayerEquipInheritEvent);

	if(!m_skillUpgradeEvent.IsValid())
		m_skillUpgradeEvent = RegEvent(m_owner, PLAYER_SKILL_UPGRADE, this, &VitalityMgr::HandlePlayerSkillUpgrade);

	if(!m_submitQuestEvent.IsValid())
		m_submitQuestEvent = RegEvent(m_owner, PLAYER_QUEST_SUBMITQUEST, this, &VitalityMgr::HandlePlayerSubmitTask);//提交任务

	if(!m_oneKeyCompleteEvent.IsValid())
		m_oneKeyCompleteEvent = RegEvent(m_owner, PLAYER_ONE_KEY_COMPLETE, this, &VitalityMgr::HandlePlayerOneKeyComplete);

	if(!m_girlWarLingeringEvent.IsValid())
		m_girlWarLingeringEvent = RegEvent(m_owner,PLAYER_GIRLWAR_LINGERING,this,&VitalityMgr::HandleGirlWarLingering);

	if(!m_TurnLuckTableEvent.IsValid())
		m_TurnLuckTableEvent = RegEvent(m_owner, PLAYER_TURN_LUCKY_TABLE, this, &VitalityMgr::HandlePlayerTurnLuckTable);

	if(!m_moneyShopEvent.IsValid())
		m_moneyShopEvent = RegEvent(m_owner, PLAYER_RECEIVE_MONEY_SHOP, this, &VitalityMgr::HandlePlayerMoneyShop);

	if(!m_passInstanceEvent.IsValid())
		m_passInstanceEvent = RegEvent(m_owner, PLAYER_PASS_INSTANCE, this, &VitalityMgr::HandlePlayerPassInstance);
}

void VitalityMgr::Release()
{
	if(m_signInEvent.IsValid())
	{
		m_signInEvent.SetNull();
	}

	if(m_BuyItemFromMallEvent.IsValid())
	{
		m_BuyItemFromMallEvent.SetNull();
	}

	if(m_equipInheritEvent.IsValid())
	{
		m_equipInheritEvent.SetNull();
	}

	if(m_skillUpgradeEvent.IsValid())
	{
		m_skillUpgradeEvent.SetNull();
	}

	if(m_submitQuestEvent.IsValid())
	{
		m_submitQuestEvent.SetNull();
	}

	if(m_oneKeyCompleteEvent.IsValid())
	{
		m_oneKeyCompleteEvent.SetNull();
	}

	if(m_girlWarLingeringEvent.IsValid())
	{
		m_girlWarLingeringEvent.SetNull();
	}

	if(m_TurnLuckTableEvent.IsValid())
	{
		m_TurnLuckTableEvent.SetNull();
	}

	if(m_moneyShopEvent.IsValid())
	{
		m_moneyShopEvent.SetNull();
	}

	m_allVitalityInfo.clear();
	m_isGetAward.clear();
}

//初始化活跃信息
void VitalityMgr::InitVitalityFromDB(const VitalityProto::ProtoVitalityInfo &info)
{
	if(info.item_size() == 0 || !CUtil::IsSameDay(info.time()))
	{
		ResetVitality();
	}
	else
	{
		for(BYTE i = 0; i < info.item_size();i++)
		{
			sVitalityInfo vitalinty;
			vitalinty.id = info.item(i).id();
			vitalinty.count = info.item(i).count();
			m_allVitalityInfo[vitalinty.id] = vitalinty;
		}
		for(BYTE i = 0; i < info.award_size();i++)
		{
			m_isGetAward[i+1] = info.award(i);
		}
		m_Score = info.score();
		m_time = info.time();
	}

	int awardID = VitalityLoader::GetInstance()->GetAwardIDByScore(m_Score);
	if (-1 != awardID)
	{
		SetCanGetMaxID(awardID);
	}
	else
	{
		SetCanGetMaxID(0);
	}

}
//保存活跃信息
void VitalityMgr::SetVitalityToDB(VitalityProto::ProtoVitalityInfo *info)
{
	info->set_score(m_Score);
	info->set_time(m_time);

	map<int, sVitalityInfo>::iterator it = m_allVitalityInfo.begin();

	for(;it!=m_allVitalityInfo.end();it++)
	{
		VitalityProto::ProtoVitalityItem *item = info->add_item();
		item->set_id(it->second.id);
		item->set_count(it->second.count);
	}

	map<int,bool>::iterator awardIt = m_isGetAward.begin();

	for(;awardIt!=m_isGetAward.end();awardIt++)
	{
		info->add_award(awardIt->second);
	}
}
//保存活跃信息
void VitalityMgr::SetVitality(VitalityProto::ProtoVitalityInfo *info)
{
	info->set_score(m_Score);

	map<int, sVitalityInfo>::iterator it = m_allVitalityInfo.begin();

	for(;it!=m_allVitalityInfo.end();it++)
	{
		VitalityProto::ProtoVitalityItem *item = info->add_item();
		item->set_id(it->second.id);
		item->set_count(it->second.count);
	}

	map<int,bool>::iterator awardIt = m_isGetAward.begin();

//	int count = VitalityLoader::GetInstance()->GetAwardCount(m_Score);  && i < count  int i = 0 ,i++,i++

	for(;awardIt!=m_isGetAward.end() ;awardIt++)
	{
		info->add_award(awardIt->second);
	}
}
//更新数据
void VitalityMgr::ResetVitality()
{
	m_Score = 0;
	SetCanGetMaxID(0);
	m_time = CUtil::GetNowSecond();
	m_allVitalityInfo.clear();
	m_isGetAward.clear();
	VitalityLoader::GetInstance()->UpdataVitality(m_allVitalityInfo,m_isGetAward);
	m_owner->SetDataFlag(eVitalityInfo);

	//有些模块已经升满了必须直接获得活跃度
	m_owner->SetVitality();
}

//是否领取过一个活跃度奖励
bool VitalityMgr::isTodayHaveReceiveOneReward()
{
	map<int,bool>::iterator tempIt = m_isGetAward.begin();
	for(;tempIt != m_isGetAward.end();++tempIt)
	{
		if(tempIt->second)
			return true;
	}

	return false;
}

//获取奖励
int VitalityMgr::GetAward(int id)
{
	vector<DWORD> goodsIdList;
	vector<DWORD> numList;
	vector<int> vecPos;
	vector<DWORD> strengthLvVec;
	vector<WORD> bindVec;
	vector<int> vecType;

	map<int,bool>::iterator it = m_isGetAward.find(id);
	if(it == m_isGetAward.end())
	{
		return eSystemError;
	}

	if(it->second)
	{
		return eReceived;
	}

	sVitalityAward *award = VitalityLoader::GetInstance()->GetVitalityAwardInfoByID(id);
	if(!award)
	{
		return eNull;
	}

	if(award->score > m_Score)
	{
		return eNotScore;
	}

//	StatisticInfo::CostAndGotInfo costAndGotInfo;
//
	for(DWORD i = 0; i < award->m_nAward.size();i++)
	{
		goodsIdList.push_back(award->m_nAward[i]);
		numList.push_back(award->m_nAwardNum[i]);
//
//		StatisticInfo::GotItemInfo * gotItem = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotItem,award->m_nAward[i],award->m_nAwardNum[i]);
	}

	Smart_Ptr<ArticleBaseMgr> pPacket = m_owner->GetContainer(ePackageType);
	if(!pPacket)
	{
		return eNull;
	}

	strengthLvVec.resize(goodsIdList.size(), 0);
	bindVec.resize(goodsIdList.size(), 1);

	//判断背包是否能领取
	if(pPacket->IsBagEnought(goodsIdList, numList,bindVec))
	{
		return ePackageFull;
	}

	int retcode = pPacket->AddItem(goodsIdList, numList, vecPos, bindVec,strengthLvVec,true,npc_fly_type);

	if(retcode == 1)
	{
		return  ePackageFull;
	}

	m_owner->ChangeBindGolden(award->golden);
	m_owner->ChangeMoney(award->money,true);

//	if(award->golden > 0)
//	{
//		StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCharGolden,award->golden);
//	}
//
//	if(award->money > 0)
//	{
//		StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,award->money);
//	}
//
//	if(eAccountType_User == m_owner->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Reward,eStaMinor_Reward_Vitality,&costAndGotInfo);

	vecType.push_back(eCharMoney);
	vecType.push_back(eCharBindGolden);

	m_owner->SetDataFlag(eBaseInfo);
	m_owner->SynCharAttribute(vecType);

	it->second = true;
	m_owner->SetDataFlag(eVitalityInfo);

	PlayerReceiveActivity tempArgs(m_owner->GetMyself(),id);
	m_owner->FireEvent(PLAYER_RECEIVE_ACTIVITY,tempArgs);

	// sm_owner->UpdateAtiveHighLight(eHL_ActivityDegree, 1, false, false);
	m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Vitality, eUIHLSmall_Vitelity_UnGetReward), 1, false);

	return 0;
}

int VitalityMgr::GetVitality(EVitalityType type)
{
	int id = type;

	map<int,sVitalityInfo>::iterator it = m_allVitalityInfo.find(id);
	if (it != m_allVitalityInfo.end())
	{
		return it->second.count;
	}

	return 0;
}

//完成活跃度
void VitalityMgr::FinshVitality(EVitalityType type,int count)
{
	int id = type;

	map<int,sVitalityInfo>::iterator it = m_allVitalityInfo.find(id);
	if (it != m_allVitalityInfo.end())
	{
		sVitality *vi = VitalityLoader::GetInstance()->GetVitalityInfoByID(id);
		if (NULL != vi)
		{
			if(it->second.count < vi->count)
			{
				VitalityProto::ProtoVitalitySim toPlayer;

				it->second.count += count;
				if(it->second.count >= vi->count)
				{
					m_Score += vi->score;
					it->second.count = vi->count;
					int awardID = VitalityLoader::GetInstance()->GetAwardIDByScore(m_Score);
					// 是否有新的奖励
					if (GetCanGetMaxID() < awardID)
					{
						sVitalityAward* awardInfo = VitalityLoader::GetInstance()->GetVitalityAwardInfoByID(awardID);
						if (NULL != awardInfo && m_Score >= awardInfo->score)
						{
							//
							SetCanGetMaxID(awardID);
							// 活跃度UI高亮
							// m_owner->UpdateAtiveHighLight(eHL_ActivityDegree, 1, false, true);
							m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Vitality, eUIHLSmall_Vitelity_UnGetReward), 1, true);
						}
					}
				}
				toPlayer.set_id(id);
				toPlayer.set_count(it->second.count);
				toPlayer.set_score(m_Score);

				m_owner->SetDataFlag(eVitalityInfo);
				//cout<<endl<<"同步活跃度信息！"<<endl;
				QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(), &toPlayer, MSG_SIM_GM2C_VITALITYINFO);
			}
		}
	}
}

//完成副本活跃度
void VitalityMgr::FinshMapInstanceVitality(int mapid)
{
	InstanceMapEntry *entry =	InstanceLoader::GetInstance()->GetEntry(mapid);
//	if(entry->m_nIsSpecial == 2)
//	{
//		m_owner->GetVitalityMgr()->FinshVitality(eViThreatInstance);
//	}
//	else
	{
		switch(entry->m_eType)
		{
			case ePersonalInstance:			//个人
			{
				m_owner->GetVitalityMgr()->FinshVitality(eViPersonalInstance);
//				if (0 <= m_owner->GetVitalityMgr()->GetUnCompleteByID(eViPersonalInstance))
//				{
//					m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), 1, false);
//				}

				break;
			}
			case eGroupInstance:				//组队
			{
//				if (0 <= m_owner->GetVitalityMgr()->GetUnCompleteByID(eViGroupInstance))
//				{
//					m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), 1, false);
//				}
				m_owner->GetVitalityMgr()->FinshVitality(eViGroupInstance);
				break;
			}
			case eBattleGroudInstance:		//混合  相当于战场
			{
				break;
			}
			case eAnswerInstance:
			{
				break;
			}
			case eCompeteInstance: //竞技场
			{
				m_owner->GetVitalityMgr()->FinshVitality(eViCompeteInstance);
				break;
			}
			case eClanBattle: //帮派战
			{
				break;
			}
			case eTreasonKing: //剿灭反王
			{
				m_owner->GetVitalityMgr()->FinshVitality(eViTreasonKing);
				break;
			}
			case eExpInstance:
			case eProtectKing: //守护秦王
			{
				m_owner->GetVitalityMgr()->FinshVitality(eViProtectKing);
				break;
			}
			case eBeyondSvrBattle: //夸服战
			{
				m_owner->GetVitalityMgr()->FinshVitality(eViBeyondSvrBattle);
				break;
			}
			case eInstanceMax:
			case eVipInstance: //VIP副本
			{
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

//bool VitalityMgr::HandlePlayerKillWorldBoss(const EventArgs & e)
//{
//	TwoParmaEventArgs* data = (TwoParmaEventArgs*)(&e);
//	if(!data)
//	{
//		return true;
//	}
//
//	Smart_Ptr<Monster> mon = data->m_myself;
//	if(mon)
//	{
//		if(mon->GetQuality() == eWorldMonster)
//		{
//			map<int64, int> enemyValue;
//			mon->GetBrain()->GetEnemyValue(enemyValue);
//			map<int64, int>::iterator it = enemyValue.begin();
//			for(; it != enemyValue.end(); ++it)
//			{
//				Smart_Ptr<Player> player;
//				PlayerMessage::GetInstance()->GetPlayerByCharid(it->first,player);
//
//				if(player)
//				{
//					player->GetVitalityMgr()->FinshVitality(eViBoss);
//				}
//			}
//		}
//	}
//	return true;
//}

void VitalityMgr::SetCanGetMaxID(int id)
{
	m_canGetMaxID = id;
}

int VitalityMgr::GetCanGetMaxID()
{
	return m_canGetMaxID;
}

int VitalityMgr::GetUnGetAwardCnt()
{
	int ret = 0;
	map<int, bool>::iterator it = m_isGetAward.begin();
	for (; it != m_isGetAward.end() && (it->first != GetCanGetMaxID()+1); ++it)
	{
		if (!it->second)
		{
			++ret;
		}
	}
	return ret;
}

int VitalityMgr::GetUnCompleteByID(EVitalityType id)
{
	int count = 0;
	map<int,sVitalityInfo>::iterator it = m_allVitalityInfo.find(id);
	if (it != m_allVitalityInfo.end())
	{
		sVitality *vi = VitalityLoader::GetInstance()->GetVitalityInfoByID(id);
		if (NULL != vi)
		{
			if(it->second.count <= vi->count)
			{
				count = vi->count - it->second.count;
			}
		}
	}
	return count;
}

bool VitalityMgr::HandlePlayerSignIn(const EventArgs& e)
{
	if(m_owner)
	{
		FinshVitality(eViSignIn);
	}
	return true;
}

bool VitalityMgr::HandlePlayerBuyItemFromMall(const EventArgs & e)
{
	BuyItemEventArgs* data = (BuyItemEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_pObj;
	if(player && player->GetID() == m_owner->GetID())
	{
		FinshVitality(eViMallShop);
	}

	return true;
}

bool VitalityMgr::HandlePlayerEquipInheritEvent(const EventArgs & e)
{
	FinshVitality(eViEquipSucceed);

	return true;
}

bool VitalityMgr::HandlePlayerSkillUpgrade(const EventArgs& e)
{
	SkillUpgradeArgs* data = (SkillUpgradeArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		FinshVitality(eViSkillUp);
	}

	return true;
}

bool VitalityMgr::HandlePlayerSubmitTask(const EventArgs& e)
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
		if(entry->m_nMainType == QuestType_Functional)
			FinshVitality(eViDailyQuest);
		else if(entry->m_nMainType == QuestType_Clan)
			FinshVitality(eViClanQuest);
	}

	return true;
}

bool VitalityMgr::HandlePlayerOneKeyComplete(const EventArgs& e)
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

		if(info->m_nMainType == QuestType_Functional)
			FinshVitality(eViDailyQuest,data->m_nTimes);
		else if(info->m_nMainType == QuestType_Clan)
			FinshVitality(eViClanQuest,data->m_nTimes);
	}

	return true;
}

bool VitalityMgr::HandleGirlWarLingering(const EventArgs & e)
{
	FinshVitality(eViGirlWarLingering);

	return true;
}

bool VitalityMgr::HandlePlayerTurnLuckTable(const EventArgs & e)
{
	TurnLuckyTableArgs* data = (TurnLuckyTableArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		FinshVitality(eViLucklyTurntable);
	}

	return true;
}

bool VitalityMgr::HandlePlayerMoneyShop(const EventArgs & e)
{
	FinshVitality(eViMoneyShop);

	return true;
}

bool VitalityMgr::HandlePlayerPassInstance(const EventArgs & e)
{
	PlayerInstancePassEventArgs* data = (PlayerInstancePassEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		FinshMapInstanceVitality(data->m_mapId);
	}

	return true;
}
