/*
 * MailMessage.cpp
 *
 *  Created on: 2014年6月17日
 *      Author: root
 */

#include "MallMessage.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "../Object/Player/PlayerMessage.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/ServerReturn3Int.pb.h"
#include "CharDefine.h"
#include "MessageCommonRet.h"
#include "../FileLoader/MallLoader.h"
#include "FightDefined.h"
#include "EventSet/EventArgs.h"
#include "EventSet/EventSet.h"
#include "../ServerEventDefine.h"
#include "../VIP/PlayerVipPart.h"
#include "./FileLoader/GameParaLoader.h"
#include "MessageStruct/MSProtoc/MS_Mall.pb.h"
#include "../StatisticMgr/StatisticHandle.h"

MallMessageMgr::MallMessageMgr()
{
//	DEF_MSG_QUICK_REQUEST_REG_FUN(MallMessageMgr, MSG_REQ_C2GT_MALLINFO);		//请求商城信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(MallMessageMgr, MSG_REQ_C2GT_BUYGOODS);		//请求购买商品

//	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_GT2GM_BUYLIMITGOODS);
//	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2GT_BUYLIMITGOODS);
//	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2WS_BUYLIMITGOODS);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_MALL_LIMIT_BUY);
}

MallMessageMgr::~MallMessageMgr()
{

}
void MallMessageMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
//	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GT2GM_BUYLIMITGOODS);
	DEF_SWITCH_TRY_DISPATCH_END
}

void MallMessageMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_MALL_LIMIT_BUY);
	DEF_SWITCH_TRY_DISPATCH_END
}

void MallMessageMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
//	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2GT_BUYLIMITGOODS);
//	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2WS_BUYLIMITGOODS);
	DEF_SWITCH_TRY_DISPATCH_END
}

//DEF_MSG_SIMPLE_DEFINE_FUN(MallMessageMgr, MSG_SIM_GT2GM_BUYLIMITGOODS)
//{
//
//}

//DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MallMessageMgr,MSG_REQ_C2GT_MALLINFO)
//{
//	LOG_DEBUG(FILEINFO, "client req gt get mall");
//}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MallMessageMgr,MSG_REQ_C2GT_BUYGOODS)
{
	LOG_DEBUG(FILEINFO, "client req gt buy goods");
	ServerReturn::ServerDoubleInt req;
	ServerReturn::ServerRetInt ret;
	MallGoodsInfo info;

	req.ParseFromString(content);

	int want_id = req.retf() & 0xffffffff, want_num = req.retf() >>32, buyType = req.rets();

	BEHAVIOR_MONI(player, BehaviorType::Main::eMall, BehaviorType::Minor::eMall_Buy, MONITOR);
	int err = MallBuy(player, want_id, want_num, buyType);
	if (-1 != err)
	{
		ret.set_ret(err);
		REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GT_BUYGOODS);
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(MallMessageMgr, MSG_REQ_WS2GM_MALL_LIMIT_BUY)
{
	MS_Mall::ReqBuyLimitGoods  req;
	MS_Mall::RetBuyLimitGoods  toWS;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;
	int ret = 0;

	con = message->GetBuffer(len);
	req.ParseFromArray(con, len);

	PlayerMessage::GetInstance()->GetPlayerByCharId(player,req.charid());
	if(bool(player))
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eMall, BehaviorType::Minor::eMall_BuyLimitGoods, MONITOR);;

		switch(req.buytype())
		{
		case MallType_TimeLimit:
			{
				int needMoney = req.count() * req.currentprice();
				ECharAttrType moneyType = (ECharAttrType)GameParaLoader::GetInstance()->GetAtrrTypeByExchangeItemId(req.moneytype());
				if(0 == moneyType)
				{
					ret = eMoneyTypeError;
				}

				if(player->getAttManage()->getValue<int>(moneyType) < needMoney)
				{
					if(moneyType == eCharBindGolden)
						ret = eNotEnogBindGolden;
					else if(moneyType == eCharGolden)
						ret = eNotEnogGolden;
					else if(moneyType == eCharMoney)
						ret = eMoneyNotEnough;
					else if(moneyType == eCredit)
						ret = eNotEnoughCredit;
					else
						ret = eMoneyTypeError;
				}

				DWORD itemID = req.itemid();
				DWORD count = req.count();
				DWORD bindType = req.bindtype();

				if(0 == ret)
					ret = player->GetContainer(ePackageType)->IsBagEnought(itemID,count,bindType);

				if(0 == ret)
				{
					vector<int> tempPos;
					vector<DWORD> tempStreghtList;
					ret = player->GetContainer(ePackageType)->AddItem(itemID,count,tempPos,bindType,0);
				}

				if(0 == ret)
				{
					toWS.set_itemid(req.itemid());
					toWS.set_count(req.count());
					toWS.set_buytype(req.buytype());

					vector<int> vecType;

					int currentValue = player->getAttManage()->getValue<int>(moneyType) - needMoney;
					player->getAttManage()->setValue(moneyType,currentValue);

					vecType.push_back(moneyType);
					player->SynCharAttribute(vecType);
					//完成活跃次数
					player->GetVitalityMgr()->FinshVitality(eViMallShop);

					BuyItemEventArgs e(player,req.itemid(),req.count(), MallType_TimeLimit);
					player->FireEvent(PLAYER_BUYITEM_FROMMALL, e);
				}


				break;
			}
		case MallType_DayLimit:
		case MallType_WeekLimit:
			{
				int want_id = req.itemid(), want_num = req.count(), buyType = req.buytype();

				ret = MallBuy(player, want_id, want_num, buyType);
				if(ret == 0)
				{
					toWS.set_itemid(req.itemid());
					toWS.set_count(req.count());
					toWS.set_buytype(req.buytype());
				}

				break;
			}
		default:
			break;
		}


		toWS.set_ret(ret);
		toWS.set_charid(req.charid());
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(message,&toWS,Ack);
		Message_Facade::Send(messRet);
	}
}

