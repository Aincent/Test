/*
 * Sector.cpp
 *
 *  Created on: 2013��10��15��
 *      Author: helh
 */

#include "Sector.h"
#include "Log/Logger.h"
#include "../Object/GirlWar/GirlWarManager.h"
#include "../Object/Creature.h"
#include "../Object/AI/AIBrain.h"
#include "../Object/Monster/Monster.h"
#include "../Object/Player/Player.h"
#include "../ServerManager.h"
#include "CharDefine.h"
#include "MessageStruct/CharLogin/GoodsMessage.pb.h"
#include "./ConfigLoader/FieldInfo.h"
#include "GameZone.h"
#include "../Object/ItemDrop/DropItemAlloc.h"
#include "./ConfigLoader/MapConfig.h"
#include "./ConfigLoader/FieldInfo.h"
#include "../EscortEx/EscortMgr.h"
#include "../Object/PlayerRegion/PlayerRegion.h"


SectorGroup::SectorGroup():m_index(-1)
{
	m_groupSector.reserve(300);
	m_pZone = NULL;
}

SectorGroup::SectorGroup(int index):m_index(index)
{
	m_groupSector.reserve(300);
	m_pZone = NULL;
}

SectorGroup::~SectorGroup()
{
	if(m_pZone != NULL && GET_MAP_TYPE(m_pZone->GetSceneID()) < eCopyMap)
	{
		LOG_ERROR(FILEINFO, "SectorGroup error : mapid=%d, group=%d, size=%d", GET_MAP_ID(m_pZone->GetSceneID()), GetGroupIndex(), GetGroupSectors());
	}

	m_groupSector.clear();
	m_nearbyGroup.clear();
	m_pZone = NULL;
}

void SectorGroup::AddNearbyGroup(SectorGroup * group)
{
	m_nearbyGroup.push_back(group);
}

void SectorGroup::AddGroupSector(Sector * pSector)
{
	m_groupSector.push_back(pSector);
	pSector->SetGroupIndex(m_index);
}

bool SectorGroup::IsNearbyGroup(SectorGroup * group)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
		if(group == *it)
		{
			return true;
		}
	}

	return false;
}

void SectorGroup::SendAllObjectToGroup(Smart_Ptr<Object> & obj, SectorGroup *group)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();

//	if(obj->GetType() == eMonster && group != 0)
//	{
//		Smart_Ptr<Monster> temp = obj;
//		cout<<endl<<"monster name is "<<temp->GetID()<<"  ";
//	}

	for(; it!=m_nearbyGroup.end(); ++it)
	{
		if(group == 0 || !group->IsNearbyGroup(*it))
		{
			(*it)->SendAllObjectToMyself(obj);
		}
	}
}

void SectorGroup::SendAllPlayerToGroup(Smart_Ptr<Object> & obj, SectorGroup *group)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
		if(group == 0 || !group->IsNearbyGroup(*it))
		{
			(*it)->SendAllPlayerToMyself(obj);
		}
	}
}

void SectorGroup::SendAllDropToGroup(Smart_Ptr<Object> & obj, SectorGroup *group)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
		if(group == 0 || !group->IsNearbyGroup(*it))
		{
			(*it)->SendAllDropToMyself(obj);
		}
	}
}

void SectorGroup::SendObjLeaveToGroup(Smart_Ptr<Object> & obj, SectorGroup *group)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
		if(group == 0)
		{
			(*it)->SendObjLeaveToMyself(obj, false);
		}
		else if(!group->IsNearbyGroup(*it))
		{
			(*it)->SendObjLeaveToMyself(obj);
		}
	}
}

void SectorGroup::SendObjLeaveToGroup(Smart_Ptr<Object> & obj)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
			(*it)->SendObjLeaveToMyself(obj);
	}
}

void SectorGroup::SendPlayerLeaveToGroup(Smart_Ptr<Object> & obj, SectorGroup *group)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
		if(group == 0)
		{
			(*it)->SendPlayerLeaveToMyself(obj, false);
		}
		else if(!group->IsNearbyGroup(*it))
		{
			(*it)->SendPlayerLeaveToMyself(obj);
		}
	}
}

void SectorGroup::SendDropLeaveToGroup(Smart_Ptr<Object> & obj, SectorGroup *group)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
		if(group == 0)
		{
			(*it)->SendDropLeaveToMyself(obj);
		}
		else if(!group->IsNearbyGroup(*it))
		{
			(*it)->SendDropLeaveToMyself(obj);
		}
	}
}

void SectorGroup::GetGroupObject(vector<Smart_Ptr<CreatureObj> > &obj)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
		(*it)->GetGroupObjectToMyself(obj);
	}
}

void SectorGroup::GetGroupObjectToMyself(vector<Smart_Ptr<CreatureObj> > &obj)
{
	vector<Sector *>::iterator it = m_groupSector.begin();
	for(; it!=m_groupSector.end(); ++it)
	{
		(*it)->GetCreatrueObj(obj);
	}
}

void SectorGroup::SendAllPlayerToMyself(Smart_Ptr<Object> & obj)
{
	vector<Sector *>::iterator it = m_groupSector.begin();
	for(; it!=m_groupSector.end(); ++it)
	{
		(*it)->SendAllPlayer(obj);
	}
}

