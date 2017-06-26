/*
 * CountryRedEnvelopeMessage.cpp
 *
 *  Created on: 2015年4月9日
 *      Author: root
 */
#include "CountryRedEnvelopeMessage.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "../Object/Player/Player.h"
#include "../Object/Player/PlayerMessage.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "Smart_Ptr.h"
#include <netinet/in.h>
#include "MessageCommonRet.h"
#include "MessageStruct/RedEnvelope/RedEnvelope.pb.h"
#include "CountryRedEnvelopeMgr.h"
#include "../Attribute/AttributeCreature.h"
#include "../ActiveUIHighLight/CActiveHighLightMgr.h"
#include "../Country/GsCountryMgr.h"
#include "../StatisticMgr/StatisticHandle.h"

CountryRedEnvelopeMessage::CountryRedEnvelopeMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(CountryRedEnvelopeMessage,MSG_REQ_C2GM_GET_REDENVELOPE);
	DEF_MSG_QUICK_REQUEST_REG_FUN(CountryRedEnvelopeMessage,MSG_REQ_C2GM_SEND_REDENVELOPE);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2WS_GET_REDENVELOPE);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2WS_SEND_REDENVELOPE);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_GET_REWARD);
//	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_ISHAVEENVELOPETOGET);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_AllGMADDENVELOPE);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_PLAYERADDENVELOPE);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_ENVSENDERBYGET);
}

CountryRedEnvelopeMessage::~CountryRedEnvelopeMessage()
{
}
void CountryRedEnvelopeMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
//	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_ISHAVEENVELOPETOGET);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_AllGMADDENVELOPE);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_PLAYERADDENVELOPE);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_ENVSENDERBYGET);

	DEF_SWITCH_TRY_DISPATCH_END
}

void CountryRedEnvelopeMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2WS_GET_REDENVELOPE);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2WS_SEND_REDENVELOPE);
	DEF_SWITCH_TRY_DISPATCH_END
}

void CountryRedEnvelopeMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_GET_REWARD);
	DEF_SWITCH_TRY_DISPATCH_END
}

//DEF_MSG_SIMPLE_DEFINE_FUN(CountryRedEnvelopeMessage, MSG_SIM_WS2GM_ISHAVEENVELOPETOGET)
//{
//	ServerReturn::ServerDoubleInt  req;
//	char *con;
//	int len = 0;
//	Smart_Ptr<Player> player;
//
//	con = message->GetBuffer(len);
//	req.ParseFromArray(con, len);
//
//	PlayerMessage::GetInstance()->GetPlayerByCharid(req.retf(),player);
//	if (bool(player))
//	{
//		player->getEnvelopeMgr()->setIsHaveEnvelopeToGet(bool(req.rets()));
//
//		player->SynchActiveHighLight();
//	}
//}

