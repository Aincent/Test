/*
 * MarryCarbon.cpp
 *
 *  Created on: 2015年11月3日
 *      Author: root
 */

#include "MarryCarbon.h"
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
#include "../Object/AI/AIBrain.h"
#include "../Object/ItemDrop/DropItemAlloc.h"
#include "./FileLoader/InternationalLoader.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/Marry/MarryInstance.pb.h"
#include "../Mail/MailMessage.h"

CMarryCarbon::CMarryCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):CGameDuplicate(onlyid,charid,paramf,params),m_bIsAward(true)
{

}

CMarryCarbon::~CMarryCarbon()
{

}

int CMarryCarbon::Init()
{
	int ret = CGameDuplicate::Init();
	return ret;
}

bool CMarryCarbon::IsCanEnter(Smart_Ptr<Player> & player)
{
	if(m_process >= eDuplicateChangeMap)
		return false;

	if(player->GetID() != m_paramf && player->GetID() != m_params)
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
		req.set_xpos(player->GetXPos());
		req.set_ypos(player->GetYPos());
		req.set_key(player->GetKey());
		req.set_hp(player->getAttManage()->getValue<int>(eCharHP));
		req.set_mp(player->getAttManage()->getValue<int>(eCharMP));

		player->SendSynchMessage(&req, MSG_SIM_GT2C_REVIVAL);
	}

	return true;
}

bool CMarryCarbon::JoinHandle(Smart_Ptr<Object>& obj)
{
	if(m_process >= eDuplicateChangeMap)
		return false;

	MapInstanceProto::SimInstanceUpdate toClient;

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;
		if(player->GetID() != m_paramf && player->GetID() != m_params)
		{
			return false;
		}

		if(!m_monsterDeadEvent.IsValid())
		{
			m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &CMarryCarbon::HandleMonsterDeadEvent);
		}

		if(!m_leaveMapEvent.IsValid())
		{
			m_leaveMapEvent = RegGlobalEventPtr(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, this, &CMarryCarbon::HandleLeaveMapEvent);
		}

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

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		if(m_process == eCreated)
			m_process = ePlayerWork;
	}

	return true;
}

int CMarryCarbon::Join(Smart_Ptr<Object> & obj)
{
	if(!JoinHandle(obj))
		return -1;

	CGameZone::Join(obj);
	return 0;
}

int CMarryCarbon::Move(Smart_Ptr<Object> &obj)
{
	CGameZone::Move(obj);
	return 0;
}

void CMarryCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
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

		player->GetInstanceMgr()->AddPassInstance(m_data->m_nMapID,1,m_data->m_eType,m_isSuccess);
	}
}

void CMarryCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
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

void CMarryCarbon::LeaveExame(Smart_Ptr<Player> & player)
{
	if(GetPlayerCount() <= 0)
	{
		m_process = eAward;
	}
}

bool CMarryCarbon::Update()
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
			DestoryMarryToWs();
			return false;
		}
	default :
		{
			break;
		}
	}

	return true;
}

void CMarryCarbon::Win()
{
	m_isSuccess = true;
	m_process = eAward;
}

void CMarryCarbon::Over()
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

void CMarryCarbon::Count()
{
	m_process = eAward;
}

void CMarryCarbon::CountEx()
{
	m_process = eOver;
}

void CMarryCarbon::Failed()
{
	m_process = eAward;
}

void CMarryCarbon::Award()
{
	Smart_Ptr<Player> player;
	INT64PLAYERMAP::iterator it = m_player.begin();
	for(; it!=m_player.end(); ++it)
	{
		if(it->second.Get()==NULL)
			continue;

		it->second->GetInstanceMgr()->SendAwardToPlayer(m_isSuccess,m_data->m_nMapID,m_createTime,CUtil::GetNowSecond(),0);

		PlayerInstancePassEventArgs tArgs(it->second,m_data->m_nMapID);

		it->second->FireEvent(PLAYER_PASS_INSTANCE,tArgs);
		FireGlobalEvent(PLAYER_GLOBE_PASSINSTANCE, tArgs);
	}

	if(m_isSuccess && m_bIsAward)
	{
		player = GetPlayer(m_paramf);
		if(!player)
		{
			SendAwardToOffLine(m_paramf);
		}

		player = GetPlayer(m_params);
		if(!player)
		{
			SendAwardToOffLine(m_params);
		}

		m_bIsAward = false;
	}

	m_process = eDuplicateChangeMap;
	m_awardTime = CUtil::GetNowSecond();
}

//怪物死亡事件
bool CMarryCarbon::HandleMonsterDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);

	if(tArgs->m_myself.Get()==NULL || GetSceneID() != tArgs->m_myself->GetMapID())
		return true;

	if(tArgs->m_other.Get()==NULL)
		return true;

	Smart_Ptr<Monster> mon = tArgs->m_myself;

	Smart_Ptr<Player>  player;
	Smart_Ptr<GirlWar>  girlwar;

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

	if(IsAllMonsterDead())
	{
		m_process = eWinFight;
	}

	return true;
}

