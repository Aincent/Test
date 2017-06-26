/*
 * CharSynch.cpp
 *
 *  Created on: 2013��10��11��
 *      Author: helh
 */

#include "CharSynch.h"
#include "MessageStruct/Map/SynchPath.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn3Int.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/ServerReturnIntChar.pb.h"
#include "MessageStruct/ServerReturnChar.pb.h"
#include "MessageStruct/CharBase/CharAttribute.pb.h"
#include "MessageStruct/CharBattle/Battle.pb.h"
#include "Log/Logger.h"
#include "../ServerManager.h"
#include "../MsgCommonClass.h"
#include "../MessageBuild.h"
#include "SvrConfig.h"
#include "MessageStruct/CharLogin/GoodsMessage.pb.h"
#include "MessageStruct/SynchMessage.pb.h"
#include "./google/protobuf/message.h"

CharSynch* CharSynch::m_instance = 0;

CharSynch::CharSynch()
{
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_C2GT_SYNCH);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_C2GT_HEARTBEAT);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_SYNCHPOS);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_PICKITEM);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_FINDPATH);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_PICKITEM);
}

CharSynch::~CharSynch()
{

}

void CharSynch::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN;

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_C2GT_SYNCH);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_C2GT_HEARTBEAT);

	DEF_SWITCH_TRY_DISPATCH_END;
}

void CharSynch::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN;

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_FINDPATH);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_PICKITEM);

	DEF_SWITCH_TRY_DISPATCH_END;
}

void CharSynch::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN;

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_SYNCHPOS);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_PICKITEM);

	DEF_SWITCH_TRY_DISPATCH_END;
}

DEF_MSG_REQUEST_DEFINE_FUN(CharSynch, MSG_REQ_C2GT_FINDPATH)
{
	int gsChannel = -1;
	int64 charid = -1;

	LOG_DEBUG(FILEINFO, "[messageid = %d] player synch path", MSG_REQ_C2GT_FINDPATH);

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_SYNCHPOS,message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player[charid=%lld] request synch path but gameserver not existed", GET_PLAYER_CHARID(charid));
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(CharSynch, MSG_REQ_C2GT_PICKITEM)
{
	int64 charid = 0;
	int gsChannel = -1;

	LOG_DEBUG(FILEINFO, "[messageid = %d] player pick up drop item", MSG_REQ_C2GT_PICKITEM);

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_PICKITEM,message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player[charid=%lld] request picking  drop item but gameserver not existed",GET_PLAYER_CHARID(charid));
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(CharSynch, MSG_SIM_C2GT_HEARTBEAT)
{
	LOG_WARNING(FILEINFO, "channel[%d] heart beat [%lld]", message->GetChannelID(), CUtil::GetNowSecond());
	ServerConHandler::GetInstance()->SynchHeartBeat(message->GetChannelID());
}

DEF_MSG_SIMPLE_DEFINE_FUN(CharSynch, MSG_SIM_C2GT_SYNCH)
{
	int gsChannel = -1;
	int64 charid = -1;

	LOG_DEBUG(FILEINFO, "[messageid = %d] player synchinfo", MSG_SIM_C2GT_SYNCH);

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_SIM_GT2GM_SYNCHPOS,message, gsChannel, SimpleMessage);
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player[charid=%lld] request synch pos but gameserver not existed",GET_PLAYER_CHARID(charid));
	}
}

DEF_MSG_ACK_DEFINE_FUN(CharSynch, MSG_REQ_GT2GM_SYNCHPOS)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver synch player position and ack timeout");

		return;
	}

	LOG_DEBUG(FILEINFO, "gateserver request gameserver synch player position and return");

	Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_REQ_C2GT_FINDPATH,message,static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(clientRet);
}

DEF_MSG_ACK_DEFINE_FUN(CharSynch, MSG_REQ_GT2GM_PICKITEM)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver pick up item and ack timeout");

		return;
	}

	LOG_DEBUG(FILEINFO, "gateserver request gameserver pick up item and return");

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_PICKITEM,message,static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}



