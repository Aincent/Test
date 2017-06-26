/*
 * MarryMessage.cpp
 *
 *  Created on: 2015年3月19日
 *      Author: root
 */

#include "MarryMessage.h"
#include "Network/Message_Facade.h"
#include "../Object/Player/PlayerMessage.h"
#include "MarryRing.h"
#include "../MessageBuild.h"
#include "../Attribute/AttributeCreature.h"
#include "MarryLoader/MarryLoader.h"
#include "../MsgCommonClass.h"
#include "../Map/GameZone.h"
#include "../StatisticMgr/StatisticHandle.h"

MarryMessage::MarryMessage()
{
	DEF_MSG_REQUEST_REG_FUN(eGameServer,MSG_REQ_WS2GM_GET_MARRY_REWARD);

	DEF_MSG_REQUEST_REG_FUN(eGameServer,MSG_REQ_WS2GM_BUCKLE_ITEM);

	DEF_MSG_REQUEST_REG_FUN(eGameServer,MSG_REQ_WS2GM_FORCE_DIVORCE_BUCKLE_GLODEN);

	DEF_MSG_REQUEST_REG_FUN(eGameServer,MSG_REQ_WS_GM_MARRY_GET_OTHER_MAP_INFO);

	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_MARRY);

	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_FORCE_DIVORCE_GIVE_GLODEN);

	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_DIVORCE_SUCCESS);

	DEF_MSG_QUICK_REQUEST_REG_FUN(MarryMessage, MSG_REQ_C2GM_REQUEST_RING_INFO);

	DEF_MSG_QUICK_SIMPLE_REG_FUN(MarryMessage, MSG_SIM_C2GM_MARRY_CHANGE_MAP);

	DEF_MSG_QUICK_SIMPLE_REG_FUN(MarryMessage,MSG_SIM_C2GM_RING_UP_STEP);

	DEF_MSG_QUICK_SIMPLE_REG_FUN(MarryMessage,MSG_SIM_C2GM_RECEIVE_REWARD);

	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2WS_MARRY_CHANGE_MAP);
}

MarryMessage::~MarryMessage()
{

}

void MarryMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_MARRY);

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_FORCE_DIVORCE_GIVE_GLODEN);

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_DIVORCE_SUCCESS);

	DEF_SWITCH_TRY_DISPATCH_END
}

void MarryMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_GET_MARRY_REWARD);

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_BUCKLE_ITEM);

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_FORCE_DIVORCE_BUCKLE_GLODEN);

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS_GM_MARRY_GET_OTHER_MAP_INFO);

	DEF_SWITCH_TRY_DISPATCH_END
}

void MarryMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2WS_MARRY_CHANGE_MAP);

	DEF_SWITCH_TRY_DISPATCH_END
}

ServerRet MarryMessage::checkSelfChangeMap(Smart_Ptr<Player>& player,Smart_Ptr<Player>& otherPlayer)
{
	int64 tempOtherID = player->getAttManage()->getValue<int64>(eCouple);
	//还没有结婚
	if(tempOtherID == 0)
	{
		return eSelfNotMarry;
	}

	//对方不在线
	PlayerMessage::GetInstance()->GetPlayerByCharid(tempOtherID,otherPlayer);
	if(!otherPlayer)
	{
		return eNotHaveOnline;
	}

	EMapType tempMapType = EMapType(GET_MAP_TYPE(otherPlayer->GetMapID()));
	if(tempMapType != eOnlyMap && tempMapType != eStaticMap)
	{
		return eMarryMapCanNotChange;
	}

	CGameZone* tempZone = player->GetGameZone();
	if(tempZone == NULL)
	{
		return eSystemError;
	}

	//九宫格不可以传送
	vector<Smart_Ptr<CreatureObj> > tempCreatureList;
	Smart_Ptr<CreatureObj> tempCreature = player;

	tempZone->GetGroupObject(tempCreature,tempCreatureList);

	for(size_t i = 0;i < tempCreatureList.size();++i)
	{
		if(tempCreatureList[i].Get() != otherPlayer.Get())
		{
			continue;
		}

		return eConveyDistanceTooNear;
	}

	return eRetSuccess;
}

