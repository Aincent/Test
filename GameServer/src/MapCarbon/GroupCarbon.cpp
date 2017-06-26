/*
 * GroupCarbon.cpp
 *
 *  Created on: 2015年6月30日
 *      Author: root
 */

#include "GroupCarbon.h"
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
#include "MessageStruct/ServerReturn2Int.pb.h"

CGroupCarbon::CGroupCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):CGameDuplicate(onlyid,charid,paramf,params),m_nCount(0)
{

}

CGroupCarbon::~CGroupCarbon()
{
	m_monsterDeadEvent.SetNull();
	m_playerDeadEvent.SetNull();
	m_leaveMapEvent.SetNull();
}

int CGroupCarbon::Init()
{
	int ret = CGameDuplicate::Init();
	return ret;
}

bool CGroupCarbon::Update()
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
			DestoryGroupToWs();
			return false;
		}
	default :
		{
			break;
		}
	}

	return true;
}

bool CGroupCarbon::JoinHandle(Smart_Ptr<Object> & obj)
{
	if(m_process >= eDuplicateChangeMap)
		return false;

	vector<int> vec;
	MapInstanceProto::SimInstanceUpdate toClient;

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		if(m_paramf != player->getAttManage()->getValue<int64>(eTeamId))	//同一队伍才能进
		{
			return false;
		}

		SendHarmRankToClient(player);

		if(vec.size() > 0)
		{
			player->SynCharAttribute(vec);
		}

		if(!m_monsterDeadEvent.IsValid())
		{
			m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &CGroupCarbon::HandleMonsterDeadEvent);
		}

		if(!m_leaveMapEvent.IsValid())
		{
			m_leaveMapEvent = RegGlobalEventPtr(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, this, &CGroupCarbon::HandleLeaveMapEvent);
		}

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		if(m_process == eCreated)
			m_process = ePlayerWork;
	}

	return true;
}

int CGroupCarbon::Join(Smart_Ptr<Object> & obj)
{
	if(!JoinHandle(obj))
		return -1;

	int ret = CGameZone::Join(obj);
	if(ret == 0 && obj->GetType() == ePlayer)
	{
		int bufferId = GameParaLoader::GetInstance()->GetGroupInstanceBuffId();
		AddBuffer(bufferId);
	}

	return ret;

//去掉掉落只能自己看见
//	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
//	if(!info)
//		return -1;
//
//	int sectorIndex = GetSectorIndex(obj->GetXPos(), obj->GetYPos());
//	if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
//	{
//		LOG_ERROR(FILEINFO, "sectorindex is invalid(mapid=%d, xpos=%d, ypos=%d)", (int)GET_MAP_ID(m_sceneID), obj->GetXPos(), obj->GetYPos());
//
//		return -1;
//	}
//
//	//���㵱ǰ���ڵľŹ���
//	int groupIndex = GetSectorGroupIndex(obj->GetXPos(), obj->GetYPos());
//	if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
//	{
//		LOG_ERROR(FILEINFO, "groupindex is invalid");
//
//		return -1;
//	}
//
//	//����ɫ�������
//	Sector * pSector = &m_totalSector[sectorIndex];
//
//	if(pSector->Join(obj))
//	{
//		LOG_ERROR(FILEINFO, " enter sector error");
//
//		return -1;
//	}
//
//	switch(obj->GetType())
//	{
//	case ePlayer:
//		{
//			Smart_Ptr<Player> player = obj;
//
//			//����ɫ���볡��
//			m_player[player->GetID()] = player;
//			m_allObject[player->GetKey()] = obj;
//			//进入地图前切换玩家的PK模式
//			//世界地图配置表里面的是表示pk模式的切换限制
//			//不是玩家所能用的模式
//			MapStaticInfo * mapInfo = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(m_sceneID));
//			if(mapInfo)
//			{
//				player->SetPkTypeByMap(mapInfo->pktype);
//			}
//			//����ҽ���֪ͨ�Ź�������
////			printf("\n++++++++++++++++++++++++++++++++++++++++charid = %lld �ӾŹ����%d������ģ�%d, %d������\n", player->GetID(),groupIndex, player->GetXPos(), player->GetYPos());
//			SectorGroup * pGroup = &m_totalGroup[groupIndex];
//
//			pGroup->SendAllObjectToGroup(obj, 0);
//
//			if(player && !(player->GetGameZone()))
//				player->SetGameZone(this);
//			break;
//		}
//	case eMonster:
//		{
//			Smart_Ptr<Monster> monster = obj;
//
//			if(monster->GetQuality() == eWorldMonster)
//			{
//				PlayerMessage::GetInstance()->UpdateWorldBoss(monster->GetID(), true);
//			}
//
//			//���������֪ͨ�Ź�������
//			SectorGroup * pGroup = &m_totalGroup[groupIndex];
//
//			pGroup->SendAllObjectToGroup(obj, 0);
//
//			if(monster && !(monster->GetGameZone()))
//				monster->SetGameZone(this);
//
//			break;
//		}
//	case eDropItem:
//	{
//		m_allObject[obj->GetKey()] = obj;
//
//		Smart_Ptr<DropItemsAllot> item = obj;
//		item->SendToOwer();
//
//		break;
//	}
//	case ePet:
//		{
//			Smart_Ptr<GirlWar> girlWar = obj;
//
//			m_girlWar[girlWar->GetKey()] = girlWar;
//			m_allObject[girlWar->GetKey()] = obj;
//
//			SectorGroup * pGroup = &m_totalGroup[groupIndex];
//
//			pGroup->SendAllObjectToGroup(obj, 0);
//
//			if(girlWar && !(girlWar->GetGameZone()))
//				girlWar->SetGameZone(this);
//			break;
//		}
//	default:
//		{
//			LOG_ERROR(FILEINFO, "unknown object type[%d] join zone", obj->GetType());
//
//			return -1;
//		}
//	}

	return 0;
}

