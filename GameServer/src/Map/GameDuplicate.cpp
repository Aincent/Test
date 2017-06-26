/*
 * GameDuplicate.cpp
 *
 *  Created on: 2014年10月14日
 *      Author: helh
 */

#include "GameDuplicate.h"
#include "util.h"
#include "Log/Logger.h"
#include "../Object/Player/Player.h"
#include "../Object/ItemDrop/DropItemMgr.h"
#include "DuplicateMgr.h"
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
#include "../Activity/FileLoader/TreasonKingLoader.h"
#include "../Activity/FileLoader/ProtectedKingLoader.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../Object/ObjectFactory.h"
#include "../Object/FileLoader/AILoader.h"
#include "../Activity/ActivityMgr.h"
#include "../MapInstance/CrossSvrCompetitionLoader/CrossSvrCompetitionLoader.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../Object/AI/AIBrain.h"
#include "../MapInstance/BeyondBattleMgr.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../Object/ItemDrop/DropItemAlloc.h"
#include "../Chat/ChatMgr.h"
#include "MessageStruct/Country/CountryWar.pb.h"
#include "../Container/ArticleEquipMgr.h"
#include "../Country/GsCountryMgr.h"
#include "MessageStruct/Country/CountryBoss.pb.h"
#include "../Mail/MailMessage.h"
#include "./FileLoader/InternationalLoader.h"
#include "../Country/GsCountryWarLoader.h"
#include "../Country/GsCountryWarMgr.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "../ActiveUIHighLight/CActiveHighLightMgr.h"
#include "../Object/FileLoader/DropItemLoader.h"
#include "FileLoader/ArenaLoader.h"
#include "FileLoader/UpgradeLoader.h"
#include "../Arena/ArenaMgr.h"

CGameDuplicate::CGameDuplicate(int64 onlyid, int64 charid, int64 paramf, int64 params):CGameZone(onlyid),m_owner(charid),m_owner_s(0),m_data(0),m_process(eCreated),
m_isSuccess(false),m_createTime(CUtil::GetNowSecond()),m_leaveTime(0),m_awardTime(0),m_paramf(paramf),m_params(params),m_clanBattle(false),m_nDigNum(0)
{
	m_arenaBeginTime.SetNull();
}

CGameDuplicate::~CGameDuplicate()
{
	if(GetPlayerCount() > 0)
	{
		LOG_FATAL(FILEINFO, "duplicate destroy but player(count=%d) is in the map", GetPlayerCount());
	}

	m_monsterDeadEvent.SetNull();
	m_playerDeadEvent.SetNull();
	m_leaveMapEvent.SetNull();

	if (m_arenaBeginTime.IsValid())
	{
		m_arenaBeginTime.SetNull();
	}
}

int CGameDuplicate::Init()
{
	m_data = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(GetSceneID()));
	if(!m_data)
		return -1;

	int ret = CGameZone::Init();
	return ret;
}

void CGameDuplicate::GetAllPlayerPosition(ClanPackage::ClientReqMMapPlayer &toClient, DWORD clanid, int key)
{
	INT64PLAYERMAP::iterator it = m_player.begin();
	for(; it!=m_player.end(); ++it)
	{
		if(key == it->second->GetKey())
		{
			continue;
		}

		toClient.add_key(it->second->GetKey());
		toClient.add_ismyself((it->second->getAttManage()->getValue<DWORD>(eCharFaction) == clanid));
		toClient.add_xpos(it->second->GetXNewPos());
		toClient.add_ypos(it->second->GetYNewPos());
	}
}

void CGameDuplicate::Win()
{
	switch(m_data->m_eType)
	{
	case eCompeteInstance:
		{
			Smart_Ptr<Player> player = GetPlayer(m_owner);

			if(player)
			{
////				if (player->GetArenaMgr()->IsFakeRank())
////				{
//					player->GetArenaMgr()->BattleOver(m_paramf, true);
//				}
//				else
//				{
					player->SynchPlayerCompeteInfoToWS(m_paramf, true);


					ArenaOverArgs tArgs( player, true);
					player->FireEvent(PLAYER_ARENA_OVER, tArgs);
//				}
			}

			break;
		}
	default :
		{
			break;
		}
	}

	m_isSuccess = true;
	m_process = eAward;
}

