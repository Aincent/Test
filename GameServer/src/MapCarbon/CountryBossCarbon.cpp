/*
 * CountryBossCarbon.cpp
 *
 *  Created on: 2015年6月30日
 *      Author: root
 */

#include "CountryBossCarbon.h"
#include "util.h"
#include "Log/Logger.h"
#include "../Object/Player/Player.h"
#include "../Object/ItemDrop/DropItemMgr.h"
#include "../Map/DuplicateMgr.h"
#include "CounterDefine.h"
#include "../ServerEventDefine.h"
#include "MessageStruct/CharLogin/MapInstanceProto.pb.h"
#include "MessageStruct/ServerReturnBool.pb.h"
#include "../QuickMessage.h"
#include "../Object/Player/PlayerMessage.h"
#include "../ServerManager.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageCommonRet.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../Object/ObjectFactory.h"
#include "../Object/FileLoader/AILoader.h"
#include "../Activity/ActivityMgr.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../Object/AI/AIBrain.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../Object/ItemDrop/DropItemAlloc.h"
#include "../Chat/ChatMgr.h"
#include "../Container/ArticleEquipMgr.h"
#include "../Country/GsCountryMgr.h"
#include "MessageStruct/Country/CountryBoss.pb.h"
#include "../Mail/MailMessage.h"
#include "./FileLoader/InternationalLoader.h"
#include "../Country/GsCountryWarLoader.h"
#include "../Country/GsCountryWarMgr.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"

bool OrderByHarm( const TeamHarm &v1, const TeamHarm &v2)
{
    return v1.m_harm > v2.m_harm;
}

CCountryBossCarbon::CCountryBossCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):CGameDuplicate(onlyid,charid,paramf,params),m_targetPointf(0),m_targetPoints(0)
{

}

CCountryBossCarbon::~CCountryBossCarbon()
{
	m_monsterDeadEvent.SetNull();
	m_playerDeadEvent.SetNull();
	m_leaveMapEvent.SetNull();

	if(m_reflushTime.IsValid())
	{
		m_reflushTime.SetNull();
	}

	if(m_countDownTime.IsValid())
	{
		m_countDownTime.SetNull();
	}

	if(m_SendNewTime.IsValid())
	{
		m_SendNewTime.SetNull();
	}
}


int CCountryBossCarbon::Init()
{
	m_reflushTime = TimerInterface::GetInstance()->RegTimePoint(&CCountryBossCarbon::FlushMonster, this, 0, 0, 0, 0, GsCountryMgr::GetInstance()->GetNextBossBornTime(m_nDigNum));
	m_countDownTime = TimerInterface::GetInstance()->RegRepeatedTime(&CCountryBossCarbon::FlushBossCon, this, 0, 0, 0, 0, GsCountryMgr::GetInstance()->GetFlushConTime(),GsCountryMgr::GetInstance()->GetFlushConTime());
	m_SendNewTime = TimerInterface::GetInstance()->RegRepeatedTime(&CCountryBossCarbon::FlushBossMessage, this, 0, 0, 0, 0, GsCountryMgr::GetInstance()->GetFlushNewsTime(),GsCountryMgr::GetInstance()->GetFlushNewsTime());

	int ret = CGameDuplicate::Init();
	return ret;
}

void CCountryBossCarbon::FlushMonster(void * arg)
{

	if(m_process > ePlayerWork)
		return;

	INTVMPNSTERMAP allMonster = m_allMonster;
	vector<Smart_Ptr<Monster> > vecMonster;

	INTVMPNSTERMAP::iterator itr =  allMonster.begin();
	for(; itr != allMonster.end(); ++itr)
	{
		vecMonster = itr->second;
		vector<Smart_Ptr<Monster> >::iterator vitr = vecMonster.begin();
		for(; vitr != vecMonster.end(); ++vitr)
		{
			(*vitr)->ClearEnemyList();
			(*vitr)->SetDead();
		}
	}

	map<int,BossMap> bossMap;
	GsCountryMgr::GetInstance()->GetBossMap(bossMap);
	map<int,BossMap>::iterator bitr =  bossMap.begin();
	for(; bitr != bossMap.end(); ++ bitr)
	{
		if(bitr->second.nId < 3)
		{
			continue;
		}

		MonsterInfo info;
		vector<Smart_Ptr<Monster> > vecMonster;

		MonsterLoader::GetInstance()->GetMonsterInfo(bitr->second.nBossId, info);
		if(info.id == -1)
		{
			LOG_ERROR(FILEINFO, "monster id is error");

			continue;
		}

		Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eMonster);
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
//		mon->SetBaseAi(info.baseAI);
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
		mon->SetMapID(m_sceneID);
		mon->SetBornTime();
		mon->SetOwnerName("");
		mon->GetBrain()->SetMonsterType(eRandomWayPos);
		mon->SetXPos(bitr->second.nXPos);
		mon->SetYPos(bitr->second.nYPos);
		mon->SetOriginalX(bitr->second.nXPos);
		mon->SetOriginalY(bitr->second.nYPos);
//			mon->SetGroup(GetSectorGroupIndex(vec[0].xpos, vec[0].ypos));

		Join(obj);

		BaseAIInfo *aiInfo = 0;
		BetterAIInfo *betterAI = 0;

		aiInfo = AILoader::GetInstance()->GetBaseAIByid(info.baseAI);
		if(!aiInfo)
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
		m_allObject[mon->GetKey()] = mon;

		vecMonster.push_back(mon);
		m_allMonster[info.id] = vecMonster;
	}

	if(m_reflushTime.IsValid())
	{
		m_reflushTime.SetNull();
	}

	m_nDigNum++;

	if(m_nDigNum < 3)
	{
		m_reflushTime = TimerInterface::GetInstance()->RegTimePoint(&CCountryBossCarbon::FlushMonster, this, 0, 0, 0, 0, GsCountryMgr::GetInstance()->GetNextBossBornTime(m_nDigNum));
	}
}

