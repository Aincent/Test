/*
 * PetStateAttack.cpp
 *
 *  Created on: 2014年12月8日
 *      Author: root
 */

#include "PetStateAttack.h"
#include "../../FileLoader/AILoader.h"
#include "Log/Logger.h"
#include "../../FileLoader/GirlWarLoader.h"
#include "../../../Map/SceneManager.h"

PetStateAttack::PetStateAttack(PetBrain* owner):PetState(owner),m_attackTime(0)
{
	m_cdTime.resize(eSkill_Max);
}

PetStateAttack::~PetStateAttack()
{

}

void PetStateAttack::Update(DWORD dwDeltaTick)
{
	Smart_Ptr<CreatureObj> target = GetBrain()->GetOwner()->GetTargetObj();
	if(!target || target->IsDead() || !(GetOwner()->GetOwner()) || (GetOwner()->GetOwner())->IsDead() || CSceneMgr::GetInstance()->GetDistance(GetBrain()->GetOwner()->GetOwner()->GetXNewPos(),GetBrain()->GetOwner()->GetOwner()->GetYNewPos(),GetBrain()->GetOwner()->GetXNewPos(),GetBrain()->GetOwner()->GetYNewPos()) > GirlWarLoader::GetInstance()->GetMoveRange(GetBrain()->GetOwner()->GetID()))
	{
		GetBrain()->ClearEnemy();

		if(target.Get()!=NULL)
		{
			if(target->GetType()==ePet||target->GetType()==eMonster)
			{
				GetBrain()->cancel(target);
			}
		}

		Smart_Ptr<CreatureObj> obj;
		GetBrain()->GetOwner()->SetTarget(obj);

		GetBrain()->ChangeState(ePetStateType_Idle);

		return;
	}

	//主人在打坐时，战积不能动
	Smart_Ptr<Player> owner = GetOwner()->GetOwner();
	if(owner.Get() != NULL && owner->IsInCreatureState(ERS_Meditation))
	{
		return;
	}

	if(GetOwner()->IsAttackInLength(GirlWarLoader::GetInstance()->GetAckRange(GetOwner()->GetID())))
	{
		m_attackTime += dwDeltaTick;
		if(m_attackTime >= GetOwner()->GetPhyAttackTime())
		{
			if(IsCanFire(eSkill_A))
			{
				SkillInfo *info = 0;
				info = SkillLoader::GetInstance()->GetSkillInfoByID(GetOwner()->GetSkill1());
				if(info != 0)
				{
					GetOwner()->Attack(GetOwner()->GetSkill1());
					StartCD(eSkill_A, info->coolTime);
				}
			}
			else
			{
				GetOwner()->Attack(GirlWarLoader::GetInstance()->GetCommonSkill(GetOwner()->GetID()));
			}

			m_attackTime = 0;
		}

		GetBrain()->ClearPathAndTime();
	}
	else
	{
		//不在攻击范围，移动到过去
		GetBrain()->UpdatePath(dwDeltaTick);
		m_attackTime = 0;
	}
}


void PetStateAttack::BeginState()
{
	//被动怪被攻击后，第一次攻击应该马上打出
	GetBrain()->ClearPathAndTime();
	m_attackTime = 0;
}


void PetStateAttack::EndState()
{
	GetBrain()->ClearPathAndTime();
}

