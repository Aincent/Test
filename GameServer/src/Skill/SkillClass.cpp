/*
 * SkillClass.cpp
 *
 *  Created on: 2014年1月21日
 *      Author: helh
 */
#include "SkillClass.h"
#include "./SkillLoader/SkillLoader.h"
#include "Log/Logger.h"
#include "../Object/Creature.h"
#include "../Map/SceneManager.h"
#include <math.h>
#include "../ServerEventDefine.h"
#include "../Object/GirlWar/GirlWarManager.h"
#include "Effect/Effect.h"

CSkillBlink::CSkillBlink(int id, Smart_Ptr<CreatureObj> & owner, int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info)
: CSkillBase(id, owner, key, xpos, ypos, onlyid, info), m_releaseTimer(0), m_allTimer(0),m_stopTimer(0)
{

}

CSkillBlink::~CSkillBlink()
{
	if(m_releaseTimer.IsValid())
	{
		m_releaseTimer.SetNull();
	}

	if(m_stopTimer.IsValid())
	{
		m_stopTimer.SetNull();
	}
}

void CSkillBlink::Init(int id, Smart_Ptr<CreatureObj> & owner,int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info)
{
	CSkillBase::InitInfo(id, owner, key, xpos, ypos, onlyid, info);

	m_releaseTimer = 0;
	m_allTimer = 0;
	m_stopTimer = 0;
}

void CSkillBlink::Release()
{
	CSkillBase::Release();
	if(m_releaseTimer.IsValid())
	{
		m_releaseTimer.SetNull();
	}

	if(m_stopTimer.IsValid())
	{
		m_stopTimer.SetNull();
	}
}

bool CSkillBlink::BeginStart()
{
	// 计算施法者位置与目标点的距离
	int delayTimer = 0;
	if (m_info->flySpeed > 0)
	{
		float fDis = 0.0;
		int xtemp = 0;
		int ytemp = 0;

		if(m_xpos <= 0 && m_ypos <= 0)
		{
			//获取目标对象obj
			Smart_Ptr<CreatureObj> targetObj;
			CSceneMgr::GetInstance()->GetObject(m_owner->GetMapID(),m_targetKey,targetObj);

			if(!(bool)targetObj || targetObj->IsDead() || !targetObj->IsInMap())
			{
				xtemp = abs(m_owner->GetXNewPos());
				ytemp = abs(m_owner->GetYNewPos());
			}
			else
			{
				xtemp = abs(m_owner->GetXNewPos() - targetObj->GetXNewPos());
				ytemp = abs(m_owner->GetYNewPos() - targetObj->GetYNewPos());
			}
		}
		else
		{
			xtemp = abs(m_owner->GetXNewPos() - m_xpos);
			ytemp = abs(m_owner->GetYNewPos() - m_ypos);
		}

		fDis = sqrt(xtemp * xtemp + ytemp * ytemp);

		// 计算技能产生效果所需要的时间,飞行时间
		delayTimer = (int)(fDis * 1000 / m_info->flySpeed);
	}

	if (delayTimer <= 200)
	{
		// 少于0.2秒，立马产生作用
		FlyTimer(NULL);
	}
	else
	{
		// 注册定时器
		if(m_releaseTimer.IsValid())
		{
			m_releaseTimer.SetNull();
		}
		m_releaseTimer = TimerInterface::GetInstance()->RegTimePoint(&CSkillBlink::FlyTimer, this, 0, 0, 0, 0, delayTimer);
	}

	return true;
}

bool CSkillBlink::EndStart()
{
	OnEnd();

	return true;
}

void CSkillBlink::OnEndImp()
{
	if(m_owner->GetType() == ePlayer && m_info->limitPro != eProfessionMax && m_info->fType != eAssistSkill)
	{
		Smart_Ptr<Player> obj = m_owner;

		obj->GetSkillMgr()->SetSkillNull(GetOnlyID());
	}
	else if(m_info->limitPro == eProfessionMax)
	{
		Smart_Ptr<Player> obj = m_owner;

		obj->GetSkillMgr()->SetBigSkill(0);
	}

	if(m_releaseTimer.IsValid())
	{
		m_releaseTimer.SetNull();
	}

	if(m_stopTimer.IsValid())
	{
		m_stopTimer.SetNull();
	}
}