void SectorGroup::SendAllDropToMyself(Smart_Ptr<Object> & obj)
{
	vector<Sector *>::iterator it = m_groupSector.begin();
	for(; it!=m_groupSector.end(); ++it)
	{
		(*it)->SendAllDrop(obj);
	}
}

void SectorGroup::SendAllObjectToMyself(Smart_Ptr<Object> & obj)
{
	vector<Sector *>::iterator it = m_groupSector.begin();
	for(; it!=m_groupSector.end(); ++it)
	{
		(*it)->SendAllObject(obj);
	}
}

void SectorGroup::SendObjLeaveToMyself(Smart_Ptr<Object> & obj, bool side)
{
	vector<Sector *>::iterator it = m_groupSector.begin();
	for(; it!=m_groupSector.end(); ++it)
	{
		(*it)->SendObjectLeave(obj, side);
	}
}

void SectorGroup::SendPlayerLeaveToMyself(Smart_Ptr<Object> & obj, bool side)
{
	vector<Sector *>::iterator it = m_groupSector.begin();
	for(; it!=m_groupSector.end(); ++it)
	{
		(*it)->SendPlayerLeave(obj, side);
	}
}

void SectorGroup::SendDropLeaveToMyself(Smart_Ptr<Object> & obj)
{
	vector<Sector *>::iterator it = m_groupSector.begin();
	for(; it!=m_groupSector.end(); ++it)
	{
		(*it)->SendDropLeave(obj);
	}
}

void SectorGroup::SendSynchCommon(::google::protobuf::Message *from,int messageid,int key, bool isMyself)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
		(*it)->SendSynchCommonToMyself(from,messageid,key, isMyself);
	}
}

void SectorGroup::SendSynchMessage(::google::protobuf::Message *from, int msgid, int key, bool isMyself)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
		(*it)->SendSynchMessageToMyself(from, msgid, key, isMyself);
	}
}

void SectorGroup::SendSynchMessageToMyself(::google::protobuf::Message *from, int msgid, int key, bool isMyself)
{
	vector<Sector *>::iterator it = m_groupSector.begin();
	for(; it!=m_groupSector.end(); ++it)
	{
		(*it)->SendSynchMessage(from, msgid, key, isMyself);
	}
}

void SectorGroup::SendSynchCommonToMyself(::google::protobuf::Message *from, int messageid, int key, bool isMyself)
{
	vector<Sector *>::iterator it = m_groupSector.begin();
	for(; it!=m_groupSector.end(); ++it)
	{
		(*it)->SendSynchCommon(from,messageid,key, isMyself);
	}
}

void SectorGroup::SendSynchPath(PlayerSynch::PathToAllPlayer &path)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
		(*it)->SendSynchPathToMyself(path);
	}
}

void SectorGroup::SendSynchPath(PlayerSynch::MonsterPathToPlayer &path)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
		(*it)->SendSynchPathToMyself(path);
	}
}

void SectorGroup::SendSynchPath(PlayerSynch::GirlWarPathToPlayer &path)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
		(*it)->SendSynchPathToMyself(path);
	}
}

void SectorGroup::SendSynchPathToMyself(PlayerSynch::MonsterPathToPlayer &path)
{
	vector<Sector *>::iterator it = m_groupSector.begin();
	for(; it!=m_groupSector.end(); ++it)
	{
		(*it)->SendSynchPath(path);
	}
}

void SectorGroup::SendSynchPathToMyself(PlayerSynch::PathToAllPlayer &path)
{
	vector<Sector *>::iterator it = m_groupSector.begin();
	for(; it!=m_groupSector.end(); ++it)
	{
		(*it)->SendSynchPath(path);
	}
}

void SectorGroup::SendSynchPathToMyself(PlayerSynch::GirlWarPathToPlayer &path)
{
	vector<Sector *>::iterator it = m_groupSector.begin();
	for(; it!=m_groupSector.end(); ++it)
	{
		(*it)->SendSynchPath(path);
	}
}

void SectorGroup::SendSynchAttr(CharBase::SynchObjectAttr &attr, bool isMyself)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
		(*it)->SendSynchAttrToMyself(attr, isMyself);
	}
}

void SectorGroup::SendSynchAttrToMyself(CharBase::SynchObjectAttr &attr, bool isMyself)
{
	vector<Sector *>::iterator it = m_groupSector.begin();
	for(; it!=m_groupSector.end(); ++it)
	{
		(*it)->SendSynchAttr(attr, isMyself);
	}
}
//
//void SectorGroup::SendSynchAttrState(CharBattle::ObjectState &attr)
//{
//	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
//	for(; it!=m_nearbyGroup.end(); ++it)
//	{
//		(*it)->SendSynchAttrStateToMyself(attr);
//	}
//}
//
//void SectorGroup::SendSynchAttrStateToMyself(CharBattle::ObjectState &attr)
//{
//	vector<Sector *>::iterator it = m_groupSector.begin();
//	for(; it!=m_groupSector.end(); ++it)
//	{
//		(*it)->SendSynchAttrState(attr);
//	}
//}

