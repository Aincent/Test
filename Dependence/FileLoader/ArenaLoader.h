/*
 * ArenaLoader.h
 *
 *  Created on: 2015年11月21日
 *      Author: root
 */

#ifndef ARENALOADER_H_
#define ARENALOADER_H_

#include "Singleton.h"
#include "define.h"
#include "Lua/LuaInterface.h"
#include <map>
#include <vector>

namespace ArenaParam
{
	typedef struct sRefreshInfo
	{
		int refreshNum;
		int costGolden;

		sRefreshInfo() : refreshNum(0), costGolden(0)
		{

		}
	}RefreshInfo;

	typedef struct sRewardInfo
	{
		int nId;
		int nType;
		int nMin;
		int nMax;
		int nMoney;
		int nCredit;
		vector<int> m_nAward;			//奖励物品ID
		vector<int> m_nAwardNum;			//对应奖励物品数量

		sRewardInfo():nId(0),nType(0),nMin(0),nMax(0),nMoney(0),nCredit(0)
		{
			m_nAward.clear();
			m_nAwardNum.clear();
		}

	}RewardInfo;

	typedef struct sChallengeCredit
	{
		int winChanllengeCre;
		int loseChanllengeCre;
		sChallengeCredit() : winChanllengeCre(0), loseChanllengeCre(0)
		{
		}
		int GetChallengeCredit(bool isWin)
		{
			if (isWin)
			{
				return winChanllengeCre;
			}
			else
			{
				return loseChanllengeCre;
			}
		}
	}ChallengeCredit;
//	typedef struct sRankRollReg
//	{
//		typedef struct sInterval
//		{
//			static const int EndPointNum;
//			int _ceil;
//			int _floor;
//			sInterval() : _ceil(0), _floor(0)
//			{
//			}
//			sInterval(int ceil_, int floor_) : _ceil(ceil_), _floor(floor_)
//			{
//			}
//		}Interval;
//
//		struct sInterval interval; // 当前区间
//		int num;
//		vector<struct sInterval> otherInterval; // 其他区间
//
//		sRankRollReg() : interval(sInterval()), num(0)
//		{
//			otherInterval.clear();
//		}
//	}RankRollReg;

}

using namespace CommBaseOut;


class ArenaLoader : public Singleton<ArenaLoader>
{

public:
	ArenaLoader();
	~ArenaLoader();

public:
	int Init(string& path);
	int InitRefresh(string& file);
	int InitRewardInfo(string& file);
	int InitArenanumInfo(string& file);
//	int InitRankRollReg(string& file);
	int InitLua(string& file);
	void InitOther();

//	vector<RankRollReg::Interval> GetOtherRankingInter(int curRank);
	int GetRewardId(int nRanking, int nType = 2);
	const ArenaParam::RewardInfo* GetGetRewardById(int nId);
	const ArenaParam::RewardInfo* GetGetReward(int nRanking, int nType = 2);

	int GetArenanumInfo(int nId,int& golden);

	int GetRefreshInfo(int num, ArenaParam::RefreshInfo& info);
	int GetRobotTotalNum();
	int GetMaxChallengePlayer();
	void GetSimulationRank(vector<int>& data);
	// 获取竞技场真实数据条件 挑战竞技场模拟数据次数
	int GetArenaRealDataComd();
	// 获取竞技场挑战声望奖励
	int GetChallengeCredit(bool isWin);
	// 胜利排名是否可以广播跑马灯
	bool RankIsMarquee(int rank);
	// 获取竞技场刷怪倒计时 毫秒
	int GetMonCountDown();

	 // ---------------------------  lua call  ----------------
	//可挑战的6个对象
	void GetRankingSize(DWORD nSelfRanking, int ceilRanking, vector<int>& vec);
	//判断是否能够挑战
//	bool IsCanChalleng(DWORD nSelfRanking,DWORD nChallengeRanking);
	//挑战CD，为15分钟
	int GetChallengeCDTime();
	//消除CD需要消耗
	int GetResetChallengeCD();
	//挑战胜利，获得经验
	int GetChallengeWin(int64 lvExp);
	//挑战失败，获得经验
	int GetChallengeLose(int64 lvExp);
	//排名奖励铜币
	int GetRankingMoneyReward(DWORD nSelfRanking);
	//获取竞技场频道
	int GetChannelType();
	//获取增加挑战次数所要golden
	int GetChallengTimes(int times, int& golden, int& addNum);
	//获取奖励经验
	int GetRankingExpReward(DWORD nSelfRanking);
	//获取声望奖励
	int GetPrestigeReward(DWORD nSelfRanking);
	// 获取假排名
	int GetFakeRanking(int realRank);
	// ----------------------------- lua call -------------------

private:
	map<int, ArenaParam::RefreshInfo> m_refineInfo;
//	vector<pair<RankRollReg::Interval, RankRollReg> > m_rankInterInfo; // 随机排名区间
	vector<int> m_robotSimulation; // 模拟机器人排行 随机Arena的假数据时用到 相关数据 数据库表ArenaTemplate
	int m_robotTotalPlayerNum; // 模拟机器人的个数
	int m_maxCanChallengePlayer;
	int m_realArenaOpenCond; // 竞技场真实数据开启条件  挑战假数据次数
	ArenaParam::ChallengeCredit m_challengeCredit; // 竞技场挑战声望奖励
	vector<int> m_winRankMarquee; // 竞技场胜利跑马灯的排名
	int m_monBeginCountDown; // 竞技场怪物开始倒计时
	vector<ArenaParam::RewardInfo> m_vRewradInfo;
	map<int,int> m_AddNumInfo;
	CLuaInterface m_lua;
};

#endif /* ARENALOADER_H_ */
