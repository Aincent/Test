/*
 * CountsRemoveEffect.cpp
 *
 *  Created on: Oct 16, 2015
 *      Author: root
 */

#include "CountsRemoveEffect.h"
#include "../../EventSet/EventDefine.h"
#include "EventSet/EventArgs.h"
#include "../../Object/Creature.h"
#include "../../ServerEventDefine.h"

CountsRemoveEffect::CountsRemoveEffect(BuffBase& father) :
Effect(father),
m_Counts(0)
{

}

CountsRemoveEffect::~CountsRemoveEffect()
{
	resetData();
}

void CountsRemoveEffect::enter(const BuffInfo& info,const EffectCommonData& data)
{
	//为0表示没有条件
	if(info.m_Condition == 0)
	{
		return ;
	}

	m_Event = RegEvent(m_Father.GetOwner(),data.m_EventID,this,&CountsRemoveEffect::onRemoveEvent);

	return ;
}

void CountsRemoveEffect::release()
{
	Effect::release();

	resetData();
}

bool CountsRemoveEffect::onRemoveEvent(const EventArgs& e)
{
	++m_Counts;

	const BuffInfo* tempInfo = m_Father.GetBuffInfoPtrByID();

	if(tempInfo == NULL)
	{
		resetData();
		m_Father.End();

		return true;
	}

	if(m_Counts < tempInfo->m_Condition)
	{
		return true;
	}

	resetData();
	m_Father.End();

	return true;
}

void CountsRemoveEffect::resetData()
{
	m_Event.SetNull();

	return ;
}