void CCountryBossCarbon::FlushBossCon(void * arg)
{
	map<int,BossMap> bossMap;
	vector<int64> vCharId;
	int nCurTargerf = 0;

	GsCountryMgr::GetInstance()->GetBossMap(bossMap);
	map<int,BossMap>::iterator bitr =  bossMap.begin();
	for(; bitr != bossMap.end(); ++ bitr)
	{
//		if(bitr->second.nId < 3)
//		{
//			continue;
//		}

		for(size_t i = 0; i < bitr->second.vPos.size(); i++)
		{
			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
			int sectorIndex = GetSectorIndex(bitr->second.vPos[i].nXPos, bitr->second.vPos[i].nYPos);
			if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex is invalid");
				continue;
			}

			Sector * pSector = &m_totalSector[sectorIndex];
			if(pSector)
			{
				vector<Smart_Ptr<Player> > obj;
				pSector->GetPlayer(obj);
				vector<Smart_Ptr<Player> >::iterator vitr = obj.begin();
				for(; vitr != obj.end(); ++vitr)
				{
					(*vitr)->AddExp(GsCountryMgr::GetInstance()->GetFlushExp((*vitr)->getAttManage()->getValue<int>(eCharLv),bitr->second.nRate));
					map<int64,CountryMember>::iterator citr = m_CountryBoss.find((*vitr)->GetID());
					if(citr != m_CountryBoss.end())
					{
						citr->second.nExp += GsCountryMgr::GetInstance()->GetFlushExp((*vitr)->getAttManage()->getValue<int>(eCharLv),bitr->second.nRate);
					}

					vCharId.push_back((*vitr)->GetID());
				}
			}
		}
	}

	INT64PLAYERMAP::iterator mPlayerItr = m_player.begin();
	for(; mPlayerItr != m_player.end(); ++mPlayerItr)
	{
		vector<int64>::iterator vitrCharid = std::find(vCharId.begin(),vCharId.end(),mPlayerItr->second->GetID());
		if(vitrCharid == vCharId.end())
		{
			mPlayerItr->second->AddExp(GsCountryMgr::GetInstance()->GetFlushExp(mPlayerItr->second->getAttManage()->getValue<int>(eCharLv),10));
			map<int64,CountryMember>::iterator citr = m_CountryBoss.find(mPlayerItr->second->GetID());
			if(citr != m_CountryBoss.end())
			{
				citr->second.nExp += GsCountryMgr::GetInstance()->GetFlushExp(mPlayerItr->second->getAttManage()->getValue<int>(eCharLv),10);
			}
		}

		mPlayerItr->second->ChangeCountryContribution(GsCountryMgr::GetInstance()->GetFlushCon());
	}

	map<int64,CountryMember>::iterator itr = m_CountryBoss.begin();
	for(; itr != m_CountryBoss.end(); ++itr)
	{
		if(itr->second.nCountry == m_paramf)
		{
			nCurTargerf ++;
		}

		itr->second.nCon += GsCountryMgr::GetInstance()->GetFlushCon(); //每5秒
		itr->second.nScore += GsCountryMgr::GetInstance()->GetPersionScore();
	}

	m_targetPointf += nCurTargerf * GsCountryMgr::GetInstance()->GetSystemScore();
	m_targetPoints += (m_CountryBoss.size() - nCurTargerf) * GsCountryMgr::GetInstance()->GetSystemScore();
}

