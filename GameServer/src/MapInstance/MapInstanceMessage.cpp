/*
 * MapInstanceMessage.cpp
 *
 *  Created on: 2014年6月3日
 *      Author: root
 */

#include "MapInstanceMessage.h"
#include "../ServerManager.h"
#include <math.h>
#include "Log/Logger.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/CharLogin/MapInstanceProto.pb.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Map/GameZone.h"
#include "../Map/SceneManager.h"
#include "../MessageBuild.h"
#include "../Map/ConfigLoader/MapConfig.h"
#include "../Map/DuplicateMgr.h"
#include "../MsgCommonClass.h"
#include "Smart_Ptr.h"
#include <netinet/in.h>
#include "MessageCommonRet.h"
#include <string.h>
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/Map/SynchPath.pb.h"
#include "MessageStruct/Map/HeroInstance.pb.h"
#include "../Counter/Counter.h"
#include "CounterDefine.h"
#include "../Mail/MailMessage.h"
#include "InstanceMapLoader.h"
#include "../CDTime/CDMgr.h"
#include "./SvrConfig.h"
#include "../Object/FileLoader/NPCLoader.h"
#include "CounterDefine.h"
#include "MessageStruct/Clan/ClanBattle.pb.h"
#include "../Clan/ClanLoader/ClanLoader.h"
#include "../Chat/ChatMgr.h"
#include "../Activity/FileLoader/TreasonKingLoader.h"
#include "../Activity/FileLoader/ProtectedKingLoader.h"
#include "../Activity/ActivityMgr.h"
#include "BeyondBattleMgr.h"
#include "MessageStruct/BeyondServerBattle/BeyondBattle.pb.h"
#include "./CrossSvrCompetitionLoader/CrossSvrCompetitionLoader.h"
#include "../ServerEventDefine.h"
#include "../EventSet/EventDefine.h"
#include "./FileLoader/InternationalLoader.h"
#include "../SysSetting/SysSettingLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "./FileLoader/GameParaLoader.h"
#include "../ActiveUIHighLight/CActiveHighLightMgr.h"
#include "MessageStruct/StatisticInfo.pb.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "MessageStruct/Marry/MarryInstance.pb.h"
#include "MessageStruct/Map/HeroInstance.pb.h"
#include "../Marry/MarryLoader/MarryInstanceLoader.h"
#include "MessageStruct/Map/ChangeMap.pb.h"
#include "../Object/GirlWar/GirlWarManager.h"
#include "../MapCarbon/GuideCarbon.h"
#include "FileLoader/ArenaLoader.h"
#include "../Arena/ArenaMgr.h"
#include "../StatisticMgr/StatisticHandle.h"
#include "PlayerChallenge.h"
#include "HeroInfo.h"
#include "../MapCarbon/ChallengeCarbon.h"

MapInstanceMessage::MapInstanceMessage()
{
	//////////////////////////////////////////////new instance///////////////////////////////////////////////////////////////
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_LEAVE_CLANWAR);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_MATCH_PLAYER);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_ALREADY_MATCH_PLAYER);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_CANCEL_MATCH_PLAYER);

	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2WS_CREATEINSTANCE);
//	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2DB_ARENAINFO);
//	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2WS_SYNCARENA);
//	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REG_GM2WS_SYNCRANKINGREWARD); //同步竞技场信息

	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GM_CREATEINSTANCE);//创建副本
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GM_MAP_ALL_CREATURE);
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GM_BEYOND_SERVER_INFO);
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GM_GET_BEYOND_REWARD);

	//////////////////////////////////////////////new instance///////////////////////////////////////////////////////////////
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GT_SWEEPINSTANCE);		//扫荡副本
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GT_SWEEPSPEEDUP);		//扫荡加速
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GT_RESETINSTANCE);		//重置副本
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GT_RESETALLINSTANCE);		//重置所有副本
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GT_OPENINSTANCESWEEP);
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GT_GETREWARDS);			//领取奖励
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage,MSG_REQ_C2GT_INSTANCETEAMINFO);	//获取副本队伍信息

	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GT_INSTANCEGROUPREADY);	//组队副本准备信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GT_INSTANCEGROUPENTER);	//组队副本进入信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GT_MEMBERFLOPINFO);			//组队副本翻牌信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GM_UPDATEINSTANCEMONSTERPOS); //将副本怪物所有的坐标点同步给客户端

	DEF_MSG_QUICK_SIMPLE_REG_FUN(MapInstanceMessage, MSG_REQ_C2GT_UPDATEINSTANCE);
	DEF_MSG_QUICK_SIMPLE_REG_FUN(MapInstanceMessage, MSG_REQ_C2GT_SWEEPINSTANCEEND);	//扫荡副本结束



//
//	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GM_RANKINGREWARD);

	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_GROUP_LIMIT);

	DEF_MSG_REQUEST_REG_FUN(eGameServer,MSG_REQ_WS2GM_INVITLIMIT);

	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GM_LOTTERY);//VIP副本抽奖

	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GM_MATCH_PLAYER);//匹配对手

	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2WS_MATCH_PLAYER); //匹配对手


	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GM_SWEEPALLINSTANCE);//请求一键扫荡
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GM_STOPSWEEPALLINSTANCE);//取消一键扫荡
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GM_ASKINSTANCERANK); //请求副本排名

	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GM_FINISH_GUIDE);	//完成指引

