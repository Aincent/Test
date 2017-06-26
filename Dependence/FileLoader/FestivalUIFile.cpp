/*
 * FestivalUIFile.cpp
 *
 *  Created on: 16/12/2015
 *      Author: root
 */

#include "FestivalUIFile.h"
#include "../CSVLoader.h"
#include "../StrArgPkg.h"
#include "Log/Logger.h"
#include "../Probability/Probability.h"


FestivalUIFile::FestivalUIFile()
{

}

FestivalUIFile::~FestivalUIFile()
{

}

int FestivalUIFile::init(string& str)
{
	string tEgg      = str + "Data/ChristmasEggBoom.csv";
	string tLogin	 = str + "Data/ChristmasFiveReward.csv";
	string tLimit	 = str + "Data/ChristmasSnapup.csv";
	string tExchange = str + "Data/ChristmasCozy.csv";
	string tSingle	 = str + "Data/ChristmasTime.csv";

	if(initEggFile(tEgg) ||
	   initLoginFile(tLogin) ||
	   initLimitBuyFile(tLimit) ||
	   initExchangeFile(tExchange) ||
	   initSingleFile(tSingle))
	{
		return -1;
	}

	return 0;
}

int FestivalUIFile::initEggFile(string& str)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(str.c_str()))
	{
		return -1;
	}

	int tRowCounts = loader.GetRowCount();
	int tColCounts = 0;
	BreakEggFileData tData;

	for(int i = 0;i < tRowCounts;++i)
	{
		tColCounts = 0;
		tData.m_ID 			= loader.GetInt(i, tColCounts++);
		tData.m_ItemID      = loader.GetInt(i, tColCounts++);
		++tColCounts;
		tData.m_Counts      = loader.GetInt(i, tColCounts++);
		tData.m_Probability = loader.GetInt(i, tColCounts++);
		tData.m_BindType    = loader.GetInt(i, tColCounts++,1);

		m_EggList.push_back(tData);
	}

	return 0;
}

int FestivalUIFile::initLoginFile(string& str)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(str.c_str()))
	{
		return -1;
	}

	int tRowCounts = loader.GetRowCount();
	int tColCounts = 0;

	LoginFileData tData;
	for(int i = 0;i < tRowCounts;++i)
	{
		tColCounts = 0;
		tData.reset();
		tData.m_Day = loader.GetInt(i,tColCounts++);

		StrArgPkg tempOutArg("|",loader.GetString(i,tColCounts++));
		for(size_t j = 0;j < tempOutArg.size();++j)
		{
			StrArgPkg tempInArg(":",tempOutArg[j].getStr());
			tData.m_ItemList.push_back(atoi(getArg(tempInArg,0).c_str()));
			tData.m_NumList.push_back(atoi(getArg(tempInArg,1).c_str()));
			tData.m_BindList.push_back(atoi(getArg(tempInArg,2).c_str()));
		}

		m_LoginList[tData.m_Day] = tData;
	}

	return 0;
}

int FestivalUIFile::initLimitBuyFile(string& str)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(str.c_str()))
	{
		return -1;
	}

	int tRowCounts = loader.GetRowCount();
	int tColCounts = 0;

	LimitBuyFileData tData;
	for(int i = 0;i < tRowCounts;++i)
	{
		tColCounts = 0;

		tData.m_ID 			= loader.GetInt(i,tColCounts++);
		tData.m_ItemID 		= loader.GetInt(i,tColCounts++);
		++tColCounts;
		tData.m_Num 		= loader.GetInt(i,tColCounts++);
		++tColCounts;
		tData.m_Golden 		= loader.GetInt(i,tColCounts++);
		tData.m_LimitNum	= loader.GetInt(i,tColCounts++);
		tData.m_Probability = loader.GetInt(i,tColCounts++);
		tData.m_BindType 	= loader.GetInt(i,tColCounts++);

		m_LimitBuyList[tData.m_ID] = tData;
	}

	return 0;
}

