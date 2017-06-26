/*
 * ClanIntegralCarbon.h
 *
 *  Created on: 2016年2月25日
 *      Author: root
 */

#ifndef CLANINTEGRALCARBON_H_
#define CLANINTEGRALCARBON_H_

#include "define.h"
#include "../Map/GameZone.h"
#include "../MapInstance/InstanceMapLoader.h"
#include <map>
#include "../EventSet/EventDefine.h"
#include "MessageStruct/Clan/ClanIntegral.pb.h"
#include "Timer/TimerInterface.h"
#include "../Map/GameDuplicate.h"

struct IntegralInfo
{
	string sName;
	int64 nCharid;
	int nBoss;
	int nKill;
	int nDead;
	int nRanking;
	int nMaxKill;
	int nTemp;

	IntegralInfo():nCharid(0),nBoss(0),nKill(0),nDead(0),nRanking(0),nMaxKill(0),nTemp(0)
	{
		sName.clear();
	}

/*	bool operator<(const IntegralInfo& info) const
	{
			return nKill > info.nKill;
	}*/
};

class CClanIntegralCarbon : public CGameDuplicate
{
public:
	CClanIntegralCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params);
	virtual ~CClanIntegralCarbon();

	int Init();
	//更新状态,false表示要删除这个场景
	bool Update();
	//加入场景
	int Join(Smart_Ptr<Object> & obj);
	//加入场景前处理
	bool JoinHandle(Smart_Ptr<Object> & obj);

	//场景中移动
	virtual int Move(Smart_Ptr<Object> &obj);

	virtual void Leave(Smart_Ptr<Object> & obj, bool isDel = true, bool delFromMap = false);
	//离开场景前处理
	virtual void LeaveHandle(Smart_Ptr<Object> & obj);
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

	//怪物死亡事件
	bool HandleMonsterDeadEvent(const EventArgs & e);
	//玩家死亡事件
	bool HandlePlayerDeadEvent(const EventArgs & e);

	//开始刷怪
	void FlushMonster(void * arg);

	//查看击杀排名
	int GetClanIntegralRanking(Smart_Ptr<Player>& player,ClanPackage::RetKillRankingInfo& info);
	//更新积分信息
	void FlushIntegral(void * arg);

	bool PlayerAttack(const EventArgs & e);

	bool HandleLeaveClanEvent(const EventArgs & e);
	//帮派被解散了
	void ClanDelete(int clanid);
private:
	//BOSS刷新后全属性增长倍率
	int GetFlushNum(int monsterId);
	//加入副本处理
	void JoinPlayer(Smart_Ptr<Player>& player);
	//处理怪物被杀积分
	void AddBossKilled(Smart_Ptr<CreatureObj>& object);
	//积分战结算
	void IntegralEnd();
	//去除离开副本的玩家
	void DorpIntegralLeave();
	//是否有一帮玩家已走完
	int GetVailPlayer();
private:
	int m_totalPlayerf;
	int m_totalPlayers;
	int m_totalBossf;
	int m_totalBosss;
	int m_clanDelete;
	map<int64,IntegralInfo> m_integralf;
	map<int64,IntegralInfo> m_integrals;
	TimerConnection m_reflushTime; //刷新怪物定时
	TimerConnection m_reflushIntegral; //刷新积分信息
	EventConnection m_playerAttackEvent;//玩家攻击事件
	EventConnection m_leaveClanEvent;
	map<int,int> m_flushNum;
};


#endif /* CLANINTEGRALCARBON_H_ */
