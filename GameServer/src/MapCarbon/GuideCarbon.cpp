/*
 * GuideCarbon.cpp
 *
 *  Created on: 2015年11月9日
 *      Author: root
 */
#include "GuideCarbon.h"
#include "Log/Logger.h"
#include "../GuideFileLoader/GuideLoader.h"
#include "../Object/ObjectFactory.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../Object/FileLoader/AILoader.h"
#include "../Object/AI/AIBrain.h"
#include "../ServerEventDefine.h"
#include "../Object/FileLoader/MonsterLoader.h"

GuideCarbon::GuideCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):CGameDuplicate(onlyid,charid,paramf,params)
{
	m_KillMonsterCount = 0;
	m_state = false;
}

GuideCarbon::~GuideCarbon()
{
	if(m_FlushTimer.IsValid())
	{
		m_FlushTimer.SetNull();
	}
}

int GuideCarbon::Finish(Smart_Ptr<Player>& player)
{
	if(!GetTaskState())
	{
		return -1;
	}

	m_awardTime = CUtil::GetNowSecond();
	m_process = eWinFight;

	return 0;
}

//怪物死亡事件
bool GuideCarbon::HandleMonsterDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
	{
		return true;
	}

	TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);
	if( tArgs==NULL || tArgs->m_myself.Get()==NULL ||
			GetSceneID() != tArgs->m_myself->GetMapID() )
	{
		return true;
	}

	int NeedCount = GuideLoader::GetInstance()->GetNeedKillCount();
	int BossID = GuideLoader::GetInstance()->GetBossID();

	//击杀的小怪数量后刷出第二波怪物
	Smart_Ptr<Monster> mon = tArgs->m_myself;
	if(mon->GetID() != BossID)
	{
		if(NeedCount == ++m_KillMonsterCount)
		{
			Smart_Ptr<Player> player = GetPlayer(m_owner);
			if(player)
			{
				FlushNPC(player);

				//开启定时器
				if(m_FlushTimer.IsValid())
				{
					m_FlushTimer.SetNull();
				}

				int times = GuideLoader::GetInstance()->GetDelayTime();
				m_FlushTimer = TimerInterface::GetInstance()->RegTimePoint(&GuideCarbon::OnFlushTimer, this, NULL, 0, 0, 0, times);
			}
		}
	}
	else
	{
		//最终boss死亡，更新新手副本完成状态
		SetTaskState(true);

		//杀死所有怪物
		const INTOBJECTMAP ref = GetAllObjects();
		INTOBJECTMAP::const_iterator it = ref.begin();
		for(; it != ref.end(); ++it)
		{
			if(it->second.Get()==NULL || it->second->GetType()!=eMonster)
				continue;

			Smart_Ptr<Monster> ref = it->second;
			if(ref->GetKey() == mon->GetKey())
				continue;

			if(ref->IsDead())
				continue;

			if(eCompeteMonster == ref->GetQuality())
				continue;

			ref->SetDead(NULL);
		}
	}

	return true;
}

//人物死亡
bool GuideCarbon::HandlePlayerDeadEvent(const EventArgs & e)
{
	PlayerKilledEventArgs *tArgs = (PlayerKilledEventArgs *)(&e);
	if(tArgs == NULL)
		return true;

	if(tArgs->m_dead.Get()==NULL || tArgs->m_dead->GetType()!= ePlayer)
		return true;

	if(tArgs->m_dead->GetID() != m_owner)
		return true;

	//死亡后，算通关了
	SetTaskState(true);

	return true;
}

void GuideCarbon::FlushNPC(Smart_Ptr<Player>& player)
{
	const std::vector< pair<int,int> >& ref = GuideLoader::GetInstance()->GetPosition();

	int profession = player->getAttManage()->getValue<int>(eCharProfession);
	int country = player->getAttManage()->getValue<int>(eCharCountry);
	int index(0);

	std::vector<int> vkeys;

	for(int i=eMuscleMan; i<=eShooter; ++i)
	{
		if(i == profession)
		{
			continue;
		}

		//随机性别
		int sex = rand()%2;

		//生成出生点
		if(index >= (int)ref.size())
		{
			return;
		}

		//根据职业 性别 位置生成怪物
		int key = 0;
		FlushNPC(i, sex, country, player->GetMapID(), ref[index].first, ref[index].second, key);
		if(key != 0)
		{
			vkeys.push_back( key );
		}
		++index;
	}

	ServerReturn::ServerRetInt32Array ret;
	for(size_t n=0; n<vkeys.size(); ++n)
	{
		ret.add_ret(vkeys[n]);
	}

	player->SendSynchCommon(&ret, MSG_REQ_GM2C_FLUSH_GUIDER);

}

