/*
 * EscortMgr.cpp
 *
 *  Created on: 2014年7月21日
 *      Author: root
 */

#include "EscortMgr.h"
//#include "../Object/Player/Player.h"
//#include "../Map/GameZone.h"
//#include "../Map/SceneManager.h"
//#include "MessageStruct/Escort/EscortProto.pb.h"
//#include "CounterDefine.h"
//#include <math.h>
//#include "MessageCommonRet.h"
//#include "../QuickMessage.h"
//#include "../Object/FileLoader/NPCLoader.h"
//#include "../Object/Creature.h"
//#include <cstdlib>
//#include "../VIP/PlayerVipPart.h"
//#include "../Map/SceneManager.h"
//#include "ServerEventDefine.h"
//
//EscortMgr::EscortMgr()
//{
//	m_mapPlayersEscort.clear();
//	m_logoutEvent = RegGlobalEventPtr(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, this,&EscortMgr::SaveEscortData);
//}
//
//EscortMgr::~EscortMgr()
//{
//	mapEscort::iterator it = m_mapPlayersEscort.begin();
//	for(;it != m_mapPlayersEscort.end();it++)
//	{
//		delete (it->second);
//	}
//	m_mapPlayersEscort.clear();
//
//	if(m_logoutEvent.IsValid())
//	{
//		m_logoutEvent.SetNull();
//	}
//}
//
//void EscortMgr::SetEscortToDB(Smart_Ptr<Player> player,PlayerInfo::EscortInfo *info)
//{
//	if(!player)
//	{
//		return;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		if(!it->second->isComplete && it->second->leftTimes > 0)
//		{
//			info->set_quality(it->second->quality);
//			info->set_line(it->second->line);
//			info->set_robtimes(it->second->robTimes);
//			info->set_iscomplete(false);
//			int lefttime = it->second->leftTimes;
//			DWORD64 nowsecond = CUtil::GetNowSecond();
//			info->set_savetime(nowsecond);
//			info->set_lefttimes(lefttime);
//			if(it->second->carMon)
//			{
//				it->second->leftSpeedUpTime = it->second->carMon->GetSpeedUpLeftTime();
//				info->set_speedlefttime(it->second->leftSpeedUpTime);
//			}
//			return;
//			//cout<<endl<<player->GetKey()<<" {{{{{{{{{{{{{{{{{{save Escort Left time is "<<it->second->leftTimes<<"   "<<CUtil::GetNowSecond()<<endl;
//		}
//	}
//
//	info->set_quality(0);
//	info->set_line(0);
//	info->set_robtimes(0);
//	info->set_iscomplete(false);
//	info->set_savetime(CUtil::GetNowSecond());
//	info->set_lefttimes(0);
//	info->set_speedlefttime(0);
//}
//
//void EscortMgr::InitEscort(Smart_Ptr<Player> player, PlayerInfo::EscortInfo &info)
//{
//	if(!player)
//	{
//		return;
//	}
//
//	int nowLeftTime = 0;
//
//	DWORD64 savet = info.savetime();
//	nowLeftTime = CUtil::GetNowSecond() - savet;
//	int leftt = info.lefttimes();
//	if(nowLeftTime > 0 && leftt > 0)
//	{
//		int diff =leftt - nowLeftTime;
//		if(diff >= 0)
//		{
//			info.set_lefttimes(diff);
//		}
//		else
//		{
//			info.set_lefttimes(0);
//		}
//	}
//
//	sEscortData *data = new sEscortData;
//	if(info.lefttimes() > 0 && info.quality() >= 0 && info.line() > 0)
//	{
//		if(data)
//		{
//			data->quality = (eCarType)info.quality(); //宝箱默认青铜
//			data->line = info.line();			//默认先选择线路1；
//			data->carMon = NULL;
//			data->robTimes = info.robtimes();
//			data->isComplete = false;
//			data->leftTimes = info.lefttimes();
//			data->leftSpeedUpTime = info.speedlefttime();
//
//			m_mapPlayersEscort[player->GetID()] = data;
//		}
//
//	}
//	else
//	{
//		data->quality = eEscortBronze; //宝箱默认青铜
//		data->line = 1;			//默认先选择线路0；
//		data->carMon = NULL;
//		data->robTimes = 0;
//		data->isComplete = false;
//		m_mapPlayersEscort[player->GetID()] = data;
//	}
//
//}
//
//int EscortMgr::GenenateQuality(Smart_Ptr<Player> &player, DWORD escortId, DWORD &quality)
//{
//	if(!player)
//	{
//		return eNull;
//	}
//
//	sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(escortId);
//	int rate = ESCORTRANDOMRATE;
//
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(entry && (it != m_mapPlayersEscort.end()))
//	{
//		//
//		int freshtimes = player->GetCounterService()->GetNumById(ESCORT_FREETIMES_COUNTER);
//		int vipTimes = 0;
//		if(player->GetVipPart())
//		{
//			vipTimes = player->GetVipPart()->GetVipTimes(eVipEscort);
//		}
//
//		if(freshtimes >= (entry->m_nFreeTimes + vipTimes))
//		{
//			int price = entry->m_nFreshCost;
//
//			uint hadMoney =  player->GetTypeValue(eCharGolden);
//			if (hadMoney < price)
//			{
//			    return eNotEnoughGold;
//			}
//			vector<int> vecType;
//			int leftgold = player->GetGolden() - price;
//			player->SetGolden(leftgold);
//			vecType.push_back(eCharGolden);
//			player->SynCharAttribute(vecType);
//		}
//
//		//srand(time(NULL));		//以时间做种子
//		int a=rand()%rate;		//rate以内的随机数
//
//		if((a > entry->m_nQuality1Probility[0]) && (a <= entry->m_nQuality1Probility[1]))
//		{
//			it->second->quality = eEscortBronze;
//		}
//		else if((a > entry->m_nQuality2Probility[0]) && (a <= entry->m_nQuality2Probility[1]))
//		{
//			it->second->quality = eEscortSilver;
//		}
//		else if((a > entry->m_nQuality3Probility[0]) && (a <= entry->m_nQuality3Probility[1]))
//		{
//			it->second->quality = eEscortGold;
//		}
//		else if((a > entry->m_nQuality4Probility[0]) && (a <= entry->m_nQuality4Probility[1]))
//		{
//			it->second->quality = eEscortPlatinum;
//		}
//		else if((a > entry->m_nQuality5Probility[0]) && (a <= entry->m_nQuality5Probility[1]))
//		{
//			it->second->quality = eEscortDiamond;
//		}
//
//		player->GetCounterService()->IncreasePlayerCounter(ESCORT_FREETIMES_COUNTER,1);
//		quality =  it->second->quality;
//		GenenateEscortArgs e(player->GetMyself(),quality);
//		player->FireEvent(PLAYER_GENENATE_ESCORT,e);
//	}
//
//	return 0;
//}
//
//sEscortData *EscortMgr::GetPlayerEscortData(int64 id)
//{
//	mapEscort::iterator it = m_mapPlayersEscort.find(id);
//	if(it != m_mapPlayersEscort.end())
//	{
//		return it->second;
//	}
//	else
//	{
//		sEscortData *data = new sEscortData;
//		if(data)
//		{
//			data->quality = eEscortBronze; //宝箱默认青铜
//			data->line = 1;			//默认先选择线路1；
//			data->carMon = NULL;
//			data->robTimes = 0;
//			data->isComplete = false;
//
//			m_mapPlayersEscort[id] = data;
//		}
//
//		return data;
//	}
//}
//
//int EscortMgr::SetEscortQuality(Smart_Ptr<Player> &player,eCarType quality)
//{
//	if(!player)
//	{
//		return eNull;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(it->second->line);
//		if(entry)
//		{
//			int price = entry->m_nQualityCost[quality];
//
//			uint hadMoney =  player->GetTypeValue(eCharGolden);
//			if (hadMoney < price)
//			{
//			    return eNotEnoughGold;
//			}
//
//			int dotimes = player->GetCounterService()->GetNumById(ESCORT_DAY_COUNTER);
//			if(dotimes >= entry->m_nDayTimes)
//			{
//				return eEscortTimesOver;
//			}
//			vector<int> vecType;
//			int leftgold = player->GetGolden() - price;
//			player->SetGolden(leftgold);
//			vecType.push_back(eCharGolden);
//			player->SynCharAttribute(vecType);
//
//			it->second->quality = quality;
//		}
//	}
//	return 0;
//}
//
//void EscortMgr::SetEscortMonster(int64 id,Smart_Ptr<Monster> mon)
//{
//	mapEscort::iterator it = m_mapPlayersEscort.find(id);
//	if(it != m_mapPlayersEscort.end())
//	{
//		it->second->carMon = mon;
//	}
//}
//
//void EscortMgr::AddRobTimes(Smart_Ptr<Player> player)
//{
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		it->second->robTimes++;
//
//		if(it->second->robTimes >=5)
//		{
//			it->second->isComplete = false;
//			EndEscort(player);
//		}
//	}
//}
//
////开始押送
//int EscortMgr::StartEscort(Smart_Ptr<Player> &player, DWORD settime, bool needIncrease)
//{
//	if(!player)
//	{
//		return eNull;
//	}
//
//   Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(player->GetMapID());
//   if(!zone)
//	 {
//	   return eRetZoneNotExist;
//	 }
//
//	if(player->IsInCreatureState(ERS_Escort))
//	{
//	   return eIsEscorting;
//	}
//
//	//根据玩家的宝箱品质生成运镖车
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		//现在没有线路选择，暂时只有1这条线路
//		//it->second->line = 1;
//
//		sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(it->second->line);
//		if(entry)
//		{
//			//切换地图时，不需要增加次数
//			int dotimes = player->GetCounterService()->GetNumById(ESCORT_DAY_COUNTER) + (int)needIncrease;
//			if(dotimes > entry->m_nDayTimes)
//			{
//				return eEscortTimesOver;
//			}
//
//			int index = (it->second->quality);
//			//srand(time(NULL));		//以时间做种子
//			int xpos=rand()%3;		//
//			int ypos=rand()%3;		//
//			Smart_Ptr<Monster> mon;
//			mon = zone->CreateMonsterDymic(entry->m_nCarType[index],player->GetXPos() + xpos, player->GetYPos() + ypos,player);
//			if(mon)
//			{
////				printf("\n Escort Create EscortMonster,Monster ID is %d \n",mon->GetKey());
//				mon->SetEscortID(it->second->line);
//				SetEscortMonster(player->GetID(),mon);
//				if(it->second->leftSpeedUpTime > 0)
//				{
//					mon->SpeedUpEscortCarTimer(it->second->leftSpeedUpTime,entry->m_nSpeedUpRate);
//				}
//			}
//			else
//			{
//				return eNull;
//			}
//
//			if(settime > 0)
//			{
//				player->StartEscortTimer(settime);
//				it->second->leftTimes = settime;
//			}
//			else
//			{
//				player->StartEscortTimer(entry->m_nLimitTime);
//				it->second->leftTimes = entry->m_nLimitTime;
//			}
//		}
//	}
//	if(player->GetCounterService() && needIncrease)
//	{
//		player->GetCounterService()->IncreasePlayerCounter(ESCORT_DAY_COUNTER,1);
//		//-------押镖次数达到上限的时候   同步 活动图标 显示信息-----------//
//		DWORD haveTime = GetEscortLeftTime(player);
//		if(haveTime == 0)
//		{
//			player->SynchActiveHighLight();
//		}
//	}
//	//启动玩家运镖的定时器
//	//
//	player->SetCreatureState(ERS_Escort);
//
//	return 0;
//}
//
////押送完成
//void EscortMgr::EndEscort(Smart_Ptr<Player> &player)
//{
//	if(!player)
//	{
//		return;
//	}
//
//	//删除押送数据
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		//
//		if(it->second->isComplete)
//		{
//			//获得奖励
//			sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(it->second->line);
//			if(entry)
//			{
//				//float exp = entry->m_nQualityExp[it->second->quality];
//				int lv = player->GetLv();
//				float exp = (pow(lv * 1.0,(entry->m_expRate1 /10.0)) + lv * entry->m_expRate2 + entry->m_expRate3) * entry->m_nQualityExp[it->second->quality];
//				exp = exp / 100;
//				exp /= 10000;
//				exp = (int)(exp + 0.5);
//				exp *= 10000;
//				//扣除被抢经验
//				if(it->second->robTimes > 0)
//				{
//					exp = (it->second->robTimes * 10 / 100.0) * exp;
//				}
//				//判断是否为双倍经验时间
//
//				player->AddExp(exp);
//
//				it->second->leftTimes = 0;
//				if(it->second->carMon)
//				{
//					it->second->carMon->EndSpeedUpEscortCarTimer(entry->m_nSpeedUpRate);
//					it->second->carMon->SetDead();
////					printf("\n  1111111 it->second->carMon Ref[%d] \n",*(it->second->carMon.GetRef()));
//					Smart_Ptr<Monster> temp;
//					it->second->carMon = temp;
//
//				}
//			}
//			//发送成功消息
//			EscortProto::RtImediaComplete toClient;
//			toClient.set_error(0);
//			QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(),&toClient,MSG_REQ_C2GM_ESCORTIMEDCOMPLETE);
//		}
//		else
//		{
//			sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(it->second->line);
//			if(entry)
//			{
//				it->second->leftTimes = 0;
//				if(it->second->carMon)
//				{
//					it->second->carMon->SetForeverDead(true);
//					it->second->carMon->EndSpeedUpEscortCarTimer(entry->m_nSpeedUpRate);
//					it->second->carMon->SetDead();
//
////					printf("\n  222222 it->second->carMon Ref[%d] \n",*(it->second->carMon.GetRef()));
//
//					Smart_Ptr<Monster> temp;
//					it->second->carMon = temp;
//				}
//			}
//			//发送失败消息
//			EscortProto::RtImediaComplete toClient;
//			toClient.set_error(eEscortFailed);
//			QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(),&toClient,MSG_REQ_C2GM_ESCORTIMEDCOMPLETE);
//		}
//
//		player->SetDataFlag(eEscortInfo);
//		player->UnsetCreatureState(ERS_Escort);
//		if(it->second)
//		{
//			delete it->second;
//		}
//		m_mapPlayersEscort.erase(it);
//	}
//}
//
//bool EscortMgr::TaskIsComplete(Smart_Ptr<Player> player)
//{
//	if(!player)
//	{
//		return false;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		return ((it->second->isComplete == true) && (it->second->leftTimes > 0));
//	}
//
//	return false;
//}
//
//int EscortMgr::GetLeftTime(Smart_Ptr<Player> player)
//{
//	if(!player)
//	{
//		return false;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		return (it->second->leftTimes);
//	}
//
//	return false;
//}
//
//int EscortMgr::GetEscortLine(Smart_Ptr<Player> player)
//{
//	if(!player)
//	{
//		return false;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		return (it->second->line);
//	}
//
//	return false;
//}
//
////判断是否完成
//bool EscortMgr::IsComplete(Smart_Ptr<Player> player)
//{
//	if(!player)
//	{
//		return false;
//	}
//	//是否到达终点NPC处
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(it->second->line);
//		if(entry)
//		{
//			NpcInfo info = NPCInfo::GetInstance()->GetNpcInfo(entry->m_nEndNpcID);
//
//			if((info.mapid == GET_MAP_ID(player->GetMapID())) && (info.mapid == GET_MAP_ID(it->second->carMon->GetMapID())))
//			{
//				int playerres = NPCInfo::GetInstance()->ExamineDistance(entry->m_nEndNpcID, player->GetXPos(), player->GetYPos());
//				int monres = NPCInfo::GetInstance()->ExamineDistance(entry->m_nEndNpcID, it->second->carMon->GetXPos(), it->second->carMon->GetYPos());
//				if(!playerres && !monres)
//				{
//					SetEscortTaskComplete(player,true);
//					return true;
//				}
//			}
//		}
//	}
//
//	return false;
//}
//
//bool EscortMgr::ImediaComplete(Smart_Ptr<Player> &player)
//{
//	if(!player)
//	{
//		return eNull;
//	}
//
//	if(!player->IsInCreatureState(ERS_Escort))
//	{
//		return eNotInEscorting;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		//是否有足够的元宝
//		sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(it->second->line);
//
//		if(!entry)
//		{
//			return eNull;
//		}
//
//		if(!(it->second->carMon))
//		{
//			return eNull;
//		}
//		//
//		int price = entry->m_nImediaComplete;
//
//		uint hadMoney =  player->GetTypeValue(eCharGolden);
//		if (hadMoney < price)
//		{
//		    return eNotEnoughGold;
//		}
//		vector<int> vecType;
//		int leftgold = player->GetGolden() - price;
//		player->SetGolden(leftgold);
//		vecType.push_back(eCharGolden);
//		player->SynCharAttribute(vecType);
//
//		it->second->isComplete = true;
//		player->UnsetCreatureState(ERS_Escort);
//		//删除定时器
//		player->DeleteEscortTimer(true,true);
//		//EndEscort(player);
//		return 0;
//	}
//
//	return eNotInEscorting;
//}
//
//bool EscortMgr::SpeedUpEscort(Smart_Ptr<Player> &player)
//{
//	if(!player)
//	{
//		return eNull;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end() && player->GetCounterService())
//	{
//		//处于加速状态，退出
//		Smart_Ptr<Monster> mon = it->second->carMon;
//		if(!mon)
//		{
//			return eNull;
//		}
//
//		if(mon->GetSpeedUpTimer())
//		{
//			return eEscortIsSpeedingUP;
//		}
//
//		//是否有足够的元宝
//		sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(it->second->line);
//		if(!entry)
//		{
//			return eNull;
//		}
//
//		int speedtimes = player->GetCounterService()->GetNumById(ESCORT_SPEEDUP_COUNTER);
//		//
//		int price = entry->m_nSpeedUpCost * pow(2.0,speedtimes);
//
//		uint hadMoney =  player->GetTypeValue(eCharGolden);
//		if (hadMoney < price)
//		{
//		    return eNotEnoughGold;
//		}
//
//		vector<int> vecType;
//		int leftgold = player->GetGolden() - price;
//		player->SetGolden(leftgold);
//		vecType.push_back(eCharGolden);
//		player->SynCharAttribute(vecType);
//
//		//启动加速定时器
//		if(it->second->carMon)
//		{
//			it->second->carMon->SpeedUpEscortCarTimer(entry->m_nSpeedUpTime,entry->m_nSpeedUpRate);
//		}
//
//		player->GetCounterService()->IncreasePlayerCounter(ESCORT_SPEEDUP_COUNTER,1);
//
//		return 0;
//	}
//
//	return eNotInEscorting;
//}
//
//void EscortMgr::AbandonEscort(Smart_Ptr<Player> &player)
//{
//	if(!player)
//	{
//		return;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end() && player->GetCounterService())
//	{
//		it->second->leftTimes = 0;
//		if(it->second->carMon && it->second->carMon->GetSpeedUpTimer())
//		{
//			it->second->carMon->DeleteSpeedUpTimer();
//		}
//	}
//
//	//删除定时器
//	player->DeleteEscortTimer(true,false);
//
//
//}
//
//void EscortMgr::SetEscortTaskComplete(Smart_Ptr<Player> player, bool complete)
//{
//	if(!player)
//	{
//		return;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		it->second->isComplete = complete;
//		//删除定时器
//		//player->DeleteEscortTimer(true,false);
//		EndEscort(player);
//	}
//}
//
//bool EscortMgr::UpdateEscortLeftTime(Smart_Ptr<Player> &player,DWORD tick)
//{
//	if(!player)
//	{
//		return false;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		it->second->leftTimes -= tick;
//
//
//		if(it->second->leftTimes > 0)
//		{
//			return true;
//		}
//	}
//
//	return false;
//}
//
//Smart_Ptr<Monster> EscortMgr::GetEscortMonster(Smart_Ptr<Player> player)
//{
//	if(!player)
//	{
//		return NULL;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		return it->second->carMon;
//	}
//
//	return NULL;
//}
//
//void EscortMgr::UpdateEscortInfo(Smart_Ptr<Player> player)
//{
//	if(!player)
//	{
//		return;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		//发送状态信息
//		EscortProto::EscortStateInfo stateInfo;
//		stateInfo.set_iscomplete(it->second->isComplete);
//		stateInfo.set_line(it->second->line);
//		stateInfo.set_lefttimes(it->second->leftTimes);
//		stateInfo.set_robtimes(it->second->robTimes);
//		stateInfo.set_quality(it->second->quality);
//		QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(),&stateInfo,MSG_SIM_GM2C_UPDATEESCORTINFO);
//	}
//}
//
//void EscortMgr::SendRobInfo(Smart_Ptr<Player> beRober, Smart_Ptr<Player> rober)
//{
//	if(!beRober || !rober)
//	{
//		return;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(beRober->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		EscortProto::RobEscortInfo info;
//		//发给被劫玩家 ,1代表被劫信息
//		info.set_robtype(1);
//		info.set_name(rober->GetName());
//		QuickMessage::GetInstance()->AddSendMessage(beRober->GetID(),beRober->GetChannelID(),&info,MSG_SIM_GM2C_ROBESCORTINFO);
//
//		//发给抢劫玩家 ,1代表被劫信息
//		info.set_robtype(0);
//		info.set_name(beRober->GetName());
//		info.set_line(it->second->line);
//		info.set_quality(it->second->quality);
//		QuickMessage::GetInstance()->AddSendMessage(rober->GetID(),rober->GetChannelID(),&info,MSG_SIM_GM2C_ROBESCORTINFO);
//	}
//}
//
//int EscortMgr::GetEscortRobTimes(Smart_Ptr<Player> player)
//{
//	if(!player)
//	{
//		return 10;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		return it->second->robTimes;
//	}
//
//	//现在规定大于等于5次即失败，查找不到返回一个大于5的数
//	return 10;
//}
//
//void EscortMgr::ClearPlayerData(Smart_Ptr<Player> &player)
//{
//	if(!player)
//	{
//		return;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		delete (it->second);
//		m_mapPlayersEscort.erase(it);
//	}
//}
//
//void EscortMgr::UpdateSpeedUpTime(Smart_Ptr<Player> &player, int value)
//{
//	if(!player)
//	{
//		return;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		it->second->leftSpeedUpTime = value;
//	}
//}
//
//
//
//
//DWORD EscortMgr::GetEscortLeftTime(Smart_Ptr<Player> &player)
//{
//	if(!player)
//	{
//		return 0;
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(player->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(it->second->line);
//		if(entry)
//		{
//
//			int dotimes = player->GetCounterService()->GetNumById(ESCORT_DAY_COUNTER);
//			if(entry->m_nDayTimes >= dotimes)
//			{
//				return (entry->m_nDayTimes-dotimes);
//			}
//			else
//			{
//				return 0;
//			}
//		}
//	}
//	return 0;
//}
//
//bool EscortMgr::SaveEscortData(const EventArgs & e)
//{
//	PlayerRemoveMemberEventArgs *pArgs = (PlayerRemoveMemberEventArgs *) (&e);
//
//	if(!pArgs)
//	{
//		return true;
//	}
//
//	if(!pArgs->m_pObj)
//	{
//		return true;
//	}
//
//	pArgs->m_pObj->DeleteEscortTimer(false, false);
//	Smart_Ptr<Monster> monobj = sEscortMgr->GetEscortMonster(pArgs->m_pObj);
//	if(monobj)
//	{
//		//将镖车的位置保存为玩家的位置
//		pArgs->m_pObj->SetXPos(monobj->GetXPos());
//		pArgs->m_pObj->SetYPos(monobj->GetYPos());
//		pArgs->m_pObj->SetMapID(monobj->GetMapID());
//		pArgs->m_pObj->SetXNewPos();
//		pArgs->m_pObj->SetYNewPos();
//		pArgs->m_pObj->SetGroup(CSceneMgr::GetInstance()->GetSectorGroup(monobj->GetMapID(), pArgs->m_pObj->GetXPos(), pArgs->m_pObj->GetYPos()));
//		pArgs->m_pObj->SetDataFlag(eBaseInfo);
//		//移除镖车
//		//CSceneMgr::GetInstance()->LeaveScene(monobj,true,true);
//		monobj->SetDead();
//		Smart_Ptr<Monster> temp;
//		SetEscortMonster(pArgs->m_pObj->GetID(),temp);
//	}
//
//	mapEscort::iterator it = m_mapPlayersEscort.find(pArgs->m_pObj->GetID());
//	if(it != m_mapPlayersEscort.end())
//	{
//		if(it->second)
//		{
//			if(it->second->carMon)
//			{
//				Smart_Ptr<Monster> temp;
//				it->second->carMon = temp;
//			}
//			delete (it->second);
//		}
//
//		m_mapPlayersEscort.erase(it);
//	}
//
//	pArgs->m_pObj->SetDataFlag(eEscortInfo);
//	return true;
//}
//
//bool EscortMgr::IsCanEnterEscort(const Smart_Ptr<Player>& player)
//{
//	sRecorceSourceParam *ptr = DailyLoader::GetInstance()->GetRecorceSourceParamPtr(eRecorceSourceParamType_escort);
//	if(!ptr)
//		assert(false);
//	if(!player->GetQuestMgr()->IsThisFunMondel(ptr->needquestID))
//		return false;
//	return true;
//}
//
//int EscortMgr::RecoverResource(const Smart_Ptr<Player>& player, int* num,
//		sRecoverResource& tmpInfo)
//{
//	sEscortEntry * entry = EscortLoader::GetInstance()->GetEscortEntry(No1EscortID);
//	if(!entry)
//		assert(false);
//	int haveNum = entry->m_nDayTimes - *num;
//	if(haveNum <=0)
//		return eEscortTimesOver;
//
//	sRecorceSourceParam *ptr = DailyLoader::GetInstance()->GetRecorceSourceParamPtr(eRecorceSourceParamType_escort);
//	if(!ptr)
//		assert(false);
//	if(!player->GetQuestMgr()->IsThisFunMondel(ptr->needquestID))
//		return eLVNotEnough;
//
//	*num = haveNum;
//	sRecoverResource nowInfo;
//
//	int exp = this->CalAddExp(haveNum, player->GetLv(), entry);
//
//	tmpInfo.exp += exp;
//	nowInfo.exp = exp;
//
//	tmpInfo.needmoney += player->GetDailyManager()->CalNeedMoney(nowInfo, ptr->moneyParm);
//	tmpInfo.needgolde += ptr->goldeCost;
//
//	return 0;
//}
//
//int EscortMgr::CalAddExp(const int& haveNum,const int &lv, const sEscortEntry* entry)
//{
//	if(!entry)
//		assert(false);
//
//	float exp = (pow(lv * 1.0,(entry->m_expRate1 /10.0)) + lv * entry->m_expRate2 + entry->m_expRate3) *
//																entry->m_nQualityExp[0];  /*已最低的来算*/
//	exp = exp / 100;
//	exp /= 10000;
//	exp = (int)(exp + 0.5);
//	exp *= 10000;
//
//	return (int)(exp *haveNum);
//}
//
//
//
//
//
//