int FestivalUIFile::initExchangeFile(string& str)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(str.c_str()))
	{
		return -1;
	}

	int tRowCounts = loader.GetRowCount();
	int tColCounts = 0;

	ExchangeFileData tData;
	for(int i = 0;i < tRowCounts;++i)
	{
		tColCounts = 0;
		tData.reset();
		tData.m_ID 	 	 = loader.GetInt(i,tColCounts++);
		tData.m_ItemID 	 = loader.GetInt(i,tColCounts++);
		tData.m_Num 	 = loader.GetInt(i,tColCounts++);
		tData.m_Golden 	 = loader.GetInt(i,tColCounts++);

		StrArgPkg tempOutArg("|",loader.GetString(i,tColCounts++));
		for(size_t j = 0;j < tempOutArg.size();++j)
		{
			StrArgPkg tempInArg(":",tempOutArg[j].getStr());
			tData.m_ItemList.push_back(atoi(getArg(tempInArg,0).c_str()));
			tData.m_NumList.push_back(atoi(getArg(tempInArg,1).c_str()));
			tData.m_BindList.push_back(atoi(getArg(tempInArg,2).c_str()));
		}

		m_ExchangeList[tData.m_ID] = tData;
	}

	return 0;
}

int FestivalUIFile::initSingleFile(string& str)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(str.c_str()))
	{
		return -1;
	}

	int tColCounts = 2;
	int tRowCounts = 0;

	string tStr = loader.GetString(tRowCounts,1);

	//登录
	initTime(m_SingleData.m_Time[eChristmasModuleLogin].m_Start,loader.GetString(tRowCounts,tColCounts++));
	initTime(m_SingleData.m_Time[eChristmasModuleLogin].m_End,loader.GetString(tRowCounts,tColCounts++));

	//兑换
	tColCounts  = 2;
	tRowCounts += 2;

	initTime(m_SingleData.m_Time[eChristmasModuleExchange].m_Start,loader.GetString(tRowCounts,tColCounts++));
	initTime(m_SingleData.m_Time[eChristmasModuleExchange].m_End,loader.GetString(tRowCounts,tColCounts++));

	//砸蛋
	tColCounts = 2;
	++tRowCounts;

	initTime(m_SingleData.m_Time[eChristmasModuleEgg].m_Start,loader.GetString(tRowCounts,tColCounts++));
	initTime(m_SingleData.m_Time[eChristmasModuleEgg].m_End,loader.GetString(tRowCounts,tColCounts++));

	m_SingleData.m_BreakMoney 		= loader.GetInt(tRowCounts,tColCounts++);
	m_SingleData.m_ReflushEggGolden = loader.GetInt(tRowCounts,tColCounts++);
	m_SingleData.m_EggReflushTime	= loader.GetInt(tRowCounts,tColCounts++);
	m_SingleData.m_EggCounts		= loader.GetInt(tRowCounts,tColCounts++);

	//限购
	tColCounts = 2;
	++tRowCounts;

	initTime(m_SingleData.m_Time[eChristmasModuleLimit].m_Start,loader.GetString(tRowCounts,tColCounts++));
	initTime(m_SingleData.m_Time[eChristmasModuleLimit].m_End,loader.GetString(tRowCounts,tColCounts++));

	++tColCounts;

	m_SingleData.m_LimitReflushGolden	= loader.GetInt(tRowCounts,tColCounts++);
	m_SingleData.m_LimitReflushTime		= loader.GetInt(tRowCounts,tColCounts++);
	m_SingleData.m_LimitCounts			= loader.GetInt(tRowCounts,tColCounts++);

	return 0;
}

const LoginFileData* FestivalUIFile::getLoginByID(short day)
{
	map<short,LoginFileData>::iterator tIt = m_LoginList.find(day);
	if(tIt == m_LoginList.end())
	{
		return NULL;
	}

	return &tIt->second;
}

const LimitBuyFileData* FestivalUIFile::getLimitBuyByID(DWORD id)
{
	map<DWORD,LimitBuyFileData>::iterator tIt = m_LimitBuyList.find(id);
	if(tIt != m_LimitBuyList.end())
	{
		return &tIt->second;
	}

	return NULL;
}

const ExchangeFileData* FestivalUIFile::getExchangeFileByID(DWORD id)
{
	map<DWORD,ExchangeFileData>::iterator tIt = m_ExchangeList.find(id);
	if(tIt != m_ExchangeList.end())
	{
		return &tIt->second;
	}

	return NULL;
}

bool FestivalUIFile::isEggTime()
{
	const ChristmasSingleData& tSingle = FestivalUIFile::GetInstance()->getSingleData();

	return isInTime(tSingle.m_Time[eChristmasModuleEgg].m_Start,tSingle.m_Time[eChristmasModuleEgg].m_End);
}

