/*
 * AIStateDead.h
 *
 *  Created on: 2013��10��23��
 *      Author: helh
 */

#ifndef AISTATEDEAD_H_
#define AISTATEDEAD_H_

#include "AIState.h"

//怪物死亡状态的类型
enum EMonsterDeadType
{
	eSimpleDead=0,//默认的死亡，不带自动复活的
	eCommonDead//一般怪物的死亡
};

class AIStateDead : public AIState
{
public:
	AIStateDead(AIBrain * owner);
	virtual ~AIStateDead();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();

	DWORD64 getLeftTime();

protected:
	DWORD m_flushTime; 	//普通怪物刷新时间
	DWORD64 m_leftTime;  //存活时间怪物刷新剩余时间
};

//一般怪物的死亡
class CommonMonsterDead : public AIStateDead
{
public:
	CommonMonsterDead(AIBrain * owner);
	~CommonMonsterDead();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
};





#endif /* AISTATEDEAD_H_ */
