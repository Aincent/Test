/*
 * SkillBase.cpp
 *
 *  Created on: 2014年1月21日
 *      Author: helh
 */
#include "SkillBase.h"
#include "SkillFactory.h"
#include "MessageStruct/CharBattle/Battle.pb.h"
#include "../Map/SceneManager.h"
#include "Log/Logger.h"
#include "MessageStruct/CharBase/CharAttribute.pb.h"
#include "../Object/FileLoader/GirlWarLoader.h"
#include <math.h>
#include "../ServerEventDefine.h"
#include "../Attribute/AttributeCreature.h"
#include "Effect/Effect.h"
#include "../Object/AI/AIBrain.h"

CSkillBase::CSkillBase(int id, Smart_Ptr<CreatureObj> & owner,int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info):
m_owner(owner),
m_targetKey(key),
m_xpos(xpos),
m_ypos(ypos),
m_id(id),
m_onlyID(onlyid),
m_bEnd(false),
m_info(info)
{
	//m_stopEvent = RegEvent(m_owner, CREATURE_DEAD_EVENT_TYPE, this, &CSkillBase::HandleCreatureDead);
	//m_leaveEvent = RegEvent(m_owner, PLAYER_LEAVE_MAP_EVENT_TYPE, this, &CSkillBase::HandleCreatureLeaveMap);
}

CSkillBase::~CSkillBase()
{
	m_ActionTimer.SetNull();
	m_owner = 0;

	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}

	if(m_leaveEvent.IsValid())
	{
		m_leaveEvent.SetNull();
	}
}

void CSkillBase::InitInfo(int id, Smart_Ptr<CreatureObj> & owner,int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info)
{
	m_owner = owner;
	m_targetKey = key;
	m_xpos = xpos;
	m_ypos = ypos;
	m_id = id;
	m_onlyID = onlyid;
	m_bEnd = false;
	m_info = info;

	if(!m_stopEvent.IsValid())
		m_stopEvent = RegEvent(m_owner, CREATURE_DEAD_EVENT_TYPE, this, &CSkillBase::HandleCreatureDead);

	if(!m_leaveEvent.IsValid())
		m_leaveEvent = RegEvent(m_owner, PLAYER_LEAVE_MAP_EVENT_TYPE, this, &CSkillBase::HandleCreatureLeaveMap);
}

void CSkillBase::Release()
{
	m_ActionTimer.SetNull();
	m_owner = 0;
	m_info = NULL;

	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}

	if(m_leaveEvent.IsValid())
	{
		m_leaveEvent.SetNull();
	}

}

// 限制目标个数
void CSkillBase::LimitTargetsNum(vector<Smart_Ptr<CreatureObj> >& targets,const unsigned int uiLimit)
{
	if(targets.size() <= uiLimit) return;

	for(unsigned int iCursor= 0; iCursor < targets.size(); ++iCursor)
	{
		for(unsigned int tCursor = iCursor+1; tCursor < targets.size(); ++tCursor)
		{
			int lXLen = abs(m_owner->GetXNewPos() - targets[iCursor]->GetXNewPos());
			int lYLen = abs(m_owner->GetYNewPos() - targets[iCursor]->GetYNewPos());
			int rXLen = abs(m_owner->GetXNewPos() - targets[tCursor]->GetXNewPos());
			int rYLen = abs(m_owner->GetYNewPos() - targets[tCursor]->GetYNewPos());

			if(sqrt(lXLen * lXLen + lYLen * lYLen) > sqrt(rXLen * rXLen + rYLen * rYLen))
			{
				swap(targets[iCursor] , targets[tCursor]);
			}
		}
	}

	targets.resize(uiLimit);
}

