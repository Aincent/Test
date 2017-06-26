///*
// * WorshipMgr.cpp
// *
// *  Created on: 2015年3月25日
// *      Author: root
// */
//
//#include "WorshipMgr.h"
//#include "CounterDefine.h"
//#include "../Counter/Counter.h"
//#include "MessageCommonRet.h"
//#include "../Object/Player/PlayerMessage.h"
//
//WorshipMgr::WorshipMgr(Player* player) : m_amount(0), m_owner(player)
//{
//
//}
//
//WorshipMgr::~WorshipMgr()
//{
//
//}
//
//
//void WorshipMgr::InitWorshipInfo(WorshipMsgProto::ClientWorshipInfo& info)
//{
//	int64 now = CUtil::GetNowSecond();
//	int64 offLineTime = m_owner->GetOffLineTime();
//	if (now/(1000*60*60*24) != offLineTime/(1000*60*60*24))
//	{
//		ResetWorshipInfo();
//	}
//
//	m_amount = info.woramount();
//
//	for (int i = 0; i < info.dayyetworofchar_size(); i++)
//	{
//		m_todayWorOfChar.push_back(info.dayyetworofchar(i));
//	}
//
//	m_owner->SetDataFlag(eWorshipInfo);
//}
//
//void WorshipMgr::SetWorshipInfo(WorshipMsgProto::ClientWorshipInfo* info)
//{
//	info->set_woramount(m_amount);
//	for (uint i = 0; i < m_todayWorOfChar.size(); ++i)
//	{
//		info->add_dayyetworofchar(m_todayWorOfChar[i]);
//	}
//}
//
//// 重置
//void WorshipMgr::ResetWorshipInfo()
//{
//	m_todayWorOfChar.clear();
//	m_owner->SetDataFlag(eWorshipInfo);
//}
//
//void WorshipMgr::SetWorshipAmount(int worshipAmount)
//{
//	m_amount = worshipAmount;
//	m_owner->SetDataFlag(eWorshipInfo);
//}
//
//int WorshipMgr::GetWorshipAmount(int& worshipAmount)
//{
//	worshipAmount = m_amount;
//	return 0;
//}
//
//// 改变总数
//int WorshipMgr::ChangeWorshipAmount(int value, bool isAdd)
//{
//	if (isAdd)
//	{
//		m_amount += value;
//	}
//	else
//	{
//			m_amount -= value;
//			if (m_amount < 0)
//			{
//				m_amount = 0;
//			}
//	}
//
//	m_owner->SetDataFlag(eWorshipInfo);
//	return m_amount;
//}
//
//int WorshipMgr::WorshipOnline(int value, int64 playerID, int& worAmtByOther, int& surplusWorCnt)
//{
//	// 膜拜次数
//	if (m_owner->GetCounterService()->GetNumById(WORSHIP_COUNT) >= EveryDay_WorshipNum)
//	{
//		return eWorshipCountUseUp;
//	}
//
//	// 膜拜的是否是自己
//	if (m_owner->GetID() == playerID)
//	{
//		return eWorshipCannotSelf;
//	}
//
//	// 是否已经膜拜过此玩家
//	vector<int64>::iterator it = find(m_todayWorOfChar.begin(), m_todayWorOfChar.end(), playerID);
//	if (it != m_todayWorOfChar.end())
//	{
//		return eWorshipAlreadyChar;
//	}
//
//
//	// 获取被膜拜的玩家
//	Smart_Ptr<Player> otherPlayer;
//	PlayerMessage::GetInstance()->GetPlayerByCharid(playerID, otherPlayer);
//	if(!otherPlayer)
//		return eNull;
//
//	worAmtByOther = otherPlayer->GetWorshipMgr()->ChangeWorshipAmount(value, true);
//	surplusWorCnt = EveryDay_WorshipNum - m_owner->GetCounterService()->GetNumById(WORSHIP_COUNT);
//
//	m_todayWorOfChar.push_back(playerID);
//	m_owner->GetCounterService()->IncreasePlayerCounter(WORSHIP_COUNT, 1);
//
//	m_owner->SetDataFlag(eWorshipInfo);
//	return 0;
//}
//
//
//int WorshipMgr::WorshipOffline(int64 playerID, int& surplusWorCnt)
//{
//	// 膜拜次数
//	if (m_owner->GetCounterService()->GetNumById(WORSHIP_COUNT) >= EveryDay_WorshipNum)
//	{
//		return eWorshipCountUseUp;
//	}
//
//	// 膜拜的是否是自己
//	if (m_owner->GetID() == playerID)
//	{
//		return eWorshipCannotSelf;
//	}
//
//	// 是否已经膜拜过此玩家
//	vector<int64>::iterator it = find(m_todayWorOfChar.begin(), m_todayWorOfChar.end(), playerID);
//	if (it != m_todayWorOfChar.end())
//	{
//		return eWorshipAlreadyChar;
//	}
//
//	m_todayWorOfChar.push_back(playerID);
//	m_owner->GetCounterService()->IncreasePlayerCounter(WORSHIP_COUNT, 1);
//
//	surplusWorCnt = EveryDay_WorshipNum - m_owner->GetCounterService()->GetNumById(WORSHIP_COUNT);
//
//	m_owner->SetDataFlag(eWorshipInfo);
//	return 0;
//}
//
//// 获取今天膜拜过的玩家的charid
//void WorshipMgr::GetTodayWorshipOfChar(vector<int64>& todayWorOfChar)
//{
//	todayWorOfChar = m_todayWorOfChar;
//
//}
//
//bool WorshipMgr::IsWorshipPlayer(int64 playerID)
//{
//	for (uint i = 0; i < m_todayWorOfChar.size(); ++i)
//	{
//		if (m_todayWorOfChar[i] == playerID)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//void WorshipMgr::AddWorshipPlayer(int64 playerID)
//{
//	m_todayWorOfChar.push_back(playerID);
//}
//
//int WorshipMgr::GetTodaySurplusAmt(int& surplusAmt)
//{
//	if (NULL != m_owner)
//	{
//		int amt = EveryDay_WorshipNum - m_owner->GetCounterService()->GetNumById(WORSHIP_COUNT);
//		surplusAmt = amt;
//		if (0 >= amt)
//		{
//			return eWorshipCountUseUp;
//		}
//
//	}
//	else
//		return -1;
//
//	return 0;
//
//
//}
