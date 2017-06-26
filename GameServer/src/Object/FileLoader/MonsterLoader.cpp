/*
 * MonsterLoader.cpp
 *
 *  Created on: 2013��10��22��
 *      Author: helh
 */

#include "MonsterLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"

MonsterLoader * MonsterLoader::m_instance = 0;

MonsterLoader::MonsterLoader()
{

}

MonsterLoader::~MonsterLoader()
{
	m_allMonster.clear();
}

int MonsterLoader::InitMonster(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		MonsterInfo info;
		int count = 0;

		info.id = loader.GetInt(i, count++, 0);
		count++;
		info.name = loader.GetString(i, count++);
		info.lv = loader.GetInt(i, count++, 0);
		info.quality = loader.GetInt(i, count++, 0);
		info.mapId = loader.GetInt(i, count++, 0);
		info.born_x = loader.GetInt(i, count++, 0);
		info.born_y = loader.GetInt(i, count++, 0);
		info.flushTime = loader.GetInt(i, count++, 0);
		info.aliveTime = loader.GetInt(i, count++, 0) / 1000;
		info.camp = loader.GetInt(i, count++, 0);
		info.idleSpeed = loader.GetInt(i, count++);
		info.moveSpeed = loader.GetInt(i, count++);
		info.baseAI = loader.GetInt(i, count++);
		info.betterAI = loader.GetInt(i, count++);

		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		funPkgItem.setArgs(funStrItem);

		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			info.fallID.push_back(atoi(getArg(funPkgItem, itemSize).c_str()));
		}

		info.exp = loader.GetInt(i, count++);
		info.score = loader.GetInt(i, count++);
		info.maxHP = loader.GetInt(i, count++);
		info.maxMP = loader.GetInt(i, count++);
		info.minAttack = loader.GetInt(i, count++);
		info.maxAttack = loader.GetInt(i, count++);
		info.minMagAttack = loader.GetInt(i, count++);
		info.maxMagAttack = loader.GetInt(i, count++);
		info.phyDefence = loader.GetInt(i, count++);
		info.magDefence = loader.GetInt(i, count++);
		info.hitRate = loader.GetInt(i, count++);
		info.avoidRate = loader.GetInt(i, count++);
		info.lucky = loader.GetInt(i, count++);
		info.crackRate = loader.GetInt(i, count++);
		info.avoidCrack = loader.GetInt(i, count++);
		info.gatherTime = loader.GetInt(i, count++);
		info.gatherTaskId = loader.GetInt(i, count++);

		m_allMonster[info.id] = info;
	}

	return 0;
}



