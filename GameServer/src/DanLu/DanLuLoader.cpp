
/*
 * DanLuLoader.cpp
 *
 *  Created on: 2014年8月7日
 *      Author: root
 */

#include "DanLuLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include <math.h>

int DanLuLoader::InitFile(const string &file)
{
	string Danlufile = file + "Data/StoveTypeData.csv";

	string DanEquipfile = file + "Data/StoveValueTypeData.csv";

	string DanLuReclaimfile = file + "Data/ReclaimDanLu.csv";

	int ret = InitDanLu(Danlufile);
	if (ret != 0)
		return -1;

	ret = InitDanEquip(DanEquipfile);
	if (ret != 0)
		return -1;

	ret = InitDanLuReclaim(DanLuReclaimfile);
	return ret;
}

int DanLuLoader::InitDanLu(const string& file)
{
	CSVLoader loader;
	if (!loader.OpenFromFile(file.c_str()))
		return -1;

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		LianLuInfo info;
		int count = 0;

		info.id = loader.GetInt(i, count++, 0);
		count += 2;

		info.quality = loader.GetInt(i, count++, 0);
		info.start = loader.GetInt(i, count++, 0);
		info.needexp = loader.GetInt(i, count++, 0);
		info.upNeedNum = loader.GetInt(i, count++, 0);

		int pro = 0;
		uint j = count;
		for(; j < loader.GetColCount() - 1;)
		{
			DanLuFunInfo danInfo;
			pro++;

			danInfo.blood 		= loader.GetInt(i, j++, 0);
			danInfo.phyatk 		= loader.GetInt(i, j++, 0);
			danInfo.phydefend   = loader.GetInt(i, j++, 0);
			danInfo.m_Hit 	    = loader.GetInt(i, j++, 0);
			danInfo.m_Dodge 	= loader.GetInt(i, j++, 0);
			danInfo.crackrate   = loader.GetInt(i, j++, 0);
			danInfo.crackdefend = loader.GetInt(i, j++, 0);

			if (danInfo.blood>0)
			{
				info.m_danluFun[pro] = danInfo;
			}
			if (pro == eShooter)
				break;
		}

		if (info.m_danluFun.size() != eShooter)
			continue;   ///表格不对

		j++;

		string funStrItem = loader.GetString(i, j++, "");
		StrArgPkg funPkgItem("|", funStrItem);

		for(uint m = 0; m < funPkgItem.size(); ++m)
		{
			string strItem = getArg(funPkgItem, m);
			StrArgPkg pkgItem(":", strItem);

			DanLuInfo::Produce item;
			item.m_produceItem.m_itemID = atoi(getArg(pkgItem, 0).c_str());
			item.m_Probability = atoi(getArg(pkgItem, 1).c_str());
			item.m_produceItem.m_itemCount = atoi(getArg(pkgItem, 2).c_str());

			if(item.m_produceItem.m_itemID  > 0 && item.m_produceItem.m_itemCount > 0)
			{
				info.m_produceItem.push_back(item);
			}
		}

		info.m_NeedMoney = loader.GetInt(i, j++, 0);

		info.m_LevelLimit = loader.GetInt(i, j++, 0);

		info.m_ItemID = loader.GetInt(i, j++, 0);
		info.m_BaseExp = loader.GetInt(i, j++, 0);
		info.m_MinCrit = loader.GetFloat(i, j++, 0);
		info.m_MinCritRate = loader.GetFloat(i, j++, 0);
		info.m_MaxCrit = loader.GetFloat(i, j++, 0);
		info.m_MaxCritRate = loader.GetFloat(i, j++, 0);

		m_allDanLu[info.id] = info;

		if (i == (loader.GetRowCount()-1))
		{
			m_maxDanluID = info.id;
		}
	}

	return 0;
}

int DanLuLoader::InitDanEquip(const string& file)
{
	CSVLoader loader;
	if (!loader.OpenFromFile(file.c_str()))
		return -1;

	for (uint i = 0; i < loader.GetRowCount(); ++i)
	{
		int count = 0;
		count++;

		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		for(uint m = 0; m < funPkgItem.size(); ++m)
		{
			int lvparm = atoi(getArg(funPkgItem, m).c_str());
			if(lvparm > 0)
				m_lianEquipParm.m_lvParm.push_back(lvparm);
		}

		string funStrItem2 = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem2("|", funStrItem2);
		for(uint m = 0; m < funPkgItem2.size(); ++m)
		{
			int quareparm = atoi(getArg(funPkgItem2, m).c_str());
			if(quareparm > 0)
				m_lianEquipParm.m_quareParm.push_back(quareparm);
		}

		m_lianEquipParm.pointCeil = loader.GetInt(i, count++);
		m_lianEquipParm.awardStack = loader.GetInt(i, count++);

	}

	return 0;
}

int DanLuLoader::InitDanLuReclaim(const string &file)
{
	CSVLoader loader;
	if (!loader.OpenFromFile(file.c_str()))
		return -1;

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		DanLuReclaim info;
		int count = 0;

		info.nId = loader.GetInt(i, count++);
		info.nType = loader.GetInt(i, count++);
		info.nQuality = loader.GetInt(i, count++);
		info.nStar = loader.GetInt(i, count++);
		info.nItemId = loader.GetInt(i, count++);
		info.nItemNum = loader.GetInt(i, count++);

		m_DanLuReclaim.push_back(info);
	}

	return 0;
}

int DanLuLoader::CalExchangePoint(int useLv, int quality)
{
	int lvIndex = useLv/10;

	if (lvIndex > (int)m_lianEquipParm.m_lvParm.size()
			|| lvIndex<0
			|| quality >(int)m_lianEquipParm.m_quareParm.size() || quality<0)
		return 0;

	return int(ceil(double(m_lianEquipParm.m_lvParm[lvIndex]) * m_lianEquipParm.m_quareParm[quality]));
}

int DanLuLoader::CalExchangeExp(int useLv, int quality)
{
	return 0;
}


int DanLuLoader::GetLianEquipCeiling()
{
	return m_lianEquipParm.pointCeil;
}

int DanLuLoader::getMaxID()
{
	if(m_allDanLu.size() == 0)
	{
		return 0;
	}

	map<int, LianLuInfo>::iterator tempIt = m_allDanLu.end();

	--tempIt;

	return tempIt->first;
}

const DanLuReclaim* DanLuLoader::GetDanLuReclaim(int nType,int nQuality,int nStar)
{
	vector<DanLuReclaim>::iterator itr = m_DanLuReclaim.begin();
	for(; itr != m_DanLuReclaim.end(); ++itr)
	{
		if(itr->nType == nType)
		{
				if(nType == eReclaim_Strengthen && itr->nQuality == nQuality && itr->nStar == nStar)
				{
					return &(*itr);
				}

				if(nType == eReclaim_Purification && itr->nQuality == nQuality)
				{
					return &(*itr);
				}
		}
	}

	return NULL;
}




