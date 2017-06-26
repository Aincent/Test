/*
 * StatisticMgr.cpp
 *
 *  Created on: 2014年10月9日
 *      Author: root
 */

#include "StatisticMgr.h"

//#include <bits/stl_map.h>
//#include <bits/stl_pair.h>
//#include <bits/stl_tree.h>
#include <CharDefine.h>
#include <define.h>
#include <EventSet/BoundSlot.h>
#include <EventSet/EventArgs.h>
#include <EventSet/EventSet.h>
#include <EventSet/SubscriberSlot.h>
#include <Log/Logger.h>
#include "../ServerEventDefine.h"
#include <Singleton.h>
#include <Smart_Ptr.h>
#include <StatisticBase/StatisticDefine.h>
#include <Timer/Timer.h>
#include <Timer/TimerInterface.h>
//#include <c++/4.1.1/new>
#include <cassert>

#include "../EventSet/EventDefine.h"
#include "../EventSet/GlobalEvent.h"
#include "../MapInstance/InstanceMapLoader.h"
#include "../Object/Creature.h"
#include "../Object/Player/Player.h"
#include "./FileLoader/GameParaLoader.h"
#include "../ServerManager.h"
#include "../MessageBuild.h"

StatisticMgr::StatisticMgr()
{
		//每十分钟发送一次记录并清除数据

//	if(m_stime.IsValid())
//	{
//		m_stime.SetNull();
//	}
//	m_stime = TimerInterface::GetInstance()->RegRepeatedTime(&StatisticMgr::SendDataUpdate,this, 0, 0, 0, 0, 1 *Time_Save_Count ,1 * Time_Save_Count);
//
//	m_passEvent = RegGlobalEventPtr(PLAYER_GLOBE_PASSINSTANCE, this,&StatisticMgr::StatisticDailyTimes);
//	m_playerLvUpgradeEvent = RegGlobalEventPtr(PLAYER_GLOBE_PASSINSTANCE, this,&StatisticMgr::StatisticPlayerLvUpgrade);
//	m_playerBeKilledEvent = RegGlobalEventPtr(CREATURE_DEAD_EVENT_TYPE, this, &StatisticMgr::HandlePlayerBeKilledEvent);
//	m_currencyChangeEvent = RegGlobalEventPtr(STATISTIC_CURRENCY_CHANGE, this, &StatisticMgr::HandleCurrencyChangeEvent);
//	m_goldenChangeEvent = RegGlobalEventPtr(STATISTIC_GOLDEN_CHANGE, this, &StatisticMgr::HandleGoldenChangeEvent);

	if (m_timerSend.IsValid())
	{
		m_timerSend.SetNull();
	}

	m_timerSend = TimerInterface::GetInstance()->RegRepeatedTime(&StatisticMgr::SendData, this, 0, 0, 0, 0, 1 * Time_Save_Count, 1 * Time_Save_Count);

}

StatisticMgr::~StatisticMgr()
{
//	if(m_stime.IsValid())
//	{
//		m_stime.SetNull();
//	}
//
//	if(m_playerLvUpgradeEvent.IsValid())
//		m_playerLvUpgradeEvent.SetNull();
//
//	if(m_passEvent.IsValid())
//		m_passEvent.SetNull();
//
//	if(m_currencyChangeEvent.IsValid())
//		m_currencyChangeEvent.SetNull();
//
//	m_DailyStatistic.clear();
//
//	m_playerStatistic.clear();
//
//	m_playerBehavior.clear();

	if (m_timerSend.IsValid())
	{
		m_timerSend.SetNull();
	}
}

