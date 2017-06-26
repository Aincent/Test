/*
 * ActivitiesTimerLoader.h
 *
 *  Created on: 2015年7月30日
 *      Author: root
 */

#ifndef ACTIVITIESTIMERLOADER_H_
#define ACTIVITIESTIMERLOADER_H_

#include "Singleton.h"
#include "define.h"

using namespace std;
using namespace CommBaseOut;

enum eCountDownTimeToClientType
{
	eCountDownTimeNull 		  	= 0,
	eCountDownTimeGroup    		= 2,	//组队副本()
	eCountDownTimeEscortEx 		= 5,    //押镖通知
	eCountDownTimeClan 			= 7,	//霸业战
	eCountDownTimeTreasonKing = 8,      //反王
	eCountDownTimeProtectKing 	= 9,	//守护秦王
	eCountDownTimeBeyond 		= 10,	//跨服活动
	eCountDownTimeCountryBoss	= 13,	//董墓争夺战
	eCountDownTimeCountryWar  	= 14,	//阵营资源战
	eCountDownTimeAnswer      	= 15,	//答题
	eCountDownTimeBox		  	= 16,	//天降宝箱
	eCountDownTimePVP		  	= 17,	//斗战重楼
	eCountDownWorldBoss1		= 18,   //世界Boss副本
	eCountDowneSeabeach			= 19,   //玫瑰海滩
	eCountDowneOnLineAtTime		= 20,   //准点在线
	eCountDownTimeClanRobber   =21,		//强盗活动
	eCountDownTimeClanIntegralBegin = 22, //积分战开启
	eCountDownTimeClanIntegral = 23, 	//积分战
	eCountDownTimeClanIntegralEnd = 24, //积分战结束

	eCountDownTimeClanWarSingUp = 25, 	//报名清除时间
	eCountDownTimeClanWarClear  = 26, 	//对战列表清除时间


	eCountDownTimeMax,
};

struct FightTime
{
	BYTE nMapId;
	BYTE nHour;
	BYTE nMinutes;
	BYTE nEndHour;
	BYTE nEndMinutes;
	BYTE nBeforeHour;
	BYTE nBeforeMinutes;
	BYTE nType;
	BYTE nLevel;
	BYTE nNotice;
	std::vector<BYTE> vWeek;

	FightTime():nMapId(0),nHour(0),nMinutes(0),nEndHour(0),nEndMinutes(0),nBeforeHour(0),nBeforeMinutes(0),nType(0),nLevel(0),nNotice(0)
	{
		vWeek.clear();
	}
};

//各种限时活动的开启时间
struct ActivityTime
{
	int nType;
	int64 beginTime;
	int64 endTime;
	ActivityTime():nType(0),beginTime(0),endTime(0)
	{

	}
};


struct WeekActiveTime
{
	WeekActiveTime(BYTE h,BYTE m):hour(h),minute(m)
	{

	}
	BYTE hour;
	BYTE minute;
};

class ActivitiesTimerLoader : public Singleton<ActivitiesTimerLoader>
{
public:
	ActivitiesTimerLoader();
	virtual ~ActivitiesTimerLoader();

	int init(string& file);
	int InitFightTimeInfo(string& file);
	int InitActivityTimeInfo(string& file);

	void initTime(int64& msec,const string& str);

	void GetFightTime(vector<FightTime> &vec){vec = m_fightTime;}

	void GetActivityTime(std::map<int,ActivityTime> &vec){vec = m_activityTime;}

	bool IsFlushGroupInstance(int64 nNextUpdateTime);

	//取到下次活动开启的时间
	int64 GetNextTime(int id);

	int GetActiveLevel(int type);

private:
	vector<FightTime> m_fightTime;
	//各种限时活动的开启时间
	std::map<int,ActivityTime> m_activityTime;
};

#endif /* ACTIVITIESTIMERLOADER_H_ */
