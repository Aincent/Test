/*
 * FestivalAttr.cpp
 *
 *  Created on: 2015年8月25日
 *      Author: root
 */

#include "FestivalAttr.h"
#include "util.h"
#include "../Object/Player/Player.h"
#include "FileLoader/GameParaLoader.h"

PlayerFestiveAttr::PlayerFestiveAttr(Player *player):m_updateTime(0),m_player(player)
{

}

PlayerFestiveAttr::~PlayerFestiveAttr()
{

}

void PlayerFestiveAttr::ReInit()
{
	m_FestiveAttr.clear();
	m_updateTime = 0;
}

void PlayerFestiveAttr::Release()
{
	m_FestiveAttr.clear();
	m_updateTime = 0;
}

int 	PlayerFestiveAttr::getAttr(int id)
{
	std::map<int,int>::iterator it = m_FestiveAttr.find(id);
	if(it==m_FestiveAttr.end())
		return 0;
	return it->second;
}

void PlayerFestiveAttr::attrAdd(int id)
{
	std::map<int,int>::iterator it = m_FestiveAttr.find(id);
	if(it==m_FestiveAttr.end())
	{
		m_FestiveAttr[id] = 1;
	}
	else
	{
		it->second += 1;
	}
}

void PlayerFestiveAttr::initAttr(const PlayerInfo::FestivalInfo& data)
{
	for(int i=0; i<data.item_size(); ++i)
	{
			const PlayerInfo::festivalItem& item = data.item(i);
			m_FestiveAttr[item.id()] = item.nums();
	}
}

void PlayerFestiveAttr::reSetAttr()
{
		std::map<int,int>::iterator it = m_FestiveAttr.begin();
		for(;it!=m_FestiveAttr.end(); ++it)
		{
				it->second = 0;
		}
		//更新保存时间
		m_updateTime=CUtil::GetNowSecond();
		//设置脏数据
		m_player->SetDataFlag(eFestivalInfo);
}

void PlayerFestiveAttr::online(const PlayerInfo::FestivalInfo& data)
{
	m_updateTime = data.time();
	if(isUpdate(data.time()))
	{
		reSetAttr();
	}
	else
	{
		initAttr(data);
	}
}

void PlayerFestiveAttr::getFestiveAttrData(PlayerInfo::FestivalInfo& data)
{
		data.set_time(m_updateTime);
		std::map<int,int>::const_iterator it=m_FestiveAttr.begin();
		for(; it!=m_FestiveAttr.end(); ++it)
		{
				PlayerInfo::festivalItem* pItem = data.add_item();
				pItem->set_id(it->first);
				pItem->set_nums(it->second);
		}
}

bool PlayerFestiveAttr::isUpdate(DWORD64 saveTime)
{
	  int hour=0,minute=0,seconds=0;
		GameParaLoader::GetInstance()->getDayUpdateTime(hour,minute,seconds);
			//清零时间
			time_t timep;
			struct tm p;
			time(&timep);

			localtime_r(&timep,&p);
			p.tm_hour = 0;
			p.tm_min = 0;
			p.tm_sec = 0;
			timep=mktime(&p) ;
			DWORD64 clearTime=(DWORD64)timep;
			clearTime = (clearTime+hour*60*60+minute*60)*1000;

			//当前时间
			DWORD64 curTime=CUtil::GetNowSecond();
			if( clearTime>saveTime &&  curTime>clearTime)
				return true;

			return false;
}

