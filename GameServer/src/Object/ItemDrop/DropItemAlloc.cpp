/*
 * DropItemAlloc.cpp
 *
 *  Created on: 2014年4月22日
 *      Author: helh
 */

#include "DropItemAlloc.h"
#include "Log/Logger.h"
#include "../FileLoader/MonsterLoader.h"
#include "../FileLoader/DropItemLoader.h"
#include "FileLoader/GoodsLoader.h"
#include "../../Map/SceneManager.h"
#include "FileLoader/GameParaLoader.h"
#include "FightDefined.h"
#include <math.h>
#include "StatisticBase/StatisticDefine.h"
#include "../../StatisticMgr/StatisticMgr.h"
#include "MessageStruct/StatisticInfo.pb.h"

void DropItemsAllot::Release()
{
	Object::Release();
	m_flag = 0;
	m_bossID = 0;
	m_marqueeEquipId.clear();
	m_owerList.clear();
	m_dropItemMap.clear();
}

bool DropItemsAllot::Init(int64 id, const vector<DropStaticInfo>& items)
{
	m_createTime = CUtil::GetNowSecond();

	vector<DropStaticInfo>::const_iterator it = items.begin();
	for(; it!=items.end(); ++it)
	{
		DropItem tItem;
		const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(it->m_ItemID);
		if(ginfo == NULL)
		{
			LOG_ERROR(FILEINFO, "Drop list item[id = %d] is error", (*it).m_ItemID);
			return false;
		}

		switch(ginfo->type)
		{
		case eWeapon:
		case eArmor:
		case eJewelry:
		{
			Smart_Ptr<EquipProp> tProp;

			tProp = new EquipProp(ginfo->itemID);
			tProp->Init();
			tItem.data = tProp;

			break;
		}
		case eTime:
		{
			int64 time=0;
			if(ginfo->valideSecs>0)
			{
					time = (int64)((CUtil::GetNowSecond()+(DWORD64)ginfo->valideSecs)*0.001);
			}
			tItem.data = new TimeProp(ginfo->itemID,1,time);

			break;
		}
		default :
		{
			tItem.data = new PropBase(ginfo->itemID);
		}
		}

		tItem.data->CreateItem(*ginfo);
		tItem.data->SetFlag(it->m_BindType);
		tItem.flag  = ++m_flag;
		tItem.count = it->m_Counts;

		m_dropItemMap[tItem.flag] = tItem;
		if(ginfo->needToBroadCast == true)
		{
			m_marqueeEquipId.push_back(it->m_ItemID);
		
		}
	}
	if(!AverageAlloc())
	{
		LOG_ERROR(FILEINFO, "Average alloc item error");

		return false;
	}

	return true;
}

bool DropItemsAllot::Init(std::vector<PropBase> propList)
{
	if(propList.size() == 0)
	{
		LOG_ERROR(FILEINFO, "Have not item drop");
		return false;
	}

	m_createTime = CUtil::GetNowSecond();

	for(size_t i = 0;i < propList.size();++i)
	{
		DropItem tItem;
		int goodID = propList[i].GetID();
		const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodID);
		if(ginfo==NULL)
		{
			LOG_ERROR(FILEINFO, "Drop list item[id = %d] is error",goodID);
			return false;
		}

		switch(ginfo->type)
		{
			case eWeapon:
			case eArmor:
			case eJewelry:
			{
				Smart_Ptr<EquipProp> tProp;

				tProp = new EquipProp(ginfo->itemID);
//				tProp->Init(ginfo.inlayNum);
				tProp->Init();
				tItem.data = tProp;

				break;
			}
			case eTime:
			{
				int64 time=0;
				if(ginfo->valideSecs>0)
				{
						time = (int64)((CUtil::GetNowSecond()+(DWORD64)ginfo->valideSecs)*0.001);
				}
				tItem.data = new TimeProp(ginfo->itemID,1,time);

				break;
			}
			default :
			{
				tItem.data = new PropBase(ginfo->itemID);
			}
		}
		tItem.data->CreateItem(*ginfo);
		tItem.flag = ++m_flag;
		tItem.count = propList[i].GetAmount();
		m_dropItemMap[tItem.flag] = tItem;
		if(ginfo->needToBroadCast == true)
		{
			m_marqueeEquipId.push_back(goodID);
		
		}
	}
	if(!AverageAlloc())
	{
		LOG_ERROR(FILEINFO, "Average alloc item error");

		return false;
	}

	return true;
}

