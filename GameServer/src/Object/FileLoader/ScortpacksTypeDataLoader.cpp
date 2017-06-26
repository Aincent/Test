/*
 * ScortpacksTypeDataLoader.cpp
 *
 *  Created on: 2014年6月20日
 *      Author: root
 */
#include "ScortpacksTypeDataLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
ScortpacksLoader * ScortpacksLoader::m_instance = 0;

ScortpacksLoader::ScortpacksLoader()
{
	//m_GoodsInfo.clear();
}

ScortpacksLoader::~ScortpacksLoader()
{

}

int ScortpacksLoader::Init(string &file)
{
	string sItemTypeDataPath = file + "Data/ScortpacksTypeData.csv";

	if(InitItemTypeData(sItemTypeDataPath))
	{
		return -1;
	}

	return 0;
}

int ScortpacksLoader::InitItemTypeData(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}
	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		ScortpacksInfo info;
		int64 id = 0;
		int64 num = 0;

		info.score = loader.GetInt(i,count++);
		//限制使用物品
		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);

		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg funPkgItems(":",getArg(funPkgItem,itemSize).c_str());
			id = atoi(getArg(funPkgItems,0).c_str());
			num = atoi(getArg(funPkgItems,1).c_str());
			info.item.push_back((id&0xffffffff)|(num<<32));
		}
		info.copper = loader.GetInt(i,count++);
		if(info.score>0)m_HaverToDoInfo.push_back(info);
	}
	return 0;
}




