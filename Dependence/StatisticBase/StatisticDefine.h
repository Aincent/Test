/*
 * StatisticDefine.h
 *
 *  Created on: 2014年10月6日
 *      Author: root
 */

#ifndef STATISTICDEFINE_H_
#define STATISTICDEFINE_H_

#include "define.h"

using namespace CommBaseOut;
//运营后台数据统计定义

namespace BehaviorType
{

	namespace Main
	{
//#define BEHAVIOR_MAINSPACE_MIN	eMain_BehaviorType_Min=500
//#define BEHAVIOR_MAINSPACE_MAX	eMain_BehaviorType_Max=5000

		enum E_TYPE // 类型
		{
//			BEHAVIOR_MAINSPACE_MIN,
			eJobLucky = 1, // 官运
			eTreasonKing, // 剿灭反王
			eMeridian, // 经脉
			ePackage, // 背包
			eDanLu, // 丹炉
			eMagicWeapon, // 神兵
			eSmith, // 锻造
			eHorse, // 坐骑
			eSkill, // 技能
			eMoneyShop, // 钱庄
			eNpcShop, // NPC商店
			eMall, // 商城
			eZazen, // 打坐
			eTask, // 任务
			eAchievement, // 成就
			eMail, // 邮件
			eClan, // 帮派
			eGirlWar, // 战姬
			eSignIn, // 签到
			eEscort, // 押镖
			eRecharge, // 充值
			eRole, // 角色
			ePvp, // 斗战重楼
			eOpeActivity, // 运营活动
			eProtectKing, // 守护秦王
			eYellowVip, // 黄钻
			eArena, // 竞技场
			eEquip, // 装备
			eCountry, // 阵营
			eResRecovery, // 资源追回
			eDestination, // 游戏目标
			eSynthesis, // 合成
			eAnswer, // 答题
			eTurnTable, // 转盘
			eInstance, // 副本
			eMarry, // 婚姻
			eMarket, // 市场
			eOnlineReward, // 在线奖励
			eTotalLogin, // 累计登录
			eFirstRecharge, // 首充
			eMicroReward, // 微端奖励
			eLevelReward, // 升级奖励
			eVIP, // vip
			eVitality, // 活跃度
			eOffExp, // 离线经验
			eCDKey, // 序列号
//			BEHAVIOR_MAINSPACE_MAX,

		};
	}

	namespace Minor
	{
//#define BEHAVIOR_MINORSPACE_MIN	eMinor_BehaviorType_Min=500
//#define BEHAVIOR_MINORSPACE_MAX	eMinor_BehaviorType_Max=5000