//void StatisticMgr::SetData(StatisticInfo::UseStatisticData * statisticData, int64 data1, int64 data2,string data3,bool addFlag,string data4)
//{
//	if(bool(statisticData) == false)
//		return;
//
//	int srcLen = statisticData->behaviorlen();
//	string srcData = statisticData->behaviordata();
//
//	char str[MAX_RECV_MSG_SIZE] = {0};
//	int len = 0;
//
//	int saveType = statisticData->savetype();
//	switch(saveType)
//	{
//	case eIntToInt:
//		{
//			ServerReturn::ServerDoubleUint32 data;
//			if(srcLen >0)
//			{
//				data.ParsePartialFromString(srcData);
//				data.set_retf(data.retf() + data1);
//				data.set_rets(data.rets() + data2);
//			}
//			else
//			{
//				data.set_retf(data1);
//				data.set_rets(data2);
//			}
//			data.SerializePartialToArray(str, data.ByteSize());
//			len = data.ByteSize();
//			break;
//		}
//		case eInt64:
//		{
//			ServerReturn::ServerRetInt data;
//			if(srcLen >0)
//			{
//				data.ParsePartialFromString(srcData);
//				data.set_ret(data.ret() + data1);
//			}
//			else
//				data.set_ret(data1);
//			data.SerializePartialToArray(str, data.ByteSize());
//			len = data.ByteSize();
//			break;
//		}
//		case eVector:
//		{
//			StatisticInfo::StatisticSaveTypeVector data;
//			if(srcLen >0)
//				data.ParsePartialFromString(srcData);
//			data.add_vec(data1);
//			data.set_size(data.vec_size());
//			data.SerializePartialToArray(str, data.ByteSize());
//			len = data.ByteSize();
//			break;
//		}
//		case eVecToStr:
//		{
//			StatisticInfo::StatisticSaveTypeIntToString data;
//			data.set_data1(data1);
//			data.set_data2(data3.c_str());
//			data.SerializePartialToArray(str, data.ByteSize());
//			len = data.ByteSize();
//			break;
//		}
//		case eMap:
//		{
//			bool isFind = false;
//			ServerReturn::ServerDoubleUint32Array data;
//			if(srcLen >0)
//				data.ParsePartialFromString(srcData);
//
//			for(int i = 0;i < data.ret_size();++i)
//			{
//				ServerReturn::ServerDoubleUint32 * tempData = data.mutable_ret(i);
//				if(tempData->retf() == data1)
//				{
//					if(addFlag)
//						tempData->set_rets(tempData->rets() + data2);
//					else
//						tempData->set_rets(data2);
//					isFind = true;
//				}
//			}
//
//			if(!isFind)
//			{
//				ServerReturn::ServerDoubleUint32 * ret = data.add_ret();
//				ret->set_retf(data1);
//				ret->set_rets(data2);
//			}
//
//			data.SerializePartialToArray(str, data.ByteSize());
//			len = data.ByteSize();
//			break;
//		}
//		case eDW64AndVec:
//		{
//			StatisticInfo::StatisticSaveTypeDW64AndVec data;
//			if(srcLen >0)
//				data.ParsePartialFromString(srcData);
//			else
//				data.set_data1(data1);
//			data.add_data2(data2);
//			data.SerializePartialToArray(str, data.ByteSize());
//			len = data.ByteSize();
//			break;
//		}
//		case eVecByIntToStr:
//		{
//			StatisticInfo::StatisticSaveTypeVecByIntToString data;
//			if(srcLen >0)
//				data.ParsePartialFromString(srcData);
//			StatisticInfo::StatisticSaveTypeIntToString * intToStringData = data.add_vecdata();
//			intToStringData->set_data1(data1);
//			intToStringData->set_data2(data3.c_str());
//			intToStringData->set_source(data4.c_str());
//			data.SerializePartialToArray(str, data.ByteSize());
//			len = data.ByteSize();
//			break;
//		}
//	}
//
//	statisticData->set_behaviorlen(len);
//	statisticData->set_behaviordata(str,len);
//}
//
//void StatisticMgr::SaveDailyData(DWORD main,DWORD minor,StatisticInfo::UseStatisticData& data)
//{
//	DWORD key = 0;
//	key = (main << 16) | minor;
//
//	m_DailyStatistic[key] = data;
//}
//
//void StatisticMgr::SavePlayerData(const Smart_Ptr<Player> &player,DWORD main,DWORD minor,StatisticInfo::UseStatisticData& data)
//{
//	if(!player)
//		return;
//
//	if(player->GetAccountType() != eAccountType_User)
//		return;
//
//	int64 charID = player->GetID();
//
//	DWORD key =	0;
//	key = (main << 16) | minor;
//	std::map<int64, StatisticMap>::iterator it = m_playerStatistic.find(charID);
//	if(it == m_playerStatistic.end())
//	{
//		StatisticMap sta;
//		sta.clear();
//		sta[key].CopyFrom(data);
//		m_playerStatistic[charID] = sta;
//	}
//	else
//	{
//		it->second[key].CopyFrom(data);
//	}
//}
//
//void StatisticMgr::SavePlayerBehaviorData(int64 charID,DWORD main,DWORD minor,StatisticInfo::UseStatisticData& data)
//{
//	if(charID <= 0)
//		return;
//
//	if(m_playerBehavior.size() > 10000)
//	{
//		LOG_FATAL(FILEINFO, "behavior memory cache too large");
//		return;
//	}
//
//	std::map<int64,std::vector<StatisticInfo::UseStatisticData> >::iterator it = m_playerBehavior.find(charID);
//	if(it != m_playerBehavior.end())
//	{
//		it->second.push_back(data);
//	}
//	else
//	{
//		std::vector<StatisticInfo::UseStatisticData> tempVector;
//		tempVector.push_back(data);
//		m_playerBehavior[charID] = tempVector;
//	}
//}
//
//StatisticInfo::UseStatisticData* StatisticMgr::GetPlayerData(const Smart_Ptr<Player> &player,DWORD main,DWORD minor)
//{
//	DWORD key = 0;
//	key = (main << 16) | minor;
//	std::map<int64, StatisticMap>::iterator it = m_playerStatistic.find(player->GetID());
//	if(it == m_playerStatistic.end())
//	{
//		return NULL;
//	}
//	else
//	{
//		StatisticMap::iterator data = it->second.find(key);
//		if(data == it->second.end())
//		{
//			return NULL;
//		}
//		else
//		{
//			return &(data->second);
//		}
//	}
//}
//
//StatisticInfo::UseStatisticData* StatisticMgr::GetDailyData(DWORD main,DWORD minor)
//{
//	DWORD key = 0;;
//	key = (main << 16) | minor;
//
//	StatisticMap::iterator it = m_DailyStatistic.find(key);
//
//	if(it == m_DailyStatistic.end())
//	{
//		return NULL;
//	}
//	else
//	{
//		return &(it->second);
//	}
//}
//
//int StatisticMgr::GetDailySaveType(DWORD main,DWORD minor)
//{
//	DWORD key = 0;
//	key = (main << 16) | minor;
//
//	StatisticMap::iterator it = m_DailyStatistic.find(key);
//
//	if(it == m_DailyStatistic.end())
//	{
//		return -1;
//	}
//	else
//	{
//		return it->second.savetype();
//	}
//}
//
//int StatisticMgr::GetPlayerSaveType(const Smart_Ptr<Player> &player,DWORD main,DWORD minor)
//{
//	DWORD key = 0;
//	key = (main << 16) | minor;
//
//	std::map<int64, StatisticMap>::iterator it = m_playerStatistic.find(player->GetID());
//
//	if(it == m_playerStatistic.end())
//	{
//		return -1;
//	}
//	else
//	{
//		DWORD key =	0;
//		key = (main << 16) | minor;
//
//		StatisticMap sta = it->second;
//		StatisticMap::iterator ita = sta.find(key);
//
//		if(ita == sta.end())
//		{
//			return -1;
//		}
//		else
//		{
//			return (ita->second).savetype();
//		}
//	}
//}
//
//
//void StatisticMgr::StatisticDailyTimes(DWORD mainType, DWORD minorType,DWORD num)
//{
//	StatisticInfo::UseStatisticData *saveData = StatisticMgr::GetInstance()->GetDailyData(mainType,minorType);
//	if(!saveData)
//	{
//		StatisticInfo::UseStatisticData statisticData;
//		statisticData.set_maintype(mainType);
//		statisticData.set_minortype(minorType);
//		statisticData.set_charid(0);
//		statisticData.set_savetype(eInt64);
//		statisticData.set_behaviorlen(0);
//		statisticData.set_behaviordata("");
//
//		SetData(&statisticData,num);
//		SaveDailyData(mainType,minorType,statisticData);
//	}
//	else
//	{
//		SetData(saveData,num);
//	}
//}

