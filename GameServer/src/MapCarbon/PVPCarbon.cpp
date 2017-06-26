/*
 * PVPCarbon.cpp
 *
 *  Created on: 2015年7月24日
 *      Author: root
 */
#include "PVPCarbon.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../Map/DuplicateMgr.h"
#include "../PVP/PVPMgr.h"
#include "../ServerEventDefine.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../ServerManager.h"
#include "../Object/Player/PlayerMessage.h"

PVPCarbon::PVPCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):CGameDuplicate(onlyid,charid,paramf,params),
m_synTime(10000),m_bossDamagerTime(10000)
{
	m_layer = paramf;
	m_worldServerID = 0;
	m_BossObject = NULL;
	m_isForbid = false;
}

PVPCarbon::~PVPCarbon()
{
	release();
}

void PVPCarbon::release()
{
	m_playerAttackEvent.SetNull();
	m_playerMoveEvent.SetNull();
	m_playerBeAttackEvent.SetNull();
	m_betAttackEvent.SetNull();

	if(m_expPrizeTimer.IsValid())
			m_expPrizeTimer.SetNull();

 if(m_PlayDataTimer.IsValid())
	 m_PlayDataTimer.SetNull();

 if(m_BossDamageTimer.IsValid())
	 m_BossDamageTimer.SetNull();

	//玩家的采集数据
	std::map<int64,PlayerGatherData>::iterator it = m_playerGather.begin();
	for(; it!=m_playerGather.end(); ++it)
	{
		if(it->second.m_timer.IsValid())
			it->second.m_timer.SetNull();

		if(it->second.m_pParam!=NULL)
		{
			delete it->second.m_pParam;
			it->second.m_pParam = NULL;
		}
	}
}


int PVPCarbon::Init()
{
	int ret = CGameDuplicate::Init();
	if(m_expPrizeTimer.IsValid())
		m_expPrizeTimer.SetNull();

	m_expPrizeTimer = TimerInterface::GetInstance()->RegRepeatedTime(&PVPCarbon::onZonePrize, this, 0, 0, 0, 0,
			PVPMgr::GetInstance()->getExpPrizeTime(),	PVPMgr::GetInstance()->getExpPrizeTime());

	if(m_PlayDataTimer.IsValid())
		m_PlayDataTimer.SetNull();
	m_PlayDataTimer = TimerInterface::GetInstance()->RegRepeatedTime(&PVPCarbon::onSynPlayerData, this, 0, 0, 0, 0,
			m_synTime,	m_synTime);

	if(m_BossDamageTimer.IsValid())
		m_BossDamageTimer.SetNull();

	m_BossDamageTimer = TimerInterface::GetInstance()->RegRepeatedTime(&PVPCarbon::onSynBossDamage, this, 0, 0, 0, 0,
		m_bossDamagerTime,	m_bossDamagerTime);

	return ret;
}

bool PVPCarbon::IsAttacked(int64 fight, int64 attacked)
{
	if(isForbid())
		return false;
	return true;
}

