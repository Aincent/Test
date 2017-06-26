/*
 * DailyLoader.cpp
 *
 *  Created on: 2014年12月5日
 *      Author: root
 */
#include "DailyLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "Log/Logger.h"

DailyLoader::DailyLoader()
{
}

DailyLoader::~DailyLoader()
{
}

int DailyLoader::InitDaily(string& file)
{
	string dailyPracticeParamFile = file +"OfflineExpTypeData.csv";
	string recovestr = file +"RecoveryTypeData.csv";
	if(InitDailyParam(dailyPracticeParamFile))
	{
		LOG_ERROR(FILEINFO, "load OfflineExpTypeData error");

		return -1;
	}

	if(InirRecorceSource(recovestr))
	{
		return -1;
	}
	return 0;
}

int DailyLoader::InitDailyParam(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		sDailyPracticeParam info;
		int count = 0;

		info.nId = loader.GetFloat(i, count++);
		info.nLv = loader.GetInt(i,count++);
		info.nExp = loader.GetInt(i,count++);

		m_practiceParam[info.nLv] = info;
	}

	return 0;
}

int DailyLoader::GetParcticeParamExp(int nLv)
{
	map<int ,sDailyPracticeParam>::iterator itr = m_practiceParam.find(nLv);
	if(itr != m_practiceParam.end())
	{
		return itr->second.nExp;
	}

	return 0;
}

int DailyLoader::InirRecorceSource(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		sRecorceSourceParam info;
		int count = 0;

		info.id = loader.GetInt(i, count++, 0);
		count++;
		info.retrieve = loader.GetFloat(i, count++, 0);
		info.accerretireve = loader.GetFloat(i, count++, 0);
		info.golden = loader.GetInt(i, count++, 0);

		m_allRecorceSource[info.id] = info;
	}

	return 0;
}
