/*
 * ProtectKingCarBon.cpp
 *
 *  Created on: 2015年6月30日
 *      Author: root
 */
#include "ProtectKingCarBon.h"
#include "util.h"
#include "Log/Logger.h"
#include "../Object/Player/Player.h"
#include "../Object/ItemDrop/DropItemMgr.h"
#include "../Map/DuplicateMgr.h"
#include "CounterDefine.h"
#include "../ServerEventDefine.h"
#include "MessageStruct/CharLogin/MapInstanceProto.pb.h"
#include "MessageStruct/ServerReturnBool.pb.h"
#include "MessageStruct/Activity/ProtectKing.pb.h"
#include "../QuickMessage.h"
#include "../Object/Player/PlayerMessage.h"
#include "../ServerManager.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "../Clan/ClanLoader/ClanLoader.h"
#include "MessageCommonRet.h"
#include "../Activity/FileLoader/ProtectedKingLoader.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../Object/ObjectFactory.h"
#include "../Object/FileLoader/AILoader.h"
#include "../Activity/ActivityMgr.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../Object/AI/AIBrain.h"
#include "../MapInstance/BeyondBattleMgr.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../Object/ItemDrop/DropItemAlloc.h"
#include "../Chat/ChatMgr.h"
#include "../Container/ArticleEquipMgr.h"
#include "../Country/GsCountryMgr.h"
#include "../Mail/MailMessage.h"
#include "./FileLoader/InternationalLoader.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "FileLoader/ActivitiesTimerLoader.h"
#include <math.h>

CProtectKingCarBon::CProtectKingCarBon(int64 onlyid, int64 charid, int64 paramf, int64 params):CGameDuplicate(onlyid,charid,paramf,params)
{

}

CProtectKingCarBon::~CProtectKingCarBon()
{
	m_monsterDeadEvent.SetNull();
	m_playerDeadEvent.SetNull();
	m_leaveMapEvent.SetNull();

	if(m_reflushTime.IsValid())
	{
		m_reflushTime.SetNull();
	}
}

int CProtectKingCarBon::Init()
{
	int ret = CGameDuplicate::Init();
	return ret;
}

bool CProtectKingCarBon::Update()
{
	switch(m_process)
	{
	case eCreated:
		{
			if(CUtil::GetNowSecond() - m_createTime >= m_data->m_nWrdelTime)
			{//一直没有人就删除副本
				m_process = eOver;
				return false;
			}

			break;
		}
	case ePlayerWork:
		{
			int64 nowTime = CUtil::GetNowSecond();

			if(GetPlayerCount() <= 0)
			{
				Count();
			}

			if(nowTime - m_createTime >= m_data->m_nLimitTime)
			{
				Over();
			}

			break;
		}
	case eWinFight:
		{
			Win();

			break;
		}
	case eFailed:
		{
			Failed();

			break;
		}
	case eAward:
		{
			Award();

			break;
		}
	case eDuplicateChangeMap:
		{
			int64 nowTime = CUtil::GetNowSecond();

			if(GetPlayerCount() <= 0)
			{
				CountEx();
			}

		   if(nowTime - m_createTime >= m_data->m_nLimitTime)
			{
				m_process = eChangeMapWait;
			}

			break;
		}
	case eChangeMapWait:
		{
			int64 nowTime = CUtil::GetNowSecond();

			if(nowTime - m_createTime >= m_data->m_nLimitTime + 10 * 1000)
			{
				//到副本结束时间就强制传送玩家出副本
				//比需要在10秒内传送出去，不然就不管了
				ForceAllPlayerChangeMap();
				m_process = eOver;
			}

			if(GetPlayerCount() <= 0)
			{
				m_process = eOver;
			}

			break;
		}
	case eOver:
		{
			return false;
		}
	default :
		{
			break;
		}
	}

	return true;
}