void CSkillBlink::FlyTimer(void * arg)
{
	if(m_releaseTimer.IsValid())
	{
		m_releaseTimer.SetNull();
	}

	Result();

	if (m_bEnd == false)
	{
		OnEnd();
	}
}

void CSkillBlink::StopTimer(void * arg)
{
	if(m_stopTimer.IsValid())
	{
		m_stopTimer.SetNull();
	}

//	printf("\n---------------------- owner [%lld] skill[%p] skillid=%d CSkillBlink::StopTimer() end --------------------------------", m_owner->GetID(), this, GetID());
	OnEnd();
}

void CSkillBlink::Result()
{
	//获取目标对象obj
	Smart_Ptr<CreatureObj> targetObj;
	CSceneMgr::GetInstance()->GetObject(m_owner->GetMapID(),m_targetKey,targetObj);

	if(m_info->effectFirstType[0] == eMove)
	{
		LOG_DEBUG(FILEINFO,"CSkillBlink::Result............m_xpos = %d ...............m_ypos = %d",m_xpos,m_ypos);
		m_owner->RelocatePos(m_xpos, m_ypos);
	}

	//属性变化
	map<int, vector<AttrChange> > allAttr;
	map<int, vector<AttrChange> > myselfAttr;

	for(int i=0; i<3; ++i)
	{
		// 查找受影响的目标
		vector<Smart_Ptr<CreatureObj> > targets;

		if(m_info->targetCount > 0)
		{
			if(m_info->targetCount > 1 && m_info->rangeType == eRangeMyself)
			{
				CSceneMgr::GetInstance()->GetSkillObject(m_owner, m_owner->GetXNewPos(), m_owner->GetYNewPos(), targetObj, m_info->m_EffectTargetType[i], m_info->targetCount, m_info->rangeType, m_info->rangeLength,m_info->rangeWidth, targets);
			}
			else
			{
				if(m_xpos > 0 || m_ypos > 0)
				{
					CSceneMgr::GetInstance()->GetSkillObject(m_owner, m_xpos, m_ypos, targetObj, m_info->m_EffectTargetType[i], m_info->targetCount, m_info->rangeType, m_info->rangeLength,m_info->rangeWidth, targets);
				}
				else if(m_info->targetCount > 1 && (bool)targetObj)
				{
					CSceneMgr::GetInstance()->GetSkillObject(m_owner, targetObj->GetXNewPos(), targetObj->GetYNewPos(), targetObj, m_info->m_EffectTargetType[i], m_info->targetCount, m_info->rangeType, m_info->rangeLength,m_info->rangeWidth, targets);
				}
				else if(m_info->targetCount == 1 && (bool)targetObj)
				{
					if(0 == m_owner->IsAttacked(targetObj, m_info->m_EffectTargetType[i]==eSkillTargetTypeNull ? eEnemyTarget : m_info->m_EffectTargetType[i]))
					{
						targets.push_back(targetObj);
					}
				}
			}
		}

		// 开始产生作用
		ResultEffect(i, targets, allAttr, myselfAttr);
	}

	//属性变更
	ResultEffectAttrChange(allAttr, myselfAttr);

}

CSkillDelay::CSkillDelay(int id, Smart_Ptr<CreatureObj> & owner, int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info)
: CSkillBase(id, owner, key, xpos, ypos, onlyid, info),m_releaseTimer(0),m_stopTimer(0), m_cycleTimer(0), m_allCount(1), m_curCount(0)
{
}

CSkillDelay::~CSkillDelay()
{
	if(m_releaseTimer.IsValid())
	{
		m_releaseTimer.SetNull();
	}

	if(m_cycleTimer.IsValid())
	{
		m_cycleTimer.SetNull();
	}

	if(m_stopTimer.IsValid())
	{
		m_stopTimer.SetNull();
	}
}

void CSkillDelay::Init(int id, Smart_Ptr<CreatureObj> & owner,int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info)
{
	CSkillBase::InitInfo(id, owner, key, xpos, ypos, onlyid, info);
	m_releaseTimer = 0;
	m_stopTimer = 0;
	m_cycleTimer = 0;
	m_allCount = info->aliveTime / info->intervalTime;
	m_curCount = 0;
}

