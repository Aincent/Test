/*
 * RegionFun.cpp
 *
 *  Created on: 01/12/2015
 *      Author: root
 */

#include "RegionFun.h"
#include "../Player/Player.h"
#include "PlayerRegion.h"
#include "../../Map/ConfigLoader/RegionManage.h"
#include "../../Attribute/AttributeCreature.h"
#include "../../QuickMessage.h"

//----------区域功能基类---------------
Player* RegionBase::getOwner()
{
	return m_Father->getOwner();
}

//--------------定时加经验-----------------
RegionIntervalExp::RegionIntervalExp(const RegionData* data,PlayerRegion* father,BYTE index) : RegionBase(father,data,index)
{

}

RegionIntervalExp::~RegionIntervalExp()
{
	m_Time.SetNull();
}

void RegionIntervalExp::time(void* p)
{
	int tBaseValue   = (int)m_Data->m_RegionFun[m_Index].m_FunPama[1];
	double tMultiple = m_Data->m_RegionFun[m_Index].m_FunPama[2];

	int tValue = RegionManage::GetInstance()->getIntervalExp(tBaseValue,getOwner()->getAttManage()->getValue<int>(eCharLv),tMultiple);

	if(m_Data->m_RegionFun[m_Index].m_FunType == eRegionIntervalExp)
	{
		getOwner()->AddExp(tValue);
	}
	else if(m_Data->m_RegionFun[m_Index].m_FunType == eRegionIntervalForce)
	{
		getOwner()->ChangeCurForce(tValue,true);

		vector<int> vecType;

		vecType.push_back(eCharForce);
		getOwner()->SynCharAttribute(vecType);
	}

	return ;
}

void RegionIntervalExp::revert()
{
	return ;
}

void RegionIntervalExp::excute()
{
	int tTime = (int)m_Data->m_RegionFun[m_Index].m_FunPama[0];
	m_Time = TimerInterface::GetInstance()->RegRepeatedTime(&RegionIntervalExp::time,this,0,0,0,0,tTime,tTime);

	return ;
}

//--------------修改属性-------------
RegionAlterAtt::RegionAlterAtt(const RegionData* data,PlayerRegion* father,BYTE index) : RegionBase(father,data,index)
{

}

void RegionAlterAtt::revert()
{
	getOwner()->getAttManage()->addValue((ECharAttrType)m_Data->m_RegionFun[m_Index].m_FunPama[0],-m_Data->m_RegionFun[m_Index].m_FunPama[1]);
}

void RegionAlterAtt::excute()
{
	getOwner()->getAttManage()->addValue((ECharAttrType)m_Data->m_RegionFun[m_Index].m_FunPama[0],m_Data->m_RegionFun[m_Index].m_FunPama[1]);
}

//--------------修改PK模式-------------
RegionAlterPkMode::RegionAlterPkMode(const RegionData* data,PlayerRegion* father,BYTE index) :
RegionBase(father,data,index),
m_OldPKMode(sPKNULL)
{

}

void RegionAlterPkMode::revert()
{
	if(m_OldPKMode == sPKNULL)
	{
		return ;
	}

	getOwner()->getAttManage()->setValue(ePKType,m_OldPKMode);

	getOwner()->SetOldPkType(m_OldPKMode);

	ServerReturn::ServerDoubleInt tMsg;
	tMsg.set_rets((int64)m_OldPKMode);
	tMsg.set_retf(eRetSuccess);

	QuickMessage::GetInstance()->AddSendMessage(getOwner()->GetID(), getOwner()->GetChannelID(),&tMsg,MSG_REQ_C2GT_CHANGEPKTYPE);

	return ;
}

void RegionAlterPkMode::excute()
{
	if(m_Data->m_RegionFun[m_Index].m_FunPama.size() == 0)
	{
		return ;
	}

	if(getOwner()->getAttManage()->getValue<int>(ePKType) == m_Data->m_RegionFun[m_Index].m_FunPama[0])
	{
		return ;
	}

	m_OldPKMode = (PkType)getOwner()->getAttManage()->getValue<int>(ePKType);

	getOwner()->getAttManage()->setValue(ePKType,m_Data->m_RegionFun[m_Index].m_FunPama[0]);

	getOwner()->SetOldPkType((PkType)m_Data->m_RegionFun[m_Index].m_FunPama[0]);

	ServerReturn::ServerDoubleInt tMsg;
	tMsg.set_rets((int64)m_Data->m_RegionFun[m_Index].m_FunPama[0]);
	tMsg.set_retf(eRetSuccess);

	QuickMessage::GetInstance()->AddSendMessage(getOwner()->GetID(), getOwner()->GetChannelID(),&tMsg,MSG_REQ_C2GT_CHANGEPKTYPE);

	return ;
}



