/*
 * AttributePlayer.h
 *
 *  Created on: 22/05/2015
 *      Author: root
 */
#ifndef ATTRIBUTEPLAYER_H_
#define ATTRIBUTEPLAYER_H_

#include "AttributeCreature.h"
#include "define.h"

using namespace CommBaseOut;


class AttributePlayer : public AttributeCreature
{
public:
	AttributePlayer();

	~AttributePlayer();

public:
	DWORD64 getTime(){return m_Time;}

	void setTime(DWORD64 time){m_Time = time;}

	virtual void init(void* data);

private:
	DWORD64 m_Time;
};


#endif /* ATTRIBUTEPLAYER_H_ */