void CGameDuplicate::Over()
{
	switch(m_data->m_eType)
	{
	case eCompeteInstance:
		{
			m_process = eFailed;

			break;
		}
	case eVipInstance:
	case ePersonalInstance:
	case eEliteInstance:
	case eCauchemarInstance:
	case eExpInstance:
	case eMoneyInstance1:
		{
			if(IsAllMonsterDead())
			{
				m_process = eWinFight;
			}
			else
			{
				m_process = eFailed;
			}

			break;
		}
	default:
		m_process = eDuplicateChangeMap;
	}
}

void CGameDuplicate::CountEx()
{
	int64 nowTime = CUtil::GetNowSecond();

	switch(m_data->m_eType)
	{
	case eVipInstance:
	case ePersonalInstance:
	case eCompeteInstance:
	case eEliteInstance:
	case eCauchemarInstance:
	case eExpInstance:
	case eMoneyInstance1:
 		{
			m_process = eOver;

			break;
		}
	default:
		{
			if(nowTime - m_leaveTime >= m_data->m_nDelTime)
			{
				m_process = eOver;
			}
		}
	}
}

void CGameDuplicate::Count()
{
	int64 nowTime = CUtil::GetNowSecond();

	switch(m_data->m_eType)
	{
	case eVipInstance:
	case ePersonalInstance:
	case eEliteInstance:
	case eCauchemarInstance:
	case eExpInstance:
	case eMoneyInstance1:
		{
			if(nowTime - m_leaveTime >= m_data->m_nDelTime)
			{
				m_process = eOver;
			}

			break;
		}
	case eCompeteInstance:
		{
			Over();
			break;
		}
	default :
		{
			break;
		}
	}
}

void CGameDuplicate::Failed()
{
	switch(m_data->m_eType)
	{
	case eCompeteInstance:
		{
			Smart_Ptr<Player> player;
			PlayerMessage::GetInstance()->GetPlayerByCharId(player,m_owner);
			if(player)
			{
				player->GetCDMgr()->StartCD(ArenaLoader::GetInstance()->GetChannelType(),
						ArenaLoader::GetInstance()->GetChallengeCDTime()
						);
//				player->GetArenaMgr()->BattleOver(m_paramf, false);
				player->SynchPlayerCompeteInfoToWS(m_params);

				ArenaOverArgs tArgs( player, false);
				player->FireEvent(PLAYER_ARENA_OVER, tArgs);
			}

			break;
		}
	default :
		{
			break;
		}
	}

	m_process = eAward;
}

//bool CGameDuplicate::GenerateRandomAward(int num)
//{
//	m_AwardList.clear();
//
//	//总共9张牌
//	for(int i = 0; i < num; ++i)
//	{
//		DropStaticInfo tempItem;
//		DropItemLoader::GetInstance()->GetOneDropItem(m_data->m_nRandomID,tempItem);
//
//		AwardFlops item;
//		item.m_itemid   = tempItem.m_ItemID;
//		item.m_Counts   = tempItem.m_Counts;
//		item.m_BindType = tempItem.m_BindType;
//
//		m_AwardList.push_back(item);
//	}
//
//	return true;
//}

