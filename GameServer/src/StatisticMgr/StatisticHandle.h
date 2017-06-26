/*
 * StatisticHandle.h
 *
 *  Created on: 2016年5月3日
 *      Author: root
 */

#ifndef STATISTICHANDLE_H_
#define STATISTICHANDLE_H_

#include "StatisticMgr.h"
#include "StatisticBase/BehaviorHandle/BehaviorHandle.h"
#include "StatisticBase/GlobalHandle/GlobalHandle.h"
#include "../Attribute/AttributeCreature.h"

#define BEHAVIOR_MONI(PLAYER, MAIN_T, MINOR_T, OBJ)\
		BehaviorMonitor OBJ(PLAYER, MAIN_T, MINOR_T)

class BehaviorMonitor
{
public:
	BehaviorMonitor(const Smart_Ptr<Player>& player, const int& mainT, const int& minorT) : m_owner(player), m_mainT(mainT), m_minorT(minorT)
	{
		if (bool(m_owner) && eAccountType_User == m_owner->GetAccountType())
		{
			BEHAVIOR_BEGIN(m_owner->GetID(), m_mainT, m_minorT, StatisticMgr::GetInstance(),
					m_owner->getAttManage()->getValue<int>(eCharGolden), m_owner->getAttManage()->getValue<int>(eCharMoney),
					m_owner->getAttManage()->getValue<int>(eCharForce), m_owner->getAttManage()->getValue<int>(eCharBindGolden));
		}
	}
	~BehaviorMonitor()
	{
		End();
	}
	void End(void)
	{
		if (bool(m_owner) && eAccountType_User == m_owner->GetAccountType())
		{
			BEHAVIOR_END(m_owner->GetID(), m_mainT, m_minorT, StatisticMgr::GetInstance(),
					m_owner->getAttManage()->getValue<int>(eCharGolden), m_owner->getAttManage()->getValue<int>(eCharMoney),
					m_owner->getAttManage()->getValue<int>(eCharForce), m_owner->getAttManage()->getValue<int>(eCharBindGolden));
		}
	}
	const Smart_Ptr<Player> m_owner;
	const int m_mainT;
	const int m_minorT;
};


#define BEHAVIOR_HANDLE_DO(__OWNER, H)  \
	if (bool(__OWNER) && eAccountType_User == __OWNER->GetAccountType())  \
	{  \
		BEHAVIOR_DO(__OWNER->GetID(), H, StatisticMgr::GetInstance());  \
	}

#define GLOBAL_STATISTIC_GM(__OWNER, MAIN_T, MINOR_T, D)  \
	if (bool(__OWNER) && eAccountType_User ==__OWNER->GetAccountType())  \
	GLOBAL_STATISTIC(MAIN_T, MINOR_T, D, StatisticMgr::GetInstance());


#endif /* STATISTICHANDLE_H_ */
