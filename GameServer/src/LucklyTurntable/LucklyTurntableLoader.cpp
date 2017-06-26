
/*
 * LucklyTurntableLoader.cpp
 *
 *  Created on: 2014年8月20日
 *      Author: root
 */

#include "LucklyTurntableLoader.h"
#include "Log/Logger.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"


int LucklyTurntableLoader::InitDate(string& file)
{
	string LucklyTurntablePath = file + "Data/LuckyTypeData.csv";

	string ProLucklyPath = file + "Data/ProTypeData.csv";

	int ret = 0;
	ret = InitLucklyTurntable(LucklyTurntablePath);
	if(ret != 0)
		return ret;

	return ret = InitProLuckly(ProLucklyPath);
}

int LucklyTurntableLoader::InitLucklyTurntable(string& file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
		return -1;

	LucklyTurntableInfo info;
	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;

		info.m_ID 		   = loader.GetInt(i, count++);
		info.m_ItemID 	   = loader.GetInt(i, count++);
		info.m_Counts 	   = loader.GetInt(i, count++);
		info.m_Type 	   = loader.GetInt(i, count++);
		info.m_Probability = loader.GetInt(i, count++);
		info.m_BindType	   = loader.GetInt(i, count++);
		if(info.m_Type == 0)
			continue;

		m_allLucklyTurntable[info.m_Type].push_back(info);
	}

	return 0;
}

int LucklyTurntableLoader::InitProLuckly(string& file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
		return -1;

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		m_proLuckly.id = loader.GetInt(i, count++, 0);
		m_proLuckly.oneCost = loader.GetInt(i, count++, 0);
		m_proLuckly.tenCost = loader.GetInt(i, count++, 0);
		m_proLuckly.refuCost = loader.GetInt(i, count++, 0);
		m_proLuckly.openLv = loader.GetInt(i, count++, 0);
		m_proLuckly.interTm = loader.GetInt(i, count++, 0);
		m_proLuckly.freeRoll = loader.GetInt(i, count++, 0);
		m_proLuckly.countNum = loader.GetInt(i, count++, 0);
	}

	return 0;
}

const ProLucklyTurntable* LucklyTurntableLoader::GetProLuckly()
{
	return &m_proLuckly;
}

const map<short,vector<LucklyTurntableInfo> >& LucklyTurntableLoader::GetFileDataList()
{
	return m_allLucklyTurntable;
}

const LucklyTurntableInfo* LucklyTurntableLoader::getInfoByID(int id)
{
	map<short,vector<LucklyTurntableInfo> >::iterator tempIt = m_allLucklyTurntable.begin();
	for(;tempIt != m_allLucklyTurntable.end();++tempIt)
	{
		for(size_t i = 0;i < tempIt->second.size();++i)
		{
			if(tempIt->second[i].m_ID != id)
				continue;

			return &tempIt->second[i];
		}
	}

	return NULL;
}

//物品数量和物品ID转成表ID，由m_tmpAward字段的修改，为了兼容数据库
int LucklyTurntableLoader::itemIDToFileDataID(int itemID,int counts)
{
	map<short,vector<LucklyTurntableInfo> >::iterator tempIt = m_allLucklyTurntable.begin();//以类型做为键值
	for(;tempIt != m_allLucklyTurntable.end();++tempIt)
	{
		for(size_t i = 0;i < tempIt->second.size();++i)
		{
			if(tempIt->second[i].m_ItemID == itemID && tempIt->second[i].m_Counts == counts)
				return tempIt->second[i].m_ID;
		}
	}

	return 0;
}








