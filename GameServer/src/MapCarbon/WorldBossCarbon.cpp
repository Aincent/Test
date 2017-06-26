/*
 * WorldBossCarbon.cpp
 *
 *  Created on: 2015年11月18日
 *      Author: root
 */
#include "WorldBossCarbon.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../Map/DuplicateMgr.h"
#include "../ServerEventDefine.h"
#include "../ServerManager.h"
#include "../Object/Player/PlayerMessage.h"
#include "../WorldBoss/WorldBoss.h"
#include "./FileLoader/InternationalLoader.h"
#include "MessageStruct/CharLogin/Mail.pb.h"
#include "../Mail/MailMessage.h"
#include "MessageStruct/StatisticInfo.pb.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "../Attribute/AttributeCreature.h"
#include <time.h>
#include "../Object/FileLoader/MonsterLoader.h"
#include "Log/Logger.h"

WorldBossCarbon::WorldBossCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):CGameDuplicate(onlyid,charid,paramf,params),m_bossDamagerTime(10000)
{
	m_worldServerID = 0;
	m_endTime = 0;
}

WorldBossCarbon::~WorldBossCarbon()
{
	if(!m_playerAttackEvent.IsValid())
	{
		m_playerAttackEvent.SetNull();
	}

	if(!m_betAttackEvent.IsValid())
	{
		m_betAttackEvent.SetNull();
	}

	if(m_BossDamagerTimer.IsValid())
	{
		m_BossDamagerTimer.SetNull();
	}

	map<int,Smart_Ptr<WorldBossData> >::iterator tIt = m_BossData.begin();
	for(; tIt != m_BossData.end(); ++tIt)
	{
		if(tIt->second.Get() == NULL)
		{
			continue;
		}

		if(tIt->second->m_Timer.IsValid())
		{
			tIt->second->m_Timer.SetNull();
		}
	}

}

int WorldBossCarbon::Init()
{
	int ret = CGameDuplicate::Init();

	InstanceMapEntry* pInstance = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(GetSceneID()));
	if(pInstance!=NULL)
	{
		m_endTime = CUtil::GetNowSecond() + (DWORD64)pInstance->m_nLimitTime;
	}
	else
	{
		m_endTime = CUtil::GetNowSecond() + 3600000;
	}

	reflushBoss();
	m_BossDamagerTimer = TimerInterface::GetInstance()->RegRepeatedTime(&WorldBossCarbon::SynBossDamage,this,NULL,0,0,0,2000,2000);

	return ret;
}

//更新状态,false表示要删除这个场景
bool WorldBossCarbon::Update()
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
			int64 nowTime = CUtil::GetNowSecond();
			if(nowTime - m_awardTime >= 10*1000)
			{
				ServerReturn::ServerRetInt tMsg;	
				SendMapAllSynchMessage(&tMsg,MSG_SIM_GS2C_WORLDBOSS_RESULT,0,0);
				m_process = eChangeMapWait;
			}
			break;
		}

		case eChangeMapWait:
		{
			ForceAllPlayerChangeMap();
			m_process = eOver;
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

//加入场景
int WorldBossCarbon::Join(Smart_Ptr<Object> & obj)
{
	if(!JoinHandle(obj))
		return -1;

	int ret=CGameZone::Join(obj);
	if(ret!=0)
		return ret;

	if(obj->GetType()==ePlayer)
	{
		Smart_Ptr<Player> player = obj;
		int64 id=player->GetID();

		//第一个玩家进来时取世界服服id
		if(m_worldServerID==0)
		{
			m_worldServerID=GET_SERVER_CHARID(id);
		}

		//通知结束时间 
		WorldBossDuplicate::TimeLimit toClient;
		toClient.set_time(m_endTime);
		player->SendSynchMessage(&toClient,MSG_SIM_GS2C_WORLDBOSS_TIME);

		//通知进入
		SendEnterOrLeave(player, true);

		sendBossUpdate(player);
	}
	else if(obj->GetType()==eMonster)
	{
		Smart_Ptr<Monster> tMonster = obj;
		map<int,Smart_Ptr<WorldBossData> >::iterator tIt = m_BossData.find(tMonster->GetID());
		if(tIt != m_BossData.end())
		{
			tIt->second->m_Time = 0;			
		}
	}
	
	return ret;
}

//加入场景前处理
bool WorldBossCarbon::JoinHandle(Smart_Ptr<Object> & obj)
{
	//玩家
	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		if(!m_monsterDeadEvent.IsValid())
		{
			m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &WorldBossCarbon::HandleMonsterDeadEvent);
		}

		if(!m_playerDeadEvent.IsValid())
		{
			m_playerDeadEvent = RegGlobalEventPtr(CREATURE_DEAD_EVENT_TYPE, this, &WorldBossCarbon::HandlePlayerDeadEvent);
		}

		if(!m_playerAttackEvent.IsValid())
		{
			m_playerAttackEvent = RegGlobalEventPtr(PLAYER_ATTACK_CREATURE, this, &WorldBossCarbon::PlayerAttack);
		}

		if(!m_betAttackEvent.IsValid())
		{
			m_betAttackEvent = RegGlobalEventPtr(PET_ATTACK, this, &WorldBossCarbon::BetAttack);
		}

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		if(m_process == eCreated)
			m_process = ePlayerWork;
	}

	return true;
}

	//场景中移动
