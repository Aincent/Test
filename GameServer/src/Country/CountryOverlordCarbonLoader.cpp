/*
 * CountryOverlordCarbonLoader.cpp
 *
 *  Created on: 2016年10月28日
 *      Author: root
 */

#include "CountryOverlordCarbonLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "util.h"
#include <math.h>

CountryOverlordCarbonLoader::CountryOverlordCarbonLoader():m_nKillPlayer(0),m_nKillBoss(0),m_nAttackBoss(0)
{
	m_nBothPont.clear();
	m_nOverlordBaseData.clear();
	m_nOverlordReward.clear();
}

CountryOverlordCarbonLoader::~CountryOverlordCarbonLoader()
{

}

int CountryOverlordCarbonLoader::Init(string &path)
{
	string campKingWarData = path + "Data/CampKingWarData.csv";
	string campKingWarReward = path + "Data/CampKingWarReward.csv";
	if(InitCampKingWarData(campKingWarData) || InitCampKingWarReward(campKingWarReward))
	{
		return -1;
	}

	return 0;
}


int CountryOverlordCarbonLoader::InitCampKingWarData(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 1;
		int nType = loader.GetInt(i, count++);
		switch(nType)
		{
		case 1:
			{
				pont p;
				p.xpos = loader.GetInt(i, count++);
				p.ypos = loader.GetInt(i, count++);
				m_nBothPont[eCountryID_sui] = p;
				break;
			}
		case 2:
			{
				pont p;
				p.xpos = loader.GetInt(i, count++);
				p.ypos = loader.GetInt(i, count++);
				m_nBothPont[eCountryID_tang] = p;
				break;
			}
		case 3:
			{
				OverlordBaseData info;
				info.m_nXpos = loader.GetInt(i, count++);
				info.m_nYpos = loader.GetInt(i, count++);
				info.m_nMosterId = loader.GetInt(i, count++);
				info.m_nCount = loader.GetInt(i, count++);
				info.m_nFlushTime = loader.GetInt(i, count++);
				m_nOverlordBaseData.push_back(info);
				break;
			}
		case 4:
			{
				m_nKillPlayer = loader.GetInt(i, count++);
				break;
			}
		case 5:
			{
				m_nKillBoss = loader.GetInt(i, count++);
				break;
			}
		case 6:
			{
				m_nAttackBoss = loader.GetInt(i, count++);
				break;
			}
		default:
			break;
		}
	}

	return 0;
}

int CountryOverlordCarbonLoader::InitCampKingWarReward(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 1;
		OverlordReward info;
		info.m_nType = loader.GetInt(i, count++);
		info.m_nNeedPoint = loader.GetInt(i, count++);
		info.m_nAwardExp = loader.GetInt(i, count++);
		info.m_nAwardMoney = loader.GetInt(i, count++);
		info.m_nAwardSprit = loader.GetInt(i, count++);
		info.m_nAwardCoun = loader.GetInt(i, count++);

		//限制使用物品
		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			for(unsigned int i=0; i<limitPkg.size(); ++i)
			{
				info.m_nAward.push_back(atoi(getArg(limitPkg, i++).c_str()));
				info.m_nAwardNum.push_back(atoi(getArg(limitPkg, i).c_str()));
			}
		}

		m_nOverlordReward.push_back(info);
	}

	return 0;
}

void CountryOverlordCarbonLoader::GetBothPos(DWORD nType,int64 &xpos, int64 &ypos)
{
	map<DWORD,pont>::iterator itr = m_nBothPont.find(nType);
	if(itr != m_nBothPont.end())
	{
		xpos = itr->second.xpos;
		ypos = itr->second.ypos;
	}
	else
	{
		xpos = -1;
		ypos = -1;
	}
}

const OverlordBaseData* CountryOverlordCarbonLoader::GetOverlordBaseData()
{
	vector<OverlordBaseData>::iterator itr = m_nOverlordBaseData.begin();
	if(itr != m_nOverlordBaseData.end())
	{
		return &(*itr);
	}

	return NULL;
}

const OverlordReward* CountryOverlordCarbonLoader::GetOverlordReward(DWORD nType,DWORD nNeed)
{
	vector<OverlordReward>::iterator itr = m_nOverlordReward.begin();
	for(; itr != m_nOverlordReward.end(); ++itr)
	{
		if(itr->m_nType == nType && itr->m_nNeedPoint == nNeed)
		{
			return &(*itr);
		}
	}

	return NULL;
}
