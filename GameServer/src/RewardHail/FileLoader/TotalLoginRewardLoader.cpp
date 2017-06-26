/*
 * TotalLoginRewardLoader.cpp
 *
 *  Created on: 2014年9月11日
 *      Author: root
 */
#include "TotalLoginRewardLoader.h"
#include "Log/Logger.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"



TotalLoginRewardLoader::TotalLoginRewardLoader()
{
}

TotalLoginRewardLoader::~TotalLoginRewardLoader()
{
	m_allTotalReaw.clear();
	m_allFirstCharge.clear();
}

int TotalLoginRewardLoader::Init(string& path)
{
	string fileParam = path + "SevenTypeData.csv";

	if(InitDate(fileParam))
		return -1;

	string fileFirst = path + "RechargeTypeData.csv";
	if(InitFirstChargeData(fileFirst))
		return -1;

	return 0;
}

int TotalLoginRewardLoader::InitDate(string& file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
		return -1;

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		TotalLoginAw info;
		int count = 0;

		info.day = loader.GetInt(i, count++);
		info.zhenqi = loader.GetInt(i, count++);
		info.money = loader.GetInt(i, count++);
		info.bglod = loader.GetInt(i, count++);

		//奖励道具信息
		string funItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem2("|", funItem);
		funPkgItem2.setArgs(funItem);
		for(uint itemSize=0; itemSize<funPkgItem2.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem2, itemSize).c_str());

			int itemId = atoi(getArg(limitPkg, 0).c_str());
			int itemNum = atoi(getArg(limitPkg, 1).c_str());
			int strengthLv = atoi(getArg(limitPkg, 2).c_str());
			if(itemId >0)
			{
				info.m_totalItem.push_back(itemId);
				info.m_totalNum.push_back(itemNum);
				info.m_totalLv.push_back(strengthLv);
			}
		}

		m_allTotalReaw[info.day] = info;
	}
//	map<int , TotalLoginAw>::iterator it = m_allTotalReaw.begin();
//	for(; it != m_allTotalReaw.end(); ++it)
//	{
//		for(uint i=0; i<it->second.m_totalItem.size(); ++i)
//		{
//			cout << endl;
//			cout << " itemId " << it->second.m_totalItem[i] << " num " << it->second.m_totalNum[i] << " lv " << it->second.m_totalLv[i] << endl;
//			cout << endl;
//		}
//	}
	return 0;
}

TotalLoginAw* TotalLoginRewardLoader::GetTotalLoginAw(int day)
{
	map<int , TotalLoginAw>::iterator it = m_allTotalReaw.find(day);
	if(it != m_allTotalReaw.end())
		return &it->second;
	else
		return NULL;
}

int TotalLoginRewardLoader::InitFirstChargeData(string& file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
		return -1;

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		FirstChargeAw info;
		int count = 0;

		info.profession = loader.GetInt(i, count++);
		info.zhenqi = loader.GetInt(i, count++);
		info.money = loader.GetInt(i, count++);
		info.bglod = loader.GetInt(i, count++);

		//奖励道具信息
		string funItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem2("|", funItem);
		funPkgItem2.setArgs(funItem);
		for(uint itemSize=0; itemSize<funPkgItem2.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem2, itemSize).c_str());

			int itemId = atoi(getArg(limitPkg, 0).c_str());
			int itemNum = atoi(getArg(limitPkg, 1).c_str());
			int strengthLv = atoi(getArg(limitPkg, 2).c_str());
			if(itemId >0)
			{
				info.m_totalItem.push_back(itemId);
				info.m_totalNum.push_back(itemNum);
				info.m_totalLv.push_back(strengthLv);
			}
		}

		m_allFirstCharge[info.profession] = info;
	}
	return 0;
}

FirstChargeAw* TotalLoginRewardLoader::GetFirstChargeAw(int profession)
{
	map<int , FirstChargeAw>::iterator it =  m_allFirstCharge.find(profession);
	if(it != m_allFirstCharge.end())
		return &it->second;
	else
		return NULL;
}


