/*
 * BaseShopMessage.cpp
 *
 *  Created on: 2016年11月7日
 *      Author: root
 */

#include "BaseShopMessage.h"
#include "MessageStruct/BaseShop/BaseShop.pb.h"
#include "BaseShop.h"

BaseShopMessage::BaseShopMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(BaseShopMessage, MSG_REQ_C2GM_BASESHOP_BUY); //请求商店购物
	DEF_MSG_QUICK_REQUEST_REG_FUN(BaseShopMessage, MSG_REQ_C2GM_BASESHOP_MYSTER); // 玩家请求神秘商店数据
	DEF_MSG_QUICK_REQUEST_REG_FUN(BaseShopMessage, MSG_REQ_C2GM_BASESHOP_FLUSH_MYSTER); // 玩家请求刷新神秘商店数据
	DEF_MSG_QUICK_REQUEST_REG_FUN(BaseShopMessage, MSG_REQ_C2GM_BASESHOP_INFO); //玩家请求商店数据
}

BaseShopMessage::~BaseShopMessage()
{

}

void BaseShopMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN


	DEF_SWITCH_TRY_DISPATCH_END
}

void BaseShopMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_SWITCH_TRY_DISPATCH_END
}

void BaseShopMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(BaseShopMessage, MSG_REQ_C2GM_BASESHOP_BUY)
{
	BaseShopInfo::ClientReqBuyItem req;
	BaseShopInfo::AckBuyItem toClient;

	req.ParseFromString(content);

	if(player->GetBaseShopMgr())
	{
		player->GetBaseShopMgr()->BuyItem(req,toClient);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_BASESHOP_BUY);
}


DEF_MSG_QUICK_REQUEST_DEFINE_FUN(BaseShopMessage, MSG_REQ_C2GM_BASESHOP_MYSTER)
{
	BaseShopInfo::AckMysterInfo toClient;

	if(player->GetBaseShopMgr())
	{
		player->GetBaseShopMgr()->GetMyster(toClient);
	}

	printf("AckMysterInfo....................%d \n",toClient.id_size());
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_BASESHOP_MYSTER);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(BaseShopMessage, MSG_REQ_C2GM_BASESHOP_FLUSH_MYSTER)
{
	BaseShopInfo::AckFlushMysterInfo toClient;

	if(player->GetBaseShopMgr())
	{
		player->GetBaseShopMgr()->GetFlushMyster(toClient);
	}

	printf("AckFlushMysterInfo....................%d \n",toClient.id_size());
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_BASESHOP_FLUSH_MYSTER);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(BaseShopMessage, MSG_REQ_C2GM_BASESHOP_INFO)
{
	BaseShopInfo::ClientReqBaseShopInfo req;
	BaseShopInfo::AckBaseShopInfo toClient;

	req.ParseFromString(content);

	if(player->GetBaseShopMgr())
	{
		player->GetBaseShopMgr()->GetBaseShopInfo(req.type(),toClient);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_BASESHOP_INFO);
}