int WorldBossCarbon::Move(Smart_Ptr<Object> &obj)
{
	return CGameZone::Move(obj);
}

//离开场景，isDel是否广播移出
void WorldBossCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);
	CGameZone::Leave(obj, isDel, delFromMap);

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;
		int64 mapid = 0;
		int xpos = 0;
		int ypos = 0;

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
			if(sMapInfo == 0)
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

		if(player->getAttManage()->getValue<int>(eCharHP) <= 0)
		{
			CharBattle::RevivalData req;

			//增加地图id和坐标，重置死亡状态，解决客户端显示“使用复活单”的提示bug，是因为玩家回城复活，那么会发两次消息给客户端
			player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));
			player->UnsetCreatureState(ERS_Dead);
			player->SetDataFlag(eBaseInfo);

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
		else
		{
			player->SetMapID(mapid);
			player->SetXPos(xpos);
			player->SetYPos(ypos);
			player->SetXNewPos();
			player->SetYNewPos();
			player->SetDataFlag(eBaseInfo);
		}
	}

}

//离开场景前处理
void WorldBossCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
{

}

//玩家强制退出副本检验(用于判断玩家退出后副本的后续操作 )
void WorldBossCarbon::LeaveExame(Smart_Ptr<Player> & player)
{

}

//此场景是否可以进入
bool WorldBossCarbon::IsCanEnter(Smart_Ptr<Player> & player)
{
	//cd时间检测
	std::map<int64,PlayerWorldBossData>::iterator it = m_playersData.find(player->GetID());
	if(it != m_playersData.end())
	{
		int64 curTime = CUtil::GetNowSecond();
		if(curTime < it->second.coolTime)
		{
			return false;
		}
	}

	//人数检测
	int mapID = GET_MAP_ID(GetSceneID());
	if( GetPlayerCount() >= WorldBossMgr::GetInstance()->GetMaxPlayer(mapID) )
	{
		return false;
	}

	return true;
}

//赢了后的操作
void WorldBossCarbon::Win()
{
	m_isSuccess = true;
	m_awardTime = CUtil::GetNowSecond();
	m_process = eAward;
}

//副本结束时间到了之后的 操作
void WorldBossCarbon::Over()
{
	m_process = eAward;
}

//副本人数为零时的操作
void WorldBossCarbon::Count()
{

}

void WorldBossCarbon::CountEx()
{
	int64 nowTime = CUtil::GetNowSecond();
	if(nowTime - m_leaveTime >= m_data->m_nDelTime)
	{
		m_process = eOver;
	}
}

//输了之后的操作
void WorldBossCarbon::Failed()
{
	m_process = eAward;
}

//奖励
void WorldBossCarbon::Award()
{
//	map<int,DamageBossData>::iterator tIt = m_BossDamageList.begin();//伤害列表
//	for(;tIt != m_BossDamageList.end();++tIt)
//	{
//		Result(tIt->first);
//	}

	m_process = eDuplicateChangeMap;
}

