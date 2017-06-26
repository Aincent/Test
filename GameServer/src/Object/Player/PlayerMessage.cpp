/*
 * PlayerMessage.cpp
 *
 *  Created on: 2013��9��29��
 *      Author: helh
 */

#include "PlayerMessage.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/ServerReturn3Int.pb.h"
#include "MessageStruct/ServerReturn4Int.pb.h"
#include "MessageStruct/ServerReturn5Int.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturnIntChar.pb.h"
#include "SvrConfig.h"
#include "Log/Logger.h"
#include "Network/Message_Facade.h"
#include "../../MessageBuild.h"
#include "../../ServerManager.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/CharLogin/CharBaseInfo.pb.h"
#include "MessageStruct/CharLogin/Monitor.pb.h"
#include "MessageStruct/CharLogin/Ranking.pb.h"
#include "MessageStruct/CharBase/CharAttribute.pb.h"
#include "MessageStruct/Quest/QuestMessage.pb.h"
#include "./MessageStruct/CharBattle/Battle.pb.h"
#include "MessageStruct/Map/ChangeMap.pb.h"
#include "MessageStruct/Map/SynchPath.pb.h"
#include "Common.h"
#include "../../MsgCommonClass.h"
#include "../../Map/SceneManager.h"
#include "../Object.h"
#include "../ObjectFactory.h"
#include "../../QuestNew/QuestDefine.h"
#include "MessageCommonRet.h"
#include "../../Map/ConfigLoader/MapConfig.h"
#include "../../Container/ContainerBase.h"
#include "../../Container/ParcelMgr.h"
#include "../../Container/ArticleEquipMgr.h"
#include "../../Container/ArticleStoreHouseMgr.h"
#include "../../Horse/HorseManager.h"
#include "../../Trade/shop.h"
#include "../../Map/ConfigLoader/MapConfig.h"
#include "../../EventSet/EventDefine.h"
#include "../../ServerEventDefine.h"
#include <cmath>

#include "../../MapInstance/InstanceMapLoader.h"
#include "../../Object/FileLoader/MonsterLoader.h"
#include "../../EscortEx/EscortMgr.h"
#include "../../EscortEx/FileLoader/EscortFileLoader.h"
#include "../../DanLu/DanLuMgr.h"
#include "MessageStruct/CharBase/MeditationProto.pb.h"
#include "../../MagicWeapon/MagicMgr.h"
#include "../../Magic/SMagicMgr.h"
#include "../../HeartMagic/HeartMagicMgr.h"

#include "../../CJobLucky/CJobLuckyMgr.h"
#include "../../Map/GameDuplicate.h"
#include "../../Map/DuplicateMgr.h"
#include "../../LucklyTurntable/LucklyTurntableMgr.h"
#include "../../RewardHail/OnlineReward.h"
#include "../../Clan/ClanLoader/ClanLoader.h"
#include "../../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "../../Activity/ActivityMgr.h"
#include "../GirlWar/GirlWarManager.h"
#include "../../MapInstance/CrossSvrCompetitionLoader/CrossSvrCompetitionLoader.h"
#include "../../Group/NewGroupMgr.h"
#include "MessageStruct/StatisticInfo.pb.h"
#include "StatisticBase/StatisticDefine.h"
#include "MessageStruct/Clan/Clan.pb.h"
#include "MessageStruct/Market/Market.pb.h"
#include "./FileLoader/InternationalLoader.h"
#include "../../Mail/MailMessage.h"
#include "../../MapInstance/BeyondBattleMgr.h"
#include "../../Marry/MarryLoader/MarryLoader.h"
#include "FriendManage.h"
#include "../../Country/GsCountryMgr.h"
#include "../../Festival/FestivalActive.h"
#include "../../Country/playerCampShopMgr.h"
#include "../../Country/GsCountryWarLoader.h"
#include "MessageStruct/MSProtoc/MS_NewActivities.pb.h"
#include "FileLoader/GameParaLoader.h"
#include "../../Attribute/AttributeCreature.h"
#include "../../ActiveUIHighLight/CActiveHighLightMgr.h"
#include "../../CountryRedEnvelope/CountryRedEnvelopeMgr.h"
#include "../../Clan/ClanMessage.h"
#include "MessageStruct/CharLogin/DayUpdate.pb.h"
#include "MessageStruct/ServerReturnChar.pb.h"
#include "MessageStruct/SystemNotice/SystemNotice.pb.h"
#include "../../Festival/FestivalAttr.h"
#include "../../PVP/PVPMgr.h"
#include "../../RewardHail/OnceReward.h"
#include "../../Marry/MarryRing.h"
#include "MessageStruct/Marry/MarryInstance.pb.h"
#include "../../Marry/MarryLoader/MarryInstanceLoader.h"
#include "../../Distortion/Distortion.h"
#include "../../Meditation/PlayerMeditation.h"
#include "FileLoader/ArenaLoader.h"
#include "MessageStruct/Pay/58Play.pb.h"
#include "MessageStruct/CharLogin/ChristmasDB.pb.h"
#include "../../FestivalUI/ChristmasManage.h"
#include "MessageStruct/Group/GroupMessage.pb.h"
#include "../../Daily/DailyMgr.h"
#include "../../FestivalUI/NewYearActivity.h"
#include "../../Activity/OnLineAtTimeMgr.h"
#include "../../MapInstance/PlayerChallenge.h"
#include "../../MapInstance/HeroInfo.h"
#include "../../Arena/ArenaMgr.h"
#include "../../StatisticMgr/StatisticHandle.h"
#include "../../Country/CountryOverlordCarbonLoader.h"

PlayerMessage * PlayerMessage::m_instance = 0;


PlayerMessage::PlayerMessage() : m_timerID(0), m_flushID(0), m_ervery_min_flushID(0),m_eightHoursTime(CUtil::GetNowSecond()+8*60*60*1000)
{
	m_PlayerCache.clear();
//	m_timerID = 0;
	//注册定时器
	if(m_timerID.IsValid())
	{
		m_timerID.SetNull();
	}

	if(m_flushID.IsValid())
	{
		m_flushID.SetNull();
	}

	if(m_ervery_min_flushID.IsValid())
	{
		m_ervery_min_flushID.SetNull();
	}

	if(m_eightHoursTimer.IsValid())
	{
		m_eightHoursTimer.SetNull();
	}

	m_timerID = TimerInterface::GetInstance()->RegRepeatedTime(&PlayerMessage::RemoveDelayPlayer, this, 0, 0, 0, 0,20 * 1000, 20 * 1000);
	m_flushID = TimerInterface::GetInstance()->RegRepeatedTime(&PlayerMessage::FlushPlayerFightState, this, 0, 0, 0, 0,5 * 1000, 5 * 1000);

	m_ervery_min_flushID = TimerInterface::GetInstance()->RegRepeatedTime(&PlayerMessage::FlushPlayerOnlineState, this, 0, 0, 0, 0, 5 * 1000, 1 * 60 * 1000);
	m_eightHoursTimer = TimerInterface::GetInstance()->RegRepeatedTime(&PlayerMessage::FlushPlayerOnlineState, this, 0, 0, 0, 0, 5 * 1000, 8 * 60 * 60 * 1000);
	m_ervery10SecondFlush = TimerInterface::GetInstance()->RegRepeatedTime(&PlayerMessage::FlushPlayerOnlineStateS, this, 0, 0, 0, 0, 5 * 1000, 10 * 1000);

	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_CLIENTIN);
//	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_GT2GM_SYNPATH);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_SYNCHPOS);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_GT2GM_SYNCHPOS);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_GT2GM_REVIVAL);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_GT2GM_MONITOR);
	//DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_GT2GM_UPDATE_CHANNEL);

	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_PLAYERINFO);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_CHARATTR);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_PLAYEREXIT);

	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_SEEOTHERPLAYERINFO);

	DEF_MSG_QUICK_REQUEST_REG_FUN(PlayerMessage, MSG_REQ_C2GT_CHANGEPKTYPE);    //切换PK模式
	DEF_MSG_QUICK_REQUEST_REG_FUN(PlayerMessage, MSG_REQ_C2GM_GATHER);//开始采集
	DEF_MSG_QUICK_REQUEST_REG_FUN(PlayerMessage, MSG_REQ_C2GM_FIYING_SHOES);    //飞鞋
	DEF_MSG_QUICK_REQUEST_REG_FUN(PlayerMessage, MSG_REQ_GET_ATTR);

	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_CHANGEMAP);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_PLAYER_ONLINE);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GT2WS_CLIENTIN);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2DB_PLAYERINFO);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GS2DB_SAVE_PLAYERDATA);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2GT_CHANGEMAP);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2DB_CONTAINERINFO);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2DB_SEEOTHERPLAYERINFO);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2GT_PLAYER_ONLINE);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_REG_WS2GM_SYNCRANKINGREWARD);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_SYNCHATTR);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_AD_SKILLBUFF);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_MAKERT_INFO);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_CHANGE_EARNINGS);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_UP_CARRIAGES);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_SYSREWARD);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_ACTIVITY_INVESTCOST);

//	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_ISHAVECOMPENSATETOGET);

	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_ARENABECHALL);

	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_SYNCHATTRCHAR);

	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2WS_GET_WS_DATA);

	DEF_MSG_QUICK_SIMPLE_REG_FUN(PlayerMessage, MSG_SIM_C2GM_UPDATEMARKET_UISTATE); // 更改市场UI状态

	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_NOTICESELLERUI);

	DEF_MSG_QUICK_REQUEST_REG_FUN(PlayerMessage,MSG_REQ_C2GM_JUMP_POINT);

	DEF_MSG_QUICK_REQUEST_REG_FUN(PlayerMessage,MSG_REQ_C2GM_DOWN_POINT);

	DEF_MSG_QUICK_REQUEST_REG_FUN(PlayerMessage, MSG_REQ_C2GM_REFRESHARENA);

	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2WS_REFRESHARENA);

	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_58PLAY_SYSREWARD);

	DEF_MSG_QUICK_REQUEST_REG_FUN(PlayerMessage,MSG_REQ_C2GM_CLICK_PLAYER_HEAD);

	DEF_MSG_QUICK_REQUEST_REG_FUN(PlayerMessage, MSG_SIM_C2GM_CHANGE_TASK_MAP); 

	this->InitServerTime();
}

PlayerMessage::~PlayerMessage()
{
	if(m_timerID.IsValid())
	{
		m_timerID.SetNull();
	}

	if(m_flushID.IsValid())
	{
		m_flushID.SetNull();
	}

	if(m_ervery_min_flushID.IsValid())
	{
		m_ervery_min_flushID.SetNull();
	}

	if(m_dayTimerID.IsValid())
	{
		m_dayTimerID.SetNull();
	}

	if(m_eightHoursTimer.IsValid())
	{
		m_eightHoursTimer.SetNull();
	}

	Smart_Ptr<Player> nullPlayer;
	Smart_Ptr<Monster> nullMon;

	INT64PLAYERMAP::iterator it = m_allPlayer.begin();
	for (; it != m_allPlayer.end(); ++it)
	{
		it->second->SetMyself(nullPlayer);
		it->second->AddTaskMonster(nullMon);
		cout << endl;
		cout << "~PlayerMessage  SetMyself  Charid "
		                << it->second->GetID() << " playerdd "
		                << &it->second << endl;
		cout << endl;
	}

	INT64PLAYERCACHEMAP::iterator itCa = m_PlayerCache.begin();
	for (; itCa != m_PlayerCache.end(); ++itCa)
	{
		itCa->second.playerCache->SetMyself(nullPlayer);
		cout << endl;
		cout << "~PlayerMessage  SetMyself  Charid "
		                << itCa->second.playerCache->GetID()
		                << " playerdd " << &itCa->second.playerCache
		                << endl;
		cout << endl;
	}

	m_PlayerCache.clear();
	m_allPlayer.clear();
}

void PlayerMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

//	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GT2GM_CLIENTIN);
//	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GT2GM_SYNPATH);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GT2GM_SYNCHPOS);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GT2GM_REVIVAL);
	//DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GT2GM_UPDATE_CHANNEL);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GT2GM_MONITOR);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_REG_WS2GM_SYNCRANKINGREWARD);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_SYNCHATTR);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_AD_SKILLBUFF);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_CHANGE_EARNINGS);
//	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_ISHAVECOMPENSATETOGET);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_ARENABECHALL);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_SYNCHATTRCHAR);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_NOTICESELLERUI);
		DEF_SWITCH_TRY_DISPATCH_END
	}

void PlayerMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
		DEF_SWITCH_TRY_DISPATCH_BEGIN

		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_CLIENTIN);
	 DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_PLAYERINFO);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_CHARATTR);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_CHANGEMAP);
//	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_CHANGESCENE);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_PLAYEREXIT);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_SEEOTHERPLAYERINFO);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_PLAYER_ONLINE);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_SYNCHPOS);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_MAKERT_INFO);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_UP_CARRIAGES);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_SYSREWARD);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_58PLAY_SYSREWARD);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_ACTIVITY_INVESTCOST);

		DEF_SWITCH_TRY_DISPATCH_END
	}

void PlayerMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
		DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2WS_CLIENTIN);
		DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2DB_PLAYERINFO);
		DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GS2DB_SAVE_PLAYERDATA);
		DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2GT_CHANGEMAP);
		DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2DB_SEEOTHERPLAYERINFO);
		DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2GT_PLAYER_ONLINE);
		DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2DB_CONTAINERINFO);
		DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2WS_GET_WS_DATA);
		DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2WS_REFRESHARENA);

	DEF_SWITCH_TRY_DISPATCH_END
}

void PlayerMessage::addSendDataCach(int64 charID, SaveDataInfo& sendData)
{
	std::map<int64,SaveDataInfo>::iterator it=m_saveDataList.find(charID);
	if(it==m_saveDataList.end())
	{
		m_saveDataList[charID] = sendData;
	}
}

void PlayerMessage::dealWithSaveResult(SaveDataInfo& data)
{
		//次数达到之后才会处理
		data.count -= 1;
		if(data.count > 0 )
			return;

		if(data.content.Get() == NULL)
			return;

		//
		switch(data.msgtype)
		{
		case SimpleMessage:
		{
			break;
		}
		case Request:
		{
			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(data.msgid, data.message, data.content.Get(), Request);
			messRet->SetAct(data.m_act);
			Message_Facade::Send(messRet);
			break;
		}
		case Ack:
		{
			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(data.msgid, data.message, data.content.Get());
			Message_Facade::Send(messRet);
			break;
		}
		default:
			break;
		}
}

int64 PlayerMessage::GetCharidByName(string name)
{
	INT64PLAYERMAP::iterator it;
	for (it = m_allPlayer.begin(); it != m_allPlayer.end(); ++it)
	{
		if (it->second->GetName() == name)
			return it->first;
	}
	return 0;
}

bool PlayerMessage::IsOnline(int64 charid)
{
	INT64PLAYERCACHEMAP::iterator itCache = m_PlayerCache.find(charid);
	if (itCache != m_PlayerCache.end())
	{
		return true;
	}

	INT64PLAYERMAP::iterator it = m_allPlayer.find(charid);
	if (it != m_allPlayer.end())
	{
		return true;
	}

	return false;
}

bool PlayerMessage::IsOnlineEx(int64 charid)
{
	INT64PLAYERMAP::iterator it = m_allPlayer.find(charid);
	if (it != m_allPlayer.end())
	{
		return true;
	}

	return false;
}

bool PlayerMessage::isInSameGroupSector(Smart_Ptr<Player>& player, int64 mapid, int xPos, int yPos)
{
	if( GET_MAP_ID(player->GetMapID()) != mapid)
		return false;

	if(player->GetGameZone() == NULL)
	{
		return false;
	}

	bool isSameGroup = player->GetGameZone()->IsInSameNineGroup( player->GetXNewPos(), player->GetYNewPos(), xPos, yPos );

	return isSameGroup;
}

//当前地图是否可以小飞鞋传送
int PlayerMessage::isCanFly(int mapID, int64 xPos, int64 yPos,int nLv)
{
		MapStaticInfo *info = CMapConfig::GetInstance()->GetStaticInfo(mapID);
		if(info==NULL)
		{
			return eMapIdError;
		}

		if(info->maptype >= eCopyMap )
		{
			return eCopyMapCannotTransfer;
		}

		if(info->mapid <= 0)
		{
			return eMapIdError;
		}

		if(info->lv > nLv)
		{
			return eLVNotEnough;
		}

		//判断坐标
		if(!CMapConfig::GetInstance()->IsCanValidPos(info->mapid, xPos, yPos))
		{
				return eStartPointError;
		}

	return 0;
}

//判断玩家是否已经登录
bool PlayerMessage::IsLoginON(Smart_Ptr<Player> &player,int flag)
{
	int64 sceneid = -1;
	//根据地图类型判断
	switch (GET_MAP_TYPE(player->GetMapID()))
	{
		case eOnlyMap:
		{
			//判断地图是否有场景
			if(CSceneMgr::GetInstance()->IsHaveScene(player->GetMapID()))
			{
				sceneid = player->GetMapID();
			}
			else
			{
				//如果没有，得到场景ID
				sceneid = CSceneMgr::GetInstance()->GetSceneIDAboutPlayerIsLoginOn(GET_MAP_ID(player->GetMapID()));
				if(sceneid > 0 && sceneid != player->GetMapID())
				{
					player->SetMapID(sceneid);
					player->SetDataFlag(eBaseInfo);
				}
			}

			break;
		}
		case eStaticMap:
		{    //静�1�7�地图，选择人数少的
			sceneid = CSceneMgr::GetInstance()->GetSceneIDAboutPlayerIsLoginOn(GET_MAP_ID(player->GetMapID()));
			if (sceneid > 0 && sceneid != player->GetMapID())
			{
				player->SetMapID(sceneid);
				player->SetDataFlag(eBaseInfo);
			}

			break;
		}
		case eBeyondMap:
		case ePVEMap:    //pve副本
		{
			bool isChange = false;

			if (CSceneMgr::GetInstance()->IsHaveScene(player->GetMapID()))
			{
				Smart_Ptr<CGameDuplicate> instance =DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
				if (instance)
				{
					//判断是否可以切换到此场景
					//帮派站副本是不能直接进去的，必须通过界面入口点击进去
					InstanceType iType = instance->GetInstanceScript()->m_eType;

					if (instance->IsCanEnter(player))
					{
						sceneid = player->GetMapID();
						isChange = true;
						if(iType == eBeyondSvrBattle)
						{
							CrossSvrCompetition::Point vecPos;
							int index = -1;
							int type = 0;

							BeyondBattleMgr::GetInstance()->GetPlayerIndexAndType(instance->GetSceneID(), player->GetID(), index, type);
							if(index >= 0)
							{
								CCrossSvrCompeteLoader::GetInstance()->GetCompeteDebutPos(instance->GetInstanceScript()->m_nMapID, index, vecPos, type);
							}

							if(vecPos.nPosX != 0 && vecPos.nPosY != 0)
							{
								player->SetXPos(vecPos.nPosX);
								player->SetYPos(vecPos.nPosY);
							}
							else
							{
								player->SetXPos(instance->GetInstanceScript()->m_nXPos);
								player->SetYPos(instance->GetInstanceScript()->m_nYPos);
							}

							player->SetXNewPos();
							player->SetYNewPos();
//							player->SetGroup(CSceneMgr::GetInstance()->GetSectorGroup(player->GetMapID(), player->GetXPos(), player->GetYPos()));
							player->SetDataFlag(eBaseInfo);
						}

						if(iType == eMarryInstance)
						{
							int64 txpos = 0;
							int64 typos = 0;

							if(instance->GetParamfValue() == player->GetID())
							{
								MarryInstanceLoader::GetInstance()->GetBothPoint(txpos,typos,true);
							}
							else
							{
								MarryInstanceLoader::GetInstance()->GetBothPoint(txpos,typos,false);
							}

							player->SetXPos(txpos);
							player->SetYPos(typos);
						}
					}
				}
			}

			if (!isChange)
			{
				LOG_ERROR(FILEINFO, "Transfer Map[%d] is error", GET_MAP_ID(player->GetMapID()));

				MapStaticInfo *sMapInfo;

				sMapInfo = CMapConfig::GetInstance()->GetFistMapInfo();
				if(sMapInfo == 0)
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

				player->SetXPos(sMapInfo->reliveXPos);
				player->SetYPos(sMapInfo->reliveYPos);
				player->SetXNewPos();
				player->SetYNewPos();

				player->SetDataFlag(eBaseInfo);
			}

			break;
		}
	}

	if (sceneid < 0)
	{
		return false;
	}

	player->SetXNewPos();
	player->SetYNewPos();

	return true;
}

