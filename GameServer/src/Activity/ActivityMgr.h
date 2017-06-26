/*
 * ActivityMgr.h
 *
 *  Created on: 2014年11月28日
 *      Author: helh
 */

#ifndef ACTIVITYMGR_H_
#define ACTIVITYMGR_H_


#include "define.h"
#include "util.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "Timer/TimerInterface.h"
#include "./FileLoader/ProtectedKingLoader.h"
#include "MessageStruct/Activity/ProtectKing.pb.h"
#include "MessageStruct/Activity/YellowVip.pb.h"
#include "MessageStruct/Activity/TreasonKingInfo.pb.h"

#include "../Daily/Daily.h"
#include "Log/Logger.h"

using namespace std;
using namespace CommBaseOut;

#define TREASONKINGMAPFIRSTID 238
#define TREASONKINGMAPLEVEL 9

enum EYellowVip
{
	eNovice_Packs = 1,
	eGrow_Packs,
	eDaily_Packs,
	eYearVipPacks,
	eGirlWarPacks,
	eVipPacks_Max,
};

enum RewardState
{
	RewardState_DontHave,//没有达到领取条件
	RewardState_NotGet,//没有去领取
	RewardState_HaveGet,//已经领取
	RewardState_Max,
};


class Player;
class ActivityMgr
{
public:
	ActivityMgr(Player * obj);
	~ActivityMgr();

	void ReInit();
	void ReleaseActivity();

	void SetActivitytableInfo(	PlayerInfo::Activity* info);
	void InitFromDB(const PlayerInfo::Activity& info);

	bool GetFinishTime()
	{
		return m_finishTime.IsValid();
	}

	//是否达到最大关数
	bool IsHighestNode()
	{
		return (m_curNode >= ProtectedKingLoader::GetInstance()->GetMaxNode() ? true : false);
	}

	bool IsGetReward(int )
	{
		return false;
	}

	int GetCurNode()
	{
		return m_curNode;
	}

	int GetPastNode()
	{
		return m_pastNode;
	}

	void ResetProtectKing()
	{
		SetCurNode(0);
		m_swapstime = 0;
	}

	void GetProtectKingInfo(ActivityKing::AckClientKingInfo& info);

	bool IsHadReward(int node)
	{
		return (m_reward & (unsigned int)(1 << node)) == 0;
	}

	void SetHadReward(int node)
	{
		m_reward &= ~(1 << node);
	}
	//增加当前的关数
	void AddCurNode();


	int StartSweep(int sceneId);

	void EndSweep(void * arg);

	bool IsInSweep()
	{
		return m_finishTime.IsValid();
	}

	bool IsCanSweep()
	{
		return m_curNode == m_pastNode;
	}

	int64 GetLeftTime();

	void SetLeftTime(int64 time)
	{
		m_lefttime = time;
	}

	void SetSwapstime(int64 time)
	{
		m_swapstime = time;
	}

	void SetCurNode(int curNode);

	bool IsCanEnterActive(int sceneId);
//	int RecoverResourceActive(int *num, sRecoverResource & resource);
	void GetYellowVipInfo(YellowVipInfo::ToClentYellowVipInfo& info);
	int GetYellowVipState(int activityId,int value);
	void SetYellowVipState(int activityId,int value,int state);
	int GetYellowVipReward(int activityId,int value);
	// 获取黄钻礼包中未领取奖励的数量
	int YNoviceRewardNotGet();
	int YGrowthRewardNotGet();
	int YDailyRewardNotGet();
	int YExclusiveGirlNotGet();

	void SetPKGetAwardIndex(int index);
	int GetPKGetAwardIndex();
	void AddPKGetAwardIndex();
	void DecPKGetAwardIndex();

	//重置皇砖数据
	void ReSetYellowVipState();

	bool AddTreasonBestTime(int mapid,int elapseTime);

	void GetTreasonBestTime(TreasonKingInfo::ToClentTreasonInfo& info);

	int GetTreasonBestTime(int mapid);

	// 获取守护秦王未重置次数
	int GetPKUnResetNum();
	// 获取守护秦王剩余关卡
	int GetPKSuplusNode();
	// 获取剿灭反王剩余挑战次数
	int GetTreasonSurplusNum();
	// 获取剿灭反王剩余挑战关卡
	int GetTreansonSuplusPass();
	// 获取总挑战次数
	int GetTotalChallengeTrean();
	// 获取守护秦王总挑战次数
	int GetTotalResetPK();
	// 获取剿灭反王剩余重置次数 返回总重置次数
	int GetTreasonSurplusResetCnt(int& surplusReset);
private:
	Player * m_owner;
	map<int,map<int,int> > m_yellowVpInfo;
	TimerConnection m_finishTime; //守护秦王扫荡定时器
	int m_curNode; //当前的关数
	unsigned int m_reward; //守护秦王的奖励，根据奖励的id移位，最多只能32个等级的奖励
	WORD m_pastNode; //以往通关到的最大关数
	WORD m_PKAwardGetIndex; // 守护秦王奖励索引
	int64 m_swapstime; //扫荡开始时间
	int64 m_lefttime;
	int m_TreasonBestTime[TREASONKINGMAPLEVEL];// 反王副本历史最优时间
};


#endif /* ACTIVITYMGR_H_ */
