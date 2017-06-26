/*
 * BuffClass.cpp
 *
 *  Created on: 2014年2月12日
 *      Author: helh
 */

#include "BuffClass.h"
#include "Log/Logger.h"
#include "util.h"
#include "SkillLoader/SkillLoader.h"
#include "../Object/Creature.h"
#include "../Object/Player/Player.h"
#include "CharDefine.h"
#include "../Attribute/AttributeCreature.h"

BuffForever::BuffForever(Smart_Ptr<CreatureObj>& owner, Smart_Ptr<CreatureObj>& caster, int id, DWORD onlyID)
: BuffBase(owner, 0, id, onlyID),
  m_firstExcute(true)
{

}

BuffForever::~BuffForever()
{

}

void BuffForever::Init(Smart_Ptr<CreatureObj> & owner, Smart_Ptr<CreatureObj> & caster, DWORD onlyID, int id, int alltime, int timer, int num)
{
	m_firstExcute = true;
	BuffBase::InitInfo(owner, caster->GetKey(), id, onlyID);
}

void BuffForever::Release()
{
	BuffBase::Release();
}

bool BuffForever::Start(bool isDel)
{
	if(!m_firstExcute || !isDel)
	{
		return true;
	}

	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
	if (info == NULL)
		return false;

	if(info->isDamage != 0)
	{
		bool isSyn = false;
		if(info->isResult)
		{
			isSyn = true;
		}

		SendBuffOperateMessage(true, isSyn);
	}

	// 立即执行一次
	Execute();

	return true;
}

//获得类型
int BuffForever::GetType()
{
	return eBuffForever;
}

void BuffForever::End()
{
	OnEnd();
}

void BuffForever::SetInfo(PlayerInfo::SkillInfo *info, bool isDel)
{
	PlayerInfo::BuffInfo *buf = info->add_buf();

	buf->set_type(eBuffForever);
	buf->set_id(m_id);
	buf->set_isdel(isDel);
}

void BuffForever::OnEndImp()
{
	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
	if(info == NULL)
	{
		LOG_ERROR(FILEINFO, "remove buff but buff is null");

		return ;
	}

	if(info->isDamage)
	{
		bool isSyn = false;
		if(info->isResult)
		{
			isSyn = true;
		}

		SendBuffOperateMessage(false, isSyn);
	}
}


BuffCount::BuffCount(Smart_Ptr<CreatureObj>& owner, Smart_Ptr<CreatureObj>& caster, int id, DWORD onlyID, int alltime, int allCount)
: BuffBase(owner, 0, id, onlyID),
m_spaceTimer(1),
m_allCount(allCount),
m_endTime(CUtil::GetNowSecond() + alltime)
{

}

BuffCount::~BuffCount()
{
	if(m_stime.IsValid())
	{
		m_stime.SetNull();
	}
}

void BuffCount::Init(Smart_Ptr<CreatureObj> & owner, Smart_Ptr<CreatureObj> & caster, DWORD onlyID, int id, int alltime, int timer, int num)
{
	BuffBase::InitInfo(owner, caster->GetKey(), id, onlyID);
	m_spaceTimer = alltime / (timer*num);
	m_allCount = timer*num;
	m_endTime = CUtil::GetNowSecond() + alltime;
}

void BuffCount::Release()
{
	BuffBase::Release();

	if(m_stime.IsValid())
	{
		m_stime.SetNull();
	}
}


bool BuffCount::Start(bool isDel)
{
	if(m_allCount <= 0)
		return false;

	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
	if (info == NULL)
		return false;

	if(info->isDamage != 0)
	{
		bool isSyn = false;
		if(info->isResult)
		{
			isSyn = true;
		}

		SendBuffOperateMessage(true, isSyn, (int)(m_spaceTimer * (m_allCount - 1)));
	}

	if(isDel)
	{
		// 立即执行一次效果，然后注册定时器
		Execute();
		if (m_curCount >= m_allCount)
		{
			// 只需要执行一次，则立即结束
			OnEnd();

			return true;
		}
	}
	if(m_stime.IsValid())
	{
		m_stime.SetNull();
	}
	m_stime = TimerInterface::GetInstance()->RegRepeatedTime(&BuffCount::CycTimer, this, 0, 0, 0, 0, (int)m_spaceTimer, (int)m_spaceTimer);

	return true;
}

