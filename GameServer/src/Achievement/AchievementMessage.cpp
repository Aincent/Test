/*
 * AchievementMessage.cpp
 *
 *  Created on: 2014年6月19日
 *      Author: root
 */

#include "AchievementMessage.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "Achievement.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/ServerReturn3Int.pb.h"
#include "MessageStruct/AchievementProto.pb.h"
#include "../Object/Player/Player.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Map/GameZone.h"
#include "../Map/SceneManager.h"
#include "../MessageBuild.h"
#include "../Map/ConfigLoader/MapConfig.h"
#include "../MsgCommonClass.h"
#include "Smart_Ptr.h"
#include <netinet/in.h>
#include "MessageCommonRet.h"
#include <string.h>
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/CharLogin/Ranking.pb.h"
#include "../StatisticMgr/StatisticHandle.h"

AchievementMessage::AchievementMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(AchievementMessage,MSG_REQ_GETRECENTPRESTIGE);		//成就总览

	DEF_MSG_QUICK_REQUEST_REG_FUN(AchievementMessage, MSG_REQ_GETTITLEINOF);//查看称号
	DEF_MSG_QUICK_REQUEST_REG_FUN(AchievementMessage,MSG_REQ_GETPRESTIGETYPE);		//查看某个成就类型
	DEF_MSG_QUICK_REQUEST_REG_FUN(AchievementMessage,MSG_REQ_GETPRESTIGEPRIZE);		//获取成就奖励
	DEF_MSG_QUICK_REQUEST_REG_FUN(AchievementMessage, MSG_REQ_ACTIVETITLE);	//激活称号
	DEF_MSG_QUICK_REQUEST_REG_FUN(AchievementMessage, MSG_REQ_GETACHSHOPINFO);	//获取成就商店信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(AchievementMessage, MSG_REQ_BUGACHSHOPGOODS);//购买成就商品
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_UPDATATITLEID);

	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2WS_GLORY);
}

AchievementMessage::~AchievementMessage()
{

}

void AchievementMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_UPDATATITLEID);

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_SIMPLE_DEFINE_FUN(AchievementMessage, MSG_SIM_WS2GM_UPDATATITLEID)
{
	ServerReturn::ServerDoubleIntArray ret;
	Smart_Ptr<Player> player;

	int len = 0;

	char * content = message->GetBuffer(len);
	ret.ParseFromArray(content, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(ret.retf(),player);
	if(!(bool)player)
	{
			return;
	}

	Smart_Ptr<Title> title = player->GetTitle();
	if(title)
	{
		title->UpDate(ret,false);
	}
}

void AchievementMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

//	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2GT_GLORY);

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2WS_GLORY);

	DEF_SWITCH_TRY_DISPATCH_END
}
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(AchievementMessage,MSG_REQ_GETRECENTPRESTIGE)
{
	AchievementProto::ReqAchievementAll req;
	AchievementProto::RetAchievementAll toClient;
	ServerMessage::ClientSingleMessage ack;

	req.ParseFromString(content);

	Smart_Ptr<Achievement> achievement = player->GetAchievement();
	if (achievement && player->IsOpenSystem(eStAch))
	{
		toClient.set_prestige(player->getAttManage()->getValue<int>(ePrestige));

		//最近完成的成就
		std::vector<ServerReturn::ServerThreeInt> recentAchievements;
		achievement->GetPrestigeInfo(AchievementType_5,
		                recentAchievements);
		for(uint i = 0; i < recentAchievements.size(); i++)
		{
			ServerReturn::ServerThreeInt* info =
			                toClient.add_prestigeinfo();
			info->CopyFrom(recentAchievements[i]);
		}

		//完成进度
		std::vector<ServerReturn::ServerRetInt> processAchi;
		achievement->GetAchievementTypePercent(processAchi);

		for(uint j = 0; j < processAchi.size(); j++)
		{
			ServerReturn::ServerRetInt *processInfo =
			                toClient.add_prestigeprogress();
			processInfo->CopyFrom(processAchi[j]);
		}
	}
	else
		return;

	//QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_GETRECENTPRESTIGE);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_GETRECENTPRESTIGE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(AchievementMessage,MSG_REQ_GETTITLEINOF)
{
	//排行榜称号
	RankingInfo::GTtoWSMeRankingInfo reqWS;

	reqWS.set_type(eCpRanking);
	reqWS.set_charid(player->GetID());

	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),worldSvr);

	if(worldSvr.Get()!=NULL)
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2WS_GLORY, &reqWS, worldSvr.Get(), Request);
		messRet->SetAct(new requestAct(message));
		Message_Facade::Send(messRet);
	}

	return;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(AchievementMessage,MSG_REQ_GETPRESTIGETYPE)
{
	AchievementProto::ReqLookForPrestigeType req;
	AchievementProto::RetLookForPrestigeType toClient;
	ServerMessage::ClientSingleMessage ack;

	req.ParseFromString(content);

//	int type = req.type();
//	ServerReturn::ServerRetInt *retType = toClient.mutable_prestigetype();
//	retType->set_ret(type);
	if(bool(player)==false)
		return;
	Smart_Ptr<Achievement> achievement = player->GetAchievement();
	if (achievement && player->IsOpenSystem(eStAch))
	{
		std::vector<ServerReturn::ServerThreeInt> recentAchievements;
		achievement->GetPrestigeInfo((AchievementType) req.type(),
		                recentAchievements);
		for(uint i = 0; i < recentAchievements.size(); i++)
		{
			ServerReturn::ServerThreeInt* info =
			                toClient.add_prestigeinfo();
			info->CopyFrom(recentAchievements[i]);
		}
	}
	else
		return;

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_GETPRESTIGETYPE);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_GETPRESTIGETYPE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(AchievementMessage,MSG_REQ_GETPRESTIGEPRIZE)
{
	ServerReturn::ServerRetInt req;
	ServerReturn::ServerThreeInt toClient;
	ServerMessage::ClientSingleMessage ack;

	req.ParseFromString(content);

	int errorCode = 0;
	int Type = (req.ret() >> 32);
	int achiID = req.ret();
	if(bool(player) == false)
		return;
	Smart_Ptr<Achievement> achievement = player->GetAchievement();
	if (achievement)
	{
		if(player->IsOpenSystem(eStAch))
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eAchievement, BehaviorType::Minor::eAchieve_GetReward, MONITOR);

			if (achievement->DrawAchievement(achiID, errorCode))
			{
				toClient.set_retf(errorCode);		//领取奖励成功
			}
			else
			{
				toClient.set_retf(errorCode);
			}

//			
		}
		else
		{
			toClient.set_retf(eNotOpen);
		}
	}

	toClient.set_rets(Type);
	toClient.set_rett(achiID);
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_GETPRESTIGEPRIZE);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_GETPRESTIGEPRIZE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(AchievementMessage,MSG_REQ_ACTIVETITLE)
{
	ServerReturn::ServerRetInt req;
	ServerReturn::ServerDoubleRetInt toClient;
	ServerMessage::ClientSingleMessage ack;

	req.ParseFromString(content);

	int error = 0;

	int titleId = req.ret();
	if(titleId == 0)
		toClient.set_retf(0);
	else
		toClient.set_retf(1);

	Smart_Ptr<Title> title = player->GetTitle();
	if (title)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eAchievement, BehaviorType::Minor::eAchieve_ActiveTitle, MONITOR);

		title->ActiveTitle(titleId, error);

//		
	}

	int64 ret = ((int64)titleId << 32) | error;

	toClient.set_rets(ret);

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_ACTIVETITLE);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_ACTIVETITLE);
}

