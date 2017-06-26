/*
 * PetStateIdle.cpp
 *
 *  Created on: 2014年12月8日
 *      Author: root
 */

#include "PetStateIdle.h"

PetStateIdle::PetStateIdle(PetBrain* owner):PetState(owner)
{

}

PetStateIdle::~PetStateIdle()
{

}

void PetStateIdle::Update(DWORD dwDeltaTick)
{
	GetBrain()->UpdatePath(dwDeltaTick);
}

void PetStateIdle::BeginState()
{
	GetBrain()->ClearPathAndTime();
	GetOwner()->BeginIdle();
}

void PetStateIdle::EndState()
{

}
