/*
 * ClanIntegralCarbon.cpp
 *
 *  Created on: 2016年2月25日
 *      Author: root
 */

#include "ClanIntegralCarbon.h"
#include "util.h"
#include "Log/Logger.h"
#include "../Object/Player/Player.h"
#include "../Map/DuplicateMgr.h"
#include "../ServerEventDefine.h"
#include "MessageStruct/Clan/ClanIntegral.pb.h"
#include "../Clan/ClanLoader/ClanLoader.h"
#include "../QuickMessage.h"
#include "../Object/Player/PlayerMessage.h"
#include "../ServerManager.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageCommonRet.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../Object/ObjectFactory.h"
#include "../Object/FileLoader/AILoader.h"
#include "../PVP/PVPMgr.h"
#include "../Object/AI/AIBrain.h"
#include "../Mail/MailMessage.h"
#include "./FileLoader/InternationalLoader.h"
#include "FileLoader/GameParaLoader.h"
 #include <algorithm>

bool CmpIntegral(const pair<int64,IntegralInfo> &lParam, const pair<int64,IntegralInfo> &rParam)
{
	return lParam.second.nKill > rParam.second.nKill;
}

void sortMapByValue(map<int64,IntegralInfo>& tMap,vector<pair<int64,IntegralInfo> >& tVector)
{
  for(map<int64,IntegralInfo>::iterator curr=tMap.begin();curr!=tMap.end();curr++)
      {
         tVector.push_back(make_pair(curr->first,curr->second));
      }
      sort(tVector.begin(),tVector.end(),CmpIntegral);
}


CClanIntegralCarbon::CClanIntegralCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):CGameDuplicate(onlyid,charid,paramf,params),m_totalPlayerf(0)
	,m_totalPlayers(0),m_totalBossf(0),m_totalBosss(0),m_clanDelete(-1)
{
	m_integralf.clear();
	m_integrals.clear();
}

CClanIntegralCarbon::~CClanIntegralCarbon()
{
	m_playerAttackEvent.SetNull();

	m_leaveClanEvent.SetNull();

	if(m_reflushTime.IsValid())
	{
		m_reflushTime.SetNull();
	}

	if(m_reflushIntegral.IsValid())
	{
		m_reflushIntegral.SetNull();
	}
}

int CClanIntegralCarbon::Init()
{
	int ret = CGameDuplicate::Init();
	m_reflushTime = TimerInterface::GetInstance()->RegTimePoint(&CClanIntegralCarbon::FlushMonster, this, 0, 0, 0, 0, ClanLoader::GetInstance()->GetRefreshTime());
	m_reflushIntegral = TimerInterface::GetInstance()->RegRepeatedTime(&CClanIntegralCarbon::FlushIntegral, this, 0, 0, 0, 0,5000,5000);
	return ret;
}

bool CClanIntegralCarbon::Update()
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

			if(nowTime - m_createTime > 300000 && GetVailPlayer() != -1)
			{
				Over();
			}

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
			m_awardTime = CUtil::GetNowSecond();
			IntegralEnd();
			m_process = eChangeMapWait;

			break;
		}
	case eChangeMapWait:
		{
			int64 nowTime = CUtil::GetNowSecond();

			if(nowTime - m_awardTime >= 20 * 1000)
			{
				//到副本结束时间就强制传送玩家出副本
				//比需要在10秒内传送出去，不然就不管了
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

int CClanIntegralCarbon::Move(Smart_Ptr<Object> &obj)
{
	CGameZone::Move(obj);
	return 0;
}

bool CClanIntegralCarbon::JoinHandle(Smart_Ptr<Object> & obj)
{
	if(m_process >= eDuplicateChangeMap)
		return false;

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;
		DWORD faction = player->getAttManage()->getValue<DWORD>(eCharFaction);

		if(faction != m_paramf && faction != m_params)
			return false;

		//添加新进成员
		JoinPlayer(player);

		if(!m_monsterDeadEvent.IsValid())
		{
			m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &CClanIntegralCarbon::HandleMonsterDeadEvent);
		}

		if(!m_playerDeadEvent.IsValid())
		{
			m_playerDeadEvent = RegGlobalEventPtr(CREATURE_DEAD_EVENT_TYPE, this, &CClanIntegralCarbon::HandlePlayerDeadEvent);
		}

		if(!m_playerAttackEvent.IsValid())
		{
			m_playerAttackEvent = RegGlobalEventPtr(PLAYER_ATTACK_CREATURE, this, &CClanIntegralCarbon::PlayerAttack);
		}

		if(!m_leaveClanEvent.IsValid())
		{
			m_leaveClanEvent = RegGlobalEventPtr(PLAYER_CLAN_LEAVECLAN, this, &CClanIntegralCarbon::HandleLeaveClanEvent);
		}

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		if(m_process == eCreated)
			m_process = ePlayerWork;
	}

	return true;
}

