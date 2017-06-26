/*
 * .cpp
 *
 *  Created on: 2014年8月11日
 *      Author: root
 */


#include "VIPMessage.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "MessageStruct/ServerReturnInt.pb.h"
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
#include "MessageStruct/CharLogin/PlayerVipProto.pb.h"
#include "CounterDefine.h"
#include "../StatisticMgr/StatisticHandle.h"

VIPMessage::VIPMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(VIPMessage,MSG_REQ_C2GM_GETVIPGIFT);		//领取VIP奖励
	DEF_MSG_QUICK_REQUEST_REG_FUN(VIPMessage,MSG_REQ_C2GM_GETGIFTINFO);		//VIP礼物领取状态
	DEF_MSG_QUICK_REQUEST_REG_FUN(VIPMessage,MSG_REQ_C2GM_PLAYERRECHARGEINFO);		//领取VIP奖励
	DEF_MSG_QUICK_REQUEST_REG_FUN(VIPMessage,MSG_REQ_C2GM_GETRECHARGEGIFT);		//VIP礼物领取状态
}

VIPMessage::~VIPMessage()
{

}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(VIPMessage,MSG_REQ_C2GM_GETVIPGIFT)
{
	PlayerVipProto::ReqGetVipGift req;
	PlayerVipProto::RetGetVipGift toClient;
	ServerMessage::ClientSingleMessage ack;

	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eStVip))
	{
		int error = 0;
		DWORD info = 0;			//1~11bit为1表示已领取
		Smart_Ptr<PlayerVipPart> vipPart = player->GetVipPart();
		if(vipPart)
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eVIP, BehaviorType::Minor::eVIP_GetAward, MONITOR);

			error = vipPart->GetVipGift(req.giftid(),toClient);
			info = vipPart->GetGiftInfo();

			
		}
		else
		{
			error = eNull;
		}

		toClient.set_error(error);
		toClient.set_giftinfo(info);
	}
	else
	{
		toClient.set_error(eNotOpen);
	}

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_GETVIPGIFT);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_GETVIPGIFT);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(VIPMessage,MSG_REQ_C2GM_GETGIFTINFO)
{
	PlayerVipProto::ReqGotGiftInfo req;
	PlayerVipProto::RetGotGiftInfo toClient;
	ServerMessage::ClientSingleMessage ack;

	req.ParseFromString(content);


	if(player && player->IsOpenSystem(eStVip))
	{
		DWORD info = 0;			//1~11bit为1表示已领取
		Smart_Ptr<PlayerVipPart> vipPart = player->GetVipPart();
		if(vipPart)
		{
			info = vipPart->GetGiftInfo();
		}

		toClient.set_giftinfo(info);
	}
	else
		return;

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_GETGIFTINFO);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_GETGIFTINFO);
}
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(VIPMessage, MSG_REQ_C2GM_PLAYERRECHARGEINFO)
{
	PlayerVipProto::ToClientRechargeInfo ret;

	if(player && player->IsOpenSystem(eStVip))
	{
		player->GetVipPart()->GetRecharge(ret);
	}
	else
	{
		return;
	}

	REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GM_PLAYERRECHARGEINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(VIPMessage, MSG_REQ_C2GM_GETRECHARGEGIFT)
{

	ServerReturn::ServerResultInt req;
	ServerReturn::ServerResultInt ret;

	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eStVip))
	{
		ret.set_ret(player->GetVipPart()->GetRechargeAward(req.ret()));
	}
	else
	{
		ret.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GM_GETRECHARGEGIFT);
}
