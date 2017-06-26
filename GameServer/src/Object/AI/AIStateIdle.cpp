/*
 * AIStateIdle.cpp
 *
 *  Created on: 2013��10��23��
 *      Author: helh
 */

#include "AIStateIdle.h"
#include "../Player/Player.h"
#include "Smart_Ptr.h"
#include <vector>
#include "Log/Logger.h"
#include <math.h>
#include "../Monster/Monster.h"
#include "../../MapInstance/InstanceMapLoader.h"
#include "../../Map/SceneManager.h"

using namespace std;
using namespace CommBaseOut;

AIStateIdle::AIStateIdle(AIBrain * owner):AIState(owner)
{

}

AIStateIdle::~AIStateIdle()
{

}

void AIStateIdle::Update(DWORD dwDeltaTick)
{
}

void AIStateIdle::BeginState()
{
	//设置目标为空
	Smart_Ptr<CreatureObj> player;
	GetOwner()->SetTarget(player);

	GetBrain()->ClearPathAndTime();
	GetBrain()->ClearEnemyOnly();
	GetOwner()->BeginIdle();
}

void AIStateIdle::EndState()
{

}

void AIStateIdle::SetTarget(Smart_Ptr<CreatureObj> & player)
{
}

ActMonsterIdle::ActMonsterIdle(AIBrain * owner):AIStateIdle(owner)
{

}

ActMonsterIdle::~ActMonsterIdle()
{

}

void ActMonsterIdle::Update(DWORD dwDeltaTick)
{
	//主动攻击怪，将敌人添加到仇恨列表
	GetBrain()->GetEnemyList();


	Smart_Ptr<CreatureObj> player = GetBrain()->GetMinLength();
	if((bool)player)
	{
		SetTarget(player);

		return;
	}


	//不是稻草人这种怪，就开始调用AI
	bool noNeedUpdate = false;
	if(GET_MAP_TYPE(GetOwner()->GetMapID()) > eCopyMap)
	{
		InstanceType iType = (InstanceType)InstanceLoader::GetInstance()->GetEntryType(GET_MAP_ID(GetOwner()->GetMapID()));
		if(iType != eProtectKing && iType != eMoneyInstance)
			noNeedUpdate = true;
	}

	if(!noNeedUpdate)
	{
		GetBrain()->UpdatePath(dwDeltaTick);
	}

}

void ActMonsterIdle::BeginState()
{
	//设置目标为空
	Smart_Ptr<CreatureObj> player;
	GetOwner()->SetTarget(player);

	GetBrain()->ClearPathAndTime();
	GetBrain()->ClearEnemyOnly();
	GetOwner()->BeginIdle();
}

void ActMonsterIdle::EndState()
{

}

void ActMonsterIdle::SetTarget(Smart_Ptr<CreatureObj> & player)
{
	GetOwner()->SetTarget(player);
	GetBrain()->ChangeState(eAIStateType_Attack);
}

GuardMonsterIdle::GuardMonsterIdle(AIBrain * owner):AIStateIdle(owner)
{
}

GuardMonsterIdle::~GuardMonsterIdle()
{
}

void GuardMonsterIdle::Update(DWORD dwDeltaTick)
{
	Smart_Ptr<CreatureObj> player = NULL;

	//主动攻击怪，将敌人添加到仇恨列表
	GetBrain()->GetEnemyList();

	//发现红名玩家直接攻击
	player = GetBrain()->GetRedNamePlayer();
	if(!(bool)player)
	{
		int curHp = GetOwner()->getAttManage()->getValue<int>(eCharHP);
		int maxHp = GetOwner()->getAttManage()->getValue<int>(eCharHPMax);
		//卫士被非红名玩家打到一半血才开始反击
		if(curHp * 2 <= maxHp)
		{
			//攻击仇恨值最大的玩家
			player = GetBrain()->GetMaxEnemy();
		}
	}

	if((bool)player)
	{
		SetTarget(player);
		return;
	}

	GetBrain()->UpdatePath(dwDeltaTick);
}

