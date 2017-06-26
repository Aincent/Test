/*
 * MoneyShopLoader.cpp
 *
 *  Created on: 2014年8月12日
 *      Author: root
 */


#include "MoneyShopLoader.h"
#include "Log/Logger.h"
#include "CSVLoader.h"


int MoneyShopLoader::InitMoneyShop(string& file)
{
	CSVLoader loader;
	string tFile = file + "BankTpyeData.csv";
	string tStr  = file + "MoneyShop.lua";

	if(initMoneyLua(tStr))
	{
		return -1;
	}

	if(!loader.OpenFromFile(tFile.c_str()))
		return -1;

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		GoMoneyShopInfo info;
		int count = 0;

		info.id = loader.GetInt(i, count++, 0);
		info.freeMax = loader.GetInt(i, count++, 0);
		info.costMax = loader.GetInt(i, count++, 0);
		info.costLostBase = loader.GetInt(i, count++, 0);
		info.costUper = loader.GetInt(i, count++, 0);
		info.crackPro = loader.GetInt(i, count++, 0);
		info.moneyPar1 = loader.GetInt(i, count++, 0);
		info.moneyPar2 = loader.GetInt(i, count++, 0);
		++count;
		info.openLv = loader.GetInt(i, count++, 0);

		m_allMoneyShop.push_back(info);
	}

	return 0;
}

int MoneyShopLoader::initMoneyLua(string& str)
{
	m_Lua.OpenLua();

	if(m_Lua.LoadFile(str) != 0 || m_Lua.Call() != 0)
	{
		return -1;
	}

	return 0;
}

int MoneyShopLoader::getComsune(int level,int exchangeCounts)
{
	m_Lua.SetLuaFunction("GetConsumeGolden");
	m_Lua.SetInt(level);
	m_Lua.SetInt(exchangeCounts);

	if(m_Lua.Call(2,1))
	{
		return 0;
	}

	return m_Lua.GetInt();
}

void MoneyShopLoader::getMoney(int level,int freeCounts,int exchangeCounts,int& critValue,int& money)
{
	m_Lua.SetLuaFunction("GetGainMoney");
	m_Lua.SetInt(level);
	m_Lua.SetInt(freeCounts);
	m_Lua.SetInt(exchangeCounts);
	m_Lua.SetInt(rand());
	if(m_Lua.Call(4,2))
	{
		return ;
	}

	money 	  = m_Lua.GetInt();
	critValue = m_Lua.GetInt();

	return ;
}












