/*
 * SkillManager.cpp
 *
 *  Created on: 2014年1月20日
 *      Author: helh
 */

#include "SkillManager.h"

//#include <c++/4.1.1/bits/stl_iterator.h>
//#include <c++/4.1.1/bits/stl_map.h>
//#include <c++/4.1.1/bits/stl_pair.h>
//#include <c++/4.1.1/bits/stl_set.h>
//#include <c++/4.1.1/bits/stl_tree.h>
//#include <c++/4.1.1/bits/stl_vector.h>
#include <CharDefine.h>
#include <define.h>
#include <EventSet/BoundSlot.h>
#include <EventSet/EventArgs.h>
#include <EventSet/EventSet.h>
#include <EventSet/SubscriberSlot.h>
#include <FileLoader/GameParaLoader.h>
#include <Log/Logger.h>
#include <MessageCommonRet.h>
#include <MessageStruct/CharBase/CharAttribute.pb.h>
#include <MessageStruct/CharLogin/PlayerInfo.pb.h>
#include <MessageStruct/Map/SynchPath.pb.h>
#include <Singleton.h>
#include <Smart_Ptr.h>
//#include <StatisticDefine.h>
#include "../Attribute/AttributeCreature.h"
#include "../Attribute/AttributeManage.h"
#include "../CDTime/CDMgr.h"
#include "../EventSet/EventDefine.h"
#include "../Fighting/Fighting.h"
#include "../Map/ConfigLoader/MapConfig.h"
#include "../Map/SceneManager.h"
#include "../MapInstance/InstanceMapLoader.h"
#include "../Object/Creature.h"
#include "../Object/Object.h"
#include "../Object/Player/Player.h"
#include "../ServerEventDefine.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "BuffBase.h"
#include "BuffClass.h"
#include "BuffFactory.h"
#include "SkillBase.h"
#include "SkillFactory.h"
#include "SkillLoader/SkillLoader.h"
#include "../Meditation/PlayerMeditation.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../Container/ArticleEquipMgr.h"
#include "SkillMessage.h"
#include "../Object/GirlWar/GirlWarManager.h"
#include "../StatisticMgr/StatisticHandle.h"

SkillMgr::SkillMgr(Player * obj):m_owner(obj)
{
	ReInit();
}

SkillMgr::~SkillMgr()
{
	this->ReleaseSkill();
}

void SkillMgr::ReInit()
{
	m_TalentID=0;
	m_bigSkill=0;

	if(!m_outEvent.IsValid())
		m_outEvent = RegEvent(m_owner,PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, this, &SkillMgr::HandlePlayerOut);
}

void SkillMgr::ReleaseSkill()
{
	if(m_bigSkill)
	{
		m_bigSkill->EndStart();
	}

	if(m_outEvent.IsValid())
	{
		m_outEvent.SetNull();
	}

	map<DWORD, BuffBase *>::iterator itBuff = m_buffList.begin();
	for(; itBuff!=m_buffList.end(); )
	{
		BuffBase * tmpBuff = itBuff->second;
		m_buffList.erase(itBuff++);
		if(!tmpBuff->IsEnd())
		{
			tmpBuff->End();
		}
		else
		{
			BuffFactory::Destory(tmpBuff);
		}
	}
	m_buffList.clear();

	map<uint,CSkillBase *>::iterator it = m_curSkillList.begin();
	for(; it!=m_curSkillList.end(); )
	{
		CSkillBase * tmpit = it->second;
		m_curSkillList.erase(it++);

		if(!tmpit->IsEnd())
		{
			tmpit->EndStart();
		}
	}

	m_curSkillList.clear();
	m_SkillList.clear();
	m_tBuffInit.clear();
	m_clanSkills.clear();

}

void SkillMgr::GetClanSkills(std::vector<int>& skills)
{
	skills = m_clanSkills;
}

void SkillMgr::GetSkillInfos(std::vector<int>& skills)
{
	map<int,OneTalentSkill>::iterator it = m_SkillList.find(m_TalentID);
	if( it != m_SkillList.end() )
	{
		skills = it->second.m_SkillList;
	}

}

void SkillMgr::ReSetSkillInfos(const std::vector<int>& skills)
{
	map<int,OneTalentSkill>::iterator it = m_SkillList.find(m_TalentID);
	if( it != m_SkillList.end() )
	{
		it->second.m_SkillList = skills;
	}

}

bool SkillMgr::IsExistedSkill(int skillid)
{
	map<int,OneTalentSkill>::iterator tIt = m_SkillList.begin();
	for(;tIt != m_SkillList.end();++tIt)
	{
		for(size_t i = 0;i < tIt->second.m_SkillList.size();++i)
		{
			if(tIt->second.m_SkillList[i] == skillid)
				return true;
		}

	}

	return false;
}

bool SkillMgr::IsExistedClanSkill(int skillid)
{
	for(size_t i=0; i<m_clanSkills.size(); ++i)
	{
		if(m_clanSkills[i] == skillid)
			return true;
	}
	return false;
}

bool SkillMgr::isCurTalentSkill(int skillID)
{
	map<int,OneTalentSkill>::iterator tIt = m_SkillList.find(m_TalentID);
	if(tIt == m_SkillList.end())
	{
		return false;
	}

	for(size_t i = 0;i < tIt->second.m_SkillList.size();++i)
	{
		if(tIt->second.m_SkillList[i] == skillID)
			return true;
	}

	return false;
}

void SkillMgr::initBornSkill()
{
	const map<ECharProf,vector<int> >& tInfo = SkillLoader::GetInstance()->getBornSkill();

	ECharProf tJob = (ECharProf)m_owner->getAttManage()->getValue<int>(eCharProfession);

	map<ECharProf,vector<int> >::const_iterator tIt = tInfo.find(tJob);
	if(tIt == tInfo.end())
	{
		return ;
	}

	for(size_t i = 0;i < tIt->second.size();++i)
	{
		SkillInfo* tempInfo = SkillLoader::GetInstance()->GetSkillInfoByID(tIt->second[i]);
		if(tempInfo == NULL)
		{
			continue;
		}

		m_SkillList[tempInfo->m_TalentID].m_SkillList.push_back(tIt->second[i]);
		m_SkillList[tempInfo->m_TalentID].m_TalentID = tempInfo->m_TalentID;
	}

	if(m_SkillList.size() == 0)
	{
		return ;
	}

	m_TalentID = m_SkillList.begin()->first;

	Smart_Ptr<ArticleEquipMgr> tEquip = m_owner->GetContainer(eEquipQuickType);
	tEquip->initQuickSkill(*m_SkillList.begin()->second.m_SkillList.begin());

	return ;
}

bool SkillMgr::HandlePlayerOut(const EventArgs & e)
{
	PlayerRemoveMemberEventArgs *pArgs = (PlayerRemoveMemberEventArgs *) (&e);

	Smart_Ptr<Player> player = pArgs->m_pObj;
	if(player->GetID() == m_owner->GetID())
	{
		map<uint,CSkillBase *>::iterator it = m_curSkillList.begin();
		for(; it!=m_curSkillList.end(); )
		{
			CSkillBase * tmpit = it->second;
			m_curSkillList.erase(it++);
			if(!tmpit->IsEnd())
			{
				tmpit->EndStart();
			}
		}

		if(m_bigSkill)
		{
			m_bigSkill->EndStart();
		}
	}

	return true;
}