DEF_MSG_REQUEST_DEFINE_FUN(CountryRedEnvelopeMessage, MSG_REQ_WS2GM_GET_REWARD)
{
	RedEnvelopeProto::ReqGMGetEnvelopeReward  req;
	RedEnvelopeProto::RetGMGetEnvelopeReward  toWS;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;
	int ret = -1;

	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);

	PlayerMessage::GetInstance()->GetPlayerByCharId(player,req.charid());
	if(bool(player))
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eCountry, BehaviorType::Minor::eCountry_GetRedEnvelope, MONITOR);;

		ret = player->getEnvelopeMgr()->GetEnvelopeReward(req);

		toWS.set_ret(ret);
		toWS.set_charid(req.charid());
		if(ret == 0)
		{
			RedEnvelopeProto::ReqGMGetEnvelopeReward *temp = toWS.mutable_getinfo();
			temp->CopyFrom(req);
		}

		
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(message,&toWS,Ack);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(CountryRedEnvelopeMessage,MSG_REQ_C2GM_GET_REDENVELOPE)
{
	RedEnvelopeProto::ReqGetEnvelope req;
	RedEnvelopeProto::RetGetEnvelope toClient;

	req.ParseFromString(content);

	if(bool(player) == false)
		LOG_DEBUG(FILEINFO, "request get envelope but player is null");

	int restGetEnvelopeCount = player->getEnvelopeMgr()->GetCurrentCanGetEnvelopeCount();
	if(restGetEnvelopeCount <=0)
	{
		toClient.set_ret(eDontHaveGetEnvelopCount);
		REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_GET_REDENVELOPE);
	}
	else
	{
		int op = req.op();
		if(op == GetEnvelopeOpType_all)
			req.set_cangetcount(restGetEnvelopeCount);

		req.set_charid(player->GetID());

		Smart_Ptr<SvrItem> worldSvr;
		ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),worldSvr);
		if(worldSvr)
		{
			Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2WS_GET_REDENVELOPE, &req, worldSvr.Get(), Request);
			messRet->SetAct(new requestAct(message));
			Message_Facade::Send(messRet);
		}
	}

	return;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(CountryRedEnvelopeMessage,MSG_REQ_C2GM_SEND_REDENVELOPE)
{
	RedEnvelopeProto::ReqSendEnvelope req;
	RedEnvelopeProto::RetSendEnvelope toClient;

	req.ParseFromString(content);

	if(bool(player) == false)
		LOG_DEBUG(FILEINFO, "request get envelope but player is null");

	int sendEnvelopeCount = req.count();
	int envelopCount = player->getAttManage()->getValue<int>(eEnvelopeCount);
	if(envelopCount <=0 || sendEnvelopeCount<=0)
	{
		toClient.set_ret(eDontHaveEnvelope);
		REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_SEND_REDENVELOPE);
	}
	else
	{
		req.set_charid(player->GetID());
		req.set_name(player->GetName());
		req.set_country(player->getAttManage()->getValue<DWORD>(eCharCountry));
		if(sendEnvelopeCount > 1)
			req.set_count(envelopCount);

		Smart_Ptr<SvrItem> worldSvr;
		ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),worldSvr);
		if(worldSvr)
		{
			Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2WS_SEND_REDENVELOPE, &req, worldSvr.Get(), Request);
			messRet->SetAct(new requestAct(message));
			Message_Facade::Send(messRet);
		}
	}
}

DEF_MSG_ACK_DEFINE_FUN(CountryRedEnvelopeMessage, MSG_REQ_GM2WS_GET_REDENVELOPE)
{
	//请求超时
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request get red envelope and ack timeout");
		return;
	}
	RedEnvelopeProto::RetGetEnvelope toClient;
	int len = 0;
	Smart_Ptr<Player> player;

	char * content = message->GetBuffer(len);
	toClient.ParseFromArray(content, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(toClient.charid(), player);
	if(bool(player))
	{
		if(toClient.ret() == 0)
		{
			int getCount = toClient.envelopidlist_size();
			if(getCount > 0)
			{
				//再次判断剩余领取次数，防止玩家使用连点器模拟发包导致玩家可以不管限制领取次数领取所有红包,因为从世界服到逻辑服的消息有时间差
				int restNum = player->getEnvelopeMgr()->GetCurrentCanGetEnvelopeCount();
				if(restNum > 0)
				{
					player->GetCounterService()->IncreasePlayerCounter(GET_ENVELOP_COUNT,getCount);
					player->ChangeBindGolden(toClient.getbindgolden(),true);
					vector<int> vecType;
					vecType.push_back(eCanGetEnvelopeCount);
					vecType.push_back(eCharBindGolden);
					player->SynCharAttribute(vecType);

//					if (toClient.getbindgolden() > 0)
//					{
//						StatisticInfo::CostAndGotInfo costAndGotInfo;
//						StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//						StatisticMgr::AddItemGotInfo(gotInfo,eCharBindGolden,toClient.getbindgolden());
//						StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_BindGolden,eStaMinor_BindGolden_Envelope,&costAndGotInfo);
//					}
					player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Country, eUIHLSmall_Country_HaveUnGetReward), getCount, false);
				}
				else
					toClient.set_ret(eDontHaveGetEnvelopCount);
			}
		}
		REQ_MSG_QUICK_ACK_FUN(static_cast<requestAct *>(act.Get())->mesReq, &toClient, MSG_REQ_C2GM_GET_REDENVELOPE);
	}

	return;
}