//void StatisticMgr::StatisticPlayerDailyTimes(const Smart_Ptr<Player> &player, DWORD mainType, DWORD minorType,DWORD num )
//{
//	if(!player)
//	{
//		return;
//	}
//
//	if(player->GetAccountType() != eAccountType_User)
//		return;
//
//	//存的时候要用去掉serverID的角色ID
//	int64 charID = GET_PLAYER_CHARID(player->GetID());
//
//	StatisticInfo::UseStatisticData *saveData = StatisticMgr::GetInstance()->GetPlayerData(player,mainType,minorType);
//	if(!saveData)
//	{
//		StatisticInfo::UseStatisticData statisticData;
//		statisticData.set_maintype(mainType);
//		statisticData.set_minortype(minorType);
//		statisticData.set_charid(charID);
//		statisticData.set_savetype(eInt64);
//		statisticData.set_behaviorlen(0);
//		statisticData.set_behaviordata("");
//
//		SetData(&statisticData,num);
//		SavePlayerData(player,mainType,minorType,statisticData);
//	}
//	else
//	{
//		SetData(saveData,num);
//	}
//}

//void StatisticMgr::StatisticPlayerGetItems(const Smart_Ptr<Player> &player, DWORD mainType, DWORD minorType, DWORD num, DWORD cost)
//{
//	if(!player)
//	{
//		return;
//	}
//
//	if(player->GetAccountType() != eAccountType_User)
//		return;
//
//	int64 charID = GET_PLAYER_CHARID(player->GetID());
//
//	StatisticInfo::UseStatisticData *saveData = StatisticMgr::GetInstance()->GetPlayerData(player,mainType,minorType);
//	if(!saveData)
//	{
//		StatisticInfo::UseStatisticData statisticData;
//		statisticData.set_maintype(mainType);
//		statisticData.set_minortype(minorType);
//		statisticData.set_charid(charID);
//		statisticData.set_savetype(eIntToInt);
//		statisticData.set_behaviorlen(0);
//		statisticData.set_behaviordata("");
//		statisticData.set_objectid(0);
//
//		SetData(&statisticData,num,cost);
//		SavePlayerData(player,mainType,minorType,statisticData);
//	}
//	else
//	{
//		SetData(saveData,num,cost);
//	}
//}