//怪物死亡事件
bool WorldBossCarbon::HandleMonsterDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
			return true;

	TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);
	if(tArgs==NULL || tArgs->m_myself.Get()==NULL ||
	   GetSceneID() != tArgs->m_myself->GetMapID()||
	   tArgs->m_other.Get()==NULL)
	{
		return true;
	}

	Smart_Ptr<Monster> mon = tArgs->m_myself;
	Smart_Ptr<Player>  player;
	Smart_Ptr<GirlWar>  girlwar;

	if(tArgs->m_other->GetType() == ePlayer)
	{
		player = tArgs->m_other;
	}
	else if(tArgs->m_other->GetType() == ePet)
	{
		girlwar = tArgs->m_other;
		player  = girlwar->GetOwner();
	}

	//记录最后一击玩家信息
	if(player.Get()!=NULL)
	{
		
		m_BossDamageList[mon->GetID()].m_CharID = player->GetID();
		m_BossDamageList[mon->GetID()].m_vipLv = player->getAttManage()->getValue<int>(eVIPLv);
		m_BossDamageList[mon->GetID()].m_charName = player->GetName();

		char szbuffer[64] = {0};
		sprintf(szbuffer,"%lld", player->GetID());

		string strM = szbuffer;
		strM += "|";

		int mapID = GET_MAP_ID(GetSceneID());
		sprintf(szbuffer,"%d", mapID);
		strM += szbuffer;

		string strE = ChatMgr::GetInstance()->GetStringByInt(mon->GetID());
		ChatMgr::GetInstance()->Marquee(eMarqueeWorldBossDead, player->GetName(), strM, strE);
	}

	//通知世界服死亡消息
	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(getWorldServer(),worldSvr);
	if(worldSvr.Get()!=NULL)
	{
		WorldBossDuplicate::GS2WSBossDead toWs;
		toWs.set_mapid(GET_MAP_ID(GetSceneID()));
		toWs.set_charid(player->GetID());
		toWs.set_viplevel(player->getAttManage()->getValue<int>(eVIPLv));
		toWs.set_charname(player->GetName());

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GS2WS_WORLDBOSS_BOSSDEAD, &toWs, worldSvr.Get(), SimpleMessage);
		Message_Facade::Send(messRet);
	}

	int tTime = WorldBossMgr::GetInstance()->getBossUpdateTime(tArgs->m_myself->GetID());
	map<int,Smart_Ptr<WorldBossData> >::iterator tIt = m_BossData.find(tArgs->m_myself->GetID());
	if(tIt != m_BossData.end())
	{
		tIt->second->m_Time = tTime + (DWORD64)time(NULL) * 1000;		
		tIt->second->m_Timer = TimerInterface::GetInstance()->RegTimePoint(&WorldBossCarbon::bossUpdateTime,this,(void*)tArgs->m_myself->GetID(),0,0,0,tTime); 
	}

	WorldBossDuplicate::WorldBossUpdate tMsg;
	WorldBossDuplicate::OneBossUpdateData *tUpdate = tMsg.add_datas();
	tUpdate->set_bossid(tIt->first);
	tUpdate->set_time(tIt->second->m_Time);

	SendMapAllSynchMessage(&tMsg,MSG_SIM_GM2C_WORLDBOSS_UPDATE,0,0);

	Result(tIt->first);	

	return true;
}

//玩家死亡事件
bool WorldBossCarbon::HandlePlayerDeadEvent(const EventArgs & e)
{
	return true;
}

//攻击
bool WorldBossCarbon::PlayerAttack(const EventArgs & e)
{
	PlayerAttackCreature *pArgs = (PlayerAttackCreature *) (&e);
	if(pArgs==NULL || pArgs->m_player.Get()==NULL || pArgs->m_player->GetMapID() != m_sceneID)
	{
		return true;
	}

	addBoosDamage(pArgs->m_player,pArgs->m_value,pArgs->m_Creature->GetID());
	bossBloodCheck(pArgs->m_Creature);

	return true;
}

	//被战姬攻击
