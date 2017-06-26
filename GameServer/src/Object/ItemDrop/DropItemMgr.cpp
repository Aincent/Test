/*
 * DropItemMgr.cpp
 *
 *  Created on: 2014年4月22日
 *      Author: helh
 */

#include "DropItemMgr.h"
#include "Log/Logger.h"
#include "Network/Message_Facade.h"
#include "../../MessageBuild.h"
#include "../../ServerManager.h"
#include "../ObjectFactory.h"
#include "../FileLoader/MonsterLoader.h"
#include "../../Map/SceneManager.h"
#include "../Player/PlayerMessage.h"
#include "MessageCommonRet.h"
#include "Timer/TimerInterface.h"
#include "../FileLoader/DropItemLoader.h"
#include "util.h"
#include "../../Chat/ChatMgr.h"
#include "../../Group/NewGroupMgr.h"
#include "../../ServerEventDefine.h"
#include "../../Festival/FestivalActive.h"
#include "../../StatisticMgr/StatisticHandle.h"

DropItemsManager::DropItemsManager()
{
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_PICKITEM);

		//注册定时刷新掉落定时器
	if(m_timerID.IsValid())
	{
		m_timerID.SetNull();
	}
	m_timerID = TimerInterface::GetInstance()->RegRepeatedTime(&DropItemsManager::HandleDeleteDrop, this, 0, 0, 0, 0, 60 * 1000, 60 * 1000);
	m_monsterKilledEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this,&DropItemsManager::HandleBattleDeath);
}

DropItemsManager::~DropItemsManager()
{
	if(m_timerID.IsValid())
	{
		m_timerID.SetNull();
	}

	if(m_monsterKilledEvent.IsValid())
		m_monsterKilledEvent.SetNull();
}


void DropItemsManager::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_SWITCH_TRY_DISPATCH_END
}

void DropItemsManager::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_PICKITEM);

	DEF_SWITCH_TRY_DISPATCH_END
}

bool DropItemsManager::GetDropPropBymonster(int monster, vector<DropStaticInfo>& item)
{
	MonsterInfo info;
	MonsterLoader::GetInstance()->GetMonsterInfo(monster, info);
	if(info.id <= 0)
	{
		LOG_ERROR(FILEINFO, "GetDropPropBymonster: drop allot but monster id is error");
		return false;
	}

	DropItemLoader::GetInstance()->GetDropItem(info.fallID, item);
	if(item.size() <= 0)
	{
		return false;
	}

	return true;
}

void DropItemsManager::DropItemWhoOwn(Smart_Ptr<DropItemsAllot> &pAllot, const int &itemID,const Smart_Ptr<Player> &owner)
{
	if(owner->IsHaveGroup())
	{
		Smart_Ptr<GroupInfo> group_ptr = NULL;
		sNewGroupMgr->GetGroupPtr(owner->GetGroupID(), group_ptr);
		if(!group_ptr)
		{
			return;
		}

		list<GroupMember *>::iterator it = group_ptr->memberList.begin();
		for(; it != group_ptr->memberList.end(); ++it)
		{
			if(*it==NULL)
				continue;

			Smart_Ptr<Player> mem_player = NULL;
			if(sNewGroupMgr->IsSameMapGroupMember(GET_MAP_ID(owner->GetMapID()), (*it)->memberID, mem_player))
			{
				pAllot->AddOwner((*it)->memberID);
			}
		}
	}
	else
	{
		pAllot->AddOwner(owner->GetID());
	}
}

//节日活动额外掉落
void DropItemsManager::festivalActiveDrop(const Smart_Ptr<Monster> & attacked,const Smart_Ptr<Player> &owner,int propID,int64 time)
{
	Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eDropItem);
	Smart_Ptr<DropItemsAllot> pAllot = obj;

	//添加队伍的归属
	DropItemWhoOwn(pAllot, attacked->GetID(), owner);

	pAllot->SetMapID(attacked->GetMapID());
	pAllot->SetXPos(attacked->GetXNewPos());
	pAllot->SetYPos(attacked->GetYNewPos());

	//要掉落的东西在此设置进去
	if(!pAllot->Init(propID,1,time))
	{
		//pAllot->Release();
		//ObjectFactory::GetInstance()->Recycle(obj);
		return;
	}

	m_dropItemsList[pAllot->GetKey()] = pAllot;

	//同步到客户端
	CSceneMgr::GetInstance()->EnterScene(obj);
}

