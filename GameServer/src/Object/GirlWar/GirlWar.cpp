/*
 * this.cpp
 *
 *  Created on: 2014年12月5日
 *      Author: root
 */

#include "GirlWar.h"
#include "Log/Logger.h"
#include "../../QuickMessage.h"
#include "Network/Message_Facade.h"
#include "../../MessageBuild.h"
#include "../../ServerManager.h"
#include "../../Skill/SkillLoader/SkillLoader.h"
#include "../../Skill/SkillBase.h"
#include "../../Skill/SkillFactory.h"
#include "../../Skill/BuffBase.h"
#include "../../Skill/BuffFactory.h"
#include "../Monster/Monster.h"
#include "../../Map/SceneManager.h"
#include "../FileLoader/GirlWarLoader.h"
#include "../FileLoader/BattleInfoLoader.h"
#include "../GirlWar/AI/PetBrain.h"
#include <math.h>
#include "../../ServerEventDefine.h"
#include "../../Attribute/AttributeGirlWar.h"
#include "FileLoader/InternationalLoader.h"
#include "MessageStruct/GirlWar/GirlWarMessage.pb.h"
#include "../../Fighting/Fighting.h"
#include "../../Skill/SkillMessage.h"
#include "GirlWarManager.h"
#include "../ObjectFactory.h"

GirlWar::GirlWar(eObjType type):CreatureObj(type),m_bIsWar(false),m_nModle(0),m_phyAtkTime(0),m_nSkill1(0),m_nSkill2(0),m_nSkill3(0),m_nStarrank(0),m_nQuality(0),m_nDeadTime(0)
{
	m_brain = new PetBrain;
	m_AttManage = new AttributeGirlWar();
}

GirlWar::~GirlWar()
{
	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();
	for(; it!=m_buffList.end();)
	{
		BuffBase * tmpBuff = it->second;
		m_buffList.erase(it++);
		if(!tmpBuff->IsEnd())
		{
			tmpBuff->End();
		}
		else
		{
			BuffFactory::Destory(tmpBuff);
		}
	}

	if(m_sReviveTime.IsValid())
	{
		m_sReviveTime.SetNull();
	}
}

void GirlWar::Realese()
{
	//CreatureObj::Release();

	m_bIsWar = false;
	m_nModle = 0;
	m_phyAtkTime = 0;
	m_nSkill1 = 0;
	m_nSkill2 = 0;
	m_nSkill3 = 0;
	m_nStarrank = 0;
	m_nQuality = 0;
	m_nDeadTime = 0;
	m_mEquit.clear();
	//m_brain->Release();

	Smart_Ptr<CreatureObj> obj;
	m_targetObj = obj;

	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();
	for(; it!=m_buffList.end();)
	{
		BuffBase * tmpBuff = it->second;
		m_buffList.erase(it++);
		if(!tmpBuff->IsEnd())
		{
			tmpBuff->End();
		}
		else
		{
			BuffFactory::Destory(tmpBuff);
		}
	}

	if(m_sReviveTime.IsValid())
	{
		m_sReviveTime.SetNull();
	}

	//回收
	//Smart_Ptr<Object> tempObject = GetMyself();
	//ObjectFactory::GetInstance()->Recycle(tempObject);

}

int GirlWar::AddExp(int exp)
{
	if (exp <= 0)
	{
		return -1;
	}

	if(getAttManage()->getValue<int>(eCharLv) >= m_Owner->getAttManage()->getValue<int>(eCharLv) || getAttManage()->getValue<int>(eCharLv) >= GirlWarLoader::GetInstance()->GetLimitLv(GetID()))
	{
		return -1;
	}

	getAttManage()->addValue(eCharExp,exp);

	if (GirlWarLoader::GetInstance()->GetUpdateLv(GetID(),getAttManage()->getValue<int>(eCharLv)) == 0)
	{
		LOG_ERROR(FILEINFO, "get upgrade info error and upgrade failed");
	}
	else
	{
		vector<int> vecType;
		bool isUpLv = false;

		const sGirlWarInfo *pInfo = GirlWarLoader::GetInstance()->GetGirlWarInfo(GetID());
		if(!pInfo)
		{
			return 0;
		}

		//一次多级
		int tempLevel = getAttManage()->getValue<int>(eCharLv);
		int64 tempExp = getAttManage()->getValue<int64>(eCharExp);

		while(tempExp >= GirlWarLoader::GetInstance()->GetUpdateLv(GetID(),tempLevel) && tempLevel < pInfo->nLimitLv)
		{
			tempExp = tempExp - GirlWarLoader::GetInstance()->GetUpdateLv(GetID(),tempLevel);
			getAttManage()->setValue(eCharLv,tempLevel + 1);
			tempLevel += 1;
			isUpLv = true;

			PlayerGirlwarUpgrade tempArgs(m_myself);
			m_Owner->FireEvent(PLAYER_GIRLWAR_UPGRADE,tempArgs);

			if(tempLevel == pInfo->nLimitLv)
			{
				tempExp = 0;
				break;
			}
		}

		getAttManage()->setValue(eCharExp,tempExp);

		if(isUpLv)
		{
			vecType.push_back(eCharExp);
			vecType.push_back(eCharLv);

			SynCharAttribute(vecType);

			m_Owner->SetDataFlag(eGirlWarInfo);
		}
	}

	return 0;
}

