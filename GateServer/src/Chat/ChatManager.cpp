/*
 * ChatManager.cpp
 *
 *  Created on: 2014年5月19日
 *      Author: root
 */

#include "ChatManager.h"
#include "SvrConfig.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageStruct/CharLogin/Chat.pb.h"
#include "CharDefine.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageCommonRet.h"
ChatManager * ChatManager::m_instance = 0;

ChatManager::ChatManager()
{
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_CHAT);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_WS2GT_CHAT);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_WS2GT_MARQUEE);
	DEF_MSG_ACK_REG_FUN(eGateServer,MSG_REQ_GT2GM_CHAT);
	DEF_MSG_ACK_REG_FUN(eGateServer,MSG_REQ_GT2WS_CHAT);
}

ChatManager::~ChatManager()
{

}

void ChatManager::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GT_CHAT);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GT_MARQUEE);

	DEF_SWITCH_TRY_DISPATCH_END
}

void ChatManager::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_CHAT);
	DEF_SWITCH_TRY_DISPATCH_END

}

void ChatManager::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_CHAT);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2WS_CHAT);
	DEF_SWITCH_TRY_DISPATCH_END
}
DEF_MSG_REQUEST_DEFINE_FUN(ChatManager,MSG_REQ_C2GT_CHAT)
{
	LOG_DEBUG(FILEINFO,"client request chat");
	ChatInfo::StartChatInfo req;
	int64 charid = -1;
	int len = 0;
	int gsChannel = -1;
	char * content = message->GetBuffer(len);
	req.ParseFromArray(content, len);

	bool isGs = ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid);
	switch(req.channel())
	{
		case eChatPrivate:
		case eChatFaction:
		case eChatInfluence:
		case eChatWorld:
		{
			if (NULL == ServerConHandler::GetInstance()->GetWorldServer() || -1 == ServerConHandler::GetInstance()->GetWorldServer()->channelID)
			{
				LOG_ERROR(FILEINFO, "GetWorldServer Retrun Data Error !!!!");
				return;
			}
			Safe_Smart_Ptr<CommBaseOut::Message> worldmess  = build_message(MSG_REQ_GT2WS_CHAT, &req, ServerConHandler::GetInstance()->GetWorldServer(), Request);
			worldmess->SetAct(new requestAct(message));
			worldmess->SetMessageTime(charid);
			Message_Facade::Send(worldmess);
			break;
		}
		case eChatOrdinary:
		case eChatRanks:
		case eChatHorn:
		{
			if(!isGs)
			{
				LOG_ERROR(FILEINFO, "Player[charid=%lld] sim chat but gameserver not existed", GET_PLAYER_CHARID(charid));

				return;
			}

			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_CHAT, message, gsChannel, Request);
			messRet->SetAct(new requestAct(message));
			messRet->SetMessageTime(charid);
			Message_Facade::Send(messRet);
			break;
		}
		default:
		{
			break;
		}
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(ChatManager,MSG_SIM_WS2GT_CHAT)
{
	LOG_DEBUG(FILEINFO,"return client chat");

	int channel = -1;
	int64 charID = message->GetMessageTime();

	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(charID, channel))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_GT2C_CHAT, message, channel, SimpleMessage);
		Message_Facade::Send(clientRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "sim chat but find not client[ charid=%lld]", GET_PLAYER_CHARID(charID));
	}
}
DEF_MSG_ACK_DEFINE_FUN(ChatManager, MSG_REQ_GT2GM_CHAT)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request GM chat and ack timeout");

		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_CHAT, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}
DEF_MSG_ACK_DEFINE_FUN(ChatManager, MSG_REQ_GT2WS_CHAT)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request WS chat and ack timeout");

		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_CHAT, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(ChatManager,MSG_SIM_WS2GT_MARQUEE)
{
	LOG_DEBUG(FILEINFO,"WS sim gt marquee");
	int channel = -1;

	int64 charID = message->GetMessageTime();
	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(charID, channel))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_GT2C_MARQUEE, message, channel, SimpleMessage);
		Message_Facade::Send(clientRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "sim chat but find not client[ charid=%lld]",GET_PLAYER_CHARID(charID));
	}
}