void CGameDuplicate::Award()
{
	switch(m_data->m_eType)
	{
	case eCompeteInstance:
		{
			Smart_Ptr<Player> player;
			PlayerMessage::GetInstance()->GetPlayerByCharId(player,m_owner);
			if(player)
			{
				int exp = 0;
				MapInstanceProto::GMToClientCompeteOver toClient;

				int lv = player->getAttManage()->getValue<int>(eCharLv);

				UpgradeInfo nowInfo;

				UpgradeLoader::GetInstance()->GetUpgradeByLv(lv, nowInfo);
				if(nowInfo.lv == 0)
				{
					LOG_ERROR(FILEINFO, "get upgrade info error and upgrade failed");

					return;
				}

				if(m_isSuccess)
				{
					exp = ArenaLoader::GetInstance()->GetChallengeWin(nowInfo.factoryExp);
				}
				else
				{
					exp = ArenaLoader::GetInstance()->GetChallengeLose(nowInfo.factoryExp);
				}

				int credit = ArenaLoader::GetInstance()->GetChallengeCredit(m_isSuccess);

				player->AddExp(exp);
				int totalCredit = player->ChangeCredit(credit, true);
				toClient.set_credit(credit);
				toClient.set_exp(exp);
				toClient.set_issuccess(m_isSuccess);
				player->SynCharAttribute(eCredit, totalCredit);
				player->SynchCharAttributeToWS(eCredit, totalCredit);

				QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(), &toClient, MSG_SIM_GM2C_COMPETE_ISOVER);
			}
			break;
		}
	case ePersonalInstance:
	case eEliteInstance:
	case eCauchemarInstance:
		{
			Smart_Ptr<Player> player = GetPlayer(m_owner);
			if(player)
			{
				if(m_isSuccess)
				{
					player->GetInstanceMgr()->AddPassInstance(m_data->m_nMapID,1,m_data->m_eType,m_isSuccess);

					PlayerInstancePassEventArgs tArgs(player,m_data->m_nMapID);

					player->FireEvent(PLAYER_PASS_INSTANCE,tArgs);
					FireGlobalEvent(PLAYER_GLOBE_PASSINSTANCE, tArgs);
				}

				player->GetInstanceMgr()->SendAwardToPlayer(m_isSuccess,m_data->m_nMapID,m_createTime,CUtil::GetNowSecond());
			}

			break;
		}
	case eExpInstance:
	case eMoneyInstance1:
		{
			Smart_Ptr<Player> player = GetPlayer(m_owner);
			if(player)
			{
				if(m_isSuccess)
				{
					player->GetInstanceMgr()->AddPassInstance(m_data->m_nMapID,1,m_data->m_eType);

					PlayerInstancePassEventArgs tArgs(player,m_data->m_nMapID);

					player->FireEvent(PLAYER_PASS_INSTANCE,tArgs);
					FireGlobalEvent(PLAYER_GLOBE_PASSINSTANCE, tArgs);
				}

				player->GetInstanceMgr()->SendAwardToPlayer(m_isSuccess,m_data->m_nMapID,m_createTime,CUtil::GetNowSecond());
			}

			break;
		}
	case eVipInstance:
		{
			Smart_Ptr<Player> player = GetPlayer(m_owner);
			if(player)
			{
				if(m_isSuccess)
				{
					player->GetInstanceMgr()->AddPassInstance(m_data->m_nMapID,1,m_data->m_eType,m_isSuccess);
				}

				//发送结算
				player->GetInstanceMgr()->SendAwardToPlayer(m_isSuccess,m_data->m_nMapID,m_createTime,CUtil::GetNowSecond());
			}

			//清除怪物及事件
			ClearAll();
			break;
		}
	default :
		{
			break;
		}
	}

	m_process = eDuplicateChangeMap;

	m_awardTime = CUtil::GetNowSecond();
}

bool CGameDuplicate::Update()
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
			DestoryRanking();

			return false;
		}
	default :
		{
			break;
		}
	}

	return true;
}

bool CGameDuplicate::JoinHandle(Smart_Ptr<Object> & obj)
{
	vector<int> vec;

	MapInstanceProto::SimInstanceUpdate toClient;

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		switch(m_data->m_eType)
		{
		case ePersonalInstance:
		case eEliteInstance:
		case eCauchemarInstance:
		case eExpInstance:
		case eMoneyInstance1:
		{
			if(m_process != eCreated)
			{
				INTVMPNSTERMAP::iterator itrEnd = m_allMonster.end();
				for(INTVMPNSTERMAP::iterator itr = m_allMonster.begin(); itr != itrEnd; ++itr)
				{
					toClient.add_remainmonsterid(itr->first);
					toClient.add_remainmonsternum(itr->second.size());
				}

				toClient.set_remaintime(m_createTime + m_data->m_nLimitTime - CUtil::GetNowSecond());

				QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GT_UPDATEINSTANCE);
			}

			break;
		}
		case eCompeteInstance:
			{


				//把血和法加满
				vector<int> vec;
				player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));
				player->SetCurMP(player->getAttManage()->getValue<int>(eCharMPMax));
				vec.push_back(eCharHP);
				vec.push_back(eCharMP);
				player->SynCharAttribute(vec);

				if(m_process == eCreated)
				{
					player->GetArenaMgr()->RedChallCnt();
					Smart_Ptr<CounterService> & counter = player->GetCounterService();
					if(counter)
					{
//						player->GetArenaMgr()->RedChallCnt();
						counter->IncreasePlayerCounter(COMPETE_INSTANCE_COUNTER, 1);
						counter->IncreasePlayerCounter(ARENACHALLROBOT_COUNT, 1);
					}

					m_arenaBeginTime = TimerInterface::GetInstance()->RegTimePoint(&CGameDuplicate::ArenaMonsterBegin, this, 0, 0, 0, 0, ArenaLoader::GetInstance()->GetMonCountDown());
				}

				break;
			}
		default :
			{
				break;
			}
		}

		if(vec.size() > 0)
		{
			player->SynCharAttribute(vec);
		}

		if(!m_monsterDeadEvent.IsValid())
		{
			m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &CGameDuplicate::HandleMonsterDeadEvent);
		}

		if(!m_playerDeadEvent.IsValid())
		{
			m_playerDeadEvent = RegGlobalEventPtr(CREATURE_DEAD_EVENT_TYPE, this, &CGameDuplicate::HandlePlayerDeadEvent);
		}

		if(!m_leaveMapEvent.IsValid())
		{
			m_leaveMapEvent = RegGlobalEventPtr(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, this, &CGameDuplicate::HandleLeaveMapEvent);
		}

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		if(m_process == eCreated)
			m_process = ePlayerWork;
	}

	return true;
}

