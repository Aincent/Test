/*
 * OnLineAtTime.cpp
 *
 *  Created on: 2016年2月22日
 *      Author: root
 */

#include "OnLineAtTimeLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "Timer/TimerInterface.h"
#include "util.h"

OnLineAtTimeLoader::OnLineAtTimeLoader()
{
}

OnLineAtTimeLoader::~OnLineAtTimeLoader()
{
}

int OnLineAtTimeLoader::Init(string& path)
{
	string awardFile = path + "Data/OnLineAtTime.csv";

	CSVLoader loader;

	if(!loader.OpenFromFile(awardFile.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		OnLineInfo info;
		int count = 0;

		info.id = loader.GetInt(i, count++,0);

		initTimePoint(info,loader.GetString(i, count++,""));

		initRewardInfo(info,loader.GetString(i, count++,""));

		m_OnLineInfoList[info.id] = info;
	}

	return 0;
}

void OnLineAtTimeLoader::initTimePoint(OnLineInfo& info, const string& str)
{
	StrArgPkg tempArg(":",str);
	info.atTimeHour = atoi(getArg(tempArg,0).c_str());
	info.atTimeMinute = atoi(getArg(tempArg,1).c_str());
}

void OnLineAtTimeLoader::initRewardInfo(OnLineInfo& info,const string& str)
{
	//奖励道具信息
	StrArgPkg funPkgItem("|", str);
	for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
	{
		StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());

		int itemId = atoi(getArg(limitPkg, 0).c_str());
		int itemNum = atoi(getArg(limitPkg, 1).c_str());

		info.items.push_back(itemId);
		info.nums.push_back(itemNum);
	}
}
