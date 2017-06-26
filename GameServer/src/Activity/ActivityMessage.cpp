/*
 * ActivityMessage.cpp
 *
 *  Created on: 2014年12月1日
 *      Author: root
 */

#include "ActivityMessage.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "../Object/Player/Player.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Map/GameZone.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "Smart_Ptr.h"
#include "MessageCommonRet.h"
#include <string.h>
#include "CounterDefine.h"
#include "FileLoader/ProtectedKingLoader.h"
#include "MessageStruct/Activity/ProtectKing.pb.h"
#include "../Map/SceneManager.h"
#include "FileLoader/GameParaLoader.h"
#include "MessageStruct/Activity/ActivityBox.pb.h"
#include "ActivityBox.h"
#include "MessageStruct/MSProtoc/MS_Activities.pb.h"
#include "MessageStruct/Activity/OnLineAtTime.pb.h"
#include "../Activity/OnLineAtTimeMgr.h"
#include "../MapCarbon/ClanCarbon.h"
#include "../ServerEventDefine.h"
#include "../StatisticMgr/StatisticHandle.h"
#include "MessageStruct/Answer/AnswerProto.pb.h"
#include "AnswerMgr.h"

ActivityMessage::ActivityMessage()
{
	//请求守护秦王活动信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(ActivityMessage,MSG_REQ_C2GM_PROTECTED_KING_INFO);
	//请求重置秦王活动信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(ActivityMessage,MSG_REQ_C2GM_REFLUSH_COUNT);
	//请求扫荡
	DEF_MSG_QUICK_REQUEST_REG_FUN(ActivityMessage,MSG_REQ_C2GM_DRIRECTED_FINISH);
	//请求获取奖励
	DEF_MSG_QUICK_REQUEST_REG_FUN(ActivityMessage,MSG_REQ_C2GM_GET_REWARD);
	//获取闯关奖励
	DEF_MSG_QUICK_SIMPLE_REG_FUN(ActivityMessage,MSG_SIM_C2GM_GET_NODES_REWARD);

	//获取黄钻信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(ActivityMessage,MSG_REQ_C2GM_YELLOWVIP_INFO);
	//新手礼包
	DEF_MSG_QUICK_REQUEST_REG_FUN(ActivityMessage,MSG_REQ_C2GM_YELLOWVIP_NOVICEPACKS);
	//成长礼包
	DEF_MSG_QUICK_REQUEST_REG_FUN(ActivityMessage,MSG_REQ_C2GM_YELLOWVIP_GROWPACKS);
	//每日礼包
	DEF_MSG_QUICK_REQUEST_REG_FUN(ActivityMessage,MSG_REQ_C2GM_YELLOWVIP_DAILYPACKS);
	//年费黄钻
	DEF_MSG_QUICK_REQUEST_REG_FUN(ActivityMessage,MSG_REQ_C2GM_YELLOWVIP_YEARVIPPACKS);
	//战姬激活
	DEF_MSG_QUICK_REQUEST_REG_FUN(ActivityMessage,MSG_REQ_C2GM_YELLOWVIP_GIRLWAR);

	 //请求采集
	DEF_MSG_QUICK_REQUEST_REG_FUN(ActivityMessage,MSG_REQ_C2GM_ASKBOX);

	DEF_MSG_QUICK_REQUEST_REG_FUN(ActivityMessage,MSG_REQ_C2GM_GET_ON_LINE_AT_TIME);


	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_NOTICEACTIVITYSTATE); // 通知GM运营活动状态

	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_DOUBLE_PROTECTEDKING); // WS通知GM开始双倍守护秦王活动

	DEF_MSG_QUICK_REQUEST_REG_FUN(ActivityMessage,MSG_REQ_C2GM_ANSWER_INFO);//请求个人答题数据
	DEF_MSG_QUICK_REQUEST_REG_FUN(ActivityMessage,MSG_REQ_C2GM_CHOICE_ANSWER);//答题
}

ActivityMessage::~ActivityMessage()
{

}

bool ActivityMessage::IsClanWarGather(Smart_Ptr<Player>& player, int key, int &ret)
{
	ret = -1;

	CGameZone *pZone = player->GetGameZone();
	if(pZone == NULL)
	{
		return false;
	}

	if(GET_MAP_TYPE(player->GetMapID()) != ePVEMap )
	{
		return false;
	}

	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
	if(entry==NULL || entry->m_eType != eClanBattle)
	{
		return false;
	}

	bool isCanGather = ((CClanCarbon*)pZone)->IsCanGatherBox(key, player);
	if(isCanGather)
	{
		ret = 0;
	}

	return true;
}

void ActivityMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_NOTICEACTIVITYSTATE);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_DOUBLE_PROTECTEDKING);
	DEF_SWITCH_TRY_DISPATCH_END
}

//请求守护秦王活动信息
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ActivityMessage,MSG_REQ_C2GM_PROTECTED_KING_INFO)
{
	if (!player)
	{
		LOG_DEBUG(FILEINFO, "ActivityMessage request ProtectKing Info Player is NULL");

		return;
	}

//	if(!player->IsOpenSystem(eProtectKingLv))
//	{
//		return;
//	}

	ActivityKing::AckClientKingInfo toClient;

	player->GetActivityMgr()->GetProtectKingInfo(toClient);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_PROTECTED_KING_INFO);
}

//请求重置秦王活动信息
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ActivityMessage,MSG_REQ_C2GM_REFLUSH_COUNT)
{
	if (!player)
	{
		LOG_DEBUG(FILEINFO, "ActivityMessage request ProtectKing  reset Player is NULL");
		return;
	}

	ActivityKing::AckClientRelush toClient;

	// 是否是VIP 增加重置次数
	int vipResetCnt = 0;//(int)player->GetVipPart()->GetValueByRight(eVipGuardKingResetCnt);
	int resetCnt = ProtectedKingLoader::GetInstance()->GetTimes();
	if (0 < vipResetCnt)
	{
		resetCnt = resetCnt + vipResetCnt;
	}

	if (player->GetCounterService()->GetNumById(PROTECT_KING_COUNTER) >= resetCnt)
	{
		toClient.set_ret(eInstanceTimesNotEnough);
	}
	else if (player->GetActivityMgr()->GetCurNode() <= 0)
	{
		toClient.set_ret(eProtectKingCanNotReset);
	}
	else if (player->GetActivityMgr()->IsInSweep())
	{
		toClient.set_ret(eInInstanceSweep);
	}
	else
	{
		player->GetActivityMgr()->ResetProtectKing();
		player->GetCounterService()->IncreasePlayerCounter(PROTECT_KING_COUNTER,1);
		toClient.set_ret(0);

		EventArgs e;
		player->FireEvent(PLAYER_RESET_PROTECTKING, e);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_REFLUSH_COUNT);
}

//请求扫荡
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ActivityMessage,MSG_REQ_C2GM_DRIRECTED_FINISH)
{
	if (!player)
	{
		LOG_DEBUG(FILEINFO, "ActivityMessage drirected finish Player is NULL");

		return;
	}

	ActivityKing::AckClientFinish toClient;

	if (player->GetActivityMgr()->IsInSweep())
	{
		toClient.set_ret(eInInstanceSweep);
	}
	else if (player->GetActivityMgr()->IsCanSweep())
	{
		toClient.set_ret(eNotPassInstance);
	}
	else if(GET_MAP_TYPE(player->GetMapID()) >= eCopyMap)
	{
		toClient.set_ret(eInInstanceMap);
	}
	else
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eProtectKing, BehaviorType::Minor::eProtectKing_StartSweep, MONITOR);

		toClient.set_ret(player->GetActivityMgr()->StartSweep(GameParaLoader::GetInstance()->getProtectKingSceneId()));

		if (toClient.ret() == 0)
		{
			toClient.set_time(player->GetActivityMgr()->GetLeftTime());
		}

		
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_DRIRECTED_FINISH);
}

