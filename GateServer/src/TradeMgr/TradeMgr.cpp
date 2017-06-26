/*
 * TradeMgr.cpp
 *
 *  Created on: 2014.5.5
 *      Author: CZ
 */

#include "TradeMgr.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "MessageStruct/CharLogin/NpcTrade.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageStruct/ServerReturnInt.pb.h"

TradeMgr * TradeMgr::m_instance = 0;

TradeMgr::TradeMgr()
{

//	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_GM2GT_UPDATE_QUESTTARGET);
//	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_GM2GT_SYNQUEST);

	//NPC交易消息
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_BUYNPCITEM);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_SELLITEM);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_BUYBACK);

	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_BUYNPCITEM);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_SELLITEM);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_BUYBACK);

	//玩家交易消息
}

TradeMgr::~TradeMgr()
{

}

void TradeMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

//	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GM2GT_UPDATE_QUESTTARGET);
//	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GM2GT_SYNQUEST);
	//DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_C2GT_CLIENTIN);

	DEF_SWITCH_TRY_DISPATCH_END
}

void TradeMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_BUYNPCITEM);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_SELLITEM);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_BUYBACK);

	DEF_SWITCH_TRY_DISPATCH_END
}

void TradeMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_BUYNPCITEM);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_SELLITEM);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_BUYBACK);

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_REQUEST_DEFINE_FUN(TradeMgr, MSG_REQ_C2GT_BUYNPCITEM)
{
	LOG_DEBUG(FILEINFO, "client request buy Npc Items");

//	NpcTrade::ClientReqBuyItem clientReq;
//	NpcTrade::GTtoGMReqBuyItem reqGM;
	int gsChannel = -1;
	int64 charid = -1;
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	clientReq.ParseFromArray(str, len);

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
//		reqGM.set_charid(charid);
//		reqGM.set_npcid(clientReq.npcid());
//		reqGM.set_itemid(clientReq.itemid());
//		reqGM.set_itemnum(clientReq.itemnum());

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_BUYNPCITEM,message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(TradeMgr, MSG_REQ_C2GT_SELLITEM)
{
	LOG_DEBUG(FILEINFO, "client request sell item to npc");

//	NpcTrade::ClientReqSellItem clientReq;
//	NpcTrade::GTtoGMReqSellItem reqGM;
	int gsChannel = -1;
	int64 charid = -1;
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	clientReq.ParseFromArray(str, len);

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
//		reqGM.set_charid(charid);
//		reqGM.set_packetgridid(clientReq.packetgridid());
//		reqGM.set_besuresell(clientReq.besuresell());

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_SELLITEM,message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(TradeMgr, MSG_REQ_C2GT_BUYBACK)
{
	LOG_DEBUG(FILEINFO, "client request buy items back");

//	NpcTrade::ClientReqBuyBack clientReq;
//	NpcTrade::GTtoGMReqBuyBack reqGM;
	int gsChannel = -1;
	int64 charid = -1;
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	clientReq.ParseFromArray(str, len);

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
//		reqGM.set_charid(charid);
//		reqGM.set_buybackindex(clientReq.buybackindex());

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_BUYBACK,message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(TradeMgr, MSG_REQ_GT2GM_BUYNPCITEM)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver player buy npc items and ack timeout");

		return;
	}

	LOG_DEBUG(FILEINFO, "gateserver request gameserver buy npc items and ack from gameserver");

//	ServerReturn::ServerRetInt ack;
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	ack.ParseFromArray(str, len);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_BUYNPCITEM,message,static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}

DEF_MSG_ACK_DEFINE_FUN(TradeMgr, MSG_REQ_GT2GM_SELLITEM)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver player sell item and ack timeout");

		return;
	}

	LOG_DEBUG(FILEINFO, "gateserver request gameserver sell item and ack from gameserver");

//	NpcTrade::GTAckToCSellItem ack;
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	ack.ParseFromArray(str, len);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_SELLITEM,message,static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}

DEF_MSG_ACK_DEFINE_FUN(TradeMgr, MSG_REQ_GT2GM_BUYBACK)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver player buy back and ack timeout");

		return;
	}


	LOG_DEBUG(FILEINFO, "gateserver request gameserver buy back and ack from gameserver");

//	ServerReturn::ServerRetInt ack;
//		int len = 0;
//		char *str = message->GetBuffer(len);
//
//	ack.ParseFromArray(str, len);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_BUYBACK,message,static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}
