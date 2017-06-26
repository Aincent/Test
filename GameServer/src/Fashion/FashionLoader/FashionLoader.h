/*
 * FashionLoader.h
 *
 *  Created on: 2015年3月4日
 *      Author: root
 */

#ifndef FASHIONLOADER_H_
#define FASHIONLOADER_H_

#include <map>
#include "define.h"
#include "Singleton.h"
#include "CharDefine.h"
#include "../../Container/ParcelMgr.h"
#include "MessageCommonRet.h"

using namespace std;
using namespace CommBaseOut;

namespace FashionInfo
{
	typedef vector<pair<ECharAttrType, DWORD64> > Attr;
//	struct Attr
//	{ // 属性
//		vector<pair<ECharAttrType, DWORD64> > attrList;
//	};

	struct Cond
	{ // 条件
		enum E_BIND_COND
		{
			eCondBind_AllCan, // 都能
			eCondBind_JustIs, // 必须是绑定
			eCondBind_JustNot, // 必须非绑定
		};

		int needItemID; // 所需物品ID
		int needItemCnt; // 所需物品数量
		E_BIND_COND needBindCond; // 所需物品绑定类型

		int Check(const Smart_Ptr<ArticleParcelMgr>& parcelMgr, map<int, bool>& isBind) const
		{
			if (!parcelMgr)
			{
				return -1;
			}
			int existCnt = 0;
			switch (needBindCond)
			{
			case FashionInfo::Cond::eCondBind_AllCan: // 都能
				{
					vector<int> pos;
					existCnt = parcelMgr->FindPropCount(needItemID);

					if (existCnt < needItemCnt)
					{
						return eFashionItemNotEnougth;
					}
					isBind[needItemID] = true; // 都能，那么优先删除绑定的

				}
				break;
			case FashionInfo::Cond::eCondBind_JustIs: // 必须是绑定
				{
					vector<int> pos;
					existCnt = parcelMgr->FindPosByPropID(needItemID, pos, true);
					if (existCnt < needItemCnt)
					{
						return eFashionItemNotEnougth;
					}

					isBind[needItemID] = true; // 必须绑定，那么只删除绑定
				}
				break;
			case FashionInfo::Cond::eCondBind_JustNot: // 必须非绑定
				{
					vector<int> pos;
					existCnt = parcelMgr->FindPosByPropID(needItemID, pos, false);
					if (existCnt < needItemCnt)
					{
						return eFashionItemNotEnougth;
					}

					isBind[needItemID] = true; // 非绑定，那么只删除非绑定
				}
				break;
			default:
				{
				}
			}

			return 0;
		}
	};
	typedef vector<Cond> Conditions;

	struct Strength // 强化
	{
		struct Add // 加成
		{
			typedef vector<int> Buff;
			Attr attr; // 属性加成
			Buff buff; // buffID
		};

		int ID; // 强化属性ID
		int lv;
		Conditions cond; // 强化条件
		Add add;
	};

	struct Activate // 激活
	{
		struct Add // 加成
		{
			Attr attr; // 激活属性加成
		};
		Conditions cond; // 激活条件
		Add add; //
	};

	struct Fashion
	{
		int ID;
		Activate activateInfo;
		typedef map<int, Strength> StrengthMap;
		StrengthMap strengthInfo; // key:是强化lv
	};

	struct Config
	{
		int strengthMinLv;
		int strengthMaxLv;
	};

}
//struct FashionInfo
//{
//	int fashionID;
//	int activateNeed
//	DWORD
//	int id;
//	uint needCount;
//	uint minCount;
//	uint time;
//	uint hp;
//	uint phyack;
//	uint magack;
//	uint phydee;
//	uint magdee;
//	uint shit;
//	uint dodge;
//	uint crit;
//	uint ung;
//	FashionInfo():id(-1),needCount(0),minCount(0),time(0),hp(0),phyack(0),magack(0),phydee(0),magdee(0),
//			shit(0),dodge(0),crit(0),ung(0)
//	{
//
//	}

//};

class FashionLoader : public Singleton<FashionLoader>
{
public:
	FashionLoader()
	{
	}

	~FashionLoader()
	{
	}

	int Init(string& path);

	int InitCsv(string& path);
	int InitFashionBase(string& file);
	int InitFashionStrength(string& file);

	int InitLua(string& path);
	int InitFashionData(string& file);

	const FashionInfo::Fashion* GetFashionInfo(const int& fashionID) const;
	const FashionInfo::Strength* GetFashionStrength(const int& fashionID, const int& strengthLv) const;

	const FashionInfo::Config& GetFashionConfig();

private:
	map<int, FashionInfo::Fashion> m_fashionShow;
	FashionInfo::Config m_config;
};

#endif /* FASHIONLOADER_H_ */
