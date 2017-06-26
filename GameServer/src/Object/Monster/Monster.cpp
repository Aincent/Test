/*
 * Monster.cpp
 *
 *  Created on: 2013��10��22��
 *      Author: helh
 */
#include <cstdlib>
#include <sstream>
#include "Monster.h"
#include "Log/Logger.h"
#include "../FileLoader/AILoader.h"
#include "SvrConfig.h"
#include "../../Map/SceneManager.h"
#include "../../Map/ConfigLoader/MapConfig.h"
#include "../../Map/ConfigLoader/FieldInfo.h"
#include "../FileLoader/MonsterLoader.h"
#include "MessageStruct/CharBase/CharAttribute.pb.h"
#include <math.h>
#include "CharDefine.h"
#include "../ObjectFactory.h"
#include "../../Skill/SkillLoader/SkillLoader.h"
#include "../../Skill/SkillBase.h"
#include "../../Skill/SkillFactory.h"
#include "../../Skill/BuffBase.h"
#include "../../Skill/BuffFactory.h"
#include "../ItemDrop/DropItemMgr.h"
#include "../../Achievement/Achievement.h"
#include "../../Achievement/AchievementLoader.h"
#include "../../Mail/MailMessage.h"
#include "../../EscortEx/EscortMgr.h"
#include "../../EscortEx/FileLoader/EscortFileLoader.h"
#include "../../Chat/ChatMgr.h"
#include "MessageCommonRet.h"
#include "../../ServerEventDefine.h"
#include "../AI/AIBrain.h"
#include "./FileLoader/InternationalLoader.h"
#include "../../Attribute/AttributeMonster.h"
#include "../../WorldBoss/WorldBoss.h"
#include "../../Skill/SkillMessage.h"

Monster::Monster(eObjType type):CreatureObj(type),m_bornTime(0),m_quality(0),m_flushTime(0),m_aliveTime(0),m_runSpeed(0),
m_stayTime(0),m_phyAtkTime(0),m_originalX(0),m_originalY(0),m_escortID(0),
m_nGatherTime(0),m_nGatherLastTime(0)
{
	m_fallID.clear();
	m_brain = new AIBrain();
	m_AttManage = new AttributeMonster();
}

Monster::~Monster()
{
	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();
	for(; it!=m_buffList.end();)
	{
		BuffBase * tmpBuff = it->second;
		m_buffList.erase(it++);
		if(!tmpBuff->IsEnd())
		{
			tmpBuff->End();
		}
		else
		{
			BuffFactory::Destory(tmpBuff);
		}
	}

	m_moveSector.clear();
}

void Monster::Release()
{
	m_brain->Release();

	m_bornTime = 0;
	m_quality = 0;
	m_flushTime = 0;
	m_aliveTime = 0;
	m_runSpeed = 0;
	m_stayTime = 0;
	m_phyAtkTime = 0;
	m_originalX = 0;
	m_originalY = 0;
	m_escortID = 0;
	m_nGatherTime = 0;
	m_nGatherLastTime = 0;

	Smart_Ptr<Player> owner;
	m_nOwner = owner;

	Smart_Ptr<CreatureObj> targetObj;
	m_targetObj = targetObj;

	m_moveSector.clear();
	m_competeData.clear();
	m_skillList.clear();

	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();
	for(; it!=m_buffList.end();)
	{
		BuffBase * tmpBuff = it->second;
		m_buffList.erase(it++);
		if(!tmpBuff->IsEnd())
		{
			tmpBuff->End();
		}
		else
		{
			BuffFactory::Destory(tmpBuff);
		}
	}

	CreatureObj::Release();

}

bool Monster::GetEnemyList(vector<Smart_Ptr<CreatureObj> > & enemyList)
{
	CSceneMgr::GetInstance()->GetObject(GetMapID(), GetXNewPos(), GetYNewPos(), m_brain->GetViewDistance(), getAttManage()->getValue<DWORD>(eCharCountry), eMonster,enemyList);
	return true;
}

void Monster::SetOwnerToBrain(Smart_Ptr<Monster> &obj)
{
	m_myself = obj;
	m_brain->SetOwner(obj);
}

bool Monster::IsActiveMonster()
{
	int ret = m_brain->GetAttackType();
	if(ret == eErrType)
	{
		LOG_ERROR(FILEINFO, "monster[id=%d] is not existed ai", GetID());

		return false;
	}
	else if(ret == ePassiveAtk || ret == eDummyType)
	{
		return false;
	}

	return true;
}

bool Monster::IsDummyMonster()
{
	int ret = m_brain->GetAttackType();
	if(ret == eErrType)
	{
		LOG_ERROR(FILEINFO, "monster[id=%d] is not existed ai", GetID());

		return false;
	}
	else if(ret != eDummyType)
	{
		return false;
	}

	return true;
}


