/*
 * PVPCarbon.h
 *
 *  Created on: 2015年7月24日
 *      Author: root
 */

#ifndef PVPCARBON_H_
#define PVPCARBON_H_

#include "define.h"
#include "../Map/GameZone.h"
#include "../MapInstance/InstanceMapLoader.h"
#include <map>
#include "../EventSet/EventDefine.h"
#include "MessageStruct/Clan/ClanBattle.pb.h"
#include "Timer/TimerInterface.h"
#include "../Map/GameDuplicate.h"
#include "PVPDefine.h"
#include "MessageStruct/PVP/PVP.pb.h"

struct FindCmp
{
	FindCmp(int64 id):m_id(id){}
	bool operator()(const BossDamageData& data)
	{
				return m_id==data.charid;
	}
	int64 m_id;
};

class PVPCarbon : public CGameDuplicate
{
public:
	PVPCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params);
	virtual ~PVPCarbon();

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

	//禁止任何操作
	void Forbid();
	bool isForbid(){ return m_isForbid; }

	//能否采集
	bool Gather(Smart_Ptr<Player> &player,int key);

	//初始化玩家pvp数据
	void initPlayerData(int64 charid,const PvPPackage::AckPlayerPVPData& info);
	void getPlayerData(int64 charid,PvPPackage::PlayerPvPData &toClient);

	void getDamageList(std::vector<BossDamageData>& lists){lists=m_BossDamageList;}

private:
	void release();

	//判断是否可以攻击另外的玩家
	virtual bool IsAttacked(int64 fight, int64 attacked);

	//玩家被攻击事件
	bool PlayerBeAttack(const EventArgs & e);
	bool PlayerAttack(const EventArgs & e);
	bool BetAttack(const EventArgs & e);
	bool PlayerMove(const EventArgs & e);
	//怪物死亡事件
	bool HandleMonsterDeadEvent(const EventArgs & e);
	//玩家死亡事件
	bool HandlePlayerDeadEvent(const EventArgs & e);

	//
	void onZonePrize(void * arg);
	void onGatherTimer(void* arg);
	void onSynPlayerData(void * arg);
	void onSynBossDamage(void *arg);

	int getLayer(){return m_layer;}

	short int getWorldServer(){return m_worldServerID;}

	//同步个人数据到世界服,主要针对杀人总数
	void synOnePlayerDataToWorld(int64 charid,const PlayerPvPDataEx& data);

	//通知离开或切换层
	void synWorldLayerChange(int64 charid,int64 flag);

	//更新玩家战场数据
	void updatePlayerData(int64 charid,int type,int addValue);

	//取消采集
	void canccelGather(int64 charid,bool isRemoveFromOjb=true, bool isSendtoClient=true);

	//杀人
	void KillPlayer(Smart_Ptr<CreatureObj>& obj,Smart_Ptr<Player>& target);

//	//采集跳转
//	void GatherJump(Smart_Ptr<Player>& player);
//
//	//跳转楼层
//	void jumpLayer(Smart_Ptr<Player>& player,bool isBeKill);
	void jumpLayer(Smart_Ptr<Player>& player);
	void jumpto(Smart_Ptr<Player>& player,int layer);

	//
	void synBossHP(Smart_Ptr<Player>& player);
	int getBossHP();

	void addBoosDamage(Smart_Ptr<Player>& player, int key, int value);
	void BrocastBossHP(int key);

	void firstFight(Smart_Ptr<Player>& player);


private:
	EventConnection m_playerAttackEvent;//玩家攻击事件
	EventConnection m_betAttackEvent;   //宠物攻击事件
	EventConnection m_playerBeAttackEvent;//玩家被攻击事件
	EventConnection m_playerMoveEvent;//玩家移动事件

	TimerConnection m_expPrizeTimer; //副本经验奖励时间间隔
	TimerConnection m_PlayDataTimer; //定时同步所有玩家的信息给世界服
	TimerConnection m_BossDamageTimer; //

	std::map<int64,PlayerGatherData> m_playerGather;//玩家的采集数据
	std::map<int64,GatherInfo> m_GatherInfos;//被采集对象数据
	std::map<int64,PlayerPvPDataEx> m_playerPvPData;//玩家活动数据
	std::vector<BossDamageData> m_BossDamageList;

	int m_layer;
	const int m_synTime;									//同步玩家信息的时间
	const int m_bossDamagerTime;				//
	short int m_worldServerID;					//世界服id
	bool m_isForbid;											//是否禁止操作

	Smart_Ptr<Monster> m_BossObject;

};





#endif /* PVPCARBON_H_ */
