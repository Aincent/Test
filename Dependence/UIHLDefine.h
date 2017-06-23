#ifndef _UIHLDEFINE_H
#define _UIHLDEFINE_H

#include <define.h>


enum E_UIHL_BIGTYPE
{
	eUIHLBig_Null = 0, // null
	eUIHLBig_EquipSmith = 10000, // 锻造
	eUIHLBig_Skill = 11000, // 技能
	eUIHLBig_Role = 12000, // 角色
	eUIHLBig_Meridian = 13000, // 背包
	eUIHLBig_Welfare = 14000, // 福利大厅
	eUIHLBig_Country = 15000, // 阵营


//	eUIHLBig_Horse = 10100, // 坐骑
//	eUIHLBig_GirlWar = 10200, // 战姬

//	eUIHLBig_Package = 13000, // 背包
//	eUIHLBig_MaterialSynth = 14100, // 神工坊
//	eUIHLBig_Faction = 15100, // 帮派
//	eUIHLBig_Market = 16000, // 市场
//	eUIHLBig_GameTarget = 20000, // 游戏目标
//	eUIHLBig_Welfare = 21000, // 福利大厅
//	eUIHLBig_ExpCalabash = 21500, // 经验葫芦
//	eUIHLBig_ActivityHall = 21010, // 新服活动
//	eUIHLBig_FirstChargeRet = 21020, // 首充返还
//	eUIHLBig_DailyCharge = 21030, // 每日首充
//	eUIHLBig_SingleCharge = 21040, // 单笔充值
//	eUIHLBig_AccumulateCharge = 21050, // 累计充值
//	eUIHLBig_RelayCharge = 21060, // 接力充值
//	eUIHLBig_YellowVip = 21070, // 黄钻礼包
//	eUIHLBig_ActivityCompensation = 21080, // 新服活动：补偿活动


//	eUIHLBig_ActivityMagicUpgrade = 21090, // 新服活动：神兵升阶  --
//	eUIHLBig_ActivityHorseUpgrade = 21100, // 新服活动：坐骑升阶  --
//	eUIHLBig_LvInvestment = 21110, // 投资活动：等级投资  --
//	eUIHLBig_SevenDayRebate = 21120, // 投资活动：七天返利  --
//	eUIHLBig_FullService = 21170, // 新服活动：合服补尝  --
//	eUIHLBig_AccDayRecharge = 21180, // 新服活动：累积冲值天数活动  --

//	eUIHLBig_AccumulateConsume = 21190, // 累计消费
//	eUIHLBig_RedList = 21200, // 冲榜活动 －－

//	eUIHLBig_DailyCharge = 21210, // 新服活动：每日充值  --


//	eUIHLBig_Arena = 22200, // 竞技场
//	eUIHLBig_BeyondBattle = 22300, // 跨服竞技
//
//	eUIHLBig_FactionBattle = 22400, // 帮派战
//
//	eBig_EquipInst = 24100, // 装备副本
//	eBig_ProtectKing = 24200, // 守护秦王
//	eBig_MoneyInst = 24400, // 铜钱副本
//
//	eUIHLBig_Escort = 24500, // 护送皇纲--
//	eUIHLBig_TroubledTimes = 24600, // 乱世枭雄
//
//	eBig_ChallengeInst = 24700, // 挑战副本
//	eBig_TreasonKing = 24800, // 剿灭反王
//	eBig_Advanced = 24900, // 进阶副本
//	eUIHLBig_Turntable = 25000, // 转盘
//	eUIHLBig_Vitality = 26000, // 活跃度
//	eUIHLBig_Activity = 30000, // 活动
//	eUIHLBig_FirstAward = 31000, // 首充礼包
//	eUIHLBig_Answer = 32000, // 答题
//	eUIHLBig_MoneyShop = 40000, // 钱庄
//	eUIHLBig_Achivement = 41000, // 成就
//	eBig_DailyTask = 50000, // 日常任务

//	eBig_ClanTask = 51000, // 帮派任务
//	eBig_CntyTask = 52000, // 阵营任务


};