void Monster::SetTarget(Smart_Ptr<CreatureObj> &obj)
{
	Smart_Ptr<Player> player;

//	if((bool)m_targetObj)
	{
		if((bool)m_targetObj && m_targetObj->GetType() == ePlayer)
		{
			player = m_targetObj;
			player->DeleteMonster(GetKey());
		}
	}

	m_targetObj = obj;

	if((bool)obj && obj->GetType() == ePlayer)
	{
		player = obj;
		player->AddMonster(m_myself);
	}
}

void Monster::UpdateEnemyList(Smart_Ptr<Player> &player)
{
	Smart_Ptr<CreatureObj> obj = player;

	m_brain->DeleteTargetEnemy(obj);
	if(m_targetObj && m_targetObj->GetKey() == player->GetKey())
	{
		m_targetObj = 0;
	}
}

void Monster::ClearEnemyList()
{
	m_brain->ClearEnemy();
}

void Monster::UpdateEnemyValue(int value, DWORD key)
{
	m_brain->UpdateEnemyValue(value, key);
}

void Monster::SynchPath(PlayerSynch::MonsterPathToPlayer &path)
{
	if(GetGameZone() && GetSector() && m_myself)
		GetGameZone()->SendSynchPath(path, m_myself);
}

void Monster::Update(int dwTick)
{
	m_brain->Update(dwTick);
}

int  Monster::GetNextPosLen(int startx , int starty, int posx, int posy)
{
	int sectorSize = CMapConfig::GetInstance()->GetSectorSize(GET_MAP_ID(GetMapID()));

	if(posx == startx && posy == starty)
	{
		return 0;
	}

	int len = 0;

	if( ( ( posx == startx ) && ( ( posy == starty - 1 ) || ( posy == starty + 1 ) ) ) ||
		( ( posy == starty ) && ( ( posx == startx - 1) || ( posx == startx + 1) ) )	)
	{
		len = sectorSize;
	}
	else if( ( ( posx == startx - 1) && ( ( posy == starty + 1 ) || ( posy == starty - 1 ) ) ) ||
		( ( posx == starty + 1) && ( ( posy == starty + 1 ) || ( posy == starty - 1 ) ) ) )
	{
		len = (int)(sqrt( 2 ) * sectorSize);
	}

	if(len == 0)
	{
		int xlen = abs(posx - startx);
		int ylen = abs(posy - starty);

		len = sectorSize * ((int)(sqrt(xlen * xlen + ylen * ylen)));
	}

	return len;
}

void Monster::RelocatePos(int xpos, int ypos, bool isSyn)
{
	Smart_Ptr<Object> obj = m_myself;

//	SetXPos(xpos);
//	SetYPos(ypos);

	int ret = m_brain->GetAttackType();
	if(ret == eErrType)
	{
		LOG_ERROR(FILEINFO, "monster[id=%d] is not existed ai", GetID());

		return ;
	}
	else if(ret == eDummyType)
	{
		return ;
	}

	if(!obj)
		return;

	m_brain->ClearPathAndTime();
	SetXNewPos(xpos);
	SetYNewPos(ypos);
	CSceneMgr::GetInstance()->MoveTo(obj);
}

int Monster::GetAIState()
{
	return m_brain->GetCurState();
}

Smart_Ptr<AIBrain> & Monster::GetBrain()
{
	return m_brain;
}

int64 Monster::GetTypeValue(int type)
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
			value = GetYNewPos();
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
			value = getAttManage()->getValue<int>(eCharHP);
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
			value = m_runSpeed;
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
	case ePosition:
		{
			value = (GetXNewPos() << 16) | GetYNewPos();
		}
	}

	return value;
}

void Monster::SetSynchData(PlayerSynch::SynchCompeteMonster * info)
{
	info->set_id(GetID());
	info->set_key(GetKey());
	info->set_hp(getAttManage()->getValue<int>(eCharHP));
	info->set_mp(getAttManage()->getValue<int>(eCharMP));
	info->set_xpos(GetXNewPos());
	info->set_ypos(GetYNewPos());
//	info->set_country(getAttManage()->getValue<DWORD>(eCharCountry));
	info->set_hpmax(getAttManage()->getValue<int>(eCharHPMax));
	info->set_mpmax(getAttManage()->getValue<int>(eCharMPMax));
	info->set_name(GetName());
	info->set_profession(m_competeData[eCompeteProfession]);
	info->set_movespeed(getAttManage()->getValue<int>(eCharMoveSpeed));
	info->set_sex(m_competeData[eCompeteSex]);
	info->set_lv(getAttManage()->getValue<int>(eCharLv));
	info->set_titleid(m_competeData[eCompeteTitleID]);
	info->set_magicweapon(m_competeData[eCompeteMagic]);
	info->set_clanname(m_clanName);
	info->set_viplv(m_competeData[eCompeteVipLv]);
	info->mutable_equip()->set_id(m_competeData[eCompeteWeapon]);
	info->mutable_equip()->set_strenggrade(m_competeData[eCompeteEquipStreng]);
	info->set_costume(m_competeData[eCompeteAGuard]);
	info->set_fashion(m_competeData[eCompeteFashion]);
	info->set_horseid(m_competeData[eCompeteHorse]);
	info->set_clanjob(m_competeData[eCompeteClanJob]);

	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();
	for(; it!=m_buffList.end(); ++it)
	{
		const BuffInfo* binfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(it->second->GetID());
		if(binfo == NULL)
		{
			LOG_ERROR(FILEINFO, "add buff but buff is null");

			continue;
		}

		if(binfo->isResult > 0)
		{
			PlayerSynch::SynchBuffChar * buf = info->add_buf();

			buf->set_id(it->second->GetID());
			buf->set_onlyid(it->second->GetOnlyID());
		}
	}
}