int CGameDuplicate::Join(Smart_Ptr<Object> & obj)
{
	if(!JoinHandle(obj))
		return -1;

	switch(m_data->m_eType)
	{
	case eVipInstance:
	case ePersonalInstance:
	case eCompeteInstance:
	case eEliteInstance:
	case eCauchemarInstance:
	case eExpInstance:
	case eMoneyInstance1:
		{
			CGameZone::Join(obj);

			break;
		}
	default :
		{
			break;
		}
	}

	return 0;
}

bool CGameDuplicate::HandleLeaveMapEvent(const EventArgs & e)
{
	PlayerRemoveMemberEventArgs *pArgs = (PlayerRemoveMemberEventArgs *) (&e);
	if(pArgs->m_pObj.Get()==NULL)
		return true;

	if(pArgs->m_eMethod != GROUP_REMOVEMETHOD_OUT)
		return true;
	if(pArgs->m_pObj->GetMapID() != m_sceneID)
		return true;

	switch(GET_MAP_TYPE(m_sceneID))
	{
	case eVipInstance:
	case ePersonalInstance:
	case eEliteInstance:
	case eCauchemarInstance:
	case eExpInstance:
	case eMoneyInstance1:
		{
			if(pArgs->m_pObj->GetID() != m_owner)
				return true;

			break;
		}
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
  pArgs->m_pObj->SetDataFlag(eBaseInfo);

	return true;
}

bool CGameDuplicate::HandleMonsterDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);
	if(tArgs->m_myself.Get()==NULL)
		return true;

	if(GetSceneID() != tArgs->m_myself->GetMapID())
		return true;


	Smart_Ptr<Monster> mon = tArgs->m_myself;
	LOG_DEBUG(FILEINFO, "CGameDuplicate::HandleMonsterDeadEvent \t %lld", mon->GetID());
	switch(m_data->m_eType)
	{
	case eCompeteInstance:
		{
			if(tArgs->m_other.Get()==NULL)
				return true;

			if(tArgs->m_other->GetType() != ePlayer && tArgs->m_other->GetType() !=  ePet)
				return true;

			Smart_Ptr<Player>  player;
			Smart_Ptr<GirlWar>  girlwar;

			if(tArgs->m_other->GetType() == ePet)
			{
				girlwar = tArgs->m_other;

				player  = girlwar->GetOwner();
			}
			else
			{
				player = tArgs->m_other;
			}

			if(mon->GetQuality() == eCompeteMonster && m_owner == player->GetID())
				m_process = eWinFight;

			break;
		}
	case eVipInstance:
	case ePersonalInstance:
	case eEliteInstance:
	case eCauchemarInstance:
	case eExpInstance:
	case eMoneyInstance1:
		{
			if(IsAllMonsterDead())
			{
				m_process = eWinFight;
			}
			break;
		}
	break;
	default :
		{
			break;
		}
	}

	return true;
}

