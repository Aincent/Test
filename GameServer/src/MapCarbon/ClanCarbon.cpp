/*
 * ClanCarbon.cpp
 *
 *  Created on: 2015年6月29日
 *      Author: root
 */

#include "ClanCarbon.h"
#include "util.h"
#include "Log/Logger.h"
#include "../Object/Player/Player.h"
#include "../Object/ItemDrop/DropItemMgr.h"
#include "../Map/DuplicateMgr.h"
#include "CounterDefine.h"
#include "../ServerEventDefine.h"
#include "MessageStruct/ServerReturnBool.pb.h"
#include "../QuickMessage.h"
#include "../Object/Player/PlayerMessage.h"
#include "../ServerManager.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "../Clan/ClanLoader/ClanLoader.h"
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
#include "../Country/GsCountryMgr.h"
#include "../Mail/MailMessage.h"
#include "./FileLoader/InternationalLoader.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "../Activity/ActivityBox.h"
#include "../Activity/FileLoader/ActivityBoxLoader.h"

CClanCarbon::CClanCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):
CGameDuplicate(onlyid,charid,paramf,params)
{
	serverid = charid;

	CampBatterData data;
	data.clanid = paramf;
	data.campid = eCamp_Red;
	m_BatterData[paramf] = data;

	data.clanid = params;
	data.campid = eCamp_Blue;
	m_BatterData[params] = data;

	InstanceMapEntry* pInstance = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(m_sceneID));
	if(pInstance!=NULL)
	{
		m_endtime = CUtil::GetNowSecond() + (DWORD64)pInstance->m_nLimitTime;
	}
	else
	{
		m_endtime = CUtil::GetNowSecond() + 3600000;
	}

}

CClanCarbon::~CClanCarbon()
{
	m_Boxs.clear();

	if(m_Boxtimer.IsValid())
		m_Boxtimer.SetNull();

	if(m_monsterDeadEvent.IsValid())
		m_monsterDeadEvent.SetNull();

	if(m_playerDeadEvent.IsValid())
		m_playerDeadEvent.SetNull();

	if(m_leaveClanEvent.IsValid())
		m_leaveClanEvent.SetNull();

	if(m_leaveMapEvent.IsValid())
		m_leaveMapEvent.SetNull();

	//旗子信息
	std::map<int64,CountryWarElementInfo>::iterator it = m_fiveElement.begin();
	for(; it != m_fiveElement.end(); ++it)
	{
		if(it->second.m_prizetimer.IsValid())
			it->second.m_prizetimer.SetNull();

		if(it->second.m_protectedtimer.IsValid())
			it->second.m_protectedtimer.SetNull();
	}

	//玩家占领旗子定时器
	std::map<int64,OccupyDataEx>::iterator iter = m_OccupyTimerList.begin();
	for(; iter != m_OccupyTimerList.end(); ++iter)
	{
		if(iter->second.m_timer.IsValid())
			iter->second.m_timer.SetNull();

		if(iter->second.m_pMemory != NULL)
		{
			delete iter->second.m_pMemory;
			iter->second.m_pMemory = NULL;
		}
	}

}

bool CClanCarbon::OccupyFlag(int64 playerID, int ObjectKey)
{
	std::map<int64,CountryWarElementInfo>::iterator it = m_fiveElement.find(ObjectKey);
	if(it==m_fiveElement.end())
			return false;

	if(!it->second.state)
			return false;

	if(it->second.time>0)
			return false;

	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharId(player,playerID);
	if( player.Get()==NULL || (DWORD)it->second.owner==player->getAttManage()->getValue<DWORD>(eCharFaction))
			return false;

	//开始计时
	OccupyData* pData = new OccupyData();
	pData->charID = playerID;
	pData->objectKey = ObjectKey;

	OccupyDataEx& tmp = m_OccupyTimerList[playerID];
	tmp.charID = pData->charID;
	tmp.objectKey = pData->objectKey;
	tmp.m_pMemory = pData;

	if(tmp.m_timer.IsValid())
			tmp.m_timer.SetNull();

	const BatterWarConfig& config = ClanLoader::GetInstance()->GetBatterConfig();
	tmp.m_timer = TimerInterface::GetInstance()->RegTimePoint(&CClanCarbon::OnOccupyTimer, this, (void*)pData, 0, 0, config.occypytime, 0);

	return true;
}

