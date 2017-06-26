/*
 * FriendManager.cpp
 *
 *  Created on: 2014年5月7日
 *      Author: root
 */


#include "FriendManager.h"
#include "SvrConfig.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageStruct/Friend/FriendMessage.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/CharLogin/CharBaseInfo.pb.h"

FriendManager * FriendManager::m_instance = 0;

FriendManager::FriendManager()
{
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_FINDFRIEND);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_OPENFRIEND);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_ADDFRIEND);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_DELETEFRIEND);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_SEEOTHERPLAYERINFO);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_RECOMMEND_FRIEND);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_CT2WS_OPENFRIEND);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_CT2WS_FINDFRIEND);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_CT2WS_ADDFRIEND);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_CT2WS_DELETEFRIEND);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_SEEOTHERPLAYERINFO);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2WS_ADDHATRED);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2WS_RECOMMEND_FRIEND);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_WS2CT_SYNUPDATE);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_WS2C_SYNADDFRIEND);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_WS2GT_BLESSFRIEND);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_GM2GT_ADDHATRED);
}

FriendManager::~FriendManager()
{

}

void FriendManager::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2CT_SYNUPDATE);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2C_SYNADDFRIEND);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GT_BLESSFRIEND);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GM2GT_ADDHATRED);

	DEF_SWITCH_TRY_DISPATCH_END
}

void FriendManager::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_FINDFRIEND);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_OPENFRIEND);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_ADDFRIEND);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_DELETEFRIEND);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_SEEOTHERPLAYERINFO);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_RECOMMEND_FRIEND);
	DEF_SWITCH_TRY_DISPATCH_END

}

void FriendManager::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_CT2WS_OPENFRIEND);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_CT2WS_FINDFRIEND);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_CT2WS_ADDFRIEND);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_CT2WS_DELETEFRIEND);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_SEEOTHERPLAYERINFO);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2WS_ADDHATRED);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2WS_RECOMMEND_FRIEND);

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_REQUEST_DEFINE_FUN(FriendManager, MSG_REQ_C2GT_RECOMMEND_FRIEND)
{

	LOG_DEBUG(FILEINFO,"client request gateserver recommended friend");

	int64 charid = -1;

	charid = ServerConHandler::GetInstance()->GetCharidByChannel(message->GetChannelID());
	if (0 == charid)
	{
		LOG_ERROR(FILEINFO, "GetCharidByChannel Return charid Error!!!");
		return;
	}

	if (NULL == ServerConHandler::GetInstance()->GetWorldServer() || -1 == ServerConHandler::GetInstance()->GetWorldServer()->channelID)
	{
		LOG_ERROR(FILEINFO, "GetWorldServer Retrun Data Error !!!!");
		return;
	}

	Safe_Smart_Ptr<CommBaseOut::Message> worldmess  = build_message(MSG_REQ_GT2WS_RECOMMEND_FRIEND, message, ServerConHandler::GetInstance()->GetWorldServer()->channelID, Request);
	worldmess->SetAct(new requestAct(message));
	worldmess->SetMessageTime(charid);
	Message_Facade::Send(worldmess);

}

DEF_MSG_REQUEST_DEFINE_FUN(FriendManager, MSG_REQ_C2GT_FINDFRIEND)
{
	LOG_DEBUG(FILEINFO,"gateserver request worldserver find friend");

	int64 charid = -1;

	charid = ServerConHandler::GetInstance()->GetCharidByChannel(message->GetChannelID());
	if (0 == charid)
	{
		LOG_ERROR(FILEINFO, "GetCharidByChannel Return charid Error!!!");
		return;
	}

	if (NULL == ServerConHandler::GetInstance()->GetWorldServer() || -1 == ServerConHandler::GetInstance()->GetWorldServer()->channelID)
	{
		LOG_ERROR(FILEINFO, "GetWorldServer Retrun Data Error !!!!");
		return;
	}

	Safe_Smart_Ptr<CommBaseOut::Message> worldmess  = build_message(MSG_REQ_CT2WS_FINDFRIEND, message, ServerConHandler::GetInstance()->GetWorldServer()->channelID, Request);
	worldmess->SetAct(new requestAct(message));
	Message_Facade::Send(worldmess);
}

