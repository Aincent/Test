/*
 * Counter.cpp
 *
 *  Created on: 2014年6月18日
 *      Author: root
 */
#include "Counter.h"
#include "Log/Logger.h"
#include "CharDefine.h"
#include "../Object/Player/Player.h"
#include "util.h"
#include "../Object/Player/PlayerMessage.h"
#include "../MoneyShop/MoneyShopMgr.h"
#include "../EscortEx/EscortMgr.h"
#include "../Daily/Daily.h"
#include "../Activity/ActivityMgr.h"
#include "../LucklyTurntable/LucklyTurntableMgr.h"
#include "MessageCommonRet.h"
#include "./FileLoader/GameParaLoader.h"

CounterService::CounterService(Player * player):
m_owner(player)
{
	ReInit();
}

CounterService::~CounterService()
{

}

void CounterService::ReInit()
{
	m_lastResetInfoTime = 0;
	m_DayType = eDayResetNull;
	m_MothType = eMothResetNull;
	m_counter.clear();
	m_yesterdayCounter.clear();
}

void CounterService::Release()
{
	m_lastResetInfoTime = 0;
	m_DayType = eDayResetNull;
	m_MothType = eMothResetNull;
	m_counter.clear();
	m_yesterdayCounter.clear();
}

int CounterService::GetRecoverSourceTypeByCountId(int countId)
{
	int recoverSourceId = 0;
	switch(countId)
	{
	case EXCHANGFREE_DAY_MONEY:
		recoverSourceId = eRecorceSourceParamType_moneyShop;
		break;
	case ESCORT_DAY_COUNTER:
		recoverSourceId = eRecorceSourceParamType_escort;
		break;
	case COMPETE_INSTANCE_COUNTER:
		recoverSourceId = eRecorceSourceParamType_arena;
		break;
	case PROTECT_KING_COUNTER:
		recoverSourceId = eRecorceSourceParamType_protectKing;
		break;
	}

	return recoverSourceId;
}

int CounterService::GetDailyToDoTypeByCountId(int countId)
{
	int dailyToDoType = 0;
	switch(countId)
	{
	case EXCHANGFREE_DAY_MONEY:
		dailyToDoType = eDailyToDoType_moneyShop;
		break;
	case ESCORT_DAY_COUNTER:
		dailyToDoType = eDailyToDoType_escort;
		break;
	case CHALLEGE_INSTANCE_COUNTER:
		dailyToDoType = eDailyToDoType_challengeInstance;
		break;
	case PROTECT_KING_COUNTER:
		dailyToDoType = eDailyToDoType_protectKing;
		break;
	case TREASON_KING_ENTER:
		dailyToDoType = eDailyToDoType_treason;
		break;
	case FREEROLL_LUCKLYTURNTABLE:
		dailyToDoType = eDailyToDoType_luckylyTurnTable;
		break;
	case COMPETE_INSTANCE_COUNTER:
		dailyToDoType = eDailyToDoType_arena;
		break;
	}

	return dailyToDoType;
}

void CounterService::GetYesterdayCounterInfo(Daily::RecoverResourceInfoList * recoverResourceInfo)
{
	map<int,int>::iterator it = m_yesterdayCounter.begin();
	for(; it != m_yesterdayCounter.end(); ++it)
	{
		int counterType = it->first;
		switch(counterType)
		{
		case EXCHANGFREE_DAY_MONEY:
		case ANSWER_ENTER_COUNT:
		case COMPETE_INSTANCE_COUNTER:
		case ESCORT_DAY_COUNTER:
		{
			if(IsCanEnterModule(counterType) == false)
				continue;
			if(it->second == -1)
				continue;

			Daily::RecoverResourceInfo temInfo;
			int ret = RecoverResource(counterType,&temInfo);
			if(ret == 0)
			{
				Daily::RecoverResourceInfo * countInfo = recoverResourceInfo->add_infolist();
				countInfo->CopyFrom(temInfo);
			}

			break;
		}
		default:
			break;
		}
	}
}

void CounterService::InitLastResetTime(DWORD64 tm)
{
	m_lastResetInfoTime = tm;

	m_DayType  = initResetDayType();

	m_MothType = initResetMothType();
}