//	DEF_MSG_QUICK_SIMPLE_REG_FUN(MapInstanceMessage, MSG_SIM_C2GM_TEAMINSTANCENOTREADY); //踢除未准备的玩家并进入副本

	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_MARRY_CARBON_LIMIT);

	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GM_SUPPORT_ANOTHER);

	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GS_HEROINFO);//请求英雄副本信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GS_FITCH_HERO_BOX);//请求领取宝箱额外奖励

	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GM_GROUPINSTANCE_INFO);//请求组队副本信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage, MSG_REQ_C2GM_SHOPPING_GROUPINSTANCE);//请求购买组队副本

	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage,MSG_REQ_C2GM_ELITE_INFO);//获取精英副本信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage,MSG_REQ_C2GM_CAUCHEMAR_INFO);//获取恶梦副本信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage,MSG_REQ_C2GM_ADVANCED_INFO); //获取进进阶副本信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage,MSG_REQ_C2GM_INSTANCE_INFO); //获取普通副本信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage,MSG_REQ_C2GM_EXP_INFO); //获取经验副本信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(MapInstanceMessage,MSG_REQ_C2GM_MONEY_INFO); //获取铜钱副本信息
}

MapInstanceMessage::~MapInstanceMessage()
{

}

void MapInstanceMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2WS_CREATEINSTANCE);

//	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2WS_SYNCARENA);
//	DEF_MSG_ACK_DISPATCH_FUN(MSG_REG_GM2WS_SYNCRANKINGREWARD); //同步竞技场信息
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2WS_MATCH_PLAYER); //同步竞技场信息

	DEF_SWITCH_TRY_DISPATCH_END
}

void MapInstanceMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_GROUP_LIMIT);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_MARRY_CARBON_LIMIT);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_INVITLIMIT);

	DEF_SWITCH_TRY_DISPATCH_END
}

void MapInstanceMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_LEAVE_CLANWAR);

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_MATCH_PLAYER);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_ALREADY_MATCH_PLAYER);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_CANCEL_MATCH_PLAYER);

	DEF_SWITCH_TRY_DISPATCH_END
}

//扫荡副本
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GT_SWEEPINSTANCE)
{
	MapInstanceProto::ReqInstanceSweep req;
	MapInstanceProto::RetInstanceSweep toClient;
	MapInstanceProto::SimSweepInstanceAward info;
	int sceneId = 0;
	int res = 0;

	if(!player)
	{
		return;
	}


	req.ParseFromString(content);
	sceneId = req.instanceid();

	res = player->GetInstanceMgr()->NewStartSweep(sceneId,info,true);
	toClient.set_dwerrorcode(res);
	toClient.set_dwremaintime(0);
	toClient.set_instanceid(sceneId);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GT_SWEEPINSTANCE);
}

//扫荡加速
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GT_SWEEPSPEEDUP)
{
	return;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GT_OPENINSTANCESWEEP)
{
	MapInstanceProto::VipInstance list;
	if(!player)
	{
		return;
	}

	player->GetInstanceMgr()->GetVipInstanceInfo(list);
	//添加挑战副本信息
//	int id = CHALLEGEMAPFIRSTID;
//	int countnum = player->GetCounterService()->GetNumById(CHALLEGE_INSTANCE_COUNTER);
//	list.set_challegeid(id);
//	list.set_challegereset(countnum);

	REQ_MSG_QUICK_ACK_FUN(message, &list, MSG_REQ_C2GT_OPENINSTANCESWEEP);
}

//重置副本
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GT_RESETINSTANCE)
{
	MapInstanceProto::ReqInstanceReset req;
	MapInstanceProto::RetInstanceReset toClient;

	req.ParseFromString(content);

	if(!player)
	{
		return;
	}

	BEHAVIOR_MONI(player, BehaviorType::Main::eInstance, BehaviorType::Minor::eInstance_ResetAllInst, MONITOR);

	int res = player->GetInstanceMgr()->ResetInstance(req.instanceid());

	

	toClient.set_dwerrorcode(res);
	toClient.set_instanceid(req.instanceid());

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GT_RESETINSTANCE);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GT_RESETINSTANCE);
}

//重置所有副本
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GT_RESETALLINSTANCE)
{
	ServerReturn::ServerResultInt req;
	ServerReturn::ServerRetInt ret;
	req.ParseFromString(content);

	int code=0;
	player->GetInstanceMgr()->ResetAllInstance(req.ret(),code);

	//高位为类型，低位为错误码
	int64 retCode=req.ret();
	retCode = retCode<<32|code;
	ret.set_ret(retCode);

	REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GT_RESETALLINSTANCE);
}

//组队副本准备信息
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GT_INSTANCEGROUPREADY)
{

}

//组队副本进入信息
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GT_INSTANCEGROUPENTER)
{

}


//组队副本翻牌信息
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GT_MEMBERFLOPINFO)
{
	MapInstanceProto::ReqMemberFlop req;
	MapInstanceProto::GMToClientMemberFlop toClient;
	int error = 0;

	req.ParseFromString(content);
	if(!player)
	{
		return;
	}

	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
	if (!entry)
	{
		LOG_DEBUG(FILEINFO,"InstanceMgr::GetAwardToPlayer: intanceId is invalid %d.",GET_MAP_ID(player->GetMapID()));
		return;
	}

	if(entry->m_eType == eGroupInstance)
	{
		Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
		if(!instance)
		{
			error = eInstanceNotExsitence;
		}
		else if(!instance->GetSuccess())
		{
			error = eInstanceInfoError;
		}
		else
		{
			error = instance->SetPlayerInstanceFlops(player->GetID(),req.flopid());
		}
	}
	else
	{
		error = eMapIdError;
	}

	toClient.set_ret(error);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GT_MEMBERFLOPINFO);
}

