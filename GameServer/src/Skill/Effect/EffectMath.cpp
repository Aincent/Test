/*
 * EffectMath.cpp
 *
 *  Created on: 22/10/2015
 *      Author: root
 */



#include "EffectMath.h"
#include "../../Object/Creature.h"
#include "../../Map/Sector.h"
#include "../../Object/Monster/Monster.h"
#include "../../Object/Player/Player.h"



void FindRectTargetByLine::operator()(Smart_Ptr<CreatureObj> pCharacter)
{
	if (!pCharacter)
	{
		return ;
	}

	EffectVector characterPos;
	characterPos.x = pCharacter->GetXNewPos();
	characterPos.y = pCharacter->GetYNewPos();

	const int tmp[5] = {0, 1, 2, 3, 0};
	for (size_t i = 0; i < 4; ++i)
	{
		if (!MathHelper::Math_LeftOrRight(characterPos, m_tzPos[tmp[i]], m_tzPos[tmp[i+1]]))
		{
			return ;
		}
	}

	m_targets.push_back(pCharacter);

	return;
}

void EffectMath::findTargetByRect(SectorGroup &sectorGroup, const EffectVector& beginPos,
		const EffectVector& endPos,vector<Smart_Ptr<CreatureObj> >& targetList,int lenght,int wide)
{
	EffectVector RectVec[4];
	EffectVector direction = endPos - beginPos;

	//向量为0,随机求得一个向量
	if(direction.x == 0 && direction.y == 0 && direction.z == 0)
	{
		direction.x = rand() % 10 + 1;

		direction.y = rand() % 10 + 1;
	}

	MathHelper::VectorNormalize(&direction,&direction);

	MathHelper::Math_GetRectFromXXX(beginPos, direction,lenght,wide,RectVec);

    RectVec[0].z = 0.0f;
    RectVec[1].z = 0.0f;
    RectVec[2].z = 0.0f;
    RectVec[3].z = 0.0f;

    FindRectTargetByLine findTarget(RectVec,targetList);
    sectorGroup.foreachAll(findTarget);

    return ;
}
