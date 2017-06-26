/*
 * BaseShop.h
 *
 *  Created on: 2016年11月7日
 *      Author: root
 */

#ifndef BASESHOP_H_
#define BASESHOP_H_

#include "define.h"
#include "Smart_Ptr.h"
#include "Goods/PropData.h"
#include <map>
#include "./EventSet/EventArgs.h"
#include "EventSet/EventSet.h"
#include "../EventSet/EventDefine.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "Timer/TimerInterface.h"
#include "ShopLoader.h"
#include "MessageStruct/BaseShop/BaseShop.pb.h"

using namespace CommBaseOut;
using namespace std;

class Player;
class BaseShopMgr
{
public:
	BaseShopMgr(Player * obj);
	~BaseShopMgr();

	void InitBaseShopInfo(const PlayerInfo::BaseShopInfo& baseShopInfo);
	void SetBaseShopInfo(PlayerInfo::BaseShopInfo *baseShopInfo);
	void ReleaseBaseShopMgr();
	void ResetBaseInfo();
	//购买商品
	void BuyItem(BaseShopInfo::ClientReqBuyItem& req,BaseShopInfo::AckBuyItem& toClient);
	//获取神秘商店物品
	void GetMyster(BaseShopInfo::AckMysterInfo& toClient);
	//获取刷新神秘商店物品
	void GetFlushMyster(BaseShopInfo::AckFlushMysterInfo& toClient);
	//获取商店物品信息
	void GetBaseShopInfo(int nType,BaseShopInfo::AckBaseShopInfo& toClient);
private:
	//获取限制数量
	int GetLimitbuy(int nType,int nId);
	//添加限制
	void AddLimitbuy(int nType,int nId,int nNum);
	//刷新神秘商店
	void FlushMyster();
	void FlushMyster(void* arg);
	//获取刷新状态
	bool GetFlushState(int64 nTime);

	bool FindMysterItemId(int nItemId);
private:
	int64 m_nFlushTime;
	Player *  m_owner;
	vector<ShopEntry> m_myster;
	map<DWORD,map<DWORD,DWORD> > m_limitbuy;
	TimerConnection m_reflushTime; //刷新定时器
};




#endif /* BASESHOP_H_ */
