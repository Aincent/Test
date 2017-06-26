/*
 *  ShopMgr.cpp
 *
 *  Created on: 2014.5.8
 *
 *  Author: cz
 */
//////////////////////////////////////////////////////////////////////////

#include "shop.h"
#include "../ServerManager.h"
#include "../MessageBuild.h"
#include "FileLoader/GoodsLoader.h"
#include "../Object/NPC/NPC.h"
#include "../Object/Player/Player.h"
#include "../Map/GameZone.h"
#include "../Map/SceneManager.h"
#include "Network/MessageManager.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/CharLogin/NpcTrade.pb.h"
#include "MessageCommonRet.h"
#include "FightDefined.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "../ServerEventDefine.h"


//*********************************** ShopMgr **************************************

//-----------------------------------------------------------------------------
// 构造&析构函数
//-----------------------------------------------------------------------------
ShopMgr * ShopMgr::m_instance = 0;

ShopMgr::ShopMgr():m_pShopProto(NULL)
{
	m_mapBuyBackItems.clear();
	m_logoutEvent = RegGlobalEventPtr(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, this,&ShopMgr::ClearMyselfBackItems);
}

ShopMgr::~ShopMgr()
{
	ClearBuyBackItems();

	if(m_logoutEvent.IsValid())
	{
		m_logoutEvent.SetNull();
	}
}


int ShopMgr::BuyItem(Smart_Ptr<Player>& pPlayer, DWORD dwNpcID, DWORD dwItemID, DWORD dwItemNum)
{
	if(!pPlayer)
		return eNull;

   if (dwItemNum < 1)
	   dwItemNum = 1;

   if (pPlayer->IsDead())
	    return eCreatureDead;

   const GoodsInfo* itemInfo = GoodsLoader::GetInstance()->GetItemDataByID(dwItemID);
   if(itemInfo == NULL)
    	{
	   return eCant_find_item;
    	}

  if(dwNpcID < 0)
	  return eRetNpcNotExist;

	DWORD price = 0;
	if(itemInfo->price > 0)
	{
		price = itemInfo->price * dwItemNum;
		uint hadMoney =  pPlayer->GetTypeValue(eCharMoney);
		if(hadMoney < price)
		{
		   return eNotEnoghMoney;
		}

		Smart_Ptr<ArticleBaseMgr> pPacket = pPlayer->GetContainer(ePackageType);

		vector<int> tempPosList;
		int ret = pPacket->AddItem(dwItemID,dwItemNum,tempPosList,0,1,true,npc_fly_type);
		if(ret != 0)
			 return ret;
		else
		{
			std::vector<int> vecType;
			vecType.push_back(eCharMoney);
			pPlayer->ChangeMoney(price, false);
			pPlayer->SynCharAttribute(vecType);
			//统计NPC商店中买药花费的金钱
//			StatisticMgr::GetInstance()->StatisticPlayerGetItems(pPlayer,eStatic_Package,eStaMinor_Package_BuyDrug,dwItemNum,price);

			//响应购买物品事件
			if(pPlayer->GetMyself())
			{
				BuyItemEventArgs e(pPlayer->GetMyself(),itemInfo->itemID,dwItemNum,0);
				pPlayer->FireEvent(PLAYER_BUYITEM_FORMSHOP, e);
			}
		}
	}
	else
		return eItemError;

	return 0;

}

