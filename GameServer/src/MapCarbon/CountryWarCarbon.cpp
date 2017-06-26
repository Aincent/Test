/*
 * CountryWarCarbon.cpp
 *
 *  Created on: 2015年6月30日
 *      Author: root
 */

#include "CountryWarCarbon.h"
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
#include "../MapInstance/BeyondBattleMgr.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../Object/ItemDrop/DropItemAlloc.h"
#include "../Chat/ChatMgr.h"
#include "MessageStruct/Country/CountryWar.pb.h"
#include "../Container/ArticleEquipMgr.h"
#include "../Country/GsCountryMgr.h"
#include "../Mail/MailMessage.h"
#include "./FileLoader/InternationalLoader.h"
#include "../Country/GsCountryWarLoader.h"
#include "../Country/GsCountryWarMgr.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"

bool sortContion(const CountryWarSortData& d1, const CountryWarSortData& d2)
{
	return d1.contribute>d2.contribute;
}

CCountryWarCarbon::CCountryWarCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):CGameDuplicate(onlyid,charid,paramf,params)
{
	m_worldServerID = 0;
}

CCountryWarCarbon::~CCountryWarCarbon()
{
	m_monsterDeadEvent.SetNull();
	m_playerDeadEvent.SetNull();
	m_leaveMapEvent.SetNull();
	m_playerAttackEvent.SetNull();
	m_playerMoveEvent.SetNull();
	m_playerBeAttackEvent.SetNull();
	clearWarTimer();
}

int CCountryWarCarbon::Init()
{
	int ret = CGameDuplicate::Init();

  if(m_WarPlayerSortTimer.IsValid())
	  m_WarPlayerSortTimer.SetNull();

  if(m_countDownTime.IsValid())
	  m_countDownTime.SetNull();

  m_WarPlayerSortTimer=TimerInterface::GetInstance()->RegRepeatedTime(&CCountryWarCarbon::onUpdateSort, this, 0, 0, 0, 0, 0, 60000);
  m_countDownTime = TimerInterface::GetInstance()->RegRepeatedTime(&CCountryWarCarbon::onZonePrize, this, 0, 0, 0, 0, CountryWarLoader::GetInstance()->getZonePrizeTime(),CountryWarLoader::GetInstance()->getZonePrizeTime());
  m_onlinePlayerNumPrize = TimerInterface::GetInstance()->RegRepeatedTime(&CCountryWarCarbon::onOnlieCountPrize, this, 0, 0, 0, 0, CountryWarLoader::GetInstance()->getPrizeTime(),CountryWarLoader::GetInstance()->getPrizeTime());

	return ret;
}

void CCountryWarCarbon::onUpdateSort(void* arg)
{
	std::map<int,std::vector<CountryWarSortData> >::iterator it=m_CountryWarSort.begin();
	for(;it!=m_CountryWarSort.end();++it)
	{
		sort(it->second.begin(),it->second.end(),sortContion);
		for(size_t i=0; i<it->second.size(); ++i)
		{
					Smart_Ptr<Player> player;
					PlayerMessage::GetInstance()->GetPlayerByCharId(player,it->second[i].charID);
					if(player.Get()==NULL)
						continue;
					CountryWarAttrAlter(player,eCountryWarAttrID_sort,i+1);
					std::map<int64,PersonCountryWarInfo>::iterator itPlayer=m_playerCountryWarData.find(it->second[i].charID);
					if(itPlayer!=m_playerCountryWarData.end())
							itPlayer->second.sort = i+1;
		}
	}
}


void CCountryWarCarbon::onZonePrize(void * arg)
{
	//按安全区与非安全区规则家经验
	int rate=CountryWarLoader::GetInstance()->getSafeZonePrizeRate();

	std::vector<Smart_Ptr<Player> > players;
	GetAllPlayers(players);
	for(size_t i=0; i<players.size(); ++i)
	{
			Smart_Ptr<Player>& ref = players[i];
			if(ref.Get()==NULL || ref->IsDead())
					continue;

			if(!CountryWarLoader::GetInstance()->IsSafeZone(ref->GetXNewPos(),ref->GetYNewPos(),0))
			{
					rate=CountryWarLoader::GetInstance()->getNoSafeZonePrizeRate();
			}

			//加经验
			ref->AddExp(CountryWarLoader::GetInstance()->getZoneExp(ref->getAttManage()->getValue<int>(eCharLv),rate));
			//加真气
			//ref->ChangeCurForce(CountryWarLoader::GetInstance()->getZoneForce(ref->getAttManage()->getValue<int>(eCharLv),rate),true);
	}

	//按区域块加经验
	/*map<int,CountryWarMapInfo> infoMap;
	CountryWarLoader::GetInstance()->GetWarMapInfo(infoMap);
	map<int,CountryWarMapInfo>::iterator bitr = infoMap.begin();
	for(; bitr != infoMap.end(); ++ bitr)
	{
		if(bitr->second.nRate == 0)
		{
			continue;
		}

		for(size_t i = 0; i < bitr->second.vPos.size(); i++)
		{
			FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(m_sceneID));
			int sectorIndex = GetSectorIndex(bitr->second.vPos[i].nXPos, bitr->second.vPos[i].nYPos);
			if(sectorIndex < 0 || sectorIndex >= info->GetAllSectorSize())
			{
				LOG_ERROR(FILEINFO, "sectorindex is invalid");
				continue;
			}
			Sector * pSector = &m_totalSector[sectorIndex];
			if(pSector==NULL)
				continue;

			vector<Smart_Ptr<Player> > obj;
			pSector->GetPlayer(obj);
			vector<Smart_Ptr<Player> >::iterator vitr = obj.begin();
			for(; vitr != obj.end(); ++vitr)
			{
					//加经验
					(*vitr)->AddExp(CountryWarLoader::GetInstance()->getZoneExp((*vitr)->GetLv(),bitr->second.nRate));
					//加真气
					(*vitr)->ChangeCurForce(CountryWarLoader::GetInstance()->getZoneForce((*vitr)->GetLv(),bitr->second.nRate),true);
			}
		}
	}*/
}

