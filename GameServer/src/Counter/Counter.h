/*
 * Counter.h
 *
 *  Created on: 2014年6月18日
 *      Author: root
 */

#ifndef COUNTER_H_
#define COUNTER_H_

#include <map>
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "Smart_Ptr.h"
#include "define.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "CounterDefine.h"
#include "MessageStruct/Daily/Daily.pb.h"


using namespace std;
using namespace CommBaseOut;

enum TriggerId
{
	TRIGGER_BY_DAY_ID = 1,
	TRIGGER_BY_MONTH_ID = 2
};

enum eDayResetType
{
	eDayResetNull 			 = 0,
	eDayResetToday 			 = 1, //今天
	eDayResetYesterday  	 = 2, //昨天
	eDayResetBeforeYesterDay = 3, //昨天之前
};

enum eMothResetType
{
	eMothResetNull			= 0,
	eMothResetThisMoth 		= 1, //本月
	eDayResetLastMoth  	 	= 2, //上个月
	eDayResetBeforeLastMoth = 3, //上个月这前
};

class Player;
class CounterService
{
public:
	CounterService(Player * player);
	~CounterService();

	void ReInit();
	void Release();

	void GetYesterdayCounterInfo(Daily::RecoverResourceInfoList * recoverResourceInfo);

	int GetRecoverSourceTypeByCountId(int countId);

	int GetDailyToDoTypeByCountId(int countId);

	void InitLastResetTime(DWORD64 tm);

	void AddCounterInfo(const PlayerInfo::CounterServiceList &counterList);

	void SetCounterServiceToDB(PlayerInfo::CounterServiceList* counterList);
	//增加今日活动次数
	void IncreasePlayerCounter(int onlyId, int num);
	void SetPlayerCounter(int onlyId, int num);
	//增加昨日活动次数
	void IncreaseYesterdayCounter(int onlyId, int num);
	//重置今日活动次数
	void ResetCounterInfo(int onlyId);
	//重置昨日活动次数
	void ResetYesterdayCounterInfo(int onlyId);
	//得到今日活动次数
	int GetNumById(int onlyId);
	//得到昨日活动次数
	int GetYesterdayNumById(int onlyId);
	//减少今日活动次数
	void  DecreasePlayerCounter(int onlyId, int num);
	//减少昨日活动次数
	void  DecreaseYesterdayCounter(int onlyId, int num);

	int RecoverResource(const int &countId,Daily::RecoverResourceInfo *info);

	void GetDailyCounterInfo(Daily::DailyToDoInfoList * toClient);

	bool IsCanEnterModule(int countId);

	int64 GetPlayerLastResetTime(){return m_lastResetInfoTime; }

	bool ResetCounterInfo();

	//把当前的次数设置成为昨天可回收的次数，在每天刷新的时候执行
	void SetCounterInfoToYesterday();

	//获取当天某个时间点时间和当前时间,返回的时间为毫秒
	static void getDayPointTimeAndCurrentTime(int hour,int minute,int second,DWORD64& resultCurrentTime,DWORD64& resultPointTime);

	//上线玩家初始化完成、定时器回调完后调用
	void initFinish();

	eDayResetType getResetDayType(){return m_DayType;}

	eMothResetType getResetMothType(){return m_MothType;}

	static int getDate();

protected:
	eDayResetType initResetDayType();

	eMothResetType initResetMothType();

private:
	Player * m_owner;
	DWORD64 m_lastResetInfoTime;
	//当天活动的进行次数
	map<int, int> m_counter;
	//昨天活动的进行次数
	map<int, int> m_yesterdayCounter;

	eDayResetType  m_DayType;		//用于上线设置日重置类型，防止各个模块多次判断，该值只有在上线或定时器回调期间可以使用，和m_MothType生存周期一样
	eMothResetType m_MothType;		//用于上线设置月重置类型，防止各个模块多次判断，该值只有在上线或定时器回调期间可以使用，和m_DayType生存周期一样
};


#endif /* COUNTER_H_ */
