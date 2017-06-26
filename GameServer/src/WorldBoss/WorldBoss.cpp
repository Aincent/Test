/*
 * WorldBoss.cpp
 *
 *  Created on: 2014年7月21日
 *      Author: root
 */

#include <vector>
#include "WorldBoss.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Map/SceneManager.h"
#include "../Map/GameZone.h"
#include "MessageCommonRet.h"
#include "Log/Logger.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "MessageStruct/WorldBoss/WorldBoss.pb.h"
#include "../Object/Player/Player.h"
#include "../Object/AI/AIBrain.h"
#include "../ServerEventDefine.h"
#include "../Attribute/AttributeCreature.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "../MapCarbon/WorldBossCarbon.h"
#include "MessageStruct/WorldBoss/WorldBossDuplicate.pb.h"

WorldBossMgr::WorldBossMgr():m_nCoolTime(0)
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(WorldBossMgr, MSG_REQ_C2GT_WORLDBOSS);
	DEF_MSG_QUICK_REQUEST_REG_FUN(WorldBossMgr, MSG_REQ_C2GS_WORLDBOSS_FITCH);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_WORLDBOSS_UIHLSTATE);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_NOTICEBOSS_OPENCNT);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_W2GM_WORLDBOSS_OVER);
	DEF_MSG_QUICK_REQUEST_REG_FUN(WorldBossMgr, MSG_REQ_C2GM_WORLDBOSS_DAMAGE_LIST);

	m_monsterReflushEvent = RegGlobalEventPtr(MONSTER_REFLUSH,this,&WorldBossMgr::HandleMonsterReflush);

	m_monsterDeadEvent    = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE,this,&WorldBossMgr::HandleMonsterDead);

	m_pLua.OpenLua();

	LoadConfig();
}

WorldBossMgr::~WorldBossMgr()
{
	m_pLua.CloseLua();
}

void WorldBossMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_WORLDBOSS_UIHLSTATE);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_NOTICEBOSS_OPENCNT);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_W2GM_WORLDBOSS_OVER);
	DEF_SWITCH_TRY_DISPATCH_END

}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(WorldBossMgr,MSG_REQ_C2GT_WORLDBOSS)
{
	WorldBoss::WorldBossInfoList toClient;

	if(!player)
	{
		return;
	}

	map<int, pair<int, int64> > worldBossZone = CSceneMgr::GetInstance()->getWorldBossZone();
	map<int, pair<int, int64> >::iterator it = worldBossZone.begin();
	for(; it != worldBossZone.end(); it++)
	{
		pair<int, int64> worldBoss = it->second;
		Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(worldBoss.second);
		if(zone)
		{
			Smart_Ptr<Monster> monster = zone->GetObjectByKey(it->first);
			if(!monster)
			{
				continue;
			}

			WorldBoss::SingleWorldBossInfo* tSingle = toClient.add_boosinfolist();
			int64 nextReflushTime = 0;
			if(eAIStateType_Dead == monster->GetAIState())
			{
				nextReflushTime = monster->GetBrain()->getLeftTime();
				nextReflushTime += CUtil::GetNowSecond();
			}
			tSingle->set_bosskey(monster->GetKey());
			tSingle->set_isactive(!monster->IsDead());
			tSingle->set_bossid(monster->GetID());
			tSingle->set_reflushtime(nextReflushTime);

			map<int,sAttackBossInfo>::iterator tInIt = m_bossDemageInfo.find(monster->GetID());
			if(tInIt == m_bossDemageInfo.end())
			{
				continue;
			}

			tSingle->set_playerid(tInIt->second.playerID);
			tSingle->set_playername(tInIt->second.name);
			tSingle->set_viplevel(tInIt->second.vipLevel);
		}
		else
		{
			LOG_ERROR(FILEINFO, "get the all world boss but zone is not exsit");
		}
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GT_WORLDBOSS);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(WorldBossMgr,MSG_REQ_C2GS_WORLDBOSS_FITCH)
{
	InstanceMapEntry* pInstance = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
	if(pInstance==NULL || pInstance->m_eType != eWorldBossInstance)
	{
		return;
	}

	CGameZone *pZone = player->GetGameZone();
	if(pZone == NULL)
	{
		return;
	}

	int ret = ((WorldBossCarbon*)pZone)->Fitch(player);

	WorldBossDuplicate::AckReqFitPrize ack;
	ack.set_ret(ret);
	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GS_WORLDBOSS_FITCH);
}

