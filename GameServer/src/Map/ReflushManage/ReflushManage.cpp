#include "ReflushManage.h"
#include "../../Object/Monster/Monster.h"
#include "../GameZone.h"


ReflushGroup::ReflushGroup(const ReflushData* pData,ReflushManage* pFather) :
m_Stage(eNotReflushStage),
m_pFileData(pData),
m_pFather(pFather)
{
	m_DataList.resize(m_pFileData->m_OneList.size());

	registerTimer();

	if(ReflushFile::GetInstance()->isInTime(m_pFileData->m_StartDate,m_pFileData->m_EndData) && m_pFileData->m_IntervalTime != 0)
	{
		reflushTimer(NULL);
	}
}

ReflushGroup::~ReflushGroup()
{
	m_Timer.SetNull();

	m_EndTimer.SetNull();
}

bool ReflushGroup::monsterLeave(Monster& monster)
{
	//移除怪物时,正在操作该迭代器
	if(eReflushStageNull == m_Stage)
	{
		return false;
	}

	for(size_t i = 0;i < m_DataList.size();++i)
	{
		vector<int>::iterator tInIt = m_DataList[i].m_KeyList.begin();
		for(;tInIt != m_DataList[i].m_KeyList.end();)
		{
			if(*tInIt != monster.GetKey())
			{
				++tInIt;
				continue;
			}
			else
			{
				tInIt = m_DataList[i].m_KeyList.erase(tInIt);

				return true;
			}
		}
	}

	return false;
}

void ReflushGroup::reflushMonster()
{
	vector<int> tKeyList;
	vector<Pos> tPosList;

	for(size_t i = 0;i < m_pFileData->m_OneList.size();++i)
	{
		tKeyList.clear();
		tPosList.clear();

		short tIndex = m_DataList[i].m_Index;
		if(tIndex == -1)
		{
			tIndex = rand() % m_pFileData->m_OneList[i].m_PosList.size();
		}

		m_DataList[i].m_Index = tIndex;

		tPosList.push_back(m_pFileData->m_OneList[i].m_PosList[tIndex]);

		if((short)m_DataList[i].m_KeyList.size() >= m_pFileData->m_OneList[i].m_Counts)
		{
			continue ;
		}

		m_pFather->getGameZone()->CreateMonster(m_pFileData->m_OneList[i].m_MonsterID,
				m_pFileData->m_OneList[i].m_Counts - m_DataList[i].m_KeyList.size(),
				tPosList,
				&tKeyList);

		for(size_t j = 0;j < tKeyList.size();++j)
		{
			m_DataList[i].m_KeyList.push_back(tKeyList[j]);
		}
	}

	return ;
}

//刷新定时器回调,开始刷新和间隔刷共用一个定时器
void ReflushGroup::reflushTimer(void* p)
{
	//目前没有年定时器，只能每月回调一次，过该时间段，该对像销毁
	if(!ReflushFile::GetInstance()->isInTime(m_pFileData->m_StartDate,m_pFileData->m_EndData))
	{
		//不是该时间段回调，则再注册开始定时器
		m_Stage = eNotReflushStage;

		registerTimer();
	}
	else
	{
		reflushMonster();

		//第一次刷怪
		if(m_Stage == eNotReflushStage)
		{
			m_Stage = eReflushIngStage;

			registerTimer();
		}
	}

	return ;
}

void ReflushGroup::endTimer(void* p)
{
	//开始定时器没有被回调过，不能结束
	if(m_Stage != eReflushIngStage)
	{
		return ;
	}

	m_Stage = eReflushStageNull;

	removeAllMonster();

	end();

	return ;
}

//注册定时器
void ReflushGroup::registerTimer()
{
	m_Timer.SetNull();

	if(m_Stage == eNotReflushStage)
	{
		//reflushTimer会调用该函数，防止两个时间有重复
		m_Timer = TimerInterface::GetInstance()->RegMonthPointRepeatedTime(
				&ReflushGroup::reflushTimer,
				this,
				0,
				m_pFileData->m_StartDate.m_Day,
				m_pFileData->m_StartDate.m_Hour,
				m_pFileData->m_StartDate.m_Minute,
				0);

		m_EndTimer.SetNull();

		if(m_pFileData->m_EndData.m_Day != 0)
		{
			m_EndTimer = TimerInterface::GetInstance()->RegMonthPointRepeatedTime(
					&ReflushGroup::endTimer,
					this,
					0,
					m_pFileData->m_EndData.m_Day,
					m_pFileData->m_EndData.m_Hour,
					m_pFileData->m_EndData.m_Minute,
					0);
		}
	}
	else
	{
		//只刷一波
		if(m_pFileData->m_IntervalTime == 0 && m_pFileData->m_EndData.m_Day == 0)
		{
			//刷完销毁
			end();
		}
		else if(m_pFileData->m_IntervalTime != 0)
		{
			m_Timer = TimerInterface::GetInstance()->RegRepeatedTime(&ReflushGroup::reflushTimer, this, 0, 0, 0, 0,
					m_pFileData->m_IntervalTime,
					m_pFileData->m_IntervalTime);
		}
	}

	return  ;
}

void ReflushGroup::end()
{
	m_Stage = eReflushStageNull;

	m_Timer.SetNull();

	m_EndTimer.SetNull();

	m_pFather->clear();

	return ;
}

void ReflushGroup::removeAllMonster()
{
	for(size_t i = 0;i < m_DataList.size();++i)
	{
		vector<int>::iterator tOneIt = m_DataList[i].m_KeyList.begin();
		for(;tOneIt != m_DataList[i].m_KeyList.end();++tOneIt)
		{
			Smart_Ptr<Object> tObject = m_pFather->getGameZone()->GetObjectByKey(*tOneIt);
			if(!tObject)
			{
				continue ;
			}

			if(tObject->GetType() != eMonster)
			{
				continue;
			}

			Smart_Ptr<Monster> tMonster = tObject;
			tMonster->SetDead(NULL);
		}
	}

	return ;
}

//---------------------刷新管理---------------------------
ReflushManage::ReflushManage(CGameZone* pGameZone,const vector<ReflushData>& dataList) :
m_pGameZone(pGameZone)
{
	excute(dataList);
}

ReflushManage::~ReflushManage()
{

}

void ReflushManage::monsterLeave(Monster& monster)
{
	for(size_t i = 0;i < m_GroupList.size();++i)
	{
		if(m_GroupList[i]->monsterLeave(monster))
		{
			return ;
		}
	}

	return ;
}

void ReflushManage::clear()
{
	vector<Smart_Ptr<ReflushGroup> >::iterator tIt = m_GroupList.begin();
	for(;tIt != m_GroupList.end();)
	{
		if((*tIt)->getStage() != eReflushStageNull)
		{
			++tIt;
			continue ;
		}

		tIt = m_GroupList.erase(tIt);
	}

	return ;
}

void ReflushManage::excute(const vector<ReflushData>& dataList)
{
	for(size_t i = 0;i < dataList.size();++i)
	{
		ReflushGroup* tGroup = new ReflushGroup(&dataList[i],this);

		m_GroupList.push_back(tGroup);
	}

	return ;
}









