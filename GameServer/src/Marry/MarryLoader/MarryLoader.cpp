/*
 * MarryLoader.cpp
 *
 *  Created on: 2015年3月19日
 *      Author: root
 */

#include "MarryLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "../MarryMessage.h"



MarryLoader::MarryLoader()
{

}

MarryLoader::~MarryLoader()
{

}

int MarryLoader::init(string& str)
{
	string tempRingStr   = str + "Data/MarryRingTypeData.csv";
	string tempSingleStr = str + "Data/Marry.lua";
	string tempRewardStr = str + "Data/Marriagetimeget.csv";

	if(!loadMarryRingData(tempRingStr) || !loadSingleData(tempSingleStr) || !loadMarryRwardData(tempRewardStr))
	{
		return -1;
	}

	return 0;
}

const MarryRingData* MarryLoader::getFileData(DWORD id)
{
	map<DWORD,MarryRingData>::iterator tempIt = m_FileDataList.find(id);
	if(tempIt == m_FileDataList.end())
	{
		return NULL;
	}

	return &tempIt->second;
}

bool MarryLoader::isMaxRise(DWORD id)
{
	if(m_FileDataList.size() == 0)
	{
		return true;
	}

	map<DWORD,MarryRingData>::iterator tempIt = m_FileDataList.end();
	--tempIt;

	return id >= tempIt->first;
}

DWORD MarryLoader::getFirstID()
{
	if(m_FileDataList.size() == 0)
	{
		return 0;
	}

	return m_FileDataList.begin()->first;
}

//奖励ID映射成索引
short MarryLoader::idMappingToIndex(DWORD id)
{
	map<DWORD,MarryRward>::iterator tempIt = m_RwardList.begin();
	short tempIndex = -1;
	bool tempFind = false;
	for(;tempIt != m_RwardList.end();++tempIt)
	{
		if(id == tempIt->first)
		{
			++tempIndex;
			tempFind = true;
			break;
		}
		++tempIndex;
	}

	return tempFind ? tempIndex : -1;
}

//获取奖励数据
const MarryRward* MarryLoader::getMarryRwardData(DWORD id)
{
	map<DWORD,MarryRward>::iterator tempIt = m_RwardList.find(id);
	if(tempIt == m_RwardList.end())
	{
		return NULL;
	}

	return &tempIt->second;
}

int MarryLoader::getMaxID()
{
	if(m_FileDataList.size() == 0)
	{
		return 0;
	}

	map<DWORD,MarryRingData>::iterator tempIt = m_FileDataList.end();
	--tempIt;

	return tempIt->first;
}

bool MarryLoader::loadMarryRingData(string& str)
{
	CSVLoader loader;
	if (!loader.OpenFromFile(str.c_str()))
	{
		return false;
	}

	MarryRingData tempData;
	int tempRow 	  = 0;
	int tempAttCounts = 0;
	unsigned int tempCounts = loader.GetRowCount();

	for (unsigned int i = 0;i < tempCounts; ++i)
	{
		tempData.m_AttList.clear();
		tempRow 		  = 0;
		tempAttCounts = 0;

		tempData.m_ID 			= loader.GetInt(i, tempRow++);
		tempRow += 3;
		tempData.m_Stars		= loader.GetInt(i, tempRow++);
		tempData.m_LevelLimit 	= loader.GetInt(i, tempRow++);
		tempData.m_Exp 			= loader.GetInt(i, tempRow++);
		tempData.m_ItemCounts 	= loader.GetInt(i, tempRow++);
		tempData.m_ConsumeMoney = loader.GetInt(i, tempRow++);
		tempAttCounts 			= loader.GetInt(i, tempRow++);

		for(int z = eMuscleMan;z <= eShooter;++z)
		{
			for(int j = 0;j < tempAttCounts;++j)
			{
				ECharAttrType tempAttType = (ECharAttrType)loader.GetInt(i, tempRow++);
				int64 tempValue			  = loader.GetInt(i, tempRow++);

				tempData.m_AttList[(ECharProf)z][tempAttType] = tempValue;
			}
		}

		tempData.m_BuckleItem  = loader.GetInt(i, tempRow++);
		tempData.m_BaseExp	   = loader.GetInt(i, tempRow++);
		tempData.m_MinCrit	   = loader.GetFloat(i, tempRow++);
		tempData.m_MinCritRate = loader.GetFloat(i, tempRow++);
		tempData.m_MaxCrit 	   = loader.GetFloat(i, tempRow++);
		tempData.m_MaxCritRate = loader.GetFloat(i, tempRow++);

		m_FileDataList[tempData.m_ID] = tempData;
	}

	return true;
}

bool MarryLoader::loadMarryRwardData(string& str)
{
	CSVLoader loader;
	if (!loader.OpenFromFile(str.c_str()))
	{
		return false;
	}

	int tempRow = 0;
	unsigned int tempCounts = loader.GetRowCount();

	for (unsigned int i = 0;i < tempCounts; ++i)
	{
		tempRow = 0;
		MarryRward tempData;
		tempData.m_ID  = loader.GetInt(i, tempRow++);
		tempData.m_Day = loader.GetInt(i, tempRow++);

		StrArgPkg tempOutItemArg("|",loader.GetString(i,tempRow++));
		size_t tempSize = tempOutItemArg.size();

		for(size_t j = 0;j < tempSize;++j)
		{
			StrArgPkg tempInItemArg(":",tempOutItemArg[j].getStr());

			DWORD tempItemID = atoi(getArg(tempInItemArg,0).c_str());
			DWORD tempNum    = atoi(getArg(tempInItemArg,1).c_str());

			tempData.m_ItemList.push_back(tempItemID);
			tempData.m_NumList.push_back(tempNum);
			tempData.m_FlagList.push_back(1);
		}

		if(tempData.m_ID == 0)
		{
			continue ;
		}

		m_RwardList[tempData.m_ID] = tempData;
	}

	return true;
}

bool MarryLoader::loadSingleData(string& str)
{
	CLuaInterface tempLua;
	tempLua.OpenLua();

	if(tempLua.LoadFile(str) != 0 || tempLua.Call() != 0)
	{
		return -1;
	}

	tempLua.SetLuaFunction("getSingleData");
	if(tempLua.Call(0,10) != 0)
	{
		return -1;
	}

	tempLua.GetInt();
	tempLua.GetInt();
	tempLua.GetInt();
	tempLua.GetInt();
	tempLua.GetInt();

	m_DivorceData.m_ForceDivorceGolden = tempLua.GetInt();
	m_DivorceData.m_ForceDivorceRebate = tempLua.GetInt();

	tempLua.CloseLua();

	return true;
}





