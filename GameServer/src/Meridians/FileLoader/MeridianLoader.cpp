/*
 * MeridianLoader.cpp
 *
 *  Created on: 2014年6月5日
 *      Author: root
 */

#include "MeridianLoader.h"
#include "Lua/LuaInterface.h"

MeridianLoader * MeridianLoader::m_instance = 0;

MeridianLoader::MeridianLoader()
{

}

MeridianLoader::~MeridianLoader()
{

}

int MeridianLoader::Init(string &path)
{
	string fileParam = path + "ChannelTypeData.csv";
	string luaFile = path + "meridians.lua";

	if(InitMeridianLoader(fileParam) || InitMeridianBase(luaFile))
	{
		return -1;
	}

	return 0;
}

int MeridianLoader::InitMeridianLoader(string & file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		MeridianStruct meridianStruct;
		int count = 0;

		int id = loader.GetInt(i, count++);
		count++;
		count++;
		count++;
		meridianStruct.i_exp = loader.GetInt(i, count++);
		meridianStruct.i_senexp = loader.GetInt(i, count++);
		meridianStruct.i_uping = loader.GetInt(i, count++);
		meridianStruct.i_upmoney = loader.GetInt(i, count++);
		meridianStruct.i_upgolden = loader.GetInt(i, count++);
		meridianStruct.i_pty = loader.GetInt(i, count++);
		count++;

		meridianStruct.m_AttList[eMuscleMan][eCharHitRate] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eMuscleMan][eCharAvoidRate] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eMuscleMan][eCrackDownRate] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eMuscleMan][eAvoidCrackRate] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eMuscleMan][eCharPhyAttackMax] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eMuscleMan][eCharHPMax] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eMuscleMan][eCharPhyDefence] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eMuscleMan][eCharMPMax] = loader.GetInt(i, count++);

		meridianStruct.i_type = loader.GetInt(i, count++);
		meridianStruct.m_LevelLimit = loader.GetInt(i, count++);

		meridianStruct.m_AttList[eMagicHuman][eCharHitRate] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eMagicHuman][eCharAvoidRate] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eMagicHuman][eCrackDownRate] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eMagicHuman][eAvoidCrackRate] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eMagicHuman][eCharPhyAttackMax] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eMagicHuman][eCharHPMax] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eMagicHuman][eCharPhyDefence] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eMagicHuman][eCharMPMax] = loader.GetInt(i, count++);

		meridianStruct.m_AttList[eShooter][eCharHitRate] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eShooter][eCharAvoidRate] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eShooter][eCrackDownRate] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eShooter][eAvoidCrackRate] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eShooter][eCharPhyAttackMax] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eShooter][eCharHPMax] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eShooter][eCharPhyDefence] = loader.GetInt(i, count++);
		meridianStruct.m_AttList[eShooter][eCharMPMax] = loader.GetInt(i, count++);

		m_meridianStruct[id] = meridianStruct;
	}

	return 0;
}

int MeridianLoader::InitMeridianBase(string& file)
{
	CLuaInterface lua;
	lua.OpenLua();

	if (0 != lua.LoadFile(file) || lua.Call() != 0)
	{
		return -1;
	}

	lua.SetLuaFunction("GetMeridiansBaseParam");
	if ( 0 != lua.Call(0, 2))
	{
		return -1;
	}

	m_baseInfo.m_throughAcpDisc = lua.GetInt();
	m_baseInfo.m_throughAcpCnt = lua.GetInt();

	lua.CloseLua();

	return 0;
}

int MeridianLoader::GetThroughAcpCnt()
{
	return m_baseInfo.m_throughAcpCnt;
}

// 获取特殊打通穴位折扣
int MeridianLoader::GetThroughAcpDisc()
{
	return m_baseInfo.m_throughAcpDisc;
}