int64 GirlWar::GetTypeValue(int type)
{
	int64 value = 0;

	switch (type)
	{
		case eCharExp:
		{
			value = getAttManage()->getValue<int64>(eCharExp);;
			break;
		}
		case eCharLv:
		{
			value = getAttManage()->getValue<int>(eCharLv);
			break;
		}
		case eCharXpos:
		{
			value = GetXNewPos();
			break;
		}
		case eCharYpos:
		{
			value = GetYNewPos();
			break;
		}
		case eCharMapid:
		{
			value = GET_MAP_ID(GetMapID());
			break;
		}
		case eCharCountry:
		{
			value = getAttManage()->getValue<DWORD>(eCharCountry);
			break;
		}
		case eCharHP:
		{
			value = getAttManage()->getValue<int>(eCharHP);
			break;
		}
		case eCharHPMax:
		{
			value = getAttManage()->getValue<int>(eCharHPMax);
			break;
		}
		case eCharMP:
		{
			value = getAttManage()->getValue<int>(eCharMP);
			break;
		}
		case eCharMPMax:
		{
			value = getAttManage()->getValue<int>(eCharMPMax);
			break;
		}
		case eCharPhyAttackMax:
		{
			value = getAttManage()->getValue<int>(eCharPhyAttackMax);
			break;
		}
		case eCharPhyDefence:
		{
			value = getAttManage()->getValue<int>(eCharPhyDefence);
			break;
		}
		case eCharHitRate:
		{
			value = getAttManage()->getValue<int>(eCharHitRate);
			break;
		}
		case eCharAvoidRate:
		{
			value = getAttManage()->getValue<int>(eCharAvoidRate);
			break;
		}
		case eCharMoveSpeed:
		{
			value = getAttManage()->getValue<int>(eCharMoveSpeed);
			break;
		}
		case eCrackDownRate:
		{
			value = getAttManage()->getValue<int>(eCrackDownRate);
			break;
		}
		case eAvoidCrackRate:
		{
			value = getAttManage()->getValue<int>(eAvoidCrackRate);
			break;
		}
		case eFightPower:
		{
			value = getAttManage()->getValue<int>(eFightPower);
			break;
		}
		case ePosition:
		{
			value = (GetXNewPos() << 16) | GetYNewPos();
			break;
		}
	}

	return value;
}

void GirlWar::SetOwner(Smart_Ptr<Player>& player)
{
	m_Owner = player;
}

Smart_Ptr<Player> GirlWar::GetOwner()
{
	return m_Owner;
}

void GirlWar::SetOwnerToBrain(Smart_Ptr<GirlWar> &obj)
{
	if(!obj)
	{
		LOG_DEBUG(FILEINFO,"\n Set GirlWar my_self NULL\n");
		m_targetObj = 0;
	}
	m_myself = obj;
	m_brain->SetOwner(obj);
}

void GirlWar::SynCharAttribute(vector<int> &type)
{
	if (type.size() <= 0)
	{
		return;
	}

	CharBase::CharAttChange toGate;

	for (unsigned int i = 0; i < type.size(); ++i)
	{
		toGate.add_type(type[i]);
		toGate.add_value(GetTypeValue(type[i]));
		toGate.add_state(eAttackSim);
	}

//		2014.07.31修改,防止过量的消息发送
	QuickMessage::GetInstance()->AddSendMessage(GetKey(), m_Owner->GetChannelID(), &toGate, MSG_SIM_GT2C_SYNCHATTR);
}


void GirlWar::SendBuffState(CharBattle::BuffOperate * buf)
{
	CharBattle::BuffOperate selfBuffOp;
	selfBuffOp.CopyFrom(*buf);

	QuickMessage::GetInstance()->AddSendMessage(GetKey(), m_Owner->GetChannelID(), &selfBuffOp, MSG_SIM_GT2C_BUFFOPERATE);
}

