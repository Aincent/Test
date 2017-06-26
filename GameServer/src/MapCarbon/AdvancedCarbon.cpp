/*
 * AdvancedCarbon.cpp
 *
 *  Created on: 2016年8月12日
 *      Author: root
 */

#include "AdvancedCarbon.h"
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


CAdvancedCarbon::CAdvancedCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):CGameDuplicate(onlyid,charid,paramf,params)
{

}

CAdvancedCarbon::~CAdvancedCarbon()
{

}


int CAdvancedCarbon::Init()
{
	int ret = CGameDuplicate::Init();
	return ret;
}

bool CAdvancedCarbon::Update()
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

int CAdvancedCarbon::Join(Smart_Ptr<Object> & obj)
{
	if(!JoinHandle(obj))
		return -1;

	int ret = CGameZone::Join(obj);
	return ret;
}

bool CAdvancedCarbon::JoinHandle(Smart_Ptr<Object> & obj)
{
	if(m_process >= eDuplicateChangeMap)
		return false;

	MapInstanceProto::SimInstanceUpdate toClient;

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;
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

		if(!m_monsterDeadEvent.IsValid())
		{
			m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &CAdvancedCarbon::HandleMonsterDeadEvent);
		}

		if(!m_leaveMapEvent.IsValid())
		{
			m_leaveMapEvent = RegGlobalEventPtr(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, this, &CAdvancedCarbon::HandleLeaveMapEvent);
		}

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		if(m_process == eCreated)
			m_process = ePlayerWork;
	}

	return true;
}

int CAdvancedCarbon::Move(Smart_Ptr<Object> &obj)
{
	CGameZone::Move(obj);
	return 0;
}

void CAdvancedCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);
	CGameZone::Leave(obj, isDel, delFromMap);
	if(GetPlayerCount() <= 0)
	{
		m_monsterDeadEvent.SetNull();
		m_leaveMapEvent.SetNull();
		m_leaveTime = CUtil::GetNowSecond();
	}
}

void CAdvancedCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
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

		if(m_process == eWinFight)
		{
			Win();
			Award();
		}

		if(m_process == eAward && m_isSuccess)
		{
			Award();
		}
	}
}

void CAdvancedCarbon::LeaveExame(Smart_Ptr<Player> & player)
{
	if(GetPlayerCount() <= 0)
	{
		m_process = eOver;
	}
}

bool CAdvancedCarbon::IsCanEnter(Smart_Ptr<Player> & player)
{
	if(player->GetID() != m_owner)
	{
		return false;
	}

	if(m_process > ePlayerWork)
	{
		return false;
	}

	return true;
}

void CAdvancedCarbon::Win()
{
	m_isSuccess = true;
	SetAllMonsterDead();
	m_process = eAward;
}

void CAdvancedCarbon::Over()
{
	if(IsAllTargetDead(m_data->m_nTarget))
	{
		m_process = eWinFight;
	}
	else
	{
		m_process = eFailed;
	}
}

void CAdvancedCarbon::Count()
{
	int64 nowTime = CUtil::GetNowSecond();
	if(nowTime - m_leaveTime >= m_data->m_nDelTime)
	{
		m_process = eOver;
	}
}

void CAdvancedCarbon::CountEx()
{
	m_process = eOver;
}

void CAdvancedCarbon::Failed()
{
	m_process = eAward;
}

void CAdvancedCarbon::Award()
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

	m_process = eDuplicateChangeMap;
	m_awardTime = CUtil::GetNowSecond();
}

bool CAdvancedCarbon::HandleMonsterDeadEvent(const EventArgs & e)
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

	if(IsAllTargetDead(m_data->m_nTarget))
	{
		m_process = eWinFight;
	}

	return true;
}

bool CAdvancedCarbon::HandleLeaveMapEvent(const EventArgs & e)
{
	PlayerRemoveMemberEventArgs *pArgs = (PlayerRemoveMemberEventArgs *) (&e);
	if(pArgs->m_pObj.Get()==NULL)
		return true;

	if(pArgs->m_eMethod != GROUP_REMOVEMETHOD_OUT)
		return true;
	if(pArgs->m_pObj->GetMapID() != m_sceneID)
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
	pArgs->m_pObj->SetDataFlag(eBaseInfo);

	return true;
}


bool CAdvancedCarbon::IsAllTargetDead(vector<DWORD> vId)
{
	bool bResult = true;
	INTVMPNSTERMAP allMonster = GetMapMonster();
	for(size_t i = 0; i < vId.size(); i++)
	{
		INTVMPNSTERMAP::iterator itr = allMonster.find(vId[i]);
		if(itr != allMonster.end())
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
	}

	return bResult;
}
