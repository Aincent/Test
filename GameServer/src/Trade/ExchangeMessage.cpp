/*
 * ExchangeMessage.cpp
 *
 *  Created on: 2014年5月15日
 *      Author: cz
 */


#include "ExchangeMessage.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/CharLogin/ExchangMessage.pb.h"
#include "msg_Exchang.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Map/GameZone.h"
#include "../Map/SceneManager.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageCommonRet.h"
#include "CharDefine.h"


ExchangeMessage::ExchangeMessage()
{	
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_INITEXCHANGE);		//请求交易
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_OTHERSTATUS);			//trader的反馈请求
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_CANCELEXCHANGE);		//取消交易
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_EXCHANGELOCK);		//锁定交易
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_VERIFYEXCHANGE);		//确认交易


	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GT2GM_INITEXCHANGE);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GT2GM_OTHERSTATUS);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GT2GM_CANCELEXCHANGE);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GT2GM_EXCHANGELOCK);		//锁定交易
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GT2GM_VERIFYEXCHANGE);		//确认交易

}

ExchangeMessage::~ExchangeMessage()
{

}

void ExchangeMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN


	DEF_SWITCH_TRY_DISPATCH_END
}

void ExchangeMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_INITEXCHANGE);		//请求交易
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_OTHERSTATUS);			//trader的反馈请求
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_CANCELEXCHANGE);		//取消交易
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_EXCHANGELOCK);		//锁定交易
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_VERIFYEXCHANGE);		//确认交易

	DEF_SWITCH_TRY_DISPATCH_END

}

void ExchangeMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_INITEXCHANGE);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_OTHERSTATUS);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_CANCELEXCHANGE);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_EXCHANGELOCK);		//锁定交易
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_VERIFYEXCHANGE);		//确认交易

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_REQUEST_DEFINE_FUN(ExchangeMessage, MSG_REQ_GT2GM_INITEXCHANGE)
{
//	Exchang::GTtoGMReqExchange reqGM;
	Exchang::ClientReqExchange reqGM;
	Exchang::GTAckSrcExchange ack;
	Smart_Ptr<Player> player;
	int len = 0;
	char *str = message->GetBuffer(len);

	reqGM.ParseFromArray(str, len);
	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{
		if(reqGM.dwtraderid() < 0)
		{
			return;
		}

		// 获取角色
		Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(player->GetMapID());
		if(!zone)
		{
			LOG_ERROR(FILEINFO, "client request exchange but scene is not existed");
		}
		Player* pTrader = NULL;

		DWORD dwErrorCode = 0;


		if(player->GetCDMgr()->IsCDEnd(EXCHANGE_CD))
		{
			dwErrorCode = player->ProcExchangeReq(pTrader, reqGM.dwtraderid());
		}
		else
		{
			dwErrorCode = eExchangeCDNotReady;
		}

		//玩家是否拒绝交易
//		if(pTrader)
//		{
//			if(pTrader->GetSettings(eSettingsTrade))
//			{
//				dwErrorCode = ePlayerNotExchange;
//			}
//		}

		if(0 == dwErrorCode && pTrader)
		{

			// 向被申请者发消息
//			Exchang::GTtoGMReqExchange toTrader;		//这个结构由于带了char_id，和一个int型字节，专门用来作交易GM给GT的一般消息
//			toTrader.set_charid(pTrader->GetID());		//网关需要char_id找到client
//
//			toTrader.set_dwtraderid(player->GetKey());

			//LOG_DEBUG(FILEINFO, "Player send to trader init exchange simple message");
//			Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_SIM_GM2GT_EXCHANGETOOTHER, &toTrader, pTrader->GetChannelID(), SimpleMessage);
//			Message_Facade::Send(messRet);

			Exchang::GTAckToApplicant ask;
			ack.set_dwtraderid(player->GetKey());

			QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&ask,MSG_SIM_GM2GT_EXCHANGETOOTHER);

			//// 向申请者反馈
			ack.set_dwerrorcode(dwErrorCode);
			ack.set_dwtraderid(pTrader->GetID());
			Safe_Smart_Ptr<CommBaseOut::Message> messRetChange  = build_message(MSG_REQ_GT2GM_INITEXCHANGE, message, &ack);
			Message_Facade::Send(messRetChange);
		}
		else if(dwErrorCode != 0 && pTrader)
		{
			pTrader->EndExchange();
			player->EndExchange();
			//// 向申请者反馈
			ack.set_dwerrorcode(dwErrorCode);
			ack.set_dwtraderid(pTrader->GetID());
			Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_INITEXCHANGE, message, &ack);
			Message_Facade::Send(messRet);
		}
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request exchange but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(ExchangeMessage, MSG_REQ_GT2GM_OTHERSTATUS)
{
//
//	Exchang::GTtoGMTraderReqRet reqGM;
	Exchang::TraderReqRet reqGM;
	Exchang::GTAckOtherClientReqExchange ack;

	Smart_Ptr<Player> trader;
	int len = 0;
	char *str = message->GetBuffer(len);

	reqGM.ParseFromArray(str, len);
	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, trader);
	if((bool)trader && !(bool)reqGM.status())
	{
		Player *pApplicant = NULL;

		int dwErrorCode = -1;
		dwErrorCode = trader->ProcExchangeReqRes(pApplicant, reqGM.dwsrcid(), reqGM.status());
		if(!pApplicant)
		{
			LOG_ERROR(FILEINFO, "trader ack exchange to applicant but Applicant is not exist");
			return;
		}

		//if(!(bool)reqGM.status())			//对方同意交易
		{
			// 检查是否需要重置交易状态
			if(dwErrorCode != E_Exchange_Accept)
			{
				trader->GetExchMgr().SetTgtPlayerKey(-1);
			}

			if(dwErrorCode != 0 )
			{
				pApplicant->EndExchange();
				trader->EndExchange();
			}

			ack.set_dwerrorcode(dwErrorCode);
			// 为简化服务器端逻辑，所以情况均向双方发送消息，由客户端处理
			if(E_Exchange_Accept == ack.dwerrorcode())
			{
				ack.set_dwcharid(pApplicant->GetKey());
				Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_OTHERSTATUS, message, &ack);
				Message_Facade::Send(messRet);
			}

			if(pApplicant)
			{
//				Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_OTHERSTATUS, message, &ack);
//				Message_Facade::Send(messRet);

//				Exchang::GTtoGMTraderReqRet toApplicant;
//				toApplicant.set_charid(pApplicant->GetID());
//				toApplicant.set_dwsrcid(trader->GetKey());
//				toApplicant.set_status(dwErrorCode);
//
//				LOG_DEBUG(FILEINFO, "Player send to applicant receive accept simple message");
//				Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_SIM_GT2C_BUILDEXCHANGE, &toApplicant, pApplicant->GetChannelID(), SimpleMessage);
//				Message_Facade::Send(messRet);

				Exchang::GTAckOtherClientReqExchange ask;
				ask.set_dwerrorcode(dwErrorCode);		//错误码
				ask.set_dwcharid(trader->GetKey());		//对方的key

				QuickMessage::GetInstance()->AddSendMessage(pApplicant->GetID(), pApplicant->GetChannelID(),&ask,MSG_SIM_GT2C_BUILDEXCHANGE);
			}
		}
