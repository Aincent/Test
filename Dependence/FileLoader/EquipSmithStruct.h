/*
 * EquipSmithStruct.h
 *
 *  Created on: 2016年1月18日
 *      Author: root
 */

#ifndef EQUIPSMITHSTRUCT_H_
#define EQUIPSMITHSTRUCT_H_

#include <map>
#include "define.h"
#include "../CharDefine.h"

using namespace CommBaseOut;
using namespace std;

//namespace EquipSmith
//{
//	typedef struct sRefineAttrItemWT // 装备洗练属性条数权重
//	{
//
//		int id;
//		int quality;
//		int attrItemNum;
//		int weight;
//		sRefineAttrItemWT() : id(0), quality(0), attrItemNum(0), weight(0)
//		{
//		}
//	}RefineAttrItemWT;
//
//
//	typedef struct sRefineAttrLevelWT // 装备洗练属性级别权重
//	{
//		int id;
//		// int quality; // 装备品质
//		int equipLv; // 装备等级
//		int attrLevel; // 属性级别
//		int weight; // 权重
//		sRefineAttrLevelWT() : id(0), equipLv(0), attrLevel(0), weight(0)
//		{
//		}
//	}RefineAttrLevelWT;
//
//	typedef struct sRefineAttrRatio
//	{
//		int level; // 星级
//		float ratio; // 比率
//		sRefineAttrRatio() : level(0), ratio(0)
//		{
//
//		}
//	}RefineAttrRatio;
//
//	typedef struct sRefineAttrBase // 装备洗练基本属性
//	{
//		int level; // 装备等级
//		int quality; // 品质
//		std::map<int, int> attrTypeCntr; // key:属性类型 value:值
//		sRefineAttrBase() : level(0), quality(0)
//		{
//			attrTypeCntr.clear();
//		}
//
//	}RefineAttrBase;
//
//	typedef struct sRefineBaseParam // 洗练基本参数
//	{
//		int attrMaxLv; // 洗练最高等级
//		int replaceLockItemID; // 定向替换锁道具唯一ID
//		std::vector<int> attrTypes; // 精炼所有属性类型
////		map<int, int> maxNum; // 最大条数
//		std::map<int, int> limitRefineLv; // key:equiplv value:refinelv
//		std::map<int, int> topLvCostGolden; // key:equipLv, value:自动刷顶级花费的元宝
//		sRefineBaseParam() : attrMaxLv(0),  replaceLockItemID(0)
//		{
//			attrTypes.clear();
//			limitRefineLv.clear();
//			topLvCostGolden.clear();
//		}
//	}RefineBaseParam;
//
//	typedef struct sStrengthBaseParam // 强化基本参数
//	{
//		int strengthMaxLv; // 强化最高等级
////		int strengthItemID; // 强化石唯一道具ID
//		int strengthProtectItemID; // 强化保护符唯一道具ID
//		std::map<int, int> limitStrengthLv; // key:装备Lv  value:强化等级
//		sStrengthBaseParam() : strengthMaxLv(0), strengthProtectItemID(0)
//		{
//			limitStrengthLv.clear();
//		}
//	}StrengthBaseParam;
//
//	typedef struct sUpgradeBaseParam // 升级基本参数
//	{
//		// int upItemID; // 升级石唯一道具ID
//		int upProtectItemID; // 升级保护符唯一道具ID
//		std::map<int, int> upMaterialID; // 升级所需材料ID
//		std::map<int, int> limitUpgrade; // key:装备Lv value:角色等级
//		sUpgradeBaseParam() : upProtectItemID(0)
//		{
//		}
//	}UpgradeBaseParam;
//
//	typedef struct sSucceedBaseParam // 继承基本参数
//	{
////		int succeedItemID; // 继承道具唯一ID
//		int succeedProtectItemID; // 继承保护符唯一道具ID
//		int succeedNeedParam; // 继承需要参数 目前用于 继承需要强化等级
//		std::map<int, int> succeedMaterialID; // 继承所需材料ID
//		int succeedRefineAttrCost; // 继承洗练属性花费元宝
//		sSucceedBaseParam() : succeedProtectItemID(0), succeedNeedParam(0), succeedRefineAttrCost(0)
//		{
//		}
//	}SucceedBasePram;
//
//	typedef struct sInlayBaseParam
//	{
//		int inlayHoleMaxCount; // 镶嵌孔的数量
////		map<int, int> inlayHoleNum; // 镶嵌控数量  key:装备品质  value:具有激活属性的镶嵌孔数量
//		std::map<int, std::vector<int> > inlayStone; // 镶嵌宝石的位置和对应的类型  key:位置  value:类型
//		std::map<int, int> limitStoneLv; // key:equipLv value:stoneLv
//		sInlayBaseParam() : inlayHoleMaxCount(0)
//		{
////			inlayHoleNum.clear();
//			inlayStone.clear();
//			limitStoneLv.clear();
//		}
//	}InlayBaseParam;
//
//}