//得到所有玩家的ID
void PlayerMessage::getAllCharId(vector<int64> &allCharId)
{
	INT64PLAYERMAP::iterator it;
	for (it = m_allPlayer.begin(); it != m_allPlayer.end(); ++it)
	{
		allCharId.push_back(it->first);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(PlayerMessage, MSG_REQ_GT2GM_CHANGEMAP)
{
	ServerReturn::ServerDoubleInt meContent;
	ChangeMap::ChangeMapInfo ack;

	char *con;
	int len = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	INT64PLAYERMAP::iterator it = m_allPlayer.find(meContent.retf());
	if(it==m_allPlayer.end())
		return;

	Smart_Ptr<Player> pkPlayer = it->second;
	if(pkPlayer.Get()==NULL)
		return;

	MapStartPoint info;
	CMapConfig::GetInstance()->GetTransferMapByStartPoint(meContent.rets(), info);

	if(info.ID > 0)		//正常地图切换
	{
		ChangeNormal(pkPlayer, message, info);
	}
	else //副本切图 换线
	{
			if(meContent.rets() == it->second->GetMapID())
				return;

			pkPlayer->setBWillInInstance(false);

			int mapType = GET_MAP_TYPE(meContent.rets());
			switch(mapType)
			{
				case eStaticMap:				//换线
				{
					ChangeMapLine(message,pkPlayer,meContent.rets(),((int64)pkPlayer->GetXNewPos() << 48) | ((int64)pkPlayer->GetYNewPos() << 32) | eChangeMapLine);
					return;
				}
				case ePVEMap:					//切入PVE副本
				case eBeyondMap:			//切入跨服副本
				{
					ChangeInDuplicate(pkPlayer, message, meContent.rets());
					return;
				}

				case 0:					//切出副本
				{
					ChangeOutDuplicate(pkPlayer, message);
					return;
				}
			}
	}

}

DEF_MSG_REQUEST_DEFINE_FUN(PlayerMessage, MSG_REQ_GT2GM_PLAYER_ONLINE)
{
	ServerReturn::ServerFiveInt meContent;
	ServerReturn::ServerRetInt req;
	char *con;
	int len = 0;
	Smart_Ptr<SvrItem> dbSvr;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "changing map and requesting online");
	//cout<<"＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋上线！"<<endl;
	//找到DB服
	ServerConHandler::GetInstance()->GetDBServerBySvrID(GET_SERVER_CHARID(meContent.retf()), dbSvr);
	if (!dbSvr)
	{
		LOG_FATAL(FILEINFO, "Gameserver find no DBServer in server [serverID=%d]",GET_SERVER_CHARID(meContent.retf()));

		return;
	}

	req.set_ret(GET_PLAYER_CHARID(meContent.retf()));

	switch(GET_MAP_TYPE(meContent.rets()))
	{
	case eBeyondMap:
	case ePVEMap:
		{
			if (CSceneMgr::GetInstance()->IsHaveScene(meContent.rets()))
			{
				Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(meContent.rets());
				if (instance)
				{
					instance->SetOldMapPoint(meContent.retf(),meContent.oldrets(),meContent.oldxpos(),meContent.oldypos());
				}
			}

			break;
		}
	}

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GM2DB_PLAYERINFO, &req, dbSvr.Get(), Request);
	messRet->SetAct(new FirstAct(message, 1));
	Message_Facade::Send(messRet);
}

DEF_MSG_SIMPLE_DEFINE_FUN(PlayerMessage, MSG_SIM_GT2GM_MONITOR)
{
	Monitor::SaveMonitor meContent;
	char *con;
	int len = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	INT64PLAYERMAP::iterator it = m_allPlayer.find(message->GetMessageTime());
	if (it != m_allPlayer.end())
	{
		PlayerInfo::MonitorInfo info;

		info = meContent.info();
		it->second->InitMonitorInfo(info);
		it->second->SetDataFlag(eMonitorInfo);
	}
	else
	{
		LOG_ERROR(FILEINFO, "client save monitor but not find player[gs charid=%lld] ",GET_PLAYER_CHARID(message->GetMessageTime()));
	}
}

//网关服发消息通知clientin
DEF_MSG_REQUEST_DEFINE_FUN(PlayerMessage, MSG_REQ_GT2GM_CLIENTIN)
{
	ServerReturn::ServerRetInt meContent;
	char *con;
	int len = 0;
	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	int64 charID = message->GetMessageTime();
	INT64PLAYERMAP::iterator itMap = m_allPlayer.find(charID);
	if (itMap != m_allPlayer.end())
	{
		LOG_ERROR(FILEINFO, "          player[%lld] already online        ", charID);
		return;
	}

	INT64PLAYERCACHEMAP::iterator it = m_PlayerCache.find(charID);
	if(it != m_PlayerCache.end())
	{
		Smart_Ptr<Player> tempPlayer = it->second.playerCache;
		CharLogin::WBaseInfo wBaseInfo;

		Smart_Ptr<Object> obj = tempPlayer;
		it->second.playerCache->ClearPath();

		if(!CSceneMgr::GetInstance()->EnterScene(obj))
		{
			LOG_ERROR(FILEINFO, "client already in map and synch view but player[gs charid=%lld] cannot enter scene",charID);
			return;
		}

		//进入某张地图事件触发
		if(tempPlayer)
		{
			PlayerLoginEventArgs tArgs(tempPlayer,GET_MAP_ID(obj->GetMapID()));
			tempPlayer->FireEvent(PLAYER_ENTERMAP_EVENT_TYPE, tArgs);
		}
		{
			getWBaseInfo(wBaseInfo,it->second.playerCache,false);
		}

		Smart_Ptr<SvrItem> worldSvr;
		ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(it->second.playerCache->GetID()),worldSvr);
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2WS_CLIENTIN, &wBaseInfo,worldSvr.Get(), Request,30);
		messRet->SetAct(new requestAct(message));
		Message_Facade::Send(messRet);

		if(meContent.ret())
		{
			//同步神兵信息
			it->second.playerCache->GetMagicWeapon()->SendToClientMagic();
			it->second.playerCache->GetSMagic()->SendToClientMagic();
			it->second.playerCache->GetHeartMagicWeapon()->SendToClientMagic();
		}

		//玩家登入完成，添加到玩家列表里，删除Cache值
		m_PlayerCache.erase(it);
		m_allPlayer[charID] = tempPlayer;

		//初始化技能
		tempPlayer->GetSkillMgr()->InitBuff();
		tempPlayer->GetDailyManager()->InitRecoverResource();
	}
	else
	{
		LOG_ERROR(FILEINFO, "MSG_REQ_GT2GM_CLIENTIN: player[%lld] login in timeout, time=%lld", GET_PLAYER_CHARID(charID), CUtil::GetNowSecond());
	}
}

DEF_MSG_ACK_DEFINE_FUN(PlayerMessage, MSG_REQ_GT2WS_CLIENTIN)
{
	//请求超时
	if (message->GetErrno() == eReqTimeOut)
	{
		LOG_DEBUG(FILEINFO, "gameserver request worldserver player info and ack timeout");
		return;
	}

	CharLogin::SyncMap syncMap;
	char *con;
	int len = 0;

	con = message->GetBuffer(len);
	syncMap.ParseFromArray(con, len);

	INT64PLAYERMAP::iterator it = m_allPlayer.find(syncMap.charid());
	if(it != m_allPlayer.end())
	{
		syncMap.set_mapid(it->second->GetMapID());

		Safe_Smart_Ptr<CommBaseOut::Message> worldmess = build_message(MSG_REQ_GT2GM_CLIENTIN, static_cast<requestAct *>(act.Get())->mesReq, &syncMap);
		Message_Facade::Send(worldmess);
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(PlayerMessage,MSG_SIM_GT2GM_SYNCHPOS)
{
	PlayerSynch::SynchInfo ret;
	char *con;
	int len = 0;
	int isValid = 0;
	bool isMove = false;

	con = message->GetBuffer(len);
	ret.ParseFromArray(con, len);

	int64 charID = message->GetMessageTime();
	INT64PLAYERMAP::iterator it = m_allPlayer.find(charID);
	if (it != m_allPlayer.end())
	{
		if(!it->second->GetDistortionMgr()->IscanMove())
		{
			return;
		}

		//取消打坐状态
		it->second->CanccelMeditation();

		if (it->second->IsInCreatureStateAny(ERS_Still | ERS_Ice | ERS_Stun))
		{
			isValid = 1;
		}
		else
		{
			Smart_Ptr<Player> temp = it->second;
			if(!it->second->GetGameZone())
			{
				return;
			}
			else
			{
				isValid = !it->second->GetGameZone()->IsValidPos(ret);
			}

			PlayerSynch::CharPos pos = ret.pos(0);
			int xpos = pos.xpos(), ypos = pos.ypos();

			if (!isValid)
			{
				if (!it->second->IsMoveToPosition(xpos,ypos))
				{
					if (xpos != it->second->GetXNewPos() || ypos != it->second->GetYNewPos())
					{
						xpos =  it->second->GetXNewPos();
						ypos = it->second->GetYNewPos();
						isValid = 2;
					}
				}
				else
					isMove = true;
			}
			else
			{
				xpos = it->second->GetXNewPos();
				ypos = it->second->GetYNewPos();
			}

			if(isValid != 0)
			{
				ServerReturn::ServerThreeInt ret;
				int64 mapid = GET_MAP_TYPE(it->second->GetMapID());

				ret.set_retf(mapid);
				ret.set_rets(xpos);
				ret.set_rett(ypos);

				QuickMessage::GetInstance()->AddSendMessage(it->second->GetID(), it->second->GetChannelID(), &ret, MSG_SIM_GT2C_SENDPOS);
			}

			if (isMove)
			{
				if(it->second.Get()!=NULL)
				{
					PlayerMoveEvent tArgs(it->second->GetID());
					FireGlobalEvent(PLAYER_MOVE, tArgs);
				}

				Smart_Ptr<Object> tmpObj = it->second;

				if (!CSceneMgr::GetInstance()->MoveTo(tmpObj))
					LOG_ERROR(FILEINFO, "player[gs charid=%lld] move  failed ", GET_PLAYER_CHARID(it->second->GetID()));

				if(ret.type() == 2)
				{
					it->second->ClearPath();
				}
			}
		}
	}
	else
		LOG_ERROR(FILEINFO, "player synch position but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
}

DEF_MSG_REQUEST_DEFINE_FUN(PlayerMessage, MSG_REQ_GT2GM_SYNCHPOS)
{
	PlayerSynch::SynchInfo ret;
	char *con;
	int len = 0;
	int isValid = 0;

	con = message->GetBuffer(len);
	ret.ParseFromArray(con, len);

	//LOG_DEBUG(FILEINFO, "Player synch position or find path to GameServer");

	int64 charID = message->GetMessageTime();
	INT64PLAYERMAP::iterator it = m_allPlayer.find(charID);
	if (it != m_allPlayer.end())
	{
		if(!it->second->GetDistortionMgr()->IscanMove())
		{
			return;
		}

		//取消打坐状态
		it->second->CanccelMeditation();

		if(it->second->IsInCreatureStateAny(ERS_Still | ERS_Ice | ERS_Stun | ERS_Sleep))
		{
			return;
		}
		else
		{
			Smart_Ptr<Player> temp = it->second;
			if(!it->second->GetGameZone())
				return;

			isValid = !it->second->GetGameZone()->IsValidPos(ret);

			if (!isValid)
			{
				if(ret.pos_size() > 0)
				{
					int tXLen = abs(ret.pos(0).xpos() - it->second->GetXNewPos());
					int tYLen = abs(ret.pos(0).ypos() - it->second->GetYNewPos());
					if((int)sqrt(tXLen * tXLen + tYLen * tYLen) > 8)
					{
						isValid = 1;
					}
					else
					{
						PlayerSynch::PathToAllPlayer path;

						path.set_charid(it->second->GetKey());
						for (int i = 0; i < ret.pos_size(); ++i)
						{
							PlayerSynch::CharPos * toSingle =path.add_pos();

							toSingle->CopyFrom(ret.pos(i));
						}

						it->second->GetGameZone()->SendSynchPath(path, it->second);
					}
				}
			}
		}

		ServerReturn::ServerRetInt pathRet;
		pathRet.set_ret(isValid);

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_SYNCHPOS, message, &pathRet, Ack);
		Message_Facade::Send(messRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "player synch position but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(PlayerMessage, MSG_REQ_GT2GM_PLAYEREXIT)
{
	ServerReturn::ServerDoubleInt meContent;
	Smart_Ptr<ServerReturn::ServerRetInt> ack = new ServerReturn::ServerRetInt();
	char *con;
	int len = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);
	ack->set_ret(meContent.retf());

	int64 offlineTime = CUtil::GetNowSecond();
	//下线，清玩家缓存
	INT64PLAYERCACHEMAP::iterator ite = m_PlayerCache.find(meContent.retf());
	if (ite != m_PlayerCache.end())
	{
		ite->second.playerCache->SetOffLineTime(offlineTime);
		if (ite->second.playerCache->GetType() == ePlayer)
		{
			PlayerLogout(ite->second.playerCache);
			SaveDataInfo sendData;
			sendData.msgtype = Ack;
			sendData.msgid = MSG_REQ_GT2GM_PLAYEREXIT;
			sendData.message = message;
			sendData.content = ack;
			ite->second.playerCache->SaveAllPlayer(sendData,true,true);

			Smart_Ptr<Player> myself;
			ite->second.playerCache->SetMyself(myself);
		}
		m_PlayerCache.erase(ite);
	}
	else
	{
		//从玩家列表中移出
		INT64PLAYERMAP::iterator it = m_allPlayer.find(meContent.retf());
		if (it != m_allPlayer.end())
		{
			it->second->SetOffLineTime(offlineTime);
			Smart_Ptr<Object> obj = it->second;
			if (obj->GetType() == ePlayer)
			{
				CSceneMgr::GetInstance()->LeaveScene(obj);
				PlayerLogout(it->second);
				{
					SaveDataInfo sendData;
					sendData.msgtype = Ack;
					sendData.msgid = MSG_REQ_GT2GM_PLAYEREXIT;
					sendData.message = message;
					sendData.content = ack;
					it->second->SaveAllPlayer(sendData,true,true);
				}
				Smart_Ptr<Player> myself;
				it->second->SetMyself(myself);
			}
			m_allPlayer.erase(it);
		}
		else
		{
			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_PLAYEREXIT, message, ack.Get());
			Message_Facade::Send(messRet);
		}
	}

}

//网关服请求游戏服玩家信息
DEF_MSG_REQUEST_DEFINE_FUN(PlayerMessage, MSG_REQ_GT2GM_PLAYERINFO)
{
	ServerReturn::ServerRetIntChar req;
	ServerReturn::ServerRetInt con;
	int len = 0;
	char *str = message->GetBuffer(len);

	req.ParseFromArray(str, len);

	LOG_DEBUG(FILEINFO, "Player login on and get playerinfo[%lld]", GET_SERVER_CHARID(req.retf()));

	Smart_Ptr<SvrItem> dbSvr = NULL;

	if(IsOnline(req.retf()))
	{
		LOG_ERROR(FILEINFO, "player[%lld] is online", GET_SERVER_CHARID(req.retf()));
		return;
	}

	//找到DB服
	ServerConHandler::GetInstance()->GetDBServerBySvrID(GET_SERVER_CHARID(req.retf()),dbSvr);
	if(!dbSvr)
	{
		LOG_FATAL(FILEINFO, "Gameserver find no DBServer in server [serverID=%lld]",GET_SERVER_CHARID(req.retf()));
		return;
	}

	con.set_ret(GET_PLAYER_CHARID(req.retf()));

	//建立游戏服向DB服请求玩家信息
	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GM2DB_PLAYERINFO, &con, dbSvr.Get(), Request);
	string client_ip = req.rets();

	messRet->SetAct(new StringIntAct(message, 0, 0, client_ip));
	Message_Facade::Send(messRet);
}

DEF_MSG_REQUEST_DEFINE_FUN(PlayerMessage, MSG_REQ_GT2GM_CHARATTR)
{
	CharBase::CharAttr toClient;
	LOG_DEBUG(FILEINFO, "Player request battle attribute");

	int64 charID = message->GetMessageTime();
	INT64PLAYERMAP::iterator it = m_allPlayer.find(charID);
	if (it != m_allPlayer.end())
	{
		it->second->SetBattleInfo(&toClient);
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_CHARATTR, message,&toClient);
		Message_Facade::Send(messRet);
	}
	else
	{
		INT64PLAYERCACHEMAP::iterator itCache = m_PlayerCache.find(charID);
		if (itCache != m_PlayerCache.end())
		{
			itCache->second.playerCache->SetBattleInfo(&toClient);
			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_CHARATTR, message,&toClient);
			Message_Facade::Send(messRet);
		}
		else
		{
			LOG_ERROR(FILEINFO, "Player request battle attribute but player[gs charid=%lld] is not exist",GET_PLAYER_CHARID(charID));
		}
	}
}

//游戏服请求DB服玩家信息
DEF_MSG_ACK_DEFINE_FUN(PlayerMessage, MSG_REQ_GM2DB_PLAYERINFO)
{
	//请求超时
	if (message->GetErrno() == eReqTimeOut)
	{
		LOG_DEBUG(FILEINFO, "gameserver request dbserver player info and ack timeout");
		return;
	}

	//声明玩家信息的各个结构体
	PlayerInfo::PlayerInfo content;
	int len = 0;
	int64 nowTime = CUtil::GetNowSecond();

	char *con = message->GetBuffer(len);
	content.ParseFromArray(con, len);
	const PlayerInfo::BaseInfo& bsinfo = content.bsinfo();

	if(IsOnline(bsinfo.charid()))
	{
		LOG_ERROR(FILEINFO, "player[%lld] is online", bsinfo.charid());
		return;
	}

	const PlayerInfo::BattleInfo& binfo = content.binfo();
	const PlayerInfo::QuestInfo& queInfo = content.questinfo();
	const PlayerInfo::SkillInfo& skInfo = content.skill();
	const PlayerInfo::CDInfo& cdInfo = content.cd();
	const PlayerInfo::MonitorInfo& mtInfo = content.mt();
	const PlayerInfo::EquipQuickInfo& eqInfo = content.equipquick();
	const PlayerInfo::GirlWarInfo& gwInfo = content.girlwarinfo();
	const PlayerInfo::CounterServiceList& counter = content.counter();
	const PlayerInfo::HorseInfo& hInfo = content.horse();
	const PlayerInfo::DanLuInfo& danluInfo = content.danlu();
	const PlayerInfo::MagicWeapon& mInfo = content.magicw();

	const PlayerInfo::MagicWeapon& sMagic = content.smagic();
	const PlayerInfo::MagicWeapon& heartMagic = content.heartmagic();

	const PlayerInfo::RandCJobLucky& cInfo = content.cjob();
	const PlayerInfo::LucklyTurntable& tableInfo = content.lucktable();
	const PlayerInfo::DestinationInfoList& destinationInfo = content.destination();
	const PlayerInfo::Activity& activityInfo = content.activity();
	const PlayerInfo::InstanceInfoTwoDays& instance = content.instancelist();
	const PlayerInfo::MoneyInstance& moneyInstance = content.info();
	const PlayerInfo::SignInInfo& signInInfo = content.signin();
	const PlayerInfo::EscortInfo& escortInfo = content.escort();
	const PlayerInfo::PlayerVipInfo& vipInfo = content.playervip();
	const PlayerInfo::PrestigeInfoList& prestige = content.prestige();
	const PlayerInfo::TitleInfoList& title = content.title();
	const FashionMsgProto::FashionShow& fashionInfo = content.fashion();
	const VitalityProto::ProtoVitalityInfo& vitality = content.vitality();
	const PlayerInfo::CampShopInfo& camp = content.campshopdata();
	const PlayerInfo::FestivalInfo& festival = content.festivaldata();
	const PlayerInfo::OnceRewardInfo& onceRewardInfo = content.oncereward();
	const PlayerInfo::GroupInstance& groupInfo = content.groupinfo();
	const PlayerInfo::EliteInstance& eliteInfo = content.eliteinfo();
	const PlayerInfo::CauchemarInstance& cauchemarInfo = content.cauchemarinfo();
	const PlayerInfo::ArenaDataInfo& arenaDBInfo = content.arenadata();
	const PlayerInfo::ChallengeInstance& challenge = content.challengedata();
	const PlayerInfo::HeroInstanceData& hero = content.herodata();
	const PlayerInfo::AdvancedInstance& advancedInfo = content.advanced();
	const  PlayerInfo::BaseShopInfo& baseShopInfo = content.baseshopinfo();
	const  PlayerInfo::AnswerInfo& answerInfo = content.answerinfo();


	Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(ePlayer);

	//将obj定为一个玩家Player
	Smart_Ptr<Player> player = obj;

	//必须调用重新初始化
	//player->ReInitPlayer();
	player->SetMyself(player);

	//设置玩家登录时间
	player->SetLoginTime(nowTime);

	//由于 counter 里面 保存 了 是否 要 重置的  标准参考时间  所以  必须 第一个  INIT 上一次时间
	player->GetCounterService()->InitLastResetTime(counter.lastresettime());

	//某些满级的模块直接获取活跃度，因此得在这些模块之前初始化之前初始化活跃度模块
	player->InitVitalityInfo(vitality);

	player->InitBaseInfo(content.bsinfo());

	player->InitBattleInfo(binfo);

	player->GetCDMgr()->InitCDInfo(cdInfo);

	player->InitMonitorInfo(mtInfo);

	player->SetChannelID(static_cast<StringIntAct *>(act.Get())->mesReq->GetChannelID());

	player->SetDBChannelID(Message_Facade::BindGroupChannel(static_cast<StringIntAct *>(act.Get())->mesReq->GetChannelID(), ServerConHandler::GetInstance()->GetDBChannelBySvrID(GET_SERVER_CHARID(player->GetID()))));

	player->GetContainer(eEquipQuickType)->SetArticleInfo(&eqInfo);

	player->InitSkillInfo(skInfo);

	//由于 可能 要 删除  任务 物品 故  初始化 任务 信息  要 在 前
	player->InitQuestInfo(queInfo);

	player->InitCounterInfo(counter);

	player->GetHorseManager()->InitHorseFromDB(hInfo);

	player->GetDanLuManager()->InitDanLuFromDB(danluInfo);

	player->GetMagicWeapon()->InitMagicWeapon(mInfo);
	player->GetSMagic()->InitMagicWeapon(sMagic);
	player->GetHeartMagicWeapon()->InitMagicWeapon(heartMagic);

	player->GetCJobLucky()->InitCJobLucky(&cInfo);

	player->GetLucklyTableMgr()->InitFromDB(tableInfo);

	player->InitDestinationInfo(destinationInfo);

	player->GetActivityMgr()->InitFromDB(activityInfo);

	player->InitInstanceInfo(instance);

	player->InitMoneyInstance(moneyInstance);

	player->GetChallenge()->InitChallengeInfo(challenge);

	player->GetHeroMgr()->InitHeroFromDB(hero);

	player->InitGroupInstance(groupInfo);

	player->InitVipInfo(vipInfo);

	player->InitSignInInfo(signInInfo);

	player->InitEscortInfo(escortInfo);

	player->InitPrestigeInfo(prestige);

	player->InitTitleInfo(title);

	player->InitFashionInfo(fashionInfo);

	player->InitOnceRewardInfo(onceRewardInfo);

	player->InitEliteInstance(eliteInfo);

	player->InitCauchemarInstance(cauchemarInfo);

	player->InitAdvancedInstance(advancedInfo);

	player->InitBaseShopInfo(baseShopInfo);

	player->InitAnswerInfo(answerInfo);

	player->InitArenaInfo(arenaDBInfo);

	player->getCampShopMgr()->online(camp);
	player->SetInitPartDataFlag(eCampShopInfo);

	player->getFestivalMgr()->online(festival);
	player->SetInitPartDataFlag(eFestivalInfo);

	player->GetMarryRing()->initFromDB(*content.mutable_ringdata());

	player->GetChristmas()->initFromDB(*content.mutable_christmasdata());

	player->GetNewYear()->initFromDB(*content.mutable_christmasdata());


	MapStaticInfo *sMapInfo = NULL;
	sMapInfo = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(player->GetMapID()));

	PLayerCache cache;
	cache.playerCache = player;
	cache.time = nowTime;
	cache.bFlag = true;

	m_PlayerCache[bsinfo.charid()] = cache;

	if(!(static_cast<StringIntAct *>(act.Get())->mesDataf))
	{		//刚登录的
		player->SetIP(static_cast<StringIntAct *>(act.Get())->mesDatafo);  //刚登录设置 玩家的 IP信息
	}
	player->InitGirlWarInfo(gwInfo);

	// 更新微端登录的计数器
	player->UpdateInfoByClientType();

	BEHAVIOR_MONI(player, BehaviorType::Main::eRole, BehaviorType::Minor::eRole_Login, MONITOR);

	BEHAVIOR_HANDLE_DO(player, BehaviorHandType::LoginRes(player->GetIP()))

	


