/*
 * SeabeachCarbon.cpp
 *
 *  Created on: 2015年11月3日
 *      Author: root
 */
#include "SeabeachCarbon.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../Map/DuplicateMgr.h"
#include "../ServerEventDefine.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../ServerManager.h"
#include "../Map/GameZone.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Object/ObjectFactory.h"
#include "MessageStruct/Seabeach/Seabeach.pb.h"
#include "MessageStruct/Map/ChangeMap.pb.h"
#include "../Seabeach/SeabeachMgr.h"
#include "../ServerEventDefine.h"
#include "../Distortion/Distortion.h"
#include "../Object/AI/AIBrain.h"
#include "../Object/FileLoader/AILoader.h"
#include "../Object/GirlWar/GirlWarManager.h"

SeabeachCarbon::SeabeachCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):
CGameDuplicate(onlyid,charid,paramf,params)
{

}

SeabeachCarbon::~SeabeachCarbon()
{
	if(m_ZoneTimer.IsValid())
	{
		m_ZoneTimer.SetNull();
	}

	if(m_playerMoveEvent.IsValid())
	{
		m_playerMoveEvent.SetNull();
	}

	if(m_playerMeditationEvent.IsValid())
	{
		m_playerMeditationEvent.SetNull();
	}

	if(m_MeditationStartEvent.IsValid())
	{
		m_MeditationStartEvent.SetNull();
	}

}

int SeabeachCarbon::Init()
{
	int ret = CGameDuplicate::Init();
	if(m_ZoneTimer.IsValid())
	{
		m_ZoneTimer.SetNull();
	}

	m_ZoneTimer = TimerInterface::GetInstance()->RegRepeatedTime(&SeabeachCarbon::onZonePrize, this, 0, 0, 0, 0,
			SeabeachMgr::GetInstance()->GetBasePrizeTime(),	SeabeachMgr::GetInstance()->GetBasePrizeTime());

	return ret;
}

//更新状态,false表示要删除这个场景
bool SeabeachCarbon::Update()
{
	switch(m_process)
	{
		case eCreated:
		{
			if(CUtil::GetNowSecond() - m_createTime >= m_data->m_nLimitTime)
			{
				m_process = eOver;
				return false;
			}
			break;
		}

		case ePlayerWork:
		{
			if(CUtil::GetNowSecond() - m_createTime >= m_data->m_nLimitTime)
			{
				Over();
			}
			break;
		}

		case eDuplicateChangeMap:
		{
			if(GetPlayerCount() <= 0)
			{
				CountEx();
			}

			m_process = eChangeMapWait;
			break;
		}

		case eChangeMapWait:
		{
			int64 nowTime = CUtil::GetNowSecond();
			if(nowTime - m_createTime >= m_data->m_nLimitTime)
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

//加入场景
int SeabeachCarbon::Join(Smart_Ptr<Object> & obj)
{
	if(obj->GetType() == ePet)
	{
		return -1;
	}

	int ret=CGameZone::Join(obj);

	if(ret!=0)
	{
		return ret;
	}

	JoinHandle(obj);

	return 0;
}

//加入场景前处理
bool SeabeachCarbon::JoinHandle(Smart_Ptr<Object> & obj)
{
	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		if(!m_playerMoveEvent.IsValid())
		{
			//m_playerMoveEvent = RegGlobalEventPtr(PLAYER_MOVE, this, &SeabeachCarbon::PlayerMove);
		}

		if(!m_playerMeditationEvent.IsValid())
		{
			m_playerMeditationEvent = RegGlobalEventPtr(PLAYER_MEDITATION_INCOME, this, &SeabeachCarbon::PlayerMeditation);
		}

		if(!m_MeditationStartEvent.IsValid())
		{
			m_MeditationStartEvent = RegGlobalEventPtr(PLAYER_MEDITATION_START, this, &SeabeachCarbon::MeditationStart);
		}

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		if(m_process == eCreated)
		{
			m_process = ePlayerWork;
		}

		//处理战姬
		InitGirlWar(player);

		//进入场景时，下发本场自己的收益信息
		InitBaseIncome(player);
	}

	return true;
}

//场景中移动
int SeabeachCarbon::Move(Smart_Ptr<Object> &obj)
{
	int ret = CGameZone::Move(obj);

	if(ret == 0 && obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;
		CanccelGather(player->GetID());
	}

	return ret;
}

//离开场景，isDel是否广播移出
void SeabeachCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);
	CGameZone::Leave(obj, isDel, delFromMap);

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;
		int64 mapid = 0;
		int xpos = 0;
		int ypos = 0;

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

			mapid = sceneid <= 0 ? CREATE_MAP(eStaticMap, 1, 1, sMapInfo->mapid, 1) : sceneid;

			xpos = sMapInfo->reliveXPos;
			ypos = sMapInfo->reliveYPos;
    	}

		player->SetMapID(mapid);
		player->SetXPos(xpos);
		player->SetYPos(ypos);
		player->SetXNewPos();
		player->SetYNewPos();
		player->SetDataFlag(eBaseInfo);
	}

}

