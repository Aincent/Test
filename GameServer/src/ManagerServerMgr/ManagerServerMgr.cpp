/*
 * ManagerServerMgr.cpp
 *
 *  Created on: 2015年2月5日
 *      Author: root
 */
#include "ManagerServerMgr.h"
#include "Log/Logger.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "SvrConfig.h"
#include "MessageCommonRet.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Object/Player/Player.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "MessageStruct/StatisticInfo.pb.h"
#include "../AntiAddictionSystem/AntiAddictionSystemMgr.h"
#include "MessageStruct/MSProtoc/OpenIDSomething.pb.h"
#include "MessageStruct/MSProtoc/MS_ExchangeGift.pb.h"
#include "MessageStruct/MSProtoc/MS_Recharge.pb.h"
#include "CharDefine.h"
#include "FileLoader/GameParaLoader.h"
#include "../StatisticMgr/StatisticHandle.h"

ManagerMessage::ManagerMessage()
{
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_FORBIDDEN);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_RECHARGE);
	//DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_EXCHANGE_GIFT);
	DEF_MSG_QUICK_SIMPLE_REG_FUN(ManagerMessage, MSG_SIM_C2GM_EXCHANGE_GIFT);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_EXCHANGE_GIFT);

	InitRegex();
}

ManagerMessage::~ManagerMessage()
{
	regfree(&reg);
}

void ManagerMessage::InitRegex(void)
{
	const char * reqStr = "(^([[:digit:]|[:alpha:]]{30})$)";
	regcomp(&reg,reqStr,REG_EXTENDED | REG_NEWLINE); //从字符串来编译这个表达式。
}

void ManagerMessage::Handle_Request(Safe_Smart_Ptr<Message>& message)
{
		DEF_SWITCH_TRY_DISPATCH_BEGIN
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_FORBIDDEN);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_RECHARGE);
//		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_EXCHANGE_GIFT);
		DEF_SWITCH_TRY_DISPATCH_END
}

void ManagerMessage::Handle_Message(Safe_Smart_Ptr<Message>& message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_EXCHANGE_GIFT);
	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_SIMPLE_DEFINE_FUN(ManagerMessage, MSG_SIM_WS2GM_EXCHANGE_GIFT)
{
	MS_ExchangeGift::Admin_ExchangeGiftInfo req;
	MS_ExchangeGift::Admin_ExchangeGiftRet toClient;
	char *con;
	int len = 0;
	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);

	Smart_Ptr<Player> player;

	PlayerMessage::GetInstance()->GetPlayerByCharid(req.charid(), player);
	if(!player)
	{
		LOG_DEBUG(FILEINFO, "ManagerMessage exchange gift but Player is NULL");
		return;
	}

	int64 charid = player->GetID();
	toClient.set_ret(req.ret());

	BEHAVIOR_MONI(player, BehaviorType::Main::eCDKey, BehaviorType::Minor::eCDKey_Exchange, MONITOR);

	const MS_MailInfo::MSSimDBSendMail& mail = req.mail();
	for(int i = 0;i < mail.itemlist().itemlist_size();++i)
	{
		BEHAVIOR_HANDLE_DO(player, BehaviorHandType::AddItem(mail.itemlist().itemlist(i).itemid(), mail.itemlist().itemlist(i).num()));
	}

	for(int i=0; i<mail.goods_size(); ++i)
	{
		BEHAVIOR_HANDLE_DO(player, BehaviorHandType::AddItem(mail.goods(i).id(), mail.goods(i).num()));
	}

	

	QuickMessage::GetInstance()->AddSendMessage(charid, player->GetChannelID(), &toClient, MSG_SIM_GM2C_EXCHANGE_GIFT);
}

DEF_MSG_REQUEST_DEFINE_FUN(ManagerMessage, MSG_REQ_WS2GM_FORBIDDEN)
{
	MS_OpenIDSomething::Aimin_Forbidden meContent;
	ServerReturn::ServerRetInt toWs;
	char *con ;
	int len = 0;
	Smart_Ptr<Player> player;
	int ret = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "worldserver request forbidden op");

	PlayerMessage::GetInstance()->GetPlayerByCharid(meContent.charid(), player);
	if((bool)player)
	{
		int type = meContent.type();
		int op = meContent.op();
		if(1 == type)//防沉迷类
		{
			int64 antiTime = 0;
			if(2 == op)//设置成防沉迷状态
				antiTime = GameParaLoader::GetInstance()->getAntiAddictionTiredTime();
			player->GetAntiAddictionSystemMgr()->SetAntiAddictionTime(antiTime);

			vector<int> attrvec;
			attrvec.push_back(eAntiTime);
			player->SynCharAttribute(attrvec);
		}
	}
	else
		ret = eNotHaveOnline;

	toWs.set_ret(ret);

	Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_REQ_WS2GM_FORBIDDEN, message, &toWs);
	Message_Facade::Send(clientRet);
}


