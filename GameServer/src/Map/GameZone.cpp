/*
 * GameZone.cpp
 *
 *  Created on: 2013-9-28
 *      Author: helh
 */

#include "GameZone.h"
#include <time.h>
#include "./ConfigLoader/MapConfig.h"
#include "./ConfigLoader/FieldInfo.h"
//#include "./ConfigLoader/FieldNPC.h"
#include "SvrConfig.h"
#include "Log/Logger.h"
#include "../Object/ObjectFactory.h"
#include "../Object/FileLoader/NPCLoader.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../Object/FileLoader/AILoader.h"
#include <math.h>
#include "../Skill/SkillLoader/SkillLoader.h"
#include "../Object/ItemDrop/DropItemMgr.h"
#include "../Object/GirlWar/GirlWar.h"
#include "../Object/GirlWar/AI/PetBrain.h"
#include "SceneManager.h"
#include "../Object/AI/AIBrain.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "../Skill/Effect/EffectMath.h"
#include "ReflushManage/ReflushFile.h"
#include "ReflushManage/ReflushManage.h"
#include "FileLoader/InternationalLoader.h"
#include "../Country/GsCountryMgr.h"
#include "../Object/Creature.h"

CGameZone::CGameZone(int64 sceneid):m_sceneID(sceneid),m_totalGroupSize(0),m_totalSector(0),m_totalGroup(0),m_springTick(0)
{

}

CGameZone::~CGameZone()
{
	if(GET_MAP_TYPE(GetSceneID()) < eCopyMap)
	{
		LOG_ERROR(FILEINFO, "CGameZone release : mapid=%d", GET_MAP_ID(GetSceneID()));
	}

	//删除掉落物品
	Smart_Ptr<Player> nullPlayer;
	Smart_Ptr<Monster> nullMon;
	Smart_Ptr<CreatureObj> nullObj;
	Smart_Ptr<GirlWar> nullGirlWar;
	INTOBJECTMAP::iterator it = m_allObject.begin();
	for(;it != m_allObject.end(); it++)
	{
		if(it->second->GetType() == eDropItem)
		{
			//Smart_Ptr<DropItemsAllot> item = it->second;
			//item->Release();

			//ObjectFactory::GetInstance()->Recycle(it->second);
			DropItemsManager::GetInstance()->DestoryItemBox(it->first);
		}

		if(it->second->GetType() == ePlayer || it->second->GetType() == ePet)
		{
			Smart_Ptr<CreatureObj> tCreature = it->second;

			tCreature->ResetGameZone();
			tCreature->ResetSector();
		}
		else if(it->second->GetType() == eMonster)
		{
			Smart_Ptr<Monster> mon = it->second;
			//mon->Release();
			mon->SetOwnerToBrain(nullMon);
			mon->SetOwner(nullPlayer);
			mon->SetTarget(nullObj);
			mon->GetBrain()->ClearEnemy();
			mon->ResetGameZone();
			mon->ResetSector();

			//ObjectFactory::GetInstance()->Recycle(it->second);
		}
	}

	m_player.clear();
	m_npc.clear();
	m_allMonster.clear();
	m_allObject.clear();
	m_girlWar.clear();

	if(m_totalSector)
	{
		delete[] m_totalSector;
		m_totalSector = 0;
	}

	if(m_totalGroup)
	{
		delete[] m_totalGroup;
		m_totalGroup = 0;
	}

}

int CGameZone::InitField()
{
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(info)
	{
		int totalsize = info->GetTotalSize();
		int i=0;

		m_totalSector = new Sector[totalsize];
		for(; i<totalsize; ++i)
		{
			m_totalSector[i].Init(i, this);
			m_totalSector[i].SetSectorFlag(info->IsValidSector(i));
		}

		int xGroup = info->GetNineXSize();
		int yGroup = info->GetNineYSize();

		m_totalGroupSize = xGroup * yGroup;
		m_totalGroup = new SectorGroup[m_totalGroupSize];

		for(i=0; i<m_totalGroupSize; ++i)
		{
			m_totalGroup[i].SetGroupIndex(i,this);
		}

		for(i=0; i<m_totalGroupSize; i++)
		{
			SectorGroup * pGroup  = &m_totalGroup[i];
			int xVirGroup = i % xGroup;
			int yVirGroup = i / xGroup;

			//һ������ľŹ���Ÿ���(�����Լ�)
			pGroup->AddNearbyGroup(pGroup);
//			printf("\n++++++++++++++++++++++++++++  sector group [ %d] +++++++++++++++++++++++++++\n\n", pGroup->GetGroupIndex());

			for(int j=-2; j<=2; ++j)
			{
				if((xVirGroup + j) >= xGroup || (xVirGroup + j) < 0)
				{
					continue;
				}

				for(int k=-2; k<=2; ++k)
				{
					if(((yVirGroup + k) < 0) || (yVirGroup + k) >= yGroup)
					{
						continue;
					}

					if(pGroup->GetGroupIndex() == ((xVirGroup + j) + (yVirGroup + k) * xGroup))
					{
						continue;
					}
					pGroup->AddNearbyGroup(&m_totalGroup[(xVirGroup + j) + (yVirGroup + k) * xGroup]);
				}
			}

//			printf("\n\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

			//������а�ĸ���
			int groupXSectorMax = (xVirGroup + 1) * info->GetNineXSector() - 1;
			int groupYSectorMax = (yVirGroup + 1) * info->GetNineYSector() - 1;
			int groupXSectorMin = xVirGroup * info->GetNineXSector();
			int groupYSectorMin = yVirGroup * info->GetNineYSector();

			for(int xIndex=groupXSectorMin; xIndex<=groupXSectorMax; ++xIndex)
			{
				if(xIndex >= info->GetXMaxNum())
				{
					continue;
				}

				for(int yIndex=groupYSectorMin; yIndex<=groupYSectorMax; ++yIndex)
				{
					if(yIndex >= info->GetYMaxNum())
					{
						continue;
					}

					int refIndex = yIndex * info->GetXMaxNum() + xIndex;
					if(refIndex >= totalsize)
					{
						LOG_ERROR(FILEINFO, "sector over write, index=%d,yIndex=%d, xIndex=%d", yIndex * info->GetXMaxNum() + xIndex, yIndex, xIndex);
						printf("\n sector over write : index = %d, yIndex=%d, xIndex=%d \n", yIndex * info->GetXMaxNum() + xIndex, yIndex, xIndex );
						continue;
					}
					pGroup->AddGroupSector(&m_totalSector[refIndex]);
				}
			}

			if(GET_MAP_TYPE(m_sceneID) < eCopyMap && pGroup->GetGroupSectors() > 300)
			{
				LOG_ERROR(FILEINFO, "init gamezone error : mapid=%d, group=%d, size=%d", GET_MAP_ID(m_sceneID), pGroup->GetGroupIndex(), pGroup->GetGroupSectors());
				throw 0;
			}

		}
	}
	else
	{
		LOG_ERROR(FILEINFO, "init gamezone[sceneid = %lld, mapid=%d] error", m_sceneID, GET_MAP_ID(m_sceneID));

		return -1;
	}

	return 0;
}

int CGameZone::InitMonsterByPlayer(Smart_Ptr<Player> player, PlayerInfo::ArenaInfo &req)
{
	ArenaInfo *info= 0;
	MonsterInfo monsterInfo;
	BaseAIInfo *aiInfo = 0;
	BetterAIInfo *betterAI = 0;
	Smart_Ptr<Object> obj;
	Smart_Ptr<Monster> mon;
	vector<Sector *> moveVec;
	vector<int> skillInit;
	string cname;

	obj = ObjectFactory::GetInstance()->AllocObject(eMonster);
	mon = obj;

	const PlayerInfo::ArenaBaseInfo& newBase = req.abinfo();

	info = AILoader::GetInstance()->GetArenaAIByid(newBase.profession());
	if(!info)
		return -1;

	MonsterLoader::GetInstance()->GetMonsterInfo(10000, monsterInfo);

	skillInit.resize(eSkill_Max);
	mon->SetCompeteData();
	mon->SetID(monsterInfo.id);
	mon->SetCompeteData(eCompeteProfession, newBase.profession());
	mon->SetCompeteData(eCompeteSex, newBase.sex());
	mon->SetCompeteData(eCompeteTitleID, newBase.titleid());
	mon->SetCompeteData(eCompeteMagic, newBase.magicw());
	mon->SetCompeteData(eCompeteVipLv, newBase.viplv());
	mon->SetCompeteData(eCompeteAGuard, newBase.costume());
	mon->SetCompeteData(eCompeteHorse, newBase.horse());
	mon->SetCompeteData(eCompeteClanJob, newBase.clanjob());
	mon->SetCompeteData(eCompeteFashion, newBase.suitrnt());
	cname = newBase.clanname();
	mon->SetClanName(cname);
	
	mon->SetCompeteData(eCompeteWeapon, newBase.mainequip().id());
	mon->SetCompeteData(eCompeteEquipStreng, newBase.mainequip().strenggrade());

	string name = newBase.charname();
	string cntyTxt = InternationalLoader::GetInstance()->GetInterStr(GsCountryMgr::GetInstance()->GetShowCntyText(newBase.country()));
	cntyTxt = cntyTxt.length() > 0 ? "【" + cntyTxt + "】" : cntyTxt;
	name.append(cntyTxt.c_str());

	mon->SetName(name);
	mon->getAttManage()->setValue(eCharLv,newBase.level());
	mon->SetQuality(monsterInfo.quality);
	mon->SetFlushTime(monsterInfo.flushTime);
	mon->SetAliveTime(monsterInfo.aliveTime);
	mon->getAttManage()->setValue(eCharCountry, monsterInfo.camp);
//	mon->SetBetterAi(newBase.profession());
	mon->getAttManage()->setValue(eCharMoveSpeed,newBase.movespeed());
	mon->SetRunSpeed(newBase.movespeed());
	mon->SetFallID(monsterInfo.fallID);
	mon->getAttManage()->setValue(eCharExp,monsterInfo.exp);
	mon->getAttManage()->setValue(eCharPhyAttackMax,newBase.phyattackmax());
	mon->getAttManage()->setValue(eCharPhyDefence,newBase.phydefence());
	mon->getAttManage()->setValue(eCharHitRate,newBase.hitrate());
	mon->getAttManage()->setValue(eCharAvoidRate,newBase.avoidrate());
	mon->getAttManage()->setValue(eCrackDownRate,newBase.crackdownrate());
	mon->getAttManage()->setValue(eAvoidCrackRate,newBase.baserte());
	mon->SetHPMax(newBase.hpmax());
	mon->SetCurHP(newBase.hpmax());
	mon->SetMPMax(newBase.mpmax());
	mon->SetCurMP(newBase.mpmax());
	mon->SetGatherTime(monsterInfo.gatherTime);
	mon->SetOwnerToBrain(mon);
	mon->SetMapID(player->GetMapID());
	mon->SetBornTime();
	mon->SetOwnerName("");
	mon->SetXPos(monsterInfo.born_x);
	mon->SetYPos(monsterInfo.born_y);
	mon->SetOriginalX(monsterInfo.born_x);
	mon->SetOriginalY(monsterInfo.born_y);
//	mon->SetGroup(CSceneMgr::GetInstance()->GetSectorGroup(player->GetMapID(), monsterInfo.born_x, monsterInfo.born_y));
	for(int i=0; i<newBase.skill_size(); ++i)
	{

		LOG_DEBUG(FILEINFO,"SKIL ID = %d",newBase.skill(i));
		SkillInfo skillinfo;

		SkillLoader::GetInstance()->GetSkillInfoByID(newBase.skill(i), skillinfo);
		if(skillinfo.skillID == -1)
		{
			return -1;
		}

		if(skillinfo.fType == ePassiveSkill)
			continue;

		for(size_t j=0; j<info->nSkill.size(); ++j)
		{
			if(skillinfo.minLv == info->nSkill[j])
			{
				skillInit[j] = skillinfo.skillID;
			}
		}
	}

	Join(obj);

	aiInfo = AILoader::GetInstance()->GetBaseAIByid(monsterInfo.baseAI);
	if(!aiInfo)
	{
		LOG_ERROR(FILEINFO, "monster[id=%d] ai is error", info->id);

		return -1;
	}

	if(monsterInfo.betterAI > 0)
	{
		betterAI = AILoader::GetInstance()->GetBetterAIByid(monsterInfo.betterAI);
		if (!betterAI)
		{
			LOG_ERROR(FILEINFO, "monster[id=%d] better ai is error", monsterInfo.id);

			return -1;
		}
	}

	mon->SetSkill(skillInit);
	mon->SetStayTime(aiInfo->stayTime);
	mon->SetPhyAttackTime(info->nCycleTime);
	GetSectorInPos(moveVec, monsterInfo.born_x, monsterInfo.born_y, aiInfo->moveDistance);
	mon->SetMoveSector(moveVec);

	Smart_Ptr<AIBrain> monBrain = mon->GetBrain();
	if(!monBrain)
		return -1;

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

//	Smart_Ptr<CreatureObj> creature = player->GetCreatureSelf();
////	mon->GetBrain()->AddEnemy(creature);
//	mon->SetTarget(creature);


	return 0;
}

int CGameZone::InitSpring()
{
	FieldSpring * spring = CMapConfig::GetInstance()->GetFieldSpringByid(GET_MAP_ID(m_sceneID));
	if(!spring)
	{
		return 0;
	}

	vector<MapSpring> vec = spring->GetSpring();
	for(size_t i=0; i<vec.size(); ++i)
	{
		Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eSpring);
		Smart_Ptr<Spring> spObj = obj;

		spObj->SetHPRate(vec[i].hprate);
		spObj->SetRange(vec[i].range);
		spObj->SetMapID(m_sceneID);
		spObj->SetXPos(vec[i].xpos);
		spObj->SetYPos(vec[i].ypos);
//		spObj->SetGroup(GetSectorGroupIndex(vec[i].xpos, vec[i].ypos));

		m_allSpring.push_back(spObj);
	}

	return 0;
}