void CCountryWarCarbon::onOnlieCountPrize(void* arg)
{
		//加阵营积分
		addScore(eCountryID_sui,CountryWarLoader::GetInstance()->getScoreByPlayerNum(m_CountryWarData.playerNum_sui));
		addScore(eCountryID_tang,CountryWarLoader::GetInstance()->getScoreByPlayerNum(m_CountryWarData.playerNum_tang));

		int contribute_sui = CountryWarLoader::GetInstance()->getContributeByNum(m_CountryWarData.playerNum_sui);
		int contribute_tang = CountryWarLoader::GetInstance()->getContributeByNum(m_CountryWarData.playerNum_tang);
		int contrbute=contribute_sui;
		//加个人贡献度
		std::vector<Smart_Ptr<Player> > players;
		GetAllPlayers(players);
		for(size_t i=0; i<players.size(); ++i)
		{
			Smart_Ptr<Player>& ref = players[i];
			if(ref.Get()==NULL)
				continue;
			if(ref->getAttManage()->getValue<DWORD>(eCharCountry)==eCountryID_tang)
				contrbute = contribute_tang;
			else
				contrbute = contribute_sui;

			std::map<int64,PersonCountryWarInfo>::iterator it=m_playerCountryWarData.find(ref->GetID());
			if(it!=m_playerCountryWarData.end())
			{
					it->second.contribute += contrbute;
					CountryWarAttrAlter(ref,eCountryWarAttrID_contribute,it->second.contribute);
			}

			ref->ChangeCountryContribution(contrbute,true);
	    	//更新排序数据
			updateSortData(ref->getAttManage()->getValue<DWORD>(eCharCountry),ref->GetID(),contrbute);
		}
}

bool CCountryWarCarbon::Update()
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
			if(GetPlayerCount() <= 0)
			{
				CountEx();
			}

			CountryWarPrize();
			m_process = eChangeMapWait;

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

int CCountryWarCarbon::Join(Smart_Ptr<Object> & obj)
{
	if(!JoinHandle(obj))
		return -1;

	CGameZone::Join(obj);
	if(obj->GetType()==eMonster)
	{
			int monsterID=((Smart_Ptr<Monster> &)obj)->GetID();
			const CountryWarConfig& config =CountryWarLoader::GetInstance()->getWarConfig();
			std::vector<int>::const_iterator it=find(config.elementID.begin(),config.elementID.end(),monsterID);
			if(it!=config.elementID.end())
			{
				CountryWarElementInfo mon;
				mon.objectKey = obj->GetKey();
				mon.npcID = ((Smart_Ptr<Monster> &)obj)->GetID();
				m_fiveElement[obj->GetKey()] = mon;
			}
	}

	//初始化玩家
	if(obj->GetType()==ePlayer)
	{
		int64 id=((Smart_Ptr<Player> &)obj)->GetID();
		int country = (int)((Smart_Ptr<Player> &)obj)->getAttManage()->getValue<DWORD>(eCharCountry);
		//第一个玩家进来时取世界服服id
		if(m_worldServerID==0)
		{
			m_worldServerID=GET_SERVER_CHARID(id);
		}

		//增加人数
		Smart_Ptr<Player> player = obj;
		if(country==eCountryID_sui)
		{
				++m_CountryWarData.playerNum_sui;
		}
		else if(country==eCountryID_tang)
		{
				++m_CountryWarData.playerNum_tang;
		}

		//任务
		std::vector<int> vTasks;
		CountryWarLoader::GetInstance()->getTaskList(vTasks);
		std::map<int64,PersonCountryWarInfo>::iterator it=m_playerCountryWarData.find(id);
		if(it==m_playerCountryWarData.end())
		{
			PersonCountryWarInfo info;
			for(size_t i=0; i<vTasks.size(); ++i)
			{
				CountryTaskInfo task;
				task.id = vTasks[i];
				info.taskInfo[task.id]=task;
			}
			m_playerCountryWarData[id]=info;
		}

	     //排名
		std::map<int,std::vector<CountryWarSortData> >::iterator itSort=m_CountryWarSort.find(country);
		if(itSort!=m_CountryWarSort.end())
		{
				bool isExist=false;
				for(size_t i=0; i<itSort->second.size();++i)
				{
					if(id==itSort->second[i].charID)
					{
						isExist=true;
						break;
					}
				}
				if(!isExist)
				{
					itSort->second.push_back(CountryWarSortData(id,0));
				}
		}
		else
		{
			std::vector<CountryWarSortData>& ref=m_CountryWarSort[country];
			ref.push_back(CountryWarSortData(id,0));
		}
	}

	return 0;
}