bool WorldBossCarbon::BetAttack(const EventArgs & e)
{
	const BetAttackArgs& tempArgs = static_cast<const BetAttackArgs&>(e);
	if(!tempArgs.m_Self)
	{
		return true;
	}

	Smart_Ptr<Player> tempplayer = tempArgs.m_Self->GetOwner();
	if(!tempplayer)
	{
		return true;
	}

	if(tempplayer->GetMapID() != m_sceneID)
	{
		return true;
	}

	addBoosDamage(tempplayer,tempArgs.m_Value,tempArgs.m_Other->GetID());

	bossBloodCheck(tempArgs.m_Other);

	return true;
}

//强制玩家离开
void WorldBossCarbon::ForceExit(Smart_Ptr<Player>& player)
{
	map<int64,OldMapInfo>::iterator itMap = m_oldMapPoint.find(player->GetID());
	if(itMap != m_oldMapPoint.end())
	{
		PlayerMessage::GetInstance()->ForceChangeMap(player, itMap->second.mapid, itMap->second.xpos, itMap->second.ypos);
	}
	else
	{
		MapStaticInfo *sMapInfo;
		int64 sceneid = 0;

		sMapInfo = CMapConfig::GetInstance()->GetFistMapInfo();
		if(sMapInfo == 0)
			return ;

		sceneid = CSceneMgr::GetInstance()->GetSceneIDAboutPlayerIsLoginOn(sMapInfo->mapid);
		if(sceneid <= 0)
		{
			sceneid = CREATE_MAP(eStaticMap, 1, 1, sMapInfo->mapid, 1);
		}

		PlayerMessage::GetInstance()->ForceChangeMap(player, sceneid, sMapInfo->reliveXPos, sMapInfo->reliveYPos);
	}

}

//通知世界服进入或离开
void WorldBossCarbon::SendEnterOrLeave(Smart_Ptr<Player>& player, bool isEnter)
{
	WorldBossDuplicate::GS2WSEnterOrLeave msg;

	if(!isEnter)
	{
		std::map<int64,PlayerWorldBossData>::iterator it = m_playersData.find(player->GetID());
		if(it != m_playersData.end())
		{
			it->second.leaveCount++;

			//计算cd时间
			int64 time = CUtil::GetNowSecond() + GetCoolTime(it->second.leaveCount);
			it->second.coolTime = time;

			msg.set_time(time);
		}
	}
	else
	{
		std::map<int64,PlayerWorldBossData>::iterator it = m_playersData.find(player->GetID());
		if(it != m_playersData.end())
		{
			it->second.coolTime = 0;
		}
		else
		{
			PlayerWorldBossData data;
			m_playersData[player->GetID()] = data;
		}
	}

	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(getWorldServer(),worldSvr);
	if(worldSvr.Get()!=NULL)
	{
		msg.set_charid(player->GetID());
		msg.set_mapid(GET_MAP_ID(GetSceneID()));
		msg.set_flag(isEnter);

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GS2WS_WORLDBOSS_JIONORLEAVE, &msg, worldSvr.Get(), SimpleMessage);
		Message_Facade::Send(messRet);
	}
}

int64 WorldBossCarbon::GetCoolTime(int count)
{
	int time = WorldBossMgr::GetInstance()->GetCoolTime(count);
	return 1000*time;
}


void WorldBossCarbon::addBoosDamage(Smart_Ptr<Player>& player, int value,int bossID)
{	
	vector<BossDamageData>::iterator it=std::find_if(m_BossDamageList[bossID].m_BossDatas.begin(), m_BossDamageList[bossID].m_BossDatas.end(),ValueCmp(player->GetID()));
	if(it!=m_BossDamageList[bossID].m_BossDatas.end())
	{
		(*it).damage += value;
	}
	else
	{
		BossDamageData data;
		data.charid = player->GetID();
		data.name = player->GetName();
		data.viplevel = player->getAttManage()->getValue<int>(eVIPLv);
		data.damage = value;
		m_BossDamageList[bossID].m_BossDatas.push_back(data);
	}

	return ;
}

bool SortByBossDamage(const BossDamageData& a, const BossDamageData& b)
{
	return a.damage > b.damage;
}

