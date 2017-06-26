/*
 * FashionMessage.cpp
 *
 *  Created on: 2015年3月2日
 *      Author: root
 */

#include "FashionMessage.h"

FashionMessage::FashionMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(FashionMessage, MSG_REQ_C2GM_FASHIONINFO);
	DEF_MSG_QUICK_REQUEST_REG_FUN(FashionMessage, MSG_REQ_C2GM_ACTIVATEFASHION);
	DEF_MSG_QUICK_REQUEST_REG_FUN(FashionMessage, MSG_REQ_C2GM_WEARFASHION);
	DEF_MSG_QUICK_REQUEST_REG_FUN(FashionMessage, MSG_REQ_C2GM_STRENTHFASHION);
	DEF_MSG_QUICK_REQUEST_REG_FUN(FashionMessage, MSG_REQ_C2GM_SHOWHIDEFASHION);
}

FashionMessage::~FashionMessage()
{
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FashionMessage, MSG_REQ_C2GM_FASHIONINFO)
{
	FashionMsgProto::ReqFashionInfo req;
	FashionMsgProto::RetFashionInfo ack;

	req.ParseFromString(content);

	//cout<<endl<<"req.ret():"<<req.ret()<<endl;
//	if(player && player->IsOpenSystem(eStFashion))
//	{
//		player->GetFashionMgr()->Show(ack);
//	}
//	else
//		return;

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_FASHIONINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FashionMessage, MSG_REQ_C2GM_ACTIVATEFASHION)
{
	//FashionMsgProto::ReqActivateFashion req;
	FashionMsgProto::ReqActivateFashion req;
	FashionMsgProto::RetActivateFashion ack;

	req.ParseFromString(content);

//	if(player && player->IsOpenSystem(eStFashion))
	{
		int err = player->GetFashionMgr()->Activate(req.fashionid(), *(ack.mutable_newfashion()));
		if (err == -1)
		{
			return;
		}

		ack.set_ret(err);
	}
//	else
//	{
//		ack.set_ret(eNotOpen);
//	}

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_ACTIVATEFASHION);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FashionMessage, MSG_REQ_C2GM_WEARFASHION)
{

	FashionMsgProto::ReqWearFashion req;
	FashionMsgProto::RetWearFashion ack;

	req.ParseFromString(content);

//	if(player && player->IsOpenSystem(eStFashion))
	{
//		ack.set_fashionid(req.fashionid());
		int err = player->GetFashionMgr()->Dressup(req.fashionid(), (Fashion::E_FASHION_STATE)req.flag(), *(ack.mutable_fashion()));
		if (-1 == err)
		{
			return;
		}

		ack.set_ret(err);
	}
//	else
//	{
//		ack.set_ret(eNotOpen);
//	}
	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_WEARFASHION);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FashionMessage, MSG_REQ_C2GM_STRENTHFASHION)
{
	FashionMsgProto::ReqStrengthFashion req;
	FashionMsgProto::RetStrengthFashion ack;

	req.ParseFromString(content);
//	if(player && player->IsOpenSystem(eStFashion))
	{
		int err = player->GetFashionMgr()->Strength(req.fashionid(), *(ack.mutable_fashion()));
		if (-1 == err)
		{
			return;
		}

		ack.set_ret(err);
	}
//	else
//	{
//		ack.set_ret(eNotOpen);
//	}

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_STRENTHFASHION);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FashionMessage, MSG_REQ_C2GM_SHOWHIDEFASHION)
{
	FashionMsgProto::ReqFashionShowState req;
	ServerReturn::ServerRetInt ack;
	req.ParseFromString(content);

//	if (player && player->IsOpenSystem(eStFashion))
	{
		int err = player->GetFashionMgr()->Display(req.fashionid(), (Fashion::E_FASHION_STATE)(req.showstate()));
		if (0 != err)
		{
			if (-1 == err)
			{
				return;
			}
		}

		ack.set_ret(err);
	}
//	else
//	{
//		ack.set_ret(eNotOpen);
//	}

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_STRENTHFASHION);
}