//离开场景前处理
void SeabeachCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
{
	if(obj->GetType() == ePlayer)
	{
		//如果是变身状态，取消玩家状态，恢复场景中的物件对象(变为可采集或者原地刷出一个采集物件
		Smart_Ptr<Player> player = obj;
		if(player->IsInCreatureState(ERS_Distortion))
		{
			LeaveObject(player);
		}

		//还原战姬
		ReverseGirlWar(player);
	}

}

//玩家强制退出副本检验(用于判断玩家退出后副本的后续操作 )
void SeabeachCarbon::LeaveExame(Smart_Ptr<Player> & player)
{

}

//此场景是否可以进入
bool SeabeachCarbon::IsCanEnter(Smart_Ptr<Player> & player)
{
	return true;
}

//赢了后的操作
void SeabeachCarbon::Win()
{
	m_isSuccess = true;
	m_process = eAward;
}

//副本结束时间到了之后的 操作
void SeabeachCarbon::Over()
{
	ForceAllPlayerChangeMap();
	m_process = eOver;
}

//副本人数为零时的操作
void SeabeachCarbon::Count()
{
	int64 nowTime = CUtil::GetNowSecond();
	if(nowTime - m_leaveTime >= m_data->m_nDelTime)
	{
		m_process = eOver;
	}
}

void SeabeachCarbon::CountEx()
{
	int64 nowTime = CUtil::GetNowSecond();
	if(nowTime - m_leaveTime >= m_data->m_nDelTime)
	{
		m_process = eOver;
	}
}

//输了之后的操作
void SeabeachCarbon::Failed()
{
	m_process = eAward;
}

//奖励
void SeabeachCarbon::Award()
{
	m_process = eDuplicateChangeMap;
	m_awardTime = CUtil::GetNowSecond();
}

int SeabeachCarbon::Gather(Smart_Ptr<Player>& player, int objkey)
{
	if(player->IsInCreatureState(ERS_Meditation))
	{
		return eGatherIdError;
	}

	//查找有没有该采集对象
	Smart_Ptr<CreatureObj> obj;
	GetObject(objkey, obj);
	if(!obj)
	{
		return eSeabeachObjectLeave;
	}

	if(obj->GetType() != eMonster)
	{
		return eGatherIdError;
	}

	Smart_Ptr<Monster> mon = obj;
	MonsterInfo info;
	MonsterLoader::GetInstance()->GetMonsterInfo(mon->GetID(),info);

	if(info.quality != eChairOrBed && info.quality != eBoad)
		return eGatherIdError;

	if(player->IsInCreatureState(ERS_Distortion))
	{
		return eGatherIdError;
	}

	//开始采集计时
	PlayerGatherData& tmp = m_playerGather[player->GetID()];
	tmp.charID = player->GetID();
	tmp.objectKey = objkey;

	if(tmp.m_timer.IsValid())
	{
		tmp.m_timer.SetNull();
	}

	GatherEx* pParam = new GatherEx(objkey,player->GetID());
	tmp.m_timer = TimerInterface::GetInstance()->RegTimePoint(&SeabeachCarbon::onGatherTimer, this, (void*)(pParam), 0, 0, 0, mon->GetGatherTime());

	//对象被玩家采集列表
	GatherInfo& data = m_GatherInfos[objkey];
	data.objectKey = objkey;
	data.gatherPlayers.push_back(player->GetID());

	return 0;
}

