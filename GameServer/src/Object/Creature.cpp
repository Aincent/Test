/*
 * Creature.cpp
 *
 *  Created on: 2013��10��9��
 *      Author: helh
 */
#include "Creature.h"
#include "OtherDefine.h"
#include "../Map/SceneManager.h"
#include "FileLoader/BattleInfoLoader.h"
#include "../Skill/SkillLoader/SkillLoader.h"
#include "SvrConfig.h"
#include "MessageCommonRet.h"
#include "../Map/GameZone.h"
#include "../Map/Sector.h"
#include "../Attribute/AttributeCreature.h"
#include "../ServerEventDefine.h"
#include "../Map/DuplicateMgr.h"

CreatureObj::CreatureObj(eObjType type):Object(type),m_ID(-1),m_pSector(NULL),m_pGameZone(NULL)
{

}

CreatureObj::~CreatureObj()
{

}

void CreatureObj::Release()
{
	Object::Release();

	m_AttManage->Release();

	m_ID = -1;
	m_pSector = NULL;
	m_pGameZone = NULL;
	CreatureState		state;
	m_CreatureState = state;
}

bool CreatureObj::IsHitObject(Smart_Ptr<CreatureObj>& targets)
{
	float tempResultRate = BattleInfoLoader::GetInstance()->GetHitRate(getAttManage()->getValue<int>(eCharHitRate),
			targets->getAttManage()->getValue<int>(eCharAvoidRate));

	tempResultRate   -= (float)targets->getAttManage()->getValue<int>(ePercentDodgeRate) / 10000;

	float tempRate = (rand() % 10000 + 1) / (float)10000;

	if(tempResultRate >= tempRate)
	{
		return true;
	}

	return false;
}

bool CreatureObj::IsAttackCrack(Smart_Ptr<CreatureObj>& target)
{
	//必暴击状态
	if(target->IsInCreatureState(ERS_MustBeCrit))
	{
		return true;
	}

	float tempResultRate = BattleInfoLoader::GetInstance()->GetCrackRate(getAttManage()->getValue<int>(eCrackDownRate),
			target->getAttManage()->getValue<int>(eAvoidCrackRate));

	tempResultRate += (float)getAttManage()->getValue<int>(ePercentCritRate) / 10000;

	float tempRate = (rand() % 10000 + 1) / (float)10000;

	if(tempResultRate >= tempRate)
	{
		return true;
	}

	return false;
}

void CreatureObj::Dead(Smart_Ptr<CreatureObj> &obj)
{

}

bool CreatureObj::IsDead()
{
	return false;
}

bool CreatureObj::IsInMap()
{
	if(m_pSector && m_pGameZone)
		return true;
	return false;
}

int CreatureObj::IsAttacked(Smart_Ptr<CreatureObj> &obj, int type)
{
	if(obj.Get()==NULL || obj->IsDead() || !obj->IsInMap())
	{
		return eBattleTargetNull;
	}

	if(GetType() == eMonster && obj->GetType() == ePet)
	{
		return eNoAttack;
	}

	switch(type)
	{
	case eEnemyTarget:
		{
			if(GetType() == eMonster)
			{
					Monster* monster = dynamic_cast<Monster*>(this);
					if(monster != NULL && monster->GetQuality() == eCompeteMonster)
					{
						if(obj->GetKey() == GetKey())
						{
							return eNoAttack;
						}

						//通过场景地图判断攻击
						if(GetGameZone() != NULL && eCopyMap <= GetGameZone()->GetInstancedType())
						{
							Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(GetMapID());
							if(instance.Get() != NULL)
							{
								InstanceMapEntry* instanceInfo = instance->GetInstanceScript();
								if(instanceInfo != NULL && instanceInfo->m_eType == eCompeteInstance)
								{
									return 0;
								}
								else if(getAttManage()->getValue<DWORD>(eCharCountry) == obj->getAttManage()->getValue<DWORD>(eCharCountry))
								{
									return eCantAttackSameCountry;
								}
							}
						}
					}
			}

			//怪物的阵营和玩家的帮派id一致也不能攻击
			if(GetType() == eMonster && obj->GetType() == ePlayer)
			{
				if(getAttManage()->getValue<DWORD>(eCharCountry) == obj->getAttManage()->getValue<DWORD>(eCharFaction))
				{
					return eNoAttack;
				}
			}

			if(getAttManage()->getValue<DWORD>(eCharCountry) == obj->getAttManage()->getValue<DWORD>(eCharCountry))
			{
				return eCantAttackSameCountry;
			}

			//目标为无敌状态
			if(obj->IsInCreatureState(ERS_Invincible))
			{
				return eStateNotAttack;
			}

			break;
		}
	case eFriendTarget:
		{
			break;
		}
	case eMyself:
		{
			if(obj->GetKey() != GetKey())
			{
				return eNoAttack;
			}

			break;
		}
	case eNoPlayer:
		{
			if(obj->GetType() == ePlayer || obj->GetKey() == GetKey())
			{
				return eNoAttack;
			}

			if(getAttManage()->getValue<DWORD>(eCharCountry) == obj->getAttManage()->getValue<DWORD>(eCharCountry))
			{
				return eCantAttackSameCountry;
			}

			break;
		}
	}
	return 0;
}

