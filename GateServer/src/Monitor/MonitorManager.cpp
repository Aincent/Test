/*
 * MonitorManager.cpp
 *
 *  Created on: 2014年9月3日
 *      Author: root
 */

#include "MonitorManager.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageStruct/CharLogin/Mail.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"


MonitorManager::MonitorManager()
{
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REG_C2GT_MAPINFO);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REG_C2GT_MAPPLAYERINFO);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_GETALLGAMESVRINFO); // 获取所有分线信息
}


MonitorManager::~MonitorManager()
{

}

void MonitorManager::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_SWITCH_TRY_DISPATCH_END
}

void MonitorManager::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REG_C2GT_MAPINFO);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REG_C2GT_MAPPLAYERINFO);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_GETALLGAMESVRINFO);
	DEF_SWITCH_TRY_DISPATCH_END
}

void MonitorManager::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_REQUEST_DEFINE_FUN(MonitorManager,MSG_REG_C2GT_MAPINFO)
{
	MonitorMap::mapInfo info;
	ServerConHandler::GetInstance()->GetAllMapInfo(info);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REG_C2GT_MAPINFO, message, &info);
	Message_Facade::Send(messRet);
}

DEF_MSG_REQUEST_DEFINE_FUN(MonitorManager,MSG_REG_C2GT_MAPPLAYERINFO)
{
	ServerReturn::ServerRetInt req;
	MonitorMap::mapPlayerInfo ret;
	int len = 0;

	char * content = message->GetBuffer(len);
	req.ParseFromArray(content, len);

	ServerConHandler::GetInstance()->GetAllPlayerInfoByMapId(req.ret(),ret);
	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REG_C2GT_MAPPLAYERINFO, message, &ret);
	Message_Facade::Send(messRet);
}

// 获取所有分线信息
DEF_MSG_REQUEST_DEFINE_FUN(MonitorManager, MSG_REQ_C2GT_GETALLGAMESVRINFO)
{
	PlayerInfo::TotalBranchLineInfo toClient;

	ServerConHandler::GetInstance()->GetAllGameSvrInfo(toClient);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_GETALLGAMESVRINFO, message, &toClient);
	Message_Facade::Send(messRet);
}
