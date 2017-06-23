/*
 * UpgradeLoader.cpp
 *
 *  Created on: 2013年12月25日
 *      Author: helh
 */

#include "UpgradeLoader.h"
#include "Log/Logger.h"
#include "../CSVLoader.h"


UpgradeLoader * UpgradeLoader::m_instance = 0;

UpgradeLoader::UpgradeLoader()
{

}

UpgradeLoader::~UpgradeLoader()
{
}

int UpgradeLoader::Init(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		UpgradeInfo info;
		int count = 0;

		info.lv = loader.GetInt(i, count++, 0);
		info.upgradeExp = loader.GetInt64(i, count++, 0);
		info.allExp = loader.GetInt64(i, count++, 0);
		info.factoryExp = loader.GetInt64(i, count++, 0);

		m_upgrade.push_back(info);
	}

	return 0;
}





