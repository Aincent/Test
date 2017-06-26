/*
 * RobberLoader.h
 *
 *  Created on: 2016年2月25日
 *      Author: root
 */

#ifndef ROBBERLOADER_H_
#define ROBBERLOADER_H_

#include "Singleton.h"
#include "define.h"
#include <vector>
#include "Lua/LuaInterface.h"

using namespace std;
using namespace CommBaseOut;

struct RobberMonsterInfo
{
	RobberMonsterInfo()
	{
		id = 0;
		money = 0;
		exp = 0;
		contribute = 0;
		clanMoney = 0;
		paramHP = 0;
		paramMP = 0;
		paramAttack = 0;
		paramDefene = 0;
		paramHit = 0;
		paramAvoid = 0;
		paramCrack = 0;
		paramAvoidCrack = 0;
	}
	int id;
	int money;
	int exp;
	int contribute;
	int clanMoney;

	float paramHP;
	float paramMP;
	float paramAttack;
	float paramDefene;

	float paramHit;
	float paramAvoid;
	float paramCrack;
	float paramAvoidCrack;
};

class RobberLoader : public Singleton<RobberLoader>
{
public:
	RobberLoader();
	~RobberLoader();
public:
	int InitLua(string & file);

	int GetNodeCount(){ return m_MonsterNodeInfo.size(); }

	void GetNodeInfo(int node, vector< pair<int,int> >& infos);

	bool GetMonsterPos(int index, int& posx, int& posy);

	const RobberMonsterInfo* GetRobberInfo(int id);

	int GetFlushTime(){ return m_times; }

	int GetFailCount(){ return m_failCount; }

	int GetWinMoney(){ return m_winMoney; }

	int GetFailMoney(){ return m_failMoney; }

private:
	CLuaInterface m_Robber;

	int m_times;

	int m_failCount;

	int m_winMoney;

	int m_failMoney;

	typedef vector< pair<int,int> > VectorPairInfo;
	map<int,VectorPairInfo> m_MonsterNodeInfo;

	VectorPairInfo m_monsterPos;

	map<int,RobberMonsterInfo> m_MonsterInfo;

};





#endif /* ROBBERLOADER_H_ */
