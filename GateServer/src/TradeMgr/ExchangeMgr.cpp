/*
 * ExchangeMgr.cpp
 *
 *  Created on: 2014年5月16日
 *      Author: root
 */

#include "ExchangeMgr.h"
#include "SvrConfig.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "MessageStruct/CharLogin/ExchangMessage.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageStruct/ServerReturnInt.pb.h"


ExchangeMgr::ExchangeMgr()
{

	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_GM2GT_EXCHANGETOOTHER);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_GM2GT_LOCKINFOTOOTHER);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_GT2C_BUILDEXCHANGE);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_GM2GT_CANCELTOOTHER);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_REQ_GT2GM_VERIFYEXCHANGE);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_GM2GT_VERIFYTOOTHER);


	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_INITEXCHANGE);		//请求交易
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_OTHERSTATUS);			//trader的反馈请求
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_CANCELEXCHANGE);		//取消交易
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_EXCHANGELOCK);		//锁定交易
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_VERIFYEXCHANGE);		//确认交易


	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_INITEXCHANGE);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_OTHERSTATUS);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_CANCELEXCHANGE);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_EXCHANGELOCK);		//锁定交易
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_VERIFYEXCHANGE);		//确认交易

	//玩家交易消息
}

ExchangeMgr::~ExchangeMgr()
{

}

void ExchangeMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GM2GT_EXCHANGETOOTHER);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GM2GT_LOCKINFOTOOTHER);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GT2C_BUILDEXCHANGE);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GM2GT_CANCELTOOTHER);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GM2GT_VERIFYTOOTHER);

	DEF_SWITCH_TRY_DISPATCH_END
}

void ExchangeMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_INITEXCHANGE);		//请求交易
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_OTHERSTATUS);			//trader的反馈请求
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_CANCELEXCHANGE);		//取消交易
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_EXCHANGELOCK);		//锁定交易
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_VERIFYEXCHANGE);		//确认交易

	DEF_SWITCH_TRY_DISPATCH_END
}

void ExchangeMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_INITEXCHANGE);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_OTHERSTATUS);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_CANCELEXCHANGE);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_EXCHANGELOCK);		//锁定交易
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_VERIFYEXCHANGE);		//确认交易

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_REQUEST_DEFINE_FUN(ExchangeMgr, MSG_REQ_C2GT_INITEXCHANGE)
{
	LOG_DEBUG(FILEINFO, "client request Init exchange");

//	Exchang::ClientReqExchange clientReq;
//	Exchang::GTtoGMReqExchange reqGM;
	int gsChannel = -1;
	int64 charid = -1;
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	clientReq.ParseFromArray(str, len);

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
//		reqGM.set_charid(charid);
//		reqGM.set_dwtraderid(clientReq.dwtraderid());

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_INITEXCHANGE,message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(ExchangeMgr, MSG_REQ_C2GT_OTHERSTATUS)
{
	LOG_DEBUG(FILEINFO, "trader reply to applicant");

//	Exchang::TraderReqRet clientReq;
//	Exchang::GTtoGMTraderReqRet reqGM;
	int gsChannel = -1;
	int64 charid = -1;
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	clientReq.ParseFromArray(str, len);

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
//		reqGM.set_charid(charid);
//		reqGM.set_dwsrcid(clientReq.dwsrcid());
//		reqGM.set_status(clientReq.status());

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_OTHERSTATUS,message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(ExchangeMgr, MSG_REQ_C2GT_CANCELEXCHANGE)
{
	LOG_DEBUG(FILEINFO, "client request cancel exchange");

//	Exchang::ClientReqCancelExchang clientReq;
//	Exchang::GTtoGMCancelExchang reqGM;
	int gsChannel = -1;
	int64 charid = -1;
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	clientReq.ParseFromArray(str, len);

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
//		reqGM.set_charid(charid);
//		reqGM.set_dwplayerid(clientReq.dwplayerid());

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_CANCELEXCHANGE,message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(ExchangeMgr, MSG_REQ_C2GT_EXCHANGELOCK)
{
	LOG_DEBUG(FILEINFO, "client request lock exchange");

//	Exchang::ClientReqLock clientReq;
//	Exchang::GTtoGMReqReqLock reqGM;
	int gsChannel = -1;
	int64 charid = -1;
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	clientReq.ParseFromArray(str, len);

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
//		reqGM.set_charid(charid);
//		reqGM.set_dwplayerid(clientReq.dwplayerid());
//		reqGM.set_dwmoney(clientReq.dwmoney());
//		for(int i = 0; i < clientReq.dwpacketpos_size(); i++)
//		{
//			reqGM.add_dwpacketpos(clientReq.dwpacketpos(i));
//		}

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_EXCHANGELOCK,message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}


DEF_MSG_REQUEST_DEFINE_FUN(ExchangeMgr, MSG_REQ_C2GT_VERIFYEXCHANGE)
{
	LOG_DEBUG(FILEINFO, "client request verify exchange");

//	Exchang::ClientReqVerifyExchang clientReq;
//	Exchang::GTtoGMVerifyExchang reqGM;
	int gsChannel = -1;
	int64 charid = -1;
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	clientReq.ParseFromArray(str, len);

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
//		reqGM.set_charid(charid);
//		reqGM.set_dwplayerid(clientReq.dwplayerid());

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_VERIFYEXCHANGE,message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(ExchangeMgr, MSG_REQ_GT2GM_INITEXCHANGE)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver request Init exchange and ack timeout");

		return;
	}

	LOG_DEBUG(FILEINFO, "gateserver request gameserver request Init exchange and ack from gameserver");

//	Exchang::GTAckSrcExchange ack;
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	ack.ParseFromArray(str, len);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_INITEXCHANGE,message,static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}

DEF_MSG_ACK_DEFINE_FUN(ExchangeMgr, MSG_REQ_GT2GM_OTHERSTATUS)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver trader reply to applicant and ack timeout");

		return;
	}

	LOG_DEBUG(FILEINFO, "gateserver request gameserver trader reply to applicant and ack from gameserver");

//	Exchang::GTAckOtherClientReqExchange ack;
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	ack.ParseFromArray(str, len);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_OTHERSTATUS,message,static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}

DEF_MSG_ACK_DEFINE_FUN(ExchangeMgr, MSG_REQ_GT2GM_CANCELEXCHANGE)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver Cancel exchange back and ack timeout");

		return;
	}


	LOG_DEBUG(FILEINFO, "gateserver request gameserver Cancel exchange and ack from gameserver");

//	Exchang::GTAckCancelExchang ack;
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	ack.ParseFromArray(str, len);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_CANCELEXCHANGE,message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}

DEF_MSG_ACK_DEFINE_FUN(ExchangeMgr, MSG_REQ_GT2GM_EXCHANGELOCK)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver lock exchange back and ack timeout");

		return;
	}


	LOG_DEBUG(FILEINFO, "gateserver request gameserver lock exchange and ack from gameserver");

//	Exchang::GTAckSrcReqLock ack;
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	ack.ParseFromArray(str, len);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_EXCHANGELOCK,message,static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}


DEF_MSG_ACK_DEFINE_FUN(ExchangeMgr, MSG_REQ_GT2GM_VERIFYEXCHANGE)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver verify exchange back and ack timeout");

		return;
	}


	LOG_DEBUG(FILEINFO, "gateserver request gameserver verify exchange and ack from gameserver");

//	Exchang::ClientReqVerifyExchang ack;
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	ack.ParseFromArray(str, len);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_VERIFYEXCHANGE,message,static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
	Message_Facade::Send(messRet);
}

