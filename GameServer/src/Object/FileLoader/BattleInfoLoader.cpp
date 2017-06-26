/*
 * BattleInfoLoader.cpp
 *
 *  Created on: 2013��10��26��
 *      Author: helh
 */

#include "BattleInfoLoader.h"
#include "CSVLoader.h"
#include "Log/Logger.h"
#include "../../Skill/SkillLoader/SkillLoader.h"

BattleInfoLoader * BattleInfoLoader::m_instance = 0;

BattleInfoLoader::BattleInfoLoader()
{
	//读取配置文件
	m_lua.OpenLua();
}

BattleInfoLoader::~BattleInfoLoader()
{
	m_lua.CloseLua();
}

int BattleInfoLoader::Init(string &path)
{
	string fileBattle = path + "BaseBattleInfo.csv";
	string fileParam = path + "BattleParam.lua";

	if(InitBattleInfo(fileBattle) || InitBattleParam(fileParam))
	{
		return -1;
	}

	return 0;
}

int BattleInfoLoader::GetAllFight(int phyattack, int phydefence, int hp,int hit, int avoid, int crack, int avoidcrack)
{
	m_lua.SetLuaFunction("GetAllFight");
	m_lua.SetInt(phyattack);
	m_lua.SetInt(phydefence);
	m_lua.SetInt(hp);
	m_lua.SetInt(hit);
	m_lua.SetInt(avoid);
	m_lua.SetInt(crack);
	m_lua.SetInt(avoidcrack);

	if(m_lua.Call(7, 1))
	{
		return 0;
	}

	return m_lua.GetInt();
}

int BattleInfoLoader::InitBattleParam(string & file)
{
	if(m_lua.LoadFile(file) != 0 || m_lua.Call() != 0)
	{
		return -1;
	}

	m_lua.SetLuaFunction("getEffectFormula");
	if(m_lua.Call(0,1) != 0)
	{
		return -1;
	}

	char tChar[64] = {0};
	int tCounts = m_lua.GetTableFieldCount();

	for(int i = 1;i <= tCounts;++i)
	{
		m_lua.GetFieldString(tChar,64,i);
		m_Formula[i] = tChar;
	}

	return 0;
}

int BattleInfoLoader::InitBattleInfo(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		LvBattleAtt info;

		info.lv = loader.GetInt(i, count++);

		for(unsigned int j=0; j<eProfessionMax-1; ++j)
		{
			info.physicalpower = loader.GetInt(i, count++);
			info.cleverAir = loader.GetInt(i, count++);
			info.cleverPower = loader.GetInt(i, count++);
			info.strength = loader.GetInt(i, count++);
			info.patience = loader.GetInt(i, count++);
			info.agility = loader.GetInt(i, count++);
			info.hitRate = loader.GetInt(i, count++);
			info.avoidAttack = loader.GetInt(i, count++);
			info.lucky = loader.GetInt(i, count++);
			info.crackDown = loader.GetInt(i, count++);
			info.avoidCrack = loader.GetInt(i, count++);

			m_battleInfo[j][info.lv] = info;
		}
	}

	return 0;
}

void BattleInfoLoader::GetBattleByLvAndPro(int lv, int profession, LvBattleAtt &attr)
{
	if(profession <= 0 || profession >= eProfessionMax)
	{
		return;
	}

	map<WORD, LvBattleAtt>::iterator it = m_battleInfo[profession - 1].find(lv);
	if(it != m_battleInfo[profession - 1].end())
	{
		attr = it->second;
	}
}

float BattleInfoLoader::GetHitRate(int ahitrate,int bavoid)
{
	m_lua.SetLuaFunction("GetHitRate");
	m_lua.SetInt(ahitrate);
	m_lua.SetInt(bavoid);
	if(m_lua.Call(2, 1))
	{
		LOG_ERROR(FILEINFO, "call GetHitRate error");

		return 0;
	}

	return m_lua.GetFloat();
}

float BattleInfoLoader::GetCrackRate(int crack,int avoidcrack)
{
	m_lua.SetLuaFunction("GetCrackRate");
	m_lua.SetInt(crack);
	m_lua.SetInt(avoidcrack);

	if(m_lua.Call(2, 1))
	{
		LOG_ERROR(FILEINFO, "call GetCrackRate error");

		return 0;
	}

	return  m_lua.GetFloat();
}

