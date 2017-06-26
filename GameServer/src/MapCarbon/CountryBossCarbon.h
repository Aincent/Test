/*
 * CountryBossCarbon.h
 *
 *  Created on: 2015年6月30日
 *      Author: root
 */

#ifndef COUNTRYBOSSCARBON_H_
#define COUNTRYBOSSCARBON_H_

#include "define.h"
#include "../Map/GameZone.h"
#include "../MapInstance/InstanceMapLoader.h"
#include <map>
#include "../EventSet/EventDefine.h"
#include "MessageStruct/Clan/ClanBattle.pb.h"
#include "MessageStruct/BeyondServerBattle/BeyondBattle.pb.h"
#include "Timer/TimerInterface.h"
#include "CountryDefine.h"
#include "../Map/GameDuplicate.h"

using namespace std;
using namespace CommBaseOut;

struct CountryMember
{
	bool bCut;
	int nScore;
	int nKilling;
	int nKilled;
	int nBoss;
	int nCon;
	int nExp;
	int nCountry;
	int64 nCharid;
	string sName;
	map<int,int> mHarm;

	CountryMember():bCut(false),nScore(0),nKilling(0),nKilled(0),nBoss(0),nCon(0),nExp(0),nCountry(0),nCharid(0)
	{
		sName.clear();
		mHarm.clear();
	}
};

class CCountryBossCarbon : public CGameDuplicate
{
public:
	CCountryBossCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params);
	virtual ~CCountryBossCarbon();

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

	//开始刷怪
	void FlushMonster(void * arg);
	//刷boss收益
	void FlushBossCon(void * arg);
	void FlushBossMessage(void * arg);

	//怪物死亡事件
	bool HandleMonsterDeadEvent(const EventArgs & e);
	//玩家死亡事件
	bool HandlePlayerDeadEvent(const EventArgs & e);

	bool SysCountryBossInfoToClient(Smart_Ptr<Player>& player);
	void UpdateCountryBossBox(int nId,int monsterId,int num);
	//判断是否可以攻击另外的玩家
	virtual bool IsAttacked(int64 fight, int64 attacked);

	void UpdateCountryHarmRank(Smart_Ptr<Player>& player,int monsterId, int value);
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

	void CountryBossOver();
private:
	TimerConnection m_reflushTime; //守护秦王刷新怪物定时器
	TimerConnection m_countDownTime; //副本进入后的倒计时
	TimerConnection m_SendNewTime; //boss战发送信息定时器

	int m_targetPointf; //攻击方的目标分数
	int m_targetPoints; //防守方的目标分数
	map<int,int> m_targerHarmf;
	map<int,int> m_targerHarms;
	map<int64,CountryMember> m_CountryBoss;
	map<int,vector<TeamHarm> > m_CountryHarm;
};
#endif /* COUNTRYBOSSCARBON_H_ */
