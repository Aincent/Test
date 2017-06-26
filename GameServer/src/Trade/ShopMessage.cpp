/*
 * ShopMessage.cpp
 *
 *      Author: cz
 */

#include "ShopMessage.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/CharLogin/NpcTrade.pb.h"
#include "shop.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Map/GameZone.h"
#include "../Map/SceneManager.h"
#include "../Object/FileLoader/Npcshop.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "../Object/FileLoader/NPCLoader.h"
#include "MessageCommonRet.h"
#include "../StatisticMgr/StatisticHandle.h"

ShopMessage * ShopMessage::m_instance = 0;


ShopMessage::ShopMessage()
{
	//
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_BUYNPCITEM);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_SELLITEM);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_BUYBACK);


	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GT2GM_BUYNPCITEM);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GT2GM_SELLITEM);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GT2GM_BUYBACK);

}

ShopMessage::~ShopMessage()
{

}

void ShopMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN


	DEF_SWITCH_TRY_DISPATCH_END
}

void ShopMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_BUYNPCITEM);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_SELLITEM);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_BUYBACK);

	DEF_SWITCH_TRY_DISPATCH_END

}

void ShopMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_BUYNPCITEM);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_SELLITEM);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_BUYBACK);

	DEF_SWITCH_TRY_DISPATCH_END
}
#include "StatisticBase/BehaviorHandle/BehaviorHandle.h"
DEF_MSG_REQUEST_DEFINE_FUN(ShopMessage, MSG_REQ_GT2GM_BUYNPCITEM)
{
//	NpcTrade::GTtoGMReqBuyItem reqGM;
	NpcTrade::ClientReqBuyItem reqGM;
	ServerReturn::ServerRetInt ack;
	Smart_Ptr<Player> player;
	int len = 0;
	char *str = message->GetBuffer(len);

	reqGM.ParseFromArray(str, len);
	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{
		NpcShop *pShop = NpcShop::GetInstance();
		int npcid = reqGM.npcid();
		if(pShop)
		{
			sNpcShop *items = pShop->GetNpcShopInfo(npcid);
			if(!items)
			{
				return;
			}

			BEHAVIOR_MONI(player, BehaviorType::Main::eNpcShop, BehaviorType::Minor::eNpcShop_BuyItem, MONITOR);;

			for(size_t i = 0; i < items->ShopItems.size(); i++)
			{
				if(items->ShopItems[i] == reqGM.itemid())
				{
					int	errType = ShopMgr::GetInstance()->BuyItem(player,npcid,reqGM.itemid(),reqGM.itemnum());
					ack.set_ret(errType);
					Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_BUYNPCITEM, message, &ack);
					Message_Facade::Send(messRet);
					return;
				}

			}

			

			LOG_ERROR(FILEINFO, "client Buy Item From Npc but item ID is not existed");
		}

	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request Buy Item From Npc but player[gs charid=%lld] is not exist",GET_PLAYER_CHARID(charID));
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(ShopMessage, MSG_REQ_GT2GM_SELLITEM)
{

//	NpcTrade::GTtoGMReqSellItem reqGM;
	NpcTrade::ClientReqSellItem reqGM;
	NpcTrade::GTAckToCSellItem ack;

	Smart_Ptr<Player> player;
	int len = 0;
	char *str = message->GetBuffer(len);

	reqGM.ParseFromArray(str, len);
	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eNpcShop, BehaviorType::Minor::eNpcShop_SaleItem, MONITOR);

		NpcShop *pShop = NpcShop::GetInstance();
		if(pShop)
		{
			Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(player->GetMapID());
			if(!zone)
			{
				LOG_ERROR(FILEINFO, "client Buy Item From Npc but scene is not existed");
			}
			CharLogin::ClientItemInfo info;
			int errType = ShopMgr::GetInstance()->SellItem(player,reqGM.packetgridid(),&info);
			ack.set_statusid(errType);
			CharLogin::ClientItemInfo *tempInfo = ack.mutable_iteminfo();
			tempInfo->CopyFrom(info);
			Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_SELLITEM, message, &ack);
			Message_Facade::Send(messRet);
		}
		
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request Buy Item From Npc but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(ShopMessage, MSG_REQ_GT2GM_BUYBACK)
{
//	NpcTrade::GTtoGMReqBuyBack reqGM;
	NpcTrade::ClientReqBuyBack reqGM;;
	ServerReturn::ServerRetInt ack;

	Smart_Ptr<Player> player;
	int len = 0;
	char *str = message->GetBuffer(len);
	reqGM.ParseFromArray(str, len);
	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{
		Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(player->GetMapID());
		if(!zone)
		{
			LOG_ERROR(FILEINFO, "client Buy Item From Npc but scene is not existed");
		}
		int errType = ShopMgr::GetInstance()->BuyBackItem(player,reqGM.buybackindex());
		int64 retVal = (((int64)reqGM.buybackindex())<<32) | errType;
		ack.set_ret(retVal);
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_BUYBACK, message, &ack);
		Message_Facade::Send(messRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request Buy Item From Npc but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
	}
}

DEF_MSG_ACK_DEFINE_FUN(ShopMessage,MSG_REQ_GT2GM_BUYBACK)
{

}

DEF_MSG_ACK_DEFINE_FUN(ShopMessage,MSG_REQ_GT2GM_SELLITEM)
{
}

DEF_MSG_ACK_DEFINE_FUN(ShopMessage,MSG_REQ_GT2GM_BUYNPCITEM)
{
}

int ShopMessage::IsCanNPChandle(Smart_Ptr<Player>& player, int npcid)
{
//	if(!(player->GetVipPart()->HasRight( eVipPortableShop)))
//	{
		if(NPCInfo::GetInstance()->IsExistedNpc(npcid))
		{
			int funEnum = NPCInfo::GetInstance()->GetNPCFunEnum(npcid);
			if(eNPC_FUN_SHOP != funEnum)
				return eRetNpcNotThisFun;
			else
				return NPCInfo::GetInstance()->ExamineDistance(npcid, player->GetXPos(), player->GetYPos());
		}
		else
		{
			if(npcid == 0)
			{
				return 0;
			}
			else
				return eRetNpcNotExist;
		}
//	}
	return 0;
}

