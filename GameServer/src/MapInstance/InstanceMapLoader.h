/*
 * InstanceMapLoader.h
 *
 *  Created on: 2014年6月6日
 *      Author: root
 */

#ifndef INSTANCEMAPLOADER_H_
#define INSTANCEMAPLOADER_H_

#include "define.h"
#include <map>
#include <vector>
#include <string>
#include "../Map/ConfigLoader/FieldInfo.h"
#include "Smart_Ptr.h"
#include "../Map/ConfigLoader/FieldMonster.h"
#include "Singleton.h"
#include "Lua/LuaInterface.h"
#include "CharDefine.h"

using namespace CommBaseOut;
using namespace std;

enum Difficulty
{
    REGULAR_DIFFICULTY           = 0,

    DUNGEON_DIFFICULTY_NORMAL    = 0,			//正常模式
    DUNGEON_DIFFICULTY_HEROIC    = 1,			//精英模式
    DUNGEON_DIFFICULTY_EPIC      = 2,			//史诗模式
};

struct DoubleTime
{
	DWORD m_hour;
	DWORD m_minute;

	DoubleTime():m_hour(0),m_minute(0)
	{

	}
};

struct InstanceMapEntry
{
	InstanceType m_eType;	//副本类型
	DWORD m_nMapID;				//副本ID
	DWORD m_nLevel;			//等级限制
	DWORD m_nXPos;				//出生的x坐标
	DWORD m_nYPos;				//出生Y坐标
	DWORD m_nReaptTimes;		//重复次数
	DWORD m_bAllowPk;			//是否允许PK
	DWORD m_nLimitTime;		//副本限制时间
	DWORD m_nResetPurchase;	//重置消耗
	DWORD m_nDelTime;		// 销毁时间
	DWORD m_nJgdelTime;		//结果销毁时间
	DWORD m_nWrdelTime;	//无人销毁时间
	DWORD m_nResetNum;		//重置次数
	DWORD m_nVipLimit; 	//VIP 限制
	string m_sName;
	string m_sDescript;		//副本描述
	vector<DWORD> m_nTarget; //副本目标
	vector<DWORD> m_nTargetNum; //对应目标数量

	InstanceMapEntry():m_eType(ePersonalInstance),m_nMapID(0),m_nLevel(0),m_nXPos(0),m_nYPos(0),m_nReaptTimes(0),m_bAllowPk(0),m_nLimitTime(0)
			,m_nResetPurchase(0),m_nDelTime(0),m_nJgdelTime(0),m_nWrdelTime(0),m_nResetNum(0),m_nVipLimit(0)
	{
		m_nTarget.clear();
		m_nTargetNum.clear();
	}
};

struct InstanceMapReward
{
	DWORD m_nMapID;				//副本ID
	DWORD m_nAwardMoney;		//奖励的金钱
	DWORD m_nAwardExp;		//奖励的经验
	DWORD m_nAwardSprit;		//奖励真气
	vector<DWORD> m_nAward;			//奖励物品ID
	vector<DWORD> m_nAwardNum;			//对应奖励物品数量
	map<DWORD,DWORD> m_nTongAward;	//通关奖励；


	InstanceMapReward():m_nMapID(0),m_nAwardMoney(0),m_nAwardExp(0),m_nAwardSprit(0)
	{
		m_nAward.clear();
		m_nAwardNum.clear();
		m_nTongAward.clear();
	}
};

class InstanceLoader : public Singleton<InstanceLoader>
{
public:
	InstanceLoader();
	~InstanceLoader();
public:
	int Init(string &file);
	int InitReward(string &file);

	map<DWORD, InstanceMapEntry>& GetEntry(){return m_InstanceInfo;}
	InstanceMapEntry *GetEntry(DWORD id);
	InstanceMapEntry *GetEntryByType(InstanceType type);

	int GetEntryType(int id)
	{
		map<DWORD, InstanceMapEntry>::iterator it =  m_InstanceInfo.find(id);
		if(it != m_InstanceInfo.end())
		{
			return it->second.m_eType;
		}

		return eInstanceMax;
	}

	InstanceMapReward *GetEntryReward(DWORD id);
	//获得副本ID
	void GetInstanceTypeIdList(InstanceType type,DWORD special,std::vector<int> & idList, int playerLv);
	//杀人不红名的地图ID
	bool IsNotAddSinValueMapID(const int& mapID);
private:
//	CLuaInterface m_pLua;
	map<DWORD, InstanceMapEntry> m_InstanceInfo;   // instanceId - info
	map<DWORD,InstanceMapReward> m_InstanceRewardInfo;
};

#endif /* INSTANCEMAPLOADER_H_ */