int WorldBossMgr::WorldBossMgr_IsHaveAliveBoss(Smart_Ptr<Player>& player)
{
	vector<pair<int,pair<int64,int> > > isActiveList;
	CSceneMgr::GetInstance()->GetAllWorldBossInfo(isActiveList, player);
	return isActiveList.size();
}

bool WorldBossMgr::HandleMonsterReflush(const EventArgs& e)
{
	const MonsterReflushArgs& data = static_cast<const MonsterReflushArgs& >(e);
	if(!data.m_Monster)
		return true;

	if(data.m_Monster->GetQuality() != eWorldMonster)
		return true;

	UpdateWorldBossUIData(data.m_Monster->GetID(), true);

	return true;
}

bool WorldBossMgr::HandleMonsterDead(const EventArgs & e)
{
	const TwoParmaEventArgs& tData = static_cast<const TwoParmaEventArgs& >(e);
	Smart_Ptr<Monster> tMonster = tData.m_myself;

	if(tMonster.Get()==NULL || tMonster->GetQuality() != eWorldMonster || tData.m_other->GetType() != ePlayer)
	{
		return true;
	}

	Smart_Ptr<Player> tPlayer = tData.m_other;

	sAttackBossInfo tInfo;
	tInfo.name     = tPlayer->GetName();
	tInfo.playerID = tPlayer->GetID();
	tInfo.vipLevel = tPlayer->getAttManage()->getValue<int>(eVIPLv);

	m_bossDemageInfo[tMonster->GetID()] = tInfo;

	return true;
}

void WorldBossMgr::UpdateWorldBossUIData(int monsterID, bool flag)
{
	MonsterInfo info;
	MonsterLoader::GetInstance()->GetMonsterInfo(monsterID, info);
	if (-1 == info.id)
	{
		return;
	}
	const INT64PLAYERMAP& players = PlayerMessage::GetInstance()->getAllPlayer();
	for (INT64PLAYERMAP::const_iterator it = players.begin(); it != players.end(); ++it)
	{
		if ((bool)it->second && it->second->IsOpenSystem(eStWildBoss) &&
				it->second->getAttManage()->getValue<int>(eCharLv) >= info.lv)
		{
			it->second->UpdateUIHighLight(eUIHLHandle_UpAndForceSyn, UIHLType(eUIHLBig_TroubledTimes, eUIHLSmall_TroubledTimes_BossCanBattle), 1, flag);
		}
	}
}

void WorldBossMgr::LoadConfig()
{
	LoadPrize();
}

void WorldBossMgr::LoadPrize()
{
	std::string strFileFullPath;
    if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return;
	}

	string strFile = strFileFullPath + "Data/WorldBoss.csv";
	CSVLoader loader;

	if(!loader.OpenFromFile(strFile.c_str()))
	{
		return;
	}

	//地图id	类型	名字    名字    等级   人数限制	自动领取时间(毫秒)	掉落信息	最后一击奖励	伤害排名奖励
	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		 int count = 0;
		 WorldBossConfig info;
		 info.mapid  = loader.GetInt(i, count++); //地图id
//		 info.m_Type = static_cast<eBossType>(loader.GetInt(i, count++));//类型
		 loader.GetString(i, count++);//index
		 info.countLimit = loader.GetInt(i,count++); //人数限制
