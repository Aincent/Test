/*
 * ExtraAtributeLoader.cpp
 *
 *  Created on: 2015年4月16日
 *      Author: root
 */
#include "ExtraAtributeLoader.h"
#include "CSVLoader.h"

ExtraAtributeLoader::ExtraAtributeLoader()
{
}

ExtraAtributeLoader::~ExtraAtributeLoader()
{
}

int ExtraAtributeLoader::InitExtraAtribute(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		ExtraAtribute info;
		int count = 0;

		count++;
		info.extraAtriLv = loader.GetInt(i, count++);
		info.type = loader.GetInt(i, count++);
		info.num = loader.GetInt(i, count++);
		info.basehp = loader.GetInt(i, count++);
		info.baphyackmax = loader.GetInt(i, count++);
		info.baphydefmax = loader.GetInt(i, count++);
		info.m_Dodge	  = loader.GetInt(i, count++);
		info.m_Hit		  = loader.GetInt(i, count++);
		info.m_Crit       = loader.GetInt(i, count++);
		info.m_ResistCrit = loader.GetInt(i, count++);

		m_extraAtributeMap[info.type][info.extraAtriLv] = info;
	}

	return 0;
}

void ExtraAtributeLoader::GetExtraAtribute(DWORD type, DWORD extraLv,
		ExtraAtribute& extraAtributeInfo)
{
	std::map<DWORD,std::map<DWORD,ExtraAtribute> >::iterator it = m_extraAtributeMap.find(type);
	if(it!=m_extraAtributeMap.end())
	{
		std::map<DWORD,ExtraAtribute>::iterator extraAtriIt = it->second.find(extraLv);
		if(extraAtriIt!=it->second.end())
		{
			extraAtributeInfo = extraAtriIt->second;
		}
	}
}

void ExtraAtributeLoader::GetExtraAtributeList(DWORD type,
		std::map<DWORD, ExtraAtribute>& infoList)
{
	std::map<DWORD,std::map<DWORD,ExtraAtribute> >::iterator it = m_extraAtributeMap.find(type);
	if(it!=m_extraAtributeMap.end())
	{
		infoList = it->second;
	}
}
