/*
 * GameDuplicate.h
 *
 *  Created on: 2014年10月14日
 *      Author: helh
 */

#ifndef GAMEDUPLICATE_H_
#define GAMEDUPLICATE_H_


#include "define.h"
#include "GameZone.h"
#include "../MapInstance/InstanceMapLoader.h"
#include <map>
#include "../EventSet/EventDefine.h"
#include "MessageStruct/Clan/ClanBattle.pb.h"
#include "MessageStruct/Clan/ClanIntegral.pb.h"
#include "MessageStruct/BeyondServerBattle/BeyondBattle.pb.h"
#include "Timer/TimerInterface.h"
#include "CountryDefine.h"

using namespace std;
using namespace CommBaseOut;

//副本进行阶段
enum DuplicateProcess
{
	eCreated=0,
	ePlayerWork,
	eWinFight,
	eFailed,
	eAward,
	eDuplicateChangeMap,
	eChangeMapWait,
	eOver,
	eProcessMax
};

struct TeamHarm
{
	int m_harm;
	int64 m_charid;
	string m_name;

	TeamHarm():m_harm(0),m_charid(0)
	{
		m_name.clear();
	}
};

struct FindTeamHarm
{
	FindTeamHarm(int64 charid) : charid(charid)
	{

	}

	bool operator()( TeamHarm& ew )
	{
		return ew.m_charid == charid;
	}

	 int64 charid;
};

struct AwardFlops
{
	DWORD m_itemid;
	DWORD m_Counts;
	WORD  m_BindType;
	int64 m_charid;

	AwardFlops():m_itemid(0),m_Counts(0),m_BindType(0),m_charid(0)
	{

	}
};

class Player;
class SinglePlayerDeque;

class CGameDuplicate : public CGameZone
{
public:
	CGameDuplicate(int64 onlyid, int64 charid, int64 paramf, int64 params);
	virtual ~CGameDuplicate();

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
	virtual int IsCanEnter(int64 charid, int fight, int clanid);

	//保存进入玩家的上次场景和坐标
	void SetOldMapPoint(int64 charid, int64 mapid, int xpos, int ypos)
	{
		OldMapInfo info;

		info.mapid = mapid;
		info.xpos = xpos;
		info.ypos = ypos;

		m_oldMapPoint[charid] = info;
	}

	OldMapInfo * GetOldMapPoint(int64 charid)
	{
		map<int64,OldMapInfo>::iterator it = m_oldMapPoint.find(charid);
		if(it != m_oldMapPoint.end())
			return &it->second;

		return 0;
	}

	InstanceMapEntry* GetInstanceScript() { return m_data; }

	//随机进入副本
	virtual void RandJoinMap();
	//帮派被解散了
	virtual void ClanDelete(int clanid);
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

	//怪物死亡事件
	bool HandleMonsterDeadEvent(const EventArgs & e);
	//玩家死亡事件
	bool HandlePlayerDeadEvent(const EventArgs & e);
	//退出场景事件(如果不希望玩家下次还能进入此场景，最好在此处理)
	bool HandleLeaveMapEvent(const EventArgs & e);
	bool HandlePlayerAttackEvent(const EventArgs & e);

	//判断帮派站是否已经开始
	bool IsClanBattleStart()
	{
		return m_clanBattle;
	}
	//获取玩家的坐标信息
	void GetAllPlayerPosition(ClanPackage::ClientReqMMapPlayer &toClient, DWORD clanid, int key);

	//反王挖掘
	void  IncreaseDroNum();

	bool IsMaxDroNum();

	//判断守护秦王是否已领奖
	bool IsHadReward()
	{
		return m_params <= 0;
	}

	void SetHadReward()
	{
		m_params = 0;
	}

	int64 GetParamfValue()
	{
		return m_paramf;
	}
	void SetParamValue(int64 charid, int value)
	{
		if(charid == m_owner)
			m_paramf = value;
		else
			m_params = value;
	}

	int64 GetParamsValue()
	{
		return m_params;
	}

	bool GetSuccess()
	{
		return m_isSuccess;
	}

	//是否是主拥有者
	bool IsMainOwner(int64 charid)
	{
		return m_owner == charid;
	}

	//
//	bool GenerateRandomAward(int num);

	//将副本怪物所有的坐标点同步给客户端（副本的怪物空闲状态不寻路）
	void GetAllMonsterPosInfo(PlayerSynch::RetInstanceMonsterPos &toClient);

	bool ForceAllPlayerChangeMap();

	virtual int SetPlayerInstanceFlops(int64 charid,int flopid);

	//判断是否可以攻击另外的玩家
	virtual bool IsAttacked(int64 fight, int64 attacked);

	void DestoryRanking();
	//查看击杀排名
	virtual int GetClanIntegralRanking(Smart_Ptr<Player>& player,ClanPackage::RetKillRankingInfo& info);

	void ArenaMonsterBegin(void* arg);

	int GetDigNum()
	{
		return m_nDigNum;
	}
private:

	//结算时移除怪物及事件订阅
	void ClearAll();

protected:

	int64 m_owner;//所属者(守护秦王和竞技场为角色id，帮派战为城池id)
	int64 m_owner_s; //所属者（夸服战有两个拥有者）
	InstanceMapEntry* m_data;
	DuplicateProcess m_process; //副本进行的阶段
	bool m_isSuccess; //是否是输了

	int64 m_createTime; //创建时
	int64 m_leaveTime; //玩家走光时的时间
	int64 m_awardTime; //奖励时间

	map<int64,OldMapInfo> m_oldMapPoint; //玩家原来的点
	vector<AwardFlops> m_AwardList;	//组队副本翻牌奖励

	int64 m_paramf; //竞技场代表目标名次，帮派站代表是攻击方的帮派id，守护秦王是最开始的关数, 夸服战为owner的积分  //组队副本队伍ID
	int64 m_params; //帮派战代表是防守方的帮派id，守护秦王是当前进行的关数(领取完奖励后，置成0)，夸服战为owner_s的积分

	EventConnection m_monsterDeadEvent;//怪物被人杀了事件
	EventConnection m_playerDeadEvent;//玩家死亡事件
	EventConnection m_leaveMapEvent;//离开场景事件

	bool m_clanBattle;//帮派站是否开始了
	int m_nDigNum; //反王挖掘次数

	TimerConnection m_arenaBeginTime;

};





#endif /* GAMEDUPLICATE_H_ */
