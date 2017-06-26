/*
 * CharDefine.h
 *
 *  Created on: 2013��9��22��
 *      Author: helh
 */

#ifndef CHARDEFINE_H_
#define CHARDEFINE_H_


#define MUSCLE_ATTACK_DISTANCE 3 //սʿ��������
#define MAGIC_ATTACK_DISTANCE 8 //��ʿ��������
#define SHOOT_ATTACK_DISTANCE 8 //���乥������
#define EQUIP_CONTAINER 12 //装备栏的个数
#define PACKAGE_CONTAINER 50 //背包的个数
#define STOREHOUSE_CONTAINER 50 //仓库的个数
#define SIN_RED_NAME_COUNT 1000.0f//达到红名的界限
#define SIN_MAX_LIMIT_COUNT 100000.0f//罪恶值的最大值
#define LINE_PLAYER_MAX 5 * 10
#define LINE_MAP_PLAYER_MAX 2 * 10
//名字最长字符数(越南)
const int MAX_NAME_LEN = 16;
//名字最长字符数(网弦)
const int MAX_NAME_LEN_TX = 10;
const int MAX_CLAN_NAME_LEN = 16;
//邮件标题最长多少
const int MAX_THEME_LEN = 30;
//邮件正文最长多少
const int MAX_SUBSTANCE_LEN = 384;
//帮派公告长度
const int MAX_CLAN_NOTICE_LEN = 200;
//��ɫְҵ
enum ECharProf
{
	eMuscleMan=1, //սʿ
	eMagicHuman, //��ʿ
	eShooter,//����
	eProfessionMax
};
enum ECharSex
{
	eSexMale=0,
	eSexFemale,
};

//��Ӫ
enum ECharCountry
{
	eCountryNull=0, //����
	eCountryTang, // ����
	eCountryCruel, // �߸�
	eCountrySui, // ����
	eCountryMax
};

enum ECharBaseStruct
{
	eSkillInfo=0,
	eBaseInfo,
	eBattleInfo,
	eQuestInfo,
	eCDInfo,
	ePackageInfo,
	eStoreHouseInfo,
	eEquipQuickInfo,
	eMonitorInfo,//挂机信息
	eHorseInfo,//坐骑信息
	eFriendInfo, //好友信息
	eInstanceInfo, //副本信息
	ePrestigeInfo,	//成就信息
	eSignInInfo, //每日签到信息
	eCounterInfo,
	eTitleInfo,		//称号
	eEscortInfo,	//运镖
	eDanLuInfo, //练炉信息
	eVipInfo,	//VIP
	eMagicWeapon, //神兵
	eCJobItem, //官印
	eLucklyTurntable, //幸运转盘
	eDestinationInfo,//游戏目标
	eActivityInfo, //活动
	eGirlWarInfo, //战姬
	eFashionInfo, // 时装信息
	eVitalityInfo,	//活跃度信息
	eCampShopInfo,//陈营商店
	eFestivalInfo,//节日属性
	eMoneyInstanceInfo,//铜钱副本
	eMarryRingInfo,	//婚戒
	eOnceRewardInfo, // 一次性奖励信息
	eGroupInstanceInfo, //组队副本信息
	eChristmasInfo,		//圣诞节活动
	eEliteInstanceInfo,//精英副本
	eCauchemarInstanceInfo, //恶梦副本
	eArenaInfo, //
	eChallengeInfo,//挑战副本
	eHeroInstanceInfo,//英雄副本
	eSMagic,		//法器
	eHeartMagic,	//心法
	eAdvancedInstanceInfo, //进阶副本
	eBaseShopInfo,	//商店
	eAnswerInfo, //答题
	eCharStructMax
};