//领取奖励
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GT_GETREWARDS)
{
	return;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GT_INSTANCETEAMINFO)
{
	return;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_UPDATEINSTANCEMONSTERPOS)
{
	PlayerSynch::ReqInstanceMonsterPos req;
	PlayerSynch::RetInstanceMonsterPos toClient;

	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
	if (!entry)
	{
		LOG_DEBUG(FILEINFO,"InstanceMgr::GetAwardToPlayer: intanceId is invalid %d.",GET_MAP_ID(player->GetMapID()));
		return ;
	}

	if(entry->m_eType == ePersonalInstance || entry->m_eType == eGroupInstance || entry->m_eType == eVipInstance || entry->m_eType == eTreasonKing || entry->m_eType == eProtectKing ||
			entry->m_eType == eMoneyInstance || entry->m_eType == eMarryInstance || entry->m_eType == eEliteInstance || entry->m_eType == eCauchemarInstance || entry->m_eType == eAdvancedInstance
			|| entry->m_eType == eExpInstance || entry->m_eType == eMoneyInstance1)
	{
		Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
		if(instance)
		{
			instance->GetAllMonsterPosInfo(toClient);
		}
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_UPDATEINSTANCEMONSTERPOS);
}


DEF_MSG_QUICK_SIMPLE_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GT_UPDATEINSTANCE)
{

}

//扫荡结束
DEF_MSG_QUICK_SIMPLE_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GT_SWEEPINSTANCEEND)
{

}




DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_CREATEINSTANCE)
{
	MapInstanceProto::ClientReqCreateInstance req;
	MapInstanceProto::RetCreateInstance toClient;
	Smart_Ptr<CGameDuplicate> instance;
	int ret = 0;

	req.ParseFromString(content);

	InstanceMapEntry * entry = InstanceLoader::GetInstance()->GetEntry(req.instanceid());
	if(!entry)
		return;

	if(player->IsDead())
	{
		return;
	}

	//处于副本组队中,不能进入副本
	if(entry->m_eType != eGroupInstance && player->getAttManage()->getValue<int64>(eTeamId) > 0)
	{
		ret = eInTeamInstance;
	}

	switch(entry->m_eType)
	{
		case eChallenge:
		case eHeroInstance:
		{
			break;
		}
		case eTreasonKing:
		{
			break;
		}
		default:
		{
			if(ret == 0 && GET_MAP_TYPE(player->GetMapID()) >= eCopyMap)
			{
				ret = eInInstanceMap;
			}
		}
	}


	if(ret == 0)
	{
		if((DWORD)player->getAttManage()->getValue<int>(eCharLv) < entry->m_nLevel)
		{//等级限制
			ret = eLVNotMatch;
		}
		else
		{
			switch(req.instancetype())
			{
				case ePVEMap:///pve副本
				{
					switch(entry->m_eType)
					{
						case ePersonalInstance://个人副本
						{
							if(player->GetInstanceMgr()->GetLeftValidTimes(req.instanceid()) <= 0)
							{
								ret =  eInstanceTimesNotEnough;
							}
							break;
						}
						case eChallenge:
						{
							ret = player->GetChallenge()->CreateChecke(req.instanceid(), req.paramf());
							break;
						}

						case eHeroInstance:
						{
							ret = player->GetHeroMgr()->CreateChecke(req.instanceid(), req.paramf());
							break;
						}

						case eCompeteInstance://竞技场副本
						{
							Smart_Ptr<CDMgr> cd = player->GetCDMgr();
							if(!cd)
								return;


							if(!player->IsOpenSystem(eStCompeteInstance))
							{
								ret = eNotOpen;
							}
							else if(player->getAttManage()->getValue<int>(eRanking) <= 0)
							{
								ret = eRankingIsNull;
							}
							else if(player->getAttManage()->getValue<int>(eRanking)== req.paramf())
							{
								ret = eCantFightThisRanking;
							}
							else if (0 >= player->GetArenaMgr()->GetCanChallCnt())
							{
								ret = eInstanceTimesNotEnough;
							}

							break;
						}
						case eTreasonKing:   //剿灭反王
						{
							Smart_Ptr<CounterService> & counter = player->GetCounterService();
							if(!counter)
								return;

//							if(GET_MAP_TYPE(player->GetMapID()) < eCopyMap)
//							{
//								counter->SetPlayerCounter(TREASON_KING_LEVELS,0);
//							}

							CSceneMgr::GetInstance()->GetInstance(player->GetMapID(),instance);

							if(GET_MAP_TYPE(player->GetMapID()) >= eCopyMap && !instance)
							{
								LOG_DEBUG(FILEINFO, "TreasonKing  Instance is NULL");
								return;
							}

							// 是否是VIP
							int reaptTimes = entry->m_nReaptTimes; // 重复次数;

							if(counter->GetNumById(TREASON_KING_ENTER) >= reaptTimes)
							{
								ret = eInstanceTimesNotEnough;
							}
							else if(req.instanceid() != TreasonKingLoader::GetInstance()->GetLevelsMapId(counter->GetNumById(TREASON_KING_LEVELS)+1))
							{
								ret = eTreasonAlreadyReset;
							}
							else if(GET_MAP_TYPE(player->GetMapID()) >= eCopyMap && !instance->GetSuccess())
							{
								ret = eTreasonKingCanNotInstance;
							}
							else if(player->getAttManage()->getValue<int>(eCharGolden) < TreasonKingLoader::GetInstance()->GetLevelsGold(counter->GetNumById(TREASON_KING_LEVELS)))
							{
								ret =eNotEnogGolden;
							}
							break;
						}
						case eProtectKing:
							{
								Smart_Ptr<ActivityMgr> activity = player->GetActivityMgr();

								if(!activity)
								{
									ret = eSystemError;
								}
								else if(activity->GetFinishTime())
								{
									ret = eInInstanceSweep;
								}
								else if(activity->IsHighestNode())
								{
									ret = eInstanceHighestNode;
								}

								break;
							}
						case eEliteInstance:
						case eCauchemarInstance:
						case eGroupInstance:
						case eExpInstance:
						case eMoneyInstance1:
							{
								if(player->GetInstanceMgr()->GetLeftValidTimes(req.instanceid()) <= 0)
								{
									ret =  eInstanceTimesNotEnough;
								}

								break;
							}
						case eVipInstance:
							{
								if(false == player->IsOpenSystem(eStVipInstanceLv))
								{
									ret = eNotOpen;
								}
								else if( entry->m_nVipLimit > player->getAttManage()->getValue<DWORD>(eVIPLv))
								{
									ret = eVipLevelNotEnough;
								}
								else if(player->GetInstanceMgr()->GetLeftValidTimes(req.instanceid()) <= 0)
								{
									ret =  eInstanceTimesNotEnough;
								}

								break;
							}

						case eMoneyInstance:
						{
							//有次数或者元宝就可以进入, 扣元宝时，在进入场景成功后完成
							int surplusNum = 0;
							ret = player->GetInstanceMgr()->checkMoneyInstanceEnter(surplusNum);
							break;
						}

						case eGuideInstance:
						{
							if(player->getAttManage()->getValue<int>(eGuide) > 0)
							{
								ret = eInstanceTypeError;
							}
							break;
						}
						case eAdvancedInstance:
						{
							if(player->GetInstanceMgr()->GetLeftValidTimes(req.instanceid()) <= 0)
							{
								ret =  eInstanceTimesNotEnough;
							}

							break;
						}
						default:
							ret = eInstanceTypeError;
							break;
					}

					break;
				}
				default:
					ret = eInstanceTypeError;
					break;
			}
		}
	}

	//运镖中，不能进入副本
	if(ret == 0 && player->IsInCreatureState(ERS_Escort))
	{
		ret = eEscortNotIntoInstance;
	}

	//有没有正在创建的副本
	if(ret == 0 && !DuplicateMgr::GetInstance()->IsCreateInstance(player->GetID()))
	{
		ret = eHasInstance;
	}

	if(ret == 0)
	{
		//请求世界服创建副本
		MapInstanceProto::GMReqWSCreateInstance toWorld;
		Smart_Ptr<SvrItem> worldSvr;

		toWorld.set_instanceid(req.instanceid());
		toWorld.set_instancetype(req.instancetype());
		toWorld.set_charid(player->GetID());
		toWorld.set_paramf(req.paramf());

		ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),worldSvr);
		if(!worldSvr)
			return;

		DuplicateMgr::GetInstance()->AddWillCreateInstance(player->GetID());

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GM2WS_CREATEINSTANCE, &toWorld, worldSvr.Get(), Request,30);
		messRet->SetAct(new FirstAct(message, player->GetID()));
		Message_Facade::Send(messRet);

		return;
	}

	toClient.set_dwerrorcode(ret);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_CREATEINSTANCE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_MAP_ALL_CREATURE)
{
	ClanPackage::ClientReqMMapPlayer toClient;

	if(GET_MAP_TYPE(player->GetMapID()) == ePVEMap)
	{
		Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
		if(instance)
		{
			instance->GetAllPlayerPosition(toClient, player->getAttManage()->getValue<DWORD>(eCharFaction), player->GetKey());
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_MAP_ALL_CREATURE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_BEYOND_SERVER_INFO)
{
	ServerBattle::AckBeyondBattle toClient;

	if(player->IsOpenSystem(eStBeyondSvrBattle) == false)
		return;

	toClient.set_count(player->GetCounterService()->GetNumById(BEYOND_BATTLE));
	toClient.set_score(player->GetSvrScore());
	toClient.set_reward(player->GetCounterService()->GetNumById(BEYOND_BATTLE_REWARD));
	toClient.set_single((CCrossSvrCompeteLoader::GetInstance()->GetSingleInvit()<< 16) | player->GetCounterService()->GetNumById(BEYOND_SINGLE_BATTLE));
	toClient.set_group((CCrossSvrCompeteLoader::GetInstance()->GetGroupInvit()<< 16) | player->GetCounterService()->GetNumById(BEYOND_GROUP_BATTLE));
	toClient.set_cdtime(player->GetCDMgr()->GetCDEndTime(BEYOND_CHANNEL));

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_BEYOND_SERVER_INFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_GET_BEYOND_REWARD)
{
	ServerBattle::AckGetBeyondReward toClient;

	if(player->IsOpenSystem(eStBeyondSvrBattle) == false)
		return;

	if(player->GetCounterService()->GetNumById(BEYOND_BATTLE) < CCrossSvrCompeteLoader::GetInstance()->GetAcceptPrizeCountCeiling())
	{
		toClient.set_ret(eNotGetReward);
	}
	else if(player->GetCounterService()->GetNumById(BEYOND_BATTLE_REWARD) >= 1)
	{
		toClient.set_ret(eHaveGetReward);
	}
	else
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eInstance, BehaviorType::Minor::eInstance_GetCrossArenaAward, MONITOR);

		vector<DWORD> itemid;
		vector<DWORD> itemnum;
		vector<int> vecPos;
		int res = 0;
		int force = 0;
		int money = 0;
		vector<int> vec;
		int index = CCrossSvrCompeteLoader::GetInstance()->GetGradeIDByScore(player->GetSvrScore());

		CCrossSvrCompeteLoader::GetInstance()->GetVictroyRewardExpMoney(index, force, money);
		CCrossSvrCompeteLoader::GetInstance()->GetVictoryRewardGoods(index, itemid, itemnum);

		vector<WORD> bindVec;
		bindVec.resize(itemid.size(), 1);

		res = player->GetContainer(ePackageType)->IsBagEnought(itemid, itemnum,bindVec);
		if(!res)
		{
			vector<DWORD> strengthLvVec;

			strengthLvVec.resize(itemid.size(), 0);

			player->GetContainer(ePackageType)->AddItem(itemid, itemnum, vecPos, bindVec,strengthLvVec,true);

			player->ChangeCurForce(force, true);
			player->ChangeMoney(money, true);
			player->GetCounterService()->IncreasePlayerCounter(BEYOND_BATTLE_REWARD, 1);
			toClient.set_ret(0);

			vec.push_back(eCharForce);
			vec.push_back(eCharMoney);
			player->SynCharAttribute(vec);

//			StatisticInfo::CostAndGotInfo costAndGotInfo;
//
//			for(uint i = 0;i < itemid.size();++i)
//			{
//				StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo,itemid[i],itemnum[i]);
//			}
//
//			if(force > 0)
//			{
//				StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo,eCharForce,force);
//			}
//
//			if(money > 0)
//			{
//				StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,money);
//			}
//
//			if(eAccountType_User == player->GetAccountType())
//				StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Reward,eStaMinor_Reward_CrossArea,&costAndGotInfo);
		}
		else
		{
			toClient.set_ret(res);
		}

		

	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_GET_BEYOND_REWARD);
}

