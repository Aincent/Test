/*
 * FestivalActiveDefine.h
 *
 *  Created on: 2015年5月4日
 *      Author: root
 */

#ifndef FESTIVALACTIVEDEFINE_H_
#define FESTIVALACTIVEDEFINE_H_
#include <vector>
#include "Common.h"
#include "define.h"
using namespace std;
using namespace CommBaseOut;

struct FestivalActiveInfo
{
	FestivalActiveInfo()
	{
		id=0;
		lv=0;
		rate=0;
		startTime=0;
		endTime=0;
	}
	long id;//活动id
	int lv;//等级差
	int rate;//掉落几率
	int64 startTime;//活动开始时间
	int64 endTime;//活动结束时间
	std::vector<std::pair<int,float> > dropInfo;//掉落信息
};

struct FestivalPrize
{
	FestivalPrize()
	{
		id=0;
		activeID=0;
		count=0;
	}
	int id;//兑换奖励id
	int activeID;//活动id
	int count;//day time
	std::vector<std::pair<int,int> > costProp;//消耗道具
	std::vector<std::pair<int,int> > getProp;//得到道具
};

#endif /* FESTIVALACTIVEDEFINE_H_ */
