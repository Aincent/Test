/*
 * AIStateAttack.cpp
 *
 *  Created on: 2013��10��23��
 *      Author: helh
 */

#include "AIStateAttack.h"
#include "../FileLoader/AILoader.h"
#include "Log/Logger.h"
#include "../Monster/Monster.h"

AIStateAttack::AIStateAttack(AIBrain * owner):AIState(owner),m_attackTime(0)
{

}

AIStateAttack::~AIStateAttack()
{

}

void AIStateAttack::Update(DWORD dwDeltaTick)
{
	GetBrain()->ChangeState(eAIStateType_Idle);
}

void AIStateAttack::BeginState()
{
}

void AIStateAttack::EndState()
{
}

TowerMonsterAttack::TowerMonsterAttack(AIBrain * owner):AIStateAttack(owner),m_attackDistance(0),m_betterAITime(0),m_callSkillTime(0)
{

}

TowerMonsterAttack::~TowerMonsterAttack()
{

}

void TowerMonsterAttack::Update(DWORD dwDeltaTick)
{
	BaseAIInfo *info = GetBrain()->GetBaseAI();
	if(!info)
	{
		LOG_ERROR(FILEINFO, "attack state get ai info error");

		return;
	}

	Smart_Ptr<CreatureObj> target = GetOwner()->GetTargetObj();
	if(target.Get()==NULL || target->IsDead())
	{
		TowerAIDo();
		return;
	}

	m_betterAITime += dwDeltaTick;
	m_callSkillTime += dwDeltaTick;
	m_attackTime += dwDeltaTick;
	m_attackDistance += dwDeltaTick;
	if(GetOwner()->IsAttackInLength(info->atkDistance))
	{
		if(m_attackTime >= GetOwner()->GetPhyAttackTime())
		{
			BetterAIInfo *beInfo = GetBrain()->GetBetterAI();

			if(beInfo && m_betterAITime >= beInfo->cycleTime)
			{
				//第一种策略
				if(GetBrain()->CaseStrategy(beInfo->firstType, beInfo->firstParam, beInfo->firstAction, beInfo->firstRate))
				{
					GetOwner()->Attack(beInfo->firstAction);
				}
				else if(GetBrain()->CaseStrategy(beInfo->secondType, beInfo->secondParam, beInfo->secondAction, beInfo->secondRate))
				{//第二种策略
					GetOwner()->Attack(beInfo->secondAction);
				}
				else if(beInfo->callSkill)
				{//召唤技能
					if(m_callSkillTime >= beInfo->callDelay)
					{
						if(GetBrain()->CaseStrategySkill(eStrategyHP, beInfo->callParam, beInfo->callSkillID, beInfo->callRate))
						{
							GetOwner()->Attack(beInfo->callSkillID);
						}

						m_callSkillTime = 0;
					}
				}
				else if(GetBrain()->FireSkill(beInfo->fSkillRate))
				{//第一种技能触发
					GetOwner()->Attack(beInfo->fSkillID);
				}
				else if(GetBrain()->FireSkill(beInfo->sSkillRate))
				{//第二种技能触发
					GetOwner()->Attack(beInfo->sSkillID);
				}
				else if(GetBrain()->FireSkill(beInfo->tSkillRate))
				{//第三种技能触发
					GetOwner()->Attack(beInfo->tSkillID);
				}
				else
				{//高级ai没执行，执行基本攻击
					GetOwner()->Attack(info->skillID);
				}

				//切换目标
				GetBrain()->ChangeTarget(beInfo->firstSelect,beInfo->selectRate);
				if(!GetBrain()->IsExistedEnemy())
				{
					GetBrain()->ChangeState(eAIStateType_Return);
				}

				m_betterAITime = 0;
			}
			else
			{
				if(info->skillID != 0)
				{
					GetOwner()->Attack(info->skillID);
				}
			}

			m_attackTime = 0;
			GetBrain()->ClearPathAndTime();
		}
	}
	else
	{
		TowerAIDo();
	}
}

void TowerMonsterAttack::BeginState()
{
	//被动怪被攻击后，第一次攻击应该马上打出
	BaseAIInfo *info = GetBrain()->GetBaseAI();
	if(!info)
	{
		LOG_ERROR(FILEINFO, "attack state get ai info error");

		return;
	}

	GetBrain()->ClearPathAndTime();
	if(info->atkType == 2)
	{
		m_attackTime = info->atkTime - 300;
	}
	else
	{
		m_attackTime = 0;
	}

	m_betterAITime = 0;
	m_attackDistance = 0;
	m_callSkillTime = 0;
}