void SkillMgr::InitSkillInfo(const PlayerInfo::SkillInfo &info)
{
	for(int i=0; i<info.skilldata_size(); ++i)
	{
		OneTalentSkill tOneTalent;

		const PlayerInfo::POneTalentSkill& tOneTalentPro = info.skilldata(i);

		tOneTalent.m_TalentID = tOneTalentPro.talentid();

		tOneTalent.m_SkillList.clear();

		for(int j = 0;j < tOneTalentPro.skilllist_size();++j)
		{
			tOneTalent.m_SkillList.push_back(tOneTalentPro.skilllist(j));
		}

		m_SkillList[tOneTalent.m_TalentID] = tOneTalent;
	}

	m_TalentID = info.curtalent();

	if(m_SkillList.size() == 0)
	{
		initBornSkill();
	}

	for(int j=0; j<info.buf_size(); ++j)
	{
		PlayerInfo::BuffInfo tmpBuf = info.buf(j);

		BuffInitInfo info;

		info.type = tmpBuf.type();
		info.id = tmpBuf.id();
		info.endtime = tmpBuf.endtime();
		info.offtime = tmpBuf.offtime();
		info.isDel = tmpBuf.isdel();

		m_tBuffInit.push_back(info);
	}

	for(int i=0; i<info.clanskills_size(); ++i)
	{
		m_clanSkills.push_back( info.clanskills(i) );
	}

}

//上线计算属性
void SkillMgr::onlineCalculateAtt()
{
	map<ECharAttrType,int64> tempAttList;
	map<int,OneTalentSkill>::iterator tIt = m_SkillList.find(m_TalentID);
	if(tIt != m_SkillList.end())
	{
		for(size_t i = 0;i < tIt->second.m_SkillList.size();++i)
		{
			SkillInfo* tempInfo = SkillLoader::GetInstance()->GetSkillInfoByID(tIt->second.m_SkillList[i]);
			if(tempInfo == NULL)
				continue ;

			getDesignation(*tempInfo,tempAttList);
		}
	}

	for(size_t i = 0;i < m_clanSkills.size();++i)
	{
		SkillInfo* tempInfo = SkillLoader::GetInstance()->GetSkillInfoByID(m_clanSkills[i]);
		if(tempInfo == NULL)
			continue ;

		if(tempInfo->limitPro != ClanSkillType_Player)
		{
			continue ;
		}

		getDesignation(*tempInfo,tempAttList);
	}

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttSkill);

	m_owner->calculateResultAddAtt(tempAttList,tempValueType);

	calculateFight();

	return ;
}

//计算一次战斗力
int SkillMgr::calculateFight()
{
	map<ECharAttrType,int64> tempAttList;
	map<int,OneTalentSkill>::iterator tIt = m_SkillList.find(m_TalentID);
	int tempResult = 0;

	if(tIt != m_SkillList.end())
	{
		for(size_t i = 0;i < tIt->second.m_SkillList.size();++i)
		{
			SkillInfo* tempInfo = SkillLoader::GetInstance()->GetSkillInfoByID(tIt->second.m_SkillList[i]);
			if(tempInfo == NULL)
				continue ;

			tempResult += tempInfo->m_Fight;
			getDesignation(*tempInfo,tempAttList);
		}
	}

	for(size_t i = 0;i < m_clanSkills.size();++i)
	{
		SkillInfo* tempInfo = SkillLoader::GetInstance()->GetSkillInfoByID(m_clanSkills[i]);
		if(tempInfo == NULL)
			continue ;

		if(tempInfo->limitPro != ClanSkillType_Player)
		{
			continue ;
		}

		tempResult += tempInfo->m_Fight;
		getDesignation(*tempInfo,tempAttList);
	}

	tempResult += m_owner->getFightingManage()->CalculateFighting(tempAttList);
	return m_owner->getFightingManage()->setFighting(eFightingSkill,tempResult);
}

//获得指定技能增加的永久属性
void SkillMgr::getDesignation(const SkillInfo& info,map<ECharAttrType,int64>& attList)
{
	for(int i = 0; i < 3; ++i)
	{
		if(info.effectType[i] != eEffectDirect)
			continue;

		if((EffectChildType)info.effectFirstType[i] == eSkillAddAtt)
		{
			attList[(ECharAttrType)info.param[i][0]] += info.param[i][1];
			continue;	
		}

		switch((EffectChildType)info.effectFirstType[i])
		{
		case eAddHPMax://增加永久HP上限
			{
				attList[eCharHPMax] += info.param[i][0];

				break;
			}
		case eAddMPMax://增加永久MP上限
			{
				attList[eCharMPMax] += info.param[i][0];

				break;
			}
		case eAddPhyAttack://增加永久物理攻击 值
			{
				attList[eCharPhyAttackMax] += info.param[i][0];

				break;
			}
		case eAddMagAttack://增加永久法术攻击值
			{
				break;
			}
		case eAddSpeed://增加永久速度值
			{
				attList[eCharMoveSpeed] += info.param[i][0];

				break;
			}
		case eAddPhyDefence://增加永久物理防御值
			{
				attList[eCharPhyDefence] += info.param[i][0];

				break;
			}
		case eAddAvoidRate://增加永久闪避值
			{
				attList[eCharAvoidRate] += info.param[i][0];

				break;
			}
		case eAddCrackRate://增加永久暴击值
			{
				attList[eCrackDownRate] += info.param[i][0];

				break;
			}
		case eAddHitRate://增加永久命中值
			{
				attList[eCharHitRate] += info.param[i][0];

				break;
			}
		case eAddAvoidCrack://永久增加抗暴ss
			{
				attList[eAvoidCrackRate] += info.param[i][0];

				break;
			}
		default:
			{
				break;
			}
		}
	}

	return ;
}

