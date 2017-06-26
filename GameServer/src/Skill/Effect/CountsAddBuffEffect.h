/*
 * CountsAddBuffEffect.h
 *
 *  Created on: Oct 16, 2015
 *      Author: root
 */

#ifndef COUNTSADDBUFFEFFECT_H_
#define COUNTSADDBUFFEFFECT_H_

#include "Effect.h"
#include "../../EventSet/EventDefine.h"

class CountsAddBuffEffect : public Effect
{
public:
	CountsAddBuffEffect(BuffBase& father);

	~CountsAddBuffEffect();

public:
	virtual void enter(const BuffInfo& info,const EffectCommonData& data);

	virtual void release();

protected:
	bool onAddEvent(const EventArgs & e);

	void resetData();

	void doThing(const BuffInfo& info,Smart_Ptr<CreatureObj> target);

private:
	EventConnection m_Event;
	int 			m_Counts;
};


#endif /* COUNTSADDBUFFEFFECT_H_ */
