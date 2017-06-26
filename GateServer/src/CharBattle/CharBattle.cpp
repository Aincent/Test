/*
 * CharBase.cpp
 *
 *  Created on: 2013��11��12��
 *      Author: helh
 */

#include "CharBattle.h"
#include "SvrConfig.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "./MessageStruct/CharBattle/Battle.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "./MessageStruct/CharLogin/Monitor.pb.h"

CharBattleMgr * CharBattleMgr::m_instance = 0;


CharBattleMgr::CharBattleMgr()
{
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_SIM_C2GT_ATTACK);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_C2GT_REVIVAL);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_C2GT_MONITOR);

	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_UPGRADE_SKILL);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_UPGRADE_SKILL);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_SIM_GT2GM_ATTACK);

}

CharBattleMgr::~CharBattleMgr()
{

}


void CharBattleMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_C2GT_REVIVAL);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_C2GT_MONITOR);

	DEF_SWITCH_TRY_DISPATCH_END
}

void CharBattleMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_UPGRADE_SKILL);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_SIM_C2GT_ATTACK);

	DEF_SWITCH_TRY_DISPATCH_END
}

void CharBattleMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_UPGRADE_SKILL);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_SIM_GT2GM_ATTACK);

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_REQUEST_DEFINE_FUN(CharBattleMgr, MSG_SIM_C2GT_ATTACK)
{
	int gsChannel = -1;
	int64 charid = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_SIM_GT2GM_ATTACK, message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player[charid=%lld] attack but gameserver not existed", GET_PLAYER_CHARID(charid));
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(CharBattleMgr, MSG_SIM_C2GT_MONITOR)
{
	int64 charid = -1;
	int gsChannel = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_SIM_GT2GM_MONITOR, message, gsChannel, SimpleMessage);
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(CharBattleMgr, MSG_REQ_C2GT_UPGRADE_SKILL)
{
	int gsChannel = -1;
	int64 charid = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_UPGRADE_SKILL, message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(CharBattleMgr, MSG_REQ_GT2GM_UPGRADE_SKILL)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request upgrade skill and ack timeout");

		return;
	}

	Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_REQ_C2GT_UPGRADE_SKILL, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(clientRet);
}

DEF_MSG_ACK_DEFINE_FUN(CharBattleMgr, MSG_SIM_GT2GM_ATTACK)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request fire skill and ack timeout");

		return;
	}

	Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_C2GT_ATTACK, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(clientRet);
}

DEF_MSG_SIMPLE_DEFINE_FUN(CharBattleMgr, MSG_SIM_C2GT_REVIVAL)
{
	int gsChannel = -1;
	int64 charid = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_SIM_GT2GM_REVIVAL, message, gsChannel, SimpleMessage);
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}