void CSkillDelay::Release()
{
	CSkillBase::Release();

	if(m_releaseTimer.IsValid())
	{
		m_releaseTimer.SetNull();
	}

	if(m_cycleTimer.IsValid())
	{
		m_cycleTimer.SetNull();
	}

	if(m_stopTimer.IsValid())
	{
		m_stopTimer.SetNull();
	}
}

float CSkillDelay::GetTotalTime() const
{
	return m_info->aliveTime;
}

bool CSkillDelay::BeginStart()
{
	if(m_stopTimer.IsValid())
	{
		m_stopTimer.SetNull();
	}

	m_stopTimer = TimerInterface::GetInstance()->RegTimePoint(&CSkillDelay::StopTimer, this, 0, 0, 0, 0, m_info->aliveTime);

	// 计算施法者位置与目标点的距离
	int delayTimer = 0;
	if (m_info->flySpeed > 0)
	{
		float fDis = 0.0;
		int xtemp = 0;
		int ytemp = 0;

		if(m_xpos <= 0 && m_ypos <= 0)
		{
			//获取目标对象obj
			Smart_Ptr<CreatureObj> targetObj;
			CSceneMgr::GetInstance()->GetObject(m_owner->GetMapID(),m_targetKey,targetObj);

			if(!(bool)targetObj || targetObj->IsDead() || !targetObj->IsInMap())
			{
				xtemp = abs(m_owner->GetXNewPos());
				ytemp = abs(m_owner->GetYNewPos());
			}
			else
			{
				xtemp = abs(m_owner->GetXNewPos() - targetObj->GetXNewPos());
				ytemp = abs(m_owner->GetYNewPos() - targetObj->GetYNewPos());
			}
		}
		else
		{
			xtemp = abs(m_owner->GetXNewPos() - m_xpos);
			ytemp = abs(m_owner->GetYNewPos() - m_ypos);
		}

		fDis = sqrt(xtemp * xtemp + ytemp * ytemp);

		//计算技能产生效果所需要的时间,飞行时间
		delayTimer = (int)(fDis * 1000 / m_info->flySpeed);
	}

	if (delayTimer <= 200)
	{
		ContinueTimer(NULL);
		if(m_cycleTimer.IsValid())
		{
			m_cycleTimer.SetNull();
		}
		m_cycleTimer = TimerInterface::GetInstance()->RegRepeatedTime(&CSkillDelay::ContinueTimer, this, 0, 0, 0, 0, m_info->intervalTime,m_info->intervalTime);
	}
	else
	{
		// 注册定时器
		if(m_releaseTimer.IsValid())
		{
			m_releaseTimer.SetNull();
		}
		m_releaseTimer = TimerInterface::GetInstance()->RegTimePoint(&CSkillDelay::FlyTimer, this, 0, 0, 0, 0, delayTimer);
	}

	return true;
}

bool CSkillDelay::EndStart()
{
	OnEnd();

	return true;
}

void CSkillDelay::OnEndImp()
{
//	printf("\n---------------------- owner [%lld] skill[%p] skillid=%d CSkillDelay::OnEndImp()1 end --------------------------------", m_owner->GetID(), this, GetID());
	if(m_owner->GetType() == ePlayer && m_info->limitPro != eProfessionMax && m_info->fType != eAssistSkill)
	{
//		printf("\n---------------------- owner [%lld] skill[%p] skillid=%d CSkillDelay::OnEndImp()2 end --------------------------------", m_owner->GetID(), this, GetID());
		Smart_Ptr<Player> obj = m_owner;

		obj->GetSkillMgr()->SetSkillNull(GetOnlyID());
	}
	else if(m_info->limitPro == eProfessionMax)
	{
//		printf("\n---------------------- owner [%lld] skill[%p] skillid=%d CSkillDelay::OnEndImp()3 end --------------------------------", m_owner->GetID(), this, GetID());
		Smart_Ptr<Player> obj = m_owner;

		obj->GetSkillMgr()->SetBigSkill(0);
	}

	if(m_releaseTimer.IsValid())
	{
		m_releaseTimer.SetNull();
	}

	if(m_cycleTimer.IsValid())
	{
		m_cycleTimer.SetNull();
	}

	if(m_stopTimer.IsValid())
	{
		m_stopTimer.SetNull();
	}
}