void Monster::SetSynchData(PlayerSynch::SynchMonsterBase * info)
{
	info->set_id(GetID());
	info->set_key(GetKey());
	info->set_hp(getAttManage()->getValue<int>(eCharHP));
	info->set_mp(getAttManage()->getValue<int>(eCharMP));
	info->set_xpos(GetXNewPos());
	info->set_ypos(GetYNewPos());
	info->set_country(getAttManage()->getValue<DWORD>(eCharCountry));
	info->set_hpmax(getAttManage()->getValue<int>(eCharHPMax));
	info->set_mpmax(getAttManage()->getValue<int>(eCharMPMax));
	info->set_name(GetOwnerName());

	if(m_quality == eEscortCar)
	{
		if(m_brain->GetCurState() == eAIStateType_Attack || m_brain->GetCurState() == eAIStateType_Return)
		{
			info->set_movespeed(GetRunSpeed());
		}
		else
		{
			info->set_movespeed(getAttManage()->getValue<int>(eCharMoveSpeed));
		}

		if(m_nOwner && m_nOwner->GetEscortMgr()->GetRobTimes() >= 5)
		{
			info->set_state(eInvincible);
		}

		m_brain->SetPath(info);
	}

	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();
	for(; it!=m_buffList.end(); ++it)
	{
		const BuffInfo* binfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(it->second->GetID());
		if(binfo == NULL)
		{
			LOG_ERROR(FILEINFO, "add buff but buff is null");

			continue;
		}

		if(binfo->isResult > 0)
		{
			PlayerSynch::SynchBuffChar * buf = info->add_buf();

			buf->set_id(it->second->GetID());
			buf->set_onlyid(it->second->GetOnlyID());
		}
	}
}

bool Monster::IsAttackInLength(int distance)
{
	//printf("��������������������������������������꣨%d, %d��     ��ɫ��꣨%d, %d��",GetXNewPos(), GetYNewPos(), m_targetObj->GetXNewPos(), m_targetObj->GetYNewPos());
	int xLen = abs(GetXNewPos()- m_targetObj->GetXNewPos());
	int yLen = abs(GetYNewPos()- m_targetObj->GetYNewPos());
	double len = sqrt(xLen * xLen + yLen * yLen);

	if(len > distance)
	{
		return false;
	}

	return true;
}

bool Monster::IsViewInLength(int xpos, int ypos)
{
	int xLen = abs(GetXNewPos()- xpos);
	int yLen = abs(GetYNewPos()- ypos);
	double len = sqrt(xLen * xLen + yLen * yLen);
	int distance = m_brain->GetViewDistance();

	if(len > distance)
	{
		return false;
	}

	return true;
}

bool Monster::IsInRunDistance( int distance )
{
	int txpos = abs( m_originalX - GetXNewPos() );
	int typos = abs( m_originalY - GetYNewPos() );
	int tDistance = (int)(sqrt( txpos * txpos + typos * typos ));

	if( tDistance > distance )
		return false;

	return true;
}

bool Monster::IsInSelectView(int xpos, int ypos)
{
	int txpos = abs(m_originalX - xpos);
	int typos = abs(m_originalY - ypos);
	int tDistance = (int)(sqrt( txpos * txpos + typos * typos ));
	int distance = m_brain->GetRunDistance();
	if(tDistance >= distance)
		return false;

	return true;
}