void CGroupCarbon::SendHarmRankToClient(Smart_Ptr<Player>& player)
{
	bool bResult = true;
	MapInstanceProto::GMToClientSimHarmRank toClient;
//	MapInstanceProto::GMToClientSimHarmRank toClientOther;

	vector<TeamHarm>::iterator itr =  m_TeamHarm.begin();
	for(; itr != m_TeamHarm.end(); ++itr)
	{
		if(itr->m_charid == player->GetID())
		{
			bResult = false;
		}

		MapInstanceProto::HarmRank* pHarmRank = toClient.add_harminfo();
		pHarmRank->set_charid(itr->m_charid);
		pHarmRank->set_charname(itr->m_name);
		pHarmRank->set_harm(itr->m_harm);
	}

	if(bResult)
	{
		TeamHarm harm;

		harm.m_charid = player->GetID();
		harm.m_name = player->GetName();
		m_TeamHarm.push_back(harm);

		MapInstanceProto::HarmRank* pHarmRank = toClient.add_harminfo();
		pHarmRank->set_charid(harm.m_charid);
		pHarmRank->set_charname(harm.m_name);
		pHarmRank->set_harm(harm.m_harm);
	}

	INT64PLAYERMAP::iterator it = m_player.begin();
	for(; it!=m_player.end(); ++it)
	{
		if(it->second.Get()==NULL)
			continue;
		QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(), &toClient, MSG_SIM_GM2C_HARM_RANK);
	}
}

int CGroupCarbon::Move(Smart_Ptr<Object> &obj)
{
	CGameZone::Move(obj);
	return 0;
}

void CGroupCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
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

		ClearBuffer(player);
		player->getAttManage()->setValue(eTeamId,0);

		if(m_process < eOver)
		{
			ServerReturn::ServerDoubleInt toWorld;
			Smart_Ptr<SvrItem> worldSvr;
			ServerConHandler::GetInstance()->GetWorldServerBySvrID(m_params,worldSvr);
			if(worldSvr)
			{
				toWorld.set_rets(player->GetID());
				toWorld.set_retf(m_paramf);
				Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_QUITE_TEAM, &toWorld, worldSvr.Get(), SimpleMessage);
				Message_Facade::Send(messRet);
			}
		}

		vector<TeamHarm>::iterator itr =  m_TeamHarm.begin();
		for(; itr != m_TeamHarm.end();)
		{
			if(itr->m_charid == player->GetID())
			{
				itr = m_TeamHarm.erase(itr);

				break;
			}
			else
			{
				++itr;
			}
		}
	}
}

void CGroupCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);
	CGameZone::Leave(obj, isDel, delFromMap);
//去掉掉落消失只能自己看见
//	switch(obj->GetType())
//	{
//	case ePlayer:
//		{
//			Smart_Ptr<Player> player = obj;
//			if(player->GetGameZone() == NULL)
//				return;
//			int sectorIndex = GetSectorIndex(player->GetXNewPos(), player->GetYNewPos());
//			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
//			if(!info || sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
//			{
//				LOG_ERROR(FILEINFO, "sectorindex in player[%lld] is invalid(mapid=%d, xpos=%d, ypos=%d) when player leave",
//						player->GetID(),
//						(int)GET_MAP_ID(m_sceneID),
//						player->GetXNewPos(),
//						player->GetYNewPos());
//
//				return;
//			}
//
//			//����ɫ�Ӹ��������Ƴ�
//			Sector * pSector = &m_totalSector[sectorIndex];
//
//			if(pSector->Leave(obj))
//			{
//				LOG_ERROR(FILEINFO, "player[gs charid=%lld] leave sector error", GET_PLAYER_CHARID(player->GetID()));
//			}
//
////			printf("\n++++++++++++++++++++++++++++++++++++++++charid = %lld �ӾŹ����%d������ģ�%d, %d������\n", player->GetID(),player->GetGroup(), player->GetXNewPos(), player->GetYNewPos());
//			if(isDel)
//			{
//				int groupIndex = GetSectorGroupIndex(player->GetXNewPos(), player->GetYNewPos());
//				if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
//				{
//					LOG_ERROR(FILEINFO, "groupOldIndex in player is invalid when player leave");
//
//					return ;
//				}
//
//				SectorGroup * pGroup = &m_totalGroup[groupIndex];
//
//				pGroup->SendObjLeaveToGroup(obj, 0);
//			}
//
//			//����Ҵӳ������Ƴ�
//			INT64PLAYERMAP::iterator it = m_player.find(player->GetID());
//			if(it != m_player.end())
//			{
//				it->second->UpdateEnemyList();
//
//				INTOBJECTMAP::iterator itObj = m_allObject.find(it->second->GetKey());
//				if(itObj != m_allObject.end())
//				{
//					m_allObject.erase(itObj);
//				}
//
//				m_player.erase(it);
//			}
//
//			if(player)
//				player->ResetGameZone();
//
//			break;
//		}
//	case eMonster:
//		{
//			Smart_Ptr<Monster> monster = obj;
//			if(monster->GetGameZone() == NULL)
//				return;
//			int sectorIndex = GetSectorIndex(monster->GetXNewPos(), monster->GetYNewPos());
//			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
//			if(!info || sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
//			{
//				LOG_ERROR(FILEINFO, "sectorindex in monster[%lld] is invalid(mapid=%d, xpos=%d, ypos=%d) when monster leave",
//						monster->GetID(),
//						(int)GET_MAP_ID(m_sceneID),
//						monster->GetXNewPos(),
//						monster->GetYNewPos());
//
//				return;
//			}
//
//			//����ɫ�Ӹ��������Ƴ�
//			Sector * pSector = &m_totalSector[sectorIndex];
//			if(pSector->Leave(obj))
//			{
//				LOG_ERROR(FILEINFO, "monster[id=%lld] leave sector error", monster->GetID());
//			}
//
//			//通知玩家怪物从其视野消失，假如是真正的删除的话就不要通知玩家移除视野，已经在monster->dead()告诉玩家怪物已经死亡
//			if(isDel && !delFromMap)
//			{
//				int groupIndex = GetSectorGroupIndex(monster->GetXNewPos(), monster->GetYNewPos());
//				if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
//				{
//					LOG_ERROR(FILEINFO, "groupOldIndex in monster is invalid when monster leave");
//					return ;
//				}
//
//				SectorGroup * pGroup = &m_totalGroup[groupIndex];
//				pGroup->SendObjLeaveToGroup(obj, 0);
//			}
//
//			if(delFromMap)
//			{
//				INTOBJECTMAP::iterator itObj = m_allObject.find(monster->GetKey());
//				if(itObj != m_allObject.end())
//				{
//					m_allObject.erase(itObj);
//				}
//
//				INTVMPNSTERMAP::iterator monObj = m_allMonster.find(monster->GetID());
//				if(monObj != m_allMonster.end())
//				{
//					vector<Smart_Ptr<Monster> >::iterator inner = monObj->second.begin();
//					for(;inner != monObj->second.end(); )
//					{
//						if((*inner)->GetKey() == monster->GetKey())
//						{
//							monObj->second.erase(inner++);
//							break;
//						}
//						else
//						{
//							inner++;
//						}
//					}
//
//					if(monObj->second.size() <= 0)
//					{
//						m_allMonster.erase(monObj);
//					}
//				}
//
//				//清空怪物的仇恨列表
//				monster->GetBrain()->ClearEnemy();
//				monster->GetBrain()->clearGirlTarget();
//
//				//monster->Release();
//				//ObjectFactory::GetInstance()->Recycle(obj);
//
//				Smart_Ptr<Monster> tempMon;
//				monster->SetOwnerToBrain(tempMon);
//			}
//			break;
//		}
//
//	case eDropItem:
//		{
//			int sectorIndex = GetSectorIndex(obj->GetXPos(), obj->GetYPos());
//			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
//			if(!info || sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
//			{
//				LOG_ERROR(FILEINFO, "sectorindex in dropitem is invalid(mapid=%d, xpos=%d, ypos=%d) when dropitem leave",
//						(int)GET_MAP_ID(m_sceneID),
//						obj->GetXPos(),
//						obj->GetYPos());
//
//				return;
//			}
//
//			//����ɫ�Ӹ��������Ƴ�
//			Sector * pSector = &m_totalSector[sectorIndex];
//
//			if(pSector->Leave(obj))
//			{
//				LOG_ERROR(FILEINFO, "dropitem leave sector error");
//			}
//
//			if(isDel)
//			{
//				Smart_Ptr<DropItemsAllot> item = obj;
//				item->SendLeaveToOwer();
//			}
//
//			INTOBJECTMAP::iterator itObj = m_allObject.find(obj->GetKey());
//			if(itObj != m_allObject.end())
//			{
//				m_allObject.erase(itObj);
//			}
//
//			//回收对象
//			//Smart_Ptr<DropItemsAllot> item = obj;
//			//item->Release();
//			//ObjectFactory::GetInstance()->Recycle(obj);
//			break;
//		}
//	case ePet:
//		{
//			Smart_Ptr<GirlWar> girlWar = obj;
//			if(girlWar->GetGameZone() == NULL)
//				return;
//			int sectorIndex = GetSectorIndex(girlWar->GetXNewPos(), girlWar->GetYNewPos());
//			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
//			if(!info || sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
//			{
//				LOG_ERROR(FILEINFO, "sectorindex in girlWar[owner=%lld, id=%d] is invalid(mapid=%d, xpos=%d, ypos=%d) when girlWar leave",
//						girlWar->GetOwner()->GetID(),
//						(int)girlWar->GetID(),
//						(int)GET_MAP_ID(m_sceneID),
//						girlWar->GetXNewPos(),
//						girlWar->GetYNewPos());
//
//				return;
//			}
//
//			Sector * pSector = &m_totalSector[sectorIndex];
//			if(pSector->Leave(obj))
//			{
//				LOG_ERROR(FILEINFO, "girlWar[id=%d] leave sector error", girlWar->GetID() );
//			}
//
//			if(isDel)
//			{
//				int groupIndex = GetSectorGroupIndex(girlWar->GetXNewPos(), girlWar->GetYNewPos());
//				if(groupIndex < 0 || groupIndex >= m_totalGroupSize)
//				{
//					LOG_ERROR(FILEINFO, "groupOldIndex in pet is invalid when pet leave");
//					return ;
//				}
//
//				SectorGroup * pGroup = &m_totalGroup[groupIndex];
//				pGroup->SendObjLeaveToGroup(obj, 0);
//			}
//
//			//����Ҵӳ������Ƴ�
//			INTGRILWARMAP::iterator it = m_girlWar.find(girlWar->GetKey());
//			if(it != m_girlWar.end())
//			{
//				INTOBJECTMAP::iterator itObj = m_allObject.find(it->second->GetKey());
//				if(itObj != m_allObject.end())
//				{
//					m_allObject.erase(itObj);
//				}
//				m_girlWar.erase(it);
//			}
//
//			if(girlWar)
//			{
//				girlWar->ClearTargets();
//				girlWar->ResetGameZone();
//			}
//			break;
//		}
//	default:
//		{
//			LOG_ERROR(FILEINFO, "unknown object type[%d] leave", obj->GetType());
//		}
//	}

	if(GetPlayerCount() <= 0)
	{
		m_monsterDeadEvent.SetNull();
		m_playerDeadEvent.SetNull();
//		m_leaveMapEvent.SetNull();
		m_leaveTime = CUtil::GetNowSecond();
	}
}

