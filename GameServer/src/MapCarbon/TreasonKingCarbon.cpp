/*
 * TreasonKingCarbon.cpp
 *
 *  Created on: 2015年6月30日
 *      Author: root
 */

#include "TreasonKingCarbon.h"
#include "util.h"
#include "Log/Logger.h"
#include "../Object/Player/Player.h"
#include "../Object/ItemDrop/DropItemMgr.h"
#include "../Map/DuplicateMgr.h"
#include "CounterDefine.h"
#include "../ServerEventDefine.h"
#include "MessageStruct/CharLogin/MapInstanceProto.pb.h"
#include "MessageStruct/ServerReturnBool.pb.h"
#include "../QuickMessage.h"
#include "../Object/Player/PlayerMessage.h"
#include "../ServerManager.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageCommonRet.h"
#include "../Activity/FileLoader/TreasonKingLoader.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../Object/ObjectFactory.h"
#include "../Object/FileLoader/AILoader.h"
#include "../Activity/ActivityMgr.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../Object/AI/AIBrain.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../Object/ItemDrop/DropItemAlloc.h"
#include "../Chat/ChatMgr.h"
#include "../Container/ArticleEquipMgr.h"
#include "../Mail/MailMessage.h"
#include "./FileLoader/InternationalLoader.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"

CTreasonKingCarbon::CTreasonKingCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):CGameDuplicate(onlyid,charid,paramf,params)
{

}

CTreasonKingCarbon::~CTreasonKingCarbon()
{
	m_monsterDeadEvent.SetNull();
	m_playerDeadEvent.SetNull();
	m_leaveMapEvent.SetNull();
}

int CTreasonKingCarbon::Init()
{
	int ret = CGameDuplicate::Init();
	return ret;
}

bool CTreasonKingCarbon::Update()
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

bool CTreasonKingCarbon::JoinHandle(Smart_Ptr<Object> & obj)
{
	if(m_process >= eDuplicateChangeMap)
		return false;

	vector<int> vec;

	MapInstanceProto::SimInstanceUpdate toClient;

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		if(m_process == eCreated)
		{
			Smart_Ptr<CounterService> & counter = player->GetCounterService();
			if(counter)
			{
				int price = TreasonKingLoader::GetInstance()->GetLevelsGold(counter->GetNumById(TREASON_KING_LEVELS));
				player->ChangeGolden(price, false);
				vec.push_back(eCharGolden);
				// 统计元宝花费 参加剿灭反王
//				StatisticMgr::GetInstance()->StatisticPlayerGetItems(player, eStatic_AboutGolden, eStaMinor_AboutGolden_JoinTreasonKing, 1, price);
			}
		}

		if(vec.size() > 0)
		{
			player->SynCharAttribute(vec);
		}

		if(!m_monsterDeadEvent.IsValid())
		{
			m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &CTreasonKingCarbon::HandleMonsterDeadEvent);
		}

		if(!m_leaveMapEvent.IsValid())
		{
			m_leaveMapEvent = RegGlobalEventPtr(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, this, &CTreasonKingCarbon::HandleLeaveMapEvent);
		}

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		if(m_process == eCreated)
			m_process = ePlayerWork;
	}

	return true;
}

int CTreasonKingCarbon::Join(Smart_Ptr<Object> & obj)
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

