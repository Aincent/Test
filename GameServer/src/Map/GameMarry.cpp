/*
 * GameMarry.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: root
 */

#include "GameMarry.h"
#include "util.h"
#include "Log/Logger.h"
#include "../Object/Player/Player.h"
#include "DuplicateMgr.h"
#include "CounterDefine.h"
#include "../ServerEventDefine.h"
#include "../QuickMessage.h"
#include "../Object/Player/PlayerMessage.h"
#include "../ServerManager.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageCommonRet.h"
#include "../Marry/MarryLoader/MarryLoader.h"
#include "../Object/ItemDrop/DropItemMgr.h"
#include "../Marry/MarryMessage.h"
#include "FileLoader/GameParaLoader.h"

CGameMarry::CGameMarry(int64 onlyid, int64 marryId, MarryMessInfo::MarryInstance& param):CGameZone(onlyid), nfFireworks(0),nfLatern(0),nfToast(0),nfMeteor(0),
nwFireworks(0),nwLatern(0),nwToast(0),nwMeteor(0),m_owner(marryId),m_data(0),m_process(eCreate),m_createTime(CUtil::GetNowSecond()),m_leaveTime(0),m_startTime(CUtil::GetNowSecond()),m_cdTime(0)
{
	m_paramf = param.retf();
	m_params = param.rets();
	m_fCharId = param.fchaid();
	m_wCharId = param.wchaid();

	for(int i = 0; i < param.rett_size(); i++)
	{
		m_vInviter.push_back(param.rett(i));
	}

	const DWORD PROCCESS_TIME = GameParaLoader::GetInstance()->getProcessTime();

	m_updateID = TimerInterface::GetInstance()->RegRepeatedTime(&CGameMarry::FlushProcessState, this, 0, 0, 0, 0,PROCCESS_TIME, PROCCESS_TIME);
}

CGameMarry::~CGameMarry()
{
	if(GetPlayerCount() > 0)
	{
		LOG_FATAL(FILEINFO, "duplicate destroy but player(count=%d) is in the map", GetPlayerCount());
	}

	if(m_updateID.IsValid())
	{
		m_updateID.SetNull();
	}
}


int CGameMarry::Init()
{
	m_data = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(GetSceneID()));
	if(!m_data)
		return -1;

	return CGameZone::Init();
}

bool CGameMarry::Update()
{
	switch(m_process)
	{
	case eCreate:
		{
			break;
		}
	case eFireworks:
		{
			break;
		}
	case eLatern:
		{
			break;
		}
	case eToast:
		{
			break;
		}
	case eMeteor:
		{
			break;
		}
	case eDuplicateChange:
		{
			int64 nowTime = CUtil::GetNowSecond();

			//到副本结束时间就强制传送玩家出副本
			//比需要在10秒内传送出去，不然就不管了
			if(nowTime - m_createTime >= m_data->m_nLimitTime)
			{
				ForceAllPlayerChangeMap();
				m_process = eChangeWait;
			}

			break;
		}
	case eChangeWait:
		{
			int64 nowTime = CUtil::GetNowSecond();

			if(nowTime - m_createTime >= m_data->m_nLimitTime + 10 * 1000)
			{
				m_process = eEnd;
			}

			break;
		}
	case eEnd:
		{
			return false;
			break;
		}
	default:
		{
			return false;
		}
	}

	return true;
}

bool CGameMarry::JoinHandle(Smart_Ptr<Object> & obj)
{
	vector<int> vec;

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		if(player->GetID() == m_fCharId || player->GetID() == m_wCharId)
		{
//			player->GetFashionMgr()->AddFashion(MarryLoader::GetInstance()->GetFashionId(m_paramf));
		}
	}


	if(!m_leaveMapEvent.IsValid())
	{
		m_leaveMapEvent = RegGlobalEventPtr(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, this, &CGameMarry::HandleLeaveMapEvent);
	}


	return true;
}


int CGameMarry::Join(Smart_Ptr<Object> & obj)
{
	if(!JoinHandle(obj))
		return -1;

	CGameZone::Join(obj);

	return 0;
}

void CGameMarry::LeaveExame(Smart_Ptr<Player> & player)
{

}

void CGameMarry::LeaveHandle(Smart_Ptr<Object> & obj)
{
	vector<int> vecType;
	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		if(player->getAttManage()->getValue<int>(eCharHP) <= 0)
		{
			CharBattle::RevivalData req;

			player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));

			req.set_key(player->GetKey());
			req.set_hp(player->getAttManage()->getValue<int>(eCharHP));
			req.set_mp(player->getAttManage()->getValue<int>(eCharMP));

			player->SendSynchMessage(&req, MSG_SIM_GT2C_REVIVAL);
		}
	}
}

