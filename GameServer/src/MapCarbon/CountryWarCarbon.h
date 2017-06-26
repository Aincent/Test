/*
 * CountryWarCarbon.h
 *
 *  Created on: 2015年6月30日
 *      Author: root
 */

#ifndef COUNTRYWARCARBON_H_
#define COUNTRYWARCARBON_H_

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

struct CountryWarDataFind
{
		CountryWarDataFind(int64 id):m_id(id){}
		bool operator()(const CountryWarSortData& data)
		{
				return m_id==data.charID;
		}
		int64 m_id;
};

class CCountryWarCarbon : public CGameDuplicate
{
public:
	CCountryWarCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params);
	virtual ~CCountryWarCarbon();

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
	//更新排名定时器
	void onUpdateSort(void* arg);
	//区域奖励定时器
	void onZonePrize(void* arg);
	//固定时间内根据阵营人数奖励积分贡献定时器
	void onOnlieCountPrize(void* arg);
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
	//排名数据更新
	void updateSortData(int country,int64 charid,int contribute);
	//更新任务目标杀怪进度
	void updateCountryWarTask(int64 charID,int channel,int monsterID);
	void countryWarKillPlayer(Smart_Ptr<CreatureObj>& player,Smart_Ptr<Player>& target);
	void updateSubTaskTarget(Smart_Ptr<Player>& player);
	void canccelOccupy(int64 charID);

	void CountryWarAttrAlter(Smart_Ptr<Player>& player,int id,int value);

	//占领五行柱
	virtual bool occupyElement(int64 charID,int64 object);
	//占领计时器
	void onOccupyTimer(void * arg);
	//得到五行柱信息
	virtual void getElementInfo(std::map<int64,CountryWarElementInfo>& info);
	//领取任务奖励
	virtual bool fitchTaskPrize(int64 charID,int taskID);
	//活动结束奖励
	void CountryWarPrize();
	void CountryWarFinalPrize(int country,bool isWin);
	//增加积分
	virtual void addScore(int country, int score);
	void updateElementState(int64 object, long country);
	//保护定时器
	void protectedTimer(void * arg);
	//五行柱占领增加积分定时器
	void addScoreTimer(void* arg);
	//删除定时器
	void clearWarTimer();
	//判断是否可以攻击另外的玩家
	virtual bool IsAttacked(int64 fight, int64 attacked);
	//得到玩家战绩信息
	virtual void getWarInfo(int64 charID,PersonCountryWarInfo& info,CountryWarData& score);
	//公告
	void broadcastOccupyAll();
	void broadcastContinueWin(int winCount);
	short int getWorldServerID(){ return m_worldServerID; }
	//玩家被攻击事件
	bool HandlePlayerBeAttack(const EventArgs & e);
	bool PlayerCountryWarAttack(const EventArgs & e);
	bool PlayerCountryWarMove(const EventArgs & e);
	//怪物死亡事件
	bool HandleMonsterDeadEvent(const EventArgs & e);
	//玩家死亡事件
	bool HandlePlayerDeadEvent(const EventArgs & e);
private:
	EventConnection m_playerAttackEvent;//玩家攻击事件
	EventConnection m_playerBeAttackEvent;//玩家被攻击事件
	EventConnection m_playerMoveEvent;//玩家移动事件

	TimerConnection m_WarPlayerSortTimer; //更新个人排名定时器
	TimerConnection m_onlinePlayerNumPrize; //根据阵营人数定时奖励积分贡献
	TimerConnection m_countDownTime; //副本进入后的倒计时

	std::map<int64,PersonCountryWarInfo> m_playerCountryWarData;//玩家的阵营战数据
	CountryWarData m_CountryWarData;//本场阵营战的数据
	std::map<int,std::vector<CountryWarSortData> > m_CountryWarSort;//战场排名数据
	std::map<int64,CountryWarElementInfo> m_fiveElement;//五行柱信息
	std::map<int64,OccupyDataEx> m_OccupyTimerList; //玩家占领五行柱定时器
	short int m_worldServerID;
};


#endif /* COUNTRYWARCARBON_H_ */