//		 info.fitchTime = loader.GetInt(i,count++);  //自动领取时间
		 m_nCoolTime = loader.GetInt(i,count++);//冷脚时间

		 loader.GetString(i, count++);//掉落信息

		  //最后一击奖励
		 std::string strAttactPrize = loader.GetString(i, count++);
		 StrArgPkg strPkg("|", strAttactPrize);
		 for(size_t n=0; n<strPkg.size(); ++n)
		  {
			 std::string strTemp = getArg(strPkg,n);
			 StrArgPkg strTempPkg(":", strTemp);
			 if(strTempPkg.size() != 2)
			  {
				 return;
			  }
			 PropItem item;
			 item.id = atoi(getArg(strTempPkg,0).c_str());
			 item.num = atoi(getArg(strTempPkg,1).c_str());
			 info.attackPrize.push_back(item);
		  }

		 //排名奖励
		 std::string strSortPrize = loader.GetString(i, count++);
		 StrArgPkg strSortPkg("|", strSortPrize);
		 for(size_t n=0; n<strSortPkg.size(); ++n)
		  {
			 PVPSortPrize prize;

			 std::string strTemp = getArg(strSortPkg,n);
			 StrArgPkg strTempPkg("_", strTemp);
			 for(size_t k=0; k<strTempPkg.size(); ++k)
			  {
				 	std::string strTemp2 = getArg(strTempPkg,k);
				  StrArgPkg strTempPkg2(":", strTemp2);

				  if(strTempPkg2.size() != 2)
				  	 {
					  return;
				  	 }

				 	 //名次
				 	if(k==0)
				 	{
				 		prize.min = atoi(getArg(strTempPkg2,0).c_str());
				 		prize.max = atoi(getArg(strTempPkg2,1).c_str());
				 	}
				 	else//奖励
				 	{
				 		PropItem item;
				 		item.id  = atoi(getArg(strTempPkg2,0).c_str());
				 		item.num = atoi(getArg(strTempPkg2,1).c_str());
				 		prize.m_items.push_back(item);
				 	}
			  }

			 info.sortPrize.push_back(prize);
		  }

		StrArgPkg tPartakePkg("|:",loader.GetString(i,count++));
		for(size_t z = 0;z < tPartakePkg.size();++z)
		{
			PropItem tPartItem;
			tPartItem.id  = atoi(getArg(tPartakePkg[z],0).c_str());
			tPartItem.num = atoi(getArg(tPartakePkg[z],1).c_str());
			tPartItem.bind = atoi(getArg(tPartakePkg[z],2).c_str());
			info.m_PartakePrize.push_back(tPartItem);	
		}

		info.m_UpdateTime.m_BossID = loader.GetInt(i,count++);  
		info.m_UpdateTime.m_Time = loader.GetInt(i,count++);  

		StrArgPkg tBossPosPkg("|",loader.GetString(i,count++));
		info.m_UpdateTime.m_X = atoi(getArg(tBossPosPkg,0).c_str());
		info.m_UpdateTime.m_Y = atoi(getArg(tBossPosPkg,1).c_str());

		 std::string strBlood = loader.GetString(i, count++);
		 StrArgPkg strBloodPkg("|", strBlood);
		 for(size_t n=0; n<strBloodPkg.size(); ++n)
		  {
				m_BossBloodPercents.push_back(atoi(getArg(strBloodPkg,n).c_str()));
		  }

		m_mapConfigInfo[info.mapid] = info;
	}

}


int WorldBossMgr::GetCoolTime(int count)
{
	return m_nCoolTime * count;
}

//根据排名得到奖励
void WorldBossMgr::GetSortPrize(int bossID, int rank, std::vector<PropItem>& items)
{
	std::map<int,WorldBossConfig>::const_iterator it = m_mapConfigInfo.begin();
	for(;it != m_mapConfigInfo.end();++it)
	{
		if(it->second.m_UpdateTime.m_BossID != bossID)
		{
			continue;
		}
		
		bool tIsSort = false;
		for(size_t i=0; i<it->second.sortPrize.size(); ++i)
		{
			if(rank >= it->second.sortPrize[i].min && rank <= it->second.sortPrize[i].max)
			{
				items.insert(items.end(), it->second.sortPrize[i].m_items.begin(), it->second.sortPrize[i].m_items.end());
				tIsSort = true;
				break;
			}
		}	

		if(!tIsSort)	
		{
			items.insert(items.end(),it->second.m_PartakePrize.begin(),it->second.m_PartakePrize.end());	
		}

		break;
	}
	
	return ;
}

//根据最后一击得到奖励
void WorldBossMgr::GetLastAttackPrize(int bossID, std::vector<PropItem>& items)
{
	std::map<int,WorldBossConfig>::const_iterator it = m_mapConfigInfo.begin();
	for(;it != m_mapConfigInfo.end();++it)
	{
		if(it->second.m_UpdateTime.m_BossID != bossID)
		{
			continue;
		}
		
		items.insert(items.end(), it->second.attackPrize.begin(), it->second.attackPrize.end());
		break;
	}
	
	return ;
}

int WorldBossMgr::GetMaxPlayer(int mapid)
{
	map<int,WorldBossConfig>::iterator tIt = m_mapConfigInfo.begin();
	for(;tIt != m_mapConfigInfo.end();++tIt)
	{
//		if(tIt->second.m_Type != eBossWorldType)
//		{
//			continue;
//		}

		return tIt->second.countLimit;
	}

	return 0; 
}