void CGameMarry::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);
	CGameZone::Leave(obj, isDel, delFromMap);
	if(GetPlayerCount() <= 0)
	{
		m_leaveTime = CUtil::GetNowSecond();
	}
}

bool CGameMarry::IsCanEnter(Smart_Ptr<Player> & player)
{
	//玩家死了不入传入地图
	if(player->IsDead())
	{
		return false;
	}

	if(player->GetID() == m_fCharId || player->GetID() == m_wCharId)
	{
		return true;
	}

	if(IsCanFire(player->GetID()))
	{
		return true;
	}

	return false;
}

void CGameMarry::Over()
{
	m_process = eDuplicateChange;
}

void CGameMarry::CountEx()
{
	int64 nowTime = CUtil::GetNowSecond();
	if(nowTime - m_leaveTime >= m_data->m_nDelTime)
	{
		m_process = eEnd;
	}
}

bool CGameMarry::ForceAllPlayerChangeMap()
{
	vector<Smart_Ptr<Player> > vec;
	GetAllPlayers(vec);

	for(size_t i=0; i<vec.size(); ++i)
	{
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

bool CGameMarry::IsCanFire(int64 charId)
{
	vector<int64>::iterator itr = m_vInviter.begin();
	for(; itr != m_vInviter.end(); ++itr)
	{
		if(*itr == charId)
		{
				return true;
		}
	}

	return false;
}

int CGameMarry::SetCount(int64 charId,int type)
{
	int ret = 0;
//	int64 time = m_cdTime - CUtil::GetNowSecond();
//	MarryMessInfo::GMToClientCelebration toClient;
//	const DWORD PROCCESS_TIME = GameParaLoader::GetInstance()->getProcessTime();
//	if(charId == m_fCharId)
//	{
//		switch(type)
//		{
//		case eFirs:
//			{
//				if(nfFireworks > nwFireworks)
//				{
//					ret  = eMarryCeleReady;
//				}
//				else if(nfFireworks >= MarryLoader::GetInstance()->GetReleaseById(m_paramf,type))
//				{
//					ret = eMarryCelebrationFull;
//				}
//				else if(time > 0)
//				{
//					ret = eCDInTime;
//				}
//				else
//				{
//					++nfFireworks;
//
//					if(nfFireworks > nwFireworks)
//					{
//						INT64PLAYERMAP::iterator itr = m_player.find(m_wCharId);
//						if(itr != m_player.end())
//						{
//							toClient.set_type(type);
//							QuickMessage::GetInstance()->AddSendMessage(itr->second->GetID(), itr->second->GetChannelID(), &toClient, MSG_SIM_GM2C_CELE_READY);
//						}
//					}
//				}
//
//				break;
//			}
//		case eLight:
//			{
//				if(nfLatern > nwLatern)
//				{
//					ret  = eMarryCeleReady;
//				}
//				else if(nfLatern >= MarryLoader::GetInstance()->GetReleaseById(m_paramf,type))
//				{
//					ret = eMarryCelebrationFull;
//				}
//				else if(time > 0)
//				{
//					ret = eCDInTime;
//				}
//				else
//				{
//					++ nfLatern;
//
//					if(nfLatern > nwLatern)
//					{
//						INT64PLAYERMAP::iterator itr = m_player.find(m_wCharId);
//						if(itr != m_player.end())
//						{
//							toClient.set_type(type);
//							QuickMessage::GetInstance()->AddSendMessage(itr->second->GetID(), itr->second->GetChannelID(), &toClient, MSG_SIM_GM2C_CELE_READY);
//						}
//					}
//				}
//
//				break;
//			}
//		case eWine:
//			{
//				if(nfToast > nwToast)
//				{
//					ret  = eMarryCeleReady;
//				}
//				else if(nfToast >= MarryLoader::GetInstance()->GetReleaseById(m_paramf,type))
//				{
//					ret = eMarryCelebrationFull;
//				}
//				else if(time > 0)
//				{
//					ret = eCDInTime;
//				}
//				else
//				{
//					++ nfToast;
//
//					if(nfToast > nwToast)
//					{
//						INT64PLAYERMAP::iterator itr = m_player.find(m_wCharId);
//						if(itr != m_player.end())
//						{
//							toClient.set_type(type);
//							QuickMessage::GetInstance()->AddSendMessage(itr->second->GetID(), itr->second->GetChannelID(), &toClient, MSG_SIM_GM2C_CELE_READY);
//						}
//					}
//				}
//
//				break;
//			}
//		case eStar:
//			{
//				if(nfMeteor > nwMeteor)
//				{
//					ret  = eMarryCeleReady;
//				}
//				else if(nfMeteor >= MarryLoader::GetInstance()->GetReleaseById(m_paramf,type))
//				{
//					ret = eMarryCelebrationFull;
//				}
//				else if(time > 0)
//				{
//					ret = eCDInTime;
//				}
//				else
//				{
//					++ nfMeteor;
//
//					if(nfMeteor > nwMeteor)
//					{
//						INT64PLAYERMAP::iterator itr = m_player.find(m_wCharId);
//						if(itr != m_player.end())
//						{
//							toClient.set_type(type);
//							QuickMessage::GetInstance()->AddSendMessage(itr->second->GetID(), itr->second->GetChannelID(), &toClient, MSG_SIM_GM2C_CELE_READY);
//						}
//					}
//				}
//
//				break;
//			}
//		}
//	}
//	else if(charId == m_wCharId)
//	{
//		switch(type)
//		{
//		case eFirs:
//			{
//				if(nwFireworks > nfFireworks)
//				{
//					ret  = eMarryCeleReady;
//				}
//				else if(nwFireworks >= MarryLoader::GetInstance()->GetReleaseById(m_paramf,type))
//				{
//					ret = eMarryCelebrationFull;
//				}
//				else if(time > 0)
//				{
//					ret = eCDInTime;
//				}
//				else
//				{
//					++nwFireworks;
//
//					if(nwFireworks > nfFireworks)
//					{
//						INT64PLAYERMAP::iterator itr = m_player.find(m_fCharId);
//						if(itr != m_player.end())
//						{
//							toClient.set_type(type);
//							QuickMessage::GetInstance()->AddSendMessage(itr->second->GetID(), itr->second->GetChannelID(), &toClient, MSG_SIM_GM2C_CELE_READY);
//						}
//					}
//				}
//
//				break;
//			}
//		case eLight:
//			{
//				if(nwLatern > nfLatern)
//				{
//					ret  = eMarryCeleReady;
//				}
//				else if(nwLatern >= MarryLoader::GetInstance()->GetReleaseById(m_paramf,type))
//				{
//					ret = eMarryCelebrationFull;
//				}
//				else if(time > 0)
//				{
//					ret = eCDInTime;
//				}
//				else
//				{
//					++ nwLatern;
//
//					if(nwLatern > nfLatern)
//					{
//						INT64PLAYERMAP::iterator itr = m_player.find(m_fCharId);
//						if(itr != m_player.end())
//						{
//							toClient.set_type(type);
//							QuickMessage::GetInstance()->AddSendMessage(itr->second->GetID(), itr->second->GetChannelID(), &toClient, MSG_SIM_GM2C_CELE_READY);
//						}
//					}
//				}
//
//				break;
//			}
//		case eWine:
//			{
//				if(nwToast > nfToast)
//				{
//					ret  = eMarryCeleReady;
//				}
//				else if(nwToast >= MarryLoader::GetInstance()->GetReleaseById(m_paramf,type))
//				{
//					ret = eMarryCelebrationFull;
//				}
//				else if(time > 0)
//				{
//					ret = eCDInTime;
//				}
//				else
//				{
//					++ nwToast;
//
//					if(nwToast > nfToast)
//					{
//						INT64PLAYERMAP::iterator itr = m_player.find(m_fCharId);
//						if(itr != m_player.end())
//						{
//							toClient.set_type(type);
//							QuickMessage::GetInstance()->AddSendMessage(itr->second->GetID(), itr->second->GetChannelID(), &toClient, MSG_SIM_GM2C_CELE_READY);
//						}
//					}
//				}
//
//				break;
//			}
//		case eStar:
//			{
//				if(nwMeteor > nfMeteor)
//				{
//					ret  = eMarryCeleReady;
//				}
//				else if(nwMeteor >= MarryLoader::GetInstance()->GetReleaseById(m_paramf,type))
//				{
//					ret = eMarryCelebrationFull;
//				}
//				else if(time > 0)
//				{
//					ret = eCDInTime;
//				}
//				else
//				{
//					++ nwMeteor;
//
//					if(nwMeteor > nfMeteor)
//					{
//						INT64PLAYERMAP::iterator itr = m_player.find(m_fCharId);
//						if(itr != m_player.end())
//						{
//							toClient.set_type(type);
//							QuickMessage::GetInstance()->AddSendMessage(itr->second->GetID(), itr->second->GetChannelID(), &toClient, MSG_SIM_GM2C_CELE_READY);
//						}
//					}
//				}
//
//				break;
//			}
//		}
//	}
//	else
//	{
//		ret = eMarryFireError;
//	}
//
//	if(ret == 0)
//	{
//		switch(type)
//		{
//		case eFirs:
//			{
//				if(nfFireworks == nwFireworks)
//				{
//					ReleaseProp(eFirs);
//					m_cdTime = CUtil::GetNowSecond() + MarryLoader::GetInstance()->GetReleaseCDById(m_paramf,type) * 1000;
//				}
//
//				if(nfFireworks >= MarryLoader::GetInstance()->GetReleaseById(m_paramf,type) && nfFireworks == nwFireworks)
//				{
//					FlushProcessState(NULL);
//
//					if(m_updateID.IsValid())
//					{
//						m_updateID.SetNull();
//					}
//
//					m_updateID = TimerInterface::GetInstance()->RegRepeatedTime(&CGameMarry::FlushProcessState, this, 0, 0, 0, 0,PROCCESS_TIME, PROCCESS_TIME);
//				}
//
//				break;
//			}
//		case eLight:
//			{
//				if(nfLatern == nwLatern)
//				{
//					ReleaseProp(eLight);
//					m_cdTime = CUtil::GetNowSecond() + MarryLoader::GetInstance()->GetReleaseCDById(m_paramf,type) * 1000;
//				}
//
//				if(nfLatern >= MarryLoader::GetInstance()->GetReleaseById(m_paramf,type) && nfLatern == nwLatern)
//				{
//					FlushProcessState(NULL);
//
//					if(m_updateID.IsValid())
//					{
//						m_updateID.SetNull();
//					}
//
//					m_updateID = TimerInterface::GetInstance()->RegRepeatedTime(&CGameMarry::FlushProcessState, this, 0, 0, 0, 0,PROCCESS_TIME, PROCCESS_TIME);
//				}
//
//				break;
//			}
//		case eWine:
//			{
//				if(nfToast == nwToast)
//				{
//					ReleaseProp(eWine);
//					m_cdTime = CUtil::GetNowSecond() + MarryLoader::GetInstance()->GetReleaseCDById(m_paramf,type) * 1000;
//				}
//
//				if(nfToast >= MarryLoader::GetInstance()->GetReleaseById(m_paramf,type) && nfToast == nwToast)
//				{
//					FlushProcessState(NULL);
//
//					if(m_updateID.IsValid())
//					{
//						m_updateID.SetNull();
//					}
//
//					m_updateID = TimerInterface::GetInstance()->RegRepeatedTime(&CGameMarry::FlushProcessState, this, 0, 0, 0, 0,PROCCESS_TIME, PROCCESS_TIME);
//				}
//
//				break;
//			}
//		case eStar:
//			{
//				if(nfMeteor == nwMeteor)
//				{
//					ReleaseProp(eStar);
//					m_cdTime = CUtil::GetNowSecond() + MarryLoader::GetInstance()->GetReleaseCDById(m_paramf,type) * 1000;
//				}
//
//				if(nfMeteor >= MarryLoader::GetInstance()->GetReleaseById(m_paramf,type) && nfMeteor == nwMeteor)
//				{
//					FlushProcessState(NULL);
//
//					if(m_updateID.IsValid())
//					{
//						m_updateID.SetNull();
//					}
//
//					m_updateID = TimerInterface::GetInstance()->RegRepeatedTime(&CGameMarry::FlushProcessState, this, 0, 0, 0, 0,PROCCESS_TIME, PROCCESS_TIME);
//				}
//
//				break;
//			}
//		}
//	}

	return ret;
}


void CGameMarry::ReleaseProp(int type)
{

}


void CGameMarry::FlushProcessState(void* arg)
{
	m_startTime = CUtil::GetNowSecond();

	switch(m_process)
	{
	case eCreate:
		{
			m_process = eFireworks;
			break;
		}
	case eFireworks:
		{
			m_process = eToast;
			break;
		}
	case eToast:
		{
			m_process = eLatern;
			break;
		}
	case eLatern:
		{
			m_process = eMeteor;
			break;
		}
	case eMeteor:
		{
			m_process = eDuplicateChange;
			break;
		}
	default:
		{
			m_process = eDuplicateChange;
		}
	}
}

void CGameMarry::GetCeleStaus(int64 charId,MarryMessInfo::GMToClientCeleTimes& info)
{

}

bool CGameMarry::HandleLeaveMapEvent(const EventArgs & e)
{

	PlayerRemoveMemberEventArgs *pArgs = (PlayerRemoveMemberEventArgs *) (&e);

	if(pArgs->m_eMethod != GROUP_REMOVEMETHOD_OUT)
		return true;
	if(pArgs->m_pObj->GetMapID() != m_sceneID)
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