void GuideCarbon::FlushNPC(int profession, int sex, int country, int64 mapid, int posx, int posy, int &key)
{
	//根据玩家职业 取对应的怪物职业   性别
	const GuiderBaseInfo* pInfo = GuideLoader::GetInstance()->GetGuiderInfo(profession,sex);
	if(pInfo == NULL)
	{
		LOG_ERROR(FILEINFO,"\n 刷新怪物失败 \n");
		return;
	}

	BaseAIInfo *aiInfo = 0;
	BetterAIInfo *betterAI = 0;
	Smart_Ptr<Object> obj;
	Smart_Ptr<Monster> mon;
	vector<Sector *> moveVec;
	vector<int> skillInit;

	obj = ObjectFactory::GetInstance()->AllocObject(eMonster);
	mon = obj;


	ArenaInfo *info = AILoader::GetInstance()->GetArenaAIByid(profession);
	if(!info)
		return ;

	MonsterInfo monsterInfo;
	MonsterLoader::GetInstance()->GetMonsterInfo(10000, monsterInfo);

	skillInit.resize(eSkill_Max);
	mon->SetCompeteData();
	mon->SetID(monsterInfo.id);
	mon->SetCompeteData(eCompeteProfession, profession);
	mon->SetCompeteData(eCompeteSex, sex);
	mon->SetCompeteData(eCompeteTitleID, 0);
	mon->SetCompeteData(eCompeteMagic, pInfo->magic);
	mon->SetCompeteData(eCompeteVipLv, 0);
	mon->SetCompeteData(eCompeteFashion, 0);
	mon->SetCompeteData(eCompeteWeapon, pInfo->weapon);
	mon->SetCompeteData(eCompeteAGuard, pInfo->fashion);
	mon->SetCompeteData(eCompeteHorse, pInfo->hourse);
	mon->SetCompeteData(eCompeteClanJob, 0);

	mon->SetName(pInfo->name);
	mon->getAttManage()->setValue(eCharLv,1);
	mon->SetQuality(monsterInfo.quality);
	mon->SetFlushTime(monsterInfo.flushTime);
	mon->SetAliveTime(monsterInfo.aliveTime);
	mon->getAttManage()->setValue(eCharCountry, country);

	mon->getAttManage()->setValue(eCharMoveSpeed,200);
	mon->SetRunSpeed(300);
	mon->SetFallID(monsterInfo.fallID);
	mon->getAttManage()->setValue(eCharExp,monsterInfo.exp);
	mon->getAttManage()->setValue(eCharPhyAttackMax,pInfo->attack);
	mon->getAttManage()->setValue(eCharPhyDefence,pInfo->defene);
	mon->getAttManage()->setValue(eCharHitRate,pInfo->hitRate);
	mon->getAttManage()->setValue(eCharAvoidRate,pInfo->avoidRate);
	mon->getAttManage()->setValue(eCrackDownRate,pInfo->DownRate);
	mon->getAttManage()->setValue(eAvoidCrackRate,pInfo->AvoidCrackRate);
	mon->SetHPMax(pInfo->hp);
	mon->SetCurHP(pInfo->hp);
	mon->SetMPMax(pInfo->mp);
	mon->SetCurMP(pInfo->mp);
	mon->SetGatherTime(monsterInfo.gatherTime);
	mon->SetOwnerToBrain(mon);
	mon->SetMapID(mapid);
	mon->SetBornTime();
	mon->SetOwnerName("");
	mon->SetXPos(posx);
	mon->SetYPos(posy);
	mon->SetOriginalX(posx);
	mon->SetOriginalY(posy);

	for(size_t j=0; j<info->nSkill.size(); ++j)
	{
		SkillInfo skillinfo;
		SkillLoader::GetInstance()->GetSkillInfoByID(info->nSkill[j], skillinfo);
		if(skillinfo.skillID == -1)
		{
			continue;
		}

		skillInit[j] = info->nSkill[j];
	}

	Join(obj);

	key = mon->GetKey();

	aiInfo = AILoader::GetInstance()->GetBaseAIByid(monsterInfo.baseAI);
	if(!aiInfo)
	{
		return;
	}

	if(monsterInfo.betterAI > 0)
	{
		betterAI = AILoader::GetInstance()->GetBetterAIByid(monsterInfo.betterAI);
		if(!betterAI)
		{
			return;
		}
	}

	mon->SetSkill(skillInit);
	mon->SetStayTime(aiInfo->stayTime);
	mon->SetPhyAttackTime(info->nCycleTime);

	//GetSectorInPos(moveVec, posx, posy, aiInfo->moveDistance);
	//mon->SetMoveSector(moveVec);

	Smart_Ptr<AIBrain> monBrain = mon->GetBrain();
	if(!monBrain)
		return ;

	mon->GetBrain()->SetMonsterType(eRandomWayPos);

	monBrain->Init(aiInfo, betterAI);
	m_allObject[mon->GetKey()] = mon;

	INTVMPNSTERMAP::iterator it = m_allMonster.find(monsterInfo.id);
	if(it == m_allMonster.end())
	{
		vector<Smart_Ptr<Monster> > vecMonster;

		vecMonster.push_back(mon);
		m_allMonster[monsterInfo.id] = vecMonster;
	}
	else
	{
		it->second.push_back(mon);
	}

	return;
}

