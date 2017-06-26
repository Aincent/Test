/*
 * SceneManager.cpp
 *
 *  Created on: 2013��9��27��
 *      Author: helh
 */

#include "SceneManager.h"
#include "ConfigLoader/MapConfig.h"
#include "SvrConfig.h"
#include "GameVillage.h"
#include "Network/MessageManager.h"
#include "Network/Message_Facade.h"
#include "MessageStruct/Map/GS2GTAllMap.pb.h"
#include "ServerMsgDefine.h"
#include "Log/Logger.h"
#include <math.h>
#include "Memory/MemAllocator.h"
#include "GameDuplicate.h"
#include "DuplicateMgr.h"
#include "../ServerEventDefine.h"
#include "../EventSet/EventDefine.h"

CSceneMgr * CSceneMgr::m_instance = 0;

CSceneMgr::CSceneMgr()
{

}

CSceneMgr::~CSceneMgr()
{
	m_allZone.clear();
}

void CSceneMgr::Update(int dwTick)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.begin();
	for(; it!=m_allZone.end(); ++it)
	{
		it->second->Update(dwTick);
	}

	DestroySceneList();
}

void CSceneMgr::AddMap(int64 id, Smart_Ptr<CGameZone> &newMap)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(id);
	if(it == m_allZone.end())
	{
		m_allZone[id] = newMap;

		if(GET_MAP_TYPE(id) > eCopyMap)
		{
			Smart_Ptr<CGameDuplicate> instance = newMap;
			DuplicateMgr::GetInstance()->AddMap(instance);
		}
	}
	else
	{
		LOG_ERROR(FILEINFO, "create map and add map but map is existed");
	}
}

void CSceneMgr::DestroySceneList()
{

}

void CSceneMgr::SendAllMap(short int remoteid, unsigned char remoteType, int channelid, short int localid, unsigned char localType)
{
	Safe_Smart_Ptr<CommBaseOut::Message> message=NEW CommBaseOut::Message();
	message->SetSecurity(false);
	message->SetMessageType(SimpleMessage);
	message->SetRemoteID(remoteid);
	message->SetRemoteType(remoteType);
	message->SetLocalID(localid);
	message->SetLocalType(localType);
	message->SetMessageID(MSG_SIM_GM2GT_PUSHALLMAP);
	message->SetChannelID(channelid);

	string con;
	GameSvrToGate::GSAllMap meContent;
	meContent.set_id(localid);

	vector<GameSvrMap>::iterator itCopy = CMapConfig::GetInstance()->GetGameMap().begin();
	vector<GameSvrMap>::iterator itCopyEnd = CMapConfig::GetInstance()->GetGameMap().end();
	for(; itCopy!=itCopyEnd; ++itCopy)
	{
		if(!CMapConfig::GetInstance()->IsHaveMap((*itCopy).mapid))
		{
			LOG_ERROR(FILEINFO, "send map to Gateserver but gameserver map is not existed in world");
		}

		GameSvrToGate::GSMapInfo* copyInfo = meContent.add_scene();

		copyInfo->set_maptype((*itCopy).maptype);
		copyInfo->set_staticid(itCopy->mapid);
	}

	meContent.SerializePartialToString(&con);
	message->SetContent(con.c_str(),con.size());
	Message_Facade::Send(message);
}

void CSceneMgr::SendSynchMessage(::google::protobuf::Message *from, int msgid, int64 sceneid, int key, int xpos, int ypos, bool isMyself)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		it->second->SendSynchMessage(from, msgid, key, xpos, ypos, isMyself);
	}
	else
	{
		LOG_ERROR(FILEINFO, "SendSynchCommon but find not scene[sceneid=%lld]", sceneid);
	}
}

void CSceneMgr::SendMapAllSynchMessage(::google::protobuf::Message *from, int msgid, int64 sceneid, int key, bool isMyself)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		it->second->SendMapAllSynchMessage(from, msgid, key, isMyself);
	}
	else
	{
		LOG_ERROR(FILEINFO, "SendSynchCommon but find not scene[sceneid=%lld]", sceneid);
	}
}

