/*
 * GoodsManager.cpp
 *
 *      Author: root
 */

#include "GoodsManager.h"
#include "SvrConfig.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/CharLogin/GoodsMessage.pb.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageStruct/CharLogin/Smith.pb.h"
#include "MessageStruct/CharLogin/EquipMessage.pb.h"

GoodsManager * GoodsManager::m_instance = 0;

GoodsManager::GoodsManager()
{
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_C2CT_SORTBACKPACK);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_GM2GT_SAVEGOODS);
	DEF_MSG_SIMPLE_REG_FUN(eGateServer, MSG_SIM_GT2GM_ADDBACKPACKSIZE);

	//移动物品
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2CT_MOVEGOODS);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2CT_USEGGOODS);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2CT_DISCARDGOODS);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2CT_ADDBACKPACKSIZE);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2CT_MOVEQUICKSECTOR);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_STONEDAADEL);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_STONECREATE);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_WEAREQUIP);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_STRENGTHEQUIP);
	DEF_MSG_REQUEST_REG_FUN(eGateServer,MSG_REQ_C2CT_REFINE);
	DEF_MSG_REQUEST_REG_FUN(eGateServer,MSG_REQ_C2CT_AFFIRMREFINE);
	DEF_MSG_REQUEST_REG_FUN(eGateServer, MSG_REQ_C2GT_SPLITGOODS);

	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_USEGOODS);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_ADDBACKPACKSIZE);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_MOVEQUICKSECTOR);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_STONEDAADEL);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_STONECREATE);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_WEAREQUIP);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_STRENGTHEQUIP);
	DEF_MSG_ACK_REG_FUN(eGateServer,MSG_REQ_GT2GM_REFINE);
	DEF_MSG_ACK_REG_FUN(eGateServer,MSG_REQ_GT2GM_AFFIRMREFINE);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_MOVEGOODS);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_SPLITGOODS);
	DEF_MSG_ACK_REG_FUN(eGateServer, MSG_REQ_GT2GM_DISCARDGOODS);
}

GoodsManager::~GoodsManager()
{

}

void GoodsManager::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_C2CT_SORTBACKPACK);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GM2GT_SAVEGOODS);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GT2GM_ADDBACKPACKSIZE);

	DEF_SWITCH_TRY_DISPATCH_END
}

void GoodsManager::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2CT_MOVEGOODS);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2CT_USEGGOODS);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2CT_DISCARDGOODS);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2CT_ADDBACKPACKSIZE);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2CT_MOVEQUICKSECTOR);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_STONEDAADEL);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_STONECREATE);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_WEAREQUIP);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_STRENGTHEQUIP);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2CT_REFINE);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2CT_AFFIRMREFINE);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_C2GT_SPLITGOODS);

	DEF_SWITCH_TRY_DISPATCH_END
}

