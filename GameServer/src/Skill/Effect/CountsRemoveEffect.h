/*
 * CountsRemoveEffect.h
 *
 *  Created on: Oct 16, 2015
 *      Author: root
 */

#ifndef COUNTSREMOVEEFFECT_H_
#define COUNTSREMOVEEFFECT_H_

#include "Effect.h"
#include "EventSet/EventArgs.h"
#include "../../EventSet/EventDefine.h"

class CountsRemoveEffect : public Effect
{
public:
	CountsRemoveEffect(BuffBase& father);

	~CountsRemoveEffect();

public:
	virtual void enter(const BuffInfo& info,const EffectCommonData& data);

	virtual void release();

protected:
	bool onRemoveEvent(const EventArgs & e);

	void resetData();

protected:
	EventConnection m_Event;
	int 			m_Counts;
};


#endif /* COUNTSREMOVEEFFECT_H_ */
