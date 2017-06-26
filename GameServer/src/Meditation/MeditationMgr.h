/*
 * MeditationMgr.h
 *
 *  Created on: 2015年11月2日
 *      Author: root
 */

#ifndef MEDITATIONMGR_H_
#define MEDITATIONMGR_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"
#include "../MsgCommonClass.h"
#include "MessageStruct/CharBase/MeditationProto.pb.h"

using namespace std;
using namespace CommBaseOut;

#define REQMEDITATION	0
#define CANCELMEDITATION 1

enum eMeditationType
{
	eMeditationType_Single,		//单修
	eMeditationType_Strange,	//陌生人
	eMeditationType_Friend,		//好友
	eMeditationType_Couple,		//夫妻
};

struct DoubleMeditationDataParam
{
	DoubleMeditationDataParam():invitor(0),target(0),beginTime(0),type(0),count(0)
	{

	}
	int64 invitor;
	int64 target;
	int64 beginTime;		//开始时间
	int type;						//双修类型
	int count;					//定时器执行次数
};

struct DoubleMeditationData
{
	DoubleMeditationData()
	{
		param = NULL;
	}
	TimerConnection timer;
	DoubleMeditationDataParam *param;
};

//修炼类型	打坐加成	亲密度加成	亲密度加成时间
struct MeditationConfig
{
	MeditationConfig()
	{
		type = 0;
		closedValue = 0;
		closedTime = 0;
		rate = 0;
	}

	int type;
	int closedValue;
	int closedTime;
	float rate;
};

class MeditationMgr : public Singleton<MeditationMgr>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	MeditationMgr();
	~MeditationMgr();
public:
	const MeditationConfig* GetConfig(int type);

	//地图区域能否打坐双修
	bool IsCanMeditation(CGameZone* pZone, int xpos, int ypos);

	//取消双修
	void CanccelDoubleMeditation(int64 invitor, int64 target);

	//从邀请列表中移出某个玩家的邀请数据
	void RemovInvits(int64 charID);

	void RemoveBeInvites(int64 charID);

	//玩家招出战㘍
	void PlayerFightGirlWar(Smart_Ptr<Player>& player);

private:
	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

	//配置信息
	void LoadConfig();

	//邀请
	int Invite(Smart_Ptr<Player>& invitor, Smart_Ptr<Player>& target);

	//同意或拒绝双修邀请
	int Reply(Smart_Ptr<Player>& player, bool flag, int64 targetID);

	//得到玩家被邀请的次数
	int GetBeInvitCount(int64 charid);

	//更新被邀请的次数
	void UpdateBeInvitCount(int64 charid, int add);

	//检测
	void CheckTimer(void * obj);

	//是否在邀请列表中
	bool IsInInviteLists(int64 inviteID, int64 targetID);

	//从某个玩家的邀请列表中移出某个
	void RemoveFromLists(int64 inviteID, int64 targetID);

	//开始双修
	void StartDoubleMeditation(Smart_Ptr<Player>& invitor, Smart_Ptr<Player>& target, int type);

	void UpdateDoubleMeditation(Smart_Ptr<Player>& invitor, Smart_Ptr<Player>& target, int type);

	//开启双修定时器
	void StartDoubleTimer(Smart_Ptr<Player>& invitor, Smart_Ptr<Player>& target, int type);

	//得到双修的数据
	//void GetDoubleMeditation(Smart_Ptr<Player>& player, Smart_Ptr<Player>& target, MeditationProto::SendStartMeditation& data);

	//根据邀请者的位置移动目标位置
	void MovePlayerPos(Smart_Ptr<Player>& invitor, Smart_Ptr<Player>& target);

	//计算位置
	bool CalNewPos(Smart_Ptr<Player>& invitor, int &newxPos, int &newypos);

	void OnDoubleMeditation(void* args);

	//奖励
	void Prize(Smart_Ptr<Player>& player, int closeValue);

	DEF_MSG_QUICK_SIMPLE_DECLARE_FUN_H(MSG_REQ_C2GM_MEDITATION);
	//DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_MEDITATION);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_DOUBLE_MEDITATION);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_REPLY_MEDITATION_INVIT);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_CANCCEL_DOUBLE_MEDITATION);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_GM2WS_PLAYER_NEXUS);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_WS2GS_NEXUS_CHANGE);

private:
	std::map<int64,std::map<int64,int64> > m_InvitLists; //邀请队列  目标id及对应的邀请时间

	std::map<int64,int64> m_beInvits;	//对象被邀请次数

	TimerConnection m_MeditationCheckTimeID;	//邀请列表检测定时器

	std::map< pair<int64,int64>, DoubleMeditationData> m_timerList; //双修定时器

	std::map<int,MeditationConfig> m_config;	//配置信息

	const int m_InvitValidTime;	//邀请的有效时间
	const int m_max_beInvite;		//最大被邀请人数

};



#endif /* MEDITATIONMGR_H_ */