int CTreasonKingCarbon::Move(Smart_Ptr<Object> &obj)
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
				LOG_ERROR(FILEINFO, "sectorindex in player[%lld] is invalid(mapid=%d,xpos=%d,ypos=%d) when player move",
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
			if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex in monster(mapid=%d,xpos=%d,ypos=%d) is invalid when player move",
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
		if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
		{
			LOG_ERROR(FILEINFO, "sectorindex in monster(mapid=%d,xpos=%d,ypos=%d) is invalid when player move",
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

//					girlWar->SetGroup(groupIndex);
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

void CTreasonKingCarbon::SendSynchPath(PlayerSynch::MonsterPathToPlayer &path, Smart_Ptr<Monster> &monster)
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

void CTreasonKingCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
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
	}
}

void CTreasonKingCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);

	switch(obj->GetType())
	{
	case ePlayer:
		{
			Smart_Ptr<Player> player = obj;
			int sectorIndex = GetSectorIndex(player->GetXNewPos(), player->GetYNewPos());
			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
			if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex in player[%lld] is invalid(mapid=%d,xpos=%d,ypos=%d) when player leave",
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
				LOG_ERROR(FILEINFO, "sectorindex in dropitem is invalid when dropitem leave");
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
		if(!info || sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
		{
			LOG_ERROR(FILEINFO, "sectorindex in girlWar(owner=%lld, id=%d) is invalid(mapid=%d,xpos=%d,ypos=%d) when girlWar leave",
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

void CTreasonKingCarbon::LeaveExame(Smart_Ptr<Player> & player)
{
	if(GetPlayerCount() <= 0)
	{
		m_process = eOver;
	}
}

bool CTreasonKingCarbon::IsCanEnter(Smart_Ptr<Player> & player)
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

void CTreasonKingCarbon::Win()
{
	m_isSuccess = true;

	Smart_Ptr<Player> player = GetPlayer(m_owner);
	if(!player)
	{
		return;
	}

	player->GetCounterService()->IncreasePlayerCounter(TREASON_KING_LEVELS,1);
//	player->UpdateUIHighLight(eUIHLHandle_UpAndForceSyn, UIHLType(eUIHLBig_InstanceHall, eUIHLSmall_InstanceHall_KillTreasonKingCanChallenge), 1, false);

	if (player->GetCounterService()->GetNumById(TREASON_KING_LEVELS) > player->GetCounterService()->GetNumById(TREASON_KING_MAXPASSLEVEL))
	{
		player->GetCounterService()->IncreasePlayerCounter(TREASON_KING_MAXPASSLEVEL,
				player->GetCounterService()->GetNumById(TREASON_KING_LEVELS) - player->GetCounterService()->GetNumById(TREASON_KING_MAXPASSLEVEL));
	}

	if (GET_MAP_ID(GetSceneID()) == TreasonKingLoader::GetInstance()->GetMaxLevelMapID())
	{
		if (GET_MAP_ID(GetSceneID()) ==
				TreasonKingLoader::GetInstance()->GetLevelsMapId(player->GetCounterService()->GetNumById(TREASON_KING_LEVELS)))
		{
			// 通关最高层 算次数
			player->GetCounterService()->IncreasePlayerCounter(TREASON_KING_ENTER, 1);

		}
		else
		{ // 只有当剿灭反王层数重置后,角色还在剿灭反王副本内,
			player->GetCounterService()->SetPlayerCounter(TREASON_KING_MAXPASSLEVEL, 0);
		}

		player->GetCounterService()->SetPlayerCounter(TREASON_KING_LEVELS, 0);

//		player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_InstanceHall, eUIHLSmall_InstanceHall_KillTreasonKingCanChallenge), player->GetActivityMgr()->GetTreansonSuplusPass());

		HintValue value;
		value.sValue = player->GetActivityMgr()->GetTreasonSurplusResetCnt(value.fValue);
		player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, value, UIHLType(eBig_TreasonKing, eSmall_TreasonKing_AlreadyFini));

	}

	int elapse = CUtil::GetNowSecond() - m_createTime;
	if(player->GetActivityMgr()->AddTreasonBestTime(m_data->m_nMapID,elapse))
	{
		MapInstanceProto::SysInstanceRanking toWorld;
		Smart_Ptr<SvrItem> worldSvr;
		ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),worldSvr);
		if(worldSvr)
		{
			toWorld.set_charid(player->GetID());
			toWorld.set_mapid(m_data->m_nMapID);
			toWorld.set_elapsedtime(elapse);
			toWorld.set_type(m_data->m_eType);

			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_SYSINSTANCERANK, &toWorld, worldSvr.Get(), SimpleMessage);
			Message_Facade::Send(messRet);
		}
	}

	m_process = eAward;
}

void CTreasonKingCarbon::Over()
{
	if(IsAllMonsterDead())
	{
		m_process = eWinFight;
	}
	else
	{
		m_process = eFailed;
	}
}

void CTreasonKingCarbon::CountEx()
{
	m_process = eOver;
}

void CTreasonKingCarbon::Count()
{
	int64 nowTime = CUtil::GetNowSecond();
	if(nowTime - m_leaveTime >= m_data->m_nDelTime)
	{
		m_process = eOver;
	}
}

void CTreasonKingCarbon::Failed()
{
	m_process = eAward;
}

void CTreasonKingCarbon::Award()
{
	m_process = eDuplicateChangeMap;
	m_awardTime = CUtil::GetNowSecond();

	Smart_Ptr<Player> player = GetPlayer(m_owner);
	if ((bool)player)
	{
		if(m_isSuccess)
		{
			SendAwardToPlayer(player);

			PlayerInstancePassEventArgs tArgs(player,m_data->m_nMapID);

			player->FireEvent(PLAYER_PASS_INSTANCE,tArgs);
		}
	}
}

bool CTreasonKingCarbon::HandleLeaveMapEvent(const EventArgs & e)
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

bool CTreasonKingCarbon::HandleMonsterDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);

	if(tArgs->m_myself.Get()==NULL || GetSceneID() != tArgs->m_myself->GetMapID())
		return true;

	Smart_Ptr<Monster> mon = tArgs->m_myself;
	LOG_DEBUG(FILEINFO, "CGameDuplicate::HandleMonsterDeadEvent \t %lld", mon->GetID());
	if(IsAllMonsterDead())
	{
		m_process = eWinFight;
	}

	return true;
}