void GirlWar::Dead(Smart_Ptr<CreatureObj> &obj)
{
	if(IsDead())
	{
		return ;
	}
	vector<int> vec;
	ChatInfo::EndChatInfo ret;

	m_brain->ChangeState(ePetStateType_Dead);

	CreatureKilledEventArgs tArgs(obj);
	FireEvent(CREATURE_DEAD_EVENT_TYPE, tArgs);
	RemoveCanBuff();
	ClearEnemy();
	m_brain->clearTarget();
	ClearPath();

	Smart_Ptr<CreatureObj> creature;
	SetTarget(creature);

	m_nDeadTime = CUtil::GetNowSecond();

	vec.push_back(eAttackDead);
	SynCharAttribute(vec);

	string sys = InternationalLoader::GetInstance()->GetInterStr(1);
	string str = InternationalLoader::GetInstance()->GetInterStr(38);
	ret.set_startname(sys);
	ret.set_channel(eChatSystem);
	ret.set_startcharid(-1);
	ret.set_str(str);

	ChatMgr::GetInstance()->WorldChat(m_Owner->GetID(),ret);
}



bool GirlWar::IsDead()
{
	if (m_nDeadTime > 0)
	{
		return true;
	}

	return false;
}

//bool GirlWar::IsInMap()
//{
//	if (GetMapID() > 0)
//	{
//		if(CreatureObj::IsInMap() == false)
//			return false;
//		return true;
//	}
//
//	return false;
//}

void GirlWar::SetTarget(Smart_Ptr<CreatureObj> &obj)
{
	if(m_targetObj.Get()!=NULL)
	{
		//原目标为玩家
		if(m_targetObj->GetType() == ePlayer)
		{
			Smart_Ptr<Player> player = m_targetObj;
			player->DeleteGirlWar(GetKey());
		}

		//原目标为怪物
		if(m_targetObj->GetType() == eMonster)
		{
			Smart_Ptr<Monster> mon=m_targetObj;
			mon->GetBrain()->removeGirlTarget(GetMyself());
		}

		//原目标为战姬
		if(m_targetObj->GetType() == ePet)
		{
			Smart_Ptr<GirlWar> pet=m_targetObj;
			pet->GetBrain()->removeTarget(GetMyself());
		}
	}

	m_targetObj = obj;

	if(m_targetObj.Get()!=NULL)
	{
		//玩家
		if(m_targetObj->GetType() == ePlayer)
		{
				Smart_Ptr<Player> player = m_targetObj;
				player->AddGirlWar(GetMyself());
		}
		//怪物
		if(m_targetObj->GetType() == eMonster)
		{
				Smart_Ptr<Monster> mon=m_targetObj;
				mon->GetBrain()->addGirlTarget(GetMyself());
		}

		//战姬
		if(m_targetObj->GetType() == ePet)
		{
				Smart_Ptr<GirlWar> pet=m_targetObj;
				pet->GetBrain()->addTarget(GetMyself());
		}
	}
}


bool GirlWar::AddBuff(int id, Smart_Ptr<CreatureObj> &obj, int num)
{
	if(IsDead())
	{
		return false;
	}
	
	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(id);
	if(info == NULL)
	{
		LOG_ERROR(FILEINFO, "add buff but buff is null");

		return false;
	}

	if(!SkillMessage::GetInstance()->isCanAddBuff(*info,m_myself,obj))
	{
		return false;
	}

	DWORD removeID = 0;
	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();
	for(; it!=m_buffList.end(); ++it)
	{
		const BuffInfo* tInfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(it->second->GetID());
		if(tInfo == NULL)
		{
			LOG_ERROR(FILEINFO, "GirlWar exist null buff");

			continue;
		}

		if(info->groupID == tInfo->groupID)
		{
			if(info->lv > tInfo->lv)
			{//顶替掉老的
				removeID = it->first;
				break;
			}
			else
			{
				return false;
			}
		}
	}

	if(removeID > 0)
	{
		DeleteBuffByOnlyID(removeID);
	}

	Smart_Ptr<CreatureObj> owner = m_myself;
	BuffBase* pBuffBase = BuffFactory::Create(owner, obj, id);
	if (pBuffBase == 0)
		return false;

	// 执行成功，则添加到怪物身上BUFF链当中
	m_buffList[pBuffBase->GetOnlyID()] = pBuffBase;

	if (pBuffBase->Start() == false)
	{
		// 添加失败
		pBuffBase->End();

		return false;
	}

	return true;
}

bool GirlWar::RemoveBuffByOnlyID(DWORD id)
{
	map<DWORD, BuffBase *>::iterator it = m_buffList.find(id);
	if(it == m_buffList.end())
	{
		return false;
	}

	m_buffList.erase(it);

	return true;
}

bool GirlWar::DeleteBuffByOnlyID(DWORD id)
{
	map<DWORD, BuffBase *>::iterator it = m_buffList.find(id);
	if(it == m_buffList.end())
	{
		return false;
	}

	BuffBase * tmpBuff = it->second;
	m_buffList.erase(it);
	if(!tmpBuff->IsEnd())
	{
		tmpBuff->End();
	}
	else
	{
		BuffFactory::Destory(tmpBuff);
	}

	return true;
}