void GuardMonsterIdle::BeginState()
{
	//设置目标为空
	Smart_Ptr<CreatureObj> player;
	GetOwner()->SetTarget(player);

	GetBrain()->ClearPathAndTime();
	GetBrain()->ClearEnemyOnly();
	GetOwner()->BeginIdle();
}

void GuardMonsterIdle::EndState()
{

}

void GuardMonsterIdle::SetTarget(Smart_Ptr<CreatureObj> & player)
{
		GetOwner()->SetTarget(player);
		GetBrain()->ChangeState(eAIStateType_Attack);
}

TowerMonsterIdle::TowerMonsterIdle(AIBrain * owner):AIStateIdle(owner)
{

}

TowerMonsterIdle::~TowerMonsterIdle()
{

}

void TowerMonsterIdle::Update(DWORD dwDeltaTick)
{
	//主动攻击怪，将敌人添加到仇恨列表
	GetBrain()->GetEnemyList();

	Smart_Ptr<CreatureObj> player = GetBrain()->GetMinLength();
	if((bool)player)
	{
		SetTarget(player);
		return;
	}
}

void TowerMonsterIdle::BeginState()
{
	//设置目标为空
	Smart_Ptr<CreatureObj> player;
	GetOwner()->SetTarget(player);

	GetBrain()->ClearPathAndTime();
	GetBrain()->ClearEnemyOnly();
	GetOwner()->BeginIdle();
}

void TowerMonsterIdle::EndState()
{

}

void TowerMonsterIdle::SetTarget(Smart_Ptr<CreatureObj> & player)
{
		GetOwner()->SetTarget(player);
		GetBrain()->ChangeState(eAIStateType_Attack);
}

EscortMonsterIdle::EscortMonsterIdle(AIBrain * owner):AIStateIdle(owner)
{

}

EscortMonsterIdle::~EscortMonsterIdle()
{

}

void EscortMonsterIdle::Update(DWORD dwDeltaTick)
{
	GetBrain()->UpdatePath(dwDeltaTick);
}

void EscortMonsterIdle::BeginState()
{
	//设置目标为空
	Smart_Ptr<CreatureObj> player;
	GetOwner()->SetTarget(player);

	GetBrain()->ClearPathAndTime();
	GetBrain()->ClearEnemyOnly();
	GetOwner()->BeginIdle();
}

void EscortMonsterIdle::EndState()
{

}

BossMonsterIdle::BossMonsterIdle(AIBrain * owner):AIStateIdle(owner),m_hpTick(0)
{

}

BossMonsterIdle::~BossMonsterIdle()
{

}

void BossMonsterIdle::Update(DWORD dwDeltaTick)
{
	//主动攻击怪，将敌人添加到仇恨列表
	GetBrain()->GetEnemyList();

	Smart_Ptr<CreatureObj> player = GetBrain()->GetMinLength();
	if((bool)player)
	{
		SetTarget(player);
		return;
	}

	//不是稻草人这种怪，就开始调用AI
	bool noNeedUpdate = false;
	if(GET_MAP_TYPE(GetOwner()->GetMapID()) > eCopyMap)
	{
		InstanceType iType = (InstanceType)InstanceLoader::GetInstance()->GetEntryType(GET_MAP_ID(GetOwner()->GetMapID()));
		if(iType != eProtectKing && iType != eMoneyInstance)
			noNeedUpdate = true;
	}

	if(!noNeedUpdate)
	{
		GetBrain()->UpdatePath(dwDeltaTick);
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

void BossMonsterIdle::BeginState()
{
	//设置目标为空
	Smart_Ptr<CreatureObj> player;
	GetOwner()->SetTarget(player);

	GetBrain()->ClearPathAndTime();
	GetBrain()->ClearEnemyOnly();
	GetOwner()->BeginIdle();
}

void BossMonsterIdle::EndState()
{

}

void BossMonsterIdle::SetTarget(Smart_Ptr<CreatureObj> & player)
{
	GetOwner()->SetTarget(player);
	GetBrain()->ChangeState(eAIStateType_Attack);
}