bool DropItemsAllot::Init(int64 id,DWORD multiple,DWORD rate)
{
	MonsterInfo info;
	vector<DropStaticInfo> item;

	MonsterLoader::GetInstance()->GetMonsterInfo(id, info);
	if(info.id <= 0)
	{
		LOG_ERROR(FILEINFO, "drop allot but monster id is error");

		return false;
	}

	DropItemLoader::GetInstance()->GetDropItem(info.fallID,item);
	if(item.size() <= 0)
	{//没有掉落
		return false;
	}

	m_createTime = CUtil::GetNowSecond();

	vector<DropStaticInfo>::iterator it = item.begin();
	for(; it!=item.end(); ++it)
	{
		DropItem tItem;
		const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID((*it).m_ItemID);
		if(ginfo == NULL)
		{
			LOG_ERROR(FILEINFO, "Drop list item[id = %d] is error", (*it).m_ItemID);
			continue;
		}

		switch(ginfo->type)
		{
		case eWeapon:
		case eArmor:
		case eJewelry:
		{
			Smart_Ptr<EquipProp> tProp;

			tProp = new EquipProp(ginfo->itemID);
			tProp->Init();
			tItem.data = tProp;

			break;
		}
		case eTime:
		{
			int64 time=0;
			if(ginfo->valideSecs>0)
			{
					time = (int64)((CUtil::GetNowSecond()+(DWORD64)ginfo->valideSecs)*0.001);
			}
			tItem.data = new TimeProp(ginfo->itemID,1,time);

			break;
		}
		default :
		{
			tItem.data = new PropBase(ginfo->itemID);
		}
		}

		tItem.data->CreateItem(*ginfo);
		tItem.flag = ++m_flag;
		tItem.count = (*it).m_Counts;
		tItem.data->SetFlag(it->m_BindType);
		m_dropItemMap[tItem.flag] = tItem;
		if(ginfo->needToBroadCast == true)
		{
			m_marqueeEquipId.push_back((*it).m_ItemID);
		}
	}
	if(!AverageAlloc())
	{
		LOG_ERROR(FILEINFO, "Average alloc item error");

		return false;
	}

	return true;
}

bool DropItemsAllot::Init(DWORD goodID,DWORD count,int64 time)
{
	m_createTime = CUtil::GetNowSecond();

	for(size_t i = 0;i < count;++i)
	{
		DropItem tItem;
		const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodID);
		if(ginfo == NULL)
		{
			LOG_ERROR(FILEINFO, "Drop list item[id = %d] is error",goodID);
			return false;
		}

		switch(ginfo->type)
		{
			case eWeapon:
			case eArmor:
			case eJewelry:
			{
				Smart_Ptr<EquipProp> tProp;

				tProp = new EquipProp(ginfo->itemID);
				tProp->Init();
				tItem.data = tProp;

				break;
			}
			case eTime:
			{
				tItem.data = new TimeProp(ginfo->itemID,1,time);
				break;
			}
			default :
			{
				tItem.data = new PropBase(ginfo->itemID);
			}
		}
		tItem.data->CreateItem(*ginfo);
		tItem.flag = ++m_flag;
		tItem.count = 1;
		m_dropItemMap[tItem.flag] = tItem;
		if(ginfo->needToBroadCast == true)
		{
			m_marqueeEquipId.push_back(goodID);

		}
	}

	if(!AverageAlloc())
	{
		LOG_ERROR(FILEINFO, "Average alloc item error");

		return false;
	}

	return true;
}