void DropItemsManager::HandleBattleDeath(const Smart_Ptr<Monster> &attacked, const Smart_Ptr<Player> &owner)
{
	 //不掉落物品的怪物
	if(!MonsterLoader::GetInstance()->IsDropItem(attacked->GetID()))
		return ;

	//先判断掉不掉物品，掉的话才会取对象(否则频繁的获取加锁对象降低效率)
	vector<DropStaticInfo> items;
	if(!GetDropPropBymonster(attacked->GetID(), items))
	{
		return;
	}

	Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eDropItem);
	Smart_Ptr<DropItemsAllot> pAllot = obj;

	//添加队伍的归属
	this->DropItemWhoOwn(pAllot, attacked->GetID(), owner);

	pAllot->SetMapID(attacked->GetMapID());
	pAllot->SetXPos(attacked->GetXNewPos());
	pAllot->SetYPos(attacked->GetYNewPos());

	//要掉落的东西在此设置进去
	if(!pAllot->Init(attacked->GetID(), items))
	{
		//pAllot->Release();
		//ObjectFactory::GetInstance()->Recycle(obj);
		return;
	}

	m_dropItemsList[pAllot->GetKey()] = pAllot;

	// 是世界BOSS 设置是Boss掉落的
	if (eWorldMonster == attacked->GetQuality())
	{
		pAllot->SetWhoDrop(attacked->GetID());
	}
	else
	{
		//掉落好装备广播跑马灯
		vector<int> marquee = pAllot->GetMarqueeEquipId();
		for(BYTE i = 0 ; i < marquee.size() ; i++)
		{
			string strM = ChatMgr::GetInstance()->GetStringByInt(attacked->GetID());
			string strE = ChatMgr::GetInstance()->GetStringByInt(marquee[i]);
			ChatMgr::GetInstance()->Marquee(eMarqueeDrop,owner->GetName(),strM,strE);
		}
	}
	//同步到客户端
	CSceneMgr::GetInstance()->EnterScene(obj);
}

void DropItemsManager::HandleBattleDeath(Smart_Ptr<Player> & attacked, std::vector<PropBase> &dropList,Smart_Ptr<CreatureObj> & creatureObj)
{
	if(dropList.size() == 0 || !attacked)
		return;

	Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eDropItem);
	Smart_Ptr<DropItemsAllot> pAllot = obj;
	if(creatureObj->GetType() == ePlayer)
	{
		//添加队伍的归属
		Smart_Ptr<Player> player = creatureObj;

		this->DropItemWhoOwn(pAllot, attacked->GetID(), player);
	}

	pAllot->SetMapID(attacked->GetMapID());
	pAllot->SetXPos(attacked->GetXNewPos());
	pAllot->SetYPos(attacked->GetYNewPos());

	if(!pAllot->Init(dropList))
	{
		//pAllot->Release();
		//ObjectFactory::GetInstance()->Recycle(obj);
		return;
	}

	//掉落好装备广播跑马灯
	vector<int> marquee = pAllot->GetMarqueeEquipId();
	for(BYTE i = 0 ; i < marquee.size() ; i++)
	{
		string strM = ChatMgr::GetInstance()->GetStringByInt(attacked->GetID());
		string strE = ChatMgr::GetInstance()->GetStringByInt(marquee[i]);
		ChatMgr::GetInstance()->Marquee(eMarqueeDrop,attacked->GetName(),strM,strE);
	}
	m_dropItemsList[pAllot->GetKey()] = pAllot;
	//同步到客户端
	CSceneMgr::GetInstance()->EnterScene(obj);
}

void DropItemsManager::HandleBattleDeath(map<DWORD,DWORD>& goodsInfo,map<int64, Smart_Ptr<Player> >& owners)
{
	Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eDropItem);
	Smart_Ptr<DropItemsAllot> pAllot = obj;

	map<int64, Smart_Ptr<Player> >::iterator it = owners.begin();
	for(; it != owners.end(); ++it)
	{
		pAllot->AddOwner(it->first);
	}

	//要掉落的东西在此设置进去
	map<DWORD,DWORD>::iterator itr = goodsInfo.begin();

	for(; itr != goodsInfo.end(); ++itr)
	{
		if(!pAllot->Init(itr->first,itr->second))
		{
			//pAllot->Release();
			//ObjectFactory::GetInstance()->Recycle(obj);
			return;
		}
	}

	m_dropItemsList[pAllot->GetKey()] = pAllot;

	//同步到客户端
	CSceneMgr::GetInstance()->EnterScene(obj);
}

void DropItemsManager::HandleBattleDeath(map<DWORD,DWORD>& goodsInfo,vector<int64>& owers,const Smart_Ptr<Player> &owner)
{
	Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eDropItem);
	Smart_Ptr<DropItemsAllot> pAllot = obj;

	vector<int64>::iterator it = owers.begin();
	for(; it != owers.end(); ++it)
	{
		pAllot->AddOwner(*it);
	}

	pAllot->SetMapID(owner->GetMapID());
	pAllot->SetXPos(owner->GetXNewPos());
	pAllot->SetYPos(owner->GetYNewPos());
	pAllot->SetGroup(CSceneMgr::GetInstance()->GetSectorGroup(owner->GetMapID(), owner->GetXNewPos(), owner->GetYNewPos()));

	//要掉落的东西在此设置进去
	map<DWORD,DWORD>::iterator itr = goodsInfo.begin();

	for(; itr != goodsInfo.end(); ++itr)
	{
		if(!pAllot->Init(itr->first,itr->second))
		{
			//pAllot->Release();
			//ObjectFactory::GetInstance()->Recycle(obj);
			return;
		}
	}

	m_dropItemsList[pAllot->GetKey()] = pAllot;

	//同步到客户端
	CSceneMgr::GetInstance()->EnterScene(obj);
}

