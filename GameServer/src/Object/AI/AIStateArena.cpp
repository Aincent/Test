/*
 * AIStateArena.cpp
 *
 *  Created on: 2014年10月14日
 *      Author: root
 */

#include "AIStateArena.h"
#include "Log/Logger.h"
#include "../Monster/Monster.h"


AIStateArena::AIStateArena(AIBrain * owner):AIState(owner),m_attackTime(0)
{
	m_cdTime.resize(eSkill_Max);
}

AIStateArena::~AIStateArena()
{

}

void AIStateArena::Update(DWORD dwDeltaTick)
{
	Smart_Ptr<CreatureObj> tCreature = GetOwner()->GetTargetObj();
	if(!GetOwner()->GetTargetObj())
	{
		GetBrain()->ChangeState(eAIStateType_Return);

		return;
	}

	m_attackTime += dwDeltaTick;
	if(m_attackTime < GetOwner()->GetPhyAttackTime())
	{
//		GetBrain()->UpdatePath(dwDeltaTick);
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

	//判断自身是否处于目标攻击范围内
	//＠todo ： 这个ai只有竞技场里面用，对手只能是玩家
	Smart_Ptr<Player> player = GetOwner()->GetTargetObj();
	if(!player->IsAttackDistance(GetOwner()->GetXNewPos(), GetOwner()->GetYNewPos()))
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

void AIStateArena::BeginState()
{
	m_attackTime = 1000;
	GetBrain()->ClearPathAndTime();
}

void AIStateArena::EndState()
{
	GetBrain()->ClearPathAndTime();
}

