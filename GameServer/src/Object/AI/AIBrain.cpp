/*
 * AIBrain.cpp
 *
 *  Created on: 2013��10��23��
 *      Author: helh
 */

#include "AIBrain.h"
#include "AIState.h"
#include "OtherDefine.h"
#include "Log/Logger.h"
#include "AIStateIdle.h"
#include "AIStateAttack.h"
#include "AIStateReturn.h"
#include "AIStateDead.h"
#include "AIStateArena.h"
#include "../FileLoader/MonsterLoader.h"
#include "../Monster/Monster.h"
#include <math.h>
#include "../FileLoader/DropItemLoader.h"
#include "../../Map/SceneManager.h"
#include "../../Map/FindPathMgr.h"
#include "../../Map/ConfigLoader/MapConfig.h"
#include "../../Map/ConfigLoader/FieldInfo.h"
#include "../../EscortEx/FileLoader/EscortFileLoader.h"
#include "../FileLoader/NPCLoader.h"
#include "../../EscortEx/EscortMgr.h"
#include "../../WorldBoss/WorldBoss.h"
#include "FileLoader/GameParaLoader.h"
#include "../../Attribute/AttributeCreature.h"
#include "../Player/PlayerMessage.h"
#include "../../ServerEventDefine.h"

#define ADJUST_MONSTER_TIME 10000			//10秒

AIBrain::AIBrain():
m_time(AI_CYCLE_TIME),
m_curState(eAIStateType_Idle),
m_clearEnemyTime(300000),
m_checkEnemyTime(0),
m_first(0),
m_isPath(false),
m_monsterType(eRandomWayPos),
m_pathTime(0),
m_curIndex(-1),
m_offset(0),
m_curStay(0),
m_AjustTime(0),
m_baseAI(0),
m_betterAI(0)
{
}

AIBrain::~AIBrain()
{
	for(int i=0; i<eAIStateType_Max; ++i)
	{
		if(m_allState[i] != NULL)
		{
			delete m_allState[i];
			m_allState[i] = 0;
		}
	}
}

void AIBrain::Init(BaseAIInfo *baseAI, BetterAIInfo *betterAI)
{
	m_baseAI = baseAI;
	m_betterAI = betterAI;

	for(int i=0; i<eAIStateType_Max; ++i)
	{
		m_allState[i] = NewState((eAIState)i, m_baseAI->state[i]);
	}
}

void AIBrain:: Release()
{
	m_time = AI_CYCLE_TIME;
	m_curState = eAIStateType_Idle;
	m_checkEnemyTime = 0;
	m_first = 0;
	m_isPath = false;
	m_monsterType = eRandomWayPos;
	m_pathTime = 0;
	m_curIndex = -1;
	m_offset = 0;
	m_curStay = 0;
	m_AjustTime = 0;
	m_baseAI = 0;
	m_betterAI = 0;

	m_GirlWarList.clear();
	m_enemyList.clear();
	m_enemyValue.clear();
	m_path.clear();

	for(int i=0; i<eAIStateType_Max; ++i)
	{
		if(m_allState[i] != NULL)
		{
			delete m_allState[i];
			m_allState[i] = 0;
		}
	}

}

void AIBrain::SetOwner(Smart_Ptr<Monster> &obj)
{
	m_owner = obj;
}

Smart_Ptr<Monster> AIBrain::GetOwner()
{
	return m_owner;
}

void AIBrain::Update(DWORD dwDeltaTick)
{
	m_time += dwDeltaTick;
	if(m_time < AI_CYCLE_TIME)
	{
		return;
	}

	//检测
	CheckEnemyList(m_time);

	try
	{
		//生存时间,自然死亡状态下不会给予伤害者任何收益
		//if(!m_owner->IsAliveTime())
		//{
		//	ClearEnemy();
		//	m_owner->SetDead();
		//}

		if(!m_owner->IsInCreatureStateAny(ERS_Still | ERS_Ice | ERS_Stun))
		{
			if(m_allState[m_curState] != 0)
			{
				m_allState[m_curState]->Update(m_time);
			}
		}
//		else
//		{
//			//冰冻，僵直，眩晕状态战斗时间还得继续走
//
//		}
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "current state [%d] update error[errmsg=%s]",m_curState, e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "current state [%d] update unknown error",m_curState);
	}

	m_time = 0;
}

bool AIBrain::ChangeState( eAIState eNewStateID )
{
	if(eNewStateID < 0 || eNewStateID > eAIStateType_Max)
	{
		LOG_ERROR(FILEINFO, "AIState id error");

		return false;
	}

	if(eNewStateID == m_curState)
	{
		LOG_ERROR(FILEINFO, "AIState id is same");

		return false;
	}

	if(m_allState[eNewStateID] == 0)
	{
		LOG_ERROR(FILEINFO, "new state id is error");

		return false;
	}

	if(m_allState[m_curState] == 0)
	{
		LOG_ERROR(FILEINFO, "current state id is error");

		return false;
	}

	try
	{
		m_allState[m_curState]->EndState();
		m_allState[eNewStateID]->BeginState();
		m_curState = eNewStateID;
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "change state error[errmsg=%s]", e.what());

		return false;
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "change state unknown error");

		return false;
	}

	return true;
}

vector<Smart_Ptr<CreatureObj> > & AIBrain::GetEnemyList()
{
	m_enemyList.clear();

	m_owner->GetEnemyList(m_enemyList);

	vector<Smart_Ptr<CreatureObj> >::iterator it = m_enemyList.begin();
	for(; it != m_enemyList.end(); )
	{
		if(!IsEnemy(*it))
		{
			it = m_enemyList.erase(it);
		}
		else
		{
			++it;
		}
	}

	for(uint i = 0 ; i < m_enemyList.size() ; i++)
	{
		//同时将怪物添加到玩家仇恨列表中
		if(m_enemyList[i]->GetType() == ePlayer)
		{
			Smart_Ptr<Player> player = m_enemyList[i];

			Smart_Ptr<CreatureObj> mon = GetOwner();
			player->AddEnemy(mon);
		}
	}
	return m_enemyList;
}

