/*
 * MoneyInstanceDefine.h
 *
 *  Created on: 2015年9月14日
 *      Author: root
 */

#ifndef MONEYINSTANCEDEFINE_H_
#define MONEYINSTANCEDEFINE_H_

#define MAX_RANK_NUM (10)
#define MAX_TOWER_LV (9)

enum eMoneyInstanceNoticeType
{
	eNoticeType_NormalMonster = 1, //小怪刷新
	eNoticeType_BossMonster = 2,		 //boss刷新
	eNoticeType_CountDown = 3,			 //倒计时通知
};

enum eMoneyInstanceAttr
{
		eMoneyInstanceAttr_money=1,
		eMoneyInstanceAttr_score=2,
		eMoneyInstanceAttr_maxScore=3,
		eMoneyInstanceAttr_time=4,
		eMoneyInstanceAttr_rate=5,
};

enum eTowerAttr
{
	eTowerAttr_lv=1,			//等级
	eTowerAttr_cool=2,		//冷却时间
	eTowerAttr_attack=3, //攻击
	eTowerAttr_defene=4, //防御
	eTowerAttr_maxCool=5,//最大cd
	eTowerAttr_defeneCount=6,//防御符次数
	eTowerAttr_attackCount=7,//强化符次数
	eTowerAttr_coolCount=8,		//冷却符次数
	eTowerAttr_hp=9,		//血量
};

enum eMoneyInstanceItemType
{
	eMoneyInstanceItemType_defene=0,//钢化
	eMoneyInstanceItemType_impose=1,//强化
	eMoneyInstanceItemType_cool=2,	  //冷却
};

//刷新节点的怪物信息
struct FlushNodeInfo
{
	FlushNodeInfo()
	{
		node = 0;
		id = 0;
		num = 0;
	}
	int node;
	int id;
	int num;
};

//修罗塔信息
struct TowerInfo
{
	TowerInfo()
	{
		lv = 0;
		attack = 0;
		defene = 0;
		hp = 0;
		point = 0;
		defeneUseNum = 0;
		defenePrice = 0;
		defeneValue = 0;
		attackUseNum = 0;
		attackPrice = 0;
		attackValue = 0;
		coolUseNum = 0;
		coolPrice = 0;
		coolValue = 0;
		prizeMoney = 0;
		moneyType1 = 0;
		moneyType2 = 0;
		moneyType3 = 0;
	}
	int lv;//等级
	int attack;//攻击系数
	int defene;//防御系数
	int hp;//生命系数
	int point;//升级积分

	int defeneUseNum;
	int defenePrice;
	int defeneValue;//增加防御百分比
	int moneyType1;

	int attackUseNum;
	int attackPrice;
	int attackValue;//增加攻击百分比
	int moneyType2;

	int coolUseNum;
	int coolPrice;
	int coolValue;//增加冷却值百分比
	int maxCool;
	int moneyType3;

	int prizeMoney;
};

//属性购买的信息
struct AttrBuyInfo
{
	AttrBuyInfo()
	{
		count = 0;
		cost = 0;
		rate = 0;
		moneyType = 0;
	}
	int count;		//次数
	int cost; 		//花费
	int moneyType;
	std::vector<int> attrID;	//属性id
	float rate;  //倍率
};

struct MoneyInstanceConfig
{
	MoneyInstanceConfig()
	{
		maxCount = 0;
		cost = 0;
		moneyType = 0;
		exitTime = 0;
		countDown = 0;
		noticeTime = 0;
		flushTime = 0;
		mapID = 0;
		towerID = 0;
		mailID = 0;
		coolRecove = 0;
	}
	int maxCount; //每天花钱进入最大次数
	int cost;			 //每次进入花费
	int moneyType;//金钱类型
	int exitTime;	//结算后自动退出时间
	int countDown;//进入副本倒计时通知时间
	int noticeTime;//倒计时刷新怪物公告时间间隔
	int flushTime;//公告后刷新怪物时间
	int mapID;			//地图id
	int towerID;		//修罗塔id
	int mailID;		//
	int coolRecove;
};

#endif /* MONEYINSTANCEDEFINE_H_ */
