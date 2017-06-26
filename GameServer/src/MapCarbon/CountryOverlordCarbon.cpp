/*
 * CountryOverlordCarbon.cpp
 *
 *  Created on: 2016年10月28日
 *      Author: root
 */

#include "CountryOverlordCarbon.h"
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
#include "../MapInstance/BeyondBattleMgr.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../Object/ItemDrop/DropItemAlloc.h"
#include "../Chat/ChatMgr.h"
#include "MessageStruct/Country/CountryWar.pb.h"
#include "../Container/ArticleEquipMgr.h"
#include "../Country/GsCountryMgr.h"
#include "../Mail/MailMessage.h"
#include "./FileLoader/InternationalLoader.h"
#include "../Country/CountryOverlordCarbonLoader.h"
#include "../Country/GsCountryWarMgr.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "MessageStruct/Country/CountryOverlord.pb.h"


CCountryOverlordCarbon::CCountryOverlordCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params,vector<int64>& charId):CGameDuplicate(onlyid,charid,paramf,params)
{
	m_worldServerID = 0;
	m_nCanEntr = charId;
	m_Tang.clear();
	m_Sui.clear();
}

CCountryOverlordCarbon::~CCountryOverlordCarbon()
{
  if(m_WarPlayerSortTimer.IsValid())
	  m_WarPlayerSortTimer.SetNull();

  if(m_reflushTime.IsValid())
	  m_reflushTime.SetNull();
}

int CCountryOverlordCarbon::Init()
{
	int ret = CGameDuplicate::Init();

	  if(m_WarPlayerSortTimer.IsValid())
		  m_WarPlayerSortTimer.SetNull();

	  if(m_reflushTime.IsValid())
		  m_reflushTime.SetNull();

	  m_WarPlayerSortTimer=TimerInterface::GetInstance()->RegRepeatedTime(&CCountryOverlordCarbon::onUpdateSort, this, 0, 0, 0, 0, 0, 50000);

	  const OverlordBaseData* pData = CountryOverlordCarbonLoader::GetInstance()->GetOverlordBaseData();
	  if(pData)
	  {
		  m_reflushTime = TimerInterface::GetInstance()->RegRepeatedTime(&CCountryOverlordCarbon::FlushMonster, this, 0, 0, 0, 0, 0,pData->m_nFlushTime);
	  }

	return ret;
}

bool CCountryOverlordCarbon::Update()
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


int CCountryOverlordCarbon::Join(Smart_Ptr<Object> & obj)
{
	if(!JoinHandle(obj))
		return -1;

	CGameZone::Join(obj);

	//初始化玩家
	if(obj->GetType()==ePlayer)
	{
		int64 id=((Smart_Ptr<Player> &)obj)->GetID();
		int country = (int)((Smart_Ptr<Player> &)obj)->getAttManage()->getValue<DWORD>(eCharCountry);
		//第一个玩家进来时取世界服服id
		if(m_worldServerID==0)
		{
			m_worldServerID=GET_SERVER_CHARID(id);
		}

		if(country == eCountryID_sui)
		{
			map<int,map<int64,LordData> >::iterator itr = m_Sui.find(country);
			if(itr != m_Sui.end())
			{
				map<int64,LordData>::iterator it = itr->second.find(id);
				if(it == itr->second.end())
				{
					LordData data;
					data.sName = ((Smart_Ptr<Player> &)obj)->GetName();
					data.sClanName = ((Smart_Ptr<Player> &)obj)->GetClanName();

					m_Sui[country][id] = data;
				}
			}
			else
			{
				LordData data;
				data.sName = ((Smart_Ptr<Player> &)obj)->GetName();
				data.sClanName = ((Smart_Ptr<Player> &)obj)->GetClanName();

				m_Sui[country][id] = data;
			}
		}
		else
		{
			map<int,map<int64,LordData> >::iterator itr = m_Tang.find(country);
			if(itr != m_Tang.end())
			{
				map<int64,LordData>::iterator it = itr->second.find(id);
				if(it == itr->second.end())
				{
					LordData data;
					data.sName = ((Smart_Ptr<Player> &)obj)->GetName();
					data.sClanName = ((Smart_Ptr<Player> &)obj)->GetClanName();

					m_Tang[country][id] = data;
				}
			}
			else
			{
				LordData data;
				data.sName = ((Smart_Ptr<Player> &)obj)->GetName();
				data.sClanName = ((Smart_Ptr<Player> &)obj)->GetClanName();

				m_Tang[country][id] = data;
			}
		}
	}

	return 0;
}