bool CCountryWarCarbon::PlayerCountryWarAttack(const EventArgs & e)
{
	PlayerAttackCreature *pArgs = (PlayerAttackCreature *) (&e);

	if(pArgs->m_player->GetMapID() != m_sceneID)
	{
		return true;
	}

	canccelOccupy(pArgs->m_player->GetID());

	return true;
}

bool CCountryWarCarbon::PlayerCountryWarMove(const EventArgs & e)
{
	PlayerMoveEvent *pArgs = (PlayerMoveEvent *) (&e);
	if(pArgs==NULL)
		return true;

	canccelOccupy(pArgs->m_charID);

	return true;
}

int CCountryWarCarbon::Move(Smart_Ptr<Object> &obj)
{
	CGameZone::Move(obj);
	return 0;
}

bool CCountryWarCarbon::JoinHandle(Smart_Ptr<Object> & obj)
{
	if(m_process >= eDuplicateChangeMap)
		return false;

	vector<int> vec;

	MapInstanceProto::SimInstanceUpdate toClient;

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		std::map<int64,PersonCountryWarInfo>::iterator itr = m_playerCountryWarData.find(player->GetID());
		if(itr != m_playerCountryWarData.end() && !itr->second.isCanEnter)
		{
				return false;
		}
		if(!m_playerAttackEvent.IsValid())
		{
			m_playerAttackEvent = RegGlobalEventPtr(PLAYER_ATTACK_CREATURE, this, &CCountryWarCarbon::PlayerCountryWarAttack);
		}

		if(!m_playerBeAttackEvent.IsValid())
		{
			m_playerBeAttackEvent = RegGlobalEventPtr(PLAYER_BEATTACK, this, &CCountryWarCarbon::HandlePlayerBeAttack);
		}
		if(!m_playerMoveEvent.IsValid())
		{
			m_playerMoveEvent = RegGlobalEventPtr(PLAYER_MOVE, this, &CCountryWarCarbon::PlayerCountryWarMove);
		}

		if(vec.size() > 0)
		{
			player->SynCharAttribute(vec);
		}

		if(!m_monsterDeadEvent.IsValid())
		{
			m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &CCountryWarCarbon::HandleMonsterDeadEvent);
		}

		if(!m_playerDeadEvent.IsValid())
		{
			m_playerDeadEvent = RegGlobalEventPtr(CREATURE_DEAD_EVENT_TYPE, this, &CCountryWarCarbon::HandlePlayerDeadEvent);
		}

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		if(m_process == eCreated)
			m_process = ePlayerWork;
	}

	return true;
}

void CCountryWarCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
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

void CCountryWarCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);

	if(obj->GetType()==ePlayer)
	{
		Smart_Ptr<Player> player = obj;
		if(player->getAttManage()->getValue<DWORD>(eCharCountry)==eCountryID_sui)
		{
			if(m_CountryWarData.playerNum_sui>0)
				--m_CountryWarData.playerNum_sui;
		}
		else if(player->getAttManage()->getValue<DWORD>(eCharCountry)==eCountryID_tang)
		{
			if(m_CountryWarData.playerNum_tang>0)
				--m_CountryWarData.playerNum_tang;
		}
	}
	CGameZone::Leave(obj, isDel, delFromMap);

	if(GetPlayerCount() <= 0)
	{
		m_monsterDeadEvent.SetNull();
		m_playerDeadEvent.SetNull();
//		m_leaveMapEvent.SetNull();
		m_leaveTime = CUtil::GetNowSecond();
	}
}

void CCountryWarCarbon::LeaveExame(Smart_Ptr<Player> & player)
{
	std::map<int64,PersonCountryWarInfo>::iterator it=m_playerCountryWarData.find(player->GetID());
	if(it!=m_playerCountryWarData.end())
	{
		it->second.isCanEnter=false;
	}
}

bool CCountryWarCarbon::IsCanEnter(Smart_Ptr<Player> & player)
{
	//传送阶段，就不允许进入了
	if(m_process >= eDuplicateChangeMap)
		return false;

	//玩家死了不入传入地图
	if(player->IsDead())
	{
		return false;
	}

	if(player->getAttManage()->getValue<DWORD>(eCharCountry) <= eCountryID_null || player->getAttManage()->getValue<DWORD>(eCharCountry) >= eCountryID_max)
		return false;
	std::map<int64,PersonCountryWarInfo>::iterator it=m_playerCountryWarData.find(player->GetID());
	if(it!=m_playerCountryWarData.end() && !it->second.isCanEnter)
		return false;

	return true;
}

void CCountryWarCarbon::Win()
{
	m_isSuccess = true;
	m_process = eAward;
}

void CCountryWarCarbon::Over()
{
	m_process = eDuplicateChangeMap;
}

