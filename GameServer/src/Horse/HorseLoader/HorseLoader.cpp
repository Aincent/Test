/*
 * HorseLoader.cpp
 *
 *  Created on: 2014年4月30日
 *      Author: helh
 */

#include "HorseLoader.h"
#include "Log/Logger.h"
#include "CSVLoader.h"


int HorseLoader::InitHorse(string &file)
{
	if (!loadSkinFile(file))
		return -1;

	string tempStr = file + "MountTypeData.csv";
	CSVLoader loader;

	if (!loader.OpenFromFile(tempStr.c_str()))
	{
		return -1;
	}

	for (unsigned int i = 0;i < loader.GetRowCount(); ++i)
	{
		HorseStaticInfo info;
		int count = 0;

		info.id = loader.GetInt(i, count++, 0);
		count++;
		info.mid = loader.GetInt(i, count++, 0);
		info.advancelv = loader.GetInt(i, count++, 0);
		info.lv = loader.GetInt(i, count++, 0);
		info.exp = loader.GetInt(i, count++, 0);

		for(int j = 0;j < eProfessionMax - 1; ++j)
		{
			info.attr[j].hp = loader.GetInt(i, count++, 0);
//			info.attr[j].mp = loader.GetInt(i, count++, 0);
			info.attr[j].phyatt = loader.GetInt(i, count++, 0);
			info.attr[j].m_Defense = loader.GetInt(i, count++, 0);
			info.attr[j].hitrate = loader.GetInt(i, count++, 0);
			info.attr[j].avoidrate = loader.GetInt(i, count++, 0);
			info.attr[j].crackrate = loader.GetInt(i, count++, 0);
			info.attr[j].avoidcrack = loader.GetInt(i, count++, 0);
			info.attr[j].movespeed = loader.GetInt(i, count++, 0);
		}

		count++;
		info.needNum      = loader.GetInt(i, count++, 0);
		info.m_NeedMoney  = loader.GetInt(i, count++, 0);
		info.m_LevelLimit = loader.GetInt(i, count++, 0);
		info.m_ItemID = loader.GetInt(i, count++, 0);
		info.m_BaseExp = loader.GetInt(i, count++, 0);
		info.m_MinCrit = loader.GetFloat(i, count++, 0);
		info.m_MinCritRate = loader.GetFloat(i, count++, 0);
		info.m_MaxCrit = loader.GetFloat(i, count++, 0);
		info.m_MaxCritRate = loader.GetFloat(i, count++, 0);

		m_allHorse[info.id] = info;
	}

	return 0;
}

bool HorseLoader::loadSkinFile(const string& file)
{
	string tempStr = file + "MountSkinTypeData.csv";
	CSVLoader loader;

	if (!loader.OpenFromFile(tempStr.c_str()))
	{
		return false;
	}

	HorseSkinData tempInfo;
	for (unsigned int i = 0;i < loader.GetRowCount(); ++i)
	{
		int count = 0;

		tempInfo.m_ID = loader.GetInt(i, count++, 0);
		count += 2;

		tempInfo.m_AttList[eCharHPMax] 		  = loader.GetInt(i, count++, 0);
		tempInfo.m_AttList[eCharPhyAttackMax] = loader.GetInt(i, count++, 0);
		tempInfo.m_AttList[eCharPhyDefence]   = loader.GetInt(i, count++, 0);
		tempInfo.m_AttList[eCharHitRate] 	  = loader.GetInt(i, count++, 0);
		tempInfo.m_AttList[eCharAvoidRate] 	  = loader.GetInt(i, count++, 0);

		++count;
		tempInfo.m_BuyGolden = loader.GetInt(i, count++, 0);
		tempInfo.m_LastTime = loader.GetInt(i, count++, 0);

		m_SkinData[tempInfo.m_ID] = tempInfo;
	}

	return true;
}

int HorseLoader::getMaxID()
{
	if(m_allHorse.size() == 0)
	{
		return 0;
	}

	map<int, HorseStaticInfo>::iterator tempIt = m_allHorse.end();

	--tempIt;

	return tempIt->first;
}











