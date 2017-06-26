/*
 * GoodsLoader.cpp
 *
 *  Created on: 2014年3月28日
 *      Author: root
 */
#include "GoodsLoader.h"
#include "../CSVLoader.h"
#include "../StrArgPkg.h"
#include "GameParaLoader.h"
#include "../Probability/Probability.h"


//获取包含物品概率
void ContainData::getItemList(vector<DWORD>& itemIDList,vector<DWORD>& numberList,ECharProf jobType)const
{
	//先从m_ContainOneList中取物品，如果没有，则从m_ContainTowList中取
	if(m_ContainOneList.size() > 0)
	{
		for(size_t i = 0;i < m_ContainOneList.size();++i)
		{
			itemIDList.push_back(m_ContainOneList[i].m_ItemID);
			numberList.push_back(m_ContainOneList[i].m_Counts);
		}
		return ;
	}

	map<ECharProf,ContainItemList>::const_iterator tempIt = m_ContainTowList.find(jobType);
	if(tempIt == m_ContainTowList.end())
		return ;

	ProbabilityVector<ContainItem> tempHandle(tempIt->second.m_ItemList);
	short tempIndex = tempHandle.calculation();
	if(tempIndex == INVALID_INDEX)
		return ;

	itemIDList.push_back(tempIt->second.m_ItemList[tempIndex].m_ItemID);
	numberList.push_back(tempIt->second.m_ItemList[tempIndex].m_Counts);

	return ;
}


GoodsLoader * GoodsLoader::m_instance = 0;

GoodsLoader::GoodsLoader()
{
	//m_GoodsInfo.clear();
}

GoodsLoader::~GoodsLoader()
{

}

int GoodsLoader::Init(string &file)
{
	string sItemTypeDataPath = file + "Data/ItemTypeData.txt";
	string tempSuitStr 		 = file + "Data/SuitAttribute.csv";

	if(InitItemTypeData(sItemTypeDataPath) || initSuitData(tempSuitStr))
	{
		return -1;
	}

	return 0;
}