bool FestivalUIFile::isLoginTime()
{
	const ChristmasSingleData& tSingle = FestivalUIFile::GetInstance()->getSingleData();

	return isInTime(tSingle.m_Time[eChristmasModuleLogin].m_Start,tSingle.m_Time[eChristmasModuleLogin].m_End);
}

bool FestivalUIFile::isLimitBuyTime()
{
	const ChristmasSingleData& tSingle = FestivalUIFile::GetInstance()->getSingleData();

	return isInTime(tSingle.m_Time[eChristmasModuleLimit].m_Start,tSingle.m_Time[eChristmasModuleLimit].m_End);
}

bool FestivalUIFile::isExchangeTime()
{
	const ChristmasSingleData& tSingle = FestivalUIFile::GetInstance()->getSingleData();

	return isInTime(tSingle.m_Time[eChristmasModuleExchange].m_Start,tSingle.m_Time[eChristmasModuleExchange].m_End);
}

bool SortTestData( const TestData &v1, const TestData &v2)
{
    return v1.m_Counts < v2.m_Counts;
}

void FestivalUIFile::gmProbabilityTest(int counts)
{
	if(counts <= 0)
	{
		return ;
	}

	map<DWORD,TestData> tData;
	DWORD tTotal = 0;
	for(size_t i = 0;i < m_EggList.size();++i)
	{
		tTotal += m_EggList[i].m_Probability;
	}

	for(int i = 0;i < counts;++i)
	{
		ProbabilityVector<BreakEggFileData> tProbability(m_EggList);
		int tIndex = tProbability.calculation();

		++tData[m_EggList[tIndex].m_ID].m_Counts;
		tData[m_EggList[tIndex].m_ID].m_ItemID 		= m_EggList[tIndex].m_ItemID;
		tData[m_EggList[tIndex].m_ID].m_ID     		= m_EggList[tIndex].m_ID;
		tData[m_EggList[tIndex].m_ID].m_Probability = m_EggList[tIndex].m_Probability;
	}

	vector<TestData> tDataList;
	map<DWORD,TestData>::iterator tIt = tData.begin();
	for(;tIt != tData.end();++tIt)
	{
		tDataList.push_back(tIt->second);
	}

	string tStrOne("\n");
	string tStrTow("\n");
	string tStrThree("\n");

	char tChar[256] = {0};
	sprintf(tChar,"---------------probabilityTest %d----------------",tTotal);
	tStrOne.append(tChar);
	tStrOne.append("\n");

	for(size_t i = 0;i < tDataList.size();++i)
	{
		float tPercent = (float)tDataList[i].m_Probability / tTotal * 100;

		if(tDataList[i].m_ID <= 50)
		{
			if(tDataList[i].m_ID < 10)
			{
				sprintf(tChar,"id=%d   itemID=%d counts=%d probability=%.3f%%100",tDataList[i].m_ID,tDataList[i].m_ItemID,tDataList[i].m_Counts,tPercent);
				tStrOne.append(tChar);
				tStrOne.append("\n");
			}
			else
			{
				sprintf(tChar,"id=%d  itemID=%d counts=%d probability=%.3f%%100",tDataList[i].m_ID,tDataList[i].m_ItemID,tDataList[i].m_Counts,tPercent);
				tStrOne.append(tChar);
				tStrOne.append("\n");
			}

		}
		else if(tDataList[i].m_ID > 50 && tDataList[i].m_ID < 100)
		{
			sprintf(tChar,"id=%d itemID=%d counts=%d probability=%.3f%%100",tDataList[i].m_ID,tDataList[i].m_ItemID,tDataList[i].m_Counts,tPercent);
			tStrTow.append(tChar);
			tStrTow.append("\n");
		}
		else
		{
			sprintf(tChar,"id=%d itemID=%d counts=%d probability=%.3f%%100",tDataList[i].m_ID,tDataList[i].m_ItemID,tDataList[i].m_Counts,tPercent);
			tStrThree.append(tChar);
			tStrThree.append("\n");
		}
	}

	LOG_DEBUG(FILEINFO,"%s",tStrOne.c_str());
	LOG_DEBUG(FILEINFO,"%s",tStrTow.c_str());
	LOG_DEBUG(FILEINFO,"%s",tStrThree.c_str());

	return ;
}