//玩家被攻击事件
	bool PVPCarbon::PlayerBeAttack(const EventArgs & e)
	{
			PlayerBeAttackArgs *tArgs = (PlayerBeAttackArgs *)(&e);
			if(tArgs==NULL || tArgs->m_self.Get()==NULL || GetSceneID() != tArgs->m_self->GetMapID())
				return true;

			if(tArgs->m_self->GetType() != ePlayer)
				return true;

			canccelGather(tArgs->m_self->GetID());
			return true;
	}

	bool PVPCarbon::PlayerAttack(const EventArgs & e)
	{
			PlayerAttackCreature *pArgs = (PlayerAttackCreature *) (&e);
			if(pArgs==NULL || pArgs->m_player.Get()==NULL || pArgs->m_player->GetMapID() != m_sceneID)
				return true;

			canccelGather(pArgs->m_player->GetID());

			addBoosDamage(pArgs->m_player,pArgs->m_key,pArgs->m_value);

			//同步血条
			BrocastBossHP(pArgs->m_key);

			//取消无敌状态
			pArgs->m_player->UnsetCreatureState(ERS_Invincible, false);
			pArgs->m_player->RemoveBuffById(PVPMgr::GetInstance()->getProtectedBufferID());

			return true;
	}

	bool PVPCarbon::BetAttack(const EventArgs & e)
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

		addBoosDamage(tempplayer,tempArgs.m_Other->GetKey(),tempArgs.m_Value);

		//同步血条
		BrocastBossHP(tempArgs.m_Other->GetKey());

		return true;
	}

	bool PVPCarbon::PlayerMove(const EventArgs & e)
	{
		PlayerMoveEvent *pArgs = (PlayerMoveEvent *) (&e);
		if(pArgs==NULL)
			return true;

		canccelGather(pArgs->m_charID);
		return true;
	}

	bool SortByDamage(const BossDamageData& a, const BossDamageData& b)
	{
			return a.damage > b.damage;
	}

	//怪物死亡事件
	bool PVPCarbon::HandleMonsterDeadEvent(const EventArgs & e)
	{
			if(m_process != ePlayerWork)
				return true;

			TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);
			if(tArgs==NULL || tArgs->m_myself.Get()==NULL ||
					GetSceneID() != tArgs->m_myself->GetMapID()||tArgs->m_other.Get()==NULL)
				return true;

			Smart_Ptr<Monster> mon = tArgs->m_myself;
			Smart_Ptr<Player>  player;
			Smart_Ptr<GirlWar>  girlwar;

			m_BossObject = NULL;

			if(tArgs->m_other->GetType() == ePlayer)
			{
					player = tArgs->m_other;
			}
			else if(tArgs->m_other->GetType() == ePet)
			{
					girlwar = tArgs->m_other;
					player  = girlwar->GetOwner();
			}

			if(player.Get()==NULL || mon.Get() == NULL)
				return true;

			if(mon->GetQuality() == eBossMonster)
			{
				PvPPackage::BossDeadData msg;
				//todo：
				std::sort(m_BossDamageList.begin(), m_BossDamageList.end(), SortByDamage);
				//boss伤害奖励
				for(size_t i=0; i<m_BossDamageList.size(); ++i)
				{
					PVPMgr::GetInstance()->GetBossDamageReward(m_BossDamageList[i].charid,i+1);
					if(i < 3)
					{
						PvPPackage::PvPBossDeadData* pDeadData = msg.add_info();
						pDeadData->set_charid(m_BossDamageList[i].charid);
						pDeadData->set_viplevel(m_BossDamageList[i].viplevel);
						pDeadData->set_damage(m_BossDamageList[i].damage);
						pDeadData->set_name(m_BossDamageList[i].name);
						pDeadData->set_sex(m_BossDamageList[i].sex);
						pDeadData->set_pro(m_BossDamageList[i].pro);
						pDeadData->set_lv(m_BossDamageList[i].lv);
					}

					if(m_BossDamageList[i].charid == player->GetID())
					{
						PvPPackage::PvPBossDeadData* pLastattact = msg.mutable_lastattact();
						pLastattact->set_charid(player->GetID());
						pLastattact->set_viplevel(m_BossDamageList[i].viplevel);
						pLastattact->set_damage(m_BossDamageList[i].damage);
						pLastattact->set_name(m_BossDamageList[i].name);
						pLastattact->set_sex(m_BossDamageList[i].sex);
						pLastattact->set_pro(m_BossDamageList[i].pro);
						pLastattact->set_lv(m_BossDamageList[i].lv);
					}
				}

				//boss最后一击奖励
				PVPMgr::GetInstance()->GetBossLastAttack(player->GetID(),m_layer);

				std::vector<Smart_Ptr<Player> > players;
				GetAllPlayers(players);
				for(size_t i=0; i<players.size(); ++i)
				{
					if(players[i].Get()==NULL)
						continue;

					int nSort=0;
					int damage=0;
					for(size_t n=0; n<m_BossDamageList.size(); ++n)
					{
						if(players[i]->GetID()==m_BossDamageList[n].charid)
						{
							nSort = n+1;
							damage = m_BossDamageList[n].damage;
							break;
						}
					}

					msg.set_sort(nSort);
					msg.set_damage(damage);
					players[i]->SendSynchCommon(&msg,MSG_SIM_GS2C_PVP_BOSSDDEAD_RANK);
				}

				updatePlayerData(player->GetID(),ePVPLayer_bossKill,1);
			}
			else
			{
				updatePlayerData(player->GetID(),ePVPLayer_commonkill,1);
			}

			jumpLayer(player);

		return true;
	}

	//玩家死亡事件
	bool PVPCarbon::HandlePlayerDeadEvent(const EventArgs & e)
	{
		if(m_process != ePlayerWork)
				return true;

		PlayerKilledEventArgs *tArgs = (PlayerKilledEventArgs *)(&e);

		if(GetSceneID() != tArgs->m_dead->GetMapID())
			return true;

		if(tArgs->m_killer.Get()==NULL || tArgs->m_killer->GetType()!= ePlayer)
			return true;

		if(tArgs->m_dead.Get()==NULL || tArgs->m_dead->GetType()!= ePlayer)
			return true;

		KillPlayer(tArgs->m_killer,tArgs->m_dead);
		return true;
  }

