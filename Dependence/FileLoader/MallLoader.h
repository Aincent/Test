/*
 * MallLoader.h
 *
 *  Created on: 2014年6月17日
 *      Author: root
 */

#ifndef MALLLOADER_H_
#define MALLLOADER_H_
#include <map>
#include "define.h"

using namespace std;
using namespace CommBaseOut;

enum MallType
{
	MallType_TimeLimit = 0,//限时商品
	MallType_HotSell = 1,//热销商品
	MallType_Common = 2,//常用
	MallType_UPgrade = 3,//升级变强
	MallType_EquipStrength = 4,//装备打造
	MallType_DayLimit = 5, //每日限购
	MallType_WeekLimit = 6, //每周限购

	MallType_Credit = 11,//声望
	MallType_Max,
};

typedef struct MallData
{
	int id;
	BYTE type;
	unsigned int price;
	unsigned int discountPrice;
	unsigned int maxCout;
	unsigned int costItemId;//购买所消耗的物品类型（铜钱，元宝，任务物品等等各种类型）
	MallData():id(-1), type(-1),price(0),	discountPrice(0), maxCout(0),costItemId(0)
	{
	}
}MallGoodsInfo;

class MallLoader
{
public:
	~MallLoader();

	int Init(string &file);

	int InitItemTypeData(string &file);

	static MallLoader * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new MallLoader();
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

	void GetItemTypeDataByID(int id,int type, MallGoodsInfo &info)
	{
		map<int,map<int, MallGoodsInfo> >::iterator it = m_MallGoodsInfo.find(type);
		if(it!=m_MallGoodsInfo.end())
		{
			map<int, MallGoodsInfo>::iterator infoIt = it->second.find(id);
			if(infoIt!=it->second.end())
			{
				info = infoIt->second;
			}
		}
	}

	int GetDanLuStoneCost(int id)
	{
		map<int, MallGoodsInfo>::iterator it = m_MallGoodsInfo[MallType_HotSell].find(id);
		if(it != m_MallGoodsInfo[MallType_HotSell].end())
		{
			return it->second.price;
		}

		it = m_MallGoodsInfo[MallType_Common].find(id);
		if(it != m_MallGoodsInfo[MallType_Common].end())
		{
			return it->second.price;
		}

	    it = m_MallGoodsInfo[MallType_UPgrade].find(id);
		if(it != m_MallGoodsInfo[MallType_UPgrade].end())
		{
			return it->second.price;
		}

		it = m_MallGoodsInfo[MallType_EquipStrength].find(id);
		if(it != m_MallGoodsInfo[MallType_EquipStrength].end())
		{
			return it->second.price;
		}

		return 0;
	}

private:
	MallLoader();

private:
	static MallLoader * m_instance;

	map<int,map<int, MallGoodsInfo> > m_MallGoodsInfo;
};

#endif /* MALLLOADER_H_ */