int MallMessageMgr::MallBuy(Smart_Ptr<Player>& player, int& want_id, int& want_num, int& buyType)
{
	MallGoodsInfo info;
	MallLoader::GetInstance()->GetItemTypeDataByID(want_id, buyType, info);
	if(info.id <=0 )
		return -1;

	if(want_num <= 0)
		return -1;

	//判断背包是否已满
	vector<DWORD> itemVec;
	vector<DWORD> numVec;

	vector<int> vecType;
	vector<int> vecLock;
	vector<DWORD> vec;

	itemVec.push_back(want_id);
	numVec.push_back(want_num);

	int errNum = 0;
	switch (buyType)
	{
	case MallType_Common:						//常用道具
	case MallType_UPgrade:						//升级强化
	case MallType_EquipStrength:						//装备打造
	case MallType_Credit:
	case MallType_HotSell://热卖商品
	case MallType_DayLimit:	//每日限购
	case MallType_WeekLimit: //每周限购
		{
			errNum = player->GetContainer(ePackageType)->IsBagEnought(want_id, want_num,1);
		}
		break;
	default:
		{
		}
		break;
	}
	if(errNum != 0)
	{
		return errNum;
	}

	switch(buyType)
	{
		case MallType_Common:						//常用道具
		case MallType_UPgrade:						//升级强化
		case MallType_EquipStrength:						//装备打造
		case MallType_HotSell:
		case MallType_DayLimit:	//每日限购
		case MallType_WeekLimit: //每周限购
		{
			int golden = player->getAttManage()->getValue<int>(eCharGolden);

			int price = ((info.price/10)*info.discountPrice) * (want_num);
			//VIP算折扣价
//			Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//			if ((bool)vip)
//			{
//				price = PlayerVipPart::CalBuyCostGolden(vip, info.price * (want_num));
//			}



			golden -= price;

			if(golden<0)
			{
				return eNotEnogGolden;
			}
			else
			{
				//完成活跃次数
//				player->GetVitalityMgr()->FinshVitality(eViMallShop);

//				StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,price,player);

				player->ChangeGolden(price, false);
				vecType.push_back(eCharGolden);
				player->SynCharAttribute(vecType);

				vector<DWORD> strengthLvVec;
				vector<WORD> bindVec;

				strengthLvVec.resize(itemVec.size(), 0);
				bindVec.resize(itemVec.size(),1);

				player->GetContainer(ePackageType)->AddItem(itemVec, numVec, vecLock, bindVec,strengthLvVec,true,shop_buy_type);
				//触发事件
				BuyItemEventArgs e(player,info.id, want_num, buyType);
				player->FireEvent(PLAYER_BUYITEM_FROMMALL, e);
			}
			break;
		}
		case MallType_Credit:
		{
			int credit = player->getAttManage()->getValue<int>(eCredit);
			credit -= info.price *(want_num);

			if(credit<0)
			{
				return eNotEnoughCredit;
			}
			else
			{
				//完成活跃次数
//				player->GetVitalityMgr()->FinshVitality(eViMallShop);
				player->ChangeCredit(info.price *(want_num), false);
				vecType.push_back(eCredit);
				player->SynCharAttribute(vecType);

				vector<DWORD> strengthLvVec;
				vector<WORD> bindVec;

				strengthLvVec.resize(itemVec.size(), 0);
				bindVec.resize(itemVec.size(),1);
				player->GetContainer(ePackageType)->AddItem(itemVec, numVec, vecLock, bindVec,strengthLvVec,true,shop_buy_type);
				BuyItemEventArgs e(player,info.id, want_num, buyType);
				player->FireEvent(PLAYER_BUYITEM_FROMMALL, e);
			}

			break;
		}
		default:
		{
			break;
		}
	}

	return 0;
}
