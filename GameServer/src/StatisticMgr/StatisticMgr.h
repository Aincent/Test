/*
 * StatisticMgr.h
 *
 *  Created on: 2014年10月9日
 *      Author: root
 */

#ifndef STATISTICMGR_H_
#define STATISTICMGR_H_

#include "define.h"
#include "Singleton.h"
#include <map>
#include <vector>
#include <string>
#include "Smart_Ptr.h"
#include "Timer/TimerInterface.h"
#include "../EventSet/EventDefine.h"
#include "StatisticBase/StatisticDefine.h"
#include "MessageStruct/StatisticInfo.pb.h"
#include "MessageStruct/CharLogin/CharBaseInfo.pb.h"
#include "CharDefine.h"
#include "SvrConfig.h"
#include "StatisticBase/StatisticBase.h"
#include "StatisticBase/BehaviorHandle/BehaviorMonitor.h"

using namespace std;
using namespace CommBaseOut;

class Player;

#define NEEDSAVETOPLAYER 1
#define NEEDSAVETODAILY 2

const int Time_Save_Count = 10 * 1000;//1 * 60 * 1000


class StatisticMgr : public StatBehavBase, public StatGlobalBase, public Singleton<StatisticMgr>
{
public:
	StatisticMgr();
	~StatisticMgr();

public:
//	void SetData(StatisticInfo::UseStatisticData * statisticData, int64 data1 = 0, int64 data2 = 0,string data3 = "", bool addFlag = true,string data4 = "");
//	//保存按日统计的数据
//	void SaveDailyData(DWORD main,DWORD minor,StatisticInfo::UseStatisticData& data);
//	//保存按玩家统计的数据
//	void SavePlayerData(const Smart_Ptr<Player> &player,DWORD main,DWORD minor,StatisticInfo::UseStatisticData& data);
//	//玩家行为日志
//	void SavePlayerBehaviorData(int64 charID,DWORD main,DWORD minor,StatisticInfo::UseStatisticData& data);
//	//获取;获取同时是否设置发送标志
//	StatisticInfo::UseStatisticData* GetPlayerData(const Smart_Ptr<Player> &player,DWORD main,DWORD minor);
//
//	StatisticInfo::UseStatisticData* GetDailyData(DWORD main,DWORD minor);
//	//获取类型数据
//	int GetDailySaveType(DWORD main,DWORD minor);
//
//	int GetPlayerSaveType(const Smart_Ptr<Player> &player,DWORD main,DWORD minor);
//	//按天统计次数的类型
//	void StatisticDailyTimes(DWORD mainType, DWORD minorType,DWORD num = 1);
//
//	void StatisticPlayerDailyTimes(const Smart_Ptr<Player> &player, DWORD mainType, DWORD minorType,DWORD num = 1);
//	//记录玩家的个人行为                objectID:对谁起作用，可以是charID也可以是公会ID或者物品ID
//	void StatisticPlayerBehavior(int64 charID,DWORD mainType, DWORD minorType,google::protobuf::Message *content = NULL,int64 objectID = 0);
//	//统计玩家获得 使用的物品数量和消耗总价
//	void StatisticPlayerGetItems(const Smart_Ptr<Player> &player, DWORD mainType, DWORD minorType, DWORD num, DWORD cost);
//	//统计玩家获得 使用的物品数量和价格
////	void StatisticPlayerItemsPair(const Smart_Ptr<Player> &player, DWORD mainType, DWORD minorType, DWORD firstNum, DWORD secondNum);
//	//存储同一分类不同物品数量   是否 要 在之前基础上累加
//	void StatisticPlayerItemsMap(const Smart_Ptr<Player> &player, DWORD mainType, DWORD minorType, DWORD itemID, DWORD num, bool addFlag =true);
//	//  统计玩家登录信息
//	void StatisticPlayerLoginInfo(const Smart_Ptr<Player>& player, DWORD mainType, DWORD minorType);
//	//给统计记录服务器发送消息
//	void SendStatisticData();
//	//定时器
//	void SendDataUpdate(void * obj);

//	bool StatisticDailyTimes(const EventArgs & e);
//
//	bool StatisticPlayerLvUpgrade(const EventArgs & e);
//
//	bool HandlePlayerBeKilledEvent(const EventArgs & e);
//
//	bool HandleCurrencyChangeEvent(const EventArgs & e);

//	bool HandleGoldenChangeEvent(const EventArgs & e);
//
//	//需配套使用
//	static void SetItemCostBefore(StatisticInfo::CostItemInfo * costItemInfo,ECharAttrType charAttrtype,int num,Smart_Ptr<Player> & player);
//	static void SetItemCostAfter(StatisticInfo::CostItemInfo * costItemInfo,ECharAttrType charAttrtype,Smart_Ptr<Player> & player);

//	//需配套使用
//	static void SetItemCostBefore(StatisticInfo::CostItemInfo * costItemInfo,ECharAttrType charAttrtype,Smart_Ptr<Player> & player);
//	static void SetItemCostAfter(StatisticInfo::CostItemInfo * costItemInfo,ECharAttrType charAttrtype,int num,Smart_Ptr<Player> & player);
//
//	static void SetItemCost(StatisticInfo::CostItemInfo * costItemInfo,int64 itemID,int itenNum);
//
//	static void SetItemCost(StatisticInfo::CostItemInfo * costItemInfo,ECharAttrType charAttrtype,int itenNum);
//
//	static void AddItemGotInfo(StatisticInfo::GotItemInfo * gotItemInfo,ECharAttrType charAttrtype,int itenNum);
//
//	static void AddItemGotInfo(StatisticInfo::GotItemInfo * gotItemInfo,int64 itemID,int itenNum = 1);
//
//	static void SetStatisticItemInfo(const CharLogin::ClientItemInfo & clientItemInfo,StatisticInfo::ClientItemInfo * StaticClientItemInfo);
private:
//	//按天作统计的数据，由高16位为主类型，低16位为副类型
//	typedef std::map<DWORD,StatisticInfo::UseStatisticData> StatisticMap;
//	//按天统计的数据
//	StatisticMap m_DailyStatistic;
//
//	typedef std::map<int64, StatisticMap> PlayerStatisticMap;
//	//按人物统计的数据
//	PlayerStatisticMap m_playerStatistic;
//
//	typedef std::map<int64, vector<StatisticInfo::UseStatisticData> > PlayerBehaviorMap;
//	//玩家行为日志
//	PlayerBehaviorMap m_playerBehavior;
//
//	TimerConnection m_stime;
//
//	EventConnection m_passEvent;//玩家通关
//	//玩家升级
//	EventConnection m_playerLvUpgradeEvent;
//
//	EventConnection m_playerBeKilledEvent;
//	//通货类日志
//	EventConnection m_currencyChangeEvent;
//	//元宝统计
//	EventConnection m_goldenChangeEvent;

public:
	BEHAVMONI_BEGIN_DECLARE;
	BEHAVMONI_HANDLE_DECLARE;
	BEHAVMONI_END_DECLARE;
//	void BehavMoniBegin(int64 charID, const int& mainType, const int& minorType, const int& golden, const int& money, const int& force);

//	void BehaviorHandle(int64 charID, E_Behavior_Handle handle, const ::google::protobuf::Message* data);
//	void BehavMoniEnd(const int& mainType, const int& minorType, const Smart_Ptr<Player>& player);

