/*
 * CrossSvrCompetitionLoader.h
 *
 *  Created on: 2014年12月9日
 *      Author: root
 */

#ifndef CROSSSVRCOMPETITIONLOADER_H_
#define CROSSSVRCOMPETITIONLOADER_H_

#include "Singleton.h"
#include "define.h"
#include <vector>
#include "Lua/LuaInterface.h"

using namespace std;
using namespace CommBaseOut;

namespace CrossSvrCompetition
{
	const int Min_Versus_Mode = 1; // 1 VS 1

	// 位置坐标 x,y
	struct Point
	{
		int nPosX; //
		int nPosY; //
		Point(): nPosX(0), nPosY(0)
		{

		}
	};

	// 奖励物品ID及数量
	struct RewardGoodsInfo
	{
		int nGoodsID; // 奖励物品ID
		int nGoodsNum; // 奖励物品数量
		RewardGoodsInfo(): nGoodsID(0), nGoodsNum(0)
		{

		}
	};

	// 跨服竞技信息
	struct CrossSvrCompeteInfo
	{
		int nDanGradingID; // 段位ID
		int nScoreFloor; // 段位积分下限
		int nScoreCeiling; // 段位积分上限
		int nAwardExp; // 奖励的经验
		int nAwardMoney; // 奖励的铜钱
		std::vector<RewardGoodsInfo> vecRewardGoods; // 奖励物品
		CrossSvrCompeteInfo(): nDanGradingID(0), nScoreFloor(0), nScoreCeiling(0), nAwardExp(0), nAwardMoney(0)
		{
			vecRewardGoods.clear();
		}
	};
}

class CCrossSvrCompeteLoader : public Singleton<CCrossSvrCompeteLoader>
{
public:
	int Init(string &path);
	int InitCrossSvrCom(string& file);
	int InitLua(string& file);
	int GetOpenLv() // 获取开启等级
	{
		return m_nOpenLv;
	}
	int GetWaitMatchTime() // 获取等待匹配时间
	{
		return m_nWaitMatchTime;
	}
	int GetCompeteSumTime() // 获取竞赛总时间
	{
		return m_nCompeteSumTime;
	}
	int GetInSceneDelay() // 获得进入场景延迟时间
	{
		return m_nInSceneDelay;
	}
	int GetAcceptPrizeCountCeiling() // 获取领取奖励次数上限
	{
		return m_nAcceptPrizeCountCeiling;
	}
	int GetSingleInvit() //单人跨服竞技次数上限
	{
		return m_nSingleInvit;
	}
	int GetGroupInvit() //组队跨服竞技次数上限
	{
		return m_nGroupInvit;
	}
	int GetChallengeCDTime() //跨服竞技CD时间
	{
		return m_nchallengeCDTime;
	}

	// 获得竞赛场景地图ID param: IN OUT mapID, In 对战模式 默认值1  ( 1 VS 1)
	int GetCompeteMapID(int& mapID, int nVersusMode = 1);

	// 获得竞技场对手登场坐标 param: in 地图ID, IN num 人员序号(从1开始), OUT 对手的登场坐标Pos, IN 对战模式 默认值1 (1 VS 1)
	int GetCompeteDebutPos(int nMapID, int num, CrossSvrCompetition::Point& pos, int nVersusMode = 1);

	// 获得胜利奖励积分 param: in 胜利者段位ID, 失败者段位ID, out:nRetScore
	void GetVictoryRewardScore(int nWinnerDanGradingID, int nLoserDanGradingID, int& nRetScore);

	// 获得胜利奖励经验和金钱 param: in 段位ID, out nExp, out nMoney
	int GetVictroyRewardExpMoney(int nDanGradingID, int& nExp, int& nMoney);

	// 获得胜利奖励物品  param: in 段位ID, out vecRewardGoodsInfo
	int GetVictoryRewardGoods(int nDanGradingID, std::vector<struct CrossSvrCompetition::RewardGoodsInfo>& vecRewardGoodsInfo);
	int GetVictoryRewardGoods(int nDanGradingID, vector<DWORD> &itemid, vector<DWORD> &itemnum);

	//根据积分，获得，可以pk的最小和最大积分
	void GetCanPkScoreByMyScore(int score, int &bScore, int &eScore);

	// 根据积分获得段位ID
	int GetGradeIDByScore(int score);
	//根据积分获得段位
	int GetIDByScore(int score);

	// 获取最高段位ID
	int GetMaxGradeID();

	CCrossSvrCompeteLoader(): m_nchallengeCDTime(0),m_nSingleInvit(0),m_nGroupInvit(0),m_nOpenLv(0), m_nWaitMatchTime(0), m_nCompeteSumTime(0), m_nInSceneDelay(0), m_nAcceptPrizeCountCeiling(0),
			m_maxDanGradingID(0)
	{
		m_mapCrossSvrCompeteInfo.clear();
		m_mapCompeteMapID.clear();
		m_lua.OpenLua();
	}
	~CCrossSvrCompeteLoader()
	{
		m_mapCrossSvrCompeteInfo.clear();
		m_lua.CloseLua();
	}


private:
	int m_nchallengeCDTime; //跨服竞技CD时间
	int m_nSingleInvit; //单人跨服竞技次数上限
	int m_nGroupInvit;//组队跨服竞技次数上限
	int m_nOpenLv; // 开启等级
	int m_nWaitMatchTime; // 等待匹配时间
	int m_nCompeteSumTime; // 竞赛总时间
	int m_nInSceneDelay; // 进入竞赛场景延迟时间
	int m_nAcceptPrizeCountCeiling; // 领奖次数上限
	int m_maxDanGradingID; // 最大段位ID

	std::map<int, int> m_mapCompeteMapID; // 竞赛地图ID;
	std::map<int, struct CrossSvrCompetition::CrossSvrCompeteInfo> m_mapCrossSvrCompeteInfo; // 跨服竞技信息
	CLuaInterface m_lua; //

public:

};
#endif /* CROSSSVRCOMPETITIONLOADER_H_ */
