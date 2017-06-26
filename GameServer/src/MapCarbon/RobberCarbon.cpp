/*
 * RobberCarbon.cpp
 *
 *  Created on: 2016年2月23日
 *      Author: root
 */
#include "RobberCarbon.h"
#include "../Clan/ClanLoader/RobberLoader.h"
#include "../Object/ObjectFactory.h"
#include "../Object/FileLoader/AILoader.h"
#include "../Object/AI/AIBrain.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../Map/DuplicateMgr.h"
#include "../ServerEventDefine.h"
#include "../ServerManager.h"
#include "../Map/GameZone.h"
#include "../Object/Player/PlayerMessage.h"
#include "MessageStruct/Clan/Clan.pb.h"
#include <math.h>


RobberCarbon::RobberCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):
CGameDuplicate(onlyid,charid,paramf,params)
{
	m_curNode = 0;
	m_IsPrize = false;
	m_IsCalDead = true;
	m_win = false;
	m_param = 0;
	m_conwin = 0;
	m_confail = 0;
	m_clanMoney = 0;
}

RobberCarbon::~RobberCarbon()
{
	if(m_FlushTimer.IsValid())
		m_FlushTimer.SetNull();
}

void RobberCarbon::Destroy(int clanid)
{
	if(clanid != m_paramf)
	{
		return;
	}

	//清除所有事件数据  活动等
	if(m_FlushTimer.IsValid())
	{
		m_FlushTimer.SetNull();
	}

	//踢人
	ForceAllPlayerChangeMap();

	m_process = eOver;

}

void RobberCarbon::Active(bool flag, int clanid, bool win, int param, int conwin, int confail)
{
	if(clanid != m_paramf)
	{
		return;
	}

	if(flag)
	{
		Start(win, param, conwin, confail);
	}
	else
	{
		End();
	}

}

void RobberCarbon::Start(bool win, int param, int conwin, int confail)
{
	m_curNode = 1;

	m_win = win;
	m_param = param > 0 ? param : 0;
	m_conwin = conwin;
	m_confail = confail;

	FreshMonster(m_curNode);

}

void RobberCarbon::End()
{
	if(m_FreshObjects.size() > 0)
	{
		if(!m_IsPrize)
		{
			Result(false);
		}

		KillMonsters();
	}
}

void RobberCarbon::FreshMonster(int node)
{
	//清除上一次的怪物
	KillMonsters();

	//关闭定时器
	if(m_FlushTimer.IsValid())
	{
		m_FlushTimer.SetNull();
	}

	//如果没有刷完，开启定时器
	if(node > RobberLoader::GetInstance()->GetNodeCount())
	{
		if(!m_IsPrize)
		{
			Result(false);
		}
		return;
	}

	m_FlushTimer = TimerInterface::GetInstance()->RegTimePoint(&RobberCarbon::onFlushMonster, this, 0, 0, 0, 0, RobberLoader::GetInstance()->GetFlushTime());

	//创建怪物
	vector< pair<int,int> > infos;
	RobberLoader::GetInstance()->GetNodeInfo(m_curNode, infos);

	int index = 0;
	int posx, posy;

	for(size_t i=0; i<infos.size(); ++i)
	{
		for(int j=0; j<infos[i].second; ++j)
		{
			if(RobberLoader::GetInstance()->GetMonsterPos(index, posx, posy))
			{
				CreateMonster(infos[i].first, posx, posy);
				++index;
			}
		}
	}

}

void RobberCarbon::onFlushMonster(void* args)
{
	FreshMonster(++m_curNode);
}

void RobberCarbon::KillMonsters()
{
	m_IsCalDead = false;
	std::map<int,Smart_Ptr<Monster> >::iterator it = m_FreshObjects.begin();
	for(; it!=m_FreshObjects.end(); ++it)
	{
		if(it->second.Get()==NULL || it->second->IsDead())
		{
			continue;
		}

		it->second->SetDead(NULL);
	}

	m_FreshObjects.clear();
	m_IsCalDead = true;
}