//加入场景
int CClanIntegralCarbon::Join(Smart_Ptr<Object> & obj)
{
	if(!JoinHandle(obj))
		return -1;

	CGameZone::Join(obj);
	return 0;
}

void CClanIntegralCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
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

			player->UnsetCreatureState(ERS_Invincible, false);
			player->RemoveBuffById(PVPMgr::GetInstance()->getProtectedBufferID());

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

void CClanIntegralCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
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

void CClanIntegralCarbon::LeaveExame(Smart_Ptr<Player> & player)
{

}

bool CClanIntegralCarbon::IsCanEnter(Smart_Ptr<Player> & player)
{
	//传送阶段，就不允许进入了
	if(m_process >= eDuplicateChangeMap)
		return false;

	//玩家死了不入传入地图
	if(player->IsDead())
	{
		return false;
	}

	if(m_paramf != player->getAttManage()->getValue<DWORD>(eCharFaction) && m_params != player->getAttManage()->getValue<DWORD>(eCharFaction))
	{
		return false;
	}

	return true;
}

void CClanIntegralCarbon::Win()
{
	m_isSuccess = true;
	m_process = eAward;
}

void CClanIntegralCarbon::Over()
{
	m_process = eDuplicateChangeMap;
}


void CClanIntegralCarbon::Count()
{

}

void CClanIntegralCarbon::CountEx()
{
	int64 nowTime = CUtil::GetNowSecond();

	if(nowTime - m_leaveTime >= m_data->m_nDelTime)
	{
		m_process = eOver;
	}
}

void CClanIntegralCarbon::Failed()
{
	m_process = eAward;
}

void CClanIntegralCarbon::Award()
{
	m_process = eDuplicateChangeMap;
	m_awardTime = CUtil::GetNowSecond();
}

bool CClanIntegralCarbon::HandleMonsterDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);

	if(GetSceneID() != tArgs->m_myself->GetMapID())
		return true;

	AddBossKilled(tArgs->m_other);

	m_reflushTime = TimerInterface::GetInstance()->RegTimePoint(&CClanIntegralCarbon::FlushMonster, this, 0, 0, 0, 0, ClanLoader::GetInstance()->GetRefreshTime());
	FlushIntegral(NULL);
	return true;
}

bool CClanIntegralCarbon::HandlePlayerDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	PlayerKilledEventArgs *tArgs = (PlayerKilledEventArgs *)(&e);

	if(GetSceneID() != tArgs->m_dead->GetMapID())
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

		if(player->getAttManage()->getValue<DWORD>(eCharFaction) == m_paramf)
		{
			map<int64,IntegralInfo>::iterator itr = m_integralf.find(player->GetID());
			if(itr != m_integralf.end())
			{
				itr->second.nKill += 1;
				itr->second.nTemp += 1;
			}

			m_totalPlayerf += 1;
		}
		else
		{
			map<int64,IntegralInfo>::iterator itr = m_integrals.find(player->GetID());
			if(itr != m_integrals.end())
			{
				itr->second.nKill += 1;
				itr->second.nTemp += 1;
			}

			m_totalPlayers += 1;
		}
	}

	if(tArgs->m_dead)
	{
		Smart_Ptr<Player>  player = tArgs->m_dead;
		if(player->getAttManage()->getValue<DWORD>(eCharFaction) == m_paramf)
		{
			map<int64,IntegralInfo>::iterator itr = m_integralf.find(player->GetID());
			if(itr != m_integralf.end())
			{
				itr->second.nDead += 1;
				if(itr->second.nTemp > itr->second.nMaxKill)
				{
					itr->second.nMaxKill = itr->second.nTemp;
				}

				itr->second.nTemp = 0;
			}
		}
		else
		{
			map<int64,IntegralInfo>::iterator itr = m_integrals.find(player->GetID());
			if(itr != m_integrals.end())
			{
				itr->second.nDead += 1;
				if(itr->second.nTemp > itr->second.nMaxKill)
				{
					itr->second.nMaxKill = itr->second.nTemp;
				}

				itr->second.nTemp = 0;
			}
		}
	}

	return true;
}

