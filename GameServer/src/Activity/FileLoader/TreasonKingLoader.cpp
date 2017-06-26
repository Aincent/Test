/*
 * TreasonKingLoader.cpp
 *
 *  Created on: 2014年11月24日
 *      Author: root
 */

#include "TreasonKingLoader.h"
#include "CSVLoader.h"
#include "Log/Logger.h"
#include "StrArgPkg.h"

TreasonKingLoader::TreasonKingLoader():m_nOneGold(0),m_digBindType(0),m_nGoGolden(0),m_goRewardBindType(0),m_nDigNum(0),
	m_maxLevelMapID(0)
{
	m_pLua.OpenLua();
	m_mReward.clear();
	m_vLevelsGold.clear();
	m_mapDigGoldCost.clear();
	m_vQA.clear();
}

TreasonKingLoader::~TreasonKingLoader()
{
	m_mReward.clear();
	m_vLevelsGold.clear();
	m_mapDigGoldCost.clear();
	m_vQA.clear();
	m_pLua.CloseLua();
}

int TreasonKingLoader::Init(string &path)
{
	string probabilityFile = path + "Data/DigProTypeData.csv";
	string luaFile = path + "Data/TreasonKing.lua";

	if(InitProbability(probabilityFile) || InitLua(luaFile))
	{
		return -1;
	}

	return 0;
}

int TreasonKingLoader::InitProbability(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		count++;

		m_digBindType = loader.GetInt(i, count++);
//		m_nOneGold = loader.GetInt(i, count++);
		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);

		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			m_mapDigGoldCost[itemSize] = (atoi(getArg(funPkgItem, itemSize).c_str()));
		}
		m_nGoGolden = loader.GetInt(i, count++);
		m_nDigNum = loader.GetInt(i, count++);

		for(;count < 14; count ++)
		{
			string funStrItem = loader.GetString(i, count, "");
			StrArgPkg funPkgItem(":", funStrItem);
			if(funPkgItem.size() > 1)
			{
				QuantityArticle qa;
				qa.nArticle = atoi(getArg(funPkgItem, 0).c_str());
				qa.nNum = atoi(getArg(funPkgItem, 1).c_str());
				m_vQA.push_back(qa);
			}
		}

		m_goRewardBindType = loader.GetInt(i, count++);
	}

	return 0;
}

int TreasonKingLoader::InitLua(string & file)
{
	int levels = 0;
	int mapid = 0;

	if(m_pLua.LoadFile(file) != 0 || m_pLua.Call() != 0)
	{
		return -1;
	}

	try
	{
		for(int i = 1 ; i < 10; i++)
		{
			m_pLua.SetLuaFunction("GetGoGolden");
			m_pLua.SetInt(i);
			m_pLua.SetInt(m_nGoGolden);
			if(m_pLua.Call(2 ,1))
			{
				return -1;
			}
			m_vLevelsGold.push_back(m_pLua.GetInt());
		}


//		for(int i = 1 ; i < 10; i++)
//		{
//			m_pLua.SetLuaFunction("GetOneGolden");
//			m_pLua.SetInt(i);
//			m_pLua.SetInt(m_nOneGold);
//			if(m_pLua.Call(2 ,1))
//			{
//				return -1;
//			}
//			m_mapDigGoldCost[i] = m_pLua.GetInt();
//		}

		m_pLua.SetLuaFunction("GetLevelsMapId");
		if(m_pLua.Call(0, 1))
		{
			return -1;
		}

		int tableCount = m_pLua.GetTableFieldCount();
		for(int i=1; i<=tableCount; ++i)
		{
			if (m_pLua.GetFieldTable(i) != 0)
			{
				return -1;
			}

			int twCount = m_pLua.GetTableFieldCount();
			for(int j=1; j<=twCount; ++j)
			{
				if (m_pLua.GetFieldTable(j) != 0)
				{
					return -1;
				}

				levels = m_pLua.GetFieldInt(1);
				mapid = m_pLua.GetFieldInt(2);
				m_mLevelsMap[levels] = mapid;

				m_pLua.CloseTable();
			}
		}

		m_pLua.SetLuaFunction("GetMaxLevelMapID");
		if (m_pLua.Call(0, 1))
		{
			return -1;
		}
		m_maxLevelMapID = m_pLua.GetInt();

		m_pLua.CloseTable();

	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "arena and get RankingSize error [errormsg=%s]", e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "arena and get RankingSize unknown error");
	}
	return 0;
}

void TreasonKingLoader::GetReward(DWORD nLevels, QuantityArticle& qa)
{
	if(m_vQA.size() >= nLevels)
	{
		qa = m_vQA[nLevels];
	}
}

int TreasonKingLoader::GetLevelsMapId(int nLevels)
{
	map<int,int>::iterator itr =   m_mLevelsMap.find(nLevels);
	if(itr != m_mLevelsMap.end())
	{
		return itr->second;
	}

	return 0;
}

int TreasonKingLoader::GetMaxLevelMapID()
{
	return m_maxLevelMapID;
}

int TreasonKingLoader::GetMaxLevel()
{
	return m_mLevelsMap.rbegin()->first;
}