//DEF_MSG_ACK_DEFINE_FUN(AchievementMessage, MSG_REQ_GM2GT_GLORY)
//{
//	//请求超时
//	if(message->GetErrno() == eReqTimeOut)
//	{
//		LOG_WARNING(FILEINFO, "gameserver request gt me ranking");
//		return;
//	}
//	ServerReturn::ServerDoubleInt req;
////	ServerReturn::ServerRetInt toClient;
////	ServerMessage::ClientSingleMessage ack;
//	Smart_Ptr<Player> player;
//
//	int len = 0;
//
//	char * content = message->GetBuffer(len);
//	req.ParseFromArray(content, len);
////	int error = 0;
//
//	PlayerMessage::GetInstance()->GetPlayerByCharid(req.retf(),player);
////	if(!(bool)player)
////	{
////		return;
////	}
//
////	Smart_Ptr<Title> title = player->GetTitle();
////	if (title)
////	{
////		int rangkingTitleId = req.rets();
////		title->addRankingTitle(rangkingTitleId);
////	}
////
////	int64 ret = ((int64)req.rets() << 32) | error;
////
////	toClient.set_ret(ret);
////
////	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_ACTIVETITLE);
//
////	AchievementProto::ReqLookForTitle req;
//	AchievementProto::RetLookForTitle toClient;
////	ServerMessage::ClientSingleMessage ack;
//
//	if(player && player->IsOpenSystem(eStAch))
//	{
//		Smart_Ptr<Title> title = player->GetTitle();
//		if (title)
//		{
//			int rangkingTitleId = req.rets();
//			title->addRankingTitle(rangkingTitleId);
//
//			std::vector<ServerReturn::ServerThreeInt> titleinfo;
//			title->GetTitleInfo(titleinfo);
//			for(uint i = 0; i < titleinfo.size(); i++)
//			{
//				ServerReturn::ServerThreeInt* info = toClient.add_titleinfo();
//				info->CopyFrom(titleinfo[i]);
//			}
//		}
//	}
//	else
//		return;
//
////	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_GETTITLEINOF);
//		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GETTITLEINOF, static_cast<requestAct *>(act.Get())->mesReq, &toClient);
//		Message_Facade::Send(messRet);
//}
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(AchievementMessage,MSG_REQ_GETACHSHOPINFO)
{
	ServerReturn::ServerResultInt req;
	AchievementProto::RetAchShopInfo ack;

	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eStAch))
	{
		if(req.ret() == 1)
		{
			player->GetAchievement()->Updata();
		}
		player->GetAchievement()->SetRetAchShopInfo(&ack);
	}
	else
		return;

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_GETACHSHOPINFO);
}
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(AchievementMessage,MSG_REQ_BUGACHSHOPGOODS)
{
	ServerReturn::ServerRetInt req;
	ServerReturn::ServerResultInt ack;

	req.ParseFromString(content);

	BEHAVIOR_MONI(player, BehaviorType::Main::eAchievement, BehaviorType::Minor::eAchieve_BuyGoods, MONITOR);;

	ack.set_ret(player->GetAchievement()->BugAchShopGoods(req.ret()>>32,req.ret()&0xffffffff));

	

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_BUGACHSHOPGOODS);
}

DEF_MSG_ACK_DEFINE_FUN(AchievementMessage, MSG_REQ_GM2WS_GLORY)
{
	//请求超时
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request ws ranking time out");
		return;
	}

	ServerReturn::ServerDoubleInt req;
	AchievementProto::RetLookForTitle toClient;
	Smart_Ptr<Player> player;

	int len = 0;

	char * content = message->GetBuffer(len);
	req.ParseFromArray(content, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(req.retf(),player);
	if(player && player->IsOpenSystem(eStAch))
	{
		Smart_Ptr<Title> title = player->GetTitle();
		if (title)
		{
			int rangkingTitleId = req.rets();
			title->addRankingTitle(rangkingTitleId);

			std::vector<ServerReturn::ServerThreeInt> titleinfo;
			title->GetTitleInfo(titleinfo);
			for(uint i = 0; i < titleinfo.size(); i++)
			{
				ServerReturn::ServerThreeInt* info = toClient.add_titleinfo();
				info->CopyFrom(titleinfo[i]);
			}
		}
	}
	else
		return;

	REQ_MSG_QUICK_ACK_FUN(static_cast<requestAct *>(act.Get())->mesReq, &toClient, MSG_REQ_GETTITLEINOF);
//		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GETTITLEINOF, static_cast<requestAct *>(act.Get())->mesReq, &toClient);
//		Message_Facade::Send(messRet);
}
