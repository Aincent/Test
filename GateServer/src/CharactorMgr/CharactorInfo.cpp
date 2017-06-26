/*
 * CharactorInfo.cpp
 *
 *  Created on: 2013��10��26��
 *      Author: helh
 */
#include "CharactorInfo.h"
#include "SvrConfig.h"
#include "MessageStruct/ServerReturnChar.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/CharBase/CharAttribute.pb.h"
#include "MessageStruct/CharLogin/CharBaseInfo.pb.h"
#include "Network/Message_Facade.h"
#include "Log/Logger.h"
#include "../ServerManager.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageStruct/CharLogin/Horse.pb.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"

CharactorInfo* CharactorInfo::m_instance = 0;

CharactorInfo::CharactorInfo()
{
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_CHARATTR);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_HORSEINFO);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_HANDLEHORSE);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_HORSEADD);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_CHARATTR);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_HORSEINFO);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_HANDLEHORSE);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_HORSEADD);
}

CharactorInfo::~CharactorInfo()
{

}

void CharactorInfo::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN;

	DEF_SWITCH_TRY_DISPATCH_END;
}

void CharactorInfo::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN;

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_CHARATTR);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_HORSEINFO);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_HANDLEHORSE);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_HORSEADD);

	DEF_SWITCH_TRY_DISPATCH_END;
}

void CharactorInfo::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN;

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_CHARATTR);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_HORSEINFO);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_HANDLEHORSE);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_HORSEADD);

	DEF_SWITCH_TRY_DISPATCH_END;
}

DEF_MSG_REQUEST_DEFINE_FUN(CharactorInfo, MSG_REQ_C2GT_CHARATTR)
{
	LOG_DEBUG(FILEINFO, "client request player battle attribute");

	int gsChannel = -1;
	int64 charid = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel,charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_CHARATTR, message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(CharactorInfo, MSG_REQ_C2GT_HORSEINFO)
{
	LOG_DEBUG(FILEINFO, "client request player Horse info");

	int64 charid = -1;
	int gsChannel = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_HORSEINFO,message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(CharactorInfo, MSG_REQ_C2GT_HANDLEHORSE)
{
	LOG_DEBUG(FILEINFO, "client request handle Horse info");

	int64 charid = -1;
	int gsChannel = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_HANDLEHORSE,message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		//messageTime用来存储玩家ID
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(CharactorInfo, MSG_REQ_C2GT_HORSEADD)
{
	LOG_DEBUG(FILEINFO, "client request Horse add");

	int64 charid = -1;
	int gsChannel = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_HORSEADD,message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(CharactorInfo, MSG_REQ_GT2GM_CHARATTR)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver player battle attribute and ack timeout");

		return;
	}

	LOG_DEBUG(FILEINFO,"gateserver request gameserver player battle attribute and ack ");

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_CHARATTR,message,static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}

DEF_MSG_ACK_DEFINE_FUN(CharactorInfo, MSG_REQ_GT2GM_HORSEINFO)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver player horse info and ack timeout");

		return;
	}

	LOG_DEBUG(FILEINFO,"gateserver request gameserver player horse info and ack ");

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_HORSEINFO,message,static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}

DEF_MSG_ACK_DEFINE_FUN(CharactorInfo, MSG_REQ_GT2GM_HANDLEHORSE)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver player handle horse and ack timeout");

		return;
	}

	LOG_DEBUG(FILEINFO,"gateserver request gameserver player handle horse and ack ");

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_HANDLEHORSE, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}

DEF_MSG_ACK_DEFINE_FUN(CharactorInfo, MSG_REQ_GT2GM_HORSEADD)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver player to add horse exp and ack timeout");

		return;
	}

	LOG_DEBUG(FILEINFO,"gateserver request gameserver player to add horse exp and ack ");

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_HORSEADD,message,static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}