void GirlWar::removeBuffByGroupID(DWORD groupID)
{
	map<DWORD, BuffBase *> tBuffLIst 	= m_buffList;
	map<DWORD, BuffBase *>::iterator it = tBuffLIst.begin();
	for(; it != tBuffLIst.end(); ++it)
	{
		const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(it->second->GetID());
		if(info == NULL)
		{
			continue ;
		}

		if(info->groupID != (int)groupID)
		{
			continue ;
		}

		DeleteBuffByOnlyID(it->second->GetOnlyID());
	}

	return ;	
}

void GirlWar::RemoveCanBuff()
{
	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();
	for(; it!=m_buffList.end(); )
	{
		BuffBase * tmpBuff = it->second;
		m_buffList.erase(it++);
		if(!tmpBuff->IsEnd())
		{
			tmpBuff->End();
		}
		else
		{
			BuffFactory::Destory(tmpBuff);
		}
	}
}

void GirlWar::SetIsWar(bool bFlag)
{
		m_bIsWar = bFlag;

		if(!bFlag && m_nDeadTime > 0)
		{
			if(m_sReviveTime.IsValid())
			{
				m_sReviveTime.SetNull();
			}

			int64 nFlushTime = 0;

			const sGirlWarInfo *pInfo = GirlWarLoader::GetInstance()->GetGirlWarInfo(GetID());
			if(!pInfo)
			{
				LOG_ERROR(FILEINFO, "girlWar ai dead state update but girlWar not find");
				return;
			}

			m_brain->ChangeState(ePetStateType_Dead);

			if((int)(CUtil::GetNowSecond() - GetDeadTime()) <  pInfo->nFlushTime)
			{
				nFlushTime = pInfo->nFlushTime - (int)(CUtil::GetNowSecond() - GetDeadTime());
				m_sReviveTime = TimerInterface::GetInstance()->RegTimePoint(&GirlWar::onReviveTime,this,0, 0, 0, 0,nFlushTime);
			}
			else
			{
				onReviveTime(NULL);
			}
		}
}

void GirlWar::Reflush()
{
	vector<int> vecType;
	int xpos = -1,ypos = -1;

	if(!CSceneMgr::GetInstance()->RandGetPetValidPos(m_Owner->GetMapID(), m_Owner->GetXNewPos(), m_Owner->GetYNewPos(), xpos, ypos))
	{
		xpos = m_Owner->GetXNewPos();
		ypos = m_Owner->GetYNewPos();
	}

	if(m_myself)
	{
		Smart_Ptr<Object>  obj = m_myself;

		SetCurHP(getAttManage()->getValue<int>(eCharHPMax));
		RemoveCanBuff();
		ClearEnemy();

		Smart_Ptr<CreatureObj> creature;
		SetTarget(creature);

		ClearPath();

		vecType.push_back(eCharHP);
		SynCharAttribute(vecType);


		if(m_myself->GetMapID() == m_Owner->GetMapID())
		{
			SetXNewPos(xpos);
			SetYNewPos(ypos);

			if(!CSceneMgr::GetInstance()->MoveTo(obj))
			{
				LOG_ERROR(FILEINFO, "girlwar[gs key=%d] move  failed ", GetKey());
			}

			CSceneMgr::GetInstance()->SendObjectToGroup(obj);
		}
		else
		{
			CSceneMgr::GetInstance()->LeaveScene(obj,true,true);

			SetXPos(xpos);
			SetYPos(ypos);
			SetMapID(m_Owner->GetMapID());

			SetXNewPos();
			SetYNewPos();
//			SetGroup(CSceneMgr::GetInstance()->GetSectorGroup(GetMapID(), xpos, ypos));

			if(!CSceneMgr::GetInstance()->EnterScene(obj))
			{
				LOG_ERROR(FILEINFO, "girlWar[girlWarid=%d] when flushing cannot enter scene", GetID());
			}
		}

		m_brain->ChangeState(ePetStateType_Idle);
		m_nDeadTime = 0;
	}
}

void GirlWar::ReflushInstance()
{
	GirlWarMessageInfo::ToClentFight toClient;
	vector<int> vecType;
	if(m_myself)
	{
		Smart_Ptr<Object>  obj = m_myself;

		SetCurHP(getAttManage()->getValue<int>(eCharHPMax));
		RemoveCanBuff();
		ClearEnemy();

		Smart_Ptr<CreatureObj> creature;
		SetTarget(creature);

		ClearPath();

		vecType.push_back(eCharHP);
		SynCharAttribute(vecType);

		m_brain->ChangeState(ePetStateType_Idle);
		m_nDeadTime = 0;

		toClient.set_ret(0);
		toClient.set_id(GetID());
		QuickMessage::GetInstance()->AddSendMessage(m_Owner->GetID(), m_Owner->GetChannelID(), &toClient, MSG_REQ_C2GM_ASKGIRLWARREVIVE);
	}
}