void RobberCarbon::CreateMonster(int id, int posx, int posy)
{
	MonsterInfo info;
	vector<Smart_Ptr<Monster> > vecMonster;

	MonsterLoader::GetInstance()->GetMonsterInfo(id, info);
	if(info.id <= 0)
	{
		return;
	}

	Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eMonster);
	Smart_Ptr<Monster> mon = obj;

	mon->SetID(info.id);
	mon->SetName(info.name);
	mon->SetQuality(info.quality);
	mon->SetFlushTime(info.flushTime);
	mon->SetAliveTime(info.aliveTime);
	mon->getAttManage()->setValue(eCharCountry,info.camp);
	mon->getAttManage()->setValue(eCharMoveSpeed,info.idleSpeed);
	mon->SetRunSpeed(info.moveSpeed);

	mon->SetFallID(info.fallID);
	mon->getAttManage()->setValue(eCharExp,info.exp);

	mon->SetGatherTime(info.gatherTime);	//采集时间
	mon->SetOwnerToBrain(mon);
	mon->SetMapID(m_sceneID);
	mon->SetBornTime();
	mon->SetOwnerName("");
	mon->GetBrain()->SetMonsterType(ePointWayPos);
	mon->SetXPos(posx);
	mon->SetYPos(posy);
	mon->SetOriginalX(posx);
	mon->SetOriginalY(posy);

	//属性
	AlterMonsterAttr(mon, info);

	Join(obj);

	vector<Sector *> moveVec;
	BaseAIInfo *aiInfo = 0;
	BetterAIInfo *betterAI = 0;

	aiInfo = AILoader::GetInstance()->GetBaseAIByid(info.baseAI);
	if(!aiInfo)
	{
		return;
	}

	if(info.betterAI > 0)
	{
		betterAI = AILoader::GetInstance()->GetBetterAIByid(info.betterAI);
		if (!betterAI)
		{
			LOG_ERROR(FILEINFO, "monster[id=%d] better ai is error", info.id);
			return;
		}
	}

	mon->SetStayTime(aiInfo->stayTime);
	mon->SetPhyAttackTime(aiInfo->atkTime);

	mon->SetMoveSector(moveVec);

	Smart_Ptr<AIBrain> monBrain = mon->GetBrain();
	if(!monBrain)
		return;

	monBrain->Init(aiInfo, betterAI);
	vecMonster.push_back(mon);
	if(info.quality == eWorldMonster)
	{
		CSceneMgr::GetInstance()->AddWorldBossToZone(mon->GetKey(), mon->GetMapID(), mon->GetID());
	}

	m_allObject[mon->GetKey()] = mon;

	INTVMPNSTERMAP::iterator itTemp = m_allMonster.find(info.id);
	if(itTemp != m_allMonster.end())
	{
		itTemp->second.insert( itTemp->second.end(), vecMonster.begin(), vecMonster.end() );
	}
	else
	{
		m_allMonster[info.id] = vecMonster;
	}

	AddMonster(mon->GetKey(), mon);

}

void RobberCarbon::AlterMonsterAttr(Smart_Ptr<Monster>& mon, const MonsterInfo& info)
{
	const RobberMonsterInfo* pRobber = RobberLoader::GetInstance()->GetRobberInfo(mon->GetID());
	if(pRobber==NULL)
	{
		return;
	}

	mon->SetHPMax((int)( info.maxHP * pow(pRobber->paramHP,m_param) ) );
	mon->SetCurHP((int)( info.maxHP * pow(pRobber->paramHP,m_param) ) );

	mon->SetMPMax((int)( info.maxMP * pow(pRobber->paramMP,m_param) ) );
	mon->SetCurMP((int)( info.maxMP * pow(pRobber->paramMP,m_param) ) );

	mon->getAttManage()->setValue(eCharPhyAttackMax,(int)( info.maxAttack * pow(pRobber->paramAttack,m_param) ) );
	mon->getAttManage()->setValue(eCharPhyDefence,(int)( info.phyDefence * pow(pRobber->paramDefene,m_param) ) );
	mon->getAttManage()->setValue(eCharHitRate,(int)( info.hitRate * pow(pRobber->paramHit,m_param) ) );
	mon->getAttManage()->setValue(eCharAvoidRate,(int)( info.avoidRate * pow(pRobber->paramAvoid,m_param) ) );
	mon->getAttManage()->setValue(eCrackDownRate,(int)( info.crackRate * pow(pRobber->paramCrack,m_param) ) );
	mon->getAttManage()->setValue(eAvoidCrackRate,(int)( info.avoidCrack * pow(pRobber->paramAvoidCrack,m_param) ) );

}

bool RobberCarbon::HandleMonsterDeadEvent(const EventArgs & e)
{
	//系统杀死的不算
	if(!m_IsCalDead)
	{
		return true;
	}

	TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);

	if(tArgs==NULL || tArgs->m_myself.Get()==NULL ||
			GetSceneID() != tArgs->m_myself->GetMapID()||tArgs->m_other.Get()==NULL)
	{
		return true;
	}

	//得到周围的人加经验 铜钱 帮工
	Smart_Ptr<Monster> mon = tArgs->m_myself;
	const RobberMonsterInfo *pInfo = RobberLoader::GetInstance()->GetRobberInfo(mon->GetID());
	if(pInfo==NULL)
		return true;

	vector<Smart_Ptr<CreatureObj> > obj;
	GetGroupObject(tArgs->m_myself, obj);
	for(size_t i=0; i<obj.size(); ++i)
	{
		if(obj[i].Get()==NULL || obj[i]->GetType() != ePlayer)
			continue;

		Smart_Ptr<Player> player = obj[i];
		player->AddExp(pInfo->exp);
		player->MoneyChange(pInfo->money,true);
		player->ChangeFContri(pInfo->contribute, true);
		player->SynchCharAttributeToWS(eFaContribution,pInfo->contribute);
	}

	//帮派加资金
	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(m_params,worldSvr);
	if(worldSvr.Get()!=NULL)
	{
		ServerReturn::ServerDoubleUint32 msg;
		msg.set_retf(m_paramf);
		msg.set_rets(pInfo->clanMoney);
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GS2WS_CLAN_CONTRIBUTE_ALTER, &msg, worldSvr.Get(), SimpleMessage);
		Message_Facade::Send(messRet);
	}

	//移除怪物
	RemoveMonster(mon->GetKey());
	if(m_FreshObjects.size() == 0)
	{
		//结算
		if(m_curNode == RobberLoader::GetInstance()->GetNodeCount())
		{
			if(!m_IsPrize)
			{
				Result(true);
			}
		}

		FreshMonster(++m_curNode);
	}

	return true;
}