//�����������
enum ECharAttrType
{
	eCharName=0, //角色明
	eCharExp, //角色经验
	eCharLv, //等级
	eCharMoney, //金钱
	eCharGolden, //元宝
	eCharBindGolden, //綁定元宝
	eCharProfession, //职业
	eCharXpos, //x坐标
	eCharYpos, //y坐标
	eCharMapid, //地图id
	eCharCountry=10, //阵营
	eCharHead, //头像
	eFaContribution, //帮贡
	eCharFaction, //帮派
	eCharFaJob, //帮派职位
	eCharMate, //配偶
	eCountryJob, //官职
	eBattleContribution, //战功
	eArmyJob, //军职
	eCharHP,//血量
	eCharHPMax=20,//血量最大
	eCharMP,//法力
	eCharMPMax,//法力最大
	eCharForce, //真气
	eCharForceMax, //真气最大
	eCharStrength, //力量
	eCharPhysicalPower, //体质
	eCharPatience, //耐力
	eCharAgility, //敏捷
	eCharPhyAttackNULL,//暂时不用，之前最小物攻，
	eCharPhyAttackMax=30,//物攻最大
	eCharPhyDefence,//物防
	eCharMagAttackNULL,//暂时不用，之前最小法攻
	eCharMagAttackMaxNULL,//暂时不用，之前法攻最大
	eCharMagDefenceNULL,//暂时不用，之前魔防
	eCharHitRate, //命中概率
	eCharAvoidRate, //闪避值
	eCharMoveSpeed,//移动速度
	eCrackDownRate, //暴击
	eReagainRate, //回复
	ePKCount=40,//pk值
	eSimpleState, //基本状态改变
	eAvoidCrackRate, //抗暴
	eCharLuckyNULL, //暂时不用，幸运值
	eCleverPower, //灵力
	eCleverAir, //灵气
	ePower, //体力
	eCountryContri, //势力贡献
	eTitleID, //称号id
	ePowerMax, //体力最大值
	eFightPower=50, //战斗力
	eSinValue, //罪恶值
	eSettings,//玩家系统设置
	eHaveTodo,//每日必做
	eHaveTodoAward,	//每日必做奖品领取
	eActivities,	//每日活动
	eActivitiesAward,	//每日活动领取SavePlayerFashionInfo
	ePrestige,//成就
	ePosition, //坐标（包括x和y坐标）
	eVIPLv,		//VIP
	eAddUpGold=60,	//累计充值
	eMainEquip,	//主武器
	eCostume,	//服装
	eContinueAnswer,//连续答对的题目数目
	eRightAnswer,	//总共答对的题目
	eRanking,	//竞技排行
	eAwardRanking, //竞技排名奖励
	ePKType,//玩家PK模式
	eContinueBeheaded,//连斩数
	eCharClanIcon,  //帮派图标
	eTeamId=70,	//队伍ID
	eAntiTime,//防沉迷时间
	eCouple, //夫妻ID
	eCountryFitchPrize,//领取官职奖励
	eCurCountryContribute,//当前贡献度
	eCredit, //声望
	eEnvelopeCount=76,//可派发的红包个数
	eCanGetEnvelopeCount = 77,//可领取的红包个数
	eMagicweaponQuality = 78, //神兵阶级
	eHorseQuality = 79, //坐骑阶级
	eVipCardLv = 80,//Vip体验卡等级
	eVipCardTime = 81,//Vip体验卡剩余时间
	eGoldenCost = 82,//元宝消耗总量
	eCoupleName, //夫妻名字
	ePercentHurt,//伤害百分比
	eGourdExp,		//经验葫芦,0表示没有，大于0表示拥有数据，-1表示葫芦已失效
	ePvpFlag,				//pvp斗战重楼首战标识
	ePercentAttVampire = 87,			//吸血百分比
	ePercentShield 	   = 88,			//百分比减免
	ePercentCritRate   = 89,			//暴击率
	ePercentDodgeRate  = 90,			//闪避率
	eRemoveState	   = 91,			//移除状态
	eGuide 							= 92,			//指引标识
	eMeditationExpRate  = 93,			//打坐经验加成
	eKillMonsterExpRate = 94,			//杀怪经验加成
	eForceRate			= 95,			//真气加成
	eInjured			= 96,			//重伤
	eStateRemove		= 97,			//移除状态
	eStrengStun			= 98,			//增强晕眩概率
	eResistStun			= 99,			//抵抗晕眩概率
	eStrengSilent		= 100,			//强化沉默概率
	eResistSilent		= 101,			//抵抗沉默概率
	eStrengPoisoning	= 102,			//强化中毒概率
	eResistPoisoning	= 103,			//抵抗中毒概率

	eChallengeMoney = 104,  //挑战兑换货币

	eCharAttrMax,
};

//������������
enum EMonsterType
{
	eMonsterHP=0,
	eMonsterMP,
	eMonsterAttrMax,
};

enum ESynchType
{
	eSynchPos=0, // ͬ�����λ��
	eSynchPath, // ͬ��������
	eSynchMax
};