int CSkillBase::GetValue(Smart_Ptr<CreatureObj> & targets, int *param)
{
	int value = 0;

	if(param[2] == 0)
	{
		value = param[0];
	}
	else
	{
		//护盾
		if(targets->IsInCreatureState(ERS_ImmuneHurtShield))
		{
			return 0;
		}

		value = BattleInfoLoader::GetInstance()->GetPhysicalDamage(
					m_owner->getAttManage()->getValue<int>(eCharPhyAttackMax),
					targets->getAttManage()->getValue<int>(eCharPhyDefence),
					param[0],
					m_owner->getAttManage()->getValue<int>(eCharLv),
					targets->getAttManage()->getValue<int>(eCharLv),
					param[1],
					m_owner->getAttManage()->getValue<int>(eFightPower),
					targets->getAttManage()->getValue<int>(eFightPower));
	}

	return value;
}

bool CSkillBase::BeginAction()
{
	SynchFire();
	Amerce();
	CDTimer();

	if(m_owner->GetType() == ePlayer)
	{
		if(m_info->limitPro != eProfessionMax && m_info->fType != eAssistSkill)
		{
			Smart_Ptr<Player> obj = m_owner;

			obj->GetSkillMgr()->AddSkill(this);
		}
		else if(m_info->limitPro == eProfessionMax)
		{
			Smart_Ptr<Player> obj = m_owner;

			obj->GetSkillMgr()->SetBigSkill(this);
		}
	}

	if(m_info->m_ActionTime > 0)
	{
		m_ActionTimer = TimerInterface::GetInstance()->RegTimePoint(&CSkillBase::actionTime, this, 0, 0, 0, 0, m_info->m_ActionTime);
	}
	else
	{
		return BeginStart();
	}

	ObjectDoingArgs tempArgs(*m_owner.Get(),eDoingReleaseSkill);
	m_owner->FireEvent(OBJECT_EXCUTE_DOING,tempArgs);

	return true;
}

