/*
 * FestivalActive.h
 *
 *  Created on: 2015年4月30日
 *      Author: root
 */

#ifndef FESTIVALACTIVE_H_
#define FESTIVALACTIVE_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"
#include "../MsgCommonClass.h"
#include "Timer/TimerInterface.h"
#include "FestivalActiveDefine.h"

using namespace std;
using namespace CommBaseOut;

class FestivalActiveMgr : public Singleton<FestivalActiveMgr>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	FestivalActiveMgr();
	~FestivalActiveMgr();
public:
	//活动是否开启
	bool isActive(int id);

	//得到掉落的物品
	int getDropItem();

	//当前活动
	const FestivalActiveInfo* getCurFestivalInfo();

	//活动信息
	const FestivalActiveInfo* getFestivalInfo(int id);

	//时间转换
	int64 timeconvert(int year,int month,int day,int hour,int minute,int seconds);

	//推送节日活动
	void	FestivalCmd(Smart_Ptr<Player>& player);

	//得到活动开始结束时间秒
	bool getfestivalTime(int id,int64& begin, int64& end);

	void initTimer();
	void onTime(void * obj);


private:
	void	loadConfig();
	void loadPrize();
	bool handleMonsterDead(const EventArgs& e);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);
private:
		DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GS_START_END_ACTIVE);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_FESTIVAL_ACTIVE_PRIZE);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_FESTIVAL_EXCHAGE_INFO);
		EventConnection m_monsterKilledEvent;
		std::map<int,FestivalActiveInfo> m_festivalInfo;//活动配置
		std::map<int,FestivalPrize> m_festivalPrizeInfo;//活动奖励
		std::map<int,int> m_curActiveFestival;//当前进行的活动
		TimerConnection m_stime;
};



#endif /* FESTIVALACTIVE_H_ */