void FestivalUIFile::initTime(DateFormat& time,const string& str)
{
	StrArgPkg tempArg(" ",str);
	StrArgPkg tempBeforeArg(":",tempArg[0].getStr());
	StrArgPkg tempAfterArg(":",tempArg[1].getStr());

	time.m_Month = atoi(getArg(tempBeforeArg,1).c_str());
	time.m_Day   = atoi(getArg(tempBeforeArg,2).c_str());

	time.m_Hour   = atoi(getArg(tempAfterArg,0).c_str());
	time.m_Minute = atoi(getArg(tempAfterArg,1).c_str());

	return ;
}

int FestivalUIFile::initNewYear(string& str)
{
	string tLimit = str + "Data/TimeLimitMall.csv";
	string tTime  = str + "Data/SpringFestivalTime.csv";
	string tLogin = str + "Data/SpringFestivalReward.csv";
	string tHorse = str + "Data/SpringFestivalMount.csv";

	if(initNewYLimit(tLimit) || initNewYTime(tTime) || initNewYLogin(tLogin) || initNewYHorse(tHorse))
	{
		return -1;
	}

	return 0;
}

const map<DWORD,NewYearLimitData>* FestivalUIFile::getLimitDatas(eFestivalUIType type)
{
	map<eFestivalUIType,map<DWORD,NewYearLimitData> >::iterator tIt =  m_NewYearLimitList.find(type);
	if(tIt == m_NewYearLimitList.end())
	{
		return NULL;
	}
	
	return &tIt->second;
}

const NewYearLimitData* FestivalUIFile::getLimitData(DWORD id)
{
	map<eFestivalUIType,map<DWORD,NewYearLimitData> >::iterator tIt =  m_NewYearLimitList.begin();
	for(;tIt != m_NewYearLimitList.end();++tIt)
	{
		map<DWORD,NewYearLimitData>::iterator tInIt = tIt->second.begin();
		for(;tInIt != tIt->second.end();++tInIt)
		{
			if(tInIt->first != id)
			{
				continue ;
			}

			return &tInIt->second;
		}
	}
	
	return NULL;
}

const NewYearHorse* FestivalUIFile::getHorseData(eFestivalUIType type)
{
	map<eFestivalUIType,NewYearHorse>::iterator tIt = m_NewYearHorses.find(type);
	if(tIt == m_NewYearHorses.end())
	{
		return NULL;
	}

	return &tIt->second;
}

const NewYearTime* FestivalUIFile::getNewYearTime(eFestivalUIType type)
{
	map<eFestivalUIType,NewYearTime>::iterator tIt = m_NewYearTimes.find(type);
	if(tIt == m_NewYearTimes.end())
	{
		return NULL;
	}

	return &tIt->second;
}

bool FestivalUIFile::isNewYearTime(eActivityUIType type)
{
	map<eFestivalUIType,NewYearTime>::iterator tIt = m_NewYearTimes.begin();
	for(;tIt != m_NewYearTimes.end();++tIt)
	{
		map<eActivityUIType,ChristmasTime>::iterator tInIt = tIt->second.m_Datas.find(type);
		if(tInIt == tIt->second.m_Datas.end())
		{
			continue ;
		}

		if(isInTime(tInIt->second.m_Start,tInIt->second.m_End))
			return true;
	}

	return false;
}

short FestivalUIFile::getNewYearDays()
{
	map<eFestivalUIType,NewYearTime>::iterator tIt = m_NewYearTimes.begin();
	for(;tIt != m_NewYearTimes.end();++tIt)
	{
		map<eActivityUIType,ChristmasTime>::iterator tInIt = tIt->second.m_Datas.find(eActivityUILogin);
		if(tInIt == tIt->second.m_Datas.end())
		{
			continue ;
		}

		if(!isInTime(tInIt->second.m_Start,tInIt->second.m_End))
		{
			continue;
		}

		struct tm tempTm;
		time_t tempCurrentTime = 0;

		time(&tempCurrentTime);
		localtime_r(&tempCurrentTime,&tempTm);

		short tCurDay = tempTm.tm_yday;

		time_t tStartTime = 0;

		tempTm.tm_mon  = tInIt->second.m_Start.m_Month - 1;
		tempTm.tm_mday = tInIt->second.m_Start.m_Day;
		tempTm.tm_hour = tInIt->second.m_Start.m_Hour;
		tempTm.tm_min  = tInIt->second.m_Start.m_Minute;
		tempTm.tm_sec  = 0;

		tStartTime = mktime(&tempTm);

		localtime_r(&tStartTime,&tempTm);

		return tCurDay - tempTm.tm_yday + 1;
	}

	return 0;
}