int CGameZone::CreateMonster(int monsterID,int counts,const vector<Pos>& posList,vector<int>* pKeyList)
{
	MonsterInfo info;

	MonsterLoader::GetInstance()->GetMonsterInfo(monsterID, info);
	if(info.id == -1)
	{
		LOG_ERROR(FILEINFO, "scene null");

		return -1;
	}

	bool notExtend = true;
	DWORD needFreshCount = 0;
	if(counts == -1)
	{
		needFreshCount = GameParaLoader::GetInstance()->getDefaultFreshMonster();
		notExtend = false;
	}
	else
	{
		needFreshCount = counts;
	}

	int twCount = posList.size();
	vector<Pos> newPos;

	FieldInfo * fInfo = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(fInfo == NULL)
	{
		LOG_ERROR(FILEINFO, "FieldInfo is NULL");
		return -1;
	}

//	const int FRESHMONSTERAREA = GameParaLoader::GetInstance()->getFreshMonsterArea();
	const int FRESHMONSTERAREA = 3;
	for(int j=0; j<twCount; ++j)
	{
		newPos.clear();
		Pos pos;
		pos.xpos = posList[j].xpos;
		pos.ypos = posList[j].ypos;

		DWORD count = 0;

		int tIndex = pos.ypos * fInfo->GetXMaxNum() + pos.xpos;

		if(IsValidPos(pos.xpos,pos.ypos) && tIndex < fInfo->GetTotalSize() && !m_totalSector[tIndex].IsMonster())
		{
			newPos.push_back(pos);
			++count;
		}

		int tempCycleCounts = 1;
		int tempLenght      = 0;

		int initXpos = 0;
		int initYpos = 0;
		int endXpos  = 0;
		int endYpos  = 0;

		while(count < needFreshCount)
		{
			//每次增大一个FRESHMONSTERAREA单位长度
			tempLenght = FRESHMONSTERAREA * tempCycleCounts;

			initXpos = pos.xpos - tempLenght;
			initYpos = pos.ypos - tempLenght;
			endXpos  = pos.xpos + tempLenght;
			endYpos  = pos.ypos + tempLenght;

			if(endXpos >= fInfo->GetXMaxNum() && endYpos >= fInfo->GetYMaxNum() && initXpos <= 0 && initYpos <= 0)
				break;

			tempLenght = FRESHMONSTERAREA * (tempCycleCounts - 1);
			for(int i = initYpos; i <= endYpos;i += 3)
			{
				for(int k = initXpos; k <= endXpos; k += 3)
				{
					if(k == pos.xpos && i == pos.ypos)
					{
						continue;
					}

					//进入上一次的矩形则跳过
					if( k >= pos.xpos - tempLenght && k <= pos.xpos + tempLenght &&
							i >= pos.ypos - tempLenght && i <= pos.ypos + tempLenght )
					{
						k = pos.xpos + tempLenght;

						continue ;
					}

					if(!IsValidPos(k,i))
						continue;

					tIndex = i * fInfo->GetXMaxNum() + k;
					if(tIndex >= fInfo->GetTotalSize() || tIndex < 0)
					{
						continue;
					}

					if(m_totalSector[tIndex].IsMonster())
					{
						continue;
					}

					Pos tempPos;
					tempPos.xpos = k;
					tempPos.ypos = i;
					newPos.push_back(tempPos);
					++count;

					if(count  >= needFreshCount)
					{
						break;
					}
				}

				if(count  >= needFreshCount)
				{
					break;
				}

			}

			++tempCycleCounts;

			if(!notExtend)
				break;
		}

		//要求生成出多少个坐标点，即产生多少个怪
		for(size_t i=0; i<newPos.size(); ++i)
		{
			Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eMonster);
			Smart_Ptr<Monster> mon = obj;

			mon->SetID(info.id);
			mon->SetName(info.name);
			mon->getAttManage()->setValue(eCharLv,info.lv);
			mon->SetQuality(info.quality);
			mon->SetFlushTime(info.flushTime);
			mon->SetAliveTime(info.aliveTime);
			mon->getAttManage()->setValue(eCharCountry,info.camp);
			mon->getAttManage()->setValue(eCharMoveSpeed,info.idleSpeed);
			mon->SetRunSpeed(info.moveSpeed);
//			mon->SetBaseAi(info.baseAI);
//			mon->SetBetterAi(info.betterAI);
			mon->SetFallID(info.fallID);
			mon->getAttManage()->setValue(eCharExp,info.exp);
			mon->getAttManage()->setValue(eCharPhyAttackMax,info.maxAttack);
			mon->getAttManage()->setValue(eCharPhyDefence,info.phyDefence);
			mon->getAttManage()->setValue(eCharHitRate,info.hitRate);
			mon->getAttManage()->setValue(eCharAvoidRate,info.avoidRate);
			mon->getAttManage()->setValue(eCrackDownRate,info.crackRate);
			mon->getAttManage()->setValue(eAvoidCrackRate,info.avoidCrack);
			mon->SetHPMax(info.maxHP);
			mon->SetCurHP(info.maxHP);
			mon->SetMPMax(info.maxMP);
			mon->SetCurMP(info.maxMP);
			mon->SetGatherTime(info.gatherTime);	//采集时间
			mon->SetOwnerToBrain(mon);
			mon->SetMapID(m_sceneID);
			mon->SetBornTime();
			mon->SetOwnerName("");

			mon->SetXPos(newPos[i].xpos);
			mon->SetYPos(newPos[i].ypos);
			mon->SetOriginalX(newPos[i].xpos);
			mon->SetOriginalY(newPos[i].ypos);

			if(Join(obj) != 0)
			{
				continue ;
			}

			vector<Sector *> moveVec;
			BaseAIInfo *aiInfo = 0;
			BetterAIInfo *betterAI = 0;

			aiInfo = AILoader::GetInstance()->GetBaseAIByid(info.baseAI);
			if(!aiInfo)
			{
				LOG_ERROR(FILEINFO, "monster[id=%d] ai is error", info.id);

				continue;
			}

			if(info.betterAI > 0)
			{
				betterAI = AILoader::GetInstance()->GetBetterAIByid(info.betterAI);
				if (!betterAI)
				{
					LOG_ERROR(FILEINFO, "monster[id=%d] better ai is error", info.id);

					continue;
				}
			}

			mon->SetStayTime(aiInfo->stayTime);
			mon->SetPhyAttackTime(aiInfo->atkTime);
			GetSectorInPos(moveVec, newPos[i].xpos, newPos[i].ypos, aiInfo->moveDistance);
			mon->SetMoveSector(moveVec);

			Smart_Ptr<AIBrain> monBrain = mon->GetBrain();
			if(!monBrain)
				continue;

			monBrain->Init(aiInfo, betterAI);
			if(info.quality == eWorldMonster)
			{
				CSceneMgr::GetInstance()->AddWorldBossToZone(mon->GetKey(), mon->GetMapID(), mon->GetID());
			}

			m_allObject[mon->GetKey()] = mon;
			m_allMonster[info.id].push_back(mon);

			if(pKeyList != NULL)
			{
				pKeyList->push_back(mon->GetKey());
			}
		}
	}

	return 0;
}

int CGameZone::CreateTypeMonster(MapMonster * aMonster)
{
	MonsterInfo info;

	MonsterLoader::GetInstance()->GetMonsterInfo(aMonster->id, info);
	if(info.id == -1)
	{
		LOG_ERROR(FILEINFO, "scene null");

		return -1;
	}

	vector<Smart_Ptr<Monster> > vecMonster;

	for(int j=0; j<aMonster->count; ++j)
	{
		if((size_t)(2 * j + 1) > aMonster->randPos.size())
		{
			LOG_ERROR(FILEINFO, "create monster but point count error");
			break;
		}

		if(!IsValidPos(aMonster->randPos[2*j].xpos,aMonster->randPos[2*j].ypos) || !IsValidPos(aMonster->randPos[2*j+1].xpos,aMonster->randPos[2*j+1].ypos))
		{
			LOG_ERROR(FILEINFO, "create monster but point error");
			break;
		}

		{
			Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eMonster);
			Smart_Ptr<Monster> mon = obj;

			mon->SetID(info.id);
			mon->SetName(info.name);
			mon->getAttManage()->setValue(eCharLv,info.lv);
			mon->SetQuality(info.quality);
			mon->SetFlushTime(info.flushTime);
			mon->SetAliveTime(info.aliveTime);
			mon->getAttManage()->setValue(eCharCountry,info.camp);
			mon->getAttManage()->setValue(eCharMoveSpeed,info.idleSpeed);
			mon->SetRunSpeed(info.moveSpeed);
//			mon->SetBaseAi(info.baseAI);
//			mon->SetBetterAi(info.betterAI);
			mon->SetFallID(info.fallID);
			mon->getAttManage()->setValue(eCharExp,info.exp);
			mon->getAttManage()->setValue(eCharPhyAttackMax,info.maxAttack);
			mon->getAttManage()->setValue(eCharPhyDefence,info.phyDefence);
			mon->getAttManage()->setValue(eCharHitRate,info.hitRate);
			mon->getAttManage()->setValue(eCharAvoidRate,info.avoidRate);
			mon->getAttManage()->setValue(eCrackDownRate,info.crackRate);
			mon->getAttManage()->setValue(eAvoidCrackRate,info.avoidCrack);
			mon->SetHPMax(info.maxHP);
			mon->SetCurHP(info.maxHP);
			mon->SetMPMax(info.maxMP);
			mon->SetCurMP(info.maxMP);
			mon->SetGatherTime(info.gatherTime);	//采集时间
			mon->SetOwnerToBrain(mon);
			mon->SetMapID(m_sceneID);
			mon->SetBornTime();
			mon->SetOwnerName("");
			mon->GetBrain()->SetMonsterType(ePointWayPos);
			mon->SetXPos(aMonster->randPos[2*j].xpos);
			mon->SetYPos(aMonster->randPos[2*j].ypos);
			mon->SetOriginalX(aMonster->randPos[2*j].xpos);
			mon->SetOriginalY(aMonster->randPos[2*j].ypos);

			Join(obj);

			vector<Sector *> moveVec;
			Sector * tSecf = 0;
			Sector * tSecs = 0;
			BaseAIInfo *aiInfo = 0;
			BetterAIInfo *betterAI = 0;

			aiInfo = AILoader::GetInstance()->GetBaseAIByid(info.baseAI);
			if(!aiInfo)
			{
				LOG_ERROR(FILEINFO, "monster[id=%d] ai is error", info.id);

				continue;
			}

			if(info.betterAI > 0)
			{
				betterAI = AILoader::GetInstance()->GetBetterAIByid(info.betterAI);
				if (!betterAI)
				{
					LOG_ERROR(FILEINFO, "monster[id=%d] better ai is error", info.id);

					continue;
				}
			}

			mon->SetStayTime(aiInfo->stayTime);
			mon->SetPhyAttackTime(aiInfo->atkTime);
			tSecf = GetSector(aMonster->randPos[2*j].xpos, aMonster->randPos[2*j].ypos);
			tSecs = GetSector(aMonster->randPos[2*j+1].xpos, aMonster->randPos[2*j+1].ypos);
			if(tSecf || tSecs)
			{
				moveVec.push_back(tSecf);
				moveVec.push_back(tSecs);
			}

			mon->SetMoveSector(moveVec);

			Smart_Ptr<AIBrain> monBrain = mon->GetBrain();
			if(!monBrain)
				continue;

			monBrain->Init(aiInfo, betterAI);
			vecMonster.push_back(mon);
			if(info.quality == eWorldMonster)
			{
				CSceneMgr::GetInstance()->AddWorldBossToZone(mon->GetKey(), mon->GetMapID(), mon->GetID());
			}

			m_allObject[mon->GetKey()] = mon;
		}

		m_allMonster[info.id] = vecMonster;
	}

	return 0;
}

Smart_Ptr<Monster> CGameZone::CreateMonster(int monster, int posx, int posy)
{
	MonsterInfo info;
	MonsterLoader::GetInstance()->GetMonsterInfo(monster, info);
	if(info.id <= 0)
	{
		return NULL;
	}

	Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eMonster);
	Smart_Ptr<Monster> mon = obj;

	mon->SetID(info.id);
	mon->SetName(info.name);
	mon->getAttManage()->setValue(eCharLv,info.lv);
	mon->SetQuality(info.quality);
	mon->SetFlushTime(info.flushTime);
	mon->SetAliveTime(info.aliveTime);
	mon->getAttManage()->setValue(eCharCountry,info.camp);
	mon->getAttManage()->setValue(eCharMoveSpeed,info.idleSpeed);
	mon->SetRunSpeed(info.moveSpeed);
	mon->SetFallID(info.fallID);
	mon->getAttManage()->setValue(eCharExp,info.exp);
	mon->getAttManage()->setValue(eCharPhyAttackMax,info.maxAttack);
	mon->getAttManage()->setValue(eCharPhyDefence,info.phyDefence);
	mon->getAttManage()->setValue(eCharHitRate,info.hitRate);
	mon->getAttManage()->setValue(eCharAvoidRate,info.avoidRate);
	mon->getAttManage()->setValue(eCrackDownRate,info.crackRate);
	mon->getAttManage()->setValue(eAvoidCrackRate,info.avoidCrack);
	mon->SetHPMax(info.maxHP);
	mon->SetCurHP(info.maxHP);
	mon->SetMPMax(info.maxMP);
	mon->SetCurMP(info.maxMP);
	mon->SetGatherTime(info.gatherTime);	//采集时间
	mon->SetOwnerToBrain(mon);
	mon->SetMapID(m_sceneID);
	mon->SetBornTime();
	mon->SetOwnerName("");

	mon->SetXPos(posx);
	mon->SetYPos(posy);
	mon->SetOriginalX(posx);
	mon->SetOriginalY(posy);

	if(Join(obj) != 0)
	{
		return NULL;
	}

	vector<Sector *> moveVec;
	BetterAIInfo *betterAI = 0;

	BaseAIInfo *aiInfo = AILoader::GetInstance()->GetBaseAIByid(info.baseAI);
	if(!aiInfo)
	{
		LOG_ERROR(FILEINFO, "monster[id=%d] ai is error", info.id);
		return NULL;
	}

	if(info.betterAI > 0)
	{
		betterAI = AILoader::GetInstance()->GetBetterAIByid(info.betterAI);
		if(betterAI == NULL)
		{
			LOG_ERROR(FILEINFO, "monster[id=%d] better ai is error", info.id);
			return NULL;
		}
	}

	mon->SetStayTime(aiInfo->stayTime);
	mon->SetPhyAttackTime(aiInfo->atkTime);
	GetSectorInPos(moveVec, posx, posy, aiInfo->moveDistance);
	mon->SetMoveSector(moveVec);

	Smart_Ptr<AIBrain> monBrain = mon->GetBrain();
	if(!monBrain)
	{
		return NULL;
	}

	monBrain->Init(aiInfo, betterAI);
	if(info.quality == eWorldMonster)
	{
		CSceneMgr::GetInstance()->AddWorldBossToZone(mon->GetKey(), mon->GetMapID(), mon->GetID());
	}

	m_allObject[mon->GetKey()] = mon;
	m_allMonster[info.id].push_back(mon);

	return mon;
}


int CGameZone::InitMonster()
{
	FieldMonster * monster = CMapConfig::GetInstance()->GetFieldMonsterByid(GET_MAP_ID(m_sceneID));
	if(!monster)
	{
		return -1;
	}

	map<int, MapMonster> mapmonster = monster->GetMapMonster();
	map<int, MapMonster>::iterator it = mapmonster.begin();

	for(; it!=mapmonster.end(); ++it)
	{
		if(it->second.type == eRandomWayPos)
		{
			if(CreateMonster(it->second.id,it->second.count,it->second.randPos))
				continue;
		}
		else
		{
			if(CreateTypeMonster(&it->second))
				continue;
		}
	}

	return 0;
}

int CGameZone::InitNpc()
{
	vector<NpcInfo> info;

	NPCInfo::GetInstance()->GetAllMapNpc(GET_MAP_ID(m_sceneID), info);
	vector<NpcInfo>::iterator it = info.begin();
	for(; it!=info.end(); ++it)
	{
		Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eNpc);
		Smart_Ptr<NPC> npc = obj;

		npc->SetXPos(it->xpos);
		npc->SetYPos(it->ypos);
		npc->SetNpcID(it->npcID);

		m_npc[it->npcID] = npc;
	}

	return 0;
}

