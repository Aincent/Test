/*
 * MaterialSynthesisLoader.h
 *
 *  Created on: 2015年9月11日
 *      Author: root
 */

#ifndef MATERIALSYNTHESISLOADER_H_
#define MATERIALSYNTHESISLOADER_H_

#include "Singleton.h"
#include "define.h"

using namespace std;
using namespace CommBaseOut;

typedef struct sMeterialSynthesisItem
{
	int destMeterialID;
	vector<pair<int, int> > srcMeterialID;
	int costMoney;
	bool isBind; // 合成后是否绑定

	sMeterialSynthesisItem() : destMeterialID(0), costMoney(0), isBind(true)
	{
		srcMeterialID.clear();
	}

}MeterialSynthesisItem;

struct SrcMeterialSynchesis
{
	int srcMeterialID;
	vector<sMeterialSynthesisItem> destItems;
};
class CMeterialSynthesisLoader : public Singleton<CMeterialSynthesisLoader>
{
public:
	CMeterialSynthesisLoader();
	~CMeterialSynthesisLoader();

	int Init(string& path);
	int InitFile(string& file);

	// 根据目标材料
	MeterialSynthesisItem* GetInfoByMaterialID(int destMeterialID);
	// 根据源材料获得第一个合成信息
	void GetInfoBySrcMaterialID(int srcMaterialID,  vector<MeterialSynthesisItem>& meterialSynInfo) const;
	map<int, MeterialSynthesisItem>& GetMaterialInfo();


private:
	map<int, MeterialSynthesisItem> m_meterialSynthesisInfo; // 目标合成物品信息
	map<int, SrcMeterialSynchesis> m_synItemBySrcInfo; // 源物品合成信息
};

#endif /* MATERIALSYNTHESISLOADER_H_ */
