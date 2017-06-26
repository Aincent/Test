/*
 * ActivityBoxLoader.cpp
 *
 *  Created on: 2015年7月29日
 *      Author: root
 */

#include "ActivityBoxLoader.h"
#include "CSVLoader.h"
#include "Log/Logger.h"
#include "StrArgPkg.h"

ActivityBoxLoader::ActivityBoxLoader():m_nMonsterId(0),m_nTimes(0),m_nNum(0),m_nPertime(0),m_nMapid(0)
{
	m_ChestPoint.clear();
}

ActivityBoxLoader::~ActivityBoxLoader()
{

}

int ActivityBoxLoader::Init(string &path)
{
	string chestPoint = path + "Data/chestpoint.csv";
	string chestInf = path + "Data/chestinf.csv";
	if(InitChestPoint(chestPoint) || InitChestInf(chestInf))
	{
		return -1;
	}

	return 0;
}

int ActivityBoxLoader::InitChestPoint(string &file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		ChestPoint point;
		int count = 0;

		count++;
		int mapid = loader.GetInt(i, count++);
		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		if(funPkgItem.size() > 1)
		{
			point.nXPos = atoi(getArg(funPkgItem, 0).c_str());
			point.nYPos = atoi(getArg(funPkgItem, 1).c_str());
		}

		map<int,vector<ChestPoint> >::iterator itr =  m_ChestPoint.find(mapid);
		if(itr != m_ChestPoint.end())
		{
			itr->second.push_back(point);
		}
		else
		{
			vector<ChestPoint> vec;
			vec.push_back(point);
			m_ChestPoint[mapid] = vec;
		}
	}

	return 0;
}

int ActivityBoxLoader::InitChestInf(string &file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;

		m_nMonsterId = loader.GetInt(i, count++);
		m_nTimes = loader.GetInt(i, count++);
		m_nNum = loader.GetInt(i, count++);
		m_nPertime = loader.GetInt(i, count++);
		m_nMapid = loader.GetInt(i, count++);
	}

	return 0;
}

void ActivityBoxLoader::GetRandomPos(int64& nXPos,int64& nYPos,int mapId)
{
	map<int,vector<ChestPoint> >::iterator itr = m_ChestPoint.find(mapId);
	if(itr != m_ChestPoint.end())
	{
		int temp = rand()%itr->second.size();
		nXPos = itr->second[temp].nXPos;
		nYPos = itr->second[temp].nYPos;
	}
}

void ActivityBoxLoader::GetRandomPos(int mapId,vector<ChestPoint>& vec)
{
	map<int,vector<ChestPoint> >::iterator itr = m_ChestPoint.find(mapId);
	if(itr != m_ChestPoint.end())
	{
		vec = itr->second;
	}
}