DEF_MSG_SIMPLE_DEFINE_FUN(ExchangeMgr, MSG_SIM_GM2GT_EXCHANGETOOTHER)
{
	Exchang::GTtoGMReqExchange toGT;
	Exchang::GTAckToApplicant ask;
	char *con = 0;
	int len = 0;
	int channel = -1;

	con = message->GetBuffer(len);
	toGT.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "Player send to trader init exchange to GateServer");

	ask.set_dwcharid(toGT.dwtraderid());		//发起交易方的Key

	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(toGT.charid(), channel))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_GM2GT_EXCHANGETOOTHER, &ask, channel, SimpleMessage);
		Message_Facade::Send(clientRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "gateserver send to trader init exchange but find not client[ charid=%lld]",GET_PLAYER_CHARID(ask.dwcharid()));
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(ExchangeMgr, MSG_SIM_GT2C_BUILDEXCHANGE)
{
	Exchang::GTtoGMTraderReqRet toGT;
	Exchang::GTAckOtherClientReqExchange ask;
	char *con = 0;
	int len = 0;
	int channel = -1;

	con = message->GetBuffer(len);
	toGT.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "Player send to build exchange to GateServer");

	ask.set_dwerrorcode(toGT.status());		//错误码
	ask.set_dwcharid(toGT.dwsrcid());		//对方的key

	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(toGT.charid(), channel))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_GT2C_BUILDEXCHANGE, &ask, channel, SimpleMessage);
		Message_Facade::Send(clientRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "gateserver send to build exchange but find not client[ charid=%lld]",GET_PLAYER_CHARID(ask.dwcharid()));
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(ExchangeMgr, MSG_SIM_GM2GT_CANCELTOOTHER)
{
	Exchang::GTtoGMReqExchange toGT;
	Exchang::GTAckCancelExchang ask;
	char *con = 0;
	int len = 0;
	int channel = -1;

	con = message->GetBuffer(len);
	toGT.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "Player send to trader cancel exchange to Client");

	ask.set_dwplayerid(toGT.dwtraderid());		//

	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(toGT.charid(), channel))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_GM2GT_CANCELTOOTHER, &ask, channel, SimpleMessage);
		Message_Facade::Send(clientRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "gateserver send to trader cancel exchange but find not client[ charid=%lld]",GET_PLAYER_CHARID(toGT.charid()));
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(ExchangeMgr, MSG_SIM_GM2GT_LOCKINFOTOOTHER)
{
	Exchang::GMToGTLockToOther toGT;
	Exchang::GTAckReqLock ask;
	char *con = 0;
	int len = 0;
	int channel = -1;

	con = message->GetBuffer(len);
	toGT.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "Player send to trader lock info to Client");

	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(toGT.charid(), channel))
	{
		ask.set_dwmoney(toGT.dwmoney());
		ask.set_dwtraderid(toGT.dwtraderid());
		for(int i = 0; i < toGT.proplist_size(); i++)
		{
			CharLogin::ClientItemInfo *clientInfo = toGT.mutable_proplist(i);
			CharLogin::ClientItemInfo *cc = ask.add_proplist();
	//		cc = clientInfo;
			cc->CopyFrom(*clientInfo);
		}

		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_GM2GT_LOCKINFOTOOTHER, &ask, channel, SimpleMessage);
		Message_Facade::Send(clientRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "gateserver send to trader cancel exchange but find not client[ charid=%lld]",GET_PLAYER_CHARID(toGT.charid()));
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(ExchangeMgr, MSG_SIM_GM2GT_VERIFYTOOTHER)
{
	Exchang::GTtoGMReqExchange toGT;
	Exchang::GTAckToApplicant ask;
	char *con = 0;
	int len = 0;
	int channel = -1;

	con = message->GetBuffer(len);
	toGT.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "Player send to trader complete exchange to GateServer");

	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(toGT.charid(), channel))
	{
		ask.set_dwcharid(toGT.charid());		//将成功的结果发给客户端
		ask.set_dwerrorcode(toGT.dwtraderid());

		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_REQ_C2GT_VERIFYEXCHANGE, &ask, channel, SimpleMessage);
		Message_Facade::Send(clientRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "gateserver send to trader complete exchange but find not client[ charid=%lld]",GET_PLAYER_CHARID(ask.dwcharid()));
	}
}