void MarryMessage::dealwithOtherChangeMap(int64 playerID,MarryServer::MarryChangeMap& returnMsg)
{
	//对方不在线
	Smart_Ptr<Player> tempPlayer;
	PlayerMessage::GetInstance()->GetPlayerByCharid(playerID,tempPlayer);
	if(!tempPlayer)
	{
		returnMsg.set_error(eNotHaveOnline);

		return ;
	}

	EMapType tempMapType = (EMapType)GET_MAP_TYPE(tempPlayer->GetMapID());

	if(tempMapType != eOnlyMap && tempMapType != eStaticMap)
	{
		returnMsg.set_error(eMarryMapCanNotChange);

		return ;
	}

	CGameZone* tempZone = tempPlayer->GetGameZone();
	if(tempZone == NULL)
	{
		returnMsg.set_error(eSystemError);

		return ;
	}

	//调整坐标
	int tempXpos = tempPlayer->GetXNewPos();
	int tempYpos = tempPlayer->GetYNewPos();

	adjustPos(*tempZone,tempXpos,tempYpos);

	returnMsg.set_mapid(tempPlayer->GetMapID());

	returnMsg.set_xpos(tempXpos);

	returnMsg.set_ypos(tempXpos);

	returnMsg.set_error(eRetSuccess);

	return ;
}

//调整坐标点，防止两个玩家位置一样
void MarryMessage::adjustPos(CGameZone& gameZone,int& xpos,int& ypos)
{
	vector<int> tempPosxList;
	vector<int> tempPosyList;

	for(int i = -2;i <= 2;++i)
	{
		for(int j = -2;j <= 2;++j)
		{
			if(j == 0 && i == 0)
			{
				continue;
			}

			if(!gameZone.IsValidPos(xpos + i,ypos + j))
			{
				continue;
			}

			tempPosxList.push_back(xpos + i);

			tempPosyList.push_back(ypos + j);

			break;
		}
	}

	if(tempPosxList.size() != 0)
	{
		int tempIndex = rand() % tempPosxList.size();
		xpos = tempPosxList[tempIndex];
		ypos = tempPosyList[tempIndex];
	}

	return ;
}

//获取结婚奖励领取情况
DEF_MSG_REQUEST_DEFINE_FUN(MarryMessage,MSG_REQ_WS2GM_GET_MARRY_REWARD)
{
	ServerReturn::ServerDoubleInt tempRequest;
	char* tempStr  = NULL;
	int tempLenght = 0;

	tempStr = message->GetBuffer(tempLenght);
	tempRequest.ParsePartialFromArray(tempStr,tempLenght);

	ServerReturn::ServerRetInt tempAck;

	Smart_Ptr<Player> tempPlayer;
	PlayerMessage::GetInstance()->GetPlayerByCharid(tempRequest.retf(),tempPlayer);
	if(tempPlayer)
	{
		tempAck.set_ret(tempPlayer->GetMarryRing()->getRewardData());
	}

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_GET_MARRY_REWARD,message,&tempAck);
	Message_Facade::Send(messRet);

	return ;
}

//送花扣除物品
DEF_MSG_REQUEST_DEFINE_FUN(MarryMessage,MSG_REQ_WS2GM_BUCKLE_ITEM)
{
	ServerReturn::ServerDoubleInt tempRequest;
	char* tempStr  = NULL;
	int tempLenght = 0;
	ServerReturn::ServerRetInt tempAck;
	Smart_Ptr<Player> tempPlayer;

	tempStr = message->GetBuffer(tempLenght);
	tempRequest.ParsePartialFromArray(tempStr,tempLenght);

	PlayerMessage::GetInstance()->GetPlayerByCharid(tempRequest.retf(),tempPlayer);

	if(tempPlayer)
	{
		int tempCounts = tempPlayer->GetContainer(ePackageType)->FindPropCount(tempRequest.rets());

		//每次只扣一个
		if(tempCounts >= 1)
		{
			BaseArgs tempArgs;

			tempArgs.GoodsID = tempRequest.rets();
			tempArgs.Num     = 1;
			tempPlayer->GetContainer(ePackageType)->DeleteGoods(&tempArgs,true);

			tempAck.set_ret(eRetSuccess);
		}
		//物品不足
		else
		{
			tempAck.set_ret(eItemNotEnough);
		}
	}
	else
	{
		tempAck.set_ret(eNotHaveOnline);
	}

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_BUCKLE_ITEM,message,&tempAck);
	Message_Facade::Send(messRet);

	return ;
}

//强制离婚扣钱
DEF_MSG_REQUEST_DEFINE_FUN(MarryMessage,MSG_REQ_WS2GM_FORCE_DIVORCE_BUCKLE_GLODEN)
{
	ServerReturn::ServerRetInt tempRequest;
	char* tempStr  = NULL;
	int tempLenght = 0;

	tempStr = message->GetBuffer(tempLenght);
	tempRequest.ParsePartialFromArray(tempStr,tempLenght);

	ServerReturn::ServerRetInt tempAck;
	Smart_Ptr<Player> tempPlayer;
	PlayerMessage::GetInstance()->GetPlayerByCharid(tempRequest.ret(),tempPlayer);

	if(tempPlayer)
	{
		const SingleDivorceData& tempData = MarryLoader::GetInstance()->getSingleDivorceData();
		if(tempPlayer->getAttManage()->getValue<int>(eCharGolden) < (int)tempData.m_ForceDivorceGolden)
		{
			tempAck.set_ret(eNotEnoughGold);
		}
		else
		{
			tempPlayer->ChangeGolden(tempData.m_ForceDivorceGolden,false);

			tempAck.set_ret(eRetSuccess);
		}
	}
	else
	{
		tempAck.set_ret(eNotHaveOnline);
	}

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_FORCE_DIVORCE_BUCKLE_GLODEN,message,&tempAck);
	Message_Facade::Send(messRet);

	return ;
}