//void StatisticMgr::StatisticPlayerItemsMap(const Smart_Ptr<Player> &player, DWORD mainType, DWORD minorType, DWORD itemID, DWORD num, bool addFlag)
//{
//	if(!player)
//	{
//		return;
//	}
//
//	if(player->GetAccountType() != eAccountType_User)
//		return;
//
//	int64 charID = GET_PLAYER_CHARID(player->GetID());
//
//	StatisticInfo::UseStatisticData *saveData = StatisticMgr::GetInstance()->GetPlayerData(player,mainType,minorType);
//	if(!saveData)
//	{
//		StatisticInfo::UseStatisticData statisticData;
//		statisticData.set_maintype(mainType);
//		statisticData.set_minortype(minorType);
//		statisticData.set_charid(charID);
//		statisticData.set_savetype(eMap);
//		statisticData.set_behaviorlen(0);
//		statisticData.set_behaviordata("");
//
//		SetData(&statisticData,itemID,num);
//		SavePlayerData(player,mainType,minorType,statisticData);
//	}
//	else
//	{
//		SetData(saveData,itemID,num,"",addFlag);
//	}
//}
//
//void StatisticMgr::SendDataUpdate(void * obj)
//{
//	StatisticMgr * owner = StatisticMgr::GetInstance();
//	if(owner)
//	{
//		owner->SendStatisticData();
//	}
//}
//
//void StatisticMgr::SendStatisticData()
//{
//	int totalByteSize = 0;
//
//	int localServerID = ServerConHandler::GetInstance()->GetServerId();
//
//	std::map<int,StatisticInfo::StatisticTotalInfo> infolist; //发送给统计服的数据
//	//发送给同服组的管理服的数据
//	StatisticInfo::StatisticTotalInfo localStatisticInfo;
//	infolist[localServerID] = localStatisticInfo;
//	//按天统计的数据
//	StatisticMap::iterator itDaily = m_DailyStatistic.begin();
//	for(;itDaily != m_DailyStatistic.end();itDaily++)
//	{
//		StatisticInfo::UseStatisticData *datainfo = infolist[localServerID].add_infolist();
//		datainfo->CopyFrom(itDaily->second);
//
//		totalByteSize = infolist[localServerID].ByteSize();
//
//		if(NORMAL_MSG_SEND_SIZE <= totalByteSize)
//		{
//			StatisticSendMgr::GetInstance()->sendPlayerStatistic(infolist);
//			infolist.clear();
//			totalByteSize = 0;
//		}
//	}
//
//	std::map<int64, StatisticMap>::iterator itplayer = m_playerStatistic.begin();
//	for(; itplayer != m_playerStatistic.end(); itplayer++)
//	{
//		int serverID = GET_SERVER_CHARID(itplayer->first);
//		std::map<int,StatisticInfo::StatisticTotalInfo>::iterator statisticIt = infolist.find(serverID);
//		if(statisticIt == infolist.end())
//		{
//			StatisticInfo::StatisticTotalInfo statisticInfo;
//			infolist[serverID] = statisticInfo;
//		}
//
//		StatisticMap::iterator itInner = itplayer->second.begin();
//		for(; itInner != itplayer->second.end(); itInner++)
//		{
//			StatisticInfo::UseStatisticData *tmpdata = infolist[serverID].add_infolist();
//			tmpdata->CopyFrom(itInner->second);
//
//			totalByteSize += tmpdata->ByteSize();
//
//			if(NORMAL_MSG_SEND_SIZE <= totalByteSize)
//			{
//				StatisticSendMgr::GetInstance()->sendPlayerStatistic(infolist);
//				infolist.clear();
//				totalByteSize = 0;
//			}
//		}
//	}
//
//	//行为数据有可能是其他服务器的玩家
//	std::map<int64, vector<StatisticInfo::UseStatisticData> >::iterator behaviorIt = m_playerBehavior.begin();
//	for(; behaviorIt != m_playerBehavior.end(); behaviorIt++)
//	{
//		int serverID = GET_SERVER_CHARID(behaviorIt->first);
//		std::map<int,StatisticInfo::StatisticTotalInfo>::iterator statisticIt = infolist.find(serverID);
//		if(statisticIt == infolist.end())
//		{
//			StatisticInfo::StatisticTotalInfo statisticInfo;
//			infolist[serverID] = statisticInfo;
//		}
//
//		vector<StatisticInfo::UseStatisticData>::iterator itInner = behaviorIt->second.begin();
//		for(; itInner != behaviorIt->second.end(); itInner++)
//		{
//			StatisticInfo::UseStatisticData *tmpdata = infolist[serverID].add_infolist();
//			tmpdata->CopyFrom(*itInner);
//
//			totalByteSize += tmpdata->ByteSize();
//
//			if(NORMAL_MSG_SEND_SIZE <= totalByteSize)
//			{
//				StatisticSendMgr::GetInstance()->sendPlayerStatistic(infolist);
//				infolist.clear();
//				totalByteSize = 0;
//			}
//		}
//	}
//
//	if(totalByteSize > 0)
//	{
//		StatisticSendMgr::GetInstance()->sendPlayerStatistic(infolist);
//		infolist.clear();
//	}
//
//	StatisticMap().swap(m_DailyStatistic);
//	PlayerStatisticMap().swap(m_playerStatistic);
//	PlayerBehaviorMap().swap(m_playerBehavior);
//}
//
//bool StatisticMgr::StatisticDailyTimes(const EventArgs & e)
//{
//	//统计成功完成副本次数
//	int instanceType = 0;
//	PlayerInstancePassEventArgs* data = (PlayerInstancePassEventArgs*)(&e);
//	if(!data)
//	{
//		return true;
//	}
//
//	Smart_Ptr<Player>  player = data->m_obj;
//	if(!player || eAccountType_User != player->GetAccountType())
//	{
//		return true;
//	}
//
//	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(data->m_mapId);
//	if (!entry)
//	{
//		LOG_DEBUG(FILEINFO,"StatisticMgr::StatisticDailyTimes: intanceId is invalid %d.",data->m_mapId);
//		return true;
//	}
//
//	//剧情副本
//	if(entry->m_nIsSpecial == 0)
//	{
//		instanceType = eStaMinor_Instance_StoryTimes;
//	}
//	else	if(entry->m_eType == ePersonalInstance)	//单人副本
//	{
//		instanceType = eStaMinor_Instance_PersonTimes;
//	}
//	else if(entry->m_eType == eGroupInstance)
//	{
//		instanceType = eStaMinor_Instance_TeamTimes;
//	}
//	else if(entry->m_nIsSpecial == 2)
//	{
//		instanceType = eStaMinor_Instance_ChallTimes;
//	}
//
//	StatisticDailyTimes(eStatic_Instance,instanceType);
//
//	return true;
//}
//
//bool StatisticMgr::StatisticPlayerLvUpgrade(const EventArgs& e)
//{
//	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
//	if(!data)
//	{
//		return true;
//	}
//
//	Smart_Ptr<Player>  player = data->m_obj;
//	if(player)
//	{
////		StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(player, eStatic_Charactor, eStaMinor_Charactor_LevelUp, 1);
//	}
//	return true;
//}
//
//bool StatisticMgr::HandlePlayerBeKilledEvent(const EventArgs & e)
//{
//	PlayerKilledEventArgs *data = (PlayerKilledEventArgs *)(&e);
//
//	if(!data)
//	{
//		return true;
//	}
//
//	if(data->m_dead->GetType() != ePlayer || data->m_killer->GetType() != ePlayer)
//		return true;
//
//	Smart_Ptr<Player>  killer = data->m_killer;
//	Smart_Ptr<Player>  dead = data->m_dead;
//
//	if(dead && killer)
//	{
//		DWORD clanID = dead->getAttManage()->getValue<DWORD>(eCharFaction);
//		if(clanID <= 0)
//			return true;
//
//		ServerReturn::ServerRetInt killerID;
//		killerID.set_ret(killer->GetID());
//
////		StatisticPlayerBehavior(dead->GetID(),eStatic_Clan,eStaMinor_Clan_Killed,&killerID,clanID);
//	}
//
//	return true;
//}
//
//bool StatisticMgr::HandleCurrencyChangeEvent(const EventArgs & e)
//{
//	StatisticCurrencyChangeArgs *data = (StatisticCurrencyChangeArgs *)(&e);
//
//	if(!data)
//	{
//		return true;
//	}
//
//	StatisticPlayerBehavior(data->m_charID,data->m_mainType,data->m_minorType,data->m_pContent,data->m_objectID);
//
//	return true;
//}
//
//bool StatisticMgr::HandleGoldenChangeEvent(const EventArgs & e)
//{
//	StatisticGoldenChangeArgs *data = (StatisticGoldenChangeArgs *)(&e);
//
//	if(!data)
//	{
//		return true;
//	}
//
//	Smart_Ptr<Player>  player = data->m_obj;
//	if(player)
//	{
//		StatisticPlayerGetItems(player,data->m_mainType,data->m_minorType,data->m_num,data->m_cost);
//	}
//
//	return true;
//}
//
////  统计玩家登录信息
//void StatisticMgr::StatisticPlayerLoginInfo(const Smart_Ptr<Player>& player, DWORD mainType, DWORD minorType)
//{
//	if(!player)
//	{
//		return;
//	}
//
//	if(player->GetAccountType() != eAccountType_User)
//		return;
//
//	int64 charID = GET_PLAYER_CHARID(player->GetID());
//
//	StatisticInfo::UseStatisticData *saveData = StatisticMgr::GetInstance()->GetPlayerData(player,mainType,minorType);
//	if(!saveData)
//	{
//		StatisticInfo::UseStatisticData statisticData;
//		statisticData.set_maintype(mainType);
//		statisticData.set_minortype(minorType);
//		statisticData.set_charid(charID);
//		statisticData.set_savetype(eVecByIntToStr);
//		statisticData.set_behaviorlen(0);
//		statisticData.set_behaviordata("");
//		statisticData.add_openid(player->GetIP());
//
//		SetData(&statisticData,charID,0,player->GetIP(),true,player->Get58PlaySouce());
//		SavePlayerData(player,mainType,minorType,statisticData);
//	}
//	else
//	{
//		SetData(saveData,charID,0,player->GetIP(),true,player->Get58PlaySouce());
//	}
//}
//
//void StatisticMgr::StatisticPlayerBehavior(int64 charID,DWORD mainType, DWORD minorType, google::protobuf::Message* content,int64 objectID)
//{
//	if(charID <= 0)
//		return;
//
//	int64 realCharID = GET_PLAYER_CHARID(charID);
//
//	StatisticInfo::UseStatisticData statisticData;
//
//	char messageStr[NORMAL_MSG_SEND_SIZE] = {0};
//	if(content)
//	{
//		content->SerializePartialToArray(messageStr,content->ByteSize());
//		statisticData.set_behaviorlen(content->ByteSize());
//		//必须用带长度的，否则遇到\0将被截断
//		statisticData.set_behaviordata(messageStr,content->ByteSize());
//	}
//	else
//	{
//		statisticData.set_behaviorlen(0);
//		statisticData.set_behaviordata("");
//	}
//	statisticData.set_maintype(mainType);
//	statisticData.set_minortype(minorType);
//	statisticData.set_charid(realCharID);
//	statisticData.set_savetype(0);
//	statisticData.set_objectid(objectID);
//
//	SavePlayerBehaviorData(charID,mainType,minorType,statisticData);
//}
//
//void StatisticMgr::SetItemCostBefore(StatisticInfo::CostItemInfo* costItemInfo,ECharAttrType charAttrtype, int num, Smart_Ptr<Player>& player)
//{
//	if(!costItemInfo)
//		return;
//
//	int64 itemId = GameParaLoader::GetInstance()->GetExchangeItemIdByAtrrType(charAttrtype);
//	if(0 == itemId)
//		return;
//
//	costItemInfo->set_beforenum(player->getAttManage()->getValue<int>(charAttrtype));
//
//	SetItemCost(costItemInfo,itemId,num);
//}
//
//void StatisticMgr::SetItemCostAfter(StatisticInfo::CostItemInfo* costItemInfo,ECharAttrType charAttrtype, Smart_Ptr<Player>& player)
//{
//	if(!costItemInfo)
//		return;
//
//	costItemInfo->set_afternum(player->getAttManage()->getValue<int>((ECharAttrType)charAttrtype));
//}
//
//void StatisticMgr::SetItemCostBefore(StatisticInfo::CostItemInfo* costItemInfo,ECharAttrType charAttrtype,Smart_Ptr<Player>& player)
//{
//	if(!costItemInfo)
//		return;
//
//	costItemInfo->set_beforenum(player->getAttManage()->getValue<int>(charAttrtype));
//}
//
//void StatisticMgr::SetItemCostAfter(StatisticInfo::CostItemInfo* costItemInfo,ECharAttrType charAttrtype,int num,Smart_Ptr<Player>& player)
//{
//	if(!costItemInfo)
//		return;
//
//	int64 itemId = GameParaLoader::GetInstance()->GetExchangeItemIdByAtrrType(charAttrtype);
//	if(0 == itemId)
//		return;
//
//	SetItemCost(costItemInfo,itemId,num);
//
//	costItemInfo->set_afternum(player->getAttManage()->getValue<int>((ECharAttrType)charAttrtype));
//}
//
//void StatisticMgr::SetItemCost(StatisticInfo::CostItemInfo* costItemInfo,int64 itemID, int itenNum)
//{
//	if(!costItemInfo)
//		return;
//
//	costItemInfo->set_itemid(itemID);
//
//	costItemInfo->set_costnum(itenNum);
//}
//
//void StatisticMgr::SetItemCost(StatisticInfo::CostItemInfo * costItemInfo,ECharAttrType charAttrtype,int itenNum)
//{
//	int64 itemId = GameParaLoader::GetInstance()->GetExchangeItemIdByAtrrType(charAttrtype);
//	if(0 == itemId)
//		return;
//
//	SetItemCost(costItemInfo,itemId,itenNum);
//}
//
//void StatisticMgr::AddItemGotInfo(StatisticInfo::GotItemInfo* gotItemInfo,int64 itemID, int itenNum)
//{
//	if(!gotItemInfo)
//		return;
//
//	gotItemInfo->set_itemid(itemID);
//
//	gotItemInfo->set_num(itenNum);
//}
//
//void StatisticMgr::AddItemGotInfo(StatisticInfo::GotItemInfo* gotItemInfo,ECharAttrType charAttrtype, int itenNum)
//{
//	if(!gotItemInfo)
//		return;
//
//	int64 itemId = GameParaLoader::GetInstance()->GetExchangeItemIdByAtrrType(charAttrtype);
//	if(0 == itemId)
//		return;
//
//	AddItemGotInfo(gotItemInfo,itemId,itenNum);
//}
//
//void StatisticMgr::SetStatisticItemInfo(
//		const CharLogin::ClientItemInfo& clientItemInfo,
//		StatisticInfo::ClientItemInfo* StaticClientItemInfo)
//{
//	if(!StaticClientItemInfo)
//		return;
//
//	StaticClientItemInfo->set_flag(clientItemInfo.flag());
//	StaticClientItemInfo->set_id(clientItemInfo.id());
////	StaticClientItemInfo->set_num(clientItemInfo.num());
////	StaticClientItemInfo->set_time(clientItemInfo.time());
//	for(int i = 0;i < clientItemInfo.newattr_size();++i)
//	{
//		StaticClientItemInfo->add_newattr(clientItemInfo.newattr(i));
//	}