//	StatisticMgr::GetInstance()->StatisticPlayerLoginInfo(player, eStatic_Charactor, eStaMinor_Charactor_Login);

	//请求容器数据
	ServerReturn::ServerFourInt req;
	req.set_retf(GET_PLAYER_CHARID(player->GetID()));

	Safe_Smart_Ptr<CommBaseOut::Message> messContain = build_message(MSG_REQ_GM2DB_CONTAINERINFO, &req, player->GetDBChannelID(), Request);
	messContain->SetAct(new commonAct(static_cast<StringIntAct *>(act.Get())->mesReq,player->GetID(), static_cast<StringIntAct *>(act.Get())->mesDataf));
	Message_Facade::Send(messContain);

#ifdef DEBUG
	int endTime = CUtil::GetNowSecond();

	if(endTime - nowTime > 100)
	{
		printf("\n 玩家初始化时间： time=%lld \n", endTime - nowTime );
		LOG_FATAL(FILEINFO, "ack player info and need more time[%lld]", endTime - nowTime);
	}
#endif
}

DEF_MSG_ACK_DEFINE_FUN(PlayerMessage, MSG_REQ_GS2DB_SAVE_PLAYERDATA)
{
	int64 charID = static_cast<IntAct*>(act.Get())->mesData;
	std::map<int64,SaveDataInfo>::iterator it=m_saveDataList.find(charID);
	if(it==m_saveDataList.end())
	{
		LOG_DEBUG(FILEINFO, "\n 请求保存玩家数据时返回找不到玩家：id=%lld \n", charID);
		return;
	}

	dealWithSaveResult(it->second);
	m_saveDataList.erase(it);

}

DEF_MSG_ACK_DEFINE_FUN(PlayerMessage, MSG_REQ_GM2GT_PLAYER_ONLINE)
{
	if (message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request gateserver player change map and online ack timeout");

		return;
	}

	LOG_DEBUG(FILEINFO, "gameserver request gameserver change map and online ack from db");

	ChangeMap::ChangeMapInfo meContent;
	char *con;
	int len = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	if(act.Get() == 0)
	{		//强制玩家切换地图
		ChangeMap::GMForceClientChangeMap toClient;

		MapStaticInfo *info = NULL;
		info = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(meContent.mapid()));
		if (!info)
		{
			return;
		}

		toClient.set_mapid(GET_MAP_ID(meContent.mapid()));
		toClient.set_xpos(meContent.pos() >> 16);
		toClient.set_ypos(meContent.pos() & 0xffff);
		toClient.set_key(meContent.key());

		Smart_Ptr<Player> player;
		GetPlayerByCharId(player,meContent.charid());
		if(player)
		{
			toClient.set_pktype(player->GetPkTypeByMap(info->pktype));
		}

		QuickMessage::GetInstance()->AddSendMessage(meContent.charid(), message->GetChannelID(), &toClient, MSG_SIM_GM2C_FORCE_CHANGEMAP);
		return;
	}

	//普通跳转地图
	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(static_cast<requestAct *>(act.Get())->mesReq->GetMessageID(), static_cast<requestAct *>(act.Get())->mesReq, &meContent);
	Message_Facade::Send(messRet);
}

DEF_MSG_ACK_DEFINE_FUN(PlayerMessage, MSG_REQ_GM2GT_CHANGEMAP)
{
	if (message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request gateserver player change map and ack timeout");

		return;
	}

	LOG_DEBUG(FILEINFO, "gameserver request gameserver change map and ack from db");

	ServerReturn::ServerThreeInt meContent;
	Smart_Ptr<ServerReturn::ServerFiveInt> toGT = new ServerReturn::ServerFiveInt();
	char *con;
	int len = 0;
	int xpos = 0, ypos = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	INT64PLAYERMAP::iterator it = m_allPlayer.find(meContent.retf());
	if(it != m_allPlayer.end() && meContent.rets())
	{
		Smart_Ptr<Object> obj = it->second;
		toGT->set_retf(meContent.retf());
		toGT->set_rets(meContent.rets());
		CSceneMgr::GetInstance()->LeaveScene(obj);

		//离开队伍
		sNewGroupMgr->LeaveGroup(it->second);

		//退出除副本以外的信息
		PlayerRemoveMemberEventArgs tArgs(it->second, GROUP_REMOVEMETHOD_LEAVE);
		FireGlobalEvent(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, tArgs);
		it->second->FireEvent(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, tArgs);
		int mapType = GET_MAP_TYPE(meContent.rets());//(meContent.rets() >> 40) & 0xfff;
		if(mapType >= eCopyMap)//切换到副本
		{
			toGT->set_oldrets(obj->GetMapID());
			toGT->set_oldxpos(obj->GetXPos());
			toGT->set_oldypos(obj->GetYPos());
		}

		switch(GET_MAP_TYPE(it->second->GetMapID()))
		{
		case eBeyondMap:
		case ePVEMap:
			{
				Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(it->second->GetMapID());
				if(instance)
				{
					instance->LeaveExame(it->second);

					OldMapInfo* info = instance->GetOldMapPoint(it->second->GetID());
					if(!info)
					{
						xpos = meContent.rett() >> 32;
						ypos = meContent.rett() & 0xffffffff;
					}
					else
					{
						xpos = info->xpos;
						ypos = info->ypos;
					}
				}
				else
				{
					xpos = meContent.rett() >> 32;
					ypos = meContent.rett() & 0xffffffff;
				}

				break;
			}
		default:
			{
				xpos = meContent.rett() >> 32;
				ypos = meContent.rett() & 0xffffffff;
			}
		}

		it->second->SetMapID(meContent.rets());
		it->second->SetXPos(xpos);
		it->second->SetYPos(ypos);
		it->second->SetXNewPos();
		it->second->SetYNewPos();
		it->second->SetDataFlag(eBaseInfo);
		it->second->SetDataFlag(eSkillInfo);

		SaveDataInfo sendData;
		sendData.content = toGT;
		sendData.message = message;
		sendData.msgtype = Request;
		sendData.msgid   = MSG_REQ_GM2GT_PLAYER_ONLINE;
		sendData.m_act   = act;
		it->second->SaveAllPlayer(sendData,true,false);

		Smart_Ptr<Player> myself;
		it->second->SetMyself(myself);
		m_allPlayer.erase(it);
	}
}

void PlayerMessage::RemoveDelayPlayer(void* arg)
{
	DWORD64 ntime = CUtil::GetNowSecond();
	DWORD64 ndeteTime = 0;

	INT64PLAYERCACHEMAP::iterator ite = m_PlayerCache.begin();
	for (; ite != m_PlayerCache.end();)
	{
		if(ite->second.bFlag)
		{
			ndeteTime = 60 * 1000;
		}
		else
		{
			ndeteTime = 15 * 1000;
		}

		DWORD64 deftime = ntime - ite->second.time;
		if (deftime > ndeteTime)
		{
			Smart_Ptr<Player> player = ite->second.playerCache;

			player->SetOffLineTime(ntime);

			LOG_ERROR(FILEINFO, "remove delay nowtime[%lld], oldtime[%lld], player[%lld] and gameserver request gt exit", ntime, ite->second.time, GET_PLAYER_CHARID(player->GetID()));
			if(player->GetType() == ePlayer)
			{
				//退出除副本以外的信息
				PlayerRemoveMemberEventArgs tArgs(player,GROUP_REMOVEMETHOD_OUT);
				FireGlobalEvent(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, tArgs);
				player->FireEvent(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, tArgs);
				SaveDataInfo sendData;
				ite->second.playerCache->SaveAllPlayer(sendData,true,true);

				//通知GT玩家下线
				ServerReturn::ServerRetInt sim;
				sim.set_ret(player->GetID());

				Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2GT_PLAYER_EXIT, &sim, player->GetChannelID(), SimpleMessage);
				Message_Facade::Send(messRet);

				Smart_Ptr<Player> myself;

				ite->second.playerCache->SetMyself(myself);
				m_PlayerCache.erase(ite++);
			}
			else
			{
				Smart_Ptr<Player> myself;

				ite->second.playerCache->SetMyself(myself);
				m_PlayerCache.erase(ite++);
			}
		}
		else
		{
			++ite;
		}
	}

}

void PlayerMessage::FlushPlayerFightState(void* arg)
{
	int64 ntime = CUtil::GetNowSecond();

	INT64PLAYERMAP::iterator it = m_allPlayer.begin();
	for (; it != m_allPlayer.end(); ++it)
	{
		it->second->UpdateFightState(ntime);
	}
}

void PlayerMessage::FlushPlayerOnlineState(void* arg)
{
//	PlayerMessage::GetInstance()->FlushPlayerOnlineState();
	PlayerMessage::GetInstance()->ReportPlayerOnlineCountToMS();
}

void PlayerMessage::FlushPlayerOnlineStateS(void* arg)
{
	FlushPlayerOnlineState();
}

void PlayerMessage::FlushPlayerOnlineState()
{
	INT64PLAYERMAP::iterator it = this->m_allPlayer.begin();
	for(; it != this->m_allPlayer.end(); ++it)
	{
		if((bool) it->second)
		{
			it->second->getOnlineReward()->CalOnlineReward();

			if(it->second->GetMagicWeapon()->GetMagicWeapon() <=0)
			{ //用于精确记录 神兵倒计时  只要 还没有 神兵 就 计算
				it->second->SetDataFlag(eMagicWeapon);
			}
		}
	}

	INT64PLAYERCACHEMAP::iterator cacheIt = this->m_PlayerCache.begin();
	for(; cacheIt != this->m_PlayerCache.end(); ++cacheIt)
	{
		if((bool) cacheIt->second.playerCache)
		{
			cacheIt->second.playerCache->getOnlineReward()->CalOnlineReward();

			if(cacheIt->second.playerCache->GetMagicWeapon()->GetMagicWeapon() <=0)
			{ //用于精确记录 神兵倒计时  只要 还没有 神兵 就 计算
				cacheIt->second.playerCache->SetDataFlag(eMagicWeapon);
			}
		}
	}
}

void PlayerMessage::ReportPlayerOnlineCountToMS()
{
	Smart_Ptr<SvrItem> magager_ptr = ServerConHandler::GetInstance()->GetManagerServerItem();
	if(magager_ptr )
	{
		ServerReturn::ServerDoubleInt toMS;

		toMS.set_retf(ServerConHandler::GetInstance()->GetLocalID());
		int count = 0;
		INT64PLAYERMAP::iterator it = m_allPlayer.begin();
		for(;it!=m_allPlayer.end();++it)
		{
			if(eAccountType_User == it->second->GetAccountType())
				++count;
		}
		toMS.set_rets(count);
		printf("\n  向管理 服 报告 在线人数[%u]\n",count);

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2MS_REPORTONLINECOUNT, &toMS, magager_ptr.Get(), SimpleMessage);
		Message_Facade::Send(messRet);
	}
}


DEF_MSG_ACK_DEFINE_FUN(PlayerMessage, MSG_REQ_GM2DB_CONTAINERINFO)
{
	if (message->GetErrno() == eReqTimeOut)
	{
		LOG_DEBUG(FILEINFO, "gameserver request dbserver player[%lld] container info and ack timeout", GET_PLAYER_CHARID(static_cast<commonAct *>(act.Get())->mesDataf));

		return;
	}

	LOG_DEBUG(FILEINFO, "gameserver request dbserver player[%lld] container info", GET_PLAYER_CHARID(static_cast<commonAct *>(act.Get())->mesDataf));

	PlayerInfo::ContainerInfo meContent;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	INT64PLAYERMAP::iterator it = m_allPlayer.find(static_cast<commonAct *>(act.Get())->mesDataf);
	if(it != m_allPlayer.end())
	{
		player = it->second;
	}
	else
	{
		INT64PLAYERCACHEMAP::iterator itCache = m_PlayerCache.find(static_cast<commonAct *>(act.Get())->mesDataf);
		if(itCache != m_PlayerCache.end())
		{
			player = itCache->second.playerCache;
		}
		else
		{
			LOG_ERROR(FILEINFO, "get player[charid=%lld] container info but player is not existed", GET_PLAYER_CHARID(static_cast<commonAct *>(act.Get())->mesDataf));

			return;
		}
	}


	player->GetContainer(ePackageType)->SetArticleInfo(&meContent);

	player->GetContainer(eStoreHouseType)->SetArticleInfo(&meContent);

	CharLogin::GMGetWSRequestData tempWSMsg;
	getWBaseInfo(*tempWSMsg.mutable_info(),player,true);

	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),worldSvr);
	Safe_Smart_Ptr<CommBaseOut::Message> tempMessRet = build_message(MSG_REQ_GM2WS_GET_WS_DATA,&tempWSMsg,worldSvr.Get(),Request);

	tempMessRet->SetAct(act);
	Message_Facade::Send(tempMessRet);

	return ;
}

DEF_MSG_SIMPLE_DEFINE_FUN(PlayerMessage, MSG_SIM_GT2GM_REVIVAL)
{
	CharBattle::ClientReqRevival meContent;
	CharBattle::RevivalData req;

	BYTE revivalType = 0;
	int64 xpos = 0;
	int64 ypos = 0;
	int mapid = 0;
	char *con;
	int len = 0;
	int64 sceneid = -1;
	int64 reliveMapId = -1;
	Smart_Ptr<Player> player;
	bool isErase = false;
	int mapType = -1;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	INT64PLAYERMAP::iterator it = m_allPlayer.find(message->GetMessageTime());
	if (it != m_allPlayer.end())
	{
		player = it->second;
		isErase = true;
	}
	else
	{
		INT64PLAYERCACHEMAP::iterator itCache = m_PlayerCache.find(message->GetMessageTime());
		if (itCache != m_PlayerCache.end())
		{
			player = itCache->second.playerCache;
		}
		else
		{
			LOG_ERROR(FILEINFO, "player[charid=%lld] revival but player is not existed", GET_PLAYER_CHARID(message->GetMessageTime()));
			return;
		}
	}

	Smart_Ptr<Object> obj = player;
	if(player.Get()==NULL || !player->IsDead())
	{
			return;
	}

	if(GET_MAP_TYPE(player->GetMapID()) == ePVEMap || GET_MAP_TYPE(player->GetMapID()) == eBeyondMap)
	{//竞技场不需复活
		unsigned int instanceIndex = GET_MAP_ID(player->GetMapID());
		InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(instanceIndex);
		if (!entry)
		{
			return ;
		}

		mapType = entry->m_eType;
		if(entry->m_eType == eCompeteInstance || entry->m_eType == eBeyondSvrBattle)
			return;

		if(entry->m_eType == eClanBattle && meContent.type() != eRevialRandom)
			return;
	}

	switch (meContent.type())
	{
		case eRevivalCommon:		//复活点复活
		{
			if (GET_MAP_TYPE(player->GetMapID()) < eCopyMap)
			{
				return;
			}
			else			//副本
			{
				InstanceMapEntry *entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
				if(!entry)
						return;

				mapid = GET_MAP_ID(player->GetMapID());
				if(mapid > 0)
				{
					revivalType = eReMove;
					if(entry->m_eType == eCountryBoss)
					{
						GsCountryMgr::GetInstance()->GetRandomPos(xpos,ypos,player->getAttManage()->getValue<DWORD>(eCharCountry));
					}
					else if(entry->m_eType == eCountryWar)
					{
						CountryWarLoader::GetInstance()->GetRandomPos(xpos,ypos,player->getAttManage()->getValue<DWORD>(eCharCountry));
					}
					else if(entry->m_eType == eMarryInstance)
					{
						Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
						if(instance)
						{
							if(instance->GetParamfValue() == player->GetID())
							{
								MarryInstanceLoader::GetInstance()->GetBothPoint(xpos,ypos,true);
							}
							else
							{
								MarryInstanceLoader::GetInstance()->GetBothPoint(xpos,ypos,false);
							}
						}
					}
					else if(entry->m_eType == eIntegralInstance)
					{
						Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
						if(instance)
						{
							if(instance->GetParamfValue() == player->getAttManage()->getValue<DWORD>(eCharFaction))
							{
								ClanLoader::GetInstance()->GetRandomPos(xpos,ypos,true);
							}
							else
							{
								ClanLoader::GetInstance()->GetRandomPos(xpos,ypos,false);
							}
						}
					}
					else if(entry->m_eType == eClanBattle)
					{
						int64 txpos = player->GetXPos();
						int64 typos = player->GetYPos();
						Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
						if(instance)
						{
							if(instance->GetParamfValue() == player->getAttManage()->getValue<DWORD>(eCharFaction))
							{
								ClanLoader::GetInstance()->GetBirthPos(eCamp_Red,txpos,typos);
							}
							else
							{
								ClanLoader::GetInstance()->GetBirthPos(eCamp_Blue,txpos,typos);
							}

							xpos = txpos;
							ypos = typos;
						}
					}
					else if(entry->m_eType == eCountryOverlord)
					{
						Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
						if(instance)
						{
							CountryOverlordCarbonLoader::GetInstance()->GetBothPos(player->getAttManage()->getValue<DWORD>(eCharCountry),xpos,ypos);
						}
					}
					else
					{
						xpos = entry->m_nXPos;
						ypos = entry->m_nYPos;
					}
				}else
				{
					return;
				}
			}
			break;
		}
		case eRevivalPlace:		//原地复活
		{
			const int revivalItemId = GameParaLoader::GetInstance()->getCostItemRevival();
			//扣除复活道具
			BaseArgs aliveArg;
			aliveArg.GoodsID = revivalItemId;
			aliveArg.Num = 1;
			aliveArg.Pos = -1;

			if(0 != player->GetContainer(ePackageType)->DeleteGoods(&aliveArg))
			{
				return ;
			}

			req.set_consumeflag(true);

//			const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(revivalItemId);
//			if(!ginfo)
//			{
//				//统计复活消耗道具
//				StatisticMgr::GetInstance()->StatisticPlayerGetItems( player, eStatic_Charactor,revivalItemId,1,ginfo->price);
//			}
			break;
		}

		case eRevivalToCity:		//回城复活
		{
			if(GET_MAP_TYPE(player->GetMapID()) >= eCopyMap)
			{
				Smart_Ptr<CGameDuplicate> newInstance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());

				if(!newInstance)
					return;

				if(newInstance)
				{
					if(newInstance->GetInstanceScript()->m_eType != eBeyondSvrBattle)
					{
						newInstance->LeaveExame(player);
					}

					OldMapInfo *info =  newInstance->GetOldMapPoint(player->GetID());
					if(info==NULL)
					{
						return;
					}

					sceneid = info->mapid;
					mapid = GET_MAP_ID(sceneid);
					xpos = info->xpos;
					ypos = info->ypos;
				}

				//这里要判断当前服有没有这个地图
				revivalType = eReLeave;
				if(!CSceneMgr::GetInstance()->IsHaveScene(sceneid))
				{
					revivalType = eReChangeMap;
				}
				break;
			}
			else		//不是副本的回城复活
			{
				MapStaticInfo *sMapInfo = CMapConfig::GetInstance()->GetStaticInfo( GET_MAP_ID(player->GetMapID()));
				if(sMapInfo == 0)
				{
					LOG_ERROR(FILEINFO, "static map[id=%d] into is error", GET_MAP_ID(player->GetMapID()));
					return;
				}

				reliveMapId = CSceneMgr::GetInstance()->GetSceneIDAboutPlayerCount(sMapInfo->reliveMap);
				sceneid = reliveMapId;
				mapid = sMapInfo->reliveMap;
				xpos = sMapInfo->reliveXPos;
				ypos = sMapInfo->reliveYPos;

				if(reliveMapId == player->GetMapID())
				{
					revivalType = eReMove;
				}
				else
				{
					//这里要判断当前服有没有这个地图
					revivalType = eReLeave;
					if(!CSceneMgr::GetInstance()->IsHaveScene(sceneid))
					{
							revivalType = eReChangeMap;
					}
				}
				break;
			}
			break;
		}

		case eRevialRandom:			//竞技场复活
		{
			if(GET_MAP_TYPE(player->GetMapID()) == ePVEMap)
			{//竞技场不需复活
				unsigned int instanceIndex = GET_MAP_ID(player->GetMapID());
				InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(instanceIndex);
				if (!entry)
				{
					return ;
				}

				if(entry->m_eType != eClanBattle)
					return;

				int64 txpos = player->GetXPos();
				int64 typos = player->GetYPos();
				Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
				if(instance.Get()==NULL)
					return;

				if(instance->GetParamfValue() == player->getAttManage()->getValue<DWORD>(eCharFaction))
				{
					ClanLoader::GetInstance()->GetBirthPos(eCamp_Red,txpos,typos);
				}
				else
				{
					ClanLoader::GetInstance()->GetBirthPos(eCamp_Blue,txpos,typos);
				}

				mapid = GET_MAP_ID(player->GetMapID());
				xpos = txpos;
				ypos = typos;
				revivalType = eReMove;
			}
			else
			{
				return;
			}
			break;
		}
		default:
		{
			return;
		}
	}

	switch(revivalType)
	{
		case eReMove:				//同地图复活
		{
			player->SetXNewPos(xpos);
			player->SetYNewPos(ypos);
			player->ClearPath();
			player->UnsetCreatureState(ERS_Dead);
			player->SetDataFlag(eBaseInfo);

			if(!CSceneMgr::GetInstance()->MoveTo(obj))
			{
				LOG_ERROR(FILEINFO, "player[gs charid=%lld] move  failed ", GET_PLAYER_CHARID(player->GetID()));
			}

			Smart_Ptr<GirlWar> girlWar =player->GetGirlWarManager()->GetGirlWar();
			if(girlWar)
			{
				girlWar->RelocatePos(xpos,ypos);
			}

			req.set_mapid(mapid);
			req.set_xpos(xpos);
			req.set_ypos(ypos);
			break;
		}
		case eReLeave:				//不同地图复活
		{
			CSceneMgr::GetInstance()->LeaveScene(obj);

			player->SetMapID(sceneid);
			player->SetXPos(xpos);
			player->SetYPos(ypos);
			player->SetXNewPos();
			player->SetYNewPos();
			player->SetDataFlag(eBaseInfo);

			PLayerCache cache;
			cache.playerCache = player;
			cache.time = CUtil::GetNowSecond();

			m_PlayerCache[player->GetID()] = cache;

			if(isErase)
			{
				m_allPlayer.erase(it);
			}

			req.set_mapid(mapid);
			req.set_xpos(xpos);
			req.set_ypos(ypos);
			break;
		}
		case eReChangeMap:			//跨服复活
		{
			//根据场景id判断选择何种方式切换传送地图,sceneid>0从哪来回那去，否则自动分配人数最少的地图
			int64 sendID=mapid;
			int sendType=eMapType;
			if(sceneid>0)
			{
					sendID = sceneid;
					sendType = eSceneType;
			}

			ServerReturn::ServerThreeInt toGate;
			toGate.set_retf(player->GetID());
			toGate.set_rets(sendID);
			toGate.set_rett((((int64)xpos << 48) | ((int64)ypos << 32)) | sendType);

			//夸服时，必须原地复活，要不然客户端会先client in 新的地图，然后不处理强制换图信息
			//req.set_mapid(mapid);
			//req.set_xpos(xpos);
			//req.set_ypos(ypos);

			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GM2GT_CHANGEMAP, &toGate, player->GetChannelID(), Request);
			Message_Facade::Send(messRet);
			break;
		}
		default:
		{
		}
	}

	if(meContent.type()==eRevivalCommon)
	{
		if(GET_MAP_TYPE(player->GetMapID()) == ePVEMap && mapType == eGroupInstance)
		{
			player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax) / 10);
			player->SetCurMP(player->getAttManage()->getValue<int>(eCharMPMax) / 10);
		}
		else if(GET_MAP_TYPE(player->GetMapID()) == ePVEMap && mapType == ePVP)
		{
			player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));
			player->SetCurMP(player->getAttManage()->getValue<int>(eCharMPMax));
			Smart_Ptr<CreatureObj> obj = player->GetMyself();
			player->AddBuff(PVPMgr::GetInstance()->getProtectedBufferID(), obj);
		}
		else
		{
			//血量和蓝量改成20%
			player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax) * 2 / 10);
			player->SetCurMP(player->getAttManage()->getValue<int>(eCharMPMax) * 2 / 10);
		}
	}
	else
	{
		//血量和蓝量全满
		player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));
		player->SetCurMP(player->getAttManage()->getValue<int>(eCharMPMax));
	}

	player->UnsetCreatureState(ERS_Dead);
	player->SetDataFlag(eBaseInfo);

	req.set_key(player->GetKey());
	req.set_hp(player->getAttManage()->getValue<int>(eCharHP));
	req.set_mp(player->getAttManage()->getValue<int>(eCharMP));

	MapStaticInfo * sMapInfo = CMapConfig::GetInstance()->GetStaticInfo(req.mapid());
	if(sMapInfo != NULL)
	{
		int type = player->GetPkTypeByMap(sMapInfo->pktype);
		req.set_pktype(type);
		player->getAttManage()->setValue(ePKType, type);
	}
	else
	{
		req.set_pktype( player->getAttManage()->getValue<int>(ePKType) );
	}

	player->SendSynchMessage(&req, MSG_SIM_GT2C_REVIVAL);
	CSceneMgr::GetInstance()->SendSynchMessage(&req, MSG_SIM_GT2C_REVIVAL, player->GetMapID(), player->GetKey(), player->GetXNewPos(), player->GetYNewPos(), false);
}

