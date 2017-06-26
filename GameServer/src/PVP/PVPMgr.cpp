/*
 * PVPMgr.cpp
 *
 *  Created on: 2015年7月24日
 *      Author: root
 */

#include "PVPMgr.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "../QuickMessage.h"
#include "../EventSet/EventDefine.h"
#include "../ServerEventDefine.h"
#include "stdlib.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "../ServerEventDefine.h"
#include "../Mail/MailMessage.h"
#include "../Map/GameZone.h"
#include "../Map/GameDuplicate.h"
#include "../MapCarbon/PVPCarbon.h"
#include "./FileLoader/InternationalLoader.h"
#include "MessageStruct/PVP/PVP.pb.h"
#include "MessageStruct/CharLogin/Mail.pb.h"
#include "../Map/DuplicateMgr.h"
#include "../Map/GameDuplicate.h"
#include "FileLoader/GameParaLoader.h"
#include "FileLoader/UpgradeLoader.h"


PVPMgr::PVPMgr():m_gatherTime(0),m_expPrizeTime(0),m_mailContentIndex(0),m_bufferID(0),m_beginTime(0)
{
		DEF_MSG_QUICK_REQUEST_REG_FUN(PVPMgr,MSG_REQ_C2GS_PVP_GATHER);

		DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GS_PVP_MAPS);
		DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GS_ADD_ITEM);
		DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GS_PVP_END);
		DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GS_PVP_NOOPERATE);
		
		DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GS2WS_PVP_PLAYER_DATA);

		DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GS_PVP_BOSS_Damage_Rank);

		m_pLua.OpenLua();
		loadConfig();
}

PVPMgr::~PVPMgr()
{
	m_pLua.CloseLua();
}



void PVPMgr::loadConfig()
{
	loadPrize();
	loadmixInfo();
	loadLayerInfo();
	loadLua();
}

void PVPMgr::loadLua()
{
	//经验奖励时间  玩家等级与层次经验奖励
	std::string strFileFullPath;
	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
			return;
	}

	string strConfig = strFileFullPath + "Data/pvp.lua";
	if(m_pLua.LoadFile(strConfig) != 0 || m_pLua.Call() != 0)
	{
			return;
	}

	//奖励时间
	m_pLua.SetLuaFunction("GetExpTime");
	if(m_pLua.Call(0, 1))
	{
			return;
	}
	m_expPrizeTime = m_pLua.GetInt();

	m_pLua.SetLuaFunction("GetMailContentID");
	if(m_pLua.Call(0, 1))
	{
			return;
	}
	m_mailContentIndex = m_pLua.GetInt();

	m_pLua.SetLuaFunction("GetBufferID");
	if(m_pLua.Call(0, 1))
	{
			return;
	}
	m_bufferID = m_pLua.GetInt();

}

int PVPMgr::getZoneExp(int lv,int layer)
{
	UpgradeInfo nowInfo;

	UpgradeLoader::GetInstance()->GetUpgradeByLv(lv, nowInfo);
	if(nowInfo.lv == 0)
	{
		LOG_ERROR(FILEINFO, "get upgrade info error and upgrade failed");
		return 0;
	}

	m_pLua.SetLuaFunction("GetZoneExp");
	m_pLua.SetFloat(nowInfo.factoryExp);
	m_pLua.SetInt(layer);
	if(m_pLua.Call(2,1))
	{
			return 0;
	}
	return m_pLua.GetInt();
}

const PVPLayerInfo* PVPMgr::getLayerInfo(int layer)
{
	std::map<int,PVPLayerInfo>::iterator it = m_layerInfo.find(layer);
	if(it!=m_layerInfo.end())
		return &it->second;
	return NULL;
}

int64 PVPMgr::getMapIDByLayer(int layer)
{
	std::map<int,int64>::iterator it=m_pvpMaplist.find(layer);
	if(it!=m_pvpMaplist.end())
		return it->second;
	return 0;
}