void CCountryWarCarbon::CountEx()
{
	int64 nowTime = CUtil::GetNowSecond();
	if(nowTime - m_leaveTime >= m_data->m_nDelTime)
	{
		m_process = eOver;
	}
}

void CCountryWarCarbon::Count()
{

}

void CCountryWarCarbon::Failed()
{
	m_process = eAward;
}

void CCountryWarCarbon::Award()
{
	m_process = eDuplicateChangeMap;
	m_awardTime = CUtil::GetNowSecond();
}

void CCountryWarCarbon::canccelOccupy(int64 charID)
{
		std::map<int64,OccupyDataEx>::iterator it=m_OccupyTimerList.find(charID);
		if(it==m_OccupyTimerList.end())
			return;

		if(it->second.m_timer.IsValid())
		{
				it->second.m_timer.SetNull();
		}

		//改变五行珠状态
		std::map<int64,CountryWarElementInfo>::iterator itElement=m_fiveElement.find(it->second.objectKey);
		if(itElement!=m_fiveElement.end())
		{
				itElement->second.state = true;
		}

		if(it->second.m_pMemory != NULL)
		{
				delete it->second.m_pMemory;
				it->second.m_pMemory = NULL;
		}

		//清除
		m_OccupyTimerList.erase(it);

}

//玩家被攻击事件
bool CCountryWarCarbon::HandlePlayerBeAttack(const EventArgs & e)
{
		PlayerBeAttackArgs *tArgs = (PlayerBeAttackArgs *)(&e);
		if(GetSceneID() != tArgs->m_self->GetMapID())
			return true;

		if(tArgs->m_self->GetType() != ePlayer)
			return true;

		canccelOccupy(tArgs->m_self->GetID());

	return true;
}

//排名数据更新
void CCountryWarCarbon::updateSortData(int country,int64 charid,int contribute)
{
	std::map<int,std::vector<CountryWarSortData> >::iterator it=m_CountryWarSort.find(country);
	if(it==m_CountryWarSort.end())
		return;

	std::vector<CountryWarSortData>::iterator itSort=std::find_if(it->second.begin(),it->second.end(),CountryWarDataFind(charid));
	if(itSort!=it->second.end())
	{
		 (*itSort).contribute += contribute;
	}
}

void CCountryWarCarbon::CountryWarAttrAlter(Smart_Ptr<Player>& player,int id,int value)
{
	if(player.Get()==NULL)
		return;

	//个人贡献度
	/*if(eCountryWarAttrID_contribute==id)
	{
		player->ChangeCountryContribution(value,true);

		//更新排序数据
		updateSortData(player->getAttManage()->getValue<DWORD>(eCharCountry),player->GetID(),value);
	}*/

	CountryWar::CountryWarAttrAlter cmd;
	cmd.set_id(id);
	cmd.set_value(value);
	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(), &cmd, MSG_SIM_GS2C_COUNTRY_WAR_ATTR_ALTER);
}

void CCountryWarCarbon::updateSubTaskTarget(Smart_Ptr<Player>& player)
{
	if(player.Get()==NULL)
		return;

	std::map<int64,PersonCountryWarInfo>::iterator it=m_playerCountryWarData.find(player->GetID());
	if(it==m_playerCountryWarData.end())
		return;

	std::map<int,CountryTaskInfo>::iterator itTask=it->second.taskInfo.begin();
	for(; itTask!=it->second.taskInfo.end(); ++itTask)
	{
		const CountryWarTaskConfigInfo* pTask=CountryWarLoader::GetInstance()->getTaskConfig(itTask->second.id);
		if(pTask==NULL)
			continue;
		if(pTask->type==eCountryWarTask_subTagert)
		{
			//更新任务目标
			if(itTask->second.targetCount<pTask->targetCount)
			{
				itTask->second.targetCount++;
				if(itTask->second.targetCount>=pTask->targetCount)
				{
					itTask->second.flag = CountryTaskState_ok;
				}
				//通知客户端数据更新
				CountryWar::CountryWarTaskUpdate cmd;
				cmd.set_id(itTask->second.id);
				cmd.set_count(itTask->second.targetCount);
				QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(), &cmd, MSG_SIM_GS2C_COUNTRY_WAR_TASK_UPDATE);
			}
			break;
		}
	}
}

