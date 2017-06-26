/*
 * AntiAddictionMessage.cpp
 *
 *  Created on: 2014年10月28日
 *      Author: root
 */

#include "AntiAddictionMessage.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "../Object/Player/Player.h"
#include "../Object/Player/PlayerMessage.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "Smart_Ptr.h"
#include <netinet/in.h>
#include "MessageCommonRet.h"
#include <string.h>
#include "AntiAddictionSystemMgr.h"

AntiAddictionMessage::AntiAddictionMessage() 
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(AntiAddictionMessage,MSG_REQ_C2GM_BIND_ANTI_ADDICTION_ID);
}

AntiAddictionMessage::~AntiAddictionMessage() 
{
	// TODO Auto-generated destructor stub
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(AntiAddictionMessage,MSG_REQ_C2GM_BIND_ANTI_ADDICTION_ID)
{
	AntiAddictionProto::ReqBindAntiAddictionID req;
	AntiAddictionProto::RetBindAntiAddictionID toClient;
	ServerMessage::ClientSingleMessage ack;

	req.ParseFromString(content);

	EAndiAddictionState antiAddictionState = player->GetAntiAddictionSystemMgr()->GetAntiAddictionState();

	int error = eBindIDError;
	//认证过身份证就不能再认证了
	if(antiAddictionState == eAAS_NotBind)
	{
		error = player->GetAntiAddictionSystemMgr()->CanBind(req);
		if((error == 0 || error == eBindIDUnderAge))
		{
			if(error == 0)
				antiAddictionState = eAAS_BindAndAdult;
			else if(error == eBindIDUnderAge)
				antiAddictionState = eAAS_BindButMinor;
			error = 0;
			player->GetAntiAddictionSystemMgr()->SetAntiAddictionState(int(antiAddictionState));
			AntiAddictionProto::ToDBBindAntiAddictionID reqDB;
			reqDB.set_antiaddictionstate(int(antiAddictionState));
			reqDB.set_antiaddictionid(req.antiaddictionid());
			reqDB.set_realname(req.realname());
			SendToDBSaveBindAntiAddictionInfo(reqDB,player);
		}
	}

	toClient.set_error(error);
	toClient.set_state(int(antiAddictionState));
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_BIND_ANTI_ADDICTION_ID);
}

//void AntiAddictionMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
//{
//	DEF_SWITCH_TRY_DISPATCH_BEGIN
//
//	DEF_SWITCH_TRY_DISPATCH_END
//}
//
//void AntiAddictionMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
//{
//	DEF_SWITCH_TRY_DISPATCH_BEGIN
//
//	DEF_SWITCH_TRY_DISPATCH_END
//}

void AntiAddictionMessage::SendToDBSaveBindAntiAddictionInfo(AntiAddictionProto::ToDBBindAntiAddictionID& reqDB,Smart_Ptr<Player> & player)
{
	//发送给DB保存身份证绑定信息，姓名，身份证ID，认证状态
	int64 charid = player->GetID();
	reqDB.set_charid(charid);
	Smart_Ptr<SvrItem> dbSvr;
	ServerConHandler::GetInstance()->GetDBServerBySvrID(GET_SERVER_CHARID(charid), dbSvr);
	if(dbSvr.Get()==NULL)
		return;
	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2DB_BIND_ANTI_ADDICTION_ID, &reqDB, dbSvr.Get(),SimpleMessage);
	Message_Facade::Send(messRet);
}

//void AntiAddictionMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
//{
//	DEF_SWITCH_TRY_DISPATCH_BEGIN
//
//
//	DEF_SWITCH_TRY_DISPATCH_END
//}

