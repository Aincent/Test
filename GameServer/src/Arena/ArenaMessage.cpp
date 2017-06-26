/*
 * ArenaMessage.cpp
 *
 *  Created on: 2016年5月4日
 *      Author: root
 */

#include "ArenaMessage.h"
#include "ArenaMgr.h"
#include "CharLogin/MapInstanceProto.pb.h"
#include "../MessageBuild.h"
#include "Network/Message_Facade.h"
#include "Network/MessageHandler.h"
#include "../Object/Player/Player.h"
#include "../Object/Player/PlayerMessage.h"
#include "FileLoader/ArenaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "../MsgCommonClass.h"
#include "../Map/SceneManager.h"
#include "../ServerEventDefine.h"
#include "../StatisticMgr/StatisticHandle.h"
#include "FileLoader/GameParaLoader.h"
#include "PVPDefine.h"
#include "../Mail/MailMessage.h"
#include "./FileLoader/InternationalLoader.h"

ArenaMessage::ArenaMessage() {
	// TODO Auto-generated constructor stub
	DEF_MSG_QUICK_REQUEST_REG_FUN(ArenaMessage, MSG_REQ_C2GM_ARENAINFO);		//请求竞技场信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(ArenaMessage, MSG_REQ_C2GM_CHANCECDTIME);
	DEF_MSG_QUICK_REQUEST_REG_FUN(ArenaMessage, MSG_REQ_C2GM_ADDCHALLENGETIMES);
	DEF_MSG_QUICK_REQUEST_REG_FUN(ArenaMessage, MSG_REQ_C2GM_RANKINGREWARD);

	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REG_GM2WS_SYNCRANKINGREWARD); //同步竞技场信息
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2DB_ARENAINFO);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2WS_SYNCARENA);
}

ArenaMessage::~ArenaMessage() {
	// TODO Auto-generated destructor stub
}

void ArenaMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{

}
void ArenaMessage::Handle_Ack(Safe_Smart_Ptr<Message>& message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2DB_ARENAINFO);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REG_GM2WS_SYNCRANKINGREWARD); //同步竞技场信息
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2WS_SYNCARENA);

	DEF_SWITCH_TRY_DISPATCH_END
}

void ArenaMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
}


DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ArenaMessage,MSG_REQ_C2GM_ARENAINFO)
{
	LOG_DEBUG(FILEINFO, "request Arena  war report info from client");

//	MapInstanceProto::ReqToGameGetArenaInfo toClient;
//
////	toClient.set_cdtime(player->GetCDMgr()->GetCDEndTime(ArenaLoader::GetInstance()->GetChannelType()));
////	toClient.set_challengetimes(player->GetCounterService()->GetNumById(COMPETE_INSTANCE_COUNTER));
//	toClient.set_challengetimes(player->GetArenaMgr()->GetCanChallCnt());
//	toClient.set_alreadyrefresh(player->GetCounterService()->GetNumById(ARENA_ALREADYREFRESH_CNT));
//	toClient.set_alreadyaddchall(player->GetCounterService()->GetNumById(ARENA_BUYCHALLENGE_CNT));
//
//	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ARENAINFO);

	MapInstanceProto::SysRankingReward toWorld;
	toWorld.set_ret(player->GetID());
	toWorld.set_retf(1);
	Smart_Ptr<SvrItem> wsSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),wsSvr);
	if(wsSvr)
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REG_GM2WS_SYNCRANKINGREWARD, &toWorld, wsSvr.Get(), Request);
		messRet->SetAct(new requestAct(message));
		Message_Facade::Send(messRet);
	}
}


DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ArenaMessage,MSG_REQ_C2GM_CHANCECDTIME)
{
	MapInstanceProto::ReturnCancelAreanCDTime toClient;

	vector<int> vecBro;

	int costGolden = 0;
	int costbGolden = 0;

	BEHAVIOR_MONI(player, BehaviorType::Main::eArena, BehaviorType::Minor::eArena_ReduceCDTime, MONITOR);

	int nCost = ArenaLoader::GetInstance()->GetResetChallengeCD();

	if(!player->IsEnoughGolden(nCost))
	{
		toClient.set_ret(eNotEnogGolden);
		REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_CHANCECDTIME);
	}
	else
	{

		//扣除元宝
		player->DecreaseGoldenCost(nCost, costGolden, costbGolden);
		if(costGolden > 0)
			vecBro.push_back(eCharGolden);
		if(costbGolden > 0)
			vecBro.push_back(eCharBindGolden);

		player->SynCharAttribute(vecBro);
		player->GetCDMgr()->ResetCDTime(ArenaLoader::GetInstance()->GetChannelType());


		toClient.set_ret(0);
		REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_CHANCECDTIME);
	}

	
}


DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ArenaMessage,MSG_REQ_C2GM_ADDCHALLENGETIMES)
{
	LOG_DEBUG(FILEINFO, "request add challenge times  from client");

	MapInstanceProto::AddChallengeTimes req;
	MapInstanceProto::ReturnAddChallengeTimes toClient;
	vector<int> vecBro;
	vecBro.push_back(eCharGolden);

	req.ParseFromString(content);

	BEHAVIOR_MONI(player, BehaviorType::Main::eArena, BehaviorType::Minor::eArena_AddChallCnt, MONITOR);

	int err = player->GetArenaMgr()->AddChallCnt();

	
	if (-1 == err)
	{
		return;
	}
	toClient.set_ret(err);
	toClient.set_count(player->GetArenaMgr()->GetCanChallCnt());
	toClient.set_alreadyaddchall(player->GetCounterService()->GetNumById(ARENA_BUYCHALLENGE_CNT));
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ADDCHALLENGETIMES);
}


DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ArenaMessage,MSG_REQ_C2GM_RANKINGREWARD)
{
	LOG_DEBUG(FILEINFO, "request ranking reward from client");

	MapInstanceProto::ReqArenaHistoryReward req;
	req.ParseFromString(content);

	MapInstanceProto::SysRankingReward toWorld;
	toWorld.set_ret(player->GetID());
	toWorld.set_retf(2);
	toWorld.set_rets(req.id());

	Smart_Ptr<SvrItem> wsSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),wsSvr);
	if(wsSvr)
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REG_GM2WS_SYNCRANKINGREWARD, &toWorld, wsSvr.Get(), Request);
		messRet->SetAct(new requestAct(message));
		Message_Facade::Send(messRet);
	}

//	int nAwardRanking = player->getAttManage()->getValue<int>(eAwardRanking);
//
//	if(nAwardRanking > 0)
//	{
//		BEHAVIOR_MONI(player, BehaviorType::Main::eArena, BehaviorType::Minor::eArena_GetRankAward, MONITOR);
//
//		vector<int> vecBro;
//
//		int money = ArenaLoader::GetInstance()->GetRankingMoneyReward(nAwardRanking);
//		player->ChangeMoney(money,true);
//		vecBro.push_back(eCharMoney);
//
//		int credit = ArenaLoader::GetInstance()->GetPrestigeReward(nAwardRanking);
//		player->ChangeCredit(credit,true);
//		vecBro.push_back(eCredit);
//
//		player->getAttManage()->setValue(eAwardRanking,0);
//		vecBro.push_back(eAwardRanking);
//		player->SynCharAttribute(vecBro);
//
//
//
//		MapInstanceProto::SysRankingReward toWorld;
//		toWorld.set_ret(player->GetID());
//
//		Smart_Ptr<SvrItem> wsSvr;
//		ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),wsSvr);
//		if(wsSvr)
//		{
//			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REG_GM2WS_SYNCRANKINGREWARD, &toWorld, wsSvr.Get(), Request);
//			messRet->SetAct(new requestAct(message));
//			Message_Facade::Send(messRet);
//		}
//
//		player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Arena, eUIHLSmall_Arena_AwardUnGet), 1, false);
//	}
//	else
//	{
//		MapInstanceProto::ReqToGameGetArenaInfo toClient;
//		toClient.set_challengetimes(eRankingRewardError);
////		toClient.set_cdtime(0);
//		REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_RANKINGREWARD);
//	}

}