void CSkillDelay::FlyTimer(void * arg)
{
	if(m_stopTimer.IsValid())
	{
		m_releaseTimer.SetNull();
	}

	if (m_curCount >= m_allCount)
	{
		// 已经结束了
		return;
	}

	Result();
	m_curCount++;

	if(m_cycleTimer.IsValid())
	{
		m_cycleTimer.SetNull();
	}
	m_cycleTimer = TimerInterface::GetInstance()->RegRepeatedTime(&CSkillDelay::ContinueTimer, this, 0, 0, 0, 0, m_info->intervalTime, m_info->intervalTime);
}

void CSkillDelay::StopTimer(void * arg)
{
	if(m_stopTimer.IsValid())
	{
		m_stopTimer.SetNull();
	}

	OnEnd();
}

void CSkillDelay::ContinueTimer(void * arg)
{
	if (m_curCount >= m_allCount)
	{
		// 已经结束了
		return;
	}

	Result();
	m_curCount++;
}

void CSkillDelay::Result()
{
	//获取目标对象obj
	Smart_Ptr<CreatureObj> targetObj;
	CSceneMgr::GetInstance()->GetObject(m_owner->GetMapID(),m_targetKey,targetObj);

	//属性变化
	map<int, vector<AttrChange> > allAttr;
	map<int, vector<AttrChange> > myselfAttr;

	// 查找受影响的目标
	for(int i=0; i<3; ++i)
	{
		// 查找受影响的目标
		vector<Smart_Ptr<CreatureObj> > targets;

		if(m_info->targetCount > 0)
		{
			if(m_info->targetCount > 1 && m_info->rangeType == eRangeMyself)
			{
				CSceneMgr::GetInstance()->GetSkillObject(m_owner, m_owner->GetXNewPos(), m_owner->GetYNewPos(), targetObj, m_info->m_EffectTargetType[i], m_info->targetCount, m_info->rangeType, m_info->rangeLength,m_info->rangeWidth, targets);
			}
			else
			{
				if(m_xpos > 0 || m_ypos > 0)
				{
					CSceneMgr::GetInstance()->GetSkillObject(m_owner, m_xpos, m_ypos, targetObj, m_info->m_EffectTargetType[i], m_info->targetCount, m_info->rangeType, m_info->rangeLength,m_info->rangeWidth, targets);
				}
				else if(m_info->targetCount > 1 && (bool)targetObj)
				{
					CSceneMgr::GetInstance()->GetSkillObject(m_owner, targetObj->GetXNewPos(), targetObj->GetYNewPos(), targetObj, m_info->m_EffectTargetType[i], m_info->targetCount, m_info->rangeType, m_info->rangeLength,m_info->rangeWidth, targets);
				}
				else if(m_info->targetCount == 1 && (bool)targetObj)
				{
					if(0 == m_owner->IsAttacked(targetObj, m_info->m_EffectTargetType[i]==eSkillTargetTypeNull ? eEnemyTarget : m_info->m_EffectTargetType[i]))
					{
						targets.push_back(targetObj);
					}
				}
			}
		}

		// 开始产生作用
		ResultEffect(i, targets, allAttr, myselfAttr);
	}

	//属性变更
	ResultEffectAttrChange(allAttr, myselfAttr);

	return ;
}


CSkillNest::CSkillNest(int id, Smart_Ptr<CreatureObj> & owner, int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info)
: CSkillBase(id, owner, key, xpos, ypos, onlyid, info),m_releaseTimer(0), m_stopTimer(0), m_cycleTimer(0), m_nextSkill(id)
{
}

CSkillNest::~CSkillNest()
{
	if(m_releaseTimer.IsValid())
	{
		m_releaseTimer.SetNull();
	}

	if(m_cycleTimer.IsValid())
	{
		m_cycleTimer.SetNull();
	}

	if(m_stopTimer.IsValid())
	{
		m_stopTimer.SetNull();
	}
}

void CSkillNest::Init(int id, Smart_Ptr<CreatureObj> & owner,int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info)
{
	CSkillBase::InitInfo(id, owner, key, xpos, ypos, onlyid, info);
	m_releaseTimer = 0;
	m_stopTimer = 0;
	m_cycleTimer = 0;
	m_nextSkill = id;
}

void CSkillNest::Release()
{
	CSkillBase::Release();

	if(m_releaseTimer.IsValid())
	{
		m_releaseTimer.SetNull();
	}

	if(m_cycleTimer.IsValid())
	{
		m_cycleTimer.SetNull();
	}

	if(m_stopTimer.IsValid())
	{
		m_stopTimer.SetNull();
	}

}