void SectorGroup::SendSynBuffChange(CharBattle::BuffOperate *buf, int key)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
		(*it)->SendSynBuffChangeToMyself(buf, key);
	}
}

void SectorGroup::SendSynBuffChangeToMyself(CharBattle::BuffOperate *buf, int key)
{
	vector<Sector *>::iterator it = m_groupSector.begin();
	for(; it!=m_groupSector.end(); ++it)
	{
		(*it)->SendSynBuffChange(buf, key);
	}
}

void SectorGroup::SendSynchAction(CharBattle::SynClientAttack &action)
{
	vector<SectorGroup *>::iterator it = m_nearbyGroup.begin();
	for(; it!=m_nearbyGroup.end(); ++it)
	{
		(*it)->SendSynchActionToMyself(action);
	}
}

void SectorGroup::SendSynchActionToMyself(CharBattle::SynClientAttack &action)
{
	vector<Sector *>::iterator it = m_groupSector.begin();
	for(; it!=m_groupSector.end(); ++it)
	{
		(*it)->SendSynchAction(action);
	}
}

Sector::Sector():m_index(-1),m_groupIndex(0),m_isValid(false),m_isHaveItem(false),m_pZone(0)
{

}

Sector::Sector(int index, CGameZone *pZone):m_index(index),m_groupIndex(0),m_isValid(false),m_isHaveItem(false),m_pZone(pZone)
{

}

Sector::~Sector()
{
	if(m_pZone != NULL && GET_MAP_TYPE(m_pZone->GetSceneID()) < eCopyMap)
	{
		LOG_ERROR(FILEINFO, "Sector error : mapid=%d, group=%d, size=%d", GET_MAP_ID(m_pZone->GetSceneID()), GetGroupIndex(), GetIndex());
	}

	m_playerHashTable.clear();
	m_monsterHashTable.clear();
	m_girlWarHashTable.clear();
}

void Sector::addRegion(int regionID,const RegionData* data)
{
	m_RegionList[regionID] = data;

	return ;
}

int Sector::Join(Smart_Ptr<Object> & obj)
{
	switch(obj->GetType())
	{
	case ePlayer :
		{
			Smart_Ptr<Player> player = obj;
			INT64PLAYERMAP::iterator it = m_playerHashTable.find(player->GetID());
			if(it != m_playerHashTable.end())
			{
				LOG_ERROR(FILEINFO, "player = %lld already in sector", player->GetID());
			}

			m_playerHashTable[player->GetID()] = player;

			if(player)
				player->SetSector(this);

			break;
		}
	case eMonster:
		{
			Smart_Ptr<Monster> monster = obj;

			INTVMPNSTERMAP::iterator it = m_monsterHashTable.find(monster->GetID());
			if(it == m_monsterHashTable.end())
			{
				vector<Smart_Ptr<Monster> > tmpVec;

				tmpVec.push_back(monster);
				m_monsterHashTable[monster->GetID()] = tmpVec;
//				if(monster->GetID() == 31060002)
//					printf("\n++++++++++++++++++++怪物进入    sectorindex = %d, sectorgroup = %d，（%d, %d） \n",  m_index, m_groupIndex,GetPosition()>>16,GetPosition()&0xffff);
			}
			else
			{
				vector<Smart_Ptr<Monster> >::iterator itVec = it->second.begin();
				for(; itVec!=it->second.end(); ++itVec)
				{
					if(obj->GetKey() == (*itVec)->GetKey())
					{
						LOG_ERROR(FILEINFO, "monster = %d already in sector", (*itVec)->GetID());
					}
				}

				it->second.push_back(monster);
			}

			if(monster)
				monster->SetSector(this);

			break;
		}
	case eDropItem:
		{
			m_dropItem = obj;

			break;
		}
	case ePet:
		{
			Smart_Ptr<GirlWar> girlWar = obj;
			INTGRILWARMAP::iterator it = m_girlWarHashTable.find(girlWar->GetKey());
			if(it != m_girlWarHashTable.end())
			{
				LOG_ERROR(FILEINFO, "girlWar = %d already in sector", girlWar->GetKey());
			}

			m_girlWarHashTable[girlWar->GetKey()] = girlWar;

			if(girlWar)
				girlWar->SetSector(this);

			break;
		}
	default :
		{
			LOG_ERROR(FILEINFO, "unknown type[%d] join sector", obj->GetType());

			return -1;
		}
	}

	return 0;
}

void Sector::joinOrLeaveRegion(Smart_Ptr<Player>& player,Sector* otherSector,bool isEnter)
{
	map<int,const RegionData*>::const_iterator tOtherIt;
	map<int,const RegionData*>::const_iterator tSelfIt = m_RegionList.begin();
	bool tIsSave = false;

	for(;tSelfIt != m_RegionList.end();++tSelfIt)
	{
		tIsSave = false;
		if(otherSector != NULL)
		{
			tOtherIt = otherSector->m_RegionList.begin();
			for(;tOtherIt != otherSector->m_RegionList.end();++tOtherIt)
			{
				if(tOtherIt->first != tSelfIt->first)
				{
					continue ;
				}
				tIsSave = true;
				break ;
			}
		}

		if(tIsSave)
		{
			continue ;
		}

		if(isEnter)
		{
			player->GetRegion()->joinRegion(tSelfIt->second,this);
		}
		else
		{
			player->GetRegion()->leaveRegion(tSelfIt->second,this);
		}
	}

	return ;
}