void AIBrain::AddEnemy(Smart_Ptr<CreatureObj> &obj)
{
	if(!obj)
	{
		return;
	}

	vector<Smart_Ptr<CreatureObj> >::iterator it = m_enemyList.begin();
	for(; it!=m_enemyList.end(); ++it)
	{
		if((*it).Get() == obj.Get() || obj.Get() == m_owner.Get())
		{
			return;
		}
	}

	m_enemyList.push_back(obj);

	//同时将怪物添加到玩家仇恨列表中
	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		Smart_Ptr<CreatureObj> mon = GetOwner();
		player->AddEnemy(mon);
	}
}

void AIBrain::DeleteTargetEnemy(Smart_Ptr<CreatureObj> & player)
{
	vector<Smart_Ptr<CreatureObj> >::iterator it = m_enemyList.begin();
	for(; it!=m_enemyList.end(); )
	{
		if((*it).Get() == 0)
		{
			//m_enemyValue[(*it)->GetKey()] = 0;
			it = m_enemyList.erase(it);
			continue;
		}

		if(player.Get() == (*it).Get())
		{
			//m_enemyValue[(*it)->GetKey()] = 0;
			it = m_enemyList.erase(it);
			continue;
		}

		if(!m_owner->IsViewInLength((*it)->GetXNewPos(), (*it)->GetYNewPos()))
		{
			//m_enemyValue[(*it)->GetKey()] = 0;
			it = m_enemyList.erase(it);
			continue;
		}

		it++;
	}

}

void AIBrain::DeleteEnemy(Smart_Ptr<CreatureObj> & player)
{
	vector<Smart_Ptr<CreatureObj> >::iterator it = m_enemyList.begin();
	for(; it!=m_enemyList.end(); )
	{
		if((*it).Get() == 0)
		{
			//m_enemyValue[(*it)->GetKey()] = 0;
			it = m_enemyList.erase(it);
			continue;
		}

		if(player.Get() == (*it).Get())
		{
			if((*it)->GetType() == ePlayer)
			{
				Smart_Ptr<Player> player = (*it);
				player->DeleteMonster(GetOwner()->GetKey());
			}

			//m_enemyValue[(*it)->GetKey()] = 0;
			it = m_enemyList.erase(it);
			continue;
		}

		if(!(*it)->IsInMap())
		{
			//m_enemyValue[(*it)->GetKey()] = 0;
			it = m_enemyList.erase(it);
			continue;
		}

		if(!m_owner->IsViewInLength((*it)->GetXNewPos(), (*it)->GetYNewPos()))
		{
			//m_enemyValue[(*it)->GetKey()] = 0;
			it = m_enemyList.erase(it);
			continue;
		}
		it++;
	}

}

bool AIBrain::IsEnemy(Smart_Ptr<CreatureObj> &obj)
{
	DWORD country = m_owner->getAttManage()->getValue<DWORD>(eCharCountry);
	if(obj->GetType()==ePlayer)
	{
		DWORD clanid = obj->getAttManage()->getValue<DWORD>(eCharFaction);
		if(clanid == country)
		{
			return false;
		}
	}
	else if(obj->GetType()==eMonster)
	{
		Smart_Ptr<Monster> mon = obj;
		if(mon->GetQuality() == eGatherMonster || mon->GetQuality() == eElementMonster || mon->GetQuality() == eBoxMonster ||
				mon->GetQuality() == ePVPGatherMonster || mon->GetQuality() == eFestivalMonster || mon->GetQuality() == eBatterFlag || mon->GetQuality() == eChallengeBox)
		{
			return false;
		}
	}

	return true;
}

void AIBrain::ClearEnemy()
{
	vector<Smart_Ptr<CreatureObj> >::iterator it = m_enemyList.begin();
	for(; it!=m_enemyList.end(); it++)
	{
		if((*it).Get() == 0)
		{
			continue;
		}

		if((*it)->GetType() == ePlayer)
		{
			Smart_Ptr<Player> player = (*it);

			if(GetOwner() && player)
			{
				player->DeleteMonster(GetOwner()->GetKey());
				//同时删除仇恨列表
				Smart_Ptr<CreatureObj> mon = m_owner;
				player->DeleteTargetEnemy(mon);
			}
		}
	}
	m_enemyList.clear();
	m_enemyValue.clear();
	m_first = 0;

}

void AIBrain::ClearEnemyOnly()
{
	vector<Smart_Ptr<CreatureObj> >::iterator it = m_enemyList.begin();
	for(; it!=m_enemyList.end(); it++)
	{
			if((*it).Get() == 0)
			{
				continue;
			}

			if((*it)->GetType() == ePlayer)
			{
				Smart_Ptr<Player> player = (*it);

				if(GetOwner() && player)
				{
					player->DeleteMonster(GetOwner()->GetKey());
					//同时删除仇恨列表
					Smart_Ptr<CreatureObj> mon = m_owner;
					player->DeleteTargetEnemy(mon);
				}
			}
	}
	m_enemyList.clear();
}

void AIBrain::ClearEnemyValue()
{
	m_enemyValue.clear();
}

void AIBrain::ClearDeadEnemy()
{
	vector<Smart_Ptr<CreatureObj> >::iterator it = m_enemyList.begin();
	for(; it!=m_enemyList.end(); it++)
	{
		if((*it).Get() == 0)
		{
			continue;
		}

		if((*it)->GetType() == ePlayer)
		{
			Smart_Ptr<Player> player = (*it);

			if(GetOwner() && player)
			{
				player->DeleteMonster(GetOwner()->GetKey());
				//同时删除仇恨列表
				Smart_Ptr<CreatureObj> mon = m_owner;
				player->DeleteTargetEnemy(mon);
			}
		}
	}
	m_enemyList.clear();
	m_first = 0;
}

