/*
 * RewardMgr.cpp
 *
 *  Created on: 2014年6月17日
 *      Author: root
 */

#include "RewardMgr.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Map/SceneManager.h"
#include "MessageCommonRet.h"
#include "Log/Logger.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/RewardHail/RewardHail.pb.h"
#include "CounterDefine.h"
#include "FightDefined.h"
#include "../ActiveUIHighLight/CActiveHighLightMgr.h"
#include "./FileLoader/GameParaLoader.h"
#include "OnceReward.h"
#include "../StatisticMgr/StatisticHandle.h"

RewardMgr::RewardMgr()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(RewardMgr, MSG_REQ_C2GT_SIGNIN);
	DEF_MSG_QUICK_REQUEST_REG_FUN(RewardMgr, MSG_REQ_C2GT_OPENSIGNIN);
	DEF_MSG_QUICK_REQUEST_REG_FUN(RewardMgr, MSG_REQ_C2GT_ONLINEREWARDINFO);
	DEF_MSG_QUICK_REQUEST_REG_FUN(RewardMgr, MSG_REQ_C2GT_LEVELREWARDINFO);
	DEF_MSG_QUICK_REQUEST_REG_FUN(RewardMgr, MSG_REQ_C2GT_GETREWARD);

	DEF_MSG_QUICK_REQUEST_REG_FUN(RewardMgr, MSG_REQ_C2GT_TOTALLOGINREWARDINFO);

}