void SkillMgr::InitBuff()
{
	Smart_Ptr<CreatureObj> fire = m_owner->GetCreatureSelf();
	vector<BuffInitInfo>::iterator it = m_tBuffInit.begin();
	for(; it!=m_tBuffInit.end(); ++it)
	{
		int tCount = 1;
		int allTime = 1;

		const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID((*it).id);
		if(info == NULL)
		{
			LOG_ERROR(FILEINFO, "init buff but buff is null");

			continue;
		}

		switch((*it).type)
		{
		case eBuffForever:
			{
				break;
			}
		case eBuffCount:
			{
				if(info->saveType == eNoIceTime)
				{
					int64 nowTime = CUtil::GetNowSecond();
					tCount = ((*it).endtime - nowTime) / (info->aliveTime / info->timer);
					allTime = (*it).endtime - nowTime;
				}
				else
				{
					tCount = ((*it).endtime - (*it).offtime) / (info->aliveTime / info->timer);
					allTime = (*it).endtime - (*it).offtime;
				}

				break;
			}
		case eBuffOnce:
			{
				if(info->saveType == eNoIceTime)
				{
					int64 nowTime = CUtil::GetNowSecond();
					allTime = (*it).endtime - nowTime;
				}
				else
				{
					allTime = (*it).endtime - (*it).offtime;
				}

				break;
			}
		case eBuffCapInvlTime:
			{
				if (info->saveType == eIceTime)
				{
					// 是buff容量的话  endtime是容量, offtime是间隔时间
					allTime = (*it).endtime;
					tCount = (*it).offtime;
				}
				break;
			}
		default:
			{
				LOG_ERROR(FILEINFO, "init buff but buf type is error");
			}
		}

		if(((*it).isDel && (tCount <= 0 || allTime <= 0)) || tCount == 0)
		{
			m_owner->SetDataFlag(eSkillInfo);
			continue;
		}

		BuffBase* pBuffBase = BuffFactory::Create(fire, fire, (*it).id, (*it).type, allTime, tCount);
		if (pBuffBase == 0)
		{
			m_owner->SetDataFlag(eSkillInfo);
			continue;
		}

		m_buffList[pBuffBase->GetOnlyID()] = pBuffBase;
		if (pBuffBase->Start() == false || tCount <= 0 || allTime <= 0)
		{
			// 添加失败
			pBuffBase->End();
			m_owner->SetDataFlag(eSkillInfo);

			continue;
		}
	}

	m_tBuffInit.clear();
}

void SkillMgr::SetSkillInfo(PlayerInfo::SkillInfo *info, bool isDel)
{
	map<int,OneTalentSkill>::iterator tIt = m_SkillList.begin();

	for(; tIt!=m_SkillList.end(); ++tIt)
	{
		PlayerInfo::POneTalentSkill* tOneTalentPro = info->add_skilldata();
		tOneTalentPro->set_talentid(tIt->first);

		for(size_t i = 0;i < tIt->second.m_SkillList.size();++i)
		{
			tOneTalentPro->add_skilllist(tIt->second.m_SkillList[i]);
		}
	}

	info->set_curtalent(m_TalentID);

	map<DWORD, BuffBase *>::iterator itBuff = m_buffList.begin();
	for(; itBuff!=m_buffList.end(); )
	{
		const BuffInfo* tmpInfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(itBuff->second->GetID());
		if(tmpInfo == NULL)
		{
			LOG_ERROR(FILEINFO, "remove buff but buff is null");
		}

		BuffBase * tmpBuff = itBuff->second;
		if(isDel)
		{
			m_buffList.erase(itBuff++);
			if(!tmpBuff->IsEnd())
			{
				tmpBuff->End();
			}
			else
			{
				BuffFactory::Destory(tmpBuff);
			}
		}

		if(tmpInfo->saveType != eNoSave)
		{
			tmpBuff->SetInfo(info, isDel);
		}

		if(!isDel)
		{
			++itBuff;
		}
	}

	for(size_t i=0; i<m_clanSkills.size(); ++i)
	{
		info->add_clanskills( m_clanSkills[i] );
	}

	return ;
}

void SkillMgr::clientGetInfo(CharLogin::skillInfoPro* info)
{
	map<int,OneTalentSkill>::iterator tIt = m_SkillList.begin();

	for(; tIt!=m_SkillList.end(); ++tIt)
	{
		CharLogin::POneTalentSkill* tOneTalentPro = info->add_talents();
		tOneTalentPro->set_talentid(tIt->first);

		for(size_t i = 0;i < tIt->second.m_SkillList.size();++i)
		{
			tOneTalentPro->add_skilllist(tIt->second.m_SkillList[i]);
		}
	}

	info->set_currenttalentid(m_TalentID);

	return ;
}

void SkillMgr::SynchSkillInfoToWS(PlayerInfo::ArenaBaseInfo *skill)
{
	map<int,OneTalentSkill>::iterator tIt = m_SkillList.find(m_TalentID);

	if(tIt != m_SkillList.end())
	{
		for(size_t i = 0;i < tIt->second.m_SkillList.size();++i)
		{
			SkillInfo info;

			SkillLoader::GetInstance()->GetSkillInfoByID(tIt->second.m_SkillList[i], info);
			if(info.skillID == -1 || info.fType == ePassiveSkill)
			{
				continue;
			}

			skill->add_skill(tIt->second.m_SkillList[i]);
		}
	}

	return ;
}

int SkillMgr::Attack(int skillid)
{
	CSkillBase* pSkillBase = 0;
	Smart_Ptr<CreatureObj> fire = m_owner->GetCreatureSelf();

	if(!fire)
	{
		return 0;
	}

	pSkillBase = CSkillFactory::Create(skillid, fire, fire->GetKey(), 0, 0);

	if (pSkillBase == NULL)
	{
		return eSystemError;
	}

	// 开始执行技能
	if(!pSkillBase->BeginAction())
	{
		CSkillFactory::FreeSkill(pSkillBase);
	}

	return 0;
}

int SkillMgr::Attack_FirstMagicSkill(int xpos, int ypos , int key, int skillid)
{
	return 0;
}

int SkillMgr::Attack(int xpos, int ypos , int key, int skillid)
{
	if(key < 0 && !CMapConfig::GetInstance()->IsValidPos(GET_MAP_ID(m_owner->GetMapID()), xpos, ypos))
	{
		return eBattleTargetNull;
	}
//	cout<<endl<<"释放技能："<<skillid<<"xpos:"<<xpos<<"ypos"<<ypos<<"key"<<key<<endl;
	Smart_Ptr<CreatureObj> target;
	Smart_Ptr<CreatureObj> fire = m_owner->GetCreatureSelf();
	if(!fire)
	{
			return eNotFindThisCharID;
	}

	int res = 0;

	if(key >= 0)
	{
		CSceneMgr::GetInstance()->GetObject(m_owner->GetMapID(), key, target, true);
	}

	if((res = IsAttacked(target, xpos, ypos, skillid)) != 0)
	{
		return res;
	}

	CSkillBase* pSkillBase = 0;
	SkillInfo info;

	SkillLoader::GetInstance()->GetSkillInfoByID(skillid, info);
	if(info.skillID == -1)
	{
		return eNull;
	}

	if(info.sType == eSkillTypeNest)
	{
		bool iskey = false;
		map<uint,CSkillBase *>::iterator it = 	m_curSkillList.begin();

		for(;it != m_curSkillList.end(); it++)
		{
			if(it->second->GetSkillInfo()->sType == eSkillTypeNest)
			{
				iskey = true;
				pSkillBase = it->second;
				pSkillBase->SetTarget(key, xpos, ypos);
			}
		}
		if(!iskey)
		{
			pSkillBase = CSkillFactory::Create(skillid, fire, key, xpos, ypos);
		}
	}
	else
	{
		pSkillBase = CSkillFactory::Create(skillid, fire, key, xpos, ypos);
	}

	if (pSkillBase == NULL)
	{
		return eSystemError;
	}

	//printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%% owner[%lld] skill[%d] factory create%%%%%%%%%%%%%%%% \n", m_owner->GetID(), skillid);
	// 开始执行技能
	if(!pSkillBase->BeginAction())
	{
//		printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%% owner[%lld] skill[%p] factory free%%%%%%%%%%%%%%%%", m_owner->GetID(), pSkillBase);
		CSkillFactory::FreeSkill(pSkillBase);
	}

	if (!m_owner->IsInCreatureStateAll(ERS_Fight))
	{
		m_owner->CanccelMeditation();

		m_owner->SetCreatureState(ERS_Fight, false);
		m_owner->SynCharAttribute(eSimpleState, 0, eFightState);
		m_owner->BroadcastCharAttribute(eSimpleState, 0, eFightState, false);

	}

	m_owner->SetFightTime();

	return res;
}

