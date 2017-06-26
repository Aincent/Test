/*
 * BuffBase.cpp
 *
 *  Created on: 2014年2月12日
 *      Author: helh
 */

#include "BuffBase.h"
#include "../Map/SceneManager.h"
#include "Log/Logger.h"
#include "SkillBase.h"
#include "CharDefine.h"
#include "Network/Message_Facade.h"
#include "ServerMsgDefine.h"
#include "../ServerManager.h"
#include "Network/MessageManager.h"
#include "../MessageBuild.h"
#include "../QuickMessage.h"
#include "BuffFactory.h"
#include "Effect/MoveEffect.h"
#include "../ServerEventDefine.h"
#include "../Meditation/PlayerMeditation.h"
#include "./SkillLoader/SkillLoader.h"

BuffBase::BuffBase(Smart_Ptr<CreatureObj>& owner, int key, int id, DWORD onlyID)
: m_owner(owner),
m_casterKey(key),
m_id(id),
m_curCount(0),
m_lOnlyID(onlyID),
m_bEnd(false),
m_value(0),
m_values(0),
m_ReverseValue(.0f),
m_Effect(NULL)
{
}

BuffBase::~BuffBase()
{
	if(m_Effect != NULL)
	{
		delete m_Effect;
		m_Effect = NULL;
	}
}

void BuffBase::InitInfo(Smart_Ptr<CreatureObj>& owner, int key, int id, DWORD onlyID)
{
	m_owner = owner;
	m_casterKey = key;
	m_id = id;
	m_curCount = 0;
	m_lOnlyID = onlyID;
	m_bEnd = false;
	m_value = 0;
	m_values = 0;
	m_ReverseValue = .0f;
	m_Effect = NULL;
}

void BuffBase::Release()
{
	Smart_Ptr<CreatureObj> owner;
	m_owner = owner;

	if(m_Effect != NULL)
	{
		delete m_Effect;
		m_Effect = NULL;
	}

}

