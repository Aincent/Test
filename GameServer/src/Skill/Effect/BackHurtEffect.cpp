#include "BackHurtEffect.h"
#include "MessageStruct/CharBase/CharAttribute.pb.h"
#include "../../ServerEventDefine.h"
#include "../../Map/SceneManager.h"



BackHurtEffect::BackHurtEffect(BuffBase& father) :
Effect(father),
m_Counts(0)
{

}

BackHurtEffect::~BackHurtEffect()
{

}

void BackHurtEffect::enter(const BuffInfo& info,const EffectCommonData& data)
{
	m_Event = RegEvent(m_Father.GetOwner(),data.m_EventID,this,&BackHurtEffect::onDamageEvent);
}

void BackHurtEffect::release()
{
	Effect::release();

	resetData();

	return ;
}

bool BackHurtEffect::onDamageEvent(const EventArgs & e)
{
	const BuffInfo* tempInfo = m_Father.GetBuffInfoPtrByID();
	if(tempInfo == NULL)
	{
		resetData();
		m_Father.End();

		return true;
	}

	const AfterBeHurtArgs& tArgs = static_cast<const AfterBeHurtArgs&>(e);
	int tempData = abs((int)(tempInfo->params * tArgs.m_DamageValue));
	tArgs.m_Target->CurHPChange(tempData,false);

	CharBase::SynchObjectAttr attr;
	CharBase::CharAttChange * change = attr.mutable_attr();

	attr.set_key(tArgs.m_Target->GetKey());
	attr.set_attkey(tArgs.m_Owner->GetKey());

	if(tArgs.m_Target->getAttManage()->getValue<int>(eCharHP) > 0)
	{
		change->add_type(eCharHP);
		change->add_value(tArgs.m_Target->getAttManage()->getValue<int>(eCharHP));
		change->add_state(eAttackBuff);
	}

	CSceneMgr::GetInstance()->SendSynchAttrChange(attr, tArgs.m_Target->GetMapID(), tArgs.m_Target->GetXNewPos(), tArgs.m_Target->GetYNewPos(), true);

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

void BackHurtEffect::resetData()
{
	m_Event.SetNull();
	m_Counts = 0;

	return ;
}
