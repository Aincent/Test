/*
 * WorldBossCarbon.h
 *
 *  Created on: 2015年11月18日
 *      Author: root
 */

#ifndef WORLDBOSSCARBON_H_
#define WORLDBOSSCARBON_H_

#include "define.h"
#include "../Map/GameZone.h"
#include "../MapInstance/InstanceMapLoader.h"
#include "../EventSet/EventDefine.h"
#include "Timer/TimerInterface.h"
#include "../Map/GameDuplicate.h"
#include "PVPDefine.h"
#include "MessageStruct/WorldBoss/WorldBossDuplicate.pb.h"

struct PlayerWorldBossData
{
	PlayerWorldBossData()
	{
		leaveCount = 0;
		coolTime = 0;
	}
	int leaveCount;
	int64 coolTime;
};

struct ValueCmp
{
	ValueCmp(int64 id):m_id(id){}
	bool operator()(const BossDamageData& data)
	{
		return m_id==data.charid;
	}
	int64 m_id;
};

struct WorldBossTempPrize
{
	WorldBossTempPrize()
	{
		damageSort=0;
		isLastAttack=false;
		isFitch=false;
	}
	int damageSort;
	bool isLastAttack;
	bool isFitch;
};

struct WorldBossData
{
	WorldBossData() : m_Time(0){}
	TimerConnection m_Timer;
	DWORD64 m_Time;
	map<int,bool> m_Prompts;
};

struct DamageBossData
{
	DamageBossData() : m_CharID(0),m_vipLv(0){}
	void reset()
	{
		m_BossDatas.clear(); 
		m_CharID = 0;
		m_vipLv = 0;
		m_charName = "";	
	}
	vector<BossDamageData> m_BossDatas; 
	int64 m_CharID;
	int   m_vipLv;
	string m_charName;
};

class WorldBossCarbon : public CGameDuplicate
{
public:
	WorldBossCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params);
	virtual ~WorldBossCarbon();

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

	//领取奖励
	int Fitch(Smart_Ptr<Player>& player);

	//刷新所有BOSS
	void reflushBoss();

	void sendBossUpdate(Smart_Ptr<Player>& player);

	void updateOneBoss(int bossID);

	void changeState();

	void GetBoosDamage(int64 charId,int bossID,WorldBossDuplicate::AckReqBossDamage& info);
private:
	//怪物死亡事件
	bool HandleMonsterDeadEvent(const EventArgs & e);

	//玩家死亡事件
	bool HandlePlayerDeadEvent(const EventArgs & e);

	//攻击
	bool PlayerAttack(const EventArgs & e);

	//被战姬攻击
	bool BetAttack(const EventArgs & e);

	//强制玩家离开
	void ForceExit(Smart_Ptr<Player>& player);

	//通知世界服进入或离开
	void SendEnterOrLeave(Smart_Ptr<Player>& player, bool isEnter);

	//计算进入冷却时间
	int64 GetCoolTime(int count);

	//定时器更新
	void onSynBossDamage(void *arg);

	//boss刷新定时器
	void bossUpdateTime(void *arg);

	short int getWorldServer(){return m_worldServerID;}

	//伤害
	void addBoosDamage(Smart_Ptr<Player>& player, int value,int bossID);

	//得到伤害排名
	int GetDamageSort(int64 charid,int bossID);

	//计算最后的战绩
	void Result(int bossID);

	//奖励
	void CalPrize(int mapID, int sort, bool isLastAttack, std::vector<PropItem>& items);

	//发邮件
	void SendMail(int64 charid, const std::vector<PropItem>& items);

	//boss血量检测
	void bossBloodCheck(Smart_Ptr<CreatureObj> creature);

	//boss血量推送
	void SynBossDamage(void *arg);

private:
	EventConnection m_playerAttackEvent;//玩家攻击事件
	EventConnection m_betAttackEvent;   //宠物攻击事件
	EventConnection m_BossUpdate;		//boss刷新
	const int m_bossDamagerTime;				  //广播伤害定时时间
	short int m_worldServerID;					  //世界服id

	int64 m_endTime;												//副本的结束时间
	map<int,DamageBossData> m_BossDamageList;//伤害列表
	map<int64,PlayerWorldBossData> m_playersData;//玩家数据
	map<int,Smart_Ptr<WorldBossData> > m_BossData;//该数据创建副本初始化一次，之后有改动则更新，中间不增加
	TimerConnection m_BossDamagerTimer; //广播血量定时器
};


#endif /* WORLDBOSSCARBON_H_ */