int SeabeachCarbon::LeaveObject(Smart_Ptr<Player>& player)
{
	int monster = player->GetDistortionMgr()->GetMonsterID();

	EndChange(player);

	SeabeachProto::PlayerLeaveObject Brocast;
	Brocast.set_key(player->GetKey());

	player->SendToAround( &Brocast, MSG_GS2C_SEABEACH_LEAVE_GATHER_OBJECT );

	FlushMonster(monster, player->GetXNewPos(), player->GetYNewPos());

	return 0;
}

void SeabeachCarbon::onZonePrize(void * arg)
{
	std::vector<Smart_Ptr<Player> > players;
	GetAllPlayers(players);

	for(size_t i=0; i<players.size(); ++i)
	{
		Smart_Ptr<Player>& ref = players[i];
		if(ref.Get()==NULL || ref->IsDead())
		{
			continue;
		}

		//加经验
		int lv=ref->getAttManage()->getValue<int>(eCharLv);
		int exp = SeabeachMgr::GetInstance()->GetPrizeValue(lv);
		ref->AddExp(exp);

		//更新数据
		UpdatePlayerData(ref, exp, 0);
	}

}

void SeabeachCarbon::onGatherTimer(void* arg)
{
	GatherEx* data = (GatherEx*)(arg);
	if(data==NULL)
		return;

	GatherEx tmp=*data;
	delete data;
	data = NULL;

	//采集成功，广播
	std::map<int64,PlayerGatherData>::iterator it = m_playerGather.find(tmp.m_charID);
	if(it!=m_playerGather.end())
	{
		Smart_Ptr<Player> player = GetPlayer(tmp.m_charID);
		if(player.Get()!=NULL)
		{
			Smart_Ptr<CreatureObj> obj;
			GetObject(tmp.m_key, obj);
			if(obj.Get() != NULL)
			{
				//在通知成功前先移动位置到采集物处
				player->SetXNewPos(obj->GetXNewPos());
				player->SetYNewPos(obj->GetYNewPos());
				player->SetDataFlag(eBaseInfo);
				player->ClearPath();

				Smart_Ptr<Object> objTemp = player;
				if(!CSceneMgr::GetInstance()->MoveToAll(objTemp))
				{
					LOG_ERROR(FILEINFO, "\n player[gs charid=%lld] move  failed onGatherTimer \n", GET_PLAYER_CHARID(player->GetID()));
				}

				ChangeMap::GMForceClientChangeMap toClient;
				toClient.set_mapid(GET_MAP_ID(player->GetMapID()));
				toClient.set_xpos(obj->GetXNewPos());
				toClient.set_ypos(obj->GetYNewPos());
				toClient.set_pktype(player->getAttManage()->getValue<int>(ePKType));
				QuickMessage::GetInstance()->AddSendMessage( player->GetID(), player->GetChannelID(), &toClient, MSG_SIM_GM2C_FORCE_CHANGEMAP);

				SeabeachProto::GatherResult msg;
				msg.set_playerkey(player->GetKey());
				msg.set_objkey(obj->GetID());
				player->SendToAround(&msg, MSG_GS2C_SEABEACH_GATHER_RESULT, true);

				//开启变身奖励定时器
				StartChangeTimer(player, obj->GetID());
			}
		}

		if(it->second.m_timer.IsValid())
				it->second.m_timer.SetNull();

		m_playerGather.erase(it);
	}

	//从队列中移出采集玩家并关闭定时器
	std::map<int64,GatherInfo>::iterator itr = m_GatherInfos.find(tmp.m_key);
	if(itr!=m_GatherInfos.end())
	{
		for(size_t i=0; i<itr->second.gatherPlayers.size(); ++i)
		{
			if(itr->second.gatherPlayers[i]!=tmp.m_charID)
			{
				CanccelGather( itr->second.gatherPlayers[i], eSeabeachObjectLeave);
			}
		}
		m_GatherInfos.erase(itr);
	}

	//采集对象移出场景
	Smart_Ptr<Player> player = GetPlayer(tmp.m_charID);
	Smart_Ptr<CreatureObj> obj;
	GetObject(tmp.m_key, obj);
	if(obj.Get()!=NULL && player.Get()!=NULL)
	{
		Smart_Ptr<Monster> mon = obj;
		mon->SetDead(NULL);
	}

}