//更新任务目标杀怪进度
void CCountryWarCarbon::updateCountryWarTask(int64 charID,int channel,int monsterID)
{
	std::map<int64,PersonCountryWarInfo>::iterator it=m_playerCountryWarData.find(charID);
	if(it==m_playerCountryWarData.end())
		return;

	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharId(player,charID);

	//增加阵营积分  个人贡献度
	int score=CountryWarLoader::GetInstance()->getScoreByMonster(monsterID);
	if(score>0)
	{
			addScore(player->getAttManage()->getValue<DWORD>(eCharCountry),score);
			it->second.contribute += score;
			CountryWarAttrAlter(player,eCountryWarAttrID_contribute,it->second.contribute);

			//更新数据
			if(player.Get())
			{
					player->ChangeCountryContribution(score,true);
					updateSortData(player->getAttManage()->getValue<DWORD>(eCharCountry),player->GetID(),score);
			}
	}

	bool isCompleteSubTask=false;
	std::map<int,CountryTaskInfo>::iterator itTask=it->second.taskInfo.begin();
	for(;itTask!=it->second.taskInfo.end(); ++itTask)
	{
		const CountryWarTaskConfigInfo* pTask=CountryWarLoader::GetInstance()->getTaskConfig(itTask->second.id);
		if(pTask==NULL)
			continue;
		if(pTask->type==eCountryWarTask_monster && pTask->targetID==monsterID)
		{
			//更新杀怪目标数
			if(itTask->second.targetCount<pTask->targetCount)
			{
				itTask->second.targetCount++;
				//通知客户端数据更新
				CountryWar::CountryWarTaskUpdate cmd;
				cmd.set_id(itTask->second.id);
				cmd.set_count(itTask->second.targetCount);
				QuickMessage::GetInstance()->AddSendMessage(charID, channel, &cmd, MSG_SIM_GS2C_COUNTRY_WAR_TASK_UPDATE);
				if(itTask->second.targetCount>=pTask->targetCount)
				{
						isCompleteSubTask=true;
						itTask->second.flag = CountryTaskState_ok;
				}
			}
			break;
		}
	}
	//更新子任务
	if(isCompleteSubTask)
	{
			if(player.Get()!=NULL)
			{
					updateSubTaskTarget(player);
			}
	}
}

bool CCountryWarCarbon::HandleMonsterDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);

	if(GetSceneID() != tArgs->m_myself->GetMapID())
		return true;


	Smart_Ptr<Monster> mon = tArgs->m_myself;
	LOG_DEBUG(FILEINFO, "CGameDuplicate::HandleMonsterDeadEvent \t %lld", mon->GetID());

	if(tArgs->m_myself->GetType() == eMonster)
	{
		Smart_Ptr<Player>  player;
		Smart_Ptr<GirlWar>  girlwar;
		if(tArgs->m_other->GetType()== ePlayer)
		{
			player = tArgs->m_other;
		}
		else if(tArgs->m_other->GetType() == ePet)
		{
			girlwar = tArgs->m_other;
			player  = girlwar->GetOwner();
		}
		if(player.Get()!=NULL)
		{
				updateCountryWarTask(player->GetID(),player->GetChannelID(),(int)mon->GetID());
		}
	}

	return true;
}

void CCountryWarCarbon::onOccupyTimer(void * arg)
{
	OccupyData* pData = (OccupyData*)(arg);
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
	//通知玩家占领成功
	CountryWar::OccupyElementSuccess cmdmsg;
	cmdmsg.set_id(temp.objectKey);
	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(), &cmdmsg, MSG_SIM_GS2C_OCCUPY_SUCCESS);

	//占领处理
	updateElementState(temp.objectKey,(long)player->getAttManage()->getValue<DWORD>(eCharCountry));

	//全服跑马灯公告
	broadcastOccupyAll();

	//增加阵营积分
	const CountryWarConfig& config= CountryWarLoader::GetInstance()->getWarConfig();
	int score=config.occupyScore;
	addScore(player->getAttManage()->getValue<DWORD>(eCharCountry),score);

	bool isCompleteSubTask=false;
	std::map<int64,PersonCountryWarInfo>::iterator itPlayer=m_playerCountryWarData.find(temp.charID);
	if(itPlayer!=m_playerCountryWarData.end())
	{
		//增加个人贡献度
		itPlayer->second.contribute += score;
		CountryWarAttrAlter(player,eCountryWarAttrID_contribute,itPlayer->second.contribute);
		//更新数据
		player->ChangeCountryContribution(score,true);
		updateSortData(player->getAttManage()->getValue<DWORD>(eCharCountry),player->GetID(),score);

		//更新任务进度
		std::map<int,CountryTaskInfo>::iterator itTask=itPlayer->second.taskInfo.begin();
		for(; itTask!=itPlayer->second.taskInfo.end(); ++itTask)
		{
			const CountryWarTaskConfigInfo* pTask=CountryWarLoader::GetInstance()->getTaskConfig(itTask->second.id);
			if(pTask==NULL)
				continue;
			if(pTask->type==eCountryWarTask_occup)
			{
				if(itTask->second.targetCount<pTask->targetCount)
				{
					itTask->second.targetCount++;
					//通知客户端数据更新
					CountryWar::CountryWarTaskUpdate cmd;
					cmd.set_id(itTask->second.id);
					cmd.set_count(itTask->second.targetCount);
					QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(), &cmd, MSG_SIM_GS2C_COUNTRY_WAR_TASK_UPDATE);

					//通知完成子目标任务数
					if(itTask->second.targetCount>=pTask->targetCount)
					{
						isCompleteSubTask = true;
						itTask->second.flag = CountryTaskState_ok;
					}
				}
				break;
			}
		}
	}
	//更新子任务
	if(isCompleteSubTask)
	{
		updateSubTaskTarget(player);
	}

	//通知全地图占领状态
	CountryWar::ElementStateAlter cmd;
	cmd.set_id(temp.objectKey);
	cmd.set_value(player->getAttManage()->getValue<DWORD>(eCharCountry));
	SendMapAllSynchMessage(&cmd,MSG_SIM_GS2C_COUNTRY_WAR_ELEMENT_STATE,0,true);
}

