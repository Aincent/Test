/*
 * MaterialSynthesisLoader.cpp
 *
 *  Created on: 2015年9月11日
 *      Author: root
 */

#include "MaterialSynthesisLoader.h"
#include "../CSVLoader.h"
#include "../StrArgPkg.h"

CMeterialSynthesisLoader::CMeterialSynthesisLoader() {
	// TODO Auto-generated constructor stub

}

CMeterialSynthesisLoader::~CMeterialSynthesisLoader() {
	// TODO Auto-generated destructor stub
}

int CMeterialSynthesisLoader::Init(string& path)
{
	string file = path + "./GodMaterialData.csv";
	if (0 != InitFile(file))
	{
		return -1;
	}

	return 0;
}

int CMeterialSynthesisLoader::InitFile(string& file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	vector<int> srcMeterialID;

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		MeterialSynthesisItem item;
		int count = 0;
		count++;
		count++;
		count++;
		count++;
		count++;
		count++;
		item.destMeterialID = loader.GetInt(i, count++);

		std::string strProp = loader.GetString(i, count++);
		StrArgPkg strPkg("|", strProp);
		for(size_t j = 0; j < strPkg.size(); j++)
		{
			StrArgPkg funItem(":", getArg(strPkg, j));
			int srcItemID = atoi(getArg(funItem, 0).c_str()), srcItemNum = atoi(getArg(funItem, 1).c_str()) ;
			item.srcMeterialID.push_back(pair<int, int>(srcItemID, srcItemNum));

			srcMeterialID.push_back(srcItemID);
		}

		count++;
		item.costMoney = loader.GetInt(i, count++);
//		item.isBind = loader.GetBool(i, count++);

		m_meterialSynthesisInfo[item.destMeterialID] = item;

		for (uint i = 0; i < srcMeterialID.size(); ++i)
		{
			int srcItemID = srcMeterialID[i];
			m_synItemBySrcInfo[srcItemID].srcMeterialID = srcItemID;
			m_synItemBySrcInfo[srcItemID].destItems.push_back(item);
		}

		srcMeterialID.clear();
	}
	return 0;
}

MeterialSynthesisItem* CMeterialSynthesisLoader::GetInfoByMaterialID(int destMeterialID)
{
	map<int, MeterialSynthesisItem>::iterator it = m_meterialSynthesisInfo.find(destMeterialID);
	if (it != m_meterialSynthesisInfo.end())
	{
		return &(it->second);
	}
	return NULL;
}
void CMeterialSynthesisLoader::GetInfoBySrcMaterialID(int srcMaterialID, vector<MeterialSynthesisItem>& meterialSynInfo) const
{
//	map<int, MeterialSynthesisItem>::iterator it = m_meterialSynthesisInfo.begin();
//	for ( ; it != m_meterialSynthesisInfo.end(); ++it)
//	{
//		for (uint i = 0; i < it->second.srcMeterialID.size(); ++i)
//		{
//			if (srcMaterialID == it->second.srcMeterialID[i].first)
//			{
//				return &(it->second);
//			}
//		}
//	}
//	return NULL;

	map<int, SrcMeterialSynchesis>::const_iterator it = m_synItemBySrcInfo.find(srcMaterialID);
	if (m_synItemBySrcInfo.end() == it)
	{
		return;
	}

	meterialSynInfo = it->second.destItems;
}

map<int, MeterialSynthesisItem>& CMeterialSynthesisLoader::GetMaterialInfo()
{
	return m_meterialSynthesisInfo;
}