int CGameZone::InitRegion()
{
	FieldInfo * tInfo = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(tInfo == NULL)
	{
		return -1;
	}

	const map<int,RegionData>& tRegionList = tInfo->getRegionList();
	map<int,RegionData>::const_iterator tIt = tRegionList.begin();
	vector<EffectVector> tPosList;
	for(;tIt != tRegionList.end();++tIt)
	{
		tPosList.clear();
		switch(tIt->second.m_Type)
		{
		case eRegionRound:
			{
				if(tIt->second.m_PosList.size() == 0)
				{
					break;
				}
				MathHelper::getRoundAllPos(*tIt->second.m_PosList.begin(),tIt->second.m_Radius,tPosList);
				break;
			}
		case eRegionPolygon:
			{
				MathHelper::getPolygonAllPos(tIt->second.m_PosList,tPosList);
				break;
			}
		default:
			{
				break;
			}
		}

		for(size_t i = 0;i < tPosList.size();++i)
		{
			if(tPosList[i].x >= tInfo->GetXMaxNum() || tPosList[i].y >= tInfo->GetYMaxNum())
			{
				continue ;
			}

			if(tPosList[i].x <= 0 || tPosList[i].y <= 0)
			{
				continue ;
			}

			//格子索引
			int tempTotalIndex = (int)(tPosList[i].y * tInfo->GetXMaxNum() + tPosList[i].x);
			m_totalSector[tempTotalIndex].addRegion(tIt->first,&tIt->second);
		}
	}

	return 0;
}

void CGameZone::reflushMonster()
{
	const vector<ReflushData>* tDataList = ReflushFile::GetInstance()->getFileData(GET_MAP_ID(m_sceneID));

	if(tDataList == NULL)
	{
		return ;
	}

	m_ReflushManage = new ReflushManage(this,*tDataList);

	return ;
}

int CGameZone::Init()
{
	//��ʼ�����
	if(InitField())
	{
		LOG_ERROR(FILEINFO, "init field sector error");

		return -1;
	}

	//��ʼ��npc
	if(InitNpc())
	{
		LOG_ERROR(FILEINFO, "init scene npc error");

		return -1;
	}

	//��ʼ������
	if(InitMonster())
	{
		LOG_ERROR(FILEINFO, "init scene Monster error");

		return -1;
	}

	if(InitSpring())
	{
		LOG_ERROR(FILEINFO, "init scene spring error");

		return -1;
	}

	if(InitRegion())
	{
		LOG_ERROR(FILEINFO, "init Region error");

		return -1;
	}

	reflushMonster();

	return 0;
}

int CGameZone::GetSectorIndex(int xpos, int ypos)
{
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(!info)
		return -1;

	return (xpos + ypos * info->GetXMaxNum());
}

int CGameZone::GetSectorGroupIndex(int xpos, int ypos)
{
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(!info)
		return -1;

	int group = (ypos / info->GetNineYSector()) * info->GetNineXSize() + xpos / info->GetNineXSector(); //(GetSectorIndex(xpos, ypos) / info->GetXMaxNum()) * info->GetNineXSize() + xpos/info->GetNineXSector();//

	return group;
}

bool CGameZone::IsInSameNineGroup(int srcPosX, int srcPosY, int destPosX, int destPosY)
{
	int src = GetSectorGroupIndex(srcPosX, srcPosY);
	int dest = GetSectorGroupIndex(destPosX, destPosY);

	if(src < 0 || src >= m_totalGroupSize || dest < 0 || dest >= m_totalGroupSize )
	{
		return false;
	}

	SectorGroup* pSrcGroup = &m_totalGroup[src];
	SectorGroup* pDestGroup = &m_totalGroup[dest];

	return pSrcGroup->IsNearbyGroup(pDestGroup);
}

bool CGameZone::IsCanMeditation(int xpos, int ypos)
{
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(info==NULL)
		return false;

	int flag = info->GetPosFlag(xpos, ypos);

	//只有0 2点才能打坐
	if(flag == 0 || flag == 2 )
	{
		return true;
	}

	return false;
}

Sector * CGameZone::GetSector(int xpos, int ypos)
{
	int sectorIndex = GetSectorIndex(xpos, ypos);
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(!info || sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
	{
		LOG_ERROR(FILEINFO, "sectorindex in position(mapid=%d, xpos=%d, ypos=%d) is invalid", GET_MAP_ID(m_sceneID), xpos, ypos);

		return 0;
	}

	return &m_totalSector[sectorIndex];
}

bool CGameZone::GetSectorInPos(vector<Sector *> &mapSector, int xpos, int ypos, int distance)
{
	if(distance <= 0)
		return false;

	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));

	int fieldXLen = info->GetXMaxNum();
	int fieldYLen = info->GetYMaxNum();
	int totalSize = info->GetTotalSize();

	int minXPos = xpos - distance;
	if(minXPos < 0)
	{
		minXPos = 0;
	}

	int maxXPos = xpos + distance;
	if(maxXPos >= fieldXLen)
	{
		maxXPos = fieldXLen - 1;
	}

	int minYPos = ypos - distance;
	if(minYPos < 0)
	{
		minYPos = 0;
	}

	int maxYPos = ypos + distance;
	if(maxYPos >= fieldYLen)
	{
		maxYPos = fieldYLen - 1;
	}

	for(int i=minYPos; i<=maxYPos; ++i)
	{
		for(int j=minXPos; j<=maxXPos; ++j)
		{
			int sectorIndex = (j + i * fieldXLen);
			if(sectorIndex < 0 || sectorIndex >= totalSize)
			{
				continue;
			}

			mapSector.push_back(&m_totalSector[sectorIndex]);
		}
	}

	return true;
}

void CGameZone::UpdateSpring(int dwTick)
{
	m_springTick += dwTick;
	if(m_springTick < 1000)
	{
		return;
	}

	m_springTick -= 1000;

	vector<Smart_Ptr<Spring> >::iterator it = m_allSpring.begin();
	for(; it!=m_allSpring.end(); ++it)
	{
		vector<Smart_Ptr<CreatureObj> > obj;
		vector<Smart_Ptr<CreatureObj> >::iterator itvec;

		GetPlayer((*it)->GetXPos(), (*it)->GetYPos(), (*it)->GetRange(), -1, obj);
		itvec = obj.begin();
		for(; itvec!=obj.end(); ++itvec)
		{
			if((*itvec)->getAttManage()->getValue<int>(eCharHP) == (*itvec)->getAttManage()->getValue<int>(eCharHPMax))
				continue;

			(*itvec)->ChangeHPRate((*it)->GetHPRate());
		}
	}
}

void CGameZone::Update(int dwTick)
{
	if(m_allSpring.size() > 0)
		UpdateSpring(dwTick);

	INTGRILWARMAP::iterator itr = m_girlWar.begin();
	for(; itr  != m_girlWar.end(); ++itr)
	{
		itr->second->Update(dwTick);
	}

	if(m_player.size() == 0)
	{
		//假如该场景没有人了,那么只刷新那些需要即时刷新的怪
		INTVMPNSTERMAP::iterator it = m_allMonster.begin();
		for(; it!=m_allMonster.end(); ++it)
		{
			vector<Smart_Ptr<Monster> >::iterator itVec = it->second.begin();
			for(; itVec!=it->second.end(); ++itVec)
			{
				if((*itVec)->GetQuality() == eWorldMonster)
				{
					(*itVec)->Update(dwTick);
				}
			}
		}

		return;
	}
	else
	{
		INTVMPNSTERMAP::iterator it = m_allMonster.begin();
		for(; it!=m_allMonster.end(); ++it)
		{
			vector<Smart_Ptr<Monster> >::iterator itVec = it->second.begin();
			for(; itVec!=it->second.end(); ++itVec)
			{
				(*itVec)->Update(dwTick);
			}
		}
	}
}

int CGameZone::Join(Smart_Ptr<Object> & obj)
{
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(!info)
		return -1;

	int sectorIndex = GetSectorIndex(obj->GetXPos(), obj->GetYPos());
	if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
	{
		LOG_ERROR(FILEINFO, "sectorindex is invalid(mapid=%d, xpos=%d, ypos=%d)", (int)GET_MAP_ID(m_sceneID), obj->GetXPos(), obj->GetYPos());

		return -1;
	}

	//���㵱ǰ���ڵľŹ���
	int groupIndex = GetSectorGroupIndex(obj->GetXPos(), obj->GetYPos());
	if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
	{
		LOG_ERROR(FILEINFO, "groupindex is invalid");

		return -1;
	}

	//����ɫ�������
	Sector * pSector = &m_totalSector[sectorIndex];

	if(pSector->Join(obj))
	{
		LOG_ERROR(FILEINFO, " enter sector error");

		return -1;
	}

	switch(obj->GetType())
	{
	case ePlayer:
		{
			Smart_Ptr<Player> player = obj;

			//����ɫ���볡��
			m_player[player->GetID()] = player;
			m_allObject[player->GetKey()] = obj;

			//进入地图前切换玩家的PK模式
			//世界地图配置表里面的是表示pk模式的切换限制
			//不是玩家所能用的模式
			MapStaticInfo * mapInfo = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(m_sceneID));
			if(mapInfo)
			{
				player->SetPkTypeByMap(mapInfo->pktype);
			}

			SectorGroup * pGroup = &m_totalGroup[groupIndex];

			pGroup->SendAllObjectToGroup(obj, 0);

			if(player && !(player->GetGameZone()))
				player->SetGameZone(this);

			pSector->joinOrLeaveRegion(player,NULL,true);

			break;
		}
	case eMonster:
		{
			Smart_Ptr<Monster> monster = obj;

			if(monster->GetQuality() == eWorldMonster)
			{
				PlayerMessage::GetInstance()->UpdateWorldBoss(monster->GetID(), true);
			}

			//���������֪ͨ�Ź�������
			SectorGroup * pGroup = &m_totalGroup[groupIndex];

			pGroup->SendAllObjectToGroup(obj, 0);

			if(monster && !(monster->GetGameZone()))
				monster->SetGameZone(this);

			break;
		}
	case eDropItem:
	{
		m_allObject[obj->GetKey()] = obj;

		SectorGroup * pGroup = &m_totalGroup[groupIndex];

		pGroup->SendAllObjectToGroup(obj, 0);

		break;
	}
	case ePet:
		{
			Smart_Ptr<GirlWar> girlWar = obj;

			m_girlWar[girlWar->GetKey()] = girlWar;
			m_allObject[girlWar->GetKey()] = obj;

			SectorGroup * pGroup = &m_totalGroup[groupIndex];

			pGroup->SendAllObjectToGroup(obj, 0);

			if(girlWar && !(girlWar->GetGameZone()))
				girlWar->SetGameZone(this);
			break;
		}
	default:
		{
			LOG_ERROR(FILEINFO, "unknown object type[%d] join zone", obj->GetType());

			return -1;
		}
	}

	return 0;
}

int CGameZone::Move(Smart_Ptr<Object> &obj)
{
	switch(obj->GetType())
	{
	case ePlayer:
		{
			Smart_Ptr<Player> player = obj;
			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
			int oldSectroIndex = GetSectorIndex(player->GetXPos(), player->GetYPos());
			int sectorIndex = GetSectorIndex(player->GetXNewPos(), player->GetYNewPos());

			if(info==NULL || oldSectroIndex < 0 || oldSectroIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "oldSectroIndex in  is invalid: player=%lld, mapid=%d, xpos=%d, ypos=%d when move",
						player->GetID(), (int)GET_MAP_ID(m_sceneID), player->GetXPos(), player->GetYPos());
				return -1;
			}


			if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex in player[%lld] is invalid(mapid=%d, xpos=%d, ypos=d) when player move",
						player->GetID(), (int)GET_MAP_ID(m_sceneID), player->GetXNewPos(), player->GetYNewPos());
				return -1;
			}

			if(oldSectroIndex != sectorIndex)
			{
				Sector * pSector = &m_totalSector[oldSectroIndex];
				if(pSector->Leave(obj))
				{
					LOG_ERROR(FILEINFO, "player[gs charid=%lld] leave sector error", GET_PLAYER_CHARID(player->GetID()));

					return -1;
				}

				pSector = &m_totalSector[sectorIndex];
				if(pSector->Join(obj))
				{
					LOG_ERROR(FILEINFO, "player[gs charid=%lld] enter sector error", GET_PLAYER_CHARID(player->GetID()));

					return -1;
				}

				m_totalSector[oldSectroIndex].joinOrLeaveRegion(player,&m_totalSector[sectorIndex],false);
				m_totalSector[sectorIndex].joinOrLeaveRegion(player,&m_totalSector[oldSectroIndex],true);
			}

			int groupIndex = GetSectorGroupIndex(player->GetXNewPos(), player->GetYNewPos());
			if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
			{
				LOG_ERROR(FILEINFO, "groupindex in player is invalid when player move");

				return -1;
			}

			int groupOldIndex = GetSectorGroupIndex(player->GetXPos(), player->GetYPos());
			if(groupOldIndex < 0 || groupOldIndex >= m_totalGroupSize)
			{
				LOG_ERROR(FILEINFO, "groupindex in player is invalid when player move");

				return -1;
			}

			if(groupOldIndex != groupIndex)
			{
				SectorGroup * pGroup = &m_totalGroup[groupIndex];
				SectorGroup * pOldGroup = &m_totalGroup[groupOldIndex];

				pOldGroup->SendObjLeaveToGroup(obj, pGroup);
				pGroup->SendAllObjectToGroup(obj, pOldGroup);
			}

			break;
		}
	case eMonster:
		{
			Smart_Ptr<Monster> mon = obj;
			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));

			int oldSectroIndex = GetSectorIndex(mon->GetXPos(), mon->GetYPos());
			int sectorIndex = GetSectorIndex(mon->GetXNewPos(), mon->GetYNewPos());

			if(info==NULL || oldSectroIndex < 0 || oldSectroIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "oldSectroIndex is invalid: monster=%d, mapid=%d, x=%d, y%d when move",
					(int)GET_MAP_ID(m_sceneID),	mon->GetID(), (int)GET_MAP_ID(m_sceneID), mon->GetXPos(), mon->GetYPos());

				return -1;
			}

			if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex in monster(mapid=%d,x=%d,y%d) is invalid when player move",
						(int)GET_MAP_ID(m_sceneID),
						mon->GetXNewPos(),
						mon->GetYNewPos());

				return -1;
			}

			if(oldSectroIndex != sectorIndex)
			{
				Sector * pSector = &m_totalSector[oldSectroIndex];
				if(pSector->Leave(obj))
				{
					LOG_ERROR(FILEINFO, "monster[id=%lld] leave sector error", mon->GetID());

					return -1;
				}

				pSector = &m_totalSector[sectorIndex];
				if(pSector->Join(obj))
				{
					LOG_ERROR(FILEINFO, "monster[id=%lld] enter sector error", mon->GetID());

					return -1;
				}
			}

			int groupIndex = GetSectorGroupIndex(mon->GetXNewPos(), mon->GetYNewPos());
			if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
			{
				LOG_ERROR(FILEINFO, "groupindex in monster is invalid when monster move");

				return -1;
			}

			int groupOldIndex = GetSectorGroupIndex(mon->GetXPos(), mon->GetYPos());
			if(groupOldIndex < 0 || groupOldIndex >= m_totalGroupSize)
			{
				LOG_ERROR(FILEINFO, "groupOldIndex in monster is invalid when monster move");

				return -1;
			}

			if(groupOldIndex != groupIndex)
			{
				SectorGroup * pGroup = &m_totalGroup[groupIndex];
				SectorGroup * pOldGroup = &m_totalGroup[groupOldIndex];

				pOldGroup->SendObjLeaveToGroup(obj, pGroup);
				pGroup->SendAllObjectToGroup(obj, pOldGroup);
			}

			break;
		}
	case ePet:
		{
			Smart_Ptr<GirlWar> girlWar = obj;

			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));

			int oldSectroIndex = GetSectorIndex(girlWar->GetXPos(), girlWar->GetYPos());
			int sectorIndex = GetSectorIndex(girlWar->GetXNewPos(), girlWar->GetYNewPos());

			if(info==NULL || oldSectroIndex < 0 || oldSectroIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "oldSectroIndex in pet(mapid=%d, x=%d,y%d) is invalid when player move",
					(int)GET_MAP_ID(m_sceneID),	girlWar->GetXPos(), girlWar->GetYPos());

				return -1;
			}

			if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex in pet(mapid=%d, x=%d,y%d) is invalid when player move",
						(int)GET_MAP_ID(m_sceneID),
						girlWar->GetXNewPos(),
						girlWar->GetYNewPos());

				return -1;
			}

			if(oldSectroIndex != sectorIndex)
			{
				Sector * pSector = &m_totalSector[oldSectroIndex];
				if(pSector->Leave(obj))
				{
					LOG_ERROR(FILEINFO, "GirlWar[id=%d] leave sector error", girlWar->GetID());

					return -1;
				}

				pSector = &m_totalSector[sectorIndex];
				if(pSector->Join(obj))
				{
					LOG_ERROR(FILEINFO, "girlWar[id=%lld] enter sector error", girlWar->GetKey());

					return -1;
				}
			}

			int groupIndex = GetSectorGroupIndex(girlWar->GetXNewPos(), girlWar->GetYNewPos());
			if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
			{
				LOG_ERROR(FILEINFO, "groupindex in girlWar is invalid when girlWar move");

				return -1;
			}

			int groupOldIndex = GetSectorGroupIndex(girlWar->GetXPos(), girlWar->GetYPos());
			if(groupOldIndex < 0 || groupOldIndex >= m_totalGroupSize)
			{
				LOG_ERROR(FILEINFO, "groupOldIndex in girlWar is invalid when girlWar move");

				return -1;
			}

			if(groupOldIndex != groupIndex)
			{
				SectorGroup * pGroup = &m_totalGroup[groupIndex];
				SectorGroup * pOldGroup = &m_totalGroup[groupOldIndex];

				pOldGroup->SendObjLeaveToGroup(obj, pGroup);
				pGroup->SendAllObjectToGroup(obj, pOldGroup);
			}

			break;
		}
	default:
		{
			LOG_ERROR(FILEINFO, "unknown object type[%d] move", obj->GetType());

			return -1;
		}
	}

	return 0;
}