void CClanIntegralCarbon::FlushMonster(void * arg)
{
	if(m_process > ePlayerWork)
		return;

	const map<int,BossInfo>* pBossInfo = ClanLoader::GetInstance()->GetBossInfo();
	map<int,BossInfo>::const_iterator itr = pBossInfo->begin();
	for(; itr != pBossInfo->end(); ++ itr)
	{
		Smart_Ptr<Monster> tempMonster = GetMonsterByID(itr->second.monsterId);
		if(tempMonster)
		{
			continue;
		}

		MonsterInfo info;
		vector<Smart_Ptr<Monster> > vecMonster;

		MonsterLoader::GetInstance()->GetMonsterInfo(itr->second.monsterId, info);
		if(info.id == -1)
		{
			LOG_ERROR(FILEINFO, "monster id is error");

			continue;
		}

		//BOSS刷新后全属性增长倍率
		int rate = ClanLoader::GetInstance()->GetBossRate(GetFlushNum(itr->second.monsterId));

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
		mon->SetFallID(info.fallID);
		mon->getAttManage()->setValue(eCharExp,info.exp);
		mon->getAttManage()->setValue(eCharPhyAttackMax,info.maxAttack * rate);
		mon->getAttManage()->setValue(eCharPhyDefence,info.phyDefence);
		mon->getAttManage()->setValue(eCharHitRate,info.hitRate);
		mon->getAttManage()->setValue(eCharAvoidRate,info.avoidRate);
		mon->getAttManage()->setValue(eCrackDownRate,info.crackRate);
		mon->getAttManage()->setValue(eAvoidCrackRate,info.avoidCrack);
		mon->SetHPMax(info.maxHP * rate);
		mon->SetCurHP(info.maxHP * rate);
		mon->SetMPMax(info.maxMP);
		mon->SetCurMP(info.maxMP);
		mon->SetGatherTime(info.gatherTime);	//采集时间
		mon->SetOwnerToBrain(mon);
		mon->SetMapID(m_sceneID);
		mon->SetBornTime();
		mon->SetOwnerName("");
		mon->GetBrain()->SetMonsterType(eRandomWayPos);
		mon->SetXPos(itr->second.pos.xpos);
		mon->SetYPos(itr->second.pos.ypos);
		mon->SetOriginalX(itr->second.pos.xpos);
		mon->SetOriginalY(itr->second.pos.ypos);

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
}

int CClanIntegralCarbon::GetFlushNum(int monsterId)
{
	int nNum = 1;
	map<int,int>::iterator itr = m_flushNum.find(monsterId);
	if(itr != m_flushNum.end())
	{
			itr->second += 1;
			nNum = itr->second;
	}
	else
	{
		m_flushNum[monsterId] = 1;
	}

	return nNum;
}

void CClanIntegralCarbon::JoinPlayer(Smart_Ptr<Player>& player)
{
	ClanPackage::SingleKill toClient;
	if(player->getAttManage()->getValue<DWORD>(eCharFaction) == m_paramf)
	{
		map<int64,IntegralInfo>::iterator itr = m_integralf.find(player->GetID());
		if(itr == m_integralf.end())
		{
			IntegralInfo info;
			info.nCharid = player->GetID();
			info.sName = player->GetName();
			info.nRanking = m_integralf.size() + 1;
			m_integralf[player->GetID()] = info;

			toClient.set_ranking(info.nRanking);
		}
		else
		{
			toClient.set_ranking(itr->second.nRanking);
			toClient.set_kill(itr->second.nKill);
			toClient.set_boss(itr->second.nBoss);
			toClient.set_die(itr->second.nDead);
		}

		toClient.set_total(m_totalPlayerf);
		toClient.set_totalboss(m_totalBossf);
		toClient.set_othertotal(m_totalPlayers);
		toClient.set_othertotalboss(m_totalBosss);
	}
	else
	{
		map<int64,IntegralInfo>::iterator itr = m_integrals.find(player->GetID());
		if(itr == m_integrals.end())
		{
			IntegralInfo info;
			info.nCharid = player->GetID();
			info.sName = player->GetName();
			info.nRanking = m_integrals.size() + 1;
			m_integrals[player->GetID()] = info;

			toClient.set_ranking(info.nRanking);
		}
		else
		{
			toClient.set_ranking(itr->second.nRanking);
			toClient.set_kill(itr->second.nKill);
			toClient.set_boss(itr->second.nBoss);
			toClient.set_die(itr->second.nDead);
		}

		toClient.set_total(m_totalPlayers);
		toClient.set_totalboss(m_totalBosss);
		toClient.set_othertotal(m_totalPlayerf);
		toClient.set_othertotalboss(m_totalBossf);
	}

	QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &toClient, MSG_SIM_GM2C_INTEGRAL_SINGLEKILL);
}