void Monster::Attacked(Smart_Ptr<CreatureObj> &obj)
{
	if(m_brain->GetCurState() == eAIStateType_Return)
		return;

	if(obj->GetType() == ePet)
		return;

	if(m_quality == eEscortCar && obj->GetType() == ePlayer && m_myself && m_myself->GetOwner())
	{
		MonsterBeAttackArgs tArgsf(m_myself, obj);
		m_myself->GetOwner()->FireEvent(MONSTER_BEATTACK, tArgsf);
	}

	//@todo : 暂时将表车和稻草人没有仇恨列表，后面该
	if(m_quality != eEscortCar && !IsDummyMonster())
	{
		if(obj->GetType() == ePet)
		{
			Smart_Ptr<GirlWar> pet = obj;
			Smart_Ptr<Player> person = pet->GetOwner();
			Smart_Ptr<CreatureObj> ref = person;
			m_brain->AddEnemy(ref);
		}
		else
		{
			m_brain->AddEnemy(obj);
		}
	}

	if(m_quality != eEscortCar && !m_targetObj)
	{
		SetTarget(obj);
	}

	if(m_quality == eEscortCar && m_nOwner)
	{
		m_nOwner->SetDataFlag(eEscortInfo);
	}

	if((m_brain->GetCurState() == eAIStateType_Idle) && !IsDummyMonster() && (m_quality != eEscortCar))
	{
		if(m_quality == eGuardMonster)
		{
			if(obj->GetType() == ePlayer)
			{
				Smart_Ptr<Player> player = obj;
				int curHp = this->getAttManage()->getValue<int>(eCharHP);
				int maxHp = this->getAttManage()->getValue<int>(eCharHPMax);
				//卫士被非红名玩家打到一半血才开始反击或被红名玩家攻击
				if(player->GetSinState() == ERS_RED_NAME || curHp * 2 <= maxHp)
				{
					m_brain->ChangeState(eAIStateType_Attack);
				}
			}
		}
		else
		{
			m_brain->ChangeState(eAIStateType_Attack);
		}
	}
}

bool Monster::IsDead()
{
	if(GetAIState() == eAIStateType_Dead)
	{
		return true;
	}

	return false;
}

//处理死亡归属
void Monster::dealWithDeadBelong(Smart_Ptr<CreatureObj> maxWeight)
{
	if(!maxWeight)
	{
		return ;
	}

	vector<Smart_Ptr<CreatureObj> > tempOnwerList;
	//取队伍玩家
	if(maxWeight->GetType() == ePlayer && maxWeight->GetGroup() != 0)
	{
		Smart_Ptr<GroupInfo> tempGroup;
		NewGroupMgr::GetInstance()->GetGroupPtr(maxWeight->GetGroup(),tempGroup);
		if(tempGroup)
		{
			list<GroupMember *>::iterator tempIt = tempGroup->memberList.begin();
			for(;tempIt != tempGroup->memberList.end();++tempIt)
			{
				Smart_Ptr<Player> tempPlayer;
				PlayerMessage::GetInstance()->GetPlayerByCharid((*tempIt)->memberID,tempPlayer);

				if(!tempPlayer)
				{
					continue ;
				}

				if(maxWeight.Get() == tempPlayer.Get())
				{
					continue ;
				}

				tempOnwerList.push_back(tempPlayer);

				MonsterBeKillBelongArgs tempEnemyArgs(m_myself,tempPlayer,eMonsterDeaBelongOnwer);

				tempPlayer->FireEvent(MONSTER_KILLED_EVENT_BELONG,tempEnemyArgs);
			}
		}
	}

	MonsterBeKillBelongArgs tempMaxEnemyArgs(m_myself,maxWeight,eMonsterDeaBelongOnwer);

	maxWeight->FireEvent(MONSTER_KILLED_EVENT_BELONG,tempMaxEnemyArgs);

	tempOnwerList.push_back(maxWeight);

	//从仇恨链中去掉队伍中的
	vector<Smart_Ptr<CreatureObj> > tempEnemyList = m_brain->getEnemyDataList();

	for(size_t i = 0;i < tempOnwerList.size();++i)
	{
		for(size_t j = 0;j < tempEnemyList.size();++j)
		{
			if(tempEnemyList[j].Get() != tempOnwerList[i].Get())
			{
				continue ;
			}

			tempEnemyList.erase(tempEnemyList.begin() + j);

			break;
		}
	}

	for(size_t i = 0;i < tempEnemyList.size();++i)
	{
		MonsterBeKillBelongArgs tempEnemyArgs(m_myself,tempEnemyList[i],eMonsterDeaBelongOnece);

		tempEnemyList[i]->FireEvent(MONSTER_KILLED_EVENT_BELONG,tempEnemyArgs);
	}

	return ;
}