//不在一条线上请求结婚对方地图信息
DEF_MSG_REQUEST_DEFINE_FUN(MarryMessage,MSG_REQ_WS_GM_MARRY_GET_OTHER_MAP_INFO)
{
	ServerReturn::ServerRetInt tempRequest;
	char* tempStr  = NULL;
	int tempLenght = 0;

	tempStr = message->GetBuffer(tempLenght);
	tempRequest.ParsePartialFromArray(tempStr,tempLenght);

	MarryServer::MarryChangeMap tempAck;

	dealwithOtherChangeMap(tempRequest.ret(),tempAck);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS_GM_MARRY_GET_OTHER_MAP_INFO,message,&tempAck);
	Message_Facade::Send(messRet);

	return ;
}

//成功结婚
DEF_MSG_SIMPLE_DEFINE_FUN(MarryMessage,MSG_SIM_WS2GM_MARRY)
{
	MarryServer::MarrySuccess tempSimpleMsg;
	char* tempStr  = NULL;
	int tempLenght = 0;

	tempStr = message->GetBuffer(tempLenght);
	tempSimpleMsg.ParseFromArray(tempStr,tempLenght);


	Smart_Ptr<Player> tempPlayer;
	PlayerMessage::GetInstance()->GetPlayerByCharid(tempSimpleMsg.selfplayerid(),tempPlayer);

	if(!tempPlayer)
	{
		int64 tempSelfPlayerID  = tempSimpleMsg.selfplayerid();
		int64 tempOtherPlayerID = tempSimpleMsg.otherplayerid();
		LOG_ERROR(FILEINFO,"marry set data but player offline selfPlayerID %lld otherPlayerID %lld",tempSelfPlayerID,tempOtherPlayerID);

		return ;
	}

	tempPlayer->GetMarryRing()->marrySuccess(tempSimpleMsg.otherplayerid(),tempSimpleMsg.othername());

	return ;
}

//强制离婚给钱
DEF_MSG_SIMPLE_DEFINE_FUN(MarryMessage,MSG_SIM_WS2GM_FORCE_DIVORCE_GIVE_GLODEN)
{
	ServerReturn::ServerRetInt tempSimpleMsg;
	char* tempStr  = NULL;
	int tempLenght = 0;

	tempStr = message->GetBuffer(tempLenght);
	tempSimpleMsg.ParseFromArray(tempStr,tempLenght);

	Smart_Ptr<Player> tempPlayer;
	PlayerMessage::GetInstance()->GetPlayerByCharid(tempSimpleMsg.ret(),tempPlayer);

	if(!tempPlayer)
	{
		return ;
	}

	const SingleDivorceData& tempData = MarryLoader::GetInstance()->getSingleDivorceData();

	DWORD tempMoney = tempData.m_ForceDivorceGolden * tempData.m_ForceDivorceRebate / 100;
	tempMoney = tempData.m_ForceDivorceGolden - tempMoney;
	tempPlayer->ChangeGolden(tempMoney,true);

	return ;
}

//离婚成功，清除数据
DEF_MSG_SIMPLE_DEFINE_FUN(MarryMessage,MSG_SIM_WS2GM_DIVORCE_SUCCESS)
{
	ServerReturn::ServerRetInt tempSimpleMsg;
	char* tempStr  = NULL;
	int tempLenght = 0;

	tempStr = message->GetBuffer(tempLenght);
	tempSimpleMsg.ParseFromArray(tempStr,tempLenght);

	Smart_Ptr<Player> tempPlayer;
	PlayerMessage::GetInstance()->GetPlayerByCharid(tempSimpleMsg.ret(),tempPlayer);

	if(!tempPlayer)
	{
		return ;
	}

	tempPlayer->GetMarryRing()->divorceSuccess();

	return ;
}

//请求婚戒信息
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MarryMessage,MSG_REQ_C2GM_REQUEST_RING_INFO)
{
	MarryUI::RingInfo tempAck;

	player->GetMarryRing()->requestRingData(tempAck);

	REQ_MSG_QUICK_ACK_FUN(message, &tempAck, MSG_REQ_C2GM_REQUEST_RING_INFO);

	return ;
}

