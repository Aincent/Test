/*
 * AILoader.cpp
 *
 *  Created on: 2013��10��23��
 *      Author: helh
 */

#include "AILoader.h"
#include "Log/Logger.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"


AILoader * AILoader::m_instance = 0;

AILoader::AILoader()
{

}

AILoader::~AILoader()
{
	m_baseAI.clear();
	m_betterAI.clear();
	m_arenaAI.clear();
}

int AILoader::Init(string &file)
{
	string baseFile = file + "baseAI.csv";
	string betterFile = file + "betterAI.csv";
	string arenaFile = file + "arenaAI.csv";

	if(InitBaseAI(baseFile))
	{
		LOG_ERROR(FILEINFO, "baseai file load error");

		return -1;
	}

	if(InitBetterAI(betterFile))
	{
		LOG_ERROR(FILEINFO, "better ai file load error");

		return -1;
	}

	if(InitArena(arenaFile))
	{
		LOG_ERROR(FILEINFO, "arena ai file load error");

		return -1;
	}

	return 0;
}

int AILoader::InitBaseAI(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		BaseAIInfo info;
		int count = 0;

		info.id = loader.GetInt(i, count++, 0);
		info.atkDistance = loader.GetInt(i, count++, 0);
		info.viewDistance = loader.GetInt(i, count++, 0);
		info.moveDistance = loader.GetInt(i, count++, 0);
		info.runDistance = loader.GetInt(i, count++, 0);
		info.stayTime = loader.GetInt(i, count++, 0);
		info.atkType = loader.GetInt(i, count++);
		info.atkTime = loader.GetInt(i, count++);
		info.skillID = loader.GetInt(i, count++);

		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		funPkgItem.setArgs(funStrItem);

		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			info.state.push_back(atoi(getArg(funPkgItem, itemSize).c_str()));
		}

		if(info.state.size() < 4)
		{
			info.state.resize(4);
		}

		m_baseAI[info.id] = info;
	}

	return 0;
}

int AILoader::InitBetterAI(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		BetterAIInfo info;
		int count = 0;

		info.id = loader.GetInt(i, count++, 0);
		info.cycleTime = loader.GetInt(i, count++, 0);
		info.firstSelect = loader.GetInt(i, count++, 0);
		info.selectRate = loader.GetInt(i, count++, 0);
		info.firstType = loader.GetInt(i, count++, 0);
		info.firstParam = loader.GetInt(i, count++, 0);
		info.firstAction = loader.GetInt(i, count++);
		info.firstRate = loader.GetInt(i, count++);
		info.secondType = loader.GetInt(i, count++, 0);
		info.secondParam = loader.GetInt(i, count++, 0);
		info.secondAction = loader.GetInt(i, count++);
		info.secondRate = loader.GetInt(i, count++);
		info.callSkill = loader.GetInt(i, count++);
		info.callParam = loader.GetInt(i, count++);
		info.callRate = loader.GetInt(i, count++);
		info.callSkillID = loader.GetInt(i, count++);
		info.callDelay = loader.GetInt(i, count++);
		info.fSkillID = loader.GetInt(i, count++);
		info.fSkillRate = loader.GetInt(i, count++);
		info.sSkillID = loader.GetInt(i, count++);
		info.sSkillRate = loader.GetInt(i, count++);
		info.tSkillID = loader.GetInt(i, count++);
		info.tSkillRate = loader.GetInt(i, count++);

		m_betterAI[info.id] = info;
	}

	return 0;
}

int AILoader::InitArena(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		ArenaInfo info;
		int count = 0;

		info.id = loader.GetInt(i, count++, 0);
		info.nCycleTime = loader.GetInt(i, count++, 0);
		info.nSkill.push_back(loader.GetInt(i, count++));
		info.nSkill.push_back(loader.GetInt(i, count++));
		info.nSkill.push_back(loader.GetInt(i, count++));
		info.nSkill.push_back(loader.GetInt(i, count++));
		info.nSkill.push_back(loader.GetInt(i, count++));
		info.nSkill.push_back(loader.GetInt(i, count++));
		info.nSkill.push_back(loader.GetInt(i, count++));
		info.nSkill.push_back(loader.GetInt(i, count++));
		info.nSkill.push_back(loader.GetInt(i, count++));

		m_arenaAI[info.id] = info;
	}
	return 0;
}