void SeabeachCarbon::CanccelGather(int64 charid, int code)
{
	std::map<int64,PlayerGatherData>::iterator it = m_playerGather.find(charid);
	if(it==m_playerGather.end())
		return;

	//从采集列表中移出
	PlayerGatherData& data = it->second;
	if(data.m_timer.IsValid())
		data.m_timer.SetNull();

	if(data.m_pParam!=NULL)
	{
		delete data.m_pParam;
		data.m_pParam = NULL;
	}

	int64 key = it->second.objectKey;
	m_playerGather.erase(it);

	//把玩家从采集对象列表中移出
	std::map<int64,GatherInfo>::iterator objIt = m_GatherInfos.find(key);
	if(objIt!=m_GatherInfos.end())
	{
		std::vector<int64>::iterator playerIt = std::find(objIt->second.gatherPlayers.begin(), objIt->second.gatherPlayers.end(), charid);
		if(playerIt!=objIt->second.gatherPlayers.end())
		{
			objIt->second.gatherPlayers.erase(playerIt);
		}
	}

	//code 小于等于0，不需通知前端
	if(code <= 0)
	{
		return;
	}

	Smart_Ptr<Player> player = GetPlayer(charid);
	if(player.Get()==NULL)
	{
		return;
	}

	//通知客户端取消
	SeabeachProto::GatherFail msg;
	msg.set_objkey(key);
	msg.set_code(code);
	player->SendSynchMessage(&msg, MSG_GS2C_SEABEACH_LEAVE_GATHER_FAIL);

}

bool SeabeachCarbon::PlayerMove(const EventArgs & e)
{
	PlayerMoveEvent *pArgs = (PlayerMoveEvent *) (&e);
	if(pArgs==NULL)
	{
		return true;
	}

	Smart_Ptr<Player> player = GetPlayer(pArgs->m_charID);
	if(player.Get()==NULL)
	{
		return true;
	}

	CanccelGather(pArgs->m_charID);

	return true;
}

bool SeabeachCarbon::PlayerMeditation(const EventArgs & e)
{
	MeditationEventArgs *pArgs = (MeditationEventArgs *) (&e);
	if(pArgs==NULL || pArgs->m_obj.Get()==NULL)
	{
		return true;
	}

	if(pArgs->m_obj->GetMapID() != GetSceneID())
	{
		return true;
	}

	UpdatePlayerData(pArgs->m_obj, pArgs->m_exp, pArgs->m_force);

	return true;
}

bool SeabeachCarbon::MeditationStart(const EventArgs & e)
{
	OneParmaEventArgs *pArgs = (OneParmaEventArgs*)(&e);
	if(pArgs==NULL || pArgs->m_obj.Get()==NULL)
	{
		return true;
	}

	if(pArgs->m_obj->GetMapID() != GetSceneID())
	{
		return true;
	}

	if(pArgs->m_obj->GetType() != ePlayer)
	{
		return true;
	}

	Smart_Ptr<Player> player = pArgs->m_obj;
	CanccelGather( player->GetID(), eGatherFailed );

	return true;
}

void SeabeachCarbon::UpdatePlayerData(Smart_Ptr<Player>& player, int exp, int force)
{
	std::map<int64,PlayerSeabeachData>::iterator it = m_playerData.find( player->GetID() );
	if(it != m_playerData.end())
	{
		it->second.exp += exp;
		it->second.force += force;
	}
	else
	{
		PlayerSeabeachData data;
		data.exp = exp;
		data.force = force;
		m_playerData[player->GetID()] = data;
	}

	//通知客户端
	SendToPlayer(player, exp, force);

}

void SeabeachCarbon::SendToPlayer(Smart_Ptr<Player>& player, int exp, int force)
{
	SeabeachProto::IncomeChange toClient;
	toClient.set_exp(exp);
	toClient.set_force(force);

	player->SendSynchMessage(&toClient,MSG_GS2C_SEABEACH_INCOME_CHANGE);
}

void SeabeachCarbon::InitBaseIncome(Smart_Ptr<Player>& player)
{
	std::map<int64,PlayerSeabeachData>::const_iterator it = m_playerData.find( player->GetID() );
	if(it != m_playerData.end())
	{
		if(it->second.exp > 0  || it->second.force > 0)
		{
			SendToPlayer(player, it->second.exp, it->second.force);
		}
	}

}

void SeabeachCarbon::StartChangeTimer(Smart_Ptr<Player>& player, int monster)
{
	//玩家进入变身状态
	player->GetDistortionMgr()->Start( monster );

}