//	for(int i = 0;i < clientItemInfo.stoneid_size();++i)
//	{
//		StaticClientItemInfo->add_stoneid(clientItemInfo.stoneid(i));
//	}
//
//	for(int i = 0;i < clientItemInfo.inlaypos_size();++i)
//	{
//		StaticClientItemInfo->add_inlaypos(clientItemInfo.inlaypos(i));
//	}

//	for(int i = 0;i < clientItemInfo.refineattrlv_size();++i)
//	{
//		StaticClientItemInfo->add_refineattrlv(clientItemInfo.refineattrlv(i));
//	}
//}

BEHAVMONI_BEGIN_DEFINE(StatisticMgr)
{
	Behave::Data::Require arg;

	arg.money = money;
	arg.golden = golden;
	arg.force = force;
	arg.bindGolden = bindGolden;

	m_monitor.Begin(charID, mainType, minorType, arg);
}


BEHAVMONI_HANDLE_DEFINE(StatisticMgr)
{
	m_monitor.Handle(charID, H, data);
}

BEHAVMONI_END_DEFINE(StatisticMgr)
{
	// 结束监视
	Behave::Data::Require arg;
	arg.money = money;
	arg.golden = golden;
	arg.force = force;
	arg.bindGolden = bindGolden;

	m_monitor.End(charID, mainType, minorType, arg);

	// 提取行为记录
	StatisticInfo::PlayerBehavior record;
	if (0 != m_monitor.Fetch(charID, record))
	{
		return;
	}

	// 记录
	m_behaviorSet[charID].push_back(record);
}