//�������״̬
enum EAttackState
{
	eAttackMiss			 = 0,
	eAttackSim  		 = 1, 		//普通状态
	eAttackCrack 		 = 2, 		//爆击
	eAttackSkill 		 = 3, 		//技能
	eAttackBuff 		 = 4, 		//buff
	eAttackDead 		 = 5, 		//死亡
	eAttackDestroy		 = 6, 		//移出
	eFightState 	     = 7, 		//战斗状态
	eAttackBackState 	 = 8, 		//击退
	eAttackFlyState 	 = 9, 		//击飞
	eAttackStill 		 = 10, 		//僵直
	eAttackIce 			 = 11, 		//冰冻
	eAttackStun 		 = 12, 		//晕眩
	eMeditation			 = 13,		//打坐状态
	eNonAttack 			 = 14,		//非战斗状态
	eInvincible 		 = 15, 		//无敌
	eAttackImmunityStill = 16,   	//免疫僵直
	eAttackImmunityIce 	 = 17,      //免疫冰冻
	eAttackImmunityStun  = 18,      //免疫晕眩
	eStateHorse 		 = 19,		//坐骑状态
	eWalkState			 = 20,		//行走状态
	eIntonate			 = 21,		//吟唱状态
	eSilentState		 = 22,		//沉默状态
	eImmuintySilent		 = 23,		//免疫沉默
	eSleepState			 = 24,		//催眠状态
	eImmunitySleep		 = 25,		//免疫催眠
	ePoisoning			 = 26,		//中毒状态
	eImmunityPoison		 = 27,		//免疫中毒
	eAttackMax,
};

//行为
enum eObjectDoing
{
	eDoingNull 		   = 0,
	eDoingReleaseSkill = 1,	//释放技能
	eDoingSwitchPKMode = 2, //切换PK模式
	eDoingMeditation   = 3, //打坐双修
};

//复活类型
enum ECharRevivalType
{
	eRevivalCommon=0, //״̬默认的，复活点复活
	eRevivalPlace, //原地复活״̬
	eRevivalToCity,	//回城复活
	eRevialRandom, //随机坐标点复活
	eRevivalMax,
};

enum EPlayerExitType
{
	eTypeOtherClient=0, //其他玩家顶下线
	eTypeLockAccount,   //被封帐号 踢下线
};
//排行榜类型
enum ERankingType
{
	eLvRanking = 0,		//等级排行
	eCpRanking,  				//战斗力排行
	eHoRanking, 				//坐骑排行
	eAchRanking, 				//成就排行
	eFaRanking,  				//帮派 暂时未加入
	eMwking,							//神兵排行
	eDanLuRanking,			//丹炉排行
	eAcupointRanking,	//经脉排行
	eGirlWarRanking, // 战姬排行
	eWorshipRanking, // 膜拜排行
	eAddUpGoldenRanking,//充值排行
	eGoldenCostRanking,//元宝消耗排行
	eSmagicRanking, //神器排行
	eHeartMagicRanking, //心法排行
	eBeyondBettleRanking,//跨服排行
	eArenaRanking, 	//竞技场排行
	eRankingMax
};
//聊天频道
enum EChatChannel
{
	eChatOrdinary = 0,				//普通
	eChatPrivate,							//私聊
	eChatRanks,									//队伍
	eChatFaction,							//帮派
	eChatInfluence,						//势力
	eChatWorld,									//世界
	eChatHorn,									//喇叭
	eChatSystem,						//系统
};
//跑马灯频道
enum EMarqueeChannel
{
	eMarqueeStrength = 1,  			//1:锻造	##player##将##item##强化到了##Iclv##，即将横扫天下!
	eMarqueeDrop,  								//2：道具	##player##击杀##monster##意外得到了##item##！
	eMarqueeActivity, 						//3：通过NPC进入的活动	 ##活动name##即将开启，请大家到##npcName##进入!
	eMarqueeGM,  										//4：纯文本输出	##文本内容##
	eMarqueeBoss,										//5：世界 boss	##Bossname##复活在##mapname##，大家可以去挑战。
	eMarqueeKillGuard,						//6：击杀守卫		##Playername##在##mapname##杀死##bossname##。
	eTreasonKingDigSomeThing, 	//7：剿灭反王挖掘得到东西		##playername##在剿灭反王副本中挖到##itemname##。
	eMarqueeGirl,										//8:战姬		##playername##获得##girlname##战姬
	eMarqueeClan,										//9:帮派占领		帮派##name##成功占领城池##name##
	eMarqueeBeyondSvr,						//10：括服竞技		恭喜##playername##达到王者段位
	eMarqueeCompete,								//11:竞技场前3名	##playername##在竞技场中击败##playername##获得第##number##名名次
	eMarqueeMw,											//12:神兵进阶		##playername##将神兵##sbname##进阶道##sbname##实力大增。
	eMarqueeDanLu,									//13:丹道进阶		##playername##将丹炉##dlname##进阶道##dlname##实力大增。
	eMarqueeAcupoint,							//14:打通经脉		##playername##将丹炉##jmname##进阶道##jmname##实力大增。
	eMarqueeHorse,								//15:坐骑进阶		##playername##将坐骑##zqname##进阶道##zqname##实力大增。
	eMarqueeLucklyTurntable,  					//16:转盘中 得到 东西		##playername##人品爆发在转盘中转到##itemname##
	eMarqueeEscort,								//17:钻石镖车			##playername##人品爆发，刷到钻石镖车
	eMarqueeCJobItem,							//18:橙色官印			##playername##获得橙色官印##gyname##
	eMarqueeMarrySuccess,						//19恭喜{相公角色名}与{妻子角色名}喜结连理，有情人终成眷属！我要结婚
	eMarqueeMarryBless,							//20婚礼祝福			##playername##送来祝福,住新人百年好合，情投意合，白头偕老
	eMarqueeMarryFlower,						//21{角色名}赠送{角色名}{itemID}，表达了他最深沉的爱！我要送花
	eMarqueeMarryEnd,							//22婚礼结束         ##playername##与##playername##婚宴已结束，感谢光临
	eMarqueeOccupyAll,							//23##xx阵营占领了所有五行柱,积分奖励翻倍了
	eMarqueeContinueWin,						//24##xx##阵营胜利了##xx##场,奖励翻倍了
	eMarqueeAnswerStart,						//25答题活动已经开启！前往参加
	eMarqueeAnswerReward,						//26{角色名}学富五车，成为本场答题状元
	eMarqueeClanCreate,							//27{角色名}创建了帮派｛帮派名｝
	eMarqueeOpenGift, 								// 28 {角色名}开启{宝箱或道具的名字}获得了{开出的物品名字}！
	eMarqueeWorldBossActive,				// 29 世界BOSS活动开启了
	eMarqueeWorldBossDead,					// 30 玩家###在###地图中击杀了怪物####
	eMarqueeRobberActive = 31,			//强盗活动开始
	eMarqueeClanWelfare = 32, 			// 32 {角色名}壕气冲天，发放了数个藏着珍贵物品的大礼包，大家快快去抢礼包啦！我要抢！！
	eMarqueeRobberWin = 33,					//帮派强盗活动胜利
	eMarqueeRobberFail = 34,				//帮派强盗失败
	eMarqueeArenaWin = 35, 					// 竞技场挑战某排名胜利
	eMarqueeClanWarResult = 36, 		// 霸业