//升阶
DEF_MSG_QUICK_SIMPLE_DEFINE_FUN(MarryMessage,MSG_SIM_C2GM_RING_UP_STEP)
{
	MarryUI::RequestAdvanceMarryRing tempRequest;
	tempRequest.ParsePartialFromString(content);

	MarryUI::UpMarryStepReturn tempProtoReturn;

	BEHAVIOR_MONI(player, BehaviorType::Main::eMarry, BehaviorType::Minor::eMarry_RingAdvance, MONITOR);

	player->GetMarryRing()->ascendeOrder(tempRequest,tempProtoReturn);

	

	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&tempProtoReturn,MSG_SIM_C2GM_RING_UP_STEP);

	return ;
}

//换图
DEF_MSG_QUICK_SIMPLE_DEFINE_FUN(MarryMessage,MSG_SIM_C2GM_MARRY_CHANGE_MAP)
{
	Smart_Ptr<Player> tempOtherPlayer;
	ServerRet tempError = checkSelfChangeMap(player,tempOtherPlayer);

	if(tempError == eRetSuccess)
	{
		CGameZone* tempZone = tempOtherPlayer->GetGameZone();
		if(tempZone != NULL)
		{
			//调整坐标
			int tempXpos = tempOtherPlayer->GetXNewPos();
			int tempYpos = tempOtherPlayer->GetYNewPos();

			adjustPos(*tempZone,tempXpos,tempYpos);

			PlayerMessage::GetInstance()->ForceChangeMap(player,tempOtherPlayer->GetMapID(),tempXpos,tempYpos);
		}
	}
	//请求世界服，可能不在一条线上
	else if(tempError == eNotHaveOnline)
	{
		Smart_Ptr<SvrItem> tempWorldSvr;
		ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),tempWorldSvr);
		ServerReturn::ServerRetInt tempRequest;
		tempRequest.set_ret(player->GetID());

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GM2WS_MARRY_CHANGE_MAP,&tempRequest,tempWorldSvr.Get(),Request,30);

		messRet->SetAct(new IntAct(player->GetID()));
		Message_Facade::Send(messRet);
	}
	else
	{
		ServerReturn::ServerRetInt tempReturnMsg;

		tempReturnMsg.set_ret(tempError);

		QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&tempReturnMsg,MSG_SIM_C2GM_MARRY_CHANGE_MAP);
	}

	return ;
}

//领取结婚奖励
DEF_MSG_QUICK_SIMPLE_DEFINE_FUN(MarryMessage,MSG_SIM_C2GM_RECEIVE_REWARD)
{
	ServerReturn::ServerRetInt tempRequest;
	tempRequest.ParsePartialFromString(content);

	ServerReturn::ServerRetInt tempReturnMsg;

	player->GetMarryRing()->receiveMarryReward(tempRequest.ret(),tempReturnMsg);

	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&tempReturnMsg,MSG_SIM_C2GM_RECEIVE_REWARD);

	return ;
}

//换图返回
DEF_MSG_ACK_DEFINE_FUN(MarryMessage, MSG_REQ_GM2WS_MARRY_CHANGE_MAP)
{
	IntAct* tempAck = static_cast<IntAct* >(act.Get());
	Smart_Ptr<Player> tempPlayer;
	PlayerMessage::GetInstance()->GetPlayerByCharid(tempAck->mesData,tempPlayer);
	if(!tempPlayer)
	{
		return ;
	}

	ServerRet tempError = eRetSuccess;
	if (message->GetErrno() == eReqTimeOut)
	{
		tempError = eRequestTimeOut;
	}
	else
	{
		char* tempChar = NULL;
		int tempLength = 0;

		tempChar = message->GetBuffer(tempLength);

		MarryServer::MarryChangeMap tempAckMsg;
		tempAckMsg.ParseFromArray(tempChar,tempLength);

		tempError = (ServerRet)tempAckMsg.error();

		//成功不通知客户端
		if(tempError == eRetSuccess)
		{
			PlayerMessage::GetInstance()->ForceChangeMap(tempPlayer,tempAckMsg.mapid(),tempAckMsg.xpos(),tempAckMsg.ypos());

			return ;
		}
	}

	ServerReturn::ServerRetInt tempReturnMsg;

	tempReturnMsg.set_ret(tempError);

	QuickMessage::GetInstance()->AddSendMessage(tempPlayer->GetID(), tempPlayer->GetChannelID(),&tempReturnMsg,MSG_SIM_C2GM_MARRY_CHANGE_MAP);

	return ;
}