void CSkillBase::ResultEffect(Smart_Ptr<CreatureObj> & targets, vector<AttrChange> &changeVec, vector<AttrChange> &changeMyself, int type, int *param,int index)
{
	AttrChange change;
	int value = GetValue(targets, param);

	switch(type)
	{
	case eDscHP://.减少对象当前HP
		{
			if(m_owner == targets)
				return;

			int tempHp = targets->getAttManage()->getValue<int>(eCharHP);
			int curhp = 0;
			if(m_owner->IsAttackCrack(targets))
			{
				value = 2 * value;
				change.state = eAttackCrack;

				HurtArgs tempCritArgs(m_owner,targets);
				m_owner->FireEvent(CRIT_EVENT,tempCritArgs);

				HurtArgs tBeCritArgs(targets,m_owner);
				targets->FireEvent(BE_CRIT_EVENT,tBeCritArgs);
			}

			//伤害百分比
			short tempPercentValue = m_owner->getAttManage()->getValue<short>(ePercentHurt);
			short tempShieldValue  = targets->getAttManage()->getValue<short>(ePercentShield);

			value = (int)(value + value * (tempPercentValue - tempShieldValue) / 100);

			BeforeBeHurtArgs tempBeforeArgs(targets,m_owner,value);
			targets->FireEvent(BEFORE_CREATURE_BEHURT_EVENT,tempBeforeArgs);

			value = tempBeforeArgs.m_DamageValue;

			curhp = targets->CurHPChange(value, false);

			AfterBeHurtArgs tAfterArgs(targets,m_owner,value);
			targets->FireEvent(CREATURE_AFTER_HURT,tAfterArgs);
			targets->UpdateEnemyValue((tempHp - curhp), m_owner->GetKey());
			if(m_owner->GetType() == ePlayer)
			{
				PlayerAttackCreature tArgs(m_owner, targets,targets->GetKey(), tempHp - curhp);

				FireGlobalEvent(PLAYER_ATTACK_CREATURE, tArgs);
			}
			//宠物攻击
			else if(m_owner->GetType() == ePet)
			{
				BetAttackArgs tempPetArgs(m_owner,targets, tempHp - curhp);

				FireGlobalEvent(PET_ATTACK,tempPetArgs);
			}

			if(change.state == eAttackSim)
			{
				change.state = eAttackSkill;
			}
			change.type = eCharHP;
			change.state |= ((int64)m_id << 32);
			change.value = targets->getAttManage()->getValue<int>(eCharHP);
			change.obj = targets;
			changeVec.push_back(change);

			if(curhp <= 0)
			{
				AttrChange changeState;
				changeState.type = eSimpleState;
				changeState.state = eAttackDead;
				changeState.value = m_owner->GetKey();
				changeState.obj = targets;
				changeState.m_Target = m_owner;

				changeVec.push_back(changeState);
			}

			//吸血
			float tempPercentVampire = (float)m_owner->getAttManage()->getValue<int>(ePercentAttVampire) / 10000;
			if(m_owner->getAttManage()->getValue<float>(eInjured) > .0f)
			{
				tempPercentVampire = tempPercentVampire * m_owner->getAttManage()->getValue<float>(eInjured);
			}

			tempPercentVampire = tempPercentVampire * value;
			if(tempPercentVampire > 0)
			{
				m_owner->CurHPChange((int)tempPercentVampire,true);
				//有几率吸收目标百分比血量

				AttrChange tempVampireChange;

				tempVampireChange.state  = eAttackSkill;
				tempVampireChange.type   = eCharHP;
				tempVampireChange.state |= ((int64)m_id << 32);
				tempVampireChange.value  = m_owner->getAttManage()->getValue<int>(eCharHP);
				tempVampireChange.obj    = m_owner;

				changeMyself.push_back(tempVampireChange);
			}

			BeHurtArgs tempArgs(targets,m_owner);
			targets->FireEvent(CREATURE_BEHURT_EVENT,tempArgs);

			HurtArgs tempHurtArgs(m_owner,targets);
			m_owner->FireEvent(CREATURE_HURT_EVENT,tempHurtArgs);

			break;
		}
	case eAddHP://增加对象当前HP
		{
			targets->CurHPChange(value);
			change.type = eCharHP;
			change.state = eAttackSkill;
			change.state |= ((int64)m_id << 32);
			change.value = targets->getAttManage()->getValue<int>(eCharHP);
			change.obj = targets;
			changeVec.push_back(change);

			break;
		}
	case eAddMP://增加对象当前MP
		{
			targets->ChangeCurMP(value);
			change.type = eCharMP;
			change.state = eAttackSkill;
			change.state |= ((int64)m_id << 32);
			change.value = targets->getAttManage()->getValue<int>(eCharMP);
			change.obj = targets;
			changeVec.push_back(change);

			break;
		}
	case eAddHPMax://增加永久HP上限
		{
			targets->ChangeHPMax(value);
			change.type = eCharHPMax;
			change.state = eAttackSkill;
			change.state |= ((int64)m_id << 32);
			change.value = targets->getAttManage()->getValue<int>(eCharHPMax);
			change.obj = targets;
			changeVec.push_back(change);

			break;
		}
	case eAddMPMax://增加永久MP上限
		{
			targets->ChangeMPMax(value);
			change.type = eCharMPMax;
			change.state = eAttackSkill;
			change.state |= ((int64)m_id << 32);
			change.value = targets->getAttManage()->getValue<int>(eCharMPMax);
			change.obj = targets;
			changeVec.push_back(change);

			break;
		}
	case eAddPhyAttack://增加永久物理攻击 值
		{
			targets->ChangePhyAttackMax(value, true);
			if(targets->GetType() == ePlayer)
			{
				change.type = eCharPhyAttackMax;
				change.state = eAttackSkill;
				change.state |= ((int64)m_id << 32);
				change.value = targets->getAttManage()->getValue<int>(eCharPhyAttackMax);
				change.obj = targets;
				changeMyself.push_back(change);
			}

			break;
		}
	case eAddMagAttack://增加永久法术攻击值
		{
			break;
		}
	case eAddSpeed://增加永久速度值
		{
			targets->ChangeMoveSpeed(value);
			change.type = eCharMoveSpeed;
			change.state = eAttackSkill;
			change.state |= ((int64)m_id << 32);
			change.value = targets->getAttManage()->getValue<int>(eCharMoveSpeed);
			change.obj = targets;
			changeVec.push_back(change);

			break;
		}
	case eAddPhyDefence://增加永久物理防御值
		{
			targets->ChangePDefence(value);
			if(targets->GetType() == ePlayer)
			{
				change.type = eCharPhyDefence;
				change.state = eAttackSkill;
				change.state |= ((int64)m_id << 32);
				change.value = targets->getAttManage()->getValue<int>(eCharPhyDefence);
				change.obj = targets;
				changeMyself.push_back(change);
			}

			break;
		}
	case eAddAvoidRate://增加永久闪避值
		{
			targets->ChangeARate(value);
			if(targets->GetType() == ePlayer)
			{
				change.type = eCharAvoidRate;
				change.state = eAttackSkill;
				change.state |= ((int64)m_id << 32);
				change.value = targets->getAttManage()->getValue<int>(eCharAvoidRate);
				change.obj = targets;
				changeMyself.push_back(change);
			}

			break;
		}
	case eAddCrackRate://增加永久暴击值
		{
			targets->ChangeCRate(value);
			if(targets->GetType() == ePlayer)
			{
				change.type = eCrackDownRate;
				change.state = eAttackSkill;
				change.state |= ((int64)m_id << 32);
				change.value = targets->getAttManage()->getValue<int>(eCrackDownRate);
				change.obj = targets;
				changeMyself.push_back(change);
			}

			break;
		}
	case eAddHitRate://增加永久命中值
		{
			targets->ChangeHRate(value);
			if(targets->GetType() == ePlayer)
			{
				change.type = eCharHitRate;
				change.state = eAttackSkill;
				change.state |= ((int64)m_id << 32);
				change.value = targets->getAttManage()->getValue<int>(eCharHitRate);
				change.obj = targets;
				changeMyself.push_back(change);
			}

			break;
		}
	case eAttackBackNULL:
		{
			break;
		}
	case eAttackFlyNULL:
		{
			break;
		}
	case eAddAvoidCrack://永久增加抗暴
		{
			targets->ChangeBaserte(value);
			if(targets->GetType() == ePlayer)
			{
				change.type = eAvoidCrackRate;
				change.state = eAttackSkill;
				change.state |= ((int64)m_id << 32);
				change.value = targets->getAttManage()->getValue<int>(eAvoidCrackRate);
				change.obj = targets;
				changeMyself.push_back(change);
			}

			break;
		}
	case eRevive: //复活主角
		{
			if(m_owner->GetType() != ePet)
				return;

			Smart_Ptr<GirlWar> girlWar = m_owner;
			if(girlWar->GetOwner())
			{
				return;
			}

			Smart_Ptr<Player> player = girlWar->GetOwner();

			if(player->IsDead())
			{
				CharBattle::RevivalData req;
				//血量和蓝量全满
				player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));
				player->SetCurMP(player->getAttManage()->getValue<int>(eCharMPMax));

				player->SetDataFlag(eBaseInfo);

				req.set_key(player->GetKey());
				req.set_hp(player->getAttManage()->getValue<int>(eCharHP));
				req.set_mp(player->getAttManage()->getValue<int>(eCharMP));

				player->SendSynchMessage(&req, MSG_SIM_GT2C_REVIVAL);
				CSceneMgr::GetInstance()->SendSynchMessage(&req, MSG_SIM_GT2C_REVIVAL, player->GetMapID(), player->GetKey(), player->GetXNewPos(), player->GetYNewPos(), false);
			}

			break;
		}
	case eSuck: //吸取血量
		{
			if(m_owner->GetType() != ePet)
				return;

			Smart_Ptr<GirlWar> girlWar = m_owner;
			if(girlWar->GetOwner())
			{
				return;
			}

			vector<int> vecType;
			Smart_Ptr<Player> player = girlWar->GetOwner();
			int suckHp = (targets->getAttManage()->getValue<int>(eCharHP) * m_info->atkRate[2])/100;

			//有几率吸收目标百分比血量
			int curhp = targets->CurHPChange(suckHp, false);
			if(change.state == eAttackSim)
			{
				change.state = eAttackSkill;
			}
			change.type = eCharHP;
			change.state |= ((int64)m_id << 32);
			change.value = targets->getAttManage()->getValue<int>(eCharHP);
			change.obj = targets;
			changeVec.push_back(change);
			if(curhp <= 0)
			{
				AttrChange changeState;
				changeState.type = eSimpleState;
				changeState.state = eAttackDead;
				changeState.value = m_owner->GetKey();
				changeState.obj = targets;
				changeVec.push_back(changeState);
			}
			//为主角增加血量
			player->CurHPChange(suckHp);
			vecType.push_back(eCharHP);
			player->SynCharAttribute(vecType);

			break;
		}
	case eBloodRecovery: //血量回复
		{
			if(m_owner->GetType() != ePet)
				return;

			Smart_Ptr<GirlWar> girlWar = m_owner;
			if(girlWar->GetOwner())
			{
				return;
			}

			vector<int> vecType;
			Smart_Ptr<Player> player = girlWar->GetOwner();
			int recoveryHp = (player->getAttManage()->getValue<int>(eCharHP) * m_info->atkRate[2])/100;
			//为主角增加血量
			player->CurHPChange(recoveryHp);
			vecType.push_back(eCharHP);
			player->SynCharAttribute(vecType);
			break;
		}
	case eMinusCD:
		{
			if(m_owner->GetType() != ePlayer)
			{
				break;
			}

			Smart_Ptr<Player> owner = m_owner;
			//不指定频道，减所有技能CD
			if(param[0] != 0)
			{
				owner->GetCDMgr()->cdTimeAlter((eChannelType)param[0],param[1]);
			}
			else
			{
				vector<eChannelType>* tempChannel = SkillLoader::GetInstance()->getCDChannelByProfession(
						(ECharProf)owner->getAttManage()->getValue<BYTE>(eCharProfession));

				if(tempChannel != NULL)
				{
					vector<int> tempCDList;
					tempCDList.resize(tempChannel->size(),param[1]);
					owner->GetCDMgr()->cdTimeAlterList(*tempChannel,tempCDList);
				}
			}

			break;
		}
	case eRemoveBuffByGrupID:
		{
			
			if(m_info->m_EffectTargetType[index] == eEnemyTarget)
			{
				targets->removeBuffByGroupID(param[0]);
			}
			else
			{
				m_owner->removeBuffByGroupID(param[0]);
			}

			break;
		}
	}
}

