/*
 * EscortMessage.cpp
 *
 *  Created on: 2014年7月21日
 *      Author: root
 */

#include "EscortMessage.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "EscortMgr.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "../Object/Player/Player.h"
#include "../MessageBuild.h"
#include "MessageCommonRet.h"
#include "CounterDefine.h"
#include "../ServerEventDefine.h"
#include "../EventSet/EventDefine.h"
#include "../Object/Player/PlayerMessage.h"
#include "../StatisticMgr/StatisticHandle.h"

EscortMessage::EscortMessage()
{
	//领取皇纲品质
	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTGETQUALITY);
	//刷新皇纲
	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTFRESH);
	//开始运镖
	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTSTART);
	//立即完成
	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTIMEDCOMPLETE);
	//加速
	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTSPEEDUP);
	//放弃
	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTABANDON);
	//领取奖励
	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTGETRPIZE);
	//获取镖车信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(EscortMessage,MSG_REQ_C2GM_OPENESCORT);
	// WS通知GM双倍押镖活动状态
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_DOUBLEESCORTBEGIN);
}

EscortMessage::~EscortMessage()
{

}

void EscortMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_DOUBLEESCORTBEGIN);
	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTGETQUALITY)
{
	EscortProto::RqGetQuality req;
	EscortProto::RtGetQuality toClient;
	int error = eEscortQualityError;

	req.ParseFromString(content);

	if ((req.quality() >= eEscortDiamond) && (req.quality() < (int)(eEscortMax)))
	{
		Smart_Ptr<EscortMgr> escort = player->GetEscortMgr();
		if (escort)
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eEscort, BehaviorType::Minor::eEscort_SelQuality, MONITOR);;

			error = escort->SetEscortQuality((eCarType)(req.quality()));

			
		}
		else
		{
			LOG_ERROR(FILEINFO, "player[%lld] escort manager is error", GET_PLAYER_CHARID(player->GetID()));

			return;
		}
	}

	toClient.set_error(error);

	if (error == 0)
		toClient.set_quality(req.quality());

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ESCORTGETQUALITY);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTFRESH)
{
	EscortProto::RqFreshTreature req;
	EscortProto::RtFreshTreature toClient;
	DWORD quality = 0;
	int error = 0;

	req.ParseFromString(content);

	Smart_Ptr<EscortMgr> escort = player->GetEscortMgr();
	if (escort)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eEscort, BehaviorType::Minor::eEscort_Refresh, MONITOR);

		quality = escort->GetQuality();
		error = escort->GenenateQuality(req.escortid(),quality);

		
	}
	else
	{
		LOG_ERROR(FILEINFO, "player[%lld] escort manager is error",GET_PLAYER_CHARID(player->GetID()));

		return;
	}

	toClient.set_quality(quality);
	toClient.set_error(error);

	int freshtimes = player->GetCounterService()->GetNumById(ESCORT_FREETIMES_COUNTER);
	toClient.set_freshtimes(freshtimes);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ESCORTFRESH);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTSTART)
{
	EscortProto::RqStartEscort req;
	EscortProto::RtStartEscort toClient;
	int error = 0;

	req.ParseFromString(content);

	if (player && player->IsOpenSystem(eStEscort))
	{
		Smart_Ptr<EscortMgr> escort = player->GetEscortMgr();
		if (escort)
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eEscort, BehaviorType::Minor::eEscort_StartEscort, MONITOR);

			error = escort->StartEscort(true);

			
		}
		else
		{
			LOG_ERROR(FILEINFO, "player[%lld] escort manager is error", GET_PLAYER_CHARID(player->GetID()));

			return;
		}

		if (!error && escort)
		{
			toClient.set_line(escort->GetLine());
			toClient.set_quality(escort->GetQuality());
			toClient.set_money(escort->getRewardMoney());
			toClient.set_exp(escort->getRewardExp());
		}
	}
	else
	{
		error = eNotOpen;
	}

	toClient.set_error(error);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ESCORTSTART);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTIMEDCOMPLETE)
{
	EscortProto::RqImediaComplete req;
	EscortProto::EscortResult toClient;
	int error = 0;
	int exp = 0;
	int money = 0;

	req.ParseFromString(content);

	Smart_Ptr<EscortMgr> escort = player->GetEscortMgr();
	int quality = 0;
	if (escort)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eEscort, BehaviorType::Minor::eEscort_ImmedComplete, MONITOR);

		quality = (int)(escort->GetQuality());
		error = escort->ImediaComplete(exp,money);

		
	}
	else
	{
		LOG_ERROR(FILEINFO, "player[%lld] escort manager is error", GET_PLAYER_CHARID(player->GetID()));

		return;
	}

	toClient.set_res(error);
	if (error == 0)
	{
		if (player->GetMyself())
		{
			GenenateEscortArgs e(player->GetMyself(),quality);
			player->FireEvent(PLAYER_GENENATE_ESCORT,e);
		}
		toClient.set_exp(exp);
		toClient.set_money(money);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ESCORTIMEDCOMPLETE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTSPEEDUP)
{
	EscortProto::RqSpeedUp req;
	EscortProto::RtSpeedUp toClient;
	int error = 0;

	req.ParseFromString(content);

	Smart_Ptr<EscortMgr> escort = player->GetEscortMgr();
	if (escort)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eEscort, BehaviorType::Minor::eEscort_Speedup, MONITOR);

		error = escort->SpeedUpEscort();

		
	}
	else
	{
		LOG_ERROR(FILEINFO, "player[%lld] escort manager is error", GET_PLAYER_CHARID(player->GetID()));

		return;
	}

	toClient.set_error(error);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ESCORTSPEEDUP);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTABANDON)
{
	EscortProto::RqAbandon req;
	EscortProto::EscortResult toClient;
	int error = 0;

	req.ParseFromString(content);

	Smart_Ptr<EscortMgr> escort = player->GetEscortMgr();
	if (escort)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eEscort, BehaviorType::Minor::eEscort_Abandon, MONITOR);

		error = escort->AbandonEscort();

		
	}
	else
	{
		LOG_ERROR(FILEINFO, "player[%lld] escort manager is error", GET_PLAYER_CHARID(player->GetID()));

		return;
	}

	toClient.set_res(error);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ESCORTABANDON);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_ESCORTGETRPIZE)
{
//	EscortProto::RqGetPrize req;
//	EscortProto::RtGetPrize toClient;
//	ServerMessage::ClientSingleMessage ack;
//
//	req.ParseFromString(content);
//
//	sEscortMgr->EndEscort(player);
//
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_ESCORTGETRPIZE);

}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EscortMessage,MSG_REQ_C2GM_OPENESCORT)
{
	EscortProto::RqOpenEscort req;
	EscortProto::RtOpenEscort toClient;

	req.ParseFromString(content);

	if (player->GetCounterService())
	{
		int freshtimes = player->GetCounterService()->GetNumById(ESCORT_FREETIMES_COUNTER);
		int dotimes = player->GetCounterService()->GetNumById(ESCORT_DAY_COUNTER);

		toClient.set_freshtimes(freshtimes);
		toClient.set_dotimes(dotimes);
	}
	else
	{
		return;
	}

	Smart_Ptr<EscortMgr> escort = player->GetEscortMgr();
	if (escort)
	{
		toClient.set_quality(escort->GetQuality());
	}
	else
	{
		LOG_ERROR(FILEINFO, "player[%lld] escort manager is error", GET_PLAYER_CHARID(player->GetID()));

		return;
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_OPENESCORT);
}

DEF_MSG_SIMPLE_DEFINE_FUN(EscortMessage, MSG_SIM_WS2GM_DOUBLEESCORTBEGIN)
{
	EscortProto::WSToGMDoubleEscort req;
	int len = 0;
	char* buffer = message->GetBuffer(len);

	req.ParseFromArray(buffer, len);
	const INT64PLAYERMAP& players = PlayerMessage::GetInstance()->getAllPlayer();
	for (INT64PLAYERMAP::const_iterator it = players.begin(); it != players.end(); ++it)
	{
		if ((bool)it->second && it->second->IsOpenSystem(eStEscort))
		{
			it->second->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Activity, eUIHLSmall_Activity_DoubleEscortOpen), 1, req.flag());
		}
	}
}
