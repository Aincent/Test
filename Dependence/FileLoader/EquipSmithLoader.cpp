///*
// * EquipSmithLoader.cpp
// *
// *  Created on: 2014年5月5日
// *      Author: root
// */
//
///*
// * BattleInfoLoader.cpp
// *
// *  Created on: 2013��10��26��
// *      Author: helh
// */
//
//#include "EquipSmithLoader.h"
//#include "../CSVLoader.h"
//#include "Lua/LuaInterface.h"
//#include "../CharDefine.h"
//#include "../Goods/EquipData.h"
//#include "Log/Logger.h"
//#include "../StrArgPkg.h"
//#include <math.h>
//
//using namespace EquipSmith;
//
//EquipSmithLoader * EquipSmithLoader::m_instance = 0;
//
//EquipSmithLoader::EquipSmithLoader()
//{
//	m_strengthFactor.clear();
//	m_succeedEquipStreng.clear();
//
//	m_refineItem.clear();
//	m_refineLevel.clear();
//	m_refineBaseAttr.clear();
//}
//
//EquipSmithLoader::~EquipSmithLoader()
//{
//
//}
//
//eStrengthType EquipSmithLoader::GetStrengthRation(int Lv,int probability,int& dropCounts)
//{
//	if(Lv < 0 || Lv >= GetStrengthMaxLv())
//	{
//		return eStrengthNull;
//	}
//
//	map<int,StrengthLevelData>::iterator tempIt =  m_StrengthList.find(Lv);
//	if(tempIt == m_StrengthList.end())
//		return eStrengthNull;
//
//	//先计算成功概率
//	int tempProbability = rand() % 10000 + 1;
//	if(tempIt->second.m_SuccessProbability + probability >= tempProbability)
//		return eStrenghSuccess;
//
//	tempProbability = rand() % 10000 + 1;
//	if(tempIt->second.m_DropProbability > tempProbability)
//	{
//		dropCounts = tempIt->second.m_DropStar;
//		return eStrenghtDrop;
//	}
//
//	return eStrengthGeneral;
//}
//
//int EquipSmithLoader::GetStrengNeedStoneCount(int Lv)
//{
//	map<int,StrengthLevelData>::iterator tempIt = m_StrengthList.find(Lv);
//	if(tempIt == m_StrengthList.end())
//	{
//		return 0;
//	}
//
//	return tempIt->second.m_NeedItemCounts;
//}
//
//int EquipSmithLoader::GetStrengNeedMoney(int Lv)
//{
//	map<int,StrengthLevelData>::iterator tempIt = m_StrengthList.find(Lv);
//	if(tempIt == m_StrengthList.end())
//	{
//		return 0;
//	}
//
//	return tempIt->second.m_NeedMoney;
//}
//
//int EquipSmithLoader::GetStrengthMaxLvByLv(int equipLv)
//{
//	map<int, int>::iterator it = m_strengthBase.limitStrengthLv.find(equipLv);
//	if (m_strengthBase.limitStrengthLv.end() == it)
//	{
//		return -1;
//	}
//	return it->second;
//}
//
//float EquipSmithLoader::GetStrengthFactor(int equipLv)
//{
//	if(equipLv <= 0 || equipLv > GetStrengthMaxLv())
//	{
//		return 1;
//	}
//	return m_strengthFactor[equipLv-1];
//}
//int EquipSmithLoader::GetUpgradeNeedStoneCount(int equipLV)
//{
//	map<int,UpgradeData>::iterator tempIt = m_UpgradeDataList.find(equipLV);
//	if(tempIt == m_UpgradeDataList.end())
//		return 0;
//
//	return tempIt->second.m_NeedItemCounts;
//}
//
//int EquipSmithLoader::GetUpgradeNeedMoney(int equipLV)
//{
//	map<int,UpgradeData>::iterator tempIt = m_UpgradeDataList.find(equipLV);
//	if(tempIt == m_UpgradeDataList.end())
//		return 0;
//
//	return tempIt->second.m_NeedMoney;
//}
//
//int EquipSmithLoader::GetUpgradeDropStar(int equipLV)
//{
//	map<int,UpgradeData>::iterator tempIt = m_UpgradeDataList.find(equipLV);
//	if(tempIt == m_UpgradeDataList.end())
//		return 0;
//
//	return tempIt->second.m_DropStar;
//}
//
//int EquipSmithLoader::GetMaxEquipLvByLv(int playerLv)
//{
//	map<int, int>::iterator it = m_upgradeBase.limitUpgrade.begin();
//	map<int, int>::iterator end = m_upgradeBase.limitUpgrade.end();
//	int before = 0;
//	for ( ; it != end ; ++it)
//	{
//		if (before < it->second && playerLv <= it->second)
//		{
//			return it->first;
//		}
//
//		before = it->second;
//	}
//
//	return 0;
//}
//
//const InheritData* EquipSmithLoader::getInheritData(int id)
//{
//	map<int,InheritData>::iterator tempIt = m_InheritList.find(id);
//	if(tempIt == m_InheritList.end())
//		return NULL;
//
//	return &tempIt->second;
//}
//
//int EquipSmithLoader::GetStrengthMaxLv()
//{
//	return m_strengthBase.strengthMaxLv;
//}
//
////int EquipSmithLoader::GetStrengthItemID()
////{
////	return m_strengthBase.strengthItemID;
////}
//
//int EquipSmithLoader::GetStrengthProtectID()
//{
//	return m_strengthBase.strengthProtectItemID;
//}
//
//int EquipSmithLoader::GetStrengthItemID(int star)
//{
//	map<int, StrengthLevelData>::iterator it = m_StrengthList.find(star);
//	if (it != m_StrengthList.end())
//	{
//		return it->second.m_strengthMaterialID;
//	}
//
//	return 0;
//}
//
//int EquipSmithLoader::Init(string &path)
//{
//	string fileParam = path + "EquipSimth.lua";
//
//	if(InitEquipSmithParam(fileParam))
//	{
//		return -1;
//	}
//
//	if(!loadStrenghtFile(path))
//	{
//		return -1;
//	}
//
//	if(!loadUpgradeFile(path))
//	{
//		return -1;
//	}
//
//	if(!loadInheritFile(path))
//	{
//		return -1;
//	}
//
//	if (InitRefine(path))
//	{
//		return -1;
//	}
//
//	if (InitInlay(path))
//	{
//		return -1;
//	}
//
//	if (InitESConsumeMaterial(path))
//	{
//		return -1;
//	}
//
//	if (InitEquipLvLimit(path))
//	{
//		return -1;
//	}
//
//	return 0;
//}
//
//int EquipSmithLoader::InitEquipSmithParam(string & file)
//{
//	CLuaInterface lua;
//
//	//读取配置文件
//	lua.OpenLua();
//
//	if(lua.LoadFile(file) != 0 || lua.Call() != 0)
//	{
//		return -1;
//	}
//
//	lua.SetLuaFunction("GetEquipFactorParam");
//
//	if(lua.Call(0, 12))
//	{
//		return -1;
//	}
//
//	float AParam = lua.GetFloat();
//	float BParam = lua.GetFloat();
//	float CParam = lua.GetFloat();
//	float DParam = lua.GetFloat();
//	float EParam = lua.GetFloat();
//	float FParam = lua.GetFloat();
//	float GParam = lua.GetFloat();
//	float HParam = lua.GetFloat();
//	float IParam = lua.GetFloat();
//	float JParam = lua.GetFloat();
//	float KParam = lua.GetFloat();
//	float LParam = lua.GetFloat();
//
//	lua.SetLuaFunction("GetEquipSucceedParam");
//
//	if(lua.Call(0, 12))
//	{
//		return -1;
//	}
//
//	int aSuParam = lua.GetInt();
//	int bSuParam = lua.GetInt();
//	int cSuParam = lua.GetInt();
//	int dSuParam = lua.GetInt();
//	int eSuParam = lua.GetInt();
//	int fSuParam = lua.GetInt();
//	int gSuParam = lua.GetInt();
//	int hSuParam = lua.GetInt();
//	int iSuParam = lua.GetInt();
//	int jSuParam = lua.GetInt();
//	int kSuParam = lua.GetInt();
//	int lSuParam = lua.GetInt();
//
//	m_strengthFactor.push_back(AParam);
//	m_strengthFactor.push_back(BParam);
//	m_strengthFactor.push_back(CParam);
//	m_strengthFactor.push_back(DParam);
//	m_strengthFactor.push_back(EParam);
//	m_strengthFactor.push_back(FParam);
//	m_strengthFactor.push_back(GParam);
//	m_strengthFactor.push_back(HParam);
//	m_strengthFactor.push_back(IParam);
//	m_strengthFactor.push_back(JParam);
//	m_strengthFactor.push_back(KParam);
//	m_strengthFactor.push_back(LParam);
//
//	m_succeedEquipStreng.push_back(aSuParam);
//	m_succeedEquipStreng.push_back(bSuParam);
//	m_succeedEquipStreng.push_back(cSuParam);
//	m_succeedEquipStreng.push_back(dSuParam);
//	m_succeedEquipStreng.push_back(eSuParam);
//	m_succeedEquipStreng.push_back(fSuParam);
//	m_succeedEquipStreng.push_back(gSuParam);
//	m_succeedEquipStreng.push_back(hSuParam);
//	m_succeedEquipStreng.push_back(iSuParam);
//	m_succeedEquipStreng.push_back(jSuParam);
//	m_succeedEquipStreng.push_back(kSuParam);
//	m_succeedEquipStreng.push_back(lSuParam);
//
//
//
//	{
//		// 洗练
//		lua.SetLuaFunction("GetEquipRefineBaseParam");
//		if(lua.Call(0, 2))
//		{
//			return -1;
//		}
//
//		m_refineBase.replaceLockItemID = lua.GetInt();
//		m_refineBase.attrMaxLv = lua.GetInt();
//
//	}
//
//
//	{
//		// 镶嵌
//		lua.SetLuaFunction("GetEquipInlayBaseParam");
//		if (lua.Call(0, 1))
//		{
//			return -1;
//		}
//
////		vector<int> equipQuality;
////		vector<int> inlayHoleNum;
//
////		int i = 1;
////		int key = 0, value = 0;
////
////		for (i = 1; i <= lua.GetTableFieldCount(); ++i)
////		{
////			value = lua.GetFieldInt(i);
////			inlayHoleNum.push_back(value);
////		}
////		lua.CloseTable();
////
////		for (i = 1 ; i <= lua.GetTableFieldCount(); ++i)
////		{
////			key = lua.GetFieldInt(i);
////			equipQuality.push_back(key);
////		}
////		lua.CloseTable();
////
////		if (equipQuality.size() != inlayHoleNum.size())
////		{
////			return -1;
////		}
////
////		uint j = 0;
////		for (j = 0; j < equipQuality.size(); ++j)
////		{
////			m_inlayBase.inlayHoleNum[equipQuality[j]] = inlayHoleNum[j];
////		}
//
//		m_inlayBase.inlayHoleMaxCount = lua.GetInt();
//	}
//
//	{
//		// 强化
//		lua.SetLuaFunction("GetEquipStrengthBaseParam");
//
//		if (lua.Call(0, 2))
//		{
//			return -1;
//		}
//
//		m_strengthBase.strengthMaxLv = lua.GetInt();
////		m_strengthBase.strengthItemID = lua.GetInt();
//		m_strengthBase.strengthProtectItemID = lua.GetInt();
//	}
//
//	{
//		// 继承
//		lua.SetLuaFunction("GetEquipSucceedBaseParam");
//
//		if (lua.Call(0, 4))
//		{
//			return -1;
//		}
//
//		m_succeedBase.succeedRefineAttrCost = lua.GetInt();
//
//		int count = lua.GetTableFieldCount();
//		for (int i = 1 ; i <= count; ++i)
//		{
//			m_notSucceedItemID.push_back(lua.GetFieldInt(i));
//		}
//		lua.CloseTable();
//
//		m_succeedBase.succeedNeedParam = lua.GetInt();
//		m_succeedBase.succeedProtectItemID = lua.GetInt();
//	}
//
//	{
//		// 升级
//		lua.SetLuaFunction("GetEquipUpBaseParam");
//
//		if (lua.Call(0, 2))
//		{
//			return -1;
//		}
//
//		int count = lua.GetTableFieldCount();
//		for (int i = 1 ; i <= count; ++i)
//		{
//			m_notUpItemID.push_back(lua.GetFieldInt(i));
//		}
//		lua.CloseTable();
//
////		m_upgradeBase.upItemID = lua.GetInt();
//		m_upgradeBase.upProtectItemID = lua.GetInt();
//	}
//
//
//	// lua.CloseTable();
//
//	lua.CloseLua();
//
//	return 0;
//}
//
//int EquipSmithLoader::InitESConsumeMaterial(string& path)
//{
//	string file = path + "./Equipinf.csv";
//	CSVLoader loader;
//	if (!loader.OpenFromFile(file.c_str()))
//	{
//		return -1;
//	}
//
//	int count = 0;
//	for (uint row = 0; row < loader.GetRowCount(); ++row)
//	{
//		count = 0;
//		int quality = loader.GetInt(row, count++);
//
////		m_strengthBase.strengthMaterialID[quality] = loader.GetInt(row, 1);
//		m_upgradeBase.upMaterialID[quality] = loader.GetInt(row, count++);
//		m_succeedBase.succeedMaterialID[quality] = loader.GetInt(row, count++);
//	}
//
//	return 0;
//}
//
//int EquipSmithLoader::InitEquipLvLimit(string& path)
//{
//	string file = path + "./forgeLimitData.csv";
//	CSVLoader loader;
//	if (!loader.OpenFromFile(file.c_str()))
//	{
//		return -1;
//	}
//
//	int count = 0;
//	for (uint row = 0; row < loader.GetRowCount(); ++row)
//	{
//		count = 0;
//		count++;
//
//		int equipLv = loader.GetInt(row, count++);
//		m_strengthBase.limitStrengthLv[equipLv] = loader.GetInt(row, count++);
//		count++;
//		m_refineBase.limitRefineLv[equipLv] = loader.GetInt(row, count++);
//		m_inlayBase.limitStoneLv[equipLv] = loader.GetInt(row, count++);
//		m_upgradeBase.limitUpgrade[equipLv] = loader.GetInt(row, count++);
//		m_refineBase.topLvCostGolden[equipLv] = loader.GetInt(row, count++);
//
//	}
//
//	return 0;
//}
//
//int EquipSmithLoader::InitRefine(string& path)
//{
//	int count = 0;
//	{
//		string AttrItemFile = path + "./PolishNum.csv";
//		CSVLoader loader;
//		if (!loader.OpenFromFile(AttrItemFile.c_str()))
//		{
//			return -1;
//		}
//
//		for (uint row = 0; row < loader.GetRowCount(); ++row)
//		{
//			count = 0;
//			RefineAttrItemWT item;
//
//			item.id = loader.GetInt(row, count++);
//			item.quality = loader.GetInt(row, count++);
//			item.attrItemNum = loader.GetInt(row, count++);
//			item.weight = loader.GetInt(row, count++);
//
//			m_refineItem[item.id] = item;
//		}
//	}
//
//	{
//		string AttrLevelFile = path + "./PolishStar.csv";
//		CSVLoader loader;
//		if (!loader.OpenFromFile(AttrLevelFile.c_str()))
//		{
//			return -1;
//		}
//
//		for (uint row = 0; row < loader.GetRowCount(); ++row)
//		{
//			count = 0;
//			RefineAttrLevelWT item;
//
//			item.id = loader.GetInt(row, count++);
//			item.equipLv = loader.GetInt(row, count++);
//			item.attrLevel = loader.GetInt(row, count++);
//			item.weight = loader.GetInt(row, count++);
//
//			m_refineLevel[item.id] = item;
//		}
//	}
//
//	{
//		string AttrBaseFile = path + "./PolishAttribute.csv";
//		CSVLoader loader;
//		if (!loader.OpenFromFile(AttrBaseFile.c_str()))
//		{
//			return -1;
//		}
//
//		for (uint row = 0; row < loader.GetRowCount(); ++row)
//		{
//			int tempCounts = 0;
//			RefineAttrBase item;
//			++tempCounts;
//			item.level    = loader.GetInt(row,tempCounts++);
//			item.quality  = loader.GetInt(row,tempCounts++);
//			for(int z = eMuscleMan;z <= eShooter;++z)
//			{
//				item.attrTypeCntr[eCharHP] = loader.GetInt(row,tempCounts++);
//				item.attrTypeCntr[eCharPhyAttackMax] = loader.GetInt(row,tempCounts++);
//				item.attrTypeCntr[eCharPhyDefence] = loader.GetInt(row,tempCounts++);
//				item.attrTypeCntr[eCharHitRate] = loader.GetInt(row,tempCounts++);
//				item.attrTypeCntr[eCharAvoidRate] = loader.GetInt(row,tempCounts++);
//				item.attrTypeCntr[eCrackDownRate] = loader.GetInt(row,tempCounts++);
//				item.attrTypeCntr[eAvoidCrackRate] = loader.GetInt(row,tempCounts++);
//
//				m_refineBaseAttr[pair<int, int>(item.level, item.quality)][(ECharProf)z] = item;
//			}
//		}
//
//		m_refineBase.attrTypes.push_back(eCharHP);
//		m_refineBase.attrTypes.push_back(eCharPhyAttackMax);
//		m_refineBase.attrTypes.push_back(eCharPhyDefence);
//		m_refineBase.attrTypes.push_back(eCharHitRate);
//		m_refineBase.attrTypes.push_back(eCharAvoidRate);
//		m_refineBase.attrTypes.push_back(eCrackDownRate);
//		m_refineBase.attrTypes.push_back(eAvoidCrackRate);
//	}
//
//	{
//		string attrRatioFile = path + "./PolishQuality.csv";
//		CSVLoader loader;
//		if (!loader.OpenFromFile(attrRatioFile.c_str()))
//		{
//			return -1;
//		}
//
//		int count = 0;
//		for (uint row = 0; row < loader.GetRowCount(); ++row)
//		{
//			count = 0;
//			RefineAttrRatio attrRatio;
//			count++;
//			attrRatio.level = loader.GetInt(row, count++);
//			attrRatio.ratio = loader.GetFloat(row, count++);
//
//			m_refineRatio[attrRatio.level] = attrRatio;
//		}
//	}
//
//	return 0;
//}
//
//int EquipSmithLoader::GetRefineItemWTSum(int equipQuality, vector<RefineAttrItemWT>& item)
//{
//	int itemWeightSum = 0;
//	bool flag = false;
//
//	map<int, RefineAttrItemWT>::iterator itemIt = EquipSmithLoader::GetInstance()->m_refineItem.begin();
//	for ( ; itemIt != EquipSmithLoader::GetInstance()->m_refineItem.end(); ++itemIt)
//	{
//		if (equipQuality == itemIt->second.quality)
//		{
//			flag = true;
//			itemWeightSum += itemIt->second.weight;
//			item.push_back(itemIt->second);
//			continue;
//		}
//		if (flag)
//		{
//				break;
//		}
//	}
//
//	return itemWeightSum;
//}
//
//int EquipSmithLoader::GetRefineLevelWTSum(int equipLv, vector<RefineAttrLevelWT>& item)
//{
//	int levelWeightSum = 0;
//	bool flag = false;
//
//	map<int, RefineAttrLevelWT>::iterator levelIt = m_refineLevel.begin();
//	for ( ; levelIt != m_refineLevel.end(); ++levelIt)
//	{
//		if (equipLv == levelIt->second.equipLv)
//		{
//			flag = true;
//			levelWeightSum += levelIt->second.weight;
//			item.push_back(levelIt->second);
//			continue;
//		}
//		if (flag)
//		{
//			break;
//		}
//	}
//
//	return levelWeightSum;
//}
//
//// 计算出装备洗练出的 洗练属性
//int EquipSmithLoader::CalEquipRefineAttr(int equipLv, int equipQuality, bool isMustTopLv, map<int, ReGetAttr>& attrs,ECharProf profession)
//{
//	int attrItemNum = 0; // 洗练属性条数
//	int attrLevel = 0; // 洗练属性等级
//
//	int before = 0;
//	int value = 0;
//	int itemWeightSum = 0;
//	int levelWeightSum = 0;
//
//	vector<RefineAttrItemWT> attrItems;
//	vector<RefineAttrLevelWT> attrLevels;
//	// 根据装备等级、装备品质和职业找到相应的属性信息
//	map<pair<int, int>, map<ECharProf,EquipSmith::RefineAttrBase> >::iterator attrIt = m_refineBaseAttr.find(pair<int, int>(equipLv, equipQuality));
//	if (attrIt == m_refineBaseAttr.end())
//	{
//		return -1;
//	}
//	map<ECharProf,EquipSmith::RefineAttrBase>::iterator tempProfessionIt = attrIt->second.find(profession);
//	if (tempProfessionIt == attrIt->second.end())
//	{
//		return -1;
//	}
//
//	itemWeightSum = GetRefineItemWTSum(equipQuality, attrItems); // 获取符合条件的洗练属性条数和权重和
//	levelWeightSum = GetRefineLevelWTSum(equipLv, attrLevels); // 获取符合条件的洗练属性等级和权重和
//
//	// 随机出 属性条数的随机值
//	value = rand() % itemWeightSum + 1;
//	vector<RefineAttrItemWT>::iterator itemIt = attrItems.begin();
//	// 找出符合 属性条数权重的属性条数
//	for ( ; itemIt != attrItems.end(); ++itemIt)
//	{
//		if(itemIt->weight == 0)
//			continue;
//
//		if (value > before && value <= itemIt->weight+before)
//		{
//			attrItemNum = itemIt->attrItemNum;
//			break;
//		}
//		before += itemIt->weight;
//	}
//
//	if (0 >= attrItemNum)
//	{
//		return -1;
//	}
//
//	// 需要随机的属性类型
//	vector<int> attrTypes = m_refineBase.attrTypes;
//
//	int maxAttrLv = GetRefineMaxLvByLv(equipLv);
//
//	if (isMustTopLv)
//	{ // 顶级保护 如果必定出顶级  那么随机不能出现顶级
//		int maxLvWeight = 0; // 最高等级相对权重值
//
//		GetRefineAttrWTByLv(equipLv, maxAttrLv, maxLvWeight);
//		levelWeightSum -= maxLvWeight;
//	}
//
//	for (int i = 0, pos = 0; i < attrItemNum && attrTypes.size() > 0; ++i)
//	{
//		bool flag = false;
//
//		if (isMustTopLv)
//		{ // 是否必定出顶级
//			attrLevel = maxAttrLv;
//			flag = true;
//			isMustTopLv = false;
//		}
//		else
//		{ // 不是必定出顶级
//			if (0 == levelWeightSum)
//			{
//				return -1;
//			}
//			value = rand() % levelWeightSum + 1;
//
//			before = 0;
//			vector<RefineAttrLevelWT>::iterator levelIt = attrLevels.begin();
//			// 找出符合 属性等级权重的属性等级
//			for ( ; levelIt != attrLevels.end(); ++levelIt)
//			{
//				if (value > before && value <= levelIt->weight+before)
//				{
//					attrLevel = levelIt->attrLevel;
//					flag = true;
//					break;
//				}
//				before += levelIt->weight;
//			}
//		}
//
//		if (!flag)
//		{
//			return -1;
//		}
//		// 取得属性比率值
//		float ratio = GetRefineAttrRatio(attrLevel);
//		if (0 == ratio)
//		{
//			return -1;
//		}
//
//		// 随机 属性类型
//		value = rand() % attrTypes.size();
//
//		map<int, int>& attrTypeCntr = tempProfessionIt->second.attrTypeCntr;
//		// 根据属性类型找到相应类型的属性类型信息
//		map<int, int>::iterator it = attrTypeCntr.find(attrTypes[value]);
//		if (it == attrTypeCntr.end())
//		{
//			break;
//		}
//
//		ReGetAttr attr;
//		attr.type = it->first;
//		attr.value = (CommBaseOut::WORD)ceil(it->second * ratio);
//		attr.level = attrLevel;
//
//		attrs[pos++] = attr;
//		attrTypes.erase(attrTypes.begin() + value);
//	}
//
//	return 0;
//}
//
//int EquipSmithLoader::GetRefineAttrMaxLv()
//{
//	return m_refineBase.attrMaxLv;
//}
//
//int EquipSmithLoader::InitInlay(string& path)
//{
//	CSVLoader loader;
//	string file = path + "StoneInlay.csv";
//	if (!loader.OpenFromFile(file.c_str()))
//	{
//		return -1;
//	}
//
//	for (uint row = 0; row < loader.GetRowCount(); ++row)
//	{
//		int inlayHole = loader.GetInt(row, 0);
//
//		StrArgPkg limitPkg("|", loader.GetString(row, 1));
//		for (uint i = 0; i < limitPkg.size(); ++i)
//		{
//			int inlayAttr = 0;
//			inlayAttr = atoi(getArg(limitPkg, i).c_str());
//			m_inlayBase.inlayStone[inlayHole].push_back(inlayAttr);
//		}
//	}
//	return 0;
//}
//
////int EquipSmithLoader::GetRefineAttrLvByWT(int weight, int& attrLv)
////{
////	int before = 1;
////	map<int, RefineAttrLevelWT>::iterator it = m_refineLevel.begin();
////	for ( ; it != m_refineLevel.end(); ++it)
////	{
////		if (before <= it->second.weight+before && weight < it->second.weight+before)
////		{
////			attrLv = it->second.attrLevel;
////			break;
////		}
////		before += it->second.weight;
////	}
////
////	if (it == m_refineLevel.end())
////	{
////		return -1;
////	}
////	return 0;
////}
//
//int EquipSmithLoader::GetRefineAttrWTByLv(int equipLv, int attrLv, int& weight)
//{
//	map<int, RefineAttrLevelWT>::iterator it = m_refineLevel.begin();
//	for ( ; it != m_refineLevel.end(); ++it)
//	{
//		if (it->second.attrLevel == attrLv && it->second.equipLv == equipLv)
//		{
//			weight = it->second.weight;
//			break;
//		}
//	}
//	if (it != m_refineLevel.end())
//	{
//		return 0;
//	}
//	return -1;
//}
//
//map<int, vector<int> >& EquipSmithLoader::GetInlayAllPosAndType()
//{
//	return m_inlayBase.inlayStone;
//}
//
//int EquipSmithLoader::GetRefineReplaceLockID()
//{
//	return m_refineBase.replaceLockItemID;
//}
//
////int EquipSmithLoader::GetRefineAttrByEquipID(int equipID, map<int, ReGetAttr>& attrs,ECharProf profession)
////{
////	const GoodsInfo* pGoods = GoodsLoader::GetInstance()->GetItemDataByID(equipID);
////	if (NULL == pGoods || !(pGoods->IsHaveAddAttr()))
////	{
////		return -1;
////	}
////
////	int type = 0, pos = 0;
////	for (uint i = 0; i < m_refineBase.attrTypes.size(); ++i)
////	{
////		ReGetAttr attr;
////		type = m_refineBase.attrTypes[i];
////
////		attr.type = type;
////		switch (type)
////		{
////		case eCharHP:
////			{
////				attr.value = pGoods->aonhpmax;
////			}
////			break;
////		case eCharPhyAttackMax:
////			{
////				attr.value = pGoods->aonphyackmax;
////			}
////			break;
////		case eCharPhyDefence:
////			{
////				attr.value = pGoods->aonphydefmax;
////			}
////			break;
////		case eCharHitRate:
////			{
////				attr.value = pGoods->aonhitmax;
////			}
////			break;
////		case eCharAvoidRate:
////			{
////				attr.value = pGoods->aondodgemax;
////			}
////			break;
////		case eCrackDownRate:
////			{
////				attr.value = pGoods->aoncritmax;
////			}
////			break;
////		case eAvoidCrackRate:
////			{
////				attr.value = pGoods->aonungmax;
////			}
////			break;
////		default:
////			{
////				attr.value = -1;
////			}
////		}
////		if (0 < attr.value)
////		{
////			int lv = GetRefineLVByAttr(pGoods->itemQuality, attr.type, attr.value,profession);
////			if (-1 != lv)
////			{
////				attr.level = lv;
////				attrs[pos++] = attr;
////			}
////		}
////
////	}
////
////	return 0;
////}
//
//int EquipSmithLoader::GetRefineMaxLvByLv(int equipLv)
//{
//	map<int, int>::iterator it = m_refineBase.limitRefineLv.find(equipLv);
//	if (m_refineBase.limitRefineLv.end() == it)
//	{
//		return -1;
//	}
//
//	return it->second;
//}
//
//// 根据装备等级获取顶级花费
//int EquipSmithLoader::GetTopCostByEquipLv(int equipLv)
//{
//	map<int, int>::iterator it = m_refineBase.topLvCostGolden.find(equipLv);
//	if (m_refineBase.topLvCostGolden.end() == it)
//	{
//		return -1;
//	}
//
//	return it->second;
//}
//
////int EquipSmithLoader::GetRefineMaxNumByQuality(int quality)
////{
////	map<int, int>::iterator it = m_refineBase.maxNum.find(quality);
////	if (it != m_refineBase.maxNum.end())
////	{
////		return it->second;
////	}
////	return 0;
////}
//
//int EquipSmithLoader::GetRefineLVByAttr(int quality, int type, int attrValue,ECharProf profession)
//{
//	for (map<pair<int, int>, map<ECharProf,EquipSmith::RefineAttrBase> >::iterator it = m_refineBaseAttr.begin();
//			it != m_refineBaseAttr.end(); ++it)
//	{
//		map<ECharProf,EquipSmith::RefineAttrBase>::iterator tempProfessionIt = it->second.find(profession);
//		if(tempProfessionIt == it->second.end())
//		{
//			continue;
//		}
//
//		map<int, int>::iterator itS = tempProfessionIt->second.attrTypeCntr.find(type);
//		if (itS != tempProfessionIt->second.attrTypeCntr.end() && attrValue == itS->second && tempProfessionIt->second.quality == quality)
//		{
//			return tempProfessionIt->second.level;
//		}
//	}
//	return -1;
//}
//
////int EquipSmithLoader::GetRefineAttrByLQ(int quality, vector<int> levels, vector<int> types, vector<ReGetAttr>& attrsValue,ECharProf profession)
////{
////	if (levels.size() != types.size())
////	{
////		return -1;
////	}
////
////	for (uint i = 0; i < levels.size(); ++i)
////	{
////		map<pair<int, int>,map<ECharProf,EquipSmith::RefineAttrBase> >::iterator itF = m_refineBaseAttr.find(pair<int, int>(levels[i], quality));
////
////		if (itF != m_refineBaseAttr.end())
////		{
////			map<ECharProf,EquipSmith::RefineAttrBase>::iterator tempProfessionIt = itF->second.find(profession);
////			if(tempProfessionIt == itF->second.end())
////			{
////				continue;
////			}
////
////			map<int, int>::iterator itS = tempProfessionIt->second.attrTypeCntr.find(types[i]);
////			if (tempProfessionIt->second.attrTypeCntr.end() != itS)
////			{
////				ReGetAttr attr;
////				attr.level = tempProfessionIt->second.level;
////				attr.type = itS->first;
////				attr.value = itS->second;
////				attrsValue.push_back(attr);
////			}
////		}
////	}
////
////	return 0;
////}
//
//void EquipSmithLoader::GetRefineAttrByLQ(int equipLv, int quality, vector<ReGetAttr> attrs, vector<ReGetAttr>& attrsValue,ECharProf profession)
//{
//	map<pair<int, int>, map<ECharProf,EquipSmith::RefineAttrBase> >::iterator
//	itF = m_refineBaseAttr.find(pair<int, int>(equipLv, quality));
//
//	if (itF == m_refineBaseAttr.end())
//	{
//		return;
//	}
//
//	map<ECharProf,EquipSmith::RefineAttrBase>::iterator tempProfessionIt = itF->second.find(profession);
//	if(tempProfessionIt ==itF->second.end())
//	{
//		return;
//	}
//
//	std::vector<int> oldPos, newPos;
//
//	for (uint i = 0; i < attrs.size(); ++i)
//	{
//		map<int, int>::iterator itS = tempProfessionIt->second.attrTypeCntr.find(attrs[i].type);
//		if(tempProfessionIt->second.attrTypeCntr.end() == itS)
//		{
//			continue;
//		}
//
//		bool isFind = false;
//		for(size_t n=0; n<attrsValue.size(); ++n)
//		{
//			std::vector<int>::iterator itFind = find(newPos.begin(), newPos.end(), n);
//			if(attrsValue[n].type == attrs[i].type && itFind == newPos.end())
//			{
//				attrsValue[n] = attrs[i];
//				newPos.push_back(n);
//				isFind = true;
//				break;
//			}
//		}
//
//		if(!isFind)
//		{
//			oldPos.push_back(i);
//		}
//	}
//
//	std::vector<int> dest;
//	for(size_t i=0; i<attrsValue.size(); ++i)
//	{
//		std::vector<int>::iterator it = find(newPos.begin(), newPos.end(), i);
//		if(it==newPos.end())
//		{
//			dest.push_back(i);
//		}
//	}
//
//	if(oldPos.size() == 0 || dest.size() == 0)
//	{
//		return;
//	}
//
//	if(oldPos.size() == dest.size())
//	{
//		for(size_t i=0; i<oldPos.size(); ++i)
//		{
//			attrsValue[dest[i]] = attrs[oldPos[i]];
//		}
//	}
//	else if(oldPos.size() > dest.size())
//	{
//		for(size_t i=0; i<dest.size(); ++i)
//		{
//			int index = rand() % oldPos.size();
//			attrsValue[dest[i]] = attrs[oldPos[index]];
//			oldPos.erase(oldPos.begin()+index);
//		}
//	}
//	else
//	{
//		for(size_t i=0; i<oldPos.size(); ++i)
//		{
//			int index = rand() % dest.size();
//			attrsValue[dest[index]] = attrs[oldPos[i]];
//			dest.erase(dest.begin()+index);
//		}
//	}
//
//}
//
//float EquipSmithLoader::GetRefineAttrRatio(int level)
//{
//	map<int, EquipSmith::RefineAttrRatio>::iterator it = m_refineRatio.find(level);
//	if (m_refineRatio.end() != it)
//	{
//		return it->second.ratio;
//	}
//
//	return 0;
//}
//
//int EquipSmithLoader::GetInalyHoleMaxCount()
//{
//	return m_inlayBase.inlayHoleMaxCount;
//}
//
//// 获取镶嵌孔位置对应的类型
//int EquipSmithLoader::GetTypeByInlayHole(int pos, vector<int>& vec)
//{
//	map<int, vector<int> >::iterator it = m_inlayBase.inlayStone.find(pos);
//	if (it != m_inlayBase.inlayStone.end())
//	{
//		vec = it->second;
//		return 0;
//	}
//	return -1;
//}
//
//// 根据宝石类型获取镶嵌孔的位置
//int EquipSmithLoader::GetInlayHolePosByStoneType(int stoneType)
//{
//	for (map<int, vector<int> >::iterator it = m_inlayBase.inlayStone.begin(); it != m_inlayBase.inlayStone.end(); ++it)
//	{
//		for (vector<int>::iterator vecIt = it->second.begin(); vecIt != it->second.end(); ++vecIt)
//		{
//			if (stoneType == *vecIt)
//			{
//				return it->first;
//			}
//		}
//	}
//	return -1;
//}
//
//int EquipSmithLoader::GetStoneLvByLv(int equipLv)
//{
//	map<int, int>::iterator it = m_inlayBase.limitStoneLv.find(equipLv);
//	if (m_inlayBase.limitStoneLv.end() == it)
//	{
//		return -1;
//	}
//
//	return it->second;
//}
//
//int EquipSmithLoader::GetUpProtectItemID()
//{
//	return m_upgradeBase.upProtectItemID;
//}
//
////int EquipSmithLoader::GetUpItemID()
////{
////	return m_upgradeBase.upItemID;
////}
//
//void EquipSmithLoader::GetNotUpItemID(vector<int>& vec)
//{
//	vec = m_notUpItemID;
//}
//
//int EquipSmithLoader::GetUpItemID(int quality)
//{
//	map<int, int>::iterator it = m_upgradeBase.upMaterialID.find(quality);
//	if (it != m_upgradeBase.upMaterialID.end())
//	{
//		return it->second;
//	}
//
//	return 0;
//}
//
//int EquipSmithLoader::GetSucceedProtectItemID()
//{
//	return m_succeedBase.succeedProtectItemID;
//}
//
//// 获取继承道具唯一ID
////int EquipSmithLoader::GetSucceedItemID()
////{
////	return m_succeedBase.succeedItemID;
////}
//
//int EquipSmithLoader::GetSucceedNeedLv()
//{
//	return m_succeedBase.succeedNeedParam;
//}
//
//void EquipSmithLoader::GetNotSucceedItemID(vector<int>& vec)
//{
//	vec = m_notSucceedItemID;
//}
//
//int EquipSmithLoader::GetSucceedItemID(int quality)
//{
//	map<int, int>::iterator it = m_succeedBase.succeedMaterialID.find(quality);
//	if (it != m_succeedBase.succeedMaterialID.end())
//	{
//		return it->second;
//	}
//
//	return 0;
//}
//
//int EquipSmithLoader::GetSucceedRefineCost()
//{
//	return m_succeedBase.succeedRefineAttrCost;
//}
//
//bool EquipSmithLoader::loadStrenghtFile(const string& str)
//{
//	string tempFileName = str + "StrengthTypeData.csv";
//	CSVLoader tempLloader;
//	if (!tempLloader.OpenFromFile(tempFileName.c_str()))
//	{
//		return false;
//	}
//
//	StrengthLevelData tempData;
//	for(unsigned int i = 0;i < tempLloader.GetRowCount();++i)
//	{
//		int tempCounts = 0;
//		tempData.m_Level 				= tempLloader.GetInt(i,tempCounts++);
//		tempData.m_SuccessProbability   = tempLloader.GetInt(i,tempCounts++);
//		tempData.m_DropStar				= tempLloader.GetInt(i,tempCounts++);
//		tempData.m_DropProbability		= tempLloader.GetInt(i,tempCounts++);
//		tempCounts++;
//		tempCounts++;
//		tempData.m_NeedItemCounts		= tempLloader.GetInt(i,tempCounts++);
//		tempData.m_NeedMoney			= tempLloader.GetInt(i,tempCounts++);
//
//		tempCounts++;
//		tempData.m_strengthMaterialID = tempLloader.GetInt(i, tempCounts++);
////		m_strengthBase.strengthMaterialID[tempData.m_Level] = tempLloader.GetInt(i, tempCounts++);
//
//		m_StrengthList[tempData.m_Level] = tempData;
//	}
//
//	return true;
//}
//
//bool EquipSmithLoader::loadUpgradeFile(const string& str)
//{
//	string tempFileName = str + "UpgradeTypeData.csv";
//	CSVLoader tempLloader;
//	if (!tempLloader.OpenFromFile(tempFileName.c_str()))
//	{
//		return false;
//	}
//
//	UpgradeData tempData;
//	for(unsigned int i = 0;i < tempLloader.GetRowCount();++i)
//	{
//		int tempCounts = 0;
//		tempData.m_Level		  = tempLloader.GetInt(i,tempCounts++);
//		tempData.m_DropStar		  = tempLloader.GetInt(i,tempCounts++);
//		tempData.m_NeedItemCounts = tempLloader.GetInt(i,tempCounts++);
//		tempData.m_NeedMoney	  = tempLloader.GetInt(i,tempCounts++);
//
//		m_UpgradeDataList[tempData.m_Level] = tempData;
//	}
//
//	return true;
//}
//
//bool EquipSmithLoader::loadInheritFile(const string& str)
//{
//	string tempFileName = str + "InheritTypeData.csv";
//	CSVLoader tempLloader;
//	if (!tempLloader.OpenFromFile(tempFileName.c_str()))
//	{
//		return false;
//	}
//
//	InheritData tempData;
//	for(unsigned int i = 0;i < tempLloader.GetRowCount();++i)
//	{
//		int tempCounts = 0;
//		tempData.m_StartLevel 	  = tempLloader.GetInt(i,tempCounts++);
//		tempData.m_DropStar 	  = tempLloader.GetInt(i,tempCounts++);
//		tempData.m_NeedItemCounts = tempLloader.GetInt(i,tempCounts++);
//
//		m_InheritList[tempData.m_StartLevel] = tempData;
//	}
//
//	return true;
//}
//
//
//
//
//
//
//
//
