/*
 * CrossSvrCompetitionLoader.cpp
 *
 *  Created on: 2014年12月9日
 *      Author: root
 */

#include "CrossSvrCompetitionLoader.h"
#include "CSVLoader.h"
#include "Log/Logger.h"
#include "StrArgPkg.h"

using namespace CrossSvrCompetition;

int CCrossSvrCompeteLoader::Init(string& path)
{
	string strCsvFile = path + "Data/CrossclothingTypeData.csv";
	string strLuaFile = path + "Data/BeyondBattle.lua";

	if(InitCrossSvrCom(strCsvFile) || InitLua(strLuaFile))
	{
		return -1;
	}

	return 0;
}

int CCrossSvrCompeteLoader::InitCrossSvrCom(string& file)
{
	CSVLoader loader;
	if (!loader.OpenFromFile(file.c_str()) )
	{
		return -1;
	}

	for (unsigned int i = 0; i < loader.GetRowCount(); i++)
	{
		struct CrossSvrCompeteInfo info;
		unsigned int index = 0;
		info.nDanGradingID = loader.GetInt(i, index++);
		index++;
		info.nScoreFloor = loader.GetInt(i, index++);
		info.nScoreCeiling = loader.GetInt(i, index++);
		info.nAwardExp = loader.GetInt(i, index++);
		info.nAwardMoney = loader.GetInt(i, index++);

		std::string strItem = loader.GetString(i, index++, " ");
		StrArgPkg pkgItem("|", strItem);

		for (unsigned int j = 0; j < pkgItem.size(); j++)
		{
			RewardGoodsInfo goodsInfo;

			StrArgPkg pkgGoodsItem(":", getArg(pkgItem, j).c_str());

			goodsInfo.nGoodsID = atoi(getArg(pkgGoodsItem, 0).c_str());
			goodsInfo.nGoodsNum = atoi(getArg(pkgGoodsItem, 1).c_str());

			info.vecRewardGoods.push_back(goodsInfo);
		}

		m_mapCrossSvrCompeteInfo[info.nDanGradingID] = info;

	}

	return 0;
}

int CCrossSvrCompeteLoader::InitLua(string& file)
{
	if(m_lua.LoadFile(file) != 0 || m_lua.Call() != 0)
	{
		return -1;
	}

	m_lua.SetLuaFunction("GetBaseParam");

	if(m_lua.Call(0, 10))
	{
		return -1;
	}

	m_maxDanGradingID = m_lua.GetInt(); // 最高段位ID

	int nVersusCount = m_lua.GetTableFieldCount();
	for (int i = Min_Versus_Mode; i <= nVersusCount; ++i)
	{
		if (0 != m_lua.GetFieldTable(i))
		{
			return -1;
		}


		m_mapCompeteMapID[i] = m_lua.GetFieldInt(1);// 竞赛地图ID

		m_lua.CloseTable();
	}

	m_lua.CloseTable();


	m_nAcceptPrizeCountCeiling = m_lua.GetInt(); // 领奖次数上限
	m_nInSceneDelay = m_lua.GetInt(); // 进入竞赛场景延迟时间
	m_nCompeteSumTime = m_lua.GetInt(); // 竞赛总时间
	m_nWaitMatchTime = m_lua.GetInt(); // 等待匹配的时间
	m_nOpenLv = m_lua.GetInt(); // 开启等级
	m_nGroupInvit = m_lua.GetInt(); //组队跨服竞技次数上限
	m_nSingleInvit = m_lua.GetInt();//单人跨服竞技次数上限
	m_nchallengeCDTime =  m_lua.GetInt(); //跨服竞技CD时间
//	m_lua.CloseTable();

	return 0;
}

// 获得竞赛场景地图ID
int CCrossSvrCompeteLoader::GetCompeteMapID(int& mapID, int nVersusMode)
{
	if (CrossSvrCompetition::Min_Versus_Mode > nVersusMode)
	{
		return -1;
	}

	mapID = m_mapCompeteMapID[nVersusMode];

	return 0;
}

// 获得竞技场对手们登场坐标
int CCrossSvrCompeteLoader::GetCompeteDebutPos(int nMapID, int num, CrossSvrCompetition::Point& pos, int nVersusMode)
{
	if (CrossSvrCompetition::Min_Versus_Mode > nVersusMode)
	{
		return -1;
	}

	try
	{

		m_lua.SetLuaFunction("GetMapDebutPos");
		m_lua.SetInt(nVersusMode);
		m_lua.SetInt(nMapID);
		if(m_lua.Call(2, 1))
		{
			return -1;
		}

		if (0 != m_lua.GetFieldTable(num))
		{
			return -1;
		}

		pos.nPosX = m_lua.GetFieldInt("xPos");
		pos.nPosY = m_lua.GetFieldInt("yPos");


//		m_lua.CloseTable();

		m_lua.CloseTable();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "get victory reward score error[%s]",e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "get victory reward score error");
	}

	return 0;
}

// 获取胜利奖励积分
void CCrossSvrCompeteLoader::GetVictoryRewardScore(int nWinnerDanGradingID, int nLoserDanGradingID, int& nRetScore)
{
	try
	{
		m_lua.SetLuaFunction("GetVictoryScore");
		m_lua.SetInt(nWinnerDanGradingID);
		m_lua.SetInt(nLoserDanGradingID);

		if(m_lua.Call(2, 1))
		{
			return ;
		}

		nRetScore = m_lua.GetInt();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "get victory reward score error[%s]",e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "get victory reward score error");
	}

}