int ShopMgr::SellItem(Smart_Ptr<Player>& pPlayer, DWORD dwPacketPos,CharLogin::ClientItemInfo *info)
{
	if(!pPlayer)
		return eNull;
    //
   if (dwPacketPos < 0)
	   return eBagPosError;


   if (pPlayer->IsDead())
	    return eCreatureDead;

   Smart_Ptr<ArticleBaseMgr> pPacket = pPlayer->GetContainer(ePackageType);
   if(!pPacket)
   	{
   		return eNull;
   	}

   Smart_Ptr<PropBase> itemInfo = pPacket->GetItemDataPtr(dwPacketPos);
   if(!itemInfo)
   {
	   return eNull;
   }
   if(itemInfo->GetID() < 0 || pPacket->IsLock(dwPacketPos))
   	   {
	   return eCant_find_item;
   	   }

   const GoodsInfo* GoodsInfo = GoodsLoader::GetInstance()->GetItemDataByID(itemInfo->GetID());
   if(GoodsInfo==NULL || GoodsInfo->salePrice<=0)
   	   {
	   return eItemError;
   	   }

//   StatisticInfo::CostAndGotInfo costAndGotInfo;
//   StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();

   itemInfo->SetClientInfo(info);

   int sellprice = GoodsInfo->salePrice * itemInfo->GetAmount();

//   StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,sellprice);
   //add Money
   std::vector<int> vecType;
	vecType.push_back(eCharMoney);
	pPlayer->ChangeMoney(sellprice,true);
	pPlayer->SynCharAttribute(vecType);

//		if(sellprice > 0)
//		{
//			StatisticInfo::CostAndGotInfo costAndGotInfo;
//			StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,sellprice);
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(pPlayer->GetID(),eStatic_Money,eStaMinor_Money_SoldThings,&costAndGotInfo);
//		}

   BaseArgs args;
   args.GoodsID = itemInfo->GetID();
   args.Num = itemInfo->GetAmount();
   args.Pos = dwPacketPos;

//   StatisticInfo::CostItemInfo * costInfo = costAndGotInfo.add_costlist();
//   StatisticMgr::SetItemCost(costInfo,args.GoodsID,args.Num);

  std::map<DWORD, BuyBackItems>::iterator it = m_mapBuyBackItems.find(pPlayer->GetKey());
	if(it == m_mapBuyBackItems.end())
	{
		BuyBackItems newitems;
		newitems.push_back(*itemInfo);
		m_mapBuyBackItems[pPlayer->GetKey()] = newitems;
	}
	else
	{
		it->second.push_back(*itemInfo);
	   if(it->second.size() > 5)
		   {
		   it->second.erase(it->second.begin());
		   }
	}

  int ret = pPacket->DeleteGoods(&args);
  if(0 != ret)
	  return ret;

//  if(eAccountType_User == pPlayer->GetAccountType())
//	  StatisticMgr::GetInstance()->StatisticPlayerBehavior(pPlayer->GetID(),eStatic_NpcShop,eStaMinor_NpcShop_SellItemEvent,&costAndGotInfo);
//	//统计NPC商店中买药花费的金钱
//  StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(pPlayer,eStatic_NpcShop,eStaMinor_NpcShop_SellGetMoney,sellprice);

  return 0;
}


int ShopMgr::BuyBackItem(Smart_Ptr<Player> &pPlayer, DWORD slot)
{
	if(!pPlayer)
		return eNull;

	if(slot > 4)
	{
		return eBagPosError;
	}


   Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(pPlayer->GetMapID());
   if(!zone)
   	 {
	   return eRetZoneNotExist;
   	 }

   std::map<DWORD, BuyBackItems>::iterator it = m_mapBuyBackItems.find(pPlayer->GetKey());
	if(it == m_mapBuyBackItems.end())
	{
		return eBuyBackItemEmpty;
	}

	BuyBackItems items = it->second;
   if(slot >= items.size())
        {
	   return eBuyBackItemEmpty;
       }
	PropBase itemInfo = items[slot];
	BuyBackItems::iterator item = it->second.begin() + slot;
   if(item == it->second.end() || item->GetID() <= 0)
	   {
	   return eCant_find_item;
	   }
   else
	   {
		Smart_Ptr<ArticleBaseMgr> pPacket = pPlayer->GetContainer(ePackageType);
		if(!pPacket)
		{
			return eNull;
		}

		const GoodsInfo* GoodsInfo =	GoodsLoader::GetInstance()->GetItemDataByID(itemInfo.GetID());
		if(GoodsInfo==NULL)
		{
			return eItemError;
		}
		uint price = GoodsInfo->salePrice * itemInfo.GetAmount();
		uint hadMoney =  pPlayer->GetTypeValue(eCharMoney);
		if(hadMoney < price)
		{
			return eNotEnoghMoney;
		}

		vector<int> tempPosList;
		if(pPacket->AddItem(itemInfo.GetID(),itemInfo.GetAmount(),tempPosList) == 0)
		{
		   std::vector<int> vecType;
			vecType.push_back(eCharMoney);
			pPlayer->ChangeMoney(price, false);
			pPlayer->SynCharAttribute(vecType);
			it->second.erase(item);
		}
		   else
		   {
			  return eCant_find_item;
		   }
	   }
   return 0;

}

void  ShopMgr::ClearBuyBackItems()
{
	m_mapBuyBackItems.clear();
}




//void ShopMgr::ClearMyselfBackItems(Smart_Ptr<Player> &player)
//{
//	   std::map<DWORD, BuyBackItems>::iterator it = m_mapBuyBackItems.find(player->GetKey());
//		if(it == m_mapBuyBackItems.end())
//		{
//			return;
//		}
//		m_mapBuyBackItems.erase(it);
//}

bool ShopMgr::ClearMyselfBackItems(const EventArgs & e)
{
	PlayerRemoveMemberEventArgs *pArgs = (PlayerRemoveMemberEventArgs *) (&e);
	std::map<DWORD, BuyBackItems>::iterator it = m_mapBuyBackItems.find(pArgs->m_pObj->GetKey());

	if(it != m_mapBuyBackItems.end())
	{
		m_mapBuyBackItems.erase(it);
	}

	return true;
}