GLOBAL_STATIS_DEFINE(StatisticMgr)
{
	GlobalKey key;
	key.mainT = mainT;
	key.minorT = minorT;
	key.saveT = saveT;

	GlobalSet::const_iterator it = m_global.find(key);
	if (it == m_global.end())
	{
		m_global[key].set_maintype(mainT);
		m_global[key].set_minortype(minorT);
		m_global[key].set_savetype(saveT);
	}

	StatisticInfo::GlobalOtherData* oth = m_global[key].mutable_othdata()->add_list();
	if (NULL != oth)
	{
		char buff[NORMAL_MSG_SEND_SIZE] = {0};
		int size = data->ByteSize();
		data->SerializePartialToArray(buff, size);

		oth->set_len(size);
		oth->set_data(buff);
	}
}

//void StatisticMgr::BehaviorMonitorBegin(const int& mainType, const int& minorType, const Smart_Ptr<Player>& player)
//{
//
//}
//void StatisticMgr::BehaviorMonitorEnd(const int& mainType, const int& minorType, const Smart_Ptr<Player>& player)
//{
//	if (!player)
//	{
//		return;
//	}
//
//
//}
//
//void StatisticMgr::BehaviorHandle(int64 charID, E_Behavior_Handle handle, const ::google::protobuf::Message* data)
//{
//
//}

// 定时发送
void StatisticMgr::SendData(void* obj)
{
	SendStatistData();

}