int CProtectKingCarBon::Join(Smart_Ptr<Object> & obj)
{
	if(!JoinHandle(obj))
		return -1;

	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
	if(!info)
		return -1;

	int sectorIndex = GetSectorIndex(obj->GetXPos(), obj->GetYPos());
	if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
	{
		LOG_ERROR(FILEINFO, "sectorindex is invalid(mapid=%d, xpos=%d, ypos=%d)", (int)GET_MAP_ID(m_sceneID), obj->GetXPos(), obj->GetYPos());

		return -1;
	}

	int groupIndex = GetSectorGroupIndex(obj->GetXPos(), obj->GetYPos());
	if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
	{
		LOG_ERROR(FILEINFO, "groupindex is invalid");

		return -1;
	}

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

			m_player[player->GetID()] = player;
			m_allObject[player->GetKey()] = obj;

			//进入地图前切换玩家的PK模式
			MapStaticInfo * mapInfo = CMapConfig::GetInstance()->GetStaticInfo(m_data->m_nMapID);
			if(mapInfo)
			{
				player->SetPkTypeByMap(mapInfo->pktype);
			}

			SectorGroup * pGroup = &m_totalGroup[groupIndex];

			pGroup->SendAllPlayerToGroup(obj, 0);
			for(int j=0; j<info->GetAllSectorSize(); ++j)
			{
				Sector * tSec = &m_totalSector[j];

				tSec->SendAllMonster(obj);
//						tSec->SendAllPlayer(obj);
			}

			if(player && !(player->GetGameZone()))
				player->SetGameZone(this);

			break;
		}
	case eMonster:
		{
			Smart_Ptr<Monster> monster = obj;

			if(monster->GetQuality() == eWorldMonster)
			{
				PlayerMessage::GetInstance()->UpdateWorldBoss(monster->GetID(), true);
			}

			for(int j=0; j<info->GetAllSectorSize(); ++j)
			{
				Sector * tSec = &m_totalSector[j];

				tSec->SendAllPlayer(obj);
			}

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

bool CProtectKingCarBon::JoinHandle(Smart_Ptr<Object> & obj)
{
	if(m_process >= eDuplicateChangeMap)
		return false;

	vector<int> vec;

	MapInstanceProto::SimInstanceUpdate toClient;

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;


		if(obj->GetType() == ePlayer)
		{
			Smart_Ptr<Monster> mon = GetMonsterByID(ProtectedKingLoader::GetInstance()->GetTargetID());
			if(mon)
			{
				Smart_Ptr<Player> player = obj;

				mon->SetOwnerName(player->GetName());
				mon->getAttManage()->setValue(eCharCountry,player->getAttManage()->getValue<DWORD>(eCharCountry));
			}

			//倒计时10描
			if(m_reflushTime.IsValid())
			{
				m_reflushTime.SetNull();
			}
			m_reflushTime = TimerInterface::GetInstance()->RegTimePoint(&CProtectKingCarBon::FlushMonster, this, 0, 0, 0, 0, ProtectedKingLoader::GetInstance()->GetCountDown());

			if(!m_leaveMapEvent.IsValid())
			{
				m_leaveMapEvent = RegGlobalEventPtr(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, this, &CProtectKingCarBon::HandleLeaveMapEvent);
			}
		}

		if(vec.size() > 0)
		{
			player->SynCharAttribute(vec);
		}

		if(!m_monsterDeadEvent.IsValid())
		{
			m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &CProtectKingCarBon::HandleMonsterDeadEvent);
		}

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		if(m_process == eCreated)
			m_process = ePlayerWork;
	}

	return true;
}

