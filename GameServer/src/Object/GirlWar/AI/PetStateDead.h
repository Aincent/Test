/*
 * PetStateDead.h
 *
 *  Created on: 2014年12月8日
 *      Author: root
 */

#ifndef PETSTATEDEAD_H_
#define PETSTATEDEAD_H_

#include "PetState.h"

class Player;

class PetStateDead : public PetState
{
public:
	PetStateDead(PetBrain * owner);
	~PetStateDead();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();

private:
	DWORD m_flushTime; // ˢ��ʱ��
};


#endif /* PETSTATEDEAD_H_ */
