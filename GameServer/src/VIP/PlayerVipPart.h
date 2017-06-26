/*
 * PlayerVipPart.h
 *
 *  Created on: 2014年8月11日
 *      Author: root
 */

#ifndef PLAYERVIPPART_H_
#define PLAYERVIPPART_H_

#include "define.h"
#include "Singleton.h"
#include <map>
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/CharLogin/PlayerVipProto.pb.h"
#include "EventSet/EventSet.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "VipCommon.h"

using namespace std;
using namespace CommBaseOut;

class Player;


//struct sVipData
//{
//	DWORD vipLv;
//	DWORD vipGetGiftInfo;			//
//	DWORD PlayerVipAddUpGold;		//
//};


class PlayerVipPart
{
public:
	PlayerVipPart(Player* owner);
	~PlayerVipPart();

	void ReInit();
	void Release();

	//存储到数据库
	void SaveToDB(PlayerInfo::PlayerVipInfo *info);
	//VIP数据初始化
	void InitInfo(const PlayerInfo::PlayerVipInfo &info);
	//VIP权限初始化
	void	InitPlayerVipRight(DWORD lv);

	//领取VIP礼包
	int GetVipGift(DWORD index, PlayerVipProto::RetGetVipGift &toClient);
	//
	void AddUpGold(DWORD value);

	void AddUpGoldAndSyn(DWORD value);

	// 获取对应权限的特权的值
	// PARAM: IN right(权限) RET: (对应的特权的值) 如果还没有某权限,返回－1 如果此权限是没有值的,返回0表示有此权限
	double GetValueByRight(eVipRight right);
	//设置vip体验卡
	void SetVipCard(int64 valideSecs);
	//移除vip体验卡
	void RemoveVipCard(void * arg);
	void ClearVipCard(void);
	//
	DWORD GetGiftInfo() { return m_nGiftInfo; }

	// --------连续充值活动
	//
	//领取充值奖励
	int GetRechargeAward(int id);
	//获得连续充值信息
	void GetRecharge(PlayerVipProto::ToClientRechargeInfo &info);

	DWORD GetAddUpGold() { return m_nAddUpGold; }
	//更新连续充值领取状态
	void UpDateRecharge();
	//获得几天前后的时间点
	int64 GetTime(int64 start,BYTE num , bool bIs);
	//连续充值活动结束
	void end(void * arg);
	//过期
	bool IsOver();

	// VIP购买花费的元宝  cost: 需花费的元宝  返回 真正需花费的元宝
	static int CalBuyCostGolden(const Smart_Ptr<PlayerVipPart>& vip,  int cost);

	int64 GetVipCardRestTime(void);
private:
	Player* m_nOwner;
	DWORD	  m_nGiftInfo;
	DWORD   m_nAddUpGold;
	int 			m_num;
	int 			m_maxNum;
	int64   m_lasetTime;
	int64 	m_endtime;
	map<int,int64> m_RechargeAwardState;
	TimerConnection	m_Timer;											//定时器
	//vip体验卡的开始时间
	int64 m_vipCardBeginTime;
	//Vip体验卡持续时间
	int64 m_valideSecs;
	//Vip体验卡持续时间定时器
	TimerConnection m_vipCardTimer;
};



#endif /* PLAYERVIPPART_H_ */