//请求获取奖励
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ActivityMessage,MSG_REQ_C2GM_GET_REWARD)
{
	if (!player)
	{
		LOG_DEBUG(FILEINFO, "ActivityMessage Get Reward Player is NULL");

		return;
	}

	ActivityKing::ClientReqGetReward fromClient;
	ActivityKing::AckGetReward toClient;
	int res = 0;

	fromClient.ParseFromString(content);

	if (fromClient.node() < 1)
	{
		toClient.set_ret(eProtectKingGetRewardlevelsError);
	}
	else if (player->GetActivityMgr()->IsHadReward(fromClient.node()-1))
	{
		toClient.set_ret(eHaveGetReward);
	}
	else
	{
		ProtectedReward * item= ProtectedKingLoader::GetInstance()->GetProtectedReward(fromClient.node() -1);
		if (!item)
		{
			res = eProtectKingRewardError;
		}
		else if (item->node > player->GetActivityMgr()->GetPastNode())
		{
			res = eProtectKingGetRewardlevelsError;
		}
		else
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eProtectKing, BehaviorType::Minor::eProtectKing_GetNodeAward, MONITOR);

			vector<WORD> tempFlagList;
			vector<DWORD> tempStreghtList;
			tempFlagList.resize(item->itemId.size(),item->bindType);
			tempStreghtList.resize(item->itemId.size(),0);

			res = player->GetContainer(ePackageType)->IsBagEnought(item->itemId,item->count,tempFlagList);
			if (!res)
			{
				vector<int> tempPos;
				player->GetContainer(ePackageType)->AddItem(item->itemId, item->count,tempPos,tempFlagList,tempStreghtList);

//				StatisticInfo::IDAndCostAndGotInfo idCostAngGotInfo;
//				idCostAngGotInfo.set_beforeid(fromClient.node());
//				StatisticInfo::CostAndGotInfo * costAndGotInfo = idCostAngGotInfo.mutable_costandgotlist();
//				for(uint i = 0;i < item->itemId.size();++i)
//				{
//					StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo->add_gotlist();
//					StatisticMgr::AddItemGotInfo(gotInfo,item->itemId[i],item->count[i]);
//				}

//				if(eAccountType_User == player->GetAccountType())
//					StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_ProtectKing,eStaMinor_ProtectKing_PassCarbonReward,&idCostAngGotInfo);
			}
			
		}

		toClient.set_ret(res);
		if (res == 0)
		{
			player->GetActivityMgr()->SetHadReward(fromClient.node()-1);

			toClient.set_node(fromClient.node());

//			player->UpdateAtiveHighLight(eHL_Threat, 1, false, false);
		}
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_GET_REWARD);
}

//获取闯关奖励
DEF_MSG_QUICK_SIMPLE_DEFINE_FUN(ActivityMessage,MSG_SIM_C2GM_GET_NODES_REWARD)
{
	if (!player)
	{
		LOG_DEBUG(FILEINFO, "ActivityMessage Get Reward Player is NULL");

		return;
	}

	Smart_Ptr<CGameDuplicate> instance;
	CSceneMgr::GetInstance()->GetInstance(player->GetMapID(),instance);

	if (!instance)
	{
		LOG_DEBUG(FILEINFO, "TreasonKingMessage request SomeThing Instance is NULL");

		return;
	}

	if (instance->IsHadReward())
	{
		return;
	}

	BEHAVIOR_MONI(player, BehaviorType::Main::eProtectKing, BehaviorType::Minor::eProtectKing_PassNodeAward, MONITOR);;

	vector<int> vec;
	int exp = 0;
	int force = 0;
	int money = 0;

	ProtectedKingLoader::GetInstance()->GetNodeReward(instance->GetParamfValue(),
																													instance->GetParamsValue() - 1,
																													exp, force, money);
	player->AddExp(exp);
	player->ChangeMoney(money, true);
	player->ChangeCurForce(force, true);

	vec.push_back(eCharMoney);
	vec.push_back(eCharForce);

	player->SynCharAttribute(vec);

	instance->SetHadReward();

	

//	StatisticInfo::CostAndGotInfo costAngGotInfo;
//	if(exp > 0)
//	{
//		StatisticInfo::GotItemInfo * gotInfo = costAngGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCharExp,exp);
//	}
//
//	if(money > 0)
//	{
//		StatisticInfo::GotItemInfo * gotInfo = costAngGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,exp);
//	}
//
//	if(force > 0)
//	{
//		StatisticInfo::GotItemInfo * gotInfo = costAngGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCharForce,exp);
//	}

//	if(eAccountType_User == player->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_ProtectKing,eStaMinor_ProtectKing_RewardInCarbon,&costAngGotInfo);
}


//获取黄钻信息
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ActivityMessage,MSG_REQ_C2GM_YELLOWVIP_INFO)
{
	if (!player)
	{
		LOG_DEBUG(FILEINFO, "ActivityMessage request Yellow Vip Info Player is NULL");

		return;
	}

	YellowVipInfo::ToClentYellowVipInfo toClient;

	player->GetActivityMgr()->GetYellowVipInfo(toClient);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_YELLOWVIP_INFO);
}