		enum E_TYPE // 类型
		{
			// ^.^：已完成 =.=：未完成
//			BEHAVIOR_MINORSPACE_MIN,
			eJobLucky_Exam = 1000, // 官运：科举 ^.^
			eJobLucky_OpenPack, // 官运：开启格子 ^.^
			eJobLucky_Exchange, // 官运：兑换官印 ^.^
			eJobLucky_Sale, // 官运：出售
			eJobLucky_Pick, // 官运：拾取
			eTreasonKing_Dig = 1100, // 剿灭反王：挖宝 ^.^
			eMeridian_Levelup = 1200, // 经脉：升级 ^.^
			ePackage_AddPack = 1300, // 背包：增加格子 ^.^
			ePackage_DropItem, // 背包：丢弃物品 ^.^
			ePackage_MoveGoods, // 背包：移动物品
			ePackage_UseGoods, // 背包：使用物品
			ePackage_PickItem, // 背包：拾取物品
			eDanLu_Levelup = 1400, // 丹炉：升级 ^.^
			eDanLu_Refine, // 丹炉：熔炼 ^.^
			eMagicWeapon_Levelup = 1500, // 神兵：升级 ^.^
			eMagicWeapon_FlushSkill, // 神兵：刷新技能 ^.^
			eSmith_Strength = 1600, // 锻造：强化^.^
			eSmith_Inlay, // 锻造：镶嵌^.^
			eSmith_Refine, // 锻造：洗炼^.^
			eSmith_Upgrade, // 锻造：升级
			eSmith_Succeed, // 锻造：继承
			eSmith_InlayQuickUp, //  锻造：镶嵌快速升级
			eHorse_Levelup = 1700, // 坐骑：升级 ^.^
			eHorse_BuySkin, // 坐骑：购买皮肤 ^.^
			eSkill_Levelup = 1800, // 技能：升级 ^.^
			eMoneyShop_GetMoney = 1900, // 钱庄：获取/兑换 金钱 ^.^
			eNpcShop_SaleItem = 2000, // NPC商店：出售物品 ^.^
			eNpcShop_BuyItem, // NPC商店：购买物品  ^.^
			eMall_Buy = 2100, // 商城：购物 ^.^
			eMall_BuyLimitGoods, // 商城：购买限时商品
			eZazen_Zazen = 2200, // 打坐：打坐 ^.^
			eZazen_CancelZazen, // 打坐：取消打坐 ^.^
			eTask_Submit = 2300, // 任务：提交任务 ^.^
			eAchieve_ActiveTitle = 2400, // 成就：激活称号 ^.^
			eAchieve_GetReward, // 成就：领取奖励 ^.^
			eAchieve_BuyGoods, // 成就：成就商店够买商品
			eAchieve_Pass, // 成就：达到成就
			eMail_GetItem = 2500, // 邮件：获取邮件物品 ^.^
			eClan_Join = 2600, // 帮派：加入帮派
			eClan_Exit, // 帮派：退出帮派
			eClan_Levelup, // 帮派升级
			eClan_GiveJob, // 帮派：任职
			eClan_Abdicate, // 帮派：禅让
			eClan_Create, // 帮派：创建帮派
			eClan_CreateGMCostGolden, // 帮派：创建帮派在GM扣除元宝
			eClan_CtrlWantJoinClan, // 帮派：审核入帮人员
			eClan_CtrlInviteJoinClan, // 帮派：审核 邀请入帮
			eClan_ExpelMember, // 帮派：开除成员
			eClan_Contribute, // 帮派：捐献
			eClan_BuyGoods, // 帮派：帮派商店购买商品
			eGirlWar_Levelup = 2700, // 战姬：升级 ^.^
 			eGirlWar_Advance, // 战姬：进阶 ^.^
 			eGirlWar_UpStar, // 战姬：升星 ^.^
 			eGirlWar_UpSkill, // 战姬：升级技能 ^.^
 			eEscort_SelQuality = 2800, // 押镖：选择高质皇纲 ^.^
 			eEscort_Refresh, // 押镖：刷新皇纲 ^.^
 			eEscort_StartEscort, // 押镖：开始护送 ^.^
 			eEscort_ImmedComplete, // 押镖：立即完成 ^.^
 			eEscort_Speedup, // 押镖：加速 ^.^
 			eEscort_Abandon, // 押镖：放弃 ^.^
 			eRecharge_Recharge = 2900, // 充值：平台充值
 			eRecharge_360Recharge, // 充值：360平台充值
 			eRecharge_58Recharge, // 充值：58平台充值
 			eRecharge_ConnRecharge, // 充值：网弦平台充值
 			eRecharge_TencentRecharge, // 充值：腾讯平台充值
 			eRole_CreateRole = 3000, // 角色：创建角色
 			eRole_Login, // 角色：登陆
 			eRole_Offline, // 角色：下线
 			ePvp_SendReward = 3100, // 斗战重楼：发送奖励
 			eOpeActivity_SendReward = 3200, // 运营活动：发送奖励
 			eOpeActivity_GetReward, // 运营活动：获得奖励
 			eOpeActivity_Invest, // 运营活动：投资
 			eProtectKing_GetNodeAward = 3300, // 守护秦王：领取奖励
 			eProtectKing_PassNodeAward, // 守护秦王：获取通关奖励
 			eProtectKing_StartSweep, // 守护秦王：开始扫荡
 			eProtectKing_SweepOver, // 守护秦王：扫荡结束
 			eYellowVip_GetNoviceAward = 3400, // 黄钻：领取新手奖励
 			eYellowVip_GetGrowAward, // 黄钻：领取成长奖励
 			eYellowVip_GetDailyAward, // 黄钻：领取每日奖励
 			eYellowVip_GetYearAward, // 黄钻：领取年费奖励
 			eYellowVip_GetGirlAward, // 黄钻：领取战姬奖励
 			eArena_GetRankAward = 3500, // 竞技场：领取排名奖励
 			eArena_AddChallCnt, // 竞技场：增加挑战次数
 			eArena_ReduceCDTime, // 竞技场：减少CD时间
 			eEquip_Wear = 3600, // 装备：穿戴
 			eEquip_Drop, // 装备：卸下
 			eCountry_Contribute = 3700, // 阵营：捐献
 			eCountry_GetJobAward, // 阵营：领取官职奖励
 			eCountry_BuyCntyShop, // 阵营：阵营商店购买商品
 			eCountry_GetRedEnvelope, // 阵营：领取红包
 			eResRecovery_ResRecovery = 3800, // 资源追回：资源追回
 			eDestination_GetPassAward = 3900, // 游戏目标：领取通过奖励
 			eSynthesis_MeterialSynth = 4000, // 合成：材料合成
 			eSynthesis_StoneSynth, // 合成：宝石合成
 			eAnswer_GetAward = 4100, // 答题：获取奖励
 			eAnswer_QuickHandle, // 答题：快捷操作
 			eAnswer_QuickHandleCost, // 答题：快捷操作消耗
 			eTurnTable_Roll = 4200, // 转盘：转盘
 			eTurnTable_Refresh, // 转盘：刷新
 			eTurnTable_GetAward, // 转盘：领取奖励
 			eInstance_ResetAllInst= 4300, // 副本：重置所有副本
 			eInstance_GetOverAward, // 副本：领取副本结束奖励
 			eInstance_GetCrossArenaAward, // 副本：领取跨服竞技奖励
 			eInstance_BuyMoneyInstCnt, // 副本：购买铜钱副本次数
 			eMarry_RingAdvance = 4400, // 婚姻：婚戒升阶
 			eMarket_ItemSell = 4500, // 市场：商品上架
 			eSignIn_SignIn = 4600, // 签到：签到
 			eSignIn_GetAward, // 签到：领取奖励
 			eOnlineReward_GetAward = 4700, // 在线奖励：领取奖励
 			eTotalLogin_GetAward = 4800, // 累计登录：领取奖励
 			eFirstRecharge_GetAward = 4900, // 首充：领取奖励
 			eMicroReward_GetAward = 5000, // 微端奖励：领取奖励
 			eLevelReward_GetAward = 5100, // 升级奖励:领取奖励
 			eVIP_GetAward = 5200, // vip：领取vip奖励
 			eVitality_GetAward = 5300, // 活跃度：领取奖励
 			eOffExp_GetExp = 5400, // 离线经验：领取经验
// 			eRecharge_PayInterface, // 充值：支付接口充值
			eCDKey_Exchange = 5500, // 序列号：兑换



//			BEHAVIOR_MINORSPACE_MAX,

		};
	}
}

namespace GlobalType
{
	namespace Main
	{
		enum E_TYPE // 类型
		{
			eJobLucky = 1, // 官运
			eEquipSmith, // 装备锻造
			eMaterialSynth, // 材料合成 神工坊
			eMeridian, // 经脉
			eMoneyShop, // 钱庄
			eSkill, // 技能
			eAllPlayer, // 所有玩家
			eCreateRole, // 创建角色
		};
	}

