/*
 * FixedShield.cpp
 *
 *  Created on: 26/10/2015
 *      Author: root
 */

#include "FixedShield.h"
#include "../../EventSet/EventDefine.h"
#include "EventSet/EventArgs.h"
#include "../../Object/Creature.h"
#include "../../ServerEventDefine.h"
#include "../../Object/Creature.h"
#include "../../Attribute/AttributeCreature.h"


FixedShield::FixedShield(BuffBase& father) :
Effect(father),
m_ShieldValue(0)
{

}

FixedShield::~FixedShield()
{

}

void FixedShield::enter(const BuffInfo& info,const EffectCommonData& data)
{
	m_Event = RegEvent(m_Father.GetOwner(),data.m_EventID,this,&FixedShield::onDamageEvent);

	//属性百分比
	if(info.params)
	{
		m_ShieldValue =(int)(info.params * m_Father.GetOwner()->getAttManage()->getValue<int>((ECharAttrType)info.m_pParam->m_Param) / 100);
	}
	//固定值
	else
	{
		m_ShieldValue = (int)info.m_pParam->m_Param;
	}

	return ;
}

void FixedShield::release()
{
	Effect::release();

	resetData();
}

bool FixedShield::onDamageEvent(const EventArgs & e)
{
	const BeforeBeHurtArgs& tempArgs = static_cast<const BeforeBeHurtArgs&>(e);

	//盾不能抵消所有伤害
	if(tempArgs.m_DamageValue >= m_ShieldValue)
	{
		tempArgs.m_DamageValue -= m_ShieldValue;
		m_ShieldValue 		    = 0;
	}
	else
	{
		m_ShieldValue 		  -= tempArgs.m_DamageValue;
		tempArgs.m_DamageValue = 0;
	}

	if(m_ShieldValue <= 0)
	{
		resetData();
		m_Father.End();
	}

	if(tempArgs.m_DamageValue <= 0)
	{
		//防止为负
		tempArgs.m_DamageValue = 0;

		//打断该事件继续传播
		return false;
	}
	else
	{
		//如果还有另外该类型盾,该事件继续发送
		return true;
	}

	return true;
}

void FixedShield::resetData()
{
	m_Event.SetNull();
	m_ShieldValue = 0;

	return ;
}






