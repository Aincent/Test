/*
 * MSopreationManager.cpp
 *
 *  Created on: 2014年6月4日
 *      Author: root
 */

#include "MSopreationManager.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "CharDefine.h"
#include "ServerMsgDefine.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "../Map/SceneManager.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "../Object/Player/PlayerMessage.h"
#include "MessageStruct/MSProtoc/MS_BranchPlayerOnlineInfo.pb.h"


MSopreationManager::MSopreationManager()
{
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_GT2GM_ABOUTTALKING);

	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_MS2GM_BRANCHONLINEPLAYERINFO);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_MS2GM_BRANCHONLINECOUNT);

}


MSopreationManager::~MSopreationManager()
{

}

void MSopreationManager::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GT2GM_ABOUTTALKING);
	DEF_SWITCH_TRY_DISPATCH_END
}

void MSopreationManager::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_MS2GM_BRANCHONLINEPLAYERINFO);

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_MS2GM_BRANCHONLINECOUNT);

	DEF_SWITCH_TRY_DISPATCH_END
}

void MSopreationManager::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_REQUEST_DEFINE_FUN(MSopreationManager,MSG_REQ_MS2GM_BRANCHONLINEPLAYERINFO)
{
	BranchPlayerInfo::Admin_BranchPlayerInfo req;

	BranchPlayerInfo::AckBranchPlayerInfo toClient;
	int len = 0;

	char * content = message->GetBuffer(len);
	req.ParseFromArray(content, len);

	int beginCount = req.beginnum();
	int endCount = req.endnum();

	PlayerMessage::GetInstance()->MS_GetBranchOnlinePlayerInfo(beginCount,endCount, &toClient);
	toClient.set_ret(0);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_MS2GM_BRANCHONLINEPLAYERINFO, message,	&toClient);
	Message_Facade::Send(messRet);
}

DEF_MSG_REQUEST_DEFINE_FUN(MSopreationManager,MSG_REQ_MS2GM_BRANCHONLINECOUNT)
{
//	BranchPlayerInfo::BranchOnlineCount toClient;
//
//	toClient.set_ret(0);
//	toClient.set_onlinecount(PlayerMessage::GetInstance()->GetOnlinePlayerCount());
//
//	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_MS2GM_BRANCHONLINECOUNT, message,	&toClient);
//	Message_Facade::Send(messRet);
}

DEF_MSG_SIMPLE_DEFINE_FUN(MSopreationManager,MSG_SIM_GT2GM_ABOUTTALKING)
{
	LOG_DEBUG(FILEINFO,"ws Manager gt send DoSomthig ");
	MS_SychProto::SychGMThig req;
	int len = 0;
	char * content = message->GetBuffer(len);

	req.ParseFromArray(content, len);
	Smart_Ptr<Player> player =	NULL;
	int64  charid = req.charid();

	PlayerMessage::GetInstance()->GetPlayerByCharid(charid, player);
	if((bool)player)
	{
		this->MSDoSomeThing(player, req);
	}
}

int MSopreationManager::MSDoSomeThing(const Smart_Ptr<Player> &player,
		MS_SychProto::SychGMThig& info)
{
	int ret = 0;

	if(info.data().type() == e_dataManager_Talk || info.data().type() == e_dataManager_Shutup)
	{
		MS_SychProto::SychManagerData data = info.data();

		int64 nexttime = data.nexttime();
//		string saytext = data.saytext();

		player->SetTalkThing(nexttime);
	}
	return ret;
}