void Monster::Dead(Smart_Ptr<CreatureObj> &obj)
{
	if(IsDead())
	{
		return ;
	}

	if(m_myself.Get() == NULL)
	{
		return;
	}

	Smart_Ptr<CreatureObj> creature = NULL;
	Smart_Ptr<Object> monster = m_myself;
	Smart_Ptr<CreatureObj> maxWeight = NULL;
	Smart_Ptr<GirlWar> girlWar = NULL;

	//人物剧情刷新的怪只属于接受此任务的玩家
	if(m_quality == eNpcChangeToMonster && m_nOwner)
	{
		maxWeight = m_nOwner;
		if(!obj)
			obj = m_nOwner;
	}
	else if(m_quality == eCountryBossMonster && obj)
	{
		maxWeight = obj;
	}
	else
	{
		if(m_quality == eEscortCar && !obj)
		{
			//镖车不是被打死的，那么就是下线或者传送造成的，就不触发事件
		}
		else if(obj && obj->GetType() == ePet)
		{
			girlWar = obj;
			if(girlWar->GetOwner())
			{
				Smart_Ptr<CreatureObj> cObj = girlWar->GetOwner();
				maxWeight = m_brain->GetMaxWeight(cObj);
			}
		}
		else
		{
			maxWeight = m_brain->GetMaxWeight(obj);
		}
	}

	//怪物死亡玩家权重没有找到时，将obj
	if(!maxWeight &&
		m_quality != eEscortCar &&
		obj &&
		(obj->GetType() == ePlayer || obj->GetType() == ePet))
	{
		maxWeight = obj;
	}

	if(maxWeight && maxWeight->GetType() == ePet)
	{
		Smart_Ptr<GirlWar>  girlwar;

		girlwar = maxWeight;

		maxWeight  = girlwar->GetOwner();
	}

	//进入死亡状态机
	m_brain->ChangeState(eAIStateType_Dead);

	if(maxWeight)
	{
		//杀怪事件
		TwoParmaEventArgs tArgs(m_myself, maxWeight);
		FireGlobalEvent(MONSTER_KILLED_EVENT_TYPE, tArgs);

		if(maxWeight->GetType() == ePlayer)
			maxWeight->FireEvent(MONSTER_KILLED_EVENT_TYPE, tArgs);

		if(m_quality == eEscortCar && m_myself && m_myself->GetOwner() && m_myself->GetOwner()->GetType() == ePlayer)
			m_myself->GetOwner()->FireEvent(MONSTER_KILLED_EVENT_TYPE, tArgs);
	}

	dealWithDeadBelong(maxWeight);

	//采集怪只要移出场景就好
	if(m_quality != eGatherMonster && m_quality != eElementMonster && m_quality != ePVPGatherMonster && m_quality != eChallengeBox)
	{
		if((bool)maxWeight)
		{
			if(maxWeight->GetType() == ePlayer)
			{
				if(GetQuality() == eWorldMonster)
				{
					KillWorldMonsterDead();
				}
			}
		}
		else
		{
			LOG_WARNING(FILEINFO, "monster dead but maxweight is null");
		}

		SetTarget(creature);
	}

	//清除伤害及仇恨
	GetBrain()->ClearEnemy();

	//将怪物移出场景
	int mapType = GET_MAP_TYPE(GetMapID());
	if(m_quality == eNpcChangeToMonster || m_quality == eEscortCar)
	{
		Smart_Ptr<Player> temp;
		if(monster)
		{
			SetOwner(temp);
			CSceneMgr::GetInstance()->LeaveScene(monster, true,true);
		}
	}
	else if(m_quality == eBoxMonster || m_quality == eChallengeBox)
	{
		CSceneMgr::GetInstance()->LeaveScene(monster, true,true);
	}
	else if(mapType >= eCopyMap)	//副本里面的怪直接删除
	{
		if(monster)
		{
			InstanceMapEntry * entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(monster->GetMapID()));
			if(entry)
			{
				switch(entry->m_eType)
				{
				case eClanBattle:
				case eCountryWar:
				case ePVP:
					{//帮派站副本也是不删除
						CSceneMgr::GetInstance()->LeaveScene(monster, false);
						break;
					}
				case eTreasonKing:
					{
						if(m_quality == eCommonMonster)
						{
							CSceneMgr::GetInstance()->LeaveScene(monster, true,true);
						}
						else
						{
							CSceneMgr::GetInstance()->LeaveScene(monster, false);
						}
						break;
					}
				case eMoneyInstance:
				{
						CSceneMgr::GetInstance()->LeaveScene(monster, false);
						break;
				}
				default:
					{
						CSceneMgr::GetInstance()->LeaveScene(monster, true,true);
					}
				}
			}
		}
	}
	else
	{
		if(monster)
		{
			CSceneMgr::GetInstance()->LeaveScene(monster, false);
		}
	}

	CreatureKilledEventArgs tArgs(obj);
	FireEvent(CREATURE_DEAD_EVENT_TYPE, tArgs);
	RemoveCanBuff();
}