void TowerMonsterAttack::EndState()
{
	GetBrain()->ClearPathAndTime();
}

void TowerMonsterAttack::TowerAIDo()
{
	GetBrain()->DeleteEnemy(GetOwner()->GetTargetObj());

	//选择在攻击范围内的玩家，设定目标
	Smart_Ptr<CreatureObj> player = GetBrain()->GetMinLength();
	if(!player)
	{
		GetBrain()->ChangeState(eAIStateType_Idle);
		return;
	}

	GetOwner()->SetTarget(player);

}

CommonMonsterAttack::CommonMonsterAttack(AIBrain * owner):AIStateAttack(owner),m_attackDistance(0),m_betterAITime(0),m_callSkillTime(0)
{

}

CommonMonsterAttack::~CommonMonsterAttack()
{

}

void CommonMonsterAttack::Update(DWORD dwDeltaTick)
{
	BaseAIInfo *info = GetBrain()->GetBaseAI();
	if(!info)
	{
		LOG_ERROR(FILEINFO, "attack state get ai info error");

		return;
	}

	Smart_Ptr<CreatureObj> target = GetBrain()->GetOwner()->GetTargetObj();

	if(!target || target->IsDead() || !target->IsInMap())
	{
		//目标死亡了，那么切换目标
		if(!GetBrain()->IsExistedEnemy())
		{
			GetBrain()->ChangeState(eAIStateType_Return);
		}
		else
		{
			Smart_Ptr<CreatureObj> tarObj = GetBrain()->GetMinLength();
			GetOwner()->SetTarget(tarObj);
		}

		return;
	}

	m_betterAITime += dwDeltaTick;
	m_callSkillTime += dwDeltaTick;
	m_attackTime += dwDeltaTick;
	m_attackDistance += dwDeltaTick;
	if(GetOwner()->IsAttackInLength(info->atkDistance))
	{
		if(m_attackTime >= GetOwner()->GetPhyAttackTime())
		{
			BetterAIInfo *beInfo = GetBrain()->GetBetterAI();

			if(beInfo && m_betterAITime >= beInfo->cycleTime)
			{
				//第一种策略
				if(GetBrain()->CaseStrategy(beInfo->firstType, beInfo->firstParam, beInfo->firstAction, beInfo->firstRate))
				{
					GetOwner()->Attack(beInfo->firstAction);
				}
				else if(GetBrain()->CaseStrategy(beInfo->secondType, beInfo->secondParam, beInfo->secondAction, beInfo->secondRate))
				{//第二种策略
					GetOwner()->Attack(beInfo->secondAction);
				}
				else if(beInfo->callSkill)
				{//召唤技能
					if(m_callSkillTime >= beInfo->callDelay)
					{
						if(GetBrain()->CaseStrategySkill(eStrategyHP, beInfo->callParam, beInfo->callSkillID, beInfo->callRate))
						{
							GetOwner()->Attack(beInfo->callSkillID);
						}

						m_callSkillTime = 0;
					}
				}
				else if(GetBrain()->FireSkill(beInfo->fSkillRate))
				{//第一种技能触发
					GetOwner()->Attack(beInfo->fSkillID);
				}
				else if(GetBrain()->FireSkill(beInfo->sSkillRate))
				{//第二种技能触发
					GetOwner()->Attack(beInfo->sSkillID);
				}
				else if(GetBrain()->FireSkill(beInfo->tSkillRate))
				{//第三种技能触发
					GetOwner()->Attack(beInfo->tSkillID);
				}
				else
				{//高级ai没执行，执行基本攻击
					GetOwner()->Attack(info->skillID);
				}

				//切换目标
				GetBrain()->ChangeTarget(beInfo->firstSelect,beInfo->selectRate);
				if(!GetBrain()->IsExistedEnemy())
				{
					GetBrain()->ChangeState(eAIStateType_Return);
				}

				m_betterAITime = 0;
			}
			else
			{
				if(info->skillID != 0)
				{
					GetOwner()->Attack(info->skillID);
				}
			}

			m_attackTime = 0;
			GetBrain()->ClearPathAndTime();
		}
	}
	else
	{
		//不在攻击范围，移动到过去
		GetBrain()->UpdatePath(dwDeltaTick);

		//是否在范围内
		if(!GetOwner()->IsInRunDistance(info->runDistance))
		{
			//更新下所在范围的玩家
			GetBrain()->DeleteEnemy(GetOwner()->GetTargetObj());

			//策划要求，一旦怪物出了最远追击，就让他返回
			GetBrain()->ChangeState(eAIStateType_Return);
		}
	}

}
void CommonMonsterAttack::BeginState()
{
	//被动怪被攻击后，第一次攻击应该马上打出
	BaseAIInfo *info = GetBrain()->GetBaseAI();
	if(!info)
	{
		LOG_ERROR(FILEINFO, "attack state get ai info error");

		return;
	}

	GetBrain()->ClearPathAndTime();
	if(info->atkType == 2)
	{
		m_attackTime = info->atkTime - 300;
	}
	else
	{
		m_attackTime = 0;
	}

	m_betterAITime = 0;
	m_attackDistance = 0;
	m_callSkillTime = 0;
}