int Sector::Leave(Smart_Ptr<Object> & obj)
{
	switch(obj->GetType())
	{
	case ePlayer :
		{
			Smart_Ptr<Player> player = obj;

			if(player->GetSector() == NULL)
				return 0;

			INT64PLAYERMAP::iterator it = m_playerHashTable.find(player->GetID());
			if(it != m_playerHashTable.end())
			{
				it->second->ResetSector();
				m_playerHashTable.erase(it);
			}
			else
			{
				LOG_ERROR(FILEINFO, "player[%lld] leave sector but not find player in sector", GET_PLAYER_CHARID(player->GetID()));
				//assert(0 && "player leave same sector");
			}

			break;
		}
	case eMonster:
		{
			Smart_Ptr<Monster> monster = obj;

			if(monster->GetSector() == NULL)
				return 0;

			INTVMPNSTERMAP::iterator it = m_monsterHashTable.find(monster->GetID());
			if(it != m_monsterHashTable.end())
			{
				vector<Smart_Ptr<Monster> >::iterator itVec = it->second.begin();
				for(; itVec!=it->second.end(); ++itVec)
				{
					if((*itVec)->GetKey() == obj->GetKey())
					{
						(*itVec)->ResetSector();
						itVec = it->second.erase(itVec);
						break;
					}
				}

				if(it->second.size() == 0)
				{
					if(monster)
						monster->ResetSector();
					m_monsterHashTable.erase(it);
				}
			}
			else
			{
				LOG_ERROR(FILEINFO, "monster[%lld] leave sector but not find monster in sector[sectorIndex=%d, sectorGroup = %d]", monster->GetID(), m_index, m_groupIndex);
				//assert(0 && "monster leave same sector");
			}

			break;
		}

	case eDropItem:
		{
			if(m_dropItem.Get() != NULL)
			{
				m_dropItem->ClearDropSectorInfo();
				m_dropItem = NULL;
			}
			break;
		}

	case ePet:
		{
			Smart_Ptr<GirlWar> girlWar = obj;

			if(girlWar->GetSector() == NULL)
				return 0;

			INTGRILWARMAP::iterator it = m_girlWarHashTable.find(girlWar->GetKey());

//			LOG_DEBUG(FILEINFO, "++++++++++++++++++++宠物离开 player id = %lld  pet id is  %d  sectorindex = %d, sectorgroup = %d，（%d, %d） \n",GET_PLAYER_CHARID(girlWar->GetOwner()->GetID()), girlWar->GetKey(), m_index, m_groupIndex,GetPosition()>>16,GetPosition()&0xffff);
			if(it != m_girlWarHashTable.end())
			{
				it->second->ResetSector();
				m_girlWarHashTable.erase(it);
			}
			else
			{
				LOG_ERROR(FILEINFO, "girlWar[%d] leave sector but not find girlWar in sector", girlWar->GetKey());
				//assert(0 && "girlWar leave same sector");
			}

			break;
		}
	default :
		{
			LOG_ERROR(FILEINFO, "unknown type[%d] Leave sector", obj->GetType());

			return -1;
		}
	}

	return 0;
}

void Sector::SendAllMonster(Smart_Ptr<Object> & obj)
{
	switch(obj->GetType())
	{
	case ePlayer :
		{
			Smart_Ptr<Player> player = obj;

			INTVMPNSTERMAP::iterator itMonsterMap = m_monsterHashTable.begin();
			for(; itMonsterMap!=m_monsterHashTable.end(); ++itMonsterMap)
			{
				vector<Smart_Ptr<Monster> >::iterator itMonsterVec = itMonsterMap->second.begin();
				for(; itMonsterVec!=itMonsterMap->second.end(); ++itMonsterVec)
				{
//					LOG_ERROR(FILEINFO,"Player::SendObjectSynch monster id is  %lld  and Quality is %d  AIState  is %d \n", (*itMonsterVec)->GetID() ,(*itMonsterVec)->GetQuality(),(*itMonsterVec)->GetAIState());

					if((*itMonsterVec)->GetAIState() == eAIStateType_Dead)
					{
						continue;
					}

					Smart_Ptr<Object> tmpObj = (*itMonsterVec);

					player->SendObjectSynch(tmpObj);
				}
			}

			break;
		}
	}
}

void Sector::SendAllDrop(Smart_Ptr<Object> & obj)
{
	switch(obj->GetType())
	{
	case ePlayer :
		{
			Smart_Ptr<Player> player = obj;

			Goods::SynToClientDrop drop;

			if((bool)m_dropItem)
			{
				Goods::DropItemInfo *dropitem = drop.add_item();

				m_dropItem->SetClientInfo(dropitem);
				player->SendMessageToGateServer(MSG_SIM_GM2GT_DROPITEM,&drop);
			}

			break;
		}
	}
}