DEF_MSG_REQUEST_DEFINE_FUN(PlayerMessage, MSG_REQ_GT2GM_SEEOTHERPLAYERINFO)
{
	LOG_DEBUG(FILEINFO,"gateserver request gameserver see friend info");

//	ServerReturn::ServerDoubleInt Ret;
	CharLogin::ReqSeePlayerInfo req;
	int len = 0;
	char * content = message->GetBuffer(len);
	req.ParseFromArray(content, len);

	Smart_Ptr<Player> SelfPlayer;
	Smart_Ptr<Player> OtherPlayer;
	int64 charID = message->GetMessageTime();

	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, SelfPlayer);
	if((bool)SelfPlayer)
	{
		int64 otherID = req.charid();
		PlayerMessage::GetInstance()->GetPlayerByCharid(otherID, OtherPlayer);
		//如果为真表明该玩家在该游戏上面
		if((bool)OtherPlayer)
		{
			CharLogin::SeePlayerInfo ack;
			ack.set_charid(OtherPlayer->GetID());
			ack.set_charname(OtherPlayer->GetName());
			ack.set_profession(OtherPlayer->getAttManage()->getValue<BYTE>(eCharProfession));
			ack.set_sex(OtherPlayer->GetSex());
			ack.set_exp(OtherPlayer->getAttManage()->getValue<int64>(eCharExp));
			ack.set_lv(OtherPlayer->getAttManage()->getValue<int>(eCharLv));
			ack.set_guildname(OtherPlayer->GetClanName());
			ack.set_guildjob(OtherPlayer->getAttManage()->getValue<BYTE>(eCharFaJob));
			ack.set_titleid(OtherPlayer->getAttManage()->getValue<int>(eTitleID));
			ack.set_hp(OtherPlayer->getAttManage()->getValue<int>(eCharHP));
			ack.set_hpmax(OtherPlayer->getAttManage()->getValue<int>(eCharHPMax));
			ack.set_mp(OtherPlayer->getAttManage()->getValue<int>(eCharMP));
			ack.set_mpmax(OtherPlayer->getAttManage()->getValue<int>(eCharMPMax));
			ack.set_strength(OtherPlayer->getAttManage()->getValue<int>(eCharStrength));
			ack.set_cleverpower(OtherPlayer->getAttManage()->getValue<int>(eCleverPower));
			ack.set_cleverair(OtherPlayer->getAttManage()->getValue<int>(eCleverAir));
			ack.set_physicalpower(OtherPlayer->getAttManage()->getValue<int>(eCharPhysicalPower));
			ack.set_patience(OtherPlayer->getAttManage()->getValue<int>(eCharPatience));
			ack.set_agility(OtherPlayer->getAttManage()->getValue<int>(eCharAgility));
			ack.set_pkcount(OtherPlayer->getAttManage()->getValue<int>(ePKCount));
			ack.set_power(OtherPlayer->getAttManage()->getValue<int>(ePower));
			ack.set_powermax(OtherPlayer->getAttManage()->getValue<int>(ePowerMax));
			ack.set_phyattackmax(OtherPlayer->getAttManage()->getValue<int>(eCharPhyAttackMax));
			ack.set_phydefence(OtherPlayer->getAttManage()->getValue<int>(eCharPhyDefence));
			ack.set_hitrate(OtherPlayer->getAttManage()->getValue<int>(eCharHitRate));
			ack.set_avoidrate(OtherPlayer->getAttManage()->getValue<int>(eCharAvoidRate));
			ack.set_crackdownrate(OtherPlayer->getAttManage()->getValue<int>(eCrackDownRate));
			ack.set_baserte(OtherPlayer->getAttManage()->getValue<int>(eAvoidCrackRate));
			ack.set_fight(OtherPlayer->getAttManage()->getValue<int>(eFightPower));
			ack.set_viplv(OtherPlayer->getAttManage()->getValue<int>(eVIPLv));
			//添加武器模型ID，服装ID，神兵ID

			if(OtherPlayer->GetMagicWeapon())
			{
				int magicid = OtherPlayer->GetMagicWeapon()->GetMagicWeapon();
				ack.set_magicequipid(magicid);
			}

			PlayerInfo::EquipQuickInfo eqInfo;
			Smart_Ptr<ArticleEquipMgr> mgr = OtherPlayer->GetContainer(eEquipQuickType);
			if(!mgr)
			{
				return;
			}
			mgr->GetArticleInfo(&eqInfo);
			for(int i = 0; i < eqInfo.equip_size(); i++)
			{
				PlayerInfo::ItemInfo *itemInfo = ack.add_equip();
				PlayerInfo::ItemInfo *dbitemInfo = eqInfo.mutable_equip(i);
				itemInfo->CopyFrom(*dbitemInfo);
			}
			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_SEEOTHERPLAYERINFO, message, &ack);
			Message_Facade::Send(messRet);
		}
		//如果该游戏服没有该玩家数据,那么就去DB上面找
		else
		{
			ServerReturn::ServerRetInt ReqDB;

//			ReqDB.set_ret(Ret.rets());
			ReqDB.set_ret(otherID);

			Safe_Smart_Ptr<CommBaseOut::Message> dbmess = build_message(MSG_REQ_GM2DB_SEEOTHERPLAYERINFO, &ReqDB, SelfPlayer->GetDBChannelID(), Request);
			dbmess->SetAct(new requestAct(message));
			Message_Facade::Send(dbmess);
		}
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request see other player but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
	}
}

DEF_MSG_ACK_DEFINE_FUN(PlayerMessage, MSG_REQ_GM2DB_SEEOTHERPLAYERINFO)
{
	if (message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request dbserver get other player info and ack timeout");
		return;
	}

	LOG_DEBUG(FILEINFO, "gameserver request dbserver get other player info and ack from db");

	CharLogin::SeePlayerInfo meContent;
	char *con;
	int len = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_SEEOTHERPLAYERINFO, static_cast<requestAct *>(act.Get())->mesReq, &meContent);
	Message_Facade::Send(messRet);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(PlayerMessage,MSG_REQ_C2GT_CHANGEPKTYPE)
{
	CharBattle::ChangePkType PkType;
	ServerReturn::ServerDoubleInt toGate;
	int res = 0;

	PkType.ParseFromString(content);
	if (!player)
	{
		return;
	}

	if(player->GetGameZone() == NULL)
	{
		return;
	}

	if(player->GetGameZone()->isCanDoing(eDoingSwitchPKMode,player->GetXNewPos(),player->GetYNewPos()))
	{
		MapStaticInfo * mapInfo = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(player->GetMapID()));
		if (mapInfo->pktype == neutral)
		{
			if (PkType.type() >= sPeace && PkType.type() <= sTeam)
			{
				if(PkType.type() == sTeam && player->GetGroupID() <= 0 )
				{
					toGate.set_retf(eNoTeamNotchangePk);
				}
				else
				{
					player->getAttManage()->setValue(ePKType,PkType.type());
					toGate.set_retf(res);
					player->SetOldPkType(PkType.type());
				}
			}
			else
			{
				toGate.set_retf(eClientSendDataError);
			}
		}
		else
		{
			toGate.set_retf(eChangePkType);
		}
	}
	else
	{
		toGate.set_retf(eRegionNotSwitchPKMode);
	}

	toGate.set_rets(PkType.type());
	REQ_MSG_QUICK_ACK_FUN(message, &toGate, MSG_REQ_C2GT_CHANGEPKTYPE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(PlayerMessage,MSG_REQ_GET_ATTR)
{
	ServerReturn::ServerRetInt32Array req;
	CharBase::CharAttChange toGM;

	req.ParseFromString(content);

	if(!bool(player))
	{
		return;
	}

	size_t size = req.ret_size();
	if(size <= 0)
	{
		return;
	}



	for (size_t i = 0; i < size; ++i)
	{
		toGM.add_type(req.ret(i));
		toGM.add_value(player->GetTypeValue(req.ret(i)));
		toGM.add_state(eAttackSim);
	}

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(), &toGM, MSG_REQ_GET_ATTR);
	REQ_MSG_QUICK_ACK_FUN(message, &toGM, MSG_REQ_GET_ATTR);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(PlayerMessage,MSG_REQ_C2GM_GATHER)
{
	CharBattle::ReqGather req;
	CharBattle::RetGather toClient;
	ServerMessage::ClientSingleMessage ack;

	req.ParseFromString(content);

	Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(player->GetMapID());
	if (!zone)
	{
		return;
	}

	Smart_Ptr<CreatureObj> obj;

	zone->GetObject(req.monsterid(), obj);
	if (!obj)
	{
		return;
	}

	int error = 0;
	if (obj->GetType() == eMonster)
	{
		Smart_Ptr<Monster> mon = obj;
		MonsterInfo info;

		MonsterLoader::GetInstance()->GetMonsterInfo(mon->GetID(),info);
		if (info.id == -1)
			return;

		if(CSceneMgr::GetInstance()->GetDistance(player->GetXNewPos(), player->GetYNewPos(), mon->GetXNewPos(), mon->GetYNewPos()) > STARTPOINT_LENGTH)
		{
			error = eOutOfRange;
		}
		else
		{
			//是否为采集怪;是否有对应的任务
			if ((mon->GetQuality() == 4 && player->GetQuestMgr()) || mon->GetQuality() == 15)
			{
				mon->EndGather(player, error);
			}
			else
				error = eGatherIdError;
		}
	}
	else
	{
		error = eGatherIdError;
	}

	toClient.set_dwerror(error);

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_GATHER);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_GATHER);
}

//DEF_MSG_QUICK_SIMPLE_DEFINE_FUN(PlayerMessage,MSG_SIM_C2GM_CHANGEMAPLINE)
//{
//	ServerReturn::ServerRetInt req;
//	req.ParseFromString(content);
//
//
//}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(PlayerMessage,MSG_REQ_C2GM_FIYING_SHOES)
{
	ServerReturn::ServerFourInt fiyingShoes;
	ServerReturn::ServerRetInt toClient;
	fiyingShoes.ParseFromString(content);
	int ret = 0;

	if(!player)
	{
		LOG_ERROR(FILEINFO, "cannt find player");
		return;
	}

	if(player->IsInCreatureState(ERS_Escort))
	{
		toClient.set_ret(eIsEscorting);
	}
	//目标地图能否传送
	else if((ret = isCanFly(fiyingShoes.retf(),fiyingShoes.rets(), fiyingShoes.rett(),player->getAttManage()->getValue<int>(eCharLv)))!= 0)
	{
		toClient.set_ret(ret);
	}
	else
	{
		if(player->getAttManage()->getValue<int>(eCharHP) > 0)
		{
			bool notneedFlyShoes = false;
			if(player->GetVipPart())
			{
				if(VIP_NOT_HAVE_THIS_RIGHT != player->GetVipPart()->GetValueByRight(eVipFlyShoesInfinet))
				{
						notneedFlyShoes = true;
				}
			}

			if(fiyingShoes.retfo() == GameParaLoader::GetInstance()->getShoesNpc())
			{
				notneedFlyShoes = true;
			}

			BaseArgs flyArg;

			const int flyItemId = GameParaLoader::GetInstance()->getFly();

			flyArg.GoodsID = flyItemId;
			flyArg.Num = 1;
			flyArg.Pos = -1;

			if(notneedFlyShoes|| 0 == player->GetContainer(ePackageType)->DeleteGoods(&flyArg))
			{
				//是否在9宫格内
				if( isInSameGroupSector(player,fiyingShoes.retf(),fiyingShoes.rets(), fiyingShoes.rett()) )
				{
					ChangeInSameGroup(player,fiyingShoes.retf(),fiyingShoes.rets(), fiyingShoes.rett());
				}
				else
				{
					if(1 == FastChangeMap(player, fiyingShoes.retf(),fiyingShoes.rets(), fiyingShoes.rett()))
					{
						MapStaticInfo *info;
						ChangeMap::GMForceClientChangeMap toClient;

						info = CMapConfig::GetInstance()->GetStaticInfo(fiyingShoes.retf());
						toClient.set_mapid(fiyingShoes.retf());
						toClient.set_xpos(fiyingShoes.rets());
						toClient.set_ypos(fiyingShoes.rett());

						toClient.set_pktype(player->GetPkTypeByMap(info->pktype));

						QuickMessage::GetInstance()->AddSendMessage( player->GetID(), player->GetChannelID(), &toClient, MSG_SIM_GM2C_FORCE_CHANGEMAP);
					}
				}

				toClient.set_ret(0);
			}
			else
			{
				toClient.set_ret(eMarketShopNumNotEnough);
			}

			if (!notneedFlyShoes)
			{
				//统计使用飞鞋次数
				//StatisticMgr::GetInstance()->StatisticPlayerGetItems(player,eStatic_Map, eStaMinor_Map_UseFlyShoe,flyItemId,1);
			}
		}
		else
		{
			toClient.set_ret(eIntanceMemberDead);
		}
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_FIYING_SHOES);
}


DEF_MSG_SIMPLE_DEFINE_FUN(PlayerMessage, MSG_SIM_WS2GM_NOTICESELLERUI)
{
	MarketMessInfo::WSToGMSellerUIState req;
	int len = 0;
	char* buffer = message->GetBuffer(len);
	Smart_Ptr<Player> player;

	req.ParseFromArray(buffer, len);

	PlayerMessage::GetInstance()->GetPlayerByCharId(player, req.charid());

	if ((bool)player)
	{
		player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Market, eUIHLSmall_Market_HaveSellItem), 1, true);
	}
}

bool PlayerMessage::PlayerLogout(Smart_Ptr<Player> pObj)
{
	if(pObj && pObj->GetType() == ePlayer)
	{
		BEHAVIOR_MONI(pObj, BehaviorType::Main::eRole, BehaviorType::Minor::eRole_Offline, MONITOR);

		BEHAVIOR_HANDLE_DO(pObj, BehaviorHandType::OfflineRes(CUtil::GetNowSecond() - pObj->GetLoginTime()))

		
		//退出除副本以外的信息
		PlayerRemoveMemberEventArgs tArgs(pObj,GROUP_REMOVEMETHOD_OUT);
		FireGlobalEvent(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, tArgs);
		pObj->FireEvent(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, tArgs);

		//下线 就 退出 队伍  由于玩家 退出 队伍时  要在 管理类中 作 相应 操作  所以 不再 这 直接  触发事件
		sNewGroupMgr->LeaveGroup(pObj);
	}

	return true;
}

void PlayerMessage::ChangeInSameGroup(Smart_Ptr<Player> &player, int id, int xPos, int yPos)
{
		if(id < 0 || !player)
		{
			LOG_ERROR(FILEINFO, "ChangeInSameGroup error: id<0 or null player ");
			return ;
		}

		int64 sceneid = 0;

		MapStaticInfo *info = CMapConfig::GetInstance()->GetStaticInfo(id);
		if(!info)
		{
			LOG_ERROR(FILEINFO, "ChangeInSameGroup error: map id=%d, player=%lld", id, GET_PLAYER_CHARID(player->GetID()));
			return ;
		}

		//取消打坐状态
		player->CanccelMeditation();

		sceneid =CSceneMgr::GetInstance()->GetSceneIDAboutPlayerCount(info->mapid);

		if(sceneid > 0)
		{
				Smart_Ptr<Object> obj = player;
				player->SetXNewPos(xPos);
				player->SetYNewPos(yPos);
				player->SetDataFlag(eBaseInfo);

				player->ClearPath();

				if (!CSceneMgr::GetInstance()->MoveToAll(obj))
				{
						LOG_ERROR(FILEINFO, "player[gs charid=%lld] move  failed ", GET_PLAYER_CHARID(player->GetID()));
				}

				ChangeMap::GMForceClientChangeMap toClient;

				toClient.set_mapid(GET_MAP_ID(sceneid));
				toClient.set_xpos(xPos);
				toClient.set_ypos(yPos);

				toClient.set_pktype(player->GetPkTypeByMap(info->pktype));

				QuickMessage::GetInstance()->AddSendMessage( player->GetID(), player->GetChannelID(), &toClient, MSG_SIM_GM2C_FORCE_CHANGEMAP);

				Smart_Ptr<GirlWar> girlWar =player->GetGirlWarManager()->GetGirlWar();
				if(girlWar)
				{
						girlWar->RelocatePos(xPos,yPos);
				}
		}

		return ;
}