void DropItemsManager::dropByparam(Smart_Ptr<CGameZone> zone,const vector<DWORD>& goodID,const vector<DWORD>& count,const vector<WORD>& flag,int posX,int posY)
{	
	Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eDropItem);
	Smart_Ptr<DropItemsAllot> pAllot = obj;

	pAllot->SetMapID(zone->GetSceneID());
	pAllot->SetXPos(posX);
	pAllot->SetYPos(posY);
	pAllot->SetGroup(CSceneMgr::GetInstance()->GetSectorGroup(zone->GetSceneID(), posX, posY));

	if(!pAllot->InitByParam(goodID,count,flag))
	{
		return;
	}

	m_dropItemsList[pAllot->GetKey()] = pAllot;

	//同步到客户端
	CSceneMgr::GetInstance()->EnterScene(obj);

	return ;
}

void DropItemsManager::HandleDeleteDrop(void *arg)
{
	HandleDeleteDrop();
}

void DropItemsManager::HandleDeleteDrop()
{
	int64 nowTime = CUtil::GetNowSecond();
	int64 deleteTime = DropItemLoader::GetInstance()->GetDropDeleteTime();

	DropItemsMap::iterator it = m_dropItemsList.begin();
	for(; it!=m_dropItemsList.end(); )
	{
		if((it->second->GetCreateTime() + deleteTime) <= nowTime)
		{
			Smart_Ptr<Object> obj = it->second;

			//广播尸体盒删除消息
			CSceneMgr::GetInstance()->LeaveScene(obj);

			m_dropItemsList.erase(it++);

		}
		else
		{
			++it;
		}
	}


}

DEF_MSG_REQUEST_DEFINE_FUN(DropItemsManager, MSG_REQ_GT2GM_PICKITEM)
{
	Goods::PickUpItem retItem;
	Goods::SynPickUpItem newItem;
	int len = 0;
	char *content = message->GetBuffer(len);
	Smart_Ptr<Player> player;

	retItem.ParseFromArray(content, len);

	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{
		int64 mapid = -1;
		int xpos = -1;
		int ypos = -1;

		BEHAVIOR_MONI(player, BehaviorType::Main::ePackage, BehaviorType::Minor::ePackage_PickItem, MONITOR);

		for(int i=0; i<retItem.item_size(); ++i)
		{
			Goods::PickedItem item = retItem.item(i);
			DropItemsMap::iterator it = m_dropItemsList.find(item.id());
			if(it != m_dropItemsList.end())
			{
				int ret = it->second->PickUpItem(player, item.onlyid());
				if(ret == 0)
				{//
					if(mapid <= 0)
					{
						mapid = it->second->GetMapID();
						xpos = it->second->GetXPos();
						ypos = it->second->GetYPos();
					}

					Goods::PickedItem *single = newItem.add_item();

					single->CopyFrom(item);

				}
				else if(ret == 1)
				{//尸体盒中物品为零了
					Smart_Ptr<Object> obj = it->second;

					//广播尸体盒删除消息
					CSceneMgr::GetInstance()->LeaveScene(obj);

					//物品空了刪除尸体盒
					m_dropItemsList.erase(it);
				}
				else if(ret == 2)
				{//背包已经满了
					newItem.set_ret(ePackageFull);
				}
			}
			else
			{
				LOG_ERROR(FILEINFO, "drop item is error");
			}
		}

		

		if(newItem.has_ret() || newItem.item_size() > 0)
		{
			Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_PICKITEM, message, &newItem);
			Message_Facade::Send(messRet);
		}

		if(newItem.item_size() > 0)
		{
			//广播给旁边的玩家, int msgid, int64 sceneid, int key, int xpos, int ypos, bool isMyself = false
			CSceneMgr::GetInstance()->SendSynchMessage(&newItem, MSG_SIM_GT2C_DELETEITEM, mapid, player->GetKey(), xpos, ypos);
		}
	}
}

bool DropItemsManager::HandleBattleDeath(const EventArgs& e)
{
	TwoParmaEventArgs* data = (TwoParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	if(data->m_other->GetType() != ePlayer && data->m_other->GetType() != ePet)
		return true;

	Smart_Ptr<Player>  player;
	Smart_Ptr<GirlWar>  girlwar;

	if(data->m_other->GetType() == ePet)
	{
		girlwar = data->m_other;

		player  = girlwar->GetOwner();
	}
	else
	{
		player = data->m_other;
	}

	Smart_Ptr<Monster> mon = data->m_myself;
	if(player && mon && mon->GetQuality() != eGatherMonster && mon->GetQuality() != ePVPGatherMonster && mon->GetQuality() != eChallengeBox)
	{
		HandleBattleDeath(mon,player);
	}
	return true;
}
