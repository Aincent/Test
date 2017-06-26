/*
 * CountsAddBuffEffect.cpp
 *
 *  Created on: Oct 16, 2015
 *      Author: root
 */


#include "CountsAddBuffEffect.h"
#include "../../EventSet/EventDefine.h"
#include "EventSet/EventArgs.h"
#include "../../Object/Creature.h"
#include "../../ServerEventDefine.h"

CountsAddBuffEffect::CountsAddBuffEffect(BuffBase& father) :
Effect(father),
m_Counts(0)
{

}

CountsAddBuffEffect::~CountsAddBuffEffect()
{
	resetData();
}

void CountsAddBuffEffect::enter(const BuffInfo& info,const EffectCommonData& data)
{
	//为0表示没有条件
	if(info.m_Condition == 0)
	{
		return ;
	}

	m_Event = RegEvent(m_Father.GetOwner(),data.m_EventID,this,&CountsAddBuffEffect::onAddEvent);

	return ;
}

void CountsAddBuffEffect::release()
{
	Effect::release();

	resetData();

	return ;
}

bool CountsAddBuffEffect::onAddEvent(const EventArgs & e)
{
	const HurtArgs& tempArgs = static_cast<const HurtArgs&>(e);

	++m_Counts;

	const BuffInfo* tempInfo = m_Father.GetBuffInfoPtrByID();

	if(tempInfo == NULL)
	{
		resetData();
		m_Father.End();

		return true;
	}

	//只执行一次
	if(tempInfo->m_Condition > 0)
	{

		if(m_Counts < tempInfo->m_Condition)
		{
			return true;
		}

		int tempRate = rand() % 100 + 1;
		if(tempInfo->fireRate >= tempRate)
		{
			doThing(*tempInfo,tempArgs.m_Target);
		}

		resetData();

		m_Father.End();
	}
	//每次都执行
	else
	{
		int tempRate = rand() % 100 + 1;
		if(tempInfo->fireRate >= tempRate)
		{
			doThing(*tempInfo,tempArgs.m_Target);
		}

		if(abs(tempInfo->m_Condition) > m_Counts)
		{
			return true;
		}

		resetData();
		m_Father.End();
	}

	return true;
}

void CountsAddBuffEffect::resetData()
{
	m_Counts = 0;
	m_Event.SetNull();

	return ;
}

void CountsAddBuffEffect::doThing(const BuffInfo& info,Smart_Ptr<CreatureObj> target)
{
	if(info.m_TargetType == eMyself)
	{
		m_Father.GetOwner()->AddBuff((int)info.m_pParam->m_Param,m_Father.GetOwner());
	}
	else if(info.m_TargetType == eEnemyTarget)
	{
		if(!target)
		{
			return ;
		}

		if(target->GetType() == ePlayer || target->GetType() == eMonster || target->GetType() == ePet)
		{
			target->AddBuff((int)info.m_pParam->m_Param,m_Father.GetOwner());
		}
	}

	return ;
}



