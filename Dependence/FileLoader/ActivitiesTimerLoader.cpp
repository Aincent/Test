/*
 * ActivitiesTimerLoader.cpp
 *
 *  Created on: 2015年7月30日
 *      Author: root
 */

#include "ActivitiesTimerLoader.h"
#include "../CSVLoader.h"
#include "../StrArgPkg.h"
#include <algorithm>

ActivitiesTimerLoader::ActivitiesTimerLoader()
{

}

ActivitiesTimerLoader::~ActivitiesTimerLoader()
{

}

int ActivitiesTimerLoader::init(string& file)
{
	string strFile = file + "Data/ActivityhallTypeData.csv";

	if(InitFightTimeInfo(strFile) != 0)
		return -1;

	strFile = file + "Data/ActivityTime.csv";

	if(InitActivityTimeInfo(strFile) != 0)
		return -1;

	return 0;
}

int ActivitiesTimerLoader::InitFightTimeInfo(string& file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
	{
			return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		FightTime info;
		int count = 0;
		count++;
		count++;

		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		if(funPkgItem.size() > 1)
		{
			StrArgPkg funItem(":", getArg(funPkgItem, 0).c_str());
			if(funItem.size() > 1)
			{
				info.nHour = atoi(getArg(funItem, 0).c_str());
				info.nMinutes = atoi(getArg(funItem, 1).c_str());
			}

			StrArgPkg funItem1(":", getArg(funPkgItem, 1).c_str());
			if(funItem1.size() > 1)
			{
				info.nEndHour = atoi(getArg(funItem1, 0).c_str());
				info.nEndMinutes = atoi(getArg(funItem1, 1).c_str());
			}
		}

		info.nLevel = loader.GetInt(i,count++);
		count++;
		count++;
		count++;
		count++;

		info.nType = loader.GetInt(i,count++);

		info.nMapId = loader.GetInt(i,count++);

		std::string strProp = loader.GetString(i, count++);
		StrArgPkg strPkg("|", strProp);
		for(size_t j = 0; j < strPkg.size(); j++)
		{
			info.vWeek.push_back(atoi(getArg(strPkg,j).c_str()));
		}


		info.nBeforeHour = (info.nHour*60 + info.nMinutes - 5)/60;
		info.nBeforeMinutes = (info.nHour*60 + info.nMinutes - 5)%60;

		m_fightTime.push_back(info);
	}

	return 0;
}

void ActivitiesTimerLoader::initTime(int64& msec, const string& str)
{
	StrArgPkg tempArg(" ",str);
	StrArgPkg tempBeforeArg(":",tempArg[0].getStr());
	StrArgPkg tempAfterArg(":",tempArg[1].getStr());

	int year = atoi(getArg(tempBeforeArg,0).c_str());
	int month = atoi(getArg(tempBeforeArg,1).c_str());
	int day   = atoi(getArg(tempBeforeArg,2).c_str());

	int hour   = atoi(getArg(tempAfterArg,0).c_str());
	int minute = atoi(getArg(tempAfterArg,1).c_str());

	struct tm local;
	time_t t = 0;
	time(&t);
	localtime_r(&t,&local);
	local.tm_year = year-1900;
	local.tm_mon = month-1;
	local.tm_mday = day;
	local.tm_hour = hour;
	local.tm_min = minute;
	local.tm_sec = 0;
	msec = mktime(&local);
	msec *= 1000;
}

int ActivitiesTimerLoader::InitActivityTimeInfo(string& file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
	{
			return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		ActivityTime info;
		int count = 0;

		info.nType = loader.GetInt(i,count++);

		initTime(info.beginTime,loader.GetString(i,count++,""));

		initTime(info.endTime,loader.GetString(i,count++,""));

		m_activityTime[info.nType] = info;
	}

	return 0;
}