RewardMgr::~RewardMgr()
{

}
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(RewardMgr,MSG_REQ_C2GT_SIGNIN)
{
	RewardHail::SignIn reqClient;
	ServerReturn::ServerDoubleInt toClient;
	ServerMessage::ClientSingleMessage ack;

	if(!player)
	{
		return;
	}

	reqClient.ParseFromString(content);

	if(player->IsOpenSystem(eStRewardHall))
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eSignIn, BehaviorType::Minor::eSignIn_SignIn, MONITOR);

		player->GetSignIn()->SetSignIn(reqClient.day(), toClient);

		
	}
	else
	{
		return;
	}

	int tempError = toClient.retf();
	LOG_DEBUG(FILEINFO, "signin ack %lld error %d",player->GetID(),tempError);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GT_SIGNIN);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(RewardMgr,MSG_REQ_C2GT_OPENSIGNIN)
{
	RewardHail::OpenSignIn reqClient;
	RewardHail::ClientSignInInfo toClient;

	if(!player)
	{
		return;
	}

	reqClient.ParseFromString(content);

	if(player->IsOpenSystem(eStRewardHall))
	{
		player->GetSignIn()->GetSIgnInInfo(toClient);
	}
	else
	{
		return;
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GT_OPENSIGNIN);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(RewardMgr,MSG_REQ_C2GT_ONLINEREWARDINFO)
{
	RewardHail::ClientOlineRewardInfo toClient;

	if(!player)
	{
		return;
	}

	if(player->IsOpenSystem(eStRewardHall))
	{
		player->getOnlineReward()->GetOnlineRewardInfo(toClient);
	}
	else
		return;

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GT_ONLINEREWARDINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(RewardMgr,MSG_REQ_C2GT_LEVELREWARDINFO)
{
	RewardHail::OpenLevelReward toClient;

	if(!player)
	{
		return;
	}

	if(player->IsOpenSystem(eStRewardHall))
	{
		int levelReward = player->GetLevelReward()->GetRecord();
		toClient.set_levelreward(levelReward);
	}
	else
		return;

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GT_LEVELREWARDINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(RewardMgr,MSG_REQ_C2GT_TOTALLOGINREWARDINFO)
{
	RewardHail::OpenToalLoginAndFirstChargeRewardInfo toClient;

	if(player)
	{
		player->GetSignIn()->GetTolalLogin(toClient);
	}
	else
		return;

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GT_TOTALLOGINREWARDINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(RewardMgr, MSG_REQ_C2GT_GETREWARD)
{
	LOG_DEBUG(FILEINFO, "client req get reward............");

	RewardHail::GetReward reqClient;
	RewardHail::ToClientGetReward toClient;
//	ServerMessage::ClientSingleMessage ack;
	int res = -1;

	if(!player)
	{
		return;
	}

	reqClient.ParseFromString(content);
	int type = reqClient.type();

	if(player->IsOpenSystem(eStRewardHall) || type == FIRSTCHARGE_REWARD)
	{
		res = GetReward(player, reqClient);
		toClient.set_res(res);

		google::protobuf::RepeatedField< ::google::protobuf::int32 > *field = toClient.mutable_list();
		field->CopyFrom(reqClient.list());
	}
	else
	{
		toClient.set_res(eNotOpen);
	}
	toClient.set_type(type);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GT_GETREWARD);
}

int RewardMgr::GetReward(Smart_Ptr<Player> player, RewardHail::GetReward &info)
{
	vector<DWORD> goods;
	vector<DWORD> num;
	vector<int> vecPos;
	vector<DWORD> strengthLvVec;
	vector<WORD> bindVec;

	int res = 0;
	int type = info.type();

	switch (type)
	{
	case SIGIN_REWARD:
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eSignIn, BehaviorType::Minor::eSignIn_GetAward, MONITOR);

			res = player->GetSignIn()->GetSignInReward(info, goods, num);

			
		}
		break;
	case ONLINE_REWARD:
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eOnlineReward, BehaviorType::Minor::eOnlineReward_GetAward, MONITOR);

			res = player->getOnlineReward()->GetOnlineReward(info, goods, num);

			
		}
		break;
	case LEVEL_REWARD:
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eLevelReward, BehaviorType::Minor::eLevelReward_GetAward, MONITOR);

			res = player->GetLevelReward()->GetLevelReward(info, goods, num);

			
		}
		break;
	case TOTALLOGIN_REWARD:
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eTotalLogin, BehaviorType::Minor::eTotalLogin_GetAward, MONITOR);

			res = player->GetSignIn()->GetTolalLoginReward(info, goods, num, strengthLvVec);

			
		}
		break;
	case FIRSTCHARGE_REWARD:
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eFirstRecharge, BehaviorType::Minor::eFirstRecharge_GetAward, MONITOR);

			res = player->GetSignIn()->GetFirstChargeRmbReward(info, goods, num, strengthLvVec);

			
		}
		break;
	case MICROCLIENT_REWARD:
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eMicroReward, BehaviorType::Minor::eMicroReward_GetAward, MONITOR);

			res = player->GetOnceReward()->GetMicroReward(info, goods, num, bindVec);

			
		}
		break;
	default:
		{
			LOG_DEBUG(FILEINFO, "client req get reward type is error");
			return 0;
		}
	}
	if(!res)
	{
//		if (MICROCLIENT_REWARD != type)
//		{
//			bindVec.resize(goods.size(), 1);
//		}
//
//		if(type ==TOTALLOGIN_REWARD || type ==FIRSTCHARGE_REWARD)
//		{
//			//info 中的 list返回 能够得到的 物品
//		}
//		else
//		{
//			strengthLvVec.resize(goods.size(), 0);
//		}
//
//		player->GetContainer(ePackageType)->AddItem(goods, num, vecPos, bindVec,strengthLvVec,true,npc_fly_type);

		string playname = player->GetName();
		string equipid = "";

		for(size_t i = 0;i < goods.size();++i)
		{

			const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goods[i]);
			if(ginfo)
			{
				if(type == FIRSTCHARGE_REWARD && equipid=="" && ginfo->type==eWeapon)
				{
					char strid[256] = {0};
					sprintf(strid, "%d", (int)ginfo->itemID);
					equipid = strid;
				}

			}
		}

		if(type == FIRSTCHARGE_REWARD && equipid != "")
		{
			ChatMgr::GetInstance()->Marquee(eMarqueeFirstCharge, playname, equipid, "");
		}

		return 0;
	}
	return res;
}