bool CClanCarbon::IsCanGatherBox(int nKey,Smart_Ptr<Player>& player)
{
	map<int,MasterStatus>::iterator itr = m_Boxs.find(nKey);
	if(itr != m_Boxs.end())
	{
		if(itr->second.bStatus)
		{
			if(itr->second.nCharId == player->GetID())
				return true;
		}
		else
		{
			itr->second.bStatus = true;
			itr->second.nCharId = player->GetID();
			return true;
		}
	}

	return false;
}

void CClanCarbon::OnOccupyTimer(void* args)
{
	OccupyData* pData = (OccupyData*)(args);
	if(pData==NULL)
		return;

	OccupyData temp = *pData;
	delete pData;
	pData=NULL;

	std::map<int64,OccupyDataEx>::iterator it=m_OccupyTimerList.find(temp.charID);
	if(it==m_OccupyTimerList.end())
	{
		return;
	}

	//销毁定时器
	if(it->second.m_timer.IsValid())
	{
		it->second.m_timer.SetNull();
	}
	m_OccupyTimerList.erase(it);

	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharId(player,temp.charID);
	if(player.Get()==NULL)
	{
		return;
	}

	int clanid = player->getAttManage()->getValue<int>(eCharFaction);
	const CampBatterData* pPtr = GetClanBatterData(clanid);
	if(pPtr == NULL)
	{
		return;
	}

	//通知其他玩家取消占领
	std::map<int64,CountryWarElementInfo>::iterator itr = m_fiveElement.find(temp.objectKey);
	if(itr!=m_fiveElement.end())
	{
		for(size_t i=0; i<itr->second.m_occupys.size(); ++i)
		{
			if(itr->second.m_occupys[i]!=temp.charID)
			{
				CanccelGather(itr->second.m_occupys[i],false);
			}
		}
	}

	//占领处理
	UpdateFlagState(temp.objectKey,(long)player->getAttManage()->getValue<DWORD>(eCharFaction));

	//广播公告通知全地图占领状态
	ClanPackage::FlagStateChange toClient;
	toClient.set_playerkey(player->GetKey());
	toClient.set_flagkey(temp.objectKey);
	toClient.set_owner(pPtr->campid);
	toClient.set_clanname(player->GetClanName());
	toClient.set_playername(player->GetName());

	SendMapAllSynchMessage(&toClient,MSG_SIM_GS2C_FLAG_STATE_CHANGE,0,true);
}

void CClanCarbon::CanccelGather(int64 charid,bool isRemoveFromOjb, bool isSendtoClient)
{
	std::map<int64,OccupyDataEx>::iterator it = m_OccupyTimerList.find(charid);
	if(it==m_OccupyTimerList.end())
		return;

	//从采集列表中移出
	OccupyDataEx& data = it->second;
	if(data.m_timer.IsValid())
		data.m_timer.SetNull();

	if(data.m_pMemory!=NULL)
	{
		delete data.m_pMemory;
		data.m_pMemory = NULL;
	}
	int64 key = it->second.objectKey;
	m_OccupyTimerList.erase(it);

	//把玩家从采集对象列表中移出
	if(isRemoveFromOjb)
	{
		std::map<int64,CountryWarElementInfo>::iterator objIt = m_fiveElement.find(key);
		if(objIt!=m_fiveElement.end())
		{
			std::vector<int64>::iterator playerIt = std::find(objIt->second.m_occupys.begin(), objIt->second.m_occupys.end(), charid);
			if(playerIt != objIt->second.m_occupys.end())
				objIt->second.m_occupys.erase(playerIt);
		}
	}

	Smart_Ptr<Player> player = GetPlayer(charid);
	if(player.Get()==NULL)
			return;

	//通知客户端取消
	ClanPackage::OccupyFail toCilent;
	toCilent.set_ret(0);
	player->SendSynchMessage(&toCilent, MSG_SIM_GS2C_OCCUPY_FAIL);
}

