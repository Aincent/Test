/*
print * PetBrain.cpp
 *
 *  Created on: 2014年12月8日
 *      Author: root
 */

#include "PetBrain.h"
#include "PetState.h"
#include "OtherDefine.h"
#include "Log/Logger.h"
#include "PetStateIdle.h"
#include "PetStateAttack.h"
#include "PetStateDead.h"
#include <math.h>
#include "../../../Map/SceneManager.h"
#include "../../../Map/FindPathMgr.h"
#include "../../../Map/ConfigLoader/MapConfig.h"
#include "../../../Map/ConfigLoader/FieldInfo.h"
#include "../../FileLoader/GirlWarLoader.h"

PetBrain::PetBrain():m_time(AI_CYCLE_TIME),m_curState(ePetStateType_Idle),m_isPath(false),m_pathTime(0),m_curIndex(-1),m_offset(0),m_curStay(0)
{
	m_allState[ePetStateType_Idle] = new PetStateIdle(this);
	m_allState[ePetStateType_Attack] = new PetStateAttack(this);
	m_allState[ePetStateType_Dead] = new PetStateDead(this);
}

PetBrain::~PetBrain()
{
	for(int i=0; i<ePetStateType_Max; ++i)
	{
		if(m_allState[i] != NULL)
		{
			delete m_allState[i];
			m_allState[i] = 0;
		}
	}
}

void PetBrain::ReInit()
{

}

void PetBrain::Release()
{
	m_time = AI_CYCLE_TIME;
	m_curState = ePetStateType_Idle;
	m_isPath = false;
	m_pathTime = 0;
	m_curIndex = -1;
	m_offset = 0;
	m_curStay = 0;

	Smart_Ptr<CreatureObj> obj;
	m_enemy = obj;
	m_path.clear();
	m_targetList.clear();
}

