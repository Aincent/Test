/*
 * CJobLuckyLoader.h
 *
 *  Created on: 2014年8月13日
 *      Author: helh
 */

#ifndef CJOBLUCKYLOADER_H_
#define CJOBLUCKYLOADER_H_


#include "Singleton.h"
#include "define.h"
#include "CharDefine.h"
#include <map>
#include "Lua/LuaInterface.h"

using namespace std;
using namespace CommBaseOut;


enum CJobLevel
{
	eSevenSon=0, //七品宣德郎
	eFourDoctor, //四品中大夫
	eThreeWei,//三公太尉
	eKingWife,//皇后
	eBeyondKing, //太上皇
};


struct OfficialItemInfo
{
	int id;
	BYTE quality;
	BYTE lv;
	int eatexp;
	int addexp;
	int sale;
	BYTE type;

	map<ECharProf,map<ECharAttrType,int64> > m_AttList;

	OfficialItemInfo():id(-1),quality(0),lv(1),eatexp(0),addexp(0),sale(0),type(0)
	{

	}
};

class CJobLuckyLoader : public Singleton<CJobLuckyLoader>
{
public:
	CJobLuckyLoader();
	~CJobLuckyLoader();

	int Init(string & path);
	int InitConvert(string & file);
	int InitOfficial(string & file);
	int InitCost(string & file);

	//科举随机官印
	void GetRandCJob(int index, int &id, int &nextIndex);
	//获取直接到皇后的元宝花费
	int GetGoldToKingWife() 	{ return m_FourLvCostGold;}
	//获取随即花费的金币
	int GetRandCost(int index)
	{
		int cost = 0;;
		switch(index)
		{
			case eSevenSon:
			{
				cost = m_oneLvCost;
				break;
			}
			case eFourDoctor:
			{
				cost = m_twoLvCost;
				break;
			}
			case eThreeWei:
			{
				cost = m_threeLvCost;
				break;
			}
			case eKingWife:
			{
				cost = m_fourLvCost;
				break;
			}
			case eBeyondKing:
			{
				cost = m_fiveLvCost;
				break;
			}
			default:
				break;
		}

		return cost;
	}

	//获取官印信息
	OfficialItemInfo * GetOfficialInfo(int id)
	{
		map<int, OfficialItemInfo>::iterator it = m_officialInfo.find(id);
		if(it != m_officialInfo.end())
		{
			return &it->second;
		}

		return 0;
	}

	//根据经验和id，获取官印信息
	OfficialItemInfo * GetOfficialInfo(int id, int exp)
	{
		map<int, OfficialItemInfo>::iterator it = m_officialInfo.find(id);
		if (it != m_officialInfo.end())
		{
			if (it->second.addexp > exp || it->second.lv == 10)
				return &it->second;

			OfficialItemInfo * info = 0;

			for (; it!=m_officialInfo.end(); ++it)
			{
				if (it->second.lv < 10 && it->second.addexp > exp)
				{
					info = &it->second;
					break;
				}
				else if (it->second.lv == 10)
				{
					info = &it->second;
					break;
				}
			}

			if (it == m_officialInfo.end() && info == 0)
			{
				--it;
				info = &it->second;
			}

			return info;
		}

		return 0;
	}

	//兑换花费
	int ExchangeCJob(int id)
	{
		map<int, int>::iterator it = m_covert.find(id);
		if (it != m_covert.end())
		{
			return it->second;
		}

		return 0;
	}

private:

	map<int, int> m_covert;
	map<int, OfficialItemInfo> m_officialInfo;
	int m_oneLvCost;
	int m_twoLvCost;
	int m_threeLvCost;
	int m_fourLvCost;
	int m_fiveLvCost;
	int m_FourLvCostGold;
	CLuaInterface m_lua;
};


#endif /* CJOBLUCKYLOADER_H_ */