//		else
//		{
//			trader->EndExchange();
//		}

	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request Buy Item From Npc but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(ExchangeMessage, MSG_REQ_GT2GM_CANCELEXCHANGE)
{
//	Exchang::GTtoGMCancelExchang reqGM;
	Exchang::ClientReqCancelExchang reqGM;
	Exchang::GTAckCancelExchang ack;     //这个结构由于带了char_id，和一个int型字节，专门用来作交易GM给GT的一般消息

	Smart_Ptr<Player> player;
	int len = 0;
	char *str = message->GetBuffer(len);
	reqGM.ParseFromArray(str, len);
	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{
		// 获取地图
	   Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(player->GetMapID());
	   if(!zone)
		 {
		   return;
		 }

		Smart_Ptr<CreatureObj> pTarget;
		zone->GetObject(reqGM.dwplayerid(), pTarget);
		if(!(pTarget.Get()))
		{
			// 被申请者不在线,或与申请者不在同一张地图中
			return;
		}

//		Player *temp = (Player*)pTarget.Get();
//		int dwErrorCode = player->ProcExchangeCancel(temp);
		// 向对方发消息
//		Exchang::GTtoGMReqExchange toOther;		//这个结构由于带了char_id，和一个int型字节，专门用来作交易GM给GT的一般消息
//		toOther.set_charid(pTarget->GetID());		//网关需要char_id找到client
//
//		toOther.set_dwtraderid(player->GetKey());	//取消交易玩家的key
//
//		LOG_DEBUG(FILEINFO, "Player send to trader init exchange simple message");
//		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_SIM_GM2GT_CANCELTOOTHER, &toOther, temp->GetChannelID(), SimpleMessage);
//		Message_Facade::Send(messRet);

		Exchang::GTAckCancelExchang ask;
		ask.set_dwplayerid(player->GetKey());

		QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&ask,MSG_SIM_GM2GT_CANCELTOOTHER);

		//// 向申请取消交易 的玩家反馈
//		ack.set_dwplayerid(player->GetKey());
//		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_CANCELEXCHANGE, message, &ack);
//		Message_Facade::Send(messRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request cancel exchange but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(ExchangeMessage, MSG_REQ_GT2GM_EXCHANGELOCK)
{
//	Exchang::GTtoGMReqReqLock reqGM;
	Exchang::ClientReqLock reqGM;
	Exchang::GTAckSrcReqLock ack;     //锁定给自己的消息

	//消息协议取消交换铜钱，将reqGM里带的money参数改成0
	reqGM.set_dwmoney(0);

	Smart_Ptr<Player> player;
	int len = 0;
	char *str = message->GetBuffer(len);
	reqGM.ParseFromArray(str, len);

	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{
		Player	*pTarget	= NULL;

		int	nInsIndex	= GT_INVALID;
		player->GetExchMgr().DelItem();
		DWORD	dwErrorCode = -1;
		for(int i = 0; i < reqGM.dwpacketpos_size(); i++)
		{
			uint packPos = reqGM.dwpacketpos(i);			//背包位置从0开始。标记从1开始
			dwErrorCode = player->ProcExchangeAdd(pTarget,packPos, nInsIndex);

			if(dwErrorCode != 0)
			{
				ack.set_dwerrorcode(dwErrorCode);
				Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_EXCHANGELOCK, message, &ack);
				Message_Facade::Send(messRet);
				return;
			}

		}

		//锁定检测
		dwErrorCode = player->ProcExchangeLock(pTarget);

		ack.set_dwerrorcode(dwErrorCode);
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_EXCHANGELOCK, message, &ack);
		Message_Facade::Send(messRet);

		if(dwErrorCode != 0 )
		{
			if(pTarget)
			{
				pTarget->EndExchange();
			}
			player->EndExchange();
		}


		//传给交易对象的数据
//		Exchang::GMToGTLockToOther toGt;
//
//		toGt.set_charid(pTarget->GetID());
//		toGt.set_dwtraderid(player->GetKey());
//
//		for(int i = 0; i < reqGM.dwpacketpos_size(); ++i)
//		{
//			CharLogin::ClientItemInfo *clientInfo = toGt.add_proplist();
//			Smart_Ptr<ArticleBaseMgr> pPacket = player->GetContainer(ePackageType);
//			Smart_Ptr<PropBase> tempItem = pPacket->GetItemDataPtr(reqGM.dwpacketpos(i));
//			tempItem->SetClientInfo(clientInfo);
//		}
//		toGt.set_dwmoney(reqGM.dwmoney());

//		Safe_Smart_Ptr<CommBaseOut::Message> messRetLock  = build_message(MSG_SIM_GM2GT_LOCKINFOTOOTHER, &toGt, pTarget->GetChannelID(), SimpleMessage);
//		Message_Facade::Send(messRetLock);

		Exchang::GTAckReqLock ask;
		ask.set_dwmoney(reqGM.dwmoney());
		ask.set_dwtraderid(player->GetKey());

		for(int i = 0; i < reqGM.dwpacketpos_size(); ++i)
		{
			Smart_Ptr<ArticleBaseMgr> pPacket = player->GetContainer(ePackageType);
			Smart_Ptr<PropBase> tempItem = pPacket->GetItemDataPtr(reqGM.dwpacketpos(i));
			if(false == bool(tempItem))
				continue;
			CharLogin::ClientItemInfo *cc = ask.add_proplist();
			tempItem->SetClientInfo(cc);
		}

		QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&ask,MSG_SIM_GM2GT_LOCKINFOTOOTHER);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(ExchangeMessage, MSG_REQ_GT2GM_VERIFYEXCHANGE)
{
//	Exchang::GTtoGMVerifyExchang reqGM;
	Exchang::ClientReqVerifyExchang reqGM;
	Exchang::GTAckExchangComplete ack;     //

	Smart_Ptr<Player> player;
	int len = 0;
	char *str = message->GetBuffer(len);
	reqGM.ParseFromArray(str, len);
	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{
		Player *pTarget = NULL;
		DWORD dwFailedPlayerID = -1;
		DWORD dwErrorCode = player->ProcExchangeVerify(pTarget,dwFailedPlayerID);
		if(dwErrorCode != 0 )		//59为第一个点确认交易的返回码
		{
			if(dwErrorCode != eExchangeOtherVerify)
			{
				if(pTarget)
				{
					pTarget->EndExchange();
				}
				player->EndExchange();
			}
		}
		ack.set_dwerrorcode(dwErrorCode);
		ack.set_dwfailedplayerkey(dwFailedPlayerID);
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_VERIFYEXCHANGE, message, &ack);
		Message_Facade::Send(messRet);

		//给对方 传递交易是否成功

//		Exchang::GTtoGMReqExchange toTrader;		//这个结构由于带了char_id，和一个int型字节，专门用来作交易GM给GT的一般消息
		if(!pTarget)
		{
			return;
		}
//		toTrader.set_charid(pTarget->GetID());		//网关需要char_id找到client
//
//		toTrader.set_dwtraderid(dwErrorCode);
//
//		LOG_DEBUG(FILEINFO, "Player send to trader complete exchange simple message");
//		Safe_Smart_Ptr<CommBaseOut::Message> messRetVer  = build_message(MSG_SIM_GM2GT_VERIFYTOOTHER, &toTrader, pTarget->GetChannelID(), SimpleMessage);
//		Message_Facade::Send(messRetVer);

		Exchang::GTAckToApplicant ask;

		ask.set_dwcharid(pTarget->GetID());		//将成功的结果发给客户端
		ask.set_dwerrorcode(dwErrorCode);

		QuickMessage::GetInstance()->AddSendMessage(pTarget->GetID(), pTarget->GetChannelID(),&ask,MSG_REQ_C2GT_VERIFYEXCHANGE);


		if(dwErrorCode == 0)
		{
			pTarget->EndExchange();
			player->EndExchange();
		}
	}
}

//-------------------------------Ack Function----------------------


DEF_MSG_ACK_DEFINE_FUN(ExchangeMessage,MSG_REQ_GT2GM_INITEXCHANGE)
{

}

DEF_MSG_ACK_DEFINE_FUN(ExchangeMessage,MSG_REQ_GT2GM_OTHERSTATUS)
{
}

DEF_MSG_ACK_DEFINE_FUN(ExchangeMessage,MSG_REQ_GT2GM_CANCELEXCHANGE)
{

}

DEF_MSG_ACK_DEFINE_FUN(ExchangeMessage,MSG_REQ_GT2GM_EXCHANGELOCK)
{
}

DEF_MSG_ACK_DEFINE_FUN(ExchangeMessage,MSG_REQ_GT2GM_VERIFYEXCHANGE)
{
}


