#include "ReflushFile.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"


ReflushFile::ReflushFile()
{

} 

ReflushFile::~ReflushFile()
{

}

int ReflushFile::init(string& str)
{
	CSVLoader loader;

	string tStr = str + "Data/ChristmasSnowman.csv";

	if(!loader.OpenFromFile(tStr.c_str()))
	{
		return -1;
	}
	
	int tRowCounts = loader.GetRowCount();
	int tColCounts = 0;
	ReflushData tData;

	for(int i = 0;i < tRowCounts;++i)
	{
		tColCounts    = 1;

		tData.m_OneList.clear();

		tData.m_MapID = loader.GetInt(i,tColCounts++);

		StrArgPkg tStartArg(":",loader.GetString(i,tColCounts++));
		tData.m_StartDate.m_Month  = atoi(getArg(tStartArg,0).c_str());
		tData.m_StartDate.m_Day    = atoi(getArg(tStartArg,1).c_str());
		tData.m_StartDate.m_Hour   = atoi(getArg(tStartArg,2).c_str());
		tData.m_StartDate.m_Minute = atoi(getArg(tStartArg,3).c_str());

	   	StrArgPkg tEndtArg(":",loader.GetString(i,tColCounts++));
		tData.m_EndData.m_Month  = atoi(getArg(tEndtArg,0).c_str());
		tData.m_EndData.m_Day    = atoi(getArg(tEndtArg,1).c_str());
		tData.m_EndData.m_Hour   = atoi(getArg(tEndtArg,2).c_str());
		tData.m_EndData.m_Minute = atoi(getArg(tEndtArg,3).c_str());

		tData.m_IntervalTime	 = loader.GetInt(i, tColCounts++);

		int tGroupCounts = loader.GetInt(i,tColCounts++);;
		for(int j = 0;j < tGroupCounts;++j)
		{
			ReflushOne tOneData;
			tOneData.m_MonsterID = loader.GetInt(i,tColCounts++);
			tOneData.m_Counts    = loader.GetInt(i,tColCounts++);

			StrArgPkg tPosArg("|:",loader.GetString(i,tColCounts++));
			for(size_t k = 0;k < tPosArg.size();++k)
			{
				Pos tPos;
				tPos.xpos  = atoi(getArg(tPosArg[k],0).c_str());
				tPos.ypos  = atoi(getArg(tPosArg[k],1).c_str());

			    tOneData.m_PosList.push_back(tPos);
			}

			tData.m_OneList.push_back(tOneData);
		}

		m_FlieList[tData.m_MapID].push_back(tData);
	}

	return 0;
}

const vector<ReflushData>* ReflushFile::getFileData(int mapID)
{
	map<int,vector<ReflushData> >::iterator tIt = m_FlieList.find(mapID);
	if(tIt != m_FlieList.end())
	{
		return &tIt->second;
	}

	return NULL;
}

bool ReflushFile::isInTime(const DateFormat& beginTime,const DateFormat& endTime)
{
	struct tm tempTm;
	time_t tempCurrentTime = 0;

	time(&tempCurrentTime);
	localtime_r(&tempCurrentTime,&tempTm);

	time_t tStartTime = 0;
	time_t tEndTime   = 0;

	tempTm.tm_mon  = beginTime.m_Month - 1;
	tempTm.tm_mday = beginTime.m_Day;
	tempTm.tm_hour = beginTime.m_Hour;
	tempTm.tm_min  = beginTime.m_Minute;
	tempTm.tm_sec  = 0;
	tStartTime = mktime(&tempTm);

	//明年
	if(beginTime.m_Month > endTime.m_Month)
	{
		tempTm.tm_year += 1;
	}

	tempTm.tm_mon  = endTime.m_Month - 1;
	tempTm.tm_mday = endTime.m_Day;
	tempTm.tm_hour = endTime.m_Hour;
	tempTm.tm_min  = endTime.m_Minute;
	tempTm.tm_sec  = 0;
	tEndTime   = mktime(&tempTm);

	if(tStartTime - 10 > tempCurrentTime || tEndTime + 10 < tempCurrentTime)
	{
		return false;
	}

	return true;
}
















