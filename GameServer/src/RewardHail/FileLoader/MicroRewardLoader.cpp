/*
 * MicroRewardLoader.cpp
 *
 *  Created on: 2015年9月17日
 *      Author: root
 */

#include "MicroRewardLoader.h"
#include "StrArgPkg.h"
#include "CSVLoader.h"
#include "Lua/LuaInterface.h"

MicroRewardLoader::MicroRewardLoader() {
	// TODO Auto-generated constructor stub

}

MicroRewardLoader::~MicroRewardLoader() {
	// TODO Auto-generated destructor stub
}


int MicroRewardLoader::Init(string& path)
{
	string file = path + "./tinyPort.csv";
	if (InitFile(file))
	{
		return -1;
	}

	file = path + "./MicroAwardParam.lua";
	if (InitLua(file))
	{
		return -1;
	}
	return 0;
}

int MicroRewardLoader::InitFile(string& file)
{
	CSVLoader loader;
	if (!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	uint index = 0;
	MicroRewardInfo info;
	for (index = 0; index < loader.GetRowCount(); ++index)
	{
		int count = 0;

		count++;

		StrArgPkg pkgItem("|", 	loader.GetString(index, count++));
		for (uint j = 0; j < pkgItem.size(); ++j)
		{
			StrArgPkg limitPkg(":", getArg(pkgItem, j));
			info.itemID.push_back(DWORD(atoi(getArg(limitPkg, 0).c_str())));
			info.itemNum.push_back(DWORD(atoi(getArg(limitPkg, 1).c_str())));
		}

		info.isBind = loader.GetBool(index, count++);
	}

	m_rewardInfo = info;

	return 0;
}

int MicroRewardLoader::InitLua(string& file)
{
	CLuaInterface lua;

	lua.OpenLua();

	if (lua.LoadFile(file) != 0 || lua.Call() != 0)
	{
		return -1;
	}

	lua.SetLuaFunction("GetMicroLoginAwardParam");

	if (lua.Call(0, 1))
	{
		return -1;
	}

	int expRatio = lua.GetInt();
	m_rewardInfo.addExpRatio = double(double(expRatio) / 100);

	lua.CloseLua();

	return 0;
}

void MicroRewardLoader::GetMicroReward(MicroRewardInfo& rewardInfo)
{
	rewardInfo = m_rewardInfo;
}
