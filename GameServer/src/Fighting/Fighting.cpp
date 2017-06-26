/*
 * Fighting.cpp
 *
 *  Created on: 10/07/2015
 *      Author: root
 */

#include "Fighting.h"
#include "../Object/Player/Player.h"
#include "../Attribute/AttributeCreature.h"
#include "../ServerEventDefine.h"
#include "../EventSet/EventDefine.h"

Fighting::Fighting(Player* player) : m_Owner(player)
{
	ReInit();
}

Fighting::~Fighting()
{
	Release();
}

void Fighting::ReInit()
{
	m_AllFighting.clear();
}

void Fighting::Release()
{
	m_AllFighting.clear();
}

int Fighting::setFighting(FightingType type,int value)
{
	if(type <= eFightingNull || type >= eFightingMax)
		return 0;

	int tempPower = m_Owner->getAttManage()->getValue<int>(eFightPower);

	int tempValue = m_AllFighting[type];

	m_Owner->getAttManage()->addValue(eFightPower,-tempValue);

	m_AllFighting[type] = value;

	if(m_AllFighting[type] < 0)
		m_AllFighting[type] = 0;

	m_Owner->getAttManage()->addValue(eFightPower,value);

	if(value > 0)
	{
		if (m_Owner->GetMyself())
		{
			PowerParmaEventArgs e(m_Owner->GetMyself(), type);
			m_Owner->FireEvent(PLAYER_FIGHT_POWER_CHANGE,e);
		}
	}

	tempPower = m_Owner->getAttManage()->getValue<int>(eFightPower);

	return value;
}

int Fighting::addFighting(FightingType type,int value)
{
	if(type <= eFightingNull || type >= eFightingMax)
		return 0;

	m_AllFighting[type] += value;

	if(m_AllFighting[type] < 0)
		m_AllFighting[type] = 0;

	m_Owner->getAttManage()->addValue(eFightPower,value);

	return m_AllFighting[type];
}

int Fighting::getFighting(FightingType type)
{
	if(type <= eFightingNull || type >= eFightingMax)
		return 0;

	map<FightingType,int>::iterator tempIt = m_AllFighting.find(type);
	if(tempIt == m_AllFighting.end())
		return 0;

	return tempIt->second;
}

int Fighting::CalculateFighting(const map<ECharAttrType,int64>& attList)
{
	int tempPhyAttack = 0;
	int tempPhyDefence = 0;
	int tempHpMax = 0;
	int tempHit = 0;
	int tempAvoid = 0;
	int tempCrack = 0;
	int tempAvoidCrack = 0;
	map<ECharAttrType,int64>::const_iterator tempIt = attList.begin();
	for(;tempIt != attList.end();++tempIt)
	{
		switch(tempIt->first)
		{
		case eCharPhyAttackMax:
			{
				tempPhyAttack += tempIt->second;
				break;
			}
		case eCharPhyDefence:
			{
				tempPhyDefence += tempIt->second;
				break;
			}
		case eCharHPMax:
			{
				tempHpMax += tempIt->second;
				break;
			}
		case eCharHitRate:
			{
				tempHit += tempIt->second;
				break;
			}
		case eCharAvoidRate:
			{
				tempAvoid += tempIt->second;
				break;
			}
		case eCrackDownRate:
			{
				tempCrack += tempIt->second;
				break;
			}
		case eAvoidCrackRate:
			{
				tempAvoidCrack += tempIt->second;
				break;
			}
		default:
			{
				break;
			}
		}
	}

	int tempFightPower = BattleInfoLoader::GetInstance()->GetAllFight(tempPhyAttack,tempPhyDefence,tempHpMax,tempHit,tempAvoid,tempCrack,tempAvoidCrack);
	return tempFightPower;
}