void CounterService::AddCounterInfo(const PlayerInfo::CounterServiceList &counterList)
{
	int i = 0;
	int j = 0;

	PlayerInfo::CounterService  counterService = counterList.counter();
	for(i = 0; i < counterService.id_size(); i++)
	{
		for(j = 0; j < COUNTER_MAX_BY_DAY; j++)
		{
			if(counterService.id(i) != j)
			{
				continue;
			}

			if(m_DayType != eDayResetToday)
			{
				map<int, int>::iterator it = m_counter.find(counterService.id(i));
				if(it == m_counter.end())
				{
					m_counter[counterService.id(i)] = 0;
				}
				//连续两天或两天以上未登录，清空
				if(m_DayType == eDayResetBeforeYesterDay)
				{
					m_yesterdayCounter[counterService.id(i)] = 0;
				}
				//上一次登录是昨天，那么直接使用昨天保存的m_counter
				else if(m_DayType == eDayResetYesterday)
				{
					m_yesterdayCounter[counterService.id(i)] = counterService.num(i);
				}
			}
			else
			{
				map<int, int>::iterator it = m_counter.find(counterService.id(i));
				if(it == m_counter.end())
				{
					m_counter[counterService.id(i)] = counterService.num(i);
				}
			}
		}
		for(j = TOTAL_SIGNIN_COUNTER; j < COUNTER_MAX_BY_Month; j++)
		{
			if(counterService.id(i) == j)
			{
				if(m_MothType != eMothResetThisMoth)
				{
					map<int, int>::iterator it = m_counter.find(counterService.id(i));
					if(it == m_counter.end())
					{
						m_counter[counterService.id(i)] = 0;
					}
				}
				else
				{
					map<int, int>::iterator it = m_counter.find(counterService.id(i));
					if(it == m_counter.end())
					{
						m_counter[counterService.id(i)] = counterService.num(i);
					}
				}
			}
		}

		for(j = CHARGE_RMB_COUNT; j<COUNTER_MAX_BY_FOREVER; ++j)
		{
			if(counterService.id(i) == j)
			{
				m_counter[counterService.id(i)] = counterService.num(i);
			}
			else
			{
				map<int, int>::iterator it = m_counter.find(counterService.id(i));
				if(it == m_counter.end())
				{
					m_counter[counterService.id(i)] = counterService.num(i);
				}
			}
		}
	}

	if(m_DayType == eDayResetToday)
	{
		PlayerInfo::CounterService  yesterdayCounter = counterList.yesterdaycounter();

		for(i = 0; i < yesterdayCounter.id_size(); i++)
		{
			for(j = 0; j < COUNTER_MAX_BY_DAY; j++)
			{
				if(yesterdayCounter.id(i) == j)
				{
					m_yesterdayCounter[yesterdayCounter.id(i)] = yesterdayCounter.num(i);
				}
			}
		}
	}

	if(m_DayType == eDayResetYesterday || m_DayType == eDayResetBeforeYesterDay)
	{
		for(int i = CHALLEGE_INSTANCE_COUNTER;i < COUNTER_MAX_BY_DAY;++i)
		{
			switch(i)
			{
			case EXCHANGFREE_DAY_MONEY:
			case ANSWER_ENTER_COUNT:
			case COMPETE_INSTANCE_COUNTER:
			case ESCORT_DAY_COUNTER:
				{
					if(IsCanEnterModule(i) == false)
						continue;
					std::map<int,int>::iterator it = m_yesterdayCounter.find(i);
					if(it == m_yesterdayCounter.end())
					{
						m_yesterdayCounter[i] = 0;//功能已开启但是没做过一次counter不会计数，但是yesterCounter必须记录
						m_owner->SetDataFlag(eCounterInfo);
					}
				}
			}
		}
	}

	m_owner->SetDataFlag(eCounterInfo);
}

void CounterService::SetCounterServiceToDB(PlayerInfo::CounterServiceList* counterList)
{
	counterList->set_lastresettime(m_lastResetInfoTime);

	PlayerInfo::CounterService *counter = counterList->mutable_counter();
	map<int, int>::iterator it = m_counter.begin();
	for(; it != m_counter.end(); ++it)
	{
		counter->add_id(it->first);
		counter->add_num(it->second);
	}

	PlayerInfo::CounterService *yesterdayCounter = counterList->mutable_yesterdaycounter();
	it = m_yesterdayCounter.begin();
	for(; it != m_yesterdayCounter.end(); ++it)
	{
		yesterdayCounter->add_id(it->first);
		yesterdayCounter->add_num(it->second);
	}
}