int CreatureObj::CurHPChange(int hp, bool flag)
{
	int tempHp = getAttManage()->getValue<int>(eCharHP);
	if(hp <= 0)
		return tempHp;

	if(flag)
	{
		int tempMaxHp = getAttManage()->getValue<int>(eCharHPMax);
		if(tempHp + hp > tempMaxHp)
		{
			tempHp = tempMaxHp;
		}
		else
		{
			tempHp = tempHp + hp;
		}
	}
	else
	{
		tempHp = tempHp - hp;

		if(tempHp < 0)
			tempHp = 0;
	}

	getAttManage()->setValue(eCharHP,tempHp);

	return tempHp;
}

int CreatureObj::ChangeHPRate(int hp, bool flag)
{
	int tempHp = getAttManage()->getValue<int>(eCharHP);
	if(hp <= 0)
		return tempHp;

	int tempMaxHp = getAttManage()->getValue<int>(eCharHPMax);
	if(flag)
	{
		if(tempHp + (tempMaxHp * hp / 100) > tempMaxHp)
		{
			tempHp = tempMaxHp;
		}
		else
		{
			tempHp = tempHp + tempMaxHp * hp / 100;
		}
	}
	else
	{
		tempHp = tempHp - tempMaxHp * hp / 100;

		if(tempHp < 0)
			tempHp = 0;
	}

	getAttManage()->setValue(eCharHP,tempHp);

	return tempHp;
}

int64 CreatureObj::GetTypeValue(int type)
{
	int64 value = 0;

	switch(type)
	{
	case eCharLv :
		{
			value = getAttManage()->getValue<int>(eCharLv);
			break;
		}
	case eCharXpos :
		{
			value = GetXNewPos();
			break;
		}
	case eCharYpos :
		{
			value = getAttManage()->getValue<int>(eCharYpos);
			break;
		}
	case eCharMapid :
		{
			value = GET_MAP_ID(GetMapID());
			break;
		}
	case eCharCountry :
		{
			value = getAttManage()->getValue<DWORD>(eCharCountry);
			break;
		}
	case eCharHP :
		{
			value = getAttManage()->getValue<int>(eCharHP);;
			break;
		}
	case eCharHPMax :
		{
			value = getAttManage()->getValue<int>(eCharHPMax);
			break;
		}
	case eCharMP :
		{
			value = getAttManage()->getValue<int>(eCharMP);
			break;
		}
	case eCharMPMax :
		{
			value = getAttManage()->getValue<int>(eCharMPMax);
			break;
		}
	case eCharPhyAttackMax :
		{
			value = getAttManage()->getValue<int>(eCharPhyAttackMax);
			break;
		}
	case eCharPhyDefence :
		{
			value = getAttManage()->getValue<int>(eCharPhyDefence);
			break;
		}
	case eCharHitRate :
		{
			value = getAttManage()->getValue<int>(eCharHitRate);
			break;
		}
	case eCharAvoidRate :
		{
			value = getAttManage()->getValue<int>(eCharAvoidRate);
			break;
		}
	case eCharMoveSpeed :
		{
			value = getAttManage()->getValue<int>(eCharMoveSpeed);
			break;
		}
	case eCrackDownRate :
		{
			value = getAttManage()->getValue<int>(eCrackDownRate);
			break;
		}
	case eAvoidCrackRate :
		{
			value = getAttManage()->getValue<int>(eAvoidCrackRate);
			break;
		}
	}

	return value;
}

int CreatureObj::ChangeMoveSpeed(const int &value, bool isAdd,eAttValueType tempValueType)
{
	int mSpeed = getAttManage()->getBasedValue<int>(eCharMoveSpeed,tempValueType);
	if (value <= 0)
		return mSpeed;

	if (isAdd)
	{
		mSpeed += value;
	}
	else
	{
		mSpeed -= value;

		if (mSpeed <= 0)
			mSpeed = 1;
	}

	getAttManage()->setValue(eCharMoveSpeed, mSpeed,tempValueType);

	return mSpeed;
}