void CGroupCarbon::LeaveExame(Smart_Ptr<Player> & player)
{
	if(GetPlayerCount() <= 0)
	{
		m_process = eAward;
	}
}

bool CGroupCarbon::IsCanEnter(Smart_Ptr<Player> & player)
{
	//传送阶段，就不允许进入了
	if(m_process >= eDuplicateChangeMap)
		return false;

	if(player->getAttManage()->getValue<int>(eCharLv) < (int)m_data->m_nLevel)
	{
		return false;
	}

	if(m_paramf != player->getAttManage()->getValue<int64>(eTeamId))	//同一队伍才能进
	{
		return false;
	}

	//玩家死了复活传入地图
	if(player->IsDead())
	{
		CharBattle::RevivalData req;

		//增加地图id和坐标，重置死亡状态，解决客户端显示“使用复活单”的提示bug，是因为玩家回城复活，那么会发两次消息给客户端
		player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));
		player->UnsetCreatureState(ERS_Dead);
		player->SetDataFlag(eBaseInfo);
//					player->SendDeadMessage(eAttackSim);
			//如果不传地图Id就表示原地复活
		req.set_mapid(0);
		req.set_xpos(m_data->m_nXPos);
		req.set_ypos(m_data->m_nYPos);
		req.set_key(player->GetKey());
		req.set_hp(player->getAttManage()->getValue<int>(eCharHP));
		req.set_mp(player->getAttManage()->getValue<int>(eCharMP));

		player->SendSynchMessage(&req, MSG_SIM_GT2C_REVIVAL);
	}

	return true;
}