void CSceneMgr::SendSynchAttrChange(CharBase::SynchObjectAttr &attr, int64 sceneid, int xpos, int ypos, bool isMyself)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		it->second->SendSynchAttr(attr, xpos, ypos, isMyself);
	}
	else
	{
		LOG_ERROR(FILEINFO, "synch creature attr but find not scene[sceneid=%lld]", sceneid);
	}
}

void CSceneMgr::SendAllMapAttr(CharBase::SynchObjectAttr &attr, int64 sceneid)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		it->second->SendAllMapAttr(attr);
	}
	else
	{
		LOG_ERROR(FILEINFO, "synch creature attr but find not scene[sceneid=%lld]", sceneid);
	}
}

void CSceneMgr::SendSynBuffChange(CharBattle::BuffOperate *buf, int key, int64 sceneid, int xpos, int ypos)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		it->second->SendSynBuffChange(buf, key, xpos, ypos);
	}
	else
	{
		LOG_ERROR(FILEINFO, "synch creature attr but find not scene[sceneid=%lld]", sceneid);
	}
}

//void CSceneMgr::SendSynchAttrState(CharBattle::ObjectState &attr, int64 sceneid, int xpos, int ypos)
//{
//	map<int64, Smart_Ptr<CGameZone> >::iterator it = m_allZone.find(sceneid);
//	if(it != m_allZone.end())
//	{
//		it->second->SendSynchAttrState(attr, xpos, ypos);
//	}
//	else
//	{
//		LOG_ERROR(FILEINFO, "synch attr state but find not scene[sceneid=%lld]", sceneid);
//	}
//}

void CSceneMgr::SendSynchAttackAction(CharBattle::SynClientAttack &action, int64 sceneid, int xpos, int ypos)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		it->second->SendSynchAction(action, xpos, ypos);
	}
	else
	{
		LOG_ERROR(FILEINFO, "synch creature action but find not scene[sceneid=%lld]", sceneid);
	}
}

int CSceneMgr::GetSectorGroup(int64 mapid, int xpos, int ypos)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(mapid);
	if(it != m_allZone.end())
	{
		return it->second->GetSectorGroupIndex(xpos, ypos);
	}
	else
	{
		LOG_ERROR(FILEINFO, "player get sector group but find not scene[mapid=%lld]", GET_MAP_ID(mapid));
	}

	return 0;
}

bool CSceneMgr::EnterScene(Smart_Ptr<Object> & obj)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(obj->GetMapID());
	if(it != m_allZone.end())
	{
		Smart_Ptr<CGameZone> tempZone = it->second;
		int res = tempZone->Join(obj);
		if(res)
		{
			LOG_ERROR(FILEINFO, "object cannot enter scene[sceneid=%lld]", obj->GetMapID());

			return false;
		}

		if(obj->GetType() == ePlayer)
		{
			Smart_Ptr<Player> player = obj;

			PlayerMapEventArgs args(obj);

			FireGlobalEvent(PLAYER_ENTER_MAP_EVENT_TYPE, args);

			player->FireEvent(PLAYER_ENTER_MAP_EVENT_TYPE, args);
		}
	}
	else
	{
		LOG_ERROR(FILEINFO, "object enter scene but find not scene[sceneid=%lld]", obj->GetMapID());

		return false;
	}

	return true;
}

bool CSceneMgr::MoveTo(Smart_Ptr<Object> &obj)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(obj->GetMapID());
	if(it != m_allZone.end())
	{
		int res = it->second->Move(obj);
		if(res)
		{
			return false;
		}

		if(obj->GetType() == ePlayer)
		{
			Smart_Ptr<Player> player = obj;
			OneParmaEventArgs e(obj);

			player->FireEvent(PLAYER_POSITION_TOPOSITION, e);
		}
	}
	else
	{
		LOG_ERROR(FILEINFO, "object move but find not scene[sceneid=%lld]", obj->GetMapID());

		return false;
	}

	return true;
}

