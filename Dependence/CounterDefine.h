/*
 * CounterDefine.h
 *
 *  Created on: 2014年6月19日
 *      Author: root
 */

#ifndef COUNTERDEFINE_H_
#define COUNTERDEFINE_H_

enum CounterByDay
{
	CHALLEGE_INSTANCE_COUNTER = 1,		//挑战副本次数
	ESCORT_FREETIMES_COUNTER = 2,		//护送皇纲刷新免费次数
	ESCORT_SPEEDUP_COUNTER	= 3,		//护送皇纲加速次数
	ESCORT_DAY_COUNTER		= 4,		//护送皇纲次数
	EXCHANGFREE_DAY_MONEY =5, //免费领取金钱次数
	EXCHANGCOST_DAY_MONEY=6,  //兑换金钱次数
	FREEROLL_LUCKLYTURNTABLE =7,  //免费 转转盘次数
	ANSWER_CONTINUERIGHTS	= 8,	//答题活动连对次数记录
	COMPETE_INSTANCE_COUNTER	= 9,	//竞技场次数
	ANSWER_TOTALRIGHTS = 10,		//答题总共答对的次数
	TREASON_KING_ENTER = 11,     //进入剿灭反王副本次数
	TREASON_KING_LEVELS = 12,     //进入剿灭反王层数
	PROTECT_KING_COUNTER = 13,		//守护秦王重置次数
	PROTECT_KING_SWEEP = 14,  ///守护秦王扫荡次数
	BEYOND_BATTLE = 15,  ///夸服战次数
	BEYOND_BATTLE_REWARD = 16,  ///夸服战领奖次数
	PROTECT_KING_CURRENT_COUNTER = 17,//守护秦王当前关数
	ANSWER_ENTER_COUNT = 18,//进入答题次数（最大只为1）
	WORSHIP_COUNT = 19, // 被膜拜次数
	GET_ENVELOP_COUNT = 20,//领取红包计数
	BEYOND_SINGLE_BATTLE = 21,//单人夸服战次数
	BEYOND_GROUP_BATTLE = 22,//组队夸服战次数
	TREASON_KING_MAXPASSLEVEL = 23, // 剿灭反王最高通关层数
	TREASON_KING_RESET = 24, //剿灭反王重置次数
	ARENA_ALREADYREFRESH_CNT = 25, // 竞技场已经刷新次数
	ARENA_BUYCHALLENGE_CNT = 26, // 竞技场购买可挑战次数
	RECOVER_RESOURCE_COUNT = 27, //资源追回次数
	COUNTER_MAX_BY_DAY,
};

enum CounterByMonth
{
	TOTAL_SIGNIN_COUNTER = 10001,
	MENDING_SIGNIN_COUNT = 10002, // 补签次数
	COUNTER_MAX_BY_Month,
};

const int 	MAX_TOTAL_LOGIN_COUNTER = 7; //最大累积登录次数

enum CounterByForever  //永远次数
{
	CHARGE_RMB_COUNT = 20001,  //充值次数
	TOTAL_SIGNIN_COUNT = 20002, //签到次数
	LOGINMICRO_COUNT = 20003, // 微端登录次数
	ARENACHALLROBOT_COUNT = 20004, // 竞技场挑战次数
	COUNTER_MAX_BY_FOREVER,
};


#endif /* COUNTERDEFINE_H_ */
