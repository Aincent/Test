/*
 * TreasonKingMessage.cpp
 *
 *  Created on: 2014年11月25日
 *      Author: root
 */

#include "TreasonKingMessage.h"
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
#include "FileLoader/TreasonKingLoader.h"
#include "../Chat/ChatMgr.h"
#include "../Object/Creature.h"
#include "../Map/SceneManager.h"
#include "FileLoader/GameParaLoader.h"
#include "../Map/DuplicateMgr.h"
#include "../Object/FileLoader/DropItemLoader.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "ActivityMgr.h"
#include <math.h>
#include "../StatisticMgr/StatisticHandle.h"

TreasonKingMessage::TreasonKingMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(TreasonKingMessage,MSG_REQ_C2GM_TREASONKING_INFO);		//获取反王通关信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(TreasonKingMessage,MSG_REQ_C2GM_TREASONKING_SOMETHING);	//进行挖宝操作
	DEF_MSG_QUICK_REQUEST_REG_FUN(TreasonKingMessage,MSG_REQ_C2GM_TREASONKING_REWARD);	//获取奖励
	DEF_MSG_QUICK_REQUEST_REG_FUN(TreasonKingMessage,MSG_REQ_C2GM_TREASONKING_RESET);//重置副本
}

TreasonKingMessage::~TreasonKingMessage()
{

}

//获取反王通关信息
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(TreasonKingMessage,MSG_REQ_C2GM_TREASONKING_INFO)
{
	TreasonKingInfo::ToClentTreasonInfo toClient;

	if(!player)
	{
		LOG_DEBUG(FILEINFO, "TreasonKingMessage request TreasonKing Info Player is NULL");
		return;
	}

	toClient.set_ret(0);
	toClient.set_level(player->GetCounterService()->GetNumById(TREASON_KING_LEVELS));
	toClient.set_enter(player->GetCounterService()->GetNumById(TREASON_KING_ENTER));
	toClient.set_reward(player->GetTreasonKingReward()->GetRewardInfo());
	toClient.set_passmaxlevel(player->GetCounterService()->GetNumById(TREASON_KING_MAXPASSLEVEL));
	toClient.set_reset(player->GetCounterService()->GetNumById(TREASON_KING_RESET));
	player->GetActivityMgr()->GetTreasonBestTime(toClient);

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_TREASONKING_INFO);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_TREASONKING_INFO);
}

//进行挖宝操作
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(TreasonKingMessage,MSG_REQ_C2GM_TREASONKING_SOMETHING)
{
	TreasonKingInfo::FromClientDrop fromClient;
	TreasonKingInfo::ToClientDrop toClient;

	Smart_Ptr<CGameDuplicate> instance;
	Smart_Ptr<CreatureObj> obj;
	vector<int> vec;

	fromClient.ParseFromString(content);


	if(!player)
	{
		LOG_DEBUG(FILEINFO, "TreasonKingMessage request SomeThing Player is NULL");
		return;
	}

	DigTreasure(player, fromClient, toClient);


	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_TREASONKING_SOMETHING);
}

//获取奖励
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(TreasonKingMessage,MSG_REQ_C2GM_TREASONKING_REWARD)
{
	TreasonKingInfo::GetTreasonKingReward fromClient;
	TreasonKingInfo::ToClientTreasonKingReward toClient;

	fromClient.ParseFromString(content);

	if(!player)
	{
		LOG_DEBUG(FILEINFO, "TreasonKingMessage request reward Player is NULL");
		return;
	}

	Smart_Ptr<CounterService> counter = player->GetCounterService();
	if (!counter)
	{
		LOG_DEBUG(FILEINFO, "TreasonKingMessage request reward counter is NULL");
		return;
	}

	if(fromClient.level() > 0 && fromClient.level() <= counter->GetNumById(TREASON_KING_MAXPASSLEVEL)) /*&& fromClient.level() <= player->GetCounterService()->GetNumById(TREASON_KING_LEVELS))*/
	{
		toClient.set_ret(player->GetTreasonKingReward()->GetLevelsReward(fromClient.level()));
		toClient.set_level(fromClient.level());
	}
	else
	{
		toClient.set_ret(eTreasonKingLevelError);
	}

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_TREASONKING_REWARD);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_TREASONKING_REWARD);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(TreasonKingMessage,MSG_REQ_C2GM_TREASONKING_RESET)
{
	int ret = 0;
	int cost = 0;
	ServerReturn::ServerRetInt req;
	ServerReturn::ServerRetInt toClient;
	vector<int> vec;

	req.ParseFromString(content);

	if(!player)
	{
		LOG_DEBUG(FILEINFO, "TreasonKingMessage request reset Player is NULL");
		return;
	}

	InstanceMapEntry * entry = InstanceLoader::GetInstance()->GetEntry(req.ret());
	if(!entry || entry->m_eType != eTreasonKing)
		return;

	Smart_Ptr<CounterService> counter = player->GetCounterService();
	if (!counter)
	{
		LOG_DEBUG(FILEINFO, "TreasonKingMessage request reset counter is NULL");
		return;
	}

	int vipAddCnt = 0;//(int)player->GetVipPart()->GetValueByRight(eVipTreasonKingCnt);
	int reaptTimes = entry->m_nResetNum + vipAddCnt;

	cost = entry->m_nResetPurchase * (((int)pow(static_cast<float>(2), counter->GetNumById(TREASON_KING_RESET))));

//	if(!player->IsOpenSystem(eTreasonKingLv))
//	{
//		ret = eNotOpen;
//	}
//	else
	if(counter->GetNumById(TREASON_KING_RESET) >= reaptTimes)
	{
		ret = eInstanceTimesNotEnough;
	}
	else if(!player->IsEnoughGolden(cost))
	{
		ret = eNotEnoughGold;
	}
	else
	{
		int costGolden = 0;
		int costbGolden = 0;

		player->DecreaseGoldenCost(cost, costGolden, costbGolden);
		if(costGolden > 0)
			vec.push_back(eCharGolden);
		if(costbGolden > 0)
			vec.push_back(eCharBindGolden);

		player->SynCharAttribute(vec);
		counter->IncreasePlayerCounter(TREASON_KING_RESET, 1);
		counter->SetPlayerCounter(TREASON_KING_LEVELS, 0);
		counter->SetPlayerCounter(TREASON_KING_ENTER, 0);

//		player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_InstanceHall, eUIHLSmall_InstanceHall_KillTreasonKingCanChallenge), player->GetActivityMgr()->GetTreansonSuplusPass());
		HintValue value;
		value.sValue = player->GetActivityMgr()->GetTreasonSurplusResetCnt(value.fValue);
		player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, value, UIHLType(eBig_TreasonKing, eSmall_TreasonKing_AlreadyFini));

	}

	toClient.set_ret(ret);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_TREASONKING_RESET);
}