	namespace Minor
	{
		enum E_TYPE // 类型
		{
			eJobLucky_ExamCostMoney = 1000, // 官印 科举花费的金钱
			eEquipSmith_StrengthCostMoney = 1100, // 装备锻造 强化花费的金钱
			eEquipSmith_RefineCostMoney, // 锻造：洗练花费金钱
			eMaterialSynth_SynthCostMoney = 1200, // 材料合成 合成消费的金钱
			eMeridian_UpgradeCostMoney = 1300, // 经脉 升级经脉花费的金钱
			eMeridian_UpgradeCostForce, // 经脉 升级
			eMoneyShop_GetMoney = 1400, // 钱庄 得到的金钱
			eMoneyShop_BuyMoneyCostGolden, // 钱庄 购买金钱花费的元宝
			eSkill_UpgradeCostMoney = 1500, // 技能 升级花费的金钱
			eSkill_UpgradeCostForce, // 技能 升级花费的真气
			eAllPlayer_LeftGoldenSum = 1600, // 所有玩家  剩余元宝总和
			eAllPlayer_CurOnline, // 所有玩家 当前在线
			eCreateRole_HoverCreatePage_On = 1700, // 创建角色 停留创建角色页面
			eCreateRole_HoverCreatePage_Leave, // 创建角色 停留创建角色页面上后离开（关闭页面或者超时）
			eCreateRole_HoverCreatePage_Enter, // 创建角色 停留创建角色页面上后并进入游戏
		};
	}
}
//
////统计部分大的分类
//enum eStatisticMainType
//{
//	eStatic_Chat=1,				//聊天相关
//	eStatic_Charactor,		//2.角色相关
//	eStatic_Meridian,			//3.经脉相关
//	eStatic_Danlu,					//4.丹道
//	eStatic_MagicWeapon, //5.神兵
//	eStatic_OfficeStrans,//官运
//	eStatic_Package,				//背包
//	eStatic_Skill,					//技能
//	eStatic_Horse,				//坐骑
//	eStatic_Achievement,	//10.成就
//	eStatic_Friend,				//好友
//	eStatic_Team,						//队伍
//	eStatic_Instance,			//副本
//	eStatic_Reward,				//奖励大厅
//	eStatic_DailyAct,			//15.每日必作
//	eStatic_Escort,				//运镖相关
//	eStatic_Title,				//称号相关
//	eStatic_Map,						//地图
//	eStatic_Mail,						//邮件
//	eStatic_HandUp,				//20.挂机
//	eStatic_MoneyShop,		//钱庄
//	eStatic_Task,						//任务
//	eStatic_Mall,						//商城
//	eStatic_NpcShop,				//Npc商城
//	eStatic_VIP,						//25.VIP
//	eStatic_Sit,						//打坐
//	eStatic_PK,							//PK
//	eStatic_Monster,				//BOSS掉落
//	eStatic_Answer,				//答题
//	eStatic_TurnTable,		//30.转盘
//	eStatic_Ranking,				//排行榜排名
//	eStatic_Forge,					//32 锻造
//	eStatic_PackageGift,	//礼包
//	eStatic_AboutGolden,   //34元宝
//	eStatic_HoverCreateRolePage, // 停留在创建角色页面
//	eStatic_ExchangeCode,//兑换码相关
//	eStatic_Recharge,//充值
//	eStatic_Clan,//38帮派
//	eStatic_TreasonKing,//39剿灭反王
//	eStatic_ActivitesEx,//40 运营活动
//	eStatic_RedName,//红名相关
//	eStatic_Country,//阵营
//	eStatic_Market,//市场
//	eStatic_Force,//44.真气相关
//	eStatic_EquipFrom,//45 装备来源
//	eStatic_Money,//46 铜钱产入
//	eStatic_BindGolden,   //47 绑定元宝
//	eStatic_Credit,//48 声望
//	eStatic_PVPCarbon,//49 斗战重楼
//	eStatic_ProtectKing,//50 守护秦王
//	eStatic_Marry,//51 结婚
//	eStatic_GirlWar,//52 战姬
//	eStatic_PVPWorldBoss,//53 乱世枭雄
//	eStatic_ClanIntegral, //54 帮派积分战
//	eStatic_Max,
//};
///*
//  注释后面 带 ----------------------表示已添加
//  	  	  	  	  	  	 **********************************  没有此需求
//  	  	  	  	  	  	########################## 待测试
//*/
//
////具体逻辑相关类型
//enum eLogicRelevant
//{
//	eStaMinor_Chat_Horn = 1,							//1.喇叭使用  --------**
//	eStaMinor_Charactor_EuipChange,  	//2.装备更换紫色 或橙色  -------**
//	eStaMinor_Charactor_LevelUp,					//3.升级 ---------**
//	eStaMinor_Charactor_AttrChange,			//4.属性变更 (包含战斗属性)---------
//	eStaMinor_Charactor_CharDead,				//5.角色死亡 -----------**
//	eStaMinor_Charactor_CharRelive,			//6.角色复活（使用道具） ----------**
//	eStaMinor_Meridian_LevelUp,					//7.打通经脉 ---------
//	eStaMinor_Meridian_CostMoney,				//8.每日经脉消耗的铜钱 --------
//	eStaMinor_Meridian_CostForce,				//9.每日经脉消耗的真气数 ----------
//	eStaMinor_Danlu_LevelStar,						//10.丹道升星  ------------------
//	eStaMinor_MagicWeapon_LevelStar,		//11.神兵升星 --------------
//	eStaMinor_MagicWeapon_FlushSkill,			//12.神兵刷新使用次数 -----------------
//	eStaMinor_OfficeStrans_Exchange,		//13.兑换官印 -------------------
//	eStaMinor_OfficeStrans_CostMoney,	//14.每日官印消耗铜钱 --------------
//	eStaMinor_OfficeStrans_CostGold,		//15.元宝探望消耗 -----------------
//	eStaMinor_OfficeStrans_GetOrgAndPurple,	//16.获得的橙色和紫色装备  ----------------
//	eStaMinor_Package_BuyDrug,						//17.NPC商店购买药品   ------**
//	eStaMinor_Package_BuyCell,						//18.购买格子  ***********************
//	eStaMinor_Package_DiscardItem,			//19.背包格子丢弃绿色以上物品 ----------------
//	eStaMinor_Package_GetVioletAndOr,			//20.获得紫色和橙色物品  ---------------**
//	eStaMinor_Skill_LevelUp,							//21.技能升级   ---------------------
//	eStaMinor_Skill_LevelUpCostMoney,	//22.技能消耗铜钱数  ------------------
//	eStaMinor_Skill_LevelUpForce,				//23.技能消耗真气  --------------------
//	eStaMinor_Horse_LevelStar,						//24.坐骑升星  -------------------
//	eStaMinor_Horse_LevelUp,							//25.坐骑升阶   -----------------------
//	eStaMinor_Achievement_ActiTitle,		//26.激活称号  -----------------------
//	eStaMinor_Achievement_GetPrestige,	//27.获得成就点(某项成就完成)  ----------------------
//	eStaMinor_Friend_AddFriend,					//28.添加好友**
//	eStaMinor_Friend_AddEnemy,						//添加敌人**
//	eStaMinor_Friend_AddBlacklist,			//30.增加黑名单**
//	eStaMinor_Team_CreateTeam,						//创建队伍  -------------#############**
//	eStaMinor_Team_InviteToTeam,					//邀请入队  --------------################**
//	eStaMinor_Team_AppToTeam,							//申请入队 -----------------############**
//	eStaMinor_Instance_StoryTimes,			//剧情次数
//	eStaMinor_Instance_PersonTimes,			//35.个人副本次数
//	eStaMinor_Instance_TeamTimes,				//队伍副本次数
//	eStaMinor_Instance_ChallTimes,			//挑战副本次数
//	eStaMinor_Instance_StoryGetMoney,			//剧情次数获得铜钱**
//	eStaMinor_Instance_PersonGetMoney,			//个人副本获得铜钱**
//	eStaMinor_Instance_TeamGetMoney,				//40.队伍副本获得铜钱**
//	eStaMinor_Instance_ChallGetMoney,			//挑战副本获得铜钱**
//	eStaMinor_Instance_StoryResetTimes,			//剧情重置次数**
//	eStaMinor_Instance_PersonResetTimes,			//个人副本重置次数**
//	eStaMinor_Instance_TeamResetTimes,				//队伍副本重置次数**
//	eStaMinor_Instance_ChallResetTimes,			//45.挑战副本重置次数**
//	eStaMinor_Instance_StorySucTimes,			//剧情成功次数**
//	eStaMinor_Instance_PersonSucTimes,			//个人副本成功次数**
//	eStaMinor_Instance_TeamSucTimes,				//队伍副本成功次数**
//	eStaMinor_Instance_ChallSucTimes,			//挑战副本成功次数**
//	eStaMinor_Instance_StoryGemTimes,			//50.剧情成功次数**
//	eStaMinor_Instance_PersonGemTimes,			//个人副本成功次数**
//	eStaMinor_Instance_TeamGemTimes,				//队伍副本成功次数**
//	eStaMinor_Instance_ChallGemTimes,			//挑战副本成功次数**
//	eStaMinor_Reward_Checkin,							//签到   ---------------
//	eStaMinor_Reward_GetCheckinPrize,	//55.获得签到奖励  ----------------
//	eStaMinor_Reward_GetOnlinePrize,			//单次获得在线奖励 ------------
//	eStaMinor_Reward_GetAllPrize,					//全部 获得在线奖励 ----------------
//	eStaMinor_Reward_GrowUpPrize,					//成长奖励  ---------------------
//	eStaMinor_Daily_ActiPoint,							//活动积分**
//	eStaMinor_Challege_KillWorldBoss,		//60.击杀世界BOSS**
//	eStaMinor_Escort_StartEscort,				//参与运镖
//	eStaMinor_Escort_GiveUpEscort,			//放弃运镖
//	eStaMinor_Escort_EscortSucess,			//运镖成功
//	eStaMinor_Escort_RobEscort,					//劫镖次数**
//	eStaMinor_Ranking_Title,								//65.排行榜称号**
//	eStaMinor_Map_UseFlyShoe,								//使用小飞鞋   -----------------------**
//	eStaMinor_Map_TotalDropMoney,					//地图掉落总共的铜钱**
//	eStaMinor_Map_TotalKillMonster,				//地图掉落总共杀怪数**
//	eStaMinor_Map_TotalMaxPlayer,					//地图最大玩家数**
//	eStaMinor_Map_MapUseChance,						//70.每张地图的使用概率**
//	eStaMinor_Map_TotalFodder,							//草料掉落总数**
//	eStaMinor_Map_TotalWorldBoss,					//世界boss击杀总数**
//	eStaMinor_Mail_GetItem,								//获取邮件物品   -----------------###########***
//	eStaMinor_HandUp_UseDrug,								//使用药品总数***
//	eStaMinor_HandUp_GetItems,							//75.获得装备总数**
//	eStaMinor_MoneyShop_GetMoney,					//钱庄获得铜钱总数   --------------
//	eStaMinor_MoneyShop_ExchangeTimes,		//钱庄兑换总的次数  ---------------
//	eStaMinor_MoneyShop_GoldIncome,				//钱庄元宝总的收入(每日钱庄的消耗元宝数量)  -----------------
//	eStaMinor_Task_FinishMainLines,				//主线任务完成数量            ----------------#######
//	eStaMinor_Task_FinishBranLines,				//80.支线任务完成数量           ----------------#########
//	eStaMinor_Task_FinishDailys,						//日常任务                                     ----------------- ##########
//	eStaMinor_Task_FinishFactions,					//帮派任务  ---------------------------##################
//	eStaMinor_Mall_CostGold,								//商城花费的元宝   -------------------#############
//	eStaMinor_Mall_CostBindGold,						//商城花费的绑定元宝 ----------------- ############
//	eStaMinor_NpcShop_SellGetMoney,				//85.Npc卖东西获得的铜钱数    ----------------########
//	eStaMinor_NpcShop_BuyDrugMoney,				//买药消耗的铜钱数     *******************************
//	eStaMinor_VIP_GetVipPrize,							//获得VIP奖励    ---------------------
//	eStaMinor_VIP_VipLevelUp,								//VIP等级提升   ----------------------
//	eStaMinor_Sit_TotalTimes,								//打坐次数   --------------------- ###########**
//	eStaMinor_Battle_KillPlayers,					//90.杀人数***
//	eStaMinor_Battle_RedName,								//红名***
//	eStaMinor_Battle_ClearRedName,				//清除红名**
//	eStaMinor_Monster_KillBoss,						//击杀BOSS数量**
//	eStaMinor_Monster_DropOrangeItem,		//BOSS掉落的**
//	eStaMinor_Monster_DropPurpleItem,				//95.掉落紫色物品***
//	eStaMinor_Answer_Participate,					//参加答题活动***
//	eStaMinor_Answer_RightAnswer,					// ****
//	eStaMinor_TurnTable_CostGold,						//转盘花费的元宝    ------------- //   !!!
//	eStaMinor_TurnTable_FreeLotterry,			//转盘免费次数    -------------
//	eStaMinor_Ranking_First,									//100.排行榜第一***
//	eStaMinor_Ranking_Second,									//排行榜第二***
//	eStaMinor_Ranking_Three,									//排行榜第三***
//	eStaMinor_Ranking_Tens,										//排行榜前十***
//	eStaMinor_Forge_StrenTimes,							//强化次数***
//	eStaMinor_Forge_StrenCostMoney,					//105.强化花费的铜钱
//	eStaMinor_Forge_StrenCostItems,					//
//	eStaMinor_Forge_MountTimes,							//镶嵌次数**
//	eStaMinor_Forge_UnMountTimes,						//摘除次数**
//	eStaMinor_Forge_SuccinctTimes,									//洗练次数***
//	eStaMinor_Forge_SuccinctCostMoney,			//110.洗练消耗铜钱数***
//	eStaMinor_Forge_SuccinctCostItem,			//洗练消耗物品数
//	eStaMinor_Forge_ComposeTimes,						//合成次数***
//	eStaMinor_Forge_ComposeCostMoney,			//合成消耗铜钱数
//	eStaMinor_Forge_ComposeCostItem,				//合成消耗物品
//	eStaMinor_SevenDay_GetPrize,         //115.领取七日礼包   -----------------
//	eStaMinor_Recharge_First,            //首充   --------------***
//	eStaMinor_Golen_Cost,    //117...花费元宝
//	eStaMinor_BindGolen_Cost,    //118...花费绑定元宝
//	eStaMinor_Charactor_Create,									// 119. 创建角色
//	eStaMinor_AboutGolden_JobLuckyOpenPackage, // 120. 花费元宝 官运开启格子
//	eStaMinor_AboutGolden_JobLuckyRandJob, // 121. 花费元宝 随机官运 **
//	eStaMinor_AboutGolden_TreasonKingDigTreasure, // 122.花费元宝 剿灭反王 挖宝 ***
//	eStaMinor_AboutGolden_AddBackpackSize, // 123.花费元宝 背包增加格子 ***
//	eStaMinor_AboutGolden_GetDailyPracticeReward, // 124.花费元宝 获取日常离线奖励  ***
//	eStaMinor_AboutGolden_EatDanlu, // 125.花费元宝 丹炉进阶***
//	eStaMinor_AboutGolden_StrengthEquip, // 126.花费元宝 强化装备 ***
//	eStaMinor_AboutGolden_UpgradeEquip, // 127.花费元宝 升costAndGotInfo级装备***
//	eStaMinor_AboutGolden_SucceedEquip, // 128.花费元宝 继承装备***
//	eStaMinor_AboutGolden_EscortSetQuality, // 129.花费元宝刷新皇纲质量***
//	eStaMinor_AboutGolden_EscortSpeedUp, // 130.花费元宝 护送皇纲 加速***
//	eStaMinor_AboutGolden_EscortImmediateComplete, // 131.花费元宝 护送皇纲 立即完成***
//	eStaMinor_AboutGolden_AdvancedEatHorse, // 132.花费元宝 高级喂养坐骑****
//	eStaMinor_AboutGolden_AutoEatHorse, // 133.花费元宝 自动喂养坐骑***
//	eStaMinor_AboutGolden_BuyHorseSkin, // 134.花费元宝 购买坐骑皮肤  ***
//	eStaMinor_AboutGolden_RollTurnTable, // 135.花费元宝 转动转盘    ***
//	eStaMinor_AboutGolden_TurnTableReflushRewam_Ownerrd, // 136.花费元宝 刷新转盘奖励***
//	eStaMinor_AboutGolden_AdvanceMagicWeapon, // 137.花费元宝 神兵进阶***
//	eStaMinor_AboutGolden_MagicWeaponFlushSkill, // 138.花费元宝 神兵刷新技能
//	eStaMinor_AboutGolden_JoinTreasonKing, // 139.花费元宝 参加剿灭反王
//	eStaMinor_AboutGolden_ResetInstance, // 140.花费元宝 重置副本***
//	eStaMinor_AboutGolden_ResetAllInstance, // 141.花费元宝 重置所有副本***
//	eStaMinor_AboutGolden_AddChallengeTimes, // 142.花费元宝 增加挑战次数 竞技场
//	eStaMinor_AboutGolden_ChangeChallengeCD, // 143.花费元宝 改变竞技场CD时间 竞技场
//	eStaMinor_AboutGolden_ExchangeMoney, // 144.花费元宝 兑换铜钱****
//	eStaMinor_AboutGolden_ReviveGirlWar, // 145.花费元宝 复活战姬****
//	eStaMinor_AboutGolden_QuestDirectComplete, // 146.花费元宝 一键完成日常/帮派任务
//	eStaMinor_Charactor_Login, // 147.角色登陆
//	eStaMinor_HoverCreateRolePage_On, // 148.在创建角色页面上
//	eStaMinor_HoverCreateRolePage_Leave, // 149.在创建角色页面上后离开（关闭页面或者超时）
//	eStaMinor_HoverCreateRolePage_Enter, // 150.在创建角色页面上后并进入游戏
//	eStaMinor_ExchangeCode_Exchange,//151兑换礼品
//	eStaMinor_ExchangeCode_Result,//152兑换礼品结果 ***
//	eStaMinor_Recharge_Result,//153充值结果
//	eStaMinor_Clan_Join,//154加入帮派
//	eStaMinor_Clan_Leave,//155离开帮派***
//	eStaMinor_Clan_Killed,//156帮派成员被某玩家杀死***
//	eStaMinor_Clan_Donation,//157帮派成员捐献多少铜钱获得多少帮贡
//	eStaMinor_Clan_HoldPose,//158某某任职某某某某职位
//	eStaMinor_Clan_WeaponLvUp,//159帮主某某升级兵符至多少级，帮派上限多少人
//	eStaMinor_Clan_LoseCity,//160我帮的某某城池被某某帮占领了***
//	eStaMinor_Clan_LoseAuction,//161我帮对某某竞价宣战失败，返回多少铜钱到帮贡仓库中
//	eStaMinor_AboutGolden_AllPlayerGoldenLeft, // 162.所有玩家剩余元宝总和
//	eStaMinor_Mall_Event,//163 玩家商城购买物品事件
//	eStaMinor_SevenDay_Event,//164 玩家领取七日礼包事件
//	eStaMinor_Instance_RestEvent,//165 玩家重置事件
//	eStaMinor_Forge_UpGradeEvent,//166 玩家装备升级
//	eStaMinor_TreasonKing_EnterEvent,//167 进入某层
//	eStaMinor_TreasonKing_DigEvent,//168 挖宝
//	eStaMinor_DailyAct_GotOffLineEvent,//169 闭关修炼经验
//	eStaMinor_Meridian_UpgradeEvent,//170 玩家打通经脉
//	eStaMinor_Danlu_LevelStarEvent,//171 丹道升星事件
//	eStaMinor_MagicWeapon_LevelStarEvent,//172 玩家神兵什星事件
//	eStaMinor_Horse_LevelStarEvent,//173 坐骑升星事件
//	eStaMinor_Horse_BugHorseSkinEvent,//174 玩家购买皮肤事件
//	eStaMinor_OfficeStrans_RandJobEvent,//175 科举消耗事件Player58PlayRecharge
//	eStaMinor_OfficeStrans_ContriExchangeEvent,//176 官印兑换
//	eStaMinor_Package_BuyCellEvent,//177 购买格子
//	eStaMinor_Escort_SpeedUpEvent,//178 运镖加速事件
//	eStaMinor_Escort_EscortImmediateCompleteEvent, // 179.花费元宝 护送皇纲 立即完成事件
//	eStaMinor_Escort_EscortSucessEvent,//180  运镖成功事件
//	eStaMinor_Escort_RobEscortEvent,//181 劫镖成功事件
//	eStaMinor_Instance_ResetAllEvent,//182 重置所有副本事件
//	eStaMinor_MoneyShop_GetMoneyEvent,//183 钱庄兑换事件
//	eStaMinor_VIP_GetVipPrizeEvent,//184 获取VIP奖励事件
//	eStaMinor_TurnTable_RollEvent,//185 转盘事件
//	eStaMinor_AboutGolden_StrengthEquipEvent,//186 	装备强化
//	eStaMinor_Forge_StoneInsetEvent,//187 宝石镶嵌
//	eStaMinor_Forge_StoneRemoveEvent,//188 摘除
//	eStaMinor_Forge_SuccinctEvent,//189 洗练事件
//	eStaMinor_Forge_ComposeEvent,//190 宝石合成事件
//	eStaMinor_Forge_SucceedEquipEvent,//191 装备继承事件
//	eStaMinor_Recharge_RechargeEvent,//192 充值事件
//	eStaMinor_ActivitesEx_GotRewardEvent,//193 领取运营活动奖励事件
//	eStaMinor_Escort_StartEscortEvent,//194 参与镖车
//	eStaMinor_Escort_GiveUpEscortEvent,//195 放弃镖车
//	eStaMinor_Escort_EscortSetQualityEvent,//196 花费元宝刷新镖车品质
//	eStaMinor_ActivitesEx_InvestEvent,//197 运营投资活动事件
//	eStaMinor_PackageGift_YellowRewardEvent,//198 黄钻礼包领取
//	eStaMinor_AboutGolden_CountryDonate,//199 花费元宝阵营捐献***
//	eStaMinor_AboutGolden_RecoverSource,//200 花费元宝资源追回***
//	eStaMinor_AboutGolden_Refine,//201 花费元宝替换洗练属性
//	eStaMinor_AboutGolden_ResetInstanceSweep,//202 花费元宝副本扫荡立即结束
//	eStaMinor_AboutGolden_MarryBuyRing,//203 花费元宝结婚买戒指***
//	eStaMinor_AboutGolden_InvitationCelebration,//204 花费元宝结婚庆祝***
//	eStaMinor_Package_DiscardItemEvent,//205 摧毁物品
//	eStaMinor_NpcShop_SellItemEvent,//206 卖出物品事件
//	eStaMinor_Reward_GetRewardEvent,//207 领取奖励大厅奖励
//	eStaMinor_Mail_GetItemEvent,//208 邮件获取
//	eStaMinor_Task_GetReward,//209 任务奖励获得
//	eStaMinor_RedName_KillCount,//210 杀人数（能增加罪恶值的）
//	eStaMinor_RedName_RedNameBegin,//211 成为红名玩家的开始时间
//	eStaMinor_RedName_RedNameEnd,//212 脱离红名时间
//	eStaMinor_RedName_ClearRedNameCost,//213 清除罪恶值消耗
//	eStaMinor_Answer_GetRewardEvent,//214 答题奖励
//	eStaMinor_ExchangeCode_GetRewardEvent,//215 激活码兑换的奖励物品
//	eStaMinor_Achievement_MallBuyEvent,//216 成就商店购买
//	eStaMinor_Country_MallBuyEvent,//217 阵营商店购买
//	eStaMinor_Clan_MallBuyEvent,//218 帮派商店购买
//	eStaMinor_ActivitesEx_SendReward,//219 运营活动结束未领取发送给玩家
//	eStaMinor_Country_CountryDonateEvent,//220 阵营捐献事件
//	eStaMinor_DailyAct_RecoverSource,//221 资源追回事件
//	eStaMinor_Market_UpCarriages,//222 物品上架（cost中的金币或铜钱表示玩家想卖的价格）
//	eStaMinor_Market_SellGoods,//223 售卖
//	eStaMinor_Market_BuyGoods,//224 购买
//	eStaMinor_AboutGolden_AnswerCost,//225 答题的元宝消耗
//	eStaMinor_Force_ClanCity,//226 城池领取***
//	eStaMinor_Force_CrossArea,//227 夸服竞技排名奖励***
//	eStaMinor_Force_Instance,//228 副本奖励**
//	eStaMinor_Force_Box,//229 天降宝箱
//	eStaMinor_Force_Sign,//230 每日签到
//	eStaMinor_Force_RecoverResource,//231 资源追回
//	eStaMinor_Force_Destination,//232 目标达成领取
//	eStaMinor_Force_Mail,//233 邮箱获取
//	eStaMinor_Force_Task,//234 任务获取**
//	eStaMinor_Force_Meditation,//235 打坐
//	eStaMinor_Force_Achievement,//236 成就领取**
//	eStaMinor_Force_GirlWar,//237 升级战姬消耗**
//	eStaMinor_Force_Skill,//238 升级技能消耗
//	eStaMinor_Force_Meridian,//239 升级经脉消耗**
//	eStaMinor_EquipFrom_WorldBoss,//240 世界BOSS
//	eStaMinor_EquipFrom_Instance,//241 副本
//	eStaMinor_EquipFrom_Activity,//242 运营活动
//	eStaMinor_EquipFrom_FirstRecharge,//243 首冲大礼包
//	eStaMinor_EquipFrom_Market,//244 市场
//	eStaMinor_EquipFrom_Mall,//245 商店
//	eStaMinor_EquipFrom_Mail,//246 邮件
//	eStaMinor_Recharge_Process,//247 充值流程
//	eStaMinor_Money_ClanWarDaily,//248 霸业战每日可领取的铜钱奖励***
//	eStaMinor_Money_AreaRanking,//249 竞技场排名奖励的铜钱***
//	eStaMinor_Money_CrossAreaRanking, //250跨服竞技排名奖励***
//	eStaMinor_Money_Instance,//251 副本铜钱奖励***
//	eStaMinor_Money_WorldBoss,//252 击杀世界BOSS铜钱奖励****
//	eStaMinor_Money_Escort=253,// 镖车铜钱奖励***
//	eStaMinor_Money_Vitality,//254 活跃度铜钱奖励***
//	eStaMinor_Money_Answer,//255答题铜钱***
//	eStaMinor_Money_Sign,//256 签到铜钱
//	eStaMinor_Money_OnLine,//257 在线铜钱
//	eStaMinor_Money_SevenLogin,//258 7日累计上线铜钱
//	eStaMinor_Money_Lv,//259 等级铜钱奖励
//	eStaMinor_Money_RecoverResource=260,// 资源追回铜钱
//	eStaMinor_Money_Destination,// 261完成目标铜钱
//	eStaMinor_Money_Bank,//262 钱庄***
//	eStaMinor_Money_ActivityEx,//263 运营活动***
//	eStaMinor_Money_YellowDiamond,//264 黄钻***
//	eStaMinor_Money_PVPCarbonRanking,//265 斗战重楼排名***
//	eStaMinor_Money_Mail,//266 邮件获取铜钱
//	eStaMinor_Money_PickUp,//267 拾取铜钱
//	eStaMinor_Money_UseMoneyItem,//268 使用铜钱物品获取铜钱
//	eStaMinor_Money_SoldThings,// 269卖出物品***
//	eStaMinor_Money_SoldJobLucky=270,// 卖出官印
//	eStaMinor_Money_Achievement,//271 成就领取***
//	eStaMinor_Money_Quest,//272 任务获取***
//	eStaMinor_Money_UseGift,//273 使用礼包获取铜钱，比如阵营奖励发送的礼包等等
//	eStaMinor_Money_Envelope,//274红包累积奖励
//	eStaMinor_BindGolden_Sign,//275 签到获得绑元
//	eStaMinor_BindGolden_Lv,//276 等级奖励
//	eStaMinor_BindGolden_Destination,//277 完成目标得绑元
//	eStaMinor_BindGolden_Mail,//278 邮箱获得绑元
//	eStaMinor_BindGolden_UseGift,//279 使用礼包获取绑元，比如阵营奖励发送的礼包等等
//	eStaMinor_BindGolden_Envelope=280,// 阵营红包奖励
//	eStaMinor_BindGolden_ActivityEx,//281 运营活动获得绑元***
//	eStaMinor_Credit_Achievement,//282 成就声望
//	eStaMinor_Credit_Area,//283 竞技场声望***
//	eStaMinor_Credit_CountryBoss,//284 阵营BOSS
//	eStaMinor_Credit_Mail,//285 邮箱获得声望
//	eStaMinor_Credit_ActivityEx,//286 运营活动获得声望***
//	eStaMinor_Package_Move,//287 物品在背包以及仓库还有装备栏移动事件
//	eStaMinor_PVPCarbon_Reward,//288 斗战重楼奖励事件
//	eStaMinor_Instance_Reward,//289 副本奖励
//	eStaMinor_Danlu_Eat,//290 丹炉熔炼
//	eStaMinor_Country_JobReward,//291 领取阵营官职奖励
//	eStaMinor_ProtectKing_RewardInCarbon,//292 守护秦王中的奖励
//	eStaMinor_ProtectKing_PassCarbonReward,//293 守护秦王通关奖励
//	eStaMinor_Marry_Relation,//294 婚姻关系
//	eStaMinor_Marry_RingAdvance,//295 婚戒升阶
//	eStaMinor_Marry_RelationValue,//296 亲密度
//	eStaMinor_Forge_MaterialComposeEvent,//297 材料合成事件
//	eStaMinor_Reward_Vitality,//298 活跃度奖励
//	eStaMinor_Clan_ClanWarDaily,//299 霸业战每日可领取的奖励
//	eStaMinor_Reward_Area,//300 竞技场奖励
//	eStaMinor_Reward_CrossArea,//301 夸服竞技场奖励
//	eStaMinor_Reward_Achievement,//302 成就奖励
//	eStaMinor_Reward_Lv,//303 等级礼包奖励
//	eStaMinor_GirlWar_Lingering,//304 战姬缠绵***
//	eStaMinor_GirlWar_Fit,//305 战姬合体***
//	eStaMinor_GirlWar_Active,//306 战姬激活***
//	eStaMinor_BindGolden_ChangeChallengeCD,//307 花费绑定元宝 改变竞技场CD时间 竞技场
//	eStaMinor_AboutGolden_TreasonKingReset, // 308.花费元宝 重置剿灭反王次数
//	eStaMinor_BindGolden_TreasonKingReset, // 309.花费绑定元宝 重置剿灭反王次数
//	eStaMinor_BindGolden_AddChallengeTimes, //310 花费绑定元宝 增加挑战次数 竞技场
//	eStaMinor_BindGolden_JobLuckyOpenPackage, // 311. 花费绑定元宝 官运开启格子
//	eStaMinor_BindGolden_Refine,//312 花费绑定元宝替换洗练属性
//	eStaMinor_BindGolden_MagicWeaponFlushSkill, // 313.花费绑定元宝 神兵刷新技能
//	eStaMinor_AboutGolden_StartSweep,//314 花费元宝副本扫荡
//	eStaMinor_BindGolden_StartSweep,//315 花费绑定元宝副本扫荡
//	eStaMinor_AboutGolden_BuyMoneyInstanceCount,//316 花费元宝购买铜钱副本次数
//	eStaMinor_BindGolden_BuyMoneyInstanceCount,//317 花费绑定元宝副本扫荡
//	eStaMinor_BindGolden_QuestDirectComplete, // 318.花费绑定元宝 一键完成日常/帮派任务
//	eStaMinor_PVPWorldBoss_Reward,//319 乱世枭雄奖励
//	eStaMinor_Instance_AddChallengeCount,//320 怒战魂兵增加挑战次数
//	eStaMinor_GirlWar_SwapExp,//321 战姬传功***
//	eStaMinor_ClanIntegral_WinReward, //322 帮派积分战胜利奖励
//	eStaMinor_Max,
//};