bool CCountryOverlordCarbon::JoinHandle(Smart_Ptr<Object> & obj)
{
	if(m_process >= eDuplicateChangeMap)
		return false;

	MapInstanceProto::SimInstanceUpdate toClient;

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		if(m_process != eCreated)
		{
			INTVMPNSTERMAP::iterator itrEnd = m_allMonster.end();
			for(INTVMPNSTERMAP::iterator itr = m_allMonster.begin(); itr != itrEnd; ++itr)
			{
				toClient.add_remainmonsterid(itr->first);
				toClient.add_remainmonsternum(itr->second.size());
			}

			toClient.set_remaintime(m_createTime + m_data->m_nLimitTime - CUtil::GetNowSecond());

			QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GT_UPDATEINSTANCE);
		}

		if(!m_monsterDeadEvent.IsValid())
		{
			m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &CCountryOverlordCarbon::HandleMonsterDeadEvent);
		}

		if(!m_playerDeadEvent.IsValid())
		{
			m_playerDeadEvent = RegGlobalEventPtr(CREATURE_DEAD_EVENT_TYPE, this, &CCountryOverlordCarbon::HandlePlayerDeadEvent);
		}

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		if(m_process == eCreated)
			m_process = ePlayerWork;
	}

	return true;
}

int CCountryOverlordCarbon::Move(Smart_Ptr<Object> &obj)
{
	CGameZone::Move(obj);
	return 0;
}

void CCountryOverlordCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);


	CGameZone::Leave(obj, isDel, delFromMap);

	if(GetPlayerCount() <= 0)
	{
		m_monsterDeadEvent.SetNull();
		m_playerDeadEvent.SetNull();
		m_leaveTime = CUtil::GetNowSecond();
	}
}

void CCountryOverlordCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
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

void CCountryOverlordCarbon::LeaveExame(Smart_Ptr<Player> & player)
{

}

bool CCountryOverlordCarbon::IsCanEnter(Smart_Ptr<Player> & player)
{
	//传送阶段，就不允许进入了
	if(m_process >= eDuplicateChangeMap)
		return false;

	//玩家死了不入传入地图
	if(player->IsDead())
	{
		return false;
	}

	if(player->getAttManage()->getValue<DWORD>(eCharCountry) <= eCountryID_null || player->getAttManage()->getValue<DWORD>(eCharCountry) >= eCountryID_max)
		return false;

	if(!IsCanEnterList(player->GetID()))
	{
		return false;
	}

	return true;
}

void CCountryOverlordCarbon::Win()
{
	m_isSuccess = true;
	m_process = eAward;
}

void CCountryOverlordCarbon::Over()
{
	m_process = eDuplicateChangeMap;
}

void CCountryOverlordCarbon::CountEx()
{
	int64 nowTime = CUtil::GetNowSecond();
	if(nowTime - m_leaveTime >= m_data->m_nDelTime)
	{
		m_process = eOver;
	}
}

void CCountryOverlordCarbon::Count()
{

}

void CCountryOverlordCarbon::Failed()
{
	m_process = eAward;
}

void CCountryOverlordCarbon::Award()
{
	m_process = eDuplicateChangeMap;
	m_awardTime = CUtil::GetNowSecond();
}

bool CCountryOverlordCarbon::IsAttacked(int64 fight, int64 attacked)
{
	return true;
}

