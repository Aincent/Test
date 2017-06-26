/*
 * FieldSpring.cpp
 *
 *  Created on: 2014年10月24日
 *      Author: helh
 */

#include "FieldSpring.h"
#include "Lua/LuaInterface.h"
#include "Log/Logger.h"
#include "Smart_Ptr.h"


using namespace CommBaseOut;


FieldSpring::FieldSpring(string &path, int id):m_mapID(id)
{
	m_file = path + "FieldSpring.lua";
}

FieldSpring::~FieldSpring()
{

}


int FieldSpring::Init()
{
	CLuaInterface lua;

	//��ȡ�����ļ�
	lua.OpenLua();

	if(lua.LoadFile(m_file) != 0 || lua.Call() != 0)
	{
		return -1;
	}

	lua.SetLuaFunction("GetAllSpring");

	if(lua.Call(0, 1))
	{
		return -1;
	}

	int tableCount = lua.GetTableFieldCount();
	for(int i=1; i<=tableCount; ++i)
	{
		if (lua.GetFieldTable(i) != 0)
		{
			return -1;
		}

		MapSpring obj;

		obj.hprate = lua.GetFieldInt("springhp");
		obj.range = lua.GetFieldInt("springrange");
		obj.xpos = lua.GetFieldInt("xpos");
		obj.ypos = lua.GetFieldInt("ypos");

		lua.CloseTable();

		m_vecSpring.push_back(obj);
	}

	lua.CloseTable();

	lua.CloseLua();

	return 0;
}




