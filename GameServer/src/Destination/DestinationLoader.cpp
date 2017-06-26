/*
 * DestinationLoader.cpp
 *
 *  Created on: 2014年11月27日
 *      Author: root
 */
#include "DestinationLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include <algorithm>

DestinationLoader::DestinationLoader()
{
}

DestinationLoader::~DestinationLoader()
{
}

int DestinationLoader::InitDestination(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		sDestinationEntry info;
		int count = 0;

		info.i_achID = loader.GetInt(i, count++);
		info.i_stage = eDestinationStage(loader.GetInt(i, count++));
		count++;
		info.i_target = (eDestinationType)loader.GetInt(i, count++);
		for(size_t j = 0;j < 2;++j)
		{
			info.i_targetnumber.push_back(loader.GetInt(i, count++));
		}

		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		int awid = atoi(getArg(funPkgItem, 0).c_str());
		int num = atoi(getArg(funPkgItem, 1).c_str());
		info.i_rewardID = awid;
		info.i_rewardCount = num;

		m_DestinationInfo[info.i_achID] = info;
	}

	return 0;
}

sDestinationEntry* DestinationLoader::GetDestinationEntry(DWORD id)
{
	map<DWORD,sDestinationEntry>::iterator it =  m_DestinationInfo.find(id);

	if(it != m_DestinationInfo.end())
	{
		return &(it->second);
	}

	return NULL;
}

void DestinationLoader::GetAllDestinationID(std::vector<DWORD> & ids)
{
	map<DWORD,sDestinationEntry>::iterator it = m_DestinationInfo.begin();
	for(;it != m_DestinationInfo.end(); it++)
	{
		ids.push_back(it->first);
	}
}

void DestinationLoader::GetSpecifiedDestinationID(int type,std::vector<DWORD> & ids)
{
	map<DWORD,sDestinationEntry>::iterator it = m_DestinationInfo.begin();
	for(;it != m_DestinationInfo.end(); it++)
	{
		if(it->second.i_target == type)
			ids.push_back(it->first);
	}
	sort(ids.begin(),ids.end());
}

void DestinationLoader::GetStageDestinationID(int stage,std::vector<DWORD>& ids)
{
	map<DWORD,sDestinationEntry>::iterator it = m_DestinationInfo.begin();
	for(;it != m_DestinationInfo.end(); it++)
	{
		if(it->second.i_stage == stage)
			ids.push_back(it->first);
	}
}
