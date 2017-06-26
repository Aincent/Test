/*
 * ActivityBox.cpp
 *
 *  Created on: 2015年7月29日
 *      Author: root
 */

#include "ActivityBox.h"
#include "Log/Logger.h"
#include "../Map/SceneManager.h"
#include "../Object/Player/Player.h"
#include "../ServerEventDefine.h"
#include "../Object/FileLoader/AILoader.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../Map/GameZone.h"
#include "../Object/ObjectFactory.h"
#include "../Object/AI/AIBrain.h"
#include "../Object/Player/PlayerMessage.h"
#include "FileLoader/ActivityBoxLoader.h"
#include "MessageStruct/Activity/ActivityBox.pb.h"
#include "FileLoader/ActivitiesTimerLoader.h"


ActivityBox::ActivityBox():m_BoxState(0),m_nTimes(0),m_nSceneId(0)
{
	m_sFightTime = TimerInterface::GetInstance()->RegRepeatedTime(&ActivityBox::onFightTime,this,0,0,0,0,5000,5000);
}

ActivityBox::~ActivityBox()
{
	m_leaveMapEvent.SetNull();
	m_playerAttackEvent.SetNull();
	m_playerMoveEvent.SetNull();
	m_MasterStatus.clear();

	if (m_sFightTime.IsValid())
	{
		m_sFightTime.SetNull();
	}

	if (m_sFlushTime.IsValid())
	{
		m_sFlushTime.SetNull();
	}
}

//获得帮战开启时间
DWORD64 ActivityBox::GetClanWarBeginTime()
{
	int nWeek = 0;
	time_t t = 0;
	struct tm local;

	time(&t);

	localtime_r(&t,&local);

	nWeek = local.tm_wday==0?7:local.tm_wday;

	vector<FightTime> vec;
	ActivitiesTimerLoader::GetInstance()->GetFightTime(vec);

	vector<FightTime>::iterator itr = vec.begin();
	for(; itr != vec.end(); ++itr)
	{
		bool bResult = false;

		switch(itr->nType)
		{
		case eCountDownTimeClan:
			{
				vector<BYTE>::iterator result = find(itr->vWeek.begin( ), itr->vWeek.end( ), nWeek);
				if (result == itr->vWeek.end( ))
				{
					continue;
				}

				if(itr->nBeforeHour == local.tm_hour && itr->nEndHour == local.tm_hour)
				{
					if(itr->nBeforeMinutes <= local.tm_min && local.tm_min <= itr->nEndMinutes)
					{
						bResult = true;
					}
				}
				else if(itr->nBeforeHour <=  local.tm_hour && itr->nEndHour >= local.tm_hour)
				{
					if(itr->nBeforeHour == local.tm_hour && local.tm_min >= itr->nBeforeMinutes)
					{
						bResult = true;
					}
					else if(itr->nEndHour == local.tm_hour && local.tm_min <= itr->nEndMinutes)
					{
						bResult = true;
					}
					else if(itr->nBeforeHour < local.tm_hour && local.tm_hour < itr->nEndHour)
					{
						bResult = true;
					}
				}
				else
				{
					bResult = false;
				}

				if(bResult)
				{
					local.tm_hour = 0;
					local.tm_min = 0;
					local.tm_sec = 0;
					t = mktime(&local) ;

					DWORD64 ntime = (DWORD64)t*1000;

					ntime += (itr->nHour*60 + itr->nMinutes)*60*1000;

					return ntime;
				}

				break;
			}
		}
	}

	return 0;
}

void ActivityBox::UpdatePlayersActivityBox(bool flag)
{
	const INT64PLAYERMAP& players = PlayerMessage::GetInstance()->getAllPlayer();
	for (INT64PLAYERMAP::const_iterator it = players.begin(); players.end() != it; ++it)
	{
//		if ((bool)it->second && it->second->IsOpenSystem(eHeavenChest))
		{
			it->second->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Activity, eUIHLSmall_Activity_SkyDropBox), 1, flag);
		}
	}
}

bool ActivityBox::IsActivityBoxBegin()
{
	return m_BoxState == -1 ? true : false;
}