//获得类型
int BuffCount::GetType()
{
	return eBuffCount;
}

void BuffCount::CycTimer(void* obj)
{
	RegCycTimer();
}

void BuffCount::RegCycTimer()
{
	Execute();
//	printf("\n11111111111111111111111111111111________________%lld________________\n", CUtil::GetNowSecond());
	if (m_bEnd || m_curCount >= m_allCount)
	{
		// 结束
		OnEnd();
		return ;
	}

	// 间隔时间为零，则立即结束
	if (m_spaceTimer <= 0)
	{
		OnEnd();
		return ;
	}
}

void BuffCount::End()
{
	if(m_stime.IsValid())
	{
		m_stime.SetNull();
	}

	OnEnd();
}

void BuffCount::SetInfo(PlayerInfo::SkillInfo *info, bool isDel)
{
	PlayerInfo::BuffInfo *buf = info->add_buf();

	buf->set_type(eBuffCount);
	buf->set_id(m_id);
	buf->set_endtime(m_endTime);
	buf->set_offtime(CUtil::GetNowSecond());
	buf->set_isdel(isDel);
}

void BuffCount::OnEndImp()
{
	if(m_stime.IsValid())
	{
		m_stime.SetNull();
	}

	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
	if(info == NULL)
	{
		LOG_ERROR(FILEINFO, "remove buff but buff is null");

		return ;
	}

	if(info->isDamage)
	{
		bool isSyn = false;
		if(info->isResult)
		{
			isSyn = true;
		}

		SendBuffOperateMessage(false, isSyn);
	}
}


BuffOnceTimer::BuffOnceTimer(Smart_Ptr<CreatureObj>& owner, Smart_Ptr<CreatureObj>& caster, int id, DWORD onlyID, int allTime)
: BuffBase(owner, 0, id, onlyID),
m_allTimer(allTime),
m_endTime(CUtil::GetNowSecond() + allTime)
{

}

BuffOnceTimer::~BuffOnceTimer()
{
	if(m_stime.IsValid())
	{
		m_stime.SetNull();
	}
}

void BuffOnceTimer::Init(Smart_Ptr<CreatureObj> & owner, Smart_Ptr<CreatureObj> & caster, DWORD onlyID, int id, int alltime, int timer, int num)
{
	BuffBase::InitInfo(owner, caster->GetKey(), id, onlyID);

	m_allTimer = alltime;
	m_endTime = CUtil::GetNowSecond() + alltime;

}

void BuffOnceTimer::Release()
{
	BuffBase::Release();

	if(m_stime.IsValid())
	{
		m_stime.SetNull();
	}
}

//获得类型
int BuffOnceTimer::GetType()
{
	return eBuffOnce;
}

//重启BUFF时间
bool BuffOnceTimer::ReStart()
{
	if(m_stime.IsValid())
	{
		m_stime.SetNull();
	}

	m_endTime = CUtil::GetNowSecond() + m_allTimer;

	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
	if (info == NULL)
		return false;

	if(info->isDamage != 0)
	{
		bool isSyn = false;
		if(info->isResult)
		{
			isSyn = true;
		}

		SendBuffOperateMessage(true, isSyn, m_allTimer);
	}

	// 注册结束定时器
	if(m_stime.IsValid())
	{
		m_stime.SetNull();
	}
	m_stime = TimerInterface::GetInstance()->RegTimePoint(&BuffOnceTimer::EndTimer,this, 0, 0, 0, 0, m_allTimer);
	return true;
}

