/*
 * MagicLoader.cpp
 *
 *  Created on: 2014年8月8日
 *      Author: helh
 */

#include "HeartMagicLoader.h"

#include "Log/Logger.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "Probability/Probability.h"
#include "Lua/LuaInterface.h"
#include "../Skill/SkillLoader/SkillLoader.h"

/**************随机获取被动技能*************/
int HeartMagicWeaponInfo::RandSkill(int quality,bool isByQuality)
{
	if (isByQuality)
		return randSkillByQuality(quality);
	else
		return randSkillNotByQuality();
}

int HeartMagicWeaponInfo::randSkillByQuality(int quality)
{
	vector<HeartPassivePorbability> tempSkillList;
	for (size_t i = 0;i < m_PassaveProbabilityList.size();++i)
	{
		SkillInfo * skillInfo = SkillLoader::GetInstance()->GetSkillInfoByID(m_PassaveProbabilityList[i].m_PassiveSkillID);
		if (skillInfo == NULL)
			continue;

		if (skillInfo->quality != quality)
			continue;

		tempSkillList.push_back(m_PassaveProbabilityList[i]);
	}

	ProbabilityVector<HeartPassivePorbability> tempHandle(tempSkillList);
	short tempIndex = tempHandle.calculation();
	if (tempIndex == INVALID_INDEX)
		return 0;

	return tempSkillList[tempIndex].m_PassiveSkillID;
}

int HeartMagicWeaponInfo::randSkillNotByQuality()
{
	ProbabilityVector<HeartPassivePorbability> tempHandle(m_PassaveProbabilityList);
	short tempIndex = tempHandle.calculation();
	if (tempIndex == INVALID_INDEX)
		return 0;

	return m_PassaveProbabilityList[tempIndex].m_PassiveSkillID;
}

/**************配表*************/
HeartMagicLoader::HeartMagicLoader()
{
}

HeartMagicLoader::~HeartMagicLoader()
{
}

int HeartMagicLoader::Init(string &file)
{
	string tempMagicStr 	  = file + "HeartTypeData.csv";
	string tempProbabilityStr = file + "HeartSkill.csv";
	//string tempLuaStr 		  = file + "/MagicConfig.lua";

	map<DWORD,vector<HeartPassivePorbability> > tempDataList;
	map<DWORD,vector<HeartPassivePorbability> >::iterator tempIt;

	loadPassive(tempProbabilityStr,tempDataList);
	//loaLua(tempLuaStr);

	CSVLoader loader;
	if (!loader.OpenFromFile(tempMagicStr.c_str()))
	{
		return -1;
	}

	for (unsigned int i = 0;i < loader.GetRowCount(); ++i)
	{
		HeartMagicWeaponInfo info;
		int count = 0;

		info.id = loader.GetInt(i, count++, 0);
		if(info.id  == 0)
			continue;
		count++;
		count++;
		count++;
		count++;
		count++;
		count++;
		count++;
		count++;
		count++;
		info.advance = loader.GetInt(i, count++, 0);
		info.lv = loader.GetInt(i, count++, 0);
		info.exp = loader.GetInt(i, count++, 0);

		tempIt = tempDataList.find(info.id);
		if (tempIt != tempDataList.end())
		{
			info.m_PassaveProbabilityList = tempIt->second;
		}

		for(int j = 0;j < eProfessionMax - 1; ++j)
		{
			info.attr[j].hp = loader.GetInt(i, count++, 0);
			info.attr[j].attack = loader.GetInt(i, count++, 0);
			info.attr[j].pdefence = loader.GetInt(i, count++, 0);
			info.attr[j].hitrate = loader.GetInt(i, count++, 0);
			info.attr[j].avoidrate = loader.GetInt(i, count++, 0);
			info.attr[j].crackrate = loader.GetInt(i, count++, 0);
			info.attr[j].avoidcrack = loader.GetInt(i, count++, 0);
		}

		info.skillid[0] = loader.GetInt(i, count++, 0);
		info.skillid[1] = loader.GetInt(i, count++, 0);
		info.skillid[2] = loader.GetInt(i, count++, 0);

		info.upNeedNum 	 = loader.GetInt(i, count++, 0);
		info.m_NeedMoney = loader.GetInt(i, count++, 0);
		info.m_LevelLimit = loader.GetInt(i, count++, 0);
		info.m_ItemID = loader.GetInt(i, count++, 0);
		info.m_MinExp = loader.GetInt(i, count++, 0);
		info.m_MaxExp = loader.GetInt(i, count++, 0);

		m_magicInfo.push_back(info);
	}

	return 0;
}

void HeartMagicLoader::loadPassive(string& file,map<DWORD,vector<HeartPassivePorbability> >& dataList)
{
	CSVLoader loader;

	if (!loader.OpenFromFile(file.c_str()))
		return ;

	int count = 0;
	for (unsigned int i = 0;i < loader.GetRowCount(); ++i)
	{
		count = 0;
		HeartPassivePorbability tempInfo;

		DWORD tempID = loader.GetInt(i,count++,0);
		tempInfo.m_PassiveSkillID = loader.GetInt(i,count++,0);
		tempInfo.m_Probability    = loader.GetInt(i,count++,0);

		dataList[tempID].push_back(tempInfo);
	}

	return;
}

void HeartMagicLoader::loaLua(string& file)
{
	CLuaInterface tempLua;
	tempLua.OpenLua();

	if (tempLua.LoadFile(file) != 0 || tempLua.Call() != 0)
		return ;

	tempLua.SetLuaFunction("GetCountdownTime");
	if (tempLua.Call(0,1) != 0)
		return;

	m_ConfigData.m_Time  = tempLua.GetInt();

	//配表中的时间为秒，程序中使用毫秒
	m_ConfigData.m_Time *= 1000;

	tempLua.CloseLua();

	return;
}

bool HeartMagicLoader::isMagicSkill(int skillID)
{
	for(size_t i = 0;i < m_magicInfo.size();++i)
	{
		for(int j = 0;j < eProfessionMax-1;++j)
		{
			if(m_magicInfo[i].skillid[j] == skillID)
				return true;
		}
	}

	return false;
}

int HeartMagicLoader::getMaxID()
{
	if(m_magicInfo.size() == 0)
	{
		return 0;
	}

	return m_magicInfo.back().id;
}
