//新手礼包
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ActivityMessage,MSG_REQ_C2GM_YELLOWVIP_NOVICEPACKS)
{
	if (!player)
	{
		LOG_DEBUG(FILEINFO, "ActivityMessage request Yellow Vip Novice Packs Player is NULL");

		return;
	}

	int state = 0;
	int ret  = 0;
	YellowVipInfo::ToClentNovicePacks toClient;

	state = player->GetActivityMgr()->GetYellowVipState(eNovice_Packs,eNovice_Packs);
	if (state == RewardState_DontHave)
	{
		toClient.set_ret(eDontHaveActivityReward);
	}
	else if (state == RewardState_HaveGet)
	{
		toClient.set_ret(eReceived);
	}
	else
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eYellowVip, BehaviorType::Minor::eYellowVip_GetNoviceAward, MONITOR);

		ret = player->GetActivityMgr()->GetYellowVipReward(eNovice_Packs,eNovice_Packs);
		toClient.set_ret(ret);

		
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_YELLOWVIP_NOVICEPACKS);
}

//成长礼包
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ActivityMessage,MSG_REQ_C2GM_YELLOWVIP_GROWPACKS)
{
	if (!player)
	{
		LOG_DEBUG(FILEINFO, "ActivityMessage request Yellow Vip Grow Packs Player is NULL");

		return;
	}

	int state = 0;
	int ret  = 0;
	YellowVipInfo::ReqGrowPacks req;
	YellowVipInfo::ToClentReqGrowPacks toClient;

	req.ParseFromString(content);

	state = player->GetActivityMgr()->GetYellowVipState(eGrow_Packs,req.value());
	if (state == RewardState_DontHave)
	{
		toClient.set_ret(eDontHaveActivityReward);
	}
	else if (state == RewardState_HaveGet)
	{
		toClient.set_ret(eReceived);
	}
	else
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eYellowVip, BehaviorType::Minor::eYellowVip_GetGrowAward, MONITOR);

		ret = player->GetActivityMgr()->GetYellowVipReward(eGrow_Packs,req.value());
		toClient.set_ret(ret);
		toClient.set_value(req.value());

		
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_YELLOWVIP_GROWPACKS);
}

//每日礼包
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ActivityMessage,MSG_REQ_C2GM_YELLOWVIP_DAILYPACKS)
{
	if (!player)
	{
		LOG_DEBUG(FILEINFO, "ActivityMessage request Yellow Vip Daily Packs Player is NULL");

		return;
	}

	int state = 0;
	int ret  = 0;
	YellowVipInfo::ReqDailyPacks req;
	YellowVipInfo::ToClentReqDailyPacks toClient;

	req.ParseFromString(content);

	state = player->GetActivityMgr()->GetYellowVipState(eDaily_Packs,req.value());
	if (state == RewardState_DontHave)
	{
		toClient.set_ret(eDontHaveActivityReward);
	}
	else if (state == RewardState_HaveGet)
	{
		toClient.set_ret(eReceived);
	}
	else
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eYellowVip, BehaviorType::Minor::eYellowVip_GetDailyAward, MONITOR);

		ret = player->GetActivityMgr()->GetYellowVipReward(eDaily_Packs,req.value());
		toClient.set_ret(ret);
		toClient.set_value(req.value());

		
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_YELLOWVIP_DAILYPACKS);
}

//年费黄钻
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ActivityMessage,MSG_REQ_C2GM_YELLOWVIP_YEARVIPPACKS)
{
	if (!player)
	{
		LOG_DEBUG(FILEINFO, "ActivityMessage request Yellow Vip YearVip Packs Player is NULL");

		return;
	}

	int state = 0;
	int ret  = 0;
	YellowVipInfo::ToClentYearVipPacks toClient;

	state = player->GetActivityMgr()->GetYellowVipState(eYearVipPacks,eYearVipPacks);
	if (state == RewardState_DontHave)
	{
		toClient.set_ret(eDontHaveActivityReward);
	}
	else if (state == RewardState_HaveGet)
	{
		toClient.set_ret(eReceived);
	}
	else
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eYellowVip, BehaviorType::Minor::eYellowVip_GetYearAward, MONITOR);

		ret = player->GetActivityMgr()->GetYellowVipReward(eYearVipPacks,eYearVipPacks);
		toClient.set_ret(ret);

		
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_YELLOWVIP_YEARVIPPACKS);
}

