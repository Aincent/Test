/*
 * ContainerMessage.cpp
 *
 *  Created on: 2014幄1�7�1�7旄1�7
 *      Author: root
 */


#include "ContainerMessage.h"
#include "Log/Logger.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "./MessageStruct/CharBattle/Battle.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "../Map/SceneManager.h"
#include "../Object/Player/PlayerMessage.h"
#include "MessageStruct/CharLogin/EquipMessage.pb.h"
#include "funUse.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "EventSet/EventArgs.h"
#include "EventSet/EventSet.h"
#include "../ServerEventDefine.h"
#include "ContailerLoader/ContailerLoader.h"
#include "MessageCommonRet.h"
#include "ArticleEquipMgr.h"
#include "../CDTime/CDMgr.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "FileLoader/GameParaLoader.h"
#include "../StatisticMgr/StatisticHandle.h"


ContainerMessage * ContainerMessage::m_instance = 0;


ContainerMessage::ContainerMessage()
{
	//请求使用物品
//	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_USEGOODS);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_MOVEGOODS);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_DISCARDGOODS);
//	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_ADDBACKPACKSIZE);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_SPLITGOODS);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_GT2GM_SORTBACKPACK);
//	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GM2DB_PLAYERINFO);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_MOVEQUICKSECTOR);
	//请求使用物品
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_WEAREQUIP);
//	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_RECOVEREQUIP);
	//请求使用物品
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_USEGOODS);

	DEF_MSG_REQUEST_REG_FUN(eGameServer,MSG_REQ_GT2GM_ADDBACKPACKSIZE);
}

ContainerMessage::~ContainerMessage()
{

}

void ContainerMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_GT2GM_SORTBACKPACK);

	DEF_SWITCH_TRY_DISPATCH_END
}

void ContainerMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

//	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_USEGOODS);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_MOVEGOODS);
//	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_ADDBACKPACKSIZE);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_DISCARDGOODS);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_MOVEQUICKSECTOR);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_USEGOODS);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_WEAREQUIP);
//	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_RECOVEREQUIP);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_SPLITGOODS);

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_ADDBACKPACKSIZE);

	DEF_SWITCH_TRY_DISPATCH_END
}

void ContainerMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