DEF_MSG_REQUEST_DEFINE_FUN(FriendManager, MSG_REQ_C2GT_OPENFRIEND)
{
	LOG_DEBUG(FILEINFO,"gateserver request worldserver open friend list");

	int64 charid = -1;

	charid = ServerConHandler::GetInstance()->GetCharidByChannel(message->GetChannelID());
	if (0 == charid)
	{
		LOG_ERROR(FILEINFO, "GetCharidByChannel Return charid Error!!!");
		return;
	}

	if (NULL == ServerConHandler::GetInstance()->GetWorldServer() || -1 == ServerConHandler::GetInstance()->GetWorldServer()->channelID)
	{
		LOG_ERROR(FILEINFO, "GetWorldServer Retrun Data Error !!!!");
		return;
	}

	Safe_Smart_Ptr<CommBaseOut::Message> worldmess  = build_message(MSG_REQ_CT2WS_OPENFRIEND, message, ServerConHandler::GetInstance()->GetWorldServer()->channelID, Request);
	worldmess->SetAct(new requestAct(message));
	worldmess->SetMessageTime(charid);
	Message_Facade::Send(worldmess);
}

DEF_MSG_REQUEST_DEFINE_FUN(FriendManager, MSG_REQ_C2GT_ADDFRIEND)
{
	LOG_DEBUG(FILEINFO,"gateserver request worldserver add friend");

	int64 charid = -1;

	charid = ServerConHandler::GetInstance()->GetCharidByChannel(message->GetChannelID());
	if (0 == charid)
	{
		LOG_ERROR(FILEINFO, "GetCharidByChannel Return charid Error!!!");
		return;
	}

	if (NULL == ServerConHandler::GetInstance()->GetWorldServer() || -1 == ServerConHandler::GetInstance()->GetWorldServer()->channelID)
	{
		LOG_ERROR(FILEINFO, "GetWorldServer Retrun Data Error !!!!");
		return;
	}

	Safe_Smart_Ptr<CommBaseOut::Message> worldmess  = build_message(MSG_REQ_CT2WS_ADDFRIEND, message, ServerConHandler::GetInstance()->GetWorldServer()->channelID, Request);
	worldmess->SetAct(new requestAct(message));
	worldmess->SetMessageTime(charid);
	Message_Facade::Send(worldmess);
}

DEF_MSG_REQUEST_DEFINE_FUN(FriendManager, MSG_REQ_C2GT_DELETEFRIEND)
{
	LOG_DEBUG(FILEINFO,"gateserver request worldserver delete friend");

	int64 charid;

	charid = ServerConHandler::GetInstance()->GetCharidByChannel(message->GetChannelID());
	if (0 == charid)
	{
		LOG_ERROR(FILEINFO, "GetCharidByChannel Return charid Error!!!");
		return;
	}

	if (NULL == ServerConHandler::GetInstance()->GetWorldServer() || -1 == ServerConHandler::GetInstance()->GetWorldServer()->channelID)
	{
		LOG_ERROR(FILEINFO, "GetWorldServer Retrun Data Error !!!!");
		return;
	}

	Safe_Smart_Ptr<CommBaseOut::Message> worldmess  = build_message(MSG_REQ_CT2WS_DELETEFRIEND, message, ServerConHandler::GetInstance()->GetWorldServer()->channelID, Request);
	worldmess->SetAct(new requestAct(message));
	worldmess->SetMessageTime(charid);
	Message_Facade::Send(worldmess);
}

