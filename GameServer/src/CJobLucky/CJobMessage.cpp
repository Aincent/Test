/*
 * CJobMessage.cpp
 *
 *  Created on: 2014年8月18日
 *      Author: helh
 */

#include "CJobMessage.h"
#include "MessageStruct/CharLogin/CJobLucky.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "CJobLuckyMgr.h"
#include "../MessageBuild.h"
#include "../StatisticMgr/StatisticHandle.h"

CJobMessage::CJobMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(CJobMessage, MSG_REQ_C2GM_CJOB_INFO);
	DEF_MSG_QUICK_REQUEST_REG_FUN(CJobMessage, MSG_REQ_C2GM_RAND_CJOB);
	DEF_MSG_QUICK_REQUEST_REG_FUN(CJobMessage, MSG_REQ_C2GM_SALE_CJOB);
	DEF_MSG_QUICK_REQUEST_REG_FUN(CJobMessage, MSG_REQ_C2GM_EXCHANGE_CJOB);
	DEF_MSG_QUICK_REQUEST_REG_FUN(CJobMessage, MSG_REQ_C2GM_EAT_CJOB);
	DEF_MSG_QUICK_REQUEST_REG_FUN(CJobMessage, MSG_REQ_C2GM_WEAR_CJOB);
	DEF_MSG_QUICK_REQUEST_REG_FUN(CJobMessage, MSG_REQ_C2GM_OPEN_CJOB);
	DEF_MSG_QUICK_REQUEST_REG_FUN(CJobMessage, MSG_REQ_C2GM_CHANGE_CJOB);
	DEF_MSG_QUICK_REQUEST_REG_FUN(CJobMessage, MSG_REQ_C2GM_UNWEAR_CJOB);
}

CJobMessage::~CJobMessage()
{
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(CJobMessage,MSG_REQ_C2GM_CJOB_INFO)
{
	PlayerInfo::RandCJobLucky toClient;
	ServerReturn::ServerRetInt req;

	req.ParseFromString(content);

//	if (player && player->IsOpenSystem(eStCJobItem))
	{
		player->GetCJobLucky()->SetCJobLucky(&toClient);
	}
//	else
//		return;

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_CJOB_INFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(CJobMessage,MSG_REQ_C2GM_RAND_CJOB)
{
	ServerReturn::ServerThreeInt toClient;
	ServerReturn::ServerRetInt req;

	req.ParseFromString(content);

	int tempReturn = 0;
	int64 temp = 0;
//	if (player && player->IsOpenSystem(eStCJobItem))
	{
		player->GetCJobLucky()->RandJob(req.ret(),tempReturn,temp);
	}
//	else
//	{
//		tempReturn = eNotOpen;
//	}

	toClient.set_rets(tempReturn);
	toClient.set_retf((temp >> 32)&0xFFFFFFFF);
	toClient.set_rett(temp&0xFFFFFFFF);

	int tempRandIndex   = player->GetCJobLucky()->GetRandIndex();
	int tempClientIndex = req.ret();

	LOG_DEBUG(FILEINFO, "RandCJob return playerID %lld value %lld randIndex %d clientIndex %d",player->GetID(),tempReturn,tempRandIndex,tempClientIndex);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_RAND_CJOB);
	
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(CJobMessage, MSG_REQ_C2GM_SALE_CJOB)
{
	ServerReturn::ServerRetInt toClient;
	ServerReturn::ServerRetInt req;

	req.ParseFromString(content);

//	if (player && player->IsOpenSystem(eStCJobItem))
	{
		toClient.set_ret(player->GetCJobLucky()->HanlePackage(req.ret() & 0x1, (req.ret() >> 1) & 0x1, (req.ret() >> 2) & 0xff, req.ret()));
	}
//	else
//	{
//		toClient.set_ret(eNotOpen);
//	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_SALE_CJOB);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(CJobMessage, MSG_REQ_C2GM_EXCHANGE_CJOB)
{
	ServerReturn::ServerRetInt toClient;
	ServerReturn::ServerRetInt req;

	req.ParseFromString(content);

	if (player)
	{

//		if (player->IsOpenSystem(eStCJobItem))
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eJobLucky, BehaviorType::Minor::eJobLucky_Exchange, MONITOR);

			toClient.set_ret(player->GetCJobLucky()->ExchangeCJob(req.ret()));

			

		}
//		else
//		{
//			toClient.set_ret(eNotOpen);
//		}

		REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_EXCHANGE_CJOB);
	}


}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(CJobMessage, MSG_REQ_C2GM_EAT_CJOB)
{
	ServerReturn::ServerThreeInt toClient;
	ServerReturn::ServerRetInt req;

	req.ParseFromString(content);

//	if (player && player->IsOpenSystem(eStCJobItem))
	{
		player->GetCJobLucky()->EatOtherCJob(req.ret(), toClient);
	}
//	else
//	{
//		toClient.set_rett(eNotOpen);
//	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_EAT_CJOB);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(CJobMessage, MSG_REQ_C2GM_WEAR_CJOB)
{
	ServerReturn::ServerRetInt toClient;
	ServerReturn::ServerRetInt req;

	req.ParseFromString(content);

//	if (player && player->IsOpenSystem(eStCJobItem))
	{
		toClient.set_ret(player->GetCJobLucky()->WearCJob(req.ret()));
	}
//	else
//	{
//		toClient.set_ret(eNotOpen);
//	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_WEAR_CJOB);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(CJobMessage, MSG_REQ_C2GM_OPEN_CJOB)
{
	ServerReturn::ServerRetInt req;
	ServerReturn::ServerRetInt toClient;

	req.ParseFromString(content);


//	if (player && player->IsOpenSystem(eStCJobItem))
	{
		int64 openSize = req.ret();
		int64 ret = 0;
		int err = 0;
		player->GetCJobLucky()->OpenPackage(openSize, err);
		ret = err;
		if(0 == ret)
		{
			//高32位能开启的格子数  低32位错误码
			ret = openSize<<32 | ret;
		}
		toClient.set_ret(ret);
	}
//	else
//	{
//		toClient.set_ret(eNotOpen);
//	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_OPEN_CJOB);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(CJobMessage, MSG_REQ_C2GM_CHANGE_CJOB)
{
	ServerReturn::ServerRetInt toClient;
	ServerReturn::ServerRetInt req;

	req.ParseFromString(content);

//	if (player && player->IsOpenSystem(eStCJobItem))
	{
		toClient.set_ret(player->GetCJobLucky()->ChangeCJobSector(req.ret()));
	}
//	else
//	{
//		toClient.set_ret(eNotOpen);
//	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_CHANGE_CJOB);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(CJobMessage, MSG_REQ_C2GM_UNWEAR_CJOB)
{
	ServerReturn::ServerRetInt toClient;
	ServerReturn::ServerRetInt req;

	req.ParseFromString(content);

//	if (player && player->IsOpenSystem(eStCJobItem))
	{
		toClient.set_ret(player->GetCJobLucky()->UnWearCJob(req.ret()));
	}
//	else
//	{
//		toClient.set_ret(eNotOpen);
//	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_UNWEAR_CJOB);
}