float CSkillNest::GetTotalTime() const
{
	return 0;
}

bool CSkillNest::Start()
{
	if(m_bEnd)
		return true;

	// 计算施法者位置与目标点的距离
	int delayTimer = 0;
	if(m_info->flySpeed > 0)
	{
		float fDis = 0.0;
		int xtemp = 0;//abs(m_owner->GetXNewPos() - m_xpos);
		int ytemp = 0;//abs(m_owner->GetYNewPos() - m_ypos);

		if(m_xpos <= 0 && m_ypos <= 0)
		{
			//获取目标对象obj
			Smart_Ptr<CreatureObj> targetObj;
			CSceneMgr::GetInstance()->GetObject(m_owner->GetMapID(),m_targetKey,targetObj);

			if(!(bool)targetObj || targetObj->IsDead() || !targetObj->IsInMap())
			{
				xtemp = abs(m_owner->GetXNewPos());
				ytemp = abs(m_owner->GetYNewPos());
			}
			else
			{
				xtemp = abs(m_owner->GetXNewPos() - targetObj->GetXNewPos());
				ytemp = abs(m_owner->GetYNewPos() - targetObj->GetYNewPos());
			}
		}
		else
		{
			xtemp = abs(m_owner->GetXNewPos() - m_xpos);
			ytemp = abs(m_owner->GetYNewPos() - m_ypos);
		}

		fDis = sqrt(xtemp * xtemp + ytemp * ytemp);

		// 计算技能产生效果所需要的时间,飞行时间
		delayTimer = (int)(fDis * 1000 / m_info->flySpeed);
	}

	if (delayTimer <= 200)
	{
		// 少于0.2秒，立马产生作用
		FlyTimer(NULL);
	}
	else
	{
		// 注册定时器
		if(m_releaseTimer.IsValid())
		{
			m_releaseTimer.SetNull();
		}
		m_releaseTimer = TimerInterface::GetInstance()->RegTimePoint(&CSkillNest::FlyTimer, this, 0, 0, 0, 0, delayTimer);
	}

	return true;
}

bool CSkillNest::BeginStart()
{
	if(m_stopTimer.IsValid())
	{
		//放出来下一段了
		SkillInfo *info = 0;

		info = SkillLoader::GetInstance()->GetSkillInfoByID(m_nextSkill);
		if(info == 0)
		{
			OnEnd();

			return true;
		}

		if(info->nextActionSkill > 0)
			m_nextSkill = info->nextActionSkill;

		//判断是否在释放
		if(m_cycleTimer.IsValid() || m_releaseTimer.IsValid())
			return true;
	}

	return Start();
}

bool CSkillNest::EndStart()
{
	OnEnd();

	return true;
}

void CSkillNest::OnEndImp()
{
	CDTimer();

	if(m_releaseTimer.IsValid())
	{
		m_releaseTimer.SetNull();
	}

	if(m_cycleTimer.IsValid())
	{
		m_cycleTimer.SetNull();
	}

	if(m_stopTimer.IsValid())
	{
		m_stopTimer.SetNull();
	}

	if(m_owner->GetType() == ePlayer && m_info->limitPro != eProfessionMax && m_info->fType != eAssistSkill)
	{
		Smart_Ptr<Player> obj = m_owner;

		obj->GetSkillMgr()->SetSkillNull(GetOnlyID());
	}
	else if(m_info->limitPro == eProfessionMax)
	{
		Smart_Ptr<Player> obj = m_owner;

		obj->GetSkillMgr()->SetBigSkill(0);
	}
}

void CSkillNest::FlyTimer(void * arg)
{
	if(m_bEnd)
		return;

	if(m_releaseTimer.IsValid())
	{
		m_releaseTimer.SetNull();
	}

	Result();

	if(m_info->nextActionSkill <= 0)
	{
		OnEnd();

		return ;
	}

	SkillInfo *ninfo = 0;
	ninfo = SkillLoader::GetInstance()->GetSkillInfoByID(m_info->nextActionSkill);
	if (ninfo == 0)
	{
		OnEnd();

		return ;
	}

	if(m_cycleTimer.IsValid())
	{
		m_cycleTimer.SetNull();
	}
	m_cycleTimer = TimerInterface::GetInstance()->RegTimePoint(&CSkillNest::ContinueTimer, this, 0, 0, 0, 0, m_info->aliveTime);

	if(m_stopTimer.IsValid())
	{
		m_stopTimer.SetNull();
	}

	m_stopTimer = TimerInterface::GetInstance()->RegTimePoint(&CSkillNest::StopTimer, this, 0, 0, 0, 0, m_info->continueTime);

	m_id = m_info->nextActionSkill;
	m_info = ninfo;
}