void CSkillBase::ResultEffect(int effectIndex, vector<Smart_Ptr<CreatureObj> > & targets, map<int, vector<AttrChange> >& allAttr, map<int, vector<AttrChange> >& myselfAttr)
{
	Smart_Ptr<CreatureObj> tOwner = m_owner;
	int randNum = rand() % 100 + 1;

	if(randNum <= m_info->atkRate[effectIndex])
	{
		LimitTargetsNum(targets, m_info->targetCount);
		vector<Smart_Ptr<CreatureObj> >::iterator it = targets.begin();

		for(; it!=targets.end(); ++it)
		{
			if(m_bEnd)
			{
				break;
			}

			if(tOwner->getAttManage()->getValue<int>(eCharHP) <= 0)
			{
				break;
			}	

			//以后如果写了模式切换的，得加上限制，判断是否能加入到仇恨列表中
			(*it)->Attacked(m_owner);
			m_owner->Attack(*it);

			vector<AttrChange> change;
			vector<AttrChange> changeMyself;

			if(m_info->effectType[effectIndex] == eEffectBuff || tOwner->IsHitObject(*it))
			{
				//命中
				if((*it)->GetType() == ePlayer)
				{
					Smart_Ptr<Player> player = (*it);
				}

				switch(m_info->effectType[effectIndex])
				{
				case eEffectDirect:
					{
						ResultEffect(*it, change, changeMyself, m_info->effectFirstType[effectIndex], m_info->param[effectIndex],effectIndex);

						if((*it)->GetType() == ePlayer)
						{
							Smart_Ptr<Player> owner = *it;
							owner->SetDataFlag(eBattleInfo);
						}

						if((*it)->GetType() == ePet)
						{
							Smart_Ptr<GirlWar> owner = *it;
							owner->GetOwner()->SetDataFlag(eGirlWarInfo);
						}

						break;
					}
				case eEffectBuff:
					{
						ECreatureState tempState = Effect::switchEnumToServer((EAttackState)m_info->param[effectIndex][0]);
						if(m_info->m_EffectTargetType[effectIndex] == eEnemyTarget)
						{
							//目前用于作BUFF条件判断
							if(tempState != ERS_NULL && !(*it)->IsInCreatureState(tempState))
							{
								break;
							}

							(*it)->AddBuff(m_info->effectFirstType[effectIndex], tOwner);
						}
						else
						{
							//目前用于作BUFF条件判断
							if(tempState != ERS_NULL && !(tOwner)->IsInCreatureState(tempState))
							{
								break;
							}

							(tOwner)->AddBuff(m_info->effectFirstType[effectIndex], tOwner);
						}
						

						break;
					}
				}
			}
			else
			{
				if((*it)->GetKey() == tOwner->GetKey() || m_info->effectFirstType[effectIndex] != eDscHP)
					continue;

				AttrChange missChange;

				missChange.type = eSimpleState;
				missChange.state = eAttackMiss;
				missChange.value = 0;
				missChange.obj = (*it);

				change.push_back(missChange);
			}

			if(tOwner->getAttManage()->getValue<int>(eCharHP) <= 0)
			{
				AttrChange changeState;
				changeState.type = eSimpleState;
				changeState.state = eAttackDead;
				changeState.value = (*it)->GetKey();
				changeState.obj = tOwner;
				changeState.m_Target = (*it) ;

				allAttr[tOwner->GetKey()].push_back(changeState);
			}

			if(change.size() > 0)
			{
				map<int, vector<AttrChange> >::iterator itAtt = allAttr.find((*it)->GetKey());
				if(itAtt == allAttr.end())
				{
					allAttr[(*it)->GetKey()] = change;
				}
				else
				{
					itAtt->second.insert(itAtt->second.end(), change.begin(), change.end());
				}
			}

			if(changeMyself.size() > 0)
			{
				map<int, vector<AttrChange> >::iterator itAtt = myselfAttr.find((*it)->GetKey());
				if(itAtt == myselfAttr.end())
				{
					myselfAttr[(*it)->GetKey()] = changeMyself;
				}
				else
				{
					itAtt->second.insert(itAtt->second.end(), changeMyself.begin(), changeMyself.end());
				}
			}
		}
	}

}