const CampBatterData* CClanCarbon::GetClanBatterData(int id)
{
	std::map<int,CampBatterData>::const_iterator it =	m_BatterData.find(id);
	if(it != m_BatterData.end())
	{
		return &it->second;
	}
	return NULL;
}

void CClanCarbon::UpdateFlagState(int64 object, long owner)
{
	std::map<int64,CountryWarElementInfo>::iterator it=m_fiveElement.find(object);
	if(it==m_fiveElement.end())
		return;
	it->second.owner = owner;
	it->second.state = false;
	it->second.time = CUtil::GetNowSecond();//取当前时间

	//启动保护时间定时器
	int64* pElementKey=new int64(object);
	if(it->second.m_protectedtimer.IsValid())
		it->second.m_protectedtimer.SetNull();

	if(it->second.m_prizetimer.IsValid())
		it->second.m_prizetimer.SetNull();

	const BatterWarConfig& config = ClanLoader::GetInstance()->GetBatterConfig();
	it->second.m_protectedtimer = TimerInterface::GetInstance()->RegTimePoint(&CClanCarbon::ProtectedTimer, this, (void*)(pElementKey), 0, 0, config.protectime, 0);

}

void CClanCarbon::ProtectedTimer(void * arg)
{
	if(arg==NULL)
		return;

	int64* pObjectKey=(int64*)(arg);
	int64 tmp=*pObjectKey;
	delete pObjectKey;
	pObjectKey=NULL;

	std::map<int64,CountryWarElementInfo>::iterator it=m_fiveElement.find(tmp);
	if(it==m_fiveElement.end())
		return;

	//更新状态
	it->second.time = 0;
	it->second.state = true;
	if(it->second.m_protectedtimer.IsValid())
	{
		it->second.m_protectedtimer.SetNull();
	}

	//启动增加积分定时器
	if(it->second.m_prizetimer.IsValid())
	{
		it->second.m_prizetimer.SetNull();
	}

	const BatterWarConfig& config = ClanLoader::GetInstance()->GetBatterConfig();

	long owner = it->second.owner;
	it->second.m_prizetimer = TimerInterface::GetInstance()->RegRepeatedTime(&CClanCarbon::AddScoreTimer,
			this, (void*)(owner),0,0,0,0,config.scoretime*1000);

	//通知全地图
	ClanPackage::FlagProtectedTimeOut cmd;
	cmd.set_key(tmp);
	SendMapAllSynchMessage(&cmd,MSG_SIM_GS2C_FLAG_TIMEOUT,0,true);
}

void CClanCarbon::AddScoreTimer(void* arg)
{
	long owner = (long)(arg);

	//增加积分
	const BatterWarConfig& config = ClanLoader::GetInstance()->GetBatterConfig();

	AddScore(owner, config.occupyscores);
}

