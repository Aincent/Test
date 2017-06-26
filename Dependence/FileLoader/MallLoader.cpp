/*
 * MallLoader.cpp
 *
 *  Created on: 2014年6月17日
 *      Author: root
 */
#include "MallLoader.h"
#include "../CSVLoader.h"
#include "../StrArgPkg.h"


MallLoader * MallLoader::m_instance = 0;

MallLoader::MallLoader()
{
	//m_GoodsInfo.clear();
}

MallLoader::~MallLoader()
{

}

int MallLoader::Init(string &file)
{
	string sItemTypeDataPath = file + "Data/MallTypeData.csv";
	if(InitItemTypeData(sItemTypeDataPath))
	{
		return -1;
	}

	return 0;
}

int MallLoader::InitItemTypeData(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}
	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		MallGoodsInfo info;
		info.id = loader.GetInt(i, count++);
		info.type = loader.GetInt(i,count++);
		info.costItemId = loader.GetInt(i,count++);
		info.price = loader.GetInt(i,count++);
		info.discountPrice = loader.GetInt(i,count++);
		info.maxCout = loader.GetInt(i,count++);

		m_MallGoodsInfo[info.type][info.costItemId] = info;
	}

	return 0;
}


