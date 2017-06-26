/*
 * SkillLoader.cpp
 *
 *  Created on: 2014年1月15日
 *      Author: helh
 */

#include "SkillLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "Lua/LuaInterface.h"
#include "../Effect/Effect.h"


void BuffDamage::getParams(vector<float>& params,EffectParam& param) const
{	
	for(size_t i = 0;i < m_DataList.size();++i)
	{
		params.push_back(Effect::getEffectValue(m_DataList[i],param));	
	}

	return ;
}

SkillLoader * SkillLoader::m_instance = 0;

SkillLoader::SkillLoader()
{

}

SkillLoader::~SkillLoader()
{

}

int SkillLoader::Init(string &file)
{
	string skillPath = file + "Data/SkillTypeData.csv";
	string buffPath = file + "Data/BuffTypeData.csv";

	if(InitSkill(skillPath) || InitBuff(buffPath) || !loadLua(file))
	{
		return -1;
	}

	string skillSPath = file + "Data/SkillSTypeData.csv";
	if (InitSkillS(skillSPath))
	{
		return -1;
	}

	return 0;
}

int SkillLoader::InitSkill(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		SkillInfo info;

		info.skillID = loader.GetInt(i, count++);
		count++;
		count++;
		count++;
		count++;
		count++;
		count++;
		info.limitPro = loader.GetInt(i, count++);
		info.m_TalentID = loader.GetInt(i, count++);
		info.minLv = loader.GetInt(i, count++);
		info.learnLv = loader.GetInt(i, count++);
		info.useMoney = loader.GetInt(i, count++);
		info.useExp = loader.GetInt(i, count++);
		info.useForce = loader.GetInt(i, count++);
		info.useFactionCon = loader.GetInt(i, count++);
		info.nowLv = loader.GetInt(i, count++);
		info.nextSkillID = loader.GetInt(i, count++);
		info.nextActionSkill = loader.GetInt(i, count++);
		info.useHP = loader.GetInt(i, count++);
		info.useMP = loader.GetInt(i, count++);
		info.fType = loader.GetInt(i, count++);
		info.sType = loader.GetInt(i, count++);
		info.targetType = loader.GetInt(i, count++);
		count++;
		info.intervalTime = loader.GetInt(i, count++);
		info.aliveTime = loader.GetInt(i, count++);
		info.flySpeed = loader.GetInt(i, count++);
		info.continueTime = loader.GetInt(i, count++);
		info.channelType = loader.GetInt(i, count++);
		info.coolTime = loader.GetInt(i, count++);

		info.distance = loader.GetInt(i, count++);
		info.rangeType = loader.GetInt(i, count++);
		info.rangeLength = loader.GetInt(i, count++);
		info.rangeWidth = loader.GetInt(i, count++);
		info.targetCount = loader.GetInt(i, count++);

		for(unsigned int j=0; j<3; ++j)
		{
			info.effectType[j] = loader.GetInt(i, count++);
			info.effectFirstType[j] = loader.GetInt(i, count++);
			info.atkRate[j] = loader.GetInt(i, count++);
			for(unsigned int k=0; k<3; ++k)
			{
				info.param[j][k] = loader.GetInt(i, count++);
			}

			info.m_EffectTargetType[j] = static_cast<SkillTargetType>(loader.GetInt(i, count++));
		}

		info.quality = loader.GetInt(i, count++);
		info.m_Fight = loader.GetInt(i, count++);
		count++;

		string tempStr = loader.GetString(i, count++);
		StrArgPkg tempCondition("|",tempStr);
		for(size_t j = 0;j < tempCondition.size();++j)
		{
			int tempData = atoi(getArg(tempCondition,j).c_str());
			info.m_StatusCondition.push_back(static_cast<EAttackState>(tempData));
		}

		count++;

		info.m_ActionTime = loader.GetInt(i, count++);

		info.m_IntonateTime = loader.GetInt(i, count++);

		//状态
		StrArgPkg tempBreakArg("|",loader.GetString(i, count++));
		if(tempBreakArg.size() > 0)
		{
			StrArgPkg tempStateBreak(";",tempBreakArg[0].getStr());
			for(size_t j = 0;j < tempStateBreak.size();++j)
			{
				int tempData = atoi(getArg(tempStateBreak,j).c_str());
				info.m_BreakStateList.push_back(static_cast<EAttackState>(tempData));
			}
		}

		//行为
		if(tempBreakArg.size() > 1)
		{
			StrArgPkg tempDoingBreak(";",tempBreakArg[1].getStr());
			for(size_t j = 0;j < tempDoingBreak.size();++j)
			{
				int tempData = atoi(getArg(tempDoingBreak,j).c_str());
				info.m_BreakDoingList.push_back(static_cast<eObjectDoing>(tempData));
			}
		}

		setCDChannel((ECharProf)info.limitPro,(eChannelType)info.channelType);

		info.skillBook = loader.GetInt(i, count++);
		info.upgradeNum = loader.GetInt(i, count++);

		m_skillInfo[info.skillID] = info;
	}

	return 0;
}