DEF_MSG_REQUEST_DEFINE_FUN(ManagerMessage, MSG_REQ_WS2GM_RECHARGE)
{
	MS_Recharge::Admin_Recharge meContent;
	MS_Recharge::Admin_RetRecharge toWs;
	char *con ;
	int len = 0;
	Smart_Ptr<Player> player;
	int ret = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "worldserver request recharge");

	PlayerMessage::GetInstance()->GetPlayerByCharid(meContent.charid(), player);
	if ((bool)player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eRecharge, BehaviorType::Minor::eRecharge_Recharge, MONITOR);

		int golden = meContent.gold();
		player->ChangeGolden(golden,true);

		if(player->GetVipPart())
		{
			player->GetVipPart()->AddUpGold(golden);
			//暂时把加元宝 设置充值次数
			player->GetCounterService()->IncreasePlayerCounter(CHARGE_RMB_COUNT, 1);
		}
		// 是否是首充
		if (!player->GetSignIn()->IsGetFirstAward() &&
				1 == player->GetCounterService()->GetNumById(CHARGE_RMB_COUNT))
		{
			player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_FirstAward, eUIHLSmall_FirstAward_FirstRecharge), 1, true);
		}


		int isFirstRecharge = (1 == player->GetCounterService()->GetNumById(CHARGE_RMB_COUNT))?1:0;
		meContent.set_isfirstrecharge(isFirstRecharge);

		BEHAVIOR_HANDLE_DO(player, BehaviorHandType::RechargeRes(meContent.money(), golden, isFirstRecharge,meContent.orderid()));

		MS_Recharge::Admin_Recharge * tempRechargeInfo = toWs.mutable_rechargeinfo();
		tempRechargeInfo->CopyFrom(meContent);

		vector<int> attrvec;
		attrvec.push_back(eCharGolden);
		player->SynCharAttribute(attrvec);

		meContent.set_source(player->Get58PlaySouce());

		
//		if(eAccountType_User == player->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(meContent.charid(),eStatic_Recharge,eStaMinor_Recharge_Result,&meContent);
	}
	else
		ret = eNotHaveOnline;

	toWs.set_ret(ret);

//	StatisticInfo::RechargeProcess process;
//	process.set_orderid(meContent.orderid());
//	process.set_time(CUtil::GetNowSecond());
//	process.set_process(eWSToGS);
//	if(0 != ret)
//		process.set_issuccess(false);

//	if(eAccountType_User == player->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(meContent.charid(),
//																	eStatic_Recharge,
//																	eStaMinor_Recharge_Process,
//																	&process);

	Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_REQ_WS2GM_RECHARGE, message, &toWs);
	Message_Facade::Send(clientRet);
}


DEF_MSG_QUICK_SIMPLE_DEFINE_FUN(ManagerMessage,MSG_SIM_C2GM_EXCHANGE_GIFT)
{
	MS_ExchangeGift::Admin_GiftCode req;

	req.ParseFromString(content);

	if(!player)
	{
		LOG_DEBUG(FILEINFO, "ManagerMessage exchange gift but Player is NULL");
		return;
	}

	int64 charid = player->GetID();

	string code = req.code();

	int ret = regexec(&reg,code.c_str(),0,NULL,0); //匹配字符串
	if(ret != 0)
	{
		MS_ExchangeGift::Admin_ExchangeGiftRet ret;
		ret.set_ret(eExchangeCodeError);

		QuickMessage::GetInstance()->AddSendMessage(charid, message->GetChannelID(), &ret, MSG_SIM_GM2C_EXCHANGE_GIFT);

		return;
	}

	req.set_charid(charid);

	req.set_name(player->GetName());

	Smart_Ptr<SvrItem> managerItem = ServerConHandler::GetInstance()->GetManagerServerItem();
	if((bool) managerItem)
	{
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(charid,eStatic_ExchangeCode,eStaMinor_ExchangeCode_Exchange,&req);

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2MS_EXCHANGE_GIFT,&req,managerItem.Get(), SimpleMessage);
		Message_Facade::Send(messRet);
	}
}






