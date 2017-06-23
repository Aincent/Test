/*
 * GoodsLoader.h
 *
 *  Created on: 2014年3月28日
 *      Author: root
 */

#ifndef GOODSLOADER_H_
#define GOODSLOADER_H_

#include <map>
#include "define.h"
//#include "../Goods/GoodsProp.h"
#include "../Goods/PropDefine.h"
#include "../CharDefine.h"

using namespace std;
using namespace CommBaseOut;

typedef struct suitAttr
{
	int type;
	int data;
	suitAttr():type(-1),data(0)
	{

	}
}SuitAR;

struct ContainItem
{
	ContainItem() : m_ItemID(),m_Probability(0),m_Counts(0){}
	DWORD m_ItemID;
	int   m_Probability;
	DWORD m_Counts;
};

struct ContainItemList
{
	vector<ContainItem> m_ItemList;
};

struct ContainData
{
	vector<ContainItem>    			m_ContainOneList;	    //对应“包含道具”字段
	map<ECharProf,ContainItemList>  m_ContainTowList;  		//对应"包含道具1"字段

	void getItemList(vector<DWORD>& itemIDList,vector<DWORD>& numberList,ECharProf jobType)const;
};

typedef struct ItemTypeData
{
	int64 itemID;   //道具id
	int	type;     //物品类型
	int bigtype; //物品大类
	int smalltype; //物品小类
	int	useLvReq; //使用等级
	int	itemQuality;//物品品质
	int64	valideSecs;	//持续时间/毫秒 (宝石物品是宝石所能镶嵌的装备的使用等级)
	int	effect;		//使用类型
	int64	buffID;		//增益效果	(或者表示使用后可获得的 战姬 坐骑皮肤)
	int	price;		//购买价格

	int	salePrice;	//出售价格
	int	bindTyle;	//绑定类型

	int	stack;		//叠加数量
	int	telCD;	//使用CD频道
	int64	useCD;		//使用间隔时间
	int advanceUseMon; //强化消耗游戏比
	int reflushUseMon; //洗练消耗游戏比
	int addUseMoney; //合成消耗游戏比
	ContainData m_ContainData;
	int equipStrengthLv;
	int	wearJob;		//装备穿戴职业
	int	suitID;		//套装ID
	int	suitrnt;	//套装条件
	SuitAR	suitptyone;	//套装属性1   (1|50)
	SuitAR	suitptytwo;	//套装属性2   (1|50)
	SuitAR	suitptythree;	//套装属性3   (1|50)
	int	itemPlace;		//装备部位
	int	inlayNum;		//镶嵌孔数
	int reflushItem; //洗练需要物品
	vector<int>	updndsll;		//合成需求卷轴ID
	int	ctegiveID;		//合成后给予道具

	int	basehp;			//基础生命
	int	basemp;			//基础魔法
	int	baphyackmin;	//基础物攻min
	int	baphyackmax;	//基础物攻max
	int	baphydefmin;	//基础物防min
	int	baphydefmax;	//基础物防max
	int	bamagackmin;	//基础法攻min
	int	bamagackmax;	//基础法攻max
	int	bamagdefmin;	//基础法防min
	int	bamagdefmax;	//基础法防max
	int	basehit;			//基础命中
	int	basedodge;		//基础闪避
	int	basecrit;		//基础暴击
	int	baseung;			//基础抗暴
	int	baselucky;		//基础幸运
	int	baserte;			//基础回复

	int	aonhpmin;		//附加气血min
	int	aonhpmax;		//附加气血max
	int	aonmpmin;		//附加魔法min
	int	aonmpmax;		//附加魔法max
	int	aonphyackmin;	//附加物攻min
	int	aonphyackmax;	//附加物攻max
	int	aonmagackmin;	//附加法攻min
	int	aonmagackmax;	//附加法攻max
	int	aonphydefmin;	//附加物防min
	int	aonphydefmax;	//附加物防max
	int	aonmagdefmin;	//附加法防min
	int	aonmagdefmax;	//附加法防max
	int	aonhitmin;		//附加命中min
	int	aonhitmax;		//附加命中max
	int	aondodgemin;	//附加闪避min
	int	aondodgemax;	//附加闪避max
	int	aoncritmin;		//附加暴击min
	int	aoncritmax;		//附加暴击max
	int	aonungmin;		//附加抗暴min
	int	aonungmax;		//附加抗暴max
	int	aonrtemin;		//附加回复min
	int	aonrtemax;		//附加回复max

	bool	isRefine;		//是否可洗练
	int stoneType; //宝石类型
	int	useTimesMax;	//每日最多使用次数
	int 	needToBroadCast;	//是否需要广播
	int questId;			//任务物品，带的任务ID
	string itemName;
	string itemdesic;

	WORD getFlag()const
	{
		WORD tempFlag = 0;
		if(bindTyle == eGetBind)
			tempFlag |= ePropBind;

		return tempFlag;
	}

	ItemTypeData():itemID(-1),type(0),bigtype(0),smalltype(0),useLvReq(0),itemQuality(0),valideSecs(0),effect(0),buffID(0),
			price(0),salePrice(0),bindTyle(0),stack(0),
			telCD(0),useCD(0),advanceUseMon(0),reflushUseMon(0),addUseMoney(0),equipStrengthLv(0),wearJob(0),
			suitID(0),suitrnt(0),itemPlace(0),inlayNum(0),reflushItem(0),ctegiveID(0),basehp(0),basemp(0),
			baphyackmin(0),baphyackmax(0),baphydefmin(0),baphydefmax(0),bamagackmin(0),bamagackmax(0),
			bamagdefmin(0),bamagdefmax(0),basehit(0),basedodge(0),basecrit(0),baseung(0),baselucky(0),
			baserte(0),aonhpmin(0),aonhpmax(0),aonmpmin(0),aonmpmax(0),aonphyackmin(0),aonphyackmax(0),aonmagackmin(0),aonmagackmax(0),
			aonphydefmin(0),aonphydefmax(0),aonmagdefmin(0),aonmagdefmax(0),aonhitmin(0),aonhitmax(0),aondodgemin(0),aondodgemax(0),
			aoncritmin(0),aoncritmax(0),aonungmin(0),aonungmax(0),aonrtemin(0),aonrtemax(0),isRefine(0),stoneType(-1),
			useTimesMax(0),needToBroadCast(0),questId(0)
	{

	}

	int GetStoneAttr()const
	{
		int value = 0;

		switch(stoneType)
		{
		case eHPStone:
			{
				value = basehp;
				break;
			}
		case eMPStone:
			{
				value = basemp;
				break;
			}
		case ePhyATTStone:
			{
				value = baphyackmax;
				break;
			}
		case ePhyDefStone:
			{
				value = baphydefmax;
				break;
			}
		case eMagATTStone:
			{
				value = bamagackmax;
				break;
			}
		case eMagDefStone:
			{
				value = bamagdefmax;
				break;
			}
		case eHitStone:
			{
				value = basehit;
				break;
			}
		case eAvoidStone:
			{
				value = basedodge;
				break;
			}
		case eCrackStone:
			{
				value = basecrit;
				break;
			}
		case eAdCrackStone:
			{
				value = baseung;
				break;
			}
		case eLuckyStone:
			{
				value = baselucky;
				break;
			}
		case eReagainStone:
			{
				value = baserte;
				break;
			}
		}

		return value;
	}

//	bool IsHaveAddAttr(void) const
//	{
//		if (aonhpmax != 0 ||  aonphyackmax != 0 || aonmagackmax != 0 || aonphydefmax != 0 ||
//				aonmagdefmax != 0 || aonhitmax != 0 || aondodgemax != 0 || aoncritmax != 0 || aonungmax != 0)
//		{
//			return true;
//		}
//		return false;
//	}

}GoodsInfo;

