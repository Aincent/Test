/*
 * ContailerLoader.cpp
 *
 *  Created on: 2014年12月26日
 *      Author: root
 */

#include "ContailerLoader.h"
#include "CSVLoader.h"
#include "Log/Logger.h"

ContailerLoader* ContailerLoader::GetInstance()
{
	static ContailerLoader m_Instance;
	return &m_Instance;
}


int ContailerLoader::Init(string &path)
{
	string luaFile = path + "Data/contailer.lua";
	string packageCsv = path + "Data/OpenBackpackData.csv";
	string houseCsv = path + "Data/OpenWarehouseData.csv";
	if(InitLua(luaFile) || InitPackage(packageCsv) || InitHouse(houseCsv))
	{
		return -1;
	}

	return 0;
}

int ContailerLoader::InitLua(string & file)
{
	CLuaInterface lua;

	lua.OpenLua();

	if(lua.LoadFile(file) != 0 || lua.Call() != 0)
	{
		return -1;
	}

	lua.SetLuaFunction("GetGolden");

	if(lua.Call(0, 1))
	{
		return -1;
	}

	m_nGolden = lua.GetInt();

	return 0;
}

int ContailerLoader::InitPackage(string & file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		OpenCountainer package;
		int count = 0;

		package.nNum = loader.GetInt(i, count++);
		package.nTime = loader.GetInt(i, count++) * 1000 * 60;
		package.nGolden = loader.GetInt(i, count++);

		m_vPackage[package.nNum] = package;
	}

	return 0;
}

int ContailerLoader::InitHouse(string & file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		OpenCountainer house;
		int count = 0;

		house.nNum = loader.GetInt(i, count++);
		house.nTime = loader.GetInt(i, count++) * 1000 * 60;
		house.nGolden = loader.GetInt(i, count++);
		m_vHouse[house.nNum] = house;
	}

	return 0;
}
int ContailerLoader::GetGoldenByNum(int index ,int num , int container)
{
	int golden = 0;
	if(container == ePackageType)
	{
		map<int,OpenCountainer>::iterator it =  m_vPackage.find(index-35+1);

		for(int i = 0;i < num && it != m_vPackage.end(); it++ ,i++)
		{
			golden+= it->second.nGolden;
		}
	}
	else if(container == eStoreHouseType)
	{
		map<int,OpenCountainer>::iterator it =  m_vHouse.find(index-28+1);

		for(int i = 0;i < num && it != m_vHouse.end(); it++ ,i++)
		{
			golden+= it->second.nGolden;
		}
	}
	return golden;
}


//int ContailerLoader::GetPackageNum(int nTime,int pos,bool flag)
//{
//	if(pos < 0 || pos >= m_vPackage.size())
//	{
//		return 0;
//	}
//
//	if(flag && m_vPackage[pos].nTime == nTime)
//	{
//		return m_vPackage[pos].nNum;
//	}
//	else
//	{
//		if(m_vPackage[pos].nTotalTime == nTime)
//		{
//			return m_vPackage[pos].nNum;
//		}
//	}
//
//	return 0;
//}

//int ContailerLoader::GetHouseNum(int nTime,int pos,bool flag)
//{
//	if(pos < 0 || pos >= m_vHouse.size())
//	{
//		return 0;
//	}
//
//	if(flag && m_vHouse[pos].nTime == nTime)
//	{
//		return m_vHouse[pos].nNum;
//	}
//	else
//	{
//		if(m_vHouse[pos].nTotalTime == nTime)
//		{
//			return m_vHouse[pos].nNum;
//		}
//	}

//	for(size_t i = 0 ; i < m_vHouse.size(); i++)
//	{
//		if(m_vHouse[i].nTotalTime == nTime)
//		{
//			return m_vHouse[i].nNum;
//		}
//	}

//	return 0;
//}
