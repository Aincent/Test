/*
 * MoneyShopMessage.cpp
 *
 *  Created on: 2014年8月9日
 *      Author: root
 */

#include "MoneyShopMessage.h"
#include "Log/Logger.h"
#include "MessageCommonRet.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "../ServerEventDefine.h"
#include "MoneyShopMgr.h"
#include "../StatisticMgr/StatisticHandle.h"

MoneyShopMessage::MoneyShopMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(MoneyShopMessage, MSG_REG_C2GT_MONSYSHOPINFO);
	DEF_MSG_QUICK_REQUEST_REG_FUN(MoneyShopMessage, MSG_REG_C2GT_EXCHANGEMONEY);
}

MoneyShopMessage::~MoneyShopMessage()
{

}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MoneyShopMessage, MSG_REG_C2GT_MONSYSHOPINFO)
{
	MoneyShopInfo::AckMoneyShopInfo ack;
//	ServerMessage::ClientSingleMessage toClient;

	LOG_DEBUG(FILEINFO, "Player request MoneyShopInfo info");

	if(player && player->IsOpenSystem(eStBank))
	{
		MoneyShopMgr::GetInstance()->GetMoneyShopInfo(player, ack);
	}
	else
	{
		ack.set_ret(eNotOpen);
	}

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&ack, MSG_REG_C2GT_MONSYSHOPINFO);
	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REG_C2GT_MONSYSHOPINFO);
//	string tStr = ack.SerializePartialAsString();
//	toClient.set_content(tStr);
//	toClient.set_id(MSG_REG_C2GT_MONSYSHOPINFO);
//
//	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_QUICKMESSAGE, message, &toClient);
//	Message_Facade::Send(messRet);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MoneyShopMessage, MSG_REG_C2GT_EXCHANGEMONEY)
{
	MoneyShopInfo::AckExchangeMoney ack;
//	ServerMessage::ClientSingleMessage toClient;

	if(player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eMoneyShop, BehaviorType::Minor::eMoneyShop_GetMoney, MONITOR);
		if (player->IsOpenSystem(eStBank))
		{
				MoneyShopMgr::GetInstance()->ExchangeMoney( player, &ack);
		}
		else
		{
			ack.set_ret(eNotOpen);
		}

		
		REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REG_C2GT_EXCHANGEMONEY);
	}
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&ack, MSG_REG_C2GT_EXCHANGEMONEY);

//	string tStr = ack.SerializePartialAsString();
//	toClient.set_content(tStr);
//	toClient.set_id(MSG_REG_C2GT_EXCHANGEMONEY);
//
//	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_QUICKMESSAGE, message, &toClient);
//	Message_Facade::Send(messRet);
}