DEF_MSG_ACK_DEFINE_FUN(MapInstanceMessage, MSG_REQ_GM2WS_CREATEINSTANCE)
{
	Smart_Ptr<Player> player;
	if (message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request worldserver create map and ack timeout");
		PlayerMessage::GetInstance()->GetPlayerByCharid(static_cast<FirstAct *>(act.Get())->mesDataf, player);
		if(player)
		{
			//删除创建列表
			DuplicateMgr::GetInstance()->DeleteWillCreateInstance(player->GetID());
		}

		return;
	}

	LOG_DEBUG(FILEINFO, "gameserver request worldserver create map");

	MapInstanceProto::AckToGMCreateInstance req;
	MapInstanceProto::RetCreateInstance toClient;
	char *con;
	int len = 0;

	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(static_cast<FirstAct *>(act.Get())->mesDataf, player);
	if(player)
	{
		if(req.ret() == 0)
		{
			toClient.set_newinstanceid(req.instanceid());

//			int treasonKingMapID = GameParaLoader::GetInstance()->getTreasonKingMapId();
//			if(treasonKingMapID <= req.instanceid() && req.instanceid() <= treasonKingMapID + 8)
//			{
//				Smart_Ptr<CounterService> & counter = player->GetCounterService();
//				if(counter)
//				{
//					StatisticInfo::IDAndCostAndGotInfo idAndCostAndGotInfo;
//					idAndCostAndGotInfo.set_beforeid(req.instanceid());
//					StatisticInfo::CostAndGotInfo * costAndGotInfo = idAndCostAndGotInfo.mutable_costandgotlist();
//					StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo->add_costlist();
//					int golden = TreasonKingLoader::GetInstance()->GetLevelsGold(counter->GetNumById(TREASON_KING_LEVELS));
//					costItemInfo->set_beforenum(player->getAttManage()->getValue<int>(eCharGolden) + golden);
//					StatisticMgr::SetItemCost(costItemInfo,GameParaLoader::GetInstance()->getExchangeGoldenItemId(),golden);
//					StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,player);
//					if(eAccountType_User == player->GetAccountType())
//						StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_TreasonKing,eStaMinor_TreasonKing_EnterEvent,&idAndCostAndGotInfo);
//				}
//			}

			player->setBWillInInstance(true);
		}

		//删除创建列表
		DuplicateMgr::GetInstance()->DeleteWillCreateInstance(player->GetID());

		toClient.set_dwerrorcode(req.ret());

		REQ_MSG_QUICK_ACK_FUN(static_cast<FirstAct *>(act.Get())->mesReq, &toClient, MSG_REQ_C2GM_CREATEINSTANCE);
	}
}
//
//DEF_MSG_ACK_DEFINE_FUN(MapInstanceMessage, MSG_REQ_GM2DB_ARENAINFO)
//{
//	if (message->GetErrno() == eReqTimeOut)
//	{
//		LOG_WARNING(FILEINFO, "gameserver request dbserver ranking player and ack timeout");
//
//		return;
//	}
//
//	LOG_DEBUG(FILEINFO, "gameserver request dbserver ranking player");
//
//	PlayerInfo::ArenaInfo req;
//	char *con;
//	int len = 0;
//	Smart_Ptr<Player> player;
//
//	con = message->GetBuffer(len);
//	req.ParseFromArray(con, len);
//
//	PlayerMessage::GetInstance()->GetPlayerByCharid(static_cast<IntAct *>(act.Get())->mesData, player);
//	if(player)
//	{
//		if (GameParaLoader::GetInstance()->getArenaMapId() != GET_MAP_ID(player->GetMapID()))
//		{
//			return;
//		}
//
//		Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(player->GetMapID());
//		if(!zone)
//			return;
//
//		zone->InitMonsterByPlayer(player, req);
//	}
//}
//
//DEF_MSG_ACK_DEFINE_FUN(MapInstanceMessage, MSG_REQ_GM2WS_SYNCARENA)
//{
//	if (message->GetErrno() == eReqTimeOut)
//	{
//		LOG_WARNING(FILEINFO, "gameserver request worldserver synch player info and ack timeout");
//
//		return;
//	}
//
//	LOG_DEBUG(FILEINFO, "gameserver request worldserver  synch player info ");
//
//	ServerReturn::ServerDoubleInt req;
//	char *con;
//	int len = 0;
//	Smart_Ptr<Player> player;
//
//	con = message->GetBuffer(len);
//	req.ParseFromArray(con, len);
//
//	PlayerMessage::GetInstance()->GetPlayerByCharid(req.retf(), player);
//	if(player)
//	{
//		if(req.rets() > 0)
//		{
//			int beforeRank = player->getAttManage()->getValue<int>(eRanking);
//			player->getAttManage()->setValue(eRanking,req.rets());
////			if (0 == beforeRank && player->GetArenaMgr())
////			{
////				player->GetArenaMgr()->OpenRealData();
////			}
//			if(req.rets() < beforeRank)
//			{
//				OneParmaEventArgs e(player->GetMyself());
//				player->FireEvent(PLAYER_RANKING_CHANGE,e);
//			}
//		}
//	}
//}

