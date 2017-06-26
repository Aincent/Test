/*
 * QuestDefine.h
 *
 *  Created on: 2014年10月20日
 *      Author: root
 */

#ifndef QUESTDEFINE_H_
#define QUESTDEFINE_H_

#include "define.h"
using namespace CommBaseOut;


#define NPC_PLAYER_DISTANCE 12

enum
{
	MAX_CONDITION_NUM				= 1,			/// 任务配置的完成条件的最大数目;现在我们任务完成条件只需要一种
	MAX_DAILYQUEST_STARLEVEL		= 5,			/// 日常任务最大星级[1,5]
	MAX_CLANQUEST_STARLEVEL			= 5,			/// 家族任务最大星级[1,5]
	MAX_CAMPQUEST_STARLEVEL			= 5,			/// 阵营任务最大星级[1,5]
	MAINTASK_STARTID					= 10001,				///主线任务开始ID
	DAILYTASK_STARTID					= 20001,				///日常任务开始ID
	CLANTASK_STARTID					= 30001,				///帮派任务开始ID
	CAMPTASK_STARTID					= 40001,				///阵营任务开始ID
	GUIDETASK_STARTID					= 50001,				///引导任务开始ID

};

enum eRefreshStarType
{
	eRefreshStarTNUll    = 0,
	eRefreshStarTNum     = 1,
	eRefreshStarReward	 = 2,
};

/// 用于任务条件的配置信息
struct CondCfg
{
	int type;	/// 条件类型
	int par1;	/// 条件参数1
	int par2;	/// 条件参数2

	CondCfg()
	: type(0)
	,	 par1(0)
	, par2(0)
	{}
};

//////////////////////////////////////////////////////////////////////////
/// 任务状态
enum QuestState
{
	QuestState_CanAccept=1,		/// 可接的
	QuestState_Accepted,		/// 已接但未完成
	QuestState_CanSubmit,		/// 可提交完成的
	QuestState_Failed,			/// 任务已失败，只能放弃重接
};

/// 任务类型
enum QuestType
{
	QuestType_Main = 1,				/// 主线任务
	QuestType_Functional,			/// 功能型日常任务
	QuestType_Clan,					/// 帮派任务
	QuestType_Camp,					/// 阵营
	QuestType_Guide,				/// 引导
	QuestType_Branch,				/// 支线
	QuestType_Week,					//  周任务
};

/*
 * 1.对话     2.杀怪
3.收集     4.采集
5.进入副本 6.NPC刷怪
7.购买普通 8.购买商品
9.送信     10.使用
11.查探    12.等级
13.强化    14.经脉
15.合成    16.镶嵌
17.坐骑    18.神兵
19.洗练    20.丹道
 * */
/// 任务条件类型
enum QuestCondType
{
	QuestCondType_Unknow,

	QuestCondType_DirectFinish,		// (对话)直接完成
	QuestCondType_KillMonster,		// 杀怪计数(怪物ID, 数量)
	QuestCondType_RemoveItem,		// 上缴物品，完成任务物品消失(物品ID,数量)
	QuestCondType_Mining,			// 采集/采集次数
	QuestCondType_EnterScene = 5,		// 进入指定场景

	QuestCondType_NpcFreMonster,	// Npc刷怪
	QuestCondType_OwnItem,			// 拥有物品，完成任务物品不消失(物品ID,数量)
	QuestCondType_BuyMallItem,		// 购买商城物品
	QuestCondType_SendMail,			// 送信
	QuestCondType_UseItem = 10,			// 使用指定道具/道具ID/数量

	QuestCondType_Check,			// 查探某地
	QuestCondType_Upgrade,			// 升级(等级数)
	QuestCondType_Strong,			// 强化/装备类型/强化等级
	QuestCondType_Channel,			// 经脉冲脉
	QuestCondType_Compose = 15,			// 合成

	QuestCondType_Slot,				// 镶嵌/装备类型/宝石ID
	QuestCondType_House,			// 坐骑
	QuestCondType_MagicWeapon, 		// 神兵
	QuestCondType_Smelt,			// 洗练/装备类型/洗练等级
	QuestCondType_Danlu = 20,			// 丹道

	/*新加*/
	QuestCondType_DailySign,        // 每日签到
	QuestCondType_SevenPackage,		// 七日礼包
	QuestCondType_Turntable,		// 转盘
	QuestCondType_OnLineGift,		// 在线礼包领取
	QuestCondType_LevelGift = 25,		// 等级礼包领取

	QuestCondType_UseTitle,			// 称号使用
	QuestCondType_SkillUpgrade,		// 技能等级升级
	QuestCondType_EquipSHop,        // 装备商店购买物品
	QuestCondType_EquipInherit,     // 装备继承
	QuestCondType_Activity = 30,			// 活跃度领取

	QuestCondType_CampContribute,	// 阵营捐献
	QuestCondType_MagicUpgrade,		// 神兵升级到指定等级
	QuestCondType_MagicRefresh,		// 神兵刷新技能
	QuestCondType_CreateGroup,		// 队伍创建
	QuestCondType_EquipUpgrade = 35,		// 锻造中升级装备

	QuestCondType_MoneyShop,		// 钱庄免费领取
	QuestCondType_HuangGang,		// 参加皇纲
	QuestCondType_GirlWarLingering,	// 战姬缠绵
	QuestCondType_GirlWarUpgrade,	// 战姬升级
	QuestCondType_GirlWarFit = 40,		// 战姬合体

	QuestCondType_JobLucky,			// 官印装备
	QuestCondType_WorldBoss,		// 世界BOSS
	QuestCondType_ParametersArena,	// 参加竞技场
	QuestCondType_House_Update, //坐骑升级到几阶几星
	QuestCondType_Danlu_Update = 45, //丹道升级到几阶几星

	QuestCondType_Strong_LV,			// 强化到几星
	QuestCondType_Arena_Success, //竞技场胜利次数
	QuestCondType_Slot_Num, //相应等级级宝石的颗数
	QuestCondType_Smelt_Num, //洗出相应星级属性的个数
	QuestCondType_EquipLv_Num = 50, //多少等级的装备数量

	QuestCondType_JobLucky_Lv, //镶嵌多少等级官印的个数
	QuestCondType_MoneyMap_Success, //铜钱副本通关了
	QuestCondType_RoleTower,		//hero tower completion times 
};

//任务 当前 状态 结果
struct Quest_CondStateInfo
{
	uint questID;
	QuestState state;
	BYTE numStar;
	BYTE rewardStar;
	BYTE times;
	int nowNum;
	int targeNum;

	Quest_CondStateInfo():questID(0),state(QuestState_CanAccept),
			numStar(1),rewardStar(1),times(0),nowNum(0),targeNum(0)
	{}
};


#endif /* QUESTDEFINE_H_ */