void PVPCarbon::onZonePrize(void * arg)
{
		std::vector<Smart_Ptr<Player> > players;
		GetAllPlayers(players);
		for(size_t i=0; i<players.size(); ++i)
		{
				Smart_Ptr<Player>& ref = players[i];
				if(ref.Get()==NULL || ref->IsDead())
						continue;

				//加经验
				int lv=ref->getAttManage()->getValue<int>(eCharLv);
				int exp = PVPMgr::GetInstance()->getZoneExp(lv,getLayer());
				ref->AddExp(exp);

				//更新数据
				updatePlayerData(ref->GetID(),ePVPLayer_exp,exp);
		}
}

void PVPCarbon::onSynPlayerData(void * arg)
{
	PvPPackage::PlayerPVPDataList msg;
	std::map<int64,PlayerPvPDataEx>::iterator it = m_playerPvPData.begin();
	for(; it != m_playerPvPData.end(); ++it)
	{
		PvPPackage::PlayerPVPData* pItem = msg.add_list();
		pItem->set_charid(it->first);
		pItem->set_layer(it->second.layer);
		pItem->set_totalkill(it->second.totalkills);
		pItem->set_totalexp(it->second.exp);
		pItem->set_name(it->second.name);
		pItem->set_level(it->second.level);
		pItem->set_power(it->second.power);
		pItem->set_viplevel(it->second.viplevel);
		pItem->set_integral(it->second.integral);
	}

	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(getWorldServer(),worldSvr);
	if(worldSvr.Get()!=NULL)
	{
			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GS2WS_PVP_PLAYER_DATA, &msg, worldSvr.Get(), SimpleMessage);
			Message_Facade::Send(messRet);
	}

}

void PVPCarbon::synOnePlayerDataToWorld(int64 charid,const PlayerPvPDataEx& data)
{
	PvPPackage::PlayerPVPDataList msg;
	PvPPackage::PlayerPVPData* pItem = msg.add_list();
	pItem->set_charid(data.charid);
	pItem->set_layer(data.layer);
	pItem->set_totalkill(data.totalkills);
	pItem->set_totalexp(data.exp);
	pItem->set_name(data.name);
	pItem->set_level(data.level);
	pItem->set_power(data.power);
	pItem->set_viplevel(data.viplevel);
	pItem->set_integral(data.integral);

	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(getWorldServer(),worldSvr);
	if(worldSvr.Get()!=NULL)
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GS2WS_PVP_PLAYER_DATA, &msg, worldSvr.Get(), SimpleMessage);
		Message_Facade::Send(messRet);
	}
}

void PVPCarbon::synWorldLayerChange(int64 charid,int64 flag)
{
		Smart_Ptr<SvrItem> worldSvr;
		ServerConHandler::GetInstance()->GetWorldServerBySvrID(getWorldServer(),worldSvr);
		if(worldSvr.Get()!=NULL)
		{
			ServerReturn::ServerDoubleInt msg;
			msg.set_retf(charid);
			msg.set_rets(flag);
			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GS2WS_LEAVE_LAYER, &msg, worldSvr.Get(), SimpleMessage);
			Message_Facade::Send(messRet);
		}
}


void PVPCarbon::onSynBossDamage(void *arg)
{
	std::sort(m_BossDamageList.begin(), m_BossDamageList.end(), SortByDamage);
	PvPPackage::PvPBossDamagData msg;
	for(size_t i=0; i<m_BossDamageList.size() && i<PVP_KILL_SORT_NUM; ++i)
	{
		PvPPackage::BossDamageData* pItem = msg.add_info();
		pItem->set_name(m_BossDamageList[i].name);
		pItem->set_viplevel(m_BossDamageList[i].viplevel);
		pItem->set_damage(m_BossDamageList[i].damage);
		pItem->set_charid(m_BossDamageList[i].charid);
	}

	std::vector<Smart_Ptr<Player> > players;
	GetAllPlayers(players);
	for(size_t i=0; i<players.size(); ++i)
	{
		if(players[i].Get()==NULL)
			continue;

		int nSort=0;
		int damage=0;
		for(size_t n=0; n<m_BossDamageList.size(); ++n)
		{
			if(players[i]->GetID()==m_BossDamageList[n].charid)
			{
				nSort = n+1;
				damage = m_BossDamageList[n].damage;
				break;
			}
		}

		msg.set_sort(nSort);
		msg.set_damage(damage);
		players[i]->SendSynchCommon(&msg,MSG_SIM_GS2C_PVP_BOSSDAMAGE_RANK);
	}

}

