/*
 * RegionManage.cpp
 *
 *  Created on: 01/12/2015
 *      Author: root
 */


#include "RegionManage.h"


RegionManage::RegionManage()
{

}

RegionManage::~RegionManage()
{
	m_Lua.CloseLua();
}

int RegionManage::init(string& str)
{
	string tPath = str + "Data/Region.lua";

	m_Lua.OpenLua();

	if(m_Lua.LoadFile(tPath) != 0 || m_Lua.Call() != 0)
	{
		return -1;
	}

	return 0;
}

int RegionManage::getIntervalExp(int baseValue,int level,double multiple)
{
	m_Lua.SetLuaFunction("getIntervalExp");
	m_Lua.SetInt(baseValue);
	m_Lua.SetInt(level);
	m_Lua.SetFloat(multiple);

	if(m_Lua.Call(3,1) != 0)
		return 0;

	int tExp = m_Lua.GetInt();
	return tExp;
}