DEF_MSG_SIMPLE_DEFINE_FUN(MapInstanceMessage, MSG_SIM_WS2GM_LEAVE_CLANWAR)
{
	ClanPackage::ToGameClanDelete meContent;
	char *con;
	int len = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	if(meContent.only() > 0)
	{
		Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(meContent.only());
		if(instance)
		{
			instance->ClanDelete(meContent.cid());
		}
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(MapInstanceMessage, MSG_SIM_WS2GM_MATCH_PLAYER)
{
	ServerBattle::BroadPlayerMatch meContent;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;
	WaitMatch item;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	if(meContent.charid() > 0)
	{
		if(!BeyondBattleMgr::GetInstance()->IsAddWaitMatch(meContent.charid()))
			return;

		//直接加入等待队列，让定时器去配对
		BeyondBattleMgr::GetInstance()->AddWaitMatch(meContent.charid(), meContent.lv(), meContent.fight(), meContent.score(), meContent.type());
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(MapInstanceMessage, MSG_SIM_WS2GM_ALREADY_MATCH_PLAYER)
{
	ServerBattle::BroadPlayerIsMatched meContent;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;
	WaitMatch item;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	if(meContent.charid() > 0)
	{
		BeyondBattleMgr::GetInstance()->DeletePlayerInfo(meContent.charid());
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(MapInstanceMessage, MSG_SIM_WS2GM_CANCEL_MATCH_PLAYER)
{
	ServerBattle::BroadPlayerCancelMatch meContent;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;
	WaitMatch item;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	if(meContent.charid() > 0)
	{
		BeyondBattleMgr::GetInstance()->CancelMatch(meContent.charid());
	}
}


DEF_MSG_REQUEST_DEFINE_FUN(MapInstanceMessage, MSG_REQ_WS2GM_GROUP_LIMIT)
{
	ServerReturn::ServerDoubleInt meContent;
	ServerReturn::ServerThreeInt toWS;
	Smart_Ptr<Player> player;
	char *con;
	int len = 0;
	int limit = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);


	PlayerMessage::GetInstance()->GetPlayerByCharid(meContent.retf(), player);
	if(player)
	{
		limit = player->GetInstanceMgr()->GetLeftValidTimes(meContent.rets());
		if (player->IsInCreatureStateAll(ERS_Escort))
		{
			toWS.set_retf(eIsEscorting);
		}
		else if(limit == 0)
		{
			toWS.set_retf(eInstanceTimesNotEnough);
		}
		else if(limit < 0)
		{
			toWS.set_retf(eInstanceNotExsitence);
		}
		else
		{
			toWS.set_retf(player->GetInstanceMgr()->GetLvEnough(meContent.rets(),player->getAttManage()->getValue<int>(eCharLv)));
			toWS.set_rets(meContent.rets());
		}

		toWS.set_rett(player->GetID());

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_GROUP_LIMIT, message, &toWS);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(MapInstanceMessage, MSG_REQ_WS2GM_INVITLIMIT)
{
	ServerReturn::ServerRetInt meContent;
	ServerReturn::ServerDoubleInt toWS;
	Smart_Ptr<Player> player;
	char *con;
	int len = 0;
	int ret = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(meContent.ret(), player);
	if(player)
	{
		if (player->IsInCreatureStateAll(ERS_Escort))
		{
			ret = eIsEscorting;
		}
	}
	else
	{
		ret = eNotHaveOnline;
	}

	toWS.set_retf(ret);
	toWS.set_rets(meContent.ret());
	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_INVITLIMIT, message, &toWS);
	Message_Facade::Send(messRet);
}

//VIP副本抽奖
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_LOTTERY)
{
	return;
}

//匹配对手
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_MATCH_PLAYER)
{
	LOG_DEBUG(FILEINFO, "request match player from client");
	ServerBattle::ClientMatchPlayerType req;
	ServerBattle::ToClientMatchPlayerType toClient;
	ServerBattle::MatchPlayerType toWs;
	int ret = 0;
	int vipTimes = 0;

	req.ParseFromString(content);
	if(!player)
	{
		return;
	}


	if(player->GetVipPart())
	{
		double times = player->GetVipPart()->GetValueByRight(eVipBeyondSvrBattle);
		if (times != VIP_NOT_HAVE_THIS_RIGHT)
		{
			vipTimes = (int)times;
		}
	}

	if(player->IsInCreatureStateAll(ERS_Escort))
	{
		ret = eIsEscorting;
	}
	else if(req.type() != eBeyondBattleOne && req.type() != eBeyondBattleThree)
	{
		ret = eBeyondSvrTypeError;
	}
	else if(player->getAttManage()->getValue<int>(eCharLv) < CCrossSvrCompeteLoader::GetInstance()->GetOpenLv())
	{
		ret = eBeyondSvrLvNotEnough;
	}
	else if(GET_MAP_TYPE(player->GetMapID()) >= eCopyMap)
	{
		ret = eInTeamInstance;
	}
	else if(player->GetCounterService()->GetNumById(BEYOND_GROUP_BATTLE) >= CCrossSvrCompeteLoader::GetInstance()->GetGroupInvit() && req.type() == eBeyondBattleThree)
	{
		ret = eInstanceTimesNotEnough;
	}
	else if(player->GetCounterService()->GetNumById(BEYOND_SINGLE_BATTLE) >= (vipTimes + CCrossSvrCompeteLoader::GetInstance()->GetSingleInvit()) && req.type() == eBeyondBattleOne)
	{
		ret = eInstanceTimesNotEnough;
	}
	else if(player->GetCDMgr()->GetCDTime(BEYOND_CHANNEL) > 0)
	{
		ret = eCDInTime;
	}
	else
	{
		Smart_Ptr<SvrItem> worldSvr;

		ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),worldSvr);
		if(!worldSvr)
			return;

		toWs.set_type(req.type());
		toWs.set_charid(player->GetID());

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GM2WS_MATCH_PLAYER, &toWs, worldSvr.Get(), Request,30);
		messRet->SetAct(new FirstAct(message, player->GetID()));
		Message_Facade::Send(messRet);
	}

	if(ret != 0)
	{
		toClient.set_ret(ret);
		REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_MATCH_PLAYER);
	}
}