bool CSceneMgr::MoveToAll(Smart_Ptr<Object> &obj)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(obj->GetMapID());
	if(it != m_allZone.end())
	{
		int res = it->second->MoveToAll(obj);
		if(res)
		{
			return false;
		}

		if(obj->GetType() == ePlayer)
		{
			Smart_Ptr<Player> player = obj;
			OneParmaEventArgs e(obj);

			player->FireEvent(PLAYER_POSITION_TOPOSITION, e);
		}
	}
	else
	{
		LOG_ERROR(FILEINFO, "object move but find not scene[sceneid=%lld]", obj->GetMapID());

		return false;
	}

	return true;
}

void CSceneMgr::LeaveScene(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(obj->GetMapID());
	if(it != m_allZone.end())
	{
		if(obj->GetType() == ePlayer || obj->GetType() == eMonster || obj->GetType() == ePet)
		{
			Smart_Ptr<CreatureObj> player = obj;
			OneParmaEventArgs args(obj);

			player->FireEvent(PLAYER_LEAVE_MAP_EVENT_TYPE, args);
		}
		it->second->Leave(obj, isDel, delFromMap);
	}
	else
	{
		LOG_ERROR(FILEINFO, "object Leave Scene but find not scene[sceneid=%lld]", obj->GetMapID());
	}
}

Smart_Ptr<CGameZone> CSceneMgr::GetGameZone(DWORD64 sceneid)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it == m_allZone.end())
	{
		LOG_ERROR(FILEINFO, "Get Scene but find not scene[sceneid=%lld]", sceneid);

		return 0;
	}

	return it->second;
}

Smart_Ptr<CGameZone> CSceneMgr::GetGameZoneByMapID(int mapid)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.begin();
	for(; it!=m_allZone.end(); ++it)
	{
		if(GET_MAP_ID(it->first) == mapid)
		{
			return it->second;
		}
	}

	return 0;
}

Sector * CSceneMgr::GetSector(DWORD64 sceneid, int xpos, int ypos)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		return it->second->GetSector(xpos, ypos);
	}

	return 0;
}

void CSceneMgr::GetObject(int64 mapid, int key, Smart_Ptr<CreatureObj> &obj, bool isMon)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(mapid);
	if(it != m_allZone.end())
	{
		it->second->GetObject(key, obj, isMon);
	}
	else
	{
		LOG_ERROR(FILEINFO, "Get object but find not scene[sceneid=%lld]", mapid);
	}
}

