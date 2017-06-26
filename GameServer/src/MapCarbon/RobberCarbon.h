/*
 * RobberCarbon.h
 *
 *  Created on: 2016年2月23日
 *      Author: root
 */

#ifndef ROBBERCARBON_H_
#define ROBBERCARBON_H_

#include "define.h"
#include "../Map/GameZone.h"
#include "../MapInstance/InstanceMapLoader.h"
#include "../EventSet/EventDefine.h"
#include "Timer/TimerInterface.h"
#include "../Map/GameDuplicate.h"
#include "../Object/FileLoader/MonsterLoader.h"

class RobberCarbon : public CGameDuplicate
{
public:
	RobberCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params);
	virtual ~RobberCarbon();

public:
	void Destroy(int clanid);
	void Active(bool flag, int clanid, bool win, int param, int conwin, int confail);

protected:
	void Start(bool win, int param, int conwin, int confail);
	void End();

	void FreshMonster(int node);
	void CreateMonster(int id, int posx, int posy);

	void AlterMonsterAttr(Smart_Ptr<Monster>& mon, const MonsterInfo& info);

	void onFlushMonster(void* args);

	void KillMonsters();

	//怪物死亡事件
	bool HandleMonsterDeadEvent(const EventArgs & e);

	void AddMonster(int key, Smart_Ptr<Monster>& mon);
	void RemoveMonster(int key);

	//结算
	void Result(bool win);

protected:
	virtual int Init();

	//更新状态,false表示要删除这个场景
	virtual bool Update();

	//加入场景
	virtual int Join(Smart_Ptr<Object> & obj);

	//加入场景前处理
	virtual bool JoinHandle(Smart_Ptr<Object> & obj);

	//场景中移动
	virtual int Move(Smart_Ptr<Object> &obj);

	//离开场景，isDel是否广播移出
	virtual void Leave(Smart_Ptr<Object> & obj, bool isDel = true, bool delFromMap = false);

	//离开场景前处理
	virtual void LeaveHandle(Smart_Ptr<Object> & obj);

	//玩家强制退出副本检验(用于判断玩家退出后副本的后续操作 )
	virtual void LeaveExame(Smart_Ptr<Player> & player);

	//此场景是否可以进入
	virtual bool IsCanEnter(Smart_Ptr<Player> & player);

	//赢了后的操作
	virtual void Win();

	//副本结束时间到了之后的 操作
	virtual void Over();

	//副本人数为零时的操作
	virtual void Count();
	virtual void CountEx();

	//输了之后的操作
	virtual void Failed();

	//奖励
	virtual void Award();

private:
	int m_curNode;

	bool m_IsPrize;
	bool m_IsCalDead;	//是否算死亡

	bool m_win;
	int m_param;
	int m_conwin;
	int m_confail;
	int m_clanMoney;

	TimerConnection m_FlushTimer;  //怪物刷新定时器

	std::map<int,Smart_Ptr<Monster> > m_FreshObjects; 	//怪物

};



#endif /* ROBBERCARBON_H_ */