bool CGameZone::isCanDoing(eObjectDoing doing,int x,int y)
{
	FieldInfo* tempInfo = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(tempInfo == NULL)
		return true;

	if(x <= 0 || y <= 0)
	{
		return true;
	}

	if(x >= tempInfo->GetXMaxNum() || y >= tempInfo->GetYMaxNum())
	{
		return true;
	}

	int index = x + y * tempInfo->GetXMaxNum();

	return m_totalSector[index].isCanDoing(doing);
}

bool CGameZone::isCanAddState(EAttackState state,int x,int y)
{
	FieldInfo* tempInfo = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(tempInfo == NULL)
		return true;

	if(x <= 0 || y <= 0)
	{
		return true;
	}

	if(x >= tempInfo->GetXMaxNum() || y >= tempInfo->GetYMaxNum())
	{
		return true;
	}

	int index = x + y * tempInfo->GetXMaxNum();

	return m_totalSector[index].isCanAddState(state);
}

void CGameZone::afterLeave(Smart_Ptr<Player> player)
{
	map<int64,OldMapInfo>::iterator it = m_oldMapPoint.find(player->GetID());
	if(it != m_oldMapPoint.end())
	{
		player->SetMapID(it->second.mapid);
		player->SetXPos(it->second.xpos);
		player->SetYPos(it->second.ypos);
		player->SetXNewPos();
		player->SetYNewPos();
		player->SetDataFlag(eBaseInfo);
		m_oldMapPoint.erase(it);
	}

	return ;
}

//保存进入玩家的上次场景和坐标
void CGameZone::SetOldMapPoint(int64 charid, int64 mapid, int xpos, int ypos)
{
	OldMapInfo info;

	info.mapid = mapid;
	info.xpos = xpos;
	info.ypos = ypos;

	m_oldMapPoint[charid] = info;
}

const OldMapInfo* CGameZone::GetOldMapPoint(int64 charid)
{
	map<int64,OldMapInfo>::iterator it = m_oldMapPoint.find(charid);
	if(it != m_oldMapPoint.end())
		return &it->second;

	return NULL;
}

void CGameZone::removeOldPoint(int64 charid)
{
	map<int64,OldMapInfo>::iterator it = m_oldMapPoint.find(charid);
	if(it == m_oldMapPoint.end())	
	{
		return ;
	}

	m_oldMapPoint.erase(it);

	return ;
}

int CGameZone::MoveToAll(Smart_Ptr<Object> &obj)
{
	switch(obj->GetType())
	{
	case ePlayer:
		{
			Smart_Ptr<Player> player = obj;
			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));

			int oldSectroIndex = GetSectorIndex(player->GetXPos(), player->GetYPos());
			int sectorIndex = GetSectorIndex(player->GetXNewPos(), player->GetYNewPos());

			if(info==NULL || oldSectroIndex < 0 || oldSectroIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "oldSectroIndex in player[%lld] is invalid(mapid=%d, xpos=%d, ypos=%d) when player move",
					player->GetID(), (int)GET_MAP_ID(m_sceneID),	player->GetXPos(), player->GetYPos());

				return -1;
			}


			if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex in player[%lld] is invalid(mapid=%d, xpos=%d, ypos=%d) when player move",
						player->GetID(), (int)GET_MAP_ID(m_sceneID), player->GetXNewPos(), player->GetYNewPos());

				return -1;
			}

			if(oldSectroIndex != sectorIndex)
			{
				Sector * pSector = &m_totalSector[oldSectroIndex];
				if(pSector->Leave(obj))
				{
					LOG_ERROR(FILEINFO, "player[gs charid=%lld] leave sector error", GET_PLAYER_CHARID(player->GetID()));
					return -1;
				}

				pSector = &m_totalSector[sectorIndex];
				if(pSector->Join(obj))
				{
					LOG_ERROR(FILEINFO, "player[gs charid=%lld] enter sector error", GET_PLAYER_CHARID(player->GetID()));
					return -1;
				}

				m_totalSector[oldSectroIndex].joinOrLeaveRegion(player,&m_totalSector[sectorIndex],false);
				m_totalSector[sectorIndex].joinOrLeaveRegion(player,&m_totalSector[oldSectroIndex],true);
			}

			int groupIndex = GetSectorGroupIndex(player->GetXNewPos(), player->GetYNewPos());
			if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
			{
				LOG_ERROR(FILEINFO, "groupindex in player is invalid when player move");
				return -1;
			}

			int groupOldIndex = GetSectorGroupIndex(player->GetXPos(), player->GetYPos());
			if(groupOldIndex < 0 || groupOldIndex >= m_totalGroupSize)
			{
				LOG_ERROR(FILEINFO, "groupindex in player is invalid when player move");
				return -1;
			}

			SectorGroup * pGroup = &m_totalGroup[groupIndex];
			SectorGroup * pOldGroup = &m_totalGroup[groupOldIndex];

			pOldGroup->SendObjLeaveToGroup(obj);
			pGroup->SendAllObjectToGroup(obj, NULL);

			break;
		}
	case ePet:
		{
			Smart_Ptr<GirlWar> girlWar = obj;

			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));

			int oldSectroIndex = GetSectorIndex(girlWar->GetXPos(), girlWar->GetYPos());
			int sectorIndex = GetSectorIndex(girlWar->GetXNewPos(), girlWar->GetYNewPos());

			if(info==NULL || oldSectroIndex < 0 || oldSectroIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "oldSectroIndex in pet(mapid=%d,x=%d,y%d) is invalid when  move",
					(int)GET_MAP_ID(m_sceneID),	girlWar->GetXPos(), girlWar->GetYPos());

				return -1;
			}

			if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex in pet(mapid=%d,x=%d,y%d) is invalid when  move",
					(int)GET_MAP_ID(m_sceneID),	girlWar->GetXNewPos(), girlWar->GetYNewPos());

				return -1;
			}

			if(oldSectroIndex != sectorIndex)
			{
				Sector * pSector = &m_totalSector[oldSectroIndex];
				if(pSector->Leave(obj))
				{
					LOG_ERROR(FILEINFO, "GirlWar[id=%d] leave sector error", girlWar->GetID());
					return -1;
				}

				pSector = &m_totalSector[sectorIndex];
				if(pSector->Join(obj))
				{
					LOG_ERROR(FILEINFO, "girlWar[id=%lld] enter sector error", girlWar->GetKey());
					return -1;
				}
			}

			int groupIndex = GetSectorGroupIndex(girlWar->GetXNewPos(), girlWar->GetYNewPos());
			if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
			{
				LOG_ERROR(FILEINFO, "groupindex in girlWar is invalid when girlWar move");
				return -1;
			}

			int groupOldIndex = GetSectorGroupIndex(girlWar->GetXPos(), girlWar->GetYPos());
			if(groupOldIndex < 0 || groupOldIndex >= m_totalGroupSize)
			{
				LOG_ERROR(FILEINFO, "groupOldIndex in girlWar is invalid when girlWar move");
				return -1;
			}

			SectorGroup * pGroup = &m_totalGroup[groupIndex];
			SectorGroup * pOldGroup = &m_totalGroup[groupOldIndex];

			pOldGroup->SendObjLeaveToGroup(obj);
			pGroup->SendAllObjectToGroup(obj, NULL);
			break;
		}
	default:
		{
			LOG_ERROR(FILEINFO, "unknown object type[%d] move", obj->GetType());

			return -1;
		}
	}
	return 0;
}

void CGameZone::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	switch(obj->GetType())
	{
	case ePlayer:
		{
			Smart_Ptr<Player> player = obj;
			if(player->GetGameZone() == NULL)
				return;

			int sectorIndex = GetSectorIndex(player->GetXNewPos(), player->GetYNewPos());
			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
			if(!info || sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex in player[%lld] is invalid(mapid=%d, xpos=%d, ypos=%d) when player leave",
						player->GetID(),
						(int)GET_MAP_ID(m_sceneID),
						player->GetXNewPos(),
						player->GetYNewPos());

				return;
			}

			Sector * pSector = &m_totalSector[sectorIndex];
			if(pSector->Leave(obj))
			{
				LOG_ERROR(FILEINFO, "player[gs charid=%lld] leave sector error", GET_PLAYER_CHARID(player->GetID()));
			}

			pSector->joinOrLeaveRegion(player,NULL,false);
			if(isDel)
			{
				int groupIndex = GetSectorGroupIndex(player->GetXNewPos(), player->GetYNewPos());
				if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
				{
					LOG_ERROR(FILEINFO, "groupindex in player is invalid when player leave");

					return;
				}

				SectorGroup * pGroup = &m_totalGroup[groupIndex];
				pGroup->SendObjLeaveToGroup(obj, 0);
			}

			INT64PLAYERMAP::iterator it = m_player.find(player->GetID());
			if(it != m_player.end())
			{
				it->second->UpdateEnemyList();

				INTOBJECTMAP::iterator itObj = m_allObject.find(it->second->GetKey());
				if(itObj != m_allObject.end())
				{
					m_allObject.erase(itObj);
				}
				m_player.erase(it);
			}

			if(player)
				player->ResetGameZone();

			afterLeave(player);

			break;
		}
	case eMonster:
		{
			Smart_Ptr<Monster> monster = obj;
			if(monster->GetGameZone() == NULL)
				return;

			int sectorIndex = GetSectorIndex(monster->GetXNewPos(), monster->GetYNewPos());
			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
			if(!info || sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex in monster[%d] is invalid(mapid=%d,xpos=%d,ypos=%d) when monster leave",
						(int)monster->GetID(),
						(int)GET_MAP_ID(m_sceneID),
						monster->GetXNewPos(),
						monster->GetYNewPos());

				return;
			}

			Sector * pSector = &m_totalSector[sectorIndex];
			if(pSector->Leave(obj))
			{
				LOG_ERROR(FILEINFO, "monster[id=%lld] leave sector error", monster->GetID());
			}

			//通知玩家怪物从其视野消失，假如是真正的删除的话就不要通知玩家移除视野，已经在monster->dead()告诉玩家怪物已经死亡
			if(isDel && !delFromMap)
			{
				int groupIndex = GetSectorGroupIndex(monster->GetXNewPos(), monster->GetYNewPos());
				if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
				{
					LOG_ERROR(FILEINFO, "groupindex in monster is invalid when monster leave");
					return;
				}

				SectorGroup * pGroup = &m_totalGroup[groupIndex];
				pGroup->SendObjLeaveToGroup(obj, 0);
			}

			if(delFromMap)
			{
				if(m_ReflushManage)
				{
					m_ReflushManage->monsterLeave(*monster.Get());
				}

				INTOBJECTMAP::iterator itObj = m_allObject.find(monster->GetKey());
				if(itObj != m_allObject.end())
				{
					m_allObject.erase(itObj);
				}

				INTVMPNSTERMAP::iterator monObj = m_allMonster.find(monster->GetID());
				if(monObj != m_allMonster.end())
				{
					vector<Smart_Ptr<Monster> >::iterator inner = monObj->second.begin();
					for(;inner != monObj->second.end(); )
					{
						if((*inner)->GetKey() == monster->GetKey())
						{
							monObj->second.erase(inner);
							break;
						}
						else
						{
							inner++;
						}
					}

					if(monObj->second.size() <= 0)
					{
						m_allMonster.erase(monObj);
					}
				}

				//清空怪物的仇恨列表
				monster->GetBrain()->ClearEnemy();
				monster->GetBrain()->clearGirlTarget();

				//monster->Release();
				//ObjectFactory::GetInstance()->Recycle(obj);

				Smart_Ptr<Monster> tempMon;
				monster->SetOwnerToBrain(tempMon);
			}
			break;
		}
	case eDropItem:
		{
			int sectorIndex = GetSectorIndex(obj->GetXPos(), obj->GetYPos());
			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
			if(!info || sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex in dropitem is invalid(mapid=%d, xpos=%d, ypos=%d) when dropitem leave",
						(int)GET_MAP_ID(m_sceneID),
						obj->GetXPos(),
						obj->GetYPos());

				return;
			}

			Sector * pSector = &m_totalSector[sectorIndex];
			if(pSector->Leave(obj))
			{
				LOG_ERROR(FILEINFO, "dropitem leave sector error");
			}

			if(isDel)
			{
				int groupIndex = GetSectorGroupIndex(obj->GetXPos(), obj->GetYPos());
				if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
				{
					LOG_ERROR(FILEINFO, "groupindex in dropitem is invalid when dropitem leave");
					return;
				}

				SectorGroup * pGroup = &m_totalGroup[groupIndex];
				pGroup->SendObjLeaveToGroup(obj, 0);
			}

			INTOBJECTMAP::iterator itObj = m_allObject.find(obj->GetKey());
			if(itObj != m_allObject.end())
			{
				m_allObject.erase(itObj);
			}

			//回收对象
			//Smart_Ptr<DropItemsAllot> item = obj;
			//item->Release();
			//ObjectFactory::GetInstance()->Recycle(obj);
			break;
		}

	case ePet:
		{
			Smart_Ptr<GirlWar> girlWar = obj;
			if(girlWar->GetGameZone() == NULL)
			{
				LOG_ERROR(FILEINFO, "girl[key=%d, owner=%lld] leave map[mapid=%d, xpos=%d, ypos=%d] but gamezone is null",
						girlWar->GetKey(),
						girlWar->GetOwner()->GetID(),
						(int)GET_MAP_ID(m_sceneID),
						girlWar->GetXNewPos(),
						girlWar->GetYNewPos());

				return;
			}

			int sectorIndex = GetSectorIndex(girlWar->GetXNewPos(), girlWar->GetYNewPos());
			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
			if(!info || sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex in girlWar[owner=%lld, id=%d] is invalid(mapid=%d,xpos=%d,ypos=%d) when girlWar leave",
						girlWar->GetOwner()->GetID(),
						(int)girlWar->GetID(),
						(int)GET_MAP_ID(m_sceneID),
						girlWar->GetXNewPos(),
						girlWar->GetYNewPos());

				return;
			}

			Sector * pSector = &m_totalSector[sectorIndex];
			if(pSector->Leave(obj))
			{
				LOG_ERROR(FILEINFO, "girlWar[id=%d] leave sector error", girlWar->GetID() );
			}

			if(isDel)
			{
				int groupIndex = GetSectorGroupIndex(girlWar->GetXNewPos(), girlWar->GetYNewPos());
				if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
				{
					LOG_ERROR(FILEINFO, "groupindex in Pet is invalid when Pet leave");
					return;
				}

				SectorGroup * pGroup = &m_totalGroup[groupIndex];
				pGroup->SendObjLeaveToGroup(obj, 0);
			}

			INTGRILWARMAP::iterator it = m_girlWar.find(girlWar->GetKey());
			if(it != m_girlWar.end())
			{
				INTOBJECTMAP::iterator itObj = m_allObject.find(it->second->GetKey());
				if(itObj != m_allObject.end())
				{
					m_allObject.erase(itObj);
				}
				m_girlWar.erase(it);
			}

			if(girlWar)
			{
				girlWar->ClearTargets();
				girlWar->ResetGameZone();
			}

			break;
		}
	default:
		{
			LOG_ERROR(FILEINFO, "unknown object type[%d] leave", obj->GetType());
		}
	}
}