void StatisticMgr::SendStatistData()
{
	map<int, StatisticInfo::StatisticData> data;

	BehaviorSet::const_iterator behaveIt = m_behaviorSet.begin();
	BehaviorSet::const_iterator behaveEnd = m_behaviorSet.end();
	for (; behaveIt != behaveEnd; ++behaveIt)
	{
		int64 charID = behaveIt->first;
		int svrID = GET_SERVER_CHARID(charID);

		vector<StatisticInfo::PlayerBehavior>::const_iterator playBehaveIt = behaveIt->second.begin();
		vector<StatisticInfo::PlayerBehavior>::const_iterator playBehaveEnd = behaveIt->second.end();
		for (; playBehaveIt != playBehaveEnd; ++playBehaveIt)
		{
			StatisticInfo::BehaviorMonitor& behaveData = *(data[svrID].mutable_behavior());

			StatisticInfo::PlayerBehavior* info = behaveData.add_list();

			info->CopyFrom(*playBehaveIt);

			if (NORMAL_MSG_SEND_SIZE < data[svrID].ByteSize())
			{
				SendStatistData(svrID, data[svrID]);
				data[svrID].Clear();
			}
		}
	}

	 Smart_Ptr<SvrItem> mgrSvr;
	ServerConHandler::GetInstance()->GetGlobalStatistMgrSvr(mgrSvr);

	if (mgrSvr)
	{ // 有管理服的情况
		int svrID = mgrSvr->serverID;
		GlobalSet::const_iterator globalIt = m_global.begin();
		GlobalSet::const_iterator globalEnd = m_global.end();
		for (; globalIt != globalEnd; ++globalIt)
		{
			StatisticInfo::GlobalMonitor& globalData = *(data[svrID].mutable_global());

			StatisticInfo::GlobalStatist* info = globalData.add_list();

			info->CopyFrom(globalIt->second);

			if (NORMAL_MSG_SEND_SIZE < data[svrID].ByteSize())
			{
				SendStatistData(svrID, data[svrID]);
				data[svrID].Clear();
			}
		}
	}

 	map<int, StatisticInfo::StatisticData>::iterator it = data.begin();
	map<int, StatisticInfo::StatisticData>::iterator end = data.end();
	for (; it != end; ++it)
	{
		if (0 < it->second.behavior().list_size() || 0 < it->second.global().list_size())
		{
			SendStatistData(it->first, it->second);
		}
	}

	m_behaviorSet.clear();
	m_global.clear();

}

void StatisticMgr::SendStatistData(const short int& svrID, StatisticInfo::StatisticData& data)
{
	Smart_Ptr<SvrItem> server;
	ServerConHandler::GetInstance()->GetManagerServerItem(svrID, server);
	if (server)
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2MS_STATISTDATA, &data, server.Get(), SimpleMessage);
		Message_Facade::Send(messRet);
	}
	else
	{
		LOG_FATAL(FILEINFO, "can not connect to managerserver, serverid = [%d]", svrID);
	}
}
//
//void BehaviorMoni::Handle(int64 charID, E_Behavior_Handle handle, eSaveType save, ::google::protobuf::Message data)
//{
//	if (0 >= m_monitorData[charID].size())
//	{
//		return;
//	}
//
//	Behavior::HandleData handleData;
//	handleData.type.concept = Behavior::Type::eHandleType;
//	handleData.type.unionType = (DWORD64)handle << 32 | save;
//	handleData.data.oth = data.SerializePartialAsString();
//
//	m_monitorData[charID].push(handleData);
//}
//
//void BehaviorMoni::Begin(int64 charID, int mainType, int minorType, Behavior::Data::Require arg)
//{
//	Begin(charID);
//
//	Behavior::HandleData handle;
//	handle.type.concept = Behavior::Type::eBehaviorType; // 声明以下值是行为类型
//	handle.type.unionType = (DWORD64)mainType << sizeof(DWORD) | minorType; // 主要类型<<32 | 次要类型
//	handle.data.req = arg;
//	m_monitorData[charID].push(handle);
//}
//
//
//void BehaviorMoni::End(int64 charID, int mainType, int minorType, Behavior::Data::Require arg)
//{
//	Behavior::HandleData handle;
//	handle.type.concept = Behavior::Type::eBehaviorType; // 声明以下值是行为类型
//	handle.type.unionType = (DWORD64)mainType << sizeof(DWORD) | minorType; // 主要类型<<32 | 次要类型
//	handle.data.req = arg;
//	m_monitorData[charID].push(handle);
//
//	End(charID);
//}
//
//int BehaviorMoni::Fetch(int64 charID, StatisticInfo::PlayerBehavior& record)
//{
//	if (0 >= m_monitorData[charID].size() ||
//			m_monitorData[charID].top().type.concept != Behavior::Type::eFlagType ||
//			m_monitorData[charID].top().type.half.first != Behavior::Type::Switch::eOff
//			)
//	{
//		return -1;
//	}
//
//	bool eFlag = false, bFlag = false;
//	while (1)
//	{
//		if (0 >= m_monitorData[charID].size())
//		{
//			break;
//		}
//
//		Behavior::HandleData handle = m_monitorData[charID].top();
//		switch (handle.type.concept)
//		{
//		case Behavior::Type::eBehaviorType:
//			{
//				if (eFlag)
//				{
//					eFlag = false;
//					record.set_aftermoney(handle.data.req.money);
//					record.set_aftergolden(handle.data.req.golden);
//					record.set_afterforce(handle.data.req.force);
//				}
//				else if (bFlag)
//				{
//					bFlag = false;
//					record.set_beforemoney(handle.data.req.money);
//					record.set_beforegolden(handle.data.req.golden);
//					record.set_beforeforce(handle.data.req.force);
//				}
//			}
//			break;
//		case Behavior::Type::eFlagType:
//			{
//				if (Behavior::Type::Switch::eOff == m_monitorData.top().type.half.first)
//				{
//					eFlag = true;
//				}
//				else if(Behavior::Type::Switch::eOn == m_monitorData.top().type.half.first)
//				{
//					bFlag = true;
//				}
//			}
//			break;
//		case Behavior::Type::eHandleType:
//			{
//				::google::protobuf::Message content;
//				content.ParseFromString(handle.data.oth);
//
//				char buff[NORMAL_MSG_SEND_SIZE] = {0};
//				int size = 0;
//
//				content.SerializePartialToArray(buff, size);
//				record.mutable_othdata()->set_data(buff, size);
//				record.mutable_othdata()->set_len(size);
//			}
//			break;
//		default:
//			{
//			}
//		}
//
//		m_monitorData[charID].pop();
//	}
//
//
//
//	return 0;
//}
//
//void BehaviorMoni::Begin()
//{
//	Behavior::HandleData handle;
//	handle.type.concept = Behavior::Type::eFlagType; // 声明以下值是标志含义 begin/end
//	handle.type.unionType = (DWORD64)Behavior::Type::Switch::eOn << sizeof(DWORD); // 开始行为监视类型
//	m_monitorData.push(handle);
//}
//void BehaviorMoni::End()
//{
//	Behavior::HandleData handle;
//	handle.type.concept = Behavior::Type::eFlagType; // 声明以下值是标志含义 begin/end
//	handle.type.unionType = (DWORD64)Behavior::Type::Switch::eOff << sizeof(DWORD); // 结束行为监视类型
//	m_monitorData.push(handle);
//}