void Sector::SendAllGirlWar(Smart_Ptr<Object> & obj)
{
	switch(obj->GetType())
	{
	case ePlayer :
		{
			Smart_Ptr<Player> player = obj;

			INTGRILWARMAP::iterator itr = m_girlWarHashTable.begin();
			for(; itr!=m_girlWarHashTable.end(); ++itr)
			{
				if(itr->second->IsDead())
				{
					continue;
				}

				Smart_Ptr<Object> tmpObj = itr->second;

				player->SendObjectSynch(tmpObj);
			}
		}
	}
}

void Sector::SendAllPlayer(Smart_Ptr<Object> & obj)
{
	switch(obj->GetType())
	{
	case ePlayer :
		{
			Smart_Ptr<Player> player = obj;

			INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
			for(; it != m_playerHashTable.end(); ++it)
			{
				if(player->GetID() == it->second->GetID())
				{
					continue;
				}

				Smart_Ptr<Object> tmpObj = it->second;

				it->second->SendObjectSynch(obj);
				player->SendObjectSynch(tmpObj);
			}

			Goods::SynToClientDrop drop;

			if((bool)m_dropItem)
			{
				Goods::DropItemInfo *dropitem = drop.add_item();

				m_dropItem->SetClientInfo(dropitem);
				player->SendMessageToGateServer(MSG_SIM_GM2GT_DROPITEM, &drop);
			}

			break;
		}
	case eMonster:
		{
			INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
			for(; it != m_playerHashTable.end(); ++it)
			{
				it->second->SendObjectSynch(obj);
			}

			break;
		}
	case ePet:
		{
			INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
			for(; it != m_playerHashTable.end(); ++it)
			{
				it->second->SendObjectSynch(obj);
			}

			break;
		}
	}
}

void Sector::SendAllObject(Smart_Ptr<Object> & obj)
{
	switch(obj->GetType())
	{
	case ePlayer :
		{
			Smart_Ptr<Player> player = obj;

			INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
			for(; it != m_playerHashTable.end(); ++it)
			{
				if(player->GetID() == it->second->GetID())
				{
					continue;
				}

				//���෢��
				Smart_Ptr<Object> tmpObj = it->second;

				it->second->SendObjectSynch(obj);
				player->SendObjectSynch(tmpObj);

				if(it->second->GetGirlWarManager() && it->second->GetGirlWarManager()->GetGirlWar())
				{
					Smart_Ptr<Object> girlwarObj = it->second->GetGirlWarManager()->GetGirlWar();
					player->SendObjectSynch(girlwarObj);
				}
			}

			INTVMPNSTERMAP::iterator itMonsterMap = m_monsterHashTable.begin();
			for(; itMonsterMap!=m_monsterHashTable.end(); ++itMonsterMap)
			{
//				FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_pZone->GetSceneID()));
//				int tPosX = m_index % info->GetXMaxNum();
//				int tPosY = m_index / info->GetXMaxNum();
				vector<Smart_Ptr<Monster> >::iterator itMonsterVec = itMonsterMap->second.begin();
//				printf("\n ( %d, %d) monster id = %lld   count = %d \n", tPosX, tPosY, itMonsterMap->first, itMonsterMap->second.size());
				for(; itMonsterVec!=itMonsterMap->second.end(); ++itMonsterVec)
				{
					if((*itMonsterVec)->GetAIState() == eAIStateType_Dead)
					{
						continue;
					}

					Smart_Ptr<Object> tmpObj = (*itMonsterVec);

					player->SendObjectSynch(tmpObj);
				}
			}

			Goods::SynToClientDrop drop;

			if((bool)m_dropItem)
			{
				Goods::DropItemInfo *dropitem = drop.add_item();

				m_dropItem->SetClientInfo(dropitem);
				player->SendMessageToGateServer(MSG_SIM_GM2GT_DROPITEM, &drop);
			}

			break;
		}
	case eMonster:
		{
			INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
			for(; it != m_playerHashTable.end(); ++it)
			{
				it->second->SendObjectSynch(obj);
			}

			break;
		}
	case eDropItem:
		{
			if(m_playerHashTable.size() <= 0)
				break;

			Smart_Ptr<DropItemsAllot> item = obj;
			Goods::SynToClientDrop drop;
			Goods::DropItemInfo *dropitem = drop.add_item();

			item->SetClientInfo(dropitem);

			INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
			for(; it != m_playerHashTable.end(); ++it)
			{
				it->second->SendMessageToGateServer(MSG_SIM_GM2GT_DROPITEM, &drop);
			}

			break;
		}
	case ePet:
		{
			INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
			for(; it != m_playerHashTable.end(); ++it)
			{
				it->second->SendObjectSynch(obj);
			}

			break;
		}
	}
}

