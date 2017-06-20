/*
 * MailManager.cpp
 *
 *  Created on: 2014年6月4日
 *      Author: root
 */

#include "MailManager.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageStruct/CharLogin/Mail.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"

MailManager::MailManager()
{
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_WS2GT_SENDMAIL);
}


MailManager::~MailManager()
{

}

void MailManager::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GT_SENDMAIL);
	DEF_SWITCH_TRY_DISPATCH_END
}

void MailManager::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_SWITCH_TRY_DISPATCH_END
}

void MailManager::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_SIMPLE_DEFINE_FUN(MailManager,MSG_SIM_WS2GT_SENDMAIL)
{
	int channel = -1;
	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(message->GetMessageTime(), channel))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_GT2C_NEWMAIL, message, channel, SimpleMessage);
		Message_Facade::Send(clientRet);
	}

}