int CreatureObj::ChangePhyAttackMax(int value, bool isAdd,eAttValueType tempValueType)
{
	int pAttack = getAttManage()->getBasedValue<int>(eCharPhyAttackMax,tempValueType);

	if(isAdd)
	{
		pAttack += value;
	}
	else
	{
		pAttack -= value;
	}

	if(pAttack <= 0)
		pAttack = 1;

	getAttManage()->setValue(eCharPhyAttackMax, pAttack,tempValueType);

	return pAttack;
}

int CreatureObj::ChangePDefence(int value, bool isAdd,eAttValueType tempValueType)
{
	int pDefence = getAttManage()->getBasedValue<int>(eCharPhyDefence,tempValueType);

	if(isAdd)
	{
		pDefence += value;
	}
	else
	{
		pDefence -= value;
	}

	if(pDefence <= 0)
		pDefence = 0;

	getAttManage()->setValue(eCharPhyDefence, pDefence,tempValueType);

	return pDefence;
}

int CreatureObj::ChangeHRate(int value, bool isAdd,eAttValueType tempValueType)
{
	int hRate = getAttManage()->getBasedValue<int>(eCharHitRate,tempValueType);
	if (value <= 0)
		return hRate;

	if (isAdd)
	{
		hRate += value;
	}
	else
	{
		hRate -= value;
		if (hRate < 0)
			hRate = 0;
	}

	getAttManage()->setValue(eCharHitRate, hRate,tempValueType);

	return hRate;
}

short CreatureObj::ChangePercentHurt(int value, bool isAdd)
{
	short tempPercentHurt = getAttManage()->getValue<short>(ePercentHurt);
	if(value <= 0)
		return tempPercentHurt;

	if(isAdd)
	{
		tempPercentHurt += value;
	}
	else
	{
		tempPercentHurt -= value;
		if(tempPercentHurt < 0)
			tempPercentHurt = 0;
	}

	getAttManage()->setValue(ePercentHurt,tempPercentHurt);

	return tempPercentHurt;
}

int CreatureObj::ChangeARate(int value, bool isAdd,eAttValueType tempValueType)
{
	int aRate = getAttManage()->getBasedValue<int>(eCharAvoidRate,tempValueType);
	if (value <= 0)
		return aRate;

	if (isAdd)
	{
		aRate += value;
	}
	else
	{
		aRate -= value;
		if (aRate < 0)
			aRate = 0;
	}

	getAttManage()->setValue(eCharAvoidRate, aRate,tempValueType);

	return aRate;
}

int CreatureObj::ChangeCRate(int value, bool isAdd,eAttValueType tempValueType)
{
	int cRate = getAttManage()->getBasedValue<int>(eCrackDownRate,tempValueType);
	if (value <= 0)
		return cRate;

	if (isAdd)
	{
		cRate += value;
	}
	else
	{
		cRate -= value;
		if (cRate < 0)
			cRate = 0;
	}

	getAttManage()->setValue(eCrackDownRate, cRate,tempValueType);

	return cRate;
}

void CreatureObj::SetHPMax(int max)
{
	getAttManage()->setValue(eCharHPMax,max);
}

int CreatureObj::ChangeHPMax(int value, bool isAdd,eAttValueType tempValueType)
{
	int hp = getAttManage()->getBasedValue<int>(eCharHPMax,tempValueType);

	if(isAdd)
	{
		hp += value;
	}
	else
	{
		hp -= value;
	}

	if (hp <= 0)
		hp = 1;

	if (hp < getAttManage()->getValue<int>(eCharHP))
		getAttManage()->setValue(eCharHP, hp);

	getAttManage()->setValue(eCharHPMax,hp,tempValueType);

	return hp;
}

void CreatureObj::SetCurHP(int hp)
{
	int tempHp = 0;
	if(hp < 0)
		tempHp = 0;
	else if(hp > getAttManage()->getValue<int>(eCharHPMax))
		tempHp = getAttManage()->getValue<int>(eCharHPMax);
	else
		tempHp = hp;

	getAttManage()->setValue(eCharHP,tempHp);

	return ;
}

void CreatureObj::SetMPMax(int max)
{
	getAttManage()->setValue(eCharMPMax,max);
}

int CreatureObj::ChangeMPMax(int value, bool isAdd,eAttValueType tempValueType)
{
	int mp = getAttManage()->getBasedValue<int>(eCharMPMax,tempValueType);
	if (value <= 0)
		return mp;

	if (isAdd)
	{
		mp += value;
	}
	else
	{
		mp -= value;
		if (mp <= 0)
			mp = 1;

		if (mp < getAttManage()->getValue<int>(eCharMP))
			CreatureObj::SetCurMP(mp);
	}

	getAttManage()->setValue(eCharMPMax,mp,tempValueType);

	return mp;
}