//	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2DB_PLAYERINFO);

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_REQUEST_DEFINE_FUN(ContainerMessage, MSG_REQ_GT2GM_MOVEGOODS)
{
	Goods::ClientReqMoveGoods reqGM;
//	Goods::GTtoGMMoveGoods reqGM;
	Goods::MoveUpdateGoodsRet toClient;
	vector<DWORD> vec;
	Smart_Ptr<Player> player;
	int len = 0;
	int res = 0;

	char *str = message->GetBuffer(len);
	reqGM.ParseFromArray(str, len);

	int64 charID = (int64)(message->GetMessageTime());

	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);

	if((bool)player)
	{
		if(!CalContainerType(reqGM.startcontainer()))
			return ;
		if(!CalContainerType(reqGM.endcontainer()))
			return ;

		if(!player->GetContainer(reqGM.startcontainer())->IsValidPos(reqGM.startpos()) ||
				!player->GetContainer(reqGM.endcontainer())->IsValidPos(reqGM.endpos()))
		{
			return;
		}
		
		BEHAVIOR_MONI(player, BehaviorType::Main::ePackage, BehaviorType::Minor::ePackage_MoveGoods, MONITOR);;

		if(!player->IsOpenSystem(eStPackage))
		{
			res = eNotOpen;
		}
		else if(reqGM.startcontainer() == reqGM.endcontainer())
		{
			res = player->GetContainer(reqGM.startcontainer())->MoveGoods(reqGM.type(), reqGM.startcontainer(),reqGM.startpos(), reqGM.endcontainer(), reqGM.endpos(), vec);
		}
		else
		{
			if(reqGM.startcontainer() == eStoreHouseType && reqGM.endcontainer() == ePackageType && player->GetContainer(eStoreHouseType)->isTimeOut(reqGM.startpos()))
			{
				res = 0;
				player->GetContainer(eStoreHouseType)->ClearGrideItemInfo(reqGM.startpos());
				vec.push_back((eStoreHouseType << 16) | reqGM.startpos());
				player->SetDataFlag(eStoreHouseType);
			}
			else
			{
				res = player->GetContainer(reqGM.startcontainer())->MoveGoods(reqGM.type(), reqGM.startcontainer(),reqGM.startpos(), reqGM.endcontainer(), vec);
			}
		}

		toClient.set_status(res);
		vector<DWORD>::iterator it = vec.begin();
		for (; it != vec.end(); ++it)
		{
			Goods::UpdateGoods * item = toClient.add_updategoods();
			CharLogin::ClientItemInfo *clientInfo = item->mutable_iteminfo();

			item->set_container((*it) >> 16);
			item->set_pos((*it) & 0xffff);

			player->GetContainer((*it) >> 16)->GetItemDataPtr((*it) & 0xffff)->SetClientInfo(clientInfo);
		}

		

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_MOVEGOODS, message, &toClient);
		Message_Facade::Send(messRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request attack but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(ContainerMessage, MSG_REQ_GT2GM_DISCARDGOODS)
{
//	Goods::GTtoGMRecoverGoods reqGM;
	Goods::ClientReqRecoverGoods reqGM;
	Goods::DiscardUpdateGoodsRet Ret;

	Smart_Ptr<Player> player;
	int len = 0;
	char *str = message->GetBuffer(len);

	reqGM.ParseFromArray(str, len);

	int64 charID = (int64)(message->GetMessageTime());

	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{

		if(!CalContainerType(reqGM.container()))
			return ;
		if(player->IsOpenSystem(eStPackage))
		{
			Smart_Ptr<ArticleParcelMgr> parcelMgr = player->GetContainer(ePackageType);
			if (parcelMgr)
			{
				int goodsID = 0, num = 0, err = 0;
				parcelMgr->DiscardGoods(ePackageType, reqGM.pos(), goodsID, num, err);
				Ret.set_goodsid(goodsID);
				Ret.set_discardnum(num);
				Ret.set_status(err);

				if (0 == err)
				{
					Goods::UpdateGoods* goods = Ret.mutable_updategoods();
					CharLogin::ClientItemInfo* itemInfo = goods->mutable_iteminfo();

					parcelMgr->GetClientItemInfo(reqGM.pos(), itemInfo);

					goods->set_container(ePackageType);
					goods->set_pos(reqGM.pos());
				}
			}
		}
		else
		{
			Ret.set_status(eNotOpen);
		}

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_DISCARDGOODS, message, &Ret);
		Message_Facade::Send(messRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request attack but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
	}
}


DEF_MSG_REQUEST_DEFINE_FUN(ContainerMessage, MSG_REQ_GT2GM_MOVEQUICKSECTOR)
{
//	Goods::CTReqGMMoveQuickSector reqGM;
	Goods::ClientReqMoveQuickSector reqGM;
	Goods::GTAckToCMoveQuickSectorRet Ret;

	Smart_Ptr<Player> player;
	int len = 0;
	char *str = message->GetBuffer(len);

	reqGM.ParseFromArray(str, len);

	int64 charID = message->GetMessageTime();

	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{
		/*由于 没用到 客户端发过来的 container 故不加判断*/
		Smart_Ptr<ArticleEquipMgr> equipMgr = player->GetContainer(eEquipQuickType);
		if(equipMgr)
		{
			int status = 0;
			if(player->IsOpenSystem(eStPackage))
			{
				status = equipMgr->MoveQuickSector(reqGM.type(), reqGM.id(), reqGM.pos(), reqGM.container());
			}
			else
			{
				status = eNotOpen;
			}
			Ret.set_type(reqGM.type());
			Ret.set_id(reqGM.id());
			Ret.set_pos(reqGM.pos());
			Ret.set_container(reqGM.container());
			Ret.set_status(status);
			Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_MOVEQUICKSECTOR, message, &Ret);
			Message_Facade::Send(messRet);
		}
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request attack but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
	}
}

