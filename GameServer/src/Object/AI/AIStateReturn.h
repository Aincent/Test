/*
 * AIStateReturn.h
 *
 *  Created on: 2013��10��23��
 *      Author: helh
 */

#ifndef AISTATERETURN_H_
#define AISTATERETURN_H_


#include "AIState.h"

//怪物返回状态的类型
enum EMonsterReturnType
{
	eSimpleReturn=0,//默认的返回，立即切回到巡逻
	eCommonReturn,//一般怪物的返回
	eBossReturn,//boss的返回（加血）
};

class AIStateReturn : public AIState
{
public:
	AIStateReturn(AIBrain * owner);
	virtual ~AIStateReturn();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();

};

//一般怪物返回
class CommonMonsterReturn : public AIStateReturn
{
public:
	CommonMonsterReturn(AIBrain * owner);
	~CommonMonsterReturn();

	virtual void Update(DWORD dwDeltaTick);
};

//Boss怪物返回(会加血)
class BossMonsterReturn : public AIStateReturn
{
public:
	BossMonsterReturn(AIBrain * owner);
	~BossMonsterReturn();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();

private:

	int m_hpTick; //加血的间隔时间
};





#endif /* AISTATERETURN_H_ */
