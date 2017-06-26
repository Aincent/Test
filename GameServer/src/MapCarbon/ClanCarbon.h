/*
 * ClanCarbon.h
 *
 *  Created on: 2015年6月29日
 *      Author: root
 */

#ifndef CLANCARBON_H_
#define CLANCARBON_H_

#include "define.h"
#include "../Map/GameZone.h"
#include "../MapInstance/InstanceMapLoader.h"
#include <map>
#include "../EventSet/EventDefine.h"
#include "MessageStruct/Clan/ClanBattle.pb.h"
#include "Timer/TimerInterface.h"
#include "CountryDefine.h"
#include "PVPDefine.h"
#include "../Map/GameDuplicate.h"
#include "../Activity/ActivityBox.h"

using namespace std;
using namespace CommBaseOut;

struct WillInBattlePlayer
{
	int64 charid;
	int64 time;
	int fight;
};

struct ClanRanking
{
	int nKill; 	//杀敌
	int nCheckFlag; //抢旗
	int nBattle;	//战功
	int64 charid;
	string sName; //名字
	int lv;//等级

	ClanRanking():nKill(0),nCheckFlag(0),nBattle(0),charid(0),lv(1)
	{
		sName.clear();
	}
};

struct CampBatterData
{
	CampBatterData()
	{
		clanid = 0;
		campid = 0;
		scroe = 0;
	}

	int clanid;	//帮派id
	int campid; //红蓝方阵营id
	int scroe;		//积分
};

class CClanCarbon : public CGameDuplicate
{
public:
	CClanCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params);
	virtual ~CClanCarbon();

public:
	//玩家占领
	bool OccupyFlag(int64 playerID, int ObjectKey);

	//箱子能否采集
	bool IsCanGatherBox(int nKey,Smart_Ptr<Player>& player);

	//获取战场基本数据
	bool GetBaseInfos(Smart_Ptr<Player>& player, ClanPackage::BaseBatterInfo& info);

protected:
	void OnOccupyTimer(void* args);
	void ProtectedTimer(void * args);
	void AddScoreTimer(void* args);
	void OnFlushBox(void * args);

	//取消采集
	void CanccelGather(int64 charid,bool isRemoveFromOjb=true, bool isSendtoClient=true);

	const CampBatterData* GetClanBatterData(int id);

	void UpdateFlagState(int64 object, long owner);

	void AddScore(int clanid, int score);

	void AskFactionName(Smart_Ptr<Player> player, int factionId);

	void Result(int winid, int firstscore, int secondscore);

	void GetReWardItems(std::vector<PropItem>& Props, bool win);

	int Init();
	//更新状态,false表示要删除这个场景
	bool Update();
	//加入场景
	int Join(Smart_Ptr<Object> & obj);
	//加入场景前处理
	bool JoinHandle(Smart_Ptr<Object> & obj);

	//场景中移动
	virtual int Move(Smart_Ptr<Object> &obj);

	//离开场景，isDel是否广播移出
	virtual void Leave(Smart_Ptr<Object> & obj, bool isDel = true, bool delFromMap = false);
	//离开场景前处理
	virtual void LeaveHandle(Smart_Ptr<Object> & obj);

	int ObjectLevel(Smart_Ptr<Object> & obj, bool isDel = true, bool delFromMap = false);

 void AfterLeaveHandle(Smart_Ptr<Object> & obj);

	//玩家强制退出副本检验(用于判断玩家退出后副本的后续操作 )
	virtual void LeaveExame(Smart_Ptr<Player> & player);

	//此场景是否可以进入
	virtual bool IsCanEnter(Smart_Ptr<Player> & player);
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

	bool HandleAttackEvent(const EventArgs & e);

	bool HandleMoveEvent(const EventArgs & e);

	bool HandleLeaveMapEvent(const EventArgs & e);

	//怪物死亡事件
	bool HandleMonsterDeadEvent(const EventArgs & e);

	//玩家死亡事件
	bool HandlePlayerDeadEvent(const EventArgs & e);

	//	退出帮派事件
	bool HandleLeaveClanEvent(const EventArgs & e);

private:
	EventConnection m_leaveClanEvent;			//离开帮派事件
	EventConnection m_playerAttackEvent;
	EventConnection m_playerMoveEvent;

	TimerConnection m_Boxtimer;	//宝箱刷新定时器(循环)

	std::map<int,CampBatterData>	m_BatterData;//帮派id对应的战场数据

	std::map<int64,CountryWarElementInfo> m_fiveElement;//五行柱信息
	std::map<int64,OccupyDataEx> m_OccupyTimerList; //玩家占领五行柱定时器

	std::map<int,MasterStatus> m_Boxs;

	int serverid;
	DWORD64 m_endtime; //结束时间
};


#endif /* CLANCARBON_H_ */
