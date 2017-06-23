/*
 * MessageCommonRet.h
 *
 *  Created on: 2013-9-20
 *      Author: helh
 */

#ifndef MESSAGECOMMONRET_H_
#define MESSAGECOMMONRET_H_

enum ServerRet
{
	eInstanceMapDestory = -110, //副本地图已销毁
	eCountryWarCantEnter = -109, //董墓争夺战退出后无法再次进入
	eCountryBossCantEnter = -108, //董墓BOSS战退出后无法再次进入
	eDontHaveEnvelope = -107,//没有可派发的红包
	eGetEnvelopeTypeError = -106,//领取红包方式错误
	eDontHaveEnvelopeReward = -105,//没有可领取的返利
	eDontHaveEnvelopeLog = -104,//没有红包记录
	eEnvelopeHaveGot = -103,//此红包已经领取过了
	eEnvelopeNotExist = -102,//红包不存在
	eDontHaveGetEnvelopCount = -101,//可领取红包次数不足
	eDontHaveEnvelop = -100,//没有任何阵营红包
	eNotOpen = -99, //没有开启
	eExchangeCodeExchangeGift = -98,//该用户已兑换过该批次里的其他礼包码
	eExchangeCodeUsed = -97,//兑换码已使用
	eExchangeCodeCannotUse = -96,//兑换码不能用在此服务器
	eExchangeCodeNotExist = -95,//兑换码不存在
	eExchangeCodeOutOfDate = -94,//兑换码过期
	eExchangeCodeError = -93,//兑换码错误
	eCJobEqual			= -92,			//官印类型已经存在
	eDiffEquipType			= -91,		//装备类型不同
	eWillLoginTimeout  = -90,   //未找到将要登录的对应帐号
	eSucceeStoneEnough = -89,	//继承宝石不足
	eStrengNull = -88,	//强化等级不够
	ePurpleStoneNull = -87,			//紫水晶不足
	eUpGradeStoneEnough	= -86,	//升级宝石不足
	eEquipQualityEnough = -85,	//装备品质不够
	eEquipLvFull = -84,		//装备已经满级
	eSignDayNotToday     = -83,     //签到日期与当前日期不符
	eMagicSkillNotFire   = -82,    //神兵技能不能释放
	eAccountInLockTime    = -81,      //帐号处于被封期间  无法登录
	eNotFindThisCharID   = -80,    //没有找到 此玩家相关信息
	eClientSendDataError = -79,   //客户端发送数据错误
	eItemNotWear = -78, //此物品不能穿戴
	eItemNotEat = -77, //此物品不能吞噬
	eSectorIsnNull=-76, //目标 格子非空
	eOpenPackageFull=-75, //开格子开满了
	eBeginIsNull = -74, //起始格子为空
	eContriIsnEnough = -73, //功勋不够
	eExchangedIDError=-72, //功勋兑换id错误
	eCJobPackageNull = -71, //官印背包为空
	ePackageIsFull = -70, //背包满了
	eQualityNotSale=-69, //此品质官印 不能卖
	eMoneyNotEnough=-68, //金钱不够
	eCJobPackageFull=-67, //科举背包已满
	eCJobIndexIstMatch=-66, //科举序号不匹配
	eRetNpcNotThisFun=-65, //NPC没有此功能
	eMagicSkillIndexBeyond=-64, //神兵技能的刷新序号超出
	eMagicWeaponMaxLv=-63, //神兵最高级了
	eNotHaveMagicWeapon=-62, //没有拥有神兵
	eMoneyShopCostNumNotEnough=-61,  //钱庄兑换次数不足
	eVIPLvNotEnough  = -60,  //VIP等级不足
	eStateNotAttack = -59, //当前状态不能攻击
	eNotEnoughForce = -58, //没有足够真气
	eSkillIsFiring = -57, //有技能正在被释放
	eSexError=-56,            //性别不符合
	ePassiveSkillNotFire=-55, //被动技能不能释放
	eStoneLvLimit = -54, //宝石已达最高级
	eNotEnogGoods	= -53,	//商品不足
	eNotEnogBindGolden	= -52,	//绑定元宝不足
	eNotEnogGolden   = -51,	//元宝不足
	eMallGoodsNull = -50,	//没有该商品
	eSettingsError = -49,	//系统设置错误
	eMailGoodsNull = -48,	//发送物品不存在
	eMailDeMailErr = -47,	//删除邮件错误
	eMailConFull = -46,	//提取邮件物品背包格子不够
	eMailNameNull = -45,	//收件人为空
	eAffirmNull = -44,		//洗练属性为空
	eChatManNull = -43,		//聊天对象不存在	 
	eChatHornNull = -42,	//聊天喇叭不足
	eChatCDTime = -41,		//聊天CD时间没到
	eLockNull = -40,		//没有洗练所
	eStrengthEquipFail = -39, //装备强化失败
	eStoneLvLetterEquipLv = -38, //强化石等级小于装备等级
	eEquipLvLimit = -37, //装备等级满级
	eAffirmError = -36,			//替换属性错误装备位置不一致
	eQualityMatch = -35,		//洗练石和装备品质不匹配
	eEquipUnRefine = -34,		//装备不能洗练
	eRefineStoneNull = -33,	//洗练石为空
	eCreateStoneFail = -32, //宝石合成失败
	eStoneNumLimit = -31, //宝石数量不够
	eDiffStoneLV = -30, //宝石等级不同
	eDiffStone = -29, //宝石类型不同
	ePlayerPackageFail=-28, //背包数据异常
	eInlayedStoneNull=-27, //镶嵌的宝石为空
	eItemCountNull=-26, //物品数目为0
	eHoleFull = -25, //孔满了
	eSameStone = -24, //相同的宝石类型
	eWhiteUnAdd = -23, //白色装备不能镶嵌
	eItemError = -22, // 物品信息错误
	eLVNotEnough = -21, //等级不够
	eCampError = -20, //阵营错误
	ePointDistance = -19, //与点距离过远
	eStartPointError=-18,//传送点错误
	eCreatureDead=-17,//死亡了
	eNoAttack=-16, //当前模式下无法攻击
	eNoLearnSKill=-15,//没学这个技能
	eNotEnoughHP=-14, //没有足够血量
	eNotEnoughMP=-13,//没有足够蓝
	eAttackBeyondDistance=-12, //攻击距离过远
	eAttackSpeedTooFast=-11, //攻击频率过快
	eBattleTargetNull=-10, //攻击目标为空
	ePlayerLogin=-9,  //玩家已经在线
	eBeyondWord=-8,  //超过字数限制
	eHeadError=-7, //头像选择错误
	eMaskWord=-6,  //屏蔽字
	eMapNotExist=-5, //没有找到角色所在的地图
	eRoleExisted=-4, //帐号已经拥有一个角色
	eNameUsed=-3, //角色名已经存在
	eNull=-2, //为空
	eSqlExeError=-1, //sql执行失败
	eRetSuccess=0,  //成功
	eRetZoneNotExist=1, //场景不存在
	eRetNpcNotExist=2, //npc不存在
	eBeyondDistance=3, //距离过远
	eQuestNotExist=4, //任务不存在
	eQuestFinished=5, //任务已经完成
	eQuestNPCNotMatch=6, //任务npc不匹配
	eQuestAccepted=7, //任务已经接取
	eLVNotMatch=8, //等级不匹配
	eConNotMatch=9, //条件不匹配
	eBeTaskNotUpdate=10, //前置任务未完成
	eNotEnoghMoney=11, //金钱不足
	eQuestNoAccepted=12, //任务没有接取
	eQuestNoFinished=13, //任务没有完成
	eSystemError=14,//未知错误
	eQuestMax=15,//任务最大个数
	eSkillIsNull=16, //技能不存在
	eProfessionNotMatch=17, //职业不匹配
	eNotEnoughExp=18, //经验不足
	eNotEnoughFContri=19, //帮贡不足
	eDailyQuestMax = 20,  //日常任务最大个数
	eHaveDailyQuest = 21, //身上已经有日常任务了
	eHaveQuest=22,        //身上已经有该任务了
	eGridNotOpen=23,      //格子还没开启
	eGoodsNotPile=24,      //物品不能堆叠
	eContainerFull=25,    //容器已满
	eSimSkillNotUp, //普通技能不能升级
	eHighestLv, //最高等级了
	eUnableQuickQuest,
	ePackageFull=29, //背包已经满了
	eCanPutDragSar=30, //不能托放到快捷栏
	eDragSarHaveSame=31, //快捷栏有相同物品了
	eNotHaveHorse = 32,//没有拥有这个坐骑
	eSameHorse=33, //换的是相同坐骑
	eNotOnHorse = 34,//没有骑着坐骑
	eNotFindItem=35, //没有发现这个物品
	eNotEnoughGold=36, //没有足够的元宝
	eReachMaxHP=37, //已经达到最大血蓝
	eReachMaxMP=38, //已经达到最大魔
	eUseGoodsCdLimit=39, //使用物品过于频繁
	eCant_find_item= 40,			//Npc找不到该物品
	eItemAlreadySold = 41,		//该物品已买完
	eSellerDontLikeYou = 42,	//npc或者玩家不存在
	eCantSellItem=43,				//不能出售此物品
	eBagEmpty=44,					//背包为空
	eYouDontOwnItem=45,			//你不拥有此物品
	eBagPosError=46,			//背包位置不对
	eBuyBackItemEmpty = 47,	//回购物品为空
	eHaveFrinedInGood=48, //好友列表已有
	eHaveFrinedInEnemy=49, //仇人列表已有
	eHaveFrinedInBlack=50, //黑名单已有
	eNotHaveOnline=51,     //玩家不在线
	eLimitFriendMax=52, //好友列表已达30
	eExchangeOtherBusy=53,	//被申请方忙
	eExchangeWithMore=54,	//不能和多人交易
	eOutOfRange		=55,		//距离在范围之外
	eNotInSameMap	=56,		//不在同一张地图
	eExchangeItemCanotFind=57,	//找不到交易物品
	eExchangeApplicantChanged=58,//交易目标发生变化
	eExchangeOtherVerify	=59,	//对方已确认交易，这个不算错误码
	ePlayersBusy		=60,		//玩家忙
	ePlayerIsNotLock	=61,		//玩家未锁定
	eNotWithMe			=62,		//不能与自己交易
	eGroupIsNotExistence = 63,	//队伍不存在
	eNotLeaderNoCompetence = 64,//不是队长，没有权限
	eGroupIsExistence = 65,	//队伍已存在
	eGroupIsFull		= 66, //队伍已满
	eInvitePlayerError = 67, //邀请玩家失败
	eNotGroupMember	 = 68,//不是队伍成员
	eLeaderNotKickMe	 = 69, //队长不能踢出自己
	eChangePkType	 = 70, //不是中立地图无法切换模式
	eCantAttackByPeace	 = 71, //当前处于和平状态下,无法进行攻击
	eCantAttackSameTeam	 = 72, //无法攻击队友
	eExchangeCDNotReady = 74, //交易冷却时间没到
	eDontOpenPerAcupoint = 75, //前一个穴位还未开启
	eNotEnoghForce = 76, //真气不足
	eOpenAcupointFail = 77, //打开穴位失败
	eStrengthNotFull = 78, //体力不足
	eInInstanceSweep = 79, //当前正在扫荡中
	eNotPassInstance = 80, //副本还未通关
	eInstanceSweepTimeLimit = 81, //扫荡达到上限
	eInstanceInfoError = 82, //副本数据异常
	eInstanceNotExsitence = 83,	//副本不存在
	eInstanceTimesNotEnough = 84,	//副本次数已满
	eIntancePlayersIsFull = 85,	//副本人数已满
	eInstanceNotGroup			=86,	//参加组队副本，但是没有队伍
	eInInstanceMap				= 87,	//处于副本地图中
	eRewardItemError = 88, //奖励物品信息错误
	ePrestigeInfoError = 89, //成就信息错误
	ePrestigeIdError	= 90,	//请求的成就ID错误
	eCantAttackSameCountry	 = 91, //无法攻击同国家的人
	eNotScore = 92,	//积分不足
	eNotFinish = 93,	//活动未完成
	eReceived = 94,	//已经领取
	ePrestigeHadGet = 95,	//已经领取成就奖励
	ePrestigeIDError	= 96,	//成就ID错误
	ePlayerTitleHadGet = 97,	//已经领取称号奖励
	ePlayerTitleNotGet = 98,	//没有获得称号
	ePlayerTitleIDERROR = 99,	//称号ID错误
	eInstanceNotUse	  = 100, //副本不允许使用此物品
	ePlayerNotInviteToTeam = 101,	//玩家不接受邀请组队
	ePlayerNotExchange	= 102,	//玩家不接受邀请交易
	eGatherFailed			= 103, //采集失败
	eGatherIdError		= 104,	//不能采集
	eGroupPlayerLvToLower = 105,	//玩家加入副本队伍等级太低
	eNotFriend = 106,//不能添加好友
	eGoodsTypeError       = 107, //物品类型出错
	eLVNotEnoughToAttack = 108,
	eNotGetReward = 109, //未达到领取条件
	eHaveGetReward = 110, //已经领取过奖励了
	eHaveSignIn = 111, //今天已经签到过了
	eMapIdError = 112,  //地图ID错误
	eSplitItemTooMuch = 113, //拆分数量超过了物品总数量
	eRewardHailConfigError = 114, //奖励大厅配置表出错,没有该项奖励
	eCantAttackMyself=115, //无法攻击自己
	eCantDiscard = 116,
	eEscortFailed = 117,	//运镖失败
	eEscortTimesOver = 118, //运镖次数用完
	eIsEscorting		= 119, //运镖中。。。。。
	eCantDiscardTaskGoods = 120, //任务物品无法丢弃
	eHaveNotTaskGoods = 121, //任务完成消耗物品不足
	eEscortChangeMapCarTooFar = 122, //跳转地图时镖车离玩家太远
	eCantAttackMyselfEscort = 123,	//不能攻击自己的镖车
	eCantAttackTeamEscort	= 124,	//不能攻击同队伍的镖车
	eNotInEscorting			= 125,	//不在运镖中
	eEscortIsSpeedingUP		= 126,	//镖车处于加速状态中
	eIsInMeditation			= 127,	//处于打坐中
	eNOTInMeditation			= 128,	//不在打坐中不能取消打坐
	eNotMeditationInSomeState = 129,	//某些状态下不能打坐
	eEscortNotIntoInstance	= 130,	//运镖不能进入副本
	eVipLevelNotEnough		= 131,	//VIP等级不够
	eVipGiftHadGot				= 132,	//VIP奖励已领取
	eNotVipRight				= 133,	//没有此VIP权限
	eAnswerActivityNotStart = 134,	//答题活动没有开始
	eClanIconError=135, //帮派图标错误
	eClanRepeatedName=136, //帮派名字重复了
	eAlreadyHasClan=137, //已经拥有帮派了
	eClanIstExisted=138,//帮派不存在
	eNameTooLong=139, //名字太长了
	eCountryNotEqual=140, //势力不同
	eClanIsFull = 141, //帮派人数满了
	eAlreadyRequestClan=142, //已经申请帮派了
	eClanPackageFull=143, //帮派仓库满了
	eNotClanBoss=144, //你不是帮主
	eNotClanJob = 145, //没有职位
	eClanPowerNotEnough=146, //权力不够
	eClanJobNotSelf=147, //帮派职位不对应
	eJobHasPlayer=148, //此职位已经有人了
	ePlayerClanNotExisted=149, //此人无帮派
	eRequestListNotPlayer=150, //申请列表中无此人
	eAskListHasPlayer=151, //邀请列表已经存在此人了
	eInstanceTypeError = 152, //副本类型错误
	eCDInTime = 153, //cd时间中
	eHasInstance = 154, //你已经创建了一个副本了
	eInstanceCreating=155,//地图已经在创建了
	eInstanceExisted=156,//场景已经存在
	eRankingIsNull=157,//排名为空
	eCantFightThisRanking=158, //没法挑战这个排名的玩家
	eRankingRewardError = 159, //暂无奖励可领
	eHaveClanTask					= 160,		//身上已有帮派任务
	eNotClanTask					= 161,		//接取帮派任 务，但是任务ID的不是帮派任务
	eNotDailyTask					= 162,		//接取日常任务，但是任务ID的不是日常任务
	eTaskCreateFailed		= 163,		// 任务创建失败
	eAllMainTaskHadComp = 164,		//所有主线任务完成
	eMainTaskNotDiscard = 165,		//主线任务不能放弃
	eCompeteInstanceCounterNotUse = 166,		//次数还没有使用
	eBindIDRealNameError = 167,//认证真实姓名有误
	eBindIDError = 168,//认证身份证有误
	eBindIDUnderAge = 169,//未成年人绑定身份证成功
	eBattleIsNotExisted = 170,//战场未准备好
	eNotYourClanBattle = 171,//不是您帮派的战场
	eBattlePlayerFull = 172, //战场玩家满了
	eClanWarEnterInstancePastTime = 173,		//进入帮派战副本超时
	eClanCityNotExisted = 174, //请加入一个帮派。
	eClanCityRewardNotExisted = 175,//没有占领城池，无奖励。
	eClanCityRewardIsHave   = 176, //奖励已领取
	eClanPaidPast = 177, //竞拍时间已过，停止竞拍。
	eClanPaidNotBoss = 178, //只有帮主才能竞拍。
	eClanDefendError = 179, //防守方不能参加竞拍。
	eClanPaidLvNotEnough = 180, //竞拍等级不够。
	eClanWarEnterInstanceNotReady = 181, //进入副本时间还未到
	eAcupointFull 				= 182,		//穴位满了
	eDailyTimesFull = 183,		//今日日常任务已满
	eClanTimesFull = 184,			//今日帮派任务已满
	eClanLevelNotEnough		= 185,	//角色等级不足
	eCityNotExisted = 186, //城池不存在
	eClanWarEnterError = 187, //你的战斗力排名不足
	eSinIsZero = 188,//罪恶值为零不需使用免罪令
	eCityNotPaid = 189, //此城池没有竞拍
	eTaskTypeError = 190, //任务类型错误
	eIntanceAtLeastReadyTwo = 191,	//组队副本，至少需要两个已准备的玩家
	eIntanceMemberDead = 192,	//玩家处于死忙状态
	eIntanceMemberNotReady = 193, //踢除未准备的玩家并进入副本
	eAnswerActivityEnd	= 194,	//答题活动结束
	eTaskItemAddressError = 195,	//任务物品不在指定地点使用
	eClanNotAskCity = 196, //帮派未申请此城池
	eTreasonKingDigEnough = 197,  //最大5次挖宝机会
	eGroupOnlyLeader = 198, 	//队伍只有队长一人
	eTreasonKingLevelError = 199,  //通关层数错误
	eBossNotDead=200,	//boss没有杀死
	eBossCanNotFind=201,	//找不到此boss
	eRandomArticleError = 202, //随机物品错误
	eDestinationNotExist = 203,//游戏目标不存在
	eDestinationNotComplete = 204,//游戏目标未完成
	eDestinationHasGotted = 205,//游戏目标奖励已被领取
	eInstanceHighestNode=206, //关卡已经到达最大关数
	eProtectKingCanNotReset = 207, //淌未创关，不能得置
	eProtectKingGetRewardlevelsError = 208, //获取奖励关数错误
	eProtectKingRewardError = 209, //获取奖励内容错误
	eCanNotDanluEquip   = 210,	//不能炉练出物品
	eAlHaveDanluEquipNotGet  = 211,  //已经 有 炉练出的 物品没 领取
	eTreasonKingMapError = 212,  //反王地图错误
	eTreasonKingCanNotInstance = 213,  //反王不能创建副本
	eServerVeryHot   = 214,  //服务器 繁忙
	eDailyPracticeRwardHaveGotted = 215,//每日必做，闭关修炼奖励已领取了
	eGetDailyPracticeRwardTypeError = 216,//每日必做，闭关修炼奖励领取方式错误
	eAlreadyMaxHorse      = 217,//已经是 最大坐骑
	eCantFindDailyId      = 218,//没有此类型的每日必做
	eDontHaveRcoverReward = 219,//没有可追回的资源
	ePetNotExist  = 220,	//你还没拥有此武姬
	ePetNotFit = 221,	//未达成合体等级
	ePetNotHadFight = 222, //没有战姬出战
	ePetIdError = 223, //战姬ID错误
	ePetIsDead = 224, //战姬已死亡
	ePetIsNotDead = 225,	//战姬没有死
	ePetReviveError = 226, //复活对象错误
	eRecoverResourceTypeError = 227,//追回资源操作类型错误
	ePetAskFit = 228, //请求合体状态
	ePetLvMax = 229, //战姬达到最大等级
	eTaskMaxStart = 230,  //已到达最大星级
	eNotCanMoveToHouse = 231,  //任务物品  不能 移动到 仓库
	eAllDailyTaskHadComp = 232,  //今日所有 日常任务 已完成
	eAllClanTaskHadComp = 233, //今日所有 帮派任务 已完成
	eThisTaskChapOver  = 234,  //本章节 任务  已全部完成 继续前进Go
	eTodayRestOver  = 235,  //今日 重置 数据 完成
	eBuySameHorseSkin = 236,  //购买 已经有 的 坐骑 皮肤
	eEscortQualityError  = 237,  //皇纲品质错误
	eEscortFreshMaxNum = 238, //皇纲刷新次数最大了
	eEscortRobed = 239, //皇纲被劫
	ePracticeVIPLvNotEnough = 240,//领取闭关修炼奖励VIP等级不够
	ePracticeNotEnogGolden = 241,//领取闭关修炼奖励所需的元宝不够
	eInstanceSweepEnd = 242, //扫荡已结束
	eEscortRobBeyondTimes = 243, //皇纲被劫次数大于5次不能攻击
	ePetHadFight = 244, //战姬已出战
	eNoWayJoinGroup = 245,  //队长不同意 加入 队伍
	eNoticeIsExist = 246,  //此公告已存在
	eNoticeNotExist = 247,  //此公告不存在
	eNoticeOverTime = 248,  //此公告超时了
	ePetLvOverPlayerLv = 249, //战姬等级不能大于玩家等级
	eItemNotEnough = 250,		//该物品数量不足
	eFightNotEnough = 251,		//战斗力不足
	eCodeError = 252,		//密码错误
	eTeamInstanceExisted = 253,		//已经拥有组队副本队伍
	eHadInTeamQueue = 254,	//已在创建队伍的队列中
	eCreateTeamFail = 255, //创建副本队伍失败
	eTeamMemberError = 256, //队员信息错误
	eTeamMemberNotReady = 257, //队员没有准备
	eFlopMsgError = 258, //没有此牌号信息
	eFlopHadOwer = 259, //此牌已被翻
	ePetExist	= 260,		//已经拥有此战姬
	eHorseExist	= 261,		//已经拥有此坐骑
	eClanLvNotEnough = 262, //帮派等级不足
	eInTeamInstance				= 263,	//处于副本组队中
	eRedNameCantChangePkType = 264,//红名玩家不可改变PK模式
	eFashionNotEnough = 265,	//时装碎片不足
	eNotFashion	= 266,			//没有此时装
	eFashionExist = 267,		//已经拥有该时装
	eFlopHadDone = 269, //已经翻牌
	eMarketShopingSelfError = 270, //不能购买属于自己的物品
	eMarketShopUnderError = 271, //不能下架别人的物品
	eMarketShopFull = 272, //店铺已满
	eMarketShopUpPriceEnough = 273, //物品价格无效
	eMarketShopBindError = 274, //绑定物品不能出售
	eMarketShopNumNotEnough = 275, //物品数量不足y
	eSQLhavenotPlayerInfoList = 276,//数据库没有符合查询条件的玩家列表
	eSQLhavenotMailList = 277,//数据库没有符合查询条件的邮件列表
	eMarryHadObject = 278, //已拥有对象
	eMarryOtherHadObject = 279, //对方已拥有对象
	eMarrySexError = 280,            //不技持同性结婚
	eMarryTimeOut = 281,		//求复回复已过期
	eMarryObjectOutLine = 282, 	//结婚对象离线
	eMarryRingError = 283, //戒指信息错误
	eMarryTimeOrderError = 284, //没有权限预定此时间段
	eMarryStateError = 285, //结婚状态错误
	eMarryInviterFull = 286, //婚宴邀请人数已满
	eMarryInviterHad = 287, //已经邀请此人
	eMarryCanNotFound = 288, //没有找到婚宴
	eMarryInviterNotExist = 289, //不存在此宾客
	eMarryCelebrationFull = 290, //该特效已用完
	eMarryFireError = 291, //非婚宴主角不能释放特效
	eMarryProcessError = 292, //播放阶段错误
	eMarryNotInviterd = 293, //没有被邀请参加婚宴
	eNotCampTask   		= 294,	  //不是阵营任务
	eHaveCampQuest 		= 295,	  //已有阵营任务
	eAllCampTaskHadComp = 296,    //今日所有 阵营任务 已完成
	eMarryCoupleNotOnlne = 297, //对方不在线
	eWorshipCountUseUp = 298, // 膜拜次数已用完
	eWorshipAlreadyChar = 299, // 已经膜拜过此玩家
	eWorshipCannotSelf = 300, // 不能膜拜自己
	eWorshipOnlyRanking = 301, // 只能膜拜排行榜上面的
	eMarryCeleReady = 302, //对方正在准备
	eMarryInviterOwerError = 303, //婚宴不能邀请主人
	eCantAttackMarry = 304, //结婚地图无法攻击
	eCampIstExisted = 305, //阵营不存在
	eCantAttackSafeZone = 306, //安全区内无法攻击
	eNotEnoughCredit    = 307, //声望值不足
	eNotOneKeyComplete  = 308, //没有一键完成功能
	eCountryBossNotStart = 309, //阵营boss战还未开启
	eCanNotGetEnvelope 		 = 310, //不能领取此红包
	eNotUpgradeProtect 		 = 311, // 没有升级保护符
	eNotSucceedProtect 		 = 312, // 没有继承保护符
	eCountryWarNotStart = 313, //阵营争夺战还未开启
	eActivityNotExist = 314,//活动不存在
	eEquipLvNotMatchCond = 315,//  继承 装备等级不符合 比现有装备等级 相等或低这个条件
	eDontHaveAnyActivity = 316,//没有任何开启中的活动
	eActivityOver = 317,//活动结束
	eDontHaveActivityReward = 318,//没有任何 可领取的活动奖励
	eDontHaveThisActivityReward = 319,//没有这项活动奖励
	eGuideLevelNotEnough 	 = 320, // 引导任务等级不足
	eActivityTypeError = 321, //活动类型错误
	eItemAlreadyHadSold = 322,		//该物品已被其他玩家购买
	eVipLvHigherThanVipCardLv = 323,//VIP等级大于VIP体验卡等级
	eIsAlreadyEscortDiamond = 324,//已经是钻石级别镖车了
	eStrengthEquipDropStar  = 325,//掉星
	eAchievementCountsFull  = 326,//次数已满
	eChatWorldLevelLimite   = 327,//世界聊天等级不足提示
	eChatPraviteLevelLimite = 328,//私人聊天等级不足提示
	eCharNotExist = 329,//此角色不存在
	eCompeteInstanceCounterFull = 330, //购买失败，当前次数不能超过15次
	eEquipNotUpgrade = 331, // 此装备不能升级
	eEquipNotSucceed = 332, // 此装备不能继承
	eWordConvertFailed = 333, // 字符集转换失败
	eRequestTimeOut =  334, //请求应答超时
	eHaveSendMail   =  335, //已发邮件
	eHaveAnswerHaveReceive = 336, //已经领取
	ehaveAnswerNoReward    = 337, //没有奖励
	eSignDayAfterToday = 338,//签到时间大于今天
	eNotAddSelfFriend = 339,//不能加自己好友
	eFriendBlessSendNotEnough = 340,//发送祝福次数不足
	eFriendBlessReceiveNotEnough = 341,//领取祝福不足
	ePackNoSpaceSendMail = 342,//背包空间不足,奖励已经发送到您的邮件，请及时查收!
	eNameCantBeNull = 343,//名字不能为空
	eNameInvalid = 344,//非法的名字
	eMailThemeNull = 345,//主题不能为空
	eMailThemeTooLong = 346,//主题太长
	eMailThemeInvalid= 347,//非法的主题
	eMailSubstanceTooLong = 348,//邮件主题太长
	eMailSendTooManyItem = 349,//发送的物品过多
	eOtherSideLvelNotEnough = 350,//对方等级不足
	eChatPrivateLvTooLow = 351,//聊天对象等级过低
	eItemCantUseInBag = 352,//非消耗类物品不可在背包中使用
	eGatherIdHaveOwer = 353, //已有玩家在采集
	eBeyondSvrTypeError = 354, //跨服战类型错误
	eBeyondSvrLvNotEnough = 355, //跨服等级不足
	eBeyondSvrMatchFaill = 356, //跨服战队列中
	eHaveSkinForever = 357,//已经拥有永久皮肤
	eFestivalCountLimit = 358,//
	eEnterSelfClanCDNot  = 359, //退帮CD未到，不能再加入帮派，自己创建或加入提示
	eEnterOtherClanCDNot = 360, //退帮CD未到，不能再加入帮派，邀请或审核时提示
	eClanHaveAskCity = 361, //帮派已成功申请此城池
	eCantSendEmptyTextMail = 362,//不能发送空内容邮件
	eHaveFullLevelJob = 363,//身上拥有满级官运
	eFullJobCanNotEate = 364,//满级官运不能再吞噬
	eMoneyInstanceLimit = 365,//当前状态不可领取奖励
	eMoneyInstanceHaveGet = 366,//奖励已经领取
	eMoneyInstanceBuyLimit = 367,//本日购买进入副本次数已经达到上限
	eMoneyInstanceTowerBuyLimit = 368, //修罗塔当前等级购买达到上限
	eMoneyInstanceTowerCDLimit = 369,	//修罗塔技能CD未到
	eMoneyInstanceTowerDead = 370,	//修罗塔已经全部被摧毁了
	eMarrySelfNotReachLevel    = 371,//您未达到结婚等级
	eMarryOtherNotReachLevel   = 372,//对方未达到结婚等级
	eMarryHaveSendToThisPlayer = 373,//您已向对方发送结婚请求
	eMarryRefuseTimeNotReach   = 374,//距离该玩家拒绝您的求婚时间还没有到达
	eMarrySelfHaveMarry		   = 375,//您已经结婚
	eMarryOtherHaveMarry	   = 376,//对方已结婚
	eMarryRefuse			   = 377,//对方拒绝您的求婚
	eNotSelfFriend			   = 378,//对方不是您的好友
	eSelfNotFriend			   = 379,//您不是对方的好友
	eMaxStart				   = 380,//已是最大星
	eRingLevelNotEnough		   = 381,//婚戒升阶等级不足
	eMarryDayNotReach		   = 382,//结婚天数没有到达奖励指定领取的天数
	eSelfNotMarry			   = 383,//您还没有结婚
	eSelfHaveSendDivorce	   = 384,//您的离婚操作过于频繁
	eOtherHaveSendDivorce	   = 385,//对方已经发送过离婚请求
	eSelfHaveSendForceDivorce  = 386,//您已经发送过强制离婚请求
	eMarryCohesionOntenough	   = 387,//亲密度不足
	eCanForceDivorce		   = 388,//允许强制离婚
	eMarryMapCanNotChange	   = 389,//对方地图不允许传送
	eMarryNexusCanNotDelete    = 390,//您和该玩家为夫妻关系,不能删除该好友
	eMarryRequestTimeOut	   = 391,//对方结婚请求已超时
	eDivorceRequestTimeOut	   = 392,//对方离婚请求已超时
	eMarryThisPlayerHaveSendYou= 393,//该玩家已向您提出结婚请求
	eConveyDistanceTooNear	   = 394,//距离过近,无法传送
	eTreasonAlreadyReset 	   = 395, // 剿灭反王已重置
	eBugSubmitTypeError = 396,//游戏反馈方式错误
	eThemeNull = 397,//主题不能为空
	eTextNull = 398,//正文不能为空
	eLinkError = 399,//联系方式错误
	eThemeTooLong = 400,//主题太长
	eTextTooLong = 401,//正文太长
	eLinkTooLong = 402,//联系方式太长
	eForceDivorceTimeOut	   = 403,//由于您太久没有操作,强制离婚已起时
	eMailItemNotGot = 404,//还有邮件物品未提取
	eEquipLvNotStrength = 405, // 装备等级不够强化最低装备
	eEquipLvNotInlay = 406, // 装备等级不够镶嵌最低装备等级
	eAreaChallenged = 407, //当前该玩家正被其它玩家挑战中（系统不执行挑战）
	eHaveInApplyList = 408,		//您已经向对方发起过申请好友，请等待对方响应
	eHaveNotApplyFriend  = 409,	//对方没有向您申请过加好友
	eSelfNotOtherFriend = 410,	//您不是对方的好友，需先删除好友关系，重新加对方为好友
	eTargetLimitFriendMax = 411,//对方好友数量已经达到上限
	eNoTeamNotchangePk = 412,		//当前没有队伍，不能切换队伍模式
	eMarryRingLevelNotEnough		   = 413,//婚戒升阶等级不足(这个前端显示是无参数的,381是有参数的)
	eRefreshFail	= 414,	//刷星失败
	eWorldBossNotActive	= 415,	//该世界BOSS活动副本尚未开启
	eRegionNotSwitchPKMode = 416, //该区域不允许切换PK模式	
	eMeditationStateLimit = 417,//当前状态不能邀请双修
	eMeditationNotSameMap = 418,//对方与您不在同一张地图中
	eMeditationTargetStateLimit = 419, //对方状态不允许双修
	eMeditationInInvit = 420,	//对方已经在您的双修邀请列表中，请耐心等待
	eMeditationInvitValid = 421,//邀请已经超时无效
	eMeditationStateChange = 422,//邀请方当前状态改变, 不能与您双修
	eMeditationGirlWarLimit = 423,//当前处于打坐状态，不能对战姬进行出战或休战操作
	eMeditationPosLimit = 424,		 //当前区域不能打坐
	eMeditationDoublePosLimit = 425,	//当前位置不能邀请双修，请到宽敞的地方
	eActivityNotActive = 426,//该活动尚未开启
	eGirlWarNotActive = 427,//请先激活该战姬
	eGirlWarActiveLimit = 428,//激活已达到上限
	eGirlWarActiveNotSwap = 429, //该战姬已经激活，不能进行传功
	eGrilWarHadFightNotSwap = 430, //该战姬已经出战，不能进行传功
	eGrilWarSwapLvLimit = 431,//被传功的战姬等级必须小于传功的战姬！
	eGirlWarActived = 432, //该战姬已激活
	eSeabeachObjectLeave = 433,//很遗憾，已经有人捷足先登了
	eChristmasNotReflushEgg = 434,//不需要刷新
	eChristmasThisEggHaveBreak = 435, //该蛋已砸过
	eChristmasAllEggHaveBreak = 436,//你没有可以被砸的蛋
	eChristmasRewardHaveReceive = 437,//该奖励已经领取
	eCHristmasRewardConditionNotEnough = 438,//领取条件不足
	eChristmasBuyCountsNotEnough = 439,//购买次数不足
	eChristmasItemOntEnough = 440,//竞换所需物品个数不足
	eOtherRefuseInvite		= 441,//对方拒绝您的邀请
	eOtherAlreadyHaveGroup	= 442,//对方已有队伍
	eRefuseOtherInviteGroup = 443,//您拒绝了对方的邀请
	eRefuseYouApplyGroup	= 444,//对方拒绝您的申请
	eRefuseOtherApplyGroup  = 445,//您拒绝对方的申请
	eGroupSuccess			= 446,//组队成功
	eGroupInviteSuccess		= 447,//邀请成功
	eGroupApplySuccess		= 448,//申请成功
	eMoneyTypeError = 449,//货币类型出错
	eDanLuEquipOverAwardStack = 450, // 丹炉锻造奖励超过堆叠数
	eStateNotUseItem		= 451,//你处于不能使用物品状态
	eGirlHaveWare = 452, //已穿戴此装备
	eGirlDataFail = 453, //战姬数据异常
	eGirlPosFull = 454, //该位置已被使用
	eGirlQualityFull = 455, //该战姬品质已满
	eGirlStarFull = 456, //该战姬星阶已满
	eNewYearHaveBuySkin = 457, //你已购买过该皮肤
	eNewYearLoginHaveReceive = 458,//今日登录奖励已领取
	eNewYearContinueHaveRecieve = 459,//连续登录奖励已领取
	eNewYearBuyPersonLimit		= 460,//你个人买购次数已达最大值
	eGirlEquipNotFull = 461,//战姬装备不全
	eGirlStarNotEnough = 462,//战姬星级不足
	eAlreadyHaveThisTitle = 463,//已拥有这个称号
	eOtherInInstanceMap = 464, //对方处于副本地图中
	eJoinCtryAwardNotGet = 465, // 无法获得加入推荐阵营奖励
	eClanContributeLimit = 466, //本日捐献额度达到限制
	eNotThisClanWelfare = 467, // 没有此帮派福利
	eAlreadyTakeClanWelfare = 468, // 已经领取过此福利
	eNotAttainTakeClanWelCon = 469, // 没达到领取帮派福利领取条件
	eSendClanWelfaeConErr = 470, // 发送帮派福利条件错误
	eClanWelfareNotCanNum = 471, // 帮派福利无可领数量
	eClanWelfareFull = 472, // 帮派福利已满
	eClanIntegralStateError = 473, //帮派积分战状态错误
	eClanMemberNotEnough = 474, //帮派成员不足
	eClanMemberLvNotEnough = 475, //帮派满足等级需求成员不足
	eClanIntegralSingupSuccess = 476, // 已报名
	eClanMapCreating	= 477,			//帮派地图正在创建中，请耐心等待
	eCountryNotEqualAskJionClan = 478,			//阵营不一致，不允许邀请
	eClanMoneyNotEnough = 479, //帮派铜钱不足
	eClanFightNotEnough = 480,//帮派战力不足
	eInstanceResetCntEnougth = 481, // 副本重置次数不足
	eCopyMapCannotTransfer  = 482, //不能传送进副本
	eFashionItemNotEnougth = 483, // 时装物品不足
	eFashionStrengthTopLv = 484, // 时装强化等级已满级
	eOrderMultiple = 485, //订单号重复
	eOrderDealing = 486, //订单正在处理中
	eClanBatterHaveSingUp = 487,  // 已经报名了
	eClanBatterNotSingUp = 488,	 // 您所在的帮派没有报名参战
	eClanBatterNotInSingUpTime = 489, //当前时间不允许报名
	eGodTreeFalling = 490,//神树正在洒落
	eGodTreeNotItem = 491,//没有神树物品
	eChallengeExchangeLimit = 492,//未达到通关要求,不能兑换
	eChallengeNotEnough = 493,//晶石不足
	eNotHaveSMagic = 494,		//没有法器
	eNotHaveHeartMagic = 495,	//没有心法
	eInlayNotOpen = 496, //镶钳孔开发等级限制
	eNotEnogCountryContribute = 497, //阵营贡献不足
};

#endif /* MESSAGECOMMONRET_H_ */
