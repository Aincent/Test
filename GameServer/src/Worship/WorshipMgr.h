///*
// * WorshipMgr.h
// *
// *  Created on: 2015年3月25日
// *      Author: root
// */
//
//#ifndef WORSHIPMGR_H_
//#define WORSHIPMGR_H_
//
//#include "define.h"
//#include "MessageStruct/CharLogin/WorshipMessage.pb.h"
//
//using namespace CommBaseOut;
//using namespace std;
//
//int const EveryDay_WorshipNum = 10;
//
//class Player;
//
//class WorshipMgr {
//public:
//	WorshipMgr(Player* player);
//	~WorshipMgr();
//
//	// 初始化
//	void InitWorshipInfo(WorshipMsgProto::ClientWorshipInfo& info);
//	// 保存
//	void SetWorshipInfo(WorshipMsgProto::ClientWorshipInfo* info);
//
//	// 重置
//	void ResetWorshipInfo();
//
//	// 膜拜别人
//	// PARAM: IN value(增加的膜拜次数), IN playerID(被膜拜的玩家ID), OUT worCntByOther(被膜拜的玩家的膜拜次数)
//	// RET: Error Code
//	int WorshipOnline(int value, int64 playerID, int& worAmtByOther, int& surplusWorCnt);
//	// 膜拜下线玩家的  在收到来自DB消息时调用
//	int WorshipOffline(int64 playerID, int& surplusWorCnt);
//	//
//	// 获取今天膜拜过的玩家的charid
//	void GetTodayWorshipOfChar(vector<int64>& todayWorOfChar);
//	// 是否膜拜过某个玩家
//	bool IsWorshipPlayer(int64 playerID);
//	// 增加膜拜过的玩家
//	void AddWorshipPlayer(int64 playerID);
//	// 获取剩余膜拜次数
//	int GetTodaySurplusAmt(int& surplusAmt);
//
//	// 设置总数
//	void SetWorshipAmount(int worshipAmount);
//	// 获取总数
//	int GetWorshipAmount(int& worshipAmount);
//	// 改变总数
//	int ChangeWorshipAmount(int value, bool isAdd = true);
//
//
//
//private:
//
//	int m_amount; // 被膜拜次数
//	vector<int64> m_todayWorOfChar; // 今天膜拜过的玩家
//
//	Player* m_owner;
//};
//
//#endif /* WORSHIPMGR_H_ */