//struct StrengthLevelData
//{
//	StrengthLevelData() : m_Level(0),m_SuccessProbability(0),m_DropStar(0),m_DropProbability(0),m_NeedItemCounts(0),m_NeedMoney(0),
//			m_strengthMaterialID(0)
//	{
//
//	}
//	int m_Level;
//	int m_SuccessProbability;
//	int m_DropStar;
//	int m_DropProbability;
//	int m_NeedItemCounts;
//	int m_NeedMoney;
//	int m_strengthMaterialID; // 强化所需材料ID
////	int m_equipMinLv; // 强化升星的最低装备等级
//};
//
//struct UpgradeData
//{
//	UpgradeData() : m_Level(0),m_DropStar(0),m_NeedItemCounts(0),m_NeedMoney(0){}
//	int m_Level;
//	int m_DropStar;
//	int m_NeedItemCounts;
//	int m_NeedMoney;
//};
//
//struct InheritData
//{
//	InheritData() : m_StartLevel(0),m_DropStar(0),m_NeedItemCounts(0){}
//	int   m_StartLevel;
//	CommBaseOut::BYTE  m_DropStar;
//	short m_NeedItemCounts;
//};

	struct StrengthBaseParam // 强化基本参数
	{
		int nStrengthLv; // 强化等级
		int nMoney;		//铜钱数量
		int nItemId;	//道具ID
		int nItemNum;	//道具数量

		StrengthBaseParam() : nStrengthLv(0),nMoney(0),nItemId(0),nItemNum(0)
		{

		}
	};


	struct StrengthLevelData
	{
		StrengthLevelData() : nPos(0),nStrengthLv(0)
		{
			mAttr.clear();
		}

		int nPos; //部位
		int nStrengthLv; // 强化等级
		std::map<int,int> mAttr;
	};


	struct InlayBaseParam
	{
		int nInlayPos; // 镶嵌孔的部位
		int nStoneType;// 宝石的类型
		std::vector<int> vLv;//镶嵌孔开放等级

		InlayBaseParam() : nInlayPos(0),nStoneType(0)
		{
			vLv.clear();
		}
	};

	struct RefineBaseParam
	{
		int nMoney; //洗炼消耗铜钱数量
		int nItemId;	//道具ID
		int nItemNum;	//道具数量
		int nGolden; //锁定消耗元宝数量

		RefineBaseParam() : nMoney(0),nItemId(0),nItemNum(0),nGolden(0)
		{

		}
	};

	struct RefineAttrBase // 装备洗练基本属性
	{
		int nStar; //星级
		int nHP;
		int nPhyAttack;
		int nPhyDef;
		int nHitRate;
		int nAvoidRate;
		int nDownRate;
		int nCrackRate;

		RefineAttrBase() : nStar(0),nHP(0),nPhyAttack(0),nPhyDef(0),nHitRate(0),nAvoidRate(0),nDownRate(0),nCrackRate(0)
		{

		}
	};

	struct UpgradeBaseParam // 升级基本参数
	{
		int nId;
		int nEquipType;
		int nEquipQuality;
		int nEquipLv;
		int nMoney; //升级消耗铜钱数量
		int nItemId;	//道具ID
		int nItemNum;	//道具数量

		UpgradeBaseParam() : nId(0),nEquipType(0),nEquipQuality(0),nEquipLv(0),nMoney(0),nItemId(0),nItemNum(0)
		{
		}
	};

#endif /* EQUIPSMITHSTRUCT_H_ */