void CGameZone::SendSynchMessage(::google::protobuf::Message *from, int msgid, int key, int xpos, int ypos, bool isMyself)
{
	int groupIndex = GetSectorGroupIndex(xpos, ypos);
	if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
	{
		LOG_ERROR(FILEINFO, "groupindex in object is invalid");

		return ;
	}

	SectorGroup * pGroup = &m_totalGroup[groupIndex];
	pGroup->SendSynchMessage(from, msgid, key, isMyself);
}

void CGameZone::SendMapAllSynchMessage(::google::protobuf::Message *from, int msgid, int key, bool isMyself)
{
	INT64PLAYERMAP::iterator it = m_player.begin();
	for(;it != m_player.end(); it++)
	{
		if(!isMyself)
		{
			if(key == it->second->GetKey())
			{
				continue;
			}
		}

		it->second->SendSynchMessage(from,msgid);
	}
}

void CGameZone::SendSynchPath(PlayerSynch::PathToAllPlayer &path, Smart_Ptr<Player> &player)
{
		int groupIndex = GetSectorGroupIndex(player->GetXNewPos(), player->GetYNewPos());
		if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
		{
			LOG_ERROR(FILEINFO, "groupindex in creature is invalid");

			return ;
		}

		SectorGroup * pGroup = &m_totalGroup[groupIndex];

		player->SetPath(path);
		pGroup->SendSynchPath(path);
}

void CGameZone::SendSynchPath(PlayerSynch::MonsterPathToPlayer &path, Smart_Ptr<Monster> &monster)
{
	int groupIndex = GetSectorGroupIndex(monster->GetXNewPos(), monster->GetYNewPos());
	if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
	{
		LOG_ERROR(FILEINFO, "groupindex in creature is invalid");

		return ;
	}

	//怪物有没有目标
	if(monster->GetTargetObj().Get() != NULL)
	{
		path.set_targetkey( monster->GetTargetObj()->GetKey() );
	}

	SectorGroup * pGroup = &m_totalGroup[groupIndex];
	pGroup->SendSynchPath(path);
}

void CGameZone::SendSynchPetPath(PlayerSynch::GirlWarPathToPlayer &path, Smart_Ptr<GirlWar> &girlWar)
{
	int groupIndex = GetSectorGroupIndex(girlWar->GetXNewPos(), girlWar->GetYNewPos());
	if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
	{
		LOG_ERROR(FILEINFO, "groupindex in creature is invalid");

		return ;
	}

	SectorGroup * pGroup = &m_totalGroup[groupIndex];

	pGroup->SendSynchPath(path);
}

void CGameZone::SendSynchAttr(CharBase::SynchObjectAttr &attr, int xpos, int ypos, bool isMyself)
{
	int groupIndex = GetSectorGroupIndex(xpos, ypos);
	if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
	{
		LOG_ERROR(FILEINFO, "groupindex in creature is invalid");

		return ;
	}

	SectorGroup * pGroup = &m_totalGroup[groupIndex];
	pGroup->SendSynchAttr(attr, isMyself);
}

void CGameZone::SendAllMapAttr(CharBase::SynchObjectAttr &attr)
{
	INT64PLAYERMAP::iterator it = m_player.begin();
	for(;it != m_player.end(); it++)
	{
		 it->second->SendSynchAttr(attr);
	}
}

void CGameZone::SendSynBuffChange(CharBattle::BuffOperate *buf, int key, int xpos, int ypos)
{
	int groupIndex = GetSectorGroupIndex(xpos, ypos);
	if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
	{
		LOG_ERROR(FILEINFO, "groupindex in creature is invalid");

		return ;
	}

	SectorGroup * pGroup = &m_totalGroup[groupIndex];
	pGroup->SendSynBuffChange(buf, key);
}

void CGameZone::SendSynchAction(CharBattle::SynClientAttack &action, int xpos, int ypos)
{
	int groupIndex = GetSectorGroupIndex(xpos, ypos);
	if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
	{
		LOG_ERROR(FILEINFO, "groupindex in creature is invalid");

		return ;
	}

	SectorGroup * pGroup = &m_totalGroup[groupIndex];
	pGroup->SendSynchAction(action);
}

Smart_Ptr<NPC> CGameZone::GetNpcByID(int npcid)
{
	if(npcid <= 0)
		return 0;

	INTNPCMAP::iterator it = m_npc.find(npcid);
	if(it == m_npc.end())
	{
		LOG_ERROR(FILEINFO, "get zone[%lld] npc[id=%d] but not existed", m_sceneID, npcid);

		return 0;
	}

	return it->second;
}

const INTOBJECTMAP & CGameZone::GetAllObjects()
{
	return m_allObject;
}

void CGameZone::GetObject(int key, Smart_Ptr<CreatureObj> &obj, bool isMon)
{
	INTOBJECTMAP::iterator itMap = m_allObject.find(key);
	if(itMap != m_allObject.end())
	{
		if(itMap->second->GetType() == ePlayer || itMap->second->GetType() == ePet)
		{
			obj = itMap->second;
		}

		if(itMap->second->GetType() == eMonster)
		{
			if(isMon)
			{
				Smart_Ptr<Monster> mon = itMap->second;

				if(mon->GetQuality() != eGatherMonster && mon->GetQuality() != eElementMonster && mon->GetQuality() != ePVPGatherMonster &&
						mon->GetQuality() != eBoxMonster && mon->GetQuality() != eBatterFlag && mon->GetQuality() != eChallengeBox)
				{
					obj = itMap->second;
				}
			}
			else
			{
				obj = itMap->second;
			}
		}
	}
}

void CGameZone::GetPlayer(int xpos, int ypos, int distance, int country, vector<Smart_Ptr<CreatureObj> > &obj)
{
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));

	int startx = 0;
	int starty = 0;
	int endx = 0;
	int endy = 0;

	startx = xpos - distance;
	starty = ypos - distance;
	endx = xpos + distance;
	endy = ypos + distance;

	if(startx < 0)
		startx = 0;

	if(starty < 0)
		starty = 0;

	if(endx >= info->GetXMaxNum())
		endx = info->GetXMaxNum() - 1;

	if(endy >= info->GetYMaxNum())
		endy = info->GetYMaxNum() - 1;

	for(int i=starty; i<=endy; ++i)
	{
		for(int j=startx; j<=endx; ++j)
		{
			int sectorIndex = GetSectorIndex(j, i);
			if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				continue;
			}

			Sector * pSector = &m_totalSector[sectorIndex];

			pSector->GetPlayer(country, obj);
		}
	}
}

void CGameZone::GetObject(int xpos, int ypos, int distance, int country,eObjType eType, vector<Smart_Ptr<CreatureObj> > &obj)
{
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));

	int startx = 0;
	int starty = 0;
	int endx = 0;
	int endy = 0;

	startx = xpos - distance;
	starty = ypos - distance;
	endx = xpos + distance;
	endy = ypos + distance;

	if(startx < 0)
		startx = 0;

	if(starty < 0)
		starty = 0;

	if(endx >= info->GetXMaxNum())
		endx = info->GetXMaxNum() - 1;

	if(endy >= info->GetYMaxNum())
		endy = info->GetYMaxNum() - 1;

	for(int i=starty; i<=endy; ++i)
	{
		for(int j=startx; j<=endx; ++j)
		{
			int sectorIndex = GetSectorIndex(j, i);
			if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				continue;
			}

			Sector * pSector = &m_totalSector[sectorIndex];

			pSector->GetCreatrueObj(country, eType,obj);
		}
	}
}

