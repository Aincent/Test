/*
 * MoneyShopLoader.h
 *
 *  Created on: 2014年8月12日
 *      Author: root
 */

#ifndef MONEYSHOPLOADER_H_
#define MONEYSHOPLOADER_H_

#include <map>
#include "define.h"
#include "Singleton.h"
#include "CharDefine.h"
#include "Lua/LuaInterface.h"

using namespace std;
using namespace CommBaseOut;

struct GoMoneyShopInfo
{
	int id; //ID
	int freeMax; //每日最大免费次数
	int costMax; //每日 兑换次数
	int costLostBase; //兑换消耗基数
	int costUper; //兑换 消耗增加值
	int crackPro; //暴机概率
	int moneyPar1;  //得到 钱参数1
	int moneyPar2;//得到 钱参数2
	int openLv; //开放等级
	GoMoneyShopInfo():id(0),freeMax(0),costMax(0),costLostBase(0),
			costUper(0),crackPro(0),moneyPar1(0),moneyPar2(0),openLv(0)
	{}
};


class MoneyShopLoader : public Singleton<MoneyShopLoader>
{
public:
	MoneyShopLoader()	{		}

	~MoneyShopLoader()	{		}

	int InitMoneyShop(string &file);
	
	int initMoneyLua(string& str);

	int getComsune(int level,int exchangeCounts);

	void getMoney(int level,int freeCounts,int exchangeCounts,int& critValue,int& money);

	//只取  首元素
	GoMoneyShopInfo &  GetGoMoneyShop()	{	return	m_allMoneyShop[0];	}

private:
	vector<GoMoneyShopInfo> m_allMoneyShop;
	CLuaInterface m_Lua;
};


#endif /* MONEYSHOPLOADER_H_ */
