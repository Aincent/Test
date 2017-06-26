/*
 * OnLineAtTimeMgr.h
 *
 *  Created on: 2016年2月22日
 *      Author: root
 */

#ifndef ONLINEATTIMEMGR_H_
#define ONLINEATTIMEMGR_H_

#include "./FileLoader/OnLineAtTimeLoader.h"
#include "define.h"
#include "util.h"
#include "Timer/TimerInterface.h"
#include "Singleton.h"
#include "../EventSet/EventDefine.h"
#include "MessageStruct/Activity/OnLineAtTime.pb.h"

using namespace std;
using namespace CommBaseOut;

class Player;

class OnLineAtTimeMgr : public Singleton<OnLineAtTimeMgr>
{
public:
	OnLineAtTimeMgr();

	virtual ~OnLineAtTimeMgr();

	void InitOnLineAtTime();

	void onGiveReward(void * arg);

	void Clear();

	void GetOnLineAtTimeInfo(OnLineAtTimeInfo::RetOnLineAtTimeInfo & toClient);
private:
	std::vector<TimerConnection> timeList;
};

#endif /* ONLINEATTIMEMGR_H_ */