void CGameZone::GetObject(Smart_Ptr<CreatureObj> &owner, int xpos, int ypos, int type, int distance,int width, vector<Smart_Ptr<CreatureObj> > &obj)
{
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));

	int startx = 0;
	int starty = 0;
	int endx = 0;
	int endy = 0;

	switch(type)
	{
	case eRangeSingle:
	case eRangeMyself:
	case eRangePoint:
		{
			startx = xpos - distance;
			starty = ypos - distance;
			endx = xpos + distance;
			endy = ypos + distance;

			if(startx < 0)
				startx = 0;

			if(starty < 0)
				starty = 0;

			if(endx >= info->GetXMaxNum())
				endx = info->GetXMaxNum() - 1;

			if(endy >= info->GetYMaxNum())
				endy = info->GetYMaxNum() - 1;

			for(int i=starty; i<=endy; ++i)
			{
				for(int j=startx; j<=endx; ++j)
				{
					int sectorIndex = GetSectorIndex(j, i);
					if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
					{
						continue;
					}

					Sector * pSector = &m_totalSector[sectorIndex];

					pSector->GetCreatrueObj(obj);
				}
			}
			break;
		}
	case eRangeFan:
		{
			if(xpos == owner->GetXNewPos())
			{
				bool isypos = false;

				if(ypos > owner->GetYNewPos())
				{
					isypos = true;
				}

				for(int i=0; i<=distance; ++i)
				{
					int typos = 0;
					if(isypos)
					{
						typos = owner->GetYNewPos() + i;
						if(typos >= info->GetYMaxNum())
							continue;
					}
					else
					{
						typos = owner->GetYNewPos() - i;
						if(typos < 0)
							continue;
					}

					for(int j=0-i; j<=i; ++j)
					{
						int txminpos = owner->GetXNewPos() + j;

						int sectorMinIndex = GetSectorIndex(txminpos, typos);
						if(sectorMinIndex < 0 || sectorMinIndex >= info->GetAllSectorSize())
						{
							continue;
						}

						Sector * pMinSector = &m_totalSector[sectorMinIndex];
						pMinSector->GetCreatrueObj(obj);
					}
				}
			}
			else if(ypos == owner->GetYNewPos())
			{
				bool isxpos = false;

				if(xpos > owner->GetXNewPos())
				{
					isxpos = true;
				}

				for(int i=0; i<=distance; ++i)
				{
					int txpos = 0;
					if(isxpos)
					{
						txpos = owner->GetXNewPos() + i;
						if(txpos >= info->GetXMaxNum())
							continue;
					}
					else
					{
						txpos = owner->GetXNewPos() - i;
						if(txpos < 0)
							continue;
					}

					for(int j=0-i; j<=i; ++j)
					{
						int tyminpos = owner->GetYNewPos() + j;

						int sectorMinIndex = GetSectorIndex(txpos, tyminpos);
						if(sectorMinIndex < 0 || sectorMinIndex >= info->GetAllSectorSize())
						{
							continue;
						}

						Sector * pMinSector = &m_totalSector[sectorMinIndex];
						pMinSector->GetCreatrueObj(obj);
					}
				}
			}
			else
			{
				bool isypos = false;
				bool isxpos = false;

				if(ypos > owner->GetYNewPos())
				{
					isypos = true;
				}
				if(xpos > owner->GetXNewPos())
				{
					isxpos = true;
				}

				for(int i=0; i<=distance; ++i)
				{
					int typos = 0;
					int txpos = 0;

					if(isypos)
					{
						typos = owner->GetYNewPos() + i;
						if(typos >= info->GetYMaxNum())
							continue;
					}
					else
					{
						typos = owner->GetYNewPos() - i;
						if(typos < 0)
							continue;
					}

					if(isxpos)
					{
						txpos = owner->GetXNewPos() + i;
						if(txpos >= info->GetXMaxNum())
							continue;
					}
					else
					{
						txpos = owner->GetXNewPos() - i;
						if(txpos < 0)
							continue;
					}

					for(int j=0; j<=i; ++j)
					{
						int ttypos = -1;

						if(isypos)
						{
							ttypos = owner->GetYNewPos() + j;
							if(ttypos >= info->GetYMaxNum())
								continue;
						}
						else
						{
							ttypos = owner->GetYNewPos() - j;
							if(ttypos < 0)
								continue;
						}

						int sectorMinIndex = GetSectorIndex(txpos, ttypos);
						if(sectorMinIndex < 0 || sectorMinIndex >= info->GetAllSectorSize())
						{
							continue;
						}

						Sector * pMinSector = &m_totalSector[sectorMinIndex];
						pMinSector->GetCreatrueObj(obj);
					}

					for(int k=0; k<i; ++k)
					{
						int ttxpos = -1;

						if(isxpos)
						{
							ttxpos = owner->GetXNewPos() + k;
							if(ttxpos >= info->GetXMaxNum())
								continue;
						}
						else
						{
							ttxpos = owner->GetXNewPos() - k;
							if(ttxpos < 0)
								continue;
						}

						int sectorMinIndex = GetSectorIndex(ttxpos, typos);
						if(sectorMinIndex < 0 || sectorMinIndex >= info->GetAllSectorSize())
						{
							continue;
						}

						Sector * pMinSector = &m_totalSector[sectorMinIndex];
						pMinSector->GetCreatrueObj(obj);
					}
				}
			}
			break;
		}
	case eRangeLine:
		{
			if(xpos == owner->GetXNewPos() || ypos == owner->GetYNewPos())
			{
				bool isxpos = false;
				bool isypos = false;

				if(xpos > owner->GetXNewPos())
				{
					isxpos = true;
				}
				if(ypos > owner->GetYNewPos())
				{
					isypos = true;
				}

				for(int i=0; i<=distance; ++i)
				{
					int txpos = 0;
					int typos = 0;

					if(xpos == owner->GetXNewPos())
					{
						if(isypos)
						{
							typos = owner->GetYNewPos() + i;
							if(typos >= info->GetYMaxNum())
								continue;
						}
						else
						{
							typos = owner->GetYNewPos() - i;
							if(typos < 0)
								continue;
						}
						txpos = xpos;
					}
					else
					{
						if(isxpos)
						{
							txpos = owner->GetXNewPos() + i;
							if(txpos >= info->GetXMaxNum())
								continue;
						}
						else
						{
							txpos = owner->GetXNewPos() - i;
							if(txpos < 0)
								continue;
						}
						typos = ypos;
					}

					int sectorMinIndex = GetSectorIndex(txpos, typos);
					if(sectorMinIndex < 0 || sectorMinIndex >= info->GetAllSectorSize())
					{
						continue;
					}

					Sector * pMinSector = &m_totalSector[sectorMinIndex];
					pMinSector->GetCreatrueObj(obj);
				}
			}
			else
			{
				bool isxpos = false;
				bool isypos = false;

				if(xpos > owner->GetXNewPos())
				{
					isxpos = true;
				}
				if(ypos > owner->GetYNewPos())
				{
					isypos = true;
				}

				int btxpos = -1;
				int btypos = -1;
				for(int i=0; i<=2 * distance; )
				{
					float txpos = 0;
					float typos = 0;

					if(i == 0)
					{
						int sectorMinIndex = GetSectorIndex(owner->GetXNewPos(), owner->GetYNewPos());
						if(sectorMinIndex < 0 || sectorMinIndex >= info->GetAllSectorSize())
						{
							++i;
							continue;
						}

						Sector * pMinSector = &m_totalSector[sectorMinIndex];
						pMinSector->GetCreatrueObj(obj);
						++i;
						btxpos = owner->GetXNewPos();
						btypos = owner->GetYNewPos();

						continue;
					}
					else
					{
						float xtan = i * 0.5;
						float ytan = i * 0.5 * abs(ypos - owner->GetYNewPos()) / abs(xpos - owner->GetXNewPos());
						int archiey = ((int)( ytan * 10 )) % 5;
						int dividey = (int)(ytan / 0.5) ;
						int tarchiey = dividey % 2;
						if(isxpos)
						{
							txpos = owner->GetXNewPos() + xtan;
							if(txpos >= info->GetXMaxNum())
							{
								i += 2;
								continue;
							}

							if(archiey > 0.0)
							{//不在相交点上面，取两边
								if(tarchiey == 0)
								{//偶数表示线条位于中点上方
									if(isypos)
									{
										typos = owner->GetYNewPos() + dividey * 0.5 ;
										if(typos >= info->GetYMaxNum())
										{
											i += 2;
											continue;
										}
									}
									else
									{
										typos = owner->GetYNewPos() - dividey * 0.5 ;
										if(typos < 0)
										{
											i += 2;
											continue;
										}
									}
								}
								else
								{//中点下方
									if(isypos)
									{
										typos = owner->GetYNewPos() + (dividey + 1) * 0.5 ;
										if(typos >= info->GetYMaxNum())
										{
											i += 2;
											continue;
										}
									}
									else
									{
										typos = owner->GetYNewPos() - (dividey + 1) * 0.5 ;
										if(typos < 0)
										{
											i += 2;
											continue;
										}
									}
								}

								int tDis = abs(btypos - (int)typos);
								if(tDis > 1)
								{
									int tbtypos = btypos;
									for(int len=1; len <tDis; ++len)
									{
										int tttypos = -1;
										if(isypos)
										{
											tttypos = tbtypos + len;
											if(tttypos >= info->GetYMaxNum())
												 break;
										}
										else
										{
											tttypos = tbtypos - len;
											if(tttypos < 0)
												break;
										}

										int sectorMinIndex = GetSectorIndex(btxpos, tttypos);
										if(sectorMinIndex < 0 || sectorMinIndex >= info->GetAllSectorSize())
										{
											break;
										}

										Sector * pMinSector = &m_totalSector[sectorMinIndex];
										pMinSector->GetCreatrueObj(obj);
										btypos = tttypos;
									}
								}

								for(float j=-0.5; j<=0.5; )
								{
									int ttxpos = -1;
									if(j == 0)
									{
										j += 0.5;
										continue;
									}

									ttxpos =	(int)(txpos + j);
									if(ttxpos >= info->GetXMaxNum() || ttxpos < 0 || (btxpos == ttxpos && btypos == typos))
									{
										j += 0.5;
										continue;
									}

									int sectorMinIndex = GetSectorIndex(ttxpos, (int)typos);
									if(sectorMinIndex < 0 || sectorMinIndex >= info->GetAllSectorSize())
									{
										j += 0.5;
										continue;
									}

									Sector * pMinSector = &m_totalSector[sectorMinIndex];
									pMinSector->GetCreatrueObj(obj);
									btxpos = ttxpos;
									btypos = (int)typos;
									j += 0.5;
								}
							}
							else
							{
								if(isypos)
								{
									typos = owner->GetYNewPos() + ytan ;
									if(typos >= info->GetYMaxNum())
									{
										i += 2;
										continue;
									}

									for(float j=-0.5; j<=0.5; )
									{
										int ttypos = -1;
										if(j == 0)
										{
											j += 0.5;
											continue;
										}

										ttypos = (int)(typos + j);
										if(ttypos >= info->GetYMaxNum() || ttypos < 0)
										{
											j += 0.5;
											continue;
										}

										for(float k=-0.5; k<=0.5; )
										{
											int ttxpos = -1;
											if(k == 0)
											{
												k += 0.5;
												continue;
											}

											ttxpos = (int)(txpos + k);
											if(ttxpos >= info->GetXMaxNum() || ttxpos < 0 || (btxpos == ttxpos && btypos == ttypos))
											{
												k += 0.5;
												continue;
											}

											int sectorMinIndex = GetSectorIndex(ttxpos, ttypos);
											if(sectorMinIndex < 0 || sectorMinIndex >= info->GetAllSectorSize())
											{
												k += 0.5;
												continue;
											}

											Sector * pMinSector = &m_totalSector[sectorMinIndex];
											pMinSector->GetCreatrueObj(obj);
											btxpos = ttxpos;
											btypos = ttypos;
											k += 0.5;
										}

										j += 0.5;
									}
								}
								else
								{
									typos = owner->GetYNewPos() - ytan;
									if(typos < 0)
									{
										i += 2;
										continue;
									}

									for(float j=0.5; j>=-0.5; )
									{
										int ttypos = -1;
										if(j == 0)
										{
											j -= 0.5;
											continue;
										}

										ttypos = (int)(typos + j);
										if(ttypos >= info->GetYMaxNum() || ttypos < 0)
										{
											j -= 0.5;
											continue;
										}

										for(float k=-0.5; k<=0.5; )
										{
											int ttxpos = -1;
											if(k == 0)
											{
												k += 0.5;
												continue;
											}

											ttxpos = (int)(txpos + k);
											if(ttxpos >= info->GetXMaxNum() || ttxpos < 0 || (btxpos == ttxpos && btypos == ttypos))
											{
												k += 0.5;
												continue;
											}

											int sectorMinIndex = GetSectorIndex(ttxpos, ttypos);
											if(sectorMinIndex < 0 || sectorMinIndex >= info->GetAllSectorSize())
											{
												k += 0.5;
												LOG_ERROR(FILEINFO, "sectorindex is invalid");
												continue;
											}

											Sector * pMinSector = &m_totalSector[sectorMinIndex];
											pMinSector->GetCreatrueObj(obj);
											btxpos = ttxpos;
											btypos = ttypos;
											k += 0.5;
										}

										j -= 0.5;
									}
								}
							}
						}
						else
						{
							txpos = owner->GetXNewPos() - xtan;
							if(txpos < 0)
							{
								i += 2;
								continue;
							}

							if(archiey > 0.0)
							{//不在相交点上面，取两边
								if(tarchiey == 0)
								{//偶数表示线条位于中点上方
									if(isypos)
									{
										typos = owner->GetYNewPos() + dividey * 0.5 ;
										if(typos >= info->GetYMaxNum())
										{
											i += 2;
											continue;
										}
									}
									else
									{
										typos = owner->GetYNewPos() - dividey * 0.5 ;
										if(typos < 0)
										{
											i += 2;
											continue;
										}
									}
								}
								else
								{//中点下方
									if(isypos)
									{
										typos = owner->GetYNewPos() + (dividey + 1) * 0.5 ;
										if(typos >= info->GetYMaxNum())
										{
											i += 2;
											continue;
										}
									}
									else
									{
										typos = owner->GetYNewPos() - (dividey + 1) * 0.5 ;
										if(typos < 0)
										{
											i += 2;
											continue;
										}
									}
								}

								int tDis = abs(btypos - (int)typos);
								if(tDis > 1)
								{
									int tbtypos = btypos;
									for(int len=1; len <tDis; ++len)
									{
										int tttypos = -1;
										if(isypos)
										{
											tttypos = tbtypos + len;
											if(tttypos >= info->GetYMaxNum())
												 break;
										}
										else
										{
											tttypos = tbtypos - len;
											if(tttypos < 0)
												break;
										}

										int sectorMinIndex = GetSectorIndex(btxpos, tttypos);
										if(sectorMinIndex < 0 || sectorMinIndex >= info->GetAllSectorSize())
										{
											break;
										}

										Sector * pMinSector = &m_totalSector[sectorMinIndex];
										pMinSector->GetCreatrueObj(obj);
										btypos = tttypos;
									}
								}

								for(float j=0.5; j>=-0.5; )
								{
									int ttxpos = -1;
									if(j == 0)
									{
										j -= 0.5;
										continue;
									}

									ttxpos = (int)(txpos + j);
									if(ttxpos >= info->GetXMaxNum() || ttxpos < 0 || (btxpos == ttxpos && btypos == typos))
									{
										j -= 0.5;
										continue;
									}

									int sectorMinIndex = GetSectorIndex(ttxpos, (int)typos);
									if(sectorMinIndex < 0 || sectorMinIndex >= info->GetAllSectorSize())
									{
										j -= 0.5;
										continue;
									}

									Sector * pMinSector = &m_totalSector[sectorMinIndex];
									pMinSector->GetCreatrueObj(obj);
									btxpos = ttxpos;
									btypos = (int)typos;
									j -= 0.5;
								}
							}
							else
							{
								if(isypos)
								{
									typos = owner->GetYNewPos() + ytan ;
									if(typos >= info->GetYMaxNum())
									{
										i += 2;
										continue;
									}

									for(float j=-0.5; j<=0.5; )
									{
										int ttypos = -1;
										if(j == 0)
										{
											j += 0.5;
											continue;
										}

										ttypos = (int)(typos + j);
										if(ttypos >= info->GetYMaxNum() || ttypos < 0)
										{
											j += 0.5;
											continue;
										}

										for(float k=0.5; k>=-0.5; )
										{
											int ttxpos = -1;
											if(k == 0)
											{
												k -= 0.5;
												continue;
											}

											ttxpos = (int)(txpos + k);
											if(ttxpos >= info->GetXMaxNum() || ttxpos < 0 || (btxpos == ttxpos && btypos == ttypos))
											{
												k -= 0.5;
												continue;
											}

											int sectorMinIndex = GetSectorIndex(ttxpos, ttypos);
											if(sectorMinIndex < 0 || sectorMinIndex >= info->GetAllSectorSize())
											{
												k -= 0.5;
												continue;
											}

											Sector * pMinSector = &m_totalSector[sectorMinIndex];
											pMinSector->GetCreatrueObj(obj);
											btxpos = ttxpos;
											btypos = ttypos;
											k -= 0.5;
										}

										j += 0.5;
									}
								}
								else
								{
									typos = owner->GetYNewPos() - ytan;
									if(typos < 0)
									{
										i += 2;
										continue;
									}

									for(float j=0.5; j>=-0.5; )
									{
										int ttypos = -1;
										if(j == 0)
										{
											j -= 0.5;
											continue;
										}

										ttypos = (int)(typos + j);
										if(ttypos >= info->GetYMaxNum() || ttypos < 0)
										{
											j -= 0.5;
											continue;
										}

										for(float k=0.5; k>=-0.5; )
										{
											int ttxpos = -1;
											if(k == 0)
											{
												k -= 0.5;
												continue;
											}

											ttxpos = (int)(txpos + k);
											if(ttxpos >= info->GetXMaxNum() || ttxpos < 0 || (btxpos == ttxpos && btypos == ttypos))
											{
												k -= 0.5;
												continue;
											}

											int sectorMinIndex = GetSectorIndex(ttxpos, ttypos);
											if(sectorMinIndex < 0 || sectorMinIndex >= info->GetAllSectorSize())
											{
												k -= 0.5;
												continue;
											}

											Sector * pMinSector = &m_totalSector[sectorMinIndex];
											pMinSector->GetCreatrueObj(obj);
											btxpos = ttxpos;
											btypos = ttypos;
											k -= 0.5;
										}

										j -= 0.5;
									}
								}
							}
						}

						i += 2;
					}
				}
			}

			break;
		}
	case eRectangle:
		{
			int tempIndex = GetSectorGroupIndex(owner->GetXNewPos(),owner->GetYNewPos());
			if(tempIndex >= m_totalGroupSize)
			{
				return ;
			}

			EffectVector tempBeginPos;
			tempBeginPos.x = (float)owner->GetXNewPos();
			tempBeginPos.y = (float)owner->GetYNewPos();

			EffectVector tempEndPos;
			tempEndPos.x = (float)xpos;
			tempEndPos.y = (float)ypos;

			EffectMath::findTargetByRect(m_totalGroup[tempIndex],tempBeginPos,tempEndPos,obj,distance,width);

			break;
		}
	}

	return ;
}

void CGameZone::GetAroundPos(int xpos, int ypos, int count, vector<Sector *> &vec)
{
	vector<int> sectorIndex;
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	size_t i = 0;

	for(; (size_t)count>sectorIndex.size(); ++i)
	{
		//如果大于一定次数的话，要退出，防止死循环
		if(i>=1000)
		{
			break;
		}
		info->GetAroundSector(this, i, xpos, ypos, count, sectorIndex);
	}

	vector<int>::iterator it = sectorIndex.begin();
	for(; it!=sectorIndex.end(); ++it)
	{
		vec.push_back(&m_totalSector[*it]);
	}
}

void CGameZone::GetAroundPos(int xpos, int ypos, int expos, int eypos, vector<Pos> &vec)
{
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(info)
	{
		info->GetAroundSector(this, xpos, ypos, expos, eypos, vec);
	}
}

