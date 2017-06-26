/*
 * AIStateDead.cpp
 *
 *  Created on: 2013��10��23��
 *      Author: helh
 */

#include "AIStateDead.h"
#include "../Monster/Monster.h"
#include "../FileLoader/MonsterLoader.h"
#include "Log/Logger.h"
#include "../../EscortEx/EscortMgr.h"


AIStateDead::AIStateDead(AIBrain * owner):AIState(owner),m_flushTime(0),m_leftTime(0)
{

}

AIStateDead::~AIStateDead()
{

}

void AIStateDead::Update(DWORD dwDeltaTick)
{
}

void AIStateDead::BeginState()
{
	//设置目标为空
	Smart_Ptr<CreatureObj> target;
	GetOwner()->SetTarget(target);

	//通知选我为目标的对象清除
	GetBrain()->clearGirlTarget();
}

void AIStateDead::EndState()
{
	m_flushTime = 0;
}

DWORD64 AIStateDead::getLeftTime()
{
	if(GetOwner()->GetQuality() == eEscortCar)
	{
			return 0;
	}

	int64 time = 0;

	if(GetOwner()->GetAliveTime() == 0)
	{
		if(GetOwner()->GetFlushTime() == 0)
			return 0;

		time = (DWORD)GetOwner()->GetFlushTime() - m_flushTime;
	}

	if(time < 0)
		time = 0;

	return time;
}

CommonMonsterDead::CommonMonsterDead(AIBrain * owner):AIStateDead(owner)
{

}

CommonMonsterDead::~CommonMonsterDead()
{

}

void CommonMonsterDead::Update(DWORD dwDeltaTick)
{
	m_flushTime += dwDeltaTick;

	//怪物刷新时间为0则不复活
	if(GetOwner()->GetAliveTime() == 0)
	{
		if(GetOwner()->GetFlushTime() == 0)
			return;

		if(m_flushTime >= (DWORD)GetOwner()->GetFlushTime())
		{
			GetOwner()->Reflush();
		}
	}
	else if(GetOwner()->GetAliveTime() > 0)
	{
		if(m_flushTime >= m_leftTime)
		{
			GetOwner()->Reflush();
		}
	}
}

void CommonMonsterDead::BeginState()
{
	//设置目标为空
	Smart_Ptr<CreatureObj> target;
	GetOwner()->SetTarget(target);

	//通知选我为目标的对象清除
	GetBrain()->clearGirlTarget();

	//存活时间类型的时间
	DWORD64 aliveTime = GetOwner()->GetAliveTime()*1000;

	//普通刷新时间
	if(aliveTime==0)
	{
		m_flushTime = 0;
		return;
	}

	//有存活时间类型的怪物剩余时间
	m_leftTime = CUtil::GetNowSecond() - GetOwner()->getBornTime();
	if(aliveTime >= m_leftTime)
	{
		m_leftTime = aliveTime - m_leftTime;
	}
	else
	{
		m_leftTime = 0;
	}

}



