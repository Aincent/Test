/*
 * ShopLoader.h
 *
 *  Created on: 2016年11月4日
 *      Author: root
 */

#ifndef SHOPLOADER_H_
#define SHOPLOADER_H_

#include "define.h"
#include <map>
#include <vector>
#include <string>
#include "Smart_Ptr.h"
#include "Singleton.h"

using namespace CommBaseOut;
using namespace std;

enum eShopType
{
	eCarryShop = 1, 	//随身商店
	eMysteryShop,		//神秘商店
	eClanShop,			//帮派商店
	eCampShop,			//阵营商店
	eCreditShop, 		//竞技场商店
	eHonorShop,			//战场商店
};

struct ShopEntry
{
	DWORD m_nType;
	DWORD m_nId;
	DWORD m_nItemId;
	DWORD m_nItemCount;
	DWORD m_nPrice;
	DWORD m_nDailyLimit;
	DWORD m_nCampLevel;
	DWORD m_nCurrencyType;
	DWORD m_nProbalility;

	ShopEntry():m_nType(0),m_nId(0),m_nItemId(0),m_nItemCount(0),m_nPrice(0),m_nDailyLimit(0),m_nCampLevel(0),m_nCurrencyType(0),m_nProbalility(0)
	{

	}
};


class ShopLoader : public Singleton<ShopLoader>
{
public:
	ShopLoader();
	~ShopLoader();

	int Init(string &path);

	int InitCarryShop(string& file);
	int InitMysteryShop(string& file);
	int InitClanShop(string& file);
	int InitCampShop(string& file);
	int InitCreditShop(string& file);
	int InitHonorShop(string& file);

	void GetFlushMyster(vector<ShopEntry>& sEntry);
	const ShopEntry* GetShopEntry(DWORD nType,DWORD nId);

private:
	DWORD m_nTotalProbalility;
	map<DWORD,map<DWORD,ShopEntry> > m_ShopEntry;
};

#endif /* SHOPLOADER_H_ */
