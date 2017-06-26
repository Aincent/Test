/*
 * PlayerRegion.cpp
 *
 *  Created on: 30/11/2015
 *      Author: root
 */

#include "PlayerRegion.h"
#include "../Player/Player.h"
#include "../../Map/Sector.h"
#include "../../Map/GameZone.h"
#include "../../Map/ConfigLoader/RegionManage.h"
#include "RegionFactory.h"
#include "RegionFun.h"

//----------------区域------------------
PlayerRegion::PlayerRegion(Player* owner) : m_Owner(owner)
{

}

PlayerRegion::~PlayerRegion()
{
	Release();
}

void PlayerRegion::ReInit()
{

}

void PlayerRegion::Release()
{
	m_DataList.clear();
}

void PlayerRegion::joinRegion(const RegionData* data,Sector* enterSector)
{
	RegionKey tKey(enterSector->getGameZone()->GetSceneID(),data->m_ID);
	for(size_t i = 0;i < data->m_RegionFun.size();++i)
	{
		RegionBase* tFun = RegionFactory::GetInstance()->getRegionFun(this,data,i);
		if(tFun == NULL)
		{
			continue ;
		}

		tFun->excute();
		m_DataList[tKey].push_back(tFun);
	}

	return ;
}

void PlayerRegion::leaveRegion(const RegionData* data,Sector* leaveSector)
{
	RegionKey tKey(leaveSector->getGameZone()->GetSceneID(),data->m_ID);
	map<RegionKey,vector<Smart_Ptr<RegionBase> > >::iterator tIt = m_DataList.find(tKey);

	if(tIt == m_DataList.end())
	{
		return ;
	}

	for(size_t i = 0;i < tIt->second.size();++i)
	{
		tIt->second[i]->revert();
	}

	m_DataList.erase(tIt);

	return ;
}