DEF_MSG_SIMPLE_DEFINE_FUN(ContainerMessage, MSG_SIM_GT2GM_SORTBACKPACK)
{
//	Goods::GTtoGMArrangeGoods reqGM;
	Goods::ClientReqArrangeGoods reqGM;
	Smart_Ptr<Player> player;
	int len = 0;
	char *str = message->GetBuffer(len);

	reqGM.ParseFromArray(str, len);
	int type = reqGM.container();
	if(!CalContainerType(type))
		return ;

	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{
		if(player->IsOpenSystem(eStPackage))
		{
			if(player->GetCDMgr()->IsCDEnd(ARRANGE_BAG))
			{
				player->GetCDMgr()->StartCD(ARRANGE_BAG, GameParaLoader::GetInstance()->getArrangeBagCdTime());

				Smart_Ptr<ArticleBaseMgr> bagMgr = player->GetContainer(type);
				if(bagMgr)
				{
					bagMgr->ArrangeContainer(type);

					player->GetContainer(type)->SendArticleInfo();
				}
			}
		}
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player sort back pack but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(ContainerMessage, MSG_REQ_GT2GM_USEGOODS)
{
//	Goods::GTtoGMUseGoods reqGM;
	Goods::ClientReqUseGoods reqGM;

	Smart_Ptr<Player> player;
	int len = 0;
	char *str = message->GetBuffer(len);
	reqGM.ParseFromArray(str, len);

	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);

	if((bool)player)
	{
		Smart_Ptr<ArticleBaseMgr> bagMgr = player->GetContainer(ePackageType);
		if(bagMgr)
		{
			if(!bagMgr->IsValidPos(reqGM.pos()) && reqGM.pos() != Comm_Pos_Num)
				return ;
			if(reqGM.num() <=0)
				return ;

			BEHAVIOR_MONI(player, BehaviorType::Main::ePackage, BehaviorType::Minor::ePackage_UseGoods, MONITOR);;

			BaseArgs checkArgs;

			checkArgs.GoodsID = reqGM.goodid();
			checkArgs.Num = reqGM.num();
			checkArgs.Pos = reqGM.pos();

			ServerReturn::ServerDoubleInt ack;

			int64 goodID = int64(reqGM.goodid());

			int res = bagMgr->UseGoods(&checkArgs);

			ack.set_retf(goodID << 32 | res);
			if(!res)
			{
				ack.set_rets(reqGM.num());
				//响应使用物品事件
				if(player->GetMyself())
				{
					UseItemEventArgs e(player->GetMyself(),reqGM.goodid(),reqGM.num());
					player->FireEvent(PLAYER_ITEM_USEITEM, e);
				}
			}
			else
			{
				ack.set_rets(0);
			}

			
			Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_USEGOODS, message, &ack);
			Message_Facade::Send(messRet);
		}

	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request use goods but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(ContainerMessage, MSG_REQ_GT2GM_WEAREQUIP)
{
//	Equip::GTReqGMWearEquip reqGM;
	Equip::ClientReqWearEquip reqGM;

	int len = 0;
	char *str = message->GetBuffer(len);
	reqGM.ParseFromArray(str, len);

	Smart_Ptr<Player> player;
	int64 charID = message->GetMessageTime();

	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);

	if((bool)player)
	{
		Smart_Ptr<ArticleEquipMgr> bagMgr = player->GetContainer(eEquipQuickType);
		if(bagMgr)
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eEquip, BehaviorType::Minor::eEquip_Wear, MONITOR);;

			ServerReturn::ServerDoubleInt ack;

			int res = bagMgr->WearEquip(reqGM.type(), reqGM.pos(), reqGM.goodsid());

			ack.set_retf(reqGM.type());
			ack.set_rets(res);

			

			Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_WEAREQUIP, message, &ack);
			Message_Facade::Send(messRet);
		}
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request wear equip but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
	}
}


