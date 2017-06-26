/*
 * HeroCarbon.cpp
 *
 *  Created on: 2016年5月18日
 *      Author: root
 */

#include "HeroCarbon.h"
#include "../ServerEventDefine.h"
#include "../Map/SceneManager.h"
#include "../MapInstance/HeroInfo.h"
#include "../MapInstance/HeroLoader.h"
#include "MessageStruct/ServerReturnBool.pb.h"

HeroCarbon::HeroCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):
CGameDuplicate(onlyid,charid,paramf,params)
{

}

HeroCarbon::~HeroCarbon()
{
	if(m_vipEvent.IsValid())
	{
		m_vipEvent.SetNull();
	}
}

int HeroCarbon::Init()
{
	return CGameDuplicate::Init();
}

bool HeroCarbon::Update()
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

				if(nowTime - m_awardTime >= 10000)
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


int HeroCarbon::Join(Smart_Ptr<Object> & obj)
{
	int ret = CGameZone::Join(obj);
	if(ret != 0)
		return ret;

	JoinHandle(obj);
	return 0;
}


bool HeroCarbon::JoinHandle(Smart_Ptr<Object> & obj)
{
	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;
		InitPlayer(player);

		if(!m_monsterDeadEvent.IsValid())
		{
			m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &HeroCarbon::HandleMonsterDeadEvent);
		}

		if(!m_playerDeadEvent.IsValid())
		{
			m_playerDeadEvent = RegGlobalEventPtr(CREATURE_DEAD_EVENT_TYPE, this, &HeroCarbon::HandlePlayerDeadEvent);
		}

		if(!m_vipEvent.IsValid())
		{
			m_vipEvent = RegEvent(player, PLAYER_UPGRADEVIPLV_EVENT_TYPE, this, &HeroCarbon::HandleUpdateVipLv);
		}

		if(m_process == eCreated)
		{
			m_process = ePlayerWork;
		}

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		//初始化场景怪物
		CreateMonsters(m_paramf);
		InitTargets();

		//推送副本信息
		SendBaseInfo(player);
	}

	return true;
}


int HeroCarbon::Move(Smart_Ptr<Object> &obj)
{
	return CGameZone::Move(obj);
}

void HeroCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);
	CGameZone::Leave(obj, isDel, delFromMap);
}

void HeroCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
{
	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		//remove buffer
		ClearBuffer(player);

		if(m_vipEvent.IsValid())
		{
			m_vipEvent.SetNull();
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

void HeroCarbon::LeaveExame(Smart_Ptr<Player> & player)
{

}

bool HeroCarbon::IsCanEnter(Smart_Ptr<Player> & player)
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

void HeroCarbon::Win()
{

}

void HeroCarbon::Failed()
{

}

void HeroCarbon::Over()
{
	m_isSuccess = false;
	m_process = eAward;
	Result();
}

void HeroCarbon::Count()
{
	m_process = eOver;
}

void HeroCarbon::CountEx()
{
	m_process = eOver;
}

void HeroCarbon::Award()
{
	m_process = eDuplicateChangeMap;
	m_awardTime = CUtil::GetNowSecond();
}

bool HeroCarbon::HandleMonsterDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
			return true;

	TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);
	if(tArgs==NULL || tArgs->m_myself.Get()==NULL)
		return true;

	if(GetSceneID() != tArgs->m_myself->GetMapID())
		return true;

	Smart_Ptr<Monster> mon = tArgs->m_myself;

	AddTarget(mon->GetID());
	return true;
}

bool HeroCarbon::HandlePlayerDeadEvent(const EventArgs & e)
{
	PlayerKilledEventArgs *tArgs = (PlayerKilledEventArgs *)(&e);
	if(tArgs == NULL)
		return true;

	if(tArgs->m_dead.Get()==NULL || tArgs->m_dead->GetType()!= ePlayer)
		return true;

	if(tArgs->m_dead->GetID() != m_owner)
		return true;

	//进入奖励领取状态
	m_process = eAward;
	m_isSuccess = false;

	Result();

	return true;
}

