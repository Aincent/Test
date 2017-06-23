/*
 * EquipSmithExLoader.h
 *
 *  Created on: 2016年9月10日
 *      Author: root
 */

#ifndef EQUIPSMITHEXLOADER_H_
#define EQUIPSMITHEXLOADER_H_

#include <map>
#include "define.h"
#include "Singleton.h"
#include "EquipSmithStruct.h"

using namespace CommBaseOut;
using namespace std;

class ReGetAttr;

class EquipSmithExLoader : public Singleton<EquipSmithExLoader>
{
public:
	EquipSmithExLoader();
	~EquipSmithExLoader();

	int Init(string &path);

	//初始化强化
	int InitEquipStrength(string& file);
	int InitEquipStrengthAdd(string& file);
	const StrengthBaseParam* GetStrengthBaseParam(int nStrengthLv);
	void GetStrengthLevelData(int nPos,int nStrengthLv,map<ECharAttrType,int64>& tempAttList);

	// 初始化镶嵌
	int InitInlay(string& file);
	int GetInlayStoneType(int nPos);
	int GetInlayPosLv(int nPos,int nStonePos);

	//初始化洗炼
	int InitRefineBase(string& file);
	int InitRefineAttr(string& file);

	const RefineBaseParam* GetRefineBaseParam();
	int CalEquipRefineAttr(map<int, ReGetAttr>& attrs);
	int GetRefineLv(int nValue,int nType);


	//初始化装备升级
	int InitEquipLvLimit(string& file);
	const UpgradeBaseParam* GetUpgradeBaseParam(int nEquipType,int nEquipQuality, int nEquipLv);
private:
	map<int,StrengthBaseParam> mStrengthBaseParam;
	vector<StrengthLevelData> vStrengthLevelData;
	map<int,InlayBaseParam> vInlayBaseParam;
	RefineBaseParam mRefine;
	vector<RefineAttrBase> vRefineAttrBase;
	vector<UpgradeBaseParam> vUpgradeBaseParam;
};

#endif /* EQUIPSMITHEXLOADER_H_ */