enum E_UIHL_SMALLTYPE
{
	eUIHLSmall_Null = 0, // null
	eUIHLSmall_EquipSmith_StrengthUnLvCeil = 10001, // 锻造装备强化未到达当前等级上限时
	eUIHLSmall_EquipSmith_InlayUnLvCeil = 10002, // 锻造镶嵌未到达当前等级上限时
	eUIHLSmall_EquipSmith_RefineUnLvCeil = 10003, // 锻造洗练未到达当前等级上限时
	eUIHLSmall_EquipSmith_UpgradeUnLvCeil = 10004, // :锻造升级未到达当前等级上限时
	eUIHLSmall_Skill_CanUpgrade = 11001, // 技能：能升级
	eUIHLSmall_Role_HorseCanUpgrade = 12001, // 角色：坐骑能进阶
	eUIHLSmall_Role_MagicCanAdvance = 12002, // 角色 神兵可以升阶
	eUIHLSmall_Role_MagicSkillCanRefresh = 12003, // 角色 神兵技能可以刷新
	eUIHLSmall_Role_SMagicCanAdvance = 12004,		// 角色 法器进阶
	eUIHLSmall_Role_DanLuCanAdvance = 12005, // 角色 丹炉可以升阶
	eUIHLSmall_Role_DanLuCanSmelting = 12006, // 角色 丹炉可以熔炼
	eUIHLSmall_Role_HeartMagicCanAdvance = 12007, 	// 角色 心法进阶
	eUIHLSmall_Meridian_CanAdvance = 13001, // 经脉：经脉可强化
	eUIHLSmall_Welfare_CanSignIn = 14001, // 福利大厅  签到 --
	eUIHLSmall_Welfare_CanMendingSignIn = 14002, // 福利大厅  补签
	eUIHLSmall_Welfare_SignInTotalAward = 14003, // 福利大厅  签到累计奖励
	eUIHLSmall_Welfare_CanGetOnlineAward = 14004, // 福利大厅 在线奖励可以领取奖励  --
	eUIHLSmall_Welfare_ServenLoginAward = 14005, // 福利大厅 七日登录奖励 可以领取 --
	eUIHLSmall_Welfare_CanGetLevelAward = 14006, // 福利大厅 等级奖励可以领取 --
	eUIHLSmall_Welfare_CanGetOffAward = 14007, // 福利大厅 离线奖励 可以领取 --
	eUIHLSmall_Welfare_CanResFound = 14008, // 福利大厅资源找回 --


//	eUIHLSmall_Role_MeridianCanAdvance = 10001, // 角色：经脉可强化
//	eUIHLSmall_Horse_GetNewSkin = 10101, // 坐骑：获得新皮肤
//
//	eUIHLSmall_GirlWar_GetNew = 10201, // 获得新战姬, 触发战姬界面关闭

//	// eUIHLSmall_Skill_ActiveSkillCanUpgrade = 18, // 技能：主动技能能升级
//	// eUIHLSmall_Skill_PassivityCanUpgrade = 47, // 技能：被动技能能升级
//	eUIHLSmall_Package_Full = 13001, // 背包满了

//	eUIHLSmall_MaterialSynth_HaveCanSynth = 14101, // 神工坊有可合成时
//
//	eUIHLSmall_Country_HaveUnGetReward = 15001, // 阵营：阵营红包奖励可领取时，完成所有可领取阵营红包奖励领取【包括阵营红包领取、红包被领取累计人数奖励、红包被领取奖励领取】、霸主争夺战奖励领取时关闭
//	eUIHLSmall_Country_CountryJobReward = 15002, // 阵营官职奖励可领取时
//	eUIHLSmall_Country_CountryShopCanBuy = 15003, // 阵营商店可购买东西
//
//	eUIHLSmall_Faction_HaveUnHandleJoin = 15101, // 帮派有未处理入帮申请时,【仅帮主、副帮主】显示,处理完入帮申请关闭
//
//	eUIHLSmall_Market_HaveSellItem = 16001, // 有出售出的物品 玩家自己上架的商品出售成功数量大于或等于1件
//
//	eUIHLSmall_GameTarget_OneCanGetReward = 20001, // 游戏目标第一个阶段有奖励未领取
//	eUIHLSmall_GameTarget_TwoCanGetReward = 20002, // 游戏目标第2个阶段有奖励未领取
//	eUIHLSmall_GameTarget_ThreeCanGetReward = 20003, // 游戏目标第3个阶段有奖励未领取
//	eUIHLSmall_GameTarget_FourCanGetReward = 20004, // 游戏目标第4个阶段有奖励未领取
//	eUIHLSmall_GameTarget_FiveCanGetReward = 20005, // 游戏目标第5个阶段有奖励未领取
//	eUIHLSmall_GameTarget_SixCanGetReward = 20006, // 游戏目标第6个阶段有奖励未领取
//	eUIHLSmall_GameTarget_SevenCanGetReward = 20007, // 游戏目标第7个阶段有奖励未领取
//	eUIHLSmall_GameTarget_EightCanGetReward = 20008, // 游戏目标第8个阶段有奖励未领取
//
//	// eUIHLSmall_Welfare_CanGetReward = 8, // 福利大厅有奖励未领取
////	eSmall_DaySign_Sign = 69, // 每日签到：每日剩余签到次数
//
//	eUIHLSmall_ExpCalabash_CanGetReward = 21501, // 经验葫芦经验可领取时
//
//	eUIHLSmall_ActivityHall_ChargeReturn = 21011, // 新服活动：首充返还
//	eUIHLSmall_ActivityHall_FirstCharge = 21012, // 新服活动：每日首充
//	eUIHLSmall_ActivityHall_Compensation = 21013, // 新服活动：补偿活动
//	eUIHLSmall_ActivityHall_MagicUpgrade = 21014, // 新服活动：神兵升阶
//	eUIHLSmall_AcitivityHall_HorseUpgrade = 21015, // 新服活动：坐骑升阶
//	eUIHLSmall_ActivityHall_AccDayRecharge = 21016, // 新服活动：累积冲值天数活动
//	eUIHLSmall_ActivityHall_AccumulateConsume = 21017, // 新服活动：累计消费
//	eUIHLSmall_ActivityHall_RedList = 21018, // 新服活动：冲榜活动
//	eUIHLSmall_ActivityHall_SingleCharge = 21019, // 新服活动：单笔充值
//	eUIHLSmall_ActivityHall_AccumulateCharge = 21020, // 新服活动:累计充值
//	eUIHLSmall_ActivityHall_RelayCharge = 21021, // 新服活动：接力充值
//
//	eUIHLSmall_InvestmentActivity_LvInvestment = 21022, // 投资活动：等级投资
//	eUIHLSmall_InvestmentActivity_SevenDayRebate = 21023, // 投资活动：七天返利
//	eUIHLSmall_ActivityHall_FullService = 21024, // 新服活动：合服补尝
//	eUIHLSmall_ActivityHall_DailyCharge = 21025, // 新服活动：每日充值
//
//	eUIHLSmall_YellowVip_NoviceGiftBag = 21131, // 黄钻活动：新手礼包
//	eUIHLSmall_YellowVip_GrowthGiftBag = 21141, // 黄钻活动：成长礼包
//	eUIHLSmall_YellowVip_DailyGiftBag = 21151, // 黄钻活动：每日礼包
//	eUIHLSmall_YellowVip_ExclusiveGirl = 21161, // 黄钻活动：专属战姬
//
//	eUIHLSmall_Arena_ChallengeNum = 22201, // 挑战次数未完成时,剩余挑战次数
//	eUIHLSmall_Arena_AwardUnGet = 22202, // 排名礼包未领取时
//
////	eUIHLSmall_BeyondBattle_ChallengeNum = 22301, // 跨服竞技活动开启剩余挑战次数大于>0
//	eSmall_SingleBeyondArena_AlreadyChallenge = 22301, // 跨服竞技：未挑战/总次数
//	eSmall_GroupBeyondArena_AlreadyChallenge = 22302, // 组队跨服竞技：未挑战/总次数
//
//	eUIHLSmall_FactionBattle_Open = 22401, // 霸业开启时
//
//	eUIHLSmall_EquipInst_AlreadyFini = 24101, // 装备副本
//
//	eSmall_ProtectKing_AlreadyFini = 24201, // 守护秦王：未完成/总次数
//	eSmall_MoneyInst_AlreadyFini = 24401, // 铜钱副本：未完成/总次数
//	eUIHLSmall_Challenge_EscortCanChallenge = 24501, // 剿灭反王：未完成/总次数
//
//	eUIHLSmall_TroubledTimes_BossCanBattle = 24601, // 乱世枭雄  有野外BOSS可挑战时
//	eUIHLSmall_TroubledTimes_WorldBoss = 24602, // 乱世枭雄 世界boss 可挑战个数
//	eSmall_ChallengeInst_AlreadyFini = 24701, // 挑战副本：未完成/总次数
//
//	eSmall_TreasonKing_AlreadyFini = 24801, // 剿灭反王剩余挑战次数大于0时
//
//	eSmall_Advanced_AlreadyFini = 24901, // 进阶副本
//
//	eUIHLSmall_Turntable_FreeNumUnUse = 25001, // 转盘免费次数未用完
//
//	eUIHLSmall_Vitelity_UnGetReward = 26001, // 活跃度奖励未领取
//
//
//	eUIHLSmall_Activity_AnswerOpen = 30001, // 答题活动开启时
//
//
//	eUIHLSmall_Activity_DoubleEscortOpen = 30002, // 双倍护送皇纲活动开启时
//	eUIHLSmall_Activity_DoubleGuardKingOpen = 30003, // 双倍守护秦王开启时
//	eUIHLSmall_Activity_BattlePVP = 30004, // 斗战重楼开启时
//
//	eUIHLSmall_Activity_SkyDropBox = 30005, // 天降宝箱
//
//	eUIHLSmall_CountryBossBattle_Open = 30006, // 董墓Boss战开启时
//	eUIHLSmall_CountryFightBattle_Open = 30007, // 董墓争夺战开启时
//
//	eUIHLSmall_FirstAward_FirstRecharge = 31001, // 玩家在游戏内第一笔充值成功时
//	eSmall_Answer_AnswerCnt = 32001, // 答题：剩余次数/总次数
//	eUIHLSmall_MoneyShop_FreeNumUnUse = 40001, // 钱庄
//
//	eUIHLSmall_Acheivement_HaveUnGetReward = 41001, // 成就奖励可领取时
//
//	eSmall_DailyTask_AlreadyFini = 50000, // 日常任务：未完成/总次数
//	eSmall_ClanTask_AlreadyFini = 51000, // 帮派任务：未完成/总次数
//	eSmall_CntyTask_AlreadyFini = 52000, // 阵营任务：未完成/总次数
};