	eMarqueeFirstCharge  = 37, 		// 领取首充
	eMarqueeVipLiveUp			= 38, 		// vip升级
	eMarqueeWorldBossUpdate = 39,		//世界Boss刷新
	eMarqueeWorldBossBloop  = 40,		//世界Boss血量低于一定值时提示
	eMarqueeSMagic = 41,				//法器升级
	eMarqueeHeartMagic = 42,			//心法升级
	eMarqueeCountryOverlordKill1 = 43,				//##playername##英勇无比,已经大杀特杀了。
	eMarqueeCountryOverlordKill1ed = 44,    //##playername##击杀了##playername##，阻止了他的疯狂杀戮
	eMarqueeCountryOverlordKill1Monster = 45, //##monstername##已被玩家##playername##斩于马下，获得大量积分
	eMarqueeCountryOverlordFoundMonster = 46, //##monstername##已经出现在战场上，各位英雄好汉速速擒拿此boss
};

//系统设置
enum ESettings
{
	eSettingsRanks = 0, //屏蔽组队
	eNearByPlayer,  //屏蔽附近的玩家
	eHideEffects,  //隐藏特效
	eHidePets, //隐藏宠物
	eBgSoundVolume, //背景音量
	eEfSoundVolume, //音效音量
	eSpSoundVolume, //语音音量
	eSettingsMax,
};




//玩家关系变化
enum eNexusChange
{
	eNexusChange_Friend,						//成为好友
	eNexusChange_RemoveFriend,		//解除好友
	eNexusChange_Couple,						//成为夫妻
	eNexusChange_RemoveCouple,		//解除夫妻
};


//玩家有可参加的活动  或者 还没领取完的奖励

