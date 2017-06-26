/*
 * ChallengeCarbon.cpp
 *
 *  Created on: 2016年4月25日
 *      Author: root
 */
#include "ChallengeCarbon.h"
#include "../MapInstance/ChallengeMgr.h"
#include "../ServerEventDefine.h"
#include "../Map/SceneManager.h"
#include "../MapInstance/PlayerChallenge.h"

ChallengeCarbon::ChallengeCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):
CGameDuplicate(onlyid,charid,paramf,params)
{
	m_star = 0;
	m_isFitch = false;
}


ChallengeCarbon::~ChallengeCarbon()
{

}


int ChallengeCarbon::Init()
{
	return CGameDuplicate::Init();
}


bool ChallengeCarbon::Update()
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

			if(nowTime - m_awardTime >= 10000 && ChallengeMgr::GetInstance()->GetMaxLayers()==m_paramf)
			{
				ForceAllPlayerChangeMap();
				m_process = eOver;
				return true;
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


int ChallengeCarbon::Join(Smart_Ptr<Object>& obj)
{
	JoinHandle(obj);
	int ret = CGameZone::Join(obj);
	return ret;
}


bool ChallengeCarbon::JoinHandle(Smart_Ptr<Object>& obj)
{
	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;
		//自动把血和法加满

		vector<int> vec;
		player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));
		player->SetCurMP(player->getAttManage()->getValue<int>(eCharMPMax));
		vec.push_back(eCharHP);
		vec.push_back(eCharMP);
		player->SynCharAttribute(vec);

		//初始化场景怪物
		std::vector<ChallengeMonstersPos> infos;
		ChallengeMgr::GetInstance()->GetMonsterPos(m_paramf, infos);
		for(size_t i=0; i<infos.size(); ++i)
		{
			for(size_t j=0; j<infos[i].vPos.size(); ++j)
			{
				CGameZone::CreateMonsterDymic(infos[i].monster, infos[i].vPos[j].first, infos[i].vPos[j].second);
			}
		}

		if(!m_monsterDeadEvent.IsValid())
		{
			m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &ChallengeCarbon::HandleMonsterDeadEvent);
		}

		InitTargets();

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		if(m_process == eCreated)
		{
			m_process = ePlayerWork;
		}

	}

	return true;
}

int ChallengeCarbon::Move(Smart_Ptr<Object>& obj)
{
	return CGameZone::Move(obj);
}


void ChallengeCarbon::Leave(Smart_Ptr<Object>& obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);
	CGameZone::Leave(obj, isDel, delFromMap);
}


void ChallengeCarbon::LeaveHandle(Smart_Ptr<Object>& obj)
{
	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		if(m_isSuccess && !m_isFitch)
		{
			FitchReWard(player);
		}

		int64 mapid = 0;
		int xpos = 0;
		int ypos = 0;
		vector<int> vecType;

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
				if(sMapInfo == 0)
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


void ChallengeCarbon::LeaveExame(Smart_Ptr<Player>& player)
{

}


bool ChallengeCarbon::IsCanEnter(Smart_Ptr<Player>& player)
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


void ChallengeCarbon::Win()
{

}


void ChallengeCarbon::Over()
{
	m_isSuccess = false;
	m_process = eAward;
	Result();
}


void ChallengeCarbon::Count()
{
	int64 nowTime = CUtil::GetNowSecond();
	if(nowTime - m_leaveTime >= m_data->m_nDelTime)
	{
		m_process = eOver;
	}
}


void ChallengeCarbon::CountEx()
{
	m_process = eOver;
}


void ChallengeCarbon::Failed()
{

}


void ChallengeCarbon::Award()
{
	m_process = eDuplicateChangeMap;
	m_awardTime = CUtil::GetNowSecond();
}


bool ChallengeCarbon::HandleMonsterDeadEvent(const EventArgs& e)
{
	if(m_process != ePlayerWork)
		return true;

	TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);
	if(tArgs->m_myself.Get()==NULL)
		return true;

	if(GetSceneID() != tArgs->m_myself->GetMapID())
		return true;

	Smart_Ptr<Monster> mon = tArgs->m_myself;

	AddTarget(mon->GetID());

	return true;
}


bool ChallengeCarbon::HandlePlayerDeadEvent(const EventArgs& e)
{
	return true;
}


int64 ChallengeCarbon::FitchReWard(Smart_Ptr<Player> player)
{
	if(!m_isSuccess)
	{
		return eInstanceInfoError;
	}

	if(m_isFitch)
	{
		return eReceived;
	}

	int64 ret = ChallengeMgr::GetInstance()->FitchReWard(player, m_paramf, m_star);
	if(ret == ePackNoSpaceSendMail || ret == 0)
	{
		m_isFitch = true;
	}

	return ret;
}

void ChallengeCarbon::InitTargets()
{
	const ChallengeLayerInfo * pLayerInfo = ChallengeMgr::GetInstance()->GetLayerInfo(m_paramf);
	if(pLayerInfo == NULL)
		return;

	for(size_t i=0; i<pLayerInfo->targets.size(); ++i)
	{
		m_killMonsters[pLayerInfo->targets[i].first] = make_pair(pLayerInfo->targets[i].second,0);
	}
}

void ChallengeCarbon::AddTarget(int id)
{
	bool isActive = false;
	std::map<int,pair<int,int> >::iterator it = m_killMonsters.find(id);
	if(it != m_killMonsters.end())
	{
		if(it->second.second >= it->second.first)
		{
			return;
		}

		if(++it->second.second >= it->second.first)
		{
			isActive = true;
		}
	}

	if(!isActive)
	{
		return;
	}

	std::map<int,pair<int,int> >::iterator itcheck = m_killMonsters.begin();
	for(; itcheck != m_killMonsters.end(); ++itcheck)
	{
		if(itcheck->second.second < itcheck->second.first)
		{
			isActive = false;
			break;
		}
	}

	if(isActive)
	{
		m_isSuccess = true;
		m_process = eAward;
		Result();
	}

}

void ChallengeCarbon::Result()
{
	Smart_Ptr<Player> player = GetPlayer(m_owner);
	if(!player)
		return;

	DWORD64 elapseTime = CUtil::GetNowSecond()-m_createTime;

	int star = ChallengeMgr::GetInstance()->GetStart(m_paramf, elapseTime);

	m_star = star;

	if(m_isSuccess)
	{
		player->GetChallenge()->UpdateLayer(m_paramf+1);
		player->GetChallenge()->UpdateRecord(m_paramf);

//		player->UpdateUIHighLight(eUIHLHandle_UpAndForceSyn, UIHLType(eUIHLBig_InstanceHall, eUIHLSmall_InstanceHall_CanChallenge), 1, false);

		PlayerInstancePassEventArgs tArgs(player,m_data->m_nMapID);

		player->FireEvent(PLAYER_PASS_INSTANCE,tArgs);
		FireGlobalEvent(PLAYER_GLOBE_PASSINSTANCE, tArgs);
	}

	ChallengeMgr::GetInstance()->SendAwardToPlayer(player, m_isSuccess, m_paramf, star, elapseTime, 0);
}