void CSkillBase::ResultEffectAttrChange(map<int, vector<AttrChange> >& allAttr, map<int, vector<AttrChange> >& myselfAttr)
{
	//发给目标自己的
	map<int, vector<AttrChange> >::iterator itSelfMap = myselfAttr.begin();
	for(; itSelfMap!=myselfAttr.end(); ++itSelfMap)
	{
		vector<int> vecMyself;
		Smart_Ptr<Player> synObj;

		vector<AttrChange>::iterator it = itSelfMap->second.begin();
		for(; it!=itSelfMap->second.end(); ++it)
		{
			if(it == itSelfMap->second.begin())
			{
				synObj = (*it).obj;
			}

			vecMyself.push_back((*it).type);
		}

		synObj->SynCharAttribute(vecMyself);
	}

	//广播
	map<int, vector<AttrChange> >::iterator itMap = allAttr.begin();
	for(; itMap!=allAttr.end(); ++itMap)
	{
		CharBase::SynchObjectAttr attr;
		CharBase::CharAttChange * change = 0;
		CharBase::SynchObjectAttr attrMap;//全图广播的
		CharBase::CharAttChange * changeMap = 0;//全图广播的
		Smart_Ptr<CreatureObj> synObj;

		attr.set_key(itMap->first);
		attrMap.set_key(itMap->first);
		if(m_owner)
		{
			attr.set_attkey(m_owner->GetKey());
			attrMap.set_attkey(m_owner->GetKey());
		}

		int64 mapID=0;
		int posx=0, posy = 0;
		int type = 0;

		vector<AttrChange>::iterator it = itMap->second.begin();
		for(; it!=itMap->second.end(); ++it)
		{
			mapID = (*it).obj->GetMapID();
			posx		= (*it).obj->GetXNewPos();
			posy		= (*it).obj->GetYNewPos();
			type		= (*it).obj->GetType();

			//todo 怪物一旦死亡离开场景，就不能再引用该对象，已经被系统回收了，所有的数据都清空了
			if(it->type == eSimpleState && it->state == eAttackDead)
			{
				it->obj->Dead(it->m_Target);
			}

			if(GET_MAP_TYPE(mapID) == ePVEMap && (*it).state == eAttackDead && type == eMonster && InstanceLoader::GetInstance()->GetEntryType(GET_MAP_ID(mapID)) == eClanBattle)
			{
				if(changeMap == 0)
				{
					changeMap = attrMap.mutable_attr();
					//synObj = (*it).obj;
				}

				changeMap->add_type((*it).type);
				changeMap->add_value((*it).value);
				changeMap->add_state((*it).state);
			}
			else
			{
				if(change == 0)
				{
					change = attr.mutable_attr();
					//synObj = (*it).obj;
				}

				change->add_type((*it).type);
				change->add_value((*it).value);
				change->add_state((*it).state);
			}
		}

		if(attr.has_attr())
		{
			CSceneMgr::GetInstance()->SendSynchAttrChange(attr, mapID, posx, posy, true);
		}

		if(attrMap.has_attr())
		{
			CSceneMgr::GetInstance()->SendAllMapAttr(attrMap, mapID);
		}
	}
}