// 获得胜利奖励经验和金钱
int CCrossSvrCompeteLoader::GetVictroyRewardExpMoney(int nDanGradingID, int& nExp, int& nMoney)
{
	if (m_mapCrossSvrCompeteInfo.size() <= 0)
	{
		return -1;
	}

	std::map<int, struct CrossSvrCompetition::CrossSvrCompeteInfo>::iterator it = m_mapCrossSvrCompeteInfo.find(nDanGradingID);

	if(it != m_mapCrossSvrCompeteInfo.end())
	{
		nExp = it->second.nAwardExp;
		nMoney = it->second.nAwardMoney;
	}

	return 0;
}

// 获得胜利奖励物品
int CCrossSvrCompeteLoader::GetVictoryRewardGoods(int nDanGradingID, std::vector<struct CrossSvrCompetition::RewardGoodsInfo>& vecRewardGoodsInfo)
{
	if (m_mapCrossSvrCompeteInfo.size() <= 0)
	{
		return -1;
	}

	std::map<int, struct CrossSvrCompetition::CrossSvrCompeteInfo>::iterator it = m_mapCrossSvrCompeteInfo.find(nDanGradingID);

	if(it != m_mapCrossSvrCompeteInfo.end())
	{
		vecRewardGoodsInfo = it->second.vecRewardGoods;
	}

	return 0;
}

int CCrossSvrCompeteLoader::GetVictoryRewardGoods(int nDanGradingID, vector<DWORD> &itemid, vector<DWORD> &itemnum)
{
	if (nDanGradingID <= 0 || m_mapCrossSvrCompeteInfo.size() <= 0 || (size_t)nDanGradingID > m_mapCrossSvrCompeteInfo.size())
	{
		return -1;
	}

	std::vector<struct CrossSvrCompetition::RewardGoodsInfo> vecRewardGoodsInfo;

	std::map<int, struct CrossSvrCompetition::CrossSvrCompeteInfo>::iterator itf = m_mapCrossSvrCompeteInfo.find(nDanGradingID);

	if(itf != m_mapCrossSvrCompeteInfo.end())
	{
		vecRewardGoodsInfo = itf->second.vecRewardGoods;
	}

	vector<struct CrossSvrCompetition::RewardGoodsInfo>::iterator it = vecRewardGoodsInfo.begin();
	for(; it!=vecRewardGoodsInfo.end(); ++it)
	{
		itemid.push_back((*it).nGoodsID);
		itemnum.push_back((*it).nGoodsNum);
	}

	return 0;
}

void CCrossSvrCompeteLoader::GetCanPkScoreByMyScore(int score, int &bScore, int &eScore)
{
	int index = m_mapCrossSvrCompeteInfo.size();
	int bindex = -1;//最低的序号，比段位id小1
	int eindex = -1;//最高的序号，比段位id小1

	map<int, struct CrossSvrCompetition::CrossSvrCompeteInfo>::iterator it = m_mapCrossSvrCompeteInfo.begin();
	for(; it!=m_mapCrossSvrCompeteInfo.end(); ++it)
	{
		if(score >= it->second.nScoreFloor && score <= it->second.nScoreCeiling)
		{
			index = it->second.nDanGradingID;
		}
	}

	bindex = ((index - 1) / 3) * 3 - 3;
	eindex = ((index - 1) / 3 + 1) * 3 + 2;
	if(bindex< 0)
		bindex = 0;

	if(eindex >= (int)m_mapCrossSvrCompeteInfo.size())
		eindex = m_mapCrossSvrCompeteInfo.size() - 1;

	std::map<int, struct CrossSvrCompetition::CrossSvrCompeteInfo>::iterator itf = m_mapCrossSvrCompeteInfo.find(bindex + 1);

	if(itf != m_mapCrossSvrCompeteInfo.end())
	{
		bScore = itf->second.nScoreFloor;
		eScore = itf->second.nScoreCeiling;
	}


}

int CCrossSvrCompeteLoader::GetGradeIDByScore(int score)
{
	int index = 0;
	map<int, struct CrossSvrCompetition::CrossSvrCompeteInfo>::iterator it = m_mapCrossSvrCompeteInfo.begin();
	for ( ; it != m_mapCrossSvrCompeteInfo.end(); ++it)
	{
		if (score >= it->second.nScoreFloor && score <= it->second.nScoreCeiling)
		{
			index = it->second.nDanGradingID;
		}
	}

	return index;
}

int CCrossSvrCompeteLoader::GetIDByScore(int score)
{
	int index = (m_mapCrossSvrCompeteInfo.size() - 1) / 3;

	map<int, struct CrossSvrCompetition::CrossSvrCompeteInfo>::iterator it = m_mapCrossSvrCompeteInfo.begin();
	for(; it!=m_mapCrossSvrCompeteInfo.end(); ++it)
	{
		if(score >= it->second.nScoreFloor && score <= it->second.nScoreCeiling)
		{
			index = (it->second.nDanGradingID - 1) / 3;
		}
	}

	return index + 1;
}

// 获取最高段位ID
int CCrossSvrCompeteLoader::GetMaxGradeID()
{
		return m_maxDanGradingID;
}