int GoodsLoader::InitItemTypeData(string &file)
{
	CSVLoader loader('	');

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		GoodsInfo info;
		int type = -1;

		info.itemID = loader.GetInt(i, count++);
		count++;
		count++;
		info.itemName = loader.GetString(i,count++);
		info.type = loader.GetInt(i, count++);

		//拍卖类型
		int saletype = loader.GetInt(i, count++);
		info.bigtype = saletype/100;
		info.smalltype = saletype%100;

		info.itemdesic = loader.GetString(i,count++);

		info.useLvReq = loader.GetInt(i, count++);
		info.itemQuality = loader.GetInt(i, count++);
		count++;
		info.valideSecs = loader.GetInt(i, count++);
		info.effect = loader.GetInt(i, count++);
		info.buffID = loader.GetInt(i, count++);
		info.price = loader.GetInt(i, count++);
		info.salePrice = loader.GetInt(i, count++);

		info.bindTyle = loader.GetInt(i, count++);
		info.stack = loader.GetInt(i, count++);

		count++;
		count++;

		info.telCD = loader.GetInt(i, count++);
		info.useCD = loader.GetInt(i, count++);
		info.advanceUseMon = loader.GetInt(i, count++);
		info.reflushUseMon = loader.GetInt(i, count++);
		info.addUseMoney = loader.GetInt(i, count++);


		string itemInclude = loader.GetString(i, count++, "");
		StrArgPkg includePkgItem("|", itemInclude);

		for(uint itemSize=0; itemSize<includePkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(includePkgItem, itemSize).c_str());
			ContainItem tempData;
			tempData.m_ItemID = atoi(getArg(limitPkg, 0).c_str());
			tempData.m_Counts = atoi(getArg(limitPkg, 1).c_str());
			if(tempData.m_Counts <= 0)
				tempData.m_Counts = 1;

			info.m_ContainData.m_ContainOneList.push_back(tempData);
		}

		string equipInclude = loader.GetString(i, count++, "");
		StrArgPkg equipPkgItem("|", equipInclude);

		//前三个为对应职业
		for(int itemSize = eMuscleMan; itemSize <= eShooter; ++itemSize)
		{
			int tempIndex = itemSize - 1;
			StrArgPkg limitPkg(";", getArg(equipPkgItem,tempIndex).c_str());
			for(unsigned int z=0; z < limitPkg.size(); ++z)
			{
				StrArgPkg tempItemPkg(":",getArg(limitPkg, z).c_str());

				ContainItem tempItemData;
				tempItemData.m_ItemID 	   = atoi(getArg(tempItemPkg,0).c_str());
				tempItemData.m_Probability = atoi(getArg(tempItemPkg,1).c_str());
				tempItemData.m_Counts      = atoi(getArg(tempItemPkg,2).c_str());

				info.m_ContainData.m_ContainTowList[(ECharProf)itemSize].m_ItemList.push_back(tempItemData);
			}
		}

		//第四个为填写强化等级
		info.equipStrengthLv = atoi(getArg(equipPkgItem, eShooter).c_str());

		info.wearJob= loader.GetInt(i, count++);
		count++;
		info.suitID = loader.GetInt(i, count++);
		count++;
		info.suitrnt = loader.GetInt(i, count++);

		string funStr1 = loader.GetString(i, count++, "");
		StrArgPkg funPkg1("|", funStr1);
		info.suitptyone.type = atoi(getArg(funPkg1, 0).c_str());
		info.suitptyone.data = atoi(getArg(funPkg1, 1).c_str());

		string funStr2 = loader.GetString(i, count++, "");
		StrArgPkg funPkg2("|", funStr2);
		info.suitptytwo.type = atoi(getArg(funPkg2, 0).c_str());
		info.suitptytwo.data = atoi(getArg(funPkg2, 1).c_str());

		string funStr3 = loader.GetString(i, count++, "");
		StrArgPkg funPkg3("|", funStr3);
		info.suitptythree.type = atoi(getArg(funPkg3, 0).c_str());
		info.suitptythree.data = atoi(getArg(funPkg3, 1).c_str());

		info.itemPlace = loader.GetInt(i, count++);
		info.inlayNum = loader.GetInt(i, count++);
		info.reflushItem = loader.GetInt(i, count++);

		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);

		for(size_t itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			info.updndsll.push_back(atoi(getArg(funPkgItem, itemSize).c_str()));
		}

		info.ctegiveID = loader.GetInt(i, count++);
		info.basehp = loader.GetInt(i, count++);
		if(info.type == eStone && info.basehp > 0)
		{
			type = eHPStone;
		}
		info.basemp = loader.GetInt(i, count++);
		if(info.type == eStone && info.basemp > 0)
		{
			type = eMPStone;
		}
		info.baphyackmin = loader.GetInt(i, count++);
		info.baphyackmax = loader.GetInt(i, count++);
		if(info.type == eStone && info.baphyackmax > 0)
		{
			type = ePhyATTStone;
		}
		info.baphydefmin = loader.GetInt(i, count++);
		info.baphydefmax = loader.GetInt(i, count++);
		if(info.type == eStone && info.baphydefmax > 0)
		{
			type = ePhyDefStone;
		}
		info.bamagackmin = loader.GetInt(i, count++);
		info.bamagackmax = loader.GetInt(i, count++);
		if(info.type == eStone && info.bamagackmax > 0)
		{
			type = eMagATTStone;
		}
		info.bamagdefmin = loader.GetInt(i, count++);
		info.bamagdefmax = loader.GetInt(i, count++);
		if(info.type == eStone && info.bamagdefmax > 0)
		{
			type = eMagDefStone;
		}
		info.basehit = loader.GetInt(i, count++);
		if(info.type == eStone && info.basehit > 0)
		{
			type = eHitStone;
		}
		info.basedodge = loader.GetInt(i, count++);
		if(info.type == eStone && info.basedodge > 0)
		{
			type = eAvoidStone;
		}
		info.basecrit = loader.GetInt(i, count++);
		if(info.type == eStone && info.basecrit > 0)
		{
			type = eCrackStone;
		}
		info.baseung= loader.GetInt(i, count++);
		if(info.type == eStone && info.baseung > 0)
		{
			type = eAdCrackStone;
		}
		info.baselucky = loader.GetInt(i, count++);
		if(info.type == eStone && info.baselucky > 0)
		{
			type = eLuckyStone;
		}
		info.baserte = loader.GetInt(i, count++);
		if(info.type == eStone && info.baserte > 0)
		{
			type = eReagainStone;
		}
		info.aonhpmin = loader.GetInt(i, count++);
		info.aonhpmax = loader.GetInt(i, count++);
		info.aonmpmin = loader.GetInt(i, count++);
		info.aonmpmax = loader.GetInt(i, count++);
		info.aonphyackmin = loader.GetInt(i, count++);
		info.aonphyackmax = loader.GetInt(i, count++);
		info.aonmagackmin = loader.GetInt(i, count++);
		info.aonmagackmax = loader.GetInt(i, count++);
		info.aonphydefmin = loader.GetInt(i, count++);
		info.aonphydefmax = loader.GetInt(i, count++);
		info.aonmagdefmin = loader.GetInt(i, count++);
		info.aonmagdefmax = loader.GetInt(i, count++);
		info.aonhitmin = loader.GetInt(i, count++);
		info.aonhitmax = loader.GetInt(i, count++);
		info.aondodgemin = loader.GetInt(i, count++);
		info.aondodgemax = loader.GetInt(i, count++);
		info.aoncritmin = loader.GetInt(i, count++);
		info.aoncritmax = loader.GetInt(i, count++);
		info.aonungmin = loader.GetInt(i, count++);
		info.aonungmax = loader.GetInt(i, count++);
		info.aonrtemin = loader.GetInt(i, count++);
		info.aonrtemax = loader.GetInt(i, count++);
		info.useTimesMax = loader.GetInt(i, count++);
		count++;
		info.needToBroadCast = loader.GetInt(i, count++);
		info.questId = loader.GetInt(i, count++);
		info.stoneType = loader.GetInt(i, count++);

		//宝石位置和物品产出途径，只有前端使用
		loader.GetInt(i, count++);
		loader.GetString(i, count++);

		if(info.stoneType >0)
		{
			int add = info.GetStoneAttr();
			if(add <=0)
				continue;   //表格不对
		}

		m_GoodsInfo[info.itemID] = info;
	}

	return 0;
}