void CCountryBossCarbon::FlushBossMessage(void * arg)
{
//--------------------------------更新怪物伤害---------------------------------------
	map<int64, int> enemyValue;
	Smart_Ptr<Player> player;
	INTVMPNSTERMAP::iterator bitr = m_allMonster.begin();
	for(; bitr != m_allMonster.end(); ++bitr)
	{
		vector<Smart_Ptr<Monster> > vMonster = bitr->second;
		vector<Smart_Ptr<Monster> >::iterator vitr = vMonster.begin();
		for(; vitr != vMonster.end(); ++vitr)
		{
			enemyValue.clear();
			(*vitr)->GetEnemyValue(enemyValue);

			map<int64, int>::iterator itr = enemyValue.begin();
			for(; itr != enemyValue.end(); ++itr)
			{
				Smart_Ptr<Player> player;
				PlayerMessage::GetInstance()->GetPlayerByCharid(itr->first,player);
				if(player)
				{
					UpdateCountryHarmRank(player,(*vitr)->GetID(),itr->second);
				}
			}
			(*vitr)->ClearEnemyValue();
		}
	}

//--------------------------------------
	INT64PLAYERMAP::iterator pitr =  m_player.begin();
	for(; pitr != m_player.end(); ++pitr)
	{
		map<int64,CountryMember>::iterator itr = m_CountryBoss.find(pitr->second->GetID());
		if(itr != m_CountryBoss.end())
		{
			CountryBossPackage::CountryIntegral integral;

			integral.set_sui(m_targetPointf);
			integral.set_tang(m_targetPoints);
			integral.set_enemy(itr->second.nKilling);
			integral.set_boss(itr->second.nBoss);
			integral.set_cor(itr->second.nCon);
			integral.set_exp(itr->second.nExp);
			integral.set_total(m_nDigNum);
			integral.set_country(itr->second.nCountry);
			integral.set_time(m_createTime + GsCountryMgr::GetInstance()->GetNextBossTime(m_nDigNum));

			INTVMPNSTERMAP::iterator bitr = m_allMonster.begin();
			for(; bitr != m_allMonster.end(); ++bitr)
			{
				vector<Smart_Ptr<Monster> >::iterator vitr = bitr->second.begin();
				for(; vitr != bitr->second.end(); ++vitr)
				{
					if((*vitr)->getAttManage()->getValue<int>(eCharHPMax) > 0)
					{
						CountryBossPackage::CountryBossHp* pHp = integral.add_bosshp();
						pHp->set_id((*vitr)->GetID());
						pHp->set_hp((((*vitr)->getAttManage()->getValue<int>(eCharHP)*100)/(*vitr)->getAttManage()->getValue<int>(eCharHPMax)));
					}
				}
			}

			QuickMessage::GetInstance()->AddSendMessage(pitr->second->GetID(),pitr->second->GetChannelID(), &integral, MSG_SIM_GM2C_COUNTRY_INTEGRAL);
		}
	}

//------------------------------------------------------------------------------------------------------------------
	map<int,BossMap> bossMap;
	bool bResult = false;
	int count = 0;
	Smart_Ptr<Monster> monster;

	GsCountryMgr::GetInstance()->GetBossMap(bossMap);
	map<int,BossMap>::iterator bossMapitr =  bossMap.begin();
	for(; bossMapitr != bossMap.end(); ++ bossMapitr)
	{
		if(bossMapitr->second.nId < 3)
		{
			continue;
		}

		for(size_t i = 0; i < bossMapitr->second.vPos.size(); i++)
		{
			monster = GetMonsterByID(bossMapitr->second.nBossId);
			if(!monster)
			{
				continue;
			}

			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
			int sectorIndex = GetSectorIndex(bossMapitr->second.vPos[i].nXPos, bossMapitr->second.vPos[i].nYPos);
			if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex is invalid");
				continue;
			}

			int groupIndex = GetSectorGroupIndex(bossMapitr->second.vPos[i].nXPos, bossMapitr->second.vPos[i].nYPos);
			if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
			{
				LOG_ERROR(FILEINFO, "groupindex is invalid");
				continue;
			}

			Sector * pSector = &m_totalSector[sectorIndex];
			if(pSector)
			{
				vector<Smart_Ptr<Player> > obj;
				pSector->GetPlayer(obj);
				vector<Smart_Ptr<Player> >::iterator vitr = obj.begin();
				for(; vitr != obj.end(); ++vitr)
				{
					CountryBossPackage::CountryBossHarmRank harmRank;
					map<int,vector<TeamHarm> >::iterator mhurtItr = m_CountryHarm.find(bossMapitr->second.nBossId);
					if(mhurtItr != m_CountryHarm.end())
					{
						std::sort(mhurtItr->second.begin(),mhurtItr->second.end(),OrderByHarm);
						vector<TeamHarm>::iterator vhurtItr = mhurtItr->second.begin();
						for(; vhurtItr != mhurtItr->second.end(); ++vhurtItr)
						{
							CountryBossPackage::CountryHarmRank* pHarm = harmRank.add_harminfo();
							pHarm->set_charid(vhurtItr->m_charid);
							pHarm->set_charname(vhurtItr->m_name);
							pHarm->set_harm(vhurtItr->m_harm);
							map<int64,CountryMember>::iterator memberItr = m_CountryBoss.find(vhurtItr->m_charid);
							if(memberItr != m_CountryBoss.end())
							{
								pHarm->set_country(memberItr->second.nCountry);
							}

							if(vhurtItr->m_charid == (*vitr)->GetID())
							{
								bResult = true;
							}

							++count;
							if(count == 10)
							{
								break;
							}
						}

						if(!bResult)
						{
							map<int64,CountryMember>::iterator memberItr = m_CountryBoss.find((*vitr)->GetID());
							if(memberItr != m_CountryBoss.end())
							{
								CountryBossPackage::CountryHarmRank* pHarm = harmRank.add_harminfo();
								pHarm->set_charid(memberItr->second.nCharid);
								pHarm->set_charname(memberItr->second.sName);
								pHarm->set_country(memberItr->second.nCountry);

								map<int,int>::iterator memberHurtItr = memberItr->second.mHarm.find(bossMapitr->second.nBossId);
								if(memberHurtItr != memberItr->second.mHarm.end())
								{
									pHarm->set_harm(memberHurtItr->second);
								}
							}
						}
					}

					map<int,int>::iterator targerItrf = m_targerHarmf.find(bossMapitr->second.nBossId);
					if(targerItrf != m_targerHarmf.end())
					{
						CountryBossPackage::BossRank* pBossHank = harmRank.add_bossinfo();
						pBossHank->set_country(m_paramf);
						pBossHank->set_harm(targerItrf->second);
					}

					map<int,int>::iterator targerItrs = m_targerHarms.find(bossMapitr->second.nBossId);
					if(targerItrs != m_targerHarms.end())
					{
						CountryBossPackage::BossRank* pBossHank = harmRank.add_bossinfo();
						pBossHank->set_country(m_params);
						pBossHank->set_harm(targerItrs->second);
					}

					harmRank.set_id(bossMapitr->second.nBossId);
					QuickMessage::GetInstance()->AddSendMessage((*vitr)->GetID(),(*vitr)->GetChannelID(), &harmRank, MSG_SIM_GM2C_COUNTRY_HARM_RANK);
				}
			}
		}
	}
}

bool CCountryBossCarbon::Update()
{
	switch(m_process)
	{
	case eCreated:
		{
			if(CUtil::GetNowSecond() - m_createTime >= m_data->m_nLimitTime)
			{
				m_process = eOver;
				return false;
			}

			break;
		}
	case ePlayerWork:
		{
			int64 nowTime = CUtil::GetNowSecond();

			if(GetPlayerCount() <= 0)
			{
				Count();
			}

			if(nowTime - m_createTime >= m_data->m_nLimitTime)
			{
				Over();
			}

			break;
		}
	case eWinFight:
		{
			Win();

			break;
		}
	case eFailed:
		{
			Failed();

			break;
		}
	case eAward:
		{
			Award();

			break;
		}
	case eDuplicateChangeMap:
		{
			if(GetPlayerCount() <= 0)
			{
				CountEx();
			}

			CountryBossOver();
			m_process = eChangeMapWait;

			break;
		}
	case eChangeMapWait:
		{
			int64 nowTime = CUtil::GetNowSecond();

			if(nowTime - m_createTime >= m_data->m_nLimitTime + 10 * 1000)
			{
				ForceAllPlayerChangeMap();
				m_process = eOver;
			}

			if(GetPlayerCount() <= 0)
			{
				m_process = eOver;
			}


			break;
		}
	case eOver:
		{
			return false;
		}
	default :
		{
			break;
		}
	}

	return true;
}