//
//void BehaviorMoni::Handle(E_Behavior_Handle handle, BehaviorSaveType::eSaveType save, ::google::protobuf::Message& data)
//{
//	if (0 >= m_monitorData.size())
//	{
//		return;
//	}
//
//	Behavior::HandleData handleData;
//	handleData.type.concept = Behavior::Type::eHandleType;
//	handleData.type.unionType = (DWORD64)handle << 32 | save;
//	handleData.data.oth = data.SerializePartialAsString();
//
//	m_monitorData.push(handleData);
//}
//
//void BehaviorMoni::Begin(int mainType, int minorType, Behavior::Data::Require arg)
//{
//	Begin();
//
//	Behavior::HandleData handle;
//	handle.type.concept = Behavior::Type::eBehaviorType; // 声明以下值是行为类型
//	handle.type.unionType = (DWORD64)mainType << sizeof(DWORD) | minorType; // 主要类型<<32 | 次要类型
//	handle.data.req = arg;
//	m_monitorData.push(handle);
//}
//
//
//void BehaviorMoni::End(int mainType, int minorType, Behavior::Data::Require arg)
//{
//	Behavior::HandleData handle;
//	handle.type.concept = Behavior::Type::eBehaviorType; // 声明以下值是行为类型
//	handle.type.unionType = (DWORD64)mainType << sizeof(DWORD) | minorType; // 主要类型<<32 | 次要类型
//	handle.data.req = arg;
//	m_monitorData.push(handle);
//
//	End();
//}
//
//int BehaviorMoni::Fetch(StatisticInfo::PlayerBehavior& record)
//{
//	if (0 >= m_monitorData.size() ||
//			m_monitorData.top().type.concept != Behavior::Type::eFlagType ||
//			m_monitorData.top().type.half.first != Behavior::Type::Switch::eOff
//			)
//	{
//		return -1;
//	}
//
//	bool eFlag = false, bFlag = false;
//	while (1)
//	{
//		if (0 >= m_monitorData.size())
//		{
//			break;
//		}
//
//		Behavior::HandleData handle = m_monitorData.top();
//		switch (handle.type.concept)
//		{
//		case Behavior::Type::eBehaviorType:
//			{
//				if (eFlag)
//				{
//					eFlag = false;
//					record.set_aftermoney(handle.data.req.money);
//					record.set_aftergolden(handle.data.req.golden);
//					record.set_afterforce(handle.data.req.force);
//				}
//				else if (bFlag)
//				{
//					bFlag = false;
//					record.set_beforemoney(handle.data.req.money);
//					record.set_beforegolden(handle.data.req.golden);
//					record.set_beforeforce(handle.data.req.force);
//				}
//			}
//			break;
//		case Behavior::Type::eFlagType:
//			{
//				if (Behavior::Type::Switch::eOff == m_monitorData.top().type.half.first)
//				{
//					eFlag = true;
//				}
//				else if(Behavior::Type::Switch::eOn == m_monitorData.top().type.half.first)
//				{
//					bFlag = true;
//				}
//			}
//			break;
//		case Behavior::Type::eHandleType:
//			{
//				::google::protobuf::Message content;
//				content.ParseFromString(handle.data.oth);
//
//				char buff[NORMAL_MSG_SEND_SIZE] = {0};
//				int size = 0;
//
//				content.SerializePartialToArray(buff, size);
//				record.mutable_othdata()->set_data(buff, size);
//				record.mutable_othdata()->set_len(size);
//			}
//			break;
//		default:
//			{
//			}
//		}
//
//		m_monitorData.pop();
//	}
//
//	return 0;
//}
//
//void BehaviorMoni::Begin()
//{
//	Behavior::HandleData handle;
//	handle.type.concept = Behavior::Type::eFlagType; // 声明以下值是标志含义 begin/end
//	handle.type.unionType = (DWORD64)Behavior::Type::Switch::eOn << sizeof(DWORD); // 开始行为监视类型
//	m_monitorData.push(handle);
//}
//void BehaviorMoni::End()
//{
//	Behavior::HandleData handle;
//	handle.type.concept = Behavior::Type::eFlagType; // 声明以下值是标志含义 begin/end
//	handle.type.unionType = (DWORD64)Behavior::Type::Switch::eOff << sizeof(DWORD); // 结束行为监视类型
//	m_monitorData.push(handle);
//}
