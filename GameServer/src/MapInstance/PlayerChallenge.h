/*
 * PlayerChallenge.h
 *
 *  Created on: 2016年5月6日
 *      Author: root
 */

#ifndef PLAYERCHALLENGE_H_
#define PLAYERCHALLENGE_H_

#include <map>
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "Smart_Ptr.h"
#include "define.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/CharLogin/MapInstanceProto.pb.h"
#include "Timer/TimerInterface.h"
#include "InstanceMapLoader.h"
#include "CharDefine.h"
#include "PVPDefine.h"
#include "../EventSet/EventDefine.h"

using namespace std;
using namespace CommBaseOut;

class Player;
class PlayerChallenge
{
public:
	PlayerChallenge(Player* player);
	~PlayerChallenge();

	void ReInit();
	void Release();

public:
	void InitChallengeInfo(const PlayerInfo::ChallengeInstance& info);
	void GetChallengeInfo(PlayerInfo::ChallengeInstance* info);

	void GetData(MapInstanceProto::AckChallengeInfo *data);

	int GetCurLayer(){ return m_layer; }
	int GetMaxLayer(){ return m_maxlayer; }

	bool IsSweep();

	int StartSweep(int &totalTime);

	int EndSweep(MapInstanceProto::ACkEndSweep* data);

	int ExChange(int id, int num);

	void Reset();

	int CreateChecke(int id, int layer);

	void UpdateLayer(int layer);

	void UpdateRecord(int layer);

private:
	void OnSweepOver(void* args);

	bool Exit(const EventArgs & e);

	void Prize(int startlayer, int endlayer, std::vector<PropItem>& items);

private:
	Player* m_player;
	int m_layer;
	int m_maxlayer;

	DWORD64 m_startTime;
	DWORD64 m_totalTime;
	TimerConnection m_sweepTimer;
	EventConnection m_exitEvent;
};





















#endif /* PLAYERCHALLENGE_H_ */