void BuffBase::ResultReverseEffect(int type, vector<AttrChange> &toMyself, vector<AttrChange> &toOther)
{
	switch(type)
	{
	case eNoRes:
		{
			break;
		}
	case eChangeHP:
		{
			break;
		}
	case eScaleChangeHP:
		{
			break;
		}
	case eChangeMP:
		{
			break;
		}
	case eScaleChangeMP:
		{
			break;
		}
	case eChangeHPMax:
		{
			AttrChange att;
			bool isadd = false;

			att.state = eAttackBuff;
			if(m_value > 0)
			{
				if(m_value + m_owner->getAttManage()->getValue<int>(eCharHP) > m_owner->getAttManage()->getValue<int>(eCharHPMax))
				{
					AttrChange change;

					change.state = eAttackBuff;
					change.type = eCharHP;
					change.value = (m_owner->getAttManage()->getValue<int>(eCharHPMax) - m_value);

					toOther.push_back(change);
				}
			}
			else
			{
				m_value = 0 - m_value;
				isadd = true;
			}

			m_owner->ChangeHPMax(m_value, isadd);
			if(m_value != 0)
			{
				att.type = eCharHPMax;
				att.value = m_owner->GetTypeValue(eCharHPMax);
				toOther.push_back(att);
			}

			break;
		}
	case eChangeMPMax:
		{
			AttrChange att;
			bool isadd = false;

			att.state = eAttackBuff;
			if(m_value > 0)
			{
				if(m_value + m_owner->getAttManage()->getValue<int>(eCharMP) > m_owner->getAttManage()->getValue<int>(eCharMPMax))
				{
					AttrChange change;

					change.state = eAttackBuff;
					change.type = eCharMP;
					change.value = (m_owner->getAttManage()->getValue<int>(eCharMPMax) - m_value);

					toOther.push_back(change);
				}
			}
			else
			{
				m_value = 0 - m_value;
				isadd = true;
			}

			m_owner->ChangeMPMax(m_value, isadd);

			if(m_value != 0)
			{
				att.type = eCharMPMax;
				att.value = m_owner->GetTypeValue(eCharMPMax);

				toOther.push_back(att);
			}

			break;
		}
	case eChangeHitRate:
		{
			bool isAdd = false;

			if(m_value < 0)
			{
				isAdd = true;
				m_value = 0 - m_value;
			}

			m_owner->ChangeHRate(m_value, isAdd);

			AttrChange change;

			change.state = eAttackBuff;
			change.type = eCharHitRate;
			change.value = m_owner->getAttManage()->getValue<int>(eCharHitRate);

			toMyself.push_back(change);

			break;
		}
	case eChangeAvdRate:
		{
			bool isAdd = false;

			if(m_value < 0)
			{
				isAdd = true;
				m_value = 0 - m_value;
			}

			m_owner->ChangeARate(m_value, isAdd);

			AttrChange change;

			change.state = eAttackBuff;
			change.type = eCharAvoidRate;
			change.value = m_owner->getAttManage()->getValue<int>(eCharAvoidRate);

			toMyself.push_back(change);

			break;
		}
	case eChangePhyAttack:
		{
			bool isAdd = false;
			if(m_value < 0)
			{
				isAdd = true;
				m_value = 0 - m_value;
			}

			m_owner->ChangePhyAttackMax(m_value, isAdd);

			AttrChange change;

			change.state = eAttackBuff;
			change.type = eCharPhyAttackMax;
			change.value = m_owner->getAttManage()->getValue<int>(eCharPhyAttackMax);

			toMyself.push_back(change);

			break;
		}
	case eChangePhyDefence:
		{
			bool isAdd = false;
			if(m_value < 0)
			{
				isAdd = true;
				m_value = 0 - m_value;
			}

			m_owner->ChangePDefence(m_value, isAdd);

			AttrChange change;

			change.state = eAttackBuff;
			change.type = eCharPhyDefence;
			change.value = m_owner->getAttManage()->getValue<int>(eCharPhyDefence);

			toMyself.push_back(change);

			break;
		}
	case eChangeMagAttack:
		{
			break;
		}
	case eChangeCrack:
		{
			bool isAdd = false;

			if(m_value < 0)
			{
				isAdd = true;
				m_value = 0 - m_value;
			}

			m_owner->ChangeCRate(m_value, isAdd);

			m_owner->getAttManage()->addPercentValue(eCrackDownRate,-m_ReverseValue);

			AttrChange change;

			change.state = eAttackBuff;
			change.type = eCrackDownRate;
			change.value = m_owner->getAttManage()->getValue<int>(eCrackDownRate);

			toMyself.push_back(change);

			break;
		}
	case eChangeAvoidCrack:
		{
			bool isAdd = false;

			if(m_value < 0)
			{
				isAdd = true;
				m_value = 0 - m_value;
			}

			m_owner->ChangeBaserte(m_value, isAdd);

			AttrChange change;

			change.state = eAttackBuff;
			change.type = eAvoidCrackRate;
			change.value = m_owner->getAttManage()->getValue<int>(eAvoidCrackRate);

			toMyself.push_back(change);

			break;
		}
	case eChangeDoubleExp:
		{
			if(m_owner->GetType() == ePlayer)
			{
				Smart_Ptr<Player> player = m_owner;
				player->getAttManage()->addValue(eKillMonsterExpRate,-m_value);
				player->SetDataFlag(eSkillInfo);
			}
			break;
		}
	case eAttack:
	{
		if(m_owner->GetType() == ePlayer)
		{
			Smart_Ptr<Player> player = m_owner;

			m_owner->ChangePhyAttackMax(m_values,false);
			AttrChange changes;
			changes.type = eCharPhyAttackMax;
			changes.state = eAttackBuff;
			changes.value = m_owner->getAttManage()->getValue<int>(eCharPhyAttackMax);
			toMyself.push_back(changes);

			player->SetDataFlag(eSkillInfo);
		}
		break;
	}
	case eChangeMvSpeed:
		{
			AttrChange att;
			bool isAdd = false;

			att.state = eAttackBuff;
			if(m_value < 0)
			{
				isAdd = true;
				m_value = 0 - m_value;
			}

			m_owner->ChangeMoveSpeed(m_value, isAdd);

			m_owner->getAttManage()->addPercentValue(eCharMoveSpeed,-m_ReverseValue);

			if(m_value != 0)
			{
				att.type = eCharMoveSpeed;
				att.value = m_owner->GetTypeValue(eCharMoveSpeed);

				toOther.push_back(att);
			}

			break;
		}
	case eStiff:
		{
			AttrChange att;

			m_owner->UnsetCreatureState(ERS_Still, false);
			att.state = eAttackStill;
			att.type = eStateRemove;
			att.value = 0;
			toOther.push_back(att);

			break;
		}
	case eIce:
		{
			AttrChange att;

			m_owner->UnsetCreatureState(ERS_Ice, false);
			att.state = eAttackIce;
			att.type = eStateRemove;
			att.value = 0;
			toOther.push_back(att);
			if(m_owner->GetType() == ePlayer)
			{
				Smart_Ptr<Player> player = m_owner;
			}
			break;
		}
	case eStun:
		{
			AttrChange att;

			m_owner->UnsetCreatureState(ERS_Stun, false);
			att.state = eAttackStun;
			att.type = eStateRemove;
			att.value = 0;
			toOther.push_back(att);

			break;
		}
	case eImmunityStiff: //免疫僵直
		{
			m_owner->UnsetCreatureState(ERS_ImmunityStill, false);

			break;
		}
	case eImmunityIce:   //免疫冰冻
		{
			m_owner->UnsetCreatureState(ERS_ImmunityIce, false);

			break;
		}
	case eImmunityStun:  //免疫晕眩
		{
			m_owner->UnsetCreatureState(ERS_ImmunityStun, false);

			break;
		}
	case eSkillPercentHurt:
		{
			m_owner->getAttManage()->addValue(ePercentHurt,-m_value);

			break;
		}
	case eInvincibleEffect:
		{
			if(m_owner->GetType() == ePlayer)
			{
				m_owner->UnsetCreatureState(ERS_Invincible, false);
			}
			break;
		}
	case eImmuneHurtShield:		//免疫伤害护盾
		{
			m_owner->UnsetCreatureState(ERS_ImmuneHurtShield, false);

			break;
		}
	case eVampire:				//吸血
		{
			m_owner->getAttManage()->addValue(ePercentAttVampire,-m_value);

			break;
		}
	//百分比护盾
	case eAttPersentShield:
		{
			m_owner->getAttManage()->addValue(ePercentShield,-m_value);

			break;
		}
	//暴击率
	case eEffectPercentCritRate:
		{
			m_owner->getAttManage()->addValue(ePercentCritRate,-m_value);

			break;
		}
	//闪避率
	case eEffectPercentDodgeRate:
		{
			m_owner->getAttManage()->addValue(ePercentDodgeRate,-m_value);

			break;
		}
	//必暴击状态
	case eEffectMustBeCrit:
		{
			m_owner->UnsetCreatureState(ERS_MustBeCrit, false);

			break;
		}
	case eRepel:
		{
			m_owner->UnsetCreatureState(ERS_AttackBack, false);
			break;
		}
	case eAddState:
		{
			AttrChange att;

			att.state = (EAttackState)m_value;
			att.type = eStateRemove;
			att.value = 0;
			toOther.push_back(att);

			m_owner->UnsetCreatureState(Effect::switchEnumToServer((EAttackState)m_value),false);

			break;
		}	
	case eAlterAtt:
		{
			m_owner->getAttManage()->addValue<float>((ECharAttrType)m_value,-m_ReverseValue);	
			break;
		}
	default:
		{
			LOG_ERROR(FILEINFO, "add buff[%d] and buf type is error", m_id);

			return;
		}
	}
}