int CProtectKingCarBon::Move(Smart_Ptr<Object> &obj)
{
	switch(obj->GetType())
	{
	case ePlayer:
		{
			Smart_Ptr<Player> player = obj;
			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
			int oldSectroIndex = GetSectorIndex(player->GetXPos(), player->GetYPos());
			int sectorIndex = GetSectorIndex(player->GetXNewPos(), player->GetYNewPos());
			if(!info || sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex in player[%lld] is invalid(mapid=%d, xpos=%d, ypos=%d) when player move",
						player->GetID(),
						(int)GET_MAP_ID(m_sceneID),
						player->GetXNewPos(),
						player->GetYNewPos());

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
				LOG_ERROR(FILEINFO, "groupOldIndex in player is invalid when player move");

				return -1;
			}

			if(groupOldIndex != groupIndex)
			{
				SectorGroup * pGroup = &m_totalGroup[groupIndex];
				SectorGroup * pOldGroup = &m_totalGroup[groupOldIndex];

				pOldGroup->SendPlayerLeaveToGroup(obj, pGroup);
				pGroup->SendAllPlayerToGroup(obj, pOldGroup);

//						player->SetGroup(groupIndex);
//				player->ClearMonsterPath();
			}

			break;
		}
	case eMonster:
		{
			Smart_Ptr<Monster> mon = obj;
			int oldSectroIndex = GetSectorIndex(mon->GetXPos(), mon->GetYPos());
			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
			int sectorIndex = GetSectorIndex(mon->GetXNewPos(), mon->GetYNewPos());
			if(!info || sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex in monster(mapid=%d, x=%d,y%d) is invalid when player move",
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

			break;
		}
	case ePet:
	{

		Smart_Ptr<GirlWar> girlWar = obj;

		int oldSectroIndex = GetSectorIndex(girlWar->GetXPos(), girlWar->GetYPos());
		FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
		int sectorIndex = GetSectorIndex(girlWar->GetXNewPos(), girlWar->GetYNewPos());
		if(!info || sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
		{
			LOG_ERROR(FILEINFO, "sectorindex in monster(mapid=%d,x=%d,y%d) is invalid when player move",
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

void CProtectKingCarBon::SendSynchPath(PlayerSynch::MonsterPathToPlayer &path, Smart_Ptr<Monster> &monster)
{
	if(false == bool(monster))
		return;

	INT64PLAYERMAP::iterator it = m_player.begin();
	for(; it!=m_player.end(); ++it)
	{
		if(it->second.Get()==NULL)
			continue;

		if(monster->GetTargetObj().Get() != NULL)
		{
			path.set_targetkey( monster->GetTargetObj()->GetKey() );
		}

		it->second->SendSynchPath(path);
	}
}


void CProtectKingCarBon::LeaveHandle(Smart_Ptr<Object> & obj)
{
	int64 mapid = 0;
	int xpos = 0;
	int ypos = 0;
	vector<int> vecType;
	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		if(player->getAttManage()->getValue<int>(eCharHP) <= 0)
		{
			CharBattle::RevivalData req;

			//增加地图id和坐标，重置死亡状态，解决客户端显示“使用复活单”的提示bug，是因为玩家回城复活，那么会发两次消息给客户端
			player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));
			player->UnsetCreatureState(ERS_Dead);
			player->SetDataFlag(eBaseInfo);
//					player->SendDeadMessage(eAttackSim);
				//如果不传地图Id就表示原地复活
			map<int64,OldMapInfo>::iterator itMap = m_oldMapPoint.find(player->GetID());
			if(itMap != m_oldMapPoint.end())
			{
				mapid = itMap->second.mapid;
				xpos = itMap->second.xpos;
				ypos = itMap->second.ypos;
			}
			else
			{
				MapStaticInfo *sMapInfo;
				int64 sceneid = 0;

				sMapInfo = CMapConfig::GetInstance()->GetFistMapInfo();
				if (sMapInfo == 0)
					return;

				sceneid = CSceneMgr::GetInstance()->GetSceneIDAboutPlayerIsLoginOn(sMapInfo->mapid);
				if(sceneid <= 0)
				{
					mapid = CREATE_MAP(eStaticMap, 1, 1, sMapInfo->mapid, 1);
				}
				else
				{
					mapid = sceneid;
				}

				xpos = sMapInfo->reliveXPos;
				ypos = sMapInfo->reliveYPos;
			}

			OldMapInfo * pInfo = GetOldMapPoint(player->GetID());
			if(!pInfo || CSceneMgr::GetInstance()->IsHaveScene(pInfo->mapid))
			{
				req.set_mapid(GET_MAP_ID(mapid));
				req.set_xpos(xpos);
				req.set_ypos(ypos);
			}

			req.set_key(player->GetKey());
			req.set_hp(player->getAttManage()->getValue<int>(eCharHP));
			req.set_mp(player->getAttManage()->getValue<int>(eCharMP));

			player->SendSynchMessage(&req, MSG_SIM_GT2C_REVIVAL);
		}

		if(m_params > 0)
		{//令奖励
			int exp = 0;
			int force = 0;
			int money = 0;

			ProtectedKingLoader::GetInstance()->GetNodeReward(m_paramf, m_params - 1, exp, force, money);

			player->AddExp(exp);
			player->ChangeMoney(money, true);
			player->ChangeCurForce(force, true);

			vecType.push_back(eCharExp);
			vecType.push_back(eCharMoney);
			vecType.push_back(eCharForce);

			player->SynCharAttribute(vecType);

			m_params = 0;

//			StatisticInfo::CostAndGotInfo costAngGotInfo;
//			if(exp > 0)
//			{
//				StatisticInfo::GotItemInfo * gotInfo = costAngGotInfo.add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo,eCharExp,exp);
//			}
//
//			if(money > 0)
//			{
//				StatisticInfo::GotItemInfo * gotInfo = costAngGotInfo.add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,exp);
//			}
//
//			if(force > 0)
//			{
//				StatisticInfo::GotItemInfo * gotInfo = costAngGotInfo.add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo,eCharForce,exp);
//			}
//
//			if(eAccountType_User == player->GetAccountType())
//				StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_ProtectKing,eStaMinor_ProtectKing_RewardInCarbon,&costAngGotInfo);
		}
	}
}

void CProtectKingCarBon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);

	switch(obj->GetType())
	{
	case ePlayer:
		{
			Smart_Ptr<Player> player = obj;
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

			if(isDel)
			{
				int groupIndex = GetSectorGroupIndex(player->GetXNewPos(), player->GetYNewPos());
				if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
				{
					LOG_ERROR(FILEINFO, "groupOldIndex in player is invalid when player leave");
					return ;
				}

				SectorGroup * pGroup = &m_totalGroup[groupIndex];

				pGroup->SendPlayerLeaveToGroup(obj, 0);
				for(int j=0; j<info->GetAllSectorSize(); ++j)
				{
					Sector * tSec = &m_totalSector[j];
					tSec->SendMonsterLeave(obj);
				}
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

			break;
		}
	case eMonster:
		{
			Smart_Ptr<Monster> monster = obj;
			int sectorIndex = GetSectorIndex(monster->GetXNewPos(), monster->GetYNewPos());
			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
			if(!info || sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex in monster[%d] is invalid(mapid=%d, xpos=%d, ypos=%d) when monster leave",
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

			if(isDel && !delFromMap)
			{
				for(int j=0; j<info->GetAllSectorSize(); ++j)
				{
					Sector * tSec = &m_totalSector[j];
					tSec->SendPlayerLeave(obj);
				}
			}

			if(delFromMap)
			{
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
							monObj->second.erase(inner++);
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
			if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
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
					LOG_ERROR(FILEINFO, "groupOldIndex in DropItem is invalid when DropItem leave");
					return ;
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
		int sectorIndex = GetSectorIndex(girlWar->GetXNewPos(), girlWar->GetYNewPos());
		FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
		if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
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
				LOG_ERROR(FILEINFO, "groupOldIndex in Pet is invalid when Pet leave");
				return ;
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

	if(GetPlayerCount() <= 0)
	{
		m_monsterDeadEvent.SetNull();
		m_playerDeadEvent.SetNull();
//		m_leaveMapEvent.SetNull();
		m_leaveTime = CUtil::GetNowSecond();
	}
}

void CProtectKingCarBon::LeaveExame(Smart_Ptr<Player> & player)
{
	if(GetPlayerCount() <= 0)
	{
		m_process = eOver;
	}
}

void CProtectKingCarBon::FlushMonster(void * arg)
{
	if(m_process > ePlayerWork)
		return;

	if(m_params > 0 && GetAllMonsterCount() > 1)
	{
		m_process = eFailed;

		{
			ActivityKing::ToClientProtectResult toClient;

			toClient.set_ret(0);

			INT64PLAYERMAP::iterator it = m_player.begin();
			for(; it!=m_player.end(); ++it)
			{
				if(it->second.Get()==NULL)
					continue;
				QuickMessage::GetInstance()->AddSendMessage(it->second->GetID(), it->second->GetChannelID(), &toClient, MSG_SIM_GM2C_PROTECTE_RESULT);
			}
		}

		return;
	}

	Smart_Ptr<Player> player = GetPlayer(m_owner);
	if(player && player->GetActivityMgr())
	{
		int curnode = player->GetActivityMgr()->GetCurNode();
		if(m_paramf <= 0)
		{
				m_paramf = curnode;
				m_params = curnode + 1;

			if(curnode < ProtectedKingLoader::GetInstance()->GetMaxNode())
			{
				FlushMonster(curnode);
			}
		}
	}
}

int CProtectKingCarBon::GetAttriByFactor(int baseAttri,int factor)
{
	if(baseAttri <= 0 || factor <= 0)
		return 0;

	return int(ceil(baseAttri * factor / 100.0f));
}

void CProtectKingCarBon::FlushMonster(int node)
{
	ProtectKingMonster * monster = 0;

	monster = ProtectedKingLoader::GetInstance()->GetKingMonster(node);
	if(monster == 0)
		return;

	vector<ProReflushMonster>::iterator it = monster->monster.begin();
	for(; it!=monster->monster.end(); ++it)
	{
		MonsterInfo info;
		vector<Smart_Ptr<Monster> > vecMonster;

		MonsterLoader::GetInstance()->GetMonsterInfo((*it).id, info);
		if(info.id == -1)
		{
			LOG_ERROR(FILEINFO, "monster id is error");

			continue;
		}

		for(int i=0; i<(*it).count; ++i)
		{
			vector<Pos> vec = ProtectedKingLoader::GetInstance()->GetPos(i);
			if(vec.size() < 2)
			{//小于2代表不能走，就不允许生成
				break;
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
//			mon->SetBaseAi(info.baseAI);
//			mon->SetBetterAi(info.betterAI);
			mon->SetFallID(info.fallID);
			mon->getAttManage()->setValue(eCharExp,info.exp);
			mon->getAttManage()->setValue(eCharPhyAttackMax,GetAttriByFactor(info.maxAttack,(*it).factor));
			mon->getAttManage()->setValue(eCharPhyDefence,GetAttriByFactor(info.phyDefence,(*it).factor));
			mon->getAttManage()->setValue(eCharHitRate,GetAttriByFactor(info.hitRate,(*it).factor));
			mon->getAttManage()->setValue(eCharAvoidRate,GetAttriByFactor(info.avoidRate,(*it).factor));
			mon->getAttManage()->setValue(eCrackDownRate,GetAttriByFactor(info.crackRate,(*it).factor));
			mon->getAttManage()->setValue(eAvoidCrackRate,GetAttriByFactor(info.avoidCrack,(*it).factor));
			mon->SetHPMax(GetAttriByFactor(info.maxHP,(*it).factor));
			mon->SetCurHP(GetAttriByFactor(info.maxHP,(*it).factor));
			mon->SetMPMax(GetAttriByFactor(info.maxMP,(*it).factor));
			mon->SetCurMP(GetAttriByFactor(info.maxMP,(*it).factor));
			mon->SetGatherTime(info.gatherTime);	//采集时间
			mon->SetOwnerToBrain(mon);
			mon->SetMapID(m_sceneID);
			mon->SetBornTime();
			mon->SetOwnerName("");
			mon->GetBrain()->SetMonsterType(eSingleWayPos);
			mon->SetXPos(vec[0].xpos);
			mon->SetYPos(vec[0].ypos);
			mon->SetOriginalX(vec[1].xpos);
			mon->SetOriginalY(vec[1].ypos);
//			mon->SetGroup(GetSectorGroupIndex(vec[0].xpos, vec[0].ypos));

			Join(obj);

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

			Sector * tSecf = 0;
			vector<Pos>::iterator itVec = vec.begin();
			for(; itVec!=vec.end(); ++itVec)
			{
				tSecf = GetSector((*itVec).xpos, (*itVec).ypos);
				if(tSecf)
				{
					moveVec.push_back(tSecf);
				}
			}

			mon->SetMoveSector(moveVec);

			Smart_Ptr<AIBrain> monBrain = mon->GetBrain();
			if(!monBrain)
				continue;

			monBrain->Init(aiInfo, betterAI);
			vecMonster.push_back(mon);
			if(info.quality == eWorldMonster)
			{
				LOG_INFO(FILEINFO,"AddWorldBossToZone monkey =  %d , mapid = %lld  , monid = %lld ", mon->GetKey(),mon->GetMapID(),mon->GetID());
				CSceneMgr::GetInstance()->AddWorldBossToZone(mon->GetKey(), mon->GetMapID(), mon->GetID());
			}

			m_allObject[mon->GetKey()] = mon;
		}

		m_allMonster[info.id] = vecMonster;
	}

	if(m_reflushTime.IsValid())
	{
		m_reflushTime.SetNull();
	}
	m_reflushTime = TimerInterface::GetInstance()->RegTimePoint(&CProtectKingCarBon::FlushMonster, this, 0, 0, 0, 0, monster->finish);
}

bool CProtectKingCarBon::IsCanEnter(Smart_Ptr<Player> & player)
{
	//传送阶段，就不允许进入了
	if(m_process >= eDuplicateChangeMap)
		return false;

	//玩家死了不入传入地图
	if(player->IsDead())
	{
		return false;
	}

	//判断拥有者
	if(player->GetID() != m_owner)
		return false;

	return true;
}

void CProtectKingCarBon::Win()
{
	m_isSuccess = true;
	m_process = eAward;
}

void CProtectKingCarBon::Over()
{
	m_process = eDuplicateChangeMap;
}

void CProtectKingCarBon::Count()
{
	m_process = eOver;
}

void CProtectKingCarBon::CountEx()
{
	int64 nowTime = CUtil::GetNowSecond();

	if(nowTime - m_leaveTime >= m_data->m_nDelTime)
	{
		m_process = eOver;
	}
}

void CProtectKingCarBon::Failed()
{
	m_process = eAward;
}

void CProtectKingCarBon::Award()
{
	m_process = eDuplicateChangeMap;
	m_awardTime = CUtil::GetNowSecond();
}

bool CProtectKingCarBon::HandleLeaveMapEvent(const EventArgs & e)
{
	PlayerRemoveMemberEventArgs *pArgs = (PlayerRemoveMemberEventArgs *) (&e);

	if(pArgs->m_eMethod != GROUP_REMOVEMETHOD_OUT)
		return true;

	if(pArgs->m_pObj.Get()==NULL || pArgs->m_pObj->GetMapID() != m_sceneID)
		return true;

	if(pArgs->m_pObj->GetID() != m_owner)
		return true;

	int64 mapid = 0;
	int xpos = 0;
	int ypos = 0;

	map<int64,OldMapInfo>::iterator itMap = m_oldMapPoint.find(pArgs->m_pObj->GetID());
	if(itMap != m_oldMapPoint.end())
	{
		mapid = itMap->second.mapid;
		xpos = itMap->second.xpos;
		ypos = itMap->second.ypos;
	}
	else
	{
		MapStaticInfo *sMapInfo;
		int64 sceneid = 0;

		sMapInfo = CMapConfig::GetInstance()->GetFistMapInfo();
		if (sMapInfo == 0)
			return false;

		sceneid = CSceneMgr::GetInstance()->GetSceneIDAboutPlayerIsLoginOn(sMapInfo->mapid);
		if(sceneid <= 0)
		{
			mapid = CREATE_MAP(eStaticMap, 1, 1, sMapInfo->mapid, 1);
		}
		else
		{
			mapid = sceneid;
		}

		xpos = sMapInfo->reliveXPos;
		ypos = sMapInfo->reliveYPos;
	}

	pArgs->m_pObj->SetMapID(mapid);
	pArgs->m_pObj->SetXPos(xpos);
	pArgs->m_pObj->SetYPos(ypos);
	pArgs->m_pObj->SetXNewPos();
	pArgs->m_pObj->SetYNewPos();
//	pArgs->m_pObj->SetGroup(CSceneMgr::GetInstance()->GetSectorGroup(pArgs->m_pObj->GetMapID(), pArgs->m_pObj->GetXPos(), pArgs->m_pObj->GetYPos()));
	pArgs->m_pObj->SetDataFlag(eBaseInfo);

	return true;
}

bool CProtectKingCarBon::HandleMonsterDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);

	if(tArgs->m_myself.Get()==NULL || GetSceneID() != tArgs->m_myself->GetMapID())
		return true;

	Smart_Ptr<Monster> mon = tArgs->m_myself;
	LOG_DEBUG(FILEINFO, "CGameDuplicate::HandleMonsterDeadEvent \t %lld", mon->GetID());

	int isSuc = -1;
	if(mon->GetID() == ProtectedKingLoader::GetInstance()->GetTargetID())
	{
		m_process = eFailed;
		isSuc = 0;
	}
	else
	{
		if(GetAllMonsterCount() <= 2)
		{
			//就剩秦王一个了，就表示通关了
			isSuc = 1;
			if(m_reflushTime.IsValid())
			{
				m_reflushTime.SetNull();
			}

			Smart_Ptr<Player> player = GetPlayer(m_owner);
			if(player && player->GetActivityMgr())
			{
				player->GetActivityMgr()->AddCurNode();
				++m_params;
				if(m_params <= ProtectedKingLoader::GetInstance()->GetMaxNode())
				{
					FlushMonster(m_params - 1);
				}
				else
				{
					m_process = eWinFight;
				}
			}
		}
	}

	if(isSuc >= 0)
	{
		ActivityKing::ToClientProtectResult toClient;

		toClient.set_ret(isSuc);

		INT64PLAYERMAP::iterator it = m_player.begin();
		for(; it!=m_player.end(); ++it)
		{
			if(it->second.Get()==NULL)
				continue;
			QuickMessage::GetInstance()->AddSendMessage(it->second->GetID(), it->second->GetChannelID(), &toClient, MSG_SIM_GM2C_PROTECTE_RESULT);
		}
	}

	return true;
}
