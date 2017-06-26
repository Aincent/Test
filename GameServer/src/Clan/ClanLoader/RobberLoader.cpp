/*
 * RobberLoader.cpp
 *
 *  Created on: 2016年2月25日
 *      Author: root
 */
#include "RobberLoader.h"
#include "StrArgPkg.h"

RobberLoader::RobberLoader():m_times(0),m_failCount(0),m_winMoney(0),m_failMoney(0)
{
	m_Robber.OpenLua();
}

RobberLoader::~RobberLoader()
{
	m_Robber.CloseLua();
}

int RobberLoader::InitLua(string & file)
{
	if(m_Robber.LoadFile(file) != 0 || m_Robber.Call() != 0)
	{
		return -1;
	}

	m_Robber.SetLuaFunction("GetTimes");
	if(m_Robber.Call(0, 1))
	{
		return -1;
	}

	m_times = m_Robber.GetInt();

	//连输场次
	m_Robber.SetLuaFunction("GetFailCount");
	if(m_Robber.Call(0, 1))
	{
		return -1;
	}

	m_failCount = m_Robber.GetInt();

	//胜利失败资金
	m_Robber.SetLuaFunction("GetFinalMoney");
	if(m_Robber.Call(0, 2))
	{
		return -1;
	}

	m_failMoney = m_Robber.GetInt();
	m_winMoney = m_Robber.GetInt();

	//刷怪信息
	m_Robber.SetLuaFunction("GetAllNode");
	if(m_Robber.Call(0, 1))
	{
		return -1;
	}

	int tableCount = m_Robber.GetTableFieldCount();
	for(int i=1; i<=tableCount; ++i)
	{
		if(m_Robber.GetFieldTable(i) != 0)
		{
			return -1;
		}

		int node = m_Robber.GetFieldInt("node");
		VectorPairInfo monsters;

		char buffer[256] = {0};
		m_Robber.GetFieldString(buffer, 128, "info");

		string strSkill = buffer;
		StrArgPkg strPkg("|", strSkill);
		for(size_t n=0; n<strPkg.size(); ++n)
		{
			string strTemp = getArg(strPkg,n);
			StrArgPkg strPkgTemp(":", strTemp);
			if(strPkgTemp.size()!=2)
				continue;

			monsters.push_back( make_pair(atoi(getArg(strPkgTemp,0).c_str()), atoi(getArg(strPkgTemp,1).c_str())) );
		}

		m_MonsterNodeInfo[node] = monsters;

		m_Robber.CloseTable();
	}
	m_Robber.CloseTable();

	//pos
	m_Robber.SetLuaFunction("GetPos");
	if(m_Robber.Call(0, 1))
	{
		return -1;
	}

	tableCount = m_Robber.GetTableFieldCount();
	for(int i=1; i<=tableCount; ++i)
	{
		if(m_Robber.GetFieldTable(i) != 0)
		{
			return -1;
		}

		int posx = m_Robber.GetFieldInt("posx");
		int posy = m_Robber.GetFieldInt("posy");
		m_monsterPos.push_back( make_pair(posx,posy) );

		m_Robber.CloseTable();
	}
	m_Robber.CloseTable();

	//monster
	m_Robber.SetLuaFunction("GetMonsterInfo");
	if(m_Robber.Call(0, 1))
	{
		return -1;
	}

	tableCount = m_Robber.GetTableFieldCount();
	for(int i=1; i<=tableCount; ++i)
	{
		if(m_Robber.GetFieldTable(i) != 0)
		{
			return -1;
		}

		RobberMonsterInfo info;
		info.id = m_Robber.GetFieldInt("monster");
		info.money = m_Robber.GetFieldInt("money");
		info.exp = m_Robber.GetFieldInt("Exp");
		info.contribute = m_Robber.GetFieldInt("contribute");
		info.clanMoney =  m_Robber.GetFieldInt("clanMoney");

		char buffer[256] = {0};
		m_Robber.GetFieldString(buffer, 128, "hp");
		info.paramHP = atof(buffer);

		m_Robber.GetFieldString(buffer, 128, "mp");
		info.paramMP = atof(buffer);

		m_Robber.GetFieldString(buffer, 128, "attack");
		info.paramAttack = atof(buffer);

		m_Robber.GetFieldString(buffer, 128, "defene");
		info.paramDefene = atof(buffer);

		m_Robber.GetFieldString(buffer, 128, "hit");
		info.paramHit = atof(buffer);

		m_Robber.GetFieldString(buffer, 128, "avoid");
		info.paramAvoid = atof(buffer);

		m_Robber.GetFieldString(buffer, 128, "crack");
		info.paramCrack = atof(buffer);

		m_Robber.GetFieldString(buffer, 128, "avoidCrack");
		info.paramAvoidCrack = atof(buffer);

		m_MonsterInfo[info.id] = info;

		m_Robber.CloseTable();
	}
	m_Robber.CloseTable();

	return 0;
}

void RobberLoader::GetNodeInfo(int node, vector< pair<int,int> >& infos)
{
	map<int,VectorPairInfo>::iterator it = m_MonsterNodeInfo.find(node);
	if(it != m_MonsterNodeInfo.end())
	{
		infos = it->second;
	}
}

bool RobberLoader::GetMonsterPos(int index, int& posx, int& posy)
{
	if(index >= (int)m_monsterPos.size())
		return false;

	posx = m_monsterPos[index].first;
	posy = m_monsterPos[index].second;
	return true;
}

const RobberMonsterInfo* RobberLoader::GetRobberInfo(int id)
{
	map<int,RobberMonsterInfo>::iterator it = m_MonsterInfo.find(id);
	if(it != m_MonsterInfo.end())
	{
		return &it->second;
	}
	return NULL;
}