void PVPCarbon::onGatherTimer(void* arg)
{
	GatherEx* data = (GatherEx*)(arg);
	if(data==NULL)
		return;

	GatherEx tmp=*data;
	delete data;
	data = NULL;

	//采集成功，更新玩家数据 通知玩家
	std::map<int64,PlayerGatherData>::iterator it = m_playerGather.find(tmp.m_charID);
	if(it!=m_playerGather.end())
	{
		updatePlayerData(tmp.m_charID,ePVPLayer_gather,1);

		Smart_Ptr<Player> player = GetPlayer(tmp.m_charID);
		if(player.Get()!=NULL)
		{
			//通知客户端成功
			PvPPackage::GaterResult msg;
			msg.set_result(0);
			QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(), &msg, MSG_SIM_GS2C_GATHER);
		}

		if(it->second.m_timer.IsValid())
			it->second.m_timer.SetNull();
		m_playerGather.erase(it);
	}

	//从队列中移出采集玩家并关闭定时器
	std::map<int64,GatherInfo>::iterator itr = m_GatherInfos.find(tmp.m_key);
	if(itr!=m_GatherInfos.end())
	{
		for(size_t i=0; i<itr->second.gatherPlayers.size(); ++i)
		{
			if(itr->second.gatherPlayers[i]!=tmp.m_charID)
			{
				canccelGather(itr->second.gatherPlayers[i],false);
			}
		}
		m_GatherInfos.erase(itr);
	}

	//采集对象移出场景
	Smart_Ptr<Player> player = GetPlayer(tmp.m_charID);
	Smart_Ptr<CreatureObj> obj;
	GetObject(tmp.m_key, obj);
	if(obj.Get()!=NULL && player.Get()!=NULL)
	{
		Smart_Ptr<Monster> mon = obj;
		mon->SetDead(player);
	}

	//采集跳转
	jumpLayer(player);

}

void PVPCarbon::updatePlayerData(int64 charid,int type,int addValue)
{
	std::map<int64,PlayerPvPDataEx>::iterator it = m_playerPvPData.find(charid);
	if(it==m_playerPvPData.end())
		return;

	const PVPLayerInfo* pInfo = PVPMgr::GetInstance()->getLayerInfo(m_layer);
	if(!pInfo)
		return;

	int value = 0;
	switch(type)
	{
	case ePVPLayer_curLayer:
		{
			it->second.layer += addValue;
			value = it->second.layer;
			break;
		}
	case ePVPLayer_killCount:
		{
			it->second.kills += addValue;
			it->second.totalkills += addValue;
			value = it->second.kills;

			it->second.integral += addValue*pInfo->kills;

			//通知世界服用于排序
//			synOnePlayerDataToWorld(charid,it->second);
//
//			Smart_Ptr<Player> player = GetPlayer(charid);
//			if(player.Get()!=NULL)
//			{
//				PvPPackage::PlayerPvPData msg;
//				PvPPackage::pvpData* pData = msg.add_info();
//				pData->set_id(ePVPLayer_totalKills);
//				pData->set_value(it->second.totalkills);
//				QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(), &msg, MSG_SIM_GS2C_PVP_DATA);
//			}
			break;
		}
	case ePVPLayer_beKilled:
		{
			it->second.bekills += addValue;
			value = it->second.bekills;

			it->second.integral += addValue*pInfo->bekills;
			break;
		}
	case ePVPLayer_gather:
		{
			it->second.gather += addValue;
			value = it->second.gather;

			it->second.integral += addValue*pInfo->gather;
			break;
		}
	case ePVPLayer_exp:
		{
			it->second.exp += addValue;
			value = it->second.exp;
			break;
		}
	case ePVPLayer_commonkill:
		{
			it->second.integral += addValue*pInfo->mon;
			break;
		}
	case ePVPLayer_bossKill:
		{
			it->second.integral += addValue*pInfo->boss;
			break;
		}
	default:
		return;
	}

//	Smart_Ptr<Player> player = GetPlayer(charid);
//	if(player.Get()==NULL)
//		return;
//
//	//通知客户端
//	PvPPackage::PlayerPvPData msg;
//	PvPPackage::pvpData* pData = msg.add_info();
//	pData->set_id(type);
//	pData->set_value(value);
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(), &msg, MSG_SIM_GS2C_PVP_DATA);
}