int CClanIntegralCarbon::GetClanIntegralRanking(Smart_Ptr<Player>& player,ClanPackage::RetKillRankingInfo& info)
{
	if(player->getAttManage()->getValue<DWORD>(eCharFaction) == m_paramf)
	{
		ClanPackage::KillRanking* pMySelf = info.mutable_myself();
		pMySelf->set_total(m_totalPlayerf);
		pMySelf->set_boss(m_totalBossf);

		map<int64,IntegralInfo>::iterator mitr = m_integralf.begin();
		for(; mitr != m_integralf.end(); ++mitr)
		{
			ClanPackage::SingleKillRanking* pSingle = pMySelf->add_info();
			pSingle->set_name(mitr->second.sName);
			pSingle->set_kill(mitr->second.nKill);
			pSingle->set_die(mitr->second.nDead);
		}

		ClanPackage::KillRanking* pOther = info.mutable_other();
		pOther->set_total(m_totalPlayers);
		pOther->set_boss(m_totalBosss);

		map<int64,IntegralInfo>::iterator oitr = m_integrals.begin();
		for(; oitr != m_integrals.end(); ++oitr)
		{
			ClanPackage::SingleKillRanking* pSingle = pOther->add_info();
			pSingle->set_name(oitr->second.sName);
			pSingle->set_kill(oitr->second.nKill);
			pSingle->set_die(oitr->second.nDead);
		}
	}
	else
	{

		ClanPackage::KillRanking* pMySelf = info.mutable_myself();
		pMySelf->set_total(m_totalPlayers);
		pMySelf->set_boss(m_totalBosss);

		map<int64,IntegralInfo>::iterator mitr = m_integrals.begin();
		for(; mitr != m_integrals.end(); ++mitr)
		{
			ClanPackage::SingleKillRanking* pSingle = pMySelf->add_info();
			pSingle->set_name(mitr->second.sName);
			pSingle->set_kill(mitr->second.nKill);
			pSingle->set_die(mitr->second.nDead);
		}

		ClanPackage::KillRanking* pOther = info.mutable_other();
		pOther->set_total(m_totalPlayerf);
		pOther->set_boss(m_totalBossf);

		map<int64,IntegralInfo>::iterator oitr = m_integralf.begin();
		for(; oitr != m_integralf.end(); ++oitr)
		{
			ClanPackage::SingleKillRanking* pSingle = pOther->add_info();
			pSingle->set_name(oitr->second.sName);
			pSingle->set_kill(oitr->second.nKill);
			pSingle->set_die(oitr->second.nDead);
		}
	}

	return 0;
}

void CClanIntegralCarbon::FlushIntegral(void * arg)
{
	Smart_Ptr<Player> player;
	ClanPackage::SingleKill toClient;
	int nRank = 1;
	vector<pair<int64,IntegralInfo> > vecf;
	vector<pair<int64,IntegralInfo> > vecs;

	sortMapByValue(m_integralf,vecf);
	vector<pair<int64,IntegralInfo> >::iterator itrf = vecf.begin();
	for(; itrf != vecf.end(); ++itrf)
	{
		itrf->second.nRanking = nRank;
		nRank++;

		player = GetPlayer(itrf->second.nCharid);
		if(player)
		{
			toClient.set_ranking(itrf->second.nRanking);
			toClient.set_kill(itrf->second.nKill);
			toClient.set_boss(itrf->second.nBoss);
			toClient.set_die(itrf->second.nDead);

			toClient.set_total(m_totalPlayerf);
			toClient.set_totalboss(m_totalBossf);
			toClient.set_othertotal(m_totalPlayers);
			toClient.set_othertotalboss(m_totalBosss);

			QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &toClient, MSG_SIM_GM2C_INTEGRAL_SINGLEKILL);
		}
	}

	nRank = 1;
	sortMapByValue(m_integrals,vecs);
	vector<pair<int64,IntegralInfo> >::iterator itrs = vecs.begin();
	for(; itrs != vecs.end(); ++itrs)
	{
		itrs->second.nRanking = nRank;
		nRank++;
		player = GetPlayer(itrs->second.nCharid);
		if(player)
		{
			toClient.set_ranking(itrs->second.nRanking);
			toClient.set_kill(itrs->second.nKill);
			toClient.set_boss(itrs->second.nBoss);
			toClient.set_die(itrs->second.nDead);

			toClient.set_total(m_totalPlayers);
			toClient.set_totalboss(m_totalBosss);
			toClient.set_othertotal(m_totalPlayerf);
			toClient.set_othertotalboss(m_totalBossf);

			QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &toClient, MSG_SIM_GM2C_INTEGRAL_SINGLEKILL);
		}
	}
}