void PVPMgr::loadPrize()
{
	std::string strFileFullPath;
	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
			return;
	}
	string strFile = strFileFullPath + "Data/floorbattlebonus.csv";
	CSVLoader loader;

	if(!loader.OpenFromFile(strFile.c_str()))
	{
			return;
	}

	//id	 类型 	排名下限	排名上限	物品数量1	道具id1	道具数量2	道具id2	道具数量3	  道具id3
	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
			int count = 0;
			loader.GetInt(i, count++);
			int type = loader.GetInt(i, count++);//类型

			PVPSortPrize prize;
			prize.min = loader.GetInt(i, count++);//排名下限
			prize.max = loader.GetInt(i, count++);//排名上限


			string funStrItem = loader.GetString(i, count++, "");
			StrArgPkg funPkgItem("|", funStrItem);
			for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
			{
				StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
				for(unsigned int i=0; i<limitPkg.size(); ++i)
				{
					PropItem item;
					item.id = (atoi(getArg(limitPkg, i++).c_str()));
					item.num = (atoi(getArg(limitPkg, i).c_str()));

					if(item.id>0 && item.num>0)
						prize.m_items.push_back(item);
				}
			}

			//类型意义
			//1是伤害排名奖励 //2是杀人总数排名奖励
			//3是楼层奖励   //4是BOSS最后一击奖励 //5是击杀人数奖励
			switch(type)
			{
			case ePVP_Damage:
			{
					m_damagePrize.push_back(prize);
					break;
			}
			case ePVP_KillSort:
			{
					m_killPrize.push_back(prize);
					break;
			}
			case ePVP_Layer:
			{
					m_layerPrize[prize.min] = prize;
					break;
			}
			case ePVP_LastAttack:
			{
				m_lastAttckPrize.push_back(prize);
				break;
			}
			case ePVP_KillNums:
			{
				m_killNumsPrize.push_back(prize);
				break;
			}
			}
	}

	return;
}

void PVPMgr::loadLayerInfo()
{
		std::string strFileFullPath;
		if(CUtil::GetModulePath(strFileFullPath) != 0)
		{
				return;
		}
		string strFile = strFileFullPath + "Data/floorinformation.csv";
		CSVLoader loader;

		if(!loader.OpenFromFile(strFile.c_str()))
		{
				return;
		}

		//id	楼层	击杀人数	被击杀次数	采集次数
		for(unsigned int i=0; i<loader.GetRowCount(); ++i)
		{
				int count = 0;
				PVPLayerInfo info;
				loader.GetInt(i, count++);
				info.layer = loader.GetInt(i, count++);
				info.min = loader.GetInt(i, count++);
				info.max = loader.GetInt(i, count++);
				info.gather = loader.GetInt(i, count++);
				info.kills = loader.GetInt(i, count++);
				info.bekills = loader.GetInt(i, count++);
				info.mon = loader.GetInt(i, count++);
				info.boss = loader.GetInt(i, count++);

				m_layerInfo[info.layer] = info;
		}
}

void PVPMgr::loadmixInfo()
{
	std::string strFileFullPath;
	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
			return;
	}
	string strFile = strFileFullPath + "Data/floorconfig.csv";
	CSVLoader loader;

	if(!loader.OpenFromFile(strFile.c_str()))
	{
			return;
	}

	//id	单线人数上限	不操作关闭时间	火种采集时间
	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
			int count = 0;
			loader.GetInt(i, count++);
			loader.GetInt(i, count++);
			loader.GetInt(i, count++);
			m_gatherTime = loader.GetInt(i, count++);
			return;
	}
}

void PVPMgr::release()
{
	m_pvpMaplist.clear();
}

void PVPMgr::Handle_Message(Safe_Smart_Ptr<Message>& message)
{
		DEF_SWITCH_TRY_DISPATCH_BEGIN
		DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GS_PVP_MAPS);
		DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GS_ADD_ITEM);
		DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GS_PVP_END);
		DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GS_PVP_NOOPERATE);
		DEF_SWITCH_TRY_DISPATCH_END
}

void PVPMgr::Handle_Request(Safe_Smart_Ptr<Message>& message)
{
		DEF_SWITCH_TRY_DISPATCH_BEGIN
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GS_PVP_BOSS_Damage_Rank);
		DEF_SWITCH_TRY_DISPATCH_END
}

