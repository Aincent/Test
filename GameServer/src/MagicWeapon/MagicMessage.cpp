/*
 * MagicMessage.cpp
 *
 *  Created on: 2014年8月8日
 *      Author: helh
 */

#include "MagicMessage.h"
#include "MessageStruct/CharLogin/MagicWeapon.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MagicMgr.h"
#include "../MessageBuild.h"
#include "../ServerEventDefine.h"
#include "../StatisticMgr/StatisticHandle.h"

MagicMessage::MagicMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(MagicMessage, MSG_REQ_C2GM_ADVANCE_MAGIC);
	DEF_MSG_QUICK_REQUEST_REG_FUN(MagicMessage, MSG_REQ_C2GM_FLUSH_MAGIC_SKILL);
	DEF_MSG_QUICK_REQUEST_REG_FUN(MagicMessage, MSG_SIM_C2GM_REPLACE_MAGIC_SKILL);
}

MagicMessage::~MagicMessage()
{

}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MagicMessage,MSG_REQ_C2GM_ADVANCE_MAGIC)
{
	MagicWeapon::RetAdvanceMagic toClient;
	MagicWeapon::RequestAdvanceMagic req;

	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eStMagicWeapon))
	{
		eMagicAdvanceType tempType = (eMagicAdvanceType)req.type();
		if(tempType == eMagicAdvanceAuto && player->GetVipPart() &&  player->GetVipPart()->GetValueByRight(eVipAutoAdvanceMagic) == VIP_NOT_HAVE_THIS_RIGHT)
		{
			toClient.set_error(eNotVipRight);
		}
		else
		{
			player->GetMagicWeapon()->AdvanceMagicWeapon(&toClient, req);
		}
	}
	else
		return;

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ADVANCE_MAGIC);

	if(toClient.id() > 0)
	{
		OneParmaEventArgs e(player);
		player->FireEvent(PLAYER_WEAPON_MAGICWEAPON,e);
	}
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MagicMessage,MSG_REQ_C2GM_FLUSH_MAGIC_SKILL)
{
	ServerMessage::ClientSingleMessage ack;
	ServerReturn::ServerRetInt toClient;
	ServerReturn::ServerRetInt req;

	req.ParseFromString(content);


	if(player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eMagicWeapon, BehaviorType::Minor::eMagicWeapon_FlushSkill, MONITOR);

		if (player->IsOpenSystem(eStMagicWeapon))
		{
			toClient.set_ret(player->GetMagicWeapon()->FlushSkill(req.ret()));
		}

		
	}
	else
		toClient.set_ret(eNotOpen);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_FLUSH_MAGIC_SKILL);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MagicMessage, MSG_SIM_C2GM_REPLACE_MAGIC_SKILL)
{
	ServerReturn::ServerRetInt toClient;
	ServerReturn::ServerRetInt req;

	req.ParseFromString(content);

	int tempError = 0;
	if(player && player->IsOpenSystem(eStMagicWeapon))
	{
		tempError = player->GetMagicWeapon()->ReplaceSkill(req.ret());
		toClient.set_ret(tempError);
	}
	else
	{
		tempError = eNotOpen;
		toClient.set_ret(eNotOpen);
	}

	int tempIndex = req.ret();

	LOG_DEBUG(FILEINFO,"Player %lld magic ack error %d index %d",player->GetID(),tempError,tempIndex);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_SIM_C2GM_REPLACE_MAGIC_SKILL);
}


