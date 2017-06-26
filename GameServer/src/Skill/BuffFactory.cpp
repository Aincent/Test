/*
 * BuffFactory.cpp
 *
 *  Created on: 2014年2月12日
 *      Author: helh
 */

#include "BuffFactory.h"
#include "BuffClass.h"
#include "./SkillLoader/SkillLoader.h"
#include "util.h"

DWORD BuffFactory::m_onlyID = 0;
std::vector<BuffBase*> BuffFactory::m_removeList;
std::queue<BuffBase*> BuffFactory::m_BuffForever;
std::queue<BuffBase*> BuffFactory::m_BuffCount;
std::queue<BuffBase*> BuffFactory::m_BuffOnceTimer;
std::queue<BuffBase*> BuffFactory::m_BuffCapInvlTime;

BuffBase* BuffFactory::Create(Smart_Ptr<CreatureObj>& owner, Smart_Ptr<CreatureObj>& caster, int id, int num)
{
	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(id);
	if (info == NULL)
		return 0;

	BuffBase* pBuffBase = 0;

	if (info->groupID == 109 || info->groupID == 110)
	{
		// 如果是容量buff, 那么aliveTime是容量  timer是间隔时间
		pBuffBase = GetBufferObject(eBuffCapInvlTime);
		if(pBuffBase)
		{
			pBuffBase->Init(owner, caster, ++m_onlyID, id, info->aliveTime * num, info->timer, 0);
		}
		//pBuffBase = new BuffCapInvlTime(owner, caster, id, ++m_onlyID, info->aliveTime * num, info->timer);
	}
	else if(info->groupID == 200)
	{
		pBuffBase = GetBufferObject(eBuffOnce);
		if(pBuffBase)
		{
			pBuffBase->Init(owner, caster, ++m_onlyID, id, info->aliveTime, 0, 0);
		}
		//pBuffBase = new BuffOnceTimer(owner, caster, id, ++m_onlyID, info->aliveTime);
	}
	else if(info->timer > 0)
	{
		pBuffBase = GetBufferObject(eBuffCount);
		if(pBuffBase)
		{
			pBuffBase->Init(owner, caster, ++m_onlyID, id, info->aliveTime, info->timer, num);
		}
		//pBuffBase = new BuffCount(owner, caster, id, ++m_onlyID, info->aliveTime, info->timer * num);
	}
	else if(info->aliveTime > 0 && info->timer == 0)
	{
		pBuffBase = GetBufferObject(eBuffOnce);
		if(pBuffBase)
		{
			pBuffBase->Init(owner, caster, ++m_onlyID, id, info->aliveTime* num, 0, 0);
		}
		//pBuffBase = new BuffOnceTimer(owner, caster, id, ++m_onlyID, info->aliveTime * num);
	}
	else if(info->aliveTime == 0 && info->timer == 0)
	{
		pBuffBase = GetBufferObject(eBuffForever);
		if(pBuffBase)
		{
			pBuffBase->Init(owner, caster, ++m_onlyID, id, 0, 0, 0);
		}
		//pBuffBase = new BuffForever(owner, caster, id, ++m_onlyID);
	}
	else
	{
		return 0;
	}

	return pBuffBase;
}

BuffBase* BuffFactory::Create(Smart_Ptr<CreatureObj> & owner, Smart_Ptr<CreatureObj> & caster, int id, int type, int alltime, int timer, int num)
{
	if(!owner || !caster)
	{
		return 0;
	}

	BuffBase* pBuffBase = GetBufferObject(type);
	if(pBuffBase == NULL)
	{
		return NULL;
	}

	pBuffBase->Init(owner, caster, ++m_onlyID, id, alltime, timer, num);
	return pBuffBase;
}

void BuffFactory::Update()
{
	if(m_removeList.size() <= 0)
		return;

	std::vector<BuffBase*>::iterator it = m_removeList.begin();
	for (; it != m_removeList.end(); ++it)
	{
		BuffBase* tmpBase = (*it);

		Recycle(tmpBase);
	}

	m_removeList.clear();
}

void BuffFactory::Destory(BuffBase* pBuffer)
{
	m_removeList.push_back(pBuffer);
}

void BuffFactory::InitObject()
{
	CreateBufferObject(eBuffForever, eBUFFERINITMAXCOUNT);
	CreateBufferObject(eBuffCount, eBUFFERINITMAXCOUNT);
	CreateBufferObject(eBuffOnce, eBUFFERINITMAXCOUNT);
	CreateBufferObject(eBuffCapInvlTime, eBUFFERINITMAXCOUNT);
}

