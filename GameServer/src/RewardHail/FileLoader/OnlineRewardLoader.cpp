/*
 * OnlineRewardLoader.cpp
 *
 *  Created on: 2014年6月19日
 *      Author: root
 */

#include "OnlineRewardLoader.h"
#include "StrArgPkg.h"

OnlineRewardLoader * OnlineRewardLoader::m_instance = 0;

OnlineRewardLoader::OnlineRewardLoader()
{

}

OnlineRewardLoader::~OnlineRewardLoader()
{

}

int OnlineRewardLoader::Init(string &path)
{
	string fileParam = path + "OnlineGiftTypeData.csv";

	if(InitOnlineRewardLoader(fileParam))
	{
		return -1;
	}

	return 0;
}

int OnlineRewardLoader::InitOnlineRewardLoader(string & file)
{

	CSVLoader loader;
	int id = 0;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		OnlineRewardStruct info;
		int count = 0;

		id = loader.GetInt(i, count++);
		info.i_online = loader.GetInt(i, count++);
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
			for(unsigned int j=0; j<limitPkg.size(); ++j)
			{
				info.m_nAward.push_back(atoi(getArg(limitPkg, j++).c_str()));
				info.m_nAwardNum.push_back(atoi(getArg(limitPkg, j).c_str()));
			}
		}

		info.i_left = loader.GetInt(i, count++);

		m_onlineRewardStruct[id] = info;
	}
	return 0;
}



