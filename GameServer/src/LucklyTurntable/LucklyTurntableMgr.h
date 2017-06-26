/*
 * LucklyTurntableMgr.h
 *
 *  Created on: 2014年8月20日
 *      Author: root
 */

#ifndef LUCKLYTURNTABLEMGR_H_
#define LUCKLYTURNTABLEMGR_H_

#include "Singleton.h"
#include "../EventSet/EventDefine.h"
#include "../Object/Player/Player.h"
#include "define.h"

#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/CharLogin/LucklyTurntable.pb.h"
#include "MessageStruct/RewardHail/RewardHail.pb.h"

#include "LucklyTurntableLoader.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"


using namespace CommBaseOut;

class Player;

		/*转盘*/
const int Luckly_BaseNum = 10000;   //转盘概率范围

class LucklyTurntableMgr
{
public:
	LucklyTurntableMgr(Player * player);
	~LucklyTurntableMgr();

	void ReInit();
	void Release();

	void InitFromDB(const PlayerInfo::LucklyTurntable &info);
	void SetLucklyTurntableInfo(PlayerInfo::LucklyTurntable *info);

	//进入  显示 界面 信息
	void EnterLucklyTurntable(LucklyTurntable::AckLucklyTurntable &info);
	bool IsCanEnter();
	//是否 超过 间隔时间
	bool IsOverInterTm(int intertm);
	//计算 还要 多久的毫秒数
	DWORD64 CalNeedTm(int intertm);
	//随机区一个 奖励ID
	void RandReaw();

	//刷新
	void RefulshLucklyTurntable(LucklyTurntable::AckRefulshOrGetLucklyTurntable &info);
	//领取
	void GetLucklyTurntable(LucklyTurntable::AckGetLucklyTurntable &info);

	//转 转盘   //为了 节约 数据结构  这里用了 符合结果要求的 协议体
	void RollLucklyTurntable(int num,RewardHail::ToClientGetReward &info);
	//计算消耗
	int CalRollCost(int num,const ProLucklyTurntable * proPtr, bool oneRollFlag);
	//减去 消耗  //返回 false 为 消耗 免费次数  true 为 元宝  是否 点击一次
	bool ChangeRollCost(int num,const ProLucklyTurntable * proPtr, bool oneRollFlag);
	//随即奖励
	void GetReawInfo(int num,list<int>& idList,bool isGolden);
	//填充 获得的 奖励数据
	void SetReawInfo(RewardHail::ToClientGetReward &info, list<int>&vec);
	//元宝变更
	void ChangGoldenAttr(int type);
	//跑马灯    广播
	void BroadCastLucklyTurntable(list<TurntableRward> &brocastVec);

	//是否还有免费转转盘次数
	int LucklyTurntable_IsHaveFreeNum();

	//产生比较分散的随机数
	static int ProduceRandNum(int randScop);

	//打乱一个列表
	template<class T>
	void randOneList(vector<T>& dataList);

private:
	Player* 			 m_owner;
	DWORD64 			 enterTm; 		 //进入  时间
	vector<int> 		 m_RewardList;   //显示奖励ID
	list<TurntableRward> m_tmpAward;     //临时奖励
};


#endif