void CClanCarbon::OnFlushBox(void * args)
{
	//清空上一波
	SetMonsterDead(ActivityBoxLoader::GetInstance()->GetMonsterId());
	m_Boxs.clear();

	vector<pont> vecPos;
	MonsterInfo info;

	const BatterWarConfig& config = ClanLoader::GetInstance()->GetBatterConfig();
	ClanLoader::GetInstance()->GetClanWarBoxPos(vecPos);
	if(vecPos.size() == 0 || (int)vecPos.size() < config.boxsnum)
	{
		return;
	}

	vector<Smart_Ptr<Monster> > vecMonster;
	for (int i = 0; i < config.boxsnum; i++)
	{
		MonsterLoader::GetInstance()->GetMonsterInfo(ActivityBoxLoader::GetInstance()->GetMonsterId(), info);
		if (info.id == -1)
		{
			LOG_ERROR(FILEINFO, "monster id is error");
			continue;
		}

		Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eMonster);
		if(false == bool(obj))
			continue;

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
		mon->SetFallID(info.fallID);
		mon->getAttManage()->setValue(eCharExp,info.exp);
		mon->getAttManage()->setValue(eCharPhyAttackMax,info.maxAttack);
		mon->getAttManage()->setValue(eCharPhyDefence,info.phyDefence);
		mon->getAttManage()->setValue(eCharHitRate,info.hitRate);
		mon->getAttManage()->setValue(eCharAvoidRate,info.avoidRate);
		mon->getAttManage()->setValue(eCrackDownRate,info.crackRate);
		mon->getAttManage()->setValue(eAvoidCrackRate,info.avoidCrack);
		mon->SetHPMax(info.maxHP);
		mon->SetCurHP(info.maxHP);
		mon->SetMPMax(info.maxMP);
		mon->SetCurMP(info.maxMP);
		mon->SetGatherTime(info.gatherTime);	//采集时间
		mon->SetOwnerToBrain(mon);
		mon->SetMapID(GetSceneID());
		mon->SetBornTime();
		mon->SetOwnerName("");
		mon->GetBrain()->SetMonsterType(eRandomWayPos);

		int temp = rand()%(int)vecPos.size();
		mon->SetXPos(vecPos[temp].xpos);
		mon->SetYPos(vecPos[temp].ypos);
		mon->SetOriginalX(vecPos[temp].xpos);
		mon->SetOriginalY(vecPos[temp].ypos);

		vecPos.erase(vecPos.begin()+ temp);

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

		Smart_Ptr<AIBrain> monBrain = mon->GetBrain();
		if(!monBrain)
			continue;

		monBrain->Init(aiInfo, betterAI);
		vecMonster.push_back(mon);
		AddObject(mon->GetKey(),mon);

		MasterStatus mStatus;
		mStatus.nKey = mon->GetKey();
		mStatus.nSceneID = GetSceneID();

		m_Boxs[mon->GetKey()] = mStatus;
	}

	AddMonster(ActivityBoxLoader::GetInstance()->GetMonsterId(),vecMonster);
}

void CClanCarbon::AddScore(int clanid, int score)
{
	if(score <= 0 || m_process >= eAward)
		return;

	std::map<int,CampBatterData>::iterator it = m_BatterData.find(clanid);
	if(it != m_BatterData.end())
	{
		it->second.scroe += score;

		//通知前端
		ClanPackage::ScoreChange toClient;
		toClient.set_owner(it->second.campid);
		toClient.set_score(it->second.scroe);

		//全地图通知
		SendMapAllSynchMessage(&toClient,MSG_SIM_GS2C_CLAN_BATTLE_UPDATE,0,true);

		//积分是否已经达到胜利条件
		const BatterWarConfig& config = ClanLoader::GetInstance()->GetBatterConfig();
		if(it->second.scroe >= config.winscore)
		{
			int firstscore = 0, secondscore = 0;
			if(clanid == m_paramf)
			{
				firstscore = it->second.scroe;
				const CampBatterData* pInfo = GetClanBatterData(m_params);
				if(pInfo!=NULL)
					secondscore = pInfo->scroe;
			}
			else
			{
				secondscore = it->second.scroe;
				const CampBatterData* pInfo = GetClanBatterData(m_paramf);
				if(pInfo!=NULL)
					firstscore = pInfo->scroe;
			}
			Result(clanid, firstscore, secondscore);
			m_process = eAward;
		}
	}
}

bool CClanCarbon::GetBaseInfos(Smart_Ptr<Player>& player, ClanPackage::BaseBatterInfo& info)
{
	//通知玩家当前战场的基本信息
	DWORD clanid = player->getAttManage()->getValue<DWORD>(eCharFaction);
	const CampBatterData* pData = GetClanBatterData(clanid);
	if(pData == NULL)
	{
		return false;
	}

	DWORD64 curTime = CUtil::GetNowSecond();

	info.set_selfflag(pData->campid);
	info.set_targetflag( pData->campid == eCamp_Red ? eCamp_Blue : eCamp_Red );
	info.set_time( m_endtime );

	//红蓝的积分
	const CampBatterData* pRedData = GetClanBatterData(m_paramf);
	const CampBatterData* pBlueData = GetClanBatterData(m_params);

	info.set_redscore(pRedData ? pRedData->scroe : 0);
	info.set_bluescore(pBlueData ? pBlueData->scroe : 0);

	std::map<int64,CountryWarElementInfo>::iterator it=m_fiveElement.begin();
	for(; it!=m_fiveElement.end(); ++it)
	{
		ClanPackage::FlagInfo* pFlag = info.add_info();
		pFlag->set_key(it->second.objectKey);
		pFlag->set_id(it->second.npcID);

		const CampBatterData* pData = GetClanBatterData(it->second.owner);
		if(it->second.owner <= 0)
		{
			pFlag->set_ower(0);
		}
		else
		{
			pFlag->set_ower(pData == NULL ? 0 : pData->campid);
		}

		if((int64)curTime >= it->second.time && it->second.time > 0)
		{
			pFlag->set_time(curTime - it->second.time);
		}
		else
		{
			pFlag->set_time(0);
		}
	}

	return true;
}

