/*
 * DateTimeService.cpp
 *
 *  Created on: 2014年5月29日
 *      Author: root
 */

#include "DateTimeService.h"

DateTimeHandler::DateTimeHandler()
	: mDateTimeList(0)
{
}

DateTimeHandler::~DateTimeHandler()
{
	if (mDateTimeList)
	{
		DateTimeService::GetInstance()->remove(this);
		mDateTimeList->clear();
		delete mDateTimeList;
		mDateTimeList = 0;
	}
}

/** 订阅日期时间  2012-02-03 23:45
@param	id		ID
@param	year	年，4位数字，如 2012, 0表示每一年
@param	month	月,[0, 12], 0表示每个月
@param	day		日,[0, 31], 0表示每一天
@param	wday	周几，[-1-6]，-1表示每一天,0表示周日
@param	hour	时，[-1, 23], -1表示每小时
@param	minute	分，[0, 59]
*/
bool DateTimeHandler::setDateTime(BYTE id, WORD year, BYTE month, BYTE day, char wday, char hour, BYTE minute)
{
	if (month > 12 || day > 31 || wday > 6 || hour >= 24 || minute >=60)
		return false;

	if (!mDateTimeList)
	{
		mDateTimeList = new DateTimeList();
		DateTimeService::GetInstance()->add(this);
	}

	DateTimeNode* node = 0;
	for (DateTimeList::iterator it=mDateTimeList->begin(); it!=mDateTimeList->end(); ++it)
	{
		if ((*it).id == id)
		{
			node = &(*it);
		}
	}

	if (!node)
	{
		mDateTimeList->push_back(DateTimeNode());
		node = &(mDateTimeList->back());
	}

	if (node)
	{
		node->id = id;
		node->removed = 0;
		node->year = year;
		node->month = month;
		node->day = day;
		node->wday = wday;
		node->hour = hour;
		node->minute = minute;
		return true;
	}

	return false;
}

bool DateTimeHandler::setDateTime(BYTE id, time_t t)
{
	time_t now = time(0);
	if (t > now) // 订阅未来时刻
	{
		struct tm lt;
		localtime_r(&t,&lt);

		setDateTime(id, (WORD)lt.tm_year+1900, (BYTE)lt.tm_mon+1, (BYTE)lt.tm_mday, -1, lt.tm_hour, lt.tm_min);

		return true;
	}

	return false;
}

void DateTimeHandler::killDateTime(BYTE id)
{
	if (mDateTimeList)
	{
		for (DateTimeList::iterator it=mDateTimeList->begin(); it!=mDateTimeList->end(); ++it)
		{
			DateTimeNode& node = *it;
			if (node.id == id)
			{
				node.removed = 1;
				return;
			}
		}
	}
}

void DateTimeHandler::polling(const struct tm* t)
{
	if (!mDateTimeList)
		return;

	for (DateTimeList::iterator it=mDateTimeList->begin(); it!=mDateTimeList->end(); ++it)
	{
		if ((*it).removed == 1)
		{
			it = mDateTimeList->erase(it);
			if (it == mDateTimeList->end())
				break;
		}

		const DateTimeNode& node = *it;
		if (node.year > 0 && node.year != t->tm_year+1900)
			continue;

		if (node.month > 0 && node.month != t->tm_mon+1)
			continue;

		if (node.day > 0 && node.day != t->tm_mday)
			continue;

		if (node.wday >= 0 && node.wday != t->tm_wday)
			continue;

		if (node.hour >= 0 && node.hour != t->tm_hour)
			continue;

		if (node.minute != t->tm_min)
			continue;

//		char time_str[32] = {0};
//		strftime(time_str, 32, "%Y-%m-%d %H:%M:%S", t);
		onDateTime(node.id);
	}
}

DateTimeService::DateTimeService()
{
	initFromSystemTime();
}

DateTimeService::~DateTimeService()
{
}

void DateTimeService::initFromSystemTime()
{
	time_t t = time(0);	// 自1970年1月1日00:00:00到当前时刻的秒数
	struct tm lt;
	localtime_r(&t,&lt);
	struct sysinfo s_info;
	if(0==sysinfo(&s_info))
	{
		mCurTick = s_info.uptime;
	}
	mZeroSecondTick = mCurTick * 1000 - lt.tm_sec * 1000; // 计算出0秒时刻的tick数
	mZeroSecondTime = t - lt.tm_sec;// 计算出0秒时刻的秒数
}

// 轮询
void DateTimeService::polling()
{
	struct sysinfo s_info;
	int64 now;
	if(0==sysinfo(&s_info))
	{
		now = s_info.uptime;
	}

	if (now < mCurTick) // 溢出？
	{
		mZeroSecondTick -= (int64)0x00000000ffffffffL;
//		EmphasisLn("Tick count overload");
	}

	mCurTick = now;
		if (now * 1000 < mZeroSecondTick + 60000) // 如果不到一分钟，直接返回
		return;

	mZeroSecondTick += 60000;
	mZeroSecondTime += 60;
	onOneMinutePassed();
}

