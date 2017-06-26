/*
 * HeroLoader.cpp
 *
 *  Created on: 2016年5月18日
 *      Author: root
 */
#include "HeroLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include <util.h>

HeroLoader::HeroLoader()
{

}

HeroLoader::~HeroLoader()
{

}

bool HeroLoader::InitData()
{
	if(LoaderFile() != 0 || LoadLua() != 0)
	{
		return false;
	}
	return true;
}

const HeroLayerInfo* HeroLoader::GetHeroLayerInfo(int layer)
{
	std::map<int,HeroLayerInfo>::const_iterator it = m_HeroLayerData.find(layer);
	if(m_HeroLayerData.end() != it)
	{
		return &it->second;
	}
	return NULL;
}

int HeroLoader::LoaderFile()
{
	std::string strFileFullPath;
	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return -1;
	}

	string strFile = strFileFullPath + "Data/HeroTowerTypeData.csv";
	CSVLoader loader;

	if(!loader.OpenFromFile(strFile.c_str()))
	{
		return -1;
	}

	//层数 level	怪物ID	怪物名字	血量	攻击	防御	命中	闪避	暴击	抗暴	奖励道具	额外奖励	推荐战力
	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		HeroLayerInfo info;
		int count = 0;

		info.layer = loader.GetInt(i, count++);
		info.level = loader.GetInt(i, count++);
		info.monster = loader.GetInt(i, count++);

		count++;

		info.hp = loader.GetInt(i, count++);
		info.attack = loader.GetInt(i, count++);
		info.defese = loader.GetInt(i, count++);
		info.hit = loader.GetInt(i, count++);
		info.avoid = loader.GetInt(i, count++);
		info.crackDown = loader.GetInt(i, count++);
		info.avoidCrack = loader.GetInt(i, count++);

		string strPrize = loader.GetString(i, count++, "");
		StrArgPkg strPrizePkg("|", strPrize);
		strPrizePkg.setArgs(strPrize);

		for(uint itemSize=0; itemSize<strPrizePkg.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(strPrizePkg, itemSize).c_str());
			if(limitPkg.size() != 2)
					return -1;

			int id = atoi(getArg(limitPkg, 0).c_str());
			int count = atoi(getArg(limitPkg, 1).c_str());

			info.prizes.push_back(make_pair(id,count));
		}

		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		funPkgItem.setArgs(funStrItem);

		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			if(limitPkg.size() != 2)
				return -1;

			int id = atoi(getArg(limitPkg, 0).c_str());
			int num = atoi(getArg(limitPkg, 1).c_str());

			info.exPrizes.push_back(make_pair(id,num));
		}

		m_HeroLayerData[info.layer] = info;
	}

	return 0;
}

int HeroLoader::LoadLua()
{
	std::string strFileFullPath;
	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return -1;
	}

	m_pLua.OpenLua();

	string strConfig = strFileFullPath + "Data/HeroTower.lua";
	if(m_pLua.LoadFile(strConfig) != 0 || m_pLua.Call() != 0)
	{
		return -1;
	}

	m_pLua.SetLuaFunction("GetCountTime");
	if(m_pLua.Call(0, 1))
	{
		return -1;
	}
	m_config.countTime = m_pLua.GetInt();

	m_pLua.SetLuaFunction("GetPos");
	if(m_pLua.Call(0, 2))
	{
		return -1;
	}
	m_config.posY = m_pLua.GetInt();
	m_config.posX = m_pLua.GetInt();

	m_pLua.CloseLua();
	return 0;
}