//enum E_UIHL_BIGTYPE
//{
//	eUIHLBig_Null = 0, // null
//	eUIHLBig_FactionBattle = 1, // 帮派战
//	eUIHLBig_Arena = 2, // 竞技场
//	eUIHLBig_BeyondBattle = 3, // 跨服竞技
//	eUIHLBig_ActivityHall = 4, // 活动大厅 改为 新服活动
//	eUIHLBig_InstanceHall = 5, // 副本大厅
//	eUIHLBig_Vitality = 6, // 活跃度
//	eUIHLBig_Welfare =7, // 福利大厅
//	eUIHLBig_GameTarget = 8, // 游戏目标
//	eUIHLBig_ExpCalabash = 9, // 经验葫芦
//	eUIHLBig_GirlWar = 10, // 战姬
//	eUIHLBig_Turntable = 11, // 转盘
//	eUIHLBig_MoneyShop = 12, // 钱庄
//	eUIHLBig_MaterialSynth = 13, // 神工坊
//	eUIHLBig_Faction = 14, // 帮派
//	eUIHLBig_Country = 15, // 阵营
//	eUIHLBig_Role = 16, // 角色
//	eUIHLBig_Skill = 17, // 技能
//	eUIHLBig_Horse = 18, // 坐骑
//	eUIHLBig_Achivement = 19, // 成就
//	eUIHLBig_EquipSmith = 20, // 锻造
//	eUIHLBig_Practice = 21, // 试练场
//	eUIHLBig_Activity = 22, // 活动
//	eUIHLBig_EveryDay = 23, // 每日必做
//	eUIHLBig_CountryBossBattle = 24, // 董墓Boss战
//	eUIHLBig_CountryFightBattle = 25, // 董墓争夺战
//	eUIHLBig_Package = 26, // 背包
//	eUIHLBig_Market = 27, // 市场
//	eUIHLBig_Challenge = 28, // 挑战
//	eUIHLBig_FirstAward = 29, // 首充礼包
//	eUIHLBig_YellowVip = 30, // 黄钻礼包
//	eUIHLBig_TroubledTimes = 31, // 乱世枭雄
//	eUIHLBig_FirstChargeRet = 32, // 首充返还
//	eUIHLBig_DailyCharge = 33, // 每日首充
//	eUIHLBig_SingleCharge = 34, // 单笔充值
//	eUIHLBig_InvestmentActivity = 35, // 投资活动
//	eUIHLBig_AccumulateCharge = 36, // 累计充值
//	eUIHLBig_RelayCharge = 37, // 接力充值
//	eUIHLBig_AccumulateConsume = 38, // 累计消费
//	eBig_DaySign = 39, // 每日签到
//	eBig_DailyTask = 40, // 日常任务
//	eBig_ClanTask = 41, // 帮派任务
//	eBig_CntyTask = 42, // 阵营任务
//	eBig_SingleInst = 43, // 单人副本
//	eBig_ChallengeInst = 44, // 挑战副本
//	eBig_ProtectKing = 45, // 守护秦王
//	eBig_TreasonKing = 46, // 剿灭反王
//	eBig_MoneyInst = 47, // 铜钱副本
//	eBig_Answer = 48, // 答题
//	eBig_EliteInst = 49, // 精英副本
//	eBig_CauchemarInst = 50, // 噩梦副本
//};

