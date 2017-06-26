/*
 * VitalityLoader.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: root
 */

#include "VitalityLoader.h"
#include "Log/Logger.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"

int VitalityLoader::Init(string &file)
{
	string LivenessData = file + "Data/LivenessData.csv";
	string LivenessRewardDataPath = file + "Data/LivenessRewardData.csv";

	if(InitLivenessData(LivenessData) || InitLivenessRewardData(LivenessRewardDataPath))
	{
		return -1;
	}
	return 0;
}
int VitalityLoader::InitLivenessData(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		sVitality info;
		int count = 0;

		info.id = loader.GetInt(i, count++, 0);
		count++;
		info.count = loader.GetInt(i, count++, 0);
		info.score = loader.GetInt(i, count++, 0);

		m_allVitality[info.id] = info;
	}
	return 0;
}

int VitalityLoader::InitLivenessRewardData(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		sVitalityAward info;
		int count = 0;

		info.id = loader.GetInt(i, count++, 0);
		info.score = loader.GetInt(i, count++, 0);
		info.golden = loader.GetInt(i, count++, 0);
		info.money = loader.GetInt(i, count++, 0);

		string funStrItem = loader.GetString(i, count++, "");
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
		m_allVitalityAward[info.id] = info;
	}
	return 0;
}

//更新数据
void VitalityLoader::UpdataVitality(map<int, sVitalityInfo> &vitalityInfo,map<int,bool> &isGetAward)
{
	map<int, sVitality>::iterator vIt = m_allVitality.begin();
	for(;vIt!=m_allVitality.end();vIt++)
	{
		sVitalityInfo item;
		item.id = vIt->second.id;
		item.count = 0;
		vitalityInfo[item.id] = item;
	}
	map<int, sVitalityAward>::iterator aIt = m_allVitalityAward.begin();
	for(;aIt!=m_allVitalityAward.end();aIt++)
	{
		isGetAward[aIt->second.id] = false;
	}
}
//可以开启多个奖励
int VitalityLoader::GetAwardCount(int score)
{
	int ret = 0;
	map<int, sVitalityAward>::iterator it = m_allVitalityAward.begin();
	for(;it!=m_allVitalityAward.end();it++)
	{
		if(it->second.score <= score)
		{
			ret++;
		}
	}
	return ret;
}

int VitalityLoader::GetAwardIDByScore(int score)
{
	int ret = -1;
	map<int, sVitalityAward>::iterator it = m_allVitalityAward.begin();
	for ( ; it != m_allVitalityAward.end(); ++it)
	{
		if (it->second.score <= score)
		{
			ret = it->second.id;
		}
		else
		{
			break;
		}
	}

	return ret;
}
