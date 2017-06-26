/*
 * MarryInstanceLoader.cpp
 *
 *  Created on: 2015年11月3日
 *      Author: root
 */

#include "MarryInstanceLoader.h"


MarryInstanceLoader::MarryInstanceLoader():m_nCountTime(0),m_nWaitTime(0),m_nMapId(0)
{
	m_bothPoint.clear();
}

MarryInstanceLoader::~MarryInstanceLoader()
{
	m_bothPoint.clear();
}

int MarryInstanceLoader::initFile(string& str)
{
	string tempSingleStr = str + "Data/MarryInstance.lua";
	if(loadSingleFileData(tempSingleStr) != 0)
	{
		return -1;
	}

	return 0;
}

int MarryInstanceLoader::loadSingleFileData(string& str)
{
	CLuaInterface tempLua;
	tempLua.OpenLua();

	if(tempLua.LoadFile(str) != 0 || tempLua.Call() != 0)
	{
		return -1;
	}

	tempLua.SetLuaFunction("getSingleData");
	if(tempLua.Call(0,3) != 0)
	{
		return -1;
	}

	m_nMapId = tempLua.GetInt();
	m_nWaitTime = tempLua.GetInt();
	m_nCountTime = tempLua.GetInt();

	tempLua.SetLuaFunction("getBothPoint");
	if(tempLua.Call(0,1) != 0)
	{
		return -1;
	}

	BothPoint tempBornData;
	int tempOutCounts = tempLua.GetTableFieldCount();
	for(int i = 1; i <= tempOutCounts;++i)
	{
		if(tempLua.GetFieldTable(i) != 0)
		{
			tempLua.CloseTable();
			continue;
		}

		tempBornData.nXpos 	 = tempLua.GetFieldInt(1);
		tempBornData.nYpos = tempLua.GetFieldInt(2);

		m_bothPoint.push_back(tempBornData);

		tempLua.CloseTable();
	}

	tempLua.CloseLua();

	return 0;
}

void MarryInstanceLoader::GetBothPoint(int64& xpos,int64& ypos,bool isFirst)
{
	if(m_bothPoint.size() >= 2)
	{
		if(isFirst)
		{
			xpos = m_bothPoint[0].nXpos;
			ypos = m_bothPoint[0].nYpos;
		}
		else
		{
			xpos = m_bothPoint[1].nXpos;
			ypos = m_bothPoint[1].nYpos;
		}
	}
}

void MarryInstanceLoader::GetBothPoint(vector<BothPoint>& vec)
{
	vec = m_bothPoint;
}