void PVPCarbon::canccelGather(int64 charid,bool isRemoveFromOjb, bool isSendtoClient)
{
	std::map<int64,PlayerGatherData>::iterator it = m_playerGather.find(charid);
	if(it==m_playerGather.end())
		return;

	//从采集列表中移出
	PlayerGatherData& data = it->second;
	if(data.m_timer.IsValid())
		data.m_timer.SetNull();

	if(data.m_pParam!=NULL)
	{
		delete data.m_pParam;
		data.m_pParam = NULL;
	}
	int64 key = it->second.objectKey;
	m_playerGather.erase(it);

	//把玩家从采集对象列表中移出
	if(isRemoveFromOjb)
	{
		std::map<int64,GatherInfo>::iterator objIt = m_GatherInfos.find(key);
		if(objIt!=m_GatherInfos.end())
		{
			std::vector<int64>::iterator playerIt = std::find(objIt->second.gatherPlayers.begin(), objIt->second.gatherPlayers.end(), charid);
			if(playerIt!=objIt->second.gatherPlayers.end())
			objIt->second.gatherPlayers.erase(playerIt);
		}
	}

	Smart_Ptr<Player> player = GetPlayer(charid);
	if(player.Get()==NULL)
			return;

	//通知客户端取消
	PvPPackage::GaterResult msg;
	msg.set_result(1);
	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(), &msg, MSG_SIM_GS2C_GATHER);

}

void PVPCarbon::KillPlayer(Smart_Ptr<CreatureObj>& obj,Smart_Ptr<Player>& target)
{
	//更新杀人数据
	Smart_Ptr<Player> player=obj;
	updatePlayerData(player->GetID(),ePVPLayer_killCount,1);
	jumpLayer(player);

	//更新被杀数据
	updatePlayerData(target->GetID(),ePVPLayer_beKilled,1);
	jumpLayer(target);
}

//void PVPCarbon::GatherJump(Smart_Ptr<Player>& player)
//{
//	if(getLayer() >= PVP_MAX_LAYER)
//		return;
//
//	const PVPLayerInfo* pInfo = PVPMgr::GetInstance()->getLayerInfo(getLayer());
//	if(pInfo==NULL)
//			return;
//
//	std::map<int64,PlayerPvPDataEx>::iterator it = m_playerPvPData.find(player->GetID());
//	if(it==m_playerPvPData.end())
//		return;
//
//	if(it->second.gather < pInfo->gather)
//		return;
//
//	jumpto(player,getLayer()+1);
//
//}
//
//void PVPCarbon::jumpLayer(Smart_Ptr<Player>& player,bool isBeKill)
//{
//	if(!isBeKill && getLayer() >= PVP_MAX_LAYER)
//		return;
//	if(isBeKill && getLayer()==1)
//		return;
//
//	std::map<int64,PlayerPvPDataEx>::iterator it = m_playerPvPData.find(player->GetID());
//	if(it==m_playerPvPData.end())
//		return;
//
//	const PVPLayerInfo* pInfo = PVPMgr::GetInstance()->getLayerInfo(getLayer());
//	if(pInfo==NULL)
//		return;
//
//	if(isBeKill)
//	{
//		if(it->second.bekills >= pInfo->bekills)
//		{
//			jumpto(player,getLayer()-1);
//		}
//	}
//	else
//	{
//		if(it->second.kills >= pInfo->kills)
//		{
//			jumpto(player,getLayer()+1);
//		}
//	}
//}

void PVPCarbon::jumpLayer(Smart_Ptr<Player>& player)
{
	if(getLayer() >= PVP_MAX_LAYER)
		return;

	const PVPLayerInfo* pInfo = PVPMgr::GetInstance()->getLayerInfo(getLayer());
	if(pInfo==NULL)
			return;

	std::map<int64,PlayerPvPDataEx>::iterator it = m_playerPvPData.find(player->GetID());
	if(it==m_playerPvPData.end())
		return;

	if(it->second.integral < pInfo->max)
		return;

	jumpto(player,getLayer()+1);
}