void CSkillNest::ContinueTimer(void * arg)
{
	if(m_bEnd)
		return;

	if(m_cycleTimer.IsValid())
	{
		m_cycleTimer.SetNull();
	}
	if(m_id <= m_nextSkill)
	{
		SynchFire();
		Amerce();
		Start();
	}
}

void CSkillNest::StopTimer(void * arg)
{
	if(m_bEnd)
	{
		return;
	}

	if(m_stopTimer.IsValid())
	{
		m_stopTimer.SetNull();
	}

	OnEnd();
}

void CSkillNest::Result()
{
	if(m_bEnd)
		return;

	//获取目标对象obj
	Smart_Ptr<CreatureObj> targetObj;
	CSceneMgr::GetInstance()->GetObject(m_owner->GetMapID(),m_targetKey,targetObj);

	if(m_info->effectFirstType[0] == eMove)
	{
		m_owner->RelocatePos(m_xpos, m_ypos);
	}

	//属性变化
	map<int, vector<AttrChange> > allAttr;
	map<int, vector<AttrChange> > myselfAttr;

	for(int i=0; i<3; ++i)
	{
		vector<Smart_Ptr<CreatureObj> > targets;// 查找受影响的目标

		if(m_info->targetCount > 0)
		{
			if(m_info->targetCount > 1 && m_info->rangeType == eRangeMyself)
			{
				CSceneMgr::GetInstance()->GetSkillObject(m_owner, m_owner->GetXNewPos(), m_owner->GetYNewPos(), targetObj, m_info->m_EffectTargetType[i], m_info->targetCount,
						m_info->rangeType, m_info->rangeLength,m_info->rangeWidth, targets);
			}
			else
			{
				if(m_xpos > 0 || m_ypos > 0)
				{
					CSceneMgr::GetInstance()->GetSkillObject(m_owner, m_xpos, m_ypos, targetObj, m_info->m_EffectTargetType[i], m_info->targetCount,
							m_info->rangeType, m_info->rangeLength,m_info->rangeWidth, targets);
				}
				else if(m_info->targetCount > 1 && (bool)targetObj)
				{
					CSceneMgr::GetInstance()->GetSkillObject(m_owner, targetObj->GetXNewPos(), targetObj->GetYNewPos(), targetObj, m_info->m_EffectTargetType[i], m_info->targetCount,
							m_info->rangeType, m_info->rangeLength,m_info->rangeWidth, targets);
				}
				else if(m_info->targetCount == 1 && (bool)targetObj)
				{
					if(0 == m_owner->IsAttacked(targetObj, m_info->m_EffectTargetType[i]==eSkillTargetTypeNull ? eEnemyTarget : m_info->m_EffectTargetType[i]))
					{
						targets.push_back(targetObj);
					}
				}
			}
		}

		// 开始产生作用
		ResultEffect(i, targets, allAttr, myselfAttr);
	}

	//属性变更
	ResultEffectAttrChange(allAttr, myselfAttr);

}


//---------------吟唱技能----------------
CSkillIntonate::CSkillIntonate(int id, Smart_Ptr<CreatureObj> & owner, int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info) :
CSkillBase(id, owner, key, xpos, ypos, onlyid, info),
m_IntonateTime(0)
{

}

CSkillIntonate::~CSkillIntonate()
{

}

void CSkillIntonate::Init(int id, Smart_Ptr<CreatureObj> & owner,int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info)
{
	CSkillBase::InitInfo(id, owner, key, xpos, ypos, onlyid, info);
	m_IntonateTime = 0;
}

void CSkillIntonate::Release()
{
	CSkillBase::Release();
}