bool CGameDuplicate::HandlePlayerDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	PlayerKilledEventArgs *tArgs = (PlayerKilledEventArgs *)(&e);
	if(tArgs->m_dead.Get()==NULL || tArgs->m_killer.Get()==NULL)
		return true;

	if(GetSceneID() != tArgs->m_dead->GetMapID())
		return true;

	switch(m_data->m_eType)
	{
	case eCompeteInstance:
		{
			if(tArgs->m_killer->GetType() == eMonster)
			{
				Smart_Ptr<Monster> mon = tArgs->m_killer;
				if(mon->GetQuality() == eCompeteMonster && m_owner == tArgs->m_dead->GetID())
				{
					m_process = eFailed;
				}
			}

			break;
		}
	default :
		{
			break;
		}
	}

	return true;
}

int CGameDuplicate::Move(Smart_Ptr<Object> &obj)
{
	switch(m_data->m_eType)
	{
	case eVipInstance:
	case ePersonalInstance:
	case eCompeteInstance:
	case eEliteInstance:
	case eCauchemarInstance:
	case eExpInstance:
	case eMoneyInstance1:
		{
			CGameZone::Move(obj);
			break;
		}
	default :
		{
			break;
		}
	}

	return 0;
}

void CGameDuplicate::LeaveHandle(Smart_Ptr<Object> & obj)
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

		switch(m_data->m_eType)
		{
		case eVipInstance:
		case ePersonalInstance:
		case eEliteInstance:
		case eCauchemarInstance:
		case eExpInstance:
		case eMoneyInstance1:
		{
			if(m_process == eWinFight)
			{
				Win();
				Award();
			}

			if(m_process == eAward && m_isSuccess)
			{
				Award();
			}
			break;
		}
		case eCompeteInstance:
			{
				if(m_process < eWinFight)
				{
					player->GetCDMgr()->StartCD(ArenaLoader::GetInstance()->GetChannelType(),
							ArenaLoader::GetInstance()->GetChallengeCDTime()
							);
				}

				break;
			}
		default :
			{
				break;
			}
		}
	}
}

void CGameDuplicate::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);

	switch(m_data->m_eType)
	{
	case ePersonalInstance:
	case eCompeteInstance:
	case eEliteInstance:
	case eCauchemarInstance:
	case eExpInstance:
	case eMoneyInstance1:
		{
			CGameZone::Leave(obj, isDel, delFromMap);
			break;
		}
	default :
		{
			break;
		}
	}

	if(GetPlayerCount() <= 0)
	{
		m_monsterDeadEvent.SetNull();
		m_playerDeadEvent.SetNull();
		m_leaveTime = CUtil::GetNowSecond();
	}
}

void CGameDuplicate::LeaveExame(Smart_Ptr<Player> & player)
{
	switch(m_data->m_eType)
	{
	case eVipInstance:
	case ePersonalInstance:
	case eEliteInstance:
	case eCauchemarInstance:
	case eExpInstance:
	case eMoneyInstance1:
		{
			if(GetPlayerCount() <= 1)
			{//复活的话，只剩于一个玩家，这个时候，还没有退出地图，所以为1
				m_process = eOver;
			}

			break;
		}
	default :
		{
			break;
		}
	}
}

bool CGameDuplicate::IsCanEnter(Smart_Ptr<Player> & player)
{
	//传送阶段，就不允许进入了
	if(m_process >= eDuplicateChangeMap)
		return false;

	//玩家死了不入传入地图
	if(player->IsDead())
	{
		return false;
	}

	switch(m_data->m_eType)
	{
	case ePersonalInstance:
	case eCompeteInstance:
	case eVipInstance:
	case eEliteInstance:
	case eCauchemarInstance:
	case eExpInstance:
	case eMoneyInstance1:
		{
			//判断拥有者
			if(player->GetID() != m_owner)
				return false;

			break;
		}
	default:
		return false;
	}

	return true;
}

int CGameDuplicate::IsCanEnter(int64 charid, int fight, int clanid)
{
	return 0;
}

//随机进入副本
void CGameDuplicate::RandJoinMap()
{

}
//帮派被解散了
void CGameDuplicate::ClanDelete(int clanid)
{

}

void CGameDuplicate::IncreaseDroNum()
{
	m_nDigNum +=1;
}

bool CGameDuplicate::IsMaxDroNum()
{
	if(m_nDigNum < TreasonKingLoader::GetInstance()->GetDigNum())
	{
		return  false;
	}
	return true;
}