DEF_MSG_ACK_DEFINE_FUN(MapInstanceMessage, MSG_REQ_GM2WS_MATCH_PLAYER)
{
	Smart_Ptr<Player> player;
	if (message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request match player and ack timeout");
		return;
	}

	LOG_DEBUG(FILEINFO, "gameserver request match player");

	ServerBattle::ToClientMatchPlayerType req;
	char *con;
	int len = 0;

	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(static_cast<FirstAct *>(act.Get())->mesDataf, player);
	if(player)
	{
		REQ_MSG_QUICK_ACK_FUN(static_cast<FirstAct *>(act.Get())->mesReq, &req, MSG_REQ_C2GM_MATCH_PLAYER);
	}
}

//请求一键扫荡
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_SWEEPALLINSTANCE)
{
	return;
//	MapInstanceProto::GMToClientSweepAllInstance toClient;
//	int ret = 0;
//	if(!player && !player->GetInstanceMgr())
//	{
//		return;
//	}
//
//	ret = player->GetInstanceMgr()->NewStartAllSweep(toClient);
//	toClient.set_ret(ret);
//	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_SWEEPALLINSTANCE);
}

//取消一键扫荡
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_STOPSWEEPALLINSTANCE)
{

	return;
	LOG_DEBUG(FILEINFO, "request cancel one key sweep from client");

//	MapInstanceProto::RetInstanceSweepSpeedUp toClient;
//	int sweepSceneId;
//	int error = 0;
//
//	if(!player && !player->GetInstanceMgr())
//	{
//		return;
//	}
//
//	error = player->GetInstanceMgr()->StopAllSweep(sweepSceneId);
//
//	toClient.set_dwerrorcode(error);
//	toClient.set_instanceid(sweepSceneId);
//
//	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_STOPSWEEPALLINSTANCE);
}

//请求副本排名
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_ASKINSTANCERANK)
{
	LOG_DEBUG(FILEINFO, "request  instance rank from client");

	int elapseTime = 0;
	MapInstanceProto::GetInstanceRanking req;
	MapInstanceProto::ReturnInstanceRanking toClient;
	req.ParseFromString(content);
	if(!player)
	{
		return;
	}

	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(req.mapid());
	if (!entry)
	{
		LOG_DEBUG(FILEINFO,"InstanceMgr::GetAwardToPlayer: intanceId is invalid %d.",req.mapid());
		return;
	}

	switch(entry->m_eType)
	{
	case ePersonalInstance:
		{
			break;
		}
	case eTreasonKing:
		{
			if(player->GetActivityMgr())
			{
				elapseTime = player->GetActivityMgr()->GetTreasonBestTime(req.mapid());
			}

			break;
		}
	default:
		{

		}
	}

	toClient.set_elapsedtime(elapseTime);
	toClient.set_mapid(req.mapid());
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ASKINSTANCERANK);
}


DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_FINISH_GUIDE)
{
	CGameZone *pZone = player->GetGameZone();
	if(pZone==NULL)
	{
		return;
	}

	if(GET_MAP_TYPE(player->GetMapID()) != ePVEMap )
	{
		return;
	}

	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
	if(entry==NULL || entry->m_eType != eGuideInstance)
	{
		return;
	}

	int res = ((GuideCarbon*)(pZone))->Finish(player);

  ServerReturn::ServerResultInt ret;
  ret.set_ret(res);
	REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GM_FINISH_GUIDE);
}




