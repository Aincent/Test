/*
 * EquipSmithExLoader.cpp
 *
 *  Created on: 2016年9月10日
 *      Author: root
 */

#include "EquipSmithExLoader.h"
#include "../CSVLoader.h"
#include "Lua/LuaInterface.h"
#include "../CharDefine.h"
#include "../Goods/EquipData.h"
#include "Log/Logger.h"
#include "../StrArgPkg.h"
#include <math.h>


EquipSmithExLoader::EquipSmithExLoader()
{

}

EquipSmithExLoader::~EquipSmithExLoader()
{

}

int EquipSmithExLoader::Init(string &path)
{
	string strengthTypeData = path + "Data/StrengthTypeData.csv";
	string strengthAttributesAdd = path + "Data/StrengthAttributesAdd.csv";
	string inlayStones = path + "Data/InlayStones.csv";
	string refiningConsume = path + "Data/RefiningConsume.csv";
	string refiningAtt = path + "Data/RefiningAttAdd.csv";
	string equipUpgrade = path + "Data/EquipUpgrade.csv";

	if(InitEquipStrength(strengthTypeData) || InitEquipStrengthAdd(strengthAttributesAdd) || InitInlay(inlayStones) || InitRefineBase(refiningConsume)
			|| InitRefineAttr(refiningAtt) || InitEquipLvLimit(equipUpgrade))
	{
		return -1;
	}

	return 0;
}

int EquipSmithExLoader::InitEquipStrength(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		StrengthBaseParam info;
		int count = 0;

		info.nStrengthLv =  loader.GetInt(i, count++);
		info.nMoney = loader.GetInt(i, count++);
		info.nItemId = loader.GetInt(i, count++);
		info.nItemNum = loader.GetInt(i, count++);

		mStrengthBaseParam[info.nStrengthLv] = info;
	}

	return 0;
}

int EquipSmithExLoader::InitEquipStrengthAdd(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		StrengthLevelData info;
		int count = 0;
		count++;

		//格子要减1 因为我们是下标从0开始
		int nPos = loader.GetInt(i, count++) - 1;
		if(nPos < 0)
		{
			nPos = 0;
		}

		info.nPos = nPos;
		info.nStrengthLv = loader.GetInt(i, count++);

		int nAttr = loader.GetInt(i, count++);
		int nValue = loader.GetInt(i, count++);
		info.mAttr[nAttr] = nValue;

		nAttr = loader.GetInt(i, count++);
		nValue = loader.GetInt(i, count++);
		info.mAttr[nAttr] = nValue;

		nAttr = loader.GetInt(i, count++);
		nValue = loader.GetInt(i, count++);
		info.mAttr[nAttr] = nValue;

		vStrengthLevelData.push_back(info);
	}

	return 0;
}

const StrengthBaseParam* EquipSmithExLoader::GetStrengthBaseParam(int nStrengthLv)
{
	map<int,StrengthBaseParam>::iterator itr = mStrengthBaseParam.find(nStrengthLv);
	if(itr != mStrengthBaseParam.end())
	{
		return &itr->second;
	}

	return NULL;
}

void EquipSmithExLoader::GetStrengthLevelData(int nPos,int nStrengthLv,map<ECharAttrType,int64>& tempAttList)
{
	map<int,int> mAttrType;
	vector<StrengthLevelData>::iterator itr = vStrengthLevelData.begin();
	for(; itr != vStrengthLevelData.end(); ++itr)
	{
		if(itr->nPos == nPos && itr->nStrengthLv == nStrengthLv)
		{
			mAttrType = itr->mAttr;
			break;
		}
	}

	map<int,int>::iterator it = mAttrType.begin();
	for(; it != mAttrType.end(); ++it)
	{
		switch(it->first)
		{
			case eHPStone:
			{
				tempAttList[eCharHPMax] += it->second;

				break;
			}
			case eMPStone:
			{
				tempAttList[eCharMPMax] += it->second;

				break;
			}
			case ePhyATTStone:
			{
				tempAttList[eCharPhyAttackMax] += it->second;

				break;
			}
			case ePhyDefStone:
			{
				tempAttList[eCharPhyDefence] += it->second;

				break;
			}
			case eHitStone:
			{
				tempAttList[eCharHitRate] += it->second;

				break;
			}
			case eAvoidStone:
			{
				tempAttList[eCharAvoidRate] += it->second;

				break;
			}
			case eCrackStone:
			{
				tempAttList[eCrackDownRate]  += it->second;

				break;
			}
			case eAdCrackStone:
			{
				tempAttList[eAvoidCrackRate] += it->second;

				break;
			}
			default:
			{
				break;
			}
		}
	}
}

