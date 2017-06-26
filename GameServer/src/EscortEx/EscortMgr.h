/*
 * EscortMgr.h
 *
 *  Created on: 2014年7月21日
 *      Author: root
 */

#ifndef ESCORTMGR_H_
#define ESCORTMGR_H_

#include "define.h"
#include <map>
#include "./EventSet/EventArgs.h"
#include "../Daily/Daily.h"
#include "./FileLoader/EscortFileLoader.h"
#include "Timer/TimerInterface.h"
#include "../EventSet/EventDefine.h"

using namespace std;
using namespace CommBaseOut;

//宝箱随机基数
#define ESCORTRANDOMRATE	10000

enum eCarType
{
	//青铜
	eEscortBronze,
	//白银
	eEscortSilver,
	//黄金
	eEscortGold,
	//白金
	eEscortPlatinum,
	//钻石
	eEscortDiamond,

	eEscortMax
};

class Player;
class Monster;

class EscortMgr
{
public:
	EscortMgr(Player *player);

	~EscortMgr();

	void ReInit();
	void ReleaseEscortMgr();

	void SetEscortToDB(PlayerInfo::EscortInfo *info);

	void InitEscort(const PlayerInfo::EscortInfo &info);

	//元宝直接领取皇纲
	int SetEscortQuality(eCarType quality);

	//随机产生宝箱品质
	int GenenateQuality(DWORD escortId, DWORD &quality);

	//开始押送
	int StartEscort(bool isAdd = false);

	//立即完成
	int ImediaComplete(int &exp,int& money);

	//加速
	bool SpeedUpEscort();

	//放弃
	int AbandonEscort();

	//压镖剩余次数
	DWORD GetEscortLeftTime();
//	// 押镖已使用次数 返回总次数
//	int GetHadEscortTimes(int& times);
	int GetEscortTotalNum();

	//压镖剩余次数
	DWORD getEsortSurplusTime();

	DWORD GetLine()
	{
		return m_line;
	}

	eCarType GetQuality()
	{
		return m_quality;
	}

	int GetTime();

	int GetSpeedTime();

	//定时结束押镖
	void EndEscort(void* arg);

	//定时结束加速
	void EndSpeedUp(void* arg);

	//定时复活(由于镖车复活跟SetDead冲突，会引起多次leave 格子)
	void Reflush(void* arg);

	//定时公告被劫镖
	void NoticeBeEscort(void* arg);

	//清除押镖信息
	void Clear();

	//移出镖车
	bool LeaveEscortData(const EventArgs & e);

	//加入镖车
	bool EnterEscortData(const EventArgs & e);

	//玩家移动
	bool MoveEscortData(const EventArgs & e);

	//杀怪
	bool KillMonster(const EventArgs & e);

	//怪物被攻击
	bool MonsterBeAttack(const EventArgs & e);

	//怪物死亡
	bool MonsterDead(const EventArgs & e);

	//获得的经验
	int GetAllExp();

	int GetExp();

	//获得金钱
	int GetAllMoney();

	int GetMoney();

	//获得镖车
	Smart_Ptr<Monster> GetEscortMonster();

	 //找回 护送黄冈 数据 相关
	int RecoverResource(int num,Daily::RecoverResourceInfo *info);

	int CalAddExp(const int &haveNum ,const int &lv , const sEscortEntry * entry);

	int CalAddMoney(const int& haveNum,const int &lv, const sEscortEntry* entry);

	int GetRobTimes()
	{
		return m_robTimes;
	}

	int getRewardExp() const
	{
		return m_rewardExp;
	}

	int getRewardMoney() const
	{
		return m_rewardMoney;
	}

private:
	//护送线路ID
	DWORD m_line;
	//宝箱品质
	eCarType m_quality;
	//骠车
	Smart_Ptr<Monster> m_carMon;
	//被抢次数
	DWORD	m_robTimes;
	//押镖开始时间
	int64 m_startTime;
	//加速开始时间
	int64 m_speedTime;
	Player *m_owner;
	int64 m_mapid;
	int m_xpos;
	int m_ypos;
	// 当前血量
	int m_hp;
	//刷新只能前进不能后退
	int m_nRate;
	int m_num;
	//经验奖励
	int m_rewardExp;
	//铜钱奖励
	int m_rewardMoney;

	//运镖定时器ID
	TimerConnection m_escortEnd;
	//加速定时器ID.
	TimerConnection m_speedUpEnd;
	//复活定时器ID.
	TimerConnection m_reflushTimer;
	//通告定时器ID
	TimerConnection m_NoticeTimer;
	//进入场景
	EventConnection m_enterEvent;
	//移动
	EventConnection m_moveEvent;
	EventConnection m_logoutEvent;
	EventConnection m_killMonsterEvent;
	EventConnection m_monsterAttacked;
	EventConnection m_monsterDead;

	map<int64,string> m_sAttackedName;
};

#endif /* ESCORTMGR_H_ */
