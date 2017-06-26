/*
 * CountryOverlordCarbonLoader.h
 *
 *  Created on: 2016年10月28日
 *      Author: root
 */

#ifndef COUNTRYOVERLORDCARBONLOADER_H_
#define COUNTRYOVERLORDCARBONLOADER_H_

#include <map>
#include "define.h"
#include "Singleton.h"
#include "CharDefine.h"
#include "CountryDefine.h"
#include "Lua/LuaInterface.h"
#include "../Clan/ClanLoader/ClanLoader.h"

using namespace std;
using namespace CommBaseOut;

struct OverlordBaseData
{
	DWORD m_nMosterId;
	DWORD m_nXpos;
	DWORD m_nYpos;
	DWORD m_nCount;
	DWORD m_nFlushTime;

	OverlordBaseData():m_nMosterId(0),m_nXpos(0),m_nYpos(0),m_nCount(0),m_nFlushTime(0)
	{

	}
};

struct OverlordReward
{
	DWORD m_nType;				//奖励类型
	DWORD m_nNeedPoint;		//需要积分
	DWORD m_nAwardMoney;		//奖励的金钱
	DWORD m_nAwardExp;		//奖励的经验
	DWORD m_nAwardSprit;		//奖励真气
	DWORD m_nAwardCoun;		//奖励阵营贡献
	vector<DWORD> m_nAward;			//奖励物品ID
	vector<DWORD> m_nAwardNum;			//对应奖励物品数量


	OverlordReward():m_nType(0),m_nNeedPoint(0),m_nAwardMoney(0),m_nAwardExp(0),m_nAwardSprit(0),m_nAwardCoun(0)
	{
		m_nAward.clear();
		m_nAwardNum.clear();
	}
};


class CountryOverlordCarbonLoader : public Singleton<CountryOverlordCarbonLoader>
{
public:
	CountryOverlordCarbonLoader();
	virtual ~CountryOverlordCarbonLoader();

	int Init(string &path);

	int InitCampKingWarData(string& file);
	int InitCampKingWarReward(string& file);

	DWORD GetKillPlayerIntegral() {return m_nKillPlayer;}
	DWORD GetKillBossIntegral() {return m_nKillBoss;}
	DWORD GetAttackBossIntegral() {return m_nAttackBoss;}

	void GetBothPos(DWORD nType,int64 &xpos, int64 &ypos);

	const OverlordBaseData* GetOverlordBaseData();

	const OverlordReward* GetOverlordReward(DWORD nType,DWORD nNeed);

private:
	DWORD m_nKillPlayer;
	DWORD m_nKillBoss;
	DWORD m_nAttackBoss;
	map<DWORD,pont> m_nBothPont;
	vector<OverlordBaseData> m_nOverlordBaseData;
	vector<OverlordReward> m_nOverlordReward;
};

#endif /* COUNTRYOVERLORDCARBONLOADER_H_ */