void GirlWar::BeginIdle()
{
	RemoveCanBuff();
}

int GirlWar::IsAttacked(Smart_Ptr<CreatureObj> &obj, int type)
{
	if(obj->IsDead())
	{
		return eNoAttack;
	}

	if(obj->GetID() == m_Owner->GetID() || obj->GetKey() == GetKey())
	{
		return eCantAttackMyself;
	}

	if(m_Owner->IsDead())
	{
		return eNoAttack;
	}

	switch(type)
	{
	case eEnemyTarget:
		{
			switch (m_Owner->getAttManage()->getValue<int>(ePKType))
			{
			case sPeace:
			{
				//和平模式下只能对怪物进行攻击
				if (obj->GetType() == ePlayer)
				{
					return eNoAttack;
				}

				if (obj->GetType() == ePet)
				{
					return eCantAttackByPeace;
				}

				//怪是同一阵营也不能攻击
				if (obj->GetType() != eMonster || m_Owner->getAttManage()->getValue<DWORD>(eCharCountry)== obj->getAttManage()->getValue<DWORD>(eCharCountry))
				{
					return eCantAttackByPeace;
				}

				break;
			}

			case sAll:
			{
				break;
			}

			case sGoodEvil:
			{
				break;
			}

			case sCountry:
			{
				if (obj->GetType() == ePlayer && m_Owner->getAttManage()->getValue<DWORD>(eCharCountry) == obj->getAttManage()->getValue<DWORD>(eCharCountry))
				{
					return eCantAttackSameCountry;
				}
				break;
			}

			case sTeam:
			{
				int64 group = m_Owner->GetGroupID();
				int64 targetGroup = 0;
				if(obj->GetType() == ePlayer)
				{
					Smart_Ptr<Player> target = obj;
					targetGroup = target->GetGroupID();
				}
				else if(obj->GetType() == ePet)
				{
					Smart_Ptr<GirlWar> pet = obj;
					targetGroup = pet->GetOwner()->GetGroupID();
				}

				//相同队伍id不能攻击
				if(group > 0 && group == targetGroup)
				{
					return eNoAttack;
				}

				break;
			}

			case sAlliance:
			{						//把联盟模式暂时用在帮派中
				if (obj->GetType() == ePlayer)
				{
					Smart_Ptr<Player> player = obj;
					if (player->getAttManage()->getValue<DWORD>(eCharFaction) != 0
							&& player->getAttManage()->getValue<DWORD>(eCharFaction)== m_Owner->getAttManage()->getValue<DWORD>(eCharFaction))
					{
						return eNoAttack;
					}
				}
				else if (obj->GetType() == eMonster)
				{//帮派站中，将怪物的阵营变成玩家的帮派id，这个怪物就等于属于这个帮派了
					if (obj->getAttManage()->getValue<DWORD>(eCharCountry) ==m_Owner->getAttManage()->getValue<DWORD>(eCharFaction))
						return eCantAttackSameCountry;
				}
				else if (obj->GetType() == ePet)
				{
					Smart_Ptr<GirlWar> girlWar = obj;
					Smart_Ptr<Player> player = girlWar->GetOwner();
					if (player->getAttManage()->getValue<DWORD>(eCharFaction) != 0
							&& player->getAttManage()->getValue<DWORD>(eCharFaction)== m_Owner->getAttManage()->getValue<DWORD>(eCharFaction))
					{
						return eNoAttack;
					}
				}

				break;
			}
			}

			break;
		}
	case eFriendTarget:
		{
			break;
		}
	case eMyself:
		{
			if (GetKey() != obj->GetKey())
			{
				return eNoAttack;
			}

			break;
		}
	case eNoPlayer:
		{

			if (obj->GetType() == ePlayer && obj->GetID() != m_Owner->GetID())
			{
				return eNoAttack;
			}

			if (obj->GetType() == eMonster)
			{
				Smart_Ptr<Monster> mon = obj;

				if (m_Owner->getAttManage()->getValue<int>(ePKType) == sAlliance)
				{
					if (obj->getAttManage()->getValue<DWORD>(eCharCountry) == m_Owner->getAttManage()->getValue<DWORD>(eCharFaction))
						return eCantAttackSameCountry;
				}

				if (mon->IsEscortCar())
				{
					return eNoAttack;
				}
			}
		}
	}

	return 0;
}

void GirlWar::Attacked(Smart_Ptr<CreatureObj> &obj)
{
	if(!obj)
		return;

	if(m_targetObj)
		return;

	if(m_brain->GetCurState() == ePetStateType_Dead)
		return;

	if(obj->GetID() == m_Owner->GetID())
		return;

	m_brain->AddEnemy(obj);

	SetTarget(obj);

	if((m_brain->GetCurState() == ePetStateType_Idle))
	{
		m_brain->ChangeState(ePetStateType_Attack);
	}
}