void PVPCarbon::jumpto(Smart_Ptr<Player>& player,int layer)
{
	InstanceMapEntry* instanceInfo = GetInstanceScript();
	if(instanceInfo==NULL)
		return;

	//先把数据同步到世界服
	std::map<int64,PlayerPvPDataEx>::iterator it = m_playerPvPData.find(player->GetID());
	if(it!=m_playerPvPData.end())
	{
		synOnePlayerDataToWorld(player->GetID(), it->second);
	}
	
	//切换层数
	int64 zoneid = PVPMgr::GetInstance()->getMapIDByLayer(layer);
	PlayerMessage::GetInstance()->ForceChangeMap(player, zoneid, instanceInfo->m_nXPos, instanceInfo->m_nYPos);
}

void PVPCarbon::synBossHP(Smart_Ptr<Player>& player)
{
	int hp = getBossHP();
	if(hp <= 0)
		return;

	PvPPackage::BossHP ret;
	ret.set_hp(hp);
	player->SendSynchCommon(&ret,MSG_SIM_GS2C_PVP_BOSS_HP);
}

int PVPCarbon::getBossHP()
{
	if(m_BossObject.Get()!=NULL)
	{
		return m_BossObject->getAttManage()->getValue<int>(eCharHP);
	}
	return 0;
}

void PVPCarbon::addBoosDamage(Smart_Ptr<Player>& player, int key, int value)
{
	if(m_BossObject.Get()==NULL || m_BossObject->GetKey()!= key)
		return;

	std::vector<BossDamageData>::iterator it=std::find_if(m_BossDamageList.begin(), m_BossDamageList.end(),FindCmp(player->GetID()));
	if(it!=m_BossDamageList.end())
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
		data.sex = player->GetSex();
		data.pro = player->getAttManage()->getValue<int>(eCharProfession);
		data.lv = player->getAttManage()->getValue<int>(eCharLv);

		m_BossDamageList.push_back(data);
	}

}

void PVPCarbon::BrocastBossHP(int key)
{
	if(m_BossObject.Get()==NULL || m_BossObject->GetKey()!= key)
			return;

	int hp=getBossHP();
	PvPPackage::BossHP ret;
	ret.set_hp(hp);
	SendMapAllSynchMessage(&ret, MSG_SIM_GS2C_PVP_BOSS_HP, 0, true);
}

void PVPCarbon::firstFight(Smart_Ptr<Player>& player)
{
	if(player.Get()==NULL)
		return;

	int flag = player->getAttManage()->getValue<int>(ePvpFlag);
	if(flag > 0)
		return;

	ServerReturn::ServerRetInt msg;
	msg.set_ret(flag);
	player->SendSynchMessage(&msg,MSG_SIM_GS2C_PVP_FIRST_WAR);
	player->getAttManage()->setValue(ePvpFlag,1);
	player->SetDataFlag(eBaseInfo);
}

bool PVPCarbon::Update()
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

				//Boss死亡领取奖励后10秒，传出副本
				if(nowTime - m_awardTime >= 10*1000)
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

int PVPCarbon::Join(Smart_Ptr<Object>& obj)
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

			synBossHP(player);
			//血量和蓝量全满
			player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));
			player->SetCurMP(player->getAttManage()->getValue<int>(eCharMPMax));

			vector<int> tempBroadcast;
			tempBroadcast.push_back(eCharHP);
			tempBroadcast.push_back(eCharMP);
			player->BroadcastCharAttribute(tempBroadcast);

			PlayerPvPDataEx &data = m_playerPvPData[id];
			data.charid = id;
			data.layer = getLayer();
			data.name = player->GetName();
			data.level = player->getAttManage()->getValue<int>(eCharLv);
			data.viplevel = player->getAttManage()->getValue<DWORD>(eVIPLv);
			data.power = player->getAttManage()->getValue<int>(eFightPower);

			//请求玩家数据排行数据
			Smart_Ptr<SvrItem> worldSvr;
			ServerConHandler::GetInstance()->GetWorldServerBySvrID(getWorldServer(),worldSvr);
			if(worldSvr.Get()!=NULL)
			{
					PvPPackage::ReqPlayerPVPData req;
					req.set_charid(id);
					req.set_layer(getLayer());
					Safe_Smart_Ptr<CommBaseOut::Message> message;
					Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GS2WS_PVP_PLAYER_DATA, &req, worldSvr.Get(), Request);
					messRet->SetAct(new FirstAct(message,id));
					Message_Facade::Send(messRet);
			}

			//玩家首战提示
			firstFight(player);
	}

	//
	if(obj->GetType()==eMonster)
	{
		Smart_Ptr<Monster> mon = obj;
		if(mon->GetQuality() == eBossMonster)
		{
			m_BossObject = mon;
			m_BossDamageList.clear();
		}
	}

	return ret;
}