const ChristmasTime* FestivalUIFile::getNewYLoginTime()
{
	map<eFestivalUIType,NewYearTime>::iterator tIt = m_NewYearTimes.begin();
	for(;tIt != m_NewYearTimes.end();++tIt)
	{
		map<eActivityUIType,ChristmasTime>::iterator tInIt = tIt->second.m_Datas.find(eActivityUILogin);
		if(tInIt == tIt->second.m_Datas.end())
		{
			continue ;
		}

		if(!isInTime(tInIt->second.m_Start,tInIt->second.m_End))
		{
			continue;
		}

		return &tInIt->second;
	}

	return NULL;
}

const OneLoginReward* FestivalUIFile::getNewYearLoginData(int day,eLoginRewardType type)
{
	eFestivalUIType tType = getCurrentUIType(eActivityUILogin);
	map<eFestivalUIType,NewYearLoginReward>::iterator tIt = m_NewYearLogins.find(tType);
	if(tIt == m_NewYearLogins.end())
	{
		return NULL;
	}

	map<eLoginRewardType,map<WORD,OneLoginReward> >::iterator tInIt = tIt->second.m_Datas.find(type);
	if(tInIt == tIt->second.m_Datas.end())
	{
		return NULL;
	}

	map<WORD,OneLoginReward>::iterator tThreeIt = tInIt->second.find(day);
	if(tThreeIt == tInIt->second.end())
	{
		return NULL;
	}

	return &tThreeIt->second;
}

bool FestivalUIFile::isNewYearTime(DWORD64 dbTime)
{
	const ChristmasTime* tData = getNewYLoginTime();

	if(tData == NULL)
	{
		return false;
	}

	struct tm tempTm;
	time_t tempCurrentTime = dbTime / 1000;

	localtime_r(&tempCurrentTime,&tempTm);

	time_t tStartTime = 0;
	time_t tEndTime   = 0;

	tempTm.tm_mon  = tData->m_Start.m_Month - 1;
	tempTm.tm_mday = tData->m_Start.m_Day;
	tempTm.tm_hour = tData->m_Start.m_Hour;
	tempTm.tm_min  = tData->m_Start.m_Minute;
	tempTm.tm_sec  = 0;
	tStartTime = mktime(&tempTm);

	//明年
	if(tData->m_Start.m_Month > tData->m_End.m_Month)
	{
		tempTm.tm_year += 1;
	}

	tempTm.tm_mon  = tData->m_End.m_Month - 1;
	tempTm.tm_mday = tData->m_End.m_Day;
	tempTm.tm_hour = tData->m_End.m_Hour;
	tempTm.tm_min  = tData->m_End.m_Minute;
	tempTm.tm_sec  = 0;
	tEndTime   = mktime(&tempTm);

	if(tStartTime - 10 > tempCurrentTime || tEndTime + 10 < tempCurrentTime)
	{
		return false;
	}

	return true;
}

bool FestivalUIFile::isInTime(const DateFormat& beginTime,const DateFormat& endTime)
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

int FestivalUIFile::initNewYLimit(string& str)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(str.c_str()))
	{
		return -1;
	}

	int tRowCounts = loader.GetRowCount();
	int tColCounts = 0;

	for(int i = 0;i < tRowCounts;++i)
	{
		NewYearLimitData tData;
		tColCounts = 0;

  	    tData.m_ID = loader.GetInt(i,tColCounts++);

		StrArgPkg tempArg(":",loader.GetString(i,tColCounts++));
		tData.m_ItemID = atoi(tempArg[0].getStr().c_str());
		tData.m_Counts = atoi(tempArg[1].getStr().c_str());
		tData.m_Flag   = atoi(tempArg[2].getStr().c_str());
		tData.m_Type   = static_cast<eFestivalUIType>(loader.GetInt(i,tColCounts++));
		
		++tColCounts;

	    tData.m_Golden     	    = loader.GetInt(i,tColCounts++);
		tData.m_PersentDiscount = loader.GetInt(i,tColCounts++); 
	    tData.m_AllLimits  		= loader.GetInt(i,tColCounts++);
	    tData.m_OneLimits   	= loader.GetInt(i,tColCounts++);
		++tColCounts;
	    tData.m_Probability 	= loader.GetInt(i,tColCounts++);

		m_NewYearLimitList[tData.m_Type][tData.m_ID] = tData;
	}

	return 0;
}

