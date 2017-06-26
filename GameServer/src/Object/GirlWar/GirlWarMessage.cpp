/*
 * GirlWarMessage.cpp
 *
 *  Created on: 2014年12月9日
 *      Author: root
 */

#include "GirlWarMessage.h"
#include "GirlWarManager.h"
#include "MessageStruct/GirlWar/GirlWarMessage.pb.h"
#include "../../StatisticMgr/StatisticHandle.h"

GirlWarMessage::GirlWarMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(GirlWarMessage,MSG_REQ_C2GM_SYNGIRLWARINFO);		//请求战姬信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(GirlWarMessage,MSG_REQ_C2GM_ASKGIRLWARUPLV);	//请求战姬升级
	DEF_MSG_QUICK_REQUEST_REG_FUN(GirlWarMessage,MSG_REQ_C2GM_ASKGIRLWARTOFIGHT);  //请求出战或休战
	DEF_MSG_QUICK_REQUEST_REG_FUN(GirlWarMessage,MSG_REQ_C2GM_ASKGIRLWARREVIVE);	//请求战姬复活
	DEF_MSG_QUICK_SIMPLE_REG_FUN(GirlWarMessage, MSG_SIM_C2GM_UPDATEGIRLWAR_UISTATE); // 更改战姬UI高亮状态
	DEF_MSG_QUICK_REQUEST_REG_FUN(GirlWarMessage,MSG_REQ_C2GM_ASKGIRLWAR_WAREEQUIP);		//请求战姬加成
	DEF_MSG_QUICK_REQUEST_REG_FUN(GirlWarMessage,MSG_REQ_C2GM_ASKGIRLWAR_ONEKEYWAREEQUIP);		//请求战姬一键加成
	DEF_MSG_QUICK_REQUEST_REG_FUN(GirlWarMessage,MSG_REQ_C2GM_ASKGIRLWAR_UPQUALITY);		//请求战姬进阶
	DEF_MSG_QUICK_REQUEST_REG_FUN(GirlWarMessage,MSG_REQ_C2GM_ASKGIRLWAR_UPSKILL);//请求战姬技能升级
	DEF_MSG_QUICK_REQUEST_REG_FUN(GirlWarMessage,MSG_REQ_C2GM_GIRLWARUPSTAR);//请求战姬升星
	DEF_MSG_QUICK_REQUEST_REG_FUN(GirlWarMessage,MSG_REQ_C2GM_GETGIRLWAR);//请求获取战姬
}

GirlWarMessage::~GirlWarMessage()
{

}

GirlWarMessage* GirlWarMessage::GetInstance()
{
	static GirlWarMessage m_Instance;
	return &m_Instance;
}

//请求战姬信息
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GirlWarMessage,MSG_REQ_C2GM_SYNGIRLWARINFO)
{
	GirlWarMessageInfo::ToClientGirlWarInfo toClient;

	if(player && player->IsOpenSystem(eStGirlWar))
	{
		Smart_Ptr<GirlWarManager> girlWarMgr = player->GetGirlWarManager();
		if(girlWarMgr)
		{
			girlWarMgr->GetAllGirlWarInfo(toClient);
		}
	}
	else
		return;

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_SYNGIRLWARINFO);
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_SYNGIRLWARINFO);
}

//请求战姬升级
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GirlWarMessage,MSG_REQ_C2GM_ASKGIRLWARUPLV)
{
	GirlWarMessageInfo::FromClentUplv fromClient;
	GirlWarMessageInfo::ToClentUplv toClient;

	fromClient.ParseFromString(content);

	Smart_Ptr<GirlWarManager> girlWarMgr = player->GetGirlWarManager();
	if(!girlWarMgr)
	{
		return;
	}

	if(player && player->IsOpenSystem(eStGirlWar))
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eGirlWar, BehaviorType::Minor::eGirlWar_Levelup, MONITOR);

		girlWarMgr->GirlWarUpLv(fromClient,toClient);

		
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ASKGIRLWARUPLV);
}

//请求出战或休战
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GirlWarMessage,MSG_REQ_C2GM_ASKGIRLWARTOFIGHT)
{
	GirlWarMessageInfo::FromClentFight fromClient;
	GirlWarMessageInfo::ToClentFight toClient;

	fromClient.ParseFromString(content);

	Smart_Ptr<GirlWarManager> girlWarMgr = player->GetGirlWarManager();
	if(!girlWarMgr)
	{
		return;
	}

	if(player->IsInCreatureState(ERS_Meditation))
	{
		toClient.set_ret(eMeditationGirlWarLimit);
		toClient.set_type(fromClient.ret());
		REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ASKGIRLWARTOFIGHT);
		return;
	}

	if(player && player->IsOpenSystem(eStGirlWar))
	{
		if(fromClient.ret() == 1)
		{
			girlWarMgr->ReseatGirlWar(fromClient.id(),toClient);
		}
		else
		{
			girlWarMgr->FightGirlWar(fromClient.id(),toClient);
		}
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}
	toClient.set_type(fromClient.ret());

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_ASKGIRLWARTOFIGHT);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ASKGIRLWARTOFIGHT);
}

//请求战姬复活
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GirlWarMessage,MSG_REQ_C2GM_ASKGIRLWARREVIVE)
{
	GirlWarMessageInfo::FromClentRevive fromClient;
	GirlWarMessageInfo::ToClentFight toClient;

	fromClient.ParseFromString(content);

	Smart_Ptr<GirlWarManager> girlWarMgr = player->GetGirlWarManager();
	if(!girlWarMgr)
	{
		return;
	}
	if(player && player->IsOpenSystem(eStGirlWar))
	{
		girlWarMgr->ReviveGirlWar(fromClient.id(),toClient);
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_ASKGIRLWARREVIVE);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ASKGIRLWARREVIVE);
}

