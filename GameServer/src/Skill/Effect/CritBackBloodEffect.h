/*
 * CritBackBloodEffect.h
 *
 *  Created on: 26/10/2015
 *      Author: root
 */

#ifndef CRITBACKBLOODEFFECT_H_
#define CRITBACKBLOODEFFECT_H_

#include "Effect.h"
#include "EventSet/EventArgs.h"
#include "../../EventSet/EventDefine.h"

class CritBackBloodEffect : public Effect
{
public:
	CritBackBloodEffect(BuffBase& father);

	~CritBackBloodEffect();

public:
	virtual void enter(const BuffInfo& info,const EffectCommonData& data);

	virtual void release();

protected:
	bool onCritEvent(const EventArgs & e);

	void resetData();

private:
	EventConnection m_Event;
	int 			m_Counts;
};


#endif /* CRITBACKBLOODEFFECT_H_ */
