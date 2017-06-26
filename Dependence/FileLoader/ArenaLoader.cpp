/*
 * ArenaLoader.cpp
 *
 *  Created on: 2015年11月21日
 *      Author: root
 */

#include "ArenaLoader.h"
#include "../CSVLoader.h"
#include "../StrArgPkg.h"
#include "Lua/LuaInterface.h"
#include "Log/Logger.h"

//const int RankRollReg::Interval::EndPointNum = 2;

ArenaLoader::ArenaLoader()
{
	// TODO Auto-generated constructor stub
	m_lua.OpenLua();
}

ArenaLoader::~ArenaLoader()
{
	// TODO Auto-generated destructor stub
	m_lua.CloseLua();
}


int ArenaLoader::Init(string& path)
{
	string luaFile = path + "./ArenaInfo.lua";
	if (0 != InitLua(luaFile))
		return -1;

//	string refreshFile = path + "./AthleticsrefreshData.csv";
//	if (0 != InitRefresh(refreshFile))
//		return -1;

	string arenanumFile = path + "./Arenanum.csv";
	if (0 != InitArenanumInfo(arenanumFile))
		return -1;


	string rewardFile = path + "./GoldRewardTypeData.csv";
	if (0 != InitRewardInfo(rewardFile))
		return -1;

	InitOther();



	return 0;
}
int ArenaLoader::InitRefresh(string& file)
{
	CSVLoader loader;
	if (!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	int col = 0;
	for (uint i = 0; i < loader.GetRowCount(); ++i)
	{
		ArenaParam::RefreshInfo info;
		col = 0;
		info.refreshNum = loader.GetInt(i, col++);
		info.costGolden = loader.GetInt(i, col++);

		m_refineInfo[info.refreshNum] = info;
	}

	return 0;
}

int ArenaLoader::InitRewardInfo(string& file)
{
	CSVLoader loader;
	if (!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	int col = 0;
	for (uint i = 0; i < loader.GetRowCount(); ++i)
	{
		ArenaParam::RewardInfo info;
		col = 0;
		info.nId = loader.GetInt(i, col++);
		info.nType = loader.GetInt(i, col++);
		info.nMin = loader.GetInt(i, col++);
		info.nMax = loader.GetInt(i, col++);
		info.nMoney = loader.GetInt(i, col++);
		info.nCredit = loader.GetInt(i, col++);
		string funStrItem = loader.GetString(i, col++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			for(unsigned int i=0; i<limitPkg.size(); ++i)
			{
				info.m_nAward.push_back(atoi(getArg(limitPkg, i++).c_str()));
				info.m_nAwardNum.push_back(atoi(getArg(limitPkg, i).c_str()));
			}
		}

		m_vRewradInfo.push_back(info);
	}

	return 0;
}

int ArenaLoader::InitArenanumInfo(string& file)
{
	CSVLoader loader;
	if (!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for (uint i = 0; i < loader.GetRowCount(); ++i)
	{
		int col = 0;
		int nId = loader.GetInt(i, col++);
		int nGolden = loader.GetInt(i, col++);
		m_AddNumInfo[nId] =nGolden;
	}

	return 0;
}

int ArenaLoader::InitLua(string& file)
{
	if(m_lua.LoadFile(file) != 0 || m_lua.Call() != 0)
	{
		return -1;
	}

	if (0 != m_lua.SetLuaFunction("GetRobotTotalNum") || 0 != m_lua.Call(0, 3))
	{
		return -1;
	}

	m_realArenaOpenCond = m_lua.GetInt();
	m_robotTotalPlayerNum = m_lua.GetInt();
	m_maxCanChallengePlayer = m_lua.GetInt();

	if (0 != m_lua.SetLuaFunction("GetChallengeCredit") || 0 != m_lua.Call(0, 2))
	{
		return -1;
	}

	m_challengeCredit.loseChanllengeCre = m_lua.GetInt();
	m_challengeCredit.winChanllengeCre = m_lua.GetInt();

	if (0 != m_lua.SetLuaFunction("GetArenaBase") || 0 != m_lua.Call(0, 2))
	{
		return -1;
	}

	m_monBeginCountDown = m_lua.GetInt();

	for (int i = 1; i <= m_lua.GetTableFieldCount(); ++i)
	{
		m_winRankMarquee.push_back(m_lua.GetFieldInt(i));
	}

	m_lua.CloseTable();

	return 0;
}

void ArenaLoader::InitOther()
{
	for (int i = 0; i < m_robotTotalPlayerNum; ++i)
	{
		m_robotSimulation.push_back(i+1);
	}
}

int ArenaLoader::GetRewardId(int nRanking, int nType)
{
	vector<ArenaParam::RewardInfo>::iterator itr = m_vRewradInfo.begin();
	for(; itr != m_vRewradInfo.end();++itr)
	{
		if((itr->nType == nType) && (itr->nMin >= nRanking && itr->nMax <= nRanking))
		{
			return itr->nId;
		}
	}

	return 0;
}

const ArenaParam::RewardInfo* ArenaLoader::GetGetRewardById(int nId)
{
	vector<ArenaParam::RewardInfo>::iterator itr = m_vRewradInfo.begin();
	for(; itr != m_vRewradInfo.end();++itr)
	{
		if(itr->nId == nId)
		{
			return &(*itr);
		}
	}

	return 0;
}

const ArenaParam::RewardInfo* ArenaLoader::GetGetReward(int nRanking, int nType)
{
	vector<ArenaParam::RewardInfo>::iterator itr = m_vRewradInfo.begin();
	for(; itr != m_vRewradInfo.end();++itr)
	{
		if((itr->nType == nType) && (itr->nMin >= nRanking && itr->nMax <= nRanking))
		{
			return &(*itr);
		}
	}

	return 0;
}

int ArenaLoader::GetArenanumInfo(int nId,int& golden)
{
	map<int,int>::iterator itr = m_AddNumInfo.find(nId);
	if(itr != m_AddNumInfo.end())
	{
		golden =  itr->second;

		return 0;
	}

	return -1;
}

int ArenaLoader::GetRefreshInfo(int num, ArenaParam::RefreshInfo& info)
{
	int ret = -1;
	map<int, ArenaParam::RefreshInfo>::iterator it = m_refineInfo.find(num);
	if (it != m_refineInfo.end())
	{
		info = it->second;
		ret = 0;
	}
	else
	{ // 如果没有这个数
		if (0 < m_refineInfo.size())
		{
			--it;
			if (it != m_refineInfo.end() && it->first <= num)
			{ // 有可能存在这个数大于表里最大的数 那么取这个数据
				info = it->second;
				ret = 0;
			}
		}

	}

	return ret;
}

int ArenaLoader::GetRobotTotalNum()
{
	return m_robotTotalPlayerNum;
}

int ArenaLoader::GetMaxChallengePlayer()
{
	return m_maxCanChallengePlayer;
}

void ArenaLoader::GetSimulationRank(vector<int>& data)
{
	data = m_robotSimulation;
}

//可挑战的6个对象
void ArenaLoader::GetRankingSize(DWORD nSelfRanking, int ceilRanking, vector<int>& vec)
{
	try
	{
		m_lua.SetLuaFunction("GetRankingSize");
		m_lua.SetInt(nSelfRanking);
		m_lua.SetInt(ceilRanking);
		if(m_lua.Call(2, 1))
		{
			return;
		}

		int fieldCount = m_lua.GetTableFieldCount();
		for(int i = 1; i <= fieldCount; i++)
		{
			vec.push_back(m_lua.GetFieldInt(i));
		}
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "arena and get RankingSize error [errormsg=%s]", e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "arena and get RankingSize unknown error");
	}
}


//挑战CD，为15分钟
int ArenaLoader::GetChallengeCDTime()
{
	int ret = 0;
	try
	{
		m_lua.SetLuaFunction("GetChallengeCDTime");
		if(m_lua.Call(0 ,1))
		{
			return 0;
		}

		ret = m_lua.GetInt();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "arena and get ChallengeCDTime error [errormsg=%s]", e.what());

		return 0;
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "arena and get ChallengeCDTime unknown error");

		return 0;
	}

	return ret;
}