bool DropItemsAllot::InitByParam(const vector<DWORD>& goodID,const vector<DWORD>& count,const vector<WORD>& flag)
{
	if(goodID.size() != count.size() || flag.size() != count.size() || goodID.size() == 0)
	{
		return false;
	}

	for(size_t i = 0;i < goodID.size();++i)
	{
		DropItem tItem;
		const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodID[i]);
		if(ginfo == NULL)
		{
			LOG_ERROR(FILEINFO, "Drop list item[id = %d] is error",goodID[i]);
			return false;
		}
		switch(ginfo->type)
		{
			case eWeapon:
			case eArmor:
			case eJewelry:
			{
				Smart_Ptr<EquipProp> tProp;

				tProp = new EquipProp(ginfo->itemID);
				tProp->Init();
				tItem.data = tProp;

				break;
			}
			case eTime:
			{
				int64 time=0;
				if(ginfo->valideSecs>0)
				{
					time = (int64)((CUtil::GetNowSecond()+(DWORD64)ginfo->valideSecs)*0.001);
				}
				tItem.data = new TimeProp(ginfo->itemID,1,time);
				break;
			}
			default :
			{
				tItem.data = new PropBase(ginfo->itemID);
			}
		}
		tItem.data->CreateItem(*ginfo);
		tItem.flag = ++m_flag;
		tItem.count = count[i];
		tItem.data->SetBind(flag[i]);
		m_dropItemMap[tItem.flag] = tItem;
		if(ginfo->needToBroadCast == true)
		{
			m_marqueeEquipId.push_back(goodID[i]);

		}
	}

	if(!AverageAlloc())
	{
		LOG_ERROR(FILEINFO, "Average alloc item error");

		return false;
	}

	return true;
}

bool DropItemsAllot::AverageAlloc()
{
	vector<Sector *> vec;
	int i=0;

	CSceneMgr::GetInstance()->GetAroundPos(GetMapID(), GetXPos(), GetYPos(), m_dropItemMap.size(), vec);
	if(vec.size() <= 0)
	{
		LOG_ERROR(FILEINFO, "get around position error");

		return false;
	}

	map<BYTE, DropItem >::iterator it = m_dropItemMap.begin();
	for(; it!=m_dropItemMap.end(); ++it, ++i)
	{
		i = i % vec.size();

		if(vec[i] == NULL)
			continue;

		it->second.sector = vec[i];
		it->second.sector->SetHaveItem(true);
	}

	return true;
}

//清除尸体盒占有的格子信息
void DropItemsAllot::ClearDropSectorInfo()
{
		map<BYTE, DropItem >::iterator it = m_dropItemMap.begin();
		for(; it != m_dropItemMap.end(); ++it)
		{
			if(it->second.sector != NULL)
			{
				it->second.sector->SetHaveItem(false);
			}
		}
}