void GuideCarbon::FlushSecondMonsters()
{
	const std::vector<GuideMonsterData>& ref = GuideLoader::GetInstance()->GetSecondMonsters();

	for(size_t i=0; i<ref.size(); ++i)
	{
		FlushMonster(ref[i].id, ref[i].posx, ref[i].posy);
	}

}

void GuideCarbon::FlushThirdMonsters()
{
	const std::vector<GuideMonsterData>& ref = GuideLoader::GetInstance()->GetThirdMonsters();

	for(size_t i=0; i<ref.size(); ++i)
	{
		FlushMonster(ref[i].id, ref[i].posx, ref[i].posy);
	}

}

void GuideCarbon::FlushMonster(int monster, int posx, int posy)
{
	Smart_Ptr<Monster> mon = CGameZone::CreateMonster(monster, posx, posy);
	if(mon.Get() == NULL)
	{
		LOG_ERROR(FILEINFO, "\n GuideCarbon::FlushMonster error: id=%d \n", monster);
	}

}

void GuideCarbon::OnFlushTimer(void* args)
{
	FlushSecondMonsters();

	FlushThirdMonsters();
}

//初始化玩家信息
void GuideCarbon::InitPlayerInfo(Smart_Ptr<Player>& player)
{
	std::vector<int> vecType;

	//重置等级
	player->getAttManage()->setValue(eCharLv, (int)100);

	//重置属性
	std::vector< pair<int,int> > refAttrs = GuideLoader::GetInstance()->GetAddAttrs();
	std::vector< pair<int,int> >::iterator itAttr = refAttrs.begin();
	for(; itAttr != refAttrs.end(); ++itAttr)
	{
		player->getAttManage()->addValue((ECharAttrType)itAttr->first,itAttr->second);
		vecType.push_back(itAttr->first);
	}

	//重置技能
	const std::map<int, vector<int> >& refSkills = GuideLoader::GetInstance()->GetAddSkills();
	const std::map<int, vector<int> >::const_iterator itSkill = refSkills.find( player->getAttManage()->getValue<int>(eCharProfession) );
	if(itSkill != refSkills.end())
	{
		player->GetSkillMgr()->GetSkillInfos(m_skillList);
		player->GetSkillMgr()->ReSetSkillInfos(itSkill->second);
	}

	if(vecType.size() > 0)
	{
		player->SynCharAttribute(vecType);
	}

}

int GuideCarbon::Init()
{
	int ret = CGameDuplicate::Init();
	return ret;
}

//更新状态,false表示要删除这个场景
bool GuideCarbon::Update()
{
	switch(m_process)
	{
	case eCreated:
	{
		if(CUtil::GetNowSecond() - m_createTime >= m_data->m_nLimitTime)
		{
			m_process = eOver;
		}
		break;
	}

	case ePlayerWork:
	{
		if(CUtil::GetNowSecond() - m_createTime >= m_data->m_nLimitTime)
		{
			OldMapInfo * pPos = GetOldMapPoint(m_owner);
			if(pPos != NULL)
			{
				pPos->xpos = 59;
				pPos->ypos = 14;
			}

			SetTaskState(true);

			ForceAllPlayerChangeMap();
			m_process = eOver;
		}
		break;
	}

	case eWinFight:
	{
		int64 curTime = CUtil::GetNowSecond();
		if(curTime - m_awardTime >= 100 )
		{
			//校正位置
			OldMapInfo * pPos = GetOldMapPoint(m_owner);
			if(pPos != NULL)
			{
				pPos->xpos = 59;
				pPos->ypos = 14;
			}
			ForceAllPlayerChangeMap();
			m_process = eOver;
		}

		break;
	}

	case eOver:
	{
		return false;
	}

	default:
	{
		break;
	}
	}

	return true;
}