DEF_MSG_REQUEST_DEFINE_FUN(ContainerMessage, MSG_REQ_GT2GM_SPLITGOODS)
{
//	Goods::GT2GMSplitGoods reqGM;
	Goods::SplitGoods reqGM;
	Goods::SplitUpdateGoodsRet toClient;
	Smart_Ptr<Player> player;
	int len = 0;
	char *str = message->GetBuffer(len);

	reqGM.ParseFromArray(str, len);

	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{
		Smart_Ptr<ArticleBaseMgr> bagMgr = player->GetContainer(ePackageType);
		if(bagMgr)
		{
			vector<DWORD> vec;

			int res = bagMgr->SplitGoods(ePackageType, reqGM.pos(), reqGM.num(), vec);

			vector<DWORD>::iterator it = vec.begin();
			for (; it != vec.end(); ++it)
			{
				Goods::UpdateGoods * item = toClient.add_updategoods();
				CharLogin::ClientItemInfo *clientInfo = item->mutable_iteminfo();

				item->set_container((*it) >> 16);
				item->set_pos((*it) & 0xffff);

				player->GetContainer((*it) >> 16)->GetItemDataPtr((*it) & 0xffff)->SetClientInfo(clientInfo);
			}
			toClient.set_status(res);
			Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_SPLITGOODS, message, &toClient);
			Message_Facade::Send(messRet);
		}
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request recover equip but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
	}
}


DEF_MSG_REQUEST_DEFINE_FUN(ContainerMessage, MSG_REQ_GT2GM_ADDBACKPACKSIZE)
{
	Goods::CReqToGTOpenGoodsRet reqGM;
	Goods::GTAckToCOpenGoodsRet toClient;

	int len = 0;
	char *str = message->GetBuffer(len);
	reqGM.ParseFromArray(str, len);

	Smart_Ptr<Player> player;
	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::ePackage, BehaviorType::Minor::ePackage_AddPack, MONITOR);
		AddBackpackSize(player,toClient,reqGM);

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_ADDBACKPACKSIZE, message, &toClient);
		Message_Facade::Send(messRet);

		
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request ADDBACKPACKSIZE but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(charID));
	}
}
void ContainerMessage::AddBackpackSize(Smart_Ptr<Player> &player,Goods::GTAckToCOpenGoodsRet &toClient,Goods::CReqToGTOpenGoodsRet &reqGM)
{
	vector<int> vecType;

	if(ePackageType != reqGM.container() && eStoreHouseType != reqGM.container())
		return;

	Smart_Ptr<ArticleBaseMgr> bagMgr = player->GetContainer(reqGM.container());
	if(!bagMgr)
		return;

	int golden = ContailerLoader::GetInstance()->GetGoldenByNum(bagMgr->GetSize(),reqGM.num(),reqGM.container());

	if(player->getAttManage()->getValue<int>(eCharGolden) < golden)
	{
		toClient.set_status(eNotEnogGolden);
	}
	else
	{
//			StatisticInfo::CostAndGotInfo costAndGotInfo;
//			StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
//			StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,golden,player);

			player->ChangeGolden(golden,false);

//			StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,player);

			vecType.push_back(eCharGolden);
			player->SynCharAttribute(vecType);

//			StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotItemInfo,0,reqGM.num());

			bagMgr->AddBackPackSize(reqGM.container(),reqGM.num(),toClient);
			bagMgr->ReStartTIme();

//			if(eAccountType_User == player->GetAccountType())
//				StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Package,eStaMinor_Package_BuyCellEvent,&costAndGotInfo);
			// 统计元宝花费
			//StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(player, eStatic_AboutGolden, eStaMinor_AboutGolden_AddBackpackSize,golden);
	}
}
