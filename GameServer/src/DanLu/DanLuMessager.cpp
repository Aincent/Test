/*
 * DanLuMessager.cpp
 *
 *  Created on: 2014年8月7日
 *      Author: root
 */

#include "DanLuMessager.h"
#include "Log/Logger.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "MessageCommonRet.h"
#include "MessageStruct/CharLogin/DanLu.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"

#include "../ServerEventDefine.h"
#include "EventSet/EventSet.h"
#include "EventSet/EventArgs.h"

#include "DanLuMgr.h"
#include "../StatisticMgr/StatisticHandle.h"

DanLuMessage::DanLuMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(DanLuMessage, MSG_REQ_C2GT_DANLUINFO);
	DEF_MSG_QUICK_REQUEST_REG_FUN(DanLuMessage, MSG_REG_C2GT_EATDANLU);

	DEF_MSG_QUICK_REQUEST_REG_FUN(DanLuMessage, MSG_REG_C2GT_DANLUEQUIP);
	DEF_MSG_QUICK_REQUEST_REG_FUN(DanLuMessage, MSG_REG_C2GT_GETDANLUEQUIP);

}

DanLuMessage::~DanLuMessage()
{

}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(DanLuMessage, MSG_REG_C2GT_DANLUEQUIP)
{
	DanLuInfo::ClientDanLuEquip req;
	DanLuInfo::RetClientDanLuEquip ack;

	req.ParseFromString(content);

	LOG_DEBUG(FILEINFO, "Player request Danlu info");

	BEHAVIOR_MONI(player, BehaviorType::Main::eDanLu, BehaviorType::Minor::eDanLu_Refine, MONITOR);

	player->GetDanLuManager()->DanLuEquip(&ack, req);

	

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REG_C2GT_DANLUEQUIP);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(DanLuMessage, MSG_REG_C2GT_GETDANLUEQUIP)
{
	DanLuInfo::RetDanluEquip acks;

	LOG_DEBUG(FILEINFO, "Player request Danlu info");

	int itemID = 0, itemNum = 0;
	int err = player->GetDanLuManager()->GetDanluEquip(itemID, itemNum);
	if (-1 == err)
	{
		return;
	}

	acks.set_ret(err);
	acks.set_goods((int64)itemID<<32 | itemNum);
	acks.set_number(player->GetDanLuManager()->GetProduceItemSize());

	REQ_MSG_QUICK_ACK_FUN(message, &acks, MSG_REG_C2GT_GETDANLUEQUIP);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(DanLuMessage, MSG_REQ_C2GT_DANLUINFO)
{
	DanLuInfo::AckPlayerDanLuInfo ack;

	LOG_DEBUG(FILEINFO, "Player request Danlu info");

	if (player && player->IsOpenSystem(eStDanLu))
	{
		player->GetDanLuManager()->GetDanLuInfo(ack);
	}
	else
	{
		ack.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GT_DANLUINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(DanLuMessage, MSG_REG_C2GT_EATDANLU)
{
	DanLuInfo::ClientDanLuAddExp req;
	DanLuInfo::AckPlayerDanLuInfo ack;

	req.ParseFromString(content);

	LOG_DEBUG(FILEINFO, "Player request Danlu info");


	if (player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eDanLu, BehaviorType::Minor::eDanLu_Levelup, MONITOR);
		if (player->IsOpenSystem(eStDanLu))
		{
			eDanLudvanceType tempType = (eDanLudvanceType)req.type();
			if(tempType == eDanLuAdvanceAuto && player->GetVipPart() &&  player->GetVipPart()->GetValueByRight(eVipAutoSmeltingDanLu) == VIP_NOT_HAVE_THIS_RIGHT)
			{
				ack.set_ret(eNotVipRight);
			}
			else
			{
				player->GetDanLuManager()->EatDanLu(&ack, req);

				if (ack.ret() == 0)
				{
					OneParmaEventArgs e(player);
					player->FireEvent(PLAYER_MERIDIAN_UPDATE,e);
				}
			}
		}
		else
		{
			ack.set_ret(eNotOpen);
		}
	}

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REG_C2GT_EATDANLU);
}