//消除CD需要消耗金钱：0表示绑定元宝，1表示元宝
int ArenaLoader::GetResetChallengeCD()
{
	int ret = 0;
	try
	{
		m_lua.SetLuaFunction("GetResetChallengeCD");

		if(m_lua.Call(0, 1))
		{
			return 0;
		}

		ret = m_lua.GetInt();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "arena and get ResetChallengeCD error [errormsg=%s]", e.what());

		return 0;
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "arena and get ResetChallengeCD unknown error");

		return 0;
	}

	return ret;
}

//挑战胜利，获得经验
int ArenaLoader::GetChallengeWin(int64 lvExp)
{
	int ret = 0;
	try
	{
		m_lua.SetLuaFunction("GetChallengeWin");
		m_lua.SetInt(lvExp);
		if(m_lua.Call(1, 1))
		{
			return 0;
		}
		ret = m_lua.GetInt();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "arena and get ChallengeWin error [errormsg=%s]", e.what());

		return 0;
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "arena and get ChallengeWin unknown error");

		return 0;
	}

	return ret;
}


//挑战失败，获得经验
int ArenaLoader::GetChallengeLose(int64 lvExp)
{
	int ret = 0;
	try
	{
		m_lua.SetLuaFunction("GetChallengeLose");
		m_lua.SetInt(lvExp);
		if(m_lua.Call(1, 1))
		{
			return 0;
		}

		ret = m_lua.GetInt();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "arena and get ChallengeLose error [errormsg=%s]", e.what());

		return 0;
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "arena and get ChallengeLose unknown error");

		return 0;
	}

	return ret;
}