enum EActiveHighLight
{
	eHL_MoneyShop = 0, //钱庄
	eHL_LucklyTable = 1, //转盘
	eHL_RewardDaTing = 2,  //奖励大厅
	eHL_DayActive = 3, //每日活动
	eHL_Threat = 4, //挑战
	eHL_PvpRank = 5, //竞技场
	eHL_SevenDay = 6,  //七日
	eHL_CountryEnvelope_Tan = 7,//唐阵营红包
	eHL_Compensate = 8,//补偿活动
	eHL_Answer = 9,//答题活动
	eHL_FirstRecharge = 10, // 开服首充
	eHL_Ranking = 11, //开服冲榜
	eHL_GoldenInvest = 12, // 元宝投资
	eHL_DailyRecharge = 13, // 每日充值
	eHL_SingleRecharge = 14, // 单笔充值
	eHL_AccRecharge = 15, // 累计充值
	eHL_DailyFirstRecharge = 16, // 每日首充
	eHL_MagicWeaponAdvance = 17,//神兵升阶
	eHL_HorseAdvance = 18,//坐骑升阶
	eHL_RelayRecharge = 19, // 接力冲值
	eHL_Festival = 20, //端午节活动
	eHL_CountryEnvelope_Sui = 21, // 隋阵营红包
	eHL_DailySign_GotReward=22,//每日签到,领取奖励
	eHL_DailySign_Sign = 23,//每日签到,签到
	eHL_RewardDaTing_OnLine = 24,//在线奖励
	eHL_RewardDaTing_Lv = 25,//等级奖励
	eHL_DailyToDo_OffLine = 26,//领取闭关修炼奖励
	eHL_DailyToDo_RecoverResource = 27,//资源追回
	eHL_ArenaReward = 28, // 竞技场奖励
	eHL_ActivityDegree = 29, // 活跃度
	eHL_ClanRequestIn = 30,//帮派申请加入
	eHL_ActivityFullService = 31, //合服补尝
	eHL_MicroReward = 32, // 微端奖励
	eHL_AccDayRecharge = 33, // 累计充值天数活动
	eHL_Max,
};

enum EDispatchMsg
{
	eChangeMap = 0,
	eRevival,
	ePlayerOnLine,
	eFastChangeMap,
	eQuickMessage,
};

//防沉迷状态
enum EAndiAddictionState
{
	eAAS_NotBind = 0,
	eAAS_BindButMinor,
	eAAS_BindAndAdult,
	eAAS_Max,
};


enum RemoveMethod
{
    GROUP_REMOVEMETHOD_DEFAULT  = 0,
    GROUP_REMOVEMETHOD_KICK     = 1,
    GROUP_REMOVEMETHOD_LEAVE    = 2,			//退出副本以外信息
    GROUP_REMOVEMETHOD_OUT	= 3							//退出清理包括副本以内信息
};

/*
 * 此枚举为 后台管理 操作 对在线玩家的影响 相关
 * */
enum eDataManagerEnum
{
	e_dataManager_Talk =1,  //可以说话
	e_dataManager_Shutup ,  // 禁止 说话
	e_dataManager_EnterLogin,  //解封帐号
	e_dataManager_LockAccount,  //禁封帐号
	e_dataManager_Max
};

//职业
enum EPro
{
	eProMengJiangMan = 1,				//男猛将
	eProMengJiangWoman,					//女猛将
	eProFeiYuMan,						//男飞羽
	eProFeiYuWoman,						//女飞羽
	eProFangShiMan,						//男方士
	eProFangShiWoman,					//女方士
};
//复活模式
enum ERevivalType
{
	eReLeave = 1,							//切换到不同地图
	eReMove,										//同地图切换位置
	eReChangeMap,							//夸服切换地图
};

enum InstanceType
{
	ePersonalInstance = 1,			//个人
	eGroupInstance,				//组队
	eBattleGroudInstance,		//混合  相当于战场
	eChallenge = 4,//挑战副本
	eAnswerInstance = 5,
	eCompeteInstance=6, //竞技场
	eClanBattle=7, //帮派战
	eTreasonKing = 8, //剿灭反王
	eProtectKing=9, //守护秦王
	eBeyondSvrBattle=10, //夸服战
	eVipInstance = 11, //VIP副本
	eMarryInstance = 12, //marry副本
	eCountryBoss = 13, //阵营boss战副本
	eCountryWar  = 14, //阵营战
	ePVP=15,							//斗战重楼
	eMoneyInstance=16, //铜钱副本
	eGuideInstance=17, //指引副本
	eWorldBossInstance=18,	//世界BOSS副本1
	eSeabeachInstance=19,		//海滩
	eIntegralInstance = 20, //积分副本
	eClanRobber = 21, 				//帮派强盗
	eEliteInstance = 22, //精英副本
	eCauchemarInstance = 23, //恶梦副本
	eHeroInstance = 24, //英雄副本
	eAdvancedInstance = 25,
	eExpInstance = 26,
	eMoneyInstance1 = 27,
	eCountryOverlord = 28, //阵营霸主争夺战
	eInstanceMax,
};

