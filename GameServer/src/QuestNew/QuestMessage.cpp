/*
 * QuestMessage.cpp
 *
 *  Created on: 2014年10月30日
 *      Author: root
 */

#include "QuestMessage.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "QuestPart.h"
#include "../Object/Player/Player.h"
#include "../Object/Player/PlayerMessage.h"
#include "../MessageBuild.h"
#include "../StatisticMgr/StatisticHandle.h"

QuestNewMessage::QuestNewMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(QuestNewMessage,MSG_REQ_C2GM_UPDATEQUEST);		//提交任务

	//DEF_MSG_QUICK_REQUEST_REG_FUN(QuestNewMessage,MSG_REQ_C2GM_FRESHSTAR);		//帮派和日常刷星

	DEF_MSG_QUICK_REQUEST_REG_FUN(QuestNewMessage,MSG_REQ_C2GM_ONEKEYCOM);		//一键完成日常和帮派任务

	DEF_MSG_QUICK_REQUEST_REG_FUN(QuestNewMessage,MSG_REQ_FINISH_ONE_QUEST);

	DEF_MSG_QUICK_REQUEST_REG_FUN(QuestNewMessage,MSG_REQ_C2GM_REFLUSH_WEEK_TASK);		
}

QuestNewMessage::~QuestNewMessage()
{

}


DEF_MSG_QUICK_REQUEST_DEFINE_FUN(QuestNewMessage,MSG_REQ_C2GM_UPDATEQUEST)
{
	QuestProto::ReqSubmitTask req;
	QuestProto::ToSubmitTask toClient;

	req.ParseFromString(content);

	int error = 0;
	if(player->GetQuestMgr())
	{
		uint questType = 0;
		ServerRet tempAcceptSuccess = eSystemError;
		error = player->GetQuestMgr()->submit(req.questid(), &questType,tempAcceptSuccess);

		toClient.set_questid(req.questid());
		toClient.set_error(error);

		REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_UPDATEQUEST);

		if( tempAcceptSuccess != eDailyTimesFull &&
			tempAcceptSuccess != eClanTimesFull &&
			tempAcceptSuccess != eAllCampTaskHadComp &&
			tempAcceptSuccess != eRetSuccess &&
			tempAcceptSuccess != eGuideLevelNotEnough &&
			tempAcceptSuccess != eAllMainTaskHadComp)
		{
			return ;
		}

		Quest_CondStateInfo tempData;
		//为了 客户端 提交任务 时 先有 结果  再 推送  新的 任务   更改 之前  没结果  就 推送
		if(questType == QuestType_Functional)
		{
			player->GetQuestMgr()->getDailyQuests()->setDirty(true);

			tempData = player->GetQuestMgr()->getDailyQuests()->updateToClient();

			//完成活跃次数
//			player->GetVitalityMgr()->FinshVitality(eViDailyQuest);
		}
		else if(questType == QuestType_Clan)
		{
			player->GetQuestMgr()->getClanQuests()->setDirty(true);

			tempData = player->GetQuestMgr()->getClanQuests()->updateToClient();

			//完成活跃次数
//			player->GetVitalityMgr()->FinshVitality(eViClanQuest);
		}
		else if(questType == QuestType_Camp)
		{
			tempData = player->GetQuestMgr()->getCampQuests()->updateToClient();
		}
		else if(questType == QuestType_Guide)
		{
			player->GetQuestMgr()->getGuideQuests()->updateToClient();
		}
		else if(questType == QuestType_Week)
		{
			player->GetQuestMgr()->getWeekTask()->updateToClient();
		}
		else
		{
			player->GetQuestMgr()->getAcceptedQuests().setDirty(true);

			tempData = player->GetQuestMgr()->getAcceptedQuests().updateToClient();
		}

		LOG_DEBUG(FILEINFO,"Player %lld accept Task,%d type,%d state,%d numStar,%d rewardStar,%d times,%d nowNum,%d targeNum,%d",
				player->GetID(),
				tempData.questID,
				questType,
				tempData.state,
				tempData.numStar,
				tempData.rewardStar,
				tempData.times,
				tempData.nowNum,
				tempData.targeNum);
	}
}

//DEF_MSG_QUICK_REQUEST_DEFINE_FUN(QuestNewMessage,MSG_REQ_C2GM_FRESHSTAR)
//{
//	QuestProto::ReqFreshStar req;
//	QuestProto::ToFreshStar toClient;
//
//	req.ParseFromString(content);
//
//	int error = 0;
//	if(player->GetQuestMgr())
//	{
//		error = player->GetQuestMgr()->upgradeQuestsStarLevel(req.taskid(),req.freshtype());
//	}
//
//	toClient.set_error(error);
//	toClient.set_freshtype(req.freshtype());
//
//	int tempQuestID = req.taskid();
//	int tempType    = req.freshtype();
//
//	LOG_DEBUG(FILEINFO,"Player %lld reflush Task %d type %d error %d",player->GetID(),tempQuestID,tempType,error);
//
//	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_FRESHSTAR);
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_FRESHSTAR);
//}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(QuestNewMessage,MSG_REQ_C2GM_ONEKEYCOM)
{
	QuestProto::ReqFreshStar req;
	QuestProto::ToFreshStar toClient;

	req.ParseFromString(content);

	int error = 0;
	if(player->GetQuestMgr())
	{
		error = player->GetQuestMgr()->oneKeyComplete(req.taskid());
	}

	toClient.set_error(error);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_ONEKEYCOM);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(QuestNewMessage,MSG_REQ_FINISH_ONE_QUEST)
{
	ServerReturn::ServerRetInt tempRequest;
	ServerReturn::ServerRetInt tempToClient;

	tempRequest.ParseFromString(content);

	ServerRet tempError = eSystemError;
	if(player->GetQuestMgr())
	{
		tempError = player->GetQuestMgr()->finishOneQuest(tempRequest.ret());
	}

	tempToClient.set_ret(tempError);

	REQ_MSG_QUICK_ACK_FUN(message, &tempToClient, MSG_REQ_FINISH_ONE_QUEST);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(QuestNewMessage,MSG_REQ_C2GM_REFLUSH_WEEK_TASK)
{
	ServerReturn::ServerRetInt tempToClient;

	ServerRet tempError = eSystemError;
	if(player->GetQuestMgr())
	{
		tempError = player->GetQuestMgr()->updateWeekTask();
	}

	tempToClient.set_ret(tempError);

	REQ_MSG_QUICK_ACK_FUN(message, &tempToClient,MSG_REQ_C2GM_REFLUSH_WEEK_TASK);
}
