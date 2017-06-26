/*
 * RechargeMessage.cpp
 *
 *  Created on: 2015年6月18日
 *      Author: root
 */

#include "RechargeMessage.h"
#include "FileLoader/PayLoader.h"
#include "Log/Logger.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "CharDefine.h"
#include "../QuickMessage.h"
#include "../ServerEventDefine.h"
#include "./FileLoader/InternationalLoader.h"
#include "../Mail/MailMessage.h"
#include "MessageStruct/MSProtoc/MS_Recharge.pb.h"
#include "RechargeDefine.h"
#include "MessageStruct/Pay/Tencent.pb.h"
#include "MessageStruct/Pay/58Play.pb.h"
#include "MessageStruct/Pay/360Play.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/Pay/Connecting.pb.h"
#include "../StatisticMgr/StatisticHandle.h"

RechargeMessage::RechargeMessage()
{
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_SYSPAYINFO);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_SYSMARKETINFO);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_SYS58PLAYPAYINFO);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_SYS360PLAYPAYINFO);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_SYSCONNECTINGINFO);
}

RechargeMessage::~RechargeMessage()
{

}

void RechargeMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
		DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_SYSMARKETINFO);
	DEF_SWITCH_TRY_DISPATCH_END
}

void RechargeMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_SYSPAYINFO);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_SYS58PLAYPAYINFO);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_SYS360PLAYPAYINFO);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_SYSCONNECTINGINFO);
	DEF_SWITCH_TRY_DISPATCH_END
}



void RechargeMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_REQUEST_DEFINE_FUN(RechargeMessage, MSG_REQ_WS2GM_SYSPAYINFO)
{
	TencentInfo::SysPayInfo req;
	ServerReturn::ServerRetInt ret;
	RePriceType  rePriceType;
	vector<int> vecType;
	char *con;
	int len = 0;
	int nGolden = 0;
	Smart_Ptr<Player> player;

	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);
	//printf("MSG_SIM_WS2GM_SYSPAYINFO -------------------------------   req.itemid() = %d   req.num() = %d \n" ,req.itemid(),req.num());
	CPayLoader::GetInstance()->GetRePriceType(req.itemid(),eTencent,rePriceType);
	//printf("MSG_SIM_WS2GM_SYSPAYINFO ------------------rePriceType-------------   rePriceType.nNum = %d \n", rePriceType.nNum);
	nGolden = req.num() * rePriceType.nNum;

	PlayerMessage::GetInstance()->GetPlayerByCharId(player,req.charid());
	if(player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eRecharge, BehaviorType::Minor::eRecharge_TencentRecharge, MONITOR);

		//printf("MSG_SIM_WS2GM_SYSPAYINFO ------------------rePriceType------------- \n");
		player->ChangeGolden(nGolden);

		player->SynchRechargeToWS(nGolden);

		if(player->GetVipPart())
		{
			bool firstRec = false;
			player->GetVipPart()->AddUpGold(nGolden);

			//暂时把加元宝 设置充值次数
			player->GetCounterService()->IncreasePlayerCounter(CHARGE_RMB_COUNT, 1);
			// 是否是首充
			if (!player->GetSignIn()->IsGetFirstAward() &&
					1 == player->GetCounterService()->GetNumById(CHARGE_RMB_COUNT))
			{
				player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_FirstAward, eUIHLSmall_FirstAward_FirstRecharge), 1, true);
				firstRec = true;
			}
			BEHAVIOR_HANDLE_DO(player, BehaviorHandType::RechargeRes(req.realmoney(), nGolden, firstRec,""));
		}

		vecType.push_back(eCharGolden);
		player->SynCharAttribute(vecType);