void Monster::KillWorldMonsterDead()
{
	vector<pair<int64, int> > DamageVec;
	int ranking = 1;

	//世界BOSS额外奖励
	m_brain->GetDamageListByNum(DamageVec, 50);

	for(uint i = 0; i < DamageVec.size(); i++)
	{
		pair<int64, int> &damagePair = DamageVec[i];
		Smart_Ptr<Player> sendPlayer;
		PlayerMessage::GetInstance()->GetPlayerByCharid(damagePair.first, sendPlayer);
		if(!sendPlayer)
		{
			continue;
		}

		sendPlayer->GetVitalityMgr()->FinshVitality(eViBoss);

//		if (1 > sendPlayer->GetVitalityMgr()->GetVitality(eViBoss))
//		{ //
//			sendPlayer->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), 1, false);
//		}

//		int addWorldBossExp = (sendPlayer->getAttManage()->getValue<int>(eCharLv) * (101-ranking) * (50/(ranking/10+1))) * getAttManage()->getValue<int>(eCharLv) / 100;
//		int addWorldBossMoney = (sendPlayer->getAttManage()->getValue<int>(eCharLv) * (101-ranking) * (12/(ranking/10+1))) * getAttManage()->getValue<int>(eCharLv) / 100;
//		sendPlayer->AddExp(addWorldBossExp, sendPlayer->GetExpRation());
//		sendPlayer->ChangeMoney(addWorldBossMoney, true,eStatic_Money,eStaMinor_Money_WorldBoss);

//		if(addWorldBossMoney > 0)
//		{
//			StatisticInfo::CostAndGotInfo costAndGotInfo;
//			StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,addWorldBossMoney);
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(sendPlayer->GetID(),eStatic_Money,eStaMinor_Money_WorldBoss,&costAndGotInfo);
//		}

//		vector<int> vecType;
//		vecType.push_back(eCharMoney);
//		sendPlayer->SynCharAttribute(vecType);

		//邮件通知
//		vector<int64> goods;
//		stringstream expStr;
//		expStr << addWorldBossExp;
//		stringstream moneyStr;
//		moneyStr << addWorldBossMoney;
//		string warning = InternationalLoader::GetInstance()->GetInterStr(12);
//		string warning1 = InternationalLoader::GetInstance()->GetInterStr(13);
//		string warning2 = InternationalLoader::GetInstance()->GetInterStr(14);
//		string substance = warning.c_str() + expStr.str() + warning1.c_str() + moneyStr.str() + warning2.c_str();
//		MailMessageMgr::GetInstance()->SendSysMail(goods, sendPlayer->GetName(), 0, substance);
		ranking++;
	}

	WorldBossMgr::GetInstance()->UpdateWorldBossUIData(GetID(), false);

	PlayerMessage::GetInstance()->UpdateWorldBoss(GetID());
}

void Monster::SetDead(Smart_Ptr<CreatureObj> killer)
{
	Smart_Ptr<CreatureObj> obj;
	CharBase::SynchObjectAttr attr;
	CharBase::CharAttChange * change = attr.mutable_attr();

	attr.set_key(GetKey());
	change->add_type(eSimpleState);
	change->add_value(0);
	change->add_state(eAttackDestroy);

	if(attr.has_attr())
	{
		CSceneMgr::GetInstance()->SendSynchAttrChange(attr, GetMapID(), GetXNewPos(), GetYNewPos());
	}

	if(killer && (m_quality == eGatherMonster || m_quality == ePVPGatherMonster || m_quality == eTowerNormalMonster ||
			m_quality == eTowerBossMonster || m_quality == eChallengeBox))
	{
		obj = killer;
	}

	Dead(obj);
}

bool Monster::IsAliveTime()
{
	if(m_aliveTime == 0 || m_brain->GetCurState() == eAIStateType_Dead)
	{
		return true;
	}

	int disTime = (CUtil::GetNowSecond() - m_bornTime) / 1000;
	if(disTime > m_aliveTime)
	{
		return false;
	}

	return true;
}

void Monster::BeginIdle()
{
		RemoveCanBuff();
}

void Monster::Reflush()
{
	SetCurHP(getAttManage()->getValue<int>(eCharHPMax));
	SetCurMP(getAttManage()->getValue<int>(eCharHPMax));
	SetBornTime();
	if(m_quality == eEscortCar)
	{
		SetXPos(GetXNewPos());
		SetYPos(GetYNewPos());
	}
	else
	{
		SetXPos(GetOriginalX());
		SetYPos(GetOriginalY());
	}

	SetXNewPos();
	SetYNewPos();

	Smart_Ptr<Object>  obj = m_myself;

	if(!CSceneMgr::GetInstance()->EnterScene(obj))
	{
		LOG_ERROR(FILEINFO, "monster[monsterid=%d] when flushing cannot enter scene", GetID());
	}

	MonsterReflushArgs tempArgs(m_myself);
	FireGlobalEvent(MONSTER_REFLUSH,tempArgs);

	m_brain->ChangeState(eAIStateType_Idle);
}

void Monster::Attack(int skillid)
{
//	if(m_curAtkTime < m_phyAtkTime)
//	{
//		m_curAtkTime += dwTick;
//
//		return;
//	}
//
//	m_curAtkTime = 0;
	//printf("\n__________________________ %d ____________________________\n", m_curAtkTime);

	if(!m_targetObj || m_targetObj->IsDead())
	{
		Smart_Ptr<CreatureObj> creature;

		SetTarget(creature);
		m_brain->ChangeState(eAIStateType_Return);

		return;
	}

	Smart_Ptr<CreatureObj> myself = m_myself;
	SkillInfo *info = 0;
	Smart_Ptr<CreatureObj> targetObj;
	int xpos = -1;
	int ypos = -1;

	info = SkillLoader::GetInstance()->GetSkillInfoByID(skillid);
	if(info == 0)
	{
		return ;
	}

	switch(info->rangeType)
	{
	case eRangeSingle://单个
		{
			targetObj = m_targetObj;

			break;
		}
	case eRangeMyself://自身范围
		{
			xpos = GetXNewPos();
			ypos = GetYNewPos();

			break;
		}
	case eRangePoint://点选范围
	case eRangeFan://扇形
	case eRangeLine:
	case eRectangle:
		{
			xpos = m_targetObj->GetXNewPos();
			ypos = m_targetObj->GetYNewPos();

			break;
		}
	default:
		return;
	}
	int key = 0;

	if((bool)targetObj)
	{
		key = targetObj->GetKey();
	}

	CSkillBase* pSkillBase = CSkillFactory::Create(skillid, myself, key, xpos, ypos);
	if (pSkillBase == NULL)
	{
		return ;
	}

	// 开始执行技能
	if(!pSkillBase->BeginAction())
	{
		CSkillFactory::FreeSkill(pSkillBase);
	}
}

