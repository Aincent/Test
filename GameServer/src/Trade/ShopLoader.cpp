/*
 * ShopLoader.cpp
 *
 *  Created on: 2016年11月4日
 *      Author: root
 */

#include "ShopLoader.h"
#include "CSVLoader.h"
#include "Log/Logger.h"
#include "StrArgPkg.h"

ShopLoader::ShopLoader():m_nTotalProbalility(0)
{

}

ShopLoader::~ShopLoader()
{

}

int ShopLoader::Init(string &path)
{
	string carryShop = path + "Data/AnywhereShop.csv";
	string mysteryShop = path + "Data/MysteryShop.csv";
	string clanShop = path + "Data/GangShop.csv";
	string campShop = path + "Data/CampShop.csv";
	string creditShop = path + "Data/ReputationShop.csv";
	string honorShop = path + "Data/HonorShop.csv";
	if(InitCarryShop(carryShop) || InitMysteryShop(mysteryShop) || InitClanShop(clanShop) || InitCampShop(campShop) || InitCreditShop(creditShop)
			||InitHonorShop(honorShop))
	{
		return -1;
	}

	vector<ShopEntry> sEntry;
	GetFlushMyster(sEntry);

	return 0;
}

int ShopLoader::InitCarryShop(string& file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		ShopEntry info;
		info.m_nType = eCarryShop;
		info.m_nId = loader.GetInt(i, count++);
		info.m_nItemId = loader.GetInt(i, count++);
		info.m_nItemCount = loader.GetInt(i, count++);
		info.m_nPrice = loader.GetInt(i, count++);

		m_ShopEntry[info.m_nType][info.m_nItemId] = info;
	}

	return 0;
}

int ShopLoader::InitMysteryShop(string& file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		ShopEntry info;
		info.m_nType = eMysteryShop;
		info.m_nId = loader.GetInt(i, count++);
		info.m_nCurrencyType = loader.GetInt(i, count++);
		info.m_nItemId = loader.GetInt(i, count++);
		info.m_nItemCount = loader.GetInt(i, count++);
		info.m_nPrice = loader.GetInt(i, count++);
		info.m_nProbalility = loader.GetInt(i, count++);

		m_nTotalProbalility += info.m_nProbalility;
		m_ShopEntry[info.m_nType][info.m_nItemId] = info;
	}

	return 0;
}

int ShopLoader::InitClanShop(string& file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		ShopEntry info;
		info.m_nType = eClanShop;
		info.m_nId = loader.GetInt(i, count++);
		info.m_nItemId = loader.GetInt(i, count++);
		info.m_nItemCount = loader.GetInt(i, count++);
		info.m_nPrice = loader.GetInt(i, count++);
		info.m_nDailyLimit = loader.GetInt(i, count++);

		m_ShopEntry[info.m_nType][info.m_nItemId] = info;
	}

	return 0;
}

int ShopLoader::InitCampShop(string& file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		ShopEntry info;
		info.m_nType = eCampShop;
		info.m_nId = loader.GetInt(i, count++);
		info.m_nCampLevel = loader.GetInt(i, count++);
		info.m_nItemId = loader.GetInt(i, count++);
		info.m_nItemCount = loader.GetInt(i, count++);
		info.m_nPrice = loader.GetInt(i, count++);
		info.m_nDailyLimit = loader.GetInt(i, count++);

		m_ShopEntry[info.m_nType][info.m_nItemId] = info;
	}

	return 0;
}

int ShopLoader::InitCreditShop(string& file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		ShopEntry info;
		info.m_nType = eCreditShop;
		info.m_nId = loader.GetInt(i, count++);
		info.m_nItemId = loader.GetInt(i, count++);
		info.m_nItemCount = loader.GetInt(i, count++);
		info.m_nPrice = loader.GetInt(i, count++);
		info.m_nDailyLimit = loader.GetInt(i, count++);

		m_ShopEntry[info.m_nType][info.m_nItemId] = info;
	}

	return 0;
}

int ShopLoader::InitHonorShop(string& file)
{
	CSVLoader loader;
	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		ShopEntry info;
		info.m_nType = eHonorShop;
		info.m_nId = loader.GetInt(i, count++);
		info.m_nItemId = loader.GetInt(i, count++);
		info.m_nItemCount = loader.GetInt(i, count++);
		info.m_nPrice = loader.GetInt(i, count++);
		info.m_nDailyLimit = loader.GetInt(i, count++);

		m_ShopEntry[info.m_nType][info.m_nItemId] = info;
	}

	return 0;
}

void ShopLoader::GetFlushMyster(vector<ShopEntry>& sEntry)
{
	int value = 0;
	if(m_nTotalProbalility == 0)
		return;

	for(int i = 0; i < 8; i++)
	{
		value  = rand()%((int)m_nTotalProbalility) + 1;
		printf("rand() ........................ %d \n",value);
		map<DWORD,map<DWORD,ShopEntry> >::iterator itr = m_ShopEntry.find(eMysteryShop);
		if(itr != m_ShopEntry.end())
		{
			int totalProbalility = 0;
			map<DWORD,ShopEntry>::iterator it = itr->second.begin();
			for(; it != itr->second.end(); ++it)
			{
				totalProbalility +=  it->second.m_nProbalility;
				if(value <= totalProbalility)
				{
					sEntry.push_back(it->second);
					printf("map<DWORD,ShopEntry>& sEntry ........id = %d .......in\n",it->first);
					break;
				}
			}
		}
	}

	printf("map<DWORD,ShopEntry>& sEntry ......................%d \n",sEntry.size());
}

const ShopEntry* ShopLoader::GetShopEntry(DWORD nType,DWORD nId)
{
	map<DWORD,map<DWORD,ShopEntry> >::iterator itr = m_ShopEntry.find(nType);
	if(itr != m_ShopEntry.end())
	{
		map<DWORD,ShopEntry>::iterator it = itr->second.find(nId);
		if(it != itr->second.end())
		{
			return &(it->second);
		}
	}

	return NULL;
}
