/*
 * RewardHail.h
 *
 *  Created on: 2014年6月17日
 *      Author: root
 */

#ifndef REWARDHAIL_H_
#define REWARDHAIL_H_

#include <map>
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "Smart_Ptr.h"
#include "define.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/RewardHail/RewardHail.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"

#include "BitArray.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "EventSet/EventSet.h"


using namespace std;
using namespace CommBaseOut;

class Player;

/*
 * 服务器 意外关闭  玩家没下线  且 上一次登录时间 是  超过一天
 * 会导致  累积 登录天数 有误差
 * */


class SignIn
{
public:
	SignIn(Player * player);
	~SignIn();

	void ReInit();
	void Release();

	void ResetSigIn();

	void SetSignInToDB(PlayerInfo::SignInInfo *signinInfo);
	void InitSignInInfo(const PlayerInfo::SignInInfo &signInInfo);

	void InitTotalLoginInfo(const PlayerInfo::SignInInfo &signInInfo);
	void SetTotalLoginToDB(PlayerInfo::SignInInfo *signinInfo);
	//设置 累积登录值
	int SetTotalLogin();

	void SetSignIn(int daily, ServerReturn::ServerDoubleInt& info);
	int GetSignInReward(RewardHail::GetReward &info, vector<DWORD> &goods, vector<DWORD> &num);
	bool CheckSignInReward();

	int GetTolalLoginReward(RewardHail::GetReward &info, vector<DWORD> &goods,	vector<DWORD> &num,vector<DWORD> &strengthLvVec);

	void GetSIgnInInfo(RewardHail::ClientSignInInfo &info);

	void GetTolalLogin(RewardHail::OpenToalLoginAndFirstChargeRewardInfo &info);
	int GetTotalLoginStatus();

	//判断是否是首次充值
	bool IsFirstChargeRmb();
	// 判断是否领奖励
	bool IsGetFirstAward();

	int GetFirstChargeRmbReward();
	int GetFirstChargeRmbReward(RewardHail::GetReward &info, vector<DWORD> &goods,	vector<DWORD> &num,vector<DWORD> &strengthLvVec);

	void SetfirstChargeRewardFlag(bool Flag);
	void SetRewardItemInfo(RewardHail::GetReward &info, vector<DWORD> &goods);

	//SignIn
//	bool SignIn_IsHaveRward();
	int GetSignInCanGetAward();

	bool TotalLogin_IsHaveRward();
	int GetTLCanGetAward();



	int CalCanReward(int sigNum);

	//更新  累积登录数据
	void UpdateTotalLogin(bool Flag);

	//统计 签到  相关
//	void StatisticSign(eStatisticMainType main_enum, eLogicRelevant fun_enum);

	// 获取补签次数
	int GetMendingSignCnt()
	{
		return m_mendingSignCnt;
	}

	void setMendingSignCnt(int mendingSignCnt)
	{
		m_mendingSignCnt = mendingSignCnt;
	}

	bool isTodayHaveSignin();

	//是否领取过七日礼包，任意一种都可能过
	bool isSevenPackageHaveReceive();

	//是否领取过某个七日礼包，指定ID
	bool isSomeSevenPackageHaveReceive(int day);

	// 操作VIP升级事件
	bool HandleUpdateVipLv(const EventArgs& e);

private:

	Player * m_owner;
	CBitArray m_playerSignIn;
	CBitArray m_playerReward;

	int m_playerTotalLogin; //累积登录
	CBitArray m_playerTotalLoginReward; //累积登录奖励
	//----------------------------
	bool m_firstChargeRewardFlag ;  //是否 首充已经操作完

	// 补签次数
	int m_mendingSignCnt;

	EventConnection m_vipEvent;
};


#endif /* REWARDHAIL_H_ */
