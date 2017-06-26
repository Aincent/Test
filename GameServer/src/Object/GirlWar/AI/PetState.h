/*
 * PetState.h
 *
 *  Created on: 2014年12月8日
 *      Author: root
 */

#ifndef PETSTATE_H_
#define PETSTATE_H_

#include "PetBrain.h"
#include "../../Creature.h"
#include "Smart_Ptr.h"

using namespace CommBaseOut;

class PetState
{
public:
	PetState(PetBrain * owner):m_owner(owner)
	{

	}

	virtual ~PetState()
	{

	}

	virtual void Update(DWORD dwDeltaTick) = 0;
	virtual void BeginState() = 0;
	virtual void EndState() = 0;

	Smart_Ptr<GirlWar> GetOwner()
	{
		return m_owner->GetOwner();
	}

	PetBrain * GetBrain()
	{
		return m_owner;
	}

private:

	PetBrain * m_owner;
};


#endif /* PETSTATE_H_ */
