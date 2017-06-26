/*
 * GMInterfaceAnswerMessage.cpp
 *
 *  Created on: 16/07/2015
 *      Author: root
 */

#include "GMInterfaceAnswerMessage.h"
#include "MessageStruct/InterfaceAnswer/InterfaceAnswer.pb.h"
#include "../Object/Player/PlayerMessage.h"
#include "Network/Message_Facade.h"
#include "../Attribute/AttributeCreature.h"
#include "../MessageBuild.h"
#include "FileLoader/InterfaceAnswerLoader.h"
#include "../Vitality/VitalityMgr.h"
#include "./FileLoader/GameParaLoader.h"
#include "../StatisticMgr/StatisticHandle.h"


GMInterfaceAnswerMessage::GMInterfaceAnswerMessage()
{
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_BUCKLE_ATT);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_GIVE_ANSWER_ITEM);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_REQ_WS2GM_PLAYER_ANSWER);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_BEGINANSWER);
}

GMInterfaceAnswerMessage::~GMInterfaceAnswerMessage()
{

}

void GMInterfaceAnswerMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_REQ_WS2GM_PLAYER_ANSWER);

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_BEGINANSWER);

	DEF_SWITCH_TRY_DISPATCH_END

	return ;
}

void GMInterfaceAnswerMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_BUCKLE_ATT);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_GIVE_ANSWER_ITEM);

	DEF_SWITCH_TRY_DISPATCH_END

	return ;
}

void GMInterfaceAnswerMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	return ;
}

DEF_MSG_REQUEST_DEFINE_FUN(GMInterfaceAnswerMessage,MSG_REQ_WS2GM_BUCKLE_ATT)
{
	InterfaceAnswer::SendBuckleAtt tempRequest;
	char* tempStr  = NULL;
	int tempLenght = 0;

	tempStr = message->GetBuffer(tempLenght);
	tempRequest.ParseFromArray(tempStr,tempLenght);

	Smart_Ptr<Player> tempPlayer;
	int64 tempPlayerID = tempRequest.playerid();

	InterfaceAnswer::ReturnBuckleAtt tempAck;

	PlayerMessage::GetInstance()->GetPlayerByCharid(tempPlayerID,tempPlayer);

	if(tempPlayer)
	{
		ECharAttrType tempAtt = (ECharAttrType)tempRequest.attid();
		int tempValue		  = tempRequest.value();

		if(tempAtt == eCharGolden)
		{
			if(tempPlayer->getAttManage()->getValue<int>(tempAtt) < tempValue)
			{
				tempAck.set_error(eNotEnoughGold);
			}
			else
			{
				BEHAVIOR_MONI(tempPlayer, BehaviorType::Main::eAnswer, BehaviorType::Minor::eAnswer_QuickHandleCost, MONITOR);

				vector<int> tempType;

				tempAck.set_error(0);
				tempPlayer->ChangeGolden(tempValue,false);
				tempType.push_back(eCharGolden);

//				StatisticMgr::GetInstance()->StatisticPlayerGetItems(tempPlayer, eStatic_AboutGolden, eStaMinor_AboutGolden_AnswerCost,1, tempValue);

				tempPlayer->SynCharAttribute(tempType);
				tempPlayer->SetDataFlag(eBaseInfo);

				
			}
		}
	}
	else
	{
		tempAck.set_error(eSellerDontLikeYou);
	}

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_BUCKLE_ATT,message,&tempAck);
	Message_Facade::Send(messRet);

	return ;
}