DEF_MSG_ACK_DEFINE_FUN(CountryRedEnvelopeMessage, MSG_REQ_GM2WS_SEND_REDENVELOPE)
{
	//请求超时
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request send envelope and ack timeout");
		return;
	}

	RedEnvelopeProto::RetSendEnvelope toClient;
	int len = 0;
	Smart_Ptr<Player> player;

	char * content = message->GetBuffer(len);
	toClient.ParseFromArray(content, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(toClient.charid(), player);
	if(bool(player))
	{
		if(toClient.ret() == 0)
		{
			int currentEnvelopeCount = player->getAttManage()->getValue<int>(eEnvelopeCount);
			int sendEnvelopeCount = toClient.count();

			player->setEnvelopCount(currentEnvelopeCount-sendEnvelopeCount);

			vector<int> vecType;
			vecType.push_back(eEnvelopeCount);
			player->SynCharAttribute(vecType);
		}
		REQ_MSG_QUICK_ACK_FUN(static_cast<requestAct *>(act.Get())->mesReq, &toClient, MSG_REQ_C2GM_SEND_REDENVELOPE);
	}

	return;
}

DEF_MSG_SIMPLE_DEFINE_FUN(CountryRedEnvelopeMessage, MSG_SIM_WS2GM_AllGMADDENVELOPE)
{
	RedEnvelopeProto::WSToAllGMAddEnvelope req;
	int len = 0;
	char* content = message->GetBuffer(len);

	req.ParseFromArray(content, len);

	if (req.country() < eCountryID_max && req.country() > eCountryID_null)
	{
		for (int i = 0; i < req.charid_size(); ++i)
		{
			Smart_Ptr<Player> player;
			PlayerMessage::GetInstance()->GetPlayerByCharId(player, req.charid(i));

			if ((bool)player && player->IsOpenSystem(eCharCountry) &&
					player->getAttManage()->getValue<int>(eCharCountry) == req.country())
			{
				int count = 0;
				count = req.count();
				if (req.flag() )
				{
					int value = 0;
					player->GetActiveHLMgr()->GetValueByType(eUIHLBig_Country, eUIHLSmall_Country_HaveUnGetReward, value);
					if (value + req.count() > player->getEnvelopeMgr()->GetCurrentCanGetEnvelopeCount())
					{
						count = player->getEnvelopeMgr()->GetCurrentCanGetEnvelopeCount() - value;
					}
				}
				player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Country, eUIHLSmall_Country_HaveUnGetReward), count, req.flag());
			}
		}
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(CountryRedEnvelopeMessage, MSG_SIM_WS2GM_PLAYERADDENVELOPE)
{
	RedEnvelopeProto::WSToGMPlayerAddEnvelope req;
	int len = 0;
	char* content = message->GetBuffer(len);

	req.ParseFromArray(content, len);

	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharId(player, req.charid());
	if ((bool)player)
	{
		player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Country, eUIHLSmall_Country_HaveUnGetReward), req.count(), true);
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(CountryRedEnvelopeMessage, MSG_SIM_WS2GM_ENVSENDERBYGET)
{
	RedEnvelopeProto::WSToEnvSenderByGet req;
	Smart_Ptr<Player> player;
	int len = 0;
	char* buffer = message->GetBuffer(len);

	req.ParseFromArray(buffer, len);

	PlayerMessage::GetInstance()->GetPlayerByCharId(player, req.charid());

	if ((bool)player)
	{
		if (0 < req.count())
		{
			player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Country, eUIHLSmall_Country_HaveUnGetReward), req.count(), true);
		}
	}

}