void CGroupCarbon::Win()
{
	m_isSuccess = true;
	m_process = eAward;
}

void CGroupCarbon::Over()
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

void CGroupCarbon::CountEx()
{
	m_process = eOver;
}

void CGroupCarbon::Count()
{
	m_process = eAward;
}

void CGroupCarbon::Failed()
{
	m_process = eAward;
}

void CGroupCarbon::Award()
{
	DWORD nType = GetPlayerCount() >= 3?1:0;
	DWORD totalFight = GetTotalFight();
	INT64PLAYERMAP::iterator it = m_player.begin();
	for(; it!=m_player.end(); ++it)
	{
		if(it->second.Get()==NULL)
			continue;

		it->second->GetInstanceMgr()->AddPassInstance(m_data->m_nMapID,1,m_data->m_eType);
		it->second->GetInstanceMgr()->SendAwardToPlayer(m_isSuccess,m_data->m_nMapID,nType,CUtil::GetNowSecond()-m_createTime,totalFight);

		PlayerInstancePassEventArgs tArgs(it->second,m_data->m_nMapID);

		it->second->FireEvent(PLAYER_PASS_INSTANCE,tArgs);
		FireGlobalEvent(PLAYER_GLOBE_PASSINSTANCE, tArgs);
	}

	m_process = eDuplicateChangeMap;
	m_awardTime = CUtil::GetNowSecond();
}

