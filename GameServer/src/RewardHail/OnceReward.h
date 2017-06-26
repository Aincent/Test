/*
 * OnceReward.h
 *
 *  Created on: 2015年9月16日
 *      Author: root
 */

#ifndef ONCEREWARD_H_
#define ONCEREWARD_H_

#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "Smart_Ptr.h"
#include "define.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/RewardHail/RewardHail.pb.h"
#include "StatisticBase/StatisticDefine.h"

using namespace std;
using namespace CommBaseOut;

class Player;
class OnceReward
{
public:
	OnceReward(Player* player);
	~OnceReward();

	void ReInit();
	void Release();

	void SetInfoToDB(PlayerInfo::OnceRewardInfo& rewardInfo);
	void InitInfoByDB(const PlayerInfo::OnceRewardInfo& rewardInfo);


	// 同步奖励到客户端
	void SynRewardToClient();

	// ---------------------- 首充由于前端原因 暂时不移植过来新模块中 --------------
	// 判断是否是首次充值
	bool IsFirstChargeRmb();
	//
	void SetFirstChargeFlag(bool flag);
	// 领取首充奖励
	int GetFirstChargeRmbReward(RewardHail::GetReward& reward, vector<DWORD>& goods, vector<DWORD>& num,
			vector<DWORD>& strengthLvVec);

	void SetRewardItemInfo(RewardHail::GetReward& info,	vector<DWORD>& goods);

	// 获得微端奖励状态 UI高亮用到的
	int GetMicroRewardState();
	// 设置
	void SetMicroRewardFlag(bool flag);
	// 检查领取微端奖励条件
	int CheckGetMicroReward();
	// 领取微端奖励
	int GetMicroReward(RewardHail::GetReward& reward, vector<DWORD>& goods, vector<DWORD>& num, vector<WORD>& bind);
	// 统计
//	void Statistic(eStatisticMainType main_enum, eLogicRelevant fun_enum);

private:
	// 判断是否用微端登录过
	bool IsMicroLogin();

private:
	Player* m_owner;

	bool m_microRewardFlag;
	bool m_firstChargeRewardFlag;
};

#endif /* ONCEREWARD_H_ */