////传输具体值的类型的定义
//enum eSaveDataType
//{
//	eStaSaveType_UseItem,		 //存储方式为 1:int32 = id; 2:int32 = num			例如 使用药品
//	eStaSaveType_ChangeItem, //存储方式为 1:int32 = id; 2:int32 = id				例如 换装备
//	eStaSaveType_NumChange,		//存储方式为 1:int32 = num; 2:int32 = num		例如 属性改变
//};



//enum eRechargeProcess
//{
//	//网页到管理服
//	eWebToMS = 1,
//	//管理服到世界服
//	eMSToWS,
//	//世界服到数据库服
//	eWSToDB,
//	//世界服到逻辑服
//	eWSToGS,
//};
//
//enum eResourceFlag
//{
//	//铜钱
//	eResourceFlag_Money = 1,
//	//经验
//	eResourceFlag_Exp = 1 << 1,
//	//真气
//	eResourceFlag_Force = 1 << 2,
//	//元宝
//	eResourceFlag_Golden = 1 << 3,
//	//绑定元宝
//	eResourceFlag_BindGolden = 1 << 4,
//	//帮贡
//	eResourceFlag_ClanContri = 1 << 5,
//	//阵营贡献
//	eResourceFlag_ContryContri = 1 << 6,
//	//声望
//	eResourceFlag_Credit = 1 << 7,
//
//	eResourceFlag_Max,
//};

//enum eMarryRelation
//{
//	//已婚
//	eMarryRelation_Married,
//	//离婚
//	eMarryRelation_Divorce,
//
//	eMarryRelation_Max,
//};


/////////////////////////////////////////////////////////////////////////////////////////////
// 由于 可能 会 delet 之后 忘记把 指针 置空  所以  加这个 宏控制

//#define Statis_SafeDelete(p)			{ if(p) { delete (p); (p) = NULL;} }
//
//#define Statis_SafeDeleteArray(p)		{ if(p) { delete [] (p); (p) = NULL;} }

//////////////////////////////////////////////////////////////////////////////////////


#endif /* STATISTICDEFINE_H_ */