void PlayerMessage::ForceChangeMap(Smart_Ptr<Player> &player, int64 sceneid,int xpos, int ypos)
{
	if (sceneid < 0 || !player)
		return;

	if (player->IsInCreatureState(ERS_Escort))
	{
		return;
	}

	MapStaticInfo *info = NULL;
	info = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(sceneid));
	if (!info)
	{
		return;
	}

	if (CSceneMgr::GetInstance()->IsHaveScene(sceneid))
	{
		Smart_Ptr<Object> obj = player;

		if(sceneid != player->GetMapID())
		{
			CSceneMgr::GetInstance()->LeaveScene(obj);
			player->SetMapID(sceneid);
			player->SetXPos(xpos);
			player->SetYPos(ypos);
			player->SetXNewPos();
			player->SetYNewPos();
			player->SetDataFlag(eBaseInfo);

			PLayerCache cache;
			cache.playerCache = player;
			cache.time =	CUtil::GetNowSecond();
			m_PlayerCache[player->GetID()] = cache;

			m_allPlayer.erase(player->GetID());

			ChangeMap::GMForceClientChangeMap toClient;

			toClient.set_mapid(GET_MAP_ID(sceneid));
			toClient.set_xpos(xpos);
			toClient.set_ypos(ypos);

			toClient.set_pktype(player->GetPkTypeByMap(info->pktype));

			QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_SIM_GM2C_FORCE_CHANGEMAP);
		}
		else
		{
			player->SetXNewPos(xpos);
			player->SetYNewPos(ypos);
			player->SetDataFlag(eBaseInfo);
			player->ClearPath();

			if (!CSceneMgr::GetInstance()->MoveTo(obj))
			{
				LOG_ERROR(FILEINFO, "player[gs charid=%lld] move  failed ", GET_PLAYER_CHARID(player->GetID()));
			}

			ChangeMap::GMForceClientChangeMap toClient;

			toClient.set_mapid(GET_MAP_ID(sceneid));
			toClient.set_xpos(xpos);
			toClient.set_ypos(ypos);

			toClient.set_pktype(player->GetPkTypeByMap(info->pktype));

			QuickMessage::GetInstance()->AddSendMessage( player->GetID(), player->GetChannelID(), &toClient, MSG_SIM_GM2C_FORCE_CHANGEMAP);

			Smart_Ptr<GirlWar> girlWar =player->GetGirlWarManager()->GetGirlWar();
			if(girlWar)
			{
				girlWar->RelocatePos(xpos,ypos);
			}
		}
	}
	else
	{
		Smart_Ptr<Player> myself;
		ServerReturn::ServerThreeInt toGate;

		toGate.set_retf(player->GetID());
		toGate.set_rets(sceneid);

		if(GET_MAP_TYPE(sceneid) == ePVEMap)
		{
			InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(sceneid));
			if(entry && entry->m_eType == eClanBattle)
			{
				toGate.set_rett((((int64)xpos << 48) | ((int64)ypos << 32)) | eBePointType);
			}
			else
			{
				toGate.set_rett((((int64)xpos << 48) | ((int64)ypos << 32)) | eSceneType);
			}
		}
		else
		{
			toGate.set_rett((((int64)xpos << 48) | ((int64)ypos << 32)) | eSceneType);
		}

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GM2GT_CHANGEMAP, &toGate, player->GetChannelID(), Request);
		Message_Facade::Send(messRet);

		return;
	}

}

//把玩家强制传送(必须在发送前验证x,y坐标和场景)--没有ACK返回
int PlayerMessage::FastChangeMap(Smart_Ptr<Player> &player, int id, int xPos, int yPos, bool activityNeed)
{
	if(id < 0 || !player)
	{
		LOG_ERROR(FILEINFO, "FastChangeMap error: map id=%d", id);
		return -1;
	}

	int64 sceneid = 0;
	int ret = 0;

	MapStaticInfo *info;
	info = CMapConfig::GetInstance()->GetStaticInfo(id);
	if(!info)
	{
		LOG_ERROR(FILEINFO, "FastChangeMap map static error: map id=%d", id);
		return -1;
	}

	if(player->IsInCreatureState(ERS_Escort))
	{
		LOG_ERROR(FILEINFO, "FastChangeMap state error");
		return -1;
	}

	//取消打坐状态
	player->CanccelMeditation();

	if(info->maptype == eCopyMap && !activityNeed)
	{
		LOG_ERROR(FILEINFO, "FastChangeMap error: eCopyMap && !activityNeed");
		return -1;
	}

	if(info->mapid > 0)
	{
		sceneid =CSceneMgr::GetInstance()->GetSceneIDAboutPlayerCount(info->mapid);

		//判断坐标
		if(!CMapConfig::GetInstance()->IsCanValidPos(info->mapid, xPos, yPos))
		{
			LOG_ERROR(FILEINFO, "\n  FastChangeMap 判断坐标错误 MapID[%d] xpos[%d]---- ypos[%d]\n",info->mapid, xPos, yPos);
			return -1;
		}

		if(sceneid > 0)
		{
			Smart_Ptr<Object> obj = player;

			if(sceneid != player->GetMapID())
			{
				CSceneMgr::GetInstance()->LeaveScene(obj);

				player->SetMapID(sceneid);
				player->SetXPos(xPos);
				player->SetYPos(yPos);
				player->SetXNewPos();
				player->SetYNewPos();
				player->SetDataFlag(eBaseInfo);

				PLayerCache cache;
				cache.playerCache = player;
				cache.time = CUtil::GetNowSecond();
				m_PlayerCache[player->GetID()] = cache;

				m_allPlayer.erase(player->GetID());
				ChangeMap::GMForceClientChangeMap toClient;

				toClient.set_mapid(GET_MAP_ID(sceneid));
				toClient.set_xpos(xPos);
				toClient.set_ypos(yPos);

				toClient.set_pktype(player->GetPkTypeByMap(info->pktype));

				QuickMessage::GetInstance()->AddSendMessage( player->GetID(), player->GetChannelID(), &toClient, MSG_SIM_GM2C_FORCE_CHANGEMAP);
			}
			else
			{
				player->SetXNewPos(xPos);
				player->SetYNewPos(yPos);
				player->SetDataFlag(eBaseInfo);

				player->ClearPath();

				if (!CSceneMgr::GetInstance()->MoveTo(obj))
				{
					LOG_ERROR(FILEINFO, "player[gs charid=%lld] move  failed ", GET_PLAYER_CHARID(player->GetID()));
				}

				ChangeMap::GMForceClientChangeMap toClient;

				toClient.set_mapid(GET_MAP_ID(sceneid));
				toClient.set_xpos(xPos);
				toClient.set_ypos(yPos);

				toClient.set_pktype(player->GetPkTypeByMap(info->pktype));

				QuickMessage::GetInstance()->AddSendMessage( player->GetID(), player->GetChannelID(), &toClient, MSG_SIM_GM2C_FORCE_CHANGEMAP);

				Smart_Ptr<GirlWar> girlWar =player->GetGirlWarManager()->GetGirlWar();
				if(girlWar)
				{
					girlWar->RelocatePos(xPos,yPos);
				}
			}
		}
		else
		{
			Smart_Ptr<Player> myself;
			ServerReturn::ServerThreeInt toGate;

			toGate.set_retf(player->GetID());
			toGate.set_rets(info->mapid);
			//toGate.set_rett(eMapType);
			toGate.set_rett((((int64)xPos << 48) | ((int64)yPos << 32)) | eMapType);

			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GM2GT_CHANGEMAP, &toGate, player->GetChannelID(), Request);
			//messRet->SetAct(new requestAct(message));
			Message_Facade::Send(messRet);

			return 1;
		}
	}
	else
	{
		ret = eMapIdError;
		LOG_ERROR(FILEINFO, "Transfer Map startpoint[%d] is error", id);
		return -1;
	}

	return 0;
}

void PlayerMessage::FastChangeMap(Safe_Smart_Ptr<CommBaseOut::Message> &message,
		Smart_Ptr<Player> &player, int id, int xPos, int yPos, bool activityNeed)
{
	if (id < 0 || !player)
		return;

	int64 sceneid = 0;
	int ret = 0;

//	map<int64, Smart_Ptr<Player> >::iterator it = m_allPlayer.find(charId);
//	if (it != m_allPlayer.end())
//	{
	MapStaticInfo *info;
	info = CMapConfig::GetInstance()->GetStaticInfo(id);
	if (!info)
	{
		return;
	}

	if (player->IsInCreatureState(ERS_Escort))
	{
		return;
	}

	if (info->maptype == eCopyMap && !activityNeed)
	{
		return;
	}

//	printf("\n  FastChangeMap MapID[%d] xpos[%d]---- ypos[%d]\n",
//					info->mapid, xPos, yPos);
	if (info->mapid > 0)
	{
		sceneid =
						CSceneMgr::GetInstance()->GetSceneIDAboutPlayerCount(
										info->mapid);
		//判断坐标
		if (!CMapConfig::GetInstance()->IsCanValidPos(
						info->mapid, xPos, yPos))
		{
			LOG_FATAL(FILEINFO, "\n  FastChangeMap 判断坐标错误 MapID[%d] xpos[%d]---- ypos[%d]\n",info->mapid, xPos, yPos);

			return;
		}

		if (sceneid > 0)
		{
			Smart_Ptr<Object> obj = player;

			if (sceneid != player->GetMapID())
			{
//				Smart_Ptr<GirlWar> girlWar =player->GetGirlWarManager()->GetGirlWar();
//				if(girlWar)
//				{
//					girlWar->Rest();
//				}

				CSceneMgr::GetInstance()->LeaveScene(obj);

				player->SetMapID(sceneid);
				player->SetXPos(xPos);
				player->SetYPos(yPos);
				player->SetXNewPos();
				player->SetYNewPos();
//				player->SetGroup(CSceneMgr::GetInstance()->GetSectorGroup(sceneid,player->GetXPos(),player->GetYPos()));
				player->SetDataFlag(eBaseInfo);

				PLayerCache cache;
				cache.playerCache = player;
				cache.time = CUtil::GetNowSecond();
				m_PlayerCache[player->GetID()] = cache;

				m_allPlayer.erase(player->GetID());
			}
			else
			{
				player->SetXNewPos(xPos);
				player->SetYNewPos(yPos);
				player->SetDataFlag(eBaseInfo);
				player->ClearPath();

				if (!CSceneMgr::GetInstance()->MoveTo(obj))
				{
					LOG_ERROR(FILEINFO, "player[gs charid=%lld] move  failed ", GET_PLAYER_CHARID(player->GetID()));
				}

				Smart_Ptr<GirlWar> girlWar =player->GetGirlWarManager()->GetGirlWar();
				if(girlWar)
				{
					girlWar->RelocatePos(xPos,yPos);
				}
			}
		}
		else
		{
			Smart_Ptr<Player> myself;
			ServerReturn::ServerThreeInt toGate;

			toGate.set_retf(player->GetID());
			toGate.set_rets(info->mapid);
			//toGate.set_rett(eMapType);
			toGate.set_rett((((int64)xPos << 48) | ((int64)yPos << 32)) | eMapType);

			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GM2GT_CHANGEMAP, &toGate, player->GetChannelID(), Request);
			messRet->SetAct(new requestAct(message));
			Message_Facade::Send(messRet);

			return;
		}
	}
	else
	{
		ret = eMapIdError;
		LOG_ERROR(FILEINFO, "Transfer Map startpoint[%d] is error", id);
		return;
	}

	ChangeMap::ChangeMapInfo ack;
	ack.set_mapid(sceneid);
	ack.set_pos((xPos << 16) | yPos);
	ack.set_charid(player->GetID());

	ack.set_pktype(player->GetPkTypeByMap(info->pktype));

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_CHANGEMAP, message, &ack);
	Message_Facade::Send(messRet);
}
//换线Paramf 场景ID或者地图ID Params 切图类型（或者包含坐标）列：(txpos << 48) | (typos << 32)) | eSceneType
void PlayerMessage::ChangeMapLine(Safe_Smart_Ptr<CommBaseOut::Message> &message,Smart_Ptr<Player> &player,int64 Paramf,int64 Params)
{
	ServerReturn::ServerThreeInt toGate;
	toGate.set_retf(player->GetID());
	toGate.set_rets(Paramf);
	toGate.set_rett(Params);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GM2GT_CHANGEMAP, &toGate, player->GetChannelID(), Request);
	messRet->SetAct(new requestAct(message));
	Message_Facade::Send(messRet);
}

void PlayerMessage::UpdateWorldBoss(int bossID, int isActive)
{
	ServerReturn::ServerDoubleInt toClient;
	toClient.set_retf(bossID);
	toClient.set_rets(isActive);
	INT64PLAYERMAP::iterator it = m_allPlayer.begin();
	for (; it != m_allPlayer.end(); it++)
	{
		if(it->second.Get()==NULL)
			continue;

//		it->second->SynchActiveHighLight();
		QuickMessage::GetInstance()->AddSendMessage(it->second->GetID(),
		                it->second->GetChannelID(), &toClient,
		                MSG_SIM_C2GT_UPDATEWORLDBOSSSTATUS);
	}
}

void PlayerMessage::UpdateDayCounter(vector<int> &vec)
{
	ServerReturn::ServerRetIntArray toClient;
	for (uint i = 0; i < vec.size(); ++i)
	{
		toClient.add_ret(vec[i]);
	}

	INT64PLAYERMAP::iterator it = m_allPlayer.begin();
	for (; it != m_allPlayer.end(); it++)
	{
//		it->second->SynchActiveHighLight();
		QuickMessage::GetInstance()->AddSendMessage(it->second->GetID(),
		                it->second->GetChannelID(), &toClient,
		                MSG_SIM_C2GT_UPDATEDAYCOUNTER);
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(PlayerMessage, MSG_REG_WS2GM_SYNCRANKINGREWARD)
{
	MapInstanceProto::SysRankingReward req;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;

	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);

	INT64PLAYERMAP::iterator it = m_allPlayer.find(req.retf());
	if(it != m_allPlayer.end())
	{
		if(it->second.Get()==NULL)
			return;

		player = it->second;
		player->getAttManage()->setValue(eAwardRanking,req.ret());

		// 通知客户端UI高亮
		player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Arena, eUIHLSmall_Arena_AwardUnGet), 1, true);

		//通知客户端更新
		DayUpdate::DayInfoUpdate info;
		info.add_info(eDayInfoUpade_CompleteGift);
		QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &info, MSG_REQ_GS2C_DAYUPDATE);
	}
}


void PlayerMessage::GetPlayerByCharId(Smart_Ptr<Player> &player, int64 charId)
{
	INT64PLAYERMAP::iterator it = m_allPlayer.find(charId);
	if (it != m_allPlayer.end())
	{
		player = it->second;
	}
}

//离开场景
void PlayerMessage::PlayerLeave(Safe_Smart_Ptr<CommBaseOut::Message> &message, Smart_Ptr<Player>& player, int64 sceneid, int xpos, int ypos,bool isOver)
{
	ChangeMap::ChangeMapInfo ack;

	if(!player)
		return;

	Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());

	Smart_Ptr<Object> obj = player;
	CSceneMgr::GetInstance()->LeaveScene(obj);

	player->SetMapID(sceneid);
	player->SetXPos(xpos);
	player->SetYPos(ypos);
	player->SetXNewPos();
	player->SetYNewPos();
	player->SetDataFlag(eBaseInfo);

	PLayerCache cache;
	cache.playerCache = player;
	cache.time = CUtil::GetNowSecond();
	m_PlayerCache[player->GetID()] = cache;

	m_allPlayer.erase(player->GetID());

	if(isOver)
	{
		if ((bool)instance)
		{
			instance->LeaveExame(player);
		}
		else
		{
			LOG_ERROR(FILEINFO, "instance is NULL");
		}
	}

	MapStaticInfo *sMapInfo = NULL;
	sMapInfo = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(sceneid));
	if(sMapInfo)
	{
		ack.set_pktype(player->GetPkTypeByMap(sMapInfo->pktype));
	}

	ack.set_mapid(sceneid);
	ack.set_pos((xpos << 16) | ypos);
	ack.set_charid(player->GetID());

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_CHANGEMAP, message, &ack);
	Message_Facade::Send(messRet);
}

//离开场景
void PlayerMessage::PlayerLeaveTask(Safe_Smart_Ptr<CommBaseOut::Message> &message, Smart_Ptr<Player>& player, int64 sceneid, int xpos, int ypos)
{
	if(!player)
		return;

	Smart_Ptr<Object> obj = player;
	CSceneMgr::GetInstance()->LeaveScene(obj);

	player->SetMapID(sceneid);
	player->SetXPos(xpos);
	player->SetYPos(ypos);
	player->SetXNewPos();
	player->SetYNewPos();
	player->SetDataFlag(eBaseInfo);

	PLayerCache cache;
	cache.playerCache = player;
	cache.time = CUtil::GetNowSecond();
	m_PlayerCache[player->GetID()] = cache;

	m_allPlayer.erase(player->GetID());

	MapStaticInfo *sMapInfo = NULL;
	sMapInfo = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(sceneid));
	int tPkType = 0;
	if(sMapInfo)
	{
		tPkType = player->GetPkTypeByMap(sMapInfo->pktype);
	}

	ServerReturn::ServerThreeInt toClient;

	int64 packRet = 0;
	packRet = ((int64)GET_MAP_ID(sceneid) << 32) | (xpos << 16 | ypos);

	toClient.set_retf(packRet);
	toClient.set_rets(0);
	toClient.set_rett(tPkType);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_SIM_C2GM_CHANGE_TASK_MAP);

	return ;
}


void PlayerMessage::MS_GetBranchOnlinePlayerInfo(const int& begin,const int& end, BranchPlayerInfo::AckBranchPlayerInfo* info)
{
	int index = 1;
	INT64PLAYERMAP::iterator it = m_allPlayer.begin();
	for(; it != m_allPlayer.end(); ++it)
	{
		if(index >= begin)
			break;
		index++;
	}

	for(; it != m_allPlayer.end(); ++it)
	{
		if(end - index >=0)
		{
			BranchPlayerInfo::BranchPlayerInfo * branch = info->add_branchinfo();

			branch->set_id(GET_PLAYER_CHARID(it->second->GetID()));
			branch->set_name(it->second->GetName());
			branch->set_lv(it->second->getAttManage()->getValue<int>(eCharLv));
			branch->set_onlinetime(it->second->CalOnlineTime());
			branch->set_ip(it->second->GetIP());
		}
		index++;
	}
}

void PlayerMessage::InitServerTime()
{
	if(m_dayTimerID.IsValid())
	{
		m_dayTimerID.SetNull();
	}

	int hour=0,minute=0,seconds=0;
	GameParaLoader::GetInstance()->getDayUpdateTime(hour,minute,seconds);

	m_dayTimerID = TimerInterface::GetInstance()->RegDayPointRepeatedTime(&PlayerMessage::onDateTime,this,0,hour,minute,seconds);
}

void PlayerMessage::onDateTime(void* arg)
{
	OnLineAtTimeMgr::GetInstance()->InitOnLineAtTime();

	this->ResetOnliePlayerInfo();

	//有些模块必须在获得新的刷新时间后才能重置
	this->ResetOnLinePlayerInfoAfterResetTime();
}
void PlayerMessage::eightHoursTime(void* arg)
{
	m_eightHoursTime = CUtil::GetNowSecond() + 8*60*60*1000;
}

void PlayerMessage::ResetOnliePlayerInfo()
{
	INT64PLAYERMAP::iterator it = this->m_allPlayer.begin();
	for(; it != this->m_allPlayer.end(); ++it)
	{
		if((bool) it->second)
		{
			it->second->ResetPlayerSomethig();
		}
	}

	INT64PLAYERCACHEMAP::iterator itCache = this->m_PlayerCache.begin();
	for(; itCache != m_PlayerCache.end(); ++itCache)
	{
		itCache->second.playerCache->ResetPlayerSomethig();
	}
}

void PlayerMessage::ResetOnLinePlayerInfoAfterResetTime()
{
	INT64PLAYERMAP::iterator it = this->m_allPlayer.begin();
	for(; it != this->m_allPlayer.end(); ++it)
	{
		if((bool) it->second)
			it->second->ResetPLayerSomethingAferResetTime();
	}

	INT64PLAYERCACHEMAP::iterator itCache = this->m_PlayerCache.begin();
	for(; itCache != m_PlayerCache.end(); ++itCache)
	{
		itCache->second.playerCache->ResetPLayerSomethingAferResetTime();
	}
}

void PlayerMessage::FuzzySelectPlayerInfo(const string &str, const uint &needNum,
		list<Smart_Ptr<Player> >& fuzzy_list)
{
	INT64PLAYERMAP::iterator it = m_allPlayer.begin();
	for(; it != m_allPlayer.end(); ++it)
	{
		int find_ret = it->second->GetName().find(str);
		if(find_ret >=0)
		{
			fuzzy_list.push_back(it->second);
		}

		if(fuzzy_list.size() >=needNum)
			return ;
	}
}



