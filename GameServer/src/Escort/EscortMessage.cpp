/*
 * EscortMessage.cpp
 *
 *  Created on: 2014年7月21日
 *      Author: root
 */

#include "EscortMessage.h"
//#include "../ServerManager.h"
//#include "Log/Logger.h"
//#include "EscortMgr.h"
//#include "MessageStruct/ServerReturnInt.pb.h"
//#include "../Object/Player/Player.h"
//#include "../Object/Player/PlayerMessage.h"
//#include "../Map/GameZone.h"
//#include "../Map/SceneManager.h"
//#include "../MessageBuild.h"
//#include "../Map/ConfigLoader/MapConfig.h"
//#include "MsgCommonClass.h"
//#include "Smart_Ptr.h"
//#include <netinet/in.h>
//#include "MessageCommonRet.h"
//#include <string.h>
//#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
//#include "EscortMgr.h"
//#include "CounterDefine.h"
//
//EscortMessage::EscortMessage()
//{
//	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTGETQUALITY);		//领取皇纲品质
//	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTFRESH);		//刷新皇纲
//	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTSTART);		//开始运镖
//	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTIMEDCOMPLETE);		//立即完成
//	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTSPEEDUP);		//加速
//	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTABANDON);		   //放弃
//	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTGETRPIZE);		   //领取奖励
//	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_OPENESCORT);
//}
//
//EscortMessage::~EscortMessage()
//{
//
//}
//
//DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTGETQUALITY)
//{
//	EscortProto::RqGetQuality req;
//	EscortProto::RtGetQuality toClient;
//	ServerMessage::ClientSingleMessage ack;
//
//	req.ParseFromString(content);
//
//	int error = 0;
//	if((req.quality() > 0) && (req.quality() < (int)(eEscortMax)))
//	{
//		error = sEscortMgr->SetEscortQuality(player,(eCarType)(req.quality()));
//	}
//	toClient.set_error(error);
//	toClient.set_quality(req.quality());
//
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_ESCORTGETQUALITY);
//
//}
//
//DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTFRESH)
//{
//	EscortProto::RqFreshTreature req;
//	EscortProto::RtFreshTreature toClient;
//	ServerMessage::ClientSingleMessage ack;
//
//	req.ParseFromString(content);
//
//	DWORD quality = 0;
//	int error = sEscortMgr->GenenateQuality(player,req.escortid(),quality);
//	toClient.set_quality(quality);
//	toClient.set_error(error);
//	int freshtimes = player->GetCounterService()->GetNumById(ESCORT_FREETIMES_COUNTER);
//	toClient.set_freshtimes(freshtimes);
//
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_ESCORTFRESH);
//
//}
//
//DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTSTART)
//{
//	EscortProto::RqStartEscort req;
//	EscortProto::RtStartEscort toClient;
//	ServerMessage::ClientSingleMessage ack;
//
//	req.ParseFromString(content);
//
//	int error = sEscortMgr->StartEscort(player);
//	toClient.set_error(error);
//
//	if(!error)
//	{
//		sEscortData *data = sEscortMgr->GetPlayerEscortData(player->GetID());
//		if(data)
//		{
//			toClient.set_line(data->line);
//			toClient.set_quality(data->quality);
//		}
//	}
//
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_ESCORTSTART);
//
//}
//
//DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTIMEDCOMPLETE)
//{
//	EscortProto::RqImediaComplete req;
//	EscortProto::RtImediaComplete toClient;
//	ServerMessage::ClientSingleMessage ack;
//
//	req.ParseFromString(content);
//
//	int error = 0;
//	error = sEscortMgr->ImediaComplete(player);
//	toClient.set_error(error);
//
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_ESCORTIMEDCOMPLETE);
//
//}
//
//DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTSPEEDUP)
//{
//	EscortProto::RqSpeedUp req;
//	EscortProto::RtSpeedUp toClient;
//	ServerMessage::ClientSingleMessage ack;
//
//	req.ParseFromString(content);
//
//	int error = sEscortMgr->SpeedUpEscort(player);
//	toClient.set_error(error);
//
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_ESCORTSPEEDUP);
//
//}
//
//DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTABANDON)
//{
//	EscortProto::RqAbandon req;
//	EscortProto::RtAbandon toClient;
//	ServerMessage::ClientSingleMessage ack;
//
//	req.ParseFromString(content);
//
//	sEscortMgr->AbandonEscort(player);
//
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_ESCORTABANDON);
//
//}
//
//DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTGETRPIZE)
//{
//	EscortProto::RqGetPrize req;
//	EscortProto::RtGetPrize toClient;
//	ServerMessage::ClientSingleMessage ack;
//
//	req.ParseFromString(content);
//
//	sEscortMgr->EndEscort(player);
//
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_ESCORTGETRPIZE);
//
//}
//
//DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_OPENESCORT)
//{
//	EscortProto::RqOpenEscort req;
//	EscortProto::RtOpenEscort toClient;
//	ServerMessage::ClientSingleMessage ack;
//
//	req.ParseFromString(content);
//
//	//sEscortMgr->EndEscort(player);
//	sEscortData *info = sEscortMgr->GetPlayerEscortData(player->GetID());
//	if(player->GetCounterService())
//	{
//		int freshtimes = player->GetCounterService()->GetNumById(ESCORT_FREETIMES_COUNTER);
//		toClient.set_freshtimes(freshtimes);
//		int dotimes = player->GetCounterService()->GetNumById(ESCORT_DAY_COUNTER);
//		toClient.set_dotimes(dotimes);
//	}
//	else
//	{
//		return;
//	}
//
//	if(info)
//	{
//		toClient.set_quality(info->quality);
//	}
//	else
//	{
//		toClient.set_quality(eEscortBronze);
//	}
//
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_OPENESCORT);
//}
