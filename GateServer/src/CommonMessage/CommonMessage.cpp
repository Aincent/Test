/*
 * CommonMessage.cpp
 *
 *  Created on: 2014年5月13日
 *      Author: helh
 */

#include "CommonMessage.h"
#include "SvrConfig.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "MessageStruct/QuickMessage.pb.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"


CommonMessage::CommonMessage()
{
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_GM2GT_QUICKMESSAGE);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_WS2C_QUICKMESSAGE);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_C2GT_QUICKMESSAGE);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_C2WS_QUICKMESSAGE);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_QUICKMESSAGE);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2WS_QUICKMESSAGE);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_QUICKMESSAGE);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2WS_QUICKMESSAGE);
}



CommonMessage::~CommonMessage()
{

}

void CommonMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GM2GT_QUICKMESSAGE);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2C_QUICKMESSAGE);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_C2GT_QUICKMESSAGE);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_C2WS_QUICKMESSAGE);

	DEF_SWITCH_TRY_DISPATCH_END
}


void CommonMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_QUICKMESSAGE);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2WS_QUICKMESSAGE);

	DEF_SWITCH_TRY_DISPATCH_END
}


void CommonMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_QUICKMESSAGE);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2WS_QUICKMESSAGE);

	DEF_SWITCH_TRY_DISPATCH_END
}


DEF_MSG_SIMPLE_DEFINE_FUN(CommonMessage, MSG_SIM_GM2GT_QUICKMESSAGE)
{
	int64 charid = 0;
	int channel = -1;

	charid = message->GetMessageTime();
	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(charid, channel))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_GT2C_QUICKMESSAGE, message, channel, SimpleMessage);
		Message_Facade::Send(clientRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "gateserver get gameserver quick message but find not client[ charid=%lld]",GET_PLAYER_CHARID(charid));
	}
}


DEF_MSG_SIMPLE_DEFINE_FUN(CommonMessage, MSG_SIM_WS2C_QUICKMESSAGE)
{
	int64 charid = 0;
	int channel = -1;

	charid = message->GetMessageTime();
	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(charid, channel))
	{
			Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_GT2C_QUICKMESSAGE, message, channel, SimpleMessage);
			Message_Facade::Send(clientRet);
	}
	else
	{
			LOG_ERROR(FILEINFO, "gateserver get worldserver quick message but find not client[ charid=%lld]",GET_PLAYER_CHARID(charid));
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(CommonMessage, MSG_SIM_C2GT_QUICKMESSAGE)
{
	int64 charid = -1;
	int gsChannel = -1;
	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_SIM_GT2GM_QUICKMESSAGE, message, gsChannel, SimpleMessage);
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(CommonMessage, MSG_SIM_C2WS_QUICKMESSAGE)
{
	int64 charid = -1;
	charid = ServerConHandler::GetInstance()->GetCharidByChannel(message->GetChannelID());
	if(charid > 0)
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_SIM_GT2WS_QUICKMESSAGE, message, ServerConHandler::GetInstance()->GetWorldServer()->channelID, SimpleMessage);
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}


DEF_MSG_REQUEST_DEFINE_FUN(CommonMessage, MSG_REQ_C2GT_QUICKMESSAGE)
{
	int64 charid = -1;
	int gsChannel = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_QUICKMESSAGE,message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(CommonMessage, MSG_REQ_C2WS_QUICKMESSAGE)
{
	int64 charid = -1;

	ConItem * item = ServerConHandler::GetInstance()->GetWorldServer();
	if(!item)
	{
		return;
	}

	charid = ServerConHandler::GetInstance()->GetCharidByChannel(message->GetChannelID());
	if(charid > 0)
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2WS_QUICKMESSAGE, message, item->channelID, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}


DEF_MSG_ACK_DEFINE_FUN(CommonMessage, MSG_REQ_GT2GM_QUICKMESSAGE)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		Safe_Smart_Ptr<CommBaseOut::Message> tempMsg = static_cast<requestAct *>(act.Get())->mesReq;
		int64 tempCharaid = ServerConHandler::GetInstance()->GetCharidByChannel(tempMsg->GetChannelID());

		int len = 0;
		char *str = tempMsg->GetBuffer(len);

		ServerMessage::ClientSingleMessage clientMsg;
		clientMsg.ParseFromArray(str,len);

		int tempMsgID = clientMsg.id();

		LOG_ERROR(FILEINFO, "gateserver request gameserver quick ack timeout playerID:%lld msgID:%d",tempCharaid,tempMsgID);

		return;
	}

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_QUICKMESSAGE, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}


DEF_MSG_ACK_DEFINE_FUN(CommonMessage, MSG_REQ_GT2WS_QUICKMESSAGE)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		Safe_Smart_Ptr<CommBaseOut::Message> tempMsg = static_cast<requestAct *>(act.Get())->mesReq;
		int64 tempCharaid = ServerConHandler::GetInstance()->GetCharidByChannel(tempMsg->GetChannelID());

		int len = 0;
		char *str = tempMsg->GetBuffer(len);

		ServerMessage::ClientSingleMessage clientMsg;
		clientMsg.ParseFromArray(str,len);

		int tempMsgID = clientMsg.id();

		LOG_ERROR(FILEINFO, "gateserver request worldServer quick ack timeout playerID:%lld msgID:%d",tempCharaid,tempMsgID);

		return;
	}

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2WS_QUICKMESSAGE, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}






