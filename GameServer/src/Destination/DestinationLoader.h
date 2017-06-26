/*
 * DestinationLoader.h
 *
 *  Created on: 2014年11月27日
 *      Author: root
 */

#ifndef DESTINATIONLOADER_H_
#define DESTINATIONLOADER_H_

#include "define.h"
#include <map>
#include <vector>
#include <string>
#include "../Map/ConfigLoader/FieldInfo.h"
#include "Smart_Ptr.h"
#include "../Map/ConfigLoader/FieldMonster.h"
#include "Singleton.h"

using namespace CommBaseOut;
using namespace std;

//所有的游戏目标阶段
enum eDestinationStage
{
	eDestinationStage_1 = 1,
	eDestinationStage_2,
	eDestinationStage_3,
	eDestinationStage_4,
	eDestinationStage_5,
	eDestinationStage_6,
	eDestinationStage_7,
	eDestinationStage_8,
	eDestinationStage_Max,
};

enum eDestinationType
{
	DestinationType_Level = 1,		//等级
	DestinationType_HorseLevel = 2,  //坐骑阶级
	DestinationType_MagicWeaponLevel = 3, //神兵阶级
	DestinationType_DanLuLevel = 4,//丹道阶级
	DestinationType_Synthetic = 5,//合成
	DestinationType_Instance = 6,		//通关副本数
	DestinationTyp_KillBoss = 7,		//击杀BOSS
	DestinationTyp_RefreshMagicWeaponCount = 8,//刷新神兵技能次数
	DestinationTyp_ShoppingCount = 9,//商城用绑定元宝购买次数
	DestinationTyp_EquipStar = 10,//装备星级
	DestinationTyp_TotalAttackValue = 11,//总战力
	DestinationTyp_JobLuckyLeval = 12,//拥有一个x阶y级的官运
	DestinationTyp_Meridian = 13,//打通至IDx的穴位
	DestinationTyp_Escort = 14,//刷新出一个x级的镖车
	DestinationType_TurnedLuckyTableCount = 15, //累计转过x次转盘
	DestinationType_ArenaRank = 16,		//获得过<=x的名次
	DestinationType_GroupWinCount = 17,		//获胜过x次帮派战
	DestinationType_SkillLevel	= 18,		//用有x个达到y级的技能
	DestinationType_GetVipGiftCount = 19,	//领取x次VIP礼包
	DestinationType_ArenaOver = 20,		//打多少次竞技场

	DestinationType_SMagicWeaponLevel = 21, //神器阶级
	DestinationType_HeartMagicLevel = 22, //心法阶级
	DestinationType_EquipInsertStone = 23, //用有x个达到y级的宝石
	DestinationType_AddGirlWar = 24, //获得战姬
	DestinationType_GirlWarLevel = 25, //战姬阶级
	DestinationType_Fieldkill = 26, //野外击杀玩家
	DestinationType_PlayerDead = 27, //死亡次数
	DestinationType_EquipWear = 28, //x件橙装
	DestinationType_Max,
};

struct sDestinationEntry
{
	DWORD i_achID;				//游戏目标ID
	eDestinationStage i_stage;	//所属阶段
	eDestinationType i_target;		//游戏目标类型
	std::vector<DWORD> i_targetnumber;//获得游戏目标取得目标的数量,有些目标需要两个指标比如技能
	DWORD i_rewardID;            //奖励的物品ID
	DWORD i_rewardCount;         //奖励的物品数量
};


class DestinationLoader : public Singleton<DestinationLoader>
{
public:
	DestinationLoader();
	~DestinationLoader();
public:
	int InitDestination(string &file);

	sDestinationEntry * GetDestinationEntry(DWORD id);

	void GetAllDestinationID(std::vector<DWORD> & ids);
	void GetSpecifiedDestinationID(int type,std::vector<DWORD> & ids);
	void GetStageDestinationID(int stage,std::vector<DWORD> & ids);
private:
	map<DWORD,sDestinationEntry> m_DestinationInfo;   // sDestinationEntry - info
};



#endif /* DESTINATIONLOADER_H_ */
