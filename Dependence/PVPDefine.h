/*
 * PVPDefine.h
 *
 *  Created on: 2015年7月23日
 *      Author: root
 */

#ifndef PVPDEFINE_H_
#define PVPDEFINE_H_
#include "Common.h"
#include "define.h"
#include "Timer/TimerInterface.h"
#include <vector>
using namespace CommBaseOut;

#define PVP_KILL_SORT_NUM (10)
#define PVP_MAX_LAYER (4) 

enum ePVP_PrizeType
{
	ePVP_Damage = 1,			//伤害排名奖励
	ePVP_KillSort = 2,		//杀人总数排名奖励
	ePVP_Layer = 3,				//楼层奖励
	ePVP_LastAttack =4,	//BOSS最后一击奖励
	ePVP_KillNums = 5,		//击杀人数奖励
};

struct GatherEx
{
	GatherEx(long key,int64 charid):m_key(key),m_charID(charid){}
	int64 m_key;
	int64 m_charID;
};

//玩家采集
struct PlayerGatherData
{
	PlayerGatherData()
	{
		charID=0;
		objectKey=0;
		m_pParam = NULL;
	}
	int64 charID;		//角色
	int64 objectKey;//火种key
	GatherEx* m_pParam;
	TimerConnection m_timer;
};

//采集物信息
struct GatherInfo
{
	GatherInfo(){objectKey=0;}
	int64 objectKey;
	std::vector<int64> gatherPlayers;
};




struct PlayerPvPData
{
	PlayerPvPData()
	{
		layer = 0;
		kills = 0;
		bekills = 0;
		killSort = 0;
		gather = 0;
		totalkills = 0;
		exp = 0;
		integral = 0;
	}

	int layer;//当前层次
	int kills;//当前层击杀次数
	int bekills;//当前层被击杀次数
	int killSort;//击杀排行
	int gather;//采集次数
	int totalkills;//累计击杀次数
	int exp;//获得经验
	int integral; //积分
};

struct PlayerPvPDataEx : public PlayerPvPData
{
	PlayerPvPDataEx():charid(0),level(0),power(0),viplevel(0){}
	int64 charid;
	int level;
	int power;
	int viplevel;
	std::string name;
};

struct BossDamageData
{
	BossDamageData()
	{
		charid=0;
		viplevel=0;
		damage=0;
		sex=0;
		pro=0;
		lv=0;
	}
	int64 charid;
	int viplevel;
	int damage;
	int sex;
	int pro;
	int lv;
	std::string name;

};

//6:击杀排名

//8:副本剩余时间


//属性id对应的枚举值及意义
enum ePVPLayer
{
	ePVPLayer_curLayer=1,		//1:当前层次
	ePVPLayer_killCount=2,		//2:当前层击杀次数
	ePVPLayer_beKilled=3,		//3:当前层被击杀次数
	ePVPLayer_gather=4,			//4:采集次数
	ePVPLayer_totalKills=5,		//5:累计击杀次数
	ePVPLayer_killSort=6,		//6:击杀排名
	ePVPLayer_exp=7,				//7:获得经验
	ePVPLayer_time=8,				//8:副本剩余时间
	ePVPLayer_commonkill=9,		//9:普通怪
	ePVPLayer_bossKill=10,		//10:boss
};

//物品奖励结构
struct PropItem
{
	PropItem():id(0),num(0),bind(1){}
	PropItem(int paramID,int paramNum,int paramBind=1):id(paramID),num(paramNum),bind(paramBind){}
	int id;
	int num;
	int bind;
};

//击杀奖励
struct PVPSortPrize
{
	PVPSortPrize():min(0),max(0){}
	int min;
	int max;
	std::vector<PropItem> m_items;
};

//楼层信息
struct PVPLayerInfo
{
	PVPLayerInfo():layer(0),min(0),max(0),gather(0),kills(0),bekills(0),mon(0),boss(0){}
	int layer;
	int min;
	int max;
	int gather;
	int kills;
	int bekills;
	int mon;
	int boss;
};



#endif /* PVPDEFINE_H_ */