bool CCountryOverlordCarbon::HandleMonsterDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);

	if(GetSceneID() != tArgs->m_myself->GetMapID())
		return true;


	Smart_Ptr<Monster> mon = tArgs->m_myself;
	LOG_DEBUG(FILEINFO, "CGameDuplicate::HandleMonsterDeadEvent \t %lld", mon->GetID());

	if(tArgs->m_myself->GetType() == eMonster && mon->GetQuality() == eBossMonster)
	{
		Smart_Ptr<Player>  player;
		Smart_Ptr<GirlWar>  girlwar;

		map<int64, int> enemyValue;
		mon->GetEnemyValue(enemyValue);

		if(tArgs->m_other->GetType()== ePlayer)
		{
			player = tArgs->m_other;
		}
		else if(tArgs->m_other->GetType() == ePet)
		{
			girlwar = tArgs->m_other;
			player  = girlwar->GetOwner();
		}

		if(player.Get()!=NULL)
		{
			addScore(player->GetID(),CountryOverlordCarbonLoader::GetInstance()->GetKillBossIntegral());
		}

		map<int64,int>::iterator itr = enemyValue.begin();
		for(; itr != enemyValue.end(); ++itr)
		{
			addScore(itr->first,CountryOverlordCarbonLoader::GetInstance()->GetAttackBossIntegral());
		}

		ChatMgr::GetInstance()->Marquee(eMarqueeCountryOverlordKill1ed,mon->GetName(),player->GetName(),"");
	}

	return true;
}

bool CCountryOverlordCarbon::HandlePlayerDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	PlayerKilledEventArgs *tArgs = (PlayerKilledEventArgs *)(&e);

	if(GetSceneID() != tArgs->m_dead->GetMapID() || tArgs->m_dead->GetType() != ePlayer)
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

		//添加积分
		addScore(player->GetID(),CountryOverlordCarbonLoader::GetInstance()->GetKillPlayerIntegral(),1);
		//清空连续击杀
		int nCurKiller = clearnCounKiller(tArgs->m_dead->GetID(),tArgs->m_dead->getAttManage()->getValue<DWORD>(eCharCountry));
		if(nCurKiller > 5)
		{
			ChatMgr::GetInstance()->Marquee(eMarqueeCountryOverlordKill1ed,player->GetName(),tArgs->m_dead->GetName(),"");
		}
	}

	return true;
}

bool CCountryOverlordCarbon::IsCanEnterList(int64 charId)
{
	vector<int64>::iterator itr = m_nCanEntr.begin();
	for(; itr != m_nCanEntr.end(); ++itr)
	{
		if(*itr == charId)
		{
			return true;
		}
	}

	return false;
}

void CCountryOverlordCarbon::addScore(int64 charid,int nIntegral,int nKiller)
{
	map<int,map<int64,LordData> >::iterator itrTang =  m_Tang.begin();
	for(;itrTang != m_Tang.end(); ++itrTang)
	{
		map<int64,LordData>::iterator itr = itrTang->second.find(charid);
		if(itr != itrTang->second.end())
		{
			itr->second.nIntegral += nIntegral;
			itr->second.nKiller += nKiller;
			itr->second.nCounKiller += nKiller;

			if(nKiller > 0)
			{
				switch(itr->second.nCounKiller)
				{
				case 3:
				case 5:
				case 8:
					{
						ChatMgr::GetInstance()->Marquee(eMarqueeCountryOverlordKill1,itr->second.sName,ChatMgr::GetInstance()->GetStringByInt(itr->second.nCounKiller),"");
						break;
					}
				}
			}
			return;
		}
	}

	map<int,map<int64,LordData> >::iterator itrSui = m_Sui.begin();
	for(;itrSui != m_Sui.end(); ++itrSui)
	{
		map<int64,LordData>::iterator itr = itrSui->second.find(charid);
		if(itr != itrSui->second.end())
		{
			itr->second.nIntegral += nIntegral;
			itr->second.nKiller += nKiller;
			itr->second.nCounKiller += nKiller;

			if(nKiller > 0)
			{
				switch(itr->second.nCounKiller)
				{
				case 3:
				case 5:
				case 8:
					{
						ChatMgr::GetInstance()->Marquee(eMarqueeCountryOverlordKill1,itr->second.sName,ChatMgr::GetInstance()->GetStringByInt(itr->second.nCounKiller),"");
						break;
					}
				}
			}

			return;
		}
	}
}