bool CCountryBossCarbon::JoinHandle(Smart_Ptr<Object> & obj)
{
	if(m_process >= eDuplicateChangeMap)
		return false;

	vector<int> vec;

	MapInstanceProto::SimInstanceUpdate toClient;

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

//		map<int64,CountryMember>::iterator itr = m_CountryBoss.find(player->GetID());
//		if(itr != m_CountryBoss.end())
//		{
//			return false;
//		}

		if(m_paramf != player->getAttManage()->getValue<DWORD>(eCharCountry) && m_params != player->getAttManage()->getValue<DWORD>(eCharCountry))
		{
			return false;
		}

		if(!SysCountryBossInfoToClient(player))
		{
			return false;
		}

		if(vec.size() > 0)
		{
			player->SynCharAttribute(vec);
		}

		if(!m_monsterDeadEvent.IsValid())
		{
			m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &CCountryBossCarbon::HandleMonsterDeadEvent);
		}

		if(!m_playerDeadEvent.IsValid())
		{
			m_playerDeadEvent = RegGlobalEventPtr(CREATURE_DEAD_EVENT_TYPE, this, &CCountryBossCarbon::HandlePlayerDeadEvent);
		}

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		if(m_process == eCreated)
			m_process = ePlayerWork;
	}

	return true;
}


int CCountryBossCarbon::Join(Smart_Ptr<Object> & obj)
{
	if(!JoinHandle(obj))
		return -1;

	CGameZone::Join(obj);
	return 0;
}

bool CCountryBossCarbon::SysCountryBossInfoToClient(Smart_Ptr<Player>& player)
{
	CountryBossPackage::CountryIntegral integral;
	CountryBossPackage::CountryBossHarmRank harmRank;

	map<int64,CountryMember>::iterator itr = m_CountryBoss.find(player->GetID());
	if(itr != m_CountryBoss.end())
	{
		integral.set_sui(m_targetPointf);
		integral.set_tang(m_targetPoints);
		integral.set_enemy(itr->second.nKilling);
		integral.set_boss(itr->second.nBoss);
		integral.set_cor(itr->second.nCon);
		integral.set_exp(itr->second.nExp);
		integral.set_total(m_nDigNum);
		integral.set_time(m_createTime + GsCountryMgr::GetInstance()->GetNextBossTime(m_nDigNum));

	}
	else
	{
		CountryMember country;
		country.nCharid = player->GetID();
		country.sName = player->GetName();
		country.nCountry = player->getAttManage()->getValue<DWORD>(eCharCountry);
		m_CountryBoss[player->GetID()] = country;

		integral.set_sui(m_targetPointf);
		integral.set_tang(m_targetPoints);
		integral.set_enemy(country.nKilling);
		integral.set_boss(country.nBoss);
		integral.set_cor(country.nCon);
		integral.set_exp(country.nExp);
		integral.set_total(m_nDigNum);
		integral.set_time(m_createTime + GsCountryMgr::GetInstance()->GetNextBossTime(m_nDigNum));
	}


	INTVMPNSTERMAP::iterator bitr = m_allMonster.begin();
	for(; bitr != m_allMonster.end(); ++bitr)
	{
		vector<Smart_Ptr<Monster> >::iterator vitr = bitr->second.begin();
		for(; vitr != bitr->second.end(); ++vitr)
		{
			if((*vitr)->getAttManage()->getValue<int>(eCharHPMax) > 0)
			{
				CountryBossPackage::CountryBossHp* pHp = integral.add_bosshp();
				pHp->set_id((*vitr)->GetID());
				pHp->set_hp((*vitr)->getAttManage()->getValue<int>(eCharHP)/(*vitr)->getAttManage()->getValue<int>(eCharHPMax));
			}
		}
	}

	QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &integral, MSG_SIM_GM2C_COUNTRY_INTEGRAL);
//------------------------------------------------------------------------------------------------------------------
//	for(int i = 0;i < m_TeamHarm.size();i++)
//	{
//		CountryBossPackage::CountryHarmRank* pHarm = harmRank.add_harminfo();
//		pHarm->set_charid(m_TeamHarm[i].m_charid);
//		pHarm->set_charname(m_TeamHarm[i].m_name);
//		pHarm->set_harm(m_TeamHarm[i].m_harm);
//	}
//
//	CountryBossPackage::CountryHarmRank* pHarm = harmRank.add_harminfo();
//	pHarm->set_charid(player->GetID());
//	pHarm->set_charname(player->GetName());
//	pHarm->set_harm(country.nHarm);
//
//	if(m_TeamHarm.size() < 10)
//	{
//		TeamHarm harm;
//		harm.m_charid = player->GetID();
//		harm.m_name = player->GetName();
//		m_TeamHarm.push_back(harm);
//	}
//
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &harmRank, MSG_SIM_GM2C_COUNTRY_HARM_RANK);

	return true;
}

int CCountryBossCarbon::Move(Smart_Ptr<Object> &obj)
{
	CGameZone::Move(obj);
	return 0;
}

void CCountryBossCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
{
	int64 mapid = 0;
	int xpos = 0;
	int ypos = 0;
	vector<int> vecType;
	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		if(player->getAttManage()->getValue<int>(eCharHP) <= 0)
		{
			CharBattle::RevivalData req;

			//增加地图id和坐标，重置死亡状态，解决客户端显示“使用复活单”的提示bug，是因为玩家回城复活，那么会发两次消息给客户端
			player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));
			player->UnsetCreatureState(ERS_Dead);
			player->SetDataFlag(eBaseInfo);
//					player->SendDeadMessage(eAttackSim);
				//如果不传地图Id就表示原地复活
			map<int64,OldMapInfo>::iterator itMap = m_oldMapPoint.find(player->GetID());
			if(itMap != m_oldMapPoint.end())
			{
				mapid = itMap->second.mapid;
				xpos = itMap->second.xpos;
				ypos = itMap->second.ypos;
			}
			else
			{
				MapStaticInfo *sMapInfo;
				int64 sceneid = 0;

				sMapInfo = CMapConfig::GetInstance()->GetFistMapInfo();
				if (sMapInfo == 0)
					return;

				sceneid = CSceneMgr::GetInstance()->GetSceneIDAboutPlayerIsLoginOn(sMapInfo->mapid);
				if(sceneid <= 0)
				{
					mapid = CREATE_MAP(eStaticMap, 1, 1, sMapInfo->mapid, 1);
				}
				else
				{
					mapid = sceneid;
				}

				xpos = sMapInfo->reliveXPos;
				ypos = sMapInfo->reliveYPos;
			}

			OldMapInfo * pInfo = GetOldMapPoint(player->GetID());
			if(!pInfo || CSceneMgr::GetInstance()->IsHaveScene(pInfo->mapid))
			{
				req.set_mapid(GET_MAP_ID(mapid));
				req.set_xpos(xpos);
				req.set_ypos(ypos);
			}

			req.set_key(player->GetKey());
			req.set_hp(player->getAttManage()->getValue<int>(eCharHP));
			req.set_mp(player->getAttManage()->getValue<int>(eCharMP));

			player->SendSynchMessage(&req, MSG_SIM_GT2C_REVIVAL);
		}
	}
}

void CCountryBossCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);

	CGameZone::Leave(obj, isDel, delFromMap);

	if(GetPlayerCount() <= 0)
	{
		m_monsterDeadEvent.SetNull();
		m_playerDeadEvent.SetNull();
//		m_leaveMapEvent.SetNull();
		m_leaveTime = CUtil::GetNowSecond();
	}
}

void CCountryBossCarbon::LeaveExame(Smart_Ptr<Player> & player)
{

}

bool CCountryBossCarbon::IsCanEnter(Smart_Ptr<Player> & player)
{
	//传送阶段，就不允许进入了
	if(m_process >= eDuplicateChangeMap)
		return false;

	//玩家死了不入传入地图
	if(player->IsDead())
	{
		return false;
	}

//	map<int64,CountryMember>::iterator itr = m_CountryBoss.find(player->GetID());
//	if(itr != m_CountryBoss.end())
//	{
//		return false;
//	}

	if(m_paramf != player->getAttManage()->getValue<DWORD>(eCharCountry) && m_params != player->getAttManage()->getValue<DWORD>(eCharCountry))
	{
		return false;
	}

	return true;
}

void CCountryBossCarbon::Win()
{
	m_isSuccess = true;
	m_process = eAward;
}

void CCountryBossCarbon::Over()
{
	m_process = eDuplicateChangeMap;
}


void CCountryBossCarbon::Count()
{

}

void CCountryBossCarbon::CountEx()
{
	int64 nowTime = CUtil::GetNowSecond();

	if(nowTime - m_leaveTime >= m_data->m_nDelTime)
	{
		m_process = eOver;
	}
}

void CCountryBossCarbon::Failed()
{
	m_process = eAward;
}

void CCountryBossCarbon::Award()
{
	m_process = eDuplicateChangeMap;
	m_awardTime = CUtil::GetNowSecond();
}