bool ActivitiesTimerLoader::IsFlushGroupInstance(int64 nNextUpdateTime)
{
	return false;

	int nWeek = 0;
	time_t t = 0;
	struct tm local;

	time(&t);

	localtime_r(&t,&local);

	nWeek = local.tm_wday==0?7:local.tm_wday;

	vector<FightTime> vec;
	GetFightTime(vec);

	vector<FightTime>::iterator itr = vec.begin();
	for(; itr != vec.end(); ++itr)
	{
		bool bResult = false;

		switch(itr->nType)
		{
		case eCountDownTimeGroup:
			{
				vector<BYTE>::iterator result = find(itr->vWeek.begin( ), itr->vWeek.end( ), nWeek);
				if (result == itr->vWeek.end( ))
				{
					continue;
				}

				if(itr->nBeforeHour == local.tm_hour && itr->nEndHour == local.tm_hour)
				{
					if(itr->nBeforeMinutes <= local.tm_min && local.tm_min <= itr->nEndMinutes)
					{
						bResult = true;
					}
				}
				else if(itr->nBeforeHour <=  local.tm_hour && itr->nEndHour >= local.tm_hour)
				{
					if(itr->nBeforeHour == local.tm_hour && local.tm_min >= itr->nBeforeMinutes)
					{
						bResult = true;
					}
					else if(itr->nEndHour == local.tm_hour && local.tm_min <= itr->nEndMinutes)
					{
						bResult = true;
					}
					else if(itr->nBeforeHour < local.tm_hour && local.tm_hour < itr->nEndHour)
					{
						bResult = true;
					}
				}
				else
				{
					bResult = false;
				}

				if(bResult)
				{
					local.tm_hour = 0;
					local.tm_min = 0;
					local.tm_sec = 0;
					t = mktime(&local) ;

					DWORD64 ntime = (DWORD64)t*1000;

					ntime += (itr->nBeforeHour*60 + itr->nBeforeMinutes)*60*1000;

					if(ntime >= (DWORD64)nNextUpdateTime)
					{
						return true;
					}

				}
			}
		}
	}

	return false;
}

int64 ActivitiesTimerLoader::GetNextTime(int id)
{
	std::vector< std::vector<WeekActiveTime> > vTemp;
	vTemp.resize(8);

	//当前的地图时间数据
	for(size_t i=0; i<m_fightTime.size(); ++i)
	{
		if(m_fightTime[i].nMapId != id)
			continue;

		for(size_t j=0; j<m_fightTime[i].vWeek.size(); ++j)
		{
			std::vector<WeekActiveTime>& vRef = vTemp[m_fightTime[i].vWeek[j]];
			vRef.push_back( WeekActiveTime(m_fightTime[i].nHour, m_fightTime[i].nMinutes) );
		}
	}

	//取最近的
	int nWeek = 0;
	struct tm local;
	time_t t = 0;

	time(&t);
	localtime_r(&t,&local);
	nWeek = local.tm_wday==0?7:local.tm_wday;

	int targetHour = 0;
	int targetMin  = 0;

	int index = 0;
	int temp = 0;
	bool isFind = false;

	for(size_t i=nWeek; i<vTemp.size(); ++i)
	{
		std::vector<WeekActiveTime>& vRef = vTemp[i];
		if(vRef.size() == 0)
		{
			index++;
			if((int)i == nWeek && ++temp > 1)
			{
				break;
			}
			continue;
		}

		for(size_t j=0; j<vRef.size(); ++j)
		{
			if(index == 0)
			{
				int64 src = local.tm_hour;
				int64 dest = vRef[j].hour;
				src = (src<<32) | local.tm_min;
				dest = (dest<<32) | vRef[j].minute;

				if(src > dest)
					continue;
			}

			isFind = true;
			targetHour = vRef[j].hour;
			targetMin  = vRef[j].minute;
			break;
		}

		if(isFind)
		{
			break;
		}

		if((int)i == nWeek && ++temp > 1)
		{
			break;
		}

		index++;

		//往后扫描完成，没找到，从开始点找
		if(i==vTemp.size()-1)
		{
			i = 0;
		}
	}

	//找到时间
	if(isFind)
	{
		//是否跨天了
		if(index == 0)
		{
			local.tm_hour = targetHour;
			local.tm_min = targetMin;
			local.tm_sec = 0;
			t=mktime(&local) ;

			int64 nextTime = (int64)t;
			nextTime *= 1000;
			return nextTime;
		}
		else
		{
			local.tm_hour = 23;
			local.tm_min = 59;
			local.tm_sec = 59;
			t=mktime(&local) ;

			int64 nextTime = (int64)(t+1); //第二天凌晨
			nextTime = nextTime + (index-1)*24*3600 + targetHour*60*60 + targetMin*60;
			nextTime = nextTime*1000;
			return nextTime;
		}
	}

	return 0;
}

int ActivitiesTimerLoader::GetActiveLevel(int type)
{
	for(size_t i=0; i<m_fightTime.size(); ++i)
	{
		if(m_fightTime[i].nType == type)
		{
			return m_fightTime[i].nLevel;
		}
	}
	return 0;
}