void CClanIntegralCarbon::AddBossKilled(Smart_Ptr<CreatureObj>& object)
{
	Smart_Ptr<Player> player;
	Smart_Ptr<GirlWar> girlwar;
	if(object)
	{
		if(object->GetType() != ePlayer && object->GetType() !=  ePet)
			return;

		if(object->GetType() == ePet)
		{
			girlwar = object;

			player  = girlwar->GetOwner();
		}
		else
		{
			player = object;
		}

		if(player->getAttManage()->getValue<DWORD>(eCharFaction) == m_paramf)
		{
			map<int64,IntegralInfo>::iterator itr = m_integralf.find(player->GetID());
			if(itr != m_integralf.end())
			{
				itr->second.nBoss += 1;
			}

			m_totalBossf += 1;
		}
		else
		{
			map<int64,IntegralInfo>::iterator itr = m_integrals.find(player->GetID());
			if(itr != m_integrals.end())
			{
				itr->second.nBoss += 1;
			}

			m_totalBosss += 1;
		}
	}
}

void CClanIntegralCarbon::IntegralEnd()
{
	//如果m_paramf胜利 则 nFlag 为 1
	int  nFlag = 0;
	Smart_Ptr<Player> player;
	ClanPackage::IntegralReward toClientf;
	ClanPackage::IntegralReward toClients;
	ClanPackage::SysIntegralToWorld toWorld;
	int nTime = m_awardTime - m_createTime;
	vector<pair<int64,IntegralInfo> > vecf;
	vector<pair<int64,IntegralInfo> > vecs;

	int scorePeoplef = ClanLoader::GetInstance()->GetScorePeople(m_totalPlayerf);
	int scoreBossf = ClanLoader::GetInstance()->GetScorePeople(m_totalBossf);
	int GetScoreAlivef = ClanLoader::GetInstance()->GetScoreAlive(m_totalPlayerf,nTime);

	int scorePeoples = ClanLoader::GetInstance()->GetScorePeople(m_totalPlayers);
	int scoreBosss = ClanLoader::GetInstance()->GetScorePeople(m_totalBosss);
	int GetScoreAlives = ClanLoader::GetInstance()->GetScoreAlive(m_totalPlayers,nTime);

	int moneyScoref = ClanLoader::GetInstance()->GetMoneyScore(m_totalPlayerf);
	int moneyScores = ClanLoader::GetInstance()->GetMoneyScore(m_totalPlayers);

	ClanPackage::SysIntegral* pIntegralf = toWorld.mutable_integralf();
	ClanPackage::SysIntegral* pIntegrals = toWorld.mutable_integrals();

	nFlag = GetVailPlayer();
	if(nFlag == -1)
	{
		if(scorePeoplef > scorePeoples || (scorePeoplef == scorePeoples && scoreBossf > scoreBosss) || (scorePeoplef == scorePeoples && scoreBossf == scoreBosss && GetScoreAlivef > GetScoreAlives))
		{
			nFlag = 1;
		}
		else if(scorePeoplef == scorePeoples && scoreBossf == scoreBosss && GetScoreAlivef == GetScoreAlives)
		{
			nFlag = 2;
		}
		else
		{
			nFlag = 0;
		}
	}

	if(nFlag == 1)
	{
		int victoryScore = ClanLoader::GetInstance()->GetVictoryScore(scorePeoplef,scoreBossf,GetScoreAlivef);
		int failScore = ClanLoader::GetInstance()->GetFailScore(scorePeoples,scoreBosss,GetScoreAlives);

		toClientf.set_issuccess(true);
		toClientf.set_clanintegral(victoryScore);

		toClients.set_issuccess(false);
		toClients.set_clanintegral(failScore);

	}
	else if(nFlag == 2)
	{
		int victoryScoref = ClanLoader::GetInstance()->GetVictoryScore(scorePeoplef,scoreBossf,GetScoreAlivef);
		int victoryScores = ClanLoader::GetInstance()->GetVictoryScore(scorePeoples,scoreBosss,GetScoreAlives);

		toClientf.set_issuccess(true);
		toClientf.set_clanintegral(victoryScoref);

		toClients.set_issuccess(true);
		toClients.set_clanintegral(victoryScores);

	}
	else
	{
		int victoryScore = ClanLoader::GetInstance()->GetVictoryScore(scorePeoples,scoreBosss,GetScoreAlives);
		int failScore = ClanLoader::GetInstance()->GetFailScore(scorePeoplef,scoreBossf,GetScoreAlivef);

		toClientf.set_issuccess(false);
		toClientf.set_clanintegral(failScore);
		toClients.set_issuccess(true);
		toClients.set_clanintegral(victoryScore);
	}

	toClientf.set_clanmoney(moneyScoref);
	toClients.set_clanmoney(moneyScores);

	pIntegralf->set_clanid(m_paramf);
	pIntegralf->set_clanintegral(toClientf.clanintegral());
	pIntegralf->set_clanmoney(toClientf.clanmoney());
	pIntegralf->set_issuccess(toClientf.issuccess());

	pIntegrals->set_clanid(m_params);
	pIntegrals->set_clanintegral(toClients.clanintegral());
	pIntegrals->set_clanmoney(toClients.clanmoney());
	pIntegrals->set_issuccess(toClients.issuccess());


	//////////////////////////过滤离开副本玩家////////////////////////////////////////////////////////////////////
	DorpIntegralLeave();

	int cotriId = GameParaLoader::GetInstance()->getExchangeContributionItemId();
	int moneyId = GameParaLoader::GetInstance()->getExchangeMoneyItemId();
	int gasId = GameParaLoader::GetInstance()->getExchangeForceItemId();

	int64 cotri = 0;
	int64 money = 0;
	int64 gas = 0;

	const IntegralRewards* pSingkillitems = ClanLoader::GetInstance()->GetIntegralRewards(1,1);

	bool bResult = true;

	sortMapByValue(m_integralf,vecf);
	vector<pair<int64,IntegralInfo> >::iterator mitr = vecf.begin();
	for(; mitr != vecf.end(); ++mitr)
	{
		player = GetPlayer(mitr->first);
		if(player)
		{
			vector<int64> goods;
			cotri = ClanLoader::GetInstance()->GetCotriReward(mitr->second.nKill);
			money = ClanLoader::GetInstance()->GetMoneyReward(mitr->second.nKill);
			gas = ClanLoader::GetInstance()->GetGasReward(mitr->second.nKill);

			ClanPackage::RewardInfo* cotriIitems = toClientf.add_items();
			cotriIitems->set_itemid(cotriId);
			cotriIitems->set_itemnum(cotri);
			ClanPackage::RewardInfo* moneyIitems = toClientf.add_items();
			moneyIitems->set_itemid(moneyId);
			moneyIitems->set_itemnum(money);
			ClanPackage::RewardInfo* gasIitems = toClientf.add_items();
			gasIitems->set_itemid(gasId);
			gasIitems->set_itemnum(gas);

			goods.push_back((cotri << 32)| cotriId);
			goods.push_back((money << 32)| moneyId);
			goods.push_back((gas << 32)| gasId);

			toClientf.set_kill(mitr->second.nKill);

			if(bResult)
			{
				ClanPackage::RewardInfo* mvpIitems = toClientf.add_mvpitems();
				int mvpItemsId = ClanLoader::GetInstance()->GetMVPRewards(player->getAttManage()->getValue<int>(eCharLv));
				mvpIitems->set_itemid(mvpItemsId);
				mvpIitems->set_itemnum(1);
				goods.push_back(((int64)1 << 32)| mvpItemsId);

				bResult = false;
			}

			if(mitr->second.nKill > 0 && pSingkillitems)
			{
				for(size_t i = 0; i < pSingkillitems->itemList.size(); i++)
				{
					ClanPackage::RewardInfo* singkillitems = toClientf.add_singkillitems();
					singkillitems->set_itemid(pSingkillitems->itemList[i]);
					singkillitems->set_itemnum(mitr->second.nKill);

					goods.push_back(((int64)mitr->second.nKill << 32)| pSingkillitems->itemList[i]);
				}
			}

			const IntegralRewards* pkillitems = ClanLoader::GetInstance()->GetIntegralRewards(3,mitr->second.nMaxKill);
			if(pkillitems)
			{
				for(size_t j = 0; j < pkillitems->itemList.size(); j++)
				{
					ClanPackage::RewardInfo* killitems = toClientf.add_killitems();
					killitems->set_itemid(pkillitems->itemList[j]);
					killitems->set_itemnum(pkillitems->num[j]);

					goods.push_back(((int64)pkillitems->num[j] << 32)| pkillitems->itemList[j]);
				}
			}

			string warning = InternationalLoader::GetInstance()->GetInterStr(59);
			string warning1 = InternationalLoader::GetInstance()->GetInterStr(59);

			pIntegralf->add_charidlist(player->GetID());

			MailMessageMgr::GetInstance()->SendSysMail(goods, player->GetName(),0,warning.c_str(),warning1.c_str());
			QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &toClientf, MSG_SIM_GM2C_INTEGRAL_REWARD);
		}

		toClientf.clear_items();
		toClientf.clear_mvpitems();
		toClientf.clear_singkillitems();
		toClientf.clear_killitems();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bResult = true;
	sortMapByValue(m_integrals,vecs);
	vector<pair<int64,IntegralInfo> >::iterator itrs = vecs.begin();
	for(; itrs != vecs.end(); ++itrs)
	{
		player = GetPlayer(itrs->first);
		if(player)
		{
			vector<int64> goods;
			cotri = ClanLoader::GetInstance()->GetCotriReward(itrs->second.nKill);
			money = ClanLoader::GetInstance()->GetMoneyReward(itrs->second.nKill);
			gas = ClanLoader::GetInstance()->GetGasReward(itrs->second.nKill);

			ClanPackage::RewardInfo* cotriIitems = toClients.add_items();
			cotriIitems->set_itemid(cotriId);
			cotriIitems->set_itemnum(cotri);
			ClanPackage::RewardInfo* moneyIitems = toClients.add_items();
			moneyIitems->set_itemid(moneyId);
			moneyIitems->set_itemnum(money);
			ClanPackage::RewardInfo* gasIitems = toClients.add_items();
			gasIitems->set_itemid(gasId);
			gasIitems->set_itemnum(gas);

			goods.push_back((cotri << 32)| cotriId);
			goods.push_back((money << 32)| moneyId);
			goods.push_back((gas << 32)| gasId);

			toClients.set_kill(itrs->second.nKill);

			if(bResult)
			{
				ClanPackage::RewardInfo* mvpIitems = toClients.add_mvpitems();
				int mvpItemsId = ClanLoader::GetInstance()->GetMVPRewards(player->getAttManage()->getValue<int>(eCharLv));
				mvpIitems->set_itemid(mvpItemsId);
				mvpIitems->set_itemnum(1);
				goods.push_back(((int64)1 << 32)| mvpItemsId);
				bResult = false;
			}

			if(itrs->second.nKill > 0 && pSingkillitems)
			{
				for(size_t i = 0; i < pSingkillitems->itemList.size(); i++)
				{
					ClanPackage::RewardInfo* singkillitems = toClients.add_singkillitems();
					singkillitems->set_itemid(pSingkillitems->itemList[i]);
					singkillitems->set_itemnum(itrs->second.nKill);

					goods.push_back(((int64)itrs->second.nKill << 32)| pSingkillitems->itemList[i]);
				}
			}

			const IntegralRewards* pkillitems = ClanLoader::GetInstance()->GetIntegralRewards(3,itrs->second.nMaxKill);
			if(pkillitems)
			{
				for(size_t j = 0; j < pkillitems->itemList.size(); j++)
				{
					ClanPackage::RewardInfo* killitems = toClients.add_killitems();
					killitems->set_itemid(pkillitems->itemList[j]);
					killitems->set_itemnum(pkillitems->num[j]);

					goods.push_back(((int64)pkillitems->num[j] << 32)| pkillitems->itemList[j]);
				}
			}

			string warning = InternationalLoader::GetInstance()->GetInterStr(59);
			string warning1 = InternationalLoader::GetInstance()->GetInterStr(59);

			pIntegrals->add_charidlist(player->GetID());

			MailMessageMgr::GetInstance()->SendSysMail(goods, player->GetName(),0,warning.c_str(),warning1.c_str());
			QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &toClients, MSG_SIM_GM2C_INTEGRAL_REWARD);
		}

		toClients.clear_items();
		toClients.clear_mvpitems();
		toClients.clear_singkillitems();
		toClients.clear_killitems();
	}

	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(m_owner,worldSvr);
	if(worldSvr)
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_INTEGRAL_SYSRESULT, &toWorld, worldSvr.Get(), SimpleMessage);
		Message_Facade::Send(messRet);
	}
}