void CClanCarbon::AskFactionName(Smart_Ptr<Player> player, int factionId)
{
	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(serverid,worldSvr);
	if(worldSvr.Get()!=NULL)
	{
		ClanPackage::ReqGS2WSGetFactionName req;
		req.set_id(factionId);

		Safe_Smart_Ptr<CommBaseOut::Message> message;
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GS2WS_FACTION_NAME, &req, worldSvr.Get(), Request);
		messRet->SetAct(new commonAct(message,player->GetID(),m_sceneID));
		Message_Facade::Send(messRet);
	}
}

void CClanCarbon::Result(int winid, int firstscore, int secondscore)
{
	//奖励道具
	std::vector<PropItem> WinProps;
	std::vector<PropItem> FailProps;
	GetReWardItems(WinProps, true);
	GetReWardItems(FailProps, false);

	if(WinProps.size() == 0)
	{
		printf("\n prize  null  \n");
	}

	//邮件标题
	const BatterWarConfig& config = ClanLoader::GetInstance()->GetBatterConfig();
	std::string strContent=InternationalLoader::GetInstance()->GetInterStr(config.mailid);

	INT64PLAYERMAP::iterator it = m_player.begin();
	for(; it != m_player.end(); ++it)
	{
		if(it->second.Get()==NULL)
			continue;

		ClanPackage::ClanBattleResult toClient;

		if((int)it->second->getAttManage()->getValue<DWORD>(eCharFaction) == winid)
		{
			toClient.set_result(1);
			it->second->SendSynchMessage(&toClient, MSG_SIM_GS2C_CLAN_BATTLE_RESULT);
			MailMessageMgr::GetInstance()->SendMail(it->second->GetID(), WinProps, strContent);
		}
		else
		{
			toClient.set_result(0);
			it->second->SendSynchMessage(&toClient, MSG_SIM_GS2C_CLAN_BATTLE_RESULT);
			MailMessageMgr::GetInstance()->SendMail(it->second->GetID(), FailProps, strContent);
		}
	}

	//通知世界服结果
	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(serverid,worldSvr);
	if(worldSvr.Get()!=NULL)
	{
		printf("\n send to world \n");

		ClanPackage::BatterInfo toWS;
		toWS.set_firstclanid(m_paramf);
		toWS.set_secondclanid(m_params);
		toWS.set_winid(winid);
		toWS.set_firstclanscore(firstscore);
		toWS.set_secondclanscore(secondscore);

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_CLAN_BATTLE_RESULT, &toWS, worldSvr.Get(), SimpleMessage);
		Message_Facade::Send(messRet);
	}
}

void CClanCarbon::GetReWardItems(std::vector<PropItem>& Props, bool win)
{
	const ClanWarRewardItem*	pItems = ClanLoader::GetInstance()->GetClanWarRewardItem(win);
	if(pItems == NULL)
		return;

	for(size_t i = 0; i < pItems->items.size(); ++i)
	{
		PropItem item;
		item.id = pItems->items[i].first;
		item.num = pItems->items[i].second;
		Props.push_back( item );
	}

}

int CClanCarbon::Init()
{
	int ret = CGameDuplicate::Init();

	const BatterWarConfig& config = ClanLoader::GetInstance()->GetBatterConfig();
	m_Boxtimer = TimerInterface::GetInstance()->RegRepeatedTime(&CClanCarbon::OnFlushBox, this, 0, 0, 0, 0, config.flushtime*1000, config.flushtime*1000);

	return ret;
}