bool SkillMgr::IsExistBufferID(int BufferID)
{
	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();
	for(; it!=m_buffList.end(); ++it)
	{
		if(it->second == NULL)
			continue;

		if(it->second->GetID() == BufferID)
			return true;
	}
	return false;
}


int SkillMgr::IsFireSkill(int skillid)
{
	if(!isCurTalentSkill(skillid) && !m_owner->GetMagicWeapon()->isMagicSkill(skillid))
	{
		return eNoLearnSKill;
	}

	SkillInfo info;

	SkillLoader::GetInstance()->GetSkillInfoByID(skillid, info);
	if(info.skillID == -1)
	{
		return eNull;
	}

	if(info.fType == ePassiveSkill)
		return ePassiveSkillNotFire;

	int tempState = 0;

	for(size_t i = 0;i < info.m_StatusCondition.size();++i)
	{
		tempState |= 1 <<  info.m_StatusCondition[i];
	}

	if(m_owner->IsInCreatureState(ERS_Still) && !(tempState & (1 << eAttackImmunityStill)))
		return eStateNotAttack;

	if(m_owner->IsInCreatureState(ERS_Ice) && !(tempState & (1 << eAttackImmunityIce)))
		return eStateNotAttack;

	if(m_owner->IsInCreatureState(ERS_Stun) && !(tempState & (1 << eAttackImmunityStun)))
		return eStateNotAttack;

	if(m_owner->IsInCreatureState(ERS_Silent) && !(tempState & (1 << eImmuintySilent)) && skillid > GameParaLoader::GetInstance()->getSpecialSkillIdBegin())
		return eStateNotAttack;
	
	if(m_owner->IsInCreatureState(ERS_Sleep) && !(tempState & (1 << eImmunitySleep)))
		return eStateNotAttack;

	if(m_owner->GetType() == ePlayer)
	{
		if(info.limitPro == eProfessionMax)
		{
			const int Arena_MapID = GameParaLoader::GetInstance()->getArenaMapId();
			if(GET_MAP_ID(m_owner->GetMapID()) == Arena_MapID)
			{
				return eMagicSkillNotFire;
			}

			//斗战重楼不可以释放神兵技能
			if(InstanceLoader::GetInstance()->GetEntryType(GET_MAP_ID(m_owner->GetMapID())) == ePVP)
			{
				return eMagicSkillNotFire;
			}
		}

		//cd时间判断
		if(skillid > GameParaLoader::GetInstance()->getSpecialSkillIdBegin())
		{//非普通攻击
			if(!m_owner->GetCDMgr()->IsCDEnd(SKILL_COMMON))
			{
				return eAttackSpeedTooFast;
			}
		}

		if(!m_owner->GetCDMgr()->IsCDEnd(info.channelType))
		{
			return eAttackSpeedTooFast;
		}

		//消耗判断
		if(m_owner->getAttManage()->getValue<int>(eCharHP) - info.useHP <= 0)
		{
			return eNotEnoughHP;
		}

		if(m_owner->getAttManage()->getValue<int>(eCharMP) - info.useMP < 0)
		{
			return eNotEnoughMP;
		}
	}

	return 0;
}

bool SkillMgr::IsInAttackDistance(int xpos, int ypos, int skillid)
{
	int xtemp = abs(xpos-m_owner->GetXNewPos());
	int ytemp = abs(ypos-m_owner->GetYNewPos());

	SkillInfo info;

	SkillLoader::GetInstance()->GetSkillInfoByID(skillid, info);
	if(info.skillID == -1)
	{
		LOG_ERROR(FILEINFO, "get skill[skillid=%d] info error", skillid);

		return false;
	}

	if(xtemp > (info.distance + 2) || ytemp > (info.distance + 2))
	{
//		printf("\n--------------distance = %f, info.distance=%d---------\n", distance, info.distance);
		return false;
	}

	return true;
}

int SkillMgr::IsAttacked(Smart_Ptr<CreatureObj> &obj, int xpos, int ypos, int skillid)
{
	if(!obj && xpos <= 0 && ypos <= 0)
		return eBattleTargetNull;

	SkillInfo info;

	SkillLoader::GetInstance()->GetSkillInfoByID(skillid, info);
	if(info.skillID == -1)
	{
		return eNull;
	}

	if((bool)obj)
	{
		int erratt = m_owner->IsAttacked(obj,info.targetType);
		if(erratt)
		{
			return erratt;
		}

		if(obj->GetType() == ePlayer || obj->GetType() == eMonster || obj->GetType() == ePet)
		{
			if(!IsInAttackDistance(obj->GetXNewPos(), obj->GetYNewPos(), skillid))
			{
				return eAttackBeyondDistance;
			}

			if(obj->IsDead())
			{
				return eCreatureDead;
			}

			if(obj->GetType()==ePlayer)
			{
				Smart_Ptr<Player> tempPlayer = obj;
				if(m_owner->GetKey()!=tempPlayer->GetKey() && tempPlayer->IsInCreatureState(ERS_Invincible))
					return eStateNotAttack;
			}
		}
		else
		{
			return eBattleTargetNull;
		}
	}
	else
	{
		if(!IsInAttackDistance(xpos, ypos, skillid))
		{
			return eAttackBeyondDistance;
		}
	}

	return 0;
}

void SkillMgr::RemoveDeadBuff()
{
	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();
	for(; it!=m_buffList.end(); )
	{
		const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(it->second->GetID());
		if(info == NULL)
		{
			LOG_ERROR(FILEINFO, "remove buff but buff is null");
		}

		if(info->isDel)
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
		else
		{
			++it;
		}
	}

	m_owner->SetDataFlag(eSkillInfo);
}

void SkillMgr::ClearBuffer()
{
	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();
	for(; it!=m_buffList.end(); )
	{
		BuffBase * tmpBuff = it->second;
		m_buffList.erase(it++);

		if(tmpBuff == NULL)
		{
			continue;
		}

		if(!tmpBuff->IsEnd())
		{
			tmpBuff->End();
		}
		else
		{
			BuffFactory::Destory(tmpBuff);
		}
	}

	m_owner->SetDataFlag(eSkillInfo);
}