void CGameZone::GetWantPlayers(const uint &want_sz, std::vector<Smart_Ptr<Player> > &players)
{
	INT64PLAYERMAP::iterator it = m_player.begin();
	for(;it != m_player.end(); it++)
	{
		players.push_back(it->second);
		if(players.size() >=want_sz)
			return ;
	}
}

//获取本场景的所有玩家
void CGameZone::GetAllPlayers(std::vector<Smart_Ptr<Player> > &players)
{
	INT64PLAYERMAP::iterator it = m_player.begin();
	for(;it != m_player.end(); it++)
	{
		players.push_back(it->second);
	}
}

void CGameZone::GetAllPlayers(INT64PLAYERMAP &players)
{
	players = m_player;
}

void CGameZone::GetGroupObject(Smart_Ptr<CreatureObj> &owner, vector<Smart_Ptr<CreatureObj> > &obj)
{
	int groupIndex = GetSectorGroupIndex(owner->GetXPos(), owner->GetYPos());
	if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
	{
		LOG_ERROR(FILEINFO, "groupindex in creature is invalid");

		return ;
	}

	SectorGroup * pGroup = &m_totalGroup[groupIndex];
	pGroup->GetGroupObject(obj);
}

bool CGameZone::isInstanced()
{
	MapStaticInfo *info = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(m_sceneID));
	if(!info)
		return false;
	return info->maptype;
}

EMapType CGameZone::GetInstancedType()
{
	MapStaticInfo *info = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(m_sceneID));
	if(!info)
		return eMapMax;

	return EMapType(info->maptype);
}

string CGameZone::GetMapName()
{
	int mapid = (m_sceneID >> 16) & 0xff;
	string mapname = "no map name";
	MapStaticInfo *info = CMapConfig::GetInstance()->GetStaticInfo(mapid);
	if(info)
	{
		return mapname = info->mapPath;
	}

	return mapname;
}

void CGameZone::UpdatePath( const PlayerSynch::SingleMonsterPath * path )
{
	INTOBJECTMAP::iterator itMap = m_allObject.find(path->key());
	if(itMap != m_allObject.end())
	{
		if(itMap->second->GetType() == ePet)
		{
			Smart_Ptr<GirlWar> girlWar = itMap->second;

			girlWar->GetBrain()->UpdatePath(path);
		}
		else
		{
			Smart_Ptr<Monster> monster = itMap->second;

			monster->GetBrain()->UpdatePath(path);
		}
	}
}

bool CGameZone::IsValidPos(PlayerSynch::SynchInfo &pos)
{
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));

	for(int i=0; i<pos.pos_size()-1; ++i)
	{
		PlayerSynch::CharPos single_f = pos.pos(i);
		PlayerSynch::CharPos single_s = pos.pos(i+1);

		int index_f = (single_f.xpos() + single_f.ypos() * info->GetXMaxNum());
		int index_s = (single_s.xpos() + single_s.ypos() * info->GetXMaxNum());

		if(index_f < 0 || index_f >= info->GetAllSectorSize() || index_s < 0 || index_s >= info->GetAllSectorSize())
		{
			return false;
		}

		if(!m_totalSector[index_f].GetSectorFlag() || !m_totalSector[index_s].GetSectorFlag())
		{
			return false;
		}

		int distanceX = abs(single_f.xpos() - single_s.xpos());
		int distanceY = abs(single_f.ypos() - single_s.ypos());

		if(distanceX > 1 || distanceY > 1)
			return false;
	}

	if(pos.pos_size() == 1)
	{
		PlayerSynch::CharPos single = pos.pos(0);

		int index = (single.xpos() + single.ypos() * info->GetXMaxNum());

		if(index < 0 || index >= info->GetAllSectorSize())
		{
			return false;
		}

		if(!m_totalSector[index].GetSectorFlag())
		{
			return false;
		}
	}

	return true;
}

bool CGameZone::IsValidPos(int xpos, int ypos)
{
	if(xpos < 0 || ypos < 0)
	{
		return false;
	}

	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(info == NULL)
		return false;

	if(xpos >= info->GetXMaxNum() || ypos >= info->GetYMaxNum())
		return false;

	int index = xpos + ypos * info->GetXMaxNum();
	if(index < 0 || index >= info->GetAllSectorSize())
		return false;

	if(!m_totalSector[index].GetSectorFlag())
		return false;

	return true;
}

bool CGameZone::HaveValidPos(int xpos, int ypos, int expos, int eypos)
{
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(info)
	{
		return info->HaveValidPos(this, xpos, ypos, expos, eypos);
	}

	return false;
}

bool CGameZone::RandGetValidPos(int xpos, int ypos, int &expos, int &eypos)
{
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(info)
	{
		return info->RandGetValidPos(this, xpos, ypos, expos, eypos);
	}

	return false;
}

bool CGameZone::RandGetPetValidPos(int xpos, int ypos, int &expos, int &eypos)
{
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(info)
	{
		return info->RandGetPetValidPos(this, xpos, ypos, expos, eypos);
	}

	return false;
}

bool CGameZone::IsValidPos(int bxpos, int bypos, int expos, int eypos)
{
	FieldInfo* tempInfo = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(tempInfo == NULL)
		return false;

	if(bxpos == expos && bypos == eypos)
		return false;

	if(bxpos < 0 || bypos < 0 || expos < 0 || eypos < 0 || bxpos >= tempInfo->GetXMaxNum() || bypos >= tempInfo->GetYMaxNum())
		return false;

	int tempXLenght = expos - bxpos;
	int tempYLenght = eypos - bypos;

	//斜率是否在-1~1区间
	bool tempIsLess = false;
	//斜率，下面用于求值，对应tan和ctan
	float tempSlope = .0f;

	if(tempXLenght == 0)
	{
		tempSlope  = 0;
		tempIsLess = false;
	}
	else if(tempYLenght == 0)
	{
		tempSlope  = 0;
		tempIsLess = true;
	}
	else
	{
		//tempXLenght不能为0，在－45度和正45度之间，包括135到225之间
		tempSlope = (float)tempYLenght / (float)tempXLenght;
		if(tempSlope >= -1 && tempSlope <= 1)
		{
			tempIsLess = true;
		}
		else
		{
			tempSlope  = (float)tempXLenght / (float)tempYLenght;
			tempIsLess = false;
		}
	}

	int tempLenght = 0;
	int tempIndexX = 0;
	int tempIndexY = 0;
	//递增方向
	int tempIncreaseDir    = 1;

	//X方向
	if(tempIsLess)
	{
		if(expos > bxpos)
			tempIncreaseDir = 1;
		else
			tempIncreaseDir = -1;

		tempLenght = abs(expos - bxpos);

		for (int i = 0;i <= tempLenght;++i)
		{
			//点处于格子中间，取整后得到索引，所以+0.5
			float tempIncrease = i * tempIncreaseDir + 0.5f;

			tempIndexX = (int)(bxpos + tempIncrease);
			tempIndexY = (int)(bypos + tempIncrease * tempSlope);

			if(tempIndexX >= tempInfo->GetXMaxNum() || tempIndexY >= tempInfo->GetYMaxNum())
				return false;

			if(tempIndexX < 0 || tempIndexY < 0)
				return false;

			//格子索引
			int tempTotalIndex = tempIndexY * tempInfo->GetXMaxNum() + tempIndexX;
			if(!m_totalSector[tempTotalIndex].GetSectorFlag())
				return false;
		}
	}
	//y方向
	else
	{
		if(bypos > eypos)
			tempIncreaseDir = 1;
		else
			tempIncreaseDir = -1;

		tempLenght = abs(eypos - bypos);

		for (int i = 0;i <= tempLenght;++i)
		{
			//点处于格子中间，取整后得到索引，所以+0.5
			float tempIncrease = i * tempIncreaseDir + 0.5f;

			tempIndexY = (int)(bypos + tempIncrease);
			tempIndexX = (int)(bxpos + tempIncrease * tempSlope);

			if(tempIndexX >= tempInfo->GetXMaxNum() || tempIndexY >= tempInfo->GetYMaxNum())
				return false;

			if(tempIndexX < 0 || tempIndexY < 0)
				return false;

			//格子索引
			int tempTotalIndex = tempIndexY * tempInfo->GetXMaxNum() + tempIndexX;
			if(!m_totalSector[tempTotalIndex].GetSectorFlag())
				return false;
		}
	}

	return true;
}

//动态创建怪物
Smart_Ptr<Monster> CGameZone::CreateMonsterDymic(DWORD monsterId,DWORD xPos,DWORD yPos, Smart_Ptr<Player> owner)
{
	if (monsterId == 0)
		return NULL;

	MonsterInfo info;

	MonsterLoader::GetInstance()->GetMonsterInfo(monsterId, info);
	if(info.id == -1)
	{
		LOG_ERROR(FILEINFO, "scene create monster but monster id is null");
		return NULL;
	}

	vector<Smart_Ptr<Monster> > vecMonster;
	Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eMonster);
	Smart_Ptr<Monster> mon = obj;

	mon->SetID(info.id);
	mon->SetName(info.name);
	mon->getAttManage()->setValue(eCharLv,info.lv);
	mon->SetQuality(info.quality);
	mon->SetFlushTime(0);				//副本BOSS不刷新
	mon->SetAliveTime(info.aliveTime);
	mon->getAttManage()->setValue(eCharCountry,info.camp);
	mon->getAttManage()->setValue(eCharMoveSpeed,info.idleSpeed);
	mon->SetRunSpeed(info.moveSpeed);
//	mon->SetBaseAi(info.baseAI);
//	mon->SetBetterAi(info.betterAI);
	mon->SetFallID(info.fallID);
	mon->getAttManage()->setValue(eCharExp,info.exp);
	mon->getAttManage()->setValue(eCharPhyAttackMax,info.maxAttack);
	mon->getAttManage()->setValue(eCharPhyDefence,info.phyDefence);
	mon->getAttManage()->setValue(eCharHitRate,info.hitRate);
	mon->getAttManage()->setValue(eCharAvoidRate,info.avoidRate);
	mon->getAttManage()->setValue(eCrackDownRate,info.crackRate);
	mon->getAttManage()->setValue(eAvoidCrackRate,info.avoidCrack);
	mon->SetHPMax(info.maxHP);
	mon->SetCurHP(info.maxHP);
	mon->SetMPMax(info.maxMP);
	mon->SetCurMP(info.maxMP);
	mon->SetOwnerToBrain(mon);
	mon->SetMapID(m_sceneID);
	mon->SetBornTime();
	mon->SetOwnerName("");

	//任务剧情刷新的怪，属于某个具体的玩家
	if(owner && info.quality == eNpcChangeToMonster)
	{
		mon->SetOwnerName(owner->GetName());
		mon->SetOwner(owner);
	}

	//镖车
	if(owner && info.quality == eEscortCar)
	{
		mon->SetOwnerName(owner->GetName());
		mon->SetOwner(owner);
		mon->getAttManage()->setValue(eCharCountry,owner->getAttManage()->getValue<DWORD>(eCharCountry));
	}

	int index = rand() % 4;

	mon->SetXPos(xPos);
	mon->SetYPos(yPos);
	mon->SetOriginalX(xPos + index);
	mon->SetOriginalY(yPos + index);
//	mon->SetGroup(GetSectorGroupIndex(xPos, yPos));

	Join(obj);

	vector<Sector *> moveVec;
	moveVec.clear();
	BaseAIInfo *aiInfo = 0;
	BetterAIInfo *betterAI = 0;

	aiInfo = AILoader::GetInstance()->GetBaseAIByid(info.baseAI);
	if(!aiInfo)
	{
		LOG_ERROR(FILEINFO, "monster[id=%d] ai is error", info.id);

		return 0;
	}

	if(info.betterAI > 0)
	{
		betterAI = AILoader::GetInstance()->GetBetterAIByid(info.betterAI);
		if (!betterAI)
		{
			LOG_ERROR(FILEINFO, "monster[id=%d] better ai is error", info.id);

			return 0;
		}
	}

	mon->SetStayTime(aiInfo->stayTime);
	mon->SetPhyAttackTime(aiInfo->atkTime);
	GetSectorInPos(moveVec, xPos, yPos, aiInfo->moveDistance);
	mon->SetMoveSector(moveVec);

	Smart_Ptr<AIBrain> monBrain = mon->GetBrain();
	if(!monBrain)
		return 0;

	monBrain->Init(aiInfo, betterAI);
	INTVMPNSTERMAP::iterator itNewMon = m_allMonster.find(info.id);
	if(itNewMon == m_allMonster.end())
	{
		vecMonster.push_back(mon);
		m_allMonster[info.id] = vecMonster;
	}
	else
	{
		itNewMon->second.push_back(mon);
	}

	m_allObject[mon->GetKey()] = mon;

	return mon;
}

Smart_Ptr<Monster> CGameZone::CreateMonsterDymic(DWORD monsterId,DWORD xPos,DWORD yPos, int hp, Smart_Ptr<Player> owner)
{
	if (monsterId == 0)
		return NULL;

	MonsterInfo info;

	MonsterLoader::GetInstance()->GetMonsterInfo(monsterId, info);
	if(info.id == -1)
	{
		LOG_ERROR(FILEINFO, "scene create monster but monster id is null");
		return NULL;
	}

	vector<Smart_Ptr<Monster> > vecMonster;
	Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eMonster);
	Smart_Ptr<Monster> mon = obj;

	mon->SetID(info.id);
	mon->SetName(info.name);
	mon->getAttManage()->setValue(eCharLv,info.lv);
	mon->SetQuality(info.quality);
	mon->SetFlushTime(0);				//副本BOSS不刷新
	mon->SetAliveTime(info.aliveTime);
	mon->getAttManage()->setValue(eCharCountry,info.camp);
	mon->getAttManage()->setValue(eCharMoveSpeed,info.idleSpeed);
	mon->SetRunSpeed(info.moveSpeed);
//	mon->SetBaseAi(info.baseAI);
//	mon->SetBetterAi(info.betterAI);
	mon->SetFallID(info.fallID);
	mon->getAttManage()->setValue(eCharExp,info.exp);
	mon->getAttManage()->setValue(eCharPhyAttackMax,info.maxAttack);
	mon->getAttManage()->setValue(eCharPhyDefence,info.phyDefence);
	mon->getAttManage()->setValue(eCharHitRate,info.hitRate);
	mon->getAttManage()->setValue(eCharAvoidRate,info.avoidRate);
	mon->getAttManage()->setValue(eCrackDownRate,info.crackRate);
	mon->getAttManage()->setValue(eAvoidCrackRate,info.avoidCrack);
	mon->SetHPMax(info.maxHP);
	mon->SetCurHP(hp);
	mon->SetMPMax(info.maxMP);
	mon->SetCurMP(info.maxMP);
	mon->SetOwnerToBrain(mon);
	mon->SetMapID(m_sceneID);
	mon->SetBornTime();
	mon->SetOwnerName("");

	//任务剧情刷新的怪，属于某个具体的玩家
	if(owner && info.quality == eNpcChangeToMonster)
	{
		mon->SetOwnerName(owner->GetName());
		mon->SetOwner(owner);
	}

	//镖车
	if(owner && info.quality == eEscortCar)
	{
		mon->SetOwnerName(owner->GetName());
		mon->SetOwner(owner);
		mon->getAttManage()->setValue(eCharCountry,owner->getAttManage()->getValue<DWORD>(eCharCountry));
	}

	int index = rand() % 4;

	mon->SetXPos(xPos);
	mon->SetYPos(yPos);
	mon->SetOriginalX(xPos + index);
	mon->SetOriginalY(yPos + index);