//加入场景
int GuideCarbon::Join(Smart_Ptr<Object> & obj)
{
	if(!JoinHandle(obj))
	{
		return -1;
	}

	int ret=CGameZone::Join(obj);
	if(ret!=0)
	{
		return ret;
	}

	if(obj->GetType() == ePlayer)
	{
		if(m_process == eCreated)
		{
			m_process = ePlayerWork;
		}

		Smart_Ptr<Player> player = obj;
		InitPlayerInfo(player);

		if(!m_monsterDeadEvent.IsValid())
		{
			m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &GuideCarbon::HandleMonsterDeadEvent);
		}

		if(!m_playerDeadEvent.IsValid())
		{
			m_playerDeadEvent = RegGlobalEventPtr(CREATURE_DEAD_EVENT_TYPE, this, &GuideCarbon::HandlePlayerDeadEvent);
		}

	}

	return 0;
}

//加入场景前处理
bool GuideCarbon::JoinHandle(Smart_Ptr<Object> & obj)
{
	return true;
}

		//场景中移动
int GuideCarbon::Move(Smart_Ptr<Object> &obj)
{
	return CGameZone::Move(obj);
}


//离开场景，isDel是否广播移出
void GuideCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	CGameZone::Leave(obj, isDel, delFromMap);
	LeaveHandle(obj);
}

//离开场景前处理
void GuideCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
{
	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		std::vector<int> vecType;

		//重置等级
		player->getAttManage()->setValue(eCharLv, (int)1);

		//重置技能
		player->GetSkillMgr()->ReSetSkillInfos(m_skillList);

		//重置buffer
		player->GetSkillMgr()->ClearBuffer();

		//重置属性
		std::vector< pair<int,int> > refAttrs = GuideLoader::GetInstance()->GetAddAttrs();
		std::vector< pair<int,int> >::const_iterator itAttr = refAttrs.begin();
		for(; itAttr != refAttrs.end(); ++itAttr)
		{
			player->getAttManage()->addValue((ECharAttrType)itAttr->first,-itAttr->second);
			vecType.push_back( itAttr->first );
		}

		//设置当前血量和蓝为最大
		int maxHP = player->getAttManage()->getValue<int>(eCharHPMax);
		int maxMP = player->getAttManage()->getValue<int>(eCharMPMax);;
		player->getAttManage()->setValue(eCharHP, maxHP);
		player->getAttManage()->setValue(eCharMP, maxMP);


		//只有在离开后并且通过了才设置标识，否则有可能导致存贮错误数据
		if(GetTaskState())
		{
			player->getAttManage()->setValue(eGuide, (int)1);
			player->SetDataFlag(eBaseInfo);
		}

		if(vecType.size() > 0)
		{
			player->SynCharAttribute(vecType);
		}

		//玩家离开就销毁
		OldMapInfo * pPos = GetOldMapPoint(m_owner);
		if(pPos != NULL)
		{
			MapStaticInfo *sMapInfo = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(pPos->mapid));
			if(sMapInfo==NULL)
				return;

			player->SetMapID(pPos->mapid);
			player->SetXPos(sMapInfo->reliveXPos);
			player->SetYPos(sMapInfo->reliveYPos);
			player->SetXNewPos();
			player->SetYNewPos();
			player->SetDataFlag(eBaseInfo);
		}
		m_process = eOver;
	}
}

//玩家强制退出副本检验(用于判断玩家退出后副本的后续操作 )
void GuideCarbon::LeaveExame(Smart_Ptr<Player> & player)
{

}

//此场景是否可以进入
bool GuideCarbon::IsCanEnter(Smart_Ptr<Player> & player)
{
	if(m_process >= eDuplicateChangeMap)
				return false;

	if(player->GetID() != m_owner)
				return false;

	return true;
}

//赢了后的操作
void GuideCarbon::Win()
{

}

//副本结束时间到了之后的 操作
void GuideCarbon::Over()
{

}

//副本人数为零时的操作
void GuideCarbon::Count()
{

}

void GuideCarbon::CountEx()
{

}

//输了之后的操作
void GuideCarbon::Failed()
{

}

//奖励
void GuideCarbon::Award()
{

}

