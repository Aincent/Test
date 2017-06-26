/*
 * CountryRedEnvelopeMgr.h
 *
 *  Created on: 2015年4月9日
 *      Author: root
 */

#ifndef COUNTRYREDENVELOPEMGR_H_
#define COUNTRYREDENVELOPEMGR_H_

#include <map>
#include "define.h"
#include "Common.h"
#include "Smart_Ptr.h"
#include <vector>
#include "RedEnvelopeDefine.h"
#include "MessageStruct/RedEnvelope/RedEnvelope.pb.h"

using namespace std;
using namespace CommBaseOut;

class Player;

class CountryRedEnvelopeMgr
{
public:
	CountryRedEnvelopeMgr(Player * player);
	~CountryRedEnvelopeMgr();

	void ReInit();
	void Release();

	int GetCanGetEnvelopeCountMax(int lv);

	int GetCurrentCanGetEnvelopeCount();

	void ReleaseEnvelopeMgr();

	int GetEnvelopeReward(const RedEnvelopeProto::ReqGMGetEnvelopeReward &req);

	int getMaxEnvelopeToday() const
	{
		return m_maxEnvelopeToday;
	}

	void setMaxEnvelopeToday(int maxEnvelopeToday)
	{
		m_maxEnvelopeToday = maxEnvelopeToday;
	}

private:
	Player * m_owner;
	//今天最多能领多少个红包,以玩家今天第一次上线时的等级为准
	int m_maxEnvelopeToday;
};


#endif /* COUNTRYREDENVELOPEMGR_H_ */
