/*
 * AIState.h
 *
 *  Created on: 2013Äê10ÔÂ23ÈÕ
 *      Author: helh
 */

#ifndef AISTATE_H_
#define AISTATE_H_

#include "AIBrain.h"
#include "../Creature.h"
#include "Smart_Ptr.h"

using namespace CommBaseOut;

class AIState
{
public:
	AIState(AIBrain * owner):m_owner(owner)
	{

	}

	virtual ~AIState()
	{

	}

	virtual void Update(DWORD dwDeltaTick) = 0;
	virtual void BeginState() = 0;
	virtual void EndState() = 0;

	Smart_Ptr<Monster> GetOwner()
	{
		return m_owner->GetOwner();
	}

	AIBrain * GetBrain()
	{
		return m_owner;
	}

private:

	AIBrain * m_owner;
};


#endif /* AISTATE_H_ */