int DropItemsAllot::PickUpItem(Smart_Ptr<Player> &player, BYTE id)
{
	int64 nowtime = CUtil::GetNowSecond();

	if(nowtime <= (DropItemLoader::GetInstance()->GetDropProtectedTime() + m_createTime))
	{//保护时间
		if(m_owerList.size() != 0)
		{
			vector<int64>::iterator itVec = m_owerList.begin();
			for(; itVec !=m_owerList.end(); ++itVec)
			{
				if(*itVec == player->GetID())
				{
					break;
				}
			}

			if(itVec == m_owerList.end())
			{
				return -1;
			}
		}
	}

	map<BYTE, DropItem >::iterator it = m_dropItemMap.find(id);
	if(it != m_dropItemMap.end())
	{
		//由于  相同 队伍的成员 已经 加入到了 m_owerList 中  故不用 再队伍归属判断

		//判断距离
		bool isValid = false;
		int xPos = player->GetXNewPos();
		int yPos = player->GetYNewPos();
		int itemPos = it->second.sector->GetPosition();
		int itemX = (itemPos >> 16) & 0xffffffff;
		int itemY = itemPos & 0x0000ffff;
		int diffx = abs(xPos-itemX);
		int diffy = abs(yPos-itemY);
		double diff = sqrt(diffx * diffx + diffy * diffy);
		if(diff <= 220)			//5个半格子
		{
			isValid = true;
		}

		if(!isValid)
		{
			return -1;
		}

		if(it->second.data->GetID() == GameParaLoader::GetInstance()->getExchangeMoneyItemId())
		{
			player->ChangeMoney(it->second.count,true);
			player->SynCharAttribute(eCharMoney, player->getAttManage()->getValue<int>(eCharMoney));
		}
		else
		{
			//副本内拾取都得绑定
			if(GET_MAP_TYPE(player->GetMapID()) >= eCopyMap)
			{
				if(it->second.data->GetBindType() != eGetBind)
					it->second.data->SetBind();
			}

			if(!player->GetPackage()->Synch_AddItem(it->second.data, drop_fly_type))
			{
				//背包满了
				return 2;
			}
			else
			{
//				StatisticInfo::CostAndGotInfo costGotInfo;
				eGoodsType equipType = (eGoodsType)it->second.data->GetItemType();
				if(GET_MAP_TYPE(player->GetMapID()) == eStaticMap || GET_MAP_TYPE(player->GetMapID()) == eOnlyMap)
				{
					if(m_bossID)
					{
						//野外BOSS
						if(equipType == eWeapon || equipType == eArmor || equipType == eJewelry)
						{
//							StatisticInfo::GotItemInfo * gotInfo = costGotInfo.add_gotlist();
//							StatisticMgr::AddItemGotInfo(gotInfo,it->second.data->GetID(),it->second.data->GetAmount());
//							if(eAccountType_User == player->GetAccountType())
//								StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_EquipFrom,eStaMinor_EquipFrom_WorldBoss,&costGotInfo);

							//世界BOSS掉落播报
							string strM = ChatMgr::GetInstance()->GetStringByInt(m_bossID);
							string strE = ChatMgr::GetInstance()->GetStringByInt(it->second.data->GetID());
							ChatMgr::GetInstance()->Marquee(eMarqueeDrop,player->GetName(),strM,strE);
						}
					}
				}
				else if(GET_MAP_TYPE(player->GetMapID()) >= eCopyMap)
				{
					if(equipType == eWeapon || equipType == eArmor || equipType == eJewelry)
					{
//						StatisticInfo::GotItemInfo * gotInfo = costGotInfo.add_gotlist();
//						StatisticMgr::AddItemGotInfo(gotInfo,it->second.data->GetID(),it->second.data->GetAmount());
//						if(eAccountType_User == player->GetAccountType())
//							StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_EquipFrom,eStaMinor_EquipFrom_Instance,&costGotInfo);
					}
				}
			}
		}

		//格子还原
		it->second.sector->SetHaveItem(false);

		m_dropItemMap.erase(it);
		if(m_dropItemMap.size() <= 0)
		{
			return 1;
		}
	}
	else
	{
		return -1;
	}

	return 0;
}


void DropItemsAllot::SendToOwer()
{
	Smart_Ptr<Player> player;
	Goods::SynToClientDrop drop;
	Goods::DropItemInfo *dropitem = drop.add_item();

	SetClientInfo(dropitem);

	vector<int64>::iterator itr = m_owerList.begin();
	for(; itr != m_owerList.end(); ++itr)
	{
		PlayerMessage::GetInstance()->GetPlayerByCharId(player,*itr);
		if(player)
		{
			player->SendMessageToGateServer(MSG_SIM_GM2GT_DROPITEM, &drop);
		}
	}
}

void DropItemsAllot::SendLeaveToOwer()
{
	Smart_Ptr<Player> player;
	Goods::DeleteDropItem deleteitem;

	deleteitem.add_id(GetKey());

	vector<int64>::iterator itr = m_owerList.begin();
	for(; itr != m_owerList.end(); ++itr)
	{
		PlayerMessage::GetInstance()->GetPlayerByCharId(player,*itr);
		if(player)
		{
			QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &deleteitem, MSG_SIM_GT2C_DELETEDROP);
		}
	}
}

