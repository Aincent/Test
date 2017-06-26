/*
 * YellowVipLoader.cpp
 *
 *  Created on: 2015年6月10日
 *      Author: root
 */

#include "YellowVipLoader.h"
#include "CSVLoader.h"
#include "Log/Logger.h"
#include "StrArgPkg.h"

YellowVipLoader::YellowVipLoader()
{

}

YellowVipLoader::~YellowVipLoader()
{

}

int YellowVipLoader::Init(string &path)
{
	string awardFile = path + "Data/DiamondTypeData.csv";
	if(InitDiamondTypes(awardFile))
	{
		return -1;
	}
	return 0;
}


int YellowVipLoader::InitDiamondTypes(string &file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		DiamondTypes diamon;
		int count = 0;

		diamon.nId = loader.GetInt(i, count++);
		diamon.nType = loader.GetInt(i, count++);
		diamon.nLv = loader.GetInt(i, count++);
		diamon.nProfess = loader.GetInt(i, count++);
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
				diamon.m_totalItem.push_back(itemId);
				diamon.m_totalNum.push_back(itemNum);
				diamon.m_totalLv.push_back(strengthLv);
			}
		}

		vDiamon.push_back(diamon);
	}

	return 0;
}

void YellowVipLoader::GetDiamondTypes(vector<DiamondTypes>& vecTypes)
{
	vecTypes = vDiamon;
}