void CounterService::IncreasePlayerCounter(int onlyId, int num)
{
	map<int, int>::iterator it = m_counter.find(onlyId);
	if(it != m_counter.end())
	{
		it->second += num;
	}
	else
	{
		m_counter[onlyId] = num;
	}

	m_owner->SetDataFlag(eCounterInfo);
}

void CounterService::SetPlayerCounter(int onlyId, int num)
{
	map<int, int>::iterator it = m_counter.find(onlyId);
	if(it != m_counter.end())
	{
		it->second = num;
	}
	else
	{
		m_counter[onlyId] = num;
	}

	m_owner->SetDataFlag(eCounterInfo);
}

void CounterService::IncreaseYesterdayCounter(int onlyId, int num)
{
	map<int, int>::iterator it = m_yesterdayCounter.find(onlyId);
	if(it != m_yesterdayCounter.end())
	{
		it->second += num;
	}
	else
	{
		m_yesterdayCounter[onlyId] = num;
	}

	m_owner->SetDataFlag(eCounterInfo);
}

void CounterService::ResetCounterInfo(int onlyId)
{
	map<int, int>::iterator it = m_counter.find(onlyId);
	if(it != m_counter.end())
	{
		it->second = 0;
		m_owner->SetDataFlag(eCounterInfo);
	}
}

void CounterService::ResetYesterdayCounterInfo(int onlyId)
{
	map<int, int>::iterator it = m_yesterdayCounter.find(onlyId);
	if(it != m_yesterdayCounter.end())
	{
		it->second = -1;
		m_owner->SetDataFlag(eCounterInfo);
	}
}

int CounterService::GetNumById(int onlyId)
{
	map<int, int>::iterator it = m_counter.find(onlyId);
	if(it != m_counter.end())
	{
		return it->second;
	}
	else
	{
		return 0;
	}
}

int CounterService::GetYesterdayNumById(int onlyId)
{
	map<int, int>::iterator it = m_yesterdayCounter.find(onlyId);
	if(it != m_yesterdayCounter.end())
	{
		return it->second;
	}
	else
	{
		return 0;
	}
}

bool CounterService::ResetCounterInfo()
{
	int j = 0;
	for(j = CHALLEGE_INSTANCE_COUNTER; j < COUNTER_MAX_BY_DAY; j++)
	{
		ResetCounterInfo(j);
	}

	//定时器重置或上线时会设置该值，该值在定时器初始化所有模块后或上线初始化完后会重置
	if(m_MothType != eMothResetThisMoth)
	{
		for(j = TOTAL_SIGNIN_COUNTER; j < COUNTER_MAX_BY_Month; j++)
		{
			ResetCounterInfo(j);
		}
	}

	return true;
}

void  CounterService::DecreasePlayerCounter(int onlyId, int num)
{
	map<int, int>::iterator it = m_counter.find(onlyId);
	if(it != m_counter.end())
	{
		it->second -= num;
		if(it->second < 0)
		{
			it->second = 0;
		}
	}

	m_owner->SetDataFlag(eCounterInfo);
}

void  CounterService::DecreaseYesterdayCounter(int onlyId, int num)
{
	map<int, int>::iterator it = m_yesterdayCounter.find(onlyId);
	if(it != m_yesterdayCounter.end())
	{
		it->second -= num;
		if(it->second < 0)
		{
			it->second = 0;
		}
	}

	m_owner->SetDataFlag(eCounterInfo);
}