//排名奖励铜币
int ArenaLoader::GetRankingMoneyReward(DWORD nSelfRanking)
{
	int ret = 0;
	try
	{
		m_lua.SetLuaFunction("GetRankingMoneyReward");
		m_lua.SetInt(nSelfRanking);
		if(m_lua.Call(1, 1))
		{
			return 0;
		}
		ret = m_lua.GetInt();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "arena and get RankingReward error [errormsg=%s]", e.what());

		return 0;
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "arena and get RankingReward unknown error");

		return 0;
	}

	return ret;
}

//获取竞技场频道
int ArenaLoader::GetChannelType()
{
	int ret = 0;
	try
	{
		m_lua.SetLuaFunction("GetChannelType");
		if(m_lua.Call(0, 1))
		{
			return 0;
		}

		ret = m_lua.GetInt();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "arena and get ChannelType error [errormsg=%s]", e.what());

		return 0;
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "arena and get ChannelType unknown error");

		return 0;
	}

	return ret;
}


//获取增加挑战次数所要golden
int ArenaLoader::GetChallengTimes(int times, int& golden, int& addNum)
{
	try
	{
		m_lua.SetLuaFunction("GetChallengTimes");
		m_lua.SetInt(times);
		if(m_lua.Call(1, 2))
		{
			return -1;
		}

		addNum = m_lua.GetInt();
		golden = m_lua.GetInt();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "arena and get ChallengTimes golden  error [errormsg=%s]", e.what());

		return -1;
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "arena and get ChallengTimes golden unknown error");

		return -1;
	}

	return 0;
}


int ArenaLoader::GetRankingExpReward(DWORD nSelfRanking)
{
	int ret = 0;
	try
	{
		m_lua.SetLuaFunction("GetRankingExpReward");
		m_lua.SetInt(nSelfRanking);
		if(m_lua.Call(1, 1))
		{
			return 0;
		}
		ret = m_lua.GetInt();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "arena and get RankingGoldenReward error [errormsg=%s]", e.what());

		return 0;
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "arena and get RankingGoldenReward unknown error");

		return 0;
	}
	return ret;
}

int ArenaLoader::GetPrestigeReward(DWORD nSelfRanking)
{
	int ret = 0;
	try
	{
		m_lua.SetLuaFunction("GetRankingPesidenReward");
		m_lua.SetInt(nSelfRanking);
		if(m_lua.Call(1, 1))
		{
			return 0;
		}
		ret = m_lua.GetInt();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "arena and get GetPrestigeReward error [errormsg=%s]", e.what());

		return 0;
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "arena and get GetPrestigeReward unknown error");

		return 0;
	}
	return ret;
}

// 获取假排名
int ArenaLoader::GetFakeRanking(int realRank)
{
	int ret = 0;
	try
	{
		m_lua.SetLuaFunction("GetFakeRanking");
		m_lua.SetInt(realRank);
		if(m_lua.Call(1, 1))
		{
			return 0;
		}
		ret = m_lua.GetInt();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "arena and get GetFakeRanking error [errormsg=%s]", e.what());

		return 0;
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "arena and get GetFakeRanking unknown error");

		return 0;
	}
	return ret;
}

int ArenaLoader::GetArenaRealDataComd()
{
	return m_realArenaOpenCond;
}

// 获取竞技场挑战声望奖励
int ArenaLoader::GetChallengeCredit(bool isWin)
{
	return m_challengeCredit.GetChallengeCredit(isWin);
}

bool ArenaLoader::RankIsMarquee(int rank)
{
	for (uint i = 0; i < m_winRankMarquee.size(); ++i)
	{
		if (m_winRankMarquee[i] == rank)
		{
			return true;
		}
	}

	return false;
}

int ArenaLoader::GetMonCountDown()
{
	return m_monBeginCountDown;
}

