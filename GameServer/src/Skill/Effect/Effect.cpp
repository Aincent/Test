/*
 * Effect.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: root
 */

#include "Effect.h"
#include "CountsRemoveEffect.h"
#include "CountsAddBuffEffect.h"
#include "MoveEffect.h"
#include "FixedShield.h"
#include "CritBackBloodEffect.h"
#include "../BuffBase.h"
#include "../../Attribute/AttributeCreature.h"
#include "BackHurtEffect.h"

Effect::Effect(BuffBase& father) : m_Father(father)
{

}

Effect::~Effect()
{

}

Effect* Effect::createEffect(const BuffInfo& info,BuffBase& buff)
{
	Effect* tempEffect = NULL;

	switch(info.useType)
	{
	//免疫伤害护盾
	case eImmuneHurtShield:
	//吸血
	case eVampire:
	//伤害百分比
	case eSkillPercentHurt:
	//必暴击状态
	case eEffectMustBeCrit:
	case eAddState:
		{
			tempEffect = new CountsRemoveEffect(buff);

			break;
		}
	//事件触发加Buff
	case eTriggerAddBuff:
		{
			tempEffect = new CountsAddBuffEffect(buff);

			break;
		}
	//击退
	case eRepel:
		{
			tempEffect = new MoveEffect(buff);

			break;
		}
	//百固定值护盾
	case eAttFixedShield:
		{
			tempEffect = new FixedShield(buff);

			break;
		}
	case eAttCritBackBlood:
		{
			tempEffect = new CritBackBloodEffect(buff);

			break;
		}
	case eBackHurtEffect:
		{
			tempEffect = new BackHurtEffect(buff);
			break;
		}	
	default:
		{
			break;
		}
	}

	return tempEffect;
}

float Effect::getEffectValue(const EffectOneParam& data,EffectParam& param)
{
	float tResult = .0f;
	switch(data.m_Type)	
	{
	case eEffectConst:
		{
			tResult = data.m_ParamList[0];
			break;
		}
	case eEffectSelfAtt:
		{
			if(param.m_Self == NULL)
			{
				break;
			}
			tResult = param.m_Self->getAttManage()->getValue<float>((ECharAttrType)data.m_ParamList[0]);
			break;
		}
	case eEffectTargetAtt:
		{
			if(param.m_Target == NULL)
			{
				break;
			}
			tResult = param.m_Target->getAttManage()->getValue<float>((ECharAttrType)data.m_ParamList[0]);
			break;
		}
	default:
		{
			break;
		}
	}

	return tResult;
}

//客户端状态转成服务器状态
ECreatureState Effect::switchEnumToServer(EAttackState type)
{
	ECreatureState tempResultType = ERS_NULL;
	switch(type)
	{
	case eAttackDead:		//死亡
		{
			tempResultType = ERS_Dead;
			break;
		}
	case eAttackBackState:	//击退
		{
			tempResultType = ERS_AttackBack;
			break;
		}
	case eAttackStill:		//僵直
		{
			tempResultType = ERS_Still;
			break;
		}
	case eAttackIce:		//冰冻
		{
			tempResultType = ERS_Ice;
			break;
		}
	case eAttackStun:		//晕眩
		{
			tempResultType = ERS_Stun;
			break;
		}
	case eStateHorse:		//坐骑状态
		{
			tempResultType = ERS_Horse;
			break;
		}
	case eWalkState:		//行走状态
		{
			tempResultType = ERS_Walk;
			break;
		}
	case eIntonate:			//吟唱状态
		{
			tempResultType = ERS_Intonate;
			break;
		}
	case eSilentState:
		{
			tempResultType = ERS_Silent;
			break;
		}
	case eImmuintySilent:
		{
			tempResultType = ERS_ImmunitySilent;
			break;
		}
	case eSleepState:
		{
			tempResultType = ERS_Sleep;
			break;
		}
	case eImmunitySleep:
		{
			tempResultType = ERS_ImmunitySleep;
			break;
		}
	case ePoisoning:	   
		 {
			 tempResultType = ERS_Poisoning;
			 break;
		 }
	case eImmunityPoison: 
		 {
		     tempResultType = RES_ImmunityPoison;	
			 break;
		 }
	default:
		{
			break;
		}
	}
	return tempResultType;
}

//服务器状态转成客户端状态
EAttackState Effect::switchEnumToClient(ECreatureState type)
{
	EAttackState tempResultType = eAttackMiss;
	switch(type)
	{
	case ERS_Dead:		//死亡
		{
			tempResultType = eAttackDead;
			break;
		}
	case ERS_AttackBack:	//击退
		{
			tempResultType = eAttackBackState;
			break;
		}
	case ERS_Still:		//僵直
		{
			tempResultType = eAttackStill;
			break;
		}
	case ERS_Ice:		//冰冻
		{
			tempResultType = eAttackIce;
			break;
		}
	case ERS_Stun:		//晕眩
		{
			tempResultType = eAttackStun;
			break;
		}
	case ERS_Horse:		//坐骑状态
		{
			tempResultType = eStateHorse;
			break;
		}
	case ERS_Walk:		//行走状态
		{
			tempResultType = eWalkState;
			break;
		}
	case ERS_Intonate:			//吟唱状态
		{
			tempResultType = eIntonate;
			break;
		}
	case ERS_Silent:
		{
			tempResultType = eSilentState;
			break;
		}
	case ERS_ImmunitySilent:
		{
			tempResultType = eImmuintySilent;
			break;
		}
	case ERS_Sleep: 
		{
			tempResultType = eSleepState;
			break;
		}
	case ERS_ImmunitySleep:
		{
			tempResultType = eImmunitySleep;
			break;
		}
	case ERS_Poisoning:	   
		 {
			 tempResultType = ePoisoning;
			 break;
		 }
	case RES_ImmunityPoison: 
		 {
		     tempResultType = eImmunityPoison;	
			 break;
		 }
	default:
		{
			break;
		}
	}

	return tempResultType;
}

//根据状态获取对应增强属性
ECharAttrType Effect::stateToStrengAtt(EAttackState state) 
{
	ECharAttrType tResult = eStrengStun;
	switch(state)
	{
	case eAttackStun:
		{
			tResult = eStrengStun;
			break;
		}
	case eSilentState:
		{
			tResult = eStrengSilent;
			break;
		}
	case ePoisoning:
		{
			tResult = eStrengPoisoning;
			break;
		}
	default:
		{
			break;
		}
	}

	return tResult;
	
}

//根据状态获取对应抵抗属性
ECharAttrType Effect::stateToResistAtt(EAttackState state)
{
	ECharAttrType tResult = eResistStun;
	switch(state)
	{
	case eAttackStun:
		{
			tResult = eResistStun;
			break;
		}
	case eSilentState:
		{
			tResult = eResistSilent;
			break;
		}
	case ePoisoning:
		{
			tResult = eResistPoisoning;
			break;
		}
	default:
		{
			break;
		}
	}

	return tResult;
}

//判断该buff是否要做状态概率控制
EAttackState Effect::isAddStateControl(const BuffInfo& info)
{
	if(info.useType == eStun)
	{
		return eAttackStun;
	}

	if(info.useType == eAddState)
	{
		if(info.m_pParam->m_Param == eSilentState) 
		{
			return eSilentState;
		}

		if(info.m_pParam->m_Param == ePoisoning)
		{
			return ePoisoning;
		}
	}

	return eAttackMiss;
}