bool CCountryBossCarbon::HandleMonsterDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);

	if(GetSceneID() != tArgs->m_myself->GetMapID())
		return true;


	Smart_Ptr<Monster> mon = tArgs->m_myself;
	LOG_DEBUG(FILEINFO, "CGameDuplicate::HandleMonsterDeadEvent \t %lld", mon->GetID());

	Smart_Ptr<Player>  player;
	Smart_Ptr<Player>  object;
	Smart_Ptr<GirlWar>  girlwar;
	map<int,BossMap> bossMap;
	map<int64,int> enemyValue;
	mon->GetEnemyValue(enemyValue);
	bool bResult = false;

	if(tArgs->m_other)
	{
		if(tArgs->m_other->GetType() != ePlayer && tArgs->m_other->GetType() !=  ePet)
			return true;

		if(tArgs->m_other->GetType() == ePet)
		{
			girlwar = tArgs->m_other;

			player  = girlwar->GetOwner();
		}
		else
		{
			player = tArgs->m_other;
		}

		GsCountryMgr::GetInstance()->GetBossMap(bossMap);
		map<int,BossMap>::iterator mapItr =  bossMap.begin();
		for(; mapItr != bossMap.end(); ++ mapItr)
		{
			if(mapItr->second.nBossId == mon->GetID())
			{
				map<int64,CountryMember>::iterator bitr = m_CountryBoss.find(player->GetID());
				if(bitr != m_CountryBoss.end())
				{
					bitr->second.nScore += GsCountryMgr::GetInstance()->GetKillBossScore(mon->GetID());
					bitr->second.nBoss += 1;
				}

				UpdateCountryBossBox(mapItr->second.nId,mapItr->second.nItemId,mapItr->second.nItemNum);

				bResult = true;
			}
		}
	}

	if(!bResult)
	{
		return true;
	}

	map<int64,int>::iterator itr = enemyValue.begin();
	for(; itr != enemyValue.end(); ++itr)
	{
		Smart_Ptr<Player> player;
		PlayerMessage::GetInstance()->GetPlayerByCharid(itr->first,player);
		if(player)
		{
			UpdateCountryHarmRank(player,mon->GetID(),itr->second);
		}
	}

	if(tArgs->m_other)
	{
		if(tArgs->m_other->GetType() != ePlayer && tArgs->m_other->GetType() !=  ePet)
			return true;

		if(tArgs->m_other->GetType() == ePet)
		{
			girlwar = tArgs->m_other;

			player  = girlwar->GetOwner();
		}
		else
		{
			player = tArgs->m_other;
		}

		{
			CountryBossPackage::BossHarmSettle toClient;
			CountryBossPackage::CountryBossEndKill toClientEndKill;
			vector<int> vecType;
			vector<int64> killGoods;
			toClient.set_endcut(true);
			toClient.set_id(mon->GetID());
			toClient.set_total(m_nDigNum);

			player->AddExp(GsCountryMgr::GetInstance()->GetEndKillExp(player->getAttManage()->getValue<int>(eCharLv),GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));
			int credit = GsCountryMgr::GetInstance()->GetEndKillCredit(player->getAttManage()->getValue<int>(eCharLv),GsCountryMgr::GetInstance()->GetBossNum(mon->GetID()));
			player->ChangeCredit(credit,true);
			player->ChangeCountryContribution(GsCountryMgr::GetInstance()->GetEndKillCon(player->getAttManage()->getValue<int>(eCharLv),GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));

//			StatisticInfo::CostAndGotInfo costAndGotInfo;
//			StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotInfo,eCredit,credit);
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Credit,eStaMinor_Credit_CountryBoss,&costAndGotInfo);

			vecType.push_back(eCharExp);
			vecType.push_back(eCredit);
			//vecType.push_back(eCurCountryContribute);

			player->SynCharAttribute(vecType);

			toClientEndKill.set_exp(GsCountryMgr::GetInstance()->GetEndKillExp(player->getAttManage()->getValue<int>(eCharLv),GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));
			toClientEndKill.set_credit(GsCountryMgr::GetInstance()->GetEndKillCredit(player->getAttManage()->getValue<int>(eCharLv),GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));
			toClientEndKill.set_con(GsCountryMgr::GetInstance()->GetEndKillCon(player->getAttManage()->getValue<int>(eCharLv),GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));


			toClient.mutable_endkill()->CopyFrom(toClientEndKill);


			GsCountryMgr::GetInstance()->GetBossPower(toClient,mon->GetID(),1);
			for(int j = 0; j < toClient.item_size(); j++)
			{
				int64 info = toClient.item(j).num();
				info = info << 32;	//物品数量
				DWORD itemid = toClient.item(j).id();
				info = info | itemid;
				killGoods.push_back(info);
			}

			string strLastHitReward = InternationalLoader::GetInstance()->GetInterStr(16);
			MailMessageMgr::GetInstance()->SendSysMail(killGoods,player->GetName(),0,strLastHitReward);

			map<int,vector<TeamHarm> >::iterator mhurtItr = m_CountryHarm.find(mon->GetID());
			if(mhurtItr != m_CountryHarm.end())
			{
				std::sort(mhurtItr->second.begin(),mhurtItr->second.end(),OrderByHarm);
				for(size_t i = 0; i < mhurtItr->second.size(); ++i)
				{
					vector<int> vecType;

					if((mhurtItr->second)[i].m_charid != player->GetID())
					{
						CountryBossPackage::BossHarmSettle toClientRank;
						CountryBossPackage::CountryBossRankHurt toClientRnakHurt;
						toClientRank.set_endcut(false);
						toClientRank.set_id(mon->GetID());
						toClientRank.set_total(m_nDigNum);
						toClientRank.set_currank(i+1);
						toClientRank.set_country(player->getAttManage()->getValue<DWORD>(eCharCountry));
						toClientRank.set_name(player->GetName());

						object = GetPlayer((mhurtItr->second)[i].m_charid);
						if(object)
						{
							vector<int64> goods;

							object->AddExp(GsCountryMgr::GetInstance()->GetHurtRankingExp(object->getAttManage()->getValue<int>(eCharLv),i+1,GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));
							int credit = GsCountryMgr::GetInstance()->GetHurtRankingCredit(object->getAttManage()->getValue<int>(eCharLv), i+1, GsCountryMgr::GetInstance()->GetBossNum(mon->GetID()));
							object->ChangeCredit(credit,true);
							object->ChangeCountryContribution(GsCountryMgr::GetInstance()->GetHurtRankingCon(object->getAttManage()->getValue<int>(eCharLv),i+1,GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));

//							StatisticInfo::CostAndGotInfo costAndGotInfo;
//							StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//							StatisticMgr::AddItemGotInfo(gotInfo,eCredit,credit);
//							StatisticMgr::GetInstance()->StatisticPlayerBehavior(object->GetID(),eStatic_Credit,eStaMinor_Credit_CountryBoss,&costAndGotInfo);

							vecType.push_back(eCharExp);
							vecType.push_back(eCredit);
							//vecType.push_back(eCurCountryContribute);

							object->SynCharAttribute(vecType);

							toClientRnakHurt.set_exp(GsCountryMgr::GetInstance()->GetHurtRankingExp(object->getAttManage()->getValue<int>(eCharLv),i+1,GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));
							toClientRnakHurt.set_credit(GsCountryMgr::GetInstance()->GetHurtRankingCredit(object->getAttManage()->getValue<int>(eCharLv), i+1, GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));
							toClientRnakHurt.set_con(GsCountryMgr::GetInstance()->GetHurtRankingCon(object->getAttManage()->getValue<int>(eCharLv),i+1,GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));

							GsCountryMgr::GetInstance()->GetBossPower(toClientRank,mon->GetID(),i+1);
							for(int j = 0; j < toClientRank.item_size(); j++)
							{
								int64 info = toClientRank.item(j).num();
								info = info << 32;	//物品数量
								DWORD itemid = toClientRank.item(j).id();
								info = info | itemid;
								goods.push_back(info);
							}

							string strRankingReward = InternationalLoader::GetInstance()->GetInterStr(17);
							MailMessageMgr::GetInstance()->SendSysMail(goods,object->GetName(),0,strRankingReward);

							if(i < 10)
							{
								toClientRank.mutable_rankhurt()->CopyFrom(toClientRnakHurt);
								QuickMessage::GetInstance()->AddSendMessage(object->GetID(),object->GetChannelID(), &toClientRank, MSG_SIM_GM2C_COUNTRY_HARM_SETTLE);
							}
						}
					}
					else
					{
						vector<int64> goods;
						CountryBossPackage::CountryBossRankHurt toClientRnakHurt;
						GsCountryMgr::GetInstance()->GetBossPower(toClient,mon->GetID(),i+1);
						for(int j = 3; j < toClient.item_size(); j++)
						{
							int64 info = toClient.item(j).num();
							info = info << 32;	//物品数量
							DWORD itemid = toClient.item(j).id();
							info = info | itemid;
							goods.push_back(info);
						}

						int tempLevel = player->getAttManage()->getValue<int>(eCharLv);
						player->AddExp(GsCountryMgr::GetInstance()->GetHurtRankingExp(tempLevel,i+1,GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));
						int credit = GsCountryMgr::GetInstance()->GetHurtRankingCredit(tempLevel, i+1, GsCountryMgr::GetInstance()->GetBossNum(mon->GetID()));
						player->ChangeCredit(credit,true);
						player->ChangeCountryContribution(GsCountryMgr::GetInstance()->GetHurtRankingCon(tempLevel,i+1,GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));

						vecType.push_back(eCharExp);
						vecType.push_back(eCredit);

//						StatisticInfo::CostAndGotInfo costAndGotInfo;
//						StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//						StatisticMgr::AddItemGotInfo(gotInfo,eCredit,credit);
//						StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Credit,eStaMinor_Credit_CountryBoss,&costAndGotInfo);

						player->SynCharAttribute(vecType);

						toClientRnakHurt.set_exp(GsCountryMgr::GetInstance()->GetHurtRankingExp(tempLevel,i+1,GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));
						toClientRnakHurt.set_credit(GsCountryMgr::GetInstance()->GetHurtRankingCredit(tempLevel, i+1, GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));
						toClientRnakHurt.set_con(GsCountryMgr::GetInstance()->GetHurtRankingCon(tempLevel,i+1,GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));

						toClient.set_currank(i+1);
						toClient.set_country(player->getAttManage()->getValue<DWORD>(eCharCountry));
						toClient.set_name(player->GetName());


						string strRankingReward = InternationalLoader::GetInstance()->GetInterStr(17);
						MailMessageMgr::GetInstance()->SendSysMail(goods,player->GetName(),0,strRankingReward);


						if(i < 10)
						{
							toClient.mutable_rankhurt()->CopyFrom(toClientRnakHurt);
						}

						QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &toClient, MSG_SIM_GM2C_COUNTRY_HARM_SETTLE);
					}
				}
			}
		}



		map<int,vector<TeamHarm> >::iterator mhurtItr = m_CountryHarm.find(mon->GetID());
		if(mhurtItr != m_CountryHarm.end())
		{
			INT64PLAYERMAP::iterator playerIter =  m_player.begin();
			for(; playerIter != m_player.end(); ++playerIter)
			{
				if(playerIter->second->getAttManage()->getValue<DWORD>(eCharCountry) == player->getAttManage()->getValue<DWORD>(eCharCountry))
				{
					vector<TeamHarm>::iterator itr = find_if(mhurtItr->second.begin(),mhurtItr->second.end(),FindTeamHarm(playerIter->second->GetID()));
					if(itr == mhurtItr->second.end())
					{
						vector<int64> goods;
						int64 info = GsCountryMgr::GetInstance()->GetGoodsNum();
						info = info << 32;	//物品数量
						DWORD itemid = GsCountryMgr::GetInstance()->GetGoodsId();
						info = info | itemid;
						goods.push_back(info);

						string warning = InternationalLoader::GetInstance()->GetInterStr(1);
						string warning1 = InternationalLoader::GetInstance()->GetInterStr(16);

						int tempLevel = playerIter->second->getAttManage()->getValue<int>(eCharLv);
						MailMessageMgr::GetInstance()->SendSysMail(goods,playerIter->second->GetName(),0,warning1,warning,GsCountryMgr::GetInstance()->GetCampAwardMoney(tempLevel,GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())),GsCountryMgr::GetInstance()->GetCampAwardExp(tempLevel,GsCountryMgr::GetInstance()->GetBossNum(mon->GetID())));
					}
				}
			}
		}

		map<int,vector<TeamHarm> >::iterator mChurtItr = m_CountryHarm.find(mon->GetID());
		if(mChurtItr != m_CountryHarm.end())
		{
			mChurtItr->second.clear();
		}

		map<int64,CountryMember>::iterator bitr = m_CountryBoss.begin();
		for(; bitr != m_CountryBoss.end(); ++bitr)
		{
			map<int,int>::iterator mItr = bitr->second.mHarm.find(mon->GetID());
			if(mItr != bitr->second.mHarm.end())
			{
				bitr->second.mHarm.erase(mItr);
			}
		}

		map<int,int>::iterator targerItrf = m_targerHarmf.find(mon->GetID());
		if(targerItrf != m_targerHarmf.end())
		{
			m_targerHarmf.erase(targerItrf);
		}

		map<int,int>::iterator targerItrs = m_targerHarms.find(mon->GetID());
		if(targerItrs != m_targerHarms.end())
		{
			m_targerHarms.erase(targerItrs);
		}
	}

	return true;
}