void GirlWar::Attack(int skillid)
{
	Smart_Ptr<CreatureObj> myself = m_myself;
	SkillInfo *info = 0;
	Smart_Ptr<CreatureObj> targetObj;
	int xpos = -1;
	int ypos = -1;

	info = SkillLoader::GetInstance()->GetSkillInfoByID(skillid);
	if(info == 0)
	{
		return ;
	}

	switch(info->rangeType)
	{
	case eRangeSingle://单个
		{
			targetObj = m_targetObj;

			break;
		}
	case eRangeMyself://自身范围
		{
			xpos = GetXNewPos();
			ypos = GetYNewPos();

			break;
		}
	case eRangePoint://点选范围
	case eRangeFan://扇形
	case eRangeLine:
		{
			if((bool)m_targetObj)
			{
				xpos = m_targetObj->GetXNewPos();
				ypos = m_targetObj->GetYNewPos();
			}

			break;
		}
	default:
		{
			targetObj = m_targetObj;

			break;
		}
	}

	int  key = 0;
	if((bool)targetObj)
	{
		key = targetObj->GetKey();
	}

	CSkillBase* pSkillBase = CSkillFactory::Create(skillid, myself,key, xpos, ypos);
	if (pSkillBase == NULL)
	{
		return ;
	}

	// 开始执行技能
	if(!pSkillBase->BeginAction())
	{
		CSkillFactory::FreeSkill(pSkillBase);
	}
}

bool GirlWar::IsViewInLength(int xpos, int ypos)
{
	int xLen = abs(GetXNewPos()- xpos);
	int yLen = abs(GetYNewPos()- ypos);
	double len = sqrt(xLen * xLen + yLen * yLen);
	int distance = GirlWarLoader::GetInstance()->GetMoveRange(GetID());

	if(len > distance)
	{
		return false;
	}

	return true;
}

void GirlWar::UpdateEnemyList(Smart_Ptr<Player> &player)
{
	Smart_Ptr<CreatureObj> obj = player;
	m_brain->DeleteEnemy(obj);
	if(m_targetObj && m_targetObj->GetKey() == obj->GetKey())
	{
		m_targetObj = 0;
	}
}

void GirlWar::ClearEnemy()
{
	m_brain->ClearEnemy();
}

//通知选我为目标的对象
void GirlWar::ClearTargets()
{
		GetBrain()->clearTarget();
}

void GirlWar::Update(int dwTick)
{
	m_brain->Update(dwTick);
}

void GirlWar::RelocatePos(int xpos, int ypos, bool isSyn)
{
	if(!m_myself)
	{
		return;
	}

	Smart_Ptr<Object> obj = m_myself;

	ClearEnemy();
	Smart_Ptr<CreatureObj> creature;
	SetTarget(creature);
	ClearPath();

	SetXNewPos(xpos);
	SetYNewPos(ypos);

	CSceneMgr::GetInstance()->MoveTo(obj);
}

void GirlWar::RelocatePosToAll(int xpos, int ypos, bool isSyn)
{
	if(!m_myself)
	{
		return;
	}

	Smart_Ptr<Object> obj = m_myself;

	ClearEnemy();
	Smart_Ptr<CreatureObj> creature;
	SetTarget(creature);
	ClearPath();

	SetXNewPos(xpos);
	SetYNewPos(ypos);

	CSceneMgr::GetInstance()->MoveToAll(obj);
}

void GirlWar::GetNextPosition(int &xpos, int &ypos)
{
	if(!CSceneMgr::GetInstance()->RandGetPetValidPos(m_Owner->GetMapID(), m_Owner->GetXNewPos(), m_Owner->GetYNewPos(), xpos, ypos))
	{
		xpos = m_Owner->GetXNewPos();
		ypos = m_Owner->GetYNewPos();
	}
}

bool GirlWar::IsAttackInLength(int distance)
{
	int xLen = abs(GetXNewPos()- m_targetObj->GetXNewPos());
	int yLen = abs(GetYNewPos()- m_targetObj->GetYNewPos());

	double len = sqrt(xLen * xLen + yLen * yLen);

	if(len > distance)
	{
		return false;
	}

	return true;
}

void GirlWar::SynchPath(PlayerSynch::GirlWarPathToPlayer &path)
{
	if(GetGameZone() && GetSector() && m_myself)
		GetGameZone()->SendSynchPetPath(path, m_myself);
}

void GirlWar::ClearPath()
{
	m_brain->ClearPathAndTime();
}