void CreatureObj::SetCurMP(int mp)
{
	int tempMp = 0;
	if(mp < 0)
		tempMp = 0;
	else if(mp > getAttManage()->getValue<int>(eCharMPMax))
		tempMp = getAttManage()->getValue<int>(eCharMPMax);
	else
		tempMp = mp;

	getAttManage()->setValue(eCharMP,tempMp);

	return ;
}

int CreatureObj::ChangeCurMP(int value, bool isAdd)
{
	int tempMp = getAttManage()->getValue<int>(eCharMP);
	if(value <= 0)
		return tempMp;

	if(isAdd)
	{
		if(tempMp + value > getAttManage()->getValue<int>(eCharMPMax))
		{
			tempMp = getAttManage()->getValue<int>(eCharMPMax);
		}
		else
		{
			tempMp = tempMp + value;
		}
	}
	else
	{
		tempMp = tempMp - value;

		if(tempMp < 0)
			tempMp = 0;
	}

	getAttManage()->setValue(eCharMP,tempMp);

	return tempMp;
}

int CreatureObj::ChangeBaserte(int value, bool isAdd,eAttValueType tempValueType)
{
	int lucky = getAttManage()->getBasedValue<int>(eAvoidCrackRate,tempValueType);
	if (value <= 0)
		return lucky;

	if (isAdd)
	{
		lucky += value;
	}
	else
	{
		lucky -= value;
		if (lucky < 0)
			lucky = 0;
	}

	getAttManage()->setValue(eAvoidCrackRate, lucky,tempValueType);

	return lucky;
}

int CreatureObj::GetXNewPos()
{
	if(getAttManage()->getValue<int>(eCharXpos) == -1)
	{
		return GetXPos();
	}

	return getAttManage()->getValue<int>(eCharXpos);
}

void CreatureObj::SetXNewPos(int xpos)
{
	if(getAttManage()->getValue<int>(eCharXpos) != -1)
	{
		SetXPos(getAttManage()->getValue<int>(eCharXpos));
	}

	getAttManage()->setValue(eCharXpos,xpos);
}

void CreatureObj::SetXNewPos()
{
	getAttManage()->setValue(eCharXpos,-1);
}

int CreatureObj::GetYNewPos()
{
	if(getAttManage()->getValue<int>(eCharYpos) == -1)
	{
		return GetYPos();
	}

	return getAttManage()->getValue<int>(eCharYpos);
}

void CreatureObj::SetYNewPos(int ypos)
{
	if(getAttManage()->getValue<int>(eCharYpos) != -1)
	{
		SetYPos(getAttManage()->getValue<int>(eCharYpos));
	}

	getAttManage()->setValue(eCharYpos,ypos);
}

void CreatureObj::SetYNewPos()
{
	getAttManage()->setValue(eCharYpos,-1);
}

void CreatureObj::SetCreatureState(ECreatureState eState, bool bSendMsg)
{
	m_CreatureState.SetState(eState);

	ObjectEnterStateArgs tempArgs(*this,eState);
	FireEvent(OBJECT_ENTER_STATE,tempArgs);

	if( bSendMsg )
	{
		//可定义一个广播消息，广播当前状态
	}
}

void CreatureObj::UnsetCreatureState(ECreatureState eState, bool bSendMsg)
{
	if( false == IsInCreatureState(eState) ) return;

	m_CreatureState.UnsetState(eState);

	if( bSendMsg )
	{
		//可定义一个广播消息，广播当前状态
	}
}

void CreatureObj::SetSector(Sector *sector)
{
	if(!sector)
	{
		cout<<endl<<"CreatureObj Set sector is NULL,CreatureObj is %lld"<<GetID();
		return;
	}

//	if(!m_pSector)
	m_pSector = sector;
}

void CreatureObj::SetGameZone(CGameZone *gameZone)
{
	if(!gameZone)
	{
		cout<<endl<<"CreatureObj Set gameZone is NULL,CreatureObj is %lld"<<GetID();
		return;
	}

//	if(!m_pGameZone)
	m_pGameZone = gameZone;
}

void CreatureObj::ResetSector(void)
{
	if(m_pSector)
		m_pSector = NULL;
}

void CreatureObj::ResetGameZone(void)
{
	if(m_pGameZone)
		m_pGameZone = NULL;
}

Smart_Ptr<AttributeCreature> CreatureObj::getAttManage()
{
	return m_AttManage;
}