bool CSkillBase::HandleCreatureDead(const EventArgs & e)
{
	OnEndEx();

	return true;
}

bool CSkillBase::HandleCreatureLeaveMap(const EventArgs & e)
{
	OnEnd();

	return true;
}

void CSkillBase::SynchFire()
{
	CharBattle::SynClientAttack attack;

	attack.set_skillid(m_id);
	attack.set_begin(m_owner->GetKey());

	//获取目标对象obj
	Smart_Ptr<CreatureObj> targetObj;
	CSceneMgr::GetInstance()->GetObject(m_owner->GetMapID(),m_targetKey,targetObj);

	if((bool)targetObj && !targetObj->IsDead() && targetObj->IsInMap())
	{
		attack.set_target(m_targetKey);

		//位移技能必须要求策划，把位移的效果配在第一个效果
		if(m_info->effectFirstType[0] == eMove)
		{
			int txpos = m_owner->GetXNewPos();
			int typos = m_owner->GetYNewPos();
			int distance = 0;

			if(m_info->fType == eAssistSkill)
			{
				txpos = m_owner->GetXNewPos();
				typos = m_owner->GetYNewPos();
			}
			else
			{
				attack.set_target(-1);
				distance = GetValue(m_owner, m_info->param[0]);
				CSceneMgr::GetInstance()->GetBeginPosLine(m_owner->GetMapID(), m_owner->GetXNewPos(), m_owner->GetYNewPos(), targetObj->GetXNewPos(), targetObj->GetYNewPos(), txpos, typos, distance);
			}

			if(txpos >= 0 && typos >= 0)
			{
				attack.set_xpos(txpos);
				attack.set_ypos(typos);
				m_xpos = txpos;
				m_ypos = typos;
			}
		}
	}
	else
	{
		attack.set_xpos(m_xpos);
		attack.set_ypos(m_ypos);

		//位移技能必须要求策划，把位移的效果配在第一个效果
		if(m_info->effectFirstType[0] == eMove)
		{
			int txpos = m_owner->GetXNewPos();
			int typos = m_owner->GetYNewPos();
			int distance = 0;


			if(m_info->fType == eAssistSkill)
			{
				if(!CSceneMgr::GetInstance()->IsValidPos(m_owner->GetMapID(), m_xpos, m_ypos))
				{
					distance = GetValue(m_owner, m_info->param[0]);
					CSceneMgr::GetInstance()->GetBeginPosLine(m_owner->GetMapID(), m_owner->GetXNewPos(), m_owner->GetYNewPos(), m_xpos, m_ypos, txpos, typos, distance);
				}
				else
				{
					txpos = m_xpos;
					typos = m_ypos;
				}
			}
			else
			{
				attack.set_target(-1);
				distance = GetValue(m_owner, m_info->param[0]);
				CSceneMgr::GetInstance()->GetBeginPosLine(m_owner->GetMapID(), m_owner->GetXNewPos(), m_owner->GetYNewPos(), m_xpos, m_ypos, txpos, typos, distance);
			}

			if(txpos >= 0 && typos >= 0)
			{
				attack.set_xpos(txpos);
				attack.set_ypos(typos);
				m_xpos = txpos;
				m_ypos = typos;
			}
		}
	}

	CSceneMgr::GetInstance()->SendSynchAttackAction(attack, m_owner->GetMapID(), m_owner->GetXNewPos(), m_owner->GetYNewPos());
}

