/*
 * PropDefine.h
 *
 *  Created on: 2014年4月8日
 *      Author: helh
 */

#ifndef PROPDEFINE_H_
#define PROPDEFINE_H_

// 道具的标识类型
enum ePropFlag
{
	ePropBind    = 1,      // 是否绑定标识
	ePropLight   = 1 << 2, // 是否开光过
	ePropCheckup = 1 << 3, // 是否已经鉴定
	ePropUseBind = 1 << 4, // 是否是使用绑定
};

// 绑定方式
enum ePropBindType
{
	eNullBind=0,		// 不绑定
	eGetBind,		// 得到后绑定
	eUseBind,		// 使用(装备)绑定
	eMaxBind,
};

// 物品类型 : 消耗类、武器类，防具类，首饰类，强化类，洗练类，限时类, 任务类
enum eGoodsType
{
	eConsume=1,		//消耗类
	eWeapon,		// 武器类
	eArmor,		// 防具类
	eJewelry,   //首饰类
	eStrengthen,   //强化类
	eSuccinct,   //洗练类
	eTime, //限时类
	eTask,  //任务类
	eStone,//宝石类
	eAdded, //合成类
};

//宝石类型
enum EStoneType
{
	eHPStone=1,//生命
	eMPStone =2,//魔法
	ePhyATTStone=3,//物工
	ePhyDefStone =4,//物防
	eMagATTStone =5,//法攻
	eMagDefStone =6,//法防
	eHitStone =7,//命中
	eAvoidStone =8,//闪避
	eCrackStone = 9,//爆击
	eAdCrackStone =10,//抗暴
	eLuckyStone = 11,//幸运值
	eReagainStone =12,//回复
};

// 物品品质
enum eQuality
{
	eGoodsQualityWhite = 0,						//   = 白色
	eGoodsQualityGreen,							//   = 绿色
	eGoodsQualityBlue,							//   = 蓝色
	eGoodsQualityPurple,							//   = 紫色
	eGoodsQualityOrange,							//   = 橙色
	eGoodsQualityRed,								//   = 红色
};

//时效类型:   使用后消失，永久性道具，获得开启计时，使用开启计时
enum eTimeType
{
	eUseDisappear=0,		// 使用后消失
	ePermanent,		// 永久性道具
	eGetOntime,		// 获得开启计时
	eUseOntime,		// 使用开启计时
};

// 使用类型 :  绑定金币，双倍经验，双倍金钱，山寨道具，传送道具，喊话道具，强化道具，洗练道具，保护道具，声望类道具,BUFF类
enum eEffect
{
	eBindGold = 1,						//  1	绑定金币
	eDoubleExp,								//  2 双倍经验
	eDoubleMry,								//  3 双倍金钱
	eCottageProp,							//  4 山寨道具
	eSendProp,									//  5	传送道具
	eMegaphoneProp,					//  6 喊话道具
	eStrengthenProp,					//  7 强化道具
	eSuccinctProp,						//  8 洗练道具
	eProtectProp,							//  9 保护道具
	ePrestigeProp,						//  10 声望类道具
	eBuffProp,             //	 11 BUFF类
	eHorseProp, 							//	12	坐骑类
	eGiftProp, 								//	13	礼包类
	eTaskProp,									//	14	任务类型
	eDecreaseSinProp,				//	15	免罪令
	eUpLvProp,									//	16	升级石头类型
	eSucceedProp,							//	17	继承石头类型
	eMallProp,									//	18 商城道具
	eGirlProp,									//	19	获得战姬道具
	eHorseSkinProp,					//	20	获得坐骑皮肤道具
	eGiftSProp,								// 21 概率礼包类
	eFashions,									//	22	时装
	eQuantityProp,						// 23 时装碎片
	eCountryProp,							//24 阵营
	eExpProp,									//25 经验道具
	eForceProp,								//26 真气道具
	eGoldenProp,							//27 元宝道具
	eVipCardProp,          //28 vip体验卡
	eBoxProp,									//29 天降宝箱
	eCountryBossBox,         //30 阵营BOSS战宝箱
	eTitleProp = 32,						//32称号物品
};

//货币类型 :    钻石，绑定钻石，金币
enum eMryType
{
	eDiamond=1,		// 钻石
	eBindDiamond,		// 绑定钻石
	eGold,		// 金币
};

//使用CD频道:    恢复hp，恢复mp，持续增加属性
enum eTelCD
{
	eRestoreHp=11,		// 恢复hp
	eRestoreMp=12,		// 恢复mp
	eIncreasingproperty=13,		// 持续增加属性
	eTelCDmax,  //预留
};

//装备穿戴职业 :   通用，猛将，方士，飞羽
enum eWearJob
{
	eWearUniversal=0,		// 通用
	eReggie,				// 猛将
	eAlchemist,			// 方士
	eFeathers,  		// 飞羽
};

//性别 :   通用，男，女
enum eSex
{
	eSexUniversal=0,		// 通用
	eMale,				// 男
	eFemale,				// 女
};

//套装条件:   2件，4件，6件
enum eSuitrnt
{
	eItemTwo=0,				// 2件
	eItemFour,				// 4件
	eItemSix,				// 6件
};

//套装属性类型 ： HP，MP，物攻，法攻，物防，法防，命中，闪避，暴击
enum eSuitptyType
{
	eSuitptyHp=1,				// HP
	eSuitptyMp,					// MP
	eSuitptyPhyAck,			// 物攻
	eSuitptyMagAck,			// 法攻
	eSuitptyPhyDef,			// 物防
	eSuitptyMagDef,			// 法防
	eSuitptyHit,				// 命中
	eSuitptyDodge,				// 闪避
	eSuitptyCrit,				// 暴击
};

//装备部位  :  神石，时装，武器，头盔，护甲，腰带，战靴，护手，护肩，项链，戒指，勋章
enum eItemPlace
{
	eItemPlaceJinseki=1,				// 神石
	eItemPlaceFashion,				// 时装
	eItemPlaceWeapon,					// 武器
	eItemPlaceHelmet,					// 头盔
	eItemPlaceArmor,					// 护甲
	eItemPlaceBelt,					// 腰带
	eItemPlaceBoots,					// 战靴
	eItemPlaceGauntlets,				// 护手
	eItemPlaceShoulders,				// 护肩
	eItemPlaceNecklace,				// 项链
	eItemPlaceRing,					// 戒指
	eItemPlaceMedal,					// 勋章
};

//物品使用限制
enum eItemRestrict
{
	eHPRestrict=1,   //回血限制
	eMPRestrict=2,   //回蓝限制
};
#endif /* PROPDEFINE_H_ */
