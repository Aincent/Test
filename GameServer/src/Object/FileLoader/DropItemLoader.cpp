/*
 * DropItemLoader.cpp
 *
 *  Created on: 2014年4月18日
 *      Author: helh
 */

#include "DropItemLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "Log/Logger.h"
#include "Probability/Probability.h"

DropItemLoader::DropItemLoader():m_protectedTime(0),m_deleteTime(0)
{
	m_pLua.OpenLua();
}

DropItemLoader::~DropItemLoader()
{
	m_pLua.CloseLua();
}

int DropItemLoader::Init(string &path)
{
	string dropitem = path + "Data/DropItemList.csv";
	string strPath = path + "Data/PropItem.lua";

	if(InitDropItem(dropitem))
	{
		LOG_ERROR(FILEINFO, "Init Drop item error");

		return -1;
	}

	if(m_pLua.LoadFile(strPath) != 0 || m_pLua.Call() != 0)
	{
		return -1;
	}

	try
	{
		m_pLua.SetLuaFunction("GetDropTime");

		if(m_pLua.Call(0, 2))
		{
			return 0;
		}

		m_deleteTime = m_pLua.GetInt();
		m_protectedTime = m_pLua.GetInt();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "get drop time error [errormsg=%s]", e.what());

		return 0;
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "get drop time unknown error");

		return 0;
	}

	return 0;
}

int DropItemLoader::InitDropItem(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		DropItemInfo tempInfo;
		tempInfo.m_ID 		   = loader.GetInt(i, count++);
		tempInfo.m_Type 	   = loader.GetInt(i, count++);
		tempInfo.m_ItemID 	   = loader.GetInt(i, count++);
		++count;
		tempInfo.m_Counts 	   = loader.GetInt(i, count++);
		tempInfo.m_Probability = loader.GetInt(i, count++);
		tempInfo.m_BindType    = loader.GetInt(i, count++);

		m_dropInfo[tempInfo.m_Type].push_back(tempInfo);
	}

	return 0;
}

int DropItemLoader::GetWeight(int damage, int hp, BYTE beFlag, BYTE enFlag)
{
	int ret = 0;

	try
	{
		m_pLua.SetLuaFunction("GetDropItemWeight");
		m_pLua.SetInt(damage);
		m_pLua.SetInt(hp);
		m_pLua.SetInt(beFlag);
		m_pLua.SetInt(enFlag);

		if(m_pLua.Call(4, 1))
		{
			return 0;
		}

		ret = m_pLua.GetInt();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "drop item and get weight error [errormsg=%s]", e.what());

		return 0;
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "drop item and get weight unknown error");

		return 0;
	}

	return ret;
}

//获得掉落物品,有可能概率对应的物品ID为0，则item有可能为0个，表示该掉落本次随机不到物品
void DropItemLoader::GetDropItem(vector<int>& id, vector<DropStaticInfo>& item)
{
	for(size_t i = 0; i < id.size(); i++)
	{
		map<int,vector<DropItemInfo> >::iterator tempIt = m_dropInfo.find(id[i]);
		if(tempIt == m_dropInfo.end())
			continue;

		ProbabilityVector<DropItemInfo> tempProbability(tempIt->second);
		short tempIndex = tempProbability.calculation();
		if(tempIndex == INVALID_INDEX)
			continue;

		//物品ID为0时，为不掉落
		if(tempIt->second[tempIndex].m_ItemID == 0)
			continue;

		DropStaticInfo tempStaticInfo;
		tempStaticInfo.m_ItemID   = tempIt->second[tempIndex].m_ItemID;
		tempStaticInfo.m_Counts   = tempIt->second[tempIndex].m_Counts;
		tempStaticInfo.m_BindType = tempIt->second[tempIndex].m_BindType;

		item.push_back(tempStaticInfo);
	}
}

//获得掉落物品,只有一个
void DropItemLoader::GetOneDropItem(int id,DropStaticInfo& item)
{
	vector<int> tempIDList;
	vector<DropStaticInfo> tempItemList;
	tempIDList.push_back(id);
	GetDropItem(tempIDList,tempItemList);
	if(tempItemList.size() <= 0)
		return ;

	item = *tempItemList.begin();

	return ;
}

