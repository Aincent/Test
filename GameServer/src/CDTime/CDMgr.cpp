/*
 * CDMgr.cpp
 *
 *  Created on: 2014年1月20日
 *      Author: helh
 */

#include "CDMgr.h"
#include "Log/Logger.h"
#include "util.h"
#include "../Object/Player/Player.h"
#include "MessageStruct/CDTime/CDTimePro.pb.h"
#include "../QuickMessage.h"



CDMgr::CDMgr(Player * obj):m_owner(obj)
{
}

CDMgr::~CDMgr()
{
	Release();
}

void CDMgr::ReInit()
{
	m_allCDTime.clear();
}

void CDMgr::Release()
{
	m_allCDTime.clear();
}

void CDMgr::StartCD(int type, int time)
{
	CDTime cdTime;

	cdTime.cdTime = time;
	cdTime.startTime = CUtil::GetNowSecond();

	m_allCDTime[type] = cdTime;
	m_owner->SetDataFlag(eCDInfo);
}

void CDMgr::InitCDInfo(const PlayerInfo::CDInfo &info)
{
	int64 nowTime = CUtil::GetNowSecond();
	for(int i=0; i<info.cd_size(); ++i)
	{
		CDTime tmpTime;
		PlayerInfo::ChannelCD tmpChannel = info.cd(i);

		tmpTime.cdTime = tmpChannel.endtime() - nowTime;
		if(tmpTime.cdTime <= 0)
		{
			continue;
		}
		else
		{
			tmpTime.startTime = nowTime;
			m_allCDTime[tmpChannel.type()] = tmpTime;
		}
	}
	m_owner->SetInitPartDataFlag(eCDInfo);
}

void CDMgr::SetCDInfo(PlayerInfo::CDInfo *info)
{
	int64 nowTime = CUtil::GetNowSecond();
	CDTIMEMAP::iterator it = m_allCDTime.begin();
	for(; it!=m_allCDTime.end(); ++it)
	{
		if(nowTime >= it->second.cdTime + it->second.startTime)
				continue;

		PlayerInfo::ChannelCD *tmpChannel = info->add_cd();

		tmpChannel->set_type(it->first);
		tmpChannel->set_endtime(it->second.cdTime + it->second.startTime);
	}
}

bool CDMgr::IsCDEnd(int type)
{
	CDTIMEMAP::iterator it = m_allCDTime.find(type);
	if(it == m_allCDTime.end())
	{
		return true;
	}

	DWORD64 tempCurrentTime = CUtil::GetNowSecond();
	DWORD64 tempCDTime		= it->second.cdTime + it->second.startTime;
	if(tempCDTime <= tempCurrentTime)
	{
		return true;
	}

	return false;
}

int64 CDMgr::GetCDTime(int type)
{
	int64 cdTime = 0;
	CDTIMEMAP::iterator it = m_allCDTime.find(type);

	if(it != m_allCDTime.end())
	{
		 if(it->second.startTime > 0)
		 {
			 cdTime =  CUtil::GetNowSecond() - it->second.startTime;
			 if(cdTime > it->second.cdTime)
			 {
				 cdTime = 0;
			 }
		 }
	}
	return cdTime;
}

void CDMgr::ResetCDTime(int type)
{
	CDTIMEMAP::iterator it = m_allCDTime.find(type);
	if(it != m_allCDTime.end())
	{
		m_allCDTime.erase(it);
	}

	m_owner->SetDataFlag(eCDInfo);
}

int64 CDMgr::GetCDEndTime(int type)
{
	int64 cdTime = 0;
	CDTIMEMAP::iterator it = m_allCDTime.find(type);

	if(it != m_allCDTime.end())
	{
		 if(it->second.startTime > 0)
		 {
			 cdTime =  it->second.startTime+  it->second.cdTime;
		 }
	}
	return cdTime;
}

//time为正表示加,负表示减
void CDMgr::cdTimeAlter(eChannelType type, int time,bool isSend)
{
	vector<eChannelType> tempTypeList;
	vector<int> tempTimeList;

	tempTypeList.push_back(type);
	tempTimeList.push_back(time);

	cdTimeAlterList(tempTypeList,tempTimeList,isSend);

	return ;
}

//time为正表示加,负表示减
void CDMgr::cdTimeAlterList(vector<eChannelType>& typeList, vector<int>& timeList,bool isSend)
{
	CDTimePro::CDTimeList tempMsg;

	for(size_t i = 0;i < typeList.size();++i)
	{
		CDTIMEMAP::iterator tempIt = m_allCDTime.find(typeList[i]);
		if(tempIt == m_allCDTime.end())
		{
			continue;
		}

		if(timeList[i] == 0)
		{
			tempIt->second.cdTime = 0;
		}
		else
		{
			tempIt->second.cdTime += timeList[i];
		}

		CDTimePro::CDTimeData* tempTime = tempMsg.add_datalist();

		tempTime->set_channeltype(typeList[i]);
		tempTime->set_time(tempIt->second.cdTime);
	}

	if(tempMsg.datalist_size() <= 0)
	{
		return ;
	}

	QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(),&tempMsg,MSG_SIM_GM2C_CD_TIME_CHANGE);

	return ;
}





