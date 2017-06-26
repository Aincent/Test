/*
 * PetStateDead.cpp
 *
 *  Created on: 2014年12月8日
 *      Author: root
 */

#include "PetStateDead.h"
#include "Log/Logger.h"
#include "../../FileLoader/GirlWarLoader.h"

PetStateDead::PetStateDead(PetBrain * owner):PetState(owner),m_flushTime(0)
{

}

PetStateDead::~PetStateDead()
{

}

void PetStateDead::Update(DWORD dwDeltaTick)
{
	m_flushTime += dwDeltaTick;

	const sGirlWarInfo *pInfo = GirlWarLoader::GetInstance()->GetGirlWarInfo(GetOwner()->GetID());
	if(!pInfo)
	{
		LOG_ERROR(FILEINFO, "girlWar ai dead state update but girlWar not find");
	}
	else
	{
		//怪物刷新时间为0则不复活
		if(pInfo->nFlushTime == 0)
			return;

		if((int)(CUtil::GetNowSecond() - GetOwner()->GetDeadTime()) >=  pInfo->nFlushTime)
		{
			GetOwner()->Reflush();
		}
	}
}

void PetStateDead::BeginState()
{
	m_flushTime = 0;
}

void PetStateDead::EndState()
{
	m_flushTime = 0;
}