//活跃类型
enum EVitalityType
{
	eViSignIn = 1,				//每日签到
	eViLogin,							//微端登录
	eViHorseUp,						//坐骑升级
	eViDanLuUp,						//丹炉进阶
	eViMWUp,								//神兵进阶

	eViDanLuSmelt,				//丹炉熔炼
	eViMallShop,					//商城购物
	eViEquipStrengthen,	//装备强化
	eViEquipUp,						//装备升级
	eViEquipSucceed,			//装备继承

	eViAcupointUp,				//升级经脉
	eViSkillUp,						//技能升级
	eViDailyQuest,				//日常任务
	eViClanQuest,					//帮派任务
	eViPersonalInstance,//个人副本

	eViGroupInstance,		//组队副本
	eViThreatInstance,	//挑战副本
	eViProtectKing,			//守卫秦王
	eViTreasonKing, 			//剿灭反王
	eViBoss,								//世界BOSS

	eViEscort,							//护送皇纲
	eViCompeteInstance,	//竞技场
	eViBeyondSvrBattle,	//夸服竞技场
	eViGirlWarLingering,//武姬缠绵
	eViLucklyTurntable,	//幸运大转盘

	eViChatWorld,					//世界发言
	eViCJobItem,					//官运科举
	eViAnswer,							//答题活动
	eViMoneyShop,					//钱庄
};


enum ESysSettingType
{
	eStPlayer = 1001,     					//角色属性
	eStPackage = 1002,	 					//背包
	eEquipStrengtLv = 1003, 				//强化
	eEquipInlayLv = 1004,					//镶嵌
	eEquipRefineLv = 1005, 					//洗练
	eEquipUpgradeLv = 1006, 				//升级
	eStHorse	= 1101,							//坐骑
	eStHorseAdvance = 1102,					//坐骑进阶
	eStHorseSkin = 1103,						//坐骑皮肤
	eStMagicWeapon = 1201,					//神兵
	eStMagicWeaponAdvance = 1202,			//神兵进阶
	eStMagicWeaponPassive = 1203,			//神兵被动
	eStSmagic = 1301,							//神器
	eStSmagicAdvance = 1302,				//神器进阶
	eStDanLu = 1401,							//丹道
	eStDanLuAdvance = 1402,					//丹道进阶
	eStDanLuSmelting = 1403,				//丹道熔炼
	eStHeartMagic = 1501, 					//心法
	eStHeartMagicAdvance = 1501, 			//心法进阶
	eStSkill = 1601,							//人物技能
	eStQuest = 2001,							//任务
	eStDailyQuest = 2002,					//日常任务
	eStClanTaskOpenLv = 2003,				// 帮派任务
	eStCntyTaskOpenLv = 2004, 				// 阵营任务
	eStTaskWeek		= 2005, 					//周任务
	eStVip = 2101,								//VIP
	eStFriend = 2301,							//好友
	eStAch = 1601,								//成就
	eStMall = 3001,							//商城
	eStClan = 3101,							//帮派
	eStCountry = 3201,						//阵营
	eStBank = 3301,							//钱庄
	eStCarryShop = 3401,						//随身商店
	eStMysteryShop = 3402,					//神秘商店
	eStClanShop = 3403,						//帮派商店
	eStCampShop = 3404,						//阵营商店
	eStCreditShop = 3405,					//竞技场商店
	eStHonorShop = 3406, 					//战场商店
	eStMaterialSynthLv = 3501,				//合成
	eStGroup = 3601,							//队伍
	eStWorldBoss = 3701, 					//世界Boss
	eStWildBoss = 3702, 						//野外BOSS
	eStAnswer = 3703,							//答题
	eStBattleParis = 3707, 					//斗战重楼
	eStEscort = 3709,							//皇岗
	eStExpCalabash = 3801, 					// 经验葫芦
	eStSingleInstanceLv = 3901, 			// 单人副本
	eStEliteInstLv = 3902, 					// 精英副本
	eStCauchemarInstLv = 3903, 			// 噩梦副本
	eStChallengeInstanceLv = 3904,		//碎片副本
	eStExpInstLv = 3905, 					// 经验副本
	eStMoneyInstOpenLv = 3906, 			// 铜钱副本
	eStAdvanceInstOpenLv = 3907,			//进阶副本
	eStVipInstanceLv = 3908, 				// VIP副本
	eStGroupInstanceLv = 4001, 			// 组队副本
	eStMeridians = 42001,					//经脉
	eStChatGeneralLv = 4301,				//普通
	eStChatClanLv = 4302,					//帮派聊天
	eStChatTeamLv = 4304,					//队伍聊天
	eStChatWorldLv = 4306,					//世界聊天
	eStChatPrivateLv = 4307,				//私聊
	eStGirlWar = 4401,						//战姬
	eStVitality = 4501,						//活跃度
	eStLucklyTurntable = 4601,				//幸运转盘
	eStCompeteInstance = 4701,				//竞技场
	eStBeyondSvrBattle = 4702,				//括服竞技
	eStGameObject = 4801,					//游戏目标
	eStChat = 4901,							//喇叭
	eStRecharge = 5001,						//首充
	eStRewardHall = 5100,					//奖励大厅
	eStLevelRewardLv = 5101,				//等级礼包
	eStActiviteCode = 5102,					//激活码
	eOfflineRewardLv = 5103, 				// 离线奖励
	eStSevenRewardLv = 5104,				//七日礼包
	eStSignLv = 5105, 						//签到
	eStOnlineRewardLv = 5106, 				//在线奖励
	eStResourceRecoverLv = 5107, 			//资源追回
	eStBingStrong = 5201,					//我要变强
};