void GirlWar::Rest()
{
	if(m_myself)
	{
		Smart_Ptr<Object>  obj = m_myself;
//		printf("\n-----------------  %p  无际退出 xpos[%d]ypos[%d]   xnewpos[%d]ynewpos[%d]---------------\n", m_Owner.Get(), GetXPos(),GetYPos(),GetXNewPos(),GetYNewPos());
		m_Owner->GetGirlWarManager()->alterClanAtt(*this,false);
		CSceneMgr::GetInstance()->LeaveScene(obj,true,true);

		RemoveCanBuff();
		ClearEnemy();
		ClearPath();

		SetXNewPos();
		SetYNewPos();
		SetMapID(0);

		Smart_Ptr<CreatureObj> creature;
		SetTarget(creature);
	}
}


void GirlWar::Fight()
{
//	int xpos = -1;
//	int ypos = -1;

	if(!m_myself)
	{
		return;
	}

//	if(CSceneMgr::GetInstance()->RandGetPetValidPos(m_Owner->GetMapID(), m_Owner->GetXNewPos(), m_Owner->GetYNewPos(), xpos, ypos))
//	{
//		SetXPos(xpos);
//		SetYPos(ypos);
//		SetMapID(m_Owner->GetMapID());
//		SetGroup(CSceneMgr::GetInstance()->GetSectorGroup(GetMapID(), GetXPos(), GetYPos()));
//	}
//	else
//	{

		SetMapID(m_Owner->GetMapID());
		SetXPos(m_Owner->GetXNewPos());
		SetYPos(m_Owner->GetYNewPos());
//		SetGroup(CSceneMgr::GetInstance()->GetSectorGroup(GetMapID(), GetXPos(), GetYPos()));
//	}

	SetXNewPos();
	SetYNewPos();

	Smart_Ptr<Object> obj = m_myself;

	m_Owner->GetGirlWarManager()->alterClanAtt(*this,true);

	ClearPath();

//	printf("\n----------------- %p   无际进入 xpos[%d]ypos[%d]   xnewpos[%d]ynewpos[%d]---------------\n", m_Owner.Get(), GetXPos(),GetYPos(),GetXNewPos(),GetYNewPos());
	CSceneMgr::GetInstance()->EnterScene(obj);
	Attack(GetSkill(2));
	Attack(GetSkill(3));
}

void GirlWar::SendGirlWarSynch()
{
	PlayerSynch::SynchGirlWar info;

	info.set_id(GetID());
	info.set_key(GetKey());
	info.set_ownerkey(GetOwner()->GetKey());
	info.set_lv(getAttManage()->getValue<int>(eCharLv));
	info.set_hp(getAttManage()->getValue<int>(eCharHP));
	info.set_hpmax(getAttManage()->getValue<int>(eCharHPMax));

	info.set_xpos(GetXNewPos());
	info.set_ypos(GetYNewPos());

	QuickMessage::GetInstance()->AddSendMessage(GetOwner()->GetID(), GetOwner()->GetChannelID(), &info, MSG_SIM_GM2C_SYNGIRLWAR);
	if(GetBrain())
	{
		PlayerSynch::GirlWarPathToPlayer topath;

		GetBrain()->SetPath(topath);
		topath.set_key(GetKey());
		SynchPath(topath);
	}
}

bool GirlWar::InDefMap()
{
	return GetMapID() != m_Owner->GetMapID();
}

void GirlWar::GetPerfectLocation(int moveSpeed, int xpos, int ypos, int &txpos, int &typos)
{
	m_brain->GetPerfectLocation(moveSpeed, xpos, ypos, txpos, typos);
}

void GirlWar::onReviveTime(void* obj)
{
	if(m_myself)
	{
		Smart_Ptr<Object>  obj = m_myself;

		SetCurHP(getAttManage()->getValue<int>(eCharHPMax));
		RemoveCanBuff();
		ClearEnemy();

		Smart_Ptr<CreatureObj> creature;
		SetTarget(creature);

		ClearPath();

		m_brain->ChangeState(ePetStateType_Idle);
		m_nDeadTime = 0;
	}
}

void GirlWar::SetSkill2(int nSkill)
{
	m_nSkill2 = nSkill;
}

void GirlWar::SetSkill3(int nSkill)
{
	m_nSkill3 = nSkill;
}


bool GirlWar::IsHaveEquip(int nEquipId)
{
	map<int,int>::iterator itr = m_mEquit.begin();
	for(; itr != m_mEquit.end(); ++itr)
	{
		if(itr->second == nEquipId)
		{
			return true;
		}
	}

	return false;
}

bool GirlWar::IsPosEmpty(int nIndex)
{
	map<int,int>::iterator itr = m_mEquit.find(nIndex);
	if(itr != m_mEquit.end())
	{
			return false;
	}

	return true;
}

int GirlWar::GetSkill(int nIndex)
{
	int skillId = 0;
	switch(nIndex)
	{
		case 1:
		{
			skillId = GetSkill1();
			break;
		}
		case 2:
		{
			skillId = GetSkill2();
			break;
		}
		case 3:
		{
			skillId = GetSkill3();
			break;
		}
		default:
		{
			skillId = 0;
		}
	}

	return skillId;
}

