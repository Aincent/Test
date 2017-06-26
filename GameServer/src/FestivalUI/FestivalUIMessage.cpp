/*
 * FestivalUIMessage.cpp
 *
 *  Created on: 16/12/2015
 *      Author: root
 */

#include "FestivalUIMessage.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "ChristmasManage.h"
#include "FileLoader/FestivalUIFile.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Attribute/AttributeCreature.h"
#include "../Container/ContainerBase.h"
#include "MessageStruct/ServerReturn4Int.pb.h"
#include "NewYearActivity.h"
#include "math.h"

FestivalUIMessage::FestivalUIMessage()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(FestivalUIMessage, MSG_REQ_C2G_GET_BREAK_EGG_INFO);

	DEF_MSG_QUICK_REQUEST_REG_FUN(FestivalUIMessage, MSG_REQ_C2G_GET_CHRISTMAS_LOGIN_INFO);

	DEF_MSG_QUICK_REQUEST_REG_FUN(FestivalUIMessage, MSG_REQ_C2G_GET_LIMIT_BUY_INFO);

	DEF_MSG_QUICK_REQUEST_REG_FUN(FestivalUIMessage, MSG_REQ_C2G_EGG_RESLUSH);

	DEF_MSG_QUICK_REQUEST_REG_FUN(FestivalUIMessage, MSG_REQ_C2G_BREAK_EGG);

	DEF_MSG_QUICK_REQUEST_REG_FUN(FestivalUIMessage, MSG_REQ_C2G_RECEIVE_REWARD);

	DEF_MSG_QUICK_REQUEST_REG_FUN(FestivalUIMessage, MSG_REQ_C2G_LIMIT_BUY_REFLUSH);

	DEF_MSG_QUICK_REQUEST_REG_FUN(FestivalUIMessage, MSG_REQ_C2G_LIMIT_BUY_BUY);

	DEF_MSG_QUICK_REQUEST_REG_FUN(FestivalUIMessage, MSG_REQ_C2G_CHRISTMAS_EXCHANGE);

	DEF_MSG_REQUEST_REG_FUN(eGameServer,MSG_REQ_W2GM_CHECK_PACKAGE);

	DEF_MSG_QUICK_REQUEST_REG_FUN(FestivalUIMessage, MSG_REQ_C2GM_NEW_YEAR_LOGIN_DATA);

	DEF_MSG_QUICK_REQUEST_REG_FUN(FestivalUIMessage, MSG_REQ_C2GM_RECEIVE_NEW_YEAR);

	DEF_MSG_QUICK_REQUEST_REG_FUN(FestivalUIMessage, MSG_REQ_C2W_BUY_HORSE_SKIN);

	DEF_MSG_REQUEST_REG_FUN(eGameServer,MSG_W2GM_GET_NEW_YEAR_DATA);
}

FestivalUIMessage::~FestivalUIMessage()
{

}

void FestivalUIMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_SWITCH_TRY_DISPATCH_END
}

void FestivalUIMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_W2GM_CHECK_PACKAGE);

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_W2GM_GET_NEW_YEAR_DATA);

	DEF_SWITCH_TRY_DISPATCH_END
}

void FestivalUIMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FestivalUIMessage,MSG_REQ_C2G_GET_BREAK_EGG_INFO)
{
	if(!FestivalUIFile::GetInstance()->isEggTime())
	{
		return ;
	}

	ChristmasActivity::BreakEggMsg tAck;

	player->GetChristmas()->clientGetEggData(tAck);

	REQ_MSG_QUICK_ACK_FUN(message, &tAck, MSG_REQ_C2G_GET_BREAK_EGG_INFO);

	return ;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FestivalUIMessage,MSG_REQ_C2G_GET_CHRISTMAS_LOGIN_INFO)
{
	if(!FestivalUIFile::GetInstance()->isLoginTime())
	{
		return ;
	}

	ChristmasActivity::ChristmasLoginMsg tAck;

	player->GetChristmas()->clientGetLoginData(tAck);

	REQ_MSG_QUICK_ACK_FUN(message, &tAck, MSG_REQ_C2G_GET_CHRISTMAS_LOGIN_INFO);

	return ;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FestivalUIMessage,MSG_REQ_C2G_GET_LIMIT_BUY_INFO)
{
	if(!FestivalUIFile::GetInstance()->isLimitBuyTime())
	{
		return ;
	}

	ChristmasActivity::ChristmasLimitBuyInfo tAck;

	player->GetChristmas()->clientGetLimitData(tAck);

	REQ_MSG_QUICK_ACK_FUN(message, &tAck, MSG_REQ_C2G_GET_LIMIT_BUY_INFO);

	return ;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FestivalUIMessage,MSG_REQ_C2G_EGG_RESLUSH)
{
	if(!FestivalUIFile::GetInstance()->isEggTime())
	{
		return ;
	}

	ServerRet tError = player->GetChristmas()->reflushEggTime();

	ServerReturn::ServerRetInt tAck;
	tAck.set_ret(tError);

	REQ_MSG_QUICK_ACK_FUN(message, &tAck, MSG_REQ_C2G_EGG_RESLUSH);

	return ;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FestivalUIMessage,MSG_REQ_C2G_BREAK_EGG)
{
	if(!FestivalUIFile::GetInstance()->isEggTime())
	{
		return ;
	}

	ServerReturn::ServerRetInt tRequest;
	ChristmasActivity::BreakEggReturn tAck;

	tRequest.ParsePartialFromString(content);

	player->GetChristmas()->breakEgg(tRequest.ret(),tAck);

	REQ_MSG_QUICK_ACK_FUN(message, &tAck, MSG_REQ_C2G_BREAK_EGG);

	return ;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FestivalUIMessage,MSG_REQ_C2G_RECEIVE_REWARD)
{
	if(!FestivalUIFile::GetInstance()->isLoginTime())
	{
		return ;
	}

	ServerReturn::ServerRetInt tRequest;
	ServerReturn::ServerRetInt tAck;

	tRequest.ParsePartialFromString(content);

	ServerRet tError = player->GetChristmas()->receiveLoginReward(tRequest.ret());

	tAck.set_ret(tRequest.ret() << 32 | tError);

	REQ_MSG_QUICK_ACK_FUN(message, &tAck, MSG_REQ_C2G_RECEIVE_REWARD);

	return ;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FestivalUIMessage,MSG_REQ_C2G_LIMIT_BUY_REFLUSH)
{
	if(!FestivalUIFile::GetInstance()->isLimitBuyTime())
	{
		return ;
	}

	ChristmasActivity::LimitBuyReturn tAck;

	player->GetChristmas()->clientReflushLimit(tAck);

	REQ_MSG_QUICK_ACK_FUN(message, &tAck, MSG_REQ_C2G_LIMIT_BUY_REFLUSH);

	return ;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FestivalUIMessage,MSG_REQ_C2G_LIMIT_BUY_BUY)
{
	if(!FestivalUIFile::GetInstance()->isLimitBuyTime())
	{
		return ;
	}

	ServerReturn::ServerRetInt tRequest;
	ServerReturn::ServerRetInt tAck;

	tRequest.ParsePartialFromString(content);

	ServerRet tError = player->GetChristmas()->buyLimitItem(tRequest.ret());

	tAck.set_ret(tRequest.ret() << 32 | tError);

	REQ_MSG_QUICK_ACK_FUN(message, &tAck, MSG_REQ_C2G_LIMIT_BUY_BUY);

	return ;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FestivalUIMessage,MSG_REQ_C2G_CHRISTMAS_EXCHANGE)
{
	if(!FestivalUIFile::GetInstance()->isExchangeTime())
	{
		return ;
	}

	ServerReturn::ServerRetInt tRequest;
	ServerReturn::ServerRetInt tAck;

	tRequest.ParsePartialFromString(content);

	ServerRet tError = player->GetChristmas()->clientExchangeItem(tRequest.ret());

	tAck.set_ret(tRequest.ret() << 32 | tError);

	REQ_MSG_QUICK_ACK_FUN(message, &tAck, MSG_REQ_C2G_CHRISTMAS_EXCHANGE);

	return ;
}

