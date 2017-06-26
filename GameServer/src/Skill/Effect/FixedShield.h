/*
 * FixedShield.h
 *
 *  Created on: 26/10/2015
 *      Author: root
 */

#ifndef FIXEDSHIELD_H_
#define FIXEDSHIELD_H_

#include "Effect.h"
#include "../../EventSet/EventDefine.h"

//固定护盾类
class FixedShield : public Effect
{
public:
	FixedShield(BuffBase& father);

	~FixedShield();

public:
	virtual void enter(const BuffInfo& info,const EffectCommonData& data);

	virtual void release();

protected:
	bool onDamageEvent(const EventArgs & e);

	void resetData();

private:
	int 			m_ShieldValue;
	EventConnection m_Event;
};


#endif /* FIXEDSHIELD_H_ */