bool BuffOnceTimer::Start(bool isDel)
{
	if(m_allTimer <= 0)
		return false;

	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
	if (info == NULL)
		return false;

	if(info->isDamage != 0)
	{
		bool isSyn = false;
		if(info->isResult)
		{
			isSyn = true;
		}

		SendBuffOperateMessage(true, isSyn, m_allTimer);
	}
//	printf("\n0000000000000000000000000000000000000__%lld________________\n", CUtil::GetNowSecond());

	if(isDel)
	{
		// 执行一次
		Execute();
	}

	// 注册结束定时器
	if(m_stime.IsValid())
	{
		m_stime.SetNull();
	}
	m_stime = TimerInterface::GetInstance()->RegTimePoint(&BuffOnceTimer::EndTimer,this, 0, 0, 0, 0, m_allTimer);

	return true;
}

void BuffOnceTimer::End()
{
//	printf("\n11111111111111111111111111111__%lld________________\n", CUtil::GetNowSecond());
	if(m_stime.IsValid())
	{
		m_stime.SetNull();
	}
	OnEnd();
}

void BuffOnceTimer::SetInfo(PlayerInfo::SkillInfo *info, bool isDel)
{
	PlayerInfo::BuffInfo *buf = info->add_buf();

	buf->set_type(eBuffOnce);
	buf->set_id(m_id);
	buf->set_endtime(m_endTime);
	buf->set_offtime(CUtil::GetNowSecond());
//	printf("\neeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee__%lld________________\n", CUtil::GetNowSecond());
	buf->set_isdel(isDel);
}

void BuffOnceTimer::EndTimer(void* obj)
{
	//cout<<endl<<"+++++++++++++++++++++++技能时间到了！！！！！！！！！！！"<<endl;
	End();
}

void BuffOnceTimer::OnEndImp()
{
	if(m_stime.IsValid())
	{
		m_stime.SetNull();
	}

	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
	if(info == NULL)
	{
		LOG_ERROR(FILEINFO, "remove buff but buff is null");

		return ;
	}

	if(info->isDamage)
	{
		bool isSyn = false;
		if(info->isResult)
		{
			isSyn = true;
		}

		SendBuffOperateMessage(false, isSyn);
	}
}


BuffCapInvlTime::BuffCapInvlTime(Smart_Ptr<CreatureObj>& owner, Smart_Ptr<CreatureObj>& caster, int id, DWORD onlyID,
		int cap, int invlTime) :
		BuffBase(owner, 0, id, onlyID), m_intervalTime(invlTime), m_capacity(cap)
{

}

BuffCapInvlTime::~BuffCapInvlTime()
{
	if(m_time.IsValid())
	{
		m_time.SetNull();
	}
}

void BuffCapInvlTime::Init(Smart_Ptr<CreatureObj> & owner, Smart_Ptr<CreatureObj> & caster, DWORD onlyID, int id, int alltime, int timer, int num)
{
	BuffBase::InitInfo(owner, caster->GetKey(), id, onlyID);
	m_intervalTime = timer;
	m_capacity = alltime;
}

void BuffCapInvlTime::Release()
{
	BuffBase::Release();

	if(m_time.IsValid())
	{
		m_time.SetNull();
	}
}

//获得类型
int BuffCapInvlTime::GetType()
{
	return eBuffCapInvlTime;
}

bool BuffCapInvlTime::ReStart()
{
	return true;
}

// 开始
bool BuffCapInvlTime::Start(bool isDel)
{
	// 当前容量不能小于0
	if(m_capacity <= 0)
			return false;

	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
	if (info == NULL)
		return false;

	if(info->isDamage != 0)
	{
		bool isSyn = false;
		if(info->isResult)
		{
			isSyn = true;
		}

		SendBuffOperateMessage(true, isSyn, m_capacity);
	}

	if(isDel)
	{
		// 执行一次
		Execute();
	}

	// 注册循环定时器
	if(m_time.IsValid())
	{
		m_time.SetNull();
	}
	m_time = TimerInterface::GetInstance()->RegRepeatedTime(&BuffCapInvlTime::LoopsTimer, this, 0, 0, 0, 0, m_intervalTime, m_intervalTime);

	return true;
}

// 结束,外部结束
void BuffCapInvlTime::End()
{
	if (m_time.IsValid())
	{
		m_time.SetNull();
	}

	OnEnd();
}