void CCountryBossCarbon::UpdateCountryBossBox(int nId,int monsterId,int num)
{
	vector<CountryBossPos> vecPos;
	MonsterInfo info;
	vector<Smart_Ptr<Monster> > vecMonster;
	GsCountryMgr::GetInstance()->GetRandomPos(nId,vecPos);
	if(vecPos.size() < (DWORD)num)
	{
		return;
	}

	for(int i = 0; i < num; i++)
	{
		MonsterLoader::GetInstance()->GetMonsterInfo(monsterId, info);
		if(info.id == -1)
		{
			LOG_ERROR(FILEINFO, "monster id is error");

			continue;
		}

		Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eMonster);
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
//		mon->SetBaseAi(info.baseAI);
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
		mon->SetMapID(m_sceneID);
		mon->SetBornTime();
		mon->SetOwnerName("");
		mon->GetBrain()->SetMonsterType(eRandomWayPos);

		int temp = rand()%vecPos.size();

		mon->SetXPos(vecPos[temp].nXPos);
		mon->SetYPos(vecPos[temp].nYPos);
		mon->SetOriginalX(vecPos[temp].nXPos);
		mon->SetOriginalY(vecPos[temp].nYPos);
		vecPos.erase(vecPos.begin()+ temp);

		Join(obj);

		vector<Sector *> moveVec;
		BaseAIInfo *aiInfo = 0;
		BetterAIInfo *betterAI = 0;

		aiInfo = AILoader::GetInstance()->GetBaseAIByid(info.baseAI);
		if(!aiInfo)
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
		m_allObject[mon->GetKey()] = mon;
		vecMonster.push_back(mon);
	}
	m_allMonster[info.id] = vecMonster;
}

