/*
 * VitalityMessage.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: root
 */

#include "VitalityMessage.h"
#include "VitalityMgr.h"
#include "../StatisticMgr/StatisticHandle.h"

VitalityMessage::VitalityMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(VitalityMessage, MSG_REQ_C2GM_GETVITALITYINFO);
	DEF_MSG_QUICK_REQUEST_REG_FUN(VitalityMessage, MSG_REQ_C2GM_GETVITALITYGIFT);
}

VitalityMessage::~VitalityMessage()
{
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(VitalityMessage, MSG_REQ_C2GM_GETVITALITYINFO)
{
	VitalityProto::ProtoVitalityInfo ack;
	//cout<<endl<<"请求活跃度信息！"<<endl;
	if(player && player->IsOpenSystem(eStVitality))
	{
		player->GetVitalityMgr()->SetVitality(&ack);
	}
	else
		return;

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_GETVITALITYINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(VitalityMessage, MSG_REQ_C2GM_GETVITALITYGIFT)
{
	ServerReturn::ServerResultInt req;
	ServerReturn::ServerRetInt ack;

	req.ParseFromString(content);
	//cout<<endl<<"请求领取活跃度奖励！"<<endl;
	int64 ret = req.ret();
	if(player && player->IsOpenSystem(eStVitality))
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eVitality, BehaviorType::Minor::eVitality_GetAward, MONITOR);

		ret = (ret<<32) | (player->GetVitalityMgr()->GetAward(req.ret()+1)&0xffffffff);

		
	}
	else
	{
		ret = eNotOpen;
	}
	ack.set_ret(ret);

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_GETVITALITYGIFT);
}