//		if (player && !player->GetSignIn()->IsGetFirstAward() && 1 == player->GetCounterService()->GetNumById(CHARGE_RMB_COUNT))
//		{
//			ret.set_ret(1);
//		}
//		else
//		{
//			ret.set_ret(0);
//		}

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_SYSPAYINFO,message,&ret);
		Message_Facade::Send(messRet);

		
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(RechargeMessage, MSG_SIM_WS2GM_SYSMARKETINFO)
{
	TencentInfo::SysMarketInfo req;
	vector<int> vecLock;
	vector<int64> goods;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;

	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);
	PlayerMessage::GetInstance()->GetPlayerByCharId(player,req.charid());
	if(player)
	{
		const GoodsInfo *ginfo = GoodsLoader::GetInstance()->GetItemDataByID(req.itemid());
		if(!ginfo)
		{
			return;
		}

		if(player->GetContainer(ePackageType)->IsBagEnought(ginfo->itemID, req.num()) == 0)
		{
			player->GetContainer(ePackageType)->AddItem(ginfo->itemID, req.num(),vecLock,0,ginfo->equipStrengthLv,true);
		}
		else
		{
			int64 info = req.num();
			info = info << 32;	//物品数量
			DWORD itemid =ginfo->itemID;
			info  = info | itemid;
			goods.push_back(info);

			if(goods.size() > 0)
			{
				string warning = InternationalLoader::GetInstance()->GetInterStr(5);
				string warning1 = InternationalLoader::GetInstance()->GetInterStr(6);
				MailMessageMgr::GetInstance()->SendSysMail(goods, player->GetName(),0,warning.c_str(),warning1.c_str());
			}
		}
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(RechargeMessage, MSG_REQ_WS2GM_SYS58PLAYPAYINFO)
{
	LOG_DEBUG(FILEINFO,"MSG_REQ_WS2GM_SYS58PLAYPAYINFO ----------------------------------------------begin");
	Pay58PlayInfo::Sys58PlayPayInfo req;
	vector<int> vecType;
	char *con;
	int len = 0;
	int nGolden = 0;
	Smart_Ptr<Player> player;
	int ret = 0;

	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);
	nGolden = req.money() * CPayLoader::GetInstance()->Get58PlayRate();
	LOG_DEBUG(FILEINFO,"req.money() = %d * CPayLoader::GetInstance()->Get58PlayRate() = %d " ,req.money() , CPayLoader::GetInstance()->Get58PlayRate() );
	PlayerMessage::GetInstance()->GetPlayerByCharId(player,req.charid());
	if(player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eRecharge, BehaviorType::Minor::eRecharge_58Recharge, MONITOR);

		player->ChangeGolden(nGolden);

		LOG_DEBUG(FILEINFO,"player name = '%s' golden = %d  orderid = '%s' success" ,player->GetName().c_str(),nGolden,req.orderid().c_str());
		player->SynchRechargeToWS(nGolden);

		bool firstRec = false;

		if(player->GetVipPart())
		{
			player->GetVipPart()->AddUpGold(nGolden);

			//暂时把加元宝 设置充值次数
			player->GetCounterService()->IncreasePlayerCounter(CHARGE_RMB_COUNT, 1);
			// 是否是首充
			if (!player->GetSignIn()->IsGetFirstAward() &&
					1 == player->GetCounterService()->GetNumById(CHARGE_RMB_COUNT))
			{
				player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_FirstAward, eUIHLSmall_FirstAward_FirstRecharge), 1, true);
				firstRec = true;
			}

			BEHAVIOR_HANDLE_DO(player, BehaviorHandType::RechargeRes(req.money(), nGolden, firstRec,req.orderid()));
		}
		vecType.push_back(eCharGolden);
		player->SynCharAttribute(vecType);

		
	}
	else
	{
		ret = eNotHaveOnline;
	}

//	StatisticInfo::RechargeProcess process;
//	process.set_orderid(req.orderid());
//	process.set_time(CUtil::GetNowSecond());
//	process.set_process(eWSToGS);
//	if(0 != ret)
//		process.set_issuccess(false);

//	if(eAccountType_User == player->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(req.charid(),
//																	eStatic_Recharge,
//																	eStaMinor_Recharge_Process,
//																	&process);

//	if (player && !player->GetSignIn()->IsGetFirstAward() && 1 == player->GetCounterService()->GetNumById(CHARGE_RMB_COUNT))
//	{
//		req.set_flag(1);
//	}
//	else
//	{
//		req.set_flag(0);
//	}

	req.set_ret(ret);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_SYS58PLAYPAYINFO,message,&req);
	Message_Facade::Send(messRet);

	LOG_DEBUG(FILEINFO,"MSG_REQ_WS2GM_SYS58PLAYPAYINFO ----------------------------------------------end");
}

DEF_MSG_REQUEST_DEFINE_FUN(RechargeMessage, MSG_REQ_WS2GM_SYS360PLAYPAYINFO)
{
	LOG_DEBUG(FILEINFO,"MSG_REQ_WS2GM_SYS360PLAYPAYINFO ----------------------------------------------begin");
	Pay360PlayInfo::Sys360PlayPayInfo req;
	vector<int> vecType;
	char *con;
	int len = 0;
	int nGolden = 0;
	Smart_Ptr<Player> player;
	int ret = 0;

	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);
	nGolden = req.coins();
	LOG_DEBUG(FILEINFO,"req.money() = %d " ,req.moneys());
	PlayerMessage::GetInstance()->GetPlayerByCharId(player,req.charid());
	if(player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eRecharge, BehaviorType::Minor::eRecharge_360Recharge, MONITOR);

		player->ChangeGolden(nGolden);

		LOG_DEBUG(FILEINFO,"player name = '%s' golden = %d  orderid = '%s' success" ,player->GetName().c_str(),nGolden,req.orderid().c_str());
		player->SynchRechargeToWS(nGolden);

		if(player->GetVipPart())
		{
			bool firstRec = false;
			player->GetVipPart()->AddUpGold(nGolden);

			//暂时把加元宝 设置充值次数
			player->GetCounterService()->IncreasePlayerCounter(CHARGE_RMB_COUNT, 1);
			// 是否是首充
			if (!player->GetSignIn()->IsGetFirstAward() &&
					1 == player->GetCounterService()->GetNumById(CHARGE_RMB_COUNT))
			{
				player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_FirstAward, eUIHLSmall_FirstAward_FirstRecharge), 1, true);
				firstRec = true;
			}

			BEHAVIOR_HANDLE_DO(player, BehaviorHandType::RechargeRes(req.moneys(), nGolden, firstRec,req.orderid()));

		}
		vecType.push_back(eCharGolden);
		player->SynCharAttribute(vecType);

		
	}
	else
	{
		ret = eNotHaveOnline;
	}