//占领五行柱
bool CCountryWarCarbon::occupyElement(int64 charID,int64 object)
{
		std::map<int64,CountryWarElementInfo>::iterator it=m_fiveElement.find(object);
		if(it==m_fiveElement.end())
			return false;
		if(!it->second.state)
			return false;
		if(it->second.time>0)
			return false;

		Smart_Ptr<Player> player;
		PlayerMessage::GetInstance()->GetPlayerByCharId(player,charID);
		if( player.Get()==NULL || (DWORD)it->second.owner==player->getAttManage()->getValue<DWORD>(eCharCountry))
			return false;

		it->second.state = false;

		//开始计时
		OccupyData* pData = new OccupyData();
		pData->charID = charID;
		pData->objectKey = object;

		OccupyDataEx& tmp = m_OccupyTimerList[charID];
		tmp.charID = pData->charID;
		tmp.objectKey = pData->objectKey;
		tmp.m_pMemory = pData;
		if(tmp.m_timer.IsValid())
			tmp.m_timer.SetNull();
		tmp.m_timer = TimerInterface::GetInstance()->RegTimePoint(&CCountryWarCarbon::onOccupyTimer, this, (void*)pData, 0, 0, 10, 0);
		return true;
}

void CCountryWarCarbon::countryWarKillPlayer(Smart_Ptr<CreatureObj>& obj,Smart_Ptr<Player>& target)
{
	//更新杀人数据
	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player=obj;
		//增加阵营积分
		const CountryWarConfig& config= CountryWarLoader::GetInstance()->getWarConfig();
		int score=config.killScore;
		addScore(player->getAttManage()->getValue<DWORD>(eCharCountry),score);

		bool isCompleteSubTask=false;
		std::map<int64,PersonCountryWarInfo>::iterator it=m_playerCountryWarData.find(player->GetID());
		if(it!=m_playerCountryWarData.end())
		{
			//增加个人贡献度
			it->second.contribute += score;
			CountryWarAttrAlter(player,eCountryWarAttrID_contribute,it->second.contribute);
			//更新数据
			if(player.Get())
			{
					player->ChangeCountryContribution(score,true);
					updateSortData(player->getAttManage()->getValue<DWORD>(eCharCountry),player->GetID(),score);
			}

			std::map<int,CountryTaskInfo>::iterator itTask=it->second.taskInfo.begin();
			for(;itTask!=it->second.taskInfo.end(); ++itTask)
			{
				const CountryWarTaskConfigInfo* pTask=CountryWarLoader::GetInstance()->getTaskConfig(itTask->second.id);
				if(pTask==NULL)
						continue;
				if(pTask->type==eCountryWarTask_kill)
				{
					//更新任务目标
					if(itTask->second.targetCount<pTask->targetCount)
					{
							itTask->second.targetCount++;
							//通知客户端数据更新
							CountryWar::CountryWarTaskUpdate cmd;
							cmd.set_id(itTask->second.id);
							cmd.set_count(itTask->second.targetCount);
							QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(), &cmd, MSG_SIM_GS2C_COUNTRY_WAR_TASK_UPDATE);

						if(itTask->second.targetCount>=pTask->targetCount)
							{
								isCompleteSubTask=true;
								itTask->second.flag = CountryTaskState_ok;
							}
					}
					break;
				}
			}
		}

		//更新子任务
		if(isCompleteSubTask)
		{
			updateSubTaskTarget(player);
		}
	}

	//更新被杀数据
	if(target.Get()!=NULL)
	{
		std::map<int64,PersonCountryWarInfo>::iterator it=m_playerCountryWarData.find(target->GetID());
		if(it!=m_playerCountryWarData.end())
		{
			it->second.beKilled++;
			CountryWarAttrAlter(target,eCountryWarAttrID_bekill,it->second.beKilled);
		}
	}
}

bool CCountryWarCarbon::HandlePlayerDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	PlayerKilledEventArgs *tArgs = (PlayerKilledEventArgs *)(&e);

	if(GetSceneID() != tArgs->m_dead->GetMapID() || tArgs->m_dead->GetType() != ePlayer)
		return true;

	countryWarKillPlayer(tArgs->m_killer,tArgs->m_dead);
	return true;
}

//得到五行柱信息
void CCountryWarCarbon::getElementInfo(std::map<int64,CountryWarElementInfo>& info)
{
	std::map<int64,CountryWarElementInfo>::iterator it=m_fiveElement.begin();
	for(; it!=m_fiveElement.end(); ++it)
	{
		CountryWarElementInfo ref;
		ref.objectKey = it->second.objectKey;
		ref.npcID = it->second.npcID;
		ref.owner = it->second.owner;
		ref.state = it->second.state;
		ref.time  = it->second.time;
		info[ref.objectKey] = ref;
	}
}

//得到玩家战绩信息
void CCountryWarCarbon::getWarInfo(int64 charID,PersonCountryWarInfo& info,CountryWarData& score)
{
		std::map<int64,PersonCountryWarInfo>::const_iterator it = m_playerCountryWarData.find(charID);
		if(it!=m_playerCountryWarData.end())
		{
				info=it->second;
		}
		score = m_CountryWarData;
}

