/*
 * QuestClan.h
 *
 *  Created on: 2014年10月21日
 *      Author: root
 */

#ifndef QUESTCLAN_H_
#define QUESTCLAN_H_

#include "define.h"
#include "QuestLoader.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/Daily/Daily.pb.h"
#include <vector>
#include "MessageStruct/Quest/QuestMessage.pb.h"
#include "MessageStruct/Daily/Daily.pb.h"


#define MAX_CLANTASK_NUM 20			/// 帮派任务

using namespace std;
using namespace CommBaseOut;

class Player;
class Quest;

class QuestClan
{
public:
	QuestClan(Player* owner);
	~QuestClan();

	void init();
	void uninit();

	void setDirty(bool bVal) { mIsDirty = bVal; }

	uint getQuestId() const		{ return mQuestId; }
	void setQuestId(const uint & questId);


	BYTE getNumStarLevel() const	{		if(mNumStarLevel == 0) return 1;		return mNumStarLevel;	}

	void setNumStarLevel(const BYTE& value)	{		mNumStarLevel = value;	}

	void setCoefficient(float coefficient){m_NumCoefficient = coefficient;}

	float getCoefficient(){return m_NumCoefficient;}

	BYTE getRewardStarLevel() const		{ return mRewardLevel; }
	void setRewardStarLevel(const BYTE& value)	{		mRewardLevel = value;	}

	BYTE getTimes() const	{		return mTimes;	}
	void setTimes(const BYTE& value){		mTimes = value;	}
	BYTE getMaxTimes() const		{ return mMaxTimes; }
	void IncreaseDoTime();
	int IncreaseDoMaxTime();

	/// 该日常任务是否可以接受
	bool canAccept(const Entry_Quest* entry, int error) const;

	/// 玩家提交帮派任务
	bool onSubmit(const uint & questId);
	bool nextIsOver();

	int GiveSubmitReward(const BYTE i_RewardLevel);


	/// 重置玩家的帮派任务，仅仅在跨日期时统一调用
	void reset();

	/// 刷星, isFull表明是否直接满级，加入isFull为false，type选择刷新的类型
//	int refreshStarLevel(bool isFull,int type);

	Quest_CondStateInfo updateToClient();

	void GetClanQuestConditionStateInfo(Quest_CondStateInfo *ptr);

	void SetUpdateToClientCmd(const Quest_CondStateInfo *ptr, QuestProto::UpdateClanTask *info);

	bool onRead(const PlayerInfo::QuestInfo &info);
	bool InitYesterdayClanQuest(const PlayerInfo::QuestInfo &info);
	bool InitLastDailyQuest(const PlayerInfo::QuestInfo &info);

	bool onWrite(PlayerInfo::QuestInfo *info);

	bool onSysActived();

	//一键完成
	int directComplete();
	//
	bool evalAcceptedQuest();

	int GetTargetNum();
	//资源追回后的操作
	void ResetYesterdayClanQuest(void)
	{
		m_YesterdayQuestIdAccepted = 0;
		m_YesterdayTimes = MAX_CLANTASK_NUM;
	};
	uint GetYesterdayQuestID(){ return  m_YesterdayQuestIdAccepted;}

	BYTE GetYesterdayTimes(){return m_YesterdayTimes;}
	void SetYesterdayClanimesMax(){ m_YesterdayTimes = mMaxTimes;}

	void GetYesterdayClanQuest(Daily::RecoverResourceInfoList * info);
	void GetClanQuest(Daily::DailyToDoInfoList * info);

	void selectQuest();

	uint GetDoLastQuestID()const {	return m_lastDoQuestID; }
	int GetDoLastCondNum() const {	return m_lastDoCondNum; }
	void SetDoLastCondNum(const int &num){ m_lastDoCondNum = num;	}

	void SetClanQuestToYesterday();

	void initResetQuest();

	// 获取未完成任务次数
	int GetUnCompleteNum();
private:
	void clear();
	void selectStarLevel(int type);
	int GetInitClanQuestId(void);
	void setNeedUpdateToDB();

	Player*					mOwner;
	bool					mIsDirty;
	uint					mBackExp;			/// 找回经验
	uint					mQuestId;			/// 帮派任务等级
	BYTE					mNumStarLevel;			/// 帮派任务数量的星级
	BYTE					mRewardLevel;				///帮派奖励星级
	BYTE					mTimes;				/// 已完成帮派任务次数
	BYTE					mMaxTimes;			/// 最大次数
	float						m_NumCoefficient;
	uint						m_YesterdayQuestIdAccepted;		//昨天的进行到的日常任务ID
	BYTE						m_YesterdayTimes;//昨天进行到第几环

	int         m_lastDoQuestID;  //上一次进行的任务ID
	int         m_lastDoCondNum;  //上一次进行 的 条件完成值
};




#endif /* QUESTCLAN_H_ */