void Sector::SendMonsterLeave(Smart_Ptr<Object> & obj)
{
	switch(obj->GetType())
	{
	case ePlayer :
		{
			Smart_Ptr<Player> player = obj;

			INTVMPNSTERMAP::iterator itMonsterMap = m_monsterHashTable.begin();
			for(; itMonsterMap!=m_monsterHashTable.end(); ++itMonsterMap)
			{
				vector<Smart_Ptr<Monster> >::iterator itMonsterVec = itMonsterMap->second.begin();
				for(; itMonsterVec!=itMonsterMap->second.end(); ++itMonsterVec)
				{
					Smart_Ptr<Object> tmpObj = (*itMonsterVec);

					player->SendObjLeave(tmpObj);
				}
			}

			break;
		}
	default :
		{
			LOG_ERROR(FILEINFO, "object leave but object type error");

			break;
		}
	}
}

void Sector::SendGirlWarLeave(Smart_Ptr<Object> & obj)
{
	switch(obj->GetType())
	{
	case ePlayer :
		{
			Smart_Ptr<Player> player = obj;

			INTGRILWARMAP::iterator itr = m_girlWarHashTable.begin();
			for(; itr!=m_girlWarHashTable.end(); ++itr)
			{
				Smart_Ptr<Object> tmpObj = itr->second;
				player->SendObjLeave(tmpObj);
			}

			break;
		}
	default :
		{
			LOG_ERROR(FILEINFO, "object leave but object type error");

			break;
		}
	}
}

void Sector::SendDropLeave(Smart_Ptr<Object> & obj)
{
	switch(obj->GetType())
	{
	case ePlayer :
		{
			Smart_Ptr<Player> player = obj;

			if((bool)m_dropItem)
			{
				Goods::DeleteDropItem deleteitem;

				deleteitem.add_id(m_dropItem->GetKey());
				QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &deleteitem, MSG_SIM_GT2C_DELETEDROP);
			}

			break;
		}
	default :
		{
			LOG_ERROR(FILEINFO, "object leave but object type error");

			break;
		}
	}
}

void Sector::SendPlayerLeave(Smart_Ptr<Object> & obj, bool side)
{
	switch(obj->GetType())
	{
	case ePlayer :
		{
			Smart_Ptr<Player> player = obj;

			INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
			for(; it != m_playerHashTable.end(); ++it)
			{
				if(player->GetID() == it->second->GetID())
				{
					continue;
				}

				it->second->SendObjLeave(obj);
				if(side)
				{
					Smart_Ptr<Object> tmpObj = it->second;

					player->SendObjLeave(tmpObj);
				}
			}

			if((bool)m_dropItem)
			{
				Goods::DeleteDropItem deleteitem;

				deleteitem.add_id(m_dropItem->GetKey());
				QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &deleteitem, MSG_SIM_GT2C_DELETEDROP);
			}

			break;
		}
	case eMonster:
		{
			INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
			for(; it != m_playerHashTable.end(); ++it)
			{
				it->second->SendObjLeave(obj);
			}

			break;
		}
	case ePet:
		{
			INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
			for(; it != m_playerHashTable.end(); ++it)
			{
				it->second->SendObjLeave(obj);
			}

			break;
		}
	default :
		{
			LOG_ERROR(FILEINFO, "object leave but object type error");

			break;
		}
	}
}

void Sector::SendObjectLeave(Smart_Ptr<Object> & obj, bool side)
{
	switch(obj->GetType())
	{
	case ePlayer :
		{
			Smart_Ptr<Player> player = obj;

			INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
			for(; it != m_playerHashTable.end(); ++it)
			{
				if(player->GetID() == it->second->GetID())
				{
					continue;
				}

				//���෢��
				it->second->SendObjLeave(obj);

				if(player->GetGirlWarManager() && player->GetGirlWarManager()->GetGirlWar())
				{
					Smart_Ptr<Object> girlwarObj = player->GetGirlWarManager()->GetGirlWar();
					it->second->SendObjLeave(girlwarObj);
				}

				if(side)
				{
					Smart_Ptr<Object> tmpObj = it->second;

					player->SendObjLeave(tmpObj);

					if(it->second->GetGirlWarManager() && it->second->GetGirlWarManager()->GetGirlWar())
					{
						Smart_Ptr<Object> girlwarObj = it->second->GetGirlWarManager()->GetGirlWar();
						player->SendObjLeave(girlwarObj);
					}
				}
			}

			INTVMPNSTERMAP::iterator itMonsterMap = m_monsterHashTable.begin();
			for(; itMonsterMap!=m_monsterHashTable.end(); ++itMonsterMap)
			{
				vector<Smart_Ptr<Monster> >::iterator itMonsterVec = itMonsterMap->second.begin();
				for(; itMonsterVec!=itMonsterMap->second.end(); ++itMonsterVec)
				{
					Smart_Ptr<Object> tmpObj = (*itMonsterVec);

					player->SendObjLeave(tmpObj);
				}
			}

			if((bool)m_dropItem)
			{
				Goods::DeleteDropItem deleteitem;

				deleteitem.add_id(m_dropItem->GetKey());
				QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &deleteitem, MSG_SIM_GT2C_DELETEDROP);
			}

			break;
		}
	case eMonster:
		{
			INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
			for(; it != m_playerHashTable.end(); ++it)
			{
			//printf("\n--------------------------------�����Ƴ�\n");
				it->second->SendObjLeave(obj);
			}

			break;
		}
	case eDropItem:
		{
//			Goods::ToGTDeleteDrop dropitem;
			Goods::DeleteDropItem deleteitem;

			deleteitem.add_id(obj->GetKey());
			INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
			for(; it != m_playerHashTable.end(); ++it)
			{
//				dropitem.set_charid(it->second->GetID());
				QuickMessage::GetInstance()->AddSendMessage(it->second->GetID(),it->second->GetChannelID(), &deleteitem, MSG_SIM_GT2C_DELETEDROP);
//				it->second->SendMessageToGateServer(MSG_SIM_GM2GT_DELETEDROP, &dropitem);
			}

			break;
		}
	case ePet:
		{
			INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
			for(; it != m_playerHashTable.end(); ++it)
			{
				it->second->SendObjLeave(obj);
			}

			break;
		}
	default :
		{
			LOG_ERROR(FILEINFO, "object leave but object type error");

			break;
		}
	}
}