void ActivityBox::onFightTime(void* obj)
{
	int nWeek = 0;
	struct tm local;
	time_t t = 0;
	time(&t);
	localtime_r(&t,&local);
	nWeek = local.tm_wday==0?7:local.tm_wday;
	vector<FightTime> vec;
	ActivitiesTimerLoader::GetInstance()->GetFightTime(vec);
	vector<FightTime>::iterator itr = vec.begin();
	for (; itr != vec.end(); ++itr)
	 {
		 	 if(itr->nType == eCountDownTimeBox)
		 	 {
				 vector<BYTE>::iterator result = find(itr->vWeek.begin( ), itr->vWeek.end( ), nWeek);
				 if ( result == itr->vWeek.end( ))
				  {
					  continue;
				  }

			 if (m_BoxState == 0)
			  {
				 if (itr->nHour == local.tm_hour && itr->nMinutes == local.tm_min)
				  {
					  BeginActivityBox();
					  m_BoxState = -1;
				  }
			  }

			 if (itr->nEndHour == local.tm_hour
					 && itr->nEndMinutes == local.tm_min
					 && m_BoxState < 0)
			  {
				 EndActivityBox();
				 m_BoxState = 0;
			  }
	 	 }
	 }
}

void ActivityBox::UpdateActivityTimes(void * arg)
{
	ActivityBoxInfo::ToClientActivityBoxTimes toClient;

	if (m_nTimes == 0)
	{
		UpdateActivityBossBox(NULL);
	}
	else
	{
		if (m_sFlushTime.IsValid())
		{
			m_sFlushTime.SetNull();
		}

		m_sFlushTime = TimerInterface::GetInstance()->RegTimePoint(&ActivityBox::UpdateActivityBossBox, this, 0, 0, 0, 0, 5000);
	}

	m_nTimes++;
	toClient.set_ntimes(m_nTimes);

	const INT64PLAYERMAP & players = PlayerMessage::GetInstance()->getAllPlayer();
	INT64PLAYERMAP::const_iterator it = players.begin();
	for (; it!=players.end(); ++it)
	{
		if (it->second.Get()==NULL)
			continue;

		if (GET_MAP_ID(it->second->GetMapID()) != ActivityBoxLoader::GetInstance()->GetMapId())
			continue;

		QuickMessage::GetInstance()->AddSendMessage(it->second->GetID(),it->second->GetChannelID(), &toClient, MSG_SIM_GM2C_BOXTIME);
	}

	if (m_nTimes < ActivityBoxLoader::GetInstance()->GetTimes())
	{
		if (m_sCountTime.IsValid())
		{
			m_sCountTime.SetNull();
		}

		m_sCountTime = TimerInterface::GetInstance()->RegTimePoint(&ActivityBox::UpdateActivityTimes,
																																			this, 0, 0, 0, 0,
																																			ActivityBoxLoader::GetInstance()->GetPertime());
	}
}

