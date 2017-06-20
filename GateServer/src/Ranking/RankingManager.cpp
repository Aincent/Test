/*
 * RankingManager.cpp
 *
 *  Created on: 2014年5月7日
 *      Author: root
 */
#include "RankingManager.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/CharLogin/Ranking.pb.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"

RankingMgr * RankingMgr::m_instance = 0;

RankingMgr::RankingMgr()
{
	DEF_MSG_REQUEST_REG_FUN(eGateServer,MSG_REQ_C2GT_RANKING);
	DEF_MSG_REQUEST_REG_FUN(eGateServer,MSG_REQ_C2GT_MERANKING);
//	DEF_MSG_REQUEST_REG_FUN(eGateServer,MSG_REQ_GM2GT_GLORY);
	DEF_MSG_ACK_REG_FUN(eGateServer,MSG_REQ_GT2WS_RANKING);
	DEF_MSG_ACK_REG_FUN(eGateServer,MSG_REQ_GT2WS_MERANKING);
//	DEF_MSG_ACK_REG_FUN(eGateServer,MSG_REQ_GT2WS_GLORY);
}

RankingMgr::~RankingMgr()
{

}

void RankingMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_SWITCH_TRY_DISPATCH_END
}

void RankingMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_RANKING);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_MERANKING);
//	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GM2GT_GLORY);
	DEF_SWITCH_TRY_DISPATCH_END
}

void RankingMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2WS_RANKING);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2WS_MERANKING);
//	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2WS_GLORY);
	DEF_SWITCH_TRY_DISPATCH_END
}
DEF_MSG_REQUEST_DEFINE_FUN(RankingMgr,MSG_REQ_C2GT_RANKING)
{
	LOG_DEBUG(FILEINFO, "client request ranking");
//	RankingInfo::ReqRankingInfo req;
//	RankingInfo::GTtoWSRankingInfo reqWS;
	int64 charid = -1;
//	int len = 0;
//	char * content = message->GetBuffer(len);
//
//	req.ParseFromArray(content, len);
	charid = ServerConHandler::GetInstance()->GetCharidByChannel(message->GetChannelID());

//	reqWS.set_charid(charid);
//	reqWS.set_type(req.type());
//	reqWS.set_index(req.index());
	ConItem * wsSvrItem = ServerConHandler::GetInstance()->GetWorldServer();
	if(NULL == wsSvrItem)
		return;
	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2WS_RANKING,message,wsSvrItem->channelID, Request);
	messRet->SetAct(new requestAct(message));
	messRet->SetMessageTime(charid);
	Message_Facade::Send(messRet);
}

DEF_MSG_ACK_DEFINE_FUN(RankingMgr,MSG_REQ_GT2WS_RANKING)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request worldserver ranking timeout");
		return;
	}
	LOG_DEBUG(FILEINFO, "return client request ranking");
//	RankingInfo::RankingInfo ret;
//	int len = 0;
//	char * content = message->GetBuffer(len);
//	ret.ParseFromArray(content, len);
	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_RANKING,message,static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}

DEF_MSG_REQUEST_DEFINE_FUN(RankingMgr,MSG_REQ_C2GT_MERANKING)
{
	LOG_DEBUG(FILEINFO, "client request me ranking");
//	RankingInfo::ReqMeRankingInfo req;
//	RankingInfo::GTtoWSMeRankingInfo reqWS;
	int64 charid = -1;
//	int len = 0;
//	char * content = message->GetBuffer(len);
//
//	req.ParseFromArray(content, len);
	charid = ServerConHandler::GetInstance()->GetCharidByChannel(message->GetChannelID());
//	reqWS.set_charid(charid);
//	reqWS.set_type(req.type());

	ConItem * wsSvrItem = ServerConHandler::GetInstance()->GetWorldServer();
	if(NULL == wsSvrItem)
		return;

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2WS_MERANKING,message,wsSvrItem->channelID, Request);
	messRet->SetAct(new requestAct(message));
	messRet->SetMessageTime(charid);
	Message_Facade::Send(messRet);
}

//DEF_MSG_REQUEST_DEFINE_FUN(RankingMgr,MSG_REQ_GM2GT_GLORY)
//{
//	LOG_DEBUG(FILEINFO, "client request me ranking");
////	RankingInfo::GTtoWSMeRankingInfo reqWS;
////	int len = 0;
////	char * content = message->GetBuffer(len);
////
////	reqWS.ParseFromArray(content, len);
//
//	ConItem * wsSvrItem = ServerConHandler::GetInstance()->GetWorldServer();
//	if(NULL == wsSvrItem)
//		return;
//
//	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2WS_GLORY,message,wsSvrItem->channelID, Request);
//	messRet->SetAct(new requestAct(message));
//	Message_Facade::Send(messRet);
//}

DEF_MSG_ACK_DEFINE_FUN(RankingMgr,MSG_REQ_GT2WS_MERANKING)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request worldserver me ranking timeout");
		return;
	}
	LOG_DEBUG(FILEINFO, "return client request me ranking");
//	RankingInfo::MeRankingInfo ret;
//	int len = 0;
//	char * content = message->GetBuffer(len);
//	ret.ParseFromArray(content, len);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_MERANKING,message,static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}
//DEF_MSG_ACK_DEFINE_FUN(RankingMgr,MSG_REQ_GT2WS_GLORY)
//{
//	if(message->GetErrno() == eReqTimeOut)
//	{
//		LOG_WARNING(FILEINFO, "gateserver request worldserver me ranking timeout");
//		return;
//	}
////	ServerReturn::ServerDoubleInt retGM;
////	int len = 0;
////	char * content = message->GetBuffer(len);
////	retGM.ParseFromArray(content, len);
//
//	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2GT_GLORY,message,static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
//	Message_Facade::Send(messRet);
//}