//战姬激活
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ActivityMessage,MSG_REQ_C2GM_YELLOWVIP_GIRLWAR)
{
	if (!player)
	{
		LOG_DEBUG(FILEINFO, "ActivityMessage request Yellow Vip YearVip Packs Player is NULL");

		return;
	}

	int state = 0;
	int ret  = 0;
	YellowVipInfo::ToClentGirlWarPacks toClient;

	state = player->GetActivityMgr()->GetYellowVipState(eGirlWarPacks,eGirlWarPacks);
	if (state == RewardState_DontHave)
	{
		toClient.set_ret(eDontHaveActivityReward);
	}
	else if (state == RewardState_HaveGet)
	{
		toClient.set_ret(eReceived);
	}
	else
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eYellowVip, BehaviorType::Minor::eYellowVip_GetGirlAward, MONITOR);

		ret = player->GetActivityMgr()->GetYellowVipReward(eGirlWarPacks,eGirlWarPacks);
		toClient.set_ret(ret);

		
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_YELLOWVIP_GIRLWAR);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ActivityMessage,MSG_REQ_C2GM_ASKBOX)
{
	if (!player)
	{
		LOG_DEBUG(FILEINFO, "ActivityMessage request ActivityBox Info Player is NULL");
		return;
	}

	ActivityBoxInfo::ReqActivityBox req;
	req.ParseFromString(content);

	ActivityBoxInfo::ToClientActivityBox toClient;
	toClient.set_ret(eGatherIdHaveOwer);

	int nRet = 0;
	if(IsClanWarGather(player, req.nkey(), nRet))
	{
		if(nRet == 0)
		{
			toClient.set_ret(nRet);
		}

		toClient.set_nkey(req.nkey());
		REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ASKBOX);
		return;
	}

	if (ActivityBox::GetInstance()->IsCanGather(req.nkey(),player))
	{
		toClient.set_ret(0);
		toClient.set_nkey(req.nkey());
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ASKBOX);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ActivityMessage,MSG_REQ_C2GM_GET_ON_LINE_AT_TIME)
{
	if (!player)
	{
		LOG_DEBUG(FILEINFO, "ActivityMessage request OnLine At Time Info Player is NULL");

		return;
	}

	OnLineAtTimeInfo::RetOnLineAtTimeInfo toClient;

	OnLineAtTimeMgr::GetInstance()->GetOnLineAtTimeInfo(toClient);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_GET_ON_LINE_AT_TIME);
}

DEF_MSG_SIMPLE_DEFINE_FUN(ActivityMessage, MSG_SIM_WS2GM_NOTICEACTIVITYSTATE)
{
	ActivitiesProto::ToGMActivityState req;
	int len = 0;
	char* content = message->GetBuffer(len);

	req.ParseFromArray(content, len);

	if (req.charid_size() == req.list_size())
	{
		ActivitiesProto::ActivityStateList list;
		for (int i = 0; i < req.charid_size(); ++i)
		{
			list = req.list(i);
			Smart_Ptr<Player> player;
			PlayerMessage::GetInstance()->GetPlayerByCharId(player, req.charid(i));
			if ((bool)player)
			{
				ActivitiesProto::ActivityState state;
				for (int j = 0; j < list.states_size(); ++j)
				{
					state = list.states(j);
					vector<UIHLType> type = CHighLightMgr::GetHLTypeByActivity((ActivityExType)state.type());
					for (uint i = 0; i < type.size(); ++i)
					{
						player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(type[i].first, type[i].second), state.num(), state.flag());
					}
				}
			}
		}
	}

}

DEF_MSG_SIMPLE_DEFINE_FUN(ActivityMessage, MSG_SIM_WS2GM_DOUBLE_PROTECTEDKING)
{
	ActivityKing::WSToGMDoubleProtectKing req;
	int len = 0;
	char* buffer = message->GetBuffer(len);

	req.ParseFromArray(buffer, len);
	const INT64PLAYERMAP players = PlayerMessage::GetInstance()->getAllPlayer();
	for (INT64PLAYERMAP::const_iterator it = players.begin(); players.end() != it; ++it)
	{
//		if ((bool)it->second && it->second->IsOpenSystem(eProtectKingLv))
		{
			it->second->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Activity, eUIHLSmall_Activity_DoubleGuardKingOpen), 1, req.flag());
		}
	}
}


DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ActivityMessage,MSG_REQ_C2GM_ANSWER_INFO)
{
	AnswerProto::AckReqAnswerInfo toClient;
	if(player)
	{
		player->GetAnswerMgr()->GetAnswerInfo(toClient);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ANSWER_INFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ActivityMessage,MSG_REQ_C2GM_CHOICE_ANSWER)
{
	AnswerProto::ReqChoiceAnswer req;
	AnswerProto::AckReqAnswerInfo toClient;
	req.ParseFromString(content);
	if(player)
	{
		player->GetAnswerMgr()->ChoiceAnswer(req.ret(),toClient);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_CHOICE_ANSWER);
}

