/*
 * FashionLoader.cpp
 *
 *  Created on: 2015年3月4日
 *      Author: root
 */

#include "FashionLoader.h"
#include "Lua/LuaInterface.h"
#include <CharDefine.h>
#include <CSVLoader.h>
#include <StrArgPkg.h>
#include "Log/Logger.h"


int FashionLoader::Init(string& path)
{
	string dir = path + "Data/";
	if (0 != InitCsv(dir) || 0 != InitLua(dir))
	{
		return -1;
	}

	return 0;
}

int FashionLoader::InitCsv(string& path)
{
	string fashionFile = path + "FashionTypeData.csv";
	string fashionStrength = path + "FashionExtraBonus.csv";

	// 必须InitFashionBase先执行
	if (0 != InitFashionBase(fashionFile) || 0 != InitFashionStrength(fashionStrength))
	{
		return -1;
	}

	return 0;
}
int FashionLoader::InitFashionBase(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	int col = 0;
	string str;

	for (uint row = 0; row < loader.GetRowCount(); ++row)
	{
		FashionInfo::Fashion info;
		str.clear();
		col = 0;

		info.ID = loader.GetInt(row, col++);
		col++, col++, col++;

		str = loader.GetString(row, col++);

		StrArgPkg transMontant("|", str);
		int argIndex = 0;

		//  激活
		FashionInfo::Activate activateInfo;

		// 激活条件
		for (uint i = 0; i < transMontant.size(); ++i)
		{
			argIndex = 0;
			StrArgPkg transComma(":", getArg(transMontant, i));
			FashionInfo::Cond cond;

			cond.needItemID = atoi(getArg(transComma, argIndex++).c_str());
			cond.needItemCnt = atoi(getArg(transComma, argIndex++).c_str());
			cond.needBindCond = (FashionInfo::Cond::E_BIND_COND)(atoi(getArg(transComma, argIndex++).c_str()));
			activateInfo.cond.push_back(cond);
		}

		// 激活属性
		int attrNum = loader.GetInt(row, col++);
		for (int i = 0; i < attrNum; ++i)
		{
			ECharAttrType type = (ECharAttrType)(loader.GetInt(row, col++));
			if (0 >= type)
			{
				col++;
				continue;
			}
			DWORD64 value = loader.GetInt64(row, col++);
			activateInfo.add.attr.push_back(pair<ECharAttrType, DWORD64>(type, value));
		}

		info.activateInfo = activateInfo;

		m_fashionShow[info.ID] = info;
	}

	return 0;
}

int FashionLoader::InitFashionStrength(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	int col = 0;
	for (uint row = 0; row < loader.GetRowCount(); ++row)
	{
		col = 0;
		FashionInfo::Strength info;
		info.ID = loader.GetInt(row, col++);
		int fashionID = loader.GetInt(row, col++);
		if (m_fashionShow.end() == m_fashionShow.find(fashionID))
		{
			return -1;
		}

		col++, col++, col++;

		info.lv = loader.GetInt(row, col++);
		if (m_fashionShow[fashionID].strengthInfo.end() != m_fashionShow[fashionID].strengthInfo.find(info.lv))
		{
			return -1;
		}

		int attrNum = loader.GetInt(row, col++);
		for (int i = 0; i < attrNum; ++i)
		{
			ECharAttrType type = (ECharAttrType)(loader.GetInt(row, col++));
			if (0 >= type)
			{
				col++;
				continue;
			}
			DWORD64 value = loader.GetInt64(row, col++);

			info.add.attr.push_back(pair<ECharAttrType, DWORD64>(type, value));
		}

		StrArgPkg transMontant("|", loader.GetString(row, col++));
		for (uint i = 0; i < transMontant.size(); ++i)
		{
			int buffID = atoi(getArg(transMontant, i).c_str());
			if (0 >= buffID)
			{
				continue;
			}
			info.add.buff.push_back(buffID);
		}

		col++;
		// 强化条件
		transMontant.setArgs(loader.GetString(row, col++));
		transMontant.setPattern("|");
		int argIndex = 0;
		for (uint i = 0; i < transMontant.size(); ++i)
		{
			argIndex = 0;
			StrArgPkg transComma(":", getArg(transMontant, i));
			FashionInfo::Cond cond;

			cond.needItemID = atoi(getArg(transComma, argIndex++).c_str());
			cond.needItemCnt = atoi(getArg(transComma, argIndex++).c_str());
			cond.needBindCond = (FashionInfo::Cond::E_BIND_COND)(atoi(getArg(transComma, argIndex++).c_str()));
			info.cond.push_back(cond);
		}

		m_fashionShow[fashionID].strengthInfo[info.lv] = info;
	}

	return 0;
}

int FashionLoader::InitLua(string& path)
{
	string fashionDataFile= path + "FashionData.lua";

	if (0 != InitFashionData(fashionDataFile))
	{
		return -1;
	}

	return 0;
}
int FashionLoader::InitFashionData(string& file)
{
	CLuaInterface lua;
	lua.OpenLua();
	if (0 != lua.LoadFile(file) || 0 != lua.Call())
	{
		return -1;
	}

	if (0 != lua.SetLuaFunction("GetStrengthInfo") || 0 != lua.Call(0, 2))
	{
		return -1;
	}

	m_config.strengthMaxLv = lua.GetInt();
	m_config.strengthMinLv = lua.GetInt();

	lua.CloseLua();

	return 0;
}

const FashionInfo::Fashion* FashionLoader::GetFashionInfo(const int& fashionID) const
{
	const map<int, FashionInfo::Fashion>::const_iterator it = m_fashionShow.find(fashionID);
	if (m_fashionShow.end() == it)
	{
		return NULL;
	}

	return &(it->second);


}

const FashionInfo::Strength* FashionLoader::GetFashionStrength(const int& fashionID, const int& strengthLv) const
{
	const FashionInfo::Fashion* info = GetFashionInfo(fashionID);
	if (NULL == info)
	{
		return NULL;
	}

	const FashionInfo::Fashion::StrengthMap::const_iterator it = info->strengthInfo.find(strengthLv);
	if (it == info->strengthInfo.end())
	{
		return NULL;
	}

	return &it->second;
}

const FashionInfo::Config& FashionLoader::GetFashionConfig()
{
	return m_config;
}