void ActivityBox::UpdateActivityBossBox(void * arg)
{
	vector<ChestPoint> vecPos;
	MonsterInfo info;

	Smart_Ptr<CGameZone> pZone = CSceneMgr::GetInstance()->GetGameZoneByMapID(ActivityBoxLoader::GetInstance()->GetMapId());
	if(!pZone)
		return;

	pZone->SetMonsterDead(ActivityBoxLoader::GetInstance()->GetMonsterId());
	m_MasterStatus.clear();

	ActivityBoxLoader::GetInstance()->GetRandomPos(ActivityBoxLoader::GetInstance()->GetMapId(),vecPos);
	if (vecPos.size() < (DWORD)ActivityBoxLoader::GetInstance()->GetNum())
	{
		return;
	}

	vector<Smart_Ptr<Monster> > vecMonster;
	for (int i = 0; i < ActivityBoxLoader::GetInstance()->GetNum(); i++)
	{
		MonsterLoader::GetInstance()->GetMonsterInfo(ActivityBoxLoader::GetInstance()->GetMonsterId(), info);
		if (info.id == -1)
		{
			LOG_ERROR(FILEINFO, "monster id is error");

			continue;
		}

		Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eMonster);
		if (false == bool(obj))
			continue;

		Smart_Ptr<Monster> mon = obj;

		mon->SetID(info.id);
		mon->SetName(info.name);
		mon->getAttManage()->setValue(eCharLv,info.lv);
		mon->SetQuality(info.quality);
		mon->SetFlushTime(info.flushTime);
		mon->SetAliveTime(info.aliveTime);
		mon->getAttManage()->setValue(eCharCountry,info.camp);
		mon->getAttManage()->setValue(eCharMoveSpeed,info.idleSpeed);
		mon->SetRunSpeed(info.moveSpeed);
//		mon->SetBetterAi(info.betterAI);
		mon->SetFallID(info.fallID);
		mon->getAttManage()->setValue(eCharExp,info.exp);
		mon->getAttManage()->setValue(eCharPhyAttackMax,info.maxAttack);
		mon->getAttManage()->setValue(eCharPhyDefence,info.phyDefence);
		mon->getAttManage()->setValue(eCharHitRate,info.hitRate);
		mon->getAttManage()->setValue(eCharAvoidRate,info.avoidRate);
		mon->getAttManage()->setValue(eCrackDownRate,info.crackRate);
		mon->getAttManage()->setValue(eAvoidCrackRate,info.avoidCrack);
		mon->SetHPMax(info.maxHP);
		mon->SetCurHP(info.maxHP);
		mon->SetMPMax(info.maxMP);
		mon->SetCurMP(info.maxMP);
		mon->SetGatherTime(info.gatherTime);	//采集时间
		mon->SetOwnerToBrain(mon);
		mon->SetMapID(pZone->GetSceneID());
		mon->SetBornTime();
		mon->SetOwnerName("");
		mon->GetBrain()->SetMonsterType(eRandomWayPos);

		int temp = rand()%vecPos.size();
		mon->SetXPos(vecPos[temp].nXPos);
		mon->SetYPos(vecPos[temp].nYPos);
		mon->SetOriginalX(vecPos[temp].nXPos);
		mon->SetOriginalY(vecPos[temp].nYPos);

		vecPos.erase(vecPos.begin()+ temp);

		pZone->Join(obj);

		vector<Sector *> moveVec;
		BaseAIInfo *aiInfo = 0;
		BetterAIInfo *betterAI = 0;

		aiInfo = AILoader::GetInstance()->GetBaseAIByid(info.baseAI);
		if (!aiInfo)
		{
			LOG_ERROR(FILEINFO, "monster[id=%d] ai is error", info.id);

			continue;
		}

		if(info.betterAI > 0)
		{
			betterAI = AILoader::GetInstance()->GetBetterAIByid(info.betterAI);
			if (!betterAI)
			{
				LOG_ERROR(FILEINFO, "monster[id=%d] better ai is error", info.id);

				continue;
			}
		}

		mon->SetStayTime(aiInfo->stayTime);
		mon->SetPhyAttackTime(aiInfo->atkTime);

		Smart_Ptr<AIBrain> monBrain = mon->GetBrain();
		if(!monBrain)
			continue;

		monBrain->Init(aiInfo, betterAI);
		vecMonster.push_back(mon);
		pZone->AddObject(mon->GetKey(),mon);

		MasterStatus mStatus;
		mStatus.nKey = mon->GetKey();
		mStatus.nSceneID = pZone->GetSceneID();

		m_MasterStatus[mon->GetKey()] = mStatus;
	}

	pZone->AddMonster(ActivityBoxLoader::GetInstance()->GetMonsterId(),vecMonster);

	m_nSceneId = pZone->GetSceneID();
}

bool ActivityBox::HandleLeaveMapEvent(const EventArgs & e)
{
	PlayerRemoveMemberEventArgs * pArgs = (PlayerRemoveMemberEventArgs *) (&e);

	if (pArgs->m_pObj->GetMapID() != m_nSceneId)
		return true;

	map<int,MasterStatus>::iterator itr = m_MasterStatus.begin();
	for (; itr != m_MasterStatus.end(); ++itr)
	{
		if (itr->second.bStatus && itr->second.nCharId == pArgs->m_pObj->GetID())
		{
			itr->second.bStatus = false;
			itr->second.nCharId = 0;
		}
	}

	return true;
}