void CSceneMgr::GetSkillObject(Smart_Ptr<CreatureObj> &owner, int xpos, int ypos, Smart_Ptr<CreatureObj>& target, int targetType, int targetNUm, int type, int distance,int width, vector<Smart_Ptr<CreatureObj> > &obj)
{
	//默认目标为敌人
	if(targetType==eSkillTargetTypeNull)
	{
		targetType = eEnemyTarget;
	}

	switch(targetType)
	{
	case eEnemyTarget:
		{
			if(targetNUm == 1 && target.Get() != NULL)
			{
				if(!target->IsDead() && target->IsInMap() && 0 == owner->IsAttacked(target, eEnemyTarget) )
				{
					obj.push_back(target);
				}
			}
			else
			{
				if(target && !target->IsDead() && target->IsInMap())
				{
					if(0 == owner->IsAttacked(target, eEnemyTarget))
					{
						obj.push_back(target);
					}
				}

				vector<Smart_Ptr<CreatureObj> > tempObject;
				GetObject(owner, xpos, ypos, type, distance, width, tempObject);

				vector<Smart_Ptr<CreatureObj> > ::iterator it = tempObject.begin();
				for(; it != tempObject.end(); ++it)
				{
					if(0 == owner->IsAttacked(*it, eEnemyTarget) && (*it).Get() != target.Get() )
					{
						obj.push_back(*it);
					}
				}
			}
			break;
		}

	case eFriendTarget:
		{
			if(targetNUm == 1 && target.Get() != NULL)
			{
				if(!target->IsDead() && target->IsInMap() && 0 == owner->IsAttacked(target, eFriendTarget) )
				{
					obj.push_back(target);
				}
			}
			else
			{
				if(target && !target->IsDead() && target->IsInMap())
				{
					if(0 == owner->IsAttacked(target, eFriendTarget))
					{
						obj.push_back(target);
					}
				}

				vector<Smart_Ptr<CreatureObj> > tempObject;
				GetObject(owner, xpos, ypos, type, distance, width, tempObject);

				vector<Smart_Ptr<CreatureObj> > ::iterator it = tempObject.begin();
				for(; it != tempObject.end(); ++it)
				{
					if(0 == owner->IsAttacked(*it, eFriendTarget) && (*it).Get() != target.Get())
					{
						obj.push_back(*it);
					}
				}
			}
			break;
		}

	case eMyself:
		{
			if(owner && !owner->IsDead() && owner->IsInMap())
			{
				obj.push_back(owner);
			}
			break;
		}

	case eNoPlayer:
		{
			vector<Smart_Ptr<CreatureObj> > tempObject;
			GetObject(owner, xpos, ypos, type, distance, width, tempObject);

			vector<Smart_Ptr<CreatureObj> > ::iterator it = tempObject.begin();
			for(; it != tempObject.end(); ++it)
			{
				if(0 == owner->IsAttacked(*it, eNoPlayer) && (*it).Get() != target.Get())
				{
					obj.push_back(*it);
				}
			}
			break;
		}
	}
}

void CSceneMgr::GetObject(Smart_Ptr<CreatureObj> &owner, int xpos, int ypos, int type, int distance,int width, vector<Smart_Ptr<CreatureObj> > &obj)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(owner->GetMapID());
	if(it != m_allZone.end())
	{
		it->second->GetObject(owner, xpos, ypos, type, distance,width, obj);
	}
	else
	{
		LOG_ERROR(FILEINFO, "Get object but find not scene[sceneid=%lld]", owner->GetMapID());
	}
}

void CSceneMgr::GetObject(int64 mapid, int xpos, int ypos, int distance, int country,eObjType eType, vector<Smart_Ptr<CreatureObj> > &obj)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(mapid);
	if(it != m_allZone.end())
	{
		it->second->GetObject(xpos, ypos, distance, country,eType, obj);
	}
	else
	{
		LOG_ERROR(FILEINFO, "Get object but find not scene[sceneid=%lld]", mapid);
	}
}

int64 CSceneMgr::GetSceneIDAboutPlayerCount(int mapid)
{
	int64 ret = -1;
	int num = -1;

	INT64CGAMEZONEMAP::iterator it = m_allZone.begin();
	for(; it!=m_allZone.end(); ++it)
	{
		if(GET_MAP_ID(it->first) == mapid)
		{
			if(ret == -1)
			{
				num = it->second->GetPlayerCount();
				ret = it->first;
				continue;
			}

			if(num > it->second->GetPlayerCount())
			{
				num = it->second->GetPlayerCount();
				ret = it->first;
			}
		}
	}

	return ret;
}


int64 CSceneMgr::GetSceneIDAboutPlayerIsLoginOn(int mapid)
{
	int64 ret = -1;
	int num = -1;

	INT64CGAMEZONEMAP::iterator it = m_allZone.begin();
	for(; it!=m_allZone.end(); ++it)
	{
		if(GET_MAP_ID(it->first) == mapid)
		{
			if(ret == -1)
			{
				num = it->second->GetPlayerCount();
				ret = it->first;
				continue;
			}

			if(num > it->second->GetPlayerCount())
			{
				num = it->second->GetPlayerCount();
				ret = it->first;
			}
		}
	}
	return ret;
}



bool CSceneMgr::IsHaveScene(int64 sceneid)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		return true;
	}

	return false;
}