DEF_MSG_QUICK_SIMPLE_DEFINE_FUN(GirlWarMessage, MSG_SIM_C2GM_UPDATEGIRLWAR_UISTATE)
{
	if (!player)
	{
		return;
	}
	player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_GirlWar, eUIHLSmall_GirlWar_GetNew), 0);

}

//请求战姬加成
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GirlWarMessage,MSG_REQ_C2GM_ASKGIRLWAR_WAREEQUIP)
{
	GirlWarMessageInfo::GirlWarEquip req;
	GirlWarMessageInfo::RetGirlWarEquip toClient;
	int ret = 0;
	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eStGirlWar))
	{
		Smart_Ptr<GirlWarManager> girlWarMgr = player->GetGirlWarManager();
		if(!girlWarMgr)
		{
			return;
		}

		ret = girlWarMgr->WareEquip(req,toClient);
	}
	else
	{
		ret = eNotOpen;
	}

	toClient.set_ret(ret);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ASKGIRLWAR_WAREEQUIP);
}

//请求战姬一键加成
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GirlWarMessage,MSG_REQ_C2GM_ASKGIRLWAR_ONEKEYWAREEQUIP)
{
	GirlWarMessageInfo::GirlWarOneKeyEquip req;
	GirlWarMessageInfo::RetGirlWarOneKeyEquip toClient;
	int ret = 0;
	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eStGirlWar))
	{
		Smart_Ptr<GirlWarManager> girlWarMgr = player->GetGirlWarManager();
		if(!girlWarMgr)
		{
			return;
		}

		ret = girlWarMgr->OneKeyWareEquip(req.id(),toClient);
	}
	else
	{
		ret = eNotOpen;
	}

	toClient.set_ret(ret);
	toClient.set_id(req.id());

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ASKGIRLWAR_ONEKEYWAREEQUIP);
}

//请求战姬进阶
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GirlWarMessage,MSG_REQ_C2GM_ASKGIRLWAR_UPQUALITY)
{
	GirlWarMessageInfo::GirlWarUpQuality req;
	GirlWarMessageInfo::RetGirlWarUpQuality toClient;
	int ret = 0;
	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eStGirlWar))
	{
		Smart_Ptr<GirlWarManager> girlWarMgr = player->GetGirlWarManager();
		if(!girlWarMgr)
		{
			return;
		}

		BEHAVIOR_MONI(player, BehaviorType::Main::eGirlWar, BehaviorType::Minor::eGirlWar_Advance, MONITOR);

		ret = girlWarMgr->UpQuality(req.id(),toClient);

		
	}
	else
	{
		ret = eNotOpen;
	}

	toClient.set_ret(ret);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ASKGIRLWAR_UPQUALITY);
}

//请求战姬技能升级
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GirlWarMessage,MSG_REQ_C2GM_ASKGIRLWAR_UPSKILL)
{
	GirlWarMessageInfo::GirlWarUpSkill req;
	GirlWarMessageInfo::RetGirlWarUpSkill toClient;
	int ret = 0;
	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eStGirlWar))
	{
		Smart_Ptr<GirlWarManager> girlWarMgr = player->GetGirlWarManager();
		if(!girlWarMgr)
		{
			return;
		}
		BEHAVIOR_MONI(player, BehaviorType::Main::eGirlWar, BehaviorType::Minor::eGirlWar_UpSkill, MONITOR);

		ret = girlWarMgr->UpSkill(req,toClient);

		
	}
	else
	{
		ret = eNotOpen;
	}

	toClient.set_ret(ret);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ASKGIRLWAR_UPSKILL);
}

//请求战姬升星
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GirlWarMessage,MSG_REQ_C2GM_GIRLWARUPSTAR)
{
	GirlWarMessageInfo::FromClientUpStar req;
	GirlWarMessageInfo::ToClientUpStar toClient;
	int ret = 0;
	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eStGirlWar))
	{
		Smart_Ptr<GirlWarManager> girlWarMgr = player->GetGirlWarManager();
		if(!girlWarMgr)
		{
			return;
		}

		BEHAVIOR_MONI(player, BehaviorType::Main::eGirlWar, BehaviorType::Minor::eGirlWar_UpStar, MONITOR);

		ret = girlWarMgr->UpStar(req.id(),toClient);

		
	}
	else
	{
		ret = eNotOpen;
	}

	toClient.set_ret(ret);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_GIRLWARUPSTAR);
}

//被动获取战姬
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GirlWarMessage,MSG_REQ_C2GM_GETGIRLWAR)
{
	GirlWarMessageInfo::FromClientGetGirlWar req;
	GirlWarMessageInfo::ToClientGetGirlWar toClient;
	int ret = 0;
	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eStGirlWar))
	{
		Smart_Ptr<GirlWarManager> girlWarMgr = player->GetGirlWarManager();
		if(!girlWarMgr)
		{
			return;
		}

		ret = girlWarMgr->GreateGirlWar(req.id());
		if(ret == 0)
		{
			toClient.set_id(req.id());
		}
	}
	else
	{
		ret = eNotOpen;
	}

	toClient.set_ret(ret);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_GETGIRLWAR);
}