BuffBase* BuffFactory::GetBufferObject(int type)
{
	BuffBase* pBuffer = NULL;

	switch(type)
	{
		case eBuffForever:
		{
			if(m_BuffForever.size() == 0)
			{
				CreateBufferObject(type, eBUFFERINITMAXCOUNT/2);
			}

			pBuffer = m_BuffForever.front();
			m_BuffForever.pop();
		}
		break;

		case eBuffCount:
		{
			if(m_BuffCount.size() == 0)
			{
				CreateBufferObject(type, eBUFFERINITMAXCOUNT/2);
			}

			pBuffer = m_BuffCount.front();
			m_BuffCount.pop();
		}
		break;

		case eBuffOnce:
		{
			if(m_BuffOnceTimer.size() == 0)
			{
				CreateBufferObject(type, eBUFFERINITMAXCOUNT/2);
			}

			pBuffer = m_BuffOnceTimer.front();
			m_BuffOnceTimer.pop();
		}
		break;

		case eBuffCapInvlTime:
		{
			if(m_BuffCapInvlTime.size() == 0)
			{
				CreateBufferObject(type, eBUFFERINITMAXCOUNT/2);
			}

			pBuffer = m_BuffCapInvlTime.front();
			m_BuffCapInvlTime.pop();
		}
		break;

		default:
			break;
	}

	return pBuffer;
}

void BuffFactory::CreateBufferObject(int type, int size)
{
	BuffBase* pBuffBase = 0;
	Smart_Ptr<CreatureObj> owner;
	Smart_Ptr<CreatureObj> caster;

	switch(type)
	{
		case eBuffForever:
		{
			for(int i=0; i<size; ++i)
			{
				pBuffBase = new BuffForever(owner, caster, 0, 0);
				m_BuffForever.push(pBuffBase);
			}
		}
		break;

		case eBuffCount:
		{
			for(int i=0; i<size; ++i)
			{
				pBuffBase = new BuffCount(owner, caster, 0, 0, 0, 0);
				m_BuffCount.push(pBuffBase);
			}
		}
		break;

		case eBuffOnce:
		{
			for(int i=0; i<size; ++i)
			{
				pBuffBase = new BuffOnceTimer(owner, caster, 0, 0, 0);
				m_BuffOnceTimer.push(pBuffBase);
			}
		}
		break;

		case eBuffCapInvlTime:
		{
			// 如果是容量buff, alltime是容量, timer是间隔时间
			for(int i=0; i<size; ++i)
			{
				pBuffBase = new BuffCapInvlTime(owner, caster, 0, 0, 0, 0);
				m_BuffCapInvlTime.push(pBuffBase);
			}
		}
		break;

		default:
			break;
	}

}


void BuffFactory::Recycle(BuffBase* pBuffer)
{
	if(pBuffer == NULL)
		return;

	pBuffer->Release();

	switch(pBuffer->GetType())
	{
		case eBuffForever:
		{
			m_BuffForever.push(pBuffer);
		}
		break;

		case eBuffCount:
		{
			m_BuffCount.push(pBuffer);
		}
		break;

		case eBuffOnce:
		{
			m_BuffOnceTimer.push(pBuffer);
		}
		break;

		case eBuffCapInvlTime:
		{
			m_BuffCapInvlTime.push(pBuffer);
		}
		break;

		default:
			break;
	}
}

void BuffFactory::DestroyAll()
{
	while(!m_BuffForever.empty())
	{
		BuffBase* pBuffer = m_BuffForever.front();
		if(pBuffer != NULL)
		{
			pBuffer = NULL;
		}
		m_BuffForever.pop();
	}

	while(!m_BuffCount.empty())
	{
		BuffBase* pBuffer = m_BuffCount.front();
		if(pBuffer != NULL)
		{
			pBuffer = NULL;
		}
		m_BuffCount.pop();
	}

	while(!m_BuffOnceTimer.empty())
	{
		BuffBase* pBuffer = m_BuffOnceTimer.front();
		if(pBuffer != NULL)
		{
			pBuffer = NULL;
		}
		m_BuffOnceTimer.pop();
	}

	while(!m_BuffCapInvlTime.empty())
	{
		BuffBase* pBuffer = m_BuffCapInvlTime.front();
		if(pBuffer != NULL)
		{
			pBuffer = NULL;
		}
		m_BuffCapInvlTime.pop();
	}

}