bool HeroCarbon::HandleUpdateVipLv(const EventArgs & e)
{
	const PlayerAddPackSizeEventArgs& args = (const PlayerAddPackSizeEventArgs&)e;
	Smart_Ptr<Player> player = args.m_obj;

	ClearBuffer(player);

	AddBuffer(player);

	return true;
}

void HeroCarbon::InitPlayer(Smart_Ptr<Player>& player)
{
	//自动把血和法加满
	vector<int> vec;
	player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));
	player->SetCurMP(player->getAttManage()->getValue<int>(eCharMPMax));

	vec.push_back(eCharHP);
	vec.push_back(eCharMP);
	player->SynCharAttribute(vec);

	//vip add buffe
	AddBuffer(player);

}

void HeroCarbon::AddBuffer(Smart_Ptr<Player>& player)
{
//	double bufferId = player->GetVipPart()->GetValueByRight(eVipHeroBuffer);
//	if(bufferId == VIP_NOT_HAVE_THIS_RIGHT)
//	{
//		return;
//	}
//
//	const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(int(bufferId));
//	if(info == NULL)
//	{
//		return;
//	}
//
//	Smart_Ptr<CreatureObj> obj = player;
//	if(player->AddBuff(int(bufferId),obj))
//	{
//		m_buffers.push_back(int(bufferId));
//	}
}

void HeroCarbon::ClearBuffer(Smart_Ptr<Player>& player)
{
	for(size_t i=0; i<m_buffers.size(); ++i)
	{
		player->RemoveBuffById(m_buffers[i]);
	}
	m_buffers.clear();
}

void HeroCarbon::CreateMonsters(int layer)
{
	const HeroLayerInfo* pInfo = HeroLoader::GetInstance()->GetHeroLayerInfo(layer);
	if(pInfo==NULL)
	{
		LOG_ERROR(FILEINFO, "HeroCarbon::CreateMonsters Error, layer=%d", layer);
		return;
	}

	const HeroConfig& config = HeroLoader::GetInstance()->GetHeroConfig();

	CGameZone::CreateMonsterDymic(pInfo->monster, config.posX, config.posY);
}

void HeroCarbon::SendBaseInfo(Smart_Ptr<Player>& player)
{
	//倒计时
	const HeroConfig& config = HeroLoader::GetInstance()->GetHeroConfig();

	//结束点
	DWORD64 endTime = 0;
	InstanceMapEntry* pInstance = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(m_sceneID));
	if(pInstance != NULL)
	{
		endTime = CUtil::GetNowSecond() + (DWORD64)pInstance->m_nLimitTime;
	}
	else
	{
		endTime = CUtil::GetNowSecond() + 3600000;
	}

	ServerReturn::ServerDoubleRetInt toClient;
	toClient.set_retf(config.countTime);
	toClient.set_rets(endTime);
	player->SendSynchMessage(&toClient, MSG_SIM_GS2C_TIME_INFO);

}

void HeroCarbon::InitTargets()
{
	const HeroLayerInfo* pInfo = HeroLoader::GetInstance()->GetHeroLayerInfo(m_paramf);
	if(pInfo == NULL || pInfo->monster <= 0)
	{
		LOG_ERROR(FILEINFO, "HeroCarbon::InitTargets Error, layer=%lld", m_paramf);
		return;
	}

	m_killMonsters[pInfo->monster] = make_pair(1,0);
}

void HeroCarbon::AddTarget(int id)
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

void HeroCarbon::Result()
{
	Smart_Ptr<Player> player = GetPlayer(m_owner);
	if(!player)
		return;

	if(m_isSuccess)
	{
		player->GetHeroMgr()->PassLayer(m_paramf);

		PlayerInstancePassEventArgs tArgs(player,m_data->m_nMapID);
		player->FireEvent(PLAYER_PASS_INSTANCE,tArgs);
		FireGlobalEvent(PLAYER_GLOBE_PASSINSTANCE, tArgs);
	}

	ServerReturn::ServerRetBool toClient;
	toClient.set_ret(m_isSuccess);
	player->SendSynchMessage(&toClient, MSG_SIM_GS2C_HERO_RESULT);
}