DEF_MSG_REQUEST_DEFINE_FUN(FestivalUIMessage,MSG_REQ_W2GM_CHECK_PACKAGE)
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
		const NewYearLimitData* tData = FestivalUIFile::GetInstance()->getLimitData(tempRequest.rets());
		if(tData == NULL)
		{
			tempAck.set_ret(eSystemError);
		}
		else
		{
			if(tempPlayer->GetContainer(ePackageType)->IsBagEnought(tData->m_ItemID,tData->m_Counts,tData->m_Flag) != 0)
			{
				tempAck.set_ret(ePackageIsFull);
			}
			else
			{
				int tGolden = (int)ceil((double)tData->m_Golden *  tData->m_PersentDiscount / 100);
				if(tGolden > tempPlayer->getAttManage()->getValue<int>(eCharGolden))
				{
					tempAck.set_ret(eNotEnoughGold);
				}
				else
				{
					tempAck.set_ret(eRetSuccess);

					vector<int> tVecPos;
					tempPlayer->GetContainer(ePackageType)->AddItem(tData->m_ItemID,tData->m_Counts,tVecPos,tData->m_Flag);
					tempPlayer->ChangeGolden(tGolden,false);

					vector<int> tAtt;
					tAtt.push_back(eCharGolden);
					tempPlayer->SynCharAttribute(tAtt);
				}
			}
		}
	}
	else
	{
		tempAck.set_ret(eNotHaveOnline);
	}

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_W2GM_CHECK_PACKAGE,message,&tempAck);
	Message_Facade::Send(messRet);

	return ;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FestivalUIMessage,MSG_REQ_C2GM_NEW_YEAR_LOGIN_DATA)
{
	if(FestivalUIFile::GetInstance()->getCurrentUIType(eActivityUILogin) == eFestivalUINULL)
	{
		return ;
	}

	ServerReturn::ServerFourInt tRetrunData;

	player->GetNewYear()->clientGetLoginData(tRetrunData);

	REQ_MSG_QUICK_ACK_FUN(message, &tRetrunData, MSG_REQ_C2GM_NEW_YEAR_LOGIN_DATA);

	return ;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FestivalUIMessage,MSG_REQ_C2GM_RECEIVE_NEW_YEAR)
{
	if(FestivalUIFile::GetInstance()->getCurrentUIType(eActivityUILogin) == eFestivalUINULL)
	{
		return ;
	}

	ServerReturn::ServerRetInt tempRequest;
	tempRequest.ParseFromString(content);

	ServerRet tError = player->GetNewYear()->receiveLoginReward(tempRequest.ret());

	tempRequest.set_ret(tempRequest.ret() << 32 | tError);

	REQ_MSG_QUICK_ACK_FUN(message, &tempRequest, MSG_REQ_C2GM_RECEIVE_NEW_YEAR);

	return ;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FestivalUIMessage,MSG_REQ_C2W_BUY_HORSE_SKIN)
{
	if(FestivalUIFile::GetInstance()->getCurrentUIType(eActivityUILogin) == eFestivalUINULL)
	{
		return ;
	}

	ServerReturn::ServerRetInt tempRequest;
	tempRequest.ParseFromString(content);

	int tSKinID = 0;
	ServerRet tError = player->GetNewYear()->buyHorseReward(tSKinID);

	tempRequest.set_ret((int64)tSKinID << 32 | tError);

	REQ_MSG_QUICK_ACK_FUN(message, &tempRequest, MSG_REQ_C2W_BUY_HORSE_SKIN);

	return ;
}

DEF_MSG_REQUEST_DEFINE_FUN(FestivalUIMessage,MSG_W2GM_GET_NEW_YEAR_DATA)
{
	char* tempStr  = NULL;
	int tempLenght = 0;
	ServerReturn::ServerRetInt tempRequest;
	Smart_Ptr<Player> tempPlayer;

	tempStr = message->GetBuffer(tempLenght);
	tempRequest.ParsePartialFromArray(tempStr,tempLenght);

	PlayerMessage::GetInstance()->GetPlayerByCharid(tempRequest.ret(),tempPlayer);

	if(tempPlayer)
	{
		tempRequest.set_ret(tempPlayer->GetNewYear()->getHorseData());
	}

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_W2GM_GET_NEW_YEAR_DATA,message,&tempRequest);
	Message_Facade::Send(messRet);

	return ;
}