void PlayerMessage::AllPlayerExitByChannel(int channelID)
{
	if(m_PlayerCache.size() == 0 && m_allPlayer.size() == 0)
	{
		return;
	}
	//LOG_INFO(FILEINFO,"++++++++++++++++++++++++++所有玩家下线并且保存信息！channelID : %d m_PlayerCache.size(): %d m_allPlayer.size(): %d ",channelID,m_PlayerCache.size(),m_allPlayer.size());
	int64 offlineTime = CUtil::GetNowSecond();
	//cout<<endl<<"++++++++++++++++++++++++++所有玩家下线并且保存信息！"<<channelID<<"m_PlayerCache.size():"<<m_PlayerCache.size()<<"m_allPlayer.size()"<<m_allPlayer.size()<<"+++++++++++++++++++++++++++"<<endl;
	INT64PLAYERCACHEMAP::iterator ite = m_PlayerCache.begin();
	for(;ite != m_PlayerCache.end();)
	{
		ite->second.playerCache->SetOffLineTime(offlineTime);
		int id = ite->second.playerCache->GetChannelID();
		cout<<endl<<"m_channelid:"<<id<<"channel"<<channelID<<endl;
		if (ite->second.playerCache->GetType() == ePlayer && ite->second.playerCache->GetChannelID() == channelID)
		{

			PlayerLogout(ite->second.playerCache);
			{
				SaveDataInfo sendData;
				ite->second.playerCache->SaveAllPlayer(sendData,true,true);
				LOG_INFO(FILEINFO,"\n\n+++++++++++++++++++++++++++保存玩家： %s 的信息！\n\n",ite->second.playerCache->GetName().c_str());
			}


			Smart_Ptr<Player> myself;
			ite->second.playerCache->SetMyself(myself);
			m_PlayerCache.erase(ite++);
		}
		else
		{
			ite++;
		}
	}

	INT64PLAYERMAP::iterator it = m_allPlayer.begin();
	for(;it != m_allPlayer.end();)
	{
		it->second->SetOffLineTime(offlineTime);

		Smart_Ptr<Object> obj = it->second;
		int id = it->second->GetChannelID();
		cout<<endl<<"m_channelid:"<<id<<"channel"<<channelID<<endl;
		if(obj->GetType() == ePlayer && it->second->GetChannelID() == channelID)
		{
			CSceneMgr::GetInstance()->LeaveScene(obj);

			PlayerLogout(it->second);
			{
				SaveDataInfo sendData;
				it->second->SaveAllPlayer(sendData,true,true);
				LOG_INFO(FILEINFO,"\n\n+++++++++++++++++++++++++++保存玩家： %s 的信息！\n\n",it->second->GetName().c_str());
			}

			Smart_Ptr<Player> myself;
			Smart_Ptr<Player> myselfTemp;
			myselfTemp = it->second->GetMyself();
			it->second->SetMyself(myself);
			m_allPlayer.erase(it++);
		}
		else
		{
			it++;
		}
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(PlayerMessage, MSG_SIM_WS2GM_SYNCHATTR)
{
	CharBase::AttChangeToGate  req;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;

	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);

	INT64PLAYERMAP::iterator it = m_allPlayer.find(req.charid());
	if (it != m_allPlayer.end())
	{
		player = it->second;

		CharBase::CharAttChange attr = req.attr();
		player->SetTypeValue(attr);

	}
	else
	{
		INT64PLAYERCACHEMAP::iterator itr = m_PlayerCache.find(req.charid());
		if(itr != m_PlayerCache.end())
		{
			player = itr->second.playerCache;
			CharBase::CharAttChange attr = req.attr();
			player->SetTypeValue(attr);
		}
	}
}


DEF_MSG_SIMPLE_DEFINE_FUN(PlayerMessage, MSG_SIM_WS2GM_SYNCHATTRCHAR)
{
	CharBase::AttChangeToGame  req;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;

	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);

	INT64PLAYERMAP::iterator it = m_allPlayer.find(req.charid());
	if (it != m_allPlayer.end())
	{
		player = it->second;

		CharBase::CharAttChangeToGame attr = req.attr();
		player->SetTypeValueChar(attr);

	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(PlayerMessage, MSG_SIM_WS2GM_AD_SKILLBUFF)
{
	ClanPackage::WSToGameADBuff req;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;

	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);

	INT64PLAYERMAP::iterator it = m_allPlayer.find(req.charid());
	if (it != m_allPlayer.end())
	{
		Smart_Ptr<CreatureObj> obj = it->second->GetMyself();

		if(req.flag())
		{
			for(int i = 0; i < req.skillid_size(); i++)
			{
				it->second->AddBuff(req.skillid(i),obj);
			}
		}
		else
		{
			for(int i = 0; i < req.skillid_size(); i++)
			{
				it->second->RemoveBuffById(req.skillid(i));
			}
		}

	}
}

DEF_MSG_REQUEST_DEFINE_FUN(PlayerMessage, MSG_REQ_WS2GM_MAKERT_INFO)
{
	MarketMessInfo::WSReqGameMarketInfo meContent;
	ServerReturn::ServerFourInt toWS;
	Smart_Ptr<Player> player;
	vector<int> vecType;
	char *con;
	int len = 0;
	int ret = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(meContent.charid(), player);
	if(player)
	{
//		StatisticInfo::CostAndGotInfo costAndGotInfo;

		const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(meContent.itemid());

		if(ginfo == NULL)
		{
			ret = eItemError;
		}
		else if(player->GetContainer(ePackageType)->IsBagEnought(meContent.itemid(), meContent.num(),1) != 0)
		{
			ret = ePackageFull;
		}
		else
		{
//			BEHAVIOR_MONI(player, BehaviorType::Main::eMarket, BehaviorType::Minor::eMarket_BuyCost, MONITOR);

//			StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotItemInfo,meContent.itemid(),meContent.num());

			if(meContent.coin() == 1)
			{
				if(player->getAttManage()->getValue<int>(eCharGolden) >= meContent.price())
				{
//					StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
//					StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,meContent.price(),player);

					player->ChangeGolden(meContent.price(),false);

//					StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,player);

					vecType.push_back(eCharGolden);
				}
				else
				{
					ret = eNotEnogGolden;
				}
			}
			else
			{
				if(player->getAttManage()->getValue<int>(eCharMoney) >= meContent.price())
				{
//					StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
//					StatisticMgr::SetItemCostBefore(costItemInfo,eCharMoney,meContent.price(),player);

					player->ChangeMoney(meContent.price(),false);

//					StatisticMgr::SetItemCostAfter(costItemInfo,eCharMoney,player);

					vecType.push_back(eCharMoney);
				}
				else
				{
					ret = eMoneyNotEnough;
				}
			}


//			
		}

		if(vecType.size() > 0)
		{
			player->SynCharAttribute(vecType);

//			if(eAccountType_User == player->GetAccountType())
//				StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Market,eStaMinor_Market_BuyGoods,&costAndGotInfo);
		}

		toWS.set_rets(ret);
		toWS.set_retf(player->GetID());
		toWS.set_rett(meContent.coin());
		toWS.set_retfo(meContent.price());

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_MAKERT_INFO, message, &toWS);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(PlayerMessage, MSG_SIM_WS2GM_CHANGE_EARNINGS)
{
//	ServerReturn::ServerFourInt meContent;
	MarketMessInfo::EarningsInfo meContent;
	PlayerInfo::ItemInfo info;
	Smart_Ptr<Player> player;

	string outPut;
	vector<DWORD> vec;
	vector<int> vecType;
	vector<int> vecLock;
	vector<int64> goods;
	char *con;
	int len = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(meContent.charid(), player);
	if(player)
	{
		if(meContent.retfo() == 1)
		{
			if(meContent.rett() == 1)
			{
				player->ChangeGolden(meContent.rets());
				vecType.push_back(eCharGolden);
			}
			else
			{
				player->ChangeMoney(meContent.rets());
				vecType.push_back(eCharMoney);
			}
		}
		else if(meContent.retfo() == 2)
		{
			const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(meContent.rets());
			if(player->GetContainer(ePackageType)->IsBagEnought(ginfo->itemID, meContent.rett()) == 0)
			{
				player->GetContainer(ePackageType)->AddItem(ginfo->itemID, meContent.rett(),vecLock,0,ginfo->equipStrengthLv,true);
//				if(ginfo && 0 == ret)
//				{
//					eGoodsType type = (eGoodsType)ginfo->type;
//					if(type == eWeapon || type == eArmor || type == eJewelry)
//					{
//						StatisticInfo::CostAndGotInfo costGotInfo;
//						StatisticInfo::GotItemInfo * gotInfo = costGotInfo.add_gotlist();
//						StatisticMgr::AddItemGotInfo(gotInfo,meContent.rets(),meContent.rett());
//						if(eAccountType_User == player->GetAccountType())
//							StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_EquipFrom,eStaMinor_EquipFrom_Market,&costGotInfo);
//					}
//				}
			}
			else
			{
				int64 info = meContent.rett();
				info = info << 32;	//物品数量
				DWORD itemid =meContent.rets();
				info  = info | itemid;
				goods.push_back(info);

				if(goods.size() > 0)
				{
					string warning = InternationalLoader::GetInstance()->GetInterStr(5);
					string warning1 = InternationalLoader::GetInstance()->GetInterStr(6);
					MailMessageMgr::GetInstance()->SendSysMail(goods, player->GetName(),0,warning.c_str(),warning1.c_str());
				}
			}
		}
		else if(meContent.retfo() == 3)
		{
			meContent.mutable_iteminfo()->SerializeToString(&outPut);
			info.ParseFromString(outPut);

			int ret = player->GetContainer(ePackageType)->SaveClientItemInfo(&info,vecLock);
			if(ret == 0)
			{
				if(vecLock.size() > 0)
				{
					vector<int>::iterator itVec = vecLock.begin();
					for(; itVec != vecLock.end(); ++itVec)
					{
						vec.push_back((ePackageType << 16) | (*itVec));
					}
				}

				player->SendSectorChange(vec);

//				const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(info.id());
//				if(ginfo)
//				{
//					eGoodsType type = (eGoodsType)ginfo->type;
//					if(type == eWeapon || type == eArmor || type == eJewelry)
//					{
//						StatisticInfo::CostAndGotInfo costGotInfo;
//						StatisticInfo::GotItemInfo * gotInfo = costGotInfo.add_gotlist();
//						StatisticMgr::AddItemGotInfo(gotInfo,info.id(),info.flag() & 0xff);
//						if(eAccountType_User == player->GetAccountType())
//							StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_EquipFrom,eStaMinor_EquipFrom_Market,&costGotInfo);
//					}
//				}
			}
			else
			{
				int64 info = meContent.rett();
				info = info << 32;	//物品数量
				DWORD itemid =meContent.rets();
				info  = info | itemid;
				goods.push_back(info);

				if(goods.size() > 0)
				{
					string warning = InternationalLoader::GetInstance()->GetInterStr(5);
					string warning1 = InternationalLoader::GetInstance()->GetInterStr(6);
					MailMessageMgr::GetInstance()->SendSysMail(goods, player->GetName(),0,warning.c_str(),warning1.c_str());
				}
			}
		}

		if(vecType.size() > 0)
		{
			player->SynCharAttribute(vecType);
		}

	}
}

DEF_MSG_REQUEST_DEFINE_FUN(PlayerMessage, MSG_REQ_WS2GM_UP_CARRIAGES)
{
	MarketMessInfo::WSReqGameMarketInfo meContent;
//	ServerReturn::ServerFiveInt toWS;
	MarketMessInfo::GMRetGameMarketInfo toWorld;
	CharLogin::ClientItemInfo info;
	Smart_Ptr<Player> player;
	vector<int> vecType;
	string outPut;
	char *con;
	int len = 0;
	int pos = 0;
	int ret = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	pos = meContent.itemid();

	PlayerMessage::GetInstance()->GetPlayerByCharid(meContent.charid(), player);
	if(player)
	{
		Smart_Ptr<PropBase>  stone = player->GetContainer(ePackageType)->GetItemDataPtr(meContent.itemid());

		if(!stone)
		{
			ret = eNotFindItem;
		}
		else if(stone->GetID() == GameParaLoader::GetInstance()->getExchangeMoneyItemId()
				|| stone->GetID() == GameParaLoader::GetInstance()->getExchangeGoldenItemId()
				|| stone->GetID() == GameParaLoader::GetInstance()->getExchangeBindGoldenItemId())
		{
			ret = eCantSellItem;
		}
		else if(stone->GetBindType() != eNullBind)
		{
			ret = eMarketShopBindError;
		}
		else if(stone->GetAmount() < meContent.num())
		{
			ret = eMarketShopNumNotEnough;
		}
		else
		{
			const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(stone->GetID());
			if(ginfo)
			{
					if(ginfo->type == eTask)
					{
							ret = eCantSellItem;
					}
			}

			if(ret == 0)
			{
				BEHAVIOR_MONI(player, BehaviorType::Main::eMarket, BehaviorType::Minor::eMarket_ItemSell, MONITOR);

				toWorld.set_itemid(stone->GetID());
				toWorld.set_num(meContent.num());
				toWorld.set_price(meContent.price());
				toWorld.set_coin(meContent.coin());
				stone->SetClientInfo(&info,meContent.num());
				info.SerializeToString(&outPut);
				toWorld.mutable_iteminfo()->ParseFromString(outPut);

	//			toWS.set_retf(meContent.price());
	//			toWS.set_rets(stone->GetID());
	//			toWS.set_oldxpos(meContent.num());
	//			toWS.set_oldypos(meContent.coin());
//				StatisticInfo::CostAndGotInfo costAndGotInfo;

//				StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
//				StatisticMgr::SetItemCost(costItemInfo,stone->GetID(),meContent.num());
//
//				if(meContent.price() > 0)
//				{
//					StatisticInfo::CostItemInfo * price = costAndGotInfo.add_costlist();
//					StatisticMgr::SetItemCost(price,eCharMoney,meContent.price());
//				}

//				if(meContent.coin() > 0)
//				{
//					StatisticInfo::CostItemInfo * coin = costAndGotInfo.add_costlist();
//					StatisticMgr::SetItemCost(coin,eCharGolden,meContent.coin());
//				}

//				if(eAccountType_User == player->GetAccountType())
//					StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Market,eStaMinor_Market_UpCarriages,&costAndGotInfo);

				player->GetContainer(ePackageType)->DeleteGrideItem(meContent.itemid(),meContent.num());

				
			}
		}
	}

//	toWS.set_oldrets(ret);
	toWorld.set_ret(ret);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_UP_CARRIAGES, message, &toWorld);
	Message_Facade::Send(messRet);
}

DEF_MSG_REQUEST_DEFINE_FUN(PlayerMessage, MSG_REQ_WS2GM_SYSREWARD)
{
	ActivitiesExProto::SysActivityReward meContent;
	ActivitiesExProto::RetSysActivityReward toWorld;
	Smart_Ptr<Player> player;
	char *con;
	int len = 0;
	int ret = 0;

	vector<int> vecType;
	vector<int> vecLock;
	vector<DWORD> vec;
	vector<int64> goods;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(meContent.charid(), player);
	if(player)
	{
//		StatisticInfo::IDAndCostAndGotInfo idAndCostAndGotInfo;
//		idAndCostAndGotInfo.set_beforeid(meContent.activityid());
//		idAndCostAndGotInfo.set_afterid(meContent.activitytype());
//		StatisticInfo::CostAndGotInfo * costAndGotInfo = idAndCostAndGotInfo.mutable_costandgotlist();

		BEHAVIOR_MONI(player, BehaviorType::Main::eOpeActivity, BehaviorType::Minor::eOpeActivity_GetReward, MONITOR);

		if(meContent.itemid_size() > 0)
		{
			vector<DWORD> goodsIdList;
			vector<DWORD> numList;
			vector<DWORD> strengthLvVec;
			vector<WORD> bindVec;

			for(int i = 0; i < meContent.itemid_size();i++)
			{
				const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(meContent.itemid(i));
				if(ginfo==NULL)
					continue;
				goodsIdList.push_back(ginfo->itemID);
				numList.resize(goodsIdList.size(), meContent.itemnum(i));

				strengthLvVec.resize(goodsIdList.size(), ginfo->equipStrengthLv);
				bindVec.resize(goodsIdList.size(), 1);
			}

			if(player->GetContainer(ePackageType)->IsBagEnought(goodsIdList, numList,bindVec) == 0)
			{
				player->GetContainer(ePackageType)->AddItem(goodsIdList, numList, vecLock, bindVec,strengthLvVec,true,efightDefinedNULL);

//				StatisticInfo::CostAndGotInfo costAndGotInfo1;
//				for(size_t i = 0;i < goodsIdList.size();++i)
//				{
//					if(goodsIdList[i] == DWORD(GameParaLoader::GetInstance()->getExchangeMoneyItemId()) && numList[i] > 0)
//					{
//						StatisticInfo::CostAndGotInfo costAndGotInfo;
//						StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//						StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,numList[i]);
//						StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Money,eStaMinor_Money_ActivityEx,&costAndGotInfo);
//					}
//					else if(goodsIdList[i] == DWORD(GameParaLoader::GetInstance()->getExchangeBindGoldenItemId()) && numList[i] > 0)
//					{
//						StatisticInfo::CostAndGotInfo costAndGotInfo;
//						StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//						StatisticMgr::AddItemGotInfo(gotInfo,eCharBindGolden,numList[i]);
//						StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_BindGolden,eStaMinor_BindGolden_ActivityEx,&costAndGotInfo);
//					}
//					else if(goodsIdList[i] == DWORD(GameParaLoader::GetInstance()->getExchangeCreditItemId()) && numList[i] > 0)
//					{
//						StatisticInfo::CostAndGotInfo costAndGotInfo;
//						StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//						StatisticMgr::AddItemGotInfo(gotInfo,eCredit,numList[i]);
//						StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Credit,eStaMinor_Credit_ActivityEx,&costAndGotInfo);
//					}
//					StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo->add_gotlist();
//					StatisticMgr::AddItemGotInfo(gotInfo,goodsIdList[i],numList[i]);

//					const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsIdList[i]);
//					if(ginfo)
//					{
//						eGoodsType type = (eGoodsType)ginfo->type;
//						if(type == eWeapon || type == eArmor || type == eJewelry)
//						{
//							StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo1.add_gotlist();
//							StatisticMgr::AddItemGotInfo(gotInfo,goodsIdList[i],numList[i]);
//						}
//					}
//				}

				if(vecLock.size() > 0)
				{
//					if(costAndGotInfo1.gotlist_size() > 0 && eAccountType_User == player->GetAccountType())
//						StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_EquipFrom,eStaMinor_EquipFrom_Activity,&costAndGotInfo1);

					vector<int>::iterator itVec = vecLock.begin();
					for(; itVec != vecLock.end(); ++itVec)
					{
						vec.push_back((ePackageType << 16) | (*itVec));
					}
				}

				player->SendSectorChange(vec);
			}
			else
			{
				ret = ePackageFull;
			}
		}

		if(ret == 0)
		{
			if(meContent.golden() > 0)
			{
				player->ChangeGolden(meContent.golden());
				vecType.push_back(eCharGolden);

//				StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo->add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo,eCharGolden,meContent.golden());
			}

			if(meContent.bgolden() > 0)
			{
				player->ChangeBindGolden(meContent.bgolden(),true);
				vecType.push_back(eCharBindGolden);

//				StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo->add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo,eCharBindGolden,meContent.bgolden());
			}

			if(vecType.size() > 0)
			{
				player->SynCharAttribute(vecType);
			}

//			if(eAccountType_User == player->GetAccountType())
//				StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_ActivitesEx,eStaMinor_ActivitesEx_GotRewardEvent,&idAndCostAndGotInfo);
		}

		
	}
	else
	{
		ret = eNotHaveOnline;
	}

	toWorld.set_ret(ret);
	toWorld.set_activityid(meContent.activityid());
	toWorld.set_activitytype(meContent.activitytype());
	toWorld.set_index(meContent.index());

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_SYSREWARD, message, &toWorld);
	Message_Facade::Send(messRet);
}