void BuffCapInvlTime::SetInfo(PlayerInfo::SkillInfo *info, bool isDel)
{
	PlayerInfo::BuffInfo *buf = info->add_buf();

	buf->set_type(eBuffCapInvlTime);
	buf->set_id(m_id);
	//  当前容量
	buf->set_endtime(m_capacity);
	// 间隔时间
	buf->set_offtime(m_intervalTime);
	buf->set_isdel(isDel);
}

// 循环定时器
void BuffCapInvlTime::LoopsTimer(void* obj)
{
	Execute();

	if (m_bEnd || m_capacity <= 0)
	{
		End();
	}
}

// 结束了
void BuffCapInvlTime::OnEndImp()
{
	if(m_time.IsValid())
	{
		m_time.SetNull();
	}

	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(m_id);
	if(info == NULL)
	{
		LOG_ERROR(FILEINFO, "remove buff but buff is null");

		return ;
	}

	if(info->isDamage)
	{
		bool isSyn = false;
		if(info->isResult)
		{
			isSyn = true;
		}

		SendBuffOperateMessage(false, isSyn);
	}
}


//效果  特例
void BuffCapInvlTime::ResultEffect(Smart_Ptr<CreatureObj> target,const BuffInfo* info, vector<AttrChange> &toMyself, vector<AttrChange> &toOther)
{
	if(!checkCondition())
		return ;
	
	int param = (int)info->m_pParam->m_Param;

	switch (info->useType)
	{
	case eScaleChangeHP:
		{
			AttrChange att;
			bool isAdd = true;
			int hp = 0;
			int curHP = m_owner->getAttManage()->getValue<int>(eCharHP); // 玩家当前血量

			if (m_owner->getAttManage()->getValue<int>(eCharHPMax) == curHP || m_owner->IsDead() || GetCurCap() <= 0)
			{
				break;
			}

			att.state = m_id; // 当是血包和蓝包时  state给BuffID
			if(param < 0)
			{
				isAdd = false;
				param = 0 - param;
			}

			hp = (int)(m_owner->getAttManage()->getValue<int>(eCharLv) * param);
			if(hp == 0)
				hp = 1;
			if (hp > m_capacity)
			{
				hp = m_capacity;
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

			hp = (m_owner->getAttManage()->getValue<int>(eCharHP) - curHP);

			if(!isAdd)
			{
				m_owner->UpdateEnemyValue(hp, m_casterKey);
				m_value = 0 - hp;
			}
			else
			{
				m_value = hp;
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

			m_capacity -= hp;

			break;
		}
	case eScaleChangeMP:
		{
			AttrChange att;
			bool isadd = true;
			int mp = 0;
			int curMP = m_owner->getAttManage()->getValue<int>(eCharMP);

			if (m_owner->getAttManage()->getValue<int>(eCharMPMax) == curMP || m_owner->IsDead() || GetCurCap() <= 0)
			{
				break;
			}

			att.state = m_id; // 当是血包和蓝包时  state给BuffID
			if(param < 0)
			{
				isadd = false;
				param = 0 - param;
			}

			mp = (int)(m_owner->getAttManage()->getValue<int>(eCharLv) * param);
			if(mp == 0)
				mp = 1;

			if (mp > m_capacity)
			{
				mp = m_capacity;
			}

			m_owner->ChangeCurMP(mp, isadd);

			mp = (m_owner->getAttManage()->getValue<int>(eCharMP) - curMP);

			if(!isadd)
			{
				m_value = 0 - mp;
			}
			else
			{
				m_value = mp;
			}

			if(param != 0)
			{
				att.type = eCharMP;
				att.value = m_owner->GetTypeValue(att.type);

				toOther.push_back(att);
			}

			m_capacity -= mp;

			break;
		}
	}
}

bool BuffCapInvlTime::checkCondition()
{
	const vector<DWORD>& tempList        = SkillLoader::GetInstance()->getConditionList();
	vector<DWORD>::const_iterator tempIt = tempList.begin();

	DWORD tempMapID = GET_MAP_ID(m_owner->GetMapID());

	for(;tempIt != tempList.end();++tempIt)
	{
		if(tempMapID != *tempIt)
			continue;

		return false;
	}

	return true;
}