void DateTimeService::add(DateTimeHandler* h)
{
	if (h)
		mDateTimeHandlerList.push_back(h);
}

void DateTimeService::remove(DateTimeHandler* h)
{
	mDateTimeHandlerList.remove(h);
}

// 在每个整1分钟触发
void DateTimeService::onOneMinutePassed()
{
	tm tt;
	if (!localtime_r(&mZeroSecondTime,&tt)) return;

	for (DateTimeHandlerList::iterator it=mDateTimeHandlerList.begin(); it!=mDateTimeHandlerList.end(); ++it)
	{
		(*it)->polling(&tt);
	}
}

bool DateTimeService::getCurData( tm& getData )
{
	time_t t = time(0);
	tm tt;

	if (!localtime_r(&t,&tt)) return false;

	getData = tt;

	return true;

}

DWORD DateTimeService::getMonthDataNum()
{
	tm setTm;
	getCurData(setTm);
	int nNewYear = setTm.tm_year;
	int nNewMon = setTm.tm_mon + 1;
	if (nNewMon > 11) //[0-11]
	{
		nNewMon = 0;
		nNewYear += 1;
	}
	setTm.tm_year = nNewYear;
	setTm.tm_mon = nNewMon;
	setTm.tm_mday = 1; //[1-31]
	setTm.tm_hour = 0; //[0,23]
	time_t t = mktime(&setTm);
	t -= 60000;	/// 减个一分钟
	tm tt;

	if(!localtime_r(&t,&tt))
	{
		return 0;
	}
	uint nReturn = (uint)tt.tm_mday;
	if(!(nReturn >= 28 && nReturn <= 31))
	{
		return 0;
	}
	return nReturn;
}

DWORD64 DateTimeService::getCurTime()
{
	return (DWORD64)time(0);
}

DWORD DateTimeService::CountDays( time_t nTimeBegin, time_t nTimeEnd, int offsetHour )
{
	if(!(offsetHour <= 24 && offsetHour >= -24))
	{
		return 0;
	}
	nTimeBegin += offsetHour * 3600;
	nTimeEnd += offsetHour * 3600;

	tm getTm;
	getTm.tm_year = 2000 - 1900;
	getTm.tm_mon = 0;
	getTm.tm_mday = 1; //[1-31]
	getTm.tm_hour = 0; //[1-31]
	getTm.tm_min = 0; //[1-31]
	getTm.tm_sec = 0; //[1-31]
	time_t t = mktime(&getTm);
	if (nTimeBegin > t)
		nTimeBegin -= t;
	else
		nTimeBegin = 0;

	if (nTimeEnd > t)
		nTimeEnd -= t;
	else
		nTimeEnd = 0;

	DWORD64 daysBegin = nTimeBegin / (3600 * 24);
	DWORD64 daysEnd = nTimeEnd / (3600 * 24);
	if(!(daysEnd >= daysBegin))
	{
		return 0;
	}
	return (DWORD)(daysEnd - daysBegin);

	//static tm *temp = localtime(&nTimeBegin);
	//IF_NOT_RETURN_VALUE(temp, 0);
	//tm tmBegin  = *temp;
	//temp = localtime(&nTimeEnd);
	//IF_NOT_RETURN_VALUE(temp, 0);
	//tm tmEnd = *temp;
	//uint nCount = 0;

	//if (tmBegin.tm_year < tmEnd.tm_year)
	//{
	//	nCount += 365 * (tmEnd.tm_year - tmBegin.tm_year);
	//}

	//uint nCount = 0;
	//uint nFind = nTimeBegin;
	//while (true)
	//{
	//	nFind += 60 * 60 * 24;
	//	if (nFind < nTimeEnd)
	//		++nCount;
	//	else
	//	{
	//		nFind -= 60 * 60 * 24;
	//		break;
	//	}
	//}
}

/// begin~end   经过了多少天 [注:此方法在当天时间超过22:00时候 天数+1]
uint DateTimeService::CountDaysExt(time_t nTimeBegin, DWORD nHourExt/* = 22*/)
{
	time_t nowTime = DateTimeService::GetInstance()->getCurTime();
	DWORD tmpDay = this->CountDays(nTimeBegin, nowTime, 24);
	tmpDay += 1;   //CountDays计算的第一天为0, 故+1

	if(nHourExt != 0) //为0表示是0点,此时已经是第二天了
	{
		tm tmpTM;
		getCurData(tmpTM);  //获取当前虚拟时间

		if(tmpTM.tm_hour >= (int)nHourExt && (tmpTM.tm_min >= 0 || tmpTM.tm_sec >= 0))   //超过当前22点表示天数+1
			tmpDay += 1;
	}

	return tmpDay;
}