void CGameDuplicate::GetAllMonsterPosInfo(PlayerSynch::RetInstanceMonsterPos &toClient)
{
	INTVMPNSTERMAP::iterator it = m_allMonster.begin();
	for(; it!=m_allMonster.end(); ++it)
	{
		vector<Smart_Ptr<Monster> >::iterator itVec = it->second.begin();
		for(; itVec!=it->second.end(); ++itVec)
		{
			if((*itVec)->IsDead())
			{
				continue;
			}

			PlayerSynch::InstanceMonsterPosInfo *info = toClient.add_posinfo();
			info->set_monid(it->first);
			info->set_monkey((*itVec)->GetKey());
			int tempXY = ((*itVec)->GetXPos() << 16) | (*itVec)->GetYPos();
			info->set_monxypos(tempXY);
		}
	}

}

bool CGameDuplicate::ForceAllPlayerChangeMap()
{
	vector<Smart_Ptr<Player> > vec;
	GetAllPlayers(vec);

	for(size_t i=0; i<vec.size(); ++i)
	{
		if(vec[i].Get()==NULL)
			continue;

		map<int64,OldMapInfo>::iterator itMap = m_oldMapPoint.find(vec[i]->GetID());
		if(itMap != m_oldMapPoint.end())
		{
			PlayerMessage::GetInstance()->ForceChangeMap(vec[i], itMap->second.mapid, itMap->second.xpos, itMap->second.ypos);
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
				sceneid = CREATE_MAP(eStaticMap, 1, 1, sMapInfo->mapid, 1);
			}

			PlayerMessage::GetInstance()->ForceChangeMap(vec[i], sceneid, sMapInfo->reliveXPos, sMapInfo->reliveYPos);
		}
	}

	return true;
}

int CGameDuplicate::SetPlayerInstanceFlops(int64 charid,int flopid)
{
	return 0;
}

bool CGameDuplicate::IsAttacked(int64 fight, int64 attacked)
{
	return true;
}


void CGameDuplicate::DestoryRanking()
{
	switch(m_data->m_eType)
	{
	case eCompeteInstance:
		{
			MapInstanceProto::SysInstanceRanking toWorld;
			Smart_Ptr<SvrItem> worldSvr;
			ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(m_owner),worldSvr);
			if(worldSvr)
			{
				toWorld.set_mapid(m_data->m_nMapID);
				toWorld.set_type(m_data->m_eType);
				toWorld.set_charid(m_paramf);
				Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_SYSINSTANCERANK, &toWorld, worldSvr.Get(), SimpleMessage);
				Message_Facade::Send(messRet);
			}

			break;
		}
	default:
		return;
	}

}

void CGameDuplicate::ArenaMonsterBegin(void* arg)
{
	//请求挑战玩家的信息
	if(!IsHaveMonster())
	{
		ServerReturn::ServerRetBoolInt meContent;
		Smart_Ptr<SvrItem> dbSvr;

//					RobotFakeData fake;
//					if (player->GetArenaMgr()->IsFakeRank(&fake))
//					{
//						meContent.set_retf(true);
//						meContent.set_rets(fake.GetRankByRank(m_paramf, true));
//					}
//					else
//					{
//						meContent.set_retf(false);
//						meContent.set_rets(m_paramf);
//					}
		meContent.set_rets(m_paramf);

		ServerConHandler::GetInstance()->GetDBServerBySvrID(GET_SERVER_CHARID(m_owner),  dbSvr);
		if (dbSvr)
		{
			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GM2DB_ARENAINFO, &meContent, dbSvr.Get(), Request);
			messRet->SetMessageTime(m_owner);
			messRet->SetAct(new IntAct(m_owner));
			Message_Facade::Send(messRet);
		}
	}
}


//结算时移除怪物及事件订阅
void CGameDuplicate::ClearAll()
{
	if(m_monsterDeadEvent.IsValid())
	{
		m_monsterDeadEvent.SetNull();
	}

	const INTOBJECTMAP ref = GetAllObjects();
	INTOBJECTMAP::const_iterator it = ref.begin();
	for(; it != ref.end(); ++it)
	{
		if(it->second.Get()==NULL || it->second->GetType()!=eMonster)
				continue;

		Smart_Ptr<Monster> mon = it->second;
		if(mon->IsDead())
				continue;

		mon->ClearEnemyList();
		mon->SetDead(NULL);
	}

}

int CGameDuplicate::GetClanIntegralRanking(Smart_Ptr<Player>& player,ClanPackage::RetKillRankingInfo& info)
{
	return 0;
}