int CCountryOverlordCarbon::clearnCounKiller(int64 charid,int nType)
{
	int nTemp = 0;
	if(nType == eCountryID_tang)
	{
		map<int,map<int64,LordData> >::iterator itrTang =  m_Tang.begin();
		for(;itrTang != m_Tang.end(); ++itrTang)
		{
			map<int64,LordData>::iterator itr = itrTang->second.find(charid);
			if(itr != itrTang->second.end())
			{
				nTemp = itr->second.nCounKiller;
				itr->second.nCounKiller = 0;
				return nTemp;
			}
		}
	}
	else
	{
		map<int,map<int64,LordData> >::iterator itrSui = m_Sui.begin();
		for(;itrSui != m_Sui.end(); ++itrSui)
		{
			map<int64,LordData>::iterator itr = itrSui->second.find(charid);
			if(itr != itrSui->second.end())
			{
				nTemp = itr->second.nCounKiller;
				itr->second.nCounKiller = 0;
				return nTemp;
			}
		}
	}

	return nTemp;
}

void CCountryOverlordCarbon::onUpdateSort(void* arg)
{
	CountryOverlord::OverlordWarData info;
	map<int,map<int64,LordData> >::iterator itrTang =  m_Tang.begin();
	for(;itrTang != m_Tang.end(); ++itrTang)
	{
		map<int64,LordData>::iterator itr =  itrTang->second.begin();
		for(; itr != itrTang->second.end(); ++itr)
		{
			CountryOverlord::WarData* pData = info.add_data();
			pData->set_charname(itr->second.sClanName);
			pData->set_killer(itr->second.nKiller);
			pData->set_integral(itr->second.nIntegral);
			pData->set_country(eCountryID_tang);
		}
	}

	map<int,map<int64,LordData> >::iterator itrSui = m_Sui.begin();
	for(;itrSui != m_Sui.end(); ++itrSui)
	{
		map<int64,LordData>::iterator itr =  itrSui->second.begin();
		for(; itr != itrSui->second.end(); ++itr)
		{
			CountryOverlord::WarData* pData = info.add_data();
			pData->set_charname(itr->second.sClanName);
			pData->set_killer(itr->second.nKiller);
			pData->set_integral(itr->second.nIntegral);
			pData->set_country(eCountryID_sui);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	INT64PLAYERMAP::iterator pitr =  m_player.begin();
	for(; pitr != m_player.end(); ++pitr)
	{
		QuickMessage::GetInstance()->AddSendMessage(pitr->second->GetID(),pitr->second->GetChannelID(), &info, MSG_SIM_GM2C_COUNTRY_OVERLOAD_WARDATA);
	}
}

void CCountryOverlordCarbon::FlushMonster(void* arg)
{
	if(m_process > ePlayerWork)
		return;

  const OverlordBaseData* pData = CountryOverlordCarbonLoader::GetInstance()->GetOverlordBaseData();
  if(!pData)
	  return;

	for(DWORD i = 0; i < pData->m_nCount; i++)
	{
		MonsterInfo info;
		vector<Smart_Ptr<Monster> > vecMonster;

		MonsterLoader::GetInstance()->GetMonsterInfo(pData->m_nMosterId, info);
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
		mon->SetXPos(pData->m_nXpos);
		mon->SetYPos(pData->m_nYpos);
		mon->SetOriginalX(pData->m_nXpos);
		mon->SetOriginalY(pData->m_nYpos);
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

		INTVMPNSTERMAP::iterator itr = m_allMonster.find(info.id);
		if(itr != m_allMonster.end())
		{
			itr->second.push_back(mon);
		}
		else
		{
			m_allMonster[info.id] = vecMonster;
		}

		ChatMgr::GetInstance()->Marquee(eMarqueeCountryOverlordFoundMonster,mon->GetName(),"","");
	}
}

void CCountryOverlordCarbon::CountryOverlordOver()
{
	int nTotalTang = 0;
	int nTotalSui = 0;
	map<int,map<int64,LordData> >::iterator itrTang =  m_Tang.begin();
	for(;itrTang != m_Tang.end(); ++itrTang)
	{
		map<int64,LordData>::iterator itr =  itrTang->second.begin();
		for(; itr != itrTang->second.end(); ++itr)
		{
			nTotalTang += itr->second.nIntegral;
		}
	}

	map<int,map<int64,LordData> >::iterator itrSui = m_Sui.begin();
	for(;itrSui != m_Sui.end(); ++itrSui)
	{
		map<int64,LordData>::iterator itr =  itrSui->second.begin();
		for(; itr != itrSui->second.end(); ++itr)
		{
			nTotalSui += itr->second.nIntegral;
		}
	}


}