int WorldBossMgr::getBossUpdateTime(int bossID)
{
	map<int,WorldBossConfig>::iterator tIt = m_mapConfigInfo.begin();
	for(;tIt != m_mapConfigInfo.end();++tIt)
	{
		if(tIt->second.m_UpdateTime.m_BossID != bossID)
		{
			continue;
		}
		return tIt->second.m_UpdateTime.m_Time;
	}

	return 0;
}

const WorldBossConfig* WorldBossMgr::getOneConfig(int bossID)
{
	map<int,WorldBossConfig>::iterator tIt = m_mapConfigInfo.begin();
	for(;tIt != m_mapConfigInfo.end();++tIt)
	{
		if(tIt->second.m_UpdateTime.m_BossID != bossID)
		{
			continue;
		}

		return &tIt->second;
	}

	return NULL;
}

DEF_MSG_SIMPLE_DEFINE_FUN(WorldBossMgr, MSG_SIM_WS2GM_WORLDBOSS_UIHLSTATE)
{
	WorldBossDuplicate::WS2GMBossUIHLState req;
	int len = 0;
	char* buffer = message->GetBuffer(len);
	req.ParseFromArray(buffer, len);

	InstanceMapEntry* info = InstanceLoader::GetInstance()->GetEntry(req.mapid());
	if (NULL == info)
	{
		return;
	}

	const INT64PLAYERMAP& players = PlayerMessage::GetInstance()->getAllPlayer();
	for (INT64PLAYERMAP::const_iterator it = players.begin(); it != players.end(); ++it)
	{
		if (it->second->IsOpenSystem(eStWorldBoss) && 0 < req.num() &&
				it->second->getAttManage()->getValue<int>(eCharLv) >= (int)(info->m_nLevel))
		{
			it->second->UpdateUIHighLight(eUIHLHandle_UpAndForceSyn,
					UIHLType(eUIHLBig_TroubledTimes, eUIHLSmall_TroubledTimes_WorldBoss), req.num(), req.flag());
		}
	}

}

DEF_MSG_SIMPLE_DEFINE_FUN(WorldBossMgr, MSG_SIM_WS2GM_NOTICEBOSS_OPENCNT)
{
	ServerReturn::ServerRetInt32Array ret;
	int len = 0;
	char* buffer = message->GetBuffer(len);

	ret.ParseFromArray(buffer, len);

	int64 charID = message->GetMessageTime();
	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharId(player, charID);

	if (player)
	{
		int worldBossOpenCnt = 0;
		for (int i = 0; i < ret.ret_size(); ++i)
		{
			int mapID = ret.ret(i);
			InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(mapID);
			if (NULL == entry)
			{
				continue;
			}
			if ((int)(entry->m_nLevel) <= player->getAttManage()->getValue<int>(eCharLv))
			{
				++worldBossOpenCnt;
			}
		}
		player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_TroubledTimes, eUIHLSmall_TroubledTimes_WorldBoss), worldBossOpenCnt);
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(WorldBossMgr, MSG_SIM_W2GM_WORLDBOSS_OVER)
{
	ServerReturn::ServerRetInt ret;
	int len = 0;
	char* buffer = message->GetBuffer(len);

	ret.ParseFromArray(buffer, len);	
	Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(ret.ret());
	if(!zone)
	{
		return ;
	}

	if(GET_MAP_TYPE(zone->GetSceneID()) != ePVEMap)
	{
		return ;	
	}

	const InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(zone->GetSceneID()));
	if(entry == NULL)
	{
		return ;
	}

	if(entry && entry->m_eType != eWorldBossInstance)
	{
		return ;
	}

	Smart_Ptr<WorldBossCarbon> tWorldCarbon = zone;
	tWorldCarbon->Win();

	return ;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(WorldBossMgr,MSG_REQ_C2GM_WORLDBOSS_DAMAGE_LIST)
{
	WorldBossDuplicate::ClientReqBossDamage req;
	WorldBossDuplicate::AckReqBossDamage toClient;
	req.ParseFromString(content);

	InstanceMapEntry* pInstance = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
	if(pInstance==NULL || pInstance->m_eType != eWorldBossInstance)
	{
		return;
	}

	CGameZone *pZone = player->GetGameZone();
	if(pZone)
	{
		((WorldBossCarbon*)pZone)->GetBoosDamage(player->GetID(),req.bossid(),toClient);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_WORLDBOSS_DAMAGE_LIST);
}