bool ActivityBox::PlayerActivityBoxAttack(const EventArgs & e)
{
	PlayerAttackCreature * pArgs = (PlayerAttackCreature *) (&e);

	if (pArgs && pArgs->m_player->GetMapID() != m_nSceneId)
	{
		return true;
	}

	map<int,MasterStatus>::iterator itr = m_MasterStatus.begin();
	for (; itr != m_MasterStatus.end(); ++itr)
	{
		if (itr->second.bStatus && itr->second.nCharId == pArgs->m_player->GetID())
		{
			itr->second.bStatus = false;
			itr->second.nCharId = 0;
		}
	}

	return true;
}

bool ActivityBox::PlayerActivityBoxMove(const EventArgs & e)
{
	PlayerMoveEvent * pArgs = (PlayerMoveEvent *) (&e);
	if (pArgs==NULL)
		return true;

	Smart_Ptr<Player> m_player;
	PlayerMessage::GetInstance()->GetPlayerByCharId(m_player,pArgs->m_charID);
	if (m_player && m_player->GetMapID() != m_nSceneId)
	{
		return true;
	}

	map<int,MasterStatus>::iterator itr = m_MasterStatus.begin();
	for (; itr != m_MasterStatus.end(); ++itr)
	{
		if (itr->second.bStatus && itr->second.nCharId == pArgs->m_charID)
		{
			itr->second.bStatus = false;
			itr->second.nCharId = 0;
		}
	}

	return true;
}

bool ActivityBox::IsCanGather(int nKey,Smart_Ptr<Player>& player)
{
	if (player->GetMapID() != m_nSceneId)
	{
		return false;
	}

	map<int,MasterStatus>::iterator itr = m_MasterStatus.find(nKey);
	if (itr != m_MasterStatus.end())
	{
		if (itr->second.bStatus)
		{
			if(itr->second.nCharId == player->GetID())
					return true;

			return false;
		}
		else
		{
			itr->second.bStatus = true;
			itr->second.nCharId = player->GetID();

			return true;
		}
	}

	return false;
}

void ActivityBox::BeginActivityBox()
{
	if (!m_leaveMapEvent.IsValid())
	{
		m_leaveMapEvent = RegGlobalEventPtr(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, this, &ActivityBox::HandleLeaveMapEvent);
	}

	if (!m_playerAttackEvent.IsValid())
	{
		m_playerAttackEvent = RegGlobalEventPtr(PLAYER_ATTACK_CREATURE, this, &ActivityBox::PlayerActivityBoxAttack);
	}

	if (!m_playerMoveEvent.IsValid())
	{
		m_playerMoveEvent = RegGlobalEventPtr(PLAYER_MOVE, this, &ActivityBox::PlayerActivityBoxMove);
	}

	UpdateActivityTimes(NULL);

	UpdatePlayersActivityBox(true);

}

void ActivityBox::EndActivityBox()
{
	m_leaveMapEvent.SetNull();
	m_playerAttackEvent.SetNull();
	m_playerMoveEvent.SetNull();
	m_MasterStatus.clear();
	m_nTimes = 0;
	m_nSceneId = 0;

	Smart_Ptr<CGameZone> pZone = CSceneMgr::GetInstance()->GetGameZoneByMapID(ActivityBoxLoader::GetInstance()->GetMapId());
	if (!pZone)
		return;

	pZone->SetMonsterDead(ActivityBoxLoader::GetInstance()->GetMonsterId());

	m_MasterStatus.clear();

	if (m_sFlushTime.IsValid())
	{
		m_sFlushTime.SetNull();
	}

	if (m_sCountTime.IsValid())
	{
		m_sCountTime.SetNull();
	}

	ActivityBoxInfo::ToClientActivityBoxTimes toClient;
	toClient.set_ntimes(-1);

	const INT64PLAYERMAP& players = PlayerMessage::GetInstance()->getAllPlayer();
	INT64PLAYERMAP::const_iterator it = players.begin();
	for (; it!=players.end(); ++it)
	{
			if(it->second.Get()==NULL)
				continue;

//			if (it->second->IsOpenSystem(eHeavenChest))
			{
				it->second->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Activity, eUIHLSmall_Activity_SkyDropBox), 1, false);
			}

			if(GET_MAP_ID(it->second->GetMapID()) != ActivityBoxLoader::GetInstance()->GetMapId())
				continue;

			QuickMessage::GetInstance()->AddSendMessage(it->second->GetID(),
																												it->second->GetChannelID(),
																												&toClient,
																												MSG_SIM_GM2C_BOXTIME);

	}
}