void SeabeachCarbon::EndChange(Smart_Ptr<Player>& player)
{
	player->GetDistortionMgr()->End();
}

//刷新怪物
void SeabeachCarbon::FlushMonster(int monster, int xpos, int ypos)
{
	MonsterInfo info;

	MonsterLoader::GetInstance()->GetMonsterInfo(monster, info);
	if(info.id == -1)
	{
		return;
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
//	mon->SetBaseAi(info.baseAI);
//	mon->SetBetterAi(info.betterAI);
	mon->SetFallID(info.fallID);
	mon->getAttManage()->setValue(eCharExp,info.exp);
	mon->getAttManage()->setValue(eCharPhyDefence,info.phyDefence);
	mon->getAttManage()->setValue(eCharHitRate,info.hitRate);
	mon->getAttManage()->setValue(eCharAvoidRate,info.avoidRate);
	mon->getAttManage()->setValue(eCrackDownRate,info.crackRate);
	mon->getAttManage()->setValue(eAvoidCrackRate,info.avoidCrack);
	mon->getAttManage()->setValue(eCharPhyAttackMax, info.maxAttack);
	mon->SetMPMax(info.maxMP);
	mon->SetCurMP(info.maxMP);

	mon->SetHPMax(info.maxHP);
	mon->SetCurHP(info.maxHP);

	mon->SetGatherTime(info.gatherTime);	//采集时间
	mon->SetOwnerToBrain(mon);
	mon->SetMapID(m_sceneID);
	mon->SetBornTime();
	mon->SetOwnerName("");
	mon->GetBrain()->SetMonsterType(eRandomWayPos);
	mon->SetXPos(xpos);
	mon->SetYPos(ypos);
	mon->SetOriginalX(xpos);
	mon->SetOriginalY(ypos);

	Join(obj);

	vector<Sector *> moveVec;
	BaseAIInfo * aiInfo = 0;
	BetterAIInfo *betterAI = 0;

	aiInfo = AILoader::GetInstance()->GetBaseAIByid(info.baseAI);
	if(!aiInfo)
	{
		LOG_ERROR(FILEINFO, "monster[%d] ai is error", info.id);

		return;
	}

	if(info.betterAI > 0)
	{
		betterAI = AILoader::GetInstance()->GetBetterAIByid(info.betterAI);
		if (!betterAI)
		{
			LOG_ERROR(FILEINFO, "monster[id=%d] better ai is error", info.id);

			return;
		}
	}

	mon->SetStayTime(aiInfo->stayTime);
	mon->SetPhyAttackTime(aiInfo->atkTime);
	mon->SetMoveSector(moveVec);

	Smart_Ptr<AIBrain> monBrain = mon->GetBrain();
	if(!monBrain)
		return;

	monBrain->Init(aiInfo, betterAI);
	m_allObject[mon->GetKey()] = mon;
	m_allMonster[info.id].push_back(mon);

}


//初始化战姬
void SeabeachCarbon::InitGirlWar(Smart_Ptr<Player>& player)
{
	Smart_Ptr<Object> girlwarObj = player->GetGirlWarManager()->GetGirlWar();
	if(girlwarObj)
	{
		Smart_Ptr<GirlWar> girlWar = girlwarObj;

		std::map<int64,PlayerSeabeachData>::iterator it = m_playerData.find( player->GetID() );
		if(it != m_playerData.end())
		{
			it->second.girlID = girlWar->GetID();
		}
		else
		{
			PlayerSeabeachData data;
			data.girlID = girlWar->GetID();
			m_playerData[player->GetID()] = data;
		}

		Smart_Ptr<GirlWar> girlWarTemp;
		player->GetGirlWarManager()->SetGirlWar(girlWarTemp);
	}

}

//还原战姬
void SeabeachCarbon::ReverseGirlWar(Smart_Ptr<Player>& player)
{
	std::map<int64,PlayerSeabeachData>::iterator it = m_playerData.find( player->GetID() );
	if(it == m_playerData.end())
		return;

	Smart_Ptr<GirlWar> girlWar;
	player->GetGirlWarManager()->GetGirlWarById(it->second.girlID,girlWar);
	player->GetGirlWarManager()->SetGirlWar(girlWar);
}





