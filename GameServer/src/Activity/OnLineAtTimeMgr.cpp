/*
 * OnLineAtTimeMgr.cpp
 *
 *  Created on: 2016年2月22日
 *      Author: root
 */
#include "OnLineAtTimeMgr.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Mail/MailMessage.h"
#include "./FileLoader/InternationalLoader.h"
#include "./FileLoader/ActivitiesTimerLoader.h"

OnLineAtTimeMgr::OnLineAtTimeMgr()
{
	InitOnLineAtTime();
}

OnLineAtTimeMgr::~OnLineAtTimeMgr()
{
	Clear();
}

void OnLineAtTimeMgr::InitOnLineAtTime()
{
	Clear();

	std::map<int,ActivityTime> activityList;
	ActivitiesTimerLoader::GetInstance()->GetActivityTime(activityList);

	std::map<int,ActivityTime>::iterator activityIt = activityList.find(eCountDowneOnLineAtTime);
	if(activityIt == activityList.end())
		return;

	int64 nowTime = CUtil::GetNowSecond();
	map<int,OnLineInfo> onLineInfoList = OnLineAtTimeLoader::GetInstance()->getOnLineInfoList();

	map<int,OnLineInfo>::iterator it = onLineInfoList.begin();
	for(;it!=onLineInfoList.end();++it)
	{
		if(nowTime > activityIt->second.endTime)
			return;

		int * pID = new int(it->second.id);
		TimerConnection timer = TimerInterface::GetInstance()->RegTimePoint(&OnLineAtTimeMgr::onGiveReward, this, (void*)pID, it->second.atTimeHour, it->second.atTimeMinute, 0);
		timeList.push_back(timer);
	}
}

void OnLineAtTimeMgr::onGiveReward(void* arg)
{
	int* pData = (int*)(arg);
	if(pData==NULL)
		return;

	int id = *pData;
	delete pData;
	pData=NULL;

	std::map<int,ActivityTime> activityList;
	ActivitiesTimerLoader::GetInstance()->GetActivityTime(activityList);

	std::map<int,ActivityTime>::iterator activityIt = activityList.find(eCountDowneOnLineAtTime);
	if(activityIt == activityList.end())
		return;

	map<int,OnLineInfo> onLineInfoList = OnLineAtTimeLoader::GetInstance()->getOnLineInfoList();
	map<int,OnLineInfo>::iterator it = onLineInfoList.find(id);
	if(it!=onLineInfoList.end())
	{
		OnLineInfo info = it->second;
		int64 nowTime = CUtil::GetNowSecond();
		if(nowTime > activityIt->second.endTime || nowTime < activityIt->second.beginTime)
			return;

		if(info.items.size() != info.nums.size())
			return;

		vector<int64> goods;
		for(uint i = 0;i < info.items.size();++i)
		{
			int64 tempID = info.items[i];
			int64 tempNum = info.nums[i];
			int64 tempGood = tempID | (tempNum << 32);
			goods.push_back(tempGood);
		}

		INT64PLAYERMAP allPlayer = PlayerMessage::GetInstance()->getAllPlayer();
		INT64PLAYERMAP::iterator playerIt = allPlayer.begin();
		for(;playerIt!=allPlayer.end();++playerIt)
		{
			string str = InternationalLoader::GetInstance()->GetInterStr(47);
			MailMessageMgr::GetInstance()->SendSysMail(goods,playerIt->second->GetName(),playerIt->first,str);
		}
	}
}

void OnLineAtTimeMgr::Clear()
{
	for(uint i = 0;i < timeList.size();++i)
	{
		if(timeList[i].IsValid())
			timeList[i].SetNull();
	}

	timeList.clear();
}

void OnLineAtTimeMgr::GetOnLineAtTimeInfo(OnLineAtTimeInfo::RetOnLineAtTimeInfo& toClient)
{
	std::map<int,ActivityTime> activityList;
	ActivitiesTimerLoader::GetInstance()->GetActivityTime(activityList);

	std::map<int,ActivityTime>::iterator activityIt = activityList.find(eCountDowneOnLineAtTime);
	if(activityIt == activityList.end())
	{
		toClient.set_ret(eActivityNotExist);

		return;
	}

	int64 nowTime = CUtil::GetNowSecond();
	if(nowTime > activityIt->second.endTime || nowTime < activityIt->second.beginTime)
	{
		toClient.set_ret(eActivityNotExist);

		return;
	}

	toClient.set_begintime(activityIt->second.beginTime);
	toClient.set_endtime(activityIt->second.endTime);

	map<int,OnLineInfo> onLineInfoList = OnLineAtTimeLoader::GetInstance()->getOnLineInfoList();
	if(onLineInfoList.size() <= 0)
	{
		toClient.set_ret(eActivityNotExist);

		return;
	}

	map<int,OnLineInfo>::iterator it = onLineInfoList.begin();
	for(;it!=onLineInfoList.end();++it)
	{
		OnLineAtTimeInfo::AtTimeInfo * atTimeInfo = toClient.add_attimelist();
		atTimeInfo->set_hour(it->second.atTimeHour);
		atTimeInfo->set_minute(it->second.atTimeMinute);
		for(uint i = 0;i < it->second.items.size();++i)
		{
			atTimeInfo->add_itemids(it->second.items[i]);
			atTimeInfo->add_itemnums(it->second.nums[i]);
		}
	}

	toClient.set_ret(0);
}