bool SkillMgr::RemoveBuffByOnlyID(DWORD id)
{
	map<DWORD, BuffBase *>::iterator it = m_buffList.find(id);
	if(it == m_buffList.end())
	{
		return false;
	}

	m_buffList.erase(it);

	return true;
}

bool SkillMgr::DeleteBuffByOnlyID(DWORD id)
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

	m_owner->SetDataFlag(eSkillInfo);

	return true;
}

bool SkillMgr::RemoveBuffByID(DWORD id)
{
	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();
	for(; it != m_buffList.end(); ++it)
	{
		if((DWORD)it->second->GetID() == id)
		{
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

			m_owner->SetDataFlag(eSkillInfo);

			return true;
		}
	}

	return false;
}

void SkillMgr::removeBuffByGroupID(DWORD groupID)
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

bool SkillMgr::addBuff(int id, Smart_Ptr<CreatureObj> & obj, int num)
{
	int64 oldTime = 0;
	int type = 0;

	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(id);
	if(info == 0)
	{
		LOG_ERROR(FILEINFO, "add buff but buff is null");

		return false;
	}	

	if(!SkillMessage::GetInstance()->isCanAddBuff(*info,m_owner->GetMyself(),obj))
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
			LOG_ERROR(FILEINFO, "player exist null buff");

			continue;
		}



		if(info->groupID == tInfo->groupID)
		{
			//低级连斩BUFF 不覆盖高级连斩BUFF
			if(info->id < tInfo->id && info->groupID == 200)
			{
				return false;
			}

			//顶替掉老的
			removeID = it->first;

			if (info->lv != tInfo->lv)
			{
				break;
			}

			if (!tInfo->isAdd)
			{
				break;
			}

			//相同连斩BUFF重启时间
			if(info->groupID == 200)
			{
				it->second->ReStart();
				m_owner->SetDataFlag(eSkillInfo);
				return true;
			}

			//相同组相同BUFF叠加时间
			type = it->second->GetType();

			switch(type)
			{
			case eBuffCount:
				{
					BuffCount * buffcount = (BuffCount *)it->second;
					oldTime = buffcount->GetEndTime();
					break;
				}
			case eBuffOnce:
				{
					BuffOnceTimer * buffoncetimer = (BuffOnceTimer *)it->second;
					oldTime = buffoncetimer->GetEndTime();
					break;
				}
			case eBuffCapInvlTime:
				{
					BuffCapInvlTime* buff = (BuffCapInvlTime*)it->second;
					oldTime = buff->GetCurCap(); // 当前容量
					break;
				}
			default:
				{
					break;
				}
			}
			break;
		}
	}
	
	Smart_Ptr<CreatureObj> fire = m_owner->GetCreatureSelf();

	if(!fire)
	{
		return false;
	}

	BuffBase* pBuffBase = NULL;

	if(removeID > 0)
	{
		DeleteBuffByOnlyID(removeID);
	}

	switch(type)
	{
	case 0:
		{
			pBuffBase = BuffFactory::Create(fire, obj, id, num);
			break;
		}
	case eBuffCount:
	case eBuffOnce:
		{
			pBuffBase = BuffFactory::Create(fire, obj, id,type,info->aliveTime * num + oldTime - CUtil::GetNowSecond(),info->timer, num);
			break;
		}
	case eBuffCapInvlTime:
		{
			pBuffBase = BuffFactory::Create(fire, obj, id, type, info->aliveTime * num + oldTime, info->timer);
			break;
		}
	default:
		{
			break;
		}
	}

	if (pBuffBase == 0)
		return false;

	// 执行成功，则添加到玩家身上BUFF链当中
	m_buffList[pBuffBase->GetOnlyID()] = pBuffBase;
	m_owner->SetDataFlag(eSkillInfo);

	if (pBuffBase->Start() == false)
	{
		// 添加失败
		pBuffBase->End();

		return false;
	}

	return true;
}

int SkillMgr::AttackPassive(int skillid, bool isDel, bool isBroadCast)
{
	SkillInfo info;

	SkillLoader::GetInstance()->GetSkillInfoByID(skillid, info);
	if(info.skillID == -1)
	{
		return eNull;
	}

	SkillMgr::ResultEffect(m_owner,&info, isDel, isBroadCast);

	return 0;
}

void SkillMgr::ResultEffect(CreatureObj* object,vector<AttrChange> &changeVec, vector<AttrChange> &changeMyself, int type, const int *param, bool isDel)
{
	AttrChange change;
	int value = param[0];
	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttSkill);
	switch(type)
	{
	case eAddHPMax://增加永久HP上限
		{
			object->ChangeHPMax(value, !isDel,tempValueType);
			change.type = eCharHPMax;
			change.value = object->getAttManage()->getValue<int>(eCharHPMax);

			changeVec.push_back(change);
			changeMyself.push_back(change);

			break;
		}
	case eAddMPMax://增加永久MP上限
		{
			object->ChangeMPMax(value, !isDel,tempValueType);
			change.type = eCharMPMax;
			change.value = object->getAttManage()->getValue<int>(eCharMPMax);
			changeVec.push_back(change);
			changeMyself.push_back(change);

			break;
		}
	case eAddPhyAttack://增加永久物理攻击 值
		{
			object->ChangePhyAttackMax(value, !isDel,tempValueType);
			{
				change.type = eCharPhyAttackMax;
				changeMyself.push_back(change);
			}

			break;
		}
	case eAddMagAttack://增加永久法术攻击值
		{
			break;
		}
	case eAddSpeed://增加永久速度值
		{
			object->ChangeMoveSpeed(value, !isDel,tempValueType);
			change.type = eCharMoveSpeed;
			change.value = object->getAttManage()->getValue<int>(eCharMoveSpeed);
			changeVec.push_back(change);
			changeMyself.push_back(change);

			break;
		}
	case eAddPhyDefence://增加永久物理防御值
		{
			object->ChangePDefence(value, !isDel,tempValueType);
			{
				change.type = eCharPhyDefence;
				changeMyself.push_back(change);
			}

			break;
		}
	case eAddAvoidRate://增加永久闪避值
		{
			object->ChangeARate(value, !isDel,tempValueType);
			{
				change.type = eCharAvoidRate;
				changeMyself.push_back(change);
			}

			break;
		}
	case eAddCrackRate://增加永久暴击值
		{
			object->ChangeCRate(value, !isDel,tempValueType);
			{
				change.type = eCrackDownRate;
				changeMyself.push_back(change);
			}

			break;
		}
	case eAddHitRate://增加永久命中值
		{
			object->ChangeHRate(value, !isDel,tempValueType);
			{
				change.type = eCharHitRate;
				changeMyself.push_back(change);
			}

			break;
		}
	case eAddAvoidCrack://永久增加抗暴ss
		{
			object->ChangeBaserte(value, !isDel,tempValueType);
			{
				change.type = eAvoidCrackRate;
				changeMyself.push_back(change);
			}

			break;
		}
	case eAddPercentCrackRate:
		{
			object->getAttManage()->addValue<int>(ePercentCritRate,isDel ? -value : value,tempValueType);

			change.type = ePercentCritRate;
			changeMyself.push_back(change);

			break;
		}
	case eSkillAddAtt:
		{
			object->getAttManage()->addValue<int>((ECharAttrType)param[0],isDel ? -param[1] : param[1],tempValueType);

			change.type = (ECharAttrType)param[0];
			changeMyself.push_back(change);

			break;
		}
	default:
		break;
	}
}

