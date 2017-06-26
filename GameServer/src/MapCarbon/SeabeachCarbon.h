/*
 * SeabeachCarbon.h
 *
 *  Created on: 2015年11月3日
 *      Author: root
 */

#ifndef SEABEACHCARBON_H_
#define SEABEACHCARBON_H_

#include "define.h"
#include "../Map/GameZone.h"
#include "../MapInstance/InstanceMapLoader.h"
#include "../EventSet/EventDefine.h"
#include "Timer/TimerInterface.h"
#include "../Map/GameDuplicate.h"
#include "PVPDefine.h"

enum eIncomeTypeID
{
	eIncomeTypeID_EXP = 1,		//经验
	eIncomeTypeID_FORCE = 2,	//真气
};

struct PlayerSeabeachData
{
	PlayerSeabeachData()
	{
		exp = 0;
		force = 0;
		girlID = 0;
	}

	int exp;
	int force;
	int64 girlID;
};

class SeabeachCarbon : public CGameDuplicate
{
public:
	SeabeachCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params);
	virtual ~SeabeachCarbon();

public:
	//采集
	int Gather(Smart_Ptr<Player>& player, int objkey);

	//离开采集对象
	int LeaveObject(Smart_Ptr<Player>& player);

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
	//玩家移动事件
	bool PlayerMove(const EventArgs & e);

	bool PlayerMeditation(const EventArgs & e);

	bool MeditationStart(const EventArgs & e);

	//基础奖励
	void onZonePrize(void * arg);

	//采集结果
	void onGatherTimer(void* arg);

	//取消采集
	void CanccelGather(int64 charid, int code=0);

	//更新玩家数据
	void UpdatePlayerData(Smart_Ptr<Player>& player, int exp, int force=0);

	//同步更新
	void SendToPlayer(Smart_Ptr<Player>& player, int exp, int force);

	//初始化基础收益信息
	void InitBaseIncome(Smart_Ptr<Player>& player);

	//开启变身奖励定时器
	void StartChangeTimer(Smart_Ptr<Player>& player, int monster);

	//结束变身
	void EndChange(Smart_Ptr<Player>& player);

	//刷新怪物
	void FlushMonster(int monster, int xpos, int ypos);

	//初始化战姬
	void InitGirlWar(Smart_Ptr<Player>& player);

	//还原战姬
	void ReverseGirlWar(Smart_Ptr<Player>& player);

private:
	TimerConnection m_ZoneTimer;  //场景奖励定时器

	EventConnection m_playerMoveEvent;

	EventConnection m_playerMeditationEvent;
	EventConnection m_MeditationStartEvent;

	std::map<int64,PlayerSeabeachData> m_playerData;	//玩家的海滩数据

	std::map<int64,PlayerGatherData> m_playerGather;	//玩家的采集数据

	std::map<int64,GatherInfo> m_GatherInfos;//被采集对象数据

};

#endif /* SEABEACHCARBON_H_ */