int EquipSmithExLoader::InitInlay(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		InlayBaseParam info;
		int count = 0;
		count++;
		//格子要减1 因为我们是下标从0开始
		int nPos = loader.GetInt(i, count++) - 1;
		if(nPos < 0)
		{
			nPos = 0;
		}
		info.nInlayPos = nPos;
		info.nStoneType = loader.GetInt(i, count++);
		info.vLv.push_back(loader.GetInt(i, count++));
		info.vLv.push_back(loader.GetInt(i, count++));
		info.vLv.push_back(loader.GetInt(i, count++));

		vInlayBaseParam[info.nInlayPos] = info;
	}

	return 0;
}

int EquipSmithExLoader::GetInlayStoneType(int nPos)
{
	//格子要加1 因为我们是下标从0开始
	map<int,InlayBaseParam>::iterator itr = vInlayBaseParam.find(nPos);
	if(itr != vInlayBaseParam.end())
	{
		return itr->second.nStoneType;
	}

	return -1;
}

int EquipSmithExLoader::GetInlayPosLv(int nPos,int nStonePos)
{
	map<int,InlayBaseParam>::iterator itr = vInlayBaseParam.find(nPos);
	if(itr != vInlayBaseParam.end())
	{
		if(nStonePos >= 0 && nStonePos < (int)itr->second.vLv.size())
		{
			return itr->second.vLv[nStonePos];
		}
	}

	return -1;
}

int EquipSmithExLoader::InitRefineBase(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;

		int nId = loader.GetInt(i, count++);
		if(nId == 1)
		{
			mRefine.nMoney = loader.GetInt(i, count++);
			mRefine.nItemId = loader.GetInt(i, count++);
			mRefine.nItemNum = loader.GetInt(i, count++);
		}
		else
		{
			mRefine.nGolden = loader.GetInt(i, count++);
		}
	}

	return 0;
}

int EquipSmithExLoader::InitRefineAttr(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		RefineAttrBase info;
		int count = 0;
		count++;

		info.nStar = loader.GetInt(i, count++);
		info.nHP = loader.GetInt(i, count++);
		info.nPhyAttack = loader.GetInt(i, count++);
		info.nPhyDef = loader.GetInt(i, count++);
		info.nHitRate = loader.GetInt(i, count++);
		info.nAvoidRate = loader.GetInt(i, count++);
		info.nDownRate = loader.GetInt(i, count++);
		info.nCrackRate = loader.GetInt(i, count++);

		vRefineAttrBase.push_back(info);
	}

	return 0;
}

const RefineBaseParam* EquipSmithExLoader::GetRefineBaseParam()
{
	return &mRefine;
}

