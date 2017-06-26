/*
 * DropItemLoader.h
 *
 *  Created on: 2014年4月18日
 *      Author: helh
 */

#ifndef DROPITEMLOADER_H_
#define DROPITEMLOADER_H_

#include <map>
#include "define.h"
#include "Singleton.h"
#include "Lua/LuaInterface.h"

using namespace std;
using namespace CommBaseOut;


struct DropItemInfo
{
	DropItemInfo() : m_ID(0),m_Type(0),m_ItemID(0),m_Counts(0),m_Probability(0),m_BindType(0){}
	int m_ID; //掉落id
	int m_Type;
	int m_ItemID;
	int m_Counts;
	int m_Probability;
	int m_BindType;
};

struct DropStaticInfo
{
	DropStaticInfo() : m_ItemID(0),m_Counts(0),m_BindType(0){}
	int m_ItemID;
	int m_Counts;
	int m_BindType;
};

class DropItemLoader : public Singleton<DropItemLoader>
{
public:
	DropItemLoader();
	~DropItemLoader();

	int Init(string &path);
	int InitDropItem(string &file);

	int GetWeight(int damage, int hp, BYTE beFlag, BYTE enFlag);

	int GetDropProtectedTime()
	{
		return m_protectedTime;
	}

	int GetDropDeleteTime()
	{
		return m_deleteTime;
	}

	//获得掉落物品,有可能概率对应的物品ID为0，则item有可能为0个，表示该掉落本次随机不到物品
	void GetDropItem(vector<int> &id, vector<DropStaticInfo> &item);

	//获得掉落物品,只有一个
	void GetOneDropItem(int id,DropStaticInfo& item);

private:

	map<int,vector<DropItemInfo> > m_dropInfo; //掉落信息
	CLuaInterface m_pLua;
	int m_protectedTime; 					   //掉落保护时间
	int m_deleteTime; 						   //掉落删除时间
};



#endif /* DROPITEMLOADER_H_ */
