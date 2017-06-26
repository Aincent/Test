/*
 * VipCommon.h
 *
 *  Created on: 08/04/2015
 *      Author: root
 */

#ifndef VIPCOMMON_H_
#define VIPCOMMON_H_

#include "define.h"

using namespace CommBaseOut;
using namespace std;

#define VIP_HAVE_THIS_RIGHT			(1)   //拥有该权限

#define VIP_NOT_HAVE_THIS_RIGHT		(0)   //不拥有该权限

enum eVipRight // 必须与VIP的csv表相对应
{
	eVipMin = 0,

	eVipExpRestFreeCnt, 			//经验副本重置次数
	eVipAdvancedRestFreeCnt,  	//进阶副本重置次数
	eVipThirdLevelInstance, 	//V3专属副本,由instance模块写了
	eVipSxithLevelInstance, 	//V6专属副本,由instance模块写了
	eVipNinthLevelInstance, 	//V9专属副本,由instance模块写了

	eVipMoneyShopAdd,				//钱庄获得铜钱次数
	eVipArenaCanChallenge, 		//竞技场可挑战次数
	eVipBeyondSvrBattle,			//跨服竞技增加挑战次数
	eVipDoubleOfflineExp, 		// 两倍离线经验
	eVipTripleOfflineExp, 		// 三倍离线经验

	eVipMonthMendingSign, 		//当月补签次数
	eVipSignInDouble,				//VIP双倍签到奖励
	eVipAutoAdvanceMagic, 		// 神兵系统自动进阶
	eVipAutoAdvanceSMagic, 		// 神器系统自动进阶
	eVipAutoSmeltingDanLu,		// 丹道系统自动进阶

	eVipAutoHeartMagic,			//心法系统自动进阶
	eVipHorseAutoAdvance, 		// 坐骑系统自动进阶
	eVipDailyTaskImmediate, 	//日常任务一键完成
	eVipCampTaskImmediate, 		//阵营任务一键完成
	eVipClanTaskImmediate, 		//帮派任务一键完成

	eVipFlyShoesInfinet, 		// 飞鞋无限(免费传送


//	eVipHeroBuffer,				// 英雄塔攻击加成
//	eVipExpAdd, 				// 打怪经验加成
//	eVipMeditationForceAdd, 	// 打坐真气加成
//	eVipOpenPackGridSpeedAdd, 	// 背包格子开启速度加成
//	eVipOpenDepotGridSpeedAdd, 	// 仓库格子开启速度加成
//
//	eVipForceInstAdd, 			// 真气副本收益加成
//	eVipExpInstAdd, 			// 经验副本收益加成
//	eVipDoubleOfflineExp, 		// 两倍离线经验
//	eVipTripleOfflineExp, 		// 三倍离线经验
//
//
//	eVipEscortRefreshCnt,		//增加黄纲刷新次数
//	eVipFriendCounts,			// 好友上限个数
//
//	eVipSingleInstResetFreeCnt, // 个人副本每日固定重置次数
//	eVipTeamInstResetFreeCnt, 	// 逐鹿天下每日个人副本固定重置次数
//
//	eVipChallInstRestFreeCnt,	// 挑战副本每日固定重置次数
//	eVipTreasonKingCnt, 		// 剿灭反王每日固定重置次数
//	eVipMoneyInstAdd, 			// 铜钱副本每日固定购买次数
//	eVipGuardKingResetCnt, 		// 增加守护秦王重置次数
//	eVipMarryInstResetFreeCnt, // 伴侣副本每日固定重置次数
//
//
//	eVipFlyShoesInfinet, 		// 飞鞋无限(免费传送)
//	eVipAutoOpenMeridian, 		// 经脉系统自动打通经脉
//
//
//
//	eVipAllTaskImmediate, 		// 所有任务一键完成，表示该玩家是否拥有该功能
//	eVipOneKeyMelting,			// 丹炉一键熔炼
//
//	eOneSweepSingleInst, // 一键个人副本扫荡
//	eVipOneKeyImperial,			// 一键科举
//	eVipLevelRewardExtraAward, // 等级奖励VIP额外奖励
//	eVipMallDiscount,			// 商城打折
//	eVipSingleInstAdd,			// 个人副本每日固定购买次数


	eVipMax
};

struct sVipEntry
{
	DWORD m_nVipID;
	DWORD m_nAddUpGold;				//累计元宝数量
	vector<DWORD> m_nAward;			//奖励物品ID
	vector<DWORD> m_nAwardNum;			//对应奖励物品数量
//	DWORD m_nAddEscortFreshTimes;	//黄纲刷新增加次数
//	DWORD m_nChalInstanceResetTimes;//挑战副本重置次数
//	DWORD m_nDepotOpenAdd;			//仓库开启格子数
//	vector<DWORD> m_nFunctions;	//对应开启功能
	std::map<int, double> m_nFunctions; // key:特权index value:相对应特权的值 例如key:重置副本索引 value:重置副本加的次数
};

class VipCommonLoader
{
public:
	static int InitVip(string& file,map<DWORD,sVipEntry>& data);
};


#endif /* VIPCOMMON_H_ */

