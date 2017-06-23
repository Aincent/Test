/*
 * MathHelper.h
 *
 *  Created on: 22/10/2015
 *      Author: root
 */

#ifndef MATHHELPER_H_
#define MATHHELPER_H_
#include "define.h"

using namespace std;
using namespace CommBaseOut;

class EffectVector;

class MathHelper
{
public:
	//向量规范划
	static EffectVector* VectorNormalize( EffectVector *pvOut, EffectVector *pvIn);

	//向量点积
	static float VectorDotProduct( const EffectVector *pv1, const EffectVector *pv2);

	//向量叉乘
	static EffectVector* VectorCrossProduct( EffectVector *pvOut, EffectVector *pv1, EffectVector *pv2);

	static bool Math_LeftOrRight(const EffectVector& p, const EffectVector& p1, const EffectVector& p2);

	static void Math_GetRectFromXXX(const EffectVector& pos, const EffectVector& Dir, float length, float width, EffectVector* pRectOut);

	static void getRoundAllPos(const EffectVector& pos,float radius,vector<EffectVector>& result);

	static void getPolygonAllPos(const vector<EffectVector>& sourPosList,vector<EffectVector>& result);

	static  DWORD64 combinationTowDWORD(DWORD high,DWORD low);
};

#endif /* MATHHELPER_H_ */