int SkillLoader::InitBuff(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i < loader.GetRowCount(); ++i)
	{
		int count = 0;
		BuffInfo info;

		info.id = loader.GetInt(i, count++);
		info.groupID = loader.GetInt(i, count++);
		info.lv = loader.GetInt(i, count++);
		count++;
		count++;
		count++;
		info.isResult = loader.GetInt(i, count++);
		info.isDamage = loader.GetInt(i, count++);
		info.aliveTime = loader.GetInt(i, count++);
		info.timer = loader.GetInt(i, count++);
		info.fireRate = loader.GetInt(i, count++);
		info.useType = loader.GetInt(i, count++);
	   	info.m_pParam = initEffectParam(loader.GetString(i, count++),(ERelsultType)info.useType);
		info.isDel = loader.GetInt(i, count++);
		info.saveType = loader.GetInt(i, count++);
		info.params = loader.GetFloat(i, count++);
		info.isAdd = loader.GetInt(i, count++);
		info.m_Condition  = loader.GetInt(i, count++);
		info.m_TargetType = static_cast<SkillTargetType>(loader.GetInt(i, count++));

		m_buffInfo[info.id] = info;
	}

	return 0;
}

BuffParamBase* SkillLoader::initEffectParam(const string& str,ERelsultType type)
{	
	BuffParamBase* tPtr = NULL;
	switch(type)
	{
	case eChangeHP:
		{
			BuffDamage* tDamage = new BuffDamage();
			tPtr = tDamage;
			StrArgPkg tArg(">",str);
			EffectOneParam tOneParam;
			if(tArg.size() == 1)
			{
				tOneParam.m_ParamList.push_back(atof(tArg.getStr().c_str()));
				tDamage->m_DataList.push_back(tOneParam);
				tDamage->m_Param = atof(tArg.getStr().c_str());
				break;
			}
			
			tDamage->m_FormularID = atoi(tArg[0].getStr().c_str());

			StrArgPkg tParamsArg("|&",tArg[1].getStr());

			for(size_t i = 0;i < tParamsArg.size();++i)
			{
				if(tParamsArg[i].size() == 1)
				{
					tOneParam.m_Type = eEffectConst; 
					tOneParam.m_ParamList.clear();

					tOneParam.m_ParamList.push_back(atof(tParamsArg[i].getStr().c_str()));
				}
				else
				{
					tOneParam.m_Type = static_cast<eEffectDataSource>(atoi(tParamsArg[i][0].getStr().c_str()));
					tOneParam.m_ParamList.clear();

					StrArgPkg tParamArg(":",tParamsArg[i][1].getStr());
					for(size_t j = 0;j < tParamArg.size();++j)
					{
						tOneParam.m_ParamList.push_back(atof(tParamArg[j].getStr().c_str()));
					}
				}

				tDamage->m_DataList.push_back(tOneParam);
			}

			break;
		}
	default:
		{
			tPtr = new BuffParamBase();	
			tPtr->m_Param = atoi(str.c_str());
			break;
		}
	}

	tPtr->m_Type = type;

	return tPtr;
}

