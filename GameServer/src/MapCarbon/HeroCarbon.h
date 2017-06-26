/*
 * HeroCarbon.h
 *
 *  Created on: 2016年5月18日
 *      Author: root
 */

#ifndef HEROCARBON_H_
#define HEROCARBON_H_

#include "define.h"
#include "../Map/GameZone.h"
#include "../MapInstance/InstanceMapLoader.h"
#include "../EventSet/EventDefine.h"
#include "Timer/TimerInterface.h"
#include "PVPDefine.h"
#include "../Map/GameDuplicate.h"

class HeroCarbon : public CGameDuplicate
{
public:
	HeroCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params);
	virtual ~HeroCarbon();

public:

protected:

	int Init();

	//更新状态,false表示要删除这个场景
	bool Update();

	//加入场景
	int Join(Smart_Ptr<Object> & obj);

	//加入场景前处理
	bool JoinHandle(Smart_Ptr<Object> & obj);

	//场景中移动
	int Move(Smart_Ptr<Object> &obj);

	//离开场景，isDel是否广播移出
	void Leave(Smart_Ptr<Object> & obj, bool isDel = true, bool delFromMap = false);

	//离开场景前处理
	void LeaveHandle(Smart_Ptr<Object> & obj);

	//玩家强制退出副本检验(用于判断玩家退出后副本的后续操作 )
	void LeaveExame(Smart_Ptr<Player> & player);

	//此场景是否可以进入
	bool IsCanEnter(Smart_Ptr<Player> & player);

	//赢了后的操作
	void Win();

	//副本结束时间到了之后的 操作
	void Over();

	//副本人数为零时的操作
	void Count();
	void CountEx();

	//输了之后的操作
	void Failed();

	//奖励
	void Award();

	//怪物死亡事件
	bool HandleMonsterDeadEvent(const EventArgs & e);

	//玩家死亡事件
	bool HandlePlayerDeadEvent(const EventArgs & e);

	//vip
	bool HandleUpdateVipLv(const EventArgs & e);

	void InitPlayer(Smart_Ptr<Player>& player);

	void AddBuffer(Smart_Ptr<Player>& player);

	void ClearBuffer(Smart_Ptr<Player>& player);

	void CreateMonsters(int layer);

	void SendBaseInfo(Smart_Ptr<Player>& player);

	void InitTargets();

	void AddTarget(int id);

	void Result();

private:
	std::vector<int> m_buffers;
	std::map<int,pair<int,int> > m_killMonsters;
	EventConnection m_vipEvent;
};




#endif /* HEROCARBON_H_ */
