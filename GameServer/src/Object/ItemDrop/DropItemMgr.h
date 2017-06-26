/*
 * DropItemMgr.h
 *
 *  Created on: 2014年4月17日
 *      Author: helh
 */

#ifndef DROPITEMMGR_H_
#define DROPITEMMGR_H_

#include "Singleton.h"
#include "define.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "./DropItemAlloc.h"

using namespace CommBaseOut;

typedef std::map<int, Smart_Ptr<DropItemsAllot> > DropItemsMap; // 尸体盒列表

// 管理所有的尸体盒,只有怪物产生之后
class DropItemsManager : public Singleton<DropItemsManager> , public Message_Handler, public Request_Handler
{
public:
	DropItemsManager();

	~DropItemsManager();

	// 删除某个尸体盒
	bool DestoryItemBox(int id)
	{
		DropItemsMap::iterator itr = m_dropItemsList.find(id);
		if (itr == m_dropItemsList.end())
			return 0;

		m_dropItemsList.erase(itr);
		return true;
	}

	bool GetDropPropBymonster(int monster, vector<DropStaticInfo>& item);

	// 得到尸体盒
	Smart_Ptr<DropItemsAllot> GetDropItem(int id)
	{
		DropItemsMap::iterator itr = m_dropItemsList.find(id);
		if (itr == m_dropItemsList.end())
			return 0;

		return itr->second;
	}

	//节日活动额外掉落
	void festivalActiveDrop(const Smart_Ptr<Monster> & attacked,const Smart_Ptr<Player> &owner,int propID,int64 time=0);

	// 战斗死亡事件
	void HandleBattleDeath(const Smart_Ptr<Monster> & attacked,const Smart_Ptr<Player> &owner);

	void HandleBattleDeath(Smart_Ptr<Player> & attacked, std::vector<PropBase> &dropList,Smart_Ptr<CreatureObj> & creatureObj);
	void HandleBattleDeath(map<DWORD,DWORD>& goodsInfo,map<int64, Smart_Ptr<Player> >& owers);
	void HandleBattleDeath(map<DWORD,DWORD>& goodsInfo,vector<int64>& owers,const Smart_Ptr<Player> &owner);
	void dropByparam(Smart_Ptr<CGameZone> zone,const vector<DWORD>& goodID,const vector<DWORD>& count,const vector<WORD>& flag,int posX,int posY);

	//物品 掉落 添加 所拥有则
	void DropItemWhoOwn(Smart_Ptr<DropItemsAllot> &pAllot, const int &itemID, const Smart_Ptr<Player> &owner);

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_PICKITEM);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);

	void HandleDeleteDrop(void *arg);
	void HandleDeleteDrop();
	bool HandleBattleDeath(const EventArgs & e);

private:
	//怪物被杀事件
	EventConnection m_monsterKilledEvent;
	DropItemsMap  m_dropItemsList;				// 尸体盒列表
	TimerConnection m_timerID;
};




#endif /* DROPITEMMGR_H_ */