bool CCountryWarCarbon::fitchTaskPrize(int64 charID,int taskID)
{
	std::map<int64,PersonCountryWarInfo>::iterator it = m_playerCountryWarData.find(charID);
	if(it==m_playerCountryWarData.end())
		return false;
	std::map<int,CountryTaskInfo>::iterator itTask=it->second.taskInfo.find(taskID);
	if(itTask==it->second.taskInfo.end())
		return false;
	if(itTask->second.flag!=CountryTaskState_ok)
		return false;
	itTask->second.flag = CountryTaskState_yes;
	return true;
}

void CCountryWarCarbon::CountryWarFinalPrize(int country,bool isWin)
{
	//奖励
	std::map<int,std::vector<CountryWarSortData> >::iterator it=m_CountryWarSort.find(country);
	if(it==m_CountryWarSort.end())
		return;

	sort(it->second.begin(), it->second.end(),sortContion);

	const CountryWarConfig& config=CountryWarLoader::GetInstance()->getWarConfig();
	for(size_t i=0; i<it->second.size(); ++i)
	{
		std::vector<PropItem> items;

		//胜利的奖励
  	if(isWin)
		{
  		for(size_t n = 0; n < config.winPrizeProps.size(); ++n)
  			{
  			items.push_back( PropItem(config.winPrizeProps[n].first, config.winPrizeProps[n].second) );
  			}
		}
  	else
  		{
  		for(size_t n = 0; n < config.winPrizeProps.size(); ++n)
  		  	{
  		  items.push_back( PropItem(config.failPrizeProps[n].first, config.failPrizeProps[n].second) );
  		  	}
  		}

		//第一名
		if(i==0)
		{
			items.push_back( PropItem(config.firstPrize, config.firstPrizeNum) );
		}
		else if(i==1)//第二名
		{
			items.push_back( PropItem(config.secondPrize, config.secondPrizeNum) );
		}
		else if(i==2)//第三
		{
			items.push_back( PropItem(config.thirdPrize, config.thirdPrizeNum) );
		}

		//发送邮件
		std::string strReward = InternationalLoader::GetInstance()->GetInterStr(19);
		MailMessageMgr::GetInstance()->SendMail(it->second[i].charID, items, strReward);

		//通知奖励信息
		CountryWar::CountryWarPrizeResult ret;
		ret.set_scoresui(m_CountryWarData.score_sui);
		ret.set_scoretang(m_CountryWarData.score_tang);
		ret.set_contribute(it->second[i].contribute);
		ret.set_sort(i+1);

		for(size_t n=0; n<items.size(); ++n)
		{
			CountryWar::PrizeItem* pItem=ret.add_item();
			pItem->set_id(items[n].id);
			pItem->set_num(items[n].num);
		}

		Smart_Ptr<Player> player;
		PlayerMessage::GetInstance()->GetPlayerByCharId(player,it->second[i].charID);
		if(player.Get()!=NULL)
		{
			QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(), &ret, MSG_SIM_GS2C_COUNTRY_WAR_REWARD);
		}
	}
}

void CCountryWarCarbon::broadcastOccupyAll()
{
	//string strM = ChatMgr::GetInstance()->GetStringByInt(attacked->GetID());
	//string strE = ChatMgr::GetInstance()->GetStringByInt(0);
	//ChatMgr::GetInstance()->Marquee(eMarqueeOccupyAll,"",strM,strE);
}

void CCountryWarCarbon::broadcastContinueWin(int winCount)
{
	//string strM = ChatMgr::GetInstance()->GetStringByInt(attacked->GetID());
	//string strE = ChatMgr::GetInstance()->GetStringByInt(0);
	//ChatMgr::GetInstance()->Marquee(eMarqueeContinueWin,"",strM,strE);
}


void CCountryWarCarbon::CountryWarPrize()
{
	//胜利方
	int WinCountry=eCountryID_sui;
	int LoseCountry=eCountryID_tang;
	if(m_CountryWarData.score_sui<m_CountryWarData.score_tang)
	{
			WinCountry=eCountryID_tang;
			LoseCountry=eCountryID_sui;
	}
	CountryWarFinalPrize(WinCountry,true);
	CountryWarFinalPrize(LoseCountry,false);

	//通知世界服结果
	GsCountryWarMgr::GetInstance()->noticeWarResult(getWorldServerID(),WinCountry,LoseCountry);

	//阵营物品奖励
	const CountryWarConfig& config=CountryWarLoader::GetInstance()->getWarConfig();
	int winCount=GsCountryWarMgr::GetInstance()->getWinCount(WinCountry);
	bool isExPrize=false;
	if(config.winTimesParam1>0 && (winCount+1)/config.winTimesParam1==0)
	{
		isExPrize=true;
	}
	for(size_t i=0; i<config.countryWinprops.size(); ++i)
	{
		CountryBossPackage::CountryBossWin toWorld;
		toWorld.set_country(WinCountry);
		toWorld.add_itemid(config.countryWinprops[i].first);
		if(isExPrize)
			toWorld.add_num(config.countryWinprops[i].second*config.winTimesParam2);
		else
			toWorld.add_num(config.countryWinprops[i].second);
		toWorld.set_money(0);

		Smart_Ptr<SvrItem> worldSvr;
		ServerConHandler::GetInstance()->GetWorldServerBySvrID(getWorldServerID(),worldSvr);
		if(worldSvr)
		{
			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_SYSWIN_REWARD, &toWorld, worldSvr.Get(), SimpleMessage);
			Message_Facade::Send(messRet);
		}
	}

	//公告连胜场次
	if(isExPrize)
	{
		broadcastContinueWin(winCount);
	}

	//销毁所有定时器
	clearWarTimer();

}

