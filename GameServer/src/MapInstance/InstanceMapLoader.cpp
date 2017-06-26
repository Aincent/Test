/*
 * InstanceMapLoader.cpp
 *
 *  Created on: 2014年6月6日
 *      Author: root
 */

#include "InstanceMapLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "Log/Logger.h"
#include "MessageCommonRet.h"
#include <math.h>
#include "../QuestNew/QuestDefine.h"
#include "./FileLoader/GameParaLoader.h"

InstanceLoader::InstanceLoader()
{
//	m_pLua.OpenLua();
	m_InstanceInfo.clear();
}

InstanceLoader::~InstanceLoader()
{
//	m_pLua.CloseLua();
}

int InstanceLoader::Init(string &file)
{
	CSVLoader loader('	');

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		InstanceMapEntry info;
		int count = 0;

		count++;
		info.m_nMapID = loader.GetInt(i, count++);
		info.m_sName = loader.GetString(i, count++);
		info.m_eType = (InstanceType)loader.GetInt(i, count++);

		/*如果发生断言  则  定义的 地图ID 不一致*/
		const int Arena_MapID = GameParaLoader::GetInstance()->getArenaMapId();
		if(info.m_eType == eCompeteInstance && (int)info.m_nMapID != Arena_MapID)
			continue ;

		const int ClanWar_MapID = GameParaLoader::GetInstance()->getClanWarMapId();
		if(info.m_eType == eClanBattle && (int)info.m_nMapID != ClanWar_MapID)
			continue ;

		info.m_nLevel = loader.GetInt(i, count++);
		info.m_nXPos = loader.GetInt(i, count++);
		info.m_nYPos = loader.GetInt(i, count++);
		info.m_nReaptTimes = loader.GetInt(i, count++);



		info.m_bAllowPk = loader.GetInt(i, count++);
		info.m_nLimitTime = loader.GetInt(i, count++);

		//限制使用物品
		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			for(unsigned int i=0; i<limitPkg.size(); ++i)
			{
				info.m_nTarget.push_back(atoi(getArg(limitPkg, i++).c_str()));
				info.m_nTargetNum.push_back(atoi(getArg(limitPkg, i).c_str()));
			}
		}

		info.m_sDescript = loader.GetString(i, count++);
		info.m_nResetPurchase = loader.GetInt(i, count++);
		info.m_nDelTime = loader.GetInt(i, count++);
		info.m_nJgdelTime = loader.GetInt(i, count++);
		info.m_nWrdelTime = loader.GetInt(i, count++);
		info.m_nResetNum = loader.GetInt(i, count++);
		info.m_nVipLimit = loader.GetInt(i, count++);

		m_InstanceInfo[info.m_nMapID] = info;
	}

	return 0;
}

InstanceMapEntry *InstanceLoader::GetEntry(DWORD id)
{
	map<DWORD, InstanceMapEntry>::iterator it =  m_InstanceInfo.find(id);

	if(it != m_InstanceInfo.end())
	{
		return &(it->second);
	}

	return NULL;
}

InstanceMapEntry *InstanceLoader::GetEntryByType(InstanceType type)
{
	map<DWORD, InstanceMapEntry>::iterator it =  m_InstanceInfo.begin();
	for( ;it != m_InstanceInfo.end(); ++it)
	{
		if(it->second.m_eType == type)
			return &(it->second);
	}

	return NULL;
}

InstanceMapReward *InstanceLoader::GetEntryReward(DWORD id)
{
	map<DWORD,InstanceMapReward>::iterator itr = m_InstanceRewardInfo.find(id);
	if(itr != m_InstanceRewardInfo.end())
	{
		return &(itr->second);
	}

	return NULL;
}

void InstanceLoader::GetInstanceTypeIdList(InstanceType type,DWORD special,std::vector<int> & idList, int playerLv)
{
	map<DWORD, InstanceMapEntry>::iterator it = m_InstanceInfo.begin();
	for(;it != m_InstanceInfo.end();++it)
	{
		if(it->second.m_eType == type && it->second.m_nLevel <= (DWORD)playerLv)
		{
			idList.push_back(it->first);
		}
	}
}

bool InstanceLoader::IsNotAddSinValueMapID(const int& mapID)
{
	/*目前 就 帮派站地图*/
	const int ClanWar_MapID = GameParaLoader::GetInstance()->getClanWarMapId();
	const int CountryWar_MapID = GameParaLoader::GetInstance()->getCountryWarMapId();
	const int Arena_MapID = GameParaLoader::GetInstance()->getArenaMapId();
	const int crossSvrBattleMapID = GameParaLoader::GetInstance()->getCrossArenaMapId();
	if(mapID == ClanWar_MapID || mapID == CountryWar_MapID || mapID == Arena_MapID || mapID == crossSvrBattleMapID)
		return true;
	return false;
}

int InstanceLoader::InitReward(string &file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		InstanceMapReward info;
		int count = 0;
		count++;

		info.m_nMapID = loader.GetInt(i, count++);

		count++;
		//限制使用物品
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

		info.m_nAwardMoney = loader.GetInt(i, count++);
		info.m_nAwardExp = loader.GetInt(i, count++);
		info.m_nAwardSprit = loader.GetInt(i, count++);

		funStrItem = loader.GetString(i, count++, "");
		funPkgItem.setArgs(funStrItem);
		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			for(unsigned int i=0; i<limitPkg.size(); ++i)
			{
				int itemid = atoi(getArg(limitPkg, i).c_str());
				int itemnum = atoi(getArg(limitPkg, ++i).c_str());
				info.m_nTongAward[itemid] = itemnum;
			}
		}

		m_InstanceRewardInfo[info.m_nMapID] = info;
	}

	return 0;
}
