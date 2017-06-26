/*
 * PlayerMeditation.h
 *
 *  Created on: 2015年11月2日
 *      Author: root
 */

#ifndef PLAYERMEDITATION_H_
#define PLAYERMEDITATION_H_
#include "define.h"
#include "Timer/TimerInterface.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"
#include "../MsgCommonClass.h"

using namespace CommBaseOut;

class Player;

class PlayerMeditation
{
public:
		PlayerMeditation(Player* player);
		~PlayerMeditation();

		void ReInit();
		void Release();

public:
		//打坐定时器
		int StartMeditationTimer();
		int DeleteMeditationTimer(bool SendToSelf=true);
		void CallBackMeditation(void * obj);
		void UpdateMeditation();

		//同意双修时，更新自己的状态
		void UpdateSelfDoubleState();

		//取消定时器
		void CanccelTimer();

		//重新开启定时器
		void ReStartTimer();

		//通知双修对方
		void NoticeTarget();

		//修炼奖励
		void Prize(float rate, int closeValue);

		//广播打坐单修
		void BroadCastSingleMeditation(bool StartFlag, bool SendToSelf=true);

		//得到双修的目标信息
		Smart_Ptr<Player> GetTargetInfo();

		void SetTargetID(int64 charid){ m_targetID = charid; }
		int64 GetTargetID(){ return m_targetID; }

private:
		Player*	m_player;

		TimerConnection m_MeditationTimeID;	//打坐定时器

		//一次打坐获得的总真气（从打坐到取消打坐）
		int m_meditationForce;

		//双修的对象
		int64 m_targetID;
};



#endif /* PLAYERMEDITATION_H_ */
