/*
 * BeyondBattleMgr.h
 *
 *  Created on: 2014年12月9日
 *      Author: helh
 */

#ifndef BEYONDBATTLEMGR_H_
#define BEYONDBATTLEMGR_H_

#include "Singleton.h"
#include "define.h"
#include "Timer/TimerInterface.h"
#include <map>
#include <vector>

using namespace std;
using namespace CommBaseOut;

enum BeyondBattleType
{
	eBeyondBattleOne=1,
	eBeyondBattleThree=3
};

struct SinglePlayerDeque
{
	int64 charid;
	int64 time;
	int lv;
	int fight;
	int score;
	int64 mapid; //需要创建的地图id
	int attack; //打出的伤害

	SinglePlayerDeque():charid(-1),time(0),lv(0),fight(0),score(0),mapid(0),attack(0)
	{

	}
};

struct WaitMatch
{
	int groupID; //组id
	int type;
	int score;//组最大积分
	int fight;//战斗力
	int64 charid;//组最大积分的id
	int64 time;//队伍成立的时间
	vector<SinglePlayerDeque> member;

	WaitMatch():groupID(0),type(0),score(0),fight(0),charid(0),time(0)
	{

	}
};

struct MatchDeque
{
	int groupID;// 自己的组id
	int targetID; //对方的组id
	int type;
	int score;//组最大积分
	int fight;//战斗力
	int64 charid;//组最大积分的id
	int64 time; //队伍成立的时间
	int64 matchTime; //匹配时的时间
	vector<SinglePlayerDeque> member;

	MatchDeque():groupID(-1),targetID(-1),type(0),score(0),fight(0),charid(0),time(0),matchTime(0)
	{

	}
};

struct MatchFight
{
	MatchDeque myself;// 自己的id
	MatchDeque target; //对方的id
	int64 time; //打的时间
};

class BeyondBattleMgr : public Singleton<BeyondBattleMgr>
{
public:
	BeyondBattleMgr();
	~BeyondBattleMgr();

	//过滤超时列表
	void DeleteWaitDeque();
	void DeleteMatchDeque();
	void DeleteFightDeque();
	//是否能够加入等待匹配表
	bool IsAddWaitMatch(int64 id);
	//匹配等待的玩家
	void MatchPlayer();
	bool MatchPlayer(WaitMatch fGroup, WaitMatch tGroup);
	//加入等待匹配列表
	void AddWaitMatch(int64 id, int lv, int fight, int score, int type);
	void AddWaitMatch(MatchDeque fGroup);
	//加入到战斗列表
	void AddFight(int64 mapid, int myself, int target);
	//删除玩家信息
	void DeletePlayerInfo(int64 id);

	//取消匹配了
	void CancelMatch(int64 id);

	//删除pk列表
	void DeleteFight(int64 mapid);

	//配对玩家加入地图id(返回 0 需要创建副本    1 不需要创建副本     －1  需要删除创建地图缓存信息)
	int AddMapIDToMatch(int64 charid, int64 mapid, int myself, int target, int64 &targetMap);

	//刷新匹配队列
	void FlushDeque(void *arg);

	//判断玩家是不是主攻方
	bool IsFirstAttack(int64 mapid, int64 charid);
	//判断玩家是不是在pk表中
	bool IsFightDeque(int64 mapid, int64 charid);

	//家伤害值
	void AddAttackValue(int64 mapid, int64 charid, int value);

//	//success  1为赢，2为输，3为平
//	void SendBattleResult(int64 mapid, int success);
	//获得两个组
	void GetBattleGroup(int64 mapid, MatchDeque &first, MatchDeque & second);

	//获取玩家的序号，以及类型
	void GetPlayerIndexAndType(int64 mapid, int64 charid, int &index, int &type);

private:

	vector<WaitMatch> m_waitMatch;//等待匹配的玩家
	map<int, MatchDeque> m_match;//已经配对的玩家
	map<int64, MatchFight> m_fight;//已经pk的玩家

	TimerConnection  m_flushTime;
	int m_onlyID; //用于生成队伍id
};



#endif /* BEYONDBATTLEMGR_H_ */
