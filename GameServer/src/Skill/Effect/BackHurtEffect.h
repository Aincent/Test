#ifndef BACKHURTEFFECT_H_
#define BACKHURTEFFECT_H_

#include "Effect.h"
#include "../../EventSet/EventDefine.h"
#include "EventSet/EventArgs.h"

class BackHurtEffect : public Effect
{
public:
	BackHurtEffect(BuffBase& father);

	virtual ~BackHurtEffect();

public:
	virtual void enter(const BuffInfo& info,const EffectCommonData& data);

	virtual void release();

protected:
	bool onDamageEvent(const EventArgs & e);

	void resetData();

private:
	EventConnection m_Event;
	int 			m_Counts;
};


#endif 