DEF_MSG_REQUEST_DEFINE_FUN(FriendManager, MSG_REQ_C2GT_SEEOTHERPLAYERINFO)
{
	LOG_DEBUG(FILEINFO,"client request gateserver see other friend info");

	int64 charid = -1;
	int gsChannel = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> msg  = build_message(MSG_REQ_GT2GM_SEEOTHERPLAYERINFO, message, gsChannel, Request);
		msg->SetAct(new requestAct(message));
		msg->SetMessageTime(charid);
		Message_Facade::Send(msg);
	}
}

DEF_MSG_ACK_DEFINE_FUN(FriendManager,MSG_REQ_CT2WS_OPENFRIEND)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request worldserver open friend list");
		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_OPENFRIEND, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(FriendManager,MSG_REQ_CT2WS_FINDFRIEND)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request worldserver find friend");
		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_FINDFRIEND, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(FriendManager,MSG_REQ_CT2WS_ADDFRIEND)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request worldserver add friend timeout");
		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_ADDFRIEND, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(FriendManager,MSG_REQ_GT2WS_RECOMMEND_FRIEND)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request worldserver recommended friend timeout");
		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_RECOMMEND_FRIEND, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(FriendManager,MSG_REQ_CT2WS_DELETEFRIEND)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request worldserver delete friend timeout");
		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_DELETEFRIEND, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(FriendManager,MSG_REQ_GT2GM_SEEOTHERPLAYERINFO)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request worldserver delete friend timeout");
		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_SEEOTHERPLAYERINFO, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(FriendManager, MSG_SIM_WS2CT_SYNUPDATE)
{
	int channel = -1;
	int64 charID = message->GetMessageTime();

	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(charID, channel))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_CT2C_SYNUPDATE, message, channel, SimpleMessage);
		Message_Facade::Send(clientRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "gateserver get worldserver broadcast friend online but find not client[ charid=%lld]",GET_PLAYER_CHARID(charID));
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(FriendManager, MSG_SIM_WS2C_SYNADDFRIEND)
{
	int channel = -1;

	int64 charID = message->GetMessageTime();

	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(charID, channel))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_CT2C_SYNADDFRIEND, message, channel, SimpleMessage);
		Message_Facade::Send(clientRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "gateserver get worldserver broadcast friend add but find not client[ charid=%lld]",GET_PLAYER_CHARID(charID));
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(FriendManager, MSG_SIM_WS2GT_BLESSFRIEND)
{
	int channel = -1;
	int64 charID = message->GetMessageTime();

	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(charID, channel))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_GT2C_BLESSFRIEND, message, channel, SimpleMessage);
		Message_Facade::Send(clientRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "gateserver get worldserver broadcast friend add but find not client[ charid=%lld]",GET_PLAYER_CHARID(charID));
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(FriendManager, MSG_SIM_GM2GT_ADDHATRED)
{
	LOG_DEBUG(FILEINFO,"gateserver request worldserver add harted value");

	if (NULL == ServerConHandler::GetInstance()->GetWorldServer() || -1 == ServerConHandler::GetInstance()->GetWorldServer()->channelID)
	{
		LOG_ERROR(FILEINFO, "GetWorldServer Retrun Data Error !!!!");
		return;
	}

	Safe_Smart_Ptr<CommBaseOut::Message> worldmess  = build_message(MSG_REQ_GT2WS_ADDHATRED, message, ServerConHandler::GetInstance()->GetWorldServer()->channelID, Request);
	worldmess->SetAct(new requestAct(message));
	Message_Facade::Send(worldmess);
}

DEF_MSG_ACK_DEFINE_FUN(FriendManager,MSG_REQ_GT2WS_ADDHATRED)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request worldserver add friend timeout");
		return;
	}

	int channel = -1;
	int64 charID = message->GetMessageTime();

	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(charID, channel))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_SIM_GT2C_ADDHATRED, message, channel, SimpleMessage);
		Message_Facade::Send(messRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "gateserver get gameserver add friend but find not client[ charid=%lld]",GET_PLAYER_CHARID(charID));
	}
}