void CCountryBossCarbon::CountryBossOver()
{
	int nCountryScoref = 0;
	int nCountryScores = 0;
	int nWinCountry = 0;
	map<int,int>* campReward;
	Smart_Ptr<Player> player;

	map<int64,CountryMember>::iterator itr = m_CountryBoss.begin();
	for(; itr != m_CountryBoss.end(); ++itr)
	{
		if(itr->second.nCountry == m_paramf)
		{
			nCountryScoref += itr->second.nScore + itr->second.nKilling * 10;
		}
		else
		{
			nCountryScores += itr->second.nScore + itr->second.nKilling * 10;
		}
	}

	if(nCountryScores == nCountryScoref)
	{
	}
	else if(nCountryScores < nCountryScoref)
	{
		nWinCountry = m_paramf;
	}
	else
	{
		nWinCountry = m_params;
	}

	if(nWinCountry > 0)
	{
		map<int64,CountryMember>::iterator itr = m_CountryBoss.begin();
		for(; itr != m_CountryBoss.end(); ++itr)
		{
			player = GetPlayer(itr->second.nCharid);
			if(player && player->getAttManage()->getValue<DWORD>(eCharCountry) == (DWORD)nWinCountry)
			{
				vector<int> vecType;

				CountryBossPackage::CountryBossReward toClient;
				toClient.set_country(itr->second.nCountry);
				toClient.set_flag(true);
				toClient.set_cor(itr->second.nCon);
				toClient.set_sui(nCountryScoref);
				toClient.set_tang(nCountryScores);

				int tempLevel = player->getAttManage()->getValue<int>(eCharLv);
				player->AddExp(GsCountryMgr::GetInstance()->GetCampWinAwardExp(tempLevel));
				player->ChangeMoney(GsCountryMgr::GetInstance()->GetCampWinAwardMoney(tempLevel));

				vecType.push_back(eCharExp);
				vecType.push_back(eCharMoney);

				player->SynCharAttribute(vecType);

				QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &toClient, MSG_SIM_GM2C_COUNRTY_REWARD);
			}
		}

		CountryBossPackage::CountryBossWin toWorld;
		toWorld.set_country(nWinCountry);
		GsCountryMgr::GetInstance()->GetCampGoods(campReward);
		if(campReward)
		{
			map<int,int>::iterator itr = campReward->begin();
			for(; itr != campReward->end(); ++itr)
			{
				toWorld.add_itemid(itr->first);
				toWorld.add_num(itr->second);
			}
		}

		toWorld.set_money(GsCountryMgr::GetInstance()->GetCampMoney());

		map<int64,CountryMember>::iterator citr = m_CountryBoss.begin();
		for(; citr != m_CountryBoss.end(); ++citr)
		{
			Smart_Ptr<SvrItem> worldSvr;
			ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(citr->second.nCharid),worldSvr);
			if(worldSvr)
			{
				Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_SYSWIN_REWARD, &toWorld, worldSvr.Get(), SimpleMessage);
				Message_Facade::Send(messRet);
			}

			break;
		}
	}
}

void CCountryBossCarbon::UpdateCountryHarmRank(Smart_Ptr<Player>& player, int monsterId,int value)
{
	bool bResult = false;
	if(player)
	{
		map<int64,CountryMember>::iterator bitr = m_CountryBoss.find(player->GetID());
		if(bitr != m_CountryBoss.end())
		{
			map<int,int>::iterator mHurtItr = bitr->second.mHarm.find(monsterId);
			if(mHurtItr != bitr->second.mHarm.end())
			{
				mHurtItr->second += value;
			}
			else
			{
				bitr->second.mHarm[monsterId]  = value;
			}

			if(player->getAttManage()->getValue<DWORD>(eCharCountry) == m_paramf)
			{
				map<int,int>::iterator targerItr = m_targerHarmf.find(monsterId);
				if(targerItr != m_targerHarmf.end())
				{
					targerItr->second += value;
				}
				else
				{
					m_targerHarmf[monsterId] = value;
				}
			}
			else
			{
				map<int,int>::iterator targerItr = m_targerHarms.find(monsterId);
				if(targerItr != m_targerHarms.end())
				{
					targerItr->second += value;
				}
				else
				{
					m_targerHarms[monsterId] = value;
				}
			}
		}

		map<int,vector<TeamHarm> >::iterator itr =  m_CountryHarm.find(monsterId);
		if(itr != m_CountryHarm.end())
		{
			vector<TeamHarm>::iterator titr =  itr->second.begin();
			for(; titr != itr->second.end(); ++titr)
			{
				if(titr->m_charid == player->GetID())
				{
					titr->m_harm += value;
					bResult = true;
					break;
				}
			}

			if(!bResult)
			{
				TeamHarm harm;
				harm.m_charid = player->GetID();
				harm.m_name = player->GetName();
				harm.m_harm = value;

				itr->second.push_back(harm);
			}
		}
		else
		{
			vector<TeamHarm> vec;
			TeamHarm harm;
			harm.m_charid = player->GetID();
			harm.m_name = player->GetName();
			harm.m_harm = value;

			vec.push_back(harm);
			m_CountryHarm[monsterId] = vec;
		}
	}
}

bool CCountryBossCarbon::IsAttacked(int64 fight, int64 attacked)
{
	Smart_Ptr<Player> player = GetPlayer(attacked);
	if(player)
	{
		return GsCountryMgr::GetInstance()->IsSafeZone(player->GetXNewPos(),player->GetYNewPos(),player->getAttManage()->getValue<DWORD>(eCharCountry));
	}

	return true;
}

bool CCountryBossCarbon::HandlePlayerDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	PlayerKilledEventArgs *tArgs = (PlayerKilledEventArgs *)(&e);

	if(GetSceneID() != tArgs->m_dead->GetMapID())
		return true;

	if(tArgs->m_dead->GetType() != ePlayer)
		return true;

	if(tArgs->m_killer->GetType() == ePlayer || tArgs->m_killer->GetType() == ePet)
	{
		Smart_Ptr<Player>  player;
		Smart_Ptr<GirlWar>  girlwar;

		if(tArgs->m_killer->GetType() == ePet)
		{
			girlwar = tArgs->m_killer;

			player  = girlwar->GetOwner();
		}
		else
		{
			player = tArgs->m_killer;
		}

		map<int64,CountryMember>::iterator bitr = m_CountryBoss.find(player->GetID());
		if(bitr != m_CountryBoss.end())
		{
			bitr->second.nScore += GsCountryMgr::GetInstance()->GetKillEnemyScore();
			bitr->second.nKilling += 1;
		}
	}

	if(tArgs->m_dead)
	{
		map<int64,CountryMember>::iterator bitr = m_CountryBoss.find(tArgs->m_dead->GetID());
		if(bitr != m_CountryBoss.end())
		{
			bitr->second.nKilled += 1;
		}
	}

	return true;
}
