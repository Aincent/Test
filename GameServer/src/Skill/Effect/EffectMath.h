/*
 * EffectMath.h
 *
 *  Created on: 22/10/2015
 *      Author: root
 */

#ifndef EFFECTMATH_H_
#define EFFECTMATH_H_

#include "define.h"
#include "EffectMath/EffectVector.h"
#include "EffectMath/MathHelper.h"
#include "Smart_Ptr.h"

using namespace CommBaseOut;

class CreatureObj;
class SectorGroup;

//判断场景对象是否在矩形内
class FindRectTargetByLine
{
public:
	FindRectTargetByLine(EffectVector* points,vector<Smart_Ptr<CreatureObj> >& targetList) :
    	m_targets(targetList),
		m_tzPos(points){}

	void operator()(Smart_Ptr<CreatureObj> pCharacter);

private:
	vector<Smart_Ptr<CreatureObj> >& m_targets;

	const EffectVector* const m_tzPos;
};

class EffectMath
{
public:
	static void findTargetByRect(SectorGroup &sectorGroup, const EffectVector& beginPos,
			const EffectVector& endPos,vector<Smart_Ptr<CreatureObj> >& targetList,int lenght,int wide);
};

#endif /* EFFECTMATH_H_ */