struct SuitData
{
	int m_SuitID;
	int m_Counts;
	map<ECharAttrType,int> m_AttList;
};


class GoodsLoader
{
public:
	~GoodsLoader();

	int Init(string &file);

	int InitItemTypeData(string &file);

	int initSuitData(string& file);

	static GoodsLoader * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new GoodsLoader();
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

	/*void GetItemTypeDataByID(const int& id, GoodsInfo &info)
	{
		map<int, GoodsInfo>::iterator it = m_GoodsInfo.find(id);
		if(it != m_GoodsInfo.end())
		{
			info = it->second;
		}
	}*/

	const GoodsInfo * GetItemDataByID(int id)
	{
		map<int, GoodsInfo>::iterator it = m_GoodsInfo.find(id);
		if(it != m_GoodsInfo.end())
		{
			return &it->second;
		}
		return NULL;
	}

	//根据物品ID 得到 对应 增加的 属性
	int GetSpecliItemFunPlayerAttr(const int& itemID);

	//根据物品ID和个数获得对应增加的属性
	map<ECharAttrType,int> getSuitAttListByIDAndCounts(int suitID,int counts);

private:
	GoodsLoader();

private:
	static GoodsLoader * m_instance;

	map<int, GoodsInfo> 		m_GoodsInfo;
	map<int,map<int,SuitData> >	m_SuitInfo;  //第一层map套装ID为键值,第二层map以个数为键值
};


#endif /* GOODSLOADER_H_ */