void Sector::SendSynchCommon(::google::protobuf::Message *from,int messageid,int key, bool isMyself )
{
	//Smart_Ptr<Player> player;
	INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
	for(; it != m_playerHashTable.end(); ++it)
	{
		if(!isMyself)
		{
			if(it->second->GetKey() == key)
			{
				continue;
			}
		}

		it->second->SendSynchCommon(from,messageid);
	}
}

void Sector::SendSynchMessage(::google::protobuf::Message *from, int msgid, int key, bool isMyself)
{
	INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
	for(; it != m_playerHashTable.end(); ++it)
	{
		if(!isMyself)
		{
			if(it->second->GetKey() == key)
			{
				continue;
			}
		}

		it->second->SendSynchMessage(from,msgid);
	}
}


void Sector::SendSynchPath(PlayerSynch::PathToAllPlayer &path)
{
	int64 charid = path.charid();
	INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
	for(; it != m_playerHashTable.end(); ++it)
	{
		if(charid == it->second->GetKey())
		{
			continue;
		}

		//����
		it->second->SendSynchPath(path);
	}
}

void Sector::SendSynchPath(PlayerSynch::MonsterPathToPlayer &path)
{
	INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
	for(; it != m_playerHashTable.end(); ++it)
	{
		it->second->SendSynchPath(path);
	}
}

void Sector::SendSynchPath(PlayerSynch::GirlWarPathToPlayer &path)
{
	INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
	for(; it != m_playerHashTable.end(); ++it)
	{
		it->second->SendSynchPath(path);
	}
}

void Sector::SendSynchAttr(CharBase::SynchObjectAttr &attr, bool isMyself)
{
	INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
	for(; it != m_playerHashTable.end(); ++it)
	{
		if(!isMyself)
		{
			if(it->second->GetKey() == attr.key())
			{
				continue;
			}
		}

		it->second->SendSynchAttr(attr);
	}
}

void Sector::SendSynBuffChange(CharBattle::BuffOperate *buf, int key)
{
	INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
	for(; it != m_playerHashTable.end(); ++it)
	{
		if(it->second->GetKey() == key)
		{
			continue;
		}

		it->second->SendBuffState(buf);
	}
}

//
//void Sector::SendSynchAttrState(CharBattle::ObjectState &attr)
//{
//	map<int64, Smart_Ptr<Player> >::iterator it = m_playerHashTable.begin();
//	for(; it != m_playerHashTable.end(); ++it)
//	{
////		if(it->second->GetKey() == attr.key())
////		{
////			continue;
////		}
//
//		it->second->SendSynchAttr(attr);
//	}
//}

void Sector::SendSynchAction(CharBattle::SynClientAttack &action)
{
	INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
	for(; it != m_playerHashTable.end(); ++it)
	{
		it->second->SendSynchAction(action);
	}
}

void Sector::GetCreatrueObj(vector<Smart_Ptr<CreatureObj> > &obj)
{
	INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
	for(; it != m_playerHashTable.end(); ++it)
	{
//		vector<Smart_Ptr<CreatureObj> >::iterator itVec = obj.begin();
//		for(; itVec!=obj.end(); ++itVec)
//		{
//			if((*itVec)->GetKey() == it->second->GetKey())
//				break;
//		}
//
//		if(itVec == obj.end())
			obj.push_back(it->second);
	}

	INTVMPNSTERMAP::iterator itMonsterMap = m_monsterHashTable.begin();
	for(; itMonsterMap!=m_monsterHashTable.end(); ++itMonsterMap)
	{
		vector<Smart_Ptr<Monster> >::iterator itMonsterVec = itMonsterMap->second.begin();
		for(; itMonsterVec!=itMonsterMap->second.end(); ++itMonsterVec)
		{
//			vector<Smart_Ptr<CreatureObj> >::iterator itVec = obj.begin();
//			for(; itVec!=obj.end(); ++itVec)
//			{
//				if((*itVec)->GetKey() == (*itMonsterVec)->GetKey())
//					break;
//			}
//
//			if(itVec == obj.end())
			if((*itMonsterVec)->GetQuality() != eGatherMonster && (*itMonsterVec)->GetQuality() != eElementMonster && (*itMonsterVec)->GetQuality() != eBoxMonster
			&& (*itMonsterVec)->GetQuality() != ePVPGatherMonster && (*itMonsterVec)->GetQuality() != eFestivalMonster && (*itMonsterVec)->GetQuality() != eBatterFlag
			&& (*itMonsterVec)->GetQuality() != eChallengeBox)
				obj.push_back(*itMonsterVec);
		}
	}

	INTGRILWARMAP::iterator itGrilWar = m_girlWarHashTable.begin();
	for(;itGrilWar != m_girlWarHashTable.end(); ++itGrilWar)
	{
		obj.push_back(itGrilWar->second);
	}
}

