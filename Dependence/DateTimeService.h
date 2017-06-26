/*
 * DateTimeService.h
 *
 *  Created on: 2014年5月29日
 *      Author: root
 */

#ifndef DATETIMESERVICE_H_
#define DATETIMESERVICE_H_

#include <list>
#include "define.h"
#include <sys/sysinfo.h>
#include "Singleton.h"

using namespace CommBaseOut;

/**
	特点：
	1、启动时以计算机时间为标准，获取修正量；
	2、运行中，可虚拟修改日期时间，而不影响计算机的系统时间；
	3、最小单位为1分钟，不提供基于秒的触发服务；
	4、触发时刻，与系统时间误差很小，依赖轮询的时间间隔，一般不会超过1秒；
*/

struct DateTimeNode
{
	BYTE	id;			/// [0,255]
	WORD	removed:1;	/// 移除标志
	WORD	year:15;	/// [0,32768]
	BYTE	month;		/// [0,12]
	BYTE	day;		/// [0,31]
	char	wday;		/// [-1,6]
	char	hour;		/// [-1,23]
	BYTE	minute;		/// [0,59]
};

typedef std::list<DateTimeNode> DateTimeList;

class DateTimeService;

class DateTimeHandler
{
	DateTimeList*	mDateTimeList;
public:
	DateTimeHandler();
	virtual ~DateTimeHandler();

	/** 订阅日期时间  2012-02-03 23:45 如：setDateTime(0, 0, 0, 0, -1, 22, 0);// 每天晚上22点触发
	@param	id		ID
	@param	year	年，4位数字，如 2012, 0表示每一年
	@param	month	月,[0, 12], 0表示每个月
	@param	day		日,[0, 31], 0表示每一天
	@param	wday	周几，[-1-6]，-1表示每一天,0表示周日
	@param	hour	时，[-1, 23], -1表示每小时
	@param	minute	分，[0, 59]
	*/
	bool setDateTime(BYTE id, WORD year, BYTE month, BYTE day, char wday, char hour, BYTE minute);
	bool setDateTime(BYTE id, time_t t);

	/// 退订日期时间
	void killDateTime(BYTE id);
//	void dump(ostrbuf& osb) const;

	virtual const char* getDateTimeName() const = 0;
	virtual void onDateTime(uint id) {}

private:
	friend class DateTimeService;
	void polling(const struct tm* t);
};

class DateTimeService
	: public Singleton<DateTimeService>
{
	typedef std::list<DateTimeHandler*>	DateTimeHandlerList;
	int64	mZeroSecondTick;	/// 0秒时刻对应的tick数
	int64	mCurTick;			/// 当前轮询时刻的tick数
	time_t	mZeroSecondTime;	/// 0秒时刻对应的日期时间
	DateTimeHandlerList	mDateTimeHandlerList;	/// 日期时间处理者列表

public:
	DateTimeService();
	virtual ~DateTimeService();

	/// 轮询
	void polling();

	void add(DateTimeHandler* h);
	void remove(DateTimeHandler* h);

	/** 设置虚拟的日期时间(虚拟时间，实际不修改系统的时间)，目前由应用层的 DateTimeService 修改，其他地方不要调用
	@param year		年,必须在1900年以后,(如 2009)
	@param month	月[1,12]
	@param day		日[1,31]
	@param hour		时[0,23]
	@param minute	分[0,59]
	@param second	秒[0,59]
	@retval 如果成功返回当前的虚拟时间，否则参数错误返回0
	*/
//	time_t setVirtualDateTime(int year, int month, int day, int hour, int minute, int second);
//
//	/// 重置本地虚拟时间，使之等同于系统时间
//	void resetVirtualDateTime();

	bool getCurData(tm& getData);

	DWORD getMonthDataNum();

	DWORD64 getCurTime();

	DWORD CountDays(time_t nTimeBegin, time_t nTimeEnd, int offsetHour);		/// begin~end   经过了多少天
	DWORD CountDaysExt(time_t nTimeBegin, DWORD nHourExt = 22);				/// begin~now   经过了多少天 [注:此方法在当天时间超过22:00时候 天数+1]


private:
	void initFromSystemTime();
	void onOneMinutePassed();
};

#endif /* DATETIMESERVICE_H_ */