float CSceneMgr::GetDistance(int bXPos, int bYPos, int tXPos, int tYPos)
{
	float distance = 0;

	float xLen = abs(bXPos - tXPos);
	float yLen = abs(bYPos - tYPos);

	distance = sqrt(xLen * xLen + yLen * yLen);

	return distance;
}

void CSceneMgr::GetAroundPos(int64 sceneid, int xpos, int ypos, int count, vector<Sector *> &vec)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		it->second->GetAroundPos(xpos, ypos, count, vec);
	}
	else
	{
		LOG_ERROR(FILEINFO, "Get around pos but find not scene[sceneid=%lld]", sceneid);
	}
}

void CSceneMgr::GetAroundPos(int64 sceneid, int xpos, int ypos, int expos, int eypos, vector<Pos> &vec)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		it->second->GetAroundPos(xpos, ypos, expos, eypos, vec);
	}
	else
	{
		LOG_ERROR(FILEINFO, "Get around pos but find not scene[sceneid=%lld]", sceneid);
	}
}

void CSceneMgr::GetGroupObject(Smart_Ptr<CreatureObj> &owner, vector<Smart_Ptr<CreatureObj> > &obj)
{
	if(!owner)
	{
		return;
	}

	INT64CGAMEZONEMAP::iterator it = m_allZone.find(owner->GetMapID());
	if(it != m_allZone.end())
	{
		it->second->GetGroupObject(owner, obj);
	}
	else
	{
		LOG_ERROR(FILEINFO, "Get object but find not scene[sceneid=%lld]", owner->GetMapID());
	}
}

void CSceneMgr::DeleteInstance(int64 instanceid)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(instanceid);
	if(it != m_allZone.end())
	{
		m_allZone.erase(it);
	}
}

void CSceneMgr::DeleteScene(int64 sceneID)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneID);
	if(it != m_allZone.end())
	{
		m_allZone.erase(it);
	}

	if(GET_MAP_TYPE(sceneID) >= eCopyMap)
	{
		DuplicateMgr::GetInstance()->DeleteMap(sceneID);
	}
}

void CSceneMgr::UpdatePath( int64 sceneid, const PlayerSynch::SingleMonsterPath * path )
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		it->second->UpdatePath( path );
	}
	else
	{
		LOG_ERROR(FILEINFO, "update object path but find not scene[sceneid=%lld]", sceneid);
	}
}

bool CSceneMgr::IsValidPos(int64 sceneid, PlayerSynch::SynchInfo &pos)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		return it->second->IsValidPos( pos );
	}
	else
	{
		LOG_ERROR(FILEINFO, "examining pos but find not scene[sceneid=%lld]", sceneid);

		return false;
	}
}

bool CSceneMgr::IsValidPos(int64 sceneid, int xpos, int ypos)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		return it->second->IsValidPos( xpos, ypos );
	}
	else
	{
		LOG_ERROR(FILEINFO, "examining pos but find not scene[sceneid=%lld]", sceneid);

		return false;
	}
}

bool CSceneMgr::HaveValidPos(int64 sceneid, int xpos, int ypos, int expos, int eypos)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		return it->second->HaveValidPos( xpos, ypos, expos, eypos );
	}
	else
	{
		LOG_ERROR(FILEINFO, "examining pos but find not scene[sceneid=%lld]", sceneid);

		return false;
	}
}

bool CSceneMgr::RandGetValidPos(int64 sceneid, int xpos, int ypos, int &expos, int &eypos)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		return it->second->RandGetValidPos(xpos, ypos, expos, eypos );
	}
	else
	{
		LOG_ERROR(FILEINFO, "rand get pos but find not scene[sceneid=%lld]", sceneid);

		return false;
	}
}

bool CSceneMgr::RandGetPetValidPos(int64 sceneid, int xpos, int ypos, int &expos, int &eypos)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		return it->second->RandGetPetValidPos(xpos, ypos, expos, eypos );
	}
	else
	{
		LOG_ERROR(FILEINFO, "rand get pos but find not scene[sceneid=%lld]", sceneid);

		return false;
	}
}

