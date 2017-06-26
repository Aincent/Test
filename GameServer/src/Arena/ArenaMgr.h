/*
 * ArenaMgr.h
 *
 *  Created on: 2016年5月4日
 *      Author: root
 */

#ifndef ARENAMGR_H_
#define ARENAMGR_H_

#include "Common.h"
#include "CharLogin/PlayerInfo.pb.h"
#include "define.h"

using namespace CommBaseOut;

class Player;

class ArenaMgr
{
public:
	ArenaMgr(Player& owner);
	~ArenaMgr();

	void InitArenaData(const PlayerInfo::ArenaDataInfo& info);
	void SaveArenaData(PlayerInfo::ArenaDataInfo& info);

	// 增加可挑战次数
	int AddChallCnt();
	void AddChallCnt(int cnt);
	// 减少可挑战次数
	int RedChallCnt(int cnt = 1);
	// 获取可挑战次数
	int GetCanChallCnt();
	// 获取基本挑战次数
	int GetBaseSumChallCnt();

	void Reset();
protected:

	// 是否可以增加挑战次数
	bool IsAddChall();


private:
	Player& m_owner;

	int m_canChallCnt; // 竞技场能挑战次数
	DWORD64 m_lastResetTime; // 最后重置时间

};

#endif /* ARENAMGR_H_ */