void PVPMgr::Handle_Ack(Safe_Smart_Ptr<Message>& message)
{
		DEF_SWITCH_TRY_DISPATCH_BEGIN
		DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GS2WS_PVP_PLAYER_DATA);
		DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_SIMPLE_DEFINE_FUN(PVPMgr,MSG_SIM_WS2GS_PVP_MAPS)
{
		PvPPackage::PVPMapInfos req;
		int len=0;
		char* con = message->GetBuffer(len);
		req.ParseFromArray(con,len);

		int64 scence=0;
		for(int i=0; i<req.info_size(); ++i)
		{
			const PvPPackage::PVPMap& item = req.info(i);
			m_pvpMaplist[item.layer()] = item.id();
			if(i==0)
			{
					scence = item.id();
			}
		}

		DWORD64 endtime = 0;
		InstanceMapEntry* pInstance = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(scence));
		if(pInstance!=NULL)
		{
			endtime = CUtil::GetNowSecond() + (DWORD64)pInstance->m_nLimitTime;
			setEndTime(endtime);
		}
		else
		{
			endtime = CUtil::GetNowSecond() + 3600000;
			setEndTime(endtime);
		}

		const INT64PLAYERMAP& players = PlayerMessage::GetInstance()->getAllPlayer();
		for (INT64PLAYERMAP::const_iterator it = players.begin(); players.end() != it; ++it)
		{
			if ((bool)it->second && it->second->IsOpenSystem(eStBattleParis))
			{
				it->second->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Activity, eUIHLSmall_Activity_BattlePVP), 1, true);
			}
		}
}

DEF_MSG_SIMPLE_DEFINE_FUN(PVPMgr,MSG_SIM_WS2GS_ADD_ITEM)
{
	PvPPackage::WstoGsAddItem req;
	int len=0;
	char* con = message->GetBuffer(len);
	req.ParseFromArray(con,len);

	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharid(req.charid(), player);

	vector<DWORD> ItemIDList;
	vector<DWORD> NumList;
	vector<WORD>  BindTypeList;
	vector<DWORD> StrengthLv;
	vector<int>   Pos;

	for(int i=0; i < req.list_size(); ++i)
	{
		ItemIDList.push_back(req.list(i).id());
		NumList.push_back(req.list(i).num());
		BindTypeList.push_back(req.list(i).bind());
	}
	StrengthLv.resize(ItemIDList.size(),0);

	//先加背包，背包满就发邮件
	if(player.Get()==NULL || player->GetContainer(ePackageType)->AddItem(ItemIDList,NumList,Pos,BindTypeList,StrengthLv) != 0)
	{
		std::vector<PropItem> props;
		for(int i = 0; i < req.list_size(); ++i)
		{
			PropItem item;
			item.id = req.list(i).id();
			item.num = req.list(i).num();
			item.bind = req.list(i).bind();
			props.push_back( item );
		}

		std::string strContent=InternationalLoader::GetInstance()->GetInterStr(getMailContentIndex());

//		bool isRecord = eAccountType_User == player->GetAccountType() ? true : false;
		MailMessageMgr::GetInstance()->SendMail(req.charid(), props, strContent, strContent);

		//提示客户端邮件信息
		if(player.Get()!=NULL)
		{
			player->SendToPlayerResultCodeMessage(ePackNoSpaceSendMail);
		}

	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(PVPMgr,MSG_SIM_WS2GS_PVP_END)
{
	std::map<int,int64>::iterator it = m_pvpMaplist.begin();
	for(; it != m_pvpMaplist.end(); ++it)
	{
		Smart_Ptr<CGameZone> pZone = CSceneMgr::GetInstance()->GetGameZone(it->second);
		if(pZone.Get()==NULL)
			continue;

		CGameZone* pTmp = pZone.Get();
		((PVPCarbon*)pTmp)->Over();
	}
	release();

	const INT64PLAYERMAP& players = PlayerMessage::GetInstance()->getAllPlayer();
	for (INT64PLAYERMAP::const_iterator it = players.begin(); players.end() != it; ++it)
	{
		if ((bool)it->second && it->second->IsOpenSystem(eStBattleParis))
		{
			it->second->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Activity, eUIHLSmall_Activity_BattlePVP), 1, false);
		}
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(PVPMgr,MSG_SIM_WS2GS_PVP_NOOPERATE)
{
	std::map<int,int64>::iterator it = m_pvpMaplist.begin();
	for(; it != m_pvpMaplist.end(); ++it)
	{
			Smart_Ptr<CGameZone> pZone = CSceneMgr::GetInstance()->GetGameZone(it->second);
			if(pZone.Get()==NULL)
				continue;

			CGameZone* pTmp = pZone.Get();
			((PVPCarbon*)pTmp)->Forbid();
	}

}


DEF_MSG_REQUEST_DEFINE_FUN(PVPMgr, MSG_REQ_WS2GS_PVP_BOSS_Damage_Rank)
{
		PvPPackage::AckWstoGsDamageRank ack;
		int64 mapID = getMapIDByLayer(PVP_MAX_LAYER);
		Smart_Ptr<CGameZone> pZone = CSceneMgr::GetInstance()->GetGameZone(mapID);
		if(pZone.Get()==NULL)
		{
			Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_WS2GS_PVP_BOSS_Damage_Rank, message, &ack);
			Message_Facade::Send(messRet);
			return;
		}

		CGameZone* pTmp = pZone.Get();
		std::vector<BossDamageData> list;
		((PVPCarbon*)pTmp)->getDamageList(list);
		for(size_t i=0; i<list.size(); ++i)
		{
			PvPPackage::BossDamageDataEX* pItem = ack.add_list();
			pItem->set_charid(list[i].charid);
			pItem->set_damage(list[i].damage);
			pItem->set_viplevel(list[i].viplevel);
			pItem->set_name(list[i].name);
		}

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_WS2GS_PVP_BOSS_Damage_Rank, message, &ack);
		Message_Facade::Send(messRet);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(PVPMgr,MSG_REQ_C2GS_PVP_GATHER)
{
		CGameZone *pZone = player->GetGameZone();
		if(pZone==NULL)
				return;

		PvPPackage::ReqGather req;
		req.ParseFromString(content);

		bool isGather = ((PVPCarbon*)(pZone))->Gather(player,req.key());
		PvPPackage::AckGather ret;
		ret.set_ret(eGatherIdError);
		if(isGather)
			ret.set_ret(0);

		REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GS_PVP_GATHER);
}