void BuffBase::ResultEffect(Smart_Ptr<CreatureObj> target,const BuffInfo* info, vector<AttrChange> &toMyself, vector<AttrChange> &toOther)
{
	int param  = (int)info->m_pParam->m_Param;
	float params = info->params;
	switch(info->useType)
	{
	case eNoRes:
		{
			break;
		}
	case eChangeHP:
		{
			EffectParam tEffectParam;
			tEffectParam.m_Self   = m_owner.Get();
			tEffectParam.m_Target = target.Get();

			param = (int)BattleInfoLoader::GetInstance()->getValueByFormula(*info->m_pParam,tEffectParam);

			AttrChange att;
			bool isAdd = true;

			att.state = eAttackBuff;

			if(param < 0)
			{
				isAdd = false;
				param = 0 - param;
				m_owner->UpdateEnemyValue(0 - param, m_casterKey);
			}
			else
			{
				if(m_owner->getAttManage()->getValue<float>(eInjured) > .0f)
				{
					param = (int)(param * m_owner->getAttManage()->getValue<float>(eInjured));
				}
			}
			
			m_value = param;

			if(m_owner->CurHPChange(param, isAdd) <= 0)
			{
				att.state = eAttackDead;

				AttrChange change;
				change.type = eCharHP;
				change.state = eAttackBuff;
				change.value = 0;
				toOther.push_back(change);
			}

			if(param != 0)
			{
				if(att.state == eAttackDead)
				{
					att.type = eSimpleState;
					att.value = m_casterKey;
				}
				else
				{
					att.type = eCharHP;
					att.value = m_owner->GetTypeValue(att.type);
				}

				toOther.push_back(att);
			}

			break;
		}
	case eScaleChangeHP:
		{
			AttrChange att;
			bool isAdd = true;
			int hp = 0;

			att.state = eAttackBuff;
			if(param < 0)
			{
				isAdd = false;
				param = 0 - param;
			}
			
			float tValue = (float) param / 100;
			if(isAdd)
			{
				if(m_owner->getAttManage()->getValue<float>(eInjured) > .0f)
				{
					tValue *=  m_owner->getAttManage()->getValue<float>(eInjured);
				}
			}

			hp = (int)(m_owner->getAttManage()->getValue<int>(eCharHPMax) * tValue);
			if(hp == 0)
				hp = 1;

			if(!isAdd)
			{
				m_owner->UpdateEnemyValue(hp, m_casterKey);
				m_value = 0 - hp;
			}
			else
			{
				m_value = hp;
			}

			if(m_owner->CurHPChange(hp, isAdd) <= 0)
			{
				att.state = eAttackDead;

				AttrChange change;
				change.type = eCharHP;
				change.state = eAttackBuff;
				change.value = 0;
				toOther.push_back(change);
			}

			if(param != 0)
			{
				if(att.state == eAttackDead)
				{
					att.type = eSimpleState;
					att.value = m_casterKey;
				}
				else
				{
					att.type = eCharHP;
					att.value = m_owner->GetTypeValue(att.type);
				}

				toOther.push_back(att);
			}

			break;
		}
	case eChangeMP:
		{
			AttrChange att;
			bool isadd = true;

			att.state = eAttackBuff;
			m_value = param;
			if(param < 0)
			{
				isadd = false;
				param = 0 - param;
			}

			m_owner->ChangeCurMP(param, isadd);

			if(param != 0)
			{
				att.type = eCharMP;
				att.value = m_owner->GetTypeValue(att.type);

				toOther.push_back(att);
			}

			break;
		}
	case eScaleChangeMP:
		{
			AttrChange att;
			bool isadd = true;
			int mp = 0;

			att.state = eAttackBuff;
			if(param < 0)
			{
				isadd = false;
				param = 0 - param;
			}

			mp = (int)(m_owner->getAttManage()->getValue<int>(eCharMPMax) * param / 100);
			if(mp == 0)
				mp = 1;
			if(!isadd)
			{
				m_value = 0 - mp;
			}
			else
			{
				m_value = mp;
			}

			m_owner->ChangeCurMP(mp, isadd);

			if(param != 0)
			{
				att.type = eCharMP;
				att.value = m_owner->GetTypeValue(att.type);

				toOther.push_back(att);
			}

			break;
		}
	case eChangeHPMax:
		{
			AttrChange att;
			bool isadd = true;

			att.state = eAttackBuff;
			m_value = param;
			if(param < 0)
			{
				isadd = false;
				param = 0 - param;
				if(param + m_owner->getAttManage()->getValue<int>(eCharHP) > m_owner->getAttManage()->getValue<int>(eCharHPMax))
				{
					AttrChange change;

					change.state = eAttackBuff;
					change.type = eCharHP;
					change.value = (m_owner->getAttManage()->getValue<int>(eCharHPMax) - param);

					toOther.push_back(change);
				}
			}

			m_owner->ChangeHPMax(param, isadd);

			if(param != 0)
			{
				att.type = eCharHPMax;
				att.value = m_owner->GetTypeValue(att.type);
				toOther.push_back(att);
			}

			break;
		}
	case eChangeMPMax:
		{
			AttrChange att;
			bool isadd = true;

			att.state = eAttackBuff;
			m_value = param;
			if(param < 0)
			{
				isadd = false;
				param = 0 - param;
				if(param + m_owner->getAttManage()->getValue<int>(eCharMP) > m_owner->getAttManage()->getValue<int>(eCharMPMax))
				{
					AttrChange change;

					change.state = eAttackBuff;
					change.type = eCharMP;
					change.value = (m_owner->getAttManage()->getValue<int>(eCharMPMax) - param);

					toOther.push_back(change);
				}
			}

			m_owner->ChangeMPMax(param, isadd);

			if(param != 0)
			{
				att.type = eCharMPMax;
				att.value = m_owner->GetTypeValue(att.type);
				toOther.push_back(att);
			}

			break;
		}
	case eChangeHitRate:
		{
			bool isAdd = true;

			m_value = param;
			if(param < 0)
			{
				param = (0 - param);
				isAdd = false;
			}

			m_owner->ChangeHRate(param, isAdd);

			AttrChange change;

			change.state = eAttackBuff;
			change.type = eCharHitRate;
			change.value = m_owner->getAttManage()->getValue<int>(eCharHitRate);
			toMyself.push_back(change);

			break;
		}
	case eChangeAvdRate:
		{
			bool isAdd = true;

			m_value = param;
			if(param < 0)
			{
				param = (0 - param);
				isAdd = false;
			}

			m_owner->ChangeARate(param, isAdd);

			AttrChange change;

			change.state = eAttackBuff;
			change.type = eCharAvoidRate;
			change.value = m_owner->getAttManage()->getValue<int>(eCharAvoidRate);
			toMyself.push_back(change);

			break;
		}
	case eChangePhyAttack:
		{
			bool isAdd = true;

			m_value = param;
			if(param < 0)
			{
				param = (0 - param);
				isAdd = false;
			}

			m_owner->ChangePhyAttackMax(param, isAdd);

			AttrChange change;

			change.state = eAttackBuff;
			change.type = eCharPhyAttackMax;
			change.value = m_owner->getAttManage()->getValue<int>(eCharPhyAttackMax);
			toMyself.push_back(change);

			break;
		}
	case eChangePhyDefence:
		{
			bool isAdd = true;

			m_value = param;
			if(param < 0)
			{
				param = (0 - param);
				isAdd = false;
			}

			m_owner->ChangePDefence(param, isAdd);

			AttrChange change;

			change.state = eAttackBuff;
			change.type = eCharPhyDefence;
			change.value = m_owner->getAttManage()->getValue<int>(eCharPhyDefence);
			toMyself.push_back(change);

			break;
		}
	case eChangeMagAttack:
		{
			break;
		}
	case eChangeCrack:
		{
			bool isAdd = true;

			m_value = param;
			if(param < 0)
			{
				param = (0 - param);
				isAdd = false;
			}

			m_owner->ChangeCRate(param, isAdd);

			m_ReverseValue = params / 100;
			m_owner->getAttManage()->addPercentValue(eCrackDownRate,m_ReverseValue);

			AttrChange change;

			change.state = eAttackBuff;
			change.type = eCrackDownRate;
			change.value = m_owner->getAttManage()->getValue<int>(eCrackDownRate);
			toMyself.push_back(change);

			break;
		}
	case eChangeAvoidCrack:
		{
			bool isAdd = true;

			m_value = param;
			if(param < 0)
			{
				param = (0 - param);
				isAdd = false;
			}

			m_owner->ChangeBaserte(param, isAdd);

			AttrChange change;

			change.state = eAttackBuff;
			change.type = eAvoidCrackRate;
			change.value = m_owner->getAttManage()->getValue<int>(eAvoidCrackRate);
			toMyself.push_back(change);

			break;
		}
	case eChangeDoubleExp:
		{
			if(m_owner->GetType() == ePlayer)
			{
				Smart_Ptr<Player> player = m_owner;
				m_value = param;

				player->getAttManage()->addValue(eKillMonsterExpRate,param);
			}
			break;
		}
	case eAttack:
		{
			if(m_owner->GetType() == ePlayer)
			{
				Smart_Ptr<Player> player = m_owner;

				m_values = (int)(m_owner->getAttManage()->getValue<int>(eCharPhyAttackMax) * param / 100);
				m_owner->ChangePhyAttackMax(m_values);

				AttrChange changes;
				changes.type = eCharPhyAttackMax;
				changes.state = eAttackBuff;
				changes.value = m_owner->getAttManage()->getValue<int>(eCharPhyAttackMax);
				toMyself.push_back(changes);
			}
			break;
		}
	case eChangeMvSpeed:
		{
			AttrChange att;
			bool isAdd = true;

			m_value = param;
			if(param < 0)
			{
				param = (0 - param);
				isAdd = false;
			}

			m_owner->ChangeMoveSpeed(param, isAdd);
			if(param != 0)
			{
				att.state = eAttackBuff;
				att.type = eCharMoveSpeed;
				att.value = m_owner->GetTypeValue(att.type);
				toOther.push_back(att);
			}

			m_ReverseValue = params / 100;

			m_owner->getAttManage()->addPercentValue(eCharMoveSpeed,m_ReverseValue);

			break;
		}
	case eStiff://僵直
		{
			AttrChange att;

			if(m_owner->GetType() == ePlayer)
			{
				Smart_Ptr<Player> temp = m_owner;
				temp->CanccelMeditation();

				temp->ClearPath();
			}
			else if(m_owner->GetType() == eMonster)
			{
				Smart_Ptr<Monster> temp = m_owner;
				temp->ClearPathAndTime();
			}

			m_owner->SetCreatureState(ERS_Still, false);
			att.state = eAttackStill;
			att.type = eSimpleState;
			att.value = 0;
			toOther.push_back(att);

			break;
		}
	case eIce: //冰冻
		{
			AttrChange att;

			if(m_owner->GetType() == ePlayer)
			{
				Smart_Ptr<Player> temp = m_owner;
				temp->CanccelMeditation();

				temp->ClearPath();
			}
			else if(m_owner->GetType() == eMonster)
			{
				Smart_Ptr<Monster> temp = m_owner;
				temp->ClearPathAndTime();
			}


			m_owner->SetCreatureState(ERS_Ice, false);
			att.state = eAttackIce;
			att.type = eSimpleState;
			att.value = 0;
			toOther.push_back(att);

			break;
		}
	case eStun://晕眩
		{
			AttrChange att;

			if(m_owner->GetType() == ePlayer)
			{
				Smart_Ptr<Player> temp = m_owner;
				temp->CanccelMeditation();

				temp->ClearPath();
			}
			else if(m_owner->GetType() == eMonster)
			{
				Smart_Ptr<Monster> temp = m_owner;
				temp->ClearPathAndTime();
			}

			m_owner->SetCreatureState(ERS_Stun, false);
			att.state = eAttackStun;
			att.type = eSimpleState;
			att.value = 0;
			toOther.push_back(att);

			break;
		}
	case eImmunityStiff: //免疫僵直
		{
			AttrChange att;
			if(m_owner->GetType() == ePlayer)
			{
				Smart_Ptr<Player> temp = m_owner;
				temp->CanccelMeditation();
			}

			m_owner->SetCreatureState(ERS_ImmunityStill, false);
			att.state = eImmunityStiff;
			att.type = eSimpleState;
			att.value = 0;
			toOther.push_back(att);

			vector<DWORD>     tempOnlyList;
			BuffGetPublicData tempData;

			tempData.m_Type = eStiff;

			m_owner->getBuffByEffectType(tempOnlyList,tempData);

			for(size_t i = 0;i < tempOnlyList.size();++i)
			{
				m_owner->DeleteBuffByOnlyID(tempOnlyList[i]);
			}

			break;
		}
	case eImmunityIce:   //免疫冰冻
		{
			AttrChange att;
			if(m_owner->GetType() == ePlayer)
			{
				Smart_Ptr<Player> temp = m_owner;
				temp->CanccelMeditation();
			}

			m_owner->SetCreatureState(ERS_ImmunityIce, false);
			att.state = eImmunityIce;
			att.type = eSimpleState;
			att.value = 0;
			toOther.push_back(att);

			vector<DWORD>     tempOnlyList;
			BuffGetPublicData tempData;

			tempData.m_Type = eIce;

			m_owner->getBuffByEffectType(tempOnlyList,tempData);

			for(size_t i = 0;i < tempOnlyList.size();++i)
			{
				m_owner->DeleteBuffByOnlyID(tempOnlyList[i]);
			}

			break;
		}
	case eImmunityStun:  //免疫晕眩
		{
			AttrChange att;
			if(m_owner->GetType() == ePlayer)
			{
				Smart_Ptr<Player> temp = m_owner;
				temp->CanccelMeditation();
			}

			m_owner->SetCreatureState(ERS_ImmunityStun, false);
			att.state = eImmunityStun;
			att.type = eSimpleState;
			att.value = 0;
			toOther.push_back(att);

			vector<DWORD>     tempOnlyList;
			BuffGetPublicData tempData;

			tempData.m_Type = eStun;

			m_owner->getBuffByEffectType(tempOnlyList,tempData);

			for(size_t i = 0;i < tempOnlyList.size();++i)
			{
				m_owner->DeleteBuffByOnlyID(tempOnlyList[i]);
			}

			break;
		}
	case eSkillPercentHurt:
		{
			m_value = param;

			m_owner->getAttManage()->addValue(ePercentHurt,m_value);

			const BuffInfo* tempInfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
			if(tempInfo != NULL)
			{
				m_Effect = Effect::createEffect(*tempInfo,*this);
				EffectCommonData tempData;
				tempData.m_EventID = CREATURE_HURT_EVENT;
				m_Effect->enter(*tempInfo,tempData);
			}

			break;
		}
	case eInvincibleEffect:
	{
		if(m_owner->GetType() == ePlayer)
		{
			Smart_Ptr<Player> temp = m_owner;
			if(!temp->IsInCreatureState(ERS_Invincible))
			{
				m_owner->SetCreatureState(ERS_Invincible, false);
			}
		}
		break;
	}
	case eImmuneHurtShield:		//免疫伤害护盾
	{
		m_owner->SetCreatureState(ERS_ImmuneHurtShield, false);

		const BuffInfo* tempInfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
		if(tempInfo != NULL)
		{
			m_Effect = Effect::createEffect(*tempInfo,*this);
			EffectCommonData tempData;
			tempData.m_EventID = CREATURE_BEHURT_EVENT;
			m_Effect->enter(*tempInfo,tempData);
		}

		break;
	}
	case eVampire:				//吸血
	{
		m_value        = param;
		m_owner->getAttManage()->addValue(ePercentAttVampire,m_value);

		const BuffInfo* tempInfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
		if(tempInfo != NULL)
		{
			m_Effect = Effect::createEffect(*tempInfo,*this);
			EffectCommonData tempData;
			tempData.m_EventID = CREATURE_HURT_EVENT;
			m_Effect->enter(*tempInfo,tempData);
		}

		break;
	}
	case eTriggerAddBuff:		//事件触发加Buff
	{
		const BuffInfo* tempInfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
		if(tempInfo != NULL)
		{
			m_Effect = Effect::createEffect(*tempInfo,*this);
			EffectCommonData tempData;
			if((int)tempInfo->params == 0)
			{
				tempData.m_EventID = CREATURE_HURT_EVENT;
			}
			else
			{
				tempData.m_EventID = (int)tempInfo->params;
			}

			m_Effect->enter(*tempInfo,tempData);
		}

		break;
	}
	case eRepel:				//击退
	{
		int txpos = m_owner->GetXNewPos();
		int typos = m_owner->GetYNewPos();

		CSceneMgr::GetInstance()->GetFinalPosLine(m_owner->GetMapID(), target->GetXNewPos(), target->GetYNewPos(), m_owner->GetXNewPos(), m_owner->GetYNewPos(), txpos, typos, param);

		AttrChange att;
		att.type   = ePosition;
		att.state  = eAttackBackState;
		att.value  = (((int64)txpos << 32) | typos);
		att.obj    = m_owner;
		toOther.push_back(att);

		const BuffInfo* tempInfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
		if(tempInfo != NULL)
		{
			m_Effect = Effect::createEffect(*tempInfo,*this);
			EffectCommonData tempData;
			tempData.m_Target    = m_owner.Get();
			tempData.m_TargetPos.m_X = txpos;
			tempData.m_TargetPos.m_Y = typos;
			m_Effect->enter(*tempInfo,tempData);
		}

		if(m_owner->GetType() == ePlayer)
		{
			Smart_Ptr<Player> temp = m_owner;
			temp->CanccelMeditation();

			temp->ClearPath();
		}
		else if(m_owner->GetType() == eMonster)
		{
			Smart_Ptr<Monster> temp = m_owner;
			temp->ClearPathAndTime();
		}

		m_owner->SetCreatureState(ERS_AttackBack, false);

		break;
	}
	//百分比护盾
	case eAttPersentShield:
	{
		m_value = param;

		m_owner->getAttManage()->addValue(ePercentShield,m_value);

		break;
	}

	//暴击率
	case eEffectPercentCritRate:
	{
		m_value = param;

		m_owner->getAttManage()->addValue(ePercentCritRate,m_value);

		break;
	}
	//闪避率
	case eEffectPercentDodgeRate:
	{
		m_value = param;

		m_owner->getAttManage()->addValue(ePercentDodgeRate,m_value);

		break;
	}
	//必暴击状态
	case eEffectMustBeCrit:
	{
		m_owner->SetCreatureState(ERS_MustBeCrit, false);

		const BuffInfo* tempInfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
		if(tempInfo != NULL)
		{
			m_Effect = Effect::createEffect(*tempInfo,*this);
			EffectCommonData tempData;
			tempData.m_EventID = CREATURE_BEHURT_EVENT;
			m_Effect->enter(*tempInfo,tempData);
		}

		break;
	}
	case eAttFixedShield:
	{
		const BuffInfo* tempInfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
		if(tempInfo != NULL)
		{
			m_Effect = Effect::createEffect(*tempInfo,*this);
			EffectCommonData tempData;
			tempData.m_EventID = BEFORE_CREATURE_BEHURT_EVENT;
			m_Effect->enter(*tempInfo,tempData);
		}
		break;
	}
	case eAttCritBackBlood:
	{
		const BuffInfo* tempInfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
		if(tempInfo != NULL)
		{
			m_Effect = Effect::createEffect(*tempInfo,*this);
			EffectCommonData tempData;
			tempData.m_EventID = CRIT_EVENT;
			m_Effect->enter(*tempInfo,tempData);
		}

		break;
	}
	case eAddState:
	{
		m_value = param;
		m_owner->SetCreatureState(Effect::switchEnumToServer((EAttackState)param),true);
		const BuffInfo* tempInfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
		if(tempInfo != NULL)
		{
			m_Effect = Effect::createEffect(*tempInfo,*this);
			EffectCommonData tempData;
			tempData.m_EventID = CREATURE_HURT_EVENT;
			m_Effect->enter(*tempInfo,tempData);
		}
	
		AttrChange att;
		att.state = (EAttackState)param;
		att.type = eSimpleState;
		att.value = 0;
		toOther.push_back(att);	

		if(params == 0)
		{
			break;
		}

		if(m_owner->GetType() == ePlayer)
		{
			Smart_Ptr<Player> temp = m_owner;
			temp->CanccelMeditation();

			temp->ClearPath();
		}
		else if(m_owner->GetType() == eMonster)
		{
			Smart_Ptr<Monster> temp = m_owner;
			temp->ClearPathAndTime();
		}

		break;
	}	
	case eAlterAtt:
	{
		m_ReverseValue = params;		
		m_value 	   = param; 
		m_owner->getAttManage()->addValue<float>((ECharAttrType)param,params);	
		break;
	}
	case eBackHurtEffect:
	{
		const BuffInfo* tempInfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
		if(tempInfo != NULL)
		{
			m_Effect = Effect::createEffect(*tempInfo,*this);
			EffectCommonData tempData;
			tempData.m_EventID = CREATURE_AFTER_HURT;
			m_Effect->enter(*tempInfo,tempData);
		}	
		break;
	}
	default:
		{
			LOG_ERROR(FILEINFO, "add buff[%d] and buf type is error", m_id);

			return;
		}
	}
}

