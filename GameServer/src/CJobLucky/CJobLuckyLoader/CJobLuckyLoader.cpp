/*
 * CJobLuckyLoader.cpp
 *
 *  Created on: 2014年8月14日
 *      Author: helh
 */

#include "CJobLuckyLoader.h"
#include "Log/Logger.h"
#include "CSVLoader.h"


CJobLuckyLoader::CJobLuckyLoader():m_oneLvCost(0),m_twoLvCost(0),m_threeLvCost(0),m_fourLvCost(0),m_fiveLvCost(0),m_FourLvCostGold(0)
{
	//读取配置文件
	m_lua.OpenLua();
}

CJobLuckyLoader::~CJobLuckyLoader()
{
	m_lua.CloseLua();
}

int CJobLuckyLoader::Init(string & path)
{
	string convert = path + "Data/ConvertTypeData.csv";
	string official = path + "Data/OfficialTypeData.csv";
	string cost = path + "Data/CostTypeData.lua";
	int ret = 0;

	ret = InitConvert(convert);
	if (ret < 0)
		return ret;

	ret = InitOfficial(official);
	if (ret < 0)
		return ret;

	ret = InitCost(cost);
	if (ret < 0)
		return ret;

	return 0;
}

int CJobLuckyLoader::InitConvert(string & file)
{
	CSVLoader loader;

	if (!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for (unsigned int i = 0; i < loader.GetRowCount(); ++i)
	{
		int count = 0;
		int id = 0;

		count++;
		id = loader.GetInt(i, count++);
		m_covert[id] = loader.GetInt(i, count++);
	}

	return 0;
}

int CJobLuckyLoader::InitOfficial(string & file)
{
	CSVLoader loader;

	if (!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for (unsigned int i = 0; i < loader.GetRowCount(); ++i)
	{
		int count = 0;
		OfficialItemInfo info;

		info.id = loader.GetInt(i, count++);
		count++;
		count++;
		info.quality = loader.GetInt(i, count++);
		info.lv = loader.GetInt(i, count++);
		info.eatexp = loader.GetInt(i, count++);
		info.addexp = loader.GetInt(i, count++);
		info.sale = loader.GetInt(i, count++);

		info.m_AttList[eMuscleMan][eCharHPMax] = loader.GetInt(i, count++);
		info.m_AttList[eMuscleMan][eCharPhyAttackMax] = loader.GetInt(i, count++);
		info.m_AttList[eMuscleMan][eCharMPMax] = loader.GetInt(i, count++);
		info.m_AttList[eMuscleMan][eCharPhyDefence] = loader.GetInt(i, count++);
		++count;
		info.m_AttList[eMuscleMan][eCharHitRate] = loader.GetInt(i, count++);
		info.m_AttList[eMuscleMan][eCharAvoidRate] = loader.GetInt(i, count++);
		info.m_AttList[eMuscleMan][eCrackDownRate] = loader.GetInt(i, count++);
		info.m_AttList[eMuscleMan][eAvoidCrackRate] = loader.GetInt(i, count++);

		count++;
		info.type = loader.GetInt(i,count++);

		info.m_AttList[eMagicHuman][eCharHPMax] = loader.GetInt(i, count++);
		info.m_AttList[eMagicHuman][eCharPhyAttackMax] = loader.GetInt(i, count++);
		info.m_AttList[eMagicHuman][eCharMPMax] = loader.GetInt(i, count++);
		info.m_AttList[eMagicHuman][eCharPhyDefence] = loader.GetInt(i, count++);
		++count;
		info.m_AttList[eMagicHuman][eCharHitRate] = loader.GetInt(i, count++);
		info.m_AttList[eMagicHuman][eCharAvoidRate] = loader.GetInt(i, count++);
		info.m_AttList[eMagicHuman][eCrackDownRate] = loader.GetInt(i, count++);
		info.m_AttList[eMagicHuman][eAvoidCrackRate] = loader.GetInt(i, count++);

		info.m_AttList[eShooter][eCharHPMax] = loader.GetInt(i, count++);
		info.m_AttList[eShooter][eCharPhyAttackMax] = loader.GetInt(i, count++);
		info.m_AttList[eShooter][eCharMPMax] = loader.GetInt(i, count++);
		info.m_AttList[eShooter][eCharPhyDefence] = loader.GetInt(i, count++);
		++count;
		info.m_AttList[eShooter][eCharHitRate] = loader.GetInt(i, count++);
		info.m_AttList[eShooter][eCharAvoidRate] = loader.GetInt(i, count++);
		info.m_AttList[eShooter][eCrackDownRate] = loader.GetInt(i, count++);
		info.m_AttList[eShooter][eAvoidCrackRate] = loader.GetInt(i, count++);

		m_officialInfo[info.id] = info;
	}

	return 0;
}

int CJobLuckyLoader::InitCost(string & file)
{
	if (m_lua.LoadFile(file) != 0 || m_lua.Call() != 0)
	{
		return -1;
	}

	m_lua.SetLuaFunction("GetOfficialUse");

	if (m_lua.Call(0, 6))
	{
		return -1;
	}

	m_FourLvCostGold = m_lua.GetInt();
	m_fiveLvCost = m_lua.GetInt();
	m_fourLvCost = m_lua.GetInt();
	m_threeLvCost = m_lua.GetInt();
	m_twoLvCost = m_lua.GetInt();
	m_oneLvCost = m_lua.GetInt();

	return 0;
}

void CJobLuckyLoader::GetRandCJob(int index, int &id, int &nextIndex)
{
	try
	{
		int randf = rand() % 10000 + 1;
		int rands = rand() % 10000 + 1;
		int randt = rand() % 10000 + 1;

		m_lua.SetLuaFunction("GetRandCJob");
		m_lua.SetInt(index);

		m_lua.SetInt(randf);
		m_lua.SetInt(rands);
		m_lua.SetInt(randt);

		if (m_lua.Call(4, 2))
		{
			return;
		}

		nextIndex = m_lua.GetInt();
		id = m_lua.GetInt();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "get rand cjob error[%s]",e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "get rand cjob unknown error");
	}
}


