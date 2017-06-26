/*
 * LucklyTurntableMessage.cpp
 *
 *  Created on: 2014年8月20日
 *      Author: root
 */


#include "LucklyTurntableMessage.h"
#include "Log/Logger.h"
#include "MessageCommonRet.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "LucklyTurntableMgr.h"
#include "../StatisticMgr/StatisticHandle.h"

LucklyTurntableMessage::LucklyTurntableMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(LucklyTurntableMessage, MSG_REG_C2GT_LUCKLYTURNTABLEINFO);

	DEF_MSG_QUICK_REQUEST_REG_FUN(LucklyTurntableMessage, MSG_REG_C2GT_REFULSH_LUCKLYTURNTABL_EREAW);
	DEF_MSG_QUICK_REQUEST_REG_FUN(LucklyTurntableMessage, MSG_REG_C2GT_ROLL_LUCKLYTURNTABL);
	DEF_MSG_QUICK_REQUEST_REG_FUN(LucklyTurntableMessage, MSG_REG_C2GT_GET_LUCKLYTURNTABL_EREAW);

}

LucklyTurntableMessage::~LucklyTurntableMessage()
{
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(LucklyTurntableMessage, MSG_REG_C2GT_LUCKLYTURNTABLEINFO)
{
	LucklyTurntable::AckLucklyTurntable ack;

	LOG_DEBUG(FILEINFO, "Player request LucklyTurntable info");

	if(player && player->IsOpenSystem(eStLucklyTurntable))
	{
		player->GetLucklyTableMgr()->EnterLucklyTurntable(ack);
	}
	else
	{
		ack.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REG_C2GT_LUCKLYTURNTABLEINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(LucklyTurntableMessage, MSG_REG_C2GT_REFULSH_LUCKLYTURNTABL_EREAW)
{
	LucklyTurntable::AckRefulshOrGetLucklyTurntable ack;

	LOG_DEBUG(FILEINFO, "Player request Refush LucklyTurntable info");

	if(player && player->IsOpenSystem(eStLucklyTurntable))
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eTurnTable, BehaviorType::Minor::eTurnTable_Refresh, MONITOR);;

		player->GetLucklyTableMgr()->RefulshLucklyTurntable( ack);

		
	}
	else
	{
		ack.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REG_C2GT_REFULSH_LUCKLYTURNTABL_EREAW);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(LucklyTurntableMessage, MSG_REG_C2GT_ROLL_LUCKLYTURNTABL)
{
	LucklyTurntable::ClientRollLucklyTurntable req;

	RewardHail::ToClientGetReward ack;

	req.ParseFromString(content);

	LOG_DEBUG(FILEINFO, "Player request Roll LucklyTurntable");

	if(player && player->IsOpenSystem(eStLucklyTurntable))
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eTurnTable, BehaviorType::Minor::eTurnTable_Refresh, MONITOR);;

		player->GetLucklyTableMgr()->RollLucklyTurntable(req.num(), ack);

		
	}
	else
	{
		ack.set_res(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REG_C2GT_ROLL_LUCKLYTURNTABL);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(LucklyTurntableMessage, MSG_REG_C2GT_GET_LUCKLYTURNTABL_EREAW)
{
	LucklyTurntable::AckGetLucklyTurntable ack;

	LOG_DEBUG(FILEINFO, "Player request Get LucklyTurntable TmpReaw");

	if(player && player->IsOpenSystem(eStLucklyTurntable))
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eTurnTable, BehaviorType::Minor::eTurnTable_GetAward, MONITOR);;

		player->GetLucklyTableMgr()->GetLucklyTurntable(ack);

		
	}
	else
	{
		ack.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REG_C2GT_GET_LUCKLYTURNTABL_EREAW);
}