// 开始技能的释放
bool CSkillIntonate::BeginStart()
{
	m_IntonateTime = CUtil::GetNowSecond();

	m_IntonateTimer.SetNull();

	if(m_info->m_IntonateTime <= 0)
	{
		intonateTimer(NULL);
	}
	else
	{
		m_IntonateTimer = TimerInterface::GetInstance()->RegTimePoint(&CSkillIntonate::intonateTimer,this,0,0,0,0,m_info->m_IntonateTime);

		m_BreakEvent.SetNull();

		m_DoingEvent.SetNull();

		m_BreakEvent = RegEvent(m_owner,OBJECT_ENTER_STATE,this,&CSkillIntonate::handleBreakState);

		m_DoingEvent = RegEvent(m_owner,OBJECT_EXCUTE_DOING,this,&CSkillIntonate::handleBreakDoing);
	}

	return true;
}

bool CSkillIntonate::EndStart()
{
	OnEnd();

	return true;
}

void CSkillIntonate::OnEndImp()
{
	if(m_owner->GetType() == ePlayer && m_info->limitPro != eProfessionMax && m_info->fType != eAssistSkill)
	{
		Smart_Ptr<Player> obj = m_owner;

		obj->GetSkillMgr()->SetSkillNull(GetOnlyID());
	}
	else if(m_info->limitPro == eProfessionMax)
	{
		Smart_Ptr<Player> obj = m_owner;

		obj->GetSkillMgr()->SetBigSkill(0);
	}

	m_releaseTimer.SetNull();

	m_IntonateTimer.SetNull();

	m_BreakEvent.SetNull();

	m_DoingEvent.SetNull();

	return ;
}

//获取同步信息
bool CSkillIntonate::getSynData(PlayerSynch::SynchPlayerBase& data)
{
	if(!m_IntonateTimer.IsValid())
	{
		return false;
	}

	PlayerSynch::SynchSkill* tempData = data.mutable_skilldata();

	tempData->set_skillid(m_id);
	tempData->set_objectkey(m_owner->GetKey());
	tempData->set_posx(m_xpos);
	tempData->set_posy(m_ypos);
	tempData->set_surplustime(CUtil::GetNowSecond() - m_IntonateTime);

	return true;
}

// 飞行定时器
void CSkillIntonate::FlyTimer(void * arg)
{
	m_releaseTimer.SetNull();

	Result();

	if (m_bEnd == false)
	{
		OnEnd();
	}

	return ;
}

void CSkillIntonate::intonateTimer(void* p)
{
	m_IntonateTimer.SetNull();

	m_BreakEvent.SetNull();

	m_DoingEvent.SetNull();

	// 计算施法者位置与目标点的距离
	int delayTimer = 0;
	if (m_info->flySpeed > 0)
	{
		float fDis = 0.0;
		int xtemp = 0;
		int ytemp = 0;

		if(m_xpos <= 0 && m_ypos <= 0)
		{
			//获取目标对象obj
			Smart_Ptr<CreatureObj> targetObj;
			CSceneMgr::GetInstance()->GetObject(m_owner->GetMapID(),m_targetKey,targetObj);

			if(!(bool)targetObj || targetObj->IsDead() || !targetObj->IsInMap())
			{
				xtemp = abs(m_owner->GetXNewPos());
				ytemp = abs(m_owner->GetYNewPos());
			}
			else
			{
				xtemp = abs(m_owner->GetXNewPos() - targetObj->GetXNewPos());
				ytemp = abs(m_owner->GetYNewPos() - targetObj->GetYNewPos());
			}
		}
		else
		{
			xtemp = abs(m_owner->GetXNewPos() - m_xpos);
			ytemp = abs(m_owner->GetYNewPos() - m_ypos);
		}

		fDis = sqrt(xtemp * xtemp + ytemp * ytemp);

		// 计算技能产生效果所需要的时间,飞行时间
		delayTimer = (int)(fDis * 1000 / m_info->flySpeed);
	}

	if (delayTimer <= 200)
	{
		// 少于0.2秒，立马产生作用
		FlyTimer(NULL);
	}
	else
	{
		// 注册定时器
		m_releaseTimer.SetNull();

		m_releaseTimer = TimerInterface::GetInstance()->RegTimePoint(&CSkillIntonate::FlyTimer, this, 0, 0, 0, 0, delayTimer);
	}

	return ;
}

//自身死亡事件
bool CSkillIntonate::handleSelfDead(const EventArgs & e)
{
	if(m_IntonateTimer.IsValid())
	{
		breakDealWith();
	}

	OnEnd();

	return true;
}