void TreasonKingMessage::DigTreasure(Smart_Ptr<Player>& player, TreasonKingInfo::FromClientDrop& fromClient, TreasonKingInfo::ToClientDrop& toClient)
{
	BEHAVIOR_MONI(player, BehaviorType::Main::eTreasonKing, BehaviorType::Minor::eTreasonKing_Dig, MONITOR);;

	Smart_Ptr<CGameDuplicate> instance;
	Smart_Ptr<CreatureObj> obj;
	vector<int> vec;
	int res = 0;

	instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());

	if(!instance || instance->GetInstanceScript()->m_eType != eTreasonKing)
	{
		LOG_DEBUG(FILEINFO, "TreasonKingMessage request SomeThing Instance is NULL");
	}
	else
	{

		instance->GetObject(fromClient.bossid(), obj);

		int price = TreasonKingLoader::GetInstance()->GetOneGold(instance->GetDigNum());

		const InstanceMapEntry* tempFileData = instance->GetInstanceScript();

		if(tempFileData == NULL)
		{
			toClient.set_ret(eSystemError);
		}
		if(instance->IsMaxDroNum())
		{
			toClient.set_ret(eTreasonKingDigEnough);
		}
		else if(player->getAttManage()->getValue<int>(eCharGolden) < price)
		{
			toClient.set_ret(eNotEnogGolden);
		}
		else if(!obj)
		{
			toClient.set_ret(eBossCanNotFind);
		}
		else if(!obj->IsDead())
		{
			toClient.set_ret(eBossNotDead);
		}
		else if(CSceneMgr::GetInstance()->GetDistance(player->GetXNewPos(),player->GetYNewPos(),obj->GetXNewPos(),obj->GetYNewPos()) > GameParaLoader::GetInstance()->getDigPointLength())
		{
			toClient.set_ret(ePointDistance);
		}
		else if(player->GetContainer(ePackageType)->GetFreeSize() < 1)
		{
			toClient.set_ret(ePackageFull);
		}
		else
		{
			DropStaticInfo tempItem;
//			DropItemLoader::GetInstance()->GetOneDropItem(tempFileData->m_nRandomID,tempItem);
			if(tempItem.m_ItemID > 0)
			{
				res = player->GetContainer(ePackageType)->IsBagEnought(tempItem.m_ItemID,tempItem.m_Counts,tempItem.m_BindType);
				if(res)
				{
					toClient.set_ret(res);
				}
				else
				{
					vector<int> tempPos;
					player->GetContainer(ePackageType)->AddItem(tempItem.m_ItemID,tempItem.m_Counts,tempPos,tempItem.m_BindType);
				}

				if(res == 0)
				{
//					StatisticInfo::CostAndGotInfo costAndGotInfo;
//					StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
//					StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo.add_gotlist();
//					StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,price,player);

					player->ChangeGolden(price,false);
					vec.push_back(eCharGolden);
					instance->IncreaseDroNum();

					toClient.set_ret(0);
					toClient.set_reward(tempItem.m_ItemID);
					toClient.set_num(tempItem.m_Counts);

//					StatisticMgr::AddItemGotInfo(gotItemInfo,tempItem.m_ItemID,tempItem.m_Counts);
					const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(tempItem.m_ItemID);
					if(ginfo != NULL && ginfo->itemQuality == 4)
					{
						string params = ChatMgr::GetInstance()->GetStringByInt(tempItem.m_ItemID);
						ChatMgr::GetInstance()->Marquee(eTreasonKingDigSomeThing,player->GetName(),params,"");
					}

					// 统计元宝花费

//					StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,player);
//					if(eAccountType_User == player->GetAccountType())
//						StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_TreasonKing,eStaMinor_TreasonKing_DigEvent,&costAndGotInfo);
//
	//				StatisticMgr::GetInstance()->StatisticPlayerGetItems(player, eStatic_AboutGolden, eStaMinor_AboutGolden_TreasonKingDigTreasure, 1, price);
				}
			}
			else
			{
				toClient.set_ret(eRandomArticleError);
			}
		}

		if(vec.size() > 0)
			player->SynCharAttribute(vec);
	}

	
}
