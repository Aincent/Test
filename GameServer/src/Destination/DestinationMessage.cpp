/*
 * DestinationMessage.cpp
 *
 *  Created on: 2014年11月27日
 *      Author: root
 */
#include "DestinationMessage.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "Destination.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/ServerReturn3Int.pb.h"
#include "MessageStruct/DestinationProto.pb.h"
#include "../Object/Player/Player.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Map/GameZone.h"
#include "../Map/SceneManager.h"
#include "../MessageBuild.h"
#include "../Map/ConfigLoader/MapConfig.h"
#include "../MsgCommonClass.h"
#include "Smart_Ptr.h"
#include <netinet/in.h>
#include "MessageCommonRet.h"
#include <string.h>
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/CharLogin/Ranking.pb.h"
#include "../StatisticMgr/StatisticHandle.h"

DestinationMessage::DestinationMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(DestinationMessage,MSG_REQ_GETDESTINATIONTYPE);
	DEF_MSG_QUICK_REQUEST_REG_FUN(DestinationMessage,MSG_REQ_GETDESTINATIONPRIZE);
	DEF_MSG_QUICK_REQUEST_REG_FUN(DestinationMessage,MSG_REQ_GETHAVEREWARDDESTINATION);
}

DestinationMessage::~DestinationMessage()
{
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(DestinationMessage,MSG_REQ_GETDESTINATIONTYPE)
{
	DestinationProto::ReqLookForDestinationType req;
	DestinationProto::RetLookForDestinationType toClient;

	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eStGameObject))
	{
		Smart_Ptr<Destination> destination = player->GetDestination();
		if (destination)
		{
			std::vector<ServerReturn::ServerThreeInt> destinations;
			destination->GetStageDestinationInfo(req.type(),destinations);
			for(uint i = 0; i < destinations.size(); i++)
			{
				ServerReturn::ServerThreeInt* info = toClient.add_destinationinfo();
				info->CopyFrom(destinations[i]);
			}
		}
	}
	else
		return;

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_GETDESTINATIONTYPE);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_GETDESTINATIONTYPE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(DestinationMessage,MSG_REQ_GETHAVEREWARDDESTINATION)
{
	DestinationProto::RetGetHaveRewardDestination toClient;

	if(player && player->IsOpenSystem(eStGameObject))
	{
		Smart_Ptr<Destination> destination = player->GetDestination();
		if (destination)
		{
			std::vector<int> destinations;
			destination->GetHaveRewardDestination(destinations);
			for(uint i = 0; i < destinations.size(); i++)
			{
				toClient.add_destinationid(destinations[i]);
			}
		}
	}
	else
		return;

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_GETHAVEREWARDDESTINATION);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_GETHAVEREWARDDESTINATION);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(DestinationMessage,MSG_REQ_GETDESTINATIONPRIZE)
{
	ServerReturn::ServerRetInt req;
	ServerReturn::ServerRetInt toClient;

	req.ParseFromString(content);

	int64 errorCode = 0;
	int achiID = req.ret();
	if(player && player->IsOpenSystem(eStGameObject))
	{
		Smart_Ptr<Destination> destination = player->GetDestination();
		if (destination)
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eDestination, BehaviorType::Minor::eDestination_GetPassAward, MONITOR);

			errorCode = destination->DrawDestination(achiID);

			
		}
		if(0 == errorCode)
		{
			int64 achieveID = achiID;
			errorCode = (achieveID << 32);
		}
		toClient.set_ret(errorCode);
	}
	else
	{
		errorCode = eNotOpen;
		toClient.set_ret(eNotOpen);
	}

	LOG_DEBUG(FILEINFO,"receiveDestinationAck playerID %lld error %d",player->GetID(),errorCode);

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_GETDESTINATIONPRIZE);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_GETDESTINATIONPRIZE);
}