int EquipSmithExLoader::CalEquipRefineAttr(map<int, ReGetAttr>& attrs)
{
	RefineAttrBase refineBaseMax = vRefineAttrBase[vRefineAttrBase.size() - 1];
	if(refineBaseMax.nStar <= 0)
	{
		return -1;
	}

	int pos = 0;
	//eCharHP
	{
		int value  = rand() % refineBaseMax.nHP + 1;
		ReGetAttr attr;
		attr.type = eCharHP;
		attr.value = value;
		attr.level = GetRefineLv(value,eCharHP);
		attrs[pos++] = attr;
	}

	//eCharPhyAttackMax
	{
		int value  = rand() % refineBaseMax.nPhyAttack + 1;
		ReGetAttr attr;
		attr.type = eCharPhyAttackMax;
		attr.value = value;
		attr.level = GetRefineLv(value,eCharPhyAttackMax);
		attrs[pos++] = attr;
	}

	//eCharPhyDefence
	{
		int value  = rand() % refineBaseMax.nPhyDef + 1;
		ReGetAttr attr;
		attr.type = eCharPhyDefence;
		attr.value = value;
		attr.level = GetRefineLv(value,eCharPhyDefence);
		attrs[pos++] = attr;
	}

	//eCharHitRate
	{
		int value  = rand() % refineBaseMax.nHitRate + 1;
		ReGetAttr attr;
		attr.type = eCharHitRate;
		attr.value = value;
		attr.level = GetRefineLv(value,eCharHitRate);
		attrs[pos++] = attr;
	}

	//eCharAvoidRate
	{
		int value  = rand() % refineBaseMax.nAvoidRate + 1;
		ReGetAttr attr;
		attr.type = eCharAvoidRate;
		attr.value = value;
		attr.level = GetRefineLv(value,eCharAvoidRate);
		attrs[pos++] = attr;
	}

	//eCrackDownRate
	{
		int value  = rand() % refineBaseMax.nDownRate + 1;
		ReGetAttr attr;
		attr.type = eCrackDownRate;
		attr.value = value;
		attr.level = GetRefineLv(value,eCrackDownRate);
		attrs[pos++] = attr;
	}

	//eAvoidCrackRate
	{
		int value  = rand() % refineBaseMax.nCrackRate + 1;
		ReGetAttr attr;
		attr.type = eAvoidCrackRate;
		attr.value = value;
		attr.level = GetRefineLv(value,eAvoidCrackRate);
		attrs[pos++] = attr;
	}

	return 0;
}

int EquipSmithExLoader::GetRefineLv(int nValue,int nType)
{
	size_t i = 0;
	switch(nType)
	{
	case eCharHP:
			{
				for(; i < vRefineAttrBase.size();i++)
				{
					if(vRefineAttrBase[i].nHP >= nValue)
					{
						break;
					}
				}

				break;
			}
		case eCharPhyAttackMax:
			{
				for(; i < vRefineAttrBase.size();i++)
				{
					if(vRefineAttrBase[i].nPhyAttack >= nValue)
					{
						break;
					}
				}

				break;
			}
		case eCharPhyDefence:
			{
				for(; i < vRefineAttrBase.size();i++)
				{
					if(vRefineAttrBase[i].nPhyDef >= nValue)
					{
						break;
					}
				}

				break;
			}
		case eCharHitRate:
			{
				for(; i < vRefineAttrBase.size();i++)
				{
					if(vRefineAttrBase[i].nHitRate >= nValue)
					{
						break;
					}
				}

				break;
			}
		case eCharAvoidRate:
			{
				for(; i < vRefineAttrBase.size();i++)
				{
					if(vRefineAttrBase[i].nAvoidRate >= nValue)
					{
						break;
					}
				}

				break;
			}
		case eCrackDownRate:
			{
				for(; i < vRefineAttrBase.size();i++)
				{
					if(vRefineAttrBase[i].nDownRate >= nValue)
					{
						break;
					}
				}

				break;
			}
		case eAvoidCrackRate:
			{
				for(; i < vRefineAttrBase.size();i++)
				{
					if(vRefineAttrBase[i].nCrackRate >= nValue)
					{
						break;
					}
				}

				break;
			}
		default:
			{
				break;
			}
	}

	return i+1;
}

int EquipSmithExLoader::InitEquipLvLimit(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		UpgradeBaseParam info;
		int count = 0;

		info.nId = loader.GetInt(i, count++);
		info.nEquipType = loader.GetInt(i, count++);
		info.nEquipQuality = loader.GetInt(i, count++);
		info.nEquipLv = loader.GetInt(i, count++);
		info.nMoney = loader.GetInt(i, count++);
		info.nItemId = loader.GetInt(i, count++);
		info.nItemNum = loader.GetInt(i, count++);

		vUpgradeBaseParam.push_back(info);
	}

	return 0;
}

const UpgradeBaseParam* EquipSmithExLoader::GetUpgradeBaseParam(int nEquipType,int nEquipQuality,int nEquipLv)
{
	for(size_t i = 0; i < vUpgradeBaseParam.size(); i++)
	{
		if(vUpgradeBaseParam[i].nEquipType == nEquipType && vUpgradeBaseParam[i].nEquipQuality == nEquipQuality && vUpgradeBaseParam[i].nEquipLv > nEquipLv)
		{
			--i;
			if(i <= 0)
				i = 0;

			return &vUpgradeBaseParam[i];
		}
	}

	return NULL;
}
