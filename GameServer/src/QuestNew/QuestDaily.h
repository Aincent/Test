/*
 * QuestDaily.h
 *
 *  Created on: 2014年10月21日
 *      Author: root
 */

#ifndef QUESTDAILY_H_
#define QUESTDAILY_H_

#include "define.h"
#include "Quest.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include <vector>
#include "MessageStruct/Daily/Daily.pb.h"
#include "MessageStruct/Quest/QuestMessage.pb.h"


#define MAX_DAILYTASK_NUM 20			/// 日常最大接受任务数

using namespace std;
using namespace CommBaseOut;

class Player;

class DailyQuests
{
public:
	DailyQuests(Player* owner);
	~DailyQuests();

	void init();
	void uninit();

	//
	void setDirty(bool bVal) { mIsDirty = bVal; }
	uint getQuestId() const		{ return mQuestIdAccepted; }
	void setQuestId(const uint & questId);

	BYTE getNumStarLevel() const	{		return mNumStarLevel;	}

	void SetNumStarLevel(const BYTE& start)	{		mNumStarLevel = start;	}

	void setCoefficient(float coefficient){m_NumCoefficient = coefficient;}

	float getCoefficient(){return m_NumCoefficient;}

	BYTE getRewardStarLevel() const		{		return mRewardLevel; }
	void setRewardStarLevel(const BYTE &value)	{		mRewardLevel = value;	}

	BYTE getTimes() const			{ return mTimes; }
	void setTimes(BYTE value) { mTimes = value; }
	BYTE getMaxTimes() const		{ return mMaxTimes; }

	void IncreaseDoTime();
	void IncreaseDoMaxTime();

	void selectStarLevel(int type);

	/// 该日常任务是否可以接受
	bool canAccept(const Entry_Quest* entry, int &error) const;

	/// 接受一个日常任务
	void onAccept(uint questId);

	bool nextIsOver();
	/// 玩家提交日常任务
	bool onSubmit(uint questId);
	int GiveSubmitReward(const BYTE& i_RewardLevel);

	/// 重置玩家的日常任务次数，仅仅由QuestManager在跨日期时统一调用
	void reset();

	/// 升星
//	int refreshStarLevel(bool isFull,int type);

	Quest_CondStateInfo updateToClient();

	void GetDailyQuestConditionStateInfo(Quest_CondStateInfo *ptr);

	void SetUpdateToClientCmd(const Quest_CondStateInfo *ptr, QuestProto::UpdateDailyTask *info);

	//数据库处理
	bool onRead(const PlayerInfo::QuestInfo &info);
	bool InitYesterdayDailyQuest(const PlayerInfo::QuestInfo &info);
	bool InitLastDailyQuest(const PlayerInfo::QuestInfo &info);

	bool onWrite(PlayerInfo::QuestInfo *info);

	bool onSysActived();

	int GetTargetNum();
	//资源追回后的操作
	void ResetYesterdayDailyQuest(void)
	{
		m_YesterdayQuestIdAccepted = 0;
		m_YesterdayTimes = MAX_DAILYTASK_NUM;
	}
	uint YesterdayDailyQuestID(){return m_YesterdayQuestIdAccepted; }
	BYTE GetYesterdayDailyTimes(){	return m_YesterdayTimes; }
	void SetYesterdayDailyTimesMax(){ m_YesterdayTimes = mMaxTimes;}

	void GetYesterdayDailyQuest(Daily::RecoverResourceInfoList * info);
	void GetDailyQuest(Daily::DailyToDoInfoList * toClient);

	void initDailyQuestList();

	uint GetDoLastQuestID()const {	return m_lastDoQuestID; }
	int GetDoLastCondNum() const {	return m_lastDoCondNum; }
	void SetDoLastCondNum(const int &num){ m_lastDoCondNum = num;	}

	void SetDailyQuesToYesterday();

	// 获取未完成任务次数
	int GetUnCompleteNum();
private:
	void clear();
	int GetInitDailyQuestId(void);
	void setNeedUpdateToDB();

	Player*					mOwner;
	ushort					mResetTimes;			// 重置次数
	uint						mQuestIdAccepted;		// 日常任务ID
	BYTE						mTimes;			//该任务已完成次数
	BYTE						mNumStarLevel;			// 帮派任务数量的星级
	BYTE						mRewardLevel;				//帮派奖励星级
	BYTE						mMaxTimes;
	bool						mIsDirty;
	float						m_NumCoefficient;

	uint						m_YesterdayQuestIdAccepted;		//昨天的进行到的日常任务ID
	BYTE						m_YesterdayTimes;//昨天进行到第几环

	uint        m_lastDoQuestID;  //上一次进行的任务ID
	int         m_lastDoCondNum;  //上一次进行 的 条件完成值
};



#endif /* QUESTDAILY_H_ */
