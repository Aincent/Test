/*
 * EscortFileLoader.cpp
 *
 *  Created on: 2014年7月21日
 *      Author: root
 */

#include "EscortFileLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "Log/Logger.h"
#include "MessageCommonRet.h"
#include <math.h>
#include "../../QuestNew/QuestDefine.h"

EscortLoader::EscortLoader()
{

}

EscortLoader::~EscortLoader()
{

}

int EscortLoader::InitEscort(string &file)
{
	CSVLoader loader;

	if (!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for (unsigned int i = 0; i < loader.GetRowCount(); ++i)
	{
		sEscortEntry info;
		int count = 0;

		info.m_nEscortID = loader.GetInt(i, count++);
		info.m_nStartNpcID = loader.GetInt(i, count++);
		info.m_nEndNpcID = loader.GetInt(i, count++);

		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);

		for (uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			info.m_nQualityExp.push_back(atoi(getArg(funPkgItem, itemSize).c_str()));
		}

		funStrItem = loader.GetString(i, count++, "");
		funPkgItem.setArgs(funStrItem);

		for (uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			info.m_nQualityCost.push_back(atoi(getArg(funPkgItem, itemSize).c_str()));
		}

		funStrItem = loader.GetString(i, count++, "");
		funPkgItem.setArgs(funStrItem);

		for (uint itemSize = 0; itemSize < funPkgItem.size(); ++itemSize)
		{
			info.m_nQuality1Probility.push_back(atoi(getArg(funPkgItem, itemSize).c_str()));
		}

		funStrItem = loader.GetString(i, count++, "");
		funPkgItem.setArgs(funStrItem);

		for (uint itemSize = 0;itemSize < funPkgItem.size(); ++itemSize)
		{
			info.m_nQuality2Probility.push_back(atoi(getArg(funPkgItem, itemSize).c_str()));
		}

		funStrItem = loader.GetString(i, count++, "");
		funPkgItem.setArgs(funStrItem);

		for (uint itemSize = 0;itemSize < funPkgItem.size(); ++itemSize)
		{
			info.m_nQuality3Probility.push_back(atoi(getArg(funPkgItem, itemSize).c_str()));
		}

		funStrItem = loader.GetString(i, count++, "");
		funPkgItem.setArgs(funStrItem);

		for (uint itemSize = 0;itemSize < funPkgItem.size(); ++itemSize)
		{
			info.m_nQuality4Probility.push_back(atoi(getArg(funPkgItem, itemSize).c_str()));
		}

		funStrItem = loader.GetString(i, count++, "");
		funPkgItem.setArgs(funStrItem);

		for (uint itemSize = 0;itemSize < funPkgItem.size(); ++itemSize)
		{
			info.m_nQuality5Probility.push_back(atoi(getArg(funPkgItem, itemSize).c_str()));
		}

		info.m_nFreshCost = loader.GetInt(i, count++);
		info.m_nFreeTimes = loader.GetInt(i, count++);
		info.m_nLimitTime = loader.GetInt(i, count++);
		info.m_nDayTimes = loader.GetInt(i, count++);
		info.m_nImediaComplete = loader.GetInt(i, count++);
		info.m_nSpeedUpCost = loader.GetInt(i, count++);
		info.m_nSpeedUpTime = loader.GetInt(i, count++);

		string tStrItem = loader.GetString(i, count++, "");
		StrArgPkg tPkgItem(":", tStrItem);

		for (uint itemSize = 0; itemSize < tPkgItem.size(); ++itemSize)
		{
			info.m_nDoubleExpStartTime.push_back(atoi(getArg(tPkgItem, itemSize).c_str()));
		}

		tStrItem = loader.GetString(i, count++, "");
		tPkgItem.setArgs(tStrItem);

		for (uint itemSize = 0;itemSize < tPkgItem.size(); ++itemSize)
		{
			info.m_nDoubleExpEndTime.push_back(atoi(getArg(tPkgItem, itemSize).c_str()));
		}

		tStrItem = loader.GetString(i, count++, "");
		funPkgItem.setArgs(tStrItem);

		for (uint itemSize = 0;itemSize < funPkgItem.size(); ++itemSize)
		{
			info.m_nCarType.push_back(atoi(getArg(funPkgItem, itemSize).c_str()));
		}

		tStrItem = loader.GetString(i, count++, "");
		funPkgItem.setArgs(tStrItem);

		for (uint itemSize = 0;itemSize < funPkgItem.size(); ++itemSize)
		{
			info.m_nByWayOfMap.push_back(atoi(getArg(funPkgItem, itemSize).c_str()));
		}

		info.m_nSpeedUpRate = loader.GetInt(i, count++);
		info.m_expRate1 = loader.GetInt(i, count++);
		info.m_expRate2 = loader.GetInt(i, count++);
		info.m_expRate3 = loader.GetInt(i, count++);

		info.m_money1 = loader.GetInt(i, count++);
		info.m_money2 = loader.GetInt(i, count++);
		info.m_money3 = loader.GetInt(i, count++);

		m_EscortInfo[info.m_nEscortID] = info;
	}

	return 0;
}

sEscortEntry *EscortLoader::GetEscortEntry(DWORD id)
{
	map<DWORD, sEscortEntry>::iterator it =  m_EscortInfo.find(id);
	if(it != m_EscortInfo.end())
	{
		return &(it->second);
	}

	return NULL;
}