void CSkillBase::OnEndEx()
{
	OnEndImp();
	if(!m_bEnd)
		CSkillFactory::FreeSkill(this);

	m_ActionTimer.SetNull();

	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}

	if(m_leaveEvent.IsValid())
	{
		m_leaveEvent.SetNull();
	}

	m_bEnd = true;
}

void CSkillBase::OnEnd()
{
	OnEndImp();
	if(!m_bEnd)
		CSkillFactory::FreeSkill(this);

	m_ActionTimer.SetNull();

	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}

	if(m_leaveEvent.IsValid())
	{
		m_leaveEvent.SetNull();
	}

	m_bEnd = true;

	m_owner = 0;
}

void CSkillBase::Amerce()
{
	vector<int> vec;
	CharBase::SynchObjectAttr attr;

	attr.set_key(m_owner->GetKey());

	if(m_info->useHP > 0)
	{
		m_owner->CurHPChange(m_info->useHP, false);
		vec.push_back(eCharHP);
	}

	if(m_info->useMP > 0)
	{
		m_owner->ChangeCurMP(m_info->useMP, false);
		vec.push_back(eCharMP);
	}

	if(vec.size() > 0)
	{
		CharBase::CharAttChange * change = attr.mutable_attr();

		for(size_t i=0; i<vec.size(); ++i)
		{
			change->add_type(vec[i]);
			change->add_value(m_owner->GetTypeValue(vec[i]));
			change->add_state(eAttackSim);
		}

		CSceneMgr::GetInstance()->SendSynchAttrChange(attr, m_owner->GetMapID(), m_owner->GetXNewPos(), m_owner->GetYNewPos(), true);
	}
}

void CSkillBase::CDTimer()
{
	switch (m_owner->GetType())
	{
	case ePlayer:
		{
			Smart_Ptr<Player> owner = m_owner;

			// 开始CD计时
			owner->GetCDMgr()->StartCD(m_info->channelType, m_info->coolTime);
			if(m_id > 1000)
			{
				const SingleSkillData& tempData = SkillLoader::GetInstance()->getSingleData();

				owner->GetCDMgr()->StartCD(SKILL_COMMON, tempData.m_SkillCommonCD);
			}

			break;
		}
	}
}

void CSkillBase::actionTime(void * p)
{
	if(m_owner->IsDead())
	{
		return ;
	}

	BeginStart();

	return ;
}