void CommonMonsterAttack::EndState()
{
	GetBrain()->ClearPathAndTime();
}

CompeteMonsterAttack::CompeteMonsterAttack(AIBrain * owner):AIStateAttack(owner)
{
	m_cdTime.resize(eSkill_Max);
}

CompeteMonsterAttack::~CompeteMonsterAttack()
{

}

void CompeteMonsterAttack::Update(DWORD dwDeltaTick)
{
	Smart_Ptr<CreatureObj> tCreature = GetOwner()->GetTargetObj();
	if(!tCreature || tCreature->IsDead() || !tCreature->IsInMap())
	{
		if(!GetBrain()->IsExistedEnemy())
		{
			GetBrain()->ChangeState(eAIStateType_Return);
		}
		else
		{
			Smart_Ptr<CreatureObj> tarObj = GetBrain()->GetMinLength();
			GetOwner()->SetTarget(tarObj);
		}
		return;
	}

	m_attackTime += dwDeltaTick;
	if(m_attackTime < GetOwner()->GetPhyAttackTime())
	{
		return;
	}

	BaseAIInfo *info = GetBrain()->GetBaseAI();
	vector<int> vec;
	bool isAttack = false;

	if(!info)
	{
		LOG_ERROR(FILEINFO, "attack state get ai info error");

		return;
	}

	vec = GetOwner()->GetSkill();
	if(vec.size() <= 0)
	{
		GetBrain()->ChangeState(eAIStateType_Return);

		return;
	}

	//判断自身是否处于目标攻击范围内
	//＠todo ： 这个ai只有竞技场里面用，对手只能是玩家
	Smart_Ptr<Player> player;
	if(GetOwner()->GetTargetObj()->GetType() == ePlayer)
	{
		player = GetOwner()->GetTargetObj();
	}

	if(player && !player->IsAttackDistance(GetOwner()->GetXNewPos(), GetOwner()->GetYNewPos()))
	{
		//不在攻击范围，释放技能a,b
		if(vec[eSkill_A] > 0)
		{
			if(IsCanFire(eSkill_A))
			{
				SkillInfo *info = 0;

				info = SkillLoader::GetInstance()->GetSkillInfoByID(vec[eSkill_A]);
				if(info != 0)
				{
					GetOwner()->Attack(vec[eSkill_A]);
					StartCD(eSkill_A, info->coolTime);
					isAttack = true;
				}
			}
		}

		if(vec[eSkill_B] > 0)
		{
			if(IsCanFire(eSkill_B))
			{
				SkillInfo *info = 0;

				info = SkillLoader::GetInstance()->GetSkillInfoByID(vec[eSkill_B]);
				if(info != 0)
				{
					GetOwner()->Attack(vec[eSkill_B]);
					StartCD(eSkill_B, info->coolTime);
					isAttack = true;
				}
			}
		}
	}

	bool isFire = false;

	if(!GetOwner()->GetTargetObj())
	{
		GetBrain()->ChangeState(eAIStateType_Return);

		return;
	}

	if(GetOwner()->IsAttackInLength(info->atkDistance))
	{
		if(vec[eSkill_D] > 0)
		{
			if(IsCanFire(eSkill_D))
			{
				SkillInfo *info = 0;

				info = SkillLoader::GetInstance()->GetSkillInfoByID(vec[eSkill_D]);
				if(info != 0)
				{
					GetOwner()->Attack(vec[eSkill_D]);
					isFire = true;
					isAttack = true;
					StartCD(eSkill_D, info->coolTime);
				}
			}
		}

		if(!isFire && vec[eSkill_E] > 0)
		{
			if(IsCanFire(eSkill_E))
			{
				SkillInfo *info = 0;

				info = SkillLoader::GetInstance()->GetSkillInfoByID(vec[eSkill_E]);
				if(info != 0)
				{
					GetOwner()->Attack(vec[eSkill_E]);
					isFire = true;
					isAttack = true;
					StartCD(eSkill_E, info->coolTime);
				}
			}
		}

		if(!isFire && vec[eSkill_F] > 0)
		{
			if(IsCanFire(eSkill_F))
			{
				SkillInfo *info = 0;

				info = SkillLoader::GetInstance()->GetSkillInfoByID(vec[eSkill_F]);
				if(info != 0)
				{
					GetOwner()->Attack(vec[eSkill_F]);
					isFire = true;
					isAttack = true;
					StartCD(eSkill_F, info->coolTime);
				}
			}
		}

		if(!isFire && vec[eSkill_G] > 0)
		{
			if(IsCanFire(eSkill_G))
			{
				SkillInfo *info = 0;

				info = SkillLoader::GetInstance()->GetSkillInfoByID(vec[eSkill_G]);
				if(info != 0)
				{
					GetOwner()->Attack(vec[eSkill_G]);
					isFire = true;
					isAttack = true;
					StartCD(eSkill_G, info->coolTime);
				}
			}
		}

		if(!isFire && vec[eSkill_H] > 0)
		{
			if(IsCanFire(eSkill_H))
			{
				SkillInfo *info = 0;

				info = SkillLoader::GetInstance()->GetSkillInfoByID(vec[eSkill_H]);
				if(info != 0)
				{
					GetOwner()->Attack(vec[eSkill_H]);
					isFire = true;
					isAttack = true;
					StartCD(eSkill_H, info->coolTime);
				}
			}
		}

		if(!isFire && vec[eSkill_I] > 0)
		{
			if(IsCanFire(eSkill_I))
			{
				SkillInfo *info = 0;

				info = SkillLoader::GetInstance()->GetSkillInfoByID(vec[eSkill_I]);
				if(info != 0)
				{
					GetOwner()->Attack(vec[eSkill_I]);
					isFire = true;
					isAttack = true;
					StartCD(eSkill_I, info->coolTime);
				}
			}
		}
	}
	else
	{
		if(vec[eSkill_C] > 0)
		{
			if(IsCanFire(eSkill_C))
			{
				SkillInfo *info = 0;

				info = SkillLoader::GetInstance()->GetSkillInfoByID(vec[eSkill_C]);
				if(info != 0)
				{
					GetOwner()->Attack(vec[eSkill_C]);
					isAttack = true;
					StartCD(eSkill_C, info->coolTime);
				}
			}
		}
	}

	if(!isAttack)
	{
		GetBrain()->UpdatePath(dwDeltaTick);

		{
			if(!GetOwner()->GetTargetObj())
			{
				GetBrain()->ChangeState(eAIStateType_Return);

				return;
			}

			//是否在范围内
			if(!GetOwner()->IsInRunDistance(info->runDistance))
			{
				//更新下所在范围的玩家
				GetBrain()->DeleteEnemy(GetOwner()->GetTargetObj());

				//没有敌人存在了就回去
				if(!GetBrain()->IsExistedEnemy())
				{
					GetBrain()->ChangeState(eAIStateType_Return);
				}
				else
				{
					//否则选择在攻击范围内的玩家，设定目标
					Smart_Ptr<CreatureObj> player = GetBrain()->GetMinLength();
					GetOwner()->SetTarget(player);
				}
			}
		}
	}
	else
	{
		m_attackTime = 0;
		GetBrain()->ClearPathAndTime();
	}
}

void CompeteMonsterAttack::BeginState()
{
	m_attackTime = 1000;
	GetBrain()->ClearPathAndTime();
}

void CompeteMonsterAttack::EndState()
{
	GetBrain()->ClearPathAndTime();
}