void PetBrain::Update(DWORD dwDeltaTick)
{
	m_time += dwDeltaTick;
	if(m_time < AI_CYCLE_TIME)
	{
		return;
	}

	try
	{
		if(!m_owner->IsInCreatureStateAny(ERS_Still | ERS_Ice | ERS_Stun))
		{
			if(m_allState[m_curState] != 0)
			{
				m_allState[m_curState]->Update(m_time);
			}
		}
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

bool PetBrain::ChangeState( ePetState eNewStateID )
{
	if(eNewStateID < 0 || eNewStateID > ePetStateType_Max)
	{
		LOG_ERROR(FILEINFO, "PetState id error");

		return false;
	}

	if(eNewStateID == m_curState)
	{
		LOG_ERROR(FILEINFO, "PetState id is same");

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

void PetBrain::AddEnemy(Smart_Ptr<CreatureObj> &obj)
{
	if(!obj)
	{
		return;
	}

	if(!m_enemy && obj.Get() != m_owner.Get())
	{
		m_enemy = obj;
		return;
	}

	if(m_enemy.Get() == obj.Get() || obj.Get() == m_owner.Get())
	{
		return ;
	}

	m_enemy = obj;
}

void PetBrain::DeleteEnemy(Smart_Ptr<CreatureObj> & obj)
{
	if(!m_enemy)
	{
		return;
	}

	if(obj.Get() == m_enemy.Get())
	{
		m_enemy  = 0;
		return;
	}

//	if(!m_owner->IsViewInLength(m_enemy->GetXNewPos(),m_enemy->GetYNewPos()))
//	{
//		m_enemy  = 0;
//		return;
//	}
}

void PetBrain::ClearEnemy()
{
	if(!m_enemy)
	{
		return;
	}

	if(m_enemy->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = m_enemy;

		if(GetOwner())
		{
			player->DeleteGirlWar(GetOwner()->GetKey());
		}
	}

	Smart_Ptr<CreatureObj> obj;
	m_enemy  = obj;
}

void PetBrain::addTarget(const Smart_Ptr<CreatureObj> &obj)
{
	if(obj.Get()==NULL)
		return;

	m_targetList[obj->GetKey()] = obj;
}

void PetBrain::removeTarget(const Smart_Ptr<CreatureObj>& obj)
{
	if(obj.Get()==NULL)
		return;

	std::map<int,Smart_Ptr<CreatureObj> >::iterator it=m_targetList.find(obj->GetKey());
	if(it!=m_targetList.end())
	{
		m_targetList.erase(it);
	}
}

void PetBrain::clearTarget()
{
	//清除我自己的目标
	cancel(GetOwner()->GetTargetObj());

	//通知选择我的目标 注意目标对象重新选择新对象后会调用删除m_targetList，此处必须用临时变量
	std::map<int,Smart_Ptr<CreatureObj> >::iterator it=m_targetList.begin();
	for(; it!=m_targetList.end(); ++it)
	{
		if(it->second.Get()==NULL)
			continue;

		//当前目标列表只需其它战姬的，玩家和怪物不算
		if(it->second->GetType()!=ePet)
			continue;

	 Smart_Ptr<GirlWar> obj = it->second;
	 if(obj->GetTargetObj().Get()!=NULL && obj->GetTargetObj()->GetKey()==GetOwner()->GetKey())
	  {
		 Smart_Ptr<CreatureObj> target;
		 obj->SetTarget(target);
	  }
	}
	m_targetList.clear();
}

//我取消了某个目标
void PetBrain::cancel(const Smart_Ptr<CreatureObj> &obj)
{
	if(obj.Get()==NULL)
		return;

	if(obj->GetType()==ePet)
	{
		Smart_Ptr<GirlWar> pet = obj;
		pet->GetBrain()->removeTarget(GetOwner()->GetMyself());
	}

	if(obj->GetType()==eMonster)
	{
		Smart_Ptr<Monster> mon = obj;
		mon->GetBrain()->removeGirlTarget(GetOwner()->GetMyself());
	}
}


bool PetBrain::FireSkill(int rate)
{
	int randNum = 0;

//	srand((unsigned int)time(NULL));
	randNum = rand() % 100 + 1;
	if(randNum > rate)
		return false;

	return true;
}

void PetBrain::UpdatePath(int dwTick)
{
	if(GetOwner()->IsDead())
	{
		ChangeState(ePetStateType_Dead);

		return;
	}

	if( m_isPath )
	{//已经发送请求寻路了
		if( m_pathTime < 10000 )
		{
			m_pathTime += dwTick;
			m_curStay += dwTick;

			return;
		}

		m_pathTime -= 10000;
	}
	else
	{
		if( (size_t)( m_curIndex + 1 ) < m_path.size() )
		{//行走
			Move( dwTick );

			return;
		}
	}

	//寻路
	FindPath( dwTick );
}

void PetBrain::UpdatePath( const PlayerSynch::SingleMonsterPath * path )
{
	if( ( path->pos( 0 ) >> 16 ) != m_owner->GetXNewPos() && ( path->pos( 0 ) & 0xffff ) != m_owner->GetYNewPos() )
	{
		LOG_WARNING(FILEINFO, "begin position is not equal");

		return;
	}

	ClearPathAndTime();

	int xpos = -1;
	int ypos = -1;
	PlayerSynch::GirlWarPathToPlayer topath;

	for( int i=0; i<path->pos_size(); ++i )
	{
		xpos = path->pos(i) >> 16;
		ypos = path->pos(i) & 0xffff;
		m_path.push_back( path->pos( i ) );

		PlayerSynch::CharPos * desPos = topath.add_pos();

		desPos->set_xpos(xpos);
		desPos->set_ypos(ypos);
	}

	if( (size_t)( m_curIndex + 1 ) < m_path.size() )
	{
		topath.set_key(m_owner->GetKey());
		m_owner->SynchPath(topath);
	}
}

void PetBrain::Move( int dwTick )
{
	int speed = 0;
	int xpos = 0;
	int ypos = 0;
	int nextLen = 0;
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_owner->GetMapID()));


	speed = m_owner->getAttManage()->getValue<int>(eCharMoveSpeed);

	m_offset += speed * dwTick / 1000;
	if( m_offset < info->GetSectorSize() )
	{//偏移少于一个格子大小，都不可能进入下一个格子
		return;
	}

	for( size_t i=m_curIndex; i<m_path.size()-1; ++i )
	{
		if( m_offset < info->GetSectorSize() )
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

		if( m_offset > ( nextLen / 2 ) )
		{
			m_offset -= nextLen;
			m_curIndex = i + 1;
			if( m_offset <= nextLen)
			{
				break;
			}
		}
		else if( m_offset == ( nextLen / 2 ) )
		{
			m_offset = 0 - nextLen / 2;
			m_curIndex = i + 1;
			break;
		}
		else
		{
			break;
		}
	}

	xpos = (m_path[m_curIndex] >> 16);
	ypos = (m_path[m_curIndex] & 0xffff);

	if(m_isPath == true)
	{
		ClearPathAndTime();
	}

	if(xpos != m_owner->GetXNewPos() || ypos != m_owner->GetYNewPos())
	{
//		printf("\n----------------- %p move to xpos[%d]ypos[%d]   xnewpos[%d]ynewpos[%d]---------------\n", m_owner->GetOwner().Get(), xpos,ypos,m_owner->GetXNewPos(),m_owner->GetYNewPos());
//		LOG_DEBUG(FILEINFO,"\n----------------- %p move to xpos[%d]ypos[%d]   xnewpos[%d]ynewpos[%d]---------------\n", m_owner->GetOwner().Get(), xpos,ypos,m_owner->GetXNewPos(),m_owner->GetYNewPos());
		m_owner->SetXNewPos(xpos);
		m_owner->SetYNewPos(ypos);

		Smart_Ptr<Object> obj = m_owner;
		CSceneMgr::GetInstance()->MoveTo(obj);
	}
}

void PetBrain::FindPath( int dwTick )
{
	int xpos = -1, ypos = -1, distance = 0;

	switch( m_curState )
	{
	case ePetStateType_Idle:
		{
			m_owner->GetNextPosition(xpos, ypos);

			if( ( xpos == -1 && ypos == -1 ) || CSceneMgr::GetInstance()->GetDistance(m_owner->GetXNewPos(), m_owner->GetYNewPos(), xpos, ypos) < 3 || m_owner->InDefMap())
				return;

			if(CSceneMgr::GetInstance()->GetDistance(xpos,ypos,m_owner->GetXNewPos(),m_owner->GetYNewPos()) > GirlWarLoader::GetInstance()->GetMoveRange(m_owner->GetID()))
			{
				m_owner->RelocatePos(xpos,ypos);
				return;
			}

			break;
		}
	case ePetStateType_Attack:
		{
			distance = GirlWarLoader::GetInstance()->GetAckRange(m_owner->GetID());;
			if(distance == 0)
			{
				LOG_ERROR(FILEINFO, "girlWar[id=%d] is not existed ai info", m_owner->GetID());

				return ;
			}

			if(GetOwner()->IsAttackInLength(distance))
			{
				return;
			}

			m_owner->GetTargetObj()->GetPerfectLocation(GetOwner()->getAttManage()->getValue<int>(eCharMoveSpeed), GetOwner()->GetXNewPos(), GetOwner()->GetYNewPos(), xpos, ypos);
			if( ( xpos == -1 && ypos == -1 ) || ( xpos == m_owner->GetXNewPos() && ypos == m_owner->GetYNewPos() ) )
				return;

			if(CSceneMgr::GetInstance()->GetDistance(xpos,ypos,m_owner->GetXNewPos(),m_owner->GetYNewPos()) <= distance)
			{
				return;
			}

			break;
		}
	case ePetStateType_Dead:
		{
			return;
			break;
		}
	default:
		{
			break;
		}
	}

//	printf("\n-----------------  %p  find way xpos[%d]ypos[%d]   xnewpos[%d]ynewpos[%d]---------------\n", m_owner->GetOwner().Get(), xpos,ypos,m_owner->GetXNewPos(),m_owner->GetYNewPos());
	m_isPath = true;
	m_pathTime = 0;
	FindPathMgr::GetInstance()->FindPath( m_owner->GetMapID(), m_owner->GetKey(), m_owner->GetXNewPos(), m_owner->GetYNewPos(), xpos, ypos, distance );
}

void PetBrain::SetPath(PlayerSynch::GirlWarPathToPlayer &path)
{
	if((size_t)(m_curIndex + 1) < m_path.size())
	{
		for(size_t i = m_curIndex; i < m_path.size();i++)
		{
			PlayerSynch::CharPos * desPos = path.add_pos();

			desPos->set_xpos(m_path[i] >> 16);
			desPos->set_ypos(m_path[i] & 0xffff);
		}
	}
}

void PetBrain::ClearPathAndTime()
{
//	printf("\n--------------%p clear path xpos = %d, ypos = %d---------------------\n", m_owner->GetOwner().Get(), m_owner->GetXNewPos(), m_owner->GetYNewPos());
	m_path.clear();
	m_isPath = false;
	m_pathTime = 0;
	m_curIndex = 0;
	m_offset = 0;
	m_curStay = 0;
}

void PetBrain::GetPerfectLocation(int moveSpeed, int xpos, int ypos, int &txpos, int &typos)
{
	if(!m_owner)
	{
		return;
	}

	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_owner->GetMapID()));

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

				return;
			}
			else if((moveDis - 1) > realDist)
			{//超出1太多，就用上一个点
				break;
			}
		}

		if(i > 0 && i < m_path.size() - 1)
		{
			--i;
			if(i < (DWORD)m_curIndex)
				i = m_curIndex;
		}

		txpos = m_path[i] >> 16;
		typos = m_path[i] & 0xffff;
	}
	else
	{
		txpos = m_owner->GetXNewPos();
		typos = m_owner->GetYNewPos();
	}
}


