/*
 * MagicMgr.h
 *
 *  Created on: 2014年8月8日
 *      Author: helh
 */

#ifndef SMAGICMGR_H_
#define SMAGICMGR_H_

#include <vector>
#include "define.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/CharLogin/MagicWeapon.pb.h"
#include "Timer/TimerInterface.h"
#include "SMagicLoader.h"
#include "MessageCommonRet.h"
#include "EventSet/EventArgs.h"
#include "../EventSet/EventDefine.h"

#define INVALID_PASSIVE_SKILL_QUALITY (-1)

using namespace std;
using namespace CommBaseOut;

class Player;

enum eSMagicMgrStateEnum
{
	//拥有
	e_SMagicMgr_Have =1,
	//可以使用临时
	e_SMagicMgr_Temp ,
	//应该倒计时
	e_SMagicMgr_BeginTime ,
	e_SMagicMgr_InTime ,
	//没有
	e_SMagicMgr_None ,
};

enum eSMagicAdvanceType
{
	eSMagicAdvanceNull   = 0,
	eSMagicAdvanceNormal = 1,
	eSMagicAdvanceAuto   = 2,
};

struct SMagicTempPassiveSkill
{
	SMagicTempPassiveSkill() : m_Index(0),m_SkillID(0)
	{

	}
	BYTE m_Index;
	int  m_SkillID;
};

class SMagicMgr
{
public:
	SMagicMgr(Player * p);

	~SMagicMgr();

	void ReInit();
	void ReleaseMagic();

	void InitMagicWeapon(const PlayerInfo::MagicWeapon & info);

	//上线计算属性
	void onlineCalculateAtt();

	void SetVitality();

	//计算一次战斗力
	int calculateFight();

	void getAttValue(int id,map<ECharAttrType,int64>& attList);

	void SetMagicWeapon(PlayerInfo::MagicWeapon * info);

	void SetMagicWeapon(MagicWeapon::ClientMagicWeapon * info);

	void SendToClientMagic();
	//初始化  锻炼度
	int InitExp(const int &exp, const int &daynum);

	//升阶
	void AdvanceMagicWeapon(MagicWeapon::RetAdvanceMagic * toClient,MagicWeapon::RequestAdvanceMagic& requestMsg);


	//一次升阶扣除
	ServerRet oneAdvanceExp(int& currentID,int& currentExp,vector<int>& passiveSkillList,map<int,short>& resultExpList,
			map<int,int>& itemList,int& money,int& golden,bool isAuto);

	//增加锻炼度
	void AddWeaponExp(int exp);

	//刷新技能
	int FlushSkill(int index);

	//替换技能
	int ReplaceSkill(int index);

	void BeginTimer(void * arg);

	void AddMagicWeapon(int id, bool isbroad = false);

	bool isMagicSkill(int skillID);

	void AttrChange(int beforeID,int currentID);

	int GetMagicWeapon()	const {return m_id;}

	//对神兵开始 倒计时
	void CountDownMagicWeapon(int needtm);

	//计算 现在 处于 什么 状态
	eSMagicMgrStateEnum CalNowState();

	void SetMagicWeaponState(const eSMagicMgrStateEnum &funEnum, MagicWeapon::ClientMagicWeapon * info);

	void SetTempID(const int id);

	int GetTempID() const {return m_tempID;}

	bool CanUpdateSumitMagic();
	//获得神兵阶数
	BYTE getMagicOrder();

	bool IsMagicMaxByLv();

	// 基本条件 是否能升级
	bool IsCanAdvance();
	// 基本条件 是否能刷新技能
	bool IsCanSkillRefresh();

protected:
	int CheckAdvanceCond();
	int CheckSkillRefreshCond();
	bool onLevelEvent(const EventArgs& e);

	// 升阶
	void Advance(MagicWeapon::RetAdvanceMagic * toClient,MagicWeapon::RequestAdvanceMagic& requestMsg);


private:
	Player*  m_owner;
	//神兵id
	int m_id;
	//神兵id
	int m_exp;
	//被动技能id
	vector<int> m_skillID;
	//上一次升阶的时间
	int64 m_advanceTime;
	//开启定时器
	TimerConnection m_beginTimer;
	//刷新出来的技能
	vector<SMagicTempPassiveSkill> m_PassiveSkillList;
	//临时ID
	int m_tempID;
	//开始 计时时刻
	int64 m_attacktime;
	//还须多少时间
	int m_needTm;
	//是否 在 地图中 是释放
	bool freeFlag ;
	//是否为第一次刷新
	bool m_FirstReflush;
	//等级事件
	EventConnection  m_LevelEvent;
};


#endif /* SMAGICMGR_H_ */
