///*
// * WorshipMessage.cpp
// *
// *  Created on: 2015年3月25日
// *      Author: root
// */
//
//#include "WorshipMessage.h"
//#include "WorshipMgr.h"
//#include "../MessageBuild.h"
//#include "../Object/Player/PlayerMessage.h"
//#include "MessageCommonRet.h"
//
//WorshipMessage::WorshipMessage()
//{
////	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_WORSHIP);
//
////	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GB_ADDBYWORSHIP);
//
////	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_CHECKWORSHIP);
//}
//
//WorshipMessage::~WorshipMessage()
//{
//}
//
//void WorshipMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
//{
//	DEF_SWITCH_TRY_DISPATCH_BEGIN
//
//	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_WORSHIP);
//
//	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GB_ADDBYWORSHIP);
//
//	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_CHECKWORSHIP);
//
//	DEF_SWITCH_TRY_DISPATCH_END
//}
//
//void WorshipMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
//{
//	DEF_SWITCH_TRY_DISPATCH_BEGIN
//
//
//	DEF_SWITCH_TRY_DISPATCH_END
//}
//
//DEF_MSG_REQUEST_DEFINE_FUN(WorshipMessage, MSG_REQ_WS2GM_WORSHIP)
//{
//	WorshipMsgProto::ReqWorship req;
//	WorshipMsgProto::RetWorship ret;
////	WorshipMsgProto::ReqWorshipToDB toDB;
//
//	int len = 0;
//	char* content = NULL;
//	content = message->GetBuffer(len);
//
//	req.ParseFromArray(content, len);
//
//	Smart_Ptr<Player> player;
//	PlayerMessage::GetInstance()->GetPlayerByCharId(player, req.selfid()); // 获得膜拜者的Player
//
//	if ((bool)player)
//	{
//		if (!player->GetWorshipMgr()->IsWorshipPlayer(req.worcharbyself())) // 是否膜拜过这个玩家了
//		{
//			Smart_Ptr<Player> otherPlayer;
//			PlayerMessage::GetInstance()->GetPlayerByCharId(otherPlayer, req.worcharbyself());
//
//
////			if ((bool)otherPlayer) // 是否在线
////			{
//			int worAmtByOther = 0, surplusWorCnt = 0;
//			int error = player->GetWorshipMgr()->WorshipOnline(req.worshiptimes(), req.worcharbyself(), worAmtByOther, surplusWorCnt);
//
//			ret.set_error(error);
//			ret.set_worshipamount(worAmtByOther);
//			ret.set_surplusworcnt(surplusWorCnt);
//		}
//		else
//		{
//			ret.set_error(eWorshipAlreadyChar);
//		}
////			Safe_Smart_Ptr<CommBaseOut::Message> msg = build_message(MSG_REQ_WS2GM_WORSHIP, message, &ret, Ack);
////			Message_Facade::Send(msg);
////			}
////			else
////			{ // 如果不在线,请求DB服向数据库写
////				Smart_Ptr<SvrItem> dbSvr;
////				ServerConHandler::GetInstance()->GetDBServerBySvrID(player->GetID() >> 32, dbSvr);
////
////				Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2DB_WORSHIP, &req, dbSvr.Get(), Request);
////
////				Message_Facade::Send(messRet);
////			}
//
//
//			Safe_Smart_Ptr<CommBaseOut::Message> msg = build_message(MSG_REQ_WS2GM_WORSHIP, message, &ret, Ack);
//			Message_Facade::Send(msg);
//
//
//	}
//}
//
//
//DEF_MSG_REQUEST_DEFINE_FUN(WorshipMessage, MSG_REQ_WS2GB_ADDBYWORSHIP)
//{
//	WorshipMsgProto::RetWorshipToGM ret;
//	WorshipMsgProto::RetWorship toWS;
//	int len = 0;
//	char* content = NULL;
//
//	content = message->GetBuffer(len);
//	ret.ParseFromArray(content, len);
//
//	Smart_Ptr<Player> player;
//	PlayerMessage::GetInstance()->GetPlayerByCharId(player, ret.selfid());
//
//	if ((bool)player)
//	{
//		int surplusWorCnt = 0;
//		toWS.set_error(player->GetWorshipMgr()->WorshipOffline(ret.worcharbyself(), surplusWorCnt));
//		toWS.set_surplusworcnt(surplusWorCnt);
//		toWS.set_worshipamount(ret.worshipamount());
//
//		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(message, &toWS, Ack);
//		Message_Facade::Send(messRet);
//	}
//}
//
//DEF_MSG_REQUEST_DEFINE_FUN(WorshipMessage, MSG_REQ_WS2GM_CHECKWORSHIP)
//{
//	WorshipMsgProto::ReqCheckWorship req;
//	WorshipMsgProto::RetCheckWorship ret;
//
//	WorshipMsgProto::ReqWorship checkInfo;
//
//	int len = 0;
//	char* content = NULL;
//
//	content = message->GetBuffer(len);
//	req.ParseFromArray(content, len);
//
//	checkInfo = req.checkinfo();
//
//
//	Smart_Ptr<Player> player;
//	PlayerMessage::GetInstance()->GetPlayerByCharId(player, checkInfo.selfid());
//
//	int error = 0;
//	if ((bool)player)
//	{
//		int surplusAmt = 0;
//		error = player->GetWorshipMgr()->GetTodaySurplusAmt(surplusAmt); // 今天剩余次数
//
//		if (0 == error )
//		{
//			if (player->GetWorshipMgr()->IsWorshipPlayer(checkInfo.worcharbyself())) // 是否膜拜过某玩家
//			{
//				error = eWorshipAlreadyChar;
//			}
//		}
//	}
//	ret.set_error(error);
//	ret.mutable_worshipinfo()->CopyFrom(checkInfo);
//
//	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(message, &ret, Ack);
//	Message_Facade::Send(messRet);
//}
