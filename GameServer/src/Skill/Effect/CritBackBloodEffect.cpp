/*
 * CritBackBloodEffect.cpp
 *
 *  Created on: 26/10/2015
 *      Author: root
 */

#include "CritBackBloodEffect.h"
#include "../../Object/Player/Player.h"
#include "../../Attribute/AttributeCreature.h"

CritBackBloodEffect::CritBackBloodEffect(BuffBase& father) : Effect(father),m_Counts(0)
{

}

CritBackBloodEffect::~CritBackBloodEffect()
{

}

void CritBackBloodEffect::enter(const BuffInfo& info,const EffectCommonData& data)
{
	m_Event = RegEvent(m_Father.GetOwner(),data.m_EventID,this,&CritBackBloodEffect::onCritEvent);
}

void CritBackBloodEffect::release()
{
	resetData();
}

bool CritBackBloodEffect::onCritEvent(const EventArgs & e)
{
	const BuffInfo* tempInfo = m_Father.GetBuffInfoPtrByID();

	if(tempInfo == NULL)
	{
		resetData();
		m_Father.End();

		return true;
	}

	if(m_Father.GetOwner()->GetType() == ePlayer)
	{
		Smart_Ptr<Player> owner = m_Father.GetOwner();
		float tempData = tempInfo->m_pParam->m_Param;
		if(owner->getAttManage()->getValue<float>(eInjured) > .0f)
		{
			tempData = owner->getAttManage()->getValue<float>(eInjured) * tempData;
		}
		m_Father.GetOwner()->CurHPChange((int)tempData,true);

		vector<int> tempAttList;
		tempAttList.push_back(eCharHP);
		owner->SynCharAttribute(tempAttList);
	}

	if(tempInfo->m_Condition == 0)
	{
		return true;
	}

	++m_Counts;

	if(m_Counts >= tempInfo->m_Condition)
	{
		resetData();
		m_Father.End();
	}

	return true;
}

void CritBackBloodEffect::resetData()
{
	m_Event.SetNull();
	m_Counts = 0;
}



