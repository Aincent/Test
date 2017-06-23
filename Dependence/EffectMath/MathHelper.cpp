/*
 * MathHelper.cpp
 *
 *  Created on: 22/10/2015
 *      Author: root
 */


#include "MathHelper.h"
#include "EffectVector.h"
#include <math.h>


//向量规范划
EffectVector* MathHelper::VectorNormalize( EffectVector *pvOut, EffectVector *pvIn)
{
	float fLength = sqrt( pvIn->x * pvIn->x + pvIn->y * pvIn->y + pvIn->z * pvIn->z );

	if ( fLength > 1e-08 )
	{
		float fInvLength = 1.0f / fLength;
		pvOut->x = pvIn->x*fInvLength;
		pvOut->y = pvIn->y*fInvLength;
		pvOut->z = pvIn->z*fInvLength;
	}

	return pvOut;
}

//向量点积
float MathHelper::VectorDotProduct( const EffectVector *pv1, const EffectVector *pv2)
{
	return ( pv1->x * pv2->x + pv1->y * pv2->y + pv1->z * pv2->z);
}

//向量叉乘
EffectVector* MathHelper::VectorCrossProduct( EffectVector *pvOut, EffectVector *pv1, EffectVector *pv2)
{
	pvOut->x = pv1->y * pv2->z - pv1->z * pv2->y;
	pvOut->y = pv1->z * pv2->x - pv1->x * pv2->z;
	pvOut->z = pv1->x * pv2->y - pv1->y * pv2->x;
	return ( pvOut);
}

bool MathHelper::Math_LeftOrRight(const EffectVector& p, const EffectVector& p1, const EffectVector& p2)
{
	EffectVector p3 = p1; p3.z += 1;

	EffectVector v1 = p3 - p1; VectorNormalize(&v1, &v1);
	EffectVector v2 = p2 - p1; VectorNormalize(&v2, &v2);
	EffectVector vCross; VectorCrossProduct(&vCross, &v1, &v2); VectorNormalize(&vCross, &vCross);

	EffectVector v3 = p - p1; VectorNormalize(&v3, &v3);
	float fDot = VectorDotProduct(&vCross, &v3);

	return fDot < -0.05f ? false : true;
}

void MathHelper::Math_GetRectFromXXX(const EffectVector& pos, const EffectVector& Dir, float length, float width, EffectVector* pRectOut)
{
	EffectVector p1 = pos;
	EffectVector p2 = pos; p2 += Dir * length;

	EffectVector vUp(.0f,.0f, 1.0f);
	EffectVector vDir = Dir;
	EffectVector vLeft;
	VectorCrossProduct(&vLeft, &vDir, &vUp);
	VectorNormalize(&vLeft, &vLeft);

	pRectOut[0] = p2 + ( vLeft) * width*0.5f;
	pRectOut[1] = p2 + (-vLeft) * width*0.5f;
	pRectOut[2] = p1 + (-vLeft) * width*0.5f;
	pRectOut[3] = p1 + ( vLeft) * width*0.5f;
}

void MathHelper::getRoundAllPos(const EffectVector& pos,float radius,vector<EffectVector>& result)
{
	int tBeginXPos = (int)(pos.x + -1 * radius);
	int tBeginYPos = (int)(pos.y + -1 * radius);

	int tEndXPos = (int)(pos.x + radius);
	int tEndYPos = (int)(pos.y + radius);

	float tRadiusSquare = radius * radius;
	EffectVector tPos;

	for(int i = tBeginXPos;i <= tEndXPos;++i)
	{
		for(int j = tBeginYPos;j <= tEndYPos;++j)
		{
			tPos.x = i;
			tPos.y = j;

			tPos = tPos - pos;

			if(tPos.getLength2() > tRadiusSquare)
			{
				continue ;
			}

			result.push_back(EffectVector((float)i,(float)j,.0f));
		}
	}

	return ;
}

void MathHelper::getPolygonAllPos(const vector<EffectVector>& sourPosList,vector<EffectVector>& result)
{
	//最少3个点
	if(sourPosList.size() < 3)
	{
		return ;
	}

	int tBeginXPos = (int)sourPosList.begin()->x;
	int tBeginYPos = (int)sourPosList.begin()->y;

	int tEndXPos = (int)sourPosList.begin()->x;
	int tEndYPos = (int)sourPosList.begin()->y;

	for(size_t i = 0;i < sourPosList.size();++i)
	{
		if(sourPosList[i].x < tBeginXPos)
		{
			tBeginXPos = (int)sourPosList[i].x;
		}

		if(sourPosList[i].y < tBeginYPos)
		{
			tBeginYPos = (int)sourPosList[i].y;
		}

		if(sourPosList[i].x > tEndXPos)
		{
			tEndXPos = (int)sourPosList[i].x;
		}

		if(sourPosList[i].y > tEndYPos)
		{
			tEndYPos = (int)sourPosList[i].y;
		}
	}

	EffectVector tCurrentPos;
	for(int i = tBeginXPos;i <= tEndXPos;++i)
	{
		for(int j = tBeginYPos;j <= tEndYPos;++j)
		{
			for(size_t k = 0;k < sourPosList.size();++k)
			{
				tCurrentPos.x = i;
				tCurrentPos.y = j;
				if(sourPosList.size()-1 == k)
				{
					if (!MathHelper::Math_LeftOrRight(tCurrentPos,sourPosList[k],sourPosList[0]))
					{
						continue ;
					}
				}
				else
				{
					if (!MathHelper::Math_LeftOrRight(tCurrentPos,sourPosList[k],sourPosList[k+1]))
					{
						continue ;
					}
				}

				result.push_back(EffectVector((float)i,(float)j,.0f));
			}
		}
	}

	return ;
}

DWORD64 MathHelper::combinationTowDWORD(DWORD high,DWORD low)
{
	DWORD64 tResult = low;
	BYTE tLowCounts = 0;
	while(low > 0)
	{
		low = low / 10;
		++tLowCounts;
	}

	BYTE tHighCounts = 0;
	DWORD64 tOneData = 0;
	while(high > 0)
	{
		++tHighCounts;
		tOneData = high % 10;
		tOneData = tOneData * (tHighCounts + tLowCounts);

	}

	return tResult;
}




