/*
 * FieldMonster.cpp
 *
 *  Created on: 2013��10��22��
 *      Author: helh
 */

#include "FieldMonster.h"
#include "Lua/LuaInterface.h"
#include "Log/Logger.h"
#include "Smart_Ptr.h"
#include "Memory/MemAllocator.h"

using namespace CommBaseOut;

FieldMonster::FieldMonster(string &path, int id):m_mapID(id)
{
	m_file = path + "FieldMonster.lua";
}

FieldMonster::~FieldMonster()
{

}

int FieldMonster::Init()
{
	Smart_Ptr<CLuaInterface> lua = NEW CLuaInterface();

	//��ȡ�����ļ�
	lua->OpenLua();

	if(lua->LoadFile(m_file) != 0 || lua->Call() != 0)
	{
		return -1;
	}

	lua->SetLuaFunction("GetAllMonster");

	if(lua->Call(0, 1))
	{
		return -1;
	}

	int tableCount = lua->GetTableFieldCount();
	for(int i=1; i<=tableCount; ++i)
	{
		if (lua->GetFieldTable(i) != 0)
		{
			return -1;
		}

		MapMonster obj;

		obj.id = lua->GetFieldInt("monsterid");
		obj.type = (FieldMonsterType)lua->GetFieldInt("monsterflushtype");
		obj.count = lua->GetFieldInt("flushcount");
		obj.flushtime = lua->GetFieldInt("flushtime");

		if (lua->GetFieldTable(1) != 0)
		{
			return -1;
		}

		int twCount = lua->GetTableFieldCount();
		for(int j=1; j<=twCount; ++j)
		{
			if (lua->GetFieldTable(j) != 0)
			{
				return -1;
			}

			Pos rand;

			rand.xpos = lua->GetFieldInt("xpos");
			rand.ypos = lua->GetFieldInt("ypos");

			obj.randPos.push_back(rand);

			lua->CloseTable();
		}

		lua->CloseTable();
		lua->CloseTable();

		map<int, MapMonster>::iterator it = m_mapMonster.find(obj.id);
		if(it != m_mapMonster.end())
		{
			LOG_ERROR(FILEINFO, "same monster id in map");

			return -1;
		}

		m_mapMonster[obj.id] = obj;
	}

	lua->CloseTable();

	lua->CloseLua();

	return 0;
}