DEF_MSG_ACK_DEFINE_FUN(PVPMgr, MSG_REQ_GS2WS_PVP_PLAYER_DATA)
{
		int64 charid = static_cast<FirstAct *>(act.Get())->mesDataf;
		Smart_Ptr<Player> player;
		PlayerMessage::GetInstance()->GetPlayerByCharid(charid,player);
		if(player.Get()==NULL)
		{
			return;
		}

		 //请求超时
		if(message->GetErrno() == eReqTimeOut)
		{
			return;
		}

		PvPPackage::AckPlayerPVPData ret;
		int len = 0;
		char * content = message->GetBuffer(len);
		ret.ParseFromArray(content,len);

		//更新内存
		Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
	  if(instance.Get()==NULL)
		  {
		  return;
		  }

		InstanceMapEntry* instanceInfo = instance->GetInstanceScript();
		if(instanceInfo==NULL ||instanceInfo->m_eType!=ePVP)
		{
			return;
		}

		CGameZone *pZone = player->GetGameZone();
		if(pZone == NULL)
		{
			return;
		}

		((PVPCarbon*)(pZone))->initPlayerData(player->GetID(), ret);
		//通知客户端
		PvPPackage::PlayerPvPData toClient;
		((PVPCarbon*)(pZone))->getPlayerData(charid, toClient);
		int endTime = getEndTime()/1000;
		PvPPackage::pvpData* pTime = toClient.add_info();
		pTime->set_id(ePVPLayer_time);
		pTime->set_value(endTime);
		player->SendSynchMessage(&toClient,MSG_SIM_GS2C_PVP_DATA);

}

void PVPMgr::GetBossDamageReward(int64 charid,int damagesort)
{
	vector<PropItem> items;
	for(size_t i=0; i<m_damagePrize.size(); ++i)
	{
		if(damagesort >= m_damagePrize[i].min && damagesort <= m_damagePrize[i].max)
		{
			items.insert(items.end(), m_damagePrize[i].m_items.begin(), m_damagePrize[i].m_items.end());

			break;
		}
	}

	if(items.size() > 0)
	{
		std::string strContent=InternationalLoader::GetInstance()->GetInterStr(getMailContentIndex());
		MailMessageMgr::GetInstance()->SendMail(charid, items, strContent, strContent);
	}
}

void PVPMgr::GetBossLastAttack(int64 charid,int layer)
{
	vector<PropItem> items;
	for(size_t i=0; i < m_lastAttckPrize.size(); ++i)
	{
		if(m_lastAttckPrize[i].min == layer)
		{
			items.insert(items.end(), m_damagePrize[i].m_items.begin(), m_damagePrize[i].m_items.end());
			break;
		}
	}

	if(items.size() > 0)
	{
		std::string strContent=InternationalLoader::GetInstance()->GetInterStr(getMailContentIndex());
		MailMessageMgr::GetInstance()->SendMail(charid, items, strContent, strContent);
	}
}