void CSceneMgr::GetFinalPosLine(int64 sceneid, int bxpos, int bypos, int expos, int eypos, int &xpos, int &ypos, int distance)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		return it->second->GetFinalPosLine( bxpos, bypos, expos, eypos, xpos, ypos, distance );
	}
	else
	{
		LOG_ERROR(FILEINFO, "get final pos by line but find not scene[sceneid=%lld]", sceneid);

		return;
	}
}

void CSceneMgr::GetBeginPosLine(int64 sceneid, int bxpos, int bypos, int expos, int eypos, int &xpos, int &ypos, int distance)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(sceneid);
	if(it != m_allZone.end())
	{
		return it->second->GetBeginPosLine( bxpos, bypos, expos, eypos, xpos, ypos, distance );
	}
	else
	{
		LOG_ERROR(FILEINFO, "get final pos by line but find not scene[sceneid=%lld]", sceneid);

		return;
	}
}

void CSceneMgr::AddWorldBossToZone(int key, DWORD64 sceneid, int monsterId)
{
	WORLDBOSSZONEMAP::iterator it = m_worldBossZone.find(key);
	if(it == m_worldBossZone.end())
	{
		pair<int, int64> worldBoss;
		worldBoss.first = monsterId;
		worldBoss.second = sceneid;
		m_worldBossZone[key] = worldBoss;
	}
	else
	{
		LOG_ERROR(FILEINFO, "add world boss to scene but [key=%d] is exsit", key);
	}
}

void CSceneMgr::RemoveBossFromZone(int key)
{
	WORLDBOSSZONEMAP::iterator it = m_worldBossZone.find(key);
	if(it != m_worldBossZone.end())
		m_worldBossZone.erase(it);
	else
		LOG_ERROR(FILEINFO, "remove world boss from scene but [key=%d] is exsit", key);
}

void CSceneMgr::GetAllWorldBossInfo(vector<pair<int,pair<int64,int> > > &isActiveList, Smart_Ptr<Player>& player)
{
	WORLDBOSSZONEMAP::iterator it = m_worldBossZone.begin();
	for(; it != m_worldBossZone.end(); it++)
	{
		pair<int, int64> worldBoss = it->second;
		Smart_Ptr<CGameZone> zone = GetGameZone(worldBoss.second);
		if(zone)
		{
			zone->IsActiveWorldBoss(it->first, isActiveList, player->getAttManage()->getValue<int>(eCharLv));
		}
		else
		{
			LOG_ERROR(FILEINFO, "get the all world boss but zone is not exsit");
		}
	}
}

Smart_Ptr<CGameZone> CSceneMgr::GetGameZoneByWorldBossKey(int key)
{
	WORLDBOSSZONEMAP::iterator it = m_worldBossZone.find(key);
	if(it != m_worldBossZone.end())
	{
		pair<int, int64> map_value = it->second;
		Smart_Ptr<CGameZone> zone = GetGameZone(map_value.second);
		return zone;
	}
	else
	{
		return 0;
	}
}

void CSceneMgr::GetInstance(int64 instanceid,Smart_Ptr<CGameDuplicate>& instance)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(instanceid);
	if(it != m_allZone.end())
	{
		instance = it->second;
	}
}

bool CSceneMgr::SendObjectToGroup(Smart_Ptr<Object> & obj)
{
	INT64CGAMEZONEMAP::iterator it = m_allZone.find(obj->GetMapID());
	if(it != m_allZone.end())
	{
		int res = it->second->SendObjectToGroup(obj);
		if(res)
		{
			LOG_ERROR(FILEINFO, "object cannot find scene[sceneid=%lld]", obj->GetMapID());

			return false;
		}
	}
	else
	{
		LOG_ERROR(FILEINFO, "object enter scene but find not scene[sceneid=%lld]", obj->GetMapID());

		return false;
	}

	return true;
}