void RobberCarbon::AddMonster(int key, Smart_Ptr<Monster>& mon)
{
	m_FreshObjects[key] = mon;
}

void RobberCarbon::RemoveMonster(int key)
{
	std::map<int,Smart_Ptr<Monster> >::iterator it = m_FreshObjects.find(key);
	if(it != m_FreshObjects.end())
	{
		m_FreshObjects.erase(it);
	}
}

void RobberCarbon::Result(bool win)
{
	m_clanMoney = 0;
	m_IsPrize = true;

	if(win)
	{
		m_param += 1;
		m_conwin += 1;
		m_confail = 0;
		m_clanMoney += RobberLoader::GetInstance()->GetWinMoney();
	}
	else
	{
		m_conwin = 0;
		m_confail += 1;

		m_clanMoney -= RobberLoader::GetInstance()->GetFailMoney();

		if(m_confail >= RobberLoader::GetInstance()->GetFailCount())
		{
			m_confail = 0;
			m_param = m_param - 1 > 0 ? m_param - 1 : 0;
		}

	}

	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(m_params,worldSvr);
	if(worldSvr.Get()!=NULL)
	{
		ClanPackage::RobberResult msg;

		msg.set_win( win );
		msg.set_param( m_param );
		msg.set_clanid( m_paramf );
		msg.set_conwin( m_conwin );
		msg.set_confail(m_confail);
		msg.set_money(m_clanMoney);

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GS2WS_ROBBER_RESULT, &msg, worldSvr.Get(), SimpleMessage);
		Message_Facade::Send(messRet);
	}

}

int RobberCarbon::Init()
{
	int ret = CGameDuplicate::Init();

	return ret;
}

//更新状态,false表示要删除这个场景
bool RobberCarbon::Update()
{
	if(m_process == eOver)
	{
		return false;
	}
	return true;
}

//加入场景
int RobberCarbon::Join(Smart_Ptr<Object> & obj)
{
	int ret=CGameZone::Join(obj);

	if(obj->GetType()==ePlayer)
	{
		if(!m_monsterDeadEvent.IsValid())
		{
			m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &RobberCarbon::HandleMonsterDeadEvent);
		}
	}

	return ret;
}

//加入场景前处理
bool RobberCarbon::JoinHandle(Smart_Ptr<Object> & obj)
{
	return true;
}

//场景中移动
int RobberCarbon::Move(Smart_Ptr<Object> &obj)
{
	int ret = CGameZone::Move(obj);
	return ret;
}

//离开场景，isDel是否广播移出
void RobberCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);
	CGameZone::Leave(obj, isDel, delFromMap);

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		int64 mapid = player->GetMapID();
		int posx = player->GetXPos();
		int posy = player->GetYPos();

		OldMapInfo* info = GetOldMapPoint(player->GetID());
		if(info)
		{
			mapid = info->mapid;
			posx = info->xpos;
			posy = info->ypos;
		}
		else
		{
			InstanceMapEntry* instanceInfo = GetInstanceScript();
			if(instanceInfo==NULL)
				return;

			MapStaticInfo * mapInfo = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(player->GetMapID()));
			if(mapInfo==NULL)
				return;

			mapid = CREATE_MAP(eStaticMap, 1, 1, mapInfo->reliveMap, 1);
			posx = mapInfo->reliveXPos;
			posy = mapInfo->reliveYPos;
		}

		player->SetMapID(mapid);
		player->SetXPos(posx);
		player->SetYPos(posy);
		player->SetXNewPos();
		player->SetYNewPos();
		player->SetDataFlag(eBaseInfo);
	}

}

//离开场景前处理
void RobberCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
{
}

//玩家强制退出副本检验(用于判断玩家退出后副本的后续操作 )
void RobberCarbon::LeaveExame(Smart_Ptr<Player> & player)
{

}

//此场景是否可以进入
bool RobberCarbon::IsCanEnter(Smart_Ptr<Player> & player)
{
	return true;
}

//赢了后的操作
void RobberCarbon::Win()
{

}

//副本结束时间到了之后的 操作
void RobberCarbon::Over()
{

}

//副本人数为零时的操作
void RobberCarbon::Count()
{

}

void RobberCarbon::CountEx()
{

}

//输了之后的操作
void RobberCarbon::Failed()
{

}

//奖励
void RobberCarbon::Award()
{

}
