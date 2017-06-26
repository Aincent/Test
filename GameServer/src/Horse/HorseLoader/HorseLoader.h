/*
 * HorseLoader.h
 *
 *  Created on: 2014年4月30日
 *      Author: helh
 */

#ifndef HORSELOADER_H_
#define HORSELOADER_H_


#include <map>
#include "define.h"
#include "Singleton.h"
#include "CharDefine.h"

using namespace std;
using namespace CommBaseOut;

struct HorseAttrInfo
{
	//血量
	int hp;
//	//法
//	int mp;
	//物攻
	int phyatt;
	//防御
	int m_Defense;
	//命中
	int hitrate;
	//闪避
	int avoidrate;
	//爆击
	int crackrate;
	//抗暴
	int avoidcrack;
	//移动速度
	int movespeed;

	HorseAttrInfo():hp(0),phyatt(0),m_Defense(0),hitrate(0),avoidrate(0),crackrate(0),avoidcrack(0),movespeed(0)
	{

	}
};

struct HorseStaticInfo
{
	int id;
	int mid;
	BYTE advancelv;
	BYTE lv;
	int exp;
	HorseAttrInfo attr[eProfessionMax - 1];
	//每点一次坐骑进阶需消耗的道具数量
	int needNum;
	int m_NeedMoney;
	WORD  m_LevelLimit;
	int m_ItemID;
	int m_BaseExp;
	float m_MinCrit;
	float m_MinCritRate;
	float m_MaxCrit;
	float m_MaxCritRate;
	HorseStaticInfo():id(-1),mid(-1),advancelv(0),lv(0),exp(0),needNum(0),m_NeedMoney(0),m_LevelLimit(0),m_ItemID(0),
			m_BaseExp(0),m_MinCrit(0),m_MinCritRate(0),m_MaxCrit(0),m_MaxCritRate(0)
	{

	}
};

enum eCommonRateType
{
	eRateTypeNUll   = 0,
	//大爆击
	eRateTypeBig    = 1,
	//小爆击
	eRateTypeSmall  = 2,
	//普通
	eRateTypeSimple = 3,
};

struct HorseSkinData
{
	HorseSkinData() : m_ID(0),m_BuyGolden(0),m_LastTime(0)
	{

	}
	int 				   m_ID;
	map<ECharAttrType,int64> m_AttList;
	int 				   m_BuyGolden;
	//持续时间
	int64       m_LastTime;
};

class HorseLoader : public Singleton<HorseLoader>
{
public:
	HorseLoader()
	{

	}

	~HorseLoader()
	{

	}

	int InitHorse(string &file);

	HorseStaticInfo * GetHorseInfoByID(int id)
	{
		map<int, HorseStaticInfo>::iterator it = m_allHorse.find(id);
		if(it != m_allHorse.end())
		{
			return &it->second;
		}

		return 0;
	}

	HorseStaticInfo * GetFirstHorse()
	{
		map<int, HorseStaticInfo>::iterator it = m_allHorse.begin();
		if(it != m_allHorse.end())
		{
			return &it->second;
		}

		return 0;
	}

	const HorseSkinData* GetHorseSkinInfo(int id)
	{
		map<int,HorseSkinData>::iterator it = m_SkinData.find(id);
		if (it != m_SkinData.end())
		{
			return &it->second;
		}

		return NULL;
	}

	//是否能使用这个模型
	bool IsUseModel(int id, int mid)
	{
		map<int, HorseStaticInfo>::iterator it = m_allHorse.find(id);
		if (it != m_allHorse.end())
		{
			map<int, HorseStaticInfo>::iterator itModel = m_allHorse.find(mid);
			if (itModel != m_allHorse.end())
			{
				if ((it->second.advancelv > itModel->second.advancelv)
						|| ((it->second.advancelv == itModel->second.advancelv)
						&& (it->second.lv >= itModel->second.lv)))
				{
					return true;
				}
			}
		}

		return false;
	}

	int GetHorseAdvanceLvByID(int id)
	{
		map<int, HorseStaticInfo>::iterator it = m_allHorse.find(id);
		if (it != m_allHorse.end())
		{
			return it->second.advancelv;
		}
		return 0;
	}

	bool IsMaxHorseID(const int & id)
	{
		map<int, HorseStaticInfo>::iterator it = m_allHorse.end();
		--it;
		if (id == it->first)
			return true;

		return false;
	}

	int getMaxID();

	const map<int, HorseStaticInfo>& getFileData(){return m_allHorse;}

protected:
	bool loadSkinFile(const string& file);

private:
	//坐骑
	map<int, HorseStaticInfo> m_allHorse;
	//皮肤
	map<int, HorseSkinData>   m_SkinData;

};


#endif /* HORSELOADER_H_ */