//boss刷新定时器
void WorldBossCarbon::bossUpdateTime(void *arg)
{
	map<int,Smart_Ptr<WorldBossData> >::iterator tIt = m_BossData.find((long)arg);
	if(tIt == m_BossData.end() || tIt->second.Get() == NULL)
	{
		return ;
	}
	
	if(tIt->second->m_Timer.IsValid())
	{
		tIt->second->m_Timer.SetNull();
	}

	tIt->second->m_Time = 0;
	updateOneBoss(tIt->first);

	char tIDChar[64] = {0};

	sprintf(tIDChar,"%d", tIt->first);
	ChatMgr::GetInstance()->Marquee(eMarqueeWorldBossUpdate,"",tIDChar,"");
	
	return ;	
}

int WorldBossCarbon::GetDamageSort(int64 charid,int bossID)
{
	for(size_t i=0; i<m_BossDamageList[bossID].m_BossDatas.size(); ++i)
	{
		if(charid == m_BossDamageList[bossID].m_BossDatas[i].charid)
		{
			return i+1;
		}
	}
	return 0;
}

void WorldBossCarbon::Result(int bossID)
{
	//统计最后伤害及奖励信息
	sort(m_BossDamageList[bossID].m_BossDatas.begin(), m_BossDamageList[bossID].m_BossDatas.end(), SortByBossDamage);

	for(size_t i = 0;i < m_BossDamageList[bossID].m_BossDatas.size();++i)
	{
		bool isLastAttck = false;
		if(m_BossDamageList[bossID].m_BossDatas[i].charid == m_BossDamageList[bossID].m_CharID)
		{
			isLastAttck =  true;
		}

		//玩家是否在副本场景内
		std::vector<PropItem> items;
		CalPrize(bossID,i + 1, isLastAttck, items);
		SendMail(m_BossDamageList[bossID].m_BossDatas[i].charid,items);
	}

	return ;
}

void WorldBossCarbon::CalPrize(int mapID, int sort, bool isLastAttack, std::vector<PropItem>& items)
{
	 WorldBossMgr::GetInstance()->GetSortPrize(mapID, sort, items);

	 if(isLastAttack)
	  {
		 WorldBossMgr::GetInstance()->GetLastAttackPrize(mapID, items);
	  }
}

void WorldBossCarbon::SendMail(int64 charid, const std::vector<PropItem>& items)
{
	std::string strContent=InternationalLoader::GetInstance()->GetInterStr(46);

	MailMessageMgr::GetInstance()->SendMail(charid, items, strContent, strContent);
}

//领取奖励
int WorldBossCarbon::Fitch(Smart_Ptr<Player>& player)
{
	return 0;
}

//刷新所有BOSS
void WorldBossCarbon::reflushBoss()
{
	m_BossData.clear();
	const map<int,WorldBossConfig>& tConfig = WorldBossMgr::GetInstance()->getConfig();
	map<int,WorldBossConfig>::const_iterator tIt = tConfig.begin();
	for(;tIt != tConfig.end();++tIt)
	{
//		if(tIt->second.m_Type != eBossWorldType)
//		{
//			continue;
//		}

		if(tIt->second.m_UpdateTime.m_BossID == 0)
		{
			continue;
		}

		m_BossData[tIt->second.m_UpdateTime.m_BossID] = new WorldBossData();	
		m_BossDamageList[tIt->second.m_UpdateTime.m_BossID].reset();
		CreateMonster(tIt->second.m_UpdateTime.m_BossID,tIt->second.m_UpdateTime.m_X,tIt->second.m_UpdateTime.m_Y);
	}


	return ;
}

void WorldBossCarbon::sendBossUpdate(Smart_Ptr<Player>& player)
{
	WorldBossDuplicate::WorldBossUpdate tMsg;
	map<int,Smart_Ptr<WorldBossData> >::iterator tIt = m_BossData.begin();
	for(;tIt != m_BossData.end();++tIt)
	{
		WorldBossDuplicate::OneBossUpdateData *tUpdate = tMsg.add_datas();
		tUpdate->set_bossid(tIt->first);
		tUpdate->set_time(tIt->second->m_Time);
	}

	player->SendSynchCommon(&tMsg,MSG_SIM_GM2C_WORLDBOSS_UPDATE);


	return ;
}