bool PVPCarbon::JoinHandle(Smart_Ptr<Object>& obj)
{
		if(obj->GetType() == ePlayer)
		{
			Smart_Ptr<Player> player = obj;

			if(!m_monsterDeadEvent.IsValid())
			{
				m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &PVPCarbon::HandleMonsterDeadEvent);
			}

			if(!m_playerAttackEvent.IsValid())
			{
				m_playerAttackEvent = RegGlobalEventPtr(PLAYER_ATTACK_CREATURE, this, &PVPCarbon::PlayerAttack);
			}

			if(!m_betAttackEvent.IsValid())
			{
				m_betAttackEvent = RegGlobalEventPtr(PET_ATTACK, this, &PVPCarbon::BetAttack);
			}

			if(!m_playerBeAttackEvent.IsValid())
			{
				m_playerBeAttackEvent = RegGlobalEventPtr(PLAYER_BEATTACK, this, &PVPCarbon::PlayerBeAttack);
			}
			if(!m_playerMoveEvent.IsValid())
			{
				m_playerMoveEvent = RegGlobalEventPtr(PLAYER_MOVE, this, &PVPCarbon::PlayerMove);
			}

			if(!m_playerDeadEvent.IsValid())
			{
				m_playerDeadEvent = RegGlobalEventPtr(CREATURE_DEAD_EVENT_TYPE, this, &PVPCarbon::HandlePlayerDeadEvent);
			}

			PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
			player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

			if(m_process == eCreated)
				m_process = ePlayerWork;
		}

//		if(obj->GetType() == eMonster)
//		{
//			//控制火种和boss的刷新
//			Smart_Ptr<Monster> mon = obj;
//			if(mon->GetID() == PVPMgr::GetInstance()->getBossID() && getLayer()!=PVP_MAX_LAYER)
//				return false;
//			if(mon->GetQuality()==ePVPGatherMonster && getLayer()==PVP_MAX_LAYER)
//				return false;
//		}

	return true;
}

int PVPCarbon::Move(Smart_Ptr<Object>& obj)
{
	return CGameZone::Move(obj);
}

void PVPCarbon::Leave(Smart_Ptr<Object>& obj, bool isDel, bool delFromMap)
{
		LeaveHandle(obj);
		CGameZone::Leave(obj, isDel, delFromMap);

		//离开后预设地图
		if(obj->GetType() == ePlayer)
		{
			InstanceMapEntry* instanceInfo = GetInstanceScript();
			if(instanceInfo==NULL)
				return;

			MapStaticInfo *sMapInfo = CMapConfig::GetInstance()->GetStaticInfo(instanceInfo->m_nMapID);
			if(sMapInfo==NULL)
				return;

			int64 mapid = CREATE_MAP(eStaticMap, 1, 1, sMapInfo->reliveMap, 1);
			Smart_Ptr<Player> player = obj;

			player->SetMapID(mapid);
			player->SetXPos(sMapInfo->reliveXPos);
			player->SetYPos(sMapInfo->reliveYPos);
			player->SetXNewPos();
			player->SetYNewPos();
			player->SetDataFlag(eBaseInfo);
		}
}

void PVPCarbon::LeaveHandle(Smart_Ptr<Object>& obj)
{
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

					//如果不传地图Id就表示原地复活
				req.set_key(player->GetKey());
				req.set_hp(player->getAttManage()->getValue<int>(eCharHP));
				req.set_mp(player->getAttManage()->getValue<int>(eCharMP));

				player->SendSynchMessage(&req, MSG_SIM_GT2C_REVIVAL);
			}

			//移除玩家无敌状态及buffer
			player->UnsetCreatureState(ERS_Invincible, false);
			player->RemoveBuffById(PVPMgr::GetInstance()->getProtectedBufferID());

			//保存数据到世界服
			std::map<int64,PlayerPvPDataEx>::iterator it = m_playerPvPData.find(player->GetID());
			if(it!=m_playerPvPData.end())
			{
					synOnePlayerDataToWorld(player->GetID(), it->second);
//					synWorldLayerChange(player->GetID(),1);
//					//删除活动数据
//					m_playerPvPData.erase(it);
			}

			//删除玩家的采集数据
			canccelGather(player->GetID(), true, false);
		}
}

void PVPCarbon::LeaveExame(Smart_Ptr<Player>& player)
{
//		synWorldLayerChange(player->GetID(),0);
}

