/*
 * DailyMgr.cpp
 *
 *  Created on: 2014年6月22日
 *      Author: root
 */

#include <cstdlib>
#include "DailyMgr.h"
#include "../Object/Player/PlayerMessage.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "../Map/SceneManager.h"
#include "MessageCommonRet.h"
#include "Log/Logger.h"
#include "../Object/FileLoader/ScortpacksTypeDataLoader.h"
#include "../Object/FileLoader/MakeTypeLoader.h"
#include "FightDefined.h"
#include "../ServerEventDefine.h"
#include "../StatisticMgr/StatisticHandle.h"
DailyMgr::DailyMgr()
{
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2WS_GETCLANCOVERSOURCEINFO);
	DEF_MSG_QUICK_REQUEST_REG_FUN(DailyMgr,MSG_REQ_C2GM_GETDAILYINFO);
	DEF_MSG_QUICK_REQUEST_REG_FUN(DailyMgr,MSG_REQ_C2GM_GETYESTERDAYCOUNTERINFO);//请求昨天的活动完成次数
	DEF_MSG_QUICK_REQUEST_REG_FUN(DailyMgr,MSG_REQ_C2GM_RECOVERRESOURCE);//资源追回

	DEF_MSG_QUICK_REQUEST_REG_FUN(DailyMgr,MSG_REQ_C2GM_GETDAILYPRACTICEINFO);//请求闭关修炼信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(DailyMgr,MSG_REQ_C2GM_GETDAILYPRACTICEREWARD);//领取闭关修炼奖励
}

DailyMgr::~DailyMgr()
{
}

void DailyMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2WS_GETCLANCOVERSOURCEINFO);
	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_ACK_DEFINE_FUN(DailyMgr, MSG_REQ_GM2WS_GETCLANCOVERSOURCEINFO)
{
	//请求超时
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request worldserver clan yesterday reward error");
		return;
	}
	ServerReturn::ServerDoubleInt ret;
	ServerMessage::ClientSingleMessage ack;
	Daily::RecoverResourceInfoList toClient;
	int len = 0;
	char * content = message->GetBuffer(len);
	ret.ParseFromArray(content, len);
	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharid(ret.retf(), player);
	if(!(bool)player)
		return;

	player->GetDailyManager()->GetRecoverResourceInfo(toClient);

	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_GETYESTERDAYCOUNTERINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(DailyMgr,MSG_REQ_C2GM_GETYESTERDAYCOUNTERINFO)
{
	ServerReturn::ServerRetInt reqGT;

//	if(player && !player->IsOpenSystem(eStDailyTodo))
//	{
//		return;
//	}

	reqGT.set_ret(player->GetID());
	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),worldSvr);
	if(worldSvr.Get()!=NULL)
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2WS_GETCLANCOVERSOURCEINFO, &reqGT, worldSvr.Get(), Request);
		//messRet->SetAct(new requestAct(message));
		Message_Facade::Send(messRet);
	}

}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(DailyMgr,MSG_REQ_C2GM_RECOVERRESOURCE)
{
	Daily::C2GMReqRecoverResource req;
	ServerReturn::ServerDoubleInt toClient;

	req.ParseFromString(content);

//	if(player && player->IsOpenSystem(eStDailyTodo))
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eResRecovery, BehaviorType::Minor::eResRecovery_ResRecovery, MONITOR);;

		int ret = player->GetDailyManager()->RecoverResource(req);
		toClient.set_retf(ret);

		
	}
//	else
//	{
//		toClient.set_retf(eNotOpen);
//	}

	if(req.op() == 0 || req.op() == 1)
		toClient.set_rets(req.id());

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_RECOVERRESOURCE);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_RECOVERRESOURCE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(DailyMgr,MSG_REQ_C2GM_GETDAILYPRACTICEINFO)
{
	ServerReturn::ServerDoubleInt toClient;

	if(player && player->IsOpenSystem(eOfflineRewardLv))
	{
		player->GetDailyManager()->GetDailyPracticeInfo(toClient);
	}
	else
		return;

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_GETDAILYPRACTICEINFO);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_GETDAILYPRACTICEINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(DailyMgr,MSG_REQ_C2GM_GETDAILYINFO)
{
	Daily::DailyToDoInfoList toClient;

//	if(player && player->IsOpenSystem(eStDailyTodo))
	{
		player->GetDailyManager()->GetDailyInfo(&toClient);
	}
//	else
//		return;

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_GETDAILYINFO);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_GETDAILYINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(DailyMgr,MSG_REQ_C2GM_GETDAILYPRACTICEREWARD)
{
	Daily::C2GMReqPracticeReward info;
	ServerReturn::ServerRetInt toClient;

	info.ParseFromString(content);

	if(player && player->IsOpenSystem(eOfflineRewardLv))
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eOffExp, BehaviorType::Minor::eOffExp_GetExp, MONITOR);

		int ret = player->GetDailyManager()->GetDailyPracticeReward(info.type());
		if(ret == 0)
		{
			player->GetDailyManager()->InitDailyPractice(0);
		}
		toClient.set_ret(ret);

		
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_GETDAILYPRACTICEREWARD);
}
