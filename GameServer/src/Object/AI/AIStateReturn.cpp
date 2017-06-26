/*
 * AIStateReturn.cpp
 *
 *  Created on: 2013��10��23��
 *      Author: helh
 */

#include "AIStateReturn.h"
#include "../Monster/Monster.h"
#include "../../Map/SceneManager.h"

AIStateReturn::AIStateReturn(AIBrain * owner):AIState(owner)
{

}

AIStateReturn::~AIStateReturn()
{

}

void AIStateReturn::Update(DWORD dwDeltaTick)
{
	GetBrain()->ChangeState(eAIStateType_Idle);
}

void AIStateReturn::BeginState()
{
	Smart_Ptr<CreatureObj> player;
	GetOwner()->SetTarget(player);
	GetBrain()->ClearEnemyOnly();
}

void AIStateReturn::EndState()
{

}

CommonMonsterReturn::CommonMonsterReturn(AIBrain * owner):AIStateReturn(owner)
{

}

CommonMonsterReturn::~CommonMonsterReturn()
{

}

void CommonMonsterReturn::Update(DWORD dwDeltaTick)
{
	GetBrain()->UpdatePath(dwDeltaTick);

	if(GetOwner()->IsOriginalPos())
	{
		GetBrain()->ChangeState(eAIStateType_Idle);
	}
}

BossMonsterReturn::BossMonsterReturn(AIBrain * owner):AIStateReturn(owner),m_hpTick(0)
{

}

BossMonsterReturn::~BossMonsterReturn()
{

}

void BossMonsterReturn::Update(DWORD dwDeltaTick)
{
	GetBrain()->UpdatePath(dwDeltaTick);

	if(GetOwner()->IsOriginalPos())
	{
		GetBrain()->ChangeState(eAIStateType_Idle);
	}

	Smart_Ptr<Monster> mon = GetOwner();

	m_hpTick += dwDeltaTick;
	if(m_hpTick >= 1000 && mon->GetTypeValue(eCharHP) < mon->GetTypeValue(eCharHPMax))
	{
		m_hpTick -= 1000;
		mon->CurHPChange(mon->GetTypeValue(eCharHPMax) * 5 / 100, true);

		CharBase::SynchObjectAttr attr;
		CharBase::CharAttChange * change = attr.mutable_attr();

		attr.set_key(mon->GetKey());
		change->add_type(eCharHP);
		change->add_value(mon->GetTypeValue(eCharHP));
		change->add_state(0);

		CSceneMgr::GetInstance()->SendSynchAttrChange(attr, mon->GetMapID(), mon->GetXNewPos(), mon->GetYNewPos());
	}
}

void BossMonsterReturn::BeginState()
{
	m_hpTick = 0;
}