void Sector::GetCreatrueObj(int country, eObjType eType, vector<Smart_Ptr<CreatureObj> > &obj)
{
	INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
	for(; it != m_playerHashTable.end(); ++it)
	{
		if(it->second->getAttManage()->getValue<DWORD>(eCharCountry) != (DWORD)country && !it->second->IsDead() && it->second->IsInMap())
			obj.push_back(it->second);
	}

	INTVMPNSTERMAP::iterator itMonsterMap = m_monsterHashTable.begin();
	for(; itMonsterMap!=m_monsterHashTable.end(); ++itMonsterMap)
	{
		vector<Smart_Ptr<Monster> >::iterator itMonsterVec = itMonsterMap->second.begin();
		for(; itMonsterVec!=itMonsterMap->second.end(); ++itMonsterVec)
		{
			if((*itMonsterVec)->getAttManage()->getValue<DWORD>(eCharCountry) != (DWORD)country && !(*itMonsterVec)->IsDead() && (*itMonsterVec)->IsInMap())
			{
				if((*itMonsterVec)->GetQuality() == eEscortCar)
				{
					Smart_Ptr<Player> player = (*itMonsterVec)->GetOwner();

					if(player && player->GetEscortMgr()->GetRobTimes() >= 5)
						continue;
				}

				obj.push_back(*itMonsterVec);
			}
		}
	}

	if(eType != eMonster)
	{
		INTGRILWARMAP::iterator itGrilWar = m_girlWarHashTable.begin();
		for(;itGrilWar != m_girlWarHashTable.end(); ++itGrilWar)
		{
			if(itGrilWar->second->getAttManage()->getValue<DWORD>(eCharCountry) != (DWORD)country && !itGrilWar->second->IsDead() && itGrilWar->second->IsInMap())
				obj.push_back(itGrilWar->second);
		}
	}
}

void Sector::GetPlayer(int country, vector<Smart_Ptr<CreatureObj> > &obj)
{
	INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
	for(; it != m_playerHashTable.end(); ++it)
	{
		if(it->second->getAttManage()->getValue<DWORD>(eCharCountry) != (DWORD)country)
			obj.push_back(it->second);
	}
}

void Sector::GetPlayer(vector<Smart_Ptr<Player> > &obj)
{
	INT64PLAYERMAP::iterator it = m_playerHashTable.begin();
	for(; it != m_playerHashTable.end(); ++it)
	{
		obj.push_back(it->second);
	}
}

DWORD Sector::GetPosition()
{
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_pZone->GetSceneID()));
	DWORD pos = 0;
	int tPosX = m_index % info->GetXMaxNum();
	int tPosY = m_index / info->GetXMaxNum();

	//cout<<endl<<"m_index is "<<m_index<<" tPosX "<<tPosX<<" tPosY "<<tPosY<<endl;

	pos = (tPosX << 16) | tPosY;

	return pos;
}

Smart_Ptr<Monster> Sector::GetSectorMonster(int64 id,int64 key)
{
	INTVMPNSTERMAP::iterator itMonsterMap = m_monsterHashTable.find(id);
	if(itMonsterMap!=m_monsterHashTable.end())
	{
		vector<Smart_Ptr<Monster> >::iterator itMonsterVec = itMonsterMap->second.begin();
		for(; itMonsterVec!=itMonsterMap->second.end(); ++itMonsterVec)
		{
			if((*itMonsterVec)->GetKey() == key)
				return *itMonsterVec;
		}
	}

	return NULL;
}

Smart_Ptr<Player> Sector::GetSectorPlayer(int64 id)
{
	INT64PLAYERMAP::iterator it = m_playerHashTable.find(id);
	if(it!=m_playerHashTable.end())
	{
		return it->second;
	}

	return NULL;
}

bool Sector::isCanDoing(eObjectDoing doing)
{
	map<int,const RegionData*>::iterator tIt = m_RegionList.begin();
	for(;tIt != m_RegionList.end();++tIt)
	{
		for(size_t i = 0;i < (*tIt->second).m_DoingLimit.size();++i)
		{
			if((*tIt->second).m_DoingLimit[i] == doing)
			{
				return false;
			}
		}
	}

	return true;
}

bool Sector::isCanAddState(EAttackState state)
{
	map<int,const RegionData*>::iterator tIt = m_RegionList.begin();
	for(;tIt != m_RegionList.end();++tIt)
	{
		for(size_t i = 0;i < (*tIt->second).m_StateLimit.size();++i)
		{
			if((*tIt->second).m_StateLimit[i] == state)
			{
				return false;
			}
		}
	}

	return true;
}