bool CounterService::IsCanEnterModule(int countId)
{
	switch(countId)
	{
	case EXCHANGFREE_DAY_MONEY:
		return m_owner->IsOpenSystem(eStBank);
	case ESCORT_DAY_COUNTER:
		return m_owner->IsOpenSystem(eStEscort);
	case COMPETE_INSTANCE_COUNTER:
		return m_owner->IsOpenSystem(eStCompeteInstance);
	case PROTECT_KING_COUNTER:
		return m_owner->GetActivityMgr()->IsCanEnterActive(GameParaLoader::GetInstance()->getProtectKingSceneId());
	case TREASON_KING_ENTER:
		return m_owner->GetActivityMgr()->IsCanEnterActive(GameParaLoader::GetInstance()->getTreasonKingMapId());
	case FREEROLL_LUCKLYTURNTABLE:
		return m_owner->IsOpenSystem(eStLucklyTurntable);
	case ANSWER_ENTER_COUNT:
		return m_owner->IsOpenSystem(eStAnswer);
	}
	return true;
}

int CounterService::RecoverResource(const int &countId, Daily::RecoverResourceInfo *info)
{
	int ret = 0;
	int restNum = this->GetYesterdayNumById(countId);
	if(restNum <= -1)
		return eDontHaveRcoverReward;

	switch(countId)
	{
		case EXCHANGFREE_DAY_MONEY:
		{
			ret = MoneyShopMgr::GetInstance()->CalRecoreMoney(m_owner->GetMyself(),restNum, info);
			break;
		}
//		case ESCORT_DAY_COUNTER:
//		{
//			ret = m_owner->GetEscortMgr()->RecoverResource(restNum, info);
//			break;
//		}
		case COMPETE_INSTANCE_COUNTER:
		{
			ret = m_owner->GetInstanceMgr()->RecoverResourceArena(restNum, info);
			break;
		}
		case ANSWER_ENTER_COUNT:
		{
			ret = m_owner->GetAnswerMgr()->RecoverResource(restNum, info);
			break;
		}
		default:
			break;

//		case PROTECT_KING_COUNTER:
//		{
//			ret = m_owner->GetActivityMgr()->RecoverResourceActive(&restNum, tmpInfo);
//			break;
//		}
	}

	return ret;
}

void CounterService::GetDailyCounterInfo(Daily::DailyToDoInfoList * toClient)
{
	for(int i = CHALLEGE_INSTANCE_COUNTER;i < COUNTER_MAX_BY_DAY;++i)
	{
		if(i == ESCORT_DAY_COUNTER ||
		   i == EXCHANGFREE_DAY_MONEY ||
		   i == COMPETE_INSTANCE_COUNTER ||
		   //i == PROTECT_KING_COUNTER ||
		   i == TREASON_KING_ENTER ||
		   i == FREEROLL_LUCKLYTURNTABLE)//|| i == ANSWER_ENTER_COUNT
		{
			if(IsCanEnterModule(i) == false)
				continue;

			Daily::DailyToDoInfo * countInfo = toClient->add_infolist();
			countInfo->set_id(GetDailyToDoTypeByCountId(i));
			std::map<int,int>::iterator it = m_counter.find(i);
			if(it != m_counter.end())
			{
				int count = it->second;
				countInfo->set_count(count);
			}
			else
				countInfo->set_count(0);
		}
	}
}

void CounterService::SetCounterInfoToYesterday()
{
	m_yesterdayCounter.clear();
	for(int i = CHALLEGE_INSTANCE_COUNTER;i < COUNTER_MAX_BY_DAY;++i)
	{
		switch(i)
		{
		case EXCHANGFREE_DAY_MONEY:
		case ANSWER_ENTER_COUNT:
		case COMPETE_INSTANCE_COUNTER:
		case ESCORT_DAY_COUNTER:
			{
				if(IsCanEnterModule(i) == false)
					continue;

				std::map<int,int>::iterator it = m_counter.find(i);
				if(it != m_counter.end())
					m_yesterdayCounter[i] = it->second;
				else
					m_yesterdayCounter[i] = 0;
			}
		}
	}
}

