/*
 * MoveEffect.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: root
 */


#include "MoveEffect.h"
#include "Timer/TimerInterface.h"
#include <math.h>
#include "../../Object/Creature.h"

MoveEffect::MoveEffect(BuffBase& father) : Effect(father)
{

}

MoveEffect::~MoveEffect()
{
	release();
}

void MoveEffect::enter(const BuffInfo& info,const EffectCommonData& data)
{
	if(data.m_Target == NULL)
	{
		time(NULL);

		return ;
	}

	m_TargetPos = data.m_TargetPos;

	int temppDistanceX = data.m_TargetPos.m_X - data.m_Target->GetXPos();
	int temppDistanceY = data.m_TargetPos.m_X - data.m_Target->GetYPos();

	int tempDistance = (int)sqrt(temppDistanceX * temppDistanceX + temppDistanceY * temppDistanceY);
	int tempTime 	 = (int)(tempDistance * 1000 / info.params);

	m_Timer = TimerInterface::GetInstance()->RegTimePoint(&MoveEffect::time,this,0,0,0,0,tempTime);

	return ;
}

void MoveEffect::release()
{
	m_Timer.SetNull();

	return ;
}

void MoveEffect::time(void* p)
{
	release();

	m_Father.GetOwner()->RelocatePos(m_TargetPos.m_X,m_TargetPos.m_Y);

	m_Father.End();

	return ;
}