bool CClanCarbon::Update()
{
	switch(m_process)
	{
	case eCreated:
		{
			if(CUtil::GetNowSecond() - m_createTime >= m_data->m_nLimitTime)
			{
				Over();
			}

			break;
		}
	case ePlayerWork:
		{
			int64 nowTime = CUtil::GetNowSecond();
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

			if(nowTime - m_awardTime >= 30 * 1000/*m_data->m_nJgdelTime*/)
			{
				ForceAllPlayerChangeMap();
				m_process = eOver;
			}

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

			if(nowTime - m_createTime >= m_data->m_nLimitTime + 30 * 1000)
			{
				//到副本结束时间就强制传送玩家出副本
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

bool CClanCarbon::JoinHandle(Smart_Ptr<Object> & obj)
{
	if(m_process >= eDuplicateChangeMap)
		return false;

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		DWORD faction = player->getAttManage()->getValue<DWORD>(eCharFaction);

		if(faction != m_paramf && faction != m_params)
			return false;

		if(m_process == eCreated)
		{
			m_process = ePlayerWork;
		}

		if(!m_playerAttackEvent.IsValid())
		{
			m_playerAttackEvent = RegGlobalEventPtr(PLAYER_ATTACK_CREATURE, this, &CClanCarbon::HandleAttackEvent);
		}

		if(!m_playerMoveEvent.IsValid())
		{
			m_playerMoveEvent = RegGlobalEventPtr(PLAYER_MOVE, this, &CClanCarbon::HandleMoveEvent);
		}

		if(!m_playerDeadEvent.IsValid())
		{
			m_playerDeadEvent = RegGlobalEventPtr(CREATURE_DEAD_EVENT_TYPE, this, &CClanCarbon::HandlePlayerDeadEvent);
		}

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		//请求对方帮派名称
		int askid = faction == m_paramf ? m_params : m_paramf;
		AskFactionName(player, askid);
	}

	//怪物处理
	if(obj->GetType() == eMonster)
	{
		Smart_Ptr<Monster> mon = obj;

		//是否是战旗
		if(mon->GetQuality() == eBatterFlag)
		{
			CountryWarElementInfo info;
			info.objectKey = obj->GetKey();
			info.npcID = mon->GetID();
			m_fiveElement[obj->GetKey()] = info;
		}

		const BatterWarConfig& config = ClanLoader::GetInstance()->GetBatterConfig();
		//红方npc
		if(config.redCampnpcId == mon->GetID())
		{
			mon->getAttManage()->setValue(eCharCountry,(DWORD)m_paramf);
		}
		//蓝方npc
		if(config.blueCampnpcId == mon->GetID())
		{
			mon->getAttManage()->setValue(eCharCountry,(DWORD)m_params);
		}
	}

	return true;
}

int CClanCarbon::Join(Smart_Ptr<Object> & obj)
{
	if(!JoinHandle(obj))
		return -1;

	int ret = CGameZone::Join(obj);
	return ret;
}

int CClanCarbon::Move(Smart_Ptr<Object> &obj)
{
	return CGameZone::Move(obj);
}

void CClanCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
{
	int64 mapid = 0;
	int xpos = 0;
	int ypos = 0;
	vector<int> vecType;

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		CanccelGather(player->GetID(), true, false);

		//箱子处理
		map<int,MasterStatus>::iterator itr = m_Boxs.begin();
		for(; itr != m_Boxs.end(); ++itr)
		{
			if(itr->second.bStatus && itr->second.nCharId == player->GetID())
			{
				itr->second.bStatus = false;
				itr->second.nCharId = 0;
			}
		}

		if(player->getAttManage()->getValue<int>(eCharHP) <= 0)
		{
			CharBattle::RevivalData req;

			//增加地图id和坐标，重置死亡状态，解决客户端显示“使用复活单”的提示bug，是因为玩家回城复活，那么会发两次消息给客户端
			player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));
			player->UnsetCreatureState(ERS_Dead);
			player->SetDataFlag(eBaseInfo);

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

void CClanCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);

	CGameZone::Leave(obj, isDel, delFromMap);

	//离开后设置地图
	AfterLeaveHandle(obj);

	if(GetPlayerCount() <= 0)
	{
		m_monsterDeadEvent.SetNull();
		m_playerDeadEvent.SetNull();
		m_leaveClanEvent.SetNull();
		m_leaveTime = CUtil::GetNowSecond();
	}
}

int CClanCarbon::ObjectLevel(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
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
					return -1;
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
					return -1;
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
				LOG_ERROR(FILEINFO, "sectorindex in monster[%lld] is invalid(mapid=%d, xpos=%d, ypos=%d) when monster leave",
						monster->GetID(),
						(int)GET_MAP_ID(m_sceneID),
						monster->GetXNewPos(),
						monster->GetYNewPos());

				return -1;
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
			if(!info || sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex in dropitem is invalid(mapid=%d, xpos=%d, ypos=%d) when dropitem leave",
						(int)GET_MAP_ID(m_sceneID),
						obj->GetXPos(),
						obj->GetYPos());

				return -1;
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
					return -1;
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
				LOG_ERROR(FILEINFO, "sectorindex in girlWar[owner=%lld, id=%d] is invalid(mapid=%d, xpos=%d, ypos=%d) when girlWar leave",
						girlWar->GetOwner()->GetID(),
						(int)girlWar->GetID(),
						(int)GET_MAP_ID(m_sceneID),
						girlWar->GetXNewPos(),
						girlWar->GetYNewPos());

				return -1;
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
					return -1;
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

	return 0;
}

void CClanCarbon::AfterLeaveHandle(Smart_Ptr<Object> & obj)
{
	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		int64 mapid = player->GetMapID();
		int posx = player->GetXPos();
		int posy = player->GetYPos();

		OldMapInfo* info = GetOldMapPoint(player->GetID());
		if(info)
		{
			mapid = info->mapid;
			posx = info->xpos;
			posy = info->ypos;
		}
		else
		{
			InstanceMapEntry* instanceInfo = GetInstanceScript();
			if(instanceInfo==NULL)
				return;

			MapStaticInfo * mapInfo = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(player->GetMapID()));
			if(mapInfo==NULL)
				return;

			mapid = CREATE_MAP(eStaticMap, 1, 1, mapInfo->reliveMap, 1);
			posx = mapInfo->reliveXPos;
			posy = mapInfo->reliveYPos;
		}

		player->SetMapID(mapid);
		player->SetXPos(posx);
		player->SetYPos(posy);
		player->SetXNewPos();
		player->SetYNewPos();
		player->SetDataFlag(eBaseInfo);
	}

}

