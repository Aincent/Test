/*
 * QuickMessage.cpp
 *
 *  Created on: 2014年4月22日
 *      Author: helh
 */

#include "QuickMessage.h"
#include "Network/Message_Facade.h"
#include "ServerManager.h"
#include "Network/MessageManager.h"
#include "Log/Logger.h"
#include "MessageBuild.h"
#include "./Object/Player/PlayerMessage.h"

QuickMessage::QuickMessage()
{
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_GT2GM_QUICKMESSAGE);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_QUICKMESSAGE);
//	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GT2GM_QUICKUPDATEQUEST);
}

QuickMessage::~QuickMessage()
{

}

void QuickMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GT2GM_QUICKMESSAGE);

	DEF_SWITCH_TRY_DISPATCH_END
}

void QuickMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_QUICKMESSAGE);

	DEF_SWITCH_TRY_DISPATCH_END
}

void QuickMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

//	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_ACCEPTQUEST);

	DEF_SWITCH_TRY_DISPATCH_END
}

void QuickMessage::AckMessage(Safe_Smart_Ptr<CommBaseOut::Message> &message, google::protobuf::Message *content, int msgID)
{
	ServerMessage::ClientSingleMessage singleMes;
	string tempStr;

	singleMes.set_id(msgID);
	content->SerializePartialToString(&tempStr);
	singleMes.set_content(tempStr.c_str(), tempStr.size());

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(message, &singleMes, Ack);
	Message_Facade::Send(messRet);
}

void QuickMessage::AddSendMessage(int64 charid, int channel, google::protobuf::Message *content, int msgID)
{
	map<pair<int64, int>, vector<ServerMessage::ClientMessagePackage> >::iterator it = m_allMessage.find(pair<int64, int>(charid, channel));
	if(it == m_allMessage.end())
	{
		vector<ServerMessage::ClientMessagePackage> vec;
		string tempStr;

		ServerMessage::ClientMessagePackage clientPack;
		ServerMessage::ClientSingleMessage * singleMes = clientPack.add_content();

		singleMes->set_id(msgID);
		content->SerializePartialToString(&tempStr);
		singleMes->set_content(tempStr.c_str(), tempStr.size());
		vec.push_back(clientPack);

		m_allMessage[pair<int64, int>(charid, channel)] = vec;
	}
	else
	{
		if((it->second[it->second.size() - 1].ByteSize() + content->ByteSize()) > 32*1024)
		{
			string tempStr;

			ServerMessage::ClientMessagePackage clientPack;
			ServerMessage::ClientSingleMessage * singleMes = clientPack.add_content();

			singleMes->set_id(msgID);
			content->SerializePartialToString(&tempStr);
			singleMes->set_content(tempStr.c_str(), tempStr.size());

			it->second.push_back(clientPack);
		}
		else
		{
			string tempStr;
			ServerMessage::ClientSingleMessage * singleMes = it->second[it->second.size() - 1].add_content();
			singleMes->set_id(msgID);
			content->SerializePartialToString(&tempStr);
			singleMes->set_content(tempStr.c_str(), tempStr.size());
		}
	}
}

void QuickMessage::UpdateMessage()
{
	map<pair<int64, int>, vector<ServerMessage::ClientMessagePackage> >::iterator it = m_allMessage.begin();
	for(; it!=m_allMessage.end(); ++it)
	{
		vector<ServerMessage::ClientMessagePackage>::iterator itVec = it->second.begin();
		for(; itVec!=it->second.end(); ++itVec)
		{
			Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_SIM_GM2GT_QUICKMESSAGE, &(*itVec), it->first.second, SimpleMessage);
			messRet->SetMessageTime(it->first.first);
			Message_Facade::Send(messRet);
		}
	}

	m_allMessage.clear();
}


DEF_MSG_SIMPLE_DEFINE_FUN(QuickMessage, MSG_SIM_GT2GM_QUICKMESSAGE)
{
	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharid(message->GetMessageTime(), player);
	if(!player)
	{
		LOG_WARNING(FILEINFO, "receive quick message but player[%lld] is not existed", message->GetMessageTime());
		return;
	}


	ServerMessage::ClientSingleMessage clientMsg;
	int len = 0;
	char *content = message->GetBuffer(len);
	clientMsg.ParseFromArray(content, len);

	string conStr = clientMsg.content();
	map<int, MessageBoundSlot>::iterator it = m_messageSlot.find(clientMsg.id());
	if(it == m_messageSlot.end())
	{
		LOG_WARNING(FILEINFO, "receive quick message but message[%d] is not registered", clientMsg.id());
		return;
	}

	try
	{
		(*it->second.obj)(message, conStr, player);
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "quick message dispatch message to other module and error[%s]", e.what());

		return;
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "quick message dispatch message to other module and unknown error");

		return;
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(QuickMessage, MSG_REQ_GT2GM_QUICKMESSAGE)
{
	int len = 0;
	char *str = message->GetBuffer(len);
	Smart_Ptr<Player> player;


	PlayerMessage::GetInstance()->GetPlayerByCharid(message->GetMessageTime(), player);
	if(!player)
	{
		LOG_WARNING(FILEINFO, "receive quick requested message but player[%lld] is not existed", message->GetMessageTime());
		return;
	}

	ServerMessage::ClientSingleMessage clientMsg;
	clientMsg.ParseFromArray(str, len);
	string conStr = clientMsg.content();

	map<int, MessageBoundSlot>::iterator it = m_messageSlot.find(clientMsg.id());
	if(it == m_messageSlot.end())
	{
		LOG_WARNING(FILEINFO, "receive quick requested message but message[%d] is not registered", clientMsg.id());
		return;
	}

	try
	{
		(*it->second.obj)(message, conStr, player);
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "quick requested message[%d] dispatch message to other module and error[%s]", clientMsg.id(), e.what());

		return;
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "quick requested message[%d] dispatch message to other module and unknown error", clientMsg.id());

		return;
	}
}