//	StatisticInfo::RechargeProcess process;
//	process.set_orderid(req.orderid());
//	process.set_time(CUtil::GetNowSecond());
//	process.set_process(eWSToGS);
//	if(0 != ret)
//		process.set_issuccess(false);

//	if(eAccountType_User == player->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(req.charid(),
//																	eStatic_Recharge,
//																	eStaMinor_Recharge_Process,
//																	&process);

//	if (player && !player->GetSignIn()->IsGetFirstAward() && 1 == player->GetCounterService()->GetNumById(CHARGE_RMB_COUNT))
//	{
//		req.set_flag(1);
//	}
//	else
//	{
//		req.set_flag(0);
//	}

	req.set_ret(ret);
	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_SYS360PLAYPAYINFO,message,&req);
	Message_Facade::Send(messRet);

	LOG_DEBUG(FILEINFO,"MSG_REQ_WS2GM_SYS360PLAYPAYINFO ----------------------------------------------end");

}

DEF_MSG_REQUEST_DEFINE_FUN(RechargeMessage, MSG_REQ_WS2GM_SYSCONNECTINGINFO)
{
	LOG_DEBUG(FILEINFO,"MSG_REQ_WS2GM_SYSCONNECTINGINFO ----------------------------------------------begin");
	ConnectingInfo::SysConnectingPayInfo req;
	vector<int> vecType;
	char *con;
	int len = 0;
	int nGolden = 0;
	Smart_Ptr<Player> player;
	int ret = 0;

	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);
	nGolden = req.money() * CPayLoader::GetInstance()->GetConnectingRate();
	LOG_DEBUG(FILEINFO,"req.money() = %d * CPayLoader::GetInstance()->GetConnectingRate() = %d " ,req.money() , CPayLoader::GetInstance()->GetConnectingRate());
	PlayerMessage::GetInstance()->GetPlayerByCharId(player,req.charid());
	if(player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eRecharge, BehaviorType::Minor::eRecharge_ConnRecharge, MONITOR);

		player->ChangeGolden(nGolden);

		LOG_DEBUG(FILEINFO,"player name = '%s' golden = %d  orderid = '%s' success" ,player->GetName().c_str(),nGolden,req.orderid().c_str());
		player->SynchRechargeToWS(nGolden);

		if(player->GetVipPart())
		{
			bool firstRec = false;
			player->GetVipPart()->AddUpGold(nGolden);

			//暂时把加元宝 设置充值次数
			player->GetCounterService()->IncreasePlayerCounter(CHARGE_RMB_COUNT, 1);
			// 是否是首充
			if (!player->GetSignIn()->IsGetFirstAward() &&
					1 == player->GetCounterService()->GetNumById(CHARGE_RMB_COUNT))
			{
				player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_FirstAward, eUIHLSmall_FirstAward_FirstRecharge), 1, true);
				firstRec = true;
			}

			BEHAVIOR_HANDLE_DO(player, BehaviorHandType::RechargeRes(req.money(), nGolden, firstRec,req.orderid()));

		}
		vecType.push_back(eCharGolden);
		player->SynCharAttribute(vecType);

		
	}
	else
	{
		ret = eNotHaveOnline;
	}

//	StatisticInfo::RechargeProcess process;
//	process.set_orderid(req.orderid());
//	process.set_time(CUtil::GetNowSecond());
//	process.set_process(eWSToGS);
//	if(0 != ret)
//		process.set_issuccess(false);

//	if(eAccountType_User == player->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(req.charid(),
//																	eStatic_Recharge,
//																	eStaMinor_Recharge_Process,
//																	&process);

	if (player && !player->GetSignIn()->IsGetFirstAward() && 1 == player->GetCounterService()->GetNumById(CHARGE_RMB_COUNT))
	{
		req.set_flag(1);
	}
	else
	{
		req.set_flag(0);
	}

	req.set_ret(ret);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_SYSCONNECTINGINFO,message,&req);
	Message_Facade::Send(messRet);

	LOG_DEBUG(FILEINFO,"MSG_REQ_WS2GM_SYSCONNECTINGINFO ----------------------------------------------end");
}