bool SkillLoader::loadLua(const string& str)
{
	CLuaInterface tempLua;
	tempLua.OpenLua();
	string tempDataStr = str + "Data/BuffCondition.lua";
	if(tempLua.LoadFile(tempDataStr) != 0 || tempLua.Call() != 0)
		return false;

	tempLua.SetLuaFunction("getConditionTable");
	if(tempLua.Call(0, 1) != 0)
		return false;

	int tempCounts = tempLua.GetTableFieldCount();

	for(int i = 1;i <= tempCounts;++i)
	{
		DWORD tempData = tempLua.GetFieldInt(i);
		m_ConditionList.push_back(tempData);
	}

	tempLua.CloseTable();

	tempLua.SetLuaFunction("getSkillCommonCD");
	if(tempLua.Call(0, 1) != 0)
		return false;

	m_SingleData.m_SkillCommonCD = tempLua.GetInt();

	tempLua.SetLuaFunction("getInitialSkill");
	if(tempLua.Call(0, 1) != 0)
		return false;

	tempCounts = tempLua.GetTableFieldCount();
	int tTjob = eMuscleMan;
	for(int i = 1;i <= tempCounts;++i)
	{
		tempLua.GetFieldTable(i);
		int tInCounts = tempLua.GetTableFieldCount();
		for(int j = 1;j <= tInCounts;++j)
		{
			m_BornSkill[(ECharProf)tTjob].push_back(tempLua.GetFieldInt(j));
		}

		++tTjob;

		tempLua.CloseTable();
	}

	tempLua.CloseTable();

	tempLua.CloseLua();

	return true;
}

int SkillLoader::InitSkillS(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i < loader.GetRowCount(); ++i)
	{
		int count = 0;
		SkillInterfaceShow info;
		info.prof = (ECharProf)loader.GetInt(i, count++);

		StrArgPkg shear("|", loader.GetString(i, count++));
		for (uint j = 0; j < shear.size(); ++j)
		{
			info.skillIDByType[eSkillInterface_ActiveSkill].push_back(atoi(getArg(shear, j).c_str()));
		}

		shear.setArgs(loader.GetString(i, count++));
		for (uint j = 0; j < shear.size(); ++j)
		{
			info.skillIDByType[eSkillInterface_PassiveSkill].push_back(atoi(getArg(shear, j).c_str()));
		}

		m_skillInterface[info.prof] = info;
	}

	return 0;

}

vector<eChannelType>* SkillLoader::getCDChannelByProfession(ECharProf profession)
{
	map<ECharProf,vector<eChannelType> >::iterator tempIt =  m_CDChannelList.find(profession);
	if(tempIt == m_CDChannelList.end())
	{
		return NULL;
	}

	return &tempIt->second;
}

void SkillLoader::setCDChannel(ECharProf proffesion,eChannelType type)
{
	map<ECharProf,vector<eChannelType> >::iterator tempIt = m_CDChannelList.find(proffesion);
	if(tempIt != m_CDChannelList.end())
	{
		for(size_t i = 0;i < tempIt->second.size();++i)
		{
			if(type != tempIt->second[i])
			{
				continue ;
			}

			return;
		}
	}

	m_CDChannelList[proffesion].push_back(type);

	return ;
}


 void SkillLoader::GetIDByProfAndType(ECharProf prof, ESkillInterfaceType type, vector<int>& skillID)
{
	map<ECharProf, SkillInterfaceShow>::iterator it = m_skillInterface.find(prof);
	if (it != m_skillInterface.end())
	{
		map<ESkillInterfaceType, vector<int> >::iterator itS = it->second.skillIDByType.find(type);
		if (itS != it->second.skillIDByType.end())
		{
			skillID = itS->second;
		}
	}
}





