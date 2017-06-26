/*
 * Distortion.h
 *
 *  Created on: 2015年12月10日
 *      Author: root
 */

#ifndef DISTORTION_H_
#define DISTORTION_H_

#include "define.h"
#include "Timer/TimerInterface.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"
#include "../MsgCommonClass.h"

using namespace CommBaseOut;

class Player;

class PlayerDistortion
{
public:
	PlayerDistortion(Player* player);
	~PlayerDistortion();

	void ReInit();
	void Release();

public:
	//变身
	void Start(int monster);

	//结束变身
	void End();

	//能否移动
	bool IscanMove();

	//
	int GetMonsterID(){ return m_monsterID; }

private:
	void OnTimer(void * args);

private:
	Player*	m_player;

	TimerConnection m_timeID;	//变身定时器

	int m_monsterID;	//变身的怪物id
};



#endif /* DISTORTION_H_ */