DEF_MSG_REQUEST_DEFINE_FUN(MapInstanceMessage, MSG_REQ_WS2GM_MARRY_CARBON_LIMIT)
{
	ServerReturn::ServerDoubleInt meContent;
	ServerReturn::ServerThreeInt toWS;
	Smart_Ptr<Player> player;
	char *con;
	int len = 0;
	int limit = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);


	PlayerMessage::GetInstance()->GetPlayerByCharid(meContent.retf(), player);
	if(player)
	{
		limit = player->GetInstanceMgr()->GetLeftValidTimes(meContent.rets());
		if (player->IsInCreatureStateAll(ERS_Escort))
		{
			toWS.set_retf(eIsEscorting);
		}
		else if(limit == 0)
		{
			toWS.set_retf(eInstanceTimesNotEnough);
		}
		else if(limit < 0)
		{
			toWS.set_retf(eInstanceNotExsitence);
		}
		else if(GET_MAP_TYPE(player->GetMapID()) >= eCopyMap)
		{
			toWS.set_retf(eInInstanceMap);
		}
		else
		{
			toWS.set_retf(player->GetInstanceMgr()->GetLvEnough(meContent.rets(),player->getAttManage()->getValue<int>(eCharLv)));
			toWS.set_rets(meContent.rets());
		}

		toWS.set_rett(player->GetID());

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_MARRY_CARBON_LIMIT, message, &toWS);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_SUPPORT_ANOTHER)
{
	MarryInstanceInfo::RetAskSupportOther toClient;
	MarryInstanceInfo::GMForceClientDownMap toOther;
	int ret = 0;
	int tPos = 0;
	vector<BothPoint> vec;
	if(!player)
	{
		return;
	}

	MapStaticInfo *info;
	info = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(player->GetMapID()));
	if(!info)
	{
		ret = eMapNotExist;
	}

	Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
	if(instance.Get() == NULL)
	{
		ret = eInstanceNotExsitence;
	}
	else
	{
		InstanceMapEntry *mapptr = instance->GetInstanceScript();
		if(!mapptr || mapptr->m_eType != eMarryInstance)
		{
			ret = eMapIdError;
		}
	}


	MarryInstanceLoader::GetInstance()->GetBothPoint(vec);
	if(vec.size() < 2)
	{
		ret = eStartPointError;
	}

	if(ret == 0)
	{
		if (CSceneMgr::GetInstance()->GetDistance(vec[0].nXpos, vec[0].nYpos,player->GetXNewPos(),player->GetYNewPos()) <
		CSceneMgr::GetInstance()->GetDistance(vec[1].nXpos, vec[1].nYpos,player->GetXNewPos(),player->GetYNewPos()))
		{
			tPos = 1;
		}

		player->CanccelMeditation();
		player->SetXNewPos(vec[tPos].nXpos);
		player->SetYNewPos(vec[tPos].nYpos);
		player->SetDataFlag(eBaseInfo);
		player->ClearPath();

		Smart_Ptr<Object> obj = player;

		if (!CSceneMgr::GetInstance()->MoveToAll(obj))
		{
			LOG_ERROR(FILEINFO, "player[gs charid=%lld] move  failed ", GET_PLAYER_CHARID(player->GetID()));
		}

		ChangeMap::GMForceClientChangeMap toClient;

		toClient.set_mapid(GET_MAP_ID(player->GetMapID()));
		toClient.set_xpos(vec[tPos].nXpos);
		toClient.set_ypos(vec[tPos].nYpos);

		toClient.set_pktype(player->GetPkTypeByMap(info->pktype));

		QuickMessage::GetInstance()->AddSendMessage( player->GetID(), player->GetChannelID(), &toClient, MSG_SIM_GM2C_FORCE_CHANGEMAP);

		Smart_Ptr<GirlWar> girlWar =player->GetGirlWarManager()->GetGirlWar();
		if(girlWar)
		{
			girlWar->RelocatePosToAll(vec[tPos].nXpos,vec[tPos].nYpos);
		}
	}

	toClient.set_ret(ret);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_SUPPORT_ANOTHER);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage, MSG_REQ_C2GS_HEROINFO)
{
	HeroPackage::AckHeroInfo ack;

	player->GetHeroMgr()->GetHeroData(&ack);

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GS_HEROINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage, MSG_REQ_C2GS_FITCH_HERO_BOX)
{
	HeroPackage::ReqFitchPrize req;
	HeroPackage::AckFitchPrize ack;

	req.ParseFromString(content);

	int ret = player->GetHeroMgr()->FitchBox(req.layer());

	ack.set_layer(req.layer());
	ack.set_result(ret);
	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GS_FITCH_HERO_BOX);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_GROUPINSTANCE_INFO)
{
	MapInstanceProto::GroupInstance toClient;
	if(!player)
	{
		return;
	}

	player->GetInstanceMgr()->GetGroupInstanceInfo(toClient);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_GROUPINSTANCE_INFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_SHOPPING_GROUPINSTANCE)
{
	int ret = 0;
	MapInstanceProto::ReqCostByGroupInstance req;
	MapInstanceProto::RetCostByGroupInstance toClient;
	req.ParseFromString(content);
	if(!player)
	{
		return;
	}

	ret = player->GetInstanceMgr()->ShopGroupInstance(req.mapid(),toClient);
	toClient.set_ret(ret);
	toClient.set_mapid(req.mapid());
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_SHOPPING_GROUPINSTANCE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_ELITE_INFO)
{
	MapInstanceProto::EliteInstance toClient;
	if(!player)
	{
		return;
	}

	player->GetInstanceMgr()->GetEliteInstanceInfo(toClient);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ELITE_INFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_CAUCHEMAR_INFO)
{
	MapInstanceProto::CauchemarInstance toClient;
	if(!player)
	{
		return;
	}

	player->GetInstanceMgr()->GetCauchemarInstanceInfo(toClient);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_CAUCHEMAR_INFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_ADVANCED_INFO)
{
	PlayerInfo::AdvancedInstance toClient;
	if(!player)
	{
		return;
	}

	player->GetInstanceMgr()->GetAdvancedInstanceInfo(toClient);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ADVANCED_INFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_INSTANCE_INFO)
{
	MapInstanceProto::CommonInstanceInfo toClient;
	if(!player)
	{
		return;
	}

	player->GetInstanceMgr()->GetCommonInstanceInfo(toClient);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_INSTANCE_INFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_EXP_INFO)
{
	MapInstanceProto::ExpInstanceInfo toClient;
	if(!player)
	{
		return;
	}

	player->GetInstanceMgr()->GetExpInstanceInfo(toClient);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_EXP_INFO);
}


DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MapInstanceMessage,MSG_REQ_C2GM_MONEY_INFO)
{
	MapInstanceProto::MoneyInstanceInfo toClient;
	if(!player)
	{
		return;
	}

	player->GetInstanceMgr()->GetMoneyInstanceInfo(toClient);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_MONEY_INFO);
}

