/*
 *  shop.h
 *
 *  Created on: 2014.5.8
 *
 *  Author: cz
 */
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "define.h"
#include "cost_type_define.h"
#include "Smart_Ptr.h"
#include "../Object/FileLoader/Npcshop.h"
#include "Goods/PropData.h"
#include <map>
#include "./EventSet/EventArgs.h"
#include "EventSet/EventSet.h"
#include "../EventSet/EventDefine.h"

using namespace CommBaseOut;
using namespace std;

class Player;

//enum BuyResult
//{
//	BUY_SUCCESS												= 0,
//    BUY_ERR_CANT_FIND_ITEM                      = 40,			//
//    BUY_ERR_ITEM_ALREADY_SOLD                   = 41,
//    BUY_ERR_NOT_ENOUGHT_MONEY                   = 11,
//    BUY_ERR_SELLER_DONT_LIKE_YOU                = 42,
//    BUY_ERR_DISTANCE_TOO_FAR                    = 3,
//    BUY_ERR_ITEM_SOLD_OUT                       = 6,
//    BUY_ERR_CANT_CARRY_MORE                     = 29
//};
//
//enum SellResult
//{
//	SELL_SUCCESS											 = 0,			//
//    SELL_ERR_CANT_FIND_ITEM                      = 40,
//    SELL_ERR_CANT_SELL_ITEM                      = 43,       //
//    SELL_ERR_CANT_FIND_VENDOR                    = 2,       //
//    SELL_ERR_YOU_DONT_OWN_THAT_ITEM              = 45,       //
//    SELL_ERR_ONLY_EMPTY_BAG                      = 44        //
//};

//-----------------------------------------------------------------------------
// NPC商店类
//-----------------------------------------------------------------------------
class ShopMgr
{
public:
	~ShopMgr();

	static ShopMgr * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new ShopMgr();
		}

		return m_instance;
	}

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}
private:
	ShopMgr();
private:
	static ShopMgr * m_instance;

public:
	int	SellItem(Smart_Ptr<Player>& pPlayer, DWORD dwPacketPos, CharLogin::ClientItemInfo *info);
	int   BuyItem(Smart_Ptr<Player>& pPlayer,DWORD dwNpcID, DWORD dwItemID, DWORD dwItemNum);
	int   BuyBackItem(Smart_Ptr<Player>& pPlayer, DWORD slot);

	void  ClearBuyBackItems();
//	void  ClearMyselfBackItems(Smart_Ptr<Player>& pPlayer);

	bool ClearMyselfBackItems(const EventArgs & e);
protected:
	const sNpcShop	*m_pShopProto;		//指向商店的静态数据

	//每个玩家都有卖出去的物品，用他的key作索引
	typedef std::vector< PropBase > BuyBackItems;   //
	std::map<DWORD, BuyBackItems> m_mapBuyBackItems;

	EventConnection m_logoutEvent;
};
