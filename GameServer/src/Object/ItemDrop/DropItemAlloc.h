/*
 * DropItemAlloc.h
 *
 *  Created on: 2014年4月18日
 *      Author: helh
 */

#ifndef DROPITEMALLOC_H_
#define DROPITEMALLOC_H_

#include "define.h"
#include <vector>
#include <map>
#include "Goods/PropData.h"
#include "util.h"
#include "../Object.h"
#include "MessageStruct/CharLogin/GoodsMessage.pb.h"
#include "../Player/PlayerMessage.h"
#include "../Player/Player.h"
#include "DropItemDefine.h"
#include "../FileLoader/DropItemLoader.h"

using namespace std;
using namespace CommBaseOut;


// 掉落物
class DropItemsAllot : public Object
{
public:

	DropItemsAllot(eObjType type):Object(type), m_flag(0), m_bossID(0)
	{
		m_createTime = CUtil::GetNowSecond();
	}

	~DropItemsAllot()
	{

	}

	void Release();

	//增加一个拥有者
	void AddOwner(const int64 &charid)
	{
		m_owerList.push_back(charid);
	}

	//初始化
	bool Init(int64 id, const vector<DropStaticInfo>& items);
	bool Init(std::vector<PropBase> propList);
	bool Init(int64 id,DWORD multiple,DWORD rate);
	bool Init(DWORD goodID,DWORD count,int64 time=0);
	bool InitByParam(const vector<DWORD>& goodID,const vector<DWORD>& count,const vector<WORD>& flag);

	//拾取物品
	int PickUpItem(Smart_Ptr<Player> &player, BYTE id);

	bool AverageAlloc();

	int64 GetCreateTime()
	{
		return m_createTime;
	}

	//清除尸体盒占有的格子信息
	void ClearDropSectorInfo();

	//初始化消息
	void SetClientInfo(Goods::DropItemInfo *info)
	{
		info->set_id(GetKey());
		vector<int64>::iterator it = m_owerList.begin();
		for(; it!=m_owerList.end(); ++it)
		{
			Smart_Ptr<Player> player ;
			PlayerMessage::GetInstance()->GetPlayerByCharid(*it, player);
			if(player)
			{
				info->add_key(player->GetKey());
			}
			else
			{
				PlayerMessage::GetInstance()->GetPlayerCacheByCharid(*it,player);
				if(player)
				{
					info->add_key(player->GetKey());
				}
			}
		}

		map<BYTE, DropItem >::iterator itMap = m_dropItemMap.begin();
		for(; itMap!=m_dropItemMap.end(); ++itMap)
		{
			Goods::SingleDropItem *item = info->add_item();

			item->set_id(itMap->second.data->GetID());
			item->set_onlyid(itMap->second.flag);
			item->set_pos(itMap->second.sector->GetPosition());
		}
		info->set_isboss(bool(m_bossID));
	}
	//得到需要广播跑马灯的物品ID
vector<int> GetMarqueeEquipId()
{
	return m_marqueeEquipId;
}

	void SendToOwer();

	void SendLeaveToOwer();

	void SetWhoDrop(int64 bossID)
	{
		m_bossID = bossID;
	}

private:
	vector<int> m_marqueeEquipId;		//需要广播的物品ID
	vector<int64> m_owerList;	// 拥有者链表
	map<BYTE, DropItem > m_dropItemMap;  // 掉落的物品链表
	BYTE m_flag; // 标识
	int64 m_createTime; //生成时间

	// 怪物boss的ID默认是0
	int64 m_bossID;
};



#endif /* DROPITEMALLOC_H_ */