int BattleInfoLoader::GetPhysicalDamage(int attack, int defense, int realDamage, int attackLevel,int defenseLevel,int ratio,int fightA,int fightB)
{
	m_lua.SetLuaFunction("GetPhysicalDamage");
	m_lua.SetInt(attack);
	m_lua.SetInt(defense);
	m_lua.SetInt(realDamage);
	m_lua.SetInt(attackLevel);
	m_lua.SetInt(defenseLevel);
	m_lua.SetInt(ratio);
	m_lua.SetInt(fightA);
	m_lua.SetInt(fightB);
	if(m_lua.Call(8, 1))
	{
		LOG_ERROR(FILEINFO, "call GetPhysicalDamage error");

		return 0;
	}

	return m_lua.GetInt();
}

int BattleInfoLoader::GetSecondAttrByFirstAttr(int type, int value)
{
	int ret = 0;

	switch(type)
	{
	case eStrengthToAt:
		{
			m_lua.SetLuaFunction("GetPhyAttackByStrength");
			m_lua.SetInt(value);
			if(m_lua.Call(1, 1))
			{
				LOG_ERROR(FILEINFO, "call GetPhyAttackByStrength error");

				return 0;
			}

			ret = m_lua.GetInt();

			break;
		}
	case eCleverPowerToMA:
		{
			m_lua.SetLuaFunction("GetMagAttackByCleverPower");
			m_lua.SetInt(value);
			if(m_lua.Call(1, 1))
			{
				LOG_ERROR(FILEINFO, "call GetMagAttackByCleverPower error");

				return 0;
			}

			ret = m_lua.GetInt();

			break;
		}
	case eCleverAirtoMP:
		{
			m_lua.SetLuaFunction("GetMPByCleverAir");
			m_lua.SetInt(value);
			if(m_lua.Call(1, 1))
			{
				LOG_ERROR(FILEINFO, "call GetMPByCleverAir error");

				return 0;
			}

			ret = m_lua.GetInt();

			break;
		}
	case ePhyPowerToHP:
		{
			m_lua.SetLuaFunction("GetHPByPhyPower");
			m_lua.SetInt(value);
			if(m_lua.Call(1, 1))
			{
				LOG_ERROR(FILEINFO, "call GetHPByPhyPower error");

				return 0;
			}

			ret = m_lua.GetInt();

			break;
		}
	case ePatienceToPD:
		{
			m_lua.SetLuaFunction("GetPhyDefenceByPatience");
			m_lua.SetInt(value);
			if(m_lua.Call(1, 1))
			{
				LOG_ERROR(FILEINFO, "call GetPhyDefenceByPatience error");

				return 0;
			}

			ret = m_lua.GetInt();

			break;
		}
	case eAgilityToMD:
		{
			m_lua.SetLuaFunction("GetMagDefenceByAgility");
			m_lua.SetInt(value);
			if(m_lua.Call(1, 1))
			{
				LOG_ERROR(FILEINFO, "call GetMagDefenceByAgility error");

				return 0;
			}

			ret = m_lua.GetInt();

			break;
		}
	}

	return ret;
}

//获得初始移动速度
int BattleInfoLoader::getBaseMoveSpeed()
{
	m_lua.SetLuaFunction("GetBaseMoveSpeed");
	//移动速度默认值为250
	if(m_lua.Call(0,1))
		return 250;

	int tempResult = m_lua.GetInt();

	return tempResult;
}

float BattleInfoLoader::getValueByFormula(const BuffParamBase& paramBase,EffectParam& param)
{
	vector<float> tParams;
	paramBase.getParams(tParams,param);

	map<int,string>::iterator tIt = m_Formula.find(paramBase.getFormulaID());
	if(tIt == m_Formula.end())
	{
		if(tParams.size() > 0)
			return tParams[0];

		return .0f;
	}

	m_lua.SetLuaFunction(tIt->second.c_str());

	for(size_t i = 0;i < tParams.size();++i)
	{
		m_lua.SetFloat(tParams[i]);		
	}

	if(m_lua.Call(tParams.size(),1))
	{
		return .0f;
	}

	return m_lua.GetFloat();
}