int GoodsLoader::initSuitData(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	int count       	  = 0;
	unsigned int tempRow  = loader.GetRowCount();
	int tempSurplus 	  = 0;
	ECharAttrType tempAtt = eCharAttrMax;
	int tempValue 	 	  = 0;

	for(unsigned int i = 0; i < tempRow; ++i)
	{
		SuitData tempData;
		count = 0;
		count++;
		count++;
		count++;
		tempData.m_SuitID = loader.GetInt(i, count++);
		tempData.m_Counts = loader.GetInt(i, count++);

		//读取属性
		tempSurplus = loader.GetInt(i, count++);
		for(int j = 0;j < tempSurplus;++j)
		{
			tempAtt   = static_cast<ECharAttrType>(loader.GetInt(i, count++));
			tempValue = loader.GetInt(i, count++);
			tempData.m_AttList[tempAtt] = tempValue;
		}

		m_SuitInfo[tempData.m_SuitID][tempData.m_Counts] = tempData;
	}

	return 0;
}

int GoodsLoader::GetSpecliItemFunPlayerAttr(const int& itemID)
{
	if(itemID <=0)
		return -1;

	const int moneyItemID = GameParaLoader::GetInstance()->getExchangeMoneyItemId();
	const int expItemID = GameParaLoader::GetInstance()->getExchangeExpItemId();
	const int forceItemID = GameParaLoader::GetInstance()->getExchangeForceItemId();
	const int goldenItemID = GameParaLoader::GetInstance()->getExchangeGoldenItemId();
	const int bindGoldenItemID = GameParaLoader::GetInstance()->getExchangeBindGoldenItemId();
	const int creditItemID = GameParaLoader::GetInstance()->getExchangeCreditItemId();
	const int challengeItemID = GameParaLoader::GetInstance()->getExchangeChallengeItem();

	int type = 0;
	if(itemID == moneyItemID)
	{
		type = eCharMoney;
	}
	else if(itemID == expItemID)
	{
		type = eCharExp;
	}
	else if(itemID == forceItemID)
	{
		type = eCharForce;
	}
	else if(itemID == goldenItemID)
	{
		type = eCharGolden;
	}
	else if(itemID == bindGoldenItemID)
	{
		type = eCharBindGolden;
	}
	else if(itemID == creditItemID)
	{
		type = eCredit;
	}
	else if(itemID == challengeItemID)
	{
		type = eChallengeMoney;
	}

	return type;
}

//根据物品ID和个数获得对应增加的属性
map<ECharAttrType,int> GoodsLoader::getSuitAttListByIDAndCounts(int suitID,int counts)
{
	map<ECharAttrType,int> tempResult;
	if(counts <= 0 || suitID == 0)
		return tempResult;

	map<int,map<int,SuitData> >::iterator tempIt = m_SuitInfo.find(suitID);
	if(tempIt == m_SuitInfo.end())
		return tempResult;

	map<ECharAttrType,int>::iterator tempAttIt;
	map<int,SuitData>::reverse_iterator  tempInIt = tempIt->second.rbegin();
	for(;tempInIt != tempIt->second.rend();++tempInIt)
	{
		if(tempInIt->second.m_Counts <= counts)
		{
			tempAttIt = tempInIt->second.m_AttList.begin();
			for(;tempAttIt != tempInIt->second.m_AttList.end();++tempAttIt)
			{
				tempResult[tempAttIt->first] += tempAttIt->second;
			}
		}
	}

	return tempResult;
}