void CClanIntegralCarbon::DorpIntegralLeave()
{
	Smart_Ptr<Player> player;
	map<int64,IntegralInfo>::iterator mitr = m_integralf.begin();
	for(; mitr != m_integralf.end();)
	{
		player = GetPlayer(mitr->first);
		if(!player)
		{
			m_integralf.erase(mitr++);
		}
		else
		{
			if(mitr->second.nTemp > mitr->second.nMaxKill)
			{
				mitr->second.nMaxKill = mitr->second.nTemp;
			}

			mitr++;
		}
	}


	map<int64,IntegralInfo>::iterator itrs = m_integrals.begin();
	for(; itrs != m_integrals.end();)
	{
		player = GetPlayer(itrs->first);
		if(!player)
		{
			m_integrals.erase(itrs++);
		}
		else
		{
			if(itrs->second.nTemp > itrs->second.nMaxKill)
			{
				itrs->second.nMaxKill = itrs->second.nTemp;
			}

			itrs++;
		}
	}
}

bool CClanIntegralCarbon::PlayerAttack(const EventArgs & e)
{
		PlayerAttackCreature *pArgs = (PlayerAttackCreature *) (&e);
		if(pArgs==NULL || pArgs->m_player.Get()==NULL || pArgs->m_player->GetMapID() != m_sceneID)
			return true;

		//取消无敌状态
		pArgs->m_player->UnsetCreatureState(ERS_Invincible, false);
		pArgs->m_player->RemoveBuffById(PVPMgr::GetInstance()->getProtectedBufferID());

		return true;
}