int FestivalUIFile::initNewYTime(string& str)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(str.c_str()))
	{
		return -1;
	}

	int tRowCounts = loader.GetRowCount();
	int tColCounts = 0;
	int tLastFestivalt = eFestivalUINULL;
	int tCurActivity   = eActivityUINull;
	for(int i = 0;i < tRowCounts;++i)
	{
		LimitBuyFileData tData;
		tColCounts = 3; 

		eFestivalUIType tCurFestivalt = static_cast<eFestivalUIType>(loader.GetInt(i,tColCounts++));
		if(tLastFestivalt == eFestivalUINULL)
		{
		   tLastFestivalt =	tCurFestivalt;
		}	
		
		if(tLastFestivalt != tCurFestivalt)
		{
			tLastFestivalt = tCurFestivalt;
			tCurActivity   = eActivityUILogin;			
		}
		else
		{
			tCurActivity += 1;
		}

		ChristmasTime tTime;
		initTime(tTime.m_Start,loader.GetString(i,tColCounts++));
		initTime(tTime.m_End,loader.GetString(i,tColCounts++));

		m_NewYearTimes[tCurFestivalt].m_Datas[(eActivityUIType)tCurActivity] = tTime;
	}

	return 0;
}

int FestivalUIFile::initNewYLogin(string& str)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(str.c_str()))
	{
		return -1;
	}

	int tRowCounts = loader.GetRowCount();
	int tColCounts = 0;
	for(int i = 0;i < tRowCounts;++i)
	{
		tColCounts = 0;
		OneLoginReward tData;

		tData.m_ID 					= loader.GetInt(i,tColCounts++);
		eFestivalUIType  tFestiType = (eFestivalUIType)loader.GetInt(i,tColCounts++);
		eLoginRewardType tType 		= (eLoginRewardType)loader.GetInt(i,tColCounts++);
		tData.m_Day 				= loader.GetInt(i,tColCounts++);
		
		StrArgPkg tempArg("|:",loader.GetString(i,tColCounts++));
		for(size_t j = 0;j < tempArg.size();++j)
		{
			tData.m_ItemList.push_back(atoi(getArg(tempArg[j],0).c_str()));
			tData.m_NumList.push_back(atoi(getArg(tempArg[j],1).c_str()));;
			tData.m_BindList.push_back(atoi(getArg(tempArg[j],2).c_str()));;
		}

		m_NewYearLogins[tFestiType].m_Datas[tType][tData.m_Day] = tData;
	}

	return 0;
}

int FestivalUIFile::initNewYHorse(string& str)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(str.c_str()))
	{
		return -1;
	}

	int tRowCounts = loader.GetRowCount();
	int tColCounts = 0;
	for(int i = 0;i < tRowCounts;++i)
	{
		tColCounts = 1;
		NewYearHorse tData;	

		StrArgPkg tArg(":",loader.GetString(i,tColCounts++));

		tData.m_ItemID = atoi(getArg(tArg,0).c_str());
		tData.m_Num    = atoi(getArg(tArg,1).c_str());
		tData.m_Bind   = atoi(getArg(tArg,2).c_str());

		++tColCounts;

		eFestivalUIType tType = (eFestivalUIType)loader.GetInt(i,tColCounts++);
	
		tData.m_Golden 		= loader.GetInt(i,tColCounts++);

		++tColCounts;

		StrArgPkg tTimeArg(":",loader.GetString(i,tColCounts++));
		tData.m_ReflushTime.m_Hour   = atoi(getArg(tTimeArg,0).c_str());
		tData.m_ReflushTime.m_Minute = atoi(getArg(tTimeArg,1).c_str());

		tData.m_Reflushs 	= loader.GetInt(i,tColCounts++);
		
		m_NewYearHorses[tType] = tData;
	}

	return 0;
}

eFestivalUIType FestivalUIFile::getCurrentUIType(eActivityUIType type)
{
	map<eFestivalUIType,NewYearTime>::const_iterator tIt = m_NewYearTimes.begin();
	for(;tIt != m_NewYearTimes.end();++tIt)
	{
		map<eActivityUIType,ChristmasTime>::const_iterator tInIt = tIt->second.m_Datas.find(type);
		if(tInIt == tIt->second.m_Datas.end())
		{
			continue;
		}

		if(isInTime(tInIt->second.m_Start,tInIt->second.m_End))
		{
			return tIt->first;
		}
	}

	return eFestivalUINULL;
}