//退出场景事件(如果不希望玩家下次还能进入此场景，最好在此处理)
bool CMarryCarbon::HandleLeaveMapEvent(const EventArgs & e)
{
	PlayerRemoveMemberEventArgs *pArgs = (PlayerRemoveMemberEventArgs *) (&e);
	if(pArgs->m_eMethod != GROUP_REMOVEMETHOD_OUT)
		return true;

	if(pArgs->m_pObj.Get()==NULL || pArgs->m_pObj->GetMapID() != m_sceneID)
		return true;

	if(pArgs->m_pObj->GetID() != m_paramf || pArgs->m_pObj->GetID() != m_params)
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

void CMarryCarbon::DestoryMarryToWs()
{
	MarryInstanceInfo::GMToWSDelMarryCarbon toWorld;
	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(m_params),worldSvr);
	if(worldSvr)
	{
		toWorld.set_charidf(m_paramf);
		toWorld.set_charids(m_params);
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_DEL_MARRY_CARBON, &toWorld, worldSvr.Get(), SimpleMessage);
		Message_Facade::Send(messRet);
	}
}

void CMarryCarbon::SendAwardToOffLine(int64 charid)
{
//	vector<int> vec;
//	vector<int64> goods;
//	vector<DWORD> idList;
//	vector<DWORD> numList;
//	//需统计的真气
//	string name = "";
//	int statisticForce = 0;
//	int exp = 0;
//	int money = 0;
//	int star = 0;
//	float awardrate[3] = {m_data->m_nOneStarRate/100.0,m_data->m_nTwoStarRate/100.0,m_data->m_nThreeStarRate/100.0};
//	unsigned long nTime =  CUtil::GetNowSecond() - m_createTime;
//
//	if(m_data->m_nRateing.size() == 3)
//	{
//		if(nTime <= m_data->m_nRateing[0])
//		{
//			star = 3;
//		}
//		else if(m_data->m_nRateing[0]< nTime &&  nTime <= m_data->m_nRateing[1])
//		{
//			star = 2;
//		}
//		else
//		{
//			star = 1;
//		}
//	}
//
//	float temp = awardrate[star-1];
//
//	idList = m_data->m_nAward;
//	numList = m_data->m_nAwardNum;
//
//
//	switch(star)
//	{
//	case 3:
//		{
//			map<DWORD,DWORD>::iterator itr = m_data->m_nThreestar.begin();
//			for(; itr != m_data->m_nThreestar.end(); ++itr)
//			{
//				idList.push_back(itr->first);
//				numList.push_back(itr->second);
//			}
//			break;
//		}
//
//	case 2:
//		{
//			map<DWORD,DWORD>::iterator itr = m_data->m_nTwostar.begin();
//			for(; itr != m_data->m_nTwostar.end(); ++itr)
//			{
//				idList.push_back(itr->first);
//				numList.push_back(itr->second);
//			}
//			break;
//		}
//	default:
//		{
//			map<DWORD,DWORD>::iterator itr = m_data->m_nOnestar.begin();
//			for(; itr != m_data->m_nOnestar.end(); ++itr)
//			{
//				idList.push_back(itr->first);
//				numList.push_back(itr->second);
//			}
//		}
//	}
//
//	statisticForce = (int)(m_data->m_nAwardSprit * temp);
//	exp = (int)(m_data->m_nAwardExp * temp);
//	money = (int)(m_data->m_nAwardMoney * temp);
//
//	if(statisticForce > 0)
//	{
//		idList.push_back(GameParaLoader::GetInstance()->getExchangeForceItemId());
//		numList.push_back(statisticForce);
//	}
//
//	if(exp > 0)
//	{
//		idList.push_back(GameParaLoader::GetInstance()->getExchangeExpItemId());
//		numList.push_back(exp);
//	}
//
//	if(money > 0)
//	{
//		idList.push_back(GameParaLoader::GetInstance()->getExchangeMoneyItemId());
//		numList.push_back(money);
//	}
//
//	for(size_t j = 0; j < idList.size(); j++)
//	{
//		int64 info = numList[j];
//		info = info << 32;	//物品数量
//		DWORD itemid = idList[j];
//		info = info | itemid;
//		goods.push_back(info);
//
//		if(goods.size()>=5)
//		{
//			string warning1 = InternationalLoader::GetInstance()->GetInterStr(8);
//			MailMessageMgr::GetInstance()->SendSysMail(goods,name,charid,warning1.c_str(),warning1.c_str());
//			goods.clear();
//		}
//	}
//
//	if(goods.size() > 0)
//	{
//		string warning1 = InternationalLoader::GetInstance()->GetInterStr(8);
//		MailMessageMgr::GetInstance()->SendSysMail(goods, name,charid,warning1.c_str(),warning1.c_str());
//	}
}
