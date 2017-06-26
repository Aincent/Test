/*
 * playerCampShopMgr.cpp
 *
 *  Created on: 2015年3月20日
 *      Author: root
 */

#include "playerCampShopMgr.h"
#include "util.h"
#include "../Object/Player/Player.h"
#include "GsCountryMgr.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"

PlayerCampShopMgr::PlayerCampShopMgr(Player *player):m_updateTime(0),m_player(player)
{

}

PlayerCampShopMgr::~PlayerCampShopMgr()
{

}

void PlayerCampShopMgr::ReInit()
{
	m_shopAttr.clear();
	m_updateTime = 0;
}

void PlayerCampShopMgr::Release()
{
	m_shopAttr.clear();
	m_updateTime = 0;
}

int PlayerCampShopMgr::getPropAttr(int id)
{
		std::map<int,int>::const_iterator it = m_shopAttr.find(id);
		if(it!=m_shopAttr.end())
		{
				return it->second;
		}
		else
		{
				const CampShopInfo* pShop = GsCountryMgr::GetInstance()->getShopInfo(id);
				if(pShop!=NULL)
				{
						m_shopAttr[id] = 0;
						return 0;
				}
		}

		return -1;
}

void PlayerCampShopMgr::initShop(const PlayerInfo::CampShopInfo& data)
{
		for(int i=0; i<data.item_size(); ++i)
		{
				const PlayerInfo::CampShopItem& item = data.item(i);

				m_shopAttr[item.id()] = item.nums();
		}
}

void PlayerCampShopMgr::getShopData(PlayerInfo::CampShopInfo& data)
{
		data.set_time(m_updateTime);
		std::map<int,int>::const_iterator it=m_shopAttr.begin();
		for(; it!=m_shopAttr.end(); ++it)
		{
			PlayerInfo::CampShopItem* pItem = data.add_item();
			pItem->set_id(it->first);
			pItem->set_nums(it->second);
		}
}

// 是否有能买购买的商品
bool PlayerCampShopMgr::IsHaveCanBuy()
{
	map<int, CampShopInfo> info;
	GsCountryMgr::GetInstance()->GetShopInfo(info);
	map<int, CampShopInfo>::iterator it = info.begin();
	map<int, CampShopInfo>::iterator end = info.end();

	for ( ; it != end; ++it)
	{
		if (GsCountryMgr::GetInstance()->getCampGrade(m_player->getAttManage()->getValue<DWORD>(eCharCountry)) >= it->second.campLv &&
						m_player->getAttManage()->getValue<int>(eCurCountryContribute) >= it->second.contribute)
		{
			map<int, int>::iterator attrIt = m_shopAttr.find(it->second.propId);
			if (m_shopAttr.end() != attrIt )
			{
				if (attrIt->second < it->second.nums)
					return true;
			}
			else
			{
				if (0 >= m_shopAttr.size())
				{
					return true;
				}
			}
		}
	}

	return false;
}

void PlayerCampShopMgr::setPropAttr(int id, int count)
{
		std::map<int,int>::iterator it = m_shopAttr.find(id);
		if(it!=m_shopAttr.end())
		{
			it->second = count;
			m_player->SetDataFlag(eCampShopInfo);
		}


}

void PlayerCampShopMgr::reSetAttr()
{
		std::map<int,int>::iterator it = m_shopAttr.begin();
		for(;it!=m_shopAttr.end(); ++it)
		{
				it->second = 0;
		}
		//更新保存时间
		m_updateTime=CUtil::GetNowSecond();
		//更新每日官职奖励标识
		m_player->setFitchFlag(0);
		//设置脏数据
		m_player->SetDataFlag(eBaseInfo);
		m_player->SetDataFlag(eCampShopInfo);
}


void PlayerCampShopMgr::online(const PlayerInfo::CampShopInfo& data)
{
		m_updateTime = data.time();
		if(isUpdate(data.time()))
		{
			reSetAttr();
		}
		else
		{
			initShop(data);
		}
}

bool PlayerCampShopMgr::isUpdate(DWORD64 saveTime)
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