eDayResetType CounterService::initResetDayType()
{
	int tempHour    = 0;
	int tempMinute  = 0;
	int tempSeconds = 0;

	GameParaLoader::GetInstance()->getDayUpdateTime(tempHour,tempMinute,tempSeconds);

	DWORD64 tempCurrentTime  = 0;
	DWORD64 tempPointTime	 = 0;

	CounterService::getDayPointTimeAndCurrentTime(tempHour,tempMinute,tempSeconds,tempCurrentTime,tempPointTime);

	//数据库记录时间大过配表重置时间则算为当天时间,00:00分~00:01分tempPointTime会大于tempCurrentTime
	if((m_lastResetInfoTime >= tempPointTime && tempCurrentTime > tempPointTime) ||
	   (m_lastResetInfoTime <  tempPointTime && tempCurrentTime < tempPointTime)
	  )
	{
		return eDayResetToday;
	}

	//24小时间毫秒
	DWORD64 tempTwentyFour = 3600 * 24 * 1000;

	//数据库重置时间在今天配表重置时间和，今天配表时间 － 24小时，则为昨天
	if(m_lastResetInfoTime >= tempPointTime - tempTwentyFour)
	{
		return eDayResetYesterday;
	}

	return eDayResetBeforeYesterDay;
}

eMothResetType CounterService::initResetMothType()
{
	//当前时间转成日期格式
	time_t tempCurrentTime  = 0;
	struct tm tempCurrentTm;

	time(&tempCurrentTime);
	localtime_r(&tempCurrentTime,&tempCurrentTm);

	//数据库时间转成时间格式
	time_t tempDataBaseTime = m_lastResetInfoTime / 1000;
	struct tm tempDataBasem;

	localtime_r(&tempDataBaseTime,&tempDataBasem);

	//不是同一年,上个月之前
	if(tempCurrentTm.tm_year != tempDataBasem.tm_year)
	{
		return eDayResetBeforeLastMoth;
	}

	//本月
	if(tempCurrentTm.tm_mon == tempDataBasem.tm_mon)
	{
		return eMothResetThisMoth;
	}

	//上个月
	if(tempCurrentTm.tm_mon == tempDataBasem.tm_mon + 1)
	{
		int tempHour    = 0;
		int tempMinute  = 0;
		int tempSeconds = 0;

		GameParaLoader::GetInstance()->getDayUpdateTime(tempHour,tempMinute,tempSeconds);

		DWORD64 tempCurrentTime  = 0;
		DWORD64 tempPointTime	 = 0;

		CounterService::getDayPointTimeAndCurrentTime(tempHour,tempMinute,tempSeconds,tempCurrentTime,tempPointTime);

		//处在31号(该月最后一天)00:00和1号定时器回调期间要处理，否则在该时间段可以无线重置月数据
		if(tempPointTime > m_lastResetInfoTime && tempCurrentTime < tempPointTime)
		{
			return eMothResetThisMoth;
		}
		else
		{
			return eDayResetLastMoth;
		}
	}

	//上个月之前
	return eDayResetBeforeLastMoth;
}

//获取当天某个时间点时间和当前时间,返回的时间为毫秒
void CounterService::getDayPointTimeAndCurrentTime(int hour,int minute,int second,DWORD64& resultCurrentTime,DWORD64& resultPointTime)
{
	time_t tempCurrentTime = 0;
	time_t tempTime		   = 0;
	struct tm tempTm;

	time(&tempCurrentTime);
	localtime_r(&tempCurrentTime,&tempTm);

	tempTm.tm_hour = 0;
	tempTm.tm_min  = 0;
	tempTm.tm_sec  = 0;
	tempTime = mktime(&tempTm);

	//当天某个时间点时间
	resultPointTime	  += ((DWORD64)tempTime + hour * 3600 + minute * 60 + second) * 1000;
	//当前时间
	resultCurrentTime = (DWORD64)tempCurrentTime * 1000;

	return ;
}

//上线玩家初始化完成、定时器回调完后调用
void CounterService::initFinish()
{
	if(m_DayType != eDayResetToday)
	{
		m_lastResetInfoTime = CUtil::GetNowSecond();

		m_owner->SetOnlineTime(0);
	}

	m_DayType  = eDayResetNull;
	m_MothType = eMothResetNull;

	return ;
}

int CounterService::getDate()
{
	int tempHour    = 0;
	int tempMinute  = 0;
	int tempSeconds = 0;

	GameParaLoader::GetInstance()->getDayUpdateTime(tempHour,tempMinute,tempSeconds);

	struct tm tempTm;
	time_t tempCurrentTime = 0;
	time(&tempCurrentTime);

	tempCurrentTime = tempCurrentTime - (tempHour * 3600 + tempMinute * 60 + tempSeconds);
	localtime_r(&tempCurrentTime,&tempTm);

	return tempTm.tm_mday;
}





