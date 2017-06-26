/*
 * CountryRedEnvelopeLoader.cpp
 *
 *  Created on: 2015年4月16日
 *      Author: root
 */
#include "CountryRedEnvelopeLoader.h"
//#include "CSVLoader.h"
#include "Log/Logger.h"
//#include "StrArgPkg.h"

int CountryRedEnvelopeLoader::Init(string& path)
{
	string strLuaFile = path + "Data/countryEnvelope.lua";
//	string strCSVFile = path + "Data/Country/EnvelopeRwardType.csv";

	if(InitLua(strLuaFile))// || InitCSVFile(strCSVFile)
	{
		return -1;
	}

	return 0;
}

int CountryRedEnvelopeLoader::InitLua(string& file)
{
	if(m_lua.LoadFile(file) != 0 || m_lua.Call() != 0)
	{
		return -1;
	}

	m_lua.SetLuaFunction("GetBaseParam");
	if(m_lua.Call(0, 12))
	{
		return -1;
	}

	m_maxCanAddEnvelopeCountLv = m_lua.GetInt();
	m_canJoinCountryLv = m_lua.GetInt();
	m_getEnvelopeCountPerLvCount = m_lua.GetInt();
	m_getenvelopeCountPer10lv = m_lua.GetInt();
	m_accCountGotRewardId = m_lua.GetInt();
	m_getCountGotRewardItem_Id = m_lua.GetInt();
	m_accCountGotReward = m_lua.GetInt();
	m_offlineGetBindGoldenCount = m_lua.GetInt();
	m_olineGetBindGoldenCount = m_lua.GetInt();
	m_rechargeAddEnvelope = m_lua.GetInt();
	m_deleteEnvelopeDays = m_lua.GetInt();
	m_deleteEnvelopeTime = m_lua.GetInt();

	return 0;
}

//int CountryRedEnvelopeLoader::InitCSVFile(string& file)
//{
//	CSVLoader loader;
//
//	if(!loader.OpenFromFile(file.c_str()))
//	{
//		return -1;
//	}
//
//	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
//	{
//		EnvelopeRewardInfo info;
//		int count = 0;
//
//		info.type = loader.GetInt(i, count++, 0);
//
//		string itemInclude = loader.GetString(i, count++, "");
//		StrArgPkg includePkgItem("|", itemInclude);
//		includePkgItem.setArgs(itemInclude);
//
//		for(uint itemSize=0; itemSize<includePkgItem.size(); ++itemSize)
//		{
//			StrArgPkg limitPkg(":", getArg(includePkgItem, itemSize).c_str());
//			for(unsigned int i=0; i<limitPkg.size(); ++i)
//			{
//				int j = i;
//				int k = ++i;
//				info.itemId.push_back(atoi(getArg(limitPkg, j).c_str()));
//				info.num.push_back(atoi(getArg(limitPkg, k).c_str()));
//			}
//		}
//
//		m_envelopeRewardType[info.type] = info;
//	}
//
//	return 0;
//}

int CountryRedEnvelopeLoader::GetCanGetEnvelopeCountMax(int lv)
{
	m_lua.SetLuaFunction("GetCanGetEnvelopeCountMax");
	m_lua.SetInt(lv);

	if(m_lua.Call(1, 1))
	{
		return 0;
	}
	int canGetEnvelopeCountMax = m_lua.GetInt();

	return canGetEnvelopeCountMax;
}

void CountryRedEnvelopeLoader::GetEnvelopeRewardByType(int type,EnvelopeRewardInfo &rewardInfo)
{
	std::map<int,EnvelopeRewardInfo>::iterator it = m_envelopeRewardType.find(type);
	if(it != m_envelopeRewardType.end())
	{
		rewardInfo = it->second;
	}
}