DWORD CGroupCarbon::GetTotalFight()
{
	DWORD fight = 0;
	INT64PLAYERMAP::iterator it = m_player.begin();
	for(; it!=m_player.end(); ++it)
	{
		if(it->second.Get()==NULL)
			continue;
		fight += it->second->getAttManage()->getValue<int>(eFightPower);
	}

	return fight;
}

bool CGroupCarbon::HandleLeaveMapEvent(const EventArgs & e)
{
	PlayerRemoveMemberEventArgs *pArgs = (PlayerRemoveMemberEventArgs *) (&e);

	if(pArgs->m_eMethod != GROUP_REMOVEMETHOD_OUT)
		return true;
	if(pArgs->m_pObj.Get()==NULL || pArgs->m_pObj->GetMapID() != m_sceneID)
		return true;

	if(m_paramf != pArgs->m_pObj->getAttManage()->getValue<int64>(eTeamId))	//同一队伍才能进
	{
		return true;
	}

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

bool CGroupCarbon::HandleMonsterDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);

	if(tArgs->m_myself.Get()==NULL || GetSceneID() != tArgs->m_myself->GetMapID())
		return true;

	if(tArgs->m_other.Get()==NULL)
		return true;

	Smart_Ptr<Monster> mon = tArgs->m_myself;
	LOG_DEBUG(FILEINFO, "CGameDuplicate::HandleMonsterDeadEvent \t %lld", mon->GetID());

	Smart_Ptr<Player>  player;
	Smart_Ptr<GirlWar>  girlwar;
	map<int64, int> enemyValue;
	mon->GetEnemyValue(enemyValue);

	if(tArgs->m_other->GetType() != ePlayer && tArgs->m_other->GetType() !=  ePet)
		return true;

	if(tArgs->m_other->GetType() == ePet)
	{
		girlwar = tArgs->m_other;

		player  = girlwar->GetOwner();
	}
	else
	{
		player = tArgs->m_other;
	}

	//去掉掉落每人一份
//	INT64PLAYERMAP::iterator it = m_player.begin();
//	for(; it!=m_player.end(); ++it)
//	{
//		if(it->second.Get()==NULL)
//			continue;
//
//		if(player->GetID() != it->second->GetID())
//		{
//			DropItemsManager::GetInstance()->HandleBattleDeath(mon,it->second);
//		}
//	}

	//小怪按最后一击算
	if(!mon->IsBetterAI())
	{
		UpdateHarmRank(player,mon->getAttManage()->getValue<int>(eCharHPMax));
	}
	else
	{
		map<int64,int>::iterator itr = enemyValue.begin();
		for(; itr != enemyValue.end(); ++itr)
		{
			Smart_Ptr<Player> player;
			PlayerMessage::GetInstance()->GetPlayerByCharid(itr->first, player);
			if(player)
			{
				UpdateHarmRank(player,itr->second);
			}
		}
	}


	m_nCount++;
	if(m_nCount % 5 == 0)
	{
		SysHarmRank();
	}

	if(IsAllMonsterDead())
	{
		m_process = eWinFight;

		if(m_nCount % 5 != 0)
		{
			SysHarmRank();
		}
	}

	return true;
}

void CGroupCarbon::UpdateHarmRank(Smart_Ptr<Player>& player, int value)
{
	if(player)
	{
		vector<TeamHarm>::iterator itr =  m_TeamHarm.begin();
		for(; itr != m_TeamHarm.end(); ++itr)
		{
			if(itr->m_charid == player->GetID())
			{
				itr->m_harm += value;
				break;
			}
		}
	}
}

void CGroupCarbon::SysHarmRank()
{
	MapInstanceProto::GMToClientSimHarmRank toClient;
	vector<TeamHarm>::iterator itr =  m_TeamHarm.begin();
	for(; itr != m_TeamHarm.end(); ++itr)
	{
		MapInstanceProto::HarmRank* pHarmRank = toClient.add_harminfo();
		pHarmRank->set_charid(itr->m_charid);
		pHarmRank->set_charname(itr->m_name);
		pHarmRank->set_harm(itr->m_harm);
	}

	INT64PLAYERMAP::iterator it = m_player.begin();
	for(; it!=m_player.end(); ++it)
	{
		if(it->second.Get()==NULL)
			continue;
		QuickMessage::GetInstance()->AddSendMessage(it->second->GetID(), it->second->GetChannelID(), &toClient, MSG_SIM_GM2C_HARM_RANK);
	}
}