//打断事件
bool CSkillIntonate::handleBreakState(const EventArgs & e)
{
	const ObjectEnterStateArgs& tempArgs = static_cast<const ObjectEnterStateArgs& >(e);

	EAttackState tempClientState = Effect::switchEnumToClient(tempArgs.m_State);

	for(size_t i = 0;i < m_info->m_BreakStateList.size();++i)
	{
		if(tempClientState != m_info->m_BreakStateList[i])
		{
			continue;
		}

		breakDealWith();

		return true;
	}

	return true;
}

//行为打断事件
bool CSkillIntonate::handleBreakDoing(const EventArgs & e)
{
	const ObjectDoingArgs& tempArgs = static_cast<const ObjectDoingArgs& >(e);

	for(size_t i = 0;i < m_info->m_BreakDoingList.size();++i)
	{
		if(tempArgs.m_Doing != m_info->m_BreakDoingList[i])
		{
			continue;
		}

		breakDealWith();

		return true;
	}

	return true;
}

void CSkillIntonate::breakDealWith()
{
	if(m_bEnd)
	{
		return ;
	}

	CharBase::SynchObjectAttr tempAttAlter;
	CharBase::CharAttChange*  tempAtt = tempAttAlter.mutable_attr();;

	tempAttAlter.set_key(m_owner->GetKey());
	tempAttAlter.set_attkey(m_owner->GetKey());

	tempAtt->add_type(eRemoveState);
	tempAtt->add_value(m_id);
	tempAtt->add_state(eIntonate);

	CSceneMgr::GetInstance()->SendSynchAttrChange(tempAttAlter,m_owner->GetMapID(),m_owner->GetXNewPos(),m_owner->GetYNewPos(),true);

	OnEnd();

	return ;
}

// 执行技能的结果
void CSkillIntonate::Result()
{
	if(m_bEnd)
		return;

	//获取目标对象obj
	Smart_Ptr<CreatureObj> targetObj;
	CSceneMgr::GetInstance()->GetObject(m_owner->GetMapID(),m_targetKey,targetObj);

	if(m_info->effectFirstType[0] == eMove)
	{
		m_owner->RelocatePos(m_xpos, m_ypos);
	}

	//属性变化
	map<int, vector<AttrChange> > allAttr;
	map<int, vector<AttrChange> > myselfAttr;

	for(int i=0; i<3; ++i)
	{
		vector<Smart_Ptr<CreatureObj> > targets;// 查找受影响的目标

		if(m_info->targetCount > 0)
		{
			if(m_info->targetCount > 1 && m_info->rangeType == eRangeMyself)
			{
				CSceneMgr::GetInstance()->GetSkillObject(m_owner, m_owner->GetXNewPos(), m_owner->GetYNewPos(), targetObj, m_info->m_EffectTargetType[i], m_info->targetCount,
						m_info->rangeType, m_info->rangeLength,m_info->rangeWidth, targets);
			}
			else
			{
				if(m_xpos > 0 || m_ypos > 0)
				{
					CSceneMgr::GetInstance()->GetSkillObject(m_owner, m_xpos, m_ypos, targetObj, m_info->m_EffectTargetType[i], m_info->targetCount,
							m_info->rangeType, m_info->rangeLength,m_info->rangeWidth, targets);
				}
				else if(m_info->targetCount > 1 && (bool)targetObj)
				{
					CSceneMgr::GetInstance()->GetSkillObject(m_owner, targetObj->GetXNewPos(), targetObj->GetYNewPos(), targetObj, m_info->m_EffectTargetType[i], m_info->targetCount,
							m_info->rangeType, m_info->rangeLength,m_info->rangeWidth, targets);
				}
				else if(m_info->targetCount == 1 && (bool)targetObj)
				{
					if(0 == m_owner->IsAttacked(targetObj, m_info->m_EffectTargetType[i]==eSkillTargetTypeNull ? eEnemyTarget : m_info->m_EffectTargetType[i]))
					{
						targets.push_back(targetObj);
					}
				}
			}
		}

		// 开始产生作用
		ResultEffect(i, targets, allAttr, myselfAttr);
	}

	//属性变更
	ResultEffectAttrChange(allAttr, myselfAttr);

}






