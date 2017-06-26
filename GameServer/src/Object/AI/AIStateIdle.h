/*
 * AIStateIdle.h
 *
 *  Created on: 2013��10��23��
 *      Author: helh
 */

#ifndef AISTATEIDLE_H_
#define AISTATEIDLE_H_

#include "AIState.h"

class Player;

//怪物巡逻状态的类型
enum EMonsterIdleType
{
	eSimpleIdle=0,//不巡逻不攻击
	eActIdle,//主动攻击且巡逻
	eGuardIdle,//主动攻击红名且巡逻
	eTowerIdle,//主动攻击且不会动
	eEscortIdle,//不攻击且会移动
	eBossIdle,//主动攻击且巡逻(会加血)
};

//不巡逻不攻击
class AIStateIdle : public AIState
{
public:
	AIStateIdle(AIBrain * owner);
	virtual ~AIStateIdle();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();
	virtual void SetTarget(Smart_Ptr<CreatureObj> & player);
};

//一般主动怪（主动攻击且会巡逻）
class ActMonsterIdle : public AIStateIdle
{
public:
	ActMonsterIdle(AIBrain * owner);
	~ActMonsterIdle();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();
	virtual void SetTarget(Smart_Ptr<CreatureObj> & player);
};

//守卫怪（主动攻击红名且会巡逻）
class GuardMonsterIdle : public AIStateIdle
{
public:
	GuardMonsterIdle(AIBrain * owner);
	~GuardMonsterIdle();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();
	virtual void SetTarget(Smart_Ptr<CreatureObj> & player);
};

//塔怪（主动攻击且不会动）
class TowerMonsterIdle : public AIStateIdle
{
public:
	TowerMonsterIdle(AIBrain * owner);
	~TowerMonsterIdle();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();
	virtual void SetTarget(Smart_Ptr<CreatureObj> & player);
};

//镖车怪（不攻击且会移动动）
class EscortMonsterIdle : public AIStateIdle
{
public:
	EscortMonsterIdle(AIBrain * owner);
	~EscortMonsterIdle();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();
};

//boss怪（主动攻击且会巡逻,会加血）
class BossMonsterIdle : public AIStateIdle
{
public:
	BossMonsterIdle(AIBrain * owner);
	~BossMonsterIdle();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();
	virtual void SetTarget(Smart_Ptr<CreatureObj> & player);

private:

	int m_hpTick;
};

#endif /* AISTATEIDLE_H_ */