//完成副本活跃度
void PlayerMessage::FinshVitality(Smart_Ptr<Player> &player,int mapid)
{
	InstanceMapEntry *entry =	InstanceLoader::GetInstance()->GetEntry(mapid);
	if (NULL == entry)
	{
		return;
	}
//	if(entry->m_nIsSpecial == 2)
//	{
//		player->GetVitalityMgr()->FinshVitality(eViThreatInstance);
//	}
	else
	{
		switch(entry->m_eType)
		{
			case ePersonalInstance:			//个人
			{
				player->GetVitalityMgr()->FinshVitality(eViPersonalInstance);
//				if (0 <= player->GetVitalityMgr()->GetUnCompleteByID(eViPersonalInstance))
//				{
//					player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), 1, false);
//				}

				break;
			}
			case eGroupInstance:				//组队
			{
				player->GetVitalityMgr()->FinshVitality(eViGroupInstance);
//				if (0 <= player->GetVitalityMgr()->GetUnCompleteByID(eViGroupInstance))
//				{
//					player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), 1, false);
//				}

				break;
			}
			case eBattleGroudInstance:		//混合  相当于战场
			{
				break;
			}
			case eAnswerInstance:
			{
				break;
			}
			case eCompeteInstance: //竞技场
			{
				player->GetVitalityMgr()->FinshVitality(eViCompeteInstance);
				break;
			}
			case eClanBattle: //帮派战
			{
				break;
			}
			case eTreasonKing: //剿灭反王
			{
				player->GetVitalityMgr()->FinshVitality(eViTreasonKing);
//				if (0 <= player->GetVitalityMgr()->GetUnCompleteByID(eViTreasonKing))
//				{
//					player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), 1, false);
//				}
				break;
			}
			case eProtectKing: //守护秦王
			{
				player->GetVitalityMgr()->FinshVitality(eViProtectKing);
//				if (0 <= player->GetVitalityMgr()->GetUnCompleteByID(eViProtectKing))
//				{
//					player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), 1, false);
//				}
				break;
			}
			case eBeyondSvrBattle: //夸服战
			{
				player->GetVitalityMgr()->FinshVitality(eViBeyondSvrBattle);
				break;
			}
			case eInstanceMax:
			case eVipInstance: //VIP副本
			{
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(PlayerMessage, MSG_REQ_WS2GM_ACTIVITY_INVESTCOST)
{
	ActivitiesExProto::AskActivityInestCost meContent;
	ActivitiesExProto::RetActivityInestCost toWorld;
	Smart_Ptr<Player> player;
	char *con;
	int len = 0;
	int ret = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

//	StatisticInfo::IDAndCostAndGotInfo idAndCostAndGotInfo;

	PlayerMessage::GetInstance()->GetPlayerByCharid(meContent.charid(), player);
	if(player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eOpeActivity, BehaviorType::Minor::eOpeActivity_Invest, MONITOR);

		if(player->getAttManage()->getValue<int>(eCharGolden) >= meContent.golden())
		{
//			idAndCostAndGotInfo.set_beforeid(meContent.activityid());
//			idAndCostAndGotInfo.set_afterid(meContent.activitytype());
//			StatisticInfo::CostAndGotInfo * costAndGotInfo = idAndCostAndGotInfo.mutable_costandgotlist();
//			StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo->add_costlist();
//			StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,meContent.golden(),player);

			player->ChangeGolden(meContent.golden(),false);

//			StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,player);
		}
		else
		{
			ret = eNotEnogGolden;
		}

		
	}
	else
	{
		ret = eNotHaveOnline;
	}

	if(ret == 0)
	{
		toWorld.set_activityid(meContent.activityid());
		toWorld.set_activitytype(meContent.activitytype());
		toWorld.set_golden(meContent.golden());

//		if(player && eAccountType_User == player->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_ActivitesEx,eStaMinor_ActivitesEx_InvestEvent,&idAndCostAndGotInfo);
	}

	toWorld.set_ret(ret);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_ACTIVITY_INVESTCOST, message, &toWorld);
	Message_Facade::Send(messRet);
}

void PlayerMessage::FinishiInit(Smart_Ptr<Player> &player,Safe_Smart_Ptr<NullBase> &act)
{
	//初始化完毕返回
	if(player->GetInitPartDataFlag())
	{
		LOG_DEBUG(FILEINFO, "finish player[%lld] info ", GET_PLAYER_CHARID(player->GetID()));
		player->setFinishiInit(true);
		player->onlineCalculateAtt();
		player->GetCounterService()->initFinish();

		Smart_Ptr<ArticleParcelMgr> parcel = player->GetContainer(ePackageType);
		parcel->SendArticleInfo();
		parcel->StartTime();

		Smart_Ptr<ArticleStorehouseMgr> house = player->GetContainer(eStoreHouseType);
		house->SendArticleInfo();
		house->StartTime();

		//检测时限物品
		player->beginCheckBagTimer();
		player->GetArenaMgr()->Reset();

		//是否可以进入场景
		if(!IsLoginON(player->GetMyself(),static_cast<commonAct *>(act.Get())->mesDatas))
		{
			Smart_Ptr<Player> myself;
			player->SetMyself(myself);
			INT64PLAYERCACHEMAP::iterator tempIt = m_PlayerCache.find(player->GetID());
			if(tempIt != m_PlayerCache.end())
			{
				m_PlayerCache.erase(tempIt);
			}

			LOG_DEBUG(FILEINFO, "gateserver request gameserver player[%lld] info and no logon in map", player->GetID());

			return ;
		}

		if(!(static_cast<commonAct *>(act.Get())->mesDatas))
		{
			LOG_DEBUG(FILEINFO, "player login in game and finish player[%lld] info ", GET_PLAYER_CHARID(player->GetID()));
			CharLogin::CharBaseInfo info;
			player->SetCharBaseInfo(&info);
			info.set_mapid(GET_MAP_ID(info.mapid()));
			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_PLAYERINFO,static_cast<commonAct *>(act.Get())->mesReq,&info);
			messRet->SetMessageTime(info.charid());
			Message_Facade::Send(messRet);

			//玩家信息 初始化 完成 之后  通知  任务 信息
			player->GetQuestMgr()->FillInQuestInfoCmd();

			//推送节日活动
			FestivalActiveMgr::GetInstance()->FestivalCmd(player);
		}
		else
		{
			LOG_DEBUG(FILEINFO, "change map and finish player[%lld] info ", GET_PLAYER_CHARID(player->GetID()));
			ChangeMap::ChangeMapInfo ret;

			ret.set_charid(player->GetID());
			ret.set_mapid(player->GetMapID());
			ret.set_pos((player->GetXNewPos() << 16) | player->GetYNewPos());
			ret.set_key(player->GetKey());

			MapStaticInfo *sMapInfo = NULL;
			sMapInfo = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(player->GetMapID()));

			if(sMapInfo)
			{
				ret.set_pktype(player->GetPkTypeByMap(sMapInfo->pktype));
			}

			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(static_cast<commonAct *>(act.Get())->mesReq->GetMessageID(),static_cast<commonAct *>(act.Get())->mesReq, &ret);
			Message_Facade::Send(messRet);
		}

		Smart_Ptr<OnceReward>& onceReward = player->GetOnceReward();
		if ((bool)onceReward)
		{
			onceReward->SynRewardToClient();
		}

		player->UpdateUIHighLight(eUIHLHandle_Init, UIHLType(eUIHLBig_Null, eUIHLSmall_Null));
	}
	else
	{
		LOG_DEBUG(FILEINFO, "finish player[%lld] info but init no end ", GET_PLAYER_CHARID(player->GetID()));
	}
}

void PlayerMessage::getWBaseInfo(CharLogin::WBaseInfo& wBaseInfo,Smart_Ptr<Player> player,bool isOnLine)
{
	wBaseInfo.set_charid(player->GetID());
	wBaseInfo.set_mapid(player->GetMapID());
	wBaseInfo.set_localtype((ServerConHandler::GetInstance()->GetLocalType() << 16)| ServerConHandler::GetInstance()->GetLocalID());
	wBaseInfo.set_name(player->GetName());
	wBaseInfo.set_lv(player->getAttManage()->getValue<int>(eCharLv));
	wBaseInfo.set_profession(player->getAttManage()->getValue<BYTE>(eCharProfession));
	wBaseInfo.set_settings(player->getAttManage()->getValue<int>(eSettings));
	wBaseInfo.set_country(player->getAttManage()->getValue<DWORD>(eCharCountry));
	wBaseInfo.set_countryjob(player->getAttManage()->getValue<WORD>(eCountryJob));
	wBaseInfo.set_contribute(player->getAttManage()->getValue<int>(eCurCountryContribute));
	wBaseInfo.set_fitchprize(player->getFitchFlag());
	wBaseInfo.set_nexttalktime(player->GetTalkingTm());
	wBaseInfo.set_score(player->GetSvrScore());
	wBaseInfo.set_offlinetime(player->GetOffLineTime());
	wBaseInfo.set_teamid(player->getAttManage()->getValue<int64>(eTeamId));
	wBaseInfo.set_fightpower(player->getAttManage()->getValue<int>(eFightPower));
	wBaseInfo.set_sex(player->GetSex());
	wBaseInfo.set_viplevel(player->getAttManage()->getValue<DWORD>(eVIPLv));
	wBaseInfo.set_openid(player->GetOpenID());
	wBaseInfo.set_openkey(player->GetOpenKey());
	wBaseInfo.set_pf(player->GetPf());
	wBaseInfo.set_pfkey(player->GetPfKey());
	wBaseInfo.set_addupgolden(player->getAttManage()->getValue<int>(eAddUpGold));
	wBaseInfo.set_golden(player->getAttManage()->getValue<int>(eCharGolden));
	wBaseInfo.set_accounttype(player->GetAccountType());
	wBaseInfo.set_source(player->Get58PlaySouce());

	if(player->GetMagicWeapon())
	{
		wBaseInfo.set_magicweapon(player->GetMagicWeapon()->getMagicOrder());
	}

	if(player->GetHorseManager())
	{
		wBaseInfo.set_horseid(player->GetHorseManager()->GetHourseQuality());
	}

	if (player->GetCounterService())
	{
		wBaseInfo.set_challengearenanum(player->GetCounterService()->GetNumById(ARENACHALLROBOT_COUNT));
	}



	return ;
}

//int PlayerMessage::RefreshCostCheck(Smart_Ptr<Player> player, int& money, int& golden, int& bindGolden)
//{
//	ArenaParam::RefreshInfo info;
//	if (-1 == ArenaLoader::GetInstance()->GetRefreshInfo(player->GetCounterService()->GetNumById(ARENA_ALREADYREFRESH_CNT) + 1,
//			info))
//	{
//		return eItemError;
//	}
//
//	int golden_ = 0, money_ = 0, bindGolden_ = 0;
//	if (0 < info.costGolden)
//	{
//		bindGolden_ += int(ceil(info.costGolden));
//	}
//
//	if (0 < bindGolden_)
//	{
//		golden_ = bindGolden_;
//		if (player->getAttManage()->getValue<int>(eCharGolden) < golden_)
//			return eNotEnogGolden;
//
////		if (player->getAttManage()->getValue<int>(eCharBindGolden) < bindGolden_)
////		{
////			golden_ = bindGolden_ - player->getAttManage()->getValue<int>(eCharBindGolden);
////			bindGolden_ = player->getAttManage()->getValue<int>(eCharBindGolden);
////
////			if (player->getAttManage()->getValue<int>(eCharGolden) < golden_)
////				return eNotEnogGolden;
////		}
//	}
//
//	money = money_;
//	golden = golden_;
//	bindGolden = bindGolden_;
//
//	return 0;
//}

void PlayerMessage::HandleWSAckData(Smart_Ptr<Player>& player, CharLogin::GMGetWSAckData& meContent, Safe_Smart_Ptr<NullBase> &act)
{
	if(meContent.clanid() != 0)
	{
		player->getAttManage()->setValue(eCharFaction, meContent.clanid());
		player->getAttManage()->setValue(eFaContribution, meContent.facontribution());
		player->SetClanName(meContent.clanname());
		player->getAttManage()->setValue(eCharFaJob, meContent.fajob());
		player->getAttManage()->setValue(eCharClanIcon, meContent.clanicon());
		player->getAttManage()->setValue(eTeamId, meContent.teamid());

		ClanMessage::GetInstance()->onlineCalculateAtt(player,meContent.clanlevel());
	}

	//设置结婚数据
	player->GetMarryRing()->onlineSetData(meContent.marryotherid(),meContent.marryothername(),meContent.marrytime());

	int tempTitleSize = meContent.title_size();
	ServerReturn::ServerDoubleIntArray tempTitleMsg;
	for(int i = 0;i < tempTitleSize;++i)
	{
		tempTitleMsg.add_rets(meContent.title(i));
	}

	player->GetTitle()->UpDate(tempTitleMsg,true);

	FinishiInit(player,act);
	player->UpdateUIHighLight(eUIHLHandle_Up, UIHLType(eUIHLBig_Faction, eUIHLSmall_Faction_HaveUnHandleJoin), meContent.reqclannum(), true);

	int envelopeCnt = 0, haveEnvelope = 0;
	if ((bool)player->getEnvelopeMgr())
	{
		haveEnvelope = player->getEnvelopeMgr()->GetCurrentCanGetEnvelopeCount();
		envelopeCnt += haveEnvelope < meContent.envelopecnt() ? haveEnvelope : meContent.envelopecnt();
	}
	envelopeCnt += (meContent.cangetdistreward() + meContent.cangetdrawenvereward());
	player->UpdateUIHighLight(eUIHLHandle_Up, UIHLType(eUIHLBig_Country, eUIHLSmall_Country_HaveUnGetReward), envelopeCnt, true);
	if (player->IsOpenSystem(eStAnswer))
		player->UpdateUIHighLight(eUIHLHandle_Up, UIHLType(eUIHLBig_Activity, eUIHLSmall_Activity_AnswerOpen), 1, meContent.isanswerbegin());
//	if (player->IsOpenSystem(eStProtectKingLv))
		player->UpdateUIHighLight(eUIHLHandle_Up, UIHLType(eUIHLBig_Activity, eUIHLSmall_Activity_DoubleGuardKingOpen), 1, meContent.isprotectkingdouble());
	if (player->IsOpenSystem(eStEscort))
		player->UpdateUIHighLight(eUIHLHandle_Up, UIHLType(eUIHLBig_Activity, eUIHLSmall_Activity_DoubleEscortOpen), 1, meContent.isescortdoublebegin());
	if (player->IsOpenSystem(eStBattleParis))
		player->UpdateUIHighLight(eUIHLHandle_Up, UIHLType(eUIHLBig_Activity, eUIHLSmall_Activity_BattlePVP), 1, meContent.ispvpfightwarbegin());
	player->UpdateUIHighLight(eUIHLHandle_Up, UIHLType(eUIHLBig_FactionBattle, eUIHLSmall_FactionBattle_Open), 1, meContent.isclanwarbegin());

	int worldBossOpenCnt = 0;
	for (int i = 0; i < meContent.worldbossopenid_size(); ++i)
	{
		int mapID = meContent.worldbossopenid(i);
		InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(mapID);
		if (NULL == entry)
		{
			continue;
		}
		if ((int)(entry->m_nLevel) <= player->getAttManage()->getValue<int>(eCharLv))
		{
			++worldBossOpenCnt;
		}
	}
	player->UpdateUIHighLight(eUIHLHandle_Mod, UIHLType(eUIHLBig_TroubledTimes, eUIHLSmall_TroubledTimes_WorldBoss), worldBossOpenCnt);

	// 新服活动状态
	vector<E_UIHL_BIGTYPE> vecBig;
	vector<E_UIHL_SMALLTYPE> vecSmall;
	vector<int> vecNum;
	vector<bool> vecFlag;
	vector<E_UIHL_HANDLE> vecHandle;
	CharLogin::ActivityStateList* list = meContent.mutable_list();
	for (int i = 0; i < list->states_size(); ++i)
	{
		CharLogin::ActivityState states = list->states(i);
		vector<UIHLType> type = CHighLightMgr::GetHLTypeByActivity((ActivityExType)(states.type()));

		for (uint i = 0; i < type.size(); ++i)
		{
			vecBig.push_back(type[i].first);
			vecSmall.push_back(type[i].second);
			vecNum.push_back(states.num());
			vecFlag.push_back(states.flag());
		}
	}
	if (0 < vecBig.size())
	{
		vecHandle.resize(vecBig.size(), eUIHLHandle_Mod);

		player->UpdateUIHighLight(vecHandle, vecBig, vecSmall, vecNum, vecFlag);
	}

	player->UpdateUIHighLight(eUIHLHandle_Syn, UIHLType(eUIHLBig_Null, eUIHLSmall_Null));
}

//DEF_MSG_SIMPLE_DEFINE_FUN(PlayerMessage, MSG_SIM_WS2GM_ISHAVECOMPENSATETOGET)
//{
//	ServerReturn::ServerDoubleInt  req;
//	char *con;
//	int len = 0;
//	Smart_Ptr<Player> player;
//
//	con = message->GetBuffer(len);
//	req.ParseFromArray(con, len);
//
//	PlayerMessage::GetInstance()->GetPlayerByCharid(req.retf(),player);
//	if (bool(player))
//	{
//		player->SetIsHaveCompensate(bool(req.rets()));
//
//		player->SynchActiveHighLight();
//	}
//}

DEF_MSG_SIMPLE_DEFINE_FUN(PlayerMessage, MSG_SIM_WS2GM_ARENABECHALL)
{
	ServerReturn::ServerThreeIntChar ret;
	SystemNoticeProto::ToClientNoticeArena toClient;
	char *buff;
	int len = 0;

	buff = message->GetBuffer(len);
	ret.ParseFromArray(buff, len);

	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharId(player, ret.rets());

	if ((bool)player)
	{
		toClient.set_time(CUtil::GetNowSecond());
		toClient.set_name(ret.retfo());
		if (0 < ret.retf())
		{
			player->getAttManage()->setValue(eRanking, ret.retf());
		}
		toClient.set_flag(ret.rett());
		toClient.set_ranking(player->getAttManage()->getValue<int>(eRanking));

		QuickMessage::GetInstance()->AddSendMessage(ret.rets(), player->GetChannelID(), &toClient, MSG_SIM_GM2C_ARENABECHALL);

	}

}

DEF_MSG_ACK_DEFINE_FUN(PlayerMessage, MSG_REQ_GM2WS_GET_WS_DATA)
{
	//请求超时
	if (message->GetErrno() == eReqTimeOut)
	{
		LOG_DEBUG(FILEINFO, "gameserver MSG_REQ_GM2WS_GET_WS_DATA ack timeout");
		return;
	}

	CharLogin::GMGetWSAckData meContent;
	char* tempChar;
	int tempLenght = 0;

	Smart_Ptr<Player> player;
	INT64PLAYERMAP::iterator it = m_allPlayer.find(static_cast<commonAct *>(act.Get())->mesDataf);
	if(it != m_allPlayer.end())
	{
		player = it->second;
	}
	else
	{
		INT64PLAYERCACHEMAP::iterator itCache = m_PlayerCache.find(static_cast<commonAct *>(act.Get())->mesDataf);
		if(itCache != m_PlayerCache.end())
		{
			player = itCache->second.playerCache;
		}
		else
		{
			LOG_ERROR(FILEINFO, "get player[charid=%lld] container info but player is not existed", GET_PLAYER_CHARID(static_cast<commonAct *>(act.Get())->mesDataf));

			return;
		}
	}

	tempChar = message->GetBuffer(tempLenght);
	meContent.ParseFromArray(tempChar,tempLenght);

	HandleWSAckData(player, meContent, act);

	return ;
}


//普通地图切换
void PlayerMessage::ChangeNormal(Smart_Ptr<Player>& player, Safe_Smart_Ptr<CommBaseOut::Message> &message, const MapStartPoint& info)
{
	int64 ret = -1;
	int64 sceneid = 0;
	int64 txpos = 0;
	int64 typos = 0;

	if(GET_MAP_ID(player->GetMapID()) != info.mapID)
	{
		ret = (int64) eStartPointError;
	}
	else if (CSceneMgr::GetInstance()->GetDistance(info.xpos, info.ypos,player->GetXNewPos(),player->GetYNewPos()) > STARTPOINT_LENGTH)
	{
		ret = (int64) ePointDistance;
	}
	else if (info.bCamp != 0 && info.bCamp != player->getAttManage()->getValue<DWORD>(eCharCountry))
	{
		ret = (int64) eCampError;
	}
	else
	{
		MapStaticInfo *sMapInfo;
		sMapInfo = CMapConfig::GetInstance()->GetStaticInfo(info.tMapID);

		txpos = info.tXPos;
		typos = info.tYPos;

		if(sMapInfo == 0)
		{
			LOG_ERROR(FILEINFO, "static map[id=%d] into is error", info.tMapID);
			return;
		}
		else if(sMapInfo->lv > player->getAttManage()->getValue<int>(eCharLv))
		{
			ret = (int64)eLVNotEnough;
		}
		else
		{
			sceneid = CSceneMgr::GetInstance()->GetSceneIDAboutPlayerCount(info.tMapID);
			if(sceneid > 0)    //镖车是要传送到changeMap
			{
				PlayerLeave(message,player,sceneid,info.tXPos,info.tYPos);
				return;
			}
			else if(sceneid <= 0)
			{
				ChangeMapLine(message,player,info.ID,eStartPoint);
				return;
			}
		}
	}

	ChangeMap::ChangeMapInfo ack;
	MapStaticInfo *sMapInfo = NULL;
	sMapInfo = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(ret));
	if(sMapInfo && player)
	{
		ack.set_pktype(player->GetPkTypeByMap(sMapInfo->pktype));
	}

	ack.set_mapid(ret==-1?eInstanceMapDestory:ret);
	ack.set_pos((txpos << 16) | typos);
	ack.set_charid(player->GetID());

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_CHANGEMAP, message, &ack);
	Message_Facade::Send(messRet);
}

//切入副本
void PlayerMessage::ChangeInDuplicate(Smart_Ptr<Player>& player, Safe_Smart_Ptr<CommBaseOut::Message> &message, int64 secneid)
{
	if(player->IsInCreatureState(ERS_Escort))
	{
		return;
	}

	FinshVitality(player,GET_MAP_ID(secneid));
	//清除连斩BUFF
	if(GET_MAP_ID(secneid) == GameParaLoader::GetInstance()->getArenaMapId() || GET_MAP_ID(secneid) == GameParaLoader::GetInstance()->getCrossArenaMapId())
	{
		player->EndContinueBeheaded();
	}

	//如果本服找不到场景，请求换线
	Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(secneid);
	if(instance.Get()==NULL)
	{
		ChangeServer(player, message, secneid);
		return;
	}

	InstanceMapEntry* instanceInfo = instance->GetInstanceScript();
	if(!instanceInfo)
		return;

	//能否进入副本
	if(!IsCanEnterDuplicate(player, message, instance, instanceInfo))
		return;

	//保存当前地图及位置坐标
	LeaveCurSecne(player, message, secneid, instance, instanceInfo);

}

//切出副本
void PlayerMessage::ChangeOutDuplicate(Smart_Ptr<Player>& player, Safe_Smart_Ptr<CommBaseOut::Message> &message)
{
	int mapTypeNow = GET_MAP_TYPE(player->GetMapID());
	switch(mapTypeNow)
	{
		case ePVEMap:								//切出PVE副本
		case eBeyondMap:						//切出跨服副本
		{
			Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
			if(instance)
			{
				Smart_Ptr<Object> obj = player;
				OldMapInfo* info = instance->GetOldMapPoint(player->GetID());
				if(!info)
				{
						MapStaticInfo * mapInfo = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(player->GetMapID()));
						if(mapInfo==NULL)
						{
							return;
						}

						//强制切图
						FastChangeMap(message,player,mapInfo->reliveMap,mapInfo->reliveXPos,mapInfo->reliveYPos,true);
						return;
				}

				if(info->mapid > 0 && CSceneMgr::GetInstance()->IsHaveScene(info->mapid))
				{
						PlayerLeave(message,player,info->mapid,info->xpos,info->ypos,true);
						return;
				}
				else
				{
						ChangeMapLine(message,player,info->mapid,((int64)info->xpos << 48) | ((int64)info->ypos << 32) | eSceneType);
						return;
				}
			}
			else
			{
					//gm的副本ID已经删除了，场景还没移除，使玩家还在副本中，将玩家传送出副本
					MapStaticInfo * mapInfo = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(player->GetMapID()));
					if(!mapInfo)
					{
						return;
					}
					//强制切图
					FastChangeMap(message,player,mapInfo->reliveMap,mapInfo->reliveXPos,mapInfo->reliveYPos,true);
					return;
			}
			break;
		}

		default:
			break;
	}
}


