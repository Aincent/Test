/*
 * RedEnvelopeDefine.h
 *
 *  Created on: 2015年4月8日
 *      Author: root
 */

#ifndef REDENVELOPEDEFINE_H_
#define REDENVELOPEDEFINE_H_

#include "Common.h"
#include "define.h"
#include <string>
#include <map>

using namespace CommBaseOut;
using namespace std;

enum GetEnvelopeOpType
{
	GetEnvelopeOpType_single = 0,//获取一个
	GetEnvelopeOpType_all,//能获取几个获取几个
	GetEnvelopeOpType_max,
};

//获取红包返利
enum GetEnvelopeRewardType
{
	GetEnvelopeRewardType_count=0,//普通返利
	GetEnvelopeRewardType_acc,//累积返利
	GetEnvelopeRewardType_bindGolden,//绑定元宝
	GetEnvelopeRewardType_max,
};

//领取记录
struct EnvelopeGetInfo
{
	int64 senderID;//谁派送的
	int64 getTime;//领取时间
	std::string getName;//谁领取
	int getBindGolden;//领取的绑定元宝数量
	int64 sendTime;//派送时间
	std::string senderName;//发送者名字
	EnvelopeGetInfo():senderID(0),getTime(0),getName(""),getBindGolden(0),sendTime(0),senderName("")
	{

	}
};

struct RedEnvelopeInfo
{
	int64 envelopeID;//红包ID
	int64 sendTime;//派发时间
	short country;//阵营
	bool state;//false 不在线
	std::string playerName;//派发人名字
	int64 charID;//派发人ID
	int getCount;//当前可领取返利的次数
	int accumulateCount;//当前可领取的红包累积奖励次数
	//std::map<int64,EnvelopeGetInfo> getInfoMap;//红包领取人列表
	std::vector<int64> getIdList;
	RedEnvelopeInfo():envelopeID(0),sendTime(0),country(0),state(0),playerName(""),
			charID(0),getCount(0),accumulateCount(0)
	{

	}
};


#endif /* REDENVELOPEDEFINE_H_ */