void CTreasonKingCarbon::SendAwardToPlayer(Smart_Ptr<Player> & player)
{
//	vector<int> vec;
//	vector<int64> goods;
//	vector<DWORD> idList;
//	vector<DWORD> numList;
//	//需统计的真气
//	int statisticForce = 0;
//	int statisticMoney = 0;
//	int star = 1;
////	StatisticInfo::CostAndGotInfo costAndGotInfo;
//
//	InstanceMapReward* entryReward = InstanceLoader::GetInstance()->GetEntryReward(m_data->m_nMapID,player->getAttManage()->getValue<int>(eCharLv));
//	if (!entryReward)
//	{
//		LOG_DEBUG(FILEINFO,"InstanceMgr::startSweep: entry Reward is invalid %d.",m_data->m_nMapID);
//		return;
//	}
//
//	//给玩家添加物品
//	Smart_Ptr<ArticleBaseMgr> pPacket = player->GetContainer(ePackageType);
//	if(!pPacket)
//	{
//		return;
//	}
//
//	float awardrate[3] = {entryReward->m_nOneStarRate/100.0,entryReward->m_nTwoStarRate/100.0,entryReward->m_nThreeStarRate/100.0};
//	float temp = awardrate[star-1];
//
//	idList = entryReward->m_nAward;
//	numList = entryReward->m_nAwardNum;
//
//
//	switch(star)
//	{
//	case 3:
//		{
//			map<DWORD,DWORD>::iterator itr = entryReward->m_nThreestar.begin();
//			for(; itr != entryReward->m_nThreestar.end(); ++itr)
//			{
//				idList.push_back(itr->first);
//				numList.push_back(itr->second);
//			}
//			break;
//		}
//
//	case 2:
//		{
//			map<DWORD,DWORD>::iterator itr = entryReward->m_nTwostar.begin();
//			for(; itr != entryReward->m_nTwostar.end(); ++itr)
//			{
//				idList.push_back(itr->first);
//				numList.push_back(itr->second);
//			}
//			break;
//		}
//	default:
//		{
//			map<DWORD,DWORD>::iterator itr = entryReward->m_nOnestar.begin();
//			for(; itr != entryReward->m_nOnestar.end(); ++itr)
//			{
//				idList.push_back(itr->first);
//				numList.push_back(itr->second);
//			}
//		}
//	}
//
//	vector<WORD> tempFlagList;
//	vector<DWORD> tempStreghtList;
//	tempFlagList.resize(idList.size(),1);
//	tempStreghtList.resize(numList.size(),0);
//
//	int res = pPacket->IsBagEnought(idList,numList,tempFlagList);
//	if (!res)
//	{
//		vector<int> tempPos;
//		pPacket->AddItem(idList, numList,tempPos,tempFlagList,tempStreghtList);
//	}
//	else
//	{
//		for(size_t j = 0; j < idList.size(); j++)
//		{
//			int64 info = numList[j];
//			info = info << 32;	//物品数量
//			DWORD itemid = idList[j];
//			info = info | itemid;
//			goods.push_back(info);
//
//			if(goods.size()>=5)
//			{
//				string warning1 = InternationalLoader::GetInstance()->GetInterStr(8);
//				MailMessageMgr::GetInstance()->SendSysMail(goods, player->GetName(),0,warning1.c_str(),warning1.c_str());
//				goods.clear();
//			}
//
////			StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
////			StatisticMgr::AddItemGotInfo(gotInfo,idList[j],numList[j]);
//		}
//
//		if(goods.size() > 0)
//		{
//			string warning1 = InternationalLoader::GetInstance()->GetInterStr(8);
//			MailMessageMgr::GetInstance()->SendSysMail(goods, player->GetName(),0,warning1.c_str(),warning1.c_str());
//		}
//	}
//
//	player->ChangeMoney((int)(entryReward->m_nAwardMoney * temp),true);
//	vec.push_back(eCharMoney);
//
//	statisticMoney = (int)(entryReward->m_nAwardMoney * temp);
//
//	player->AddExp((int)(entryReward->m_nAwardExp * temp));
//	vec.push_back(eCharExp);
//
//	statisticForce = (int)(entryReward->m_nAwardSprit * temp);
//
//	player->ChangeCurForce(statisticForce,true);
//	vec.push_back(eCharForce);
//
//	if(vec.size() > 0)
//	{
//		player->SynCharAttribute(vec);
//	}
//
////	if(statisticForce > 0)
////	{
////		StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
////		StatisticMgr::AddItemGotInfo(gotInfo,eCharForce,statisticForce);
////	}
////
////	if(statisticMoney > 0)
////	{
////		StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
////		StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,statisticMoney);
////	}
////
////	if(eAccountType_User == player->GetAccountType())
////		StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Instance,eStaMinor_Instance_Reward,&costAndGotInfo);
}