void PlayerMessage::ChangeServer(Smart_Ptr<Player>& player, CommBaseOut::Safe_Smart_Ptr<CommBaseOut::Message> & message, int64 secneid)
{
		InstanceMapEntry *entry =	InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(secneid));
		if(NULL == entry)
			return;

		int64 txpos=0, typos=0;

		switch(entry->m_eType)
		{
			case eCountryBoss:
			{
					GsCountryMgr::GetInstance()->GetRandomPos(txpos,typos,player->getAttManage()->getValue<DWORD>(eCharCountry));
					ChangeMapLine(message,player,secneid,(txpos << 48) | (typos << 32) |eSceneType);
			}
			break;

			case eCountryWar:
			{
					CountryWarLoader::GetInstance()->GetRandomPos(txpos,typos,player->getAttManage()->getValue<DWORD>(eCharCountry));
					ChangeMapLine(message,player,secneid,(txpos << 48) | (typos << 32) |eSceneType);
			}
			break;
			default:
			{
					ChangeMapLine(message,player,secneid,((int64)entry->m_nXPos << 48) | ((int64)entry->m_nYPos << 32) |eSceneType);
			}
		}

}

bool PlayerMessage::IsCanEnterDuplicate(Smart_Ptr<Player>& player, Safe_Smart_Ptr<CommBaseOut::Message> &message, Smart_Ptr<CGameDuplicate>& instance, const InstanceMapEntry* instanceInfo)
{
		//等级限制
		if(instanceInfo->m_nLevel > DWORD(player->getAttManage()->getValue<int>(eCharLv)))
				return false;

		//副本传副本处理
		if(GET_MAP_TYPE(player->GetMapID()) >= eCopyMap)
		{
				Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
				if(instance.Get() != NULL)
				{
					InstanceMapEntry* oldInstanceInfo = instance->GetInstanceScript();
					if(oldInstanceInfo!=NULL)
					{
							bool isCanChange=false;
							if(oldInstanceInfo->m_eType == instanceInfo->m_eType)
							{
									if(instanceInfo->m_eType==eChallenge || instanceInfo->m_eType==eHeroInstance || instanceInfo->m_eType==eTreasonKing || instanceInfo->m_eType==ePVP)
									{
											isCanChange = true;
									}
							}

							if(!isCanChange)
							{
									return false;
							}
					}
				}
		}

		//副本检测
		if(!instance->IsCanEnter(player))
		{
			ChangeMap::ChangeMapInfo ack;

			switch(instanceInfo->m_eType)
			{
				case eCountryBoss:
				{
						ack.set_mapid(eCountryBossCantEnter);
						Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_CHANGEMAP, message, &ack);
						Message_Facade::Send(messRet);
						break;
				}

				case eCountryWar:
				{
						ack.set_mapid(eCountryWarCantEnter);
						Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_CHANGEMAP, message, &ack);
						Message_Facade::Send(messRet);
						break;
			   }

				default:
				{
				 break;
				}
			}

			return false;
	  }

	return true;
}

void PlayerMessage::LeaveCurSecne(Smart_Ptr<Player>& player, Safe_Smart_Ptr<CommBaseOut::Message> &message, int64 secenid, Smart_Ptr<CGameDuplicate>& instance, const InstanceMapEntry* instanceInfo)
{
		Smart_Ptr<Object> obj = player;

		int64 txpos=0, typos=0;

		//记录之前的地图信息
		switch(instanceInfo->m_eType)
		{
			case ePVP:
			case	eTreasonKing:
			case	ePersonalInstance:
			case eChallenge:
			case eHeroInstance:
				{
						if(GET_MAP_TYPE(player->GetMapID()) >= eCopyMap)
						{
								Smart_Ptr<CGameDuplicate> oldinstance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
								if((bool)oldinstance)
								{
										OldMapInfo *  oldMapInfo = oldinstance->GetOldMapPoint(player->GetID());
										if(NULL != oldMapInfo)
										{
												instance->SetOldMapPoint(player->GetID(),oldMapInfo->mapid,oldMapInfo->xpos,oldMapInfo->ypos);
										}
								}
						}
						else
						{
								instance->SetOldMapPoint(player->GetID(), player->GetMapID(), player->GetXNewPos(), player->GetYNewPos());
						}

						txpos = instanceInfo->m_nXPos;
						typos = instanceInfo->m_nYPos;
						break;
				}

				case eBeyondSvrBattle:
				{
						instance->SetOldMapPoint(player->GetID(), player->GetMapID(), player->GetXNewPos(), player->GetYNewPos());
						CrossSvrCompetition::Point vecPos;
						int index = -1;
						int type = 0;

						BeyondBattleMgr::GetInstance()->GetPlayerIndexAndType(instance->GetSceneID(), player->GetID(), index, type);
						if(index >= 0)
						{
								CCrossSvrCompeteLoader::GetInstance()->GetCompeteDebutPos(instanceInfo->m_nMapID, index, vecPos, type);
						}

						if(vecPos.nPosX > 0 && vecPos.nPosY > 0)
						{
								txpos = vecPos.nPosX;
								typos = vecPos.nPosY;
						}
						else
						{
								txpos = instanceInfo->m_nXPos;
								typos = instanceInfo->m_nYPos;
						}
						break;
			  }

			case eCountryBoss:
			{
					instance->SetOldMapPoint(player->GetID(), player->GetMapID(), player->GetXNewPos(), player->GetYNewPos());
					GsCountryMgr::GetInstance()->GetRandomPos(txpos,typos,player->getAttManage()->getValue<DWORD>(eCharCountry));
					break;
			}

			case eCountryWar:
			{
					instance->SetOldMapPoint(player->GetID(), player->GetMapID(), player->GetXNewPos(), player->GetYNewPos());
					CountryWarLoader::GetInstance()->GetRandomPos(txpos,typos,player->getAttManage()->getValue<DWORD>(eCharCountry));
					break;
			}
			case eMarryInstance:
			{
				instance->SetOldMapPoint(player->GetID(), player->GetMapID(), player->GetXNewPos(), player->GetYNewPos());
				if(instance->GetParamfValue() == player->GetID())
				{
					MarryInstanceLoader::GetInstance()->GetBothPoint(txpos,typos,true);
				}
				else
				{
					MarryInstanceLoader::GetInstance()->GetBothPoint(txpos,typos,false);
				}

				break;
			}
			case eIntegralInstance:
			{
				instance->SetOldMapPoint(player->GetID(), player->GetMapID(), player->GetXNewPos(), player->GetYNewPos());

				if(instance->GetParamfValue() == player->getAttManage()->getValue<DWORD>(eCharFaction))
				{
					ClanLoader::GetInstance()->GetRandomPos(txpos,typos,true);
				}
				else
				{
					ClanLoader::GetInstance()->GetRandomPos(txpos,typos,false);
				}
				break;
			}
			case eClanBattle:
			{
				instance->SetOldMapPoint(player->GetID(), player->GetMapID(), player->GetXNewPos(), player->GetYNewPos());

				if(instance->GetParamfValue() == player->getAttManage()->getValue<DWORD>(eCharFaction))
				{
					ClanLoader::GetInstance()->GetBirthPos(eCamp_Red,txpos,typos);
				}
				else
				{
					ClanLoader::GetInstance()->GetBirthPos(eCamp_Blue,txpos,typos);
				}
				break;
			}
			case eCountryOverlord:
			{
				instance->SetOldMapPoint(player->GetID(), player->GetMapID(), player->GetXNewPos(), player->GetYNewPos());
				CountryOverlordCarbonLoader::GetInstance()->GetBothPos(player->getAttManage()->getValue<DWORD>(eCharCountry),txpos,typos);

				break;
			}
		 default:
			{
					instance->SetOldMapPoint(player->GetID(), player->GetMapID(), player->GetXNewPos(), player->GetYNewPos());
					txpos = instanceInfo->m_nXPos;
					typos = instanceInfo->m_nYPos;
			}
		}

	  //换图
	 PlayerLeave(message,player,secenid,txpos,typos);

}

DEF_MSG_QUICK_SIMPLE_DEFINE_FUN(PlayerMessage, MSG_SIM_C2GM_UPDATEMARKET_UISTATE)
{
	if (!player)
	{
		return;
	}
	player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Market, eUIHLSmall_Market_HaveSellItem), 0);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(PlayerMessage,MSG_REQ_C2GM_JUMP_POINT)
{
	MarryInstanceInfo::AskJumpPoint req;
	MarryInstanceInfo::RetAskJumpPoint toClient;
	MarryInstanceInfo::GMForceClientJumpMap toOther;

	MapStartPoint info;
	int ret = 0;

	req.ParseFromString(content);
	if (!player)
	{
		return;
	}

	CMapConfig::GetInstance()->GetTransferMapByStartPoint(req.id(), info);
	if(GET_MAP_ID(player->GetMapID()) != info.mapID ||  GET_MAP_ID(player->GetMapID()) != info.tMapID)
	{
		ret = (int64) eStartPointError;
	}

	if (CSceneMgr::GetInstance()->GetDistance(info.xpos, info.ypos,player->GetXNewPos(),player->GetYNewPos()) > STARTPOINT_LENGTH)
	{
		ret = (int64) ePointDistance;
	}

	toOther.set_charid(player->GetID());
	toOther.set_id(req.id());

	if(ret == 0)
	{
		if(GET_MAP_TYPE(player->GetMapID()) >= eCopyMap)
			{
					Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
					if(instance.Get() != NULL)
					{
						InstanceMapEntry* InstanceInfo = instance->GetInstanceScript();
						if(InstanceInfo!=NULL)
						{
								switch(InstanceInfo->m_eType)
								{
								case eMarryInstance:
									{
										if(instance->GetMonsterCount() > 1)
										{
											ret = eMarryStateError;
											break;
										}

										INT64PLAYERMAP players;
										instance->GetAllPlayers(players);
										INT64PLAYERMAP::iterator itr = players.begin();

										for(; itr != players.end(); ++itr)
										{
												if(itr->second->GetID() != player->GetID())
												{
													QuickMessage::GetInstance()->AddSendMessage(itr->second->GetID(), itr->second->GetChannelID(), &toOther, MSG_SIM_GM2C_JUMP_MAP);
												}
										}

											break;
									}
								default:
									{

									}
								}
						}
					}
			}
	}

	toClient.set_ret(ret);
	toClient.set_id(req.id());
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_JUMP_POINT);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(PlayerMessage,MSG_REQ_C2GM_DOWN_POINT)
{
	MarryInstanceInfo::AskDownPoint req;
	MarryInstanceInfo::RetAskDownPoint toClient;
	MarryInstanceInfo::GMForceClientDownMap toOther;

	MapStartPoint info;
	int ret = 0;

	req.ParseFromString(content);
	if (!player)
	{
		return;
	}

	CMapConfig::GetInstance()->GetTransferMapByStartPoint(req.id(), info);
	if(GET_MAP_ID(player->GetMapID()) != info.mapID ||  GET_MAP_ID(player->GetMapID()) != info.tMapID)
	{
		ret = (int64) eStartPointError;
	}

	if (CSceneMgr::GetInstance()->GetDistance(info.xpos, info.ypos,player->GetXNewPos(),player->GetYNewPos()) > STARTPOINT_LENGTH)
	{
		ret = (int64) ePointDistance;
	}

	toOther.set_charid(player->GetKey());
	toOther.set_mapid(info.tMapID);
	toOther.set_xpos(info.tXPos);
	toOther.set_ypos(info.tYPos);

	if(ret == 0)
	{
		if(GET_MAP_TYPE(player->GetMapID()) >= eCopyMap)
			{
					Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
					if(instance.Get() != NULL)
					{
						InstanceMapEntry* InstanceInfo = instance->GetInstanceScript();
						if(InstanceInfo!=NULL)
						{
								switch(InstanceInfo->m_eType)
								{
								case eMarryInstance:
									{
										if(instance->GetMonsterCount() > 1)
										{
											ret = eMarryStateError;
											break;
										}

										INT64PLAYERMAP players;
										instance->GetAllPlayers(players);
										INT64PLAYERMAP::iterator itr = players.begin();

										for(; itr != players.end(); ++itr)
										{
												if(itr->second->GetID() != player->GetID())
												{
													QuickMessage::GetInstance()->AddSendMessage(itr->second->GetID(), itr->second->GetChannelID(), &toOther, MSG_SIM_GM2C_DOWN_MAP);
												}
										}

										break;
									}
								default:
									{

									}
								}
						}
					}
			}

		player->SetXNewPos(info.tXPos);
		player->SetYNewPos(info.tYPos);
		player->SetDataFlag(eBaseInfo);
		player->ClearPath();

		Smart_Ptr<Object> obj = player;
		if (!CSceneMgr::GetInstance()->MoveTo(obj))
		{
			LOG_ERROR(FILEINFO, "player[gs charid=%lld] move  failed ", GET_PLAYER_CHARID(player->GetID()));
		}

		Smart_Ptr<GirlWar> girlWar =player->GetGirlWarManager()->GetGirlWar();
		if(girlWar)
		{
			girlWar->RelocatePos(info.tXPos,info.tYPos);
		}
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_DOWN_POINT);
}


DEF_MSG_QUICK_REQUEST_DEFINE_FUN(PlayerMessage, MSG_REQ_C2GM_REFRESHARENA)
{
	MapInstanceProto::RetArenaRefresh ret;
//	ArenaParam::RefreshInfo info;
//	if (-1 == ArenaLoader::GetInstance()->GetRefreshInfo(player->GetCounterService()->GetNumById(ARENA_ALREADYREFRESH_CNT) + 1,
//			info))
//	{
//		ret.set_err(eItemError);
//	}
//	else
//	{
//		int golden = 0, money = 0, bindGolden = 0;
//		int err = 0;
//		vector<int> vecAttr;
//
//		err = PlayerMessage::GetInstance()->RefreshCostCheck(player, money, golden, bindGolden);
//
//		if (0 != err)
//		{
//			ret.set_err(err);
//		}
//		else
//		{
//
//			if (0 < golden)
//			{
//				player->ChangeGolden(golden, false);
//				vecAttr.push_back(eCharGolden);
//			}
//			if (0 < money)
//			{
//				player->ChangeMoney(money, false);
//				vecAttr.push_back(eCharMoney);
//			}
//			if (0 < bindGolden)
//			{
//				player->ChangeBindGolden(bindGolden, false);
//				vecAttr.push_back(eCharBindGolden);
//			}

			player->GetCounterService()->IncreasePlayerCounter(ARENA_ALREADYREFRESH_CNT, 1);
//			player->SynCharAttribute(vecAttr);

			ret.set_err(0);
			ret.set_alreadyrefresh(player->GetCounterService()->GetNumById(ARENA_ALREADYREFRESH_CNT));

			Smart_Ptr<SvrItem> svrItem;
			ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()), svrItem);

			 Safe_Smart_Ptr<Message> msg = build_message(MSG_REQ_GM2WS_REFRESHARENA, &ret, svrItem.Get(), Request);
			 msg->SetAct(new requestAct(message));
			 msg->SetMessageTime(player->GetID());
			 Message_Facade::Send(msg);

			 return;
//		}
//	}
//
//	REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GM_REFRESHARENA);
}

DEF_MSG_ACK_DEFINE_FUN(PlayerMessage, MSG_REQ_GM2WS_REFRESHARENA)
{
	if (message->GetErrno() == eReqTimeOut)
	{
		return;
	}
	MapInstanceProto::RetArenaRefresh toClient;
	int len = message->GetLength();
	char* buffer = message->GetBuffer(len);

	toClient.ParseFromArray(buffer, len);

	REQ_MSG_QUICK_ACK_FUN(dynamic_cast<requestAct*>(act.Get())->mesReq, &toClient, MSG_REQ_C2GM_REFRESHARENA);
}

DEF_MSG_REQUEST_DEFINE_FUN(PlayerMessage, MSG_REQ_WS2GM_58PLAY_SYSREWARD)
{
	Pay58PlayInfo::SysFBReward meContent;
	Pay58PlayInfo::RetSysFBReward toWorld;
	Smart_Ptr<Player> player;
	char *con;
	int len = 0;
	int ret = 0;

	vector<int> vecType;
	vector<int> vecLock;
	vector<DWORD> vec;
	vector<int64> goods;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(meContent.charid(), player);
	if(player)
	{
//		StatisticInfo::IDAndCostAndGotInfo idAndCostAndGotInfo;
//		idAndCostAndGotInfo.set_beforeid(meContent.activityid());
//		idAndCostAndGotInfo.set_afterid(meContent.activitytype());
//		StatisticInfo::CostAndGotInfo * costAndGotInfo = idAndCostAndGotInfo.mutable_costandgotlist();

		if(meContent.itemid_size() > 0)
		{
			vector<DWORD> goodsIdList;
			vector<DWORD> numList;
			vector<DWORD> strengthLvVec;
			vector<WORD> bindVec;

			for(int i = 0; i < meContent.itemid_size();i++)
			{
				const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(meContent.itemid(i));
				if(ginfo==NULL)
					continue;
				goodsIdList.push_back(ginfo->itemID);
				numList.resize(goodsIdList.size(), meContent.itemnum(i));

				strengthLvVec.resize(goodsIdList.size(), ginfo->equipStrengthLv);
				bindVec.resize(goodsIdList.size(), 1);
			}

			if(player->GetContainer(ePackageType)->IsBagEnought(goodsIdList, numList,bindVec) == 0)
			{
				player->GetContainer(ePackageType)->AddItem(goodsIdList, numList, vecLock, bindVec,strengthLvVec,true,efightDefinedNULL);

//				StatisticInfo::CostAndGotInfo costAndGotInfo1;
//				for(size_t i = 0;i < goodsIdList.size();++i)
//				{
//					StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo->add_gotlist();
//					StatisticMgr::AddItemGotInfo(gotInfo,goodsIdList[i],numList[i]);
//
//					const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsIdList[i]);
//					if(ginfo)
//					{
//						eGoodsType type = (eGoodsType)ginfo->type;
//						if(type == eWeapon || type == eArmor || type == eJewelry)
//						{
//							StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo1.add_gotlist();
//							StatisticMgr::AddItemGotInfo(gotInfo,goodsIdList[i],numList[i]);
//						}
//					}
//				}

//				if(vecLock.size() > 0)
//				{
//					if(costAndGotInfo1.gotlist_size() > 0)
//						StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_EquipFrom,eStaMinor_EquipFrom_Activity,&costAndGotInfo1);

//					vector<int>::iterator itVec = vecLock.begin();
//					for(; itVec != vecLock.end(); ++itVec)
//					{
//						vec.push_back((ePackageType << 16) | (*itVec));
//					}
//				}
//
//				player->SendSectorChange(vec);
			}
			else
			{
				ret = ePackageFull;
			}
		}
	}
	else
	{
		ret = eNotHaveOnline;
	}

	toWorld.set_ret(ret);
	toWorld.set_index(meContent.index());
	toWorld.set_type(meContent.type());

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_58PLAY_SYSREWARD, message, &toWorld);
	Message_Facade::Send(messRet);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(PlayerMessage, MSG_REQ_C2GM_CLICK_PLAYER_HEAD)
{
	ServerReturn::ServerRetInt req;
	req.ParseFromString(content);

	GroupMessage::ClickPlayerHead tAck;

	Smart_Ptr<Player> tPlayer;
	GetPlayerByCharid(req.ret(),tPlayer);
	if(tPlayer)
		tAck.set_groupid(tPlayer->GetGroupID());

	REQ_MSG_QUICK_ACK_FUN(message, &tAck, MSG_REQ_C2GM_CLICK_PLAYER_HEAD);

	return ;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(PlayerMessage, MSG_SIM_C2GM_CHANGE_TASK_MAP)
{
	ServerReturn::ServerRetInt tMsg;
	tMsg.ParseFromString(content);

	if(tMsg.ret() != 0)
	{
		const MapStaticInfo *tMapInfo = CMapConfig::GetInstance()->GetStaticInfo(tMsg.ret());
		if(tMapInfo == NULL)
		{
			return ;
		}
		
		int64 sceneid = CSceneMgr::GetInstance()->GetSceneIDAboutPlayerCount(tMapInfo->mapid);

		if(sceneid > 0)    //镖车是要传送到changeMap
		{
			Smart_Ptr<CGameZone> tZone = CSceneMgr::GetInstance()->GetGameZone(sceneid);
			if(tZone)
			{
				tZone->SetOldMapPoint(player->GetID(), player->GetMapID(), player->GetXNewPos(), player->GetYNewPos());
			}
			PlayerLeaveTask(message,player,sceneid,tMapInfo->reliveXPos,tMapInfo->reliveYPos);
		}	
	}
	else
	{
		if(player->GetGameZone() == NULL)
			return;

		const OldMapInfo *tOld = player->GetGameZone()->GetOldMapPoint(player->GetID());
		if(tOld != NULL)
		{
			PlayerLeaveTask(message,player,tOld->mapid,tOld->xpos,tOld->ypos);
		}
		else
		{
			const MapStaticInfo* tMapInfo = CMapConfig::GetInstance()->getTaskDefault();
			if(tMapInfo != NULL)
			{
				int64 sceneid = CSceneMgr::GetInstance()->GetSceneIDAboutPlayerCount(tMapInfo->mapid);
				PlayerLeaveTask(message,player,sceneid,tMapInfo->reliveXPos,tMapInfo->reliveYPos);
			}
		}

	}

	return ;	
}