//	mon->SetGroup(GetSectorGroupIndex(xPos, yPos));

	Join(obj);

	vector<Sector *> moveVec;
	moveVec.clear();
	BaseAIInfo *aiInfo = 0;
	BetterAIInfo *betterAI = 0;

	aiInfo = AILoader::GetInstance()->GetBaseAIByid(info.baseAI);
	if(!aiInfo)
	{
		LOG_ERROR(FILEINFO, "monster[id=%d] ai is error", info.id);

		return 0;
	}

	if(info.betterAI > 0)
	{
		betterAI = AILoader::GetInstance()->GetBetterAIByid(info.betterAI);
		if (!betterAI)
		{
			LOG_ERROR(FILEINFO, "monster[id=%d] better ai is error", info.id);

			return 0;
		}
	}

	mon->SetStayTime(aiInfo->stayTime);
	mon->SetPhyAttackTime(aiInfo->atkTime);
	GetSectorInPos(moveVec, xPos, yPos, aiInfo->moveDistance);
	mon->SetMoveSector(moveVec);

	Smart_Ptr<AIBrain> monBrain = mon->GetBrain();
	if(!monBrain)
		return 0;

	monBrain->Init(aiInfo, betterAI);
	INTVMPNSTERMAP::iterator itNewMon = m_allMonster.find(info.id);
	if(itNewMon == m_allMonster.end())
	{
		vecMonster.push_back(mon);
		m_allMonster[info.id] = vecMonster;
	}
	else
	{
		itNewMon->second.push_back(mon);
	}

	m_allObject[mon->GetKey()] = mon;

	return mon;
}

void CGameZone::GetBeginPosLine(int beginX,int beginY,int endX,int endY,int& resultX,int& resultY,int distance)
{
	FieldInfo* tempInfo = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(tempInfo == NULL)
		return ;

	if(beginX == endX && beginY == endY)
		return ;

	if(beginX < 0 || beginY < 0 || beginX >= tempInfo->GetXMaxNum() || beginY >= tempInfo->GetYMaxNum())
		return ;

	int tempXLenght = endX - beginX;
	int tempYLenght = endY - beginY;

	//斜率是否在-1~1区间
	bool tempIsLess = false;
	//斜率，下面用于求值，对应tan和ctan
	float tempSlope = .0f;

	if(tempXLenght == 0)
	{
		tempSlope  = 0;
		tempIsLess = false;
	}
	else if(tempYLenght == 0)
	{
		tempSlope  = 0;
		tempIsLess = true;
	}
	else
	{
		//tempXLenght不能为0，在－45度和正45度之间，包括135到225之间
		tempSlope = (float)tempYLenght / (float)tempXLenght;
		if(tempSlope >= -1 && tempSlope <= 1)
		{
			tempIsLess = true;
		}
		else
		{
			tempSlope  = (float)tempXLenght / (float)tempYLenght;
			tempIsLess = false;
		}
	}

	int tempLenght = 0;
	int tempIndexX = 0;
	int tempIndexY = 0;
	//递增方向
	int tempIncreaseDir    = 1;

	//X方向
	if(tempIsLess)
	{
		if(endX > beginX)
			tempIncreaseDir = 1;
		else
			tempIncreaseDir = -1;

		tempLenght = abs(endX - beginX);

		for (int i = 0;i <= tempLenght;++i)
		{
			//点处于格子中间，取整后得到索引，所以+0.5
			float tempIncrease = i * tempIncreaseDir + 0.5f;

			tempIndexX = (int)(beginX + tempIncrease);
			tempIndexY = (int)(beginY + tempIncrease * tempSlope);

			if(tempIndexX >= tempInfo->GetXMaxNum() || tempIndexY >= tempInfo->GetYMaxNum())
				return ;

			if(tempIndexX < 0 || tempIndexY < 0)
				return ;

			//先算出Y的长度，再算出递增长度，tempIncrease为X长度
			float tempYLenght = tempIncrease * tempSlope;
			tempYLenght       = tempIncrease * tempIncrease + tempYLenght * tempYLenght;
			if(tempYLenght > distance * distance)
					return ;

			//格子索引
			int tempTotalIndex = tempIndexY * tempInfo->GetXMaxNum() + tempIndexX;
			if(!m_totalSector[tempTotalIndex].GetSectorFlag())
				return ;

			resultX = tempIndexX;
			resultY = tempIndexY;
		}
	}
	//y方向
	else
	{
		if(endY > beginY)
			tempIncreaseDir = 1;
		else
			tempIncreaseDir = -1;

		tempLenght = abs(endY - beginY);

		for (int i = 0;i <= tempLenght;++i)
		{
			//点处于格子中间，取整后得到索引，所以+0.5
			float tempIncrease = i * tempIncreaseDir + 0.5f;

			tempIndexY = (int)(beginY + tempIncrease);
			tempIndexX = (int)(beginX + tempIncrease * tempSlope);

			if(tempIndexX >= tempInfo->GetXMaxNum() || tempIndexY >= tempInfo->GetYMaxNum())
				return ;

			if(tempIndexX < 0 || tempIndexY < 0)
				return ;

			//算递增的长度
			float tempXLenght = tempIncrease * tempSlope;
			tempXLenght       = tempIncrease * tempIncrease + tempXLenght * tempXLenght;
			if(tempXLenght > distance * distance)
					return ;

			//格子索引
			int tempTotalIndex = tempIndexY * tempInfo->GetXMaxNum() + tempIndexX;
			if(!m_totalSector[tempTotalIndex].GetSectorFlag())
				return ;

			resultX = tempIndexX;
			resultY = tempIndexY;
		}
	}

	return ;
}

void CGameZone::GetFinalPosLine(int bxpos, int bypos, int expos, int eypos, int &xpos, int &ypos, int distance)
{
	FieldInfo* tempInfo = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(tempInfo == NULL)
		return ;

	if(bxpos == expos && bypos == eypos)
		return ;

	if(bxpos < 0 || bypos < 0 || bxpos >= tempInfo->GetXMaxNum() || bypos >= tempInfo->GetYMaxNum())
		return ;

	int tempXLenght = expos - bxpos;
	int tempYLenght = eypos - bypos;

	//斜率是否在-1~1区间
	bool tempIsLess = false;
	//斜率，下面用于求值，对应tan和ctan
	float tempSlope = .0f;
	int desXPos = 0;
	int desYPos = 0;

	if(tempXLenght == 0)
	{
		tempSlope  = 0;
		tempIsLess = false;
	}
	else if(tempYLenght == 0)
	{
		tempSlope  = 0;
		tempIsLess = true;
	}
	else
	{
		//tempXLenght不能为0，在－45度和正45度之间，包括135到225之间
		tempSlope = (float)tempYLenght / (float)tempXLenght;
		if(tempSlope >= -1 && tempSlope <= 1)
		{
			desXPos = (int)sqrt((distance*distance)/(tempSlope*tempSlope+1)) + expos;
			desYPos = (int)tempSlope*(desXPos-expos) + eypos;

			tempIsLess = true;
		}
		else
		{
			tempSlope  = (float)tempXLenght / (float)tempYLenght;
			desYPos = (int)sqrt((distance*distance)/(tempSlope*tempSlope+1)) + eypos;
			desXPos = (int)tempSlope*(desYPos-eypos) + expos;

			tempIsLess = false;
		}
	}

	int tempLenght = 0;
	int tempIndexX = 0;
	int tempIndexY = 0;

	//递增方向
	int tempIncreaseDir    = 1;

	//X方向
	if(tempIsLess)
	{
		if(expos > bxpos)
			tempIncreaseDir = 1;
		else
			tempIncreaseDir = -1;

		tempLenght = abs(desXPos - expos);

		for (int i = 0;i <= tempLenght;++i)
		{
			//点处于格子中间，取整后得到索引，所以+0.5
			float tempIncrease = i * tempIncreaseDir + 0.5f;

			tempIndexX = (int)(expos + tempIncrease);
			tempIndexY = (int)(eypos + tempIncrease * tempSlope);

			if(tempIndexX >= tempInfo->GetXMaxNum() || tempIndexY >= tempInfo->GetYMaxNum())
				return ;

			if(tempIndexX < 0 || tempIndexY < 0)
				return ;

			//算递增的长度
			float tempLenght = tempIncrease * tempSlope;
			tempLenght       = tempIncrease * tempIncrease + tempLenght * tempLenght;
			if(tempLenght > distance * distance)
					return ;

			//格子索引
			int tempTotalIndex = tempIndexY * tempInfo->GetXMaxNum() + tempIndexX;
			if(!m_totalSector[tempTotalIndex].GetSectorFlag())
				return ;

			xpos = tempIndexX;
			ypos = tempIndexY;
		}
	}
	//y方向
	else
	{
		if(eypos > bypos)
			tempIncreaseDir = 1;
		else
			tempIncreaseDir = -1;

		tempLenght = abs(desYPos - eypos);

		for (int i = 0;i <= tempLenght;++i)
		{
			//点处于格子中间，取整后得到索引，所以+0.5
			float tempIncrease = i * tempIncreaseDir + 0.5f;

			tempIndexY = (int)(eypos + tempIncrease);
			tempIndexX = (int)(expos + tempIncrease * tempSlope);

			if(tempIndexX >= tempInfo->GetXMaxNum() || tempIndexY >= tempInfo->GetYMaxNum())
				return ;

			if(tempIndexX < 0 || tempIndexY < 0)
				return ;

			//算递增的长度
			float tempLenght = tempIncrease * tempSlope;
			tempLenght       = tempIncrease * tempIncrease + tempLenght * tempLenght;
			if(tempLenght > distance * distance)
					return ;

			//格子索引
			int tempTotalIndex = tempIndexY * tempInfo->GetXMaxNum() + tempIndexX;
			if(!m_totalSector[tempTotalIndex].GetSectorFlag())
				return ;

			xpos = tempIndexX;
			ypos = tempIndexY;
		}
	}
}

void CGameZone::IsActiveWorldBoss(int key, vector<pair<int,pair<int64,int> > > &isActiveList, int playerLevel)
{
	INTOBJECTMAP::iterator itObj = m_allObject.find(key);
	if(itObj != m_allObject.end())
	{
		if(itObj->second->GetType() == eMonster)
		{
			Smart_Ptr<Monster> monster = itObj->second;
			if(monster )
			{
				MonsterInfo info;
				MonsterLoader::GetInstance()->GetMonsterInfo(monster->GetID(), info);
				if (!(monster->IsDead()) && -1 != info.id &&
						(playerLevel == 0 || playerLevel >= info.lv))
				{
					pair<int, pair<int,int> > key_monster;
					pair<int,int> key_value;
					key_value.first = monster->GetID();
					key_value.second = monster->IsDead();
					key_monster.first = monster->GetKey();
					key_monster.second = key_value;
					isActiveList.push_back(key_monster);
				}
			}
		}
	}
}

int CGameZone::GetAllMonsterCount()
{
	int nCount = 0;

	INTVMPNSTERMAP::iterator itrEnd = m_allMonster.end();
	for(INTVMPNSTERMAP::iterator itr = m_allMonster.begin(); itr != itrEnd ; ++itr)
	{
		vector<Smart_Ptr<Monster> >::iterator vitrEnd = itr->second.end();
		for(vector<Smart_Ptr<Monster> >::iterator vitr = itr->second.begin(); vitr != vitrEnd; ++vitr)
		{
			++ nCount;
		}
	}

	return nCount;
}

bool CGameZone::IsAllMonsterDead()
{
	bool bResult = true;

	INTVMPNSTERMAP::iterator itrEnd = m_allMonster.end();
	for(INTVMPNSTERMAP::iterator itr = m_allMonster.begin(); itr != itrEnd ; ++itr)
	{
		vector<Smart_Ptr<Monster> >::iterator vitrEnd = itr->second.end();
		for(vector<Smart_Ptr<Monster> >::iterator vitr = itr->second.begin(); vitr != vitrEnd; ++vitr)
		{
			if((*vitr)->GetAIState() != eAIStateType_Dead)
			{
				bResult = false;
				break;
			}
		}
	}

	return bResult;
}

Smart_Ptr<Monster> CGameZone::GetMonsterByID(int id)
{
	INTVMPNSTERMAP::iterator it = m_allMonster.find(id);
	if(it != m_allMonster.end())
	{
		vector<Smart_Ptr<Monster> >::iterator itVec = it->second.begin();
		if(itVec != it->second.end())
		{
			return (*itVec);
		}
	}

	return 0;
}

int CGameZone::SendObjectToGroup(Smart_Ptr<Object> & obj)
{
	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	int sectorIndex = GetSectorIndex(obj->GetXPos(), obj->GetYPos());
	if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
	{
		LOG_ERROR(FILEINFO, "sectorindex is invalid");

		return -1;
	}

	int groupIndex = GetSectorGroupIndex(obj->GetXPos(), obj->GetYPos());
	if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
	{
		LOG_ERROR(FILEINFO, "groupindex is invalid");

		return -1;
	}

	switch(obj->GetType())
	{
	case ePlayer:
		{
			SectorGroup * pGroup = &m_totalGroup[groupIndex];

			pGroup->SendAllObjectToGroup(obj, 0);

			break;
		}
	case eMonster:
		{
			SectorGroup * pGroup = &m_totalGroup[groupIndex];

			pGroup->SendAllObjectToGroup(obj, 0);

			break;
		}
	case eDropItem:
	{
		SectorGroup * pGroup = &m_totalGroup[groupIndex];

		pGroup->SendAllObjectToGroup(obj, 0);

		break;
	}
	case ePet:
		{
			SectorGroup * pGroup = &m_totalGroup[groupIndex];

			pGroup->SendAllObjectToGroup(obj, 0);
			break;
		}
	default:
		{
			LOG_ERROR(FILEINFO, "unknown object type[%d] join zone", obj->GetType());

			return -1;
		}
	}

	return 0;
}

void CGameZone::SetMonsterDead(int id)
{
	vector<Smart_Ptr<Monster> > vec;
	INTVMPNSTERMAP::iterator it = m_allMonster.find(id);
	if(it != m_allMonster.end())
	{
			vec = it->second;
	}

	vector<Smart_Ptr<Monster> >::iterator itVec = vec.begin();
	for(;itVec != vec.end(); ++itVec)
	{
		(*itVec)->SetDead(NULL);
	}
}

void CGameZone::SetAllMonsterDead()
{
	vector<int> nId;
	INTVMPNSTERMAP::iterator it = m_allMonster.begin();
	for(; it != m_allMonster.end();++it)
	{
			nId.push_back(it->first);
	}

	for(size_t i = 0; i < nId.size(); i++)
	{
		SetMonsterDead(nId[i]);
	}
}

void CGameZone::AddObject(int nKey,Smart_Ptr<Object> object)
{
	m_allObject[nKey] = object;
}

void CGameZone::AddMonster(int id, vector<Smart_Ptr<Monster> > & vec)
{
	m_allMonster[id] = vec;
}



