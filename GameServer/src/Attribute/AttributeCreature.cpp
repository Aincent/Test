/*
 * AttributeCreature.cpp
 *
 *  Created on: 22/05/2015
 *      Author: root
 */
#include "AttributeCreature.h"
#include "AttributeManage.h"


AttributeCreature::AttributeCreature()
{

}

AttributeCreature::~AttributeCreature()
{

}

void AttributeCreature::Release()
{
	m_AttList.Release();
}

void AttributeCreature::createAtt(eObjType objectType)
{
	const map<ECharAttrType,AttData>& tempDataList = AttributeManage::GetInstance()->getAttData();
	map<ECharAttrType,AttData>::const_iterator tempOutIt = tempDataList.begin();

	for(;tempOutIt != tempDataList.end();++tempOutIt)
	{
		vector<eObjType>::const_iterator tempInIt;
		bool tempIsFind = false;
		for(tempInIt = tempOutIt->second.m_HaveObject.begin();tempInIt != tempOutIt->second.m_HaveObject.end();++tempInIt)
		{
			if(*tempInIt != objectType)
				continue;

			tempIsFind = true;
			break;
		}

		if(!tempIsFind)
			continue;

		AttributeBase* tempBasePtr = AttributeManage::GetInstance()->makeBasePtr(tempOutIt->second.m_ClassType,tempOutIt->second.m_DefaultValue);
		m_AttList.setBasePtr(tempBasePtr,tempOutIt->second.m_AttType);
	}

	return ;
}
