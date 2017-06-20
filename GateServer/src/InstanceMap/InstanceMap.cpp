/*
 * InstanceMap.cpp
 *
 *  Created on: 2014年6月5日
 *      Author: root
 */

#include "InstanceMap.h"
#include "SvrConfig.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "MessageStruct/CharLogin/NpcTrade.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/CharLogin/MapInstanceProto.pb.h"
#include "MessageStruct/Map/GS2GTAllMap.pb.h"

InstanceMap * InstanceMap::m_instance = 0;

InstanceMap::InstanceMap()
{
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_CREATEINSTANCE);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2WS_CREATEINSTANCE);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_REQ_GM2GT_ENDINSTANCE);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_REQ_GM2GT_DELETEINSTANCE);
}


InstanceMap::~InstanceMap()
{

}

void InstanceMap::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_REQ_GM2GT_ENDINSTANCE);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_REQ_GM2GT_DELETEINSTANCE);


	DEF_SWITCH_TRY_DISPATCH_END
}

void InstanceMap::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_CREATEINSTANCE);

	DEF_SWITCH_TRY_DISPATCH_END
}

void InstanceMap::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2WS_CREATEINSTANCE);
	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_SIMPLE_DEFINE_FUN(InstanceMap, MSG_REQ_GM2GT_DELETEINSTANCE)
{
	MapInstanceProto::ReqDeleteInstance req;

	int len = 0;

	char * content = message->GetBuffer(len);
	req.ParseFromArray(content, len);

	{
		LOG_DEBUG(FILEINFO, "+++++++++++++GM request GT Delete Instance %lld++++++++++++++++",req.instanceid());
//		//删除GT服id
		ServerConHandler::GetInstance()->DeleteSceneID(req.instanceid());

		//给世界服发送删除副本id
		GameSvrToGate::DeleteMap meContent;
		WORD gsid =  message->GetRemoteID();

		meContent.set_id(gsid & 0xff);
		meContent.add_sceneid(req.instanceid());

		ServerConHandler::GetInstance()->SendMessageToServer(SimpleMessage, eWorldServer, MSG_SIM_GT2WS_DELETEGSMAP, &meContent);
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(InstanceMap, MSG_REQ_GM2GT_ENDINSTANCE)
{
	MapInstanceProto::ToGTAwardInfo toGT;
	MapInstanceProto::SimInstanceEnd toMemberAward;
	char *con = 0;
	int len = 0;
	int channel = -1;

	con = message->GetBuffer(len);
	toGT.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "send to Player Instance Award to GateServer");

	MapInstanceProto::SimInstanceEnd endinfo = toGT.awardinfo();
	toMemberAward.CopyFrom(endinfo);

	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(toGT.charid(), channel))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_GT2C_ENDINSTANCE, &toMemberAward, channel, SimpleMessage);
		Message_Facade::Send(clientRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "gateserver send to trader init exchange but find not client[ charid=%lld]",GET_PLAYER_CHARID(toGT.charid()));
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(InstanceMap, MSG_REQ_C2GT_CREATEINSTANCE)
{
	MapInstanceProto::ReqCreateInstance fromClient;
	MapInstanceProto::ReqWSCreateInstance reqWs;
	int len = 0;
	int gsChannel = -1;
	int64 charid = 0;
	Safe_Smart_Ptr<sSvrItem> gs;

	char * content = message->GetBuffer(len);
	fromClient.ParseFromArray(content, len);

	ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid);

	//获取人数最少的服来进行创建副本
	short int mapid = fromClient.instanceid();
	gs = ServerConHandler::GetInstance()->GetGameServerBymapID(mapid,gsChannel);
	if(!gs)
	{
		return;
	}

	reqWs.set_instanceid(fromClient.instanceid());
	reqWs.set_instancetype(fromClient.instancetype());
	reqWs.set_localid(ServerConHandler::GetInstance()->GetLocalID());
	reqWs.set_romateid(gs->remoteID);
	reqWs.set_charid(charid);

	Safe_Smart_Ptr<CommBaseOut::Message> worldmess  = build_message(MSG_REQ_GT2WS_CREATEINSTANCE, &reqWs, ServerConHandler::GetInstance()->GetWorldServer(), Request);
	worldmess->SetAct(new requestAct(message));
	Message_Facade::Send(worldmess);
}

DEF_MSG_ACK_DEFINE_FUN(InstanceMap, MSG_REQ_GT2WS_CREATEINSTANCE)
{
	//请求超时
	if (message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request worldserver create instance and ack timeout");

		return;
	}

	MapInstanceProto::RetCreateInstance toClient;
	int len = 0;

	char * content = message->GetBuffer(len);
	toClient.ParseFromArray(content, len);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_CREATEINSTANCE, static_cast<requestAct *>(act.Get())->mesReq, &toClient);
	Message_Facade::Send(messRet);
}