void AIBrain::UpdateEnemyValue(int value, int64 key)
{
	//归属最后一击的怪物，不加伤害值
	Smart_Ptr<Monster> mon = GetOwner();
	if(mon && !mon->IsBetterAI())
	{
		return;
	}

	Smart_Ptr<CreatureObj> obj = GetEnemy(key);
	if(!obj)
	{
		return;
	}

	//战姬保存的是所属玩家的信息
	Smart_Ptr<CreatureObj> attactObj = obj;
	if(obj->GetType()==ePet)
	{
		Smart_Ptr<GirlWar> pet = obj;
		if(pet && pet->GetOwner())
		{
			key = pet->GetOwner()->GetKey();
			attactObj = pet->GetOwner();
		}
		else
		{
			return;
		}
	}
	else if(obj->GetType()!=ePlayer)
	{
		return;
	}

	//todo:此处保存的是角色的id，非key
	int64 charID = attactObj->GetID();
	map<int64, EnemyDamageInfo>::iterator it = m_enemyValue.find(charID);
	if(it == m_enemyValue.end())
	{
		EnemyDamageInfo data;
		data.time = CUtil::GetNowSecond();
		data.value = value;
		m_enemyValue[charID] = data;
	}
	else
	{
		it->second.value += value;
		it->second.time = CUtil::GetNowSecond();
	}

	if(attactObj && m_owner && m_owner->GetQuality() == eWorldMonster && attactObj->GetType() == ePlayer)
	{
		EventArgs tempArgs;
		attactObj->FireEvent(PLAYER_ATTACK_WORLD_BOSS,tempArgs);
	}

	if(m_first==0 && attactObj && attactObj->GetType()==ePlayer)
	{
		m_first = charID;
	}
}

