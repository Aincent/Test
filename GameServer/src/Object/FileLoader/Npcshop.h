/*
 * NPCShop.h
 *
 *  Created on: 2014.5.8
 *      Author: cz
 */

#ifndef NPCSHOP_H_
#define NPCSHOP_H_

#include <map>
#include <vector>
#include "OtherDefine.h"
#include "define.h"


using namespace std;
using namespace CommBaseOut;

typedef struct sNpcShop
{
	int npcID;  // npcid
	vector<int> ShopItems; 	//	出售物品id
}sNpcShop;

class NpcShop
{

public:
	~NpcShop();

	static NpcShop * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new NpcShop();
		}

		return m_instance;
	}

	int Init(string &file);
	bool IsExistedNpc(int id);
	sNpcShop *GetNpcShopInfo(int id);

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

private:
	NpcShop();

private:
	static NpcShop * m_instance;

	map<int, sNpcShop> m_npcShopInfo;   // npcID - info
};


#endif /* NPCSHOP_H_ */
