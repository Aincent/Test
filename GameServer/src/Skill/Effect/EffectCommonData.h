/*
 * EffectCommonData.h
 *
 *  Created on: Oct 16, 2015
 *      Author: root
 */

#ifndef EFFECTCOMMONDATA_H_
#define EFFECTCOMMONDATA_H_

#include "define.h"

class CreatureObj;

struct EffectPos
{
	EffectPos() : m_X(0),m_Y(0){}
	int m_X;
	int m_Y;
};

//效果参数
struct EffectCommonData
{
	EffectCommonData() : m_Target(NULL),m_EventID(0){}
	CreatureObj* m_Target;
	EffectPos	 m_TargetPos;
	int			 m_EventID;
};

//直线斜截式
struct StraightLineBeveled
{
	StraightLineBeveled() : m_Slope(0.f),m_B(0.f){}

	float m_Slope;
	float m_B;
};

//直线一般式
struct StraightLineGeneral
{
	StraightLineGeneral() : m_A(0.f),m_B(0.f),m_C(0.f){}

	void operator = (const StraightLineBeveled& data)
	{
		m_A = data.m_Slope;
		m_B = -1;
		m_C = data.m_B;
	}

	float m_A;
	float m_B;
	float m_C;
};

struct RectangleData
{
	StraightLineGeneral m_One;
	StraightLineGeneral m_Two;
	StraightLineGeneral m_Three;
	StraightLineGeneral m_Four;
};


#endif /* EFFECTCOMMONDATA_H_ */
