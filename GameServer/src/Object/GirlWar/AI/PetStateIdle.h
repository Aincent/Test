/*
 * PetStateIdle.h
 *
 *  Created on: 2014年12月8日
 *      Author: root
 */

#ifndef PETSTATEIDLE_H_
#define PETSTATEIDLE_H_

#include "PetState.h"

class Player;

class PetStateIdle : public PetState
{
public:
	PetStateIdle(PetBrain * owner);
	~PetStateIdle();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();
};


#endif /* PETSTATEIDLE_H_ */