DEF_MSG_ACK_DEFINE_FUN(ArenaMessage, MSG_REG_GM2WS_SYNCRANKINGREWARD)
{
	if (message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request wsserver player Ranking reward service info and ack timeout");
		return;
	}

	MapInstanceProto::SysRankingReward fromWorld;
	Smart_Ptr<Player> player;
	vector<int> vec;
	vector<DWORD> idList;
	vector<DWORD> numList;
	vector<WORD> tempFlagList;
	vector<DWORD> tempStreghtList;
	vector<PropItem> items;

	int len = 0;
	char *con = message->GetBuffer(len);
	fromWorld.ParseFromArray(con, len);

	PlayerMessage::GetInstance()->GetPlayerByCharId(player,fromWorld.ret());
	if(!player)
	{
		return;
	}

	if(fromWorld.retf() == 1)
	{
		MapInstanceProto::ReqToGameGetArenaInfo toClient;

	//	toClient.set_cdtime(player->GetCDMgr()->GetCDEndTime(ArenaLoader::GetInstance()->GetChannelType()));
	//	toClient.set_challengetimes(player->GetCounterService()->GetNumById(COMPETE_INSTANCE_COUNTER));
		toClient.set_challengetimes(player->GetArenaMgr()->GetCanChallCnt());
		toClient.set_alreadyrefresh(player->GetCounterService()->GetNumById(ARENA_ALREADYREFRESH_CNT));
		toClient.set_alreadyaddchall(player->GetCounterService()->GetNumById(ARENA_BUYCHALLENGE_CNT));

		toClient.set_ranking(fromWorld.ranking());
		toClient.set_historyrank(fromWorld.historyrank());
		toClient.mutable_historyreward()->CopyFrom(fromWorld.historyreward());

		for(int i = 0 ; i < toClient.historyreward().reward_size(); i++)
			LOG_DEBUG(FILEINFO, "toClient.historyreward().reward_size() id = %d value = %d", toClient.historyreward().reward(i), toClient.historyreward().havareward(i));

		REQ_MSG_QUICK_ACK_FUN(static_cast<requestAct *>(act.Get())->mesReq, &toClient, MSG_REQ_C2GM_ARENAINFO);
	}
	else
	{
		MapInstanceProto::RspArenaHistoryReward toClient;
		if(fromWorld.ranking() == 0)
		{
			toClient.set_ret(0);
			toClient.set_id(fromWorld.rets());

			const ArenaParam::RewardInfo* pInfo = ArenaLoader::GetInstance()->GetGetRewardById(fromWorld.rets());
			if(pInfo)
			{
				player->ChangeMoney(pInfo->nMoney,true);
				vec.push_back(eCharMoney);

				player->ChangeCredit(pInfo->nCredit,true);
				vec.push_back(eCredit);

				player->SynCharAttribute(vec);

				for(size_t i = 0; i < pInfo->m_nAward.size(); i++)
				{
					idList.push_back(pInfo->m_nAward[i]);
					numList.push_back(pInfo->m_nAwardNum[i]);
				}

				tempFlagList.resize(idList.size(),1);
				tempStreghtList.resize(numList.size(),0);
				if(idList.size() > 0)
				{
					Smart_Ptr<ArticleBaseMgr> pPacket = player->GetContainer(ePackageType);
					if(!pPacket)
					{
						return;
					}

					int res = pPacket->IsBagEnought(idList,numList,tempFlagList);
					if (!res)
					{
						vector<int> tempPos;
						pPacket->AddItem(idList, numList,tempPos,tempFlagList,tempStreghtList);
					}
					else
					{
						for(size_t i = 0; i < idList.size(); i++)
						{
							PropItem item;
							item.id = idList[i];
							item.num = numList[i];

							items.push_back(item);
						}

						if(items.size() > 0)
						{
							MailMessageMgr::GetInstance()->SendMail(player->GetID(),items,InternationalLoader::GetInstance()->GetInterStr(71).c_str(),InternationalLoader::GetInstance()->GetInterStr(71).c_str());
						}
					}
				}
			}
		}
		else
		{
			toClient.set_ret(fromWorld.ranking());
		}


		REQ_MSG_QUICK_ACK_FUN(static_cast<requestAct *>(act.Get())->mesReq, &toClient, MSG_REQ_C2GM_RANKINGREWARD);

	}
}


DEF_MSG_ACK_DEFINE_FUN(ArenaMessage, MSG_REQ_GM2DB_ARENAINFO)
{
	if (message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request dbserver ranking player and ack timeout");

		return;
	}

	LOG_DEBUG(FILEINFO, "gameserver request dbserver ranking player");

	PlayerInfo::ArenaInfo req;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;

	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(static_cast<IntAct *>(act.Get())->mesData, player);
	if(player)
	{
		if (GameParaLoader::GetInstance()->getArenaMapId() != GET_MAP_ID(player->GetMapID()))
		{
			return;
		}

		Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(player->GetMapID());
		if(!zone)
			return;

		zone->InitMonsterByPlayer(player, req);
	}
}

DEF_MSG_ACK_DEFINE_FUN(ArenaMessage, MSG_REQ_GM2WS_SYNCARENA)
{
	if (message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request worldserver synch player info and ack timeout");

		return;
	}

	LOG_DEBUG(FILEINFO, "gameserver request worldserver  synch player info ");

	ServerReturn::ServerDoubleInt req;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;

	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(req.retf(), player);
	if(player)
	{
		if(req.rets() > 0)
		{
			int beforeRank = player->getAttManage()->getValue<int>(eRanking);
			player->getAttManage()->setValue(eRanking,req.rets());
//			if (0 == beforeRank && player->GetArenaMgr())
//			{
//				player->GetArenaMgr()->OpenRealData();
//			}
			if(req.rets() < beforeRank)
			{
				OneParmaEventArgs e(player->GetMyself());
				player->FireEvent(PLAYER_RANKING_CHANGE,e);
			}
		}
	}
}