void CClanCarbon::LeaveExame(Smart_Ptr<Player> & player)
{

}

bool CClanCarbon::IsCanEnter(Smart_Ptr<Player> & player)
{
	//传送阶段，就不允许进入了
	if(m_process >= eDuplicateChangeMap)
	{
		return false;
	}

	//玩家死了不入传入地图
	if(player->IsDead())
	{
		return false;
	}

	int id = (int)player->getAttManage()->getValue<DWORD>(eCharFaction);
	if(id != m_paramf && id != m_params)
	{
		return false;
	}

	return true;
}


void CClanCarbon::Win()
{
	m_isSuccess = true;
	m_process = eAward;
}

void CClanCarbon::Over()
{
	//直接区分输赢结果
	int redScore = 0, blueScore = 0;

	//红方
	const CampBatterData* pRed = GetClanBatterData(m_paramf);
	if(pRed)
		redScore = pRed->scroe;

	//蓝方
	const CampBatterData* pBlue = GetClanBatterData(m_params);
	if(pBlue)
		blueScore = pBlue->scroe;

	int winid = 0;

	//计算胜负,  随机
	if(redScore == blueScore)
	{
		winid = rand()%2 == 0 ? m_paramf : m_params;
	}
	else
	{
		winid = redScore > blueScore ? m_paramf : m_params;
	}

	//结算
	Result(winid, redScore, blueScore);

	m_process = eAward;
}

void CClanCarbon::CountEx()
{
	int64 nowTime = CUtil::GetNowSecond();

	if(nowTime - m_leaveTime >= m_data->m_nDelTime)
	{
		m_process = eOver;
	}
}

void CClanCarbon::Count()
{

}

void CClanCarbon::Failed()
{
	m_process = eAward;
}

void CClanCarbon::Award()
{

	m_process = eDuplicateChangeMap;
	m_awardTime = CUtil::GetNowSecond();
}