bool Monster::AddBuff(int id, Smart_Ptr<CreatureObj> &obj, int num)
{
	if(!m_myself)
	{
		return false;
	}

	if(IsDead())
	{
		return false;
	}	

	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(id);
	if(info == NULL)
	{
		//LOG_ERROR(FILEINFO, "add buff[%d] but buff is null", id);

		return false;
	}

	if(!SkillMessage::GetInstance()->isCanAddBuff(*info,m_myself,obj))
	{
		return false;
	}

	DWORD removeID = 0;
	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();
	for(; it!=m_buffList.end(); ++it)
	{
		const BuffInfo* tInfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(it->second->GetID());
		if(tInfo == NULL)
		{
			LOG_ERROR(FILEINFO, "monster exist null buff");

			continue;
		}

		if(info->groupID == tInfo->groupID)
		{
			if(info->lv > tInfo->lv)
			{//顶替掉老的
				removeID = it->first;
				break;
			}
			else
			{
				return false;
			}
		}
	}

	if(removeID > 0)
	{
		DeleteBuffByOnlyID(removeID);
	}

	Smart_Ptr<CreatureObj> owner = m_myself;
	BuffBase* pBuffBase = BuffFactory::Create(owner, obj, id);
	if (pBuffBase == 0)
		return false;

	// 执行成功，则添加到怪物身上BUFF链当中
	m_buffList[pBuffBase->GetOnlyID()] = pBuffBase;

	if (pBuffBase->Start() == false)
	{
		// 添加失败
		pBuffBase->End();

		return false;
	}

	return true;
}

void Monster::RemoveCanBuff()
{
	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();
	for(; it!=m_buffList.end(); )
	{
		BuffBase * tmpBuff = it->second;
		m_buffList.erase(it++);
		if(!tmpBuff->IsEnd())
		{
			tmpBuff->End();
		}
		else
		{
			BuffFactory::Destory(tmpBuff);
		}
	}
}

bool Monster::RemoveBuffByOnlyID(DWORD id)
{
	map<DWORD, BuffBase *>::iterator it = m_buffList.find(id);
	if(it == m_buffList.end())
	{
		return false;
	}

	m_buffList.erase(it);

	return true;
}

bool Monster::DeleteBuffByOnlyID(DWORD id)
{
	map<DWORD, BuffBase *>::iterator it = m_buffList.find(id);
	if(it == m_buffList.end())
	{
		return false;
	}

	BuffBase * tmpBuff = it->second;
	m_buffList.erase(it);
	if(!tmpBuff->IsEnd())
	{
		tmpBuff->End();
	}
	else
	{
		BuffFactory::Destory(tmpBuff);
	}

	return true;
}

void Monster::removeBuffByGroupID(DWORD groupID)
{
	map<DWORD, BuffBase *> tBuffLIst 	= m_buffList;
	map<DWORD, BuffBase *>::iterator it = tBuffLIst.begin();
	for(; it != tBuffLIst.end(); ++it)
	{
		const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(it->second->GetID());
		if(info == NULL)
		{
			continue ;
		}

		if(info->groupID != (int)groupID)
		{
			continue ;
		}

		DeleteBuffByOnlyID(it->second->GetOnlyID());
	}

}

