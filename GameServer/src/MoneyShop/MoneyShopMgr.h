/*
 * MoneyShopMgr.h
 *
 *  Created on: 2014年8月9日
 *      Author: root
 */

#ifndef MONEYSHOPMGR_H_
#define MONEYSHOPMGR_H_

#include "Singleton.h"
#include "../EventSet/EventDefine.h"
#include "../Object/Player/Player.h"
#include "MoneyShopLoader.h"

#include "MessageStruct/CharLogin/MoneyShop.pb.h"
#include "MessageStruct/Daily/Daily.pb.h"


using namespace CommBaseOut;

class Player;

		/*钱庄*/

class MoneyShopMgr : public Singleton<MoneyShopMgr>
{
public:
	MoneyShopMgr();
	~MoneyShopMgr();

	void GetMoneyShopInfo(const Smart_Ptr<Player> &player,MoneyShopInfo::AckMoneyShopInfo &info);

	void ExchangeMoney(const Smart_Ptr<Player> &player,MoneyShopInfo::AckExchangeMoney *info);
	//判定处于什么状态
	int JugeExchangeStaus(const Smart_Ptr<Player> &player);

	//能够得到的钱
	int GiveMoney(int lv ,int freeCounts);

	void ExchangeMoney(const Smart_Ptr<Player> &player,int type,int& critValue,int& money);

	bool IsCanEnterMoneyShop(const Smart_Ptr<Player> &player);

	//是否钱庄 还有 免费兑换次数
	int MoneyShop_IsHaveFreeNum(const Smart_Ptr<Player> &player);

	int CalRecoreMoney(const Smart_Ptr<Player> &player, int num,Daily::RecoverResourceInfo *info);

	// 获取最大免费次数
	int GetMoneyShopFreeMax();


private:
	GoMoneyShopInfo m_moneyShop;  //钱庄操作的规则配置
};


#endif /* MONEYSHOPMGR_H_ */
