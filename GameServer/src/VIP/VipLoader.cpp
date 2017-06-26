/*
 * VipLoader.cpp
 *
 *  Created on: 2014年8月11日
 *      Author: root
 */

#include "VipLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "Log/Logger.h"
#include "MessageCommonRet.h"
#include <math.h>
#include "../QuestNew/QuestDefine.h"
#include "VipCommon.h"


VipLoader::VipLoader()
:  m_nRows(0)
{
	m_VipInfo.clear();
}

VipLoader::~VipLoader()
{
	m_VipInfo.clear();
}
int VipLoader::Init(string &file)
{
	string VipPath = file + "Data/VIPTypeData.csv";
	string RechargePath = file + "Data/ContinuouTypeData.csv";

	if(VipCommonLoader::InitVip(VipPath,m_VipInfo) || InitRecharge(RechargePath))
	{
		return -1;
	}

	return 0;
}

int VipLoader::InitRecharge(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	m_nRows = loader.GetRowCount();
	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		sRecharge info;
		int count = 0;

		info.id = loader.GetInt(i, count++);
		info.day = loader.GetInt(i, count++);

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

		m_RechargeInfo[info.id] = info;
	}
	return 0;
}

int VipLoader::InitAwardState(map<int,int64> &info)
{
	map<int, sRecharge>::iterator it =  m_RechargeInfo.begin();
	for(;it != m_RechargeInfo.end() ; it++)
	{
		int64 id = it->first;
		info[it->first] = (id<<32)|eRechargeCantMayReceive;
	}
	return 0;
}

sVipEntry *VipLoader::GetVipEntry(DWORD id)
{
	map<DWORD, sVipEntry>::iterator it =  m_VipInfo.find(id);

	if(it != m_VipInfo.end())
	{
		return &(it->second);
	}

	return NULL;
}

sRecharge *VipLoader::GetRecharge(DWORD id)
{
	map<int, sRecharge>::iterator it =  m_RechargeInfo.find(id);

	if(it != m_RechargeInfo.end())
	{
		return &(it->second);
	}

	return NULL;
}

sRecharge *VipLoader::GetRechargeByDay(int day)
{
	map<int, sRecharge>::iterator it =  m_RechargeInfo.begin();
	for(;it != m_RechargeInfo.end() ; it++)
	{
		if(it->second.day == (DWORD)day)
		{
			return &(it->second);
		}
	}
	return NULL;
}

DWORD VipLoader::GetVipLevel(DWORD addUpGold)
{
	map<DWORD, sVipEntry>::iterator it =  m_VipInfo.begin();

	DWORD lastNeedGold = 0;
	DWORD lastVipLv = 0;
	for(;it != m_VipInfo.end(); it++)
	{
		DWORD needGold = it->second.m_nAddUpGold;
		if(addUpGold > needGold)
		{
			lastVipLv = it->second.m_nVipID;
			lastNeedGold = needGold;
			continue;
		}

		if(addUpGold < needGold && addUpGold >= lastNeedGold)
		{
			return lastVipLv;
		}
		else if(addUpGold == needGold)
		{
			return it->second.m_nVipID;
		}

	}

	map<DWORD, sVipEntry>::reverse_iterator rit = m_VipInfo.rbegin();
	if(addUpGold > 0 && addUpGold >  rit->second.m_nAddUpGold)
	{
		return 10;
	}

	return 0;
}





