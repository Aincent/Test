/*
 * MSOpreationMessage.cpp
 *
 *  Created on: 2014年11月3日
 *      Author: root
 */
#include "MSOpreationMessage.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "../MessageBuild.h"
#include "CharDefine.h"
#include "../MsgCommonClass.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/MSProtoc/MS_OpreationSych.pb.h"
#include "SvrConfig.h"

MSopreationManager::MSopreationManager()
{
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_WS2GT_ABOUTTALKING);
}


MSopreationManager::~MSopreationManager()
{

}

void MSopreationManager::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GT_ABOUTTALKING);
	DEF_SWITCH_TRY_DISPATCH_END
}

void MSopreationManager::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_SWITCH_TRY_DISPATCH_END
}

void MSopreationManager::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_SIMPLE_DEFINE_FUN(MSopreationManager,MSG_SIM_WS2GT_ABOUTTALKING)
{
	LOG_DEBUG(FILEINFO,"ws Manager gt send DoSomthig ");
	MS_SychProto::SychGMThig req;
	int len = 0;
	int channel = -1;
	char * content = message->GetBuffer(len);
	int gsChannel = -1;

	req.ParseFromArray(content, len);
	int64  charid = req.charid();
	int type = req.data().type();

	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(charid, channel))
	{
		//通知客户端
		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_GT2C_ABOUTTALKING, &req, channel, SimpleMessage);
		Message_Facade::Send(clientRet);

		//通知 GM 更改相关数据
		if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(channel, gsChannel, charid))
		{
			if(type == e_dataManager_LockAccount)
			{
				//如果是  封帐号  //待测试 是否可行
				ServerReturn::ServerRetInt con;

				con.set_ret(eTypeLockAccount);

				LOG_ERROR(FILEINFO, "MS request gt exit and player[%lld] exit", GET_PLAYER_CHARID(req.charid()));
				Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_GT2C_PLAYER_EXIT, &con, channel, SimpleMessage);
				Message_Facade::Send(clientRet);
				LOG_WARNING(FILEINFO,"ServerConHandler::PlayerExitEx(const int64 &charid, bool flag, Safe_Smart_Ptr<Message> message).................................. %lld", GET_PLAYER_CHARID(charid));
				ServerConHandler::GetInstance()->PlayerExit(charid, 1);  //
//				ServerConHandler::GetInstance()->PlayerExit(charid, false, message);
			}
			else
			{
				Safe_Smart_Ptr<CommBaseOut::Message> clientGM  = build_message(MSG_SIM_GT2GM_ABOUTTALKING, &req, gsChannel, SimpleMessage);
				Message_Facade::Send(clientGM);
			}
		}
	}
	else
	{
		LOG_ERROR(FILEINFO, "sim chat but find not client[ charid=%lld]",req.charid());
	}
}





