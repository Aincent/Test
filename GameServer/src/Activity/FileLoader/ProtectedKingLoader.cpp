/*
 * ProtectedKingLoader.cpp
 *
 *  Created on: 2014年11月28日
 *      Author: helh
 */

#include "ProtectedKingLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "Lua/LuaInterface.h"


ProtectedKingLoader::ProtectedKingLoader():m_nodeCount(0),m_vipLV(0),m_times(0),m_targetID(0),m_countDown(0),m_nSweepTimes(0)
{

}

ProtectedKingLoader::~ProtectedKingLoader()
{

}

int ProtectedKingLoader::Init(string &path)
{
	string nodeString = path + "Data/GuardTypeData.csv";
	string rewardString = path + "Data/GuardRewardTypeData.csv";
	string otherString = path + "Data/ProtectKing.lua";

	if(InitNodes(nodeString) || InitReward(rewardString) || InitOther(otherString))
	{
		return -1;
	}

	return 0;
}

int ProtectedKingLoader::InitNodes(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		ProtectKingMonster info;

		info.id = loader.GetInt(i, count++);

		std::vector<int> tempVec;
		string itemInclude = loader.GetString(i, count++);
		StrArgPkg includePkgItem("|", itemInclude);
		for(unsigned int k=0; k<includePkgItem.size(); ++k)
				tempVec.push_back(atoi(getArg(includePkgItem, k).c_str()));
		ProReflushMonster mon;

		if(tempVec.size() >= 3)
		{
			mon.id = tempVec[0];
			mon.count = tempVec[1];
			DWORD factor = tempVec[2];
			mon.factor = factor;

			info.monster.push_back(mon);
		}
		tempVec.clear();

		info.exp = loader.GetInt(i, count++);
		info.money = loader.GetInt(i, count++);
		info.force = loader.GetInt(i, count++);
		info.time = loader.GetInt(i, count++);
		info.finish = loader.GetInt(i, count++);

		m_allNodes.push_back(info);
	}

	return 0;
}

int ProtectedKingLoader::InitReward(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		ProtectedReward info;

		info.id = loader.GetInt(i, count++);
		info.node = loader.GetInt(i, count++);

		string itemInclude = loader.GetString(i, count++);
		StrArgPkg includePkgItem("|", itemInclude);

		for(WORD j=0; j<includePkgItem.size(); ++j)
		{
			StrArgPkg limitPkg(":", getArg(includePkgItem, j).c_str());
			for(unsigned int k=0; k<limitPkg.size(); ++k)
			{
				info.itemId.push_back(atoi(getArg(limitPkg, k).c_str()));
				info.count.push_back(atoi(getArg(limitPkg, ++k).c_str()));
			}
		}

		info.bindType = loader.GetInt(i, count++);

		m_allReward.push_back(info);
	}

	return 0;
}

int ProtectedKingLoader::InitOther(string &file)
{
	CLuaInterface lua;

	lua.OpenLua();
	if(lua.LoadFile(file) != 0 || lua.Call() != 0)
	{
		return -1;
	}

	lua.SetLuaFunction("GetAllPath");
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

		vector<Pos> tVec;

//		if (lua.GetFieldTable(1) != 0)
//		{
//			return -1;
//		}


		int twCount = lua.GetTableFieldCount();
		for(int j=1; j<=twCount; ++j)
		{
			if (lua.GetFieldTable(j) != 0)
			{
				return -1;
			}

			Pos rand;

			rand.xpos = lua.GetFieldInt(1);
			rand.ypos = lua.GetFieldInt(2);

			tVec.push_back(rand);
			lua.CloseTable();
		}

		m_reflushPoint.push_back(tVec);
		lua.CloseTable();
	}

	lua.CloseTable();

	lua.SetLuaFunction("GetOtherParam");
	if(lua.Call(0, 5))
	{
		return -1;
	}

	m_countDown = lua.GetInt();
	m_targetID = lua.GetInt();
	m_times = lua.GetInt();
	m_vipLV = lua.GetInt();
	m_nodeCount = lua.GetInt();


	lua.SetLuaFunction("GetSweepTimes");
	if(lua.Call(0, 1))
	{
		return -1;
	}

	m_nSweepTimes =lua.GetInt();

	lua.CloseLua();

	return 0;
}


void ProtectedKingLoader::GetNodeReward(int bnode, int enode, int &exp, int &force, int &money)
{
	if(bnode < 0 || enode < 0 || bnode >= enode || (size_t)bnode > m_allNodes.size() || (size_t)enode > m_allNodes.size())
		return ;

	if(bnode > 0)
	{
		ProtectKingMonster bmon = m_allNodes[bnode - 1];

		exp = bmon.exp;
		force = bmon.force;
		money = bmon.money;
	}

	ProtectKingMonster emon = m_allNodes[enode - 1];

	exp = emon.exp - exp;
	force = emon.force - force;
	money = emon.money - money;
}

int ProtectedKingLoader::GetNodeSweepTime(int bnode, int enode)
{
	int time = 0;

	if(bnode < 0 || enode < 0 || bnode >= enode || (size_t)bnode > m_allNodes.size() || (size_t)enode > m_allNodes.size())
		return 0;

	if(bnode > 0)
	{
		ProtectKingMonster bmon = m_allNodes[bnode - 1];
		time = bmon.time;
	}

	ProtectKingMonster emon = m_allNodes[enode - 1];

	time = emon.time - time;

	return time;
}

int ProtectedKingLoader::GetCanGetAwardNode(int node, int& realNode)
{
	int lastNode = 0, count = 0;
	for (uint i = 0; i < m_allReward.size(); ++i)
	{
		if (m_allReward[i].node > node)
		{
			break;
		}
		lastNode = m_allReward[i].node;
		++count;
	}

	realNode = lastNode;
	return count;
}
