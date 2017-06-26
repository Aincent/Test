/*
 * AIStateAttack.h
 *
 *  Created on: 2013��10��23��
 *      Author: root
 */

#ifndef AISTATEATTACK_H_
#define AISTATEATTACK_H_


#include "AIState.h"

//怪物攻击状态的类型
enum EMonsterAttackType
{
	eSimpleAttack=0,//不攻击不走动，立即切回到巡逻状态
	eTowerAttack,//主动攻击且不会动
	eCommonAttack,//攻击且会移动
	eCompeteAttack,//攻击并且移动,并且有就角色的技能
};

//不攻击
class AIStateAttack : public AIState
{
public:
	AIStateAttack(AIBrain * owner);
	virtual ~AIStateAttack();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();

	int GetAttackTime() { 	return m_attackTime; }

protected:
	int m_attackTime; //ս������ʱ��
};

//塔怪攻击（攻击并且不移动）
class TowerMonsterAttack : public AIStateAttack
{
public:
	TowerMonsterAttack(AIBrain * owner);
	~TowerMonsterAttack();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();

private:
	void TowerAIDo();

private:
	int m_attackDistance; //�������ʱ��
	int m_betterAITime; //�м�ai��ѯʱ��
	int m_callSkillTime; //�ٻ�����ʱ��
};

//一般怪攻击（攻击并且移动）
class CommonMonsterAttack : public AIStateAttack
{
public:
	CommonMonsterAttack(AIBrain * owner);
	~CommonMonsterAttack();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();

private:
	int m_attackDistance; //�������ʱ��
	int m_betterAITime; //�м�ai��ѯʱ��
	int m_callSkillTime; //�ٻ�����ʱ��
};

//竞技场怪攻击（攻击并且移动,并且有就角色的技能）
class CompeteMonsterAttack : public AIStateAttack
{
public:
	CompeteMonsterAttack(AIBrain * owner);
	~CompeteMonsterAttack();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();

	//判断cd
	bool IsCanFire(int id)
	{
		if(id < 0 || id >= eSkill_Max)
			return false;

		int64 nowTime = CUtil::GetNowSecond();
		if(nowTime > m_cdTime[id])
			return true;

		return false;
	}
	//加入cd
	void StartCD(int id, int time)
	{
		if(id < 0 || id >= eSkill_Max || time <= 0)
			return;

		m_cdTime[id] = CUtil::GetNowSecond() + time;
	}

private:
	vector<int64> m_cdTime; //技能的cd频道
};



#endif /* AISTATEATTACK_H_ */