void WorldBossCarbon::updateOneBoss(int bossID)
{
	const WorldBossConfig* tData = WorldBossMgr::GetInstance()->getOneConfig(bossID);
	if(tData ==NULL)
	{
		return ;
	}

	CreateMonster(tData->m_UpdateTime.m_BossID,tData->m_UpdateTime.m_X,tData->m_UpdateTime.m_Y);

	WorldBossDuplicate::WorldBossUpdate tMsg;
	WorldBossDuplicate::OneBossUpdateData *tUpdate = tMsg.add_datas();
	tUpdate->set_bossid(bossID);
	tUpdate->set_time(0);

	SendMapAllSynchMessage(&tMsg,MSG_SIM_GM2C_WORLDBOSS_UPDATE,0,0);
	
	return ;
}

//boss血量检测
void WorldBossCarbon::bossBloodCheck(Smart_Ptr<CreatureObj> creature)
{
	const vector<int>& tPrompt = WorldBossMgr::GetInstance()->getBossBloodPrompt();
	int tCurHp = creature->getAttManage()->getValue<int>(eCharHP);
	if(tCurHp <= 0)
	{
		return ;
	}

	int tMaxHp = creature->getAttManage()->getValue<int>(eCharHPMax);
	int tPercent = (int)((float)tCurHp / (float)tMaxHp * 10000);

	for(size_t i = 0;i < tPrompt.size();++i)
	{
		if(tPercent > tPrompt[i])		
		{
			continue;
		}

		map<int,Smart_Ptr<WorldBossData> >::iterator tIt = m_BossData.find(creature->GetID());
		if(tIt == m_BossData.end())
		{
			break;
		}

		map<int,bool>::iterator tInIt = tIt->second->m_Prompts.find(tPrompt[i]);
		if(tInIt != tIt->second->m_Prompts.end())
		{
			break;
		}

		tIt->second->m_Prompts[tPrompt[i]] = true;

		char tIDChar[64] = {0};
		char tBloodChar[32] = {0};

		sprintf(tIDChar,"%lld", creature->GetID());
		sprintf(tBloodChar,"%d", tPrompt[i] / 100);
		ChatMgr::GetInstance()->Marquee(eMarqueeWorldBossBloop,"",tIDChar,tBloodChar);

		break;
	}

	return ;
}

void WorldBossCarbon::SynBossDamage(void *arg)
{
	WorldBossDuplicate::BossDamage tMsg;
	INTVMPNSTERMAP::iterator itr =  m_allMonster.begin();
	for(; itr !=  m_allMonster.end(); ++itr)
	{
		vector<Smart_Ptr<Monster> >::iterator it = itr->second.begin();
		for(; it != itr->second.end(); ++it)
		{

			map<int,Smart_Ptr<WorldBossData> >::iterator tIt = m_BossData.find((*it)->GetID());
			if(tIt == m_BossData.end())
			{
				continue;
			}

			tMsg.add_bood((*it)->getAttManage()->getValue<int>(eCharHP));
			tMsg.add_bossid((*it)->GetID());
		}
	}

	SendMapAllSynchMessage(&tMsg,MSG_SIM_GS2C_WORLDBOSS_DAMAGE,0,0);
}

void WorldBossCarbon::GetBoosDamage(int64 charId,int bossID,WorldBossDuplicate::AckReqBossDamage& info)
{
	//统计最后伤害及奖励信息
	sort(m_BossDamageList[bossID].m_BossDatas.begin(), m_BossDamageList[bossID].m_BossDatas.end(), SortByBossDamage);
	map<int,DamageBossData>::iterator itr =  m_BossDamageList.find(bossID);
	if(itr != m_BossDamageList.end())
	{
		size_t i = 0;
		vector<BossDamageData>::iterator it =  itr->second.m_BossDatas.begin();
		for(; it != itr->second.m_BossDatas.end()  ; ++it,i++)
		{
			if(i < 10)
			{
				WorldBossDuplicate::DamageData* pDamage = info.add_infos();
				pDamage->set_charname(it->name);
				pDamage->set_charid(it->charid);
				pDamage->set_viplevel(it->viplevel);
				pDamage->set_damage(it->damage);
			}

			if(it->charid == charId)
			{
				info.set_selfdamage(it->damage);
				info.set_selfsort(i+1);
			}
		}
	}
}