//enum E_UIHL_SMALLTYPE
//{
//	eUIHLSmall_Null = 0, // null
//	eUIHLSmall_FactionBattle_Open = 1, // 霸业开启时
//	eUIHLSmall_Arena_ChallengeNum = 2, // 挑战次数未完成时,剩余挑战次数
//	eUIHLSmall_Arena_AwardUnGet = 3, // 排名礼包未领取时
//	eUIHLSmall_BeyondBattle_ChallengeNum = 4, // 跨服竞技活动开启剩余挑战次数大于>0
//	eUIHLSmall_ActivityHall_Open = 5, // 活动大厅开启时
//	eUIHLSmall_InstanceHall_CanChallenge = 6, // 副本大厅有可挑战(次数)
//	eUIHLSmall_Vitelity_UnGetReward = 7, // 活跃度奖励未领取
//	eUIHLSmall_Welfare_CanGetReward = 8, // 福利大厅有奖励未领取
//	eUIHLSmall_GameTarget_OneCanGetReward = 9, // 游戏目标第一个阶段有奖励未领取
//	eUIHLSmall_ExpCalabash_CanGetReward = 10, // 经验葫芦经验可领取时
//	eUIHLSmall_GirlWar_GetNew = 11, // 获得新战姬, 触发战姬界面关闭
//	eUIHLSmall_Turntable_FreeNumUnUse = 12, // 转盘免费次数未用完
//	eUIHLSmall_MoneyShop_FreeNumUnUse = 13, // 钱庄免费领取次数未用完
//	eUIHLSmall_MaterialSynth_HaveCanSynth = 14, // 神工坊有可合成时
//	eUIHLSmall_Faction_HaveUnHandleJoin = 15, // 帮派有未处理入帮申请时,【仅帮主、副帮主】显示,处理完入帮申请关闭
//	eUIHLSmall_Country_HaveUnGetReward = 16, // 阵营：阵营红包奖励可领取时，完成所有可领取阵营红包奖励领取【包括阵营红包领取、红包被领取累计人数奖励、红包被领取奖励领取】、霸主争夺战奖励领取时关闭
//	eUIHLSmall_Role_MeridianCanAdvance = 17, // 角色：经脉可强化
//	eUIHLSmall_Skill_ActiveSkillCanUpgrade = 18, // 技能：主动技能能升级
//	eUIHLSmall_Horse_CanUpgrade = 19, // 坐骑：坐骑能进阶
//	eUIHLSmall_Acheivement_HaveUnGetReward = 20, // 成就奖励可领取时
//	eUIHLSmall_Practice_HaveCanChallenge = 21, // 试炼场:挑战副本未挑战完成，功能ICON图标显示红色数值【二级挑战模块挑战次数之和】提示图标，所有二级功能模块挑战完毕，关闭图标
//	eUIHLSmall_EquipSmith_StrengthUnLvCeil = 22, // 锻造装备强化未到达当前等级上限时
//	eUIHLSmall_EquipSmith_RefineUnLvCeil = 23, // 锻造洗练未到达当前等级上限时
//	eUIHLSmall_EquipSmith_UpgradeUnLvCeil = 24, // :锻造升级未到达当前等级上限时
//	eUIHLSmall_EquipSmith_InlayUnLvCeil = 25, // 锻造镶嵌未到达当前等级上限时
//	eUIHLSmall_EveryDay_HaveUnComplete = 26, // 每日必做有未完成的
//	eUIHLSmall_CountryBossBattle_Open = 27, // 董墓Boss战开启时
//	eUIHLSmall_CountryFightBattle_Open = 28, // 董墓争夺战开启时
//	eUIHLSmall_Package_Full = 29, // 背包满了
//	eUIHLSmall_Market_HaveSellItem = 30, // 有出售出的物品 玩家自己上架的商品出售成功数量大于或等于1件
//	eUIHLSmall_TroubledTimes_BossCanBattle = 31, // 乱世枭雄  有野外BOSS可挑战时
//	eUIHLSmall_InstanceHall_GuardKingCanChallenge = 32, // 守护秦王剩余重置次数大于0时 有可能更改
//	eUIHLSmall_InstanceHall_KillTreasonKingCanChallenge = 33, // 剿灭反王剩余挑战次数大于0时
//	eUIHLSmall_Challenge_EscortCanChallenge = 34, // 护送皇纲剩余护送次数大于0时
//	eUIHLSmall_FirstAward_FirstRecharge = 35, // 玩家在游戏内第一笔充值成功时
//	eUIHLSmall_YellowVip_AwardCanGet = 36, // 黄钻礼包中有奖励可领取时 -- 已没用
//	eUIHLSmall_Activity_AnswerOpen = 37, // 答题活动开启时
//	eUIHLSmall_Activity_DoubleEscortOpen = 38, // 双倍护送皇纲活动开启时
//	eUIHLSmall_Activity_DoubleGuardKingOpen = 39, // 双倍守护秦王开启时
//	eUIHLSmall_Activity_BattlePVP = 40, // 斗战重楼开启时
//	eUIHLSmall_Activity_SkyDropBox = 41, // 天降宝箱
//	eUIHLSmall_Country_CountryJobReward = 42, // 阵营官职奖励可领取时
//	eUIHLSmall_Role_MagicCanAdvance = 43, // 角色 神兵可以升阶
//	eUIHLSmall_Role_DanLuCanAdvance = 44, // 角色 丹炉可以升阶
//	eUIHLSmall_Country_CountryShopCanBuy = 45, // 阵营商店可购买东西
//	eUIHLSmall_Role_MagicSkillCanRefresh = 46, // 角色 神兵技能可以刷新
//	eUIHLSmall_Skill_PassivityCanUpgrade = 47, // 技能：被动技能能升级
//	eUIHLSmall_Horse_GetNewSkin = 48, // 坐骑：获得新皮肤
//	eUIHLSmall_TroubledTimes_WorldBoss = 49, // 乱世枭雄 世界boss 可挑战个数
//	eUIHLSmall_ActivityHall_ChargeReturn = 50, // 新服活动：首充返还
//	eUIHLSmall_ActivityHall_RedList = 51, // 新服活动：冲榜活动
//	eUIHLSmall_ActivityHall_DailyCharge = 52, // 新服活动：每日充值
//	eUIHLSmall_ActivityHall_SingleCharge = 53, // 新服活动：单笔充值
//	eUIHLSmall_ActivityHall_AccumulateCharge = 54, // 新服活动:累计充值
//	eUIHLSmall_ActivityHall_FirstCharge = 55, // 新服活动：每日首充
//	eUIHLSmall_ActivityHall_MagicUpgrade = 56, // 新服活动：神兵升阶
//	eUIHLSmall_AcitivityHall_HorseUpgrade = 57, // 新服活动：坐骑升阶
//	eUIHLSmall_ActivityHall_RelayCharge = 58, // 新服活动：接力充值
//	eUIHLSmall_InvestmentActivity_LvInvestment = 59, // 投资活动：等级投资
//	eUIHLSmall_InvestmentActivity_SevenDayRebate = 60, // 投资活动：七天返利
//	eUIHLSmall_ActivityHall_AccumulateConsume = 61, // 新服活动：累计消费
//	eUIHLSmall_YellowVip_NoviceGiftBag = 62, // 黄钻活动：新手礼包
//	eUIHLSmall_YellowVip_GrowthGiftBag = 63, // 黄钻活动：成长礼包
//	eUIHLSmall_YellowVip_DailyGiftBag = 64, // 黄钻活动：每日礼包
//	eUIHLSmall_YellowVip_ExclusiveGirl = 65, // 黄钻活动：专属战姬
//	eUIHLSmall_ActivityHall_Compensation = 66, // 新服活动：补偿活动
//	eUIHLSmall_ActivityHall_FullService = 67, // 新服活动：合服补尝
//	eUIHLSmall_ActivityHall_AccDayRecharge=68, // 新服活动：累积冲值天数活动
//
//	eSmall_DaySign_Sign = 69, // 每日签到：每日剩余签到次数
//	eSmall_DailyTask_AlreadyFini = 70, // 日常任务：未完成/总次数
//	eSmall_ClanTask_AlreadyFini = 71, // 帮派任务：未完成/总次数
//	eSmall_CntyTask_AlreadyFini = 72, // 阵营任务：未完成/总次数
//	eSmall_SingleInst_AlreadyFini = 73, // 单人副本：未完成/总次数
//	eSmall_ChallengeInst_AlreadyFini = 74, // 挑战副本：未完成/总次数
//	eSmall_ProtectKing_AlreadyFini = 75, // 守护秦王：未完成/总次数
//	eSmall_TreasonKing_AlreadyFini = 76, // 剿灭反王：未完成/总次数
//	eSmall_MoneyInst_AlreadyFini = 77, // 铜钱副本：未完成/总次数
////	eSmall_Arena_AlreadyChallenge = 81, // 竞技场：未挑战/总次数
//	eSmall_SingleBeyondArena_AlreadyChallenge = 78, // 跨服竞技：未挑战/总次数
//	eSmall_GroupBeyondArena_AlreadyChallenge = 79, // 组队跨服竞技：未挑战/总次数
//	eSmall_Answer_AnswerCnt = 80, // 答题：剩余次数/总次数
//	eSmall_EliteInst_AlreadyFini = 81, // 精英副本：未完成/总次数
//	eSmall_CauchemarInst_AlreadyFini = 82, // 噩梦副本：未完成/总次数
//
//	eUIHLSmall_GameTarget_TwoCanGetReward = 83, // 游戏目标第2个阶段有奖励未领取
//	eUIHLSmall_GameTarget_ThreeCanGetReward = 84, // 游戏目标第3个阶段有奖励未领取
//	eUIHLSmall_GameTarget_FourCanGetReward = 85, // 游戏目标第4个阶段有奖励未领取
//	eUIHLSmall_GameTarget_FiveCanGetReward = 86, // 游戏目标第5个阶段有奖励未领取
//	eUIHLSmall_GameTarget_SixCanGetReward = 87, // 游戏目标第6个阶段有奖励未领取
//	eUIHLSmall_GameTarget_SevenCanGetReward = 88, // 游戏目标第7个阶段有奖励未领取
//	eUIHLSmall_GameTarget_EightCanGetReward = 89, // 游戏目标第8个阶段有奖励未领取
//};


enum E_UIHL_HANDLE
{
	eUIHLHandle_Null = 0, // 空操作
	eUIHLHandle_Add,
	eUIHLHandle_Del,
	eUIHLHandle_UpAndSyn, // 更新并智能同步
	eUIHLHandle_Mod,
	eUIHLHandle_Syn,
	eUIHLHandle_Up,
	eUIHLHandle_Init,
	eUIHLHandle_ModAndSyn,
	eUIHLHandle_UpAndForceSyn, // 更新并强制同步
	eUIHLHandle_ResetAndSyn, // 重置并同步
};

enum E_HINT_SHOW
{
	eHintShow_Null = 0, // 空
	eHintShow_UIHL, // UI高亮
	eHintShow_TaskPanel, // 任务面板
};
typedef std::pair<E_UIHL_BIGTYPE, E_UIHL_SMALLTYPE> UIHLType;

#endif // UIHLDefine.h