void AIBrain::CheckEnemyList(DWORD dwDeltaTick)
{
	m_checkEnemyTime += dwDeltaTick;
	if(m_checkEnemyTime < 60000)
		return;

	m_checkEnemyTime = 0;
	if(m_enemyValue.size()==0)
		return;

	int64 time = CUtil::GetNowSecond();
	map<int64,EnemyDamageInfo>::iterator it=m_enemyValue.begin();
	for(; it!=m_enemyValue.end(); )
	{
		if(it->second.time + m_clearEnemyTime <= time)
		{
			m_enemyValue.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

//攻击了怪物的战姬
	void AIBrain::addGirlTarget(const Smart_Ptr<CreatureObj>& obj)
	{
		if(obj.Get()==NULL)
			return;

		m_GirlWarList[obj->GetKey()] = obj;
	}

	void AIBrain::removeGirlTarget(const Smart_Ptr<CreatureObj>& obj)
	{
		if(obj.Get()==NULL)
			return;

		std::map<int,Smart_Ptr<CreatureObj> >::iterator it=m_GirlWarList.find(obj->GetKey());
		if(it!=m_GirlWarList.end())
		{
			m_GirlWarList.erase(it);
		}
	}

	void AIBrain::clearGirlTarget()
	{
		//注意目标对象重新选择新对象后会调用删除m_GirlWarList，此处必须用临时变量
		std::map<int,Smart_Ptr<CreatureObj> > tempTargetList=m_GirlWarList;
		std::map<int,Smart_Ptr<CreatureObj> >::iterator it=tempTargetList.begin();
		for(; it!=tempTargetList.end(); ++it)
		{
			if(it->second.Get()==NULL)
				continue;

			if(it->second->GetType()!=ePet)
				continue;

			Smart_Ptr<GirlWar> obj = it->second;
			if(obj->GetTargetObj().Get()!=NULL && obj->GetTargetObj()->GetKey()==m_owner->GetKey())
			{
				Smart_Ptr<CreatureObj> target;
				obj->SetTarget(target);
			}
		}
		m_GirlWarList.clear();
	}


Smart_Ptr<CreatureObj> AIBrain::GetMaxWeight(Smart_Ptr<CreatureObj> &end)
{
	//最后一击的归属怪物
	Smart_Ptr<Monster> mon = GetOwner();
	if(mon && !mon->IsBetterAI())
	{
		return end;
	}

	//最终归属
	Smart_Ptr<CreatureObj> ower = NULL;
	int value = 0;

	map<int64, EnemyDamageInfo>::iterator it = m_enemyValue.begin();
	for(; it != m_enemyValue.end(); ++it)
	{
		Smart_Ptr<Player> player;
		PlayerMessage::GetInstance()->GetPlayerByCharid(it->first,player);
		if(player.Get()==NULL)
			continue;

		int tempDamage = it->second.value;
		bool beFlag = false, enFlag = false;

		//初次攻击
		if(m_first == it->first)
		{
			beFlag = true;
		}

		//最后攻击
		if(end && end->GetType() == ePlayer && end->GetID()==it->first)
		{
			enFlag = true;
		}

		if(ower.Get()==NULL)
		{
			ower = player;
			value = DropItemLoader::GetInstance()->GetWeight(tempDamage, m_owner->getAttManage()->getValue<int>(eCharHPMax), beFlag, enFlag);
			continue;
		}

		int tmpWeight = DropItemLoader::GetInstance()->GetWeight(tempDamage, m_owner->getAttManage()->getValue<int>(eCharHPMax), beFlag, enFlag);
		if(tmpWeight > value)
		{
			value = tmpWeight;
			ower = player;
		}
	}

	//找不到归属者，以最后一击为准
	if(ower.Get()==NULL)
		return end;

	return ower;
}

void AIBrain::GetDamageListByNum(vector<pair<int64, int> > &DamageVec, int Num)
{
	vector<pair<int, int64> > DamageList;

	map<int64,EnemyDamageInfo>::iterator it = m_enemyValue.begin();
	for(; it != m_enemyValue.end(); ++it)
	{
		if(it->second.value > 0)
		{
			Smart_Ptr<Player> player;
			PlayerMessage::GetInstance()->GetPlayerByCharid(it->first,player);
			if(player.Get()==NULL)
				continue;

			DamageList.push_back( make_pair(it->second.value, player->GetID()) );
		}
	}

	sort(DamageList.begin(), DamageList.end());
	std::reverse(DamageList.begin(), DamageList.end());
	for(size_t i = 0;i < DamageList.size();++i)
	{
		if(i >= (size_t)Num)
			break;
		DamageVec.push_back(make_pair(DamageList[i].second,DamageList[i].first));
	}
}

Smart_Ptr<CreatureObj> AIBrain::GetMinLength()
{
	int target = -1;
	Smart_Ptr<CreatureObj> player;
	int size = 0;

	DeleteEnemy(player);
	if(m_enemyList.size() != 0)
	{//��һ��λ������
		int xLen = abs(m_enemyList[0]->GetXNewPos() - GetOwner()->GetXNewPos());
		int yLen = abs(m_enemyList[0]->GetYNewPos() - GetOwner()->GetYNewPos());
		double length = sqrt(xLen * xLen + yLen * yLen);
		target = 0;

		for(unsigned int i=0; i<m_enemyList.size(); ++i)
		{
			int txLen = abs(m_enemyList[i]->GetXNewPos() - GetOwner()->GetXNewPos());
			int tyLen = abs(m_enemyList[i]->GetYNewPos() - GetOwner()->GetYNewPos());
			double tLength = sqrt(txLen * txLen + tyLen * tyLen);

			if(length > tLength)
			{
				target = i;
			}
		}
	}

	if(target == -1)
	{
		size = m_enemyList.size();
		return 0;
	}

	return m_enemyList[target];
}

Smart_Ptr<CreatureObj> AIBrain::GetMinHP()
{
	int target = -1;
	DWORD curHP = 0;
	Smart_Ptr<CreatureObj> player;
	int size = 0;

	DeleteEnemy(player);

	if(m_enemyList.size() != 0)
	{
		curHP = m_enemyList[0]->getAttManage()->getValue<int>(eCharHP);
		target = 0;

		for(unsigned int i=0; i<m_enemyList.size(); ++i)
		{
			DWORD tCurHP = 0;

			tCurHP = m_enemyList[i]->getAttManage()->getValue<int>(eCharHP);

			if(curHP > tCurHP)
			{
				target = i;
			}
		}
	}

	if(target == -1)
	{
		size = m_enemyList.size();
		return 0;
	}

	return m_enemyList[target];
}

Smart_Ptr<CreatureObj> AIBrain::GetMinLV()
{
	int target = 0;
	Smart_Ptr<CreatureObj> player;
	int size = 0;

	DeleteEnemy(player);

	if(m_enemyList.size() != 0)
	{
		int curlv = m_enemyList[0]->getAttManage()->getValue<int>(eCharLv);
		target = 0;

		for(unsigned int i=0; i<m_enemyList.size(); ++i)
		{
			if(curlv > m_enemyList[i]->getAttManage()->getValue<int>(eCharLv))
			{
				target = i;
			}
		}
	}

	if(target == -1)
	{
		size = m_enemyList.size();
		return 0;
	}

	return m_enemyList[target];
}

Smart_Ptr<CreatureObj> AIBrain::GetMaxEnemy()
{
	int target = -1;
	int fvalue = -1;
	int index = -1;

	if(m_enemyValue.size() != 0)
	{
		Smart_Ptr<CreatureObj> player;

		DeleteEnemy(player);
	}

	map<int64, EnemyDamageInfo>::iterator it = m_enemyValue.begin();
	for(; it!=m_enemyValue.end(); ++it)
	{
		//是否在线
		Smart_Ptr<Player> player;
		PlayerMessage::GetInstance()->GetPlayerByCharid(it->first,player);
		if(player.Get()==NULL)
			continue;

		//是否同一张地图
		if(player->GetMapID() != GetOwner()->GetMapID())
			continue;

		//是否在攻击视野范围
		if( !GetOwner()->IsInSelectView(player->GetXNewPos(), player->GetYNewPos()))
			continue;

		if(fvalue < 0)
		{
			fvalue = it->second.value;
			index = it->first;
			continue;
		}

		if(it->second.value > fvalue)
		{
			fvalue = it->second.value;
			index = it->first;
		}
	}

	fvalue = 0;
	vector<Smart_Ptr<CreatureObj> >::iterator itVec = m_enemyList.begin();
	for(; itVec!=m_enemyList.end(); ++itVec)
	{
		++fvalue;
		if((*itVec).Get() == 0)
		{
			continue;
		}

		if( (*itVec)->GetType()==ePlayer && index == (*itVec)->GetID() )
		{
			target = fvalue - 1;
			break;
		}
	}

	if( target < 0 )
	{
		if( m_enemyList.size() == 0 )
		{
			return NULL;
		}
		//直接取第一个
		target = 0;
	}

	return m_enemyList[target];
}

Smart_Ptr<CreatureObj> AIBrain::GetEnemy(DWORD key)
{
	vector<Smart_Ptr<CreatureObj> >::iterator itVec = m_enemyList.begin();
	for(; itVec!=m_enemyList.end(); ++itVec)
	{
		if((*itVec).Get() == 0)
		{
			continue;
		}

		if((DWORD)(*itVec)->GetKey() == key)
		{
			return (*itVec);
		}
	}

	return 0;
}

Smart_Ptr<CreatureObj> AIBrain::GetRedNamePlayer()
{
	Smart_Ptr<CreatureObj> player;

	DeleteEnemy(player);

	if(m_enemyList.size() == 0)
	{
		GetEnemyList();
	}

	if(m_enemyList.size() != 0)
	{
		for(unsigned int i=0; i<m_enemyList.size(); ++i)
		{
			if(m_enemyList[i]->GetType() == ePlayer)
			{
				Smart_Ptr<Player> tempPlayer = m_enemyList[i];
				if(tempPlayer->GetSinState() == ERS_RED_NAME)
				{
					return m_enemyList[i];
				}
			}
		}
	}

	return 0;
}

bool AIBrain::CaseStrategy(int type, int param, int action, int rate)
{
	bool success = false;

	switch(type)
	{
	case eStrategyNull:
		{
			break;
		}
	case eStrategyHP:
		{
			MonsterInfo info;
			MonsterLoader::GetInstance()->GetMonsterInfo(m_owner->GetID(), info);

			int rateHP = m_owner->getAttManage()->getValue<int>(eCharHPMax) * param / 100;
			if(rateHP >= m_owner->getAttManage()->getValue<int>(eCharHP))
			{
				success = true;
			}

			break;
		}
	case eStrategyAtkTime:
		{
			if(param <= static_cast<AIStateAttack *>(m_allState[eAIStateType_Attack])->GetAttackTime())
			{
				success = true;
			}

			break;
		}
	default :
		{
			LOG_ERROR(FILEINFO, "monster act strategy but type is error");

			return false;
		}
	}

	if(success)
	{
		int randNum = rand()%100 + 1;

		if(randNum <= rate)
		{
			return true;
		}
	}

	return false;
}

bool AIBrain::FireSkill(int rate)
{
	int randNum = 0;

	randNum = rand() % 100 + 1;
	if(randNum > rate)
		return false;

	return true;
}

bool AIBrain::CaseStrategySkill(int type, int param, int action, int rate)
{
	switch(type)
	{
	case eStrategyHP :
		{
			MonsterInfo info;
			MonsterLoader::GetInstance()->GetMonsterInfo(m_owner->GetID(), info);

			int rateHP = m_owner->getAttManage()->getValue<int>(eCharHPMax) * param / 100;
			if(rateHP >= m_owner->getAttManage()->getValue<int>(eCharHP))
			{
				int randNum = rand()/100 + 1;

				if(randNum <= rate)
				{//todo  武灵做完后添加
					return true;
				}
			}

			break;
		}
	default :
		{
			LOG_ERROR(FILEINFO, "monster act skill strategy but type is error");

			return false;
		}
	}

	return false;
}

void AIBrain::ChangeTarget(int type, int rate)
{
	int randIndex = rand()%100 + 1;

	if(randIndex > rate)
	{
		return;
	}

	switch(type)
	{
	case eMinDistance:
		{
			Smart_Ptr<CreatureObj> player = AIBrain::GetMinLength();

//			if((bool)player)
//			{
				m_owner->SetTarget(player);
//			}

			break;
		}
	case eMinHP:
		{
			Smart_Ptr<CreatureObj> player = AIBrain::GetMinHP();

//			if((bool)player)
//			{
				m_owner->SetTarget(player);
//			}

			break;
		}
	case eLvHP:
		{
			Smart_Ptr<CreatureObj> player = AIBrain::GetMinLV();

//			if((bool)player)
//			{
				m_owner->SetTarget(player);
//			}

			break;
		}
	case eMaxEnemy:
		{
			Smart_Ptr<CreatureObj> player = AIBrain::GetMaxEnemy();

//			if((bool)player)
//			{
				m_owner->SetTarget(player);
//			}

			break;
		}
	case eNoChange:
		{
			break;
		}
	default :
		{
			LOG_ERROR(FILEINFO, "ai change target but type error");

			return;
		}
	}
}

void AIBrain::UpdatePath(int dwTick)
{
	if(GetOwner()->IsMonsterDead())
	{
		ChangeState(eAIStateType_Dead);
		ClearEnemy();
		return;
	}

	//速度为零不让移动
	if(GetOwner()->getAttManage()->getValue<int>(eCharMoveSpeed) <= 0)
	{
		return;
	}

	//到达目的地，则死亡；离玩家太远，则停止移动
	if(m_owner->IsEscortCar())
	{
		sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(m_owner->GetEscortID());
		if(entry)
		{
			NpcInfo npcinfo = NPCInfo::GetInstance()->GetNpcInfo(entry->m_nEndNpcID);
			Smart_Ptr<Player> player = m_owner->GetOwner();

			//玩家离镖车太远，则镖车不动
			if(player && !player->IsInDistance(m_owner,15))
			{
				if(!GetOwner()->IsInCreatureState(ERS_MONSTERESCORT_STAND))
				{
					GetOwner()->SetCreatureState(ERS_MONSTERESCORT_STAND);

					//镖车不移动了，通知客户端
					PlayerSynch::MonsterPathToPlayer topath;
					PlayerSynch::CharPos * desPos = topath.add_pos();

					desPos->set_xpos(GetOwner()->GetXNewPos());
					desPos->set_ypos(GetOwner()->GetYNewPos());
					topath.set_key(m_owner->GetKey());
					topath.set_state(m_curState);
					if(m_curState == eAIStateType_Attack || m_curState == eAIStateType_Return)
					{
						topath.set_movespeed(m_owner->GetRunSpeed());
					}
					else
					{
						topath.set_movespeed(m_owner->getAttManage()->getValue<int>(eCharMoveSpeed));
					}

					GetOwner()->SynchPath(topath);
					ClearPathAndTime();
				}

				return;
			}
			else
			{
				//当出现在玩家九宫格时同步一下路径，玩家一直在九宫格的只同步一次
				if(GetOwner()->IsInCreatureState(ERS_MONSTERESCORT_STAND))
				{
					GetOwner()->UnsetCreatureState(ERS_MONSTERESCORT_STAND);
				}
			}
		}
	}

	if( m_isPath )
	{//已经发送请求寻路了
		if( m_pathTime < 10000 )
		{
			m_pathTime += dwTick;
			m_curStay += dwTick;

			return;
		}

		m_isPath = false;
		m_pathTime -= 10000;
//		printf("\n\n++++++++++++++++++++++++%lld++++++++++++++++++\n\n", m_owner->GetID());
	}
	else
	{
		if( (size_t)( m_curIndex + 1 ) < m_path.size() )
		{
			//行走
			m_AjustTime += dwTick;
			Move( dwTick );

			return;
		}
	}

	//寻路
	FindPath( dwTick );
}

void AIBrain::UpdatePath( const PlayerSynch::SingleMonsterPath * path )
{
	if( ( path->pos( 0 ) >> 16 ) != m_owner->GetXNewPos() && ( path->pos( 0 ) & 0xffff ) != m_owner->GetYNewPos() )
	{
		LOG_WARNING(FILEINFO, "begin position is not equal");

		return;
	}

	if(!m_isPath)
	{
		LOG_WARNING(FILEINFO, "path already come");
		return;
	}

	if(m_owner->IsDead())
	{
		return;
	}

	if(m_owner->IsInCreatureStateAny(ERS_Still | ERS_Ice | ERS_Stun))
	{//处于这些状态，不能寻路
		return;
	}

	ClearPathAndTime();

	PlayerSynch::MonsterPathToPlayer topath;

	for( int i=0; i<path->pos_size(); ++i )
	{
		//镖车距离不能太远
		if(i >= GameParaLoader::GetInstance()->getMaxCanLeaveEscortDis() && m_owner->IsEscortCar())
			break;

		PlayerSynch::CharPos * desPos = topath.add_pos();

		desPos->set_xpos(path->pos( i ) >> 16);
		desPos->set_ypos(path->pos( i ) & 0xffff);

		m_path.push_back( path->pos( i ) );
	}

	if(m_path.size() <= 1)
	{
		m_path.clear();
	}
	else
	{
		if(!m_owner->IsInCreatureState(ERS_Walk))
		{
			m_owner->SetCreatureState(ERS_Walk);
		}
	}

	if( (size_t)( m_curIndex + 1 ) < m_path.size() )
	{
		topath.set_key(m_owner->GetKey());
		topath.set_state(m_curState);
		if(m_curState == eAIStateType_Attack || m_curState == eAIStateType_Return)
		{
			topath.set_movespeed(m_owner->GetRunSpeed());
		}
		else
		{
			topath.set_movespeed(m_owner->getAttManage()->getValue<int>(eCharMoveSpeed));
		}

		m_owner->SynchPath( topath );
	}
}

void AIBrain::Move( int dwTick )
{
	
	int speed = 0;
	int xpos = 0;
	int ypos = 0;
	int expos = 0;
	int eypos = 0;
	int nextLen = 0;
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_owner->GetMapID()));
	Sector * tSec = 0;
	bool isDel = false;

	if( m_curState == eAIStateType_Attack || m_curState == eAIStateType_Return )
	{
		speed = m_owner->GetRunSpeed();
	}
	else
	{
		speed = m_owner->getAttManage()->getValue<int>(eCharMoveSpeed);
	}

	if(speed <= 0)
	{
		return;
	}

	m_offset += speed * dwTick / 1000;
	if( m_offset < (info->GetSectorSize() / 2) )
	{//偏移少于一个格子大小，都不可能进入下一个格子
		return;
	}

	for( size_t i=m_curIndex; i<m_path.size()-1; ++i )
	{
		if( m_offset < (info->GetSectorSize() / 2) )
			break;

		int txLen = abs( ( int )( m_path[i] >> 16 ) - ( int )( m_path[i + 1] >> 16 ) );
		int tyLen = abs( ( int )( m_path[i] & 0xffff ) - ( int )( m_path[i + 1] & 0xffff ) );

		if(txLen == 1 && tyLen == 1)
		{
			nextLen = (int)(sqrt(2) * info->GetSectorSize());
		}
		else
		{
			nextLen = info->GetSectorSize();
		}

		if( m_offset >= ( nextLen / 2 ) )
		{
			expos = (m_path[i + 1] >> 16);
			eypos = (m_path[i + 1] & 0xffff);

			tSec = CSceneMgr::GetInstance()->GetSector(m_owner->GetMapID(), expos, eypos);
			if(!m_owner->IsEscortCar() && m_curState == eAIStateType_Attack &&
					(!tSec || (tSec && tSec->IsMonster())) &&
					m_AjustTime > ADJUST_MONSTER_TIME)
			{
				isDel = true;
				break;
			}

			m_offset -= nextLen;
			m_curIndex = i + 1;
			if( m_offset < (nextLen / 2))
			{
				break;
			}
		}
		else
		{
			break;
		}

		if(m_offset >= 0)
		{
			expos = (m_path[i + 1] >> 16);
			eypos = (m_path[i + 1] & 0xffff);

			tSec = CSceneMgr::GetInstance()->GetSector(m_owner->GetMapID(), expos, eypos);
			if(!m_owner->IsEscortCar() && m_curState == eAIStateType_Attack && (!tSec || (tSec && tSec->IsMonster()))&&
					m_AjustTime > ADJUST_MONSTER_TIME)
			{
				isDel = true;
				break;
			}
		}
	}

	xpos = (m_path[m_curIndex] >> 16);
	ypos = (m_path[m_curIndex] & 0xffff);

	if(isDel == true)
	{
		ClearPathAndTime();
	}

	if(xpos != m_owner->GetXNewPos() || ypos != m_owner->GetYNewPos())
	{
		{
			m_owner->SetXNewPos(xpos);
			m_owner->SetYNewPos(ypos);

			if(xpos == 0 || ypos == 0)
			{
				//assert( "pos" );
			}

			Smart_Ptr<Object> obj = m_owner;
			CSceneMgr::GetInstance()->MoveTo(obj);
		}

		if(m_owner->IsEscortCar())
		{//需要保存镖车的位置
			if(m_owner->GetOwner())
			{
				m_owner->GetOwner()->SetDataFlag(eEscortInfo);
			}
		}
	}

	if(isDel == true)
	{
		vector<Pos> posVec;
		int tpos = -1;

		CSceneMgr::GetInstance()->GetAroundPos(m_owner->GetMapID(), xpos, ypos, expos, eypos, posVec);

		if(posVec.size() > 1)
		{
			PlayerSynch::MonsterPathToPlayer topath;

			for( WORD i=0; i<posVec.size(); ++i )
			{
				PlayerSynch::CharPos * desPos = topath.add_pos();

				desPos->set_xpos(posVec[i].xpos);
				desPos->set_ypos(posVec[i].ypos);
				tpos = (posVec[i].xpos << 16) | posVec[i].ypos;
				m_path.push_back(tpos);
			}

			if( (size_t)( m_curIndex + 1 ) < m_path.size() )
			{
				topath.set_key(m_owner->GetKey());
				topath.set_state(m_curState);
				if(m_curState == eAIStateType_Attack || m_curState == eAIStateType_Return)
				{
					topath.set_movespeed(m_owner->GetRunSpeed());
				}
				else
				{
					topath.set_movespeed(m_owner->getAttManage()->getValue<int>(eCharMoveSpeed));
				}

				m_owner->SynchPath( topath );
			}
		}
	}
}

void AIBrain::FindPath( int xpos, int ypos, int expos, int eypos )
{
	int distance = 0;

	switch( m_curState )
	{
	case eAIStateType_Idle:
		{
				int ret = m_owner->GetNextPosition(xpos, ypos);
				if(ret != 0)
				{
					return;
				}

				if( ( xpos == -1 && ypos == -1 ) || ( xpos == m_owner->GetXNewPos() && ypos == m_owner->GetYNewPos() ) )
					return;
			break;
		}
	case eAIStateType_Attack:
		{
			if(m_baseAI->atkDistance == 0)
			{
				LOG_ERROR(FILEINFO, "monster[id=%d] is not existed ai info", m_owner->GetID());

				return ;
			}

			xpos = m_owner->GetTargetObj()->GetXNewPos();
			ypos = m_owner->GetTargetObj()->GetYNewPos();
			if( ( xpos == -1 && ypos == -1 ) || ( xpos == m_owner->GetXNewPos() && ypos == m_owner->GetYNewPos() ) )
				return;

			break;
		}
	case eAIStateType_Return:
		{
			xpos = m_owner->GetOriginalX();
			ypos = m_owner->GetOriginalY();

			break;
		}
	case eAIStateType_Dead:
		{
			break;
		}
	default:
		{
			break;
		}
	}

	if(m_owner->getAttManage()->getValue<int>(eCharMoveSpeed) <= 0)
	{
			return;
	}

	m_isPath = true;
	m_pathTime = 0;
	FindPathMgr::GetInstance()->FindPath( m_owner->GetMapID(), m_owner->GetKey(), m_owner->GetXNewPos(), m_owner->GetYNewPos(), xpos, ypos, distance );
}

void AIBrain::FindPath( int dwTick )
{
	int xpos = -1, ypos = -1, distance = 0;

	switch( m_curState )
	{
	case eAIStateType_Idle:
		{
			//镖车
			if(m_owner->IsEscortCar())
			{
				sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(m_owner->GetEscortID());
				if(entry)
				{
					DWORD index = GET_MAP_ID(m_owner->GetMapID());
					size_t i = 0;

					for(; i < (entry->m_nByWayOfMap.size() - 1);i++)
					{
						if(index == entry->m_nByWayOfMap[i])
						{
							int newindex = entry->m_nByWayOfMap[i+1];
							MapStartPoint pointInfo;
							CMapConfig::GetInstance()->GetTransferMapByTargetMapID(newindex,pointInfo);

							//已经寻过路了；不在同步路径
							if(m_isPath == true &&  (GetOwner()->GetXPos() != pointInfo.xpos && GetOwner()->GetYPos() != pointInfo.ypos))
							{
								return;
							}

							if(pointInfo.xpos > 0 && pointInfo.ypos > 0)
							{
								xpos = pointInfo.xpos;
								ypos = pointInfo.ypos;

								if(GetOwner()->GetXNewPos() == pointInfo.xpos && GetOwner()->GetYNewPos() == pointInfo.ypos)
								{
									return;
								}
							}

							break;
						}
					}

					if(index == entry->m_nByWayOfMap[(entry->m_nByWayOfMap.size()-1)])
					{
						NpcInfo npcinfo = NPCInfo::GetInstance()->GetNpcInfo(entry->m_nEndNpcID);
						Smart_Ptr<Player> player = m_owner->GetOwner();

						//到达目的地图
						//已经寻过路了；不在同步路径
						if(xpos == npcinfo.xpos && ypos == npcinfo.ypos)
						{
							return;
						}

						if((DWORD)npcinfo.mapid == index)
						{
							xpos = npcinfo.xpos;
							ypos = npcinfo.ypos;
						}
						break;
					}
				}
				else
					return;
			}
			else
			{
				if(m_curStay < m_owner->GetStayTime())
				{
					m_curStay += dwTick;

					return;
				}

				m_curStay -= m_owner->GetStayTime();
				int ret = m_owner->GetNextPosition(xpos, ypos);
				if(ret != 0)
				{
					return;
				}

				if( ( xpos == -1 && ypos == -1 ) || ( xpos == m_owner->GetXNewPos() && ypos == m_owner->GetYNewPos() ) )
				{
					return;
				}
			}

			break;
		}
	case eAIStateType_Attack:
		{
			distance = m_baseAI->atkDistance;
			if(distance == 0)
			{
				LOG_ERROR(FILEINFO, "monster[id=%d] is not existed ai info", m_owner->GetID());

				return ;
			}

			if(GetOwner()->IsAttackInLength(distance))
			{
				return;
			}

			m_owner->GetTargetObj()->GetPerfectLocation(GetOwner()->getAttManage()->getValue<int>(eCharMoveSpeed), GetOwner()->GetXNewPos(), GetOwner()->GetYNewPos(), xpos, ypos);
			if( ( xpos == -1 && ypos == -1 ) || ( xpos == m_owner->GetXNewPos() && ypos == m_owner->GetYNewPos() ) )
			{
				return;
			}

			if(!CSceneMgr::GetInstance()->HaveValidPos(m_owner->GetMapID(), m_owner->GetXNewPos(), m_owner->GetYNewPos(), xpos, ypos))
			{
				if(CSceneMgr::GetInstance()->RandGetValidPos(m_owner->GetMapID(), m_owner->GetXNewPos(), m_owner->GetYNewPos(), xpos, ypos))
				{
					if(xpos != m_owner->GetXNewPos() && ypos != m_owner->GetYNewPos())
					{
						PlayerSynch::MonsterPathToPlayer topath;
						PlayerSynch::CharPos * desPosf = topath.add_pos();
						PlayerSynch::CharPos * desPoss = topath.add_pos();

						desPosf->set_xpos(m_owner->GetXNewPos());
						desPosf->set_ypos(m_owner->GetYNewPos());
						desPoss->set_xpos(xpos);
						desPoss->set_ypos(ypos);

						if( (size_t)( m_curIndex + 1 ) < m_path.size() )
						{
							topath.set_key(m_owner->GetKey());
							topath.set_state(m_curState);
							if(m_curState == eAIStateType_Attack || m_curState == eAIStateType_Return)
							{
								topath.set_movespeed(m_owner->GetRunSpeed());
							}
							else
							{
								topath.set_movespeed(m_owner->getAttManage()->getValue<int>(eCharMoveSpeed));
							}

							m_owner->SynchPath( topath );
						}
					}
				}

			
				return;
			}

			break;
		}
	case eAIStateType_Return:
		{
			xpos = m_owner->GetOriginalX();
			ypos = m_owner->GetOriginalY();

			break;
		}
	case eAIStateType_Dead:
		{
			break;
		}
	default:
		{
			break;
		}
	}

	if(m_owner->getAttManage()->getValue<int>(eCharMoveSpeed) <= 0)
	{
		return;
	}

	m_isPath = true;
	m_pathTime = 0;
	if(m_owner->IsInCreatureState(ERS_Walk))
	{
		m_owner->UnsetCreatureState(ERS_Walk);
	}

	FindPathMgr::GetInstance()->FindPath( m_owner->GetMapID(), m_owner->GetKey(), m_owner->GetXNewPos(), m_owner->GetYNewPos(), xpos, ypos, distance );
}

void AIBrain::SetPath(std::vector<DWORD> &path)
{
	if((size_t)(m_curIndex + 1) < m_path.size())
	{
		for(size_t i = m_curIndex; i < m_path.size();i++)
		{
			path.push_back(m_path[i]);
		}
	}
}

void AIBrain::SetPath(PlayerSynch::SynchMonsterBase * info)
{
	if((size_t)(m_curIndex + 1) < m_path.size())
	{
		for(unsigned int i=m_curIndex; i<m_path.size(); ++i)
		{
			PlayerSynch::CharPos * desPos = info->add_pos();

			desPos->set_xpos(m_path[i] >> 16);
			desPos->set_ypos(m_path[i] & 0xffff);
		}
	}
}

void AIBrain::ClearPathAndTime()
{
//	printf("\n--------------clear path xpos = %d, ypos = %d---------------------\n", m_owner->GetXNewPos(), m_owner->GetYNewPos());
	m_path.clear();
	m_isPath = false;
	m_pathTime = 0;
	m_curIndex = 0;
	m_offset = 0;
	m_curStay = 0;
	m_AjustTime = 0;

	if(m_owner->IsInCreatureState(ERS_Walk))
	{
		m_owner->UnsetCreatureState(ERS_Walk);
	}
}

void AIBrain::GetEnemyValue(map<int64, int>& enemyValue)
{
	map<int64,EnemyDamageInfo>::iterator it=m_enemyValue.begin();
	for(; it!=m_enemyValue.end(); ++it)
	{
		enemyValue[it->first] = it->second.value;
	}
}


bool AIBrain::GetPerfectLocation(int moveSpeed, int xpos, int ypos, int &txpos, int &typos)
{
	if(!m_owner)
		return false;

	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_owner->GetMapID()));
	if(!info)
		return false;

	if((size_t)(m_curIndex + 1) < m_path.size() && info)
	{
		float nextLen = 0;
		size_t i = 0;

		for(i = m_curIndex; i < m_path.size() - 1;i++)
		{
			int txLen = abs( ( int )( m_path[i] >> 16 ) - ( int )( m_path[i + 1] >> 16 ) );
			int tyLen = abs( ( int )( m_path[i] & 0xffff ) - ( int )( m_path[i + 1] & 0xffff ) );

			if(txLen == 1 && tyLen == 1)
			{
				nextLen += (int)(sqrt(2) * info->GetSectorSize());
			}
			else
			{
				nextLen += info->GetSectorSize();
			}

			float timeDis = nextLen / (float)m_owner->getAttManage()->getValue<int>(eCharMoveSpeed);
			float moveDis = timeDis * moveSpeed;

			int realDistX = abs(xpos - (int)(m_path[i] >> 16));
			int realDistY = abs(ypos - (int)(m_path[i] & 0xffff));
			float realDist = sqrt(realDistX * realDistX + realDistY * realDistY);

			if((moveDis + 1) >= realDist && (moveDis - 1) <= realDist)
			{//1的范围内，就算是最佳点
				txpos = m_path[i] >> 16;
				typos = m_path[i] & 0xffff;

				return true;
			}
			else if((moveDis - 1) > realDist)
			{//超出1太多，就用上一个点
				break;
			}
		}

		if(i > 0 && i < m_path.size() - 1)
		{
			--i;
			if(i < (size_t)m_curIndex)
				i = (size_t)m_curIndex;
		}

		txpos = m_path[i] >> 16;
		typos = m_path[i] & 0xffff;
	}
	else
	{
		txpos = m_owner->GetXNewPos();
		typos = m_owner->GetYNewPos();
	}

	return true;
}