bool CClanIntegralCarbon::HandleLeaveClanEvent(const EventArgs & e)
{
	PlayerLeaveClanArgs *tArgs = (PlayerLeaveClanArgs *)(&e);

	if(tArgs->m_obj.Get()==NULL || GetSceneID() != tArgs->m_obj->GetMapID())
		return true;

	Smart_Ptr<Player> player = tArgs->m_obj;

	map<int64,OldMapInfo>::iterator itMap = m_oldMapPoint.find(player->GetID());
	if(itMap != m_oldMapPoint.end())
	{
		PlayerMessage::GetInstance()->ForceChangeMap(player, itMap->second.mapid, itMap->second.xpos, itMap->second.ypos);
	}
	else
	{
		MapStaticInfo *sMapInfo =	NULL;
		int64 sceneid = 0;

		sMapInfo = CMapConfig::GetInstance()->GetFistMapInfo();
		if (!sMapInfo)
			return false;

		sceneid = CSceneMgr::GetInstance()->GetSceneIDAboutPlayerIsLoginOn(sMapInfo->mapid);
		if(sceneid <= 0)
		{
			player->SetMapID(CREATE_MAP(eStaticMap, 1, 1, sMapInfo->mapid, 1));
			sceneid = player->GetMapID();
		}
		else
		{
			player->SetMapID(sceneid);
		}

		PlayerMessage::GetInstance()->ForceChangeMap(player, sceneid, sMapInfo->reliveXPos, sMapInfo->reliveYPos);
	}

	return true;
}

int CClanIntegralCarbon::GetVailPlayer()
{
	int countf = 0;
	int counts = 0;
	INT64PLAYERMAP::iterator itr = m_player.begin();
	for(; itr != m_player.end(); ++ itr)
	{
		if(itr->second->getAttManage()->getValue<DWORD>(eCharFaction) == m_paramf)
		{
			countf++;
		}
		else
		{
			counts++;
		}
	}

	if(m_clanDelete > -1)
	{
		return m_clanDelete;
	}

	if(countf == 0 && counts == 0)
	{
		return 2;
	}

	if(countf == 0)
	{
		return 0;
	}

	if(counts == 0)
	{
		return 1;
	}

	return -1;
}

void CClanIntegralCarbon::ClanDelete(int clanid)
{
	if(clanid == m_paramf)
	{
		m_process = eDuplicateChangeMap;
		m_clanDelete = 0;
	}
	else if(clanid == m_params)
	{
		m_process = eDuplicateChangeMap;
		m_clanDelete = 1;
	}
	else
	{
		return;
	}
}
