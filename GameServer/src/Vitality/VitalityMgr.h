/*
 * VitalityMgr.h
 *
 *  Created on: 2015年3月17日
 *      Author: root
 */

#ifndef VITALITYMGR_H_
#define VITALITYMGR_H_

#include "MessageStruct/CharLogin/Vitality.pb.h"
#include "EventSet/EventArgs.h"
#include "../EventSet/EventDefine.h"
#include "VitalityLoader/VitalityLoader.h"
#include "Timer/TimerInterface.h"

using namespace CommBaseOut;

class Player;

class VitalityMgr
{

public:
	VitalityMgr(Player * player);
	~VitalityMgr();

	void ReInit();
	void Release();

	//初始化活跃信息
	void InitVitalityFromDB(const VitalityProto::ProtoVitalityInfo &info);
	//保存活跃信息
	void SetVitalityToDB(VitalityProto::ProtoVitalityInfo *info);
	//保存活跃信息发送给客户端
	void SetVitality(VitalityProto::ProtoVitalityInfo *info);
	//获取奖励
	int GetAward(int id);
	//完成活跃度
	void FinshVitality(EVitalityType type,int count = 1);
	//获取活跃度
	int GetVitality(EVitalityType type);
	//副本活跃度
	void FinshMapInstanceVitality(int mapid);
	//重置数据
	void ResetVitality();
	//是否领取过一个活跃度奖励
	bool isTodayHaveReceiveOneReward();

	// 操作能领取最大奖励ID
	void SetCanGetMaxID(int id);
	int GetCanGetMaxID();

	// 获取未领取奖励的数量
	int GetUnGetAwardCnt();
	// 获取某ID的未完成次数
	int GetUnCompleteByID(EVitalityType id);

	bool HandlePlayerSignIn(const EventArgs & e);
	bool HandlePlayerBuyItemFromMall(const EventArgs & e);
	bool HandlePlayerEquipInheritEvent(const EventArgs & e);
	bool HandlePlayerSkillUpgrade(const EventArgs & e);
	bool HandlePlayerSubmitTask(const EventArgs & e);
	bool HandlePlayerOneKeyComplete(const EventArgs & e);
	bool HandleGirlWarLingering(const EventArgs & e);
	bool HandlePlayerTurnLuckTable(const EventArgs & e);
	bool HandlePlayerMoneyShop(const EventArgs & e);
	bool HandlePlayerPassInstance(const EventArgs & e);
private:
	Player * m_owner;
	int m_Score;
	int64 m_time;
	map<int, sVitalityInfo> m_allVitalityInfo;
	map<int,bool> m_isGetAward;
	int m_canGetMaxID; // 能领取最大奖励ID

	EventConnection m_signInEvent;
	EventConnection m_BuyItemFromMallEvent;
	EventConnection m_equipInheritEvent;
	EventConnection m_skillUpgradeEvent;
	EventConnection m_submitQuestEvent;
	EventConnection m_oneKeyCompleteEvent;
	EventConnection m_girlWarLingeringEvent;
	EventConnection m_TurnLuckTableEvent;
	EventConnection m_moneyShopEvent;
	EventConnection m_passInstanceEvent;
};

#endif /* VITALITYMGR_H_ */