void GirlWar::SetSkill(int nIndex,int skillId)
{
	switch(nIndex)
	{
		case 1:
		{
			SetSkill1(skillId);
			break;
		}
		case 2:
		{
			ReSkill(nIndex,skillId);
			SetSkill2(skillId);
			break;
		}
		case 3:
		{
			ReSkill(nIndex,skillId);
			SetSkill3(skillId);
			break;
		}
		default:
		{
		}
	}
}

void GirlWar::ReSkill(int nIndex,int skillId)
{
	if(m_bIsWar)
	{
		SkillInfo info;
		SkillLoader::GetInstance()->GetSkillInfoByID(GetSkill(nIndex), info);
		if(info.skillID != -1)
		{
			for(int i = 0; i < 3; i++)
			{
				DeleteBuffByOnlyID(info.effectFirstType[i]);
			}
		}

		Attack(skillId);
	}
}


void GirlWar::ChangeOwerAttribute(bool bFlag)
{
	vector<int> vecType;
	int tempValue = 0;
	int tempResult = 0;
	const sGirlWarStarrank *pStar = GirlWarLoader::GetInstance()->GetGirlWarStarrank(GetID(),GetStarrank());
	if(pStar)
	{
		m_Owner->ChangeHPMax(pStar->nAddOwerHp,bFlag);
		m_Owner->ChangePhyAttackMax(pStar->nAddOwerPhyAck,bFlag);
		m_Owner->ChangePDefence(pStar->nAddOwerPhydef,bFlag);
		m_Owner->ChangeARate(pStar->nAddOwerDodge,bFlag);
		m_Owner->ChangeCRate(pStar->nAddOwerCrit,bFlag);
		m_Owner->ChangeBaserte(pStar->nAddOwerUng,bFlag);

		int tempfightpower = BattleInfoLoader::GetInstance()->GetAllFight(
				pStar->nAddOwerPhyAck,pStar->nAddOwerPhydef,pStar->nAddOwerHp,0,pStar->nAddOwerDodge,pStar->nAddOwerCrit,pStar->nAddOwerUng);

		tempValue = m_Owner->getFightingManage()->getFighting(eFightingGirlWar);
		if(bFlag)
		{
			tempResult =  tempValue + tempfightpower;
			m_Owner->getFightingManage()->setFighting(eFightingGirlWar,tempResult);
		}
		else
		{
			tempResult =  tempValue - tempfightpower;
			m_Owner->getFightingManage()->setFighting(eFightingGirlWar,tempResult);
		}

		vecType.push_back(eCharHP);
		vecType.push_back(eCharHPMax);
		vecType.push_back(eCharPhyAttackMax);
		vecType.push_back(eCharPhyDefence);
		vecType.push_back(eCharAvoidRate);
		vecType.push_back(eCrackDownRate);
		vecType.push_back(eAvoidCrackRate);
		vecType.push_back(eFightPower);

		m_Owner->SynCharAttribute(vecType);
	}
}

void GirlWar::ChangeAttribute(int nHp,int nPhyAck,int nPhydef,int nCrit,int nUng,int nDodge,bool bFlag)
{
	vector<int> vecType;
	this->ChangeHPMax(nHp,bFlag);
	this->ChangePhyAttackMax(nPhyAck,bFlag);
	this->ChangePDefence(nPhydef,bFlag);
	this->ChangeARate(nDodge,bFlag);
	this->ChangeCRate(nCrit,bFlag);
	this->ChangeBaserte(nUng,bFlag);

	this->SetCurHP(this->getAttManage()->getValue<int>(eCharHPMax));

	int fightpower = BattleInfoLoader::GetInstance()->GetAllFight(
			this->getAttManage()->getValue<int>(eCharPhyAttackMax),
			this->getAttManage()->getValue<int>(eCharPhyDefence),
			this->getAttManage()->getValue<int>(eCharHPMax),
			this->getAttManage()->getValue<int>(eCharHitRate),
			this->getAttManage()->getValue<int>(eCharAvoidRate),
			this->getAttManage()->getValue<int>(eCrackDownRate),
			this->getAttManage()->getValue<int>(eAvoidCrackRate));

	this->getAttManage()->setValue(eFightPower,fightpower);

	vecType.push_back(eCharHP);
	vecType.push_back(eCharHPMax);
	vecType.push_back(eCharPhyAttackMax);
	vecType.push_back(eCharPhyDefence);
	vecType.push_back(eCharAvoidRate);
	vecType.push_back(eCrackDownRate);
	vecType.push_back(eAvoidCrackRate);
	vecType.push_back(eFightPower);

	this->SynCharAttribute(vecType);

	m_Owner->SetDataFlag(eGirlWarInfo);
}
