/*
 * InternationalLoader.cpp
 *
 *  Created on: 2015年1月23日
 *      Author: root
 */
#include "InternationalLoader.h"
#include "../CSVLoader.h"

InternationalLoader::InternationalLoader()
{
}

InternationalLoader::~InternationalLoader()
{
}

int InternationalLoader::InitInternational(string& file)
{
	CSVLoader loader('	');

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	int index = 1;

	for(unsigned int i=1; i<loader.GetColCount(); ++i)
	{
		int flag = loader.GetInt(0, index);
		//表的第一行用来标识用什么语言，遇到不为零则读取此列的文字
		if(flag > 0)
			break;
		index++;
	}

	for(unsigned int i=1; i<loader.GetRowCount(); ++i)
	{
		sInternationalEntry info;
		info.id = loader.GetInt(i, 0);
		if(info.id <= 0)
			continue;
		info.internationalStr = loader.GetString(i,index);
		m_internationalMap[info.id] = info;
	}

	return 0;
}

sInternationalEntry * InternationalLoader::GetInternationalEntry(int id)
{
	map<int,sInternationalEntry>::iterator it = m_internationalMap.find(id);
	if(it != m_internationalMap.end())
	{
		return &(it->second);
	}

	return NULL;
}

string InternationalLoader::GetInterStr(int id)
{
	sInternationalEntry * entry = GetInternationalEntry(id);
	if(entry)
		return entry->internationalStr;
	return "";
}
