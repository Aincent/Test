/*
 * HorseMessage.cpp
 *
 *  Created on: 2014年4月30日
 *      Author: helh
 */

#include "HorseMessage.h"
#include "Log/Logger.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "MessageCommonRet.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Object/Player/Player.h"
#include "MessageStruct/CharLogin/Horse.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "HorseManager.h"
#include "../Daily/DailyMgr.h"
#include "../ServerEventDefine.h"
#include "EventSet/EventSet.h"
#include "EventSet/EventArgs.h"
#include "../StatisticMgr/StatisticHandle.h"

enum HandleHorseEnum
{
	e_CommHandle	=0,
	e_DirectHandle,
};

HorseMessage::HorseMessage()
{
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_HORSEINFO);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_HANDLEHORSE);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_HORSEADD);

	DEF_MSG_QUICK_REQUEST_REG_FUN(HorseMessage, MSG_C2GM_BUYHORSESKIN);

	DEF_MSG_QUICK_SIMPLE_REG_FUN(HorseMessage, MSG_SIM_C2GM_ALTERHORSESKINUIHL);

}

HorseMessage::~HorseMessage()
{

}


void HorseMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_SWITCH_TRY_DISPATCH_END
}

void HorseMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_HORSEINFO);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_HANDLEHORSE);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_HORSEADD);

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_REQUEST_DEFINE_FUN(HorseMessage, MSG_REQ_GT2GM_HORSEINFO)
{
	PlayerInfo::HorseInfo ack;
	Smart_Ptr<Player> player;

	LOG_DEBUG(FILEINFO, "Player request horse info");

	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if ((bool)player)
	{
		if (player->IsOpenSystem(eStHorse))
		{
			player->GetHorseManager()->SetHorseToDB(&ack);
		}
		else
			return;

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_HORSEINFO, message, &ack);
		Message_Facade::Send(messRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "player request horse info but player is not logined");
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(HorseMessage, MSG_REQ_GT2GM_HANDLEHORSE)
{
	HorseInfo::ControlHorse meContent;
	ServerReturn::ServerRetInt ack;
	char *con;
	int len = 0;
	int64 ret = 0;
	Smart_Ptr<Player> player;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "Player request handling horse");

	PlayerMessage::GetInstance()->GetPlayerByCharid(message->GetMessageTime(), player);
	if ((bool)player)
	{
		if (player->IsOpenSystem(eStHorse))
		{
			ret = player->GetHorseManager()->NewHandleHorse(meContent.id(), meContent.type());
		}
		else
		{
			ret = eNotOpen;
		}

		int64 horseId =  meContent.id();
		ret = horseId<<32 | ret;

		ack.set_ret(ret);

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_HANDLEHORSE, message, &ack);
		Message_Facade::Send(messRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "player request horse info but player is not logined");
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(HorseMessage, MSG_REQ_GT2GM_HORSEADD)
{
	HorseInfo::HorseAdvance meContent;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "Player request handling horse");

	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if ((bool)player)
	{
		if (!player->IsOpenSystem(eStHorse))
		{
			return;
		}

		BEHAVIOR_MONI(player, BehaviorType::Main::eHorse, BehaviorType::Minor::eHorse_Levelup, MONITOR);
		HorseInfo::AddExpReturn ret ;

		player->GetHorseManager()->EatHorse(meContent,&ret);


		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_HORSEADD, message, &ret);
		Message_Facade::Send(messRet);

		if (ret.ret() == 0)
		{
			OneParmaEventArgs e(player);
			player->FireEvent(PLAYER_HORSE_UPDATE,e);
		}

		

	}
	else
	{
		LOG_ERROR(FILEINFO, "player request horse info but player is not logined");
	}
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(HorseMessage, MSG_C2GM_BUYHORSESKIN)
{
	ServerReturn::ServerRetInt req;
	req.ParseFromString(content);

	ServerReturn::ServerDoubleInt ack;

	LOG_DEBUG(FILEINFO, "Player request Danlu info");

	if (player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eHorse, BehaviorType::Minor::eHorse_BuySkin, MONITOR);

		if (player->IsOpenSystem(eStHorse))
		{
			int64 ret = player->GetHorseManager()->BuyHorseSkin(req.ret());
			int64 skinId = req.ret();
			ret = skinId<<32 | ret;
			ack.set_retf(ret);

			hSkinInfo skinInfo;
			player->GetHorseManager()->GetHorseSkinInfo(skinId,skinInfo);
			ack.set_rets(skinInfo.endTime);
		}
		else
		{
			ack.set_retf(eNotOpen);
			ack.set_rets(0);
		}

		
	}

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_C2GM_BUYHORSESKIN);
}

DEF_MSG_QUICK_SIMPLE_DEFINE_FUN(HorseMessage, MSG_SIM_C2GM_ALTERHORSESKINUIHL)
{
	if (!player)
	{
		return;
	}
	player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Horse, eUIHLSmall_Horse_GetNewSkin), 0);

}