DWORD64 AIBrain::getLeftTime()
{
	if(eAIStateType_Dead != m_curState)
		return 0;

	AIStateDead * state = (AIStateDead*)m_allState[eAIStateType_Dead];
	return state->getLeftTime();
}

AIState * AIBrain::NewState(eAIState type, int id)
{
	AIState *newState = 0;

	switch(type)
	{
	case eAIStateType_Idle:
		switch(id)
		{
		case eSimpleIdle:
			newState = new AIStateIdle(this);
			break;
		case eActIdle:
			newState = new ActMonsterIdle(this);
			break;
		case eGuardIdle:
			newState = new GuardMonsterIdle(this);
			break;
		case eTowerIdle:
			newState = new TowerMonsterIdle(this);
			break;
		case eEscortIdle:
			newState = new EscortMonsterIdle(this);
			break;
		case eBossIdle:
			newState = new BossMonsterIdle(this);
			break;
		default:
			break;
		}
		break;
	case eAIStateType_Attack:
		switch(id)
		{
		case eSimpleAttack:
			newState = new AIStateAttack(this);
			break;
		case eTowerAttack:
			newState = new TowerMonsterAttack(this);
			break;
		case eCommonAttack:
			newState = new CommonMonsterAttack(this);
			break;
		case eCompeteAttack:
			newState = new CompeteMonsterAttack(this);
			break;
		default:
			break;
		}
		break;
	case eAIStateType_Return:
		switch(id)
		{
		case eSimpleReturn:
			newState = new AIStateReturn(this);
			break;
		case eCommonReturn:
			newState = new CommonMonsterReturn(this);
			break;
		case eBossReturn:
			newState = new BossMonsterReturn(this);
			break;
		default:
			break;
		}
		break;
	case eAIStateType_Dead:
		switch(id)
		{
		case eSimpleDead:
			newState = new AIStateDead(this);
			break;
		case eCommonDead:
			newState = new CommonMonsterDead(this);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return newState;
}