bool PVPCarbon::IsCanEnter(Smart_Ptr<Player>& player)
{
	//传送阶段，就不允许进入了
		if(m_process >= eDuplicateChangeMap)
				return false;

		const PVPLayerInfo* pInfo = PVPMgr::GetInstance()->getLayerInfo(m_layer);
		if(!pInfo)
			return false;

		if(pInfo->min > 0)
		{
			std::map<int64,PlayerPvPDataEx>::iterator it = m_playerPvPData.find(player->GetID());
			if(it==m_playerPvPData.end())
				return false;

			if(it->second.integral < pInfo->min)
				return false;
		}

		return true;
}

void PVPCarbon::Win()
{
		m_isSuccess = true;
		m_process = eAward;
}

void PVPCarbon::Over()
{
	m_process = eAward;
}

void PVPCarbon::Count()
{
}

void PVPCarbon::CountEx()
{
		int64 nowTime = CUtil::GetNowSecond();
		if(nowTime - m_leaveTime >= m_data->m_nDelTime)
		{
			m_process = eOver;
		}
}

void PVPCarbon::Failed()
{
	m_process = eAward;
}

void PVPCarbon::Award()
{
		m_process = eDuplicateChangeMap;
		m_awardTime = CUtil::GetNowSecond();
}

void PVPCarbon::Forbid()
{
	m_isForbid = true;
	release();
}

bool PVPCarbon::Gather(Smart_Ptr<Player> &player, int key)
{
		if(isForbid())
			return false;

		//查找有没有该采集对象
		Smart_Ptr<CreatureObj> obj;
		GetObject(key, obj);
		if(!obj)
		{
			return false;
		}

		if(obj->GetType() != eMonster)
			return false;

		Smart_Ptr<Monster> mon = obj;
		MonsterInfo info;
		MonsterLoader::GetInstance()->GetMonsterInfo(mon->GetID(),info);
		if(info.quality != ePVPGatherMonster)
			return false;

		int time=PVPMgr::GetInstance()->getGatherTime()/1000;
		//开始采集计时
		PlayerGatherData& tmp = m_playerGather[player->GetID()];
		tmp.charID = player->GetID();
		tmp.objectKey = key;
		if(tmp.m_timer.IsValid())
				tmp.m_timer.SetNull();

		GatherEx* pParam = new GatherEx(key,player->GetID());
		tmp.m_timer = TimerInterface::GetInstance()->RegTimePoint(&PVPCarbon::onGatherTimer, this, (void*)(pParam), 0, 0, time, 0);

		//对象被玩家采集列表
		GatherInfo& data = m_GatherInfos[key];
		data.objectKey = key;
		data.gatherPlayers.push_back(player->GetID());

		return true;
}

void PVPCarbon::initPlayerData(int64 charid,const PvPPackage::AckPlayerPVPData& info)
{
	PlayerPvPDataEx &data = m_playerPvPData[charid];
	data.charid = charid;
	data.totalkills = info.info().totalkill();
	data.exp = info.info().totalexp();
	data.killSort = info.info().killsort();
	data.integral = info.info().integral();
}


void PVPCarbon::getPlayerData(int64 charid,PvPPackage::PlayerPvPData &toClient)
{
		std::map<int64,PlayerPvPDataEx>::iterator it = m_playerPvPData.find(charid);
		if(it != m_playerPvPData.end())
		{
			PvPPackage::pvpData* pItem = toClient.add_info();
			pItem->set_id(ePVPLayer_curLayer);
			pItem->set_value(it->second.layer);

			pItem = toClient.add_info();
			pItem->set_id(ePVPLayer_killCount);
			pItem->set_value(it->second.kills);

			pItem = toClient.add_info();
			pItem->set_id(ePVPLayer_beKilled);
			pItem->set_value(it->second.bekills);

			pItem = toClient.add_info();
			pItem->set_id(ePVPLayer_gather);
			pItem->set_value(it->second.gather);

			pItem = toClient.add_info();
			pItem->set_id(ePVPLayer_totalKills);
			pItem->set_value(it->second.totalkills);

			pItem = toClient.add_info();
			pItem->set_id(ePVPLayer_killSort);
			pItem->set_value(it->second.killSort);

			pItem = toClient.add_info();
			pItem->set_id(ePVPLayer_exp);
			pItem->set_value(it->second.exp);
		}
		else
		{
			PvPPackage::pvpData* pItem = toClient.add_info();
			pItem->set_id(ePVPLayer_curLayer);
			pItem->set_value(1);
		}
}


