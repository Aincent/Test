/*
 * LevelRewardLoader.cpp
 *
 *  Created on: 2014年6月20日
 *      Author: root
 */

#include "LevelRewardLoader.h"
#include "StrArgPkg.h"

LevelRewardLoader * LevelRewardLoader::m_instance = 0;

LevelRewardLoader::LevelRewardLoader()
{

}

LevelRewardLoader::~LevelRewardLoader()
{

}

int LevelRewardLoader::Init(string &path)
{
	string fileParam = path + "UpgradeGiftTypeData.csv";

	if(InitLevelRewardLoader(fileParam))
	{
		return -1;
	}

	return 0;
}

int LevelRewardLoader::InitLevelRewardLoader(string & file)
{

	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		LevelRewardStruct info;
		int count = 0;

		int id = loader.GetInt(i, count++);
		info.i_lv = loader.GetInt(i, count++);
		info.i_zhenqi = loader.GetInt(i, count++);
		info.i_money = loader.GetInt(i, count++);
		info.i_bglod = loader.GetInt(i, count++);

		//奖励物品ID
		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		funPkgItem.setArgs(funStrItem);

		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			uint i = 0;
			info.m_nAward.push_back(atoi(getArg(limitPkg, i++).c_str()));
			info.m_nAwardNum.push_back(atoi(getArg(limitPkg, i).c_str()));
		}

		funStrItem = loader.GetString(i, count++, "");
		funPkgItem.setArgs(funStrItem);
		for (uint itemSize = 0; itemSize < funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			uint i = 0;
			info.m_nVipAward.push_back(atoi(getArg(limitPkg, i++).c_str()));
			info.m_nVipAwardNum.push_back(atoi(getArg(limitPkg, i).c_str()));
		}

		m_levelRewardStruct[id] = info;
	}
	return 0;
}