	GLOBAL_STATIS_DECLARE;

protected:
	// 定时发送
	void SendData(void* obj);

	void SendStatistData();
	void SendStatistData(const short int& svrID, StatisticInfo::StatisticData& data);

private:
	typedef map<int64, vector<StatisticInfo::PlayerBehavior> > BehaviorSet;
	BehaviorSet m_behaviorSet;

	typedef map<GlobalKey, StatisticInfo::GlobalStatist> GlobalSet;
//	typedef vector<StatisticInfo::GlobalStatist> GlobalSet;
	GlobalSet m_global;

	TimerConnection m_timerSend;

	BehaviorMoni m_monitor;

};

//#define BEHAVIOR_HANDLE(H_T, S_T, D) StatisticMgr::GetInstance()->Handle(H_T, S_T, D);

//#define BEHAVIOR_HANDLE_DO(H, T, C) StatisticMgr::GetInstance()->Handle(H, T._t, C);
//
//class BehaviorHandleSave
//{
//	static void Handle(E_Behavior_Handle H, BehaviorSaveType::INT64 T = BehaviorSaveType::INT64(), int64 param = 0)
//	{
//		ServerReturn::ServerRetInt content;
//		content.set_ret(param);
//
//		BEHAVIOR_HANDLE_DO(H, T, content);
//	}
//};
//
//class BehaviorMoni
//{
//public:
//	// 一个操作
//	void Handle(int64 charID, E_Behavior_Handle handle, eSaveType save, ::google::protobuf::Message data);
//
//	// 行为开始
//	void Begin(int64 charID, int mainType, int minorType, Behavior::Data::Require arg);
//	// 行为结束
//	void End(int64 charID, int mainType, int minorType, Behavior::Data::Require arg);
//
//	// 行为提取
//	int Fetch(int64 charID, StatisticInfo::PlayerBehavior& record);
//
//private:
//	void Begin(int64 charID);
//	void End(int64 charID);
//
//private:
//	map<int64, stack<Behavior::HandleData> > m_monitorData;
//
//};

//class BehaviorMoni
//{
//public:
//	// 一个操作
//	void Handle(E_Behavior_Handle handle, BehaviorSaveType::eSaveType save, ::google::protobuf::Message& data);
//
//	// 行为开始
//	void Begin(int mainType, int minorType, Behavior::Data::Require arg);
//	// 行为结束
//	void End(int mainType, int minorType, Behavior::Data::Require arg);
//
//	// 行为提取
//	int Fetch(StatisticInfo::PlayerBehavior& record);
//
//private:
//	void Begin();
//	void End();
//
//private:
//	stack<Behavior::HandleData> m_monitorData;
//
//};


#endif /* STATISTICMGR_H_ */
