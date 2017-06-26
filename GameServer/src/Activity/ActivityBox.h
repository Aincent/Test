/*
 * ActivityBox.h
 *
 *  Created on: 2015年7月29日
 *      Author: root
 */

#ifndef ACTIVEITYBOX_H_
#define ACTIVEITYBOX_H_

#include "define.h"
#include "util.h"
#include "Timer/TimerInterface.h"
#include "Singleton.h"
#include "../EventSet/EventDefine.h"

using namespace std;
using namespace CommBaseOut;

struct MasterStatus
{
	int nKey;
	int64 nCharId;
	int64 nSceneID;
	bool bStatus;

	MasterStatus():nKey(0),nCharId(0),nSceneID(0),bStatus(0)
	{

	}
};

class Player;

class ActivityBox : public Singleton<ActivityBox>
{
public:
	ActivityBox();

	virtual ~ActivityBox();

	void onFightTime(void* obj);

	void UpdateActivityBossBox(void * arg);

	void UpdateActivityTimes(void* arg);

	bool HandleLeaveMapEvent(const EventArgs & e);

	bool PlayerActivityBoxAttack(const EventArgs & e);

	bool PlayerActivityBoxMove(const EventArgs & e);

	bool IsCanGather(int nKey,Smart_Ptr<Player>& player);

	void BeginActivityBox();

	void EndActivityBox();

	DWORD64 GetClanWarBeginTime();

	// 更新玩家活动箱子状态
	void UpdatePlayersActivityBox(bool flag);
	// 天降宝箱活动是否开始
	bool IsActivityBoxBegin();
private:
	int m_BoxState;
	int m_nTimes;
	int64 m_nSceneId;

	//开启时间
	TimerConnection m_sFightTime;
	TimerConnection m_sFlushTime;
	TimerConnection m_sCountTime;

	map<int,MasterStatus> m_MasterStatus;

	//怪物离开地图事件
	EventConnection m_leaveMapEvent;
	//玩家攻击事件
	EventConnection m_playerAttackEvent;
	//玩家移动事件
	EventConnection m_playerMoveEvent;
};


#endif /* ACTIVEITYBOX_H_ */