void SkillMgr::ResultEffect(CreatureObj* object,const SkillInfo *info, bool isDel, bool isBroadCast)
{
	if(object == NULL)
	{
		return ;
	}

	if(info == NULL)
	{
		return ;
	}

	vector<AttrChange> allAttr;
	vector<AttrChange> myselfAttr;

	for(int i = 0; i < 3; ++i)
	{
		int randNum = rand() % 100 + 1;

		if(randNum > info->atkRate[i])
		{
			continue;
		}

		vector<AttrChange> change;
		vector<AttrChange> changeMyself;

		switch(info->effectType[i])
		{
		case eEffectDirect:
			{
				SkillMgr::ResultEffect(object,change, changeMyself, info->effectFirstType[i], info->param[i], isDel);

				break;
			}
		default:
			break;
		}

		if(isBroadCast)
		{
			if(change.size() > 0)
			{
				allAttr.insert(allAttr.end(), change.begin(), change.end());
			}

			if(changeMyself.size() > 0)
			{
				myselfAttr.insert(myselfAttr.end(), changeMyself.begin(), changeMyself.end());
			}
		}
	}

	if(object->GetType() != ePlayer)
	{
		return ;
	}	

	if(isBroadCast)
	{
		Player* tPlayer = static_cast<Player*>(object);
		int beforeFight = tPlayer->getFightingManage()->getFighting(eFightingSkill);
		tPlayer->GetSkillMgr()->calculateFight();
		int afterFight = tPlayer->getFightingManage()->getFighting(eFightingSkill);
		//发给目标自己的
		{
			vector<int> vec;

			vector<AttrChange>::iterator it = myselfAttr.begin();
			for(; it != myselfAttr.end(); ++it)
			{
				vec.push_back((*it).type);
			}

			if(vec.size() > 0 || beforeFight != afterFight)
			{
				vec.push_back(eFightPower);
			}

			tPlayer->SynCharAttribute(vec);
		}

		//广播
		{
			CharBase::SynchObjectAttr attr;
			CharBase::CharAttChange * change = 0;

			attr.set_key(object->GetKey());
			vector<AttrChange>::iterator it = allAttr.begin();
			for(; it != allAttr.end(); ++it)
			{
				if(change == 0)
				{
					change = attr.mutable_attr();
				}

				change->add_type((*it).type);
				change->add_value((*it).value);
				change->add_state(eAttackSim);
			}

			if(attr.has_attr())
			{
				CSceneMgr::GetInstance()->SendSynchAttrChange(attr,object->GetMapID(),object->GetXNewPos(), object->GetYNewPos(), false);
			}
		}
	}
}

int SkillMgr::GetspecifiedSkillLvCount(int lv)
{
	int count = 0;
	map<int,OneTalentSkill>::iterator tIt = m_SkillList.find(m_TalentID);

	if(tIt != m_SkillList.end())
	{
		for(size_t i = 0;i < tIt->second.m_SkillList.size(); ++i)
		{
			SkillInfo info;

			SkillLoader::GetInstance()->GetSkillInfoByID(tIt->second.m_SkillList[i], info);
			if(info.nowLv >= lv)
				count++;
		}
	}

	return count;
}

void SkillMgr::AddSkill(CSkillBase * skill)
{
	m_curSkillList[skill->GetOnlyID()] = skill;
}

void SkillMgr::SetSkillNull(uint id)
{
	map<uint,CSkillBase *>::iterator it = m_curSkillList.find(id);

	if(it != m_curSkillList.end())
	{
		m_curSkillList.erase(it);
	}
}

ServerRet SkillMgr::upgradeOneSkill(const SkillInfo& info,int& contribute,int& exp,int& money,int& force,map<int,short>& skillBooks)
{
	if(IsExistedSkill(info.skillID))
	{
		return eSkillIsNull;
	}	

	if(info.limitPro != m_owner->getAttManage()->getValue<BYTE>(eCharProfession))
	{
		return eProfessionNotMatch;
	}

	if(info.learnLv > m_owner->getAttManage()->getValue<int>(eCharLv))
	{
		return eLVNotMatch;
	}

	if(money + info.useMoney> m_owner->getAttManage()->getValue<int>(eCharMoney))
	{
		return eNotEnoghMoney;
	}
	money += info.useMoney; 

	if(exp + info.useExp > m_owner->getAttManage()->getValue<int64>(eCharExp))
	{
		return eNotEnoughExp;
	}
	exp += info.useExp;

	if(force + info.useForce > m_owner->getAttManage()->getValue<int>(eCharForce))
	{
		return eNotEnoughForce;
	}
	force += info.useForce;

	if(contribute + info.useFactionCon > m_owner->getAttManage()->getValue<int>(eFaContribution))
	{
		return eNotEnoughFContri;
	}
	contribute += info.useFactionCon;

	//技能书扣减
	if(m_owner->GetContainer(ePackageType)->FindPropCount(info.skillBook) < skillBooks[info.skillBook] + info.upgradeNum)
	{
		return eItemNotEnough;
	}
	skillBooks[info.skillBook] += info.upgradeNum;

	return eRetSuccess;
}