void BuffBase::SendAttrChange(Smart_Ptr<CreatureObj>& fire, vector<AttrChange> &toMyself, vector<AttrChange> &toOther)
{
	CharBase::SynchObjectAttr attr;
	CharBase::CharAttChange * change = 0;

	attr.set_key(m_owner->GetKey());
	attr.set_attkey(m_casterKey);
	vector<AttrChange>::iterator itOther = toOther.begin();
	for(; itOther!=toOther.end(); ++itOther)
	{
		if(change == 0)
		{
			change = attr.mutable_attr();
		}

		change->add_type((*itOther).type);
		change->add_value((*itOther).value);
		change->add_state((*itOther).state);

		if((*itOther).state == eAttackDead)
		{
			m_owner->Dead(fire);
		}
	}

	if(m_owner->GetType() == ePlayer && toMyself.size() > 0)
	{
		CharBase::CharAttChange toAttr;
		Smart_Ptr<Player> playerOwner = m_owner;

		vector<AttrChange>::iterator itMyself = toMyself.begin();
		for(; itMyself!=toMyself.end(); ++itMyself)
		{
			toAttr.add_type((*itMyself).type);
			toAttr.add_state((*itMyself).state);
			toAttr.add_value((*itMyself).value);
		}

		QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), playerOwner->GetChannelID(), &toAttr, MSG_SIM_GT2C_SYNCHATTR);
	}

	if(attr.has_attr())
	{
		CSceneMgr::GetInstance()->SendSynchAttrChange(attr, m_owner->GetMapID(), m_owner->GetXNewPos(), m_owner->GetYNewPos(), true);
	}
}

