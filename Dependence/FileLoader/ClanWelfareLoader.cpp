/*
 * ClanWelfareLoader.cpp
 *
 *  Created on: 2016年2月23日
 *      Author: root
 */

#include "ClanWelfareLoader.h"
#include "../CSVLoader.h"
#include "../StrArgPkg.h"
#include "Lua/LuaInterface.h"

ClanWelfareLoader::ClanWelfareLoader() {
	// TODO Auto-generated constructor stub

}

ClanWelfareLoader::~ClanWelfareLoader() {
	// TODO Auto-generated destructor stub
}

int ClanWelfareLoader::Init(string& path)
{
	string clanWelfareFile = path + "Data/GangGiftTypeData.csv";
	string clanWelBaseLua = path + "Data/ClanBase.lua";

	if (InitClanWelfare(clanWelfareFile) || InitWelfareBase(clanWelBaseLua))
	{
		return -1;
	}

	return 0;
}

int ClanWelfareLoader::InitClanWelfare(string& file)
{
	CSVLoader loader;

	if (!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	int count = 0;
	ClanWelfareInfo info;
	for (uint i = 0; i < loader.GetRowCount(); ++i)
	{
		count = 0;
		info.Clear();

		info.ID = loader.GetInt(i, count++);

		StrArgPkg transCom(",", loader.GetString(i, count++)); // 逗号
		int k = 0;
		ClanWelfareInfo::Items items;
		for (uint j = 0; j < transCom.size(); ++j)
		{
			k = 0;

			StrArgPkg transColon("|", getArg(transCom, j));

			items.itemID.push_back(atoi(getArg(transColon, k++).c_str()));
			items.itemNum.push_back(atoi(getArg(transColon, k++).c_str()));
			items.isBind.push_back(atoi(getArg(transColon, k++).c_str()));
		}
		if (items.itemID.size() != items.itemNum.size() || items.itemNum.size() != items.isBind.size())
		{
			return -1;
		}

		info.items = items;
		count++;
		info.price = loader.GetInt(i, count++);

		m_clanWelfareInfo[info.ID] = info;
	}

	return 0;
}

int ClanWelfareLoader::InitWelfareBase(string& file)
{
	CLuaInterface lua;
	lua.OpenLua();

	if (0 != lua.LoadFile(file) || 0 != lua.Call())
	{
		return -1;
	}

	lua.SetLuaFunction("GetWelfareBase");
	if (0 != lua.Call(0, 5))
	{
		return -1;
	}

	m_clanWelfareBase.giveDueWelContent = lua.GetInt();
	m_clanWelfareBase.giveDueWelTheme = lua.GetInt();
//	m_clanWelfareBase.sendWelNoticeContent = lua.GetInt();
	m_clanWelfareBase.welfareNum = lua.GetInt();
	m_clanWelfareBase.overTime = lua.GetInt();
	{
		if (0 != lua.GetFieldTable("memLv"))
		{
			return -1;
		}
		m_clanWelfareBase.lv.floor = lua.GetFieldInt("lvFloor");
		m_clanWelfareBase.lv.ceil = lua.GetFieldInt("lvCeil");

		lua.CloseTable();
	}
	{
		if (0 != lua.GetFieldTable("joinTime"))
		{
			return -1;
		}
		m_clanWelfareBase.joinTime.floor = lua.GetFieldInt("timeFloor");
		m_clanWelfareBase.joinTime.ceil = lua.GetFieldInt("timeCeil");

		lua.CloseTable();
	}
	{
		if (0 != lua.GetFieldTable("historyContri"))
		{
			return -1;
		}
		m_clanWelfareBase.contri.floor = lua.GetFieldInt("contriFloor");
		m_clanWelfareBase.contri.ceil = lua.GetFieldInt("contriCeil");

		lua.CloseTable();
	}
	{
		if (0 != lua.GetFieldTable("sendNum"))
		{
			return -1;
		}
		m_clanWelfareBase.sendNum.floor = lua.GetFieldInt("numFloor");
		m_clanWelfareBase.sendNum.ceil = lua.GetFieldInt("numCeil");

		lua.CloseTable();
	}

	lua.CloseTable();

	return 0;
}

const ClanWelfareInfo* ClanWelfareLoader::GetClanWelfareInfo(const int ID) const
{
	map<int, ClanWelfareInfo>::const_iterator it = m_clanWelfareInfo.find(ID);
	if (m_clanWelfareInfo.end() != it)
	{
		return &it->second;
	}

	return NULL;
}

const ClanWelfareBase* ClanWelfareLoader::GetClanWelfareBase() const
{
	return &m_clanWelfareBase;
}