int Monster::GetNextPosition(int &xpos, int &ypos)
{
	if(m_moveSector.size() > 0)
	{
		switch(m_brain->GetMonsterType())
		{
		case eRandomWayPos:
			{
				int index = rand() % m_moveSector.size();

				DWORD pos = m_moveSector[index]->GetPosition();

				xpos = pos >> 16;
				ypos = pos & 0xffff;

				break;
			}
		case ePointWayPos:
			{
				DWORD pos = m_moveSector[1]->GetPosition();

				xpos = pos >> 16;
				ypos = pos & 0xffff;

				if(xpos == GetXNewPos() && ypos == GetYNewPos())
				{
					pos = m_moveSector[0]->GetPosition();

					xpos = pos >> 16;
					ypos = pos & 0xffff;
				}

				break;
			}
		case eSingleWayPos:
			{
				DWORD pos = 0;
				int txpos = 0;
				int typos = 0;
				int moveSector_size = (int)m_moveSector.size();

				for(int i=0; i<moveSector_size; ++i)
				{
					pos = m_moveSector[i]->GetPosition();

					txpos = pos >> 16;
					typos = pos & 0xffff;

					if(txpos == m_originalX && typos == m_originalY)
					{
						if(GetXNewPos() == m_originalX && GetYNewPos() == m_originalY)
						{
							if(i == moveSector_size - 1)
							{
								InstanceType iType = (InstanceType)InstanceLoader::GetInstance()->GetEntryType(GET_MAP_ID(GetMapID()));
								if(iType == eMoneyInstance)
								{
									Smart_Ptr<Monster> temp = m_myself;
									temp->SetDead();
									return -1;
								}
							}

							if(i <= (moveSector_size - 2))
							{
								pos = m_moveSector[i + 1]->GetPosition();

								txpos = pos >> 16;
								typos = pos & 0xffff;

								xpos = m_originalX;
								ypos = m_originalY;
								m_originalX = txpos;
								m_originalY = typos;

								break;
							}
						}
						else
						{
							xpos = m_originalX;
							ypos = m_originalY;

							break;
						}
					}
				}

				break;
			}
		}
	}
	return 0;
}

bool Monster::StartGather(Smart_Ptr<Player> player)
{
	if(!player)
		return false;

	if(!player->IsInCreatureState(ERS_Gather) && !player->IsInCreatureState(ERS_Fight)  && m_quality == eGatherMonster)
	{
		m_nGatherLastTime = m_nGatherTime;
		player->SetCreatureState(ERS_Gather);
		return true;
	}

	return false;
}

bool Monster::EndGather(Smart_Ptr<Player> &player, int &error)
{
	if(!player)
	{
		error = eNull;
		return false;
	}

//客户端要求写成直接发送采集，即采集完成,不需要服务端做计时和判断
	if(!IsDead())
	{
		MonsterInfo info;
		MonsterLoader::GetInstance()->GetMonsterInfo(GetID(), info);

		Smart_Ptr<ArticleBaseMgr> pack = player->GetContainer(ePackageType);
		if(pack)
		{
			//判断背包是否已满
			if(pack->IsBagEnought(info.fallID[0],1))
			{
				error = ePackageFull;
				return false;
			}

			if(info.gatherTaskId == 0 &&  info.fallID.size() == 1)
			{
				const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(info.fallID[0]);
				if(ginfo == NULL)
				{
					error = eItemError;
					return false;
				}

				vector<int> tempPosList;
				if(pack->AddItem(info.fallID[0],1,tempPosList,ginfo->bindTyle))
				{
					return false;
				}

				Smart_Ptr<CreatureObj> killer = player;
				SetDead(killer);

				return true;
			}
		}

		if(player->GetQuestMgr())
		{
			DWORD questid = 0;
			for(int i = QuestType_Main;i <= QuestType_Week;++i)
			{
				questid = player->GetQuestMgr()->getAcceptedQuests().findQuestByType((QuestType)i);
				Quest* task = player->GetQuestMgr()->getAcceptedQuests().findQuest(questid);
				if(task == NULL)
				{
					continue;
				}
				const Entry_Quest* tQuestData = task->getEntry();
				if(tQuestData == NULL)
				{
					continue;
				}
				if(tQuestData->m_nMinorType != QuestCondType_Mining)
				{
					continue;
				}
				if(info.id == (int)tQuestData->m_nTaskTarget && task->getQuestState() == QuestState_Accepted)
				{
					Smart_Ptr<CreatureObj> killer = player;

					SetDead(killer);

					return true;
				}
			}

			error = eQuestNoAccepted;
		}
		return false;
	}
	else
		return false;
}

void Monster::SetOwner(Smart_Ptr<Player> player)
{
	if((m_quality == eNpcChangeToMonster || m_quality == eEscortCar))
		m_nOwner = player;

	if(m_quality == eNpcChangeToMonster && player)
	{
		player->AddTaskMonster(m_myself);
	}
}

Smart_Ptr<Player> Monster::GetOwner()
{
	if(m_quality == eNpcChangeToMonster || m_quality == eEscortCar)
	{
		return m_nOwner;
	}

	Smart_Ptr<Player> temp;
	return temp;
}

void Monster::GetEnemyValue(map<int64, int>& enemyValue)
{
	if(m_brain)
	{
		m_brain->GetEnemyValue(enemyValue);
	}
}

void Monster::ClearEnemyValue()
{
	if(m_brain)
	{
		m_brain->ClearEnemyValue();
	}
}

void Monster::GetPerfectLocation(int moveSpeed, int xpos, int ypos, int &txpos, int &typos)
{
	if(!m_brain->GetPerfectLocation(moveSpeed, xpos, ypos, txpos, typos))
	{
		txpos = GetXNewPos();
		typos = GetYNewPos();
	}
}

void Monster::ClearPathAndTime()
{
	if(m_brain)
	{
		m_brain->ClearPathAndTime();
	}
}

bool Monster::IsBetterAI()
{
	if(m_brain)
		return m_brain->IsBetterAI();

	return false;
}