//enum ESysSettingType
//{
//	eStPlayer = 10,			//人物
//	eStPackage = 11,			//包裹
//	eStSkill = 12,				//技能
//	eStCJobItem = 13,		//官运
//	eStHorse	= 14,				//坐骑
//	eStAch = 15,					//成就
//	eStForging = 16,			//锻造
//	eStMall = 17,					//商城
//	eStClan = 18,					//帮派
//	eStFriend = 19,			//好友
//	eStGroup = 20,				//队伍
//	eStQuest = 21,				//任务
//	eStVip = 22,					//VIP
//	eStSevenDays = 23,	//七日礼包
//	eStRewardHall = 24,		//奖励大厅
//	eStBank = 25,						//钱庄
//	eStInstance = 26,			//副本
//	eStDekaron = 27,				//挑战
//	eStLucklyTurntable = 28,	//幸运转盘
//	eStCompeteInstance = 29,	//竞技场
//	eStBr	= 30,				//霸业
//	eStDailyTodo = 31,		//每日必做
//	eStGirlWar = 32,				//战姬
//	eStGameObject = 33,		//游戏目标
//	eStBeyondSvrBattle = 34,	//括服竞技
//	eStChat = 36,			//喇叭
//	eStActivities = 37,		//活动大厅
//	eStMeditation = 39,	//打坐
//	eCountry = 40,//阵营
//	eStMarket = 41, //市场
//	eStVitality = 44,//活跃度
//	eStFashion = 45,//时装
//	eStMeridians = 46,//经脉
//	eStMagicWeapon = 47,//神兵
//	eStDanLu = 48,//丹道
//	eStVIP = 49,//VIP
//	eStWedding = 50,//结婚
//	eEscort = 51,//皇岗
//	eDailyQuest = 52,//日常任务
//	eCountryResourceWar = 53,//阵营资源站
//	eCountryBossWar = 54,//阵营BOSS战
//	eAnswer = 55,//答题
//	eChatClanLv = 56,//帮派聊天
//	eChatWorldLv = 57,//世界聊天
//	eChatTeamLv = 58,//队伍聊天
//	eChatPrivateLv = 59,//私聊
//	eChatGeneralLv = 60,//普通
//	eHeavenChest = 62, //天降宝箱
//	eBattleParis = 63, //斗战重楼
//	eExpCalabash = 65, // 经验葫芦
//	eTroubledTimesLv = 67, // 乱世枭雄
//	eStSMagic = 106,  	//法器
//	eStHeartMagic = 107,  //心法
//	eAdvencedLv = 108, // 进阶副本
//	eEquipStrengtLv = 160, // 强化
//	eEquipInlayLv = 161, // 镶嵌
//	eMaterialSynthLv = 162, // 合成
//	eEquipRefineLv = 163, // 洗练
//	eEquipUpgradeLv = 164, // 升级
//	eEquipSucceedLv = 165, // 继承
//	eClanTaskOpenLv = 212, // 帮派任务
//	eCntyTaskOpenLv = 213, // 阵营任务
//	eTaskWeek		= 214, //周任务
//	eSevenRewardLv = 230, // 七日礼包
//	eSignLv = 240, // 签到
//	eOnlineRewardLv = 241, // 在线奖励
//	eLevelRewardLv = 242, // 等级奖励
//	eSingleInstanceLv = 260, // 单人副本
//	eGroupInstanceLv = 261, // 组队副本
//	eChallengeInstanceLv = 262, // 挑战副本
//	eEliteInstLv = 263, // 精英副本
//	eCauchemarInstLv = 264, // 噩梦副本
//	eWildBoss = 270, // 野外BOSS
//	eProtectKingLv = 271, // 守护秦王
//	eTreasonKingLv = 272, // 剿灭反王
//	eEscortLv = 273, // 护送黄纲
//	eResourceRecoverLv = 312, // 资源追回
//	eOfflineRewardLv = 313, // 离线奖励
//	egourdExp		 = 400,//经验葫芦
//	eMoneyInstOpenLv = 401, // 铜钱副本
//	eVipInstanceLv = 402, // VIP副本
//	eWorldBoss = 403, // 世界Boss
//};