void GoodsManager::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_USEGOODS);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_ADDBACKPACKSIZE);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_MOVEQUICKSECTOR);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_STONEDAADEL);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_STONECREATE);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_WEAREQUIP);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_STRENGTHEQUIP);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_REFINE);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_AFFIRMREFINE);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_MOVEGOODS);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_SPLITGOODS);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2GM_DISCARDGOODS);

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_REQUEST_DEFINE_FUN(GoodsManager, MSG_REQ_C2CT_MOVEGOODS)
{
	LOG_DEBUG(FILEINFO, "client request move container goods");

	int gsChannel = -1;
	int64 charid = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_MOVEGOODS, message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(GoodsManager, MSG_REQ_C2CT_USEGGOODS)
{
	LOG_DEBUG(FILEINFO, "client request use container goods");

	int gsChannel = -1;
	int64 charid = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_USEGOODS, message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(GoodsManager, MSG_REQ_C2CT_DISCARDGOODS)
{
	LOG_DEBUG(FILEINFO, "client request discard container goods");

	int gsChannel = -1;
	int64 charid = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_DISCARDGOODS, message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(GoodsManager, MSG_REQ_C2CT_ADDBACKPACKSIZE)
{
	LOG_DEBUG(FILEINFO, "client request add back pack size");

	int gsChannel = -1;
	int64 charid = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_ADDBACKPACKSIZE, message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(GoodsManager, MSG_REQ_C2CT_MOVEQUICKSECTOR)
{
	LOG_DEBUG(FILEINFO, "client request move quick sector");

	int gsChannel = -1;
	int64 charid = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_MOVEQUICKSECTOR, message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(GoodsManager, MSG_REQ_C2GT_STONEDAADEL)
{
	LOG_DEBUG(FILEINFO, "client request add or delete stone from equip");

	int64 charid = -1;
	int gsChannel = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_STONEDAADEL, message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(GoodsManager, MSG_REQ_C2GT_STONECREATE)
{
	LOG_DEBUG(FILEINFO, "client request create stone from equip");

	int gsChannel = -1;
	int64 charid = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_STONECREATE, message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(GoodsManager, MSG_REQ_C2GT_WEAREQUIP)
{
	LOG_DEBUG(FILEINFO, "client request wear equip");

	int gsChannel = -1;
	int64 charid = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_WEAREQUIP, message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(GoodsManager, MSG_REQ_C2GT_STRENGTHEQUIP)
{
	LOG_DEBUG(FILEINFO, "client request strength equip");

	int gsChannel = -1;
	int64 charid = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_STRENGTHEQUIP, message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(GoodsManager, MSG_REQ_GT2GM_USEGOODS)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver player use goods and ack timeout");

		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2CT_USEGGOODS, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(GoodsManager, MSG_REQ_GT2GM_MOVEGOODS)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver player move goods and ack timeout");

		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2CT_MOVEGOODS, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(GoodsManager, MSG_REQ_GT2GM_STONEDAADEL)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver to add or delete stone and ack timeout");

		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_STONEDAADEL, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}


DEF_MSG_ACK_DEFINE_FUN(GoodsManager, MSG_REQ_GT2GM_ADDBACKPACKSIZE)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver to add BACKPACKSIZE and ack timeout");

		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2CT_ADDBACKPACKSIZE, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(GoodsManager, MSG_SIM_GT2GM_ADDBACKPACKSIZE)
{
	LOG_DEBUG(FILEINFO, "[messageid = %d] gateserver get client", MSG_SIM_GT2GM_ADDBACKPACKSIZE);

	Goods::GMAckToCTOpenGoodsRet toGate;
	int len = 0;
	char *content = message->GetBuffer(len);
	int64 charid = 0;
	int channel = -1;

	toGate.ParseFromArray(content, len);

	Goods::GTAckToCOpenGoodsRet *toClient = toGate.mutable_ret();

	charid = toGate.charid();
	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(charid, channel))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_REQ_C2CT_ADDBACKPACKSIZE, toClient, channel);
		Message_Facade::Send(clientRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "gateserver get gameserver syn player attr change but find not client[ charid=%lld]",GET_PLAYER_CHARID(charid));
	}

}

DEF_MSG_ACK_DEFINE_FUN(GoodsManager, MSG_REQ_GT2GM_MOVEQUICKSECTOR)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request move quick sector and ack timeout");

		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2CT_MOVEQUICKSECTOR, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(GoodsManager, MSG_REQ_GT2GM_STONECREATE)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver to create stone and ack timeout");

		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_STONECREATE, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(GoodsManager, MSG_REQ_GT2GM_WEAREQUIP)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver player wear equip and ack timeout");

		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_WEAREQUIP, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(GoodsManager, MSG_REQ_GT2GM_STRENGTHEQUIP)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver to strength equip and ack timeout");

		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_STRENGTHEQUIP, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(GoodsManager, MSG_SIM_C2CT_SORTBACKPACK)
{
	LOG_DEBUG(FILEINFO, "[messageid = %d] gateserver get client", MSG_SIM_C2CT_SORTBACKPACK);

	int gsChannel = -1;
	int64 charid = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_SIM_GT2GM_SORTBACKPACK, message, gsChannel, SimpleMessage);
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(GoodsManager, MSG_SIM_GM2GT_SAVEGOODS)
{
	LOG_DEBUG(FILEINFO, "[messageid = %d] gateserver get add item from gameserver", MSG_SIM_GM2GT_SAVEGOODS);

	Goods::ToGTAddItem  clientReq;
	int channel = -1;
	int64 charid = -1;
	int len = 0;
	char *str = message->GetBuffer(len);

	clientReq.ParseFromArray(str, len);

	Goods::GTAckToCUpdateGoodsRet item = clientReq.item();

	if(ServerConHandler::GetInstance()->GetClientChannelByCharID(clientReq.charid(), channel))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_SIM_GT2C_SAVEGOODS, &item, channel, SimpleMessage);
		Message_Facade::Send(clientRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "gateserver get gameserver broadcast object action but find not client[ charid=%lld]",GET_PLAYER_CHARID(charid));
	}
}
//请求洗练装备
DEF_MSG_REQUEST_DEFINE_FUN(GoodsManager,MSG_REQ_C2CT_REFINE)
{
	int gsChannel = -1;
	int64 charid = -1;

	LOG_DEBUG(FILEINFO, "client request refine equip");

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_REFINE, message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}
//返回洗练属性
DEF_MSG_ACK_DEFINE_FUN(GoodsManager,MSG_REQ_GT2GM_REFINE)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver refine equip and ack timeout");
		return;
	}

	LOG_DEBUG(FILEINFO, "return client refine equip");

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2CT_REFINE, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}
//确认洗练装备
DEF_MSG_REQUEST_DEFINE_FUN(GoodsManager,MSG_REQ_C2CT_AFFIRMREFINE)
{
	LOG_DEBUG(FILEINFO, "client request affirm refine equip");

	int gsChannel = -1;
	int64 charid = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_AFFIRMREFINE, message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}
//确认洗练属性
DEF_MSG_ACK_DEFINE_FUN(GoodsManager,MSG_REQ_GT2GM_AFFIRMREFINE)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver affirmRefine equip and ack timeout");
		return;
	}

	LOG_DEBUG(FILEINFO, "return client affirmRefine equip");

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2CT_AFFIRMREFINE, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}


DEF_MSG_REQUEST_DEFINE_FUN(GoodsManager,MSG_REQ_C2GT_SPLITGOODS)
{
	LOG_DEBUG(FILEINFO, "client request split goods");

	int gsChannel = -1;
	int64 charid = -1;

	if(ServerConHandler::GetInstance()->GetGSChannelAndCharIDByChannel(message->GetChannelID(), gsChannel, charid))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2GM_SPLITGOODS, message, gsChannel, Request);
		messRet->SetAct(new requestAct(message));
		messRet->SetMessageTime(charid);
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(GoodsManager,MSG_REQ_GT2GM_SPLITGOODS)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver split goods and ack timeout");
		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2GT_SPLITGOODS, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}

DEF_MSG_ACK_DEFINE_FUN(GoodsManager,MSG_REQ_GT2GM_DISCARDGOODS)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gateserver request gameserver discard goods and ack timeout");
		return;
	}

	if ((bool)act && (bool)(static_cast<requestAct *>(act.Get())->mesReq))
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_C2CT_DISCARDGOODS, message, static_cast<requestAct *>(act.Get())->mesReq->GetChannelID());
		Message_Facade::Send(messRet);
	}
}