//增加积分
void CCountryWarCarbon::addScore(int country, int score)
{
	if(score<=0)
		return;

	CountryWar::CountryWarAttrAlter cmd;
	if(country==eCountryID_sui)
	{
		m_CountryWarData.score_sui += score;
		cmd.set_id(eCountryWarAttrID_sui);
		cmd.set_value(m_CountryWarData.score_sui);
	}
	else if(eCountryID_tang)
	{
		m_CountryWarData.score_tang += score;
		cmd.set_id(eCountryWarAttrID_tang);
		cmd.set_value(m_CountryWarData.score_tang);
	}
	else
	{
		return;
	}
	//全地图通知
	SendMapAllSynchMessage(&cmd,MSG_SIM_GS2C_COUNTRY_WAR_ATTR_ALTER,0,true);
}

void CCountryWarCarbon::updateElementState( int64 object, long country )
{
	std::map<int64,CountryWarElementInfo>::iterator it=m_fiveElement.find(object);
	if(it==m_fiveElement.end())
		return;
	it->second.owner = country;
	it->second.state = true;
	it->second.time = CUtil::GetNowSecond();//取当前时间

	const CountryWarConfig& config = CountryWarLoader::GetInstance()->getWarConfig();
	//启动保护时间定时器
	int64* pElementKey=new int64(object);
	if(it->second.m_protectedtimer.IsValid())
		it->second.m_protectedtimer.SetNull();

	if(it->second.m_prizetimer.IsValid())
		it->second.m_prizetimer.SetNull();

	it->second.m_protectedtimer = TimerInterface::GetInstance()->RegTimePoint(&CCountryWarCarbon::protectedTimer, this, (void*)(pElementKey), 0, 0, config.protectime, 0);
	//启动增加积分定时器
	it->second.m_prizetimer = TimerInterface::GetInstance()->RegRepeatedTime(&CCountryWarCarbon::addScoreTimer, this, (void*)(country),0,0,0,0,config.addscoreTime*1000);
}

//删除定时器
void CCountryWarCarbon::clearWarTimer()
{
			//销毁所有定时器
			if(m_WarPlayerSortTimer.IsValid())
				m_WarPlayerSortTimer.SetNull();

			if(m_countDownTime.IsValid())
				m_countDownTime.SetNull();

			if(m_onlinePlayerNumPrize.IsValid())
				m_onlinePlayerNumPrize.SetNull();

			std::map<int64,CountryWarElementInfo>::iterator itElement=m_fiveElement.begin();
			for(; itElement!=m_fiveElement.end(); ++itElement)
			{
				if(itElement->second.m_prizetimer.IsValid())
				{
					itElement->second.m_prizetimer.SetNull();
				}
				if(itElement->second.m_protectedtimer.IsValid())
				{
					itElement->second.m_protectedtimer.SetNull();
				}
			}

			std::map<int64,OccupyDataEx>::iterator itOccupy=m_OccupyTimerList.begin();
			for(;itOccupy!=m_OccupyTimerList.end();++itOccupy)
			{
				if(itOccupy->second.m_timer.IsValid())
				{
					itOccupy->second.m_timer.SetNull();
				}
				if(itOccupy->second.m_pMemory !=NULL)
				{
						delete itOccupy->second.m_pMemory;
						itOccupy->second.m_pMemory = NULL;
				}
			}
}

void CCountryWarCarbon::protectedTimer(void * arg)
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
		//通知全地图
		CountryWar::ElementProtectedTimeOut cmd;
		cmd.set_id(tmp);
		SendMapAllSynchMessage(&cmd,MSG_SIM_GS2C_ELEMENT_PROTECTED_TIME,0,true);
}

void CCountryWarCarbon::addScoreTimer(void* arg)
{
		long country = (long)(arg);
		//增加积分
		const CountryWarConfig& config=CountryWarLoader::GetInstance()->getWarConfig();
		int score=config.addscore;
		bool isAll=true;
		std::map<int64,CountryWarElementInfo>::iterator it=m_fiveElement.begin();
		for(; it!=m_fiveElement.end(); ++it)
		{
			if(country!=it->second.owner)
				{
					isAll=false;
					break;
				}
		}
		if(isAll)
		{
			score = config.addscore*config.scoreRate;
		}
		addScore(country,score);
}

bool CCountryWarCarbon::IsAttacked(int64 fight, int64 attacked)
{
	Smart_Ptr<Player> player = GetPlayer(attacked);
	if(player)
	{
		bool isSafe=CountryWarLoader::GetInstance()->IsSafeZone(player->GetXNewPos(),player->GetYNewPos(),player->getAttManage()->getValue<DWORD>(eCharCountry));
		if(isSafe)
			return false;
	}

	return true;
}
