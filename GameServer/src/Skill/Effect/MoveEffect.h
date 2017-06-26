/*
 * MoveEffect.h
 *
 *  Created on: Oct 15, 2015
 *      Author: root
 */

#ifndef MOVEEFFECT_H_
#define MOVEEFFECT_H_

#include "Timer/TimerInterface.h"
#include "Effect.h"

using namespace CommBaseOut;

class MoveEffect : public Effect
{
public:
	MoveEffect(BuffBase& father);

	~MoveEffect();

public:
	virtual void enter(const BuffInfo& info,const EffectCommonData& data);

	virtual void release();

protected:
	void time(void* p);

private:
	TimerConnection m_Timer;
	EffectPos	 	m_TargetPos;
};


#endif /* MOVEEFFECT_H_ */