enum eDayInfoUpade
{
	eDayInfoUpade_Instance=1,			//副本
	eDayInfoUpade_TREASON_KING=2,//剿灭反王
	eDayInfoUpade_PROTECT_KING=3,//守护秦王
	eDayInfoUpade_ESCORT=4,				//皇岗
	eDayInfoUpade_LuckTable=5,		//转盘
	eDayInfoUpade_MoneyShop=6,		//钱庄
	eDayInfoUpade_Complete=7,			//竞技场次数
	eDayInfoUpade_CampShop=8,			//阵营商店物品
	eDayInfoUpade_YellowVip=9,		//黄钻领奖
	eDayInfoUpade_Benefits=10,	  //福利大厅(包含在线奖励签到资源等等)
	eDayInfoUpade_RedBag=11,		  //阵营红包领取次数
	eDayInfoUpade_Active=12,	  	 //活动大厅
	eDayInfoUpade_Clan=13,					 //帮派每日奖励及商店物品
	eDayInfoUpade_Recharge=14,		 //元宝投资-七日返利   每日首充  每日累计充值
	eDayInfoUpade_VIP_Consecutive_charge=15,	//vip连续充值
	eDayInfoUpade_CompleteGift=16,		//竞技场礼包更新
	eDayInfoUpade_Beyond_batter=17,	//夸服竞技次数及奖励
	eDayInfoUpade_Vitality=18,				//活跃度
	eDayInfoUpade_BaseShop = 19,			//商店
};

enum ActivityExType
{
	ActivityType_FirstRecharge=0,//开服首冲
	ActivityType_Ranking,//开服冲版
	ActivityType_GoldenInvest,//元宝投资
	ActivityType_DailyRecharge,//每日冲值
	ActivityType_SingleRecharge,//单笔冲值
	ActivityType_AccRecharge,//累积冲值
	ActivityType_DailyFirstRecharge,//每日首冲
	ActivityType_MagicWeaponAdvance,//神兵升阶
	ActivityType_HorseAdvance,//坐骑升阶
	ActivityType_RelayRecharge,//接力冲值
	ActivityType_Festival=10, //端午节活动
	ActivityType_Compensation,//补偿活动
	ActivityType_GoldenInvestLv,//元宝等级投资
	ActivityType_GoldenInvestSevenDays,//元宝七日返利
	ActivityType_FullService,//合服补尝
	ActivityType_AccDayRecharge=16,//累积冲值天数活动
	ActivityType_AccConsume=17,//累积消费活动
};

enum ActivityRanking
{
	Ranking_Fight,//开服冲版战斗力版
	Ranking_Lv,//开服冲版等级版
	Ranking_Horse,//开服冲版坐骑版
	Ranking_MagicWeapon,//开服冲版神兵版
	Ranking_DanLu,//开服冲版胆道版
};

enum EClientType
{
	eClientType_WebPage = 1, // 网页端
	eClientType_Micro = 2, // 微端
};

enum E_ACCOUNT_TYPE
{
	eAccountType_User = 1, // 用户玩家
	eAccountType_ArenaRobot = 2, // 竞技场机器人
	eAccountType_TestRobot = 3, // 外部测试机器人
};

enum E_ARENA_CHALLENGETYPE
{ // 竞技场挑战对象类型
	eArenaChallenge_Real = 0, // 真实
	eArenaChallenge_Simulation = 1, // 虚拟
};

//****************************************************************//
const int ServerResetHour = 0; //服务器 在 多少点 重置
const int ServerResetMin   = 1;  //服务器 在 0点 多少分 重置

//-------------------------------------------//
const int Char_NoClanIcon_ID = -1;  //代表 玩家 没有 帮派图标值

#endif /* CHARDEFINE_H_ */