int SkillMgr::SkillUpgrade(int id,eUpSkillType type)
{
	Smart_Ptr<Player> player = m_owner->GetMyself();
	const SkillInfo* tLearnInfo = NULL;
	const SkillInfo* tInfo	  = NULL;
	tLearnInfo = tInfo = SkillLoader::GetInstance()->GetSkillInfoByID(id);

	if(tInfo == NULL)
	{
		return eSkillIsNull;
	}	

	if(IsExistedSkill(tInfo->skillID))
	{
		tLearnInfo = SkillLoader::GetInstance()->GetSkillInfoByID(tInfo->nextSkillID);
		if(tLearnInfo == NULL)
		{
			return eSkillIsNull;
		}
	}
	else
	{
		//同组判断
		//新学技能
		SkillInfo sinfo;
		map<int,OneTalentSkill>::iterator tIt = m_SkillList.find(tInfo->m_TalentID);
		if(tIt != m_SkillList.end())
		{
			for(size_t i = 0;i  < tIt->second.m_SkillList.size(); ++i)
			{
				SkillLoader::GetInstance()->GetSkillInfoByID(tIt->second.m_SkillList[i], sinfo);
				if(sinfo.skillID == -1)
				{
					return eSkillIsNull;
				}

				if(sinfo.minLv == tInfo->minLv)
				{
					return eSkillIsNull;
				}
			}
		}
	}

	int tContribute = 0;
	int tExp = 0;
	int tMoney = 0;
	int tForce = 0;
	map<int,short> tSKillBooks;
	ServerRet tError = eRetSuccess;
	bool tOne = false;

	const SkillInfo* tCurrentInfo = tLearnInfo;
	if(type == eUpSkillOne)
	{
		tError = upgradeOneSkill(*tLearnInfo,tContribute,tExp,tMoney,tForce,tSKillBooks);
	}
	else
	{
		while(tCurrentInfo != NULL && tError == eRetSuccess)
		{
			tError = upgradeOneSkill(*tCurrentInfo,tContribute,tExp,tMoney,tForce,tSKillBooks);
			if(tError == eRetSuccess)
			{
				tOne = true;	
				tLearnInfo = tCurrentInfo;	
				tCurrentInfo = SkillLoader::GetInstance()->GetSkillInfoByID(tLearnInfo->nextSkillID);
			}
		}
	}

	if(tError != eRetSuccess && !tOne)
	{
		return tError;
	}

	map<int,short>::iterator tBookIt = tSKillBooks.begin();
	for(;tBookIt != tSKillBooks.end();++tBookIt)
	{
		if(tBookIt->second == 0)
		{
			continue;
		}
		m_owner->GetContainer(ePackageType)->DeleteGoodsById(tBookIt->first,tBookIt->second,true);
	}

	vector<int> vec;
	if(tContribute > 0)
	{
		m_owner->ChangeFContri(tContribute, false);
		m_owner->SynchCharAttributeToWS(eFaContribution, -tContribute);
		vec.push_back(eFaContribution);
	}

	if(tExp > 0)
	{
		m_owner->DecreaseExp(tExp);
		vec.push_back(eCharExp);
	}

	if(tMoney > 0)
	{
		m_owner->ChangeMoney(tMoney, false);
		vec.push_back(eCharMoney);
	}

	if(tForce > 0)
	{
		m_owner->ChangeCurForce(tForce, false);
		vec.push_back(eCharForce);
	}

	if(vec.size() > 0)
	{
		m_owner->SynCharAttribute(vec);
	}

	m_owner->SetDataFlag(eSkillInfo);


	tError = eRetSuccess;

	m_SkillList[tLearnInfo->m_TalentID].m_SkillList.push_back(tLearnInfo->skillID);

	if(tLearnInfo != tInfo)
	{
		map<int,OneTalentSkill>::iterator tIt = m_SkillList.find(tInfo->m_TalentID);
		if(tIt != m_SkillList.end())
		{
			for(size_t i = 0;i < tIt->second.m_SkillList.size();++i)
			{
				if(tIt->second.m_SkillList[i] == id)
				{
					tIt->second.m_SkillList.erase(tIt->second.m_SkillList.begin() + i);
					break;
				}
			}
		}
	}

	if(tLearnInfo != tInfo)
	{
		SkillMgr::ResultEffect(m_owner,tInfo);
	}

	SkillMgr::ResultEffect(m_owner,tLearnInfo,true);

#ifdef DEBUG
	LOG_ERROR(FILEINFO,"playerSkillUpGrade playerID:%lld skillID:%d",m_owner->GetID(),tInfo->nextSkillID);
#endif

//	StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(player, eStatic_Skill,eStaMinor_Skill_LevelUp,1);
	SkillUpgradeArgs e(player->GetMyself(),tLearnInfo->nowLv,tLearnInfo->skillID);
	player->FireEvent(PLAYER_SKILL_UPGRADE,e);


//	if(eAccountType_User == player->GetAccountType())
//	{
//		//统计一天消耗的铜钱数
//		StatisticMgr::GetInstance()->StatisticDailyTimes(eStatic_Skill,eStaMinor_Skill_LevelUpCostMoney,tLearnInfo->useMoney);
//
//		GLOBAL_STATISTIC_GM(player, GlobalType::Main::eSkill, GlobalType::Minor::eSkill_UpgradeCostMoney, StatistSaveType::Int64_Data(tLearnInfo->useMoney));
//
//
//		//统计一天消耗的总真气数
//		StatisticMgr::GetInstance()->StatisticDailyTimes(eStatic_Skill,eStaMinor_Skill_LevelUpForce,tLearnInfo->useForce);

	GLOBAL_STATISTIC_GM(player, GlobalType::Main::eSkill, GlobalType::Minor::eSkill_UpgradeCostForce, StatistSaveType::Int64_Data(tLearnInfo->useForce));
//	}



	return tLearnInfo->skillID;
}

int SkillMgr::ClanSkillUpGrade(int id, int& newid)
{
	newid = 0;
	vector<int> vec;

	Smart_Ptr<Player> player = m_owner->GetMyself();
	SkillInfo* tLearnInfo = NULL;
	SkillInfo* tInfo	  = NULL;

	tInfo = SkillLoader::GetInstance()->GetSkillInfoByID(id);
	if(tInfo == NULL)
	{
		return eSkillIsNull;
	}

	if(IsExistedClanSkill(id))
	{
		tLearnInfo = SkillLoader::GetInstance()->GetSkillInfoByID(tInfo->nextSkillID);
		if(tLearnInfo == NULL)
		{
			return eHighestLv;
		}
	}
	else
	{
		tLearnInfo = tInfo;
	}

	//暂时取值
	if(tLearnInfo->limitPro != ClanSkillType_Player && tLearnInfo->limitPro != ClanSkillType_GirlWar)
	{
		return eProfessionNotMatch;
	}

	if(tLearnInfo->learnLv > m_owner->getAttManage()->getValue<int>(eCharLv))
	{
		return eLVNotMatch;
	}

	if(tLearnInfo->useMoney > m_owner->getAttManage()->getValue<int>(eCharMoney))
	{
		return eNotEnoghMoney;
	}

	if(tLearnInfo->useExp > m_owner->getAttManage()->getValue<int64>(eCharExp))
	{
		return eNotEnoughExp;
	}

	if(tLearnInfo->useForce > m_owner->getAttManage()->getValue<int>(eCharForce))
	{
		return eNotEnoughForce;
	}

	if(tLearnInfo->useFactionCon > m_owner->getAttManage()->getValue<int>(eFaContribution))
	{
		return eNotEnoughFContri;
	}

	//更新技能
	m_clanSkills.push_back(tLearnInfo->skillID);
	newid = tLearnInfo->skillID;

	if(tLearnInfo != tInfo)
	{
		for(size_t i = 0; i < m_clanSkills.size(); ++i)
		{
			if(m_clanSkills[i] == id)
			{
				m_clanSkills.erase(m_clanSkills.begin() + i);
				break;
			}
		}
	}

	//扣除
	if(tLearnInfo->useExp > 0)
	{
		m_owner->DecreaseExp(tLearnInfo->useExp);
		vec.push_back(eCharExp);
	}

	if(tLearnInfo->useMoney > 0)
	{
		m_owner->ChangeMoney(tLearnInfo->useMoney, false);
		vec.push_back(eCharMoney);
	}

	if(tLearnInfo->useForce > 0)
	{
		m_owner->ChangeCurForce(tLearnInfo->useForce, false);
		vec.push_back(eCharForce);
	}

	if(tLearnInfo->useFactionCon > 0)
	{
		m_owner->ChangeFContri(tLearnInfo->useFactionCon, false);
		m_owner->SynchCharAttributeToWS(eFaContribution, -tLearnInfo->useFactionCon);
		vec.push_back(eFaContribution);
	}

	if(vec.size() > 0)
	{
		m_owner->SynCharAttribute(vec);
	}

	m_owner->SetDataFlag(eSkillInfo);

	//增加帮派技能的效果，待定
	if(tLearnInfo != tInfo)
	{
		if(tLearnInfo->limitPro == ClanSkillType_Player)
		{
			SkillMgr::ResultEffect(m_owner,tInfo,true,false);
		}
		else
		{
			SkillMgr::ResultEffect(m_owner->GetGirlWarManager()->GetGirlWar().Get(),tInfo,true,false);
		}
	}

	if(tLearnInfo->limitPro == ClanSkillType_Player)
	{
		SkillMgr::ResultEffect(m_owner,tLearnInfo);
	}
	else
	{
		SkillMgr::ResultEffect(m_owner->GetGirlWarManager()->GetGirlWar().Get(),tLearnInfo,false,false);
	}

	return 0;
}

