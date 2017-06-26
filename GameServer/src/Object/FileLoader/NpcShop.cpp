/*
 * NPCShop.cpp
 *
 *  Created on: 2014.5.8
 *      Author: cz
 */

#include "Npcshop.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "Log/Logger.h"
#include "MessageCommonRet.h"
#include <math.h>
#include "../../QuestNew/QuestDefine.h"

NpcShop * NpcShop::m_instance = 0;


NpcShop::NpcShop()
{

}

NpcShop::~NpcShop()
{

}

int NpcShop::Init(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		sNpcShop info;
		int count = 0;

		info.npcID = loader.GetInt(i, count++, 0);
//		if(info.npcID == 0)
//		{
//			continue;
//		}

		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);

		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			info.ShopItems.push_back(atoi(getArg(funPkgItem, itemSize).c_str()));
		}

		m_npcShopInfo[info.npcID] = info;
	}

	return 0;
}

bool NpcShop::IsExistedNpc(int id)
{
	map<int, sNpcShop>::iterator it = m_npcShopInfo.find(id);
	if(it == m_npcShopInfo.end())
		return false;
	else
		return true;
}

sNpcShop *NpcShop::GetNpcShopInfo(int id)
{
	map<int, sNpcShop>::iterator it = m_npcShopInfo.find(id);
	if(it == m_npcShopInfo.end())
	{
		LOG_ERROR(FILEINFO, "npcShop is not existed");

		return NULL;
	}

	return &it->second;
}
