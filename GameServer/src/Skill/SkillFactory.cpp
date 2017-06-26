/*
 * SkillFactory.cpp
 *
 *  Created on: 2014年1月21日
 *      Author: helh
 */
#include "SkillFactory.h"
#include "SkillClass.h"
#include "./SkillLoader/SkillLoader.h"
#include "Log/Logger.h"

DWORD CSkillFactory::m_onlyID = 0;
std::vector<CSkillBase*> CSkillFactory::g_delSkillList;
std::queue<CSkillBase*> CSkillFactory::m_SkillBlink;
std::queue<CSkillBase*> CSkillFactory::m_SkillDelay;
std::queue<CSkillBase*> CSkillFactory::m_SkillNest;
std::queue<CSkillBase*> CSkillFactory::m_SkillIntonate;

CSkillBase* CSkillFactory::Create(int id, Smart_Ptr<CreatureObj> & caster,int key, int xpos, int ypos)
{
	SkillInfo *info = 0;
	info = SkillLoader::GetInstance()->GetSkillInfoByID(id);
	if (info == 0)
		return NULL;

	CSkillBase* pSkillBase = GetSkillObject(info->sType);
	if(pSkillBase == NULL)
		return NULL;

	pSkillBase->Init(id, caster, key, xpos, ypos, ++m_onlyID, info);

	return pSkillBase;
}

void CSkillFactory::Update()
{
	if (g_delSkillList.size() != 0)
	{
		for (size_t i = 0; i < g_delSkillList.size(); ++i)
		{
			DeleteSkill(g_delSkillList[i]);
		}

		g_delSkillList.clear();
	}
}

void CSkillFactory::FreeSkill(CSkillBase* pBase)
{
	g_delSkillList.push_back(pBase);
}


void CSkillFactory::DeleteSkill(CSkillBase* pBase)
{
	try
	{
		if(pBase)
		{
			//只回收，不删除
			Recycle(pBase);

			//delete pBase;
			//pBase = 0;
		}
	}
	catch(std::exception& e)
	{
		LOG_ERROR(FILEINFO, "Delete skill error[%s]", e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "Delete skill unknown error");
	}
}

void CSkillFactory::InitObject()
{
	CreateObject(eSkillTypeBlink, eSKILLINITMAXCOUNT);
	CreateObject(eSkillTypeDelay, eSKILLINITMAXCOUNT);
	CreateObject(eSkillTypeNest, eSKILLINITMAXCOUNT);
	CreateObject(eSkillTypeIntonate, eSKILLINITMAXCOUNT);
}

CSkillBase* CSkillFactory::GetSkillObject(int type)
{
	CSkillBase* pSkillBase = 0;
	switch(type)
	{
		case eSkillTypeBlink:
		{
			if(m_SkillBlink.size() == 0)
			{
				CreateObject(eSkillTypeBlink, eSKILLINITMAXCOUNT/2);
			}

			pSkillBase = m_SkillBlink.front();
			m_SkillBlink.pop();
			break;
		}

		case eSkillTypeDelay:
		{
			if(m_SkillDelay.size() == 0)
			{
				CreateObject(eSkillTypeDelay, eSKILLINITMAXCOUNT/2);
			}

			pSkillBase = m_SkillDelay.front();
			m_SkillDelay.pop();
			break;
		}

		case eSkillTypeNest:
		{
			if(m_SkillNest.size() == 0)
			{
				CreateObject(eSkillTypeNest, eSKILLINITMAXCOUNT/2);
			}

			pSkillBase = m_SkillNest.front();
			m_SkillNest.pop();
			break;
		}

		case eSkillTypeIntonate:
		{
			if(m_SkillIntonate.size() == 0)
			{
				CreateObject(eSkillTypeIntonate, eSKILLINITMAXCOUNT/2);
			}

			pSkillBase = m_SkillIntonate.front();
			m_SkillIntonate.pop();
			break;
		}

		default:
		{
			LOG_ERROR(FILEINFO, "create skill but type is error");
			return 0;
		}
		break;
	}

	return pSkillBase;
}

void CSkillFactory::CreateObject(int type, int size)
{
	CSkillBase* pSkillBase = 0;
	Smart_Ptr<CreatureObj> obj;

	switch(type)
	{
		case eSkillTypeBlink:
		{
			for(int i=0; i<size; ++i)
			{
				pSkillBase = new CSkillBlink(0, obj, 0, 0, 0, 0, NULL);
				m_SkillBlink.push(pSkillBase);
			}
		}
		break;

		case eSkillTypeDelay:
		{
			for(int i=0; i<size; ++i)
			{
				pSkillBase = new CSkillDelay(0, obj, 0, 0, 0, 0, NULL);
				m_SkillDelay.push(pSkillBase);
			}
		}
		break;

		case eSkillTypeNest:
		{
			for(int i=0; i<size; ++i)
			{
				pSkillBase = new CSkillNest(0, obj, 0, 0, 0, 0, NULL);
				m_SkillNest.push(pSkillBase);
			}
		}
		break;

		case eSkillTypeIntonate:
		{
			for(int i=0; i<size; ++i)
			{
				pSkillBase = new CSkillIntonate(0, obj, 0, 0, 0, 0, NULL);
				m_SkillIntonate.push(pSkillBase);
			}
		}
		break;

		default:
		{

		}
		break;
	}

}

void CSkillFactory::Recycle(CSkillBase* pBase)
{
	if(pBase == NULL)
		return;

	int type = pBase->GetType();

	pBase->Release();

	switch(type)
	{
		case eSkillTypeBlink:
		{
			m_SkillBlink.push(pBase);
		}
		break;

		case eSkillTypeDelay:
		{
			m_SkillDelay.push(pBase);
		}
		break;

		case eSkillTypeNest:
		{
			m_SkillNest.push(pBase);
		}
		break;

		case eSkillTypeIntonate:
		{
			m_SkillIntonate.push(pBase);
		}
		break;

		default:
			break;
	}

}

void CSkillFactory::DestroyAll()
{
	while(!m_SkillBlink.empty())
	{
		CSkillBase* pSkill = m_SkillBlink.front();
		if(pSkill != NULL)
		{
			delete pSkill;
			pSkill = NULL;
		}
		m_SkillBlink.pop();
	}

	while(!m_SkillDelay.empty())
	{
		CSkillBase* pSkill = m_SkillDelay.front();
		if(pSkill != NULL)
		{
			delete pSkill;
			pSkill = NULL;
		}
		m_SkillDelay.pop();
	}

	while(!m_SkillNest.empty())
	{
		CSkillBase* pSkill = m_SkillNest.front();
		if(pSkill != NULL)
		{
			delete pSkill;
			pSkill = NULL;
		}
		m_SkillNest.pop();
	}

	while(!m_SkillIntonate.empty())
	{
		CSkillBase* pSkill = m_SkillIntonate.front();
		if(pSkill != NULL)
		{
			delete pSkill;
			pSkill = NULL;
		}
		m_SkillIntonate.pop();
	}

}