void BuffBase::Execute()
{
	++m_curCount;

	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
	if (info == NULL)
		return ;

	if(m_owner->IsDead() && info->isDel)
	{
		return;
	}

	// 查找
	Smart_Ptr<CreatureObj> fire;

	if (m_casterKey == m_owner->GetKey())
	{
		fire = m_owner;
	}
	else
	{
		CSceneMgr::GetInstance()->GetObject(m_owner->GetMapID(), m_casterKey, fire, true);
	}

	if (!fire)
	{
		return ;
	}

	vector<AttrChange> toMyself;
	vector<AttrChange> toOther;

	ResultEffect(fire,info,toMyself, toOther);

	SendAttrChange(fire, toMyself, toOther);
}

void BuffBase::Reverse()
{
	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
	if (info == NULL)
		return ;

	vector<AttrChange> toMyself;
	vector<AttrChange> toOther;

	Smart_Ptr<CreatureObj> fire;

	ResultReverseEffect(info->useType, toMyself, toOther);
	SendAttrChange(fire, toMyself, toOther);
}

void BuffBase::OnEnd()
{
	if (m_bEnd == false)
	{
		// 还原
		Reverse();
		OnEndImp();
		m_bEnd = true;

		if(m_Effect != NULL)
		{
			m_Effect->release();
		}

		m_owner->RemoveBuffByOnlyID(GetOnlyID());
		BuffFactory::Destory(this);
	}
}

void BuffBase::SendBuffOperateMessage(bool type, bool isSyn, int time)
{
	CharBattle::BuffOperate buffOp;

	buffOp.set_onlyid(m_lOnlyID);
	buffOp.set_id(m_id);
	buffOp.set_targetid(m_owner->GetKey());
	buffOp.set_type(type);
	if(time > 0)
	{
		buffOp.set_time(time);
	}

	if(m_owner->GetType() == ePlayer)
	{
		Smart_Ptr<Player> owner = m_owner;

		owner->SendBuffState(&buffOp);
	}

	if(isSyn)
	{
		CSceneMgr::GetInstance()->SendSynBuffChange(&buffOp,
													m_owner->GetKey(),
													m_owner->GetMapID(),
													m_owner->GetXNewPos(),
													m_owner->GetYNewPos());
	}
}

const BuffInfo* BuffBase::GetBuffInfoPtrByID()
{
	return SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);;
}













