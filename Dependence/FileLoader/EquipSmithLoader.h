/*
 * EquipSmithLoader.h
 *
 *  Created on: 2014年5月5日
 *      Author: root
 */

//#ifndef EQUIPSMITHLOADER_H_
//#define EQUIPSMITHLOADER_H_
//
//#include <map>
//#include "define.h"
//#include "../CharDefine.h"
//#include "EquipSmithStruct.h"
//
//using namespace CommBaseOut;
//using namespace std;
//
//class ReGetAttr;

//enum eStrengthType
//{
//	eStrengthNull    = 0,
//	eStrengthGeneral = 1, //普通
//	eStrenghtDrop    = 2, //掉星
//	eStrenghSuccess  = 3, //成功
//};


//class EquipSmithLoader
//{

//public:
//	~EquipSmithLoader();
//
//
//	// ---------------- Commom ----------------
//	int Init(string &path);
//	int InitEquipSmithParam(string & file);
//	int InitESConsumeMaterial(string& path); // 使用材料
//	int InitEquipLvLimit(string& path);
//
//	// ----------------- 强化 -----------------------
//	int GetStrengthMaxLv();
////	int GetStrengthItemID();
//	int GetStrengthProtectID();
//	int GetStrengthItemID(int star);
//
//	eStrengthType GetStrengthRation(int Lv,int probability,int& dropCounts);
//	float GetStrengthFactor(int equipLv);
//	int GetStrengNeedStoneCount(int Lv);
//	int GetStrengNeedMoney(int Lv);
//	// 根据强化等级获取装备等级
//	int GetStrengthMaxLvByLv(int equipLv);
//
//	// ----------------- 洗练 -----------------------
//	// 初始化 洗练
//	int InitRefine(string& path);
//	// 算出属性条数的权重和
//	int GetRefineItemWTSum(int equipQuality, vector<EquipSmith::RefineAttrItemWT>& item);
//	// 算出属性等级的权重和
//	int GetRefineLevelWTSum(int equipLv, vector<EquipSmith::RefineAttrLevelWT>& item);
//	// 计算出装备洗练出的 洗练属性     PARAM:  equipQuality(装备质量), isMustTopLv(是否必须顶级)  attr(洗练出来的属性)
//	int CalEquipRefineAttr(int equipLv, int equipQuality, bool isMustTopLv, map<int, ReGetAttr>& attrs,ECharProf profession);
//
//	// 获取洗练属性最高等级
//	int GetRefineAttrMaxLv();
//	// 根据属性等级获取 等级权重相对
//	int GetRefineAttrWTByLv(int equipLv,int attrLv, int& weight);
//	// 获取定向替换锁道具唯一ID
//	int GetRefineReplaceLockID();
////	// 根据装备id 获取洗练属性
////	int GetRefineAttrByEquipID(int equipID, map<int, ReGetAttr>& attrs,ECharProf profession);
//
//	int GetRefineLVByAttr(int quality, int type, int attrValue,ECharProf profession);
//
//	// 根据装备等级获取最高星级
//	int GetRefineMaxLvByLv(int equipLv);
//	// 根据装备等级获取顶级花费
//	int GetTopCostByEquipLv(int equipLv);
//
//	// 根据装备等级和品质获取属性
////	int GetRefineAttrByLQ(int quality, vector<int> levels, vector<int> types, vector<ReGetAttr>& attrsValue,ECharProf profession);
//	void GetRefineAttrByLQ(int equipLv, int quality, vector<ReGetAttr> attrs, vector<ReGetAttr>& attrsValue,ECharProf profession);
//
//	// 根据星级获取属性比率
//	float GetRefineAttrRatio(int level);
//
//	// --------------- 镶嵌 -------------------
//	// 初始化镶嵌
//	int InitInlay(string& path);
//	// 获取镶嵌所有位置和类型
//	map<int, vector<int> >& GetInlayAllPosAndType();
//	// 获取镶嵌孔的最大数量
//	int GetInalyHoleMaxCount();
//	// 获取镶嵌孔位置对应的类型
//	int GetTypeByInlayHole(int pos, vector<int>& vec);
//	// 根据宝石类型获取镶嵌孔的位置
//	int GetInlayHolePosByStoneType(int stoneType);
//	// 根据装备等级获取镶嵌的宝石最高等级
//	int GetStoneLvByLv(int equipLv);
//
//	// ---------- 升级 -------------
//	// 获取升级保护道具唯一ID
//	int GetUpProtectItemID();
////	int GetUpItemID();
//	void GetNotUpItemID(vector<int>& vec);
//	// 获取升级所需材料ID
//	int GetUpItemID(int quality);
//
//	int GetUpgradeNeedStoneCount(int equipLV);
//	int GetUpgradeNeedMoney(int equipLV);
//	int GetUpgradeDropStar(int equipLV);
//	// 根据玩家等级获取装备最高等级
//	int GetMaxEquipLvByLv(int playerLv);
//
//	// ------------ 继承 ---------------
//	// 获取继承保护道具唯一ID
//	int GetSucceedProtectItemID();
//	// 获取继承道具唯一ID
////	int GetSucceedItemID();
//	// 获取继承需要的至少的强化等级
//	int GetSucceedNeedLv();
//	// 获取不能继承装备ID
//	void GetNotSucceedItemID(vector<int>& vec);
//	// 获取继承所需材料ID
//	int GetSucceedItemID(int quality);
//	int GetSucceedRefineCost();
//	const InheritData* getInheritData(int id);
//
//protected:
//	bool loadStrenghtFile(const string& str);
//
//	bool loadUpgradeFile(const string& str);
//
//	bool loadInheritFile(const string& str);
//
//public:
//	static EquipSmithLoader * GetInstance()
//	{
//		if(m_instance == 0)
//		{
//			m_instance = new EquipSmithLoader();
//		}
//
//		return m_instance;
//	}
//
//	void DestroyInstance()
//	{
//		if(m_instance)
//		{
//			delete m_instance;
//			m_instance = 0;
//		}
//	}
//
//private:
//	EquipSmithLoader();
//
//
//private:
//	// ------------ Common ---------
//
//	// ------------ 强化 --------------
//	vector<float> m_strengthFactor; //强化系数
//	vector<int> m_strengNeedStoneCount;	//强化需要的强化石数量
//	map<int,StrengthLevelData>  m_StrengthList;
//
//	EquipSmith::StrengthBaseParam m_strengthBase; // 强化基本
//
//	// ---------- 镶嵌 --------------
//	EquipSmith::InlayBaseParam m_inlayBase; // 镶嵌基本
//
//	// ------------- 洗练 ------------
//	map<int, EquipSmith::RefineAttrItemWT > m_refineItem;
//	map<int, EquipSmith::RefineAttrLevelWT> m_refineLevel;
//	map<pair<int, int>, map<ECharProf,EquipSmith::RefineAttrBase> > m_refineBaseAttr; // key: pair<装备等级,品质>
//	map<int, EquipSmith::RefineAttrRatio> m_refineRatio; // 洗练属性比率 key:星级
//
//	EquipSmith::RefineBaseParam m_refineBase; // 洗练基本
//
//	// ------------- 升级 -----------
//	map<int,UpgradeData> m_UpgradeDataList;
//
//	EquipSmith::UpgradeBaseParam m_upgradeBase; // 升级基本
//	vector<int> m_notUpItemID; // 不能升级的物品ID
//
//	// ------------ 继承 ---------------
//	EquipSmith::SucceedBasePram m_succeedBase; // 继承基本
//	map<int,InheritData> m_InheritList;
//	vector<int> m_notSucceedItemID; // 不能继承的物品ID
//	vector<int> m_succeedEquipStreng;			//继承装备消耗的宝石数量
//
//private:
//	static EquipSmithLoader * m_instance;
//};

//#endif /* EQUIPSMITHLOADER_H_ */