bool CClanCarbon::HandleAttackEvent(const EventArgs & e)
{
	PlayerAttackCreature *pArgs = (PlayerAttackCreature *) (&e);
	if(pArgs==NULL || pArgs->m_player.Get()==NULL || pArgs->m_player->GetMapID() != m_sceneID)
			return true;

	CanccelGather(pArgs->m_player->GetID());

	map<int,MasterStatus>::iterator itr = m_Boxs.begin();
	for(; itr != m_Boxs.end(); ++itr)
	{
		if(itr->second.bStatus && itr->second.nCharId == pArgs->m_player->GetID())
		{
			itr->second.bStatus = false;
			itr->second.nCharId = 0;
		}
	}

	return true;
}

bool CClanCarbon::HandleMoveEvent(const EventArgs & e)
{
	PlayerMoveEvent *pArgs = (PlayerMoveEvent *) (&e);
	if(pArgs==NULL)
		return true;

	CanccelGather(pArgs->m_charID);

	map<int,MasterStatus>::iterator itr = m_Boxs.begin();
	for(; itr != m_Boxs.end(); ++itr)
	{
		if(itr->second.bStatus && itr->second.nCharId == pArgs->m_charID)
		{
			itr->second.bStatus = false;
			itr->second.nCharId = 0;
		}
	}

	return true;
}

bool CClanCarbon::HandleLeaveMapEvent(const EventArgs & e)
{
	return true;
}

bool CClanCarbon::HandleMonsterDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	return true;
}

bool CClanCarbon::HandlePlayerDeadEvent(const EventArgs & e)
{
	PlayerKilledEventArgs *tArgs = (PlayerKilledEventArgs *)(&e);

	if(tArgs->m_killer.Get()==NULL || tArgs->m_dead.Get()==NULL || GetSceneID() != tArgs->m_dead->GetMapID())
		return true;

	if(tArgs->m_dead->GetType() != ePlayer)
	{
		return true;
	}

	//取消采集
	CanccelGather(tArgs->m_dead->GetID());

	if(tArgs->m_killer->GetType() != ePlayer && tArgs->m_killer->GetType() != ePet)
		return true;

	Smart_Ptr<Player>  player;
	Smart_Ptr<GirlWar>  girlwar;

	if(tArgs->m_killer->GetType() == ePet)
	{
		girlwar = tArgs->m_killer;
		player  = girlwar->GetOwner();
	}
	else
	{
		player = tArgs->m_killer;
	}

	const BatterWarConfig& config = ClanLoader::GetInstance()->GetBatterConfig();
	int faction = player->getAttManage()->getValue<DWORD>(eCharFaction);
	AddScore(faction, config.killscores);

	return true;
}

bool CClanCarbon::HandleLeaveClanEvent(const EventArgs & e)
{
	PlayerLeaveClanArgs *tArgs = (PlayerLeaveClanArgs *)(&e);

	if(tArgs->m_obj.Get()==NULL || GetSceneID() != tArgs->m_obj->GetMapID())
		return true;

	Smart_Ptr<Player> player = tArgs->m_obj;

	map<int64,OldMapInfo>::iterator itMap = m_oldMapPoint.find(player->GetID());
	if(itMap != m_oldMapPoint.end())
	{
		PlayerMessage::GetInstance()->ForceChangeMap(player, itMap->second.mapid, itMap->second.xpos, itMap->second.ypos);
	}
	else
	{
		MapStaticInfo *sMapInfo =	NULL;
		int64 sceneid = 0;

		sMapInfo = CMapConfig::GetInstance()->GetFistMapInfo();
		if (!sMapInfo)
			return false;

		sceneid = CSceneMgr::GetInstance()->GetSceneIDAboutPlayerIsLoginOn(sMapInfo->mapid);
		if(sceneid <= 0)
		{
			player->SetMapID(CREATE_MAP(eStaticMap, 1, 1, sMapInfo->mapid, 1));
			sceneid = player->GetMapID();
		}
		else
		{
			player->SetMapID(sceneid);
		}

		PlayerMessage::GetInstance()->ForceChangeMap(player, sceneid, sMapInfo->reliveXPos, sMapInfo->reliveYPos);
	}

	return true;
}
