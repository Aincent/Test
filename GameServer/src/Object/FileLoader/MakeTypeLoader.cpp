/*
 * MakeTypeLoader.cpp
 *
 *  Created on: 2014年6月20日
 *      Author: root
 */
#include "MakeTypeLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
haveToDoLoader * haveToDoLoader::m_instance = 0;

haveToDoLoader::haveToDoLoader()
{
	//m_GoodsInfo.clear();
}

haveToDoLoader::~haveToDoLoader()
{

}

int haveToDoLoader::Init(string &file)
{
	string sItemTypeDataPath = file + "Data/MakeTypeData.csv";

	if(InitItemTypeData(sItemTypeDataPath))
	{
		return -1;
	}

	return 0;
}

int haveToDoLoader::InitItemTypeData(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}
	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		HaveToDoInfo info;

		info.id = loader.GetInt(i,count++);
		count++;
		info.maxCount = loader.GetInt(i,count++);
		info.score = loader.GetInt(i,count++);
		if(info.maxCount>0)m_HaveToDoInfo[info.id] = info;
	}
	return 0;
}