DEF_MSG_REQUEST_DEFINE_FUN(GMInterfaceAnswerMessage,MSG_REQ_WS2GM_GIVE_ANSWER_ITEM)
{
	InterfaceAnswer::AnswerGiveItem tempRequest;
	char* tempStr  = NULL;
	int tempLenght = 0;

	tempStr = message->GetBuffer(tempLenght);
	tempRequest.ParseFromArray(tempStr,tempLenght);

	Smart_Ptr<Player> tempPlayer;
	int64 tempPlayerID = tempRequest.playerid();

	PlayerMessage::GetInstance()->GetPlayerByCharid(tempPlayerID,tempPlayer);
	InterfaceAnswer::ReturnBuckleAtt tempAck;

	if(tempPlayer)
	{
		BEHAVIOR_MONI(tempPlayer, BehaviorType::Main::eAnswer, BehaviorType::Minor::eAnswer_GetAward, MONITOR);;

		vector<DWORD> tempItemIDList;
		vector<DWORD> tempNumList;
		vector<WORD>  tempBindTypeList;
		vector<DWORD> tempStrengthLv;
		vector<int>   tempPos;

		for(int i = 0;i < tempRequest.id_size();++i)
		{
			InterfaceAnswerLoader::GetInstance()->getItemByItemID(tempRequest.id(i),tempItemIDList,tempNumList,tempBindTypeList);
		}

		tempStrengthLv.resize(tempItemIDList.size(),0);

		if(tempPlayer->GetContainer(ePackageType)->AddItem(tempItemIDList,tempNumList,tempPos,tempBindTypeList,tempStrengthLv) != 0)
		{
			tempAck.set_error(ePackageFull);
		}
		else
		{
			tempAck.set_error(eRetSuccess);
			vector<int> tempAllVector;
//			StatisticInfo::CostAndGotInfo tempStatisticBuffer;

			for(size_t i = 0;i < tempItemIDList.size() && i < tempNumList.size();++i)
			{
//				StatisticInfo::GotItemInfo* tempGotBuffer = tempStatisticBuffer.add_gotlist();
//				StatisticMgr::AddItemGotInfo(tempGotBuffer,tempItemIDList[i],tempNumList[i]);

				int tempType = GoodsLoader::GetInstance()->GetSpecliItemFunPlayerAttr(tempItemIDList[i]);
				if(tempType == 0)
					continue;
				tempAllVector.push_back(tempType);

//				if(tempType == GameParaLoader::GetInstance()->getExchangeMoneyItemId() && tempNumList[i] > 0)
//				{
//					StatisticInfo::CostAndGotInfo costAndGotInfo;
//					StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//					StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,tempNumList[i]);
//					StatisticMgr::GetInstance()->StatisticPlayerBehavior(tempPlayer->GetID(),eStatic_Money,eStaMinor_Money_Answer,&costAndGotInfo);
//				}
			}

//			if(eAccountType_User == tempPlayer->GetAccountType())
//				StatisticMgr::GetInstance()->StatisticPlayerBehavior(tempPlayerID,eStatic_Answer,eStaMinor_Answer_GetRewardEvent,&tempStatisticBuffer);

			//同步属性
			tempPlayer->SynCharAttribute(tempAllVector);


		}

		
	}
	else
	{
		tempAck.set_error(eSellerDontLikeYou);
	}

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_GIVE_ANSWER_ITEM,message,&tempAck);
	Message_Facade::Send(messRet);

	return ;
}

DEF_MSG_SIMPLE_DEFINE_FUN(GMInterfaceAnswerMessage,MSG_REQ_WS2GM_PLAYER_ANSWER)
{
	InterfaceAnswer::AnswerActivity tempSimPle;
	char* tempStr  = NULL;
	int tempLenght = 0;

	tempStr = message->GetBuffer(tempLenght);
	tempSimPle.ParseFromArray(tempStr,tempLenght);

	int64 tempPlayerID = tempSimPle.playerid();
	Smart_Ptr<Player> tempPlayer;

	PlayerMessage::GetInstance()->GetPlayerByCharid(tempPlayerID,tempPlayer);
	if(!tempPlayer)
	{
		LOG_WARNING(FILEINFO, "Call GetPlayerByCharid(), answer add vitality not find player, ID:%lld", tempPlayerID);

		PlayerMessage::GetInstance()->GetPlayerCacheByCharid(tempPlayerID, tempPlayer);
		if (!tempPlayer)
		{
			LOG_WARNING(FILEINFO, "Call GetPlayerCacheByCharid(), answer add vitality not find player, ID:%lld", tempPlayerID);
			return;
		}
	}

	tempPlayer->GetVitalityMgr()->FinshVitality(eViAnswer);
	tempPlayer->GetCounterService()->IncreasePlayerCounter(ANSWER_ENTER_COUNT,1);

	HintValue value;
	int count = InterfaceAnswerLoader::GetInstance()->getOtherData().m_dayAnswerCnt;
	value.fValue = count - tempPlayer->GetCounterService()->GetNumById(ANSWER_ENTER_COUNT);
	value.sValue = count;
	tempPlayer->UpdateUIHighLight(eUIHLHandle_ModAndSyn, value, UIHLType(eUIHLBig_Answer, eSmall_Answer_AnswerCnt));

	return ;
}

DEF_MSG_SIMPLE_DEFINE_FUN(GMInterfaceAnswerMessage, MSG_SIM_WS2GM_BEGINANSWER)
{
	InterfaceAnswer::WS2GMAnswerState req;
	int len = 0;
	char* buffer = message->GetBuffer(len);

	req.ParseFromArray(buffer, len);

	const INT64PLAYERMAP& players= PlayerMessage::GetInstance()->getAllPlayer();
	for (INT64PLAYERMAP::const_iterator it = players.begin(); it != players.end(); ++it)
	{
		Smart_Ptr<Player> player = it->second;
		if (player && player->IsOpenSystem(eStAnswer))
		{
			player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Activity, eUIHLSmall_Activity_AnswerOpen), 1, req.flag());
		}
	}
}