void SkillMgr::SetBuffInfo(PlayerSynch::SynchPlayerBase * info)
{
	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();
	for(; it!=m_buffList.end(); ++it)
	{
		const BuffInfo* binfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(it->second->GetID());
		if(binfo == NULL)
		{
			LOG_ERROR(FILEINFO, "add buff but buff is null");

			continue;
		}

		if(binfo->isResult > 0)
		{
			PlayerSynch::SynchBuffChar * buf = info->add_buf();

			buf->set_id(it->second->GetID());
			buf->set_onlyid(it->second->GetOnlyID());
		}
	}
}

//结束连斩BUFF
void SkillMgr::EndContinueBeheaded()
{
	map<DWORD, BuffBase *>::iterator it = m_buffList.begin();

	for(;it != m_buffList.end();)
	{
		const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(it->second->GetID());

		if(info == NULL)
		{
			continue ;
		}

		if(info->useType == eAttack)
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
		else
		{
			++it;
		}
	}
}

//获得为参数指定特效的BUFF，目前用于霸体
void SkillMgr::getBuffByEffectType(vector<DWORD>& onlyIDList,const BuffGetPublicData& data)
{
	map<DWORD, BuffBase *>::iterator tempIt = m_buffList.begin();
	for(;tempIt != m_buffList.end();++tempIt)
	{
		const BuffInfo* tempInfo = SkillLoader::GetInstance()->GetBuffInfoPtrByID(tempIt->second->GetID());

		if(tempInfo->useType != data.m_Type)
			continue;

		onlyIDList.push_back(tempIt->first);
	}

	return ;
}

//是否有一个技能等级大过参数指定等级
bool SkillMgr::isHaveSkillLevelMoreThan(int level)
{
	map<int,OneTalentSkill>::iterator tIt = m_SkillList.begin();

	SkillInfo* tempInfo = NULL;
	for(;tIt != m_SkillList.end();++tIt)
	{
		for(size_t i = 0;i < tIt->second.m_SkillList.size();++i)
		{
			tempInfo = SkillLoader::GetInstance()->GetSkillInfoByID(tIt->second.m_SkillList[i]);
			if(tempInfo == NULL)
				continue;

			if(tempInfo->nowLv >= level)
				return true;
		}
	}

	return false;
}

// skillType: true 主动技能 false 被动技能
bool SkillMgr::IsHaveSkillUp(ESkillInterfaceType skillType)
{
	vector<int> vecSkillID;
	SkillLoader::GetInstance()->GetIDByProfAndType((ECharProf)m_owner->getAttManage()->getValue<BYTE>(eCharProfession),
			skillType, vecSkillID);

	for (uint i = 0; i < vecSkillID.size(); ++i)
	{
		int skillID = 0;
		map<int,OneTalentSkill>::iterator tIt = m_SkillList.begin();
		bool tIfind = false;
		for( ; tIt != m_SkillList.end(); ++tIt)
		{
			for(size_t j = 0;j < tIt->second.m_SkillList.size();++j)
			{
				SkillInfo info;
				SkillLoader::GetInstance()->GetSkillInfoByID(tIt->second.m_SkillList[j], info);
				if(info.skillID == -1)
				{
					continue;
				}

//				printf("skillID=%d,  info.minLv=%d\n", vecSkillID[i], info.minLv);
				if (info.minLv != vecSkillID[i])
				{
					continue;
				}

				skillID = info.skillID;
				tIfind = true;
				break;
			}

			if(tIfind)
			{
				break;
			}
		}

		if (0 < skillID && 0 == IsCanUpgrade(skillID))
		{
			return true;
		}
	}

	return false;
}

int SkillMgr::IsCanUpgrade(int id)
{
	SkillInfo info;
	Smart_Ptr<Player> player = m_owner->GetMyself();
	if (!player->IsOpenSystem(eStSkill))
	{
		return eNotOpen;
	}
	SkillLoader::GetInstance()->GetSkillInfoByID(id, info);
	if(info.skillID == -1)
	{
		return eSkillIsNull;
	}

	if(info.nextSkillID <= 0)
	{
		return eHighestLv;
	}

	SkillInfo newInfo;

	if (IsExistedSkill(id))
	{
		SkillLoader::GetInstance()->GetSkillInfoByID(info.nextSkillID, newInfo);
	}
	else
	{
		newInfo = info;
	}

	if(newInfo.skillID == -1)
	{
		return eSkillIsNull;
	}

	if(newInfo.limitPro != m_owner->getAttManage()->getValue<BYTE>(eCharProfession))
	{
		return eProfessionNotMatch;
	}

	if(newInfo.learnLv > m_owner->getAttManage()->getValue<int>(eCharLv))
	{
		return eLVNotMatch;
	}

	if(newInfo.useMoney > m_owner->getAttManage()->getValue<int>(eCharMoney))
	{
		return eNotEnoghMoney;
	}

	if(newInfo.useExp > m_owner->getAttManage()->getValue<int64>(eCharExp))
	{
		return eNotEnoughExp;
	}

	if(newInfo.useForce > m_owner->getAttManage()->getValue<int>(eCharForce))
	{
		return eNotEnoughForce;
	}

	if(newInfo.useFactionCon > m_owner->getAttManage()->getValue<int>(eFaContribution))
	{
		return eNotEnoughFContri;
	}

	//技能书扣减
	if(m_owner->GetContainer(ePackageType)->FindPropCount(newInfo.skillBook) < newInfo.upgradeNum)
	{
		return eItemNotEnough;
	}

	return 0;
}


void SkillMgr::getSynData(PlayerSynch::SynchPlayerBase& data)
{
	map<uint,CSkillBase *>::iterator tempIt = m_curSkillList.begin();

	for(;tempIt != m_curSkillList.end();++tempIt)
	{
		//只同步一个技能，任取一个
		if(tempIt->second->getSynData(data))
		{
			break;
		}
	}

	return ;
}

const vector<int>& SkillMgr::getClanSkillID()
{
	 return m_clanSkills;
}