int CGroupCarbon::SetPlayerInstanceFlops(int64 charid,int flopid)
{
	vector<int> vecLock;
	MapInstanceProto::RetMemberFlop toClient;

	Smart_Ptr<Player> player = GetPlayer(charid);
	if(!player)
	{
		return eNotFindThisCharID;
	}

	if(flopid >= (int)m_AwardList.size())
	{
		return eFlopMsgError;
	}

	vector<AwardFlops>::iterator itr = m_AwardList.begin();
	for(; itr != m_AwardList.end(); ++itr)
	{
		if(itr->m_charid == charid)
		{
			return eFlopHadDone;
		}
	}

	if(m_AwardList[flopid].m_charid > 0)
	{
		return eFlopHadOwer;
	}

	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(m_AwardList[flopid].m_itemid);
	if(ginfo == NULL)
	{
		return eItemError;
	}

	Smart_Ptr<ArticleBaseMgr> pPacket = player->GetContainer(ePackageType);
	if(!pPacket)
	{
		return ePlayerPackageFail;
	}

	if(player->GetContainer(ePackageType)->IsBagEnought(m_AwardList[flopid].m_itemid,m_AwardList[flopid].m_Counts,m_AwardList[flopid].m_BindType) != 0)
	{
		return ePackageFull;
	}

	m_AwardList[flopid].m_charid = charid;

	player->GetContainer(ePackageType)->AddItem(m_AwardList[flopid].m_itemid,m_AwardList[flopid].m_Counts,vecLock,m_AwardList[flopid].m_BindType,ginfo->equipStrengthLv,true, gift_open_type);

	toClient.set_flopid(flopid);
	toClient.set_flopname(player->GetName());
	toClient.set_itemid(m_AwardList[flopid].m_itemid);
	toClient.set_itemnum(m_AwardList[flopid].m_Counts);
	toClient.set_bindtype(m_AwardList[flopid].m_BindType);

	INT64PLAYERMAP::iterator it = m_player.begin();
	for(; it!=m_player.end(); ++it)
	{
		if(it->second.Get()==NULL)
			continue;

		QuickMessage::GetInstance()->AddSendMessage(it->second->GetID(), it->second->GetChannelID(), &toClient, MSG_SIM_GM2C_MEMBERFLOPINFO);
	}

	return 0;
}

void CGroupCarbon::DestoryGroupToWs()
{
	MapInstanceProto::GMToWSDelTeam toWorld;
	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(m_params,worldSvr);
	if(worldSvr)
	{
		toWorld.set_teamid(m_paramf);
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_DEL_TEAM, &toWorld, worldSvr.Get(), SimpleMessage);
		Message_Facade::Send(messRet);
	}
}

void CGroupCarbon::AddBuffer(int bufferId)
{
	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(bufferId);
	if(info == NULL)
	{
		return;
	}

	if(GetPlayerCount() < 3)
		return;

	INT64PLAYERMAP::iterator it = m_player.begin();
	for(; it!=m_player.end(); ++it)
	{
		if(it->second.Get()==NULL)
			continue;

		Smart_Ptr<CreatureObj> obj = it->second;
		if(it->second->AddBuff(bufferId,obj))
		{
			map<int64,vector<int> >::iterator itr = m_buffers.find(it->second->GetID());
			if(itr != m_buffers.end())
			{
				itr->second.push_back(bufferId);
			}
			else
			{
				vector<int> vec;
				vec.push_back(bufferId);
				m_buffers[it->second->GetID()] = vec;
			}
		}
	}
}

void CGroupCarbon::ClearBuffer(Smart_Ptr<Player>& player)
{
	map<int64,vector<int> >::iterator itr = m_buffers.find(player->GetID());
	if(itr != m_buffers.end())
	{
		for(size_t i = 0; i < itr->second.size(); i++)
		{
			player->RemoveBuffById(itr->second[i]);
		}
	}

	m_buffers.erase(player->GetID());
}
