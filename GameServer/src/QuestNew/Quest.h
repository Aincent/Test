/*
 * Quest.h
 *			任务对象
 *  Created on: 2014年10月20日
 *      Author: root
 */

#ifndef QUEST_H_
#define QUEST_H_

#include "QuestDefine.h"
#include "QuestCondition.h"
#include "QuestLoader.h"

using namespace std;
using namespace CommBaseOut;

class Player;

class Quest
{
	typedef std::vector<QuestCondition*> QuestConditionList;

	const Entry_Quest*	mEntry;					/// 任务配置
	Player*				mOwner;					/// 拥有该任务的玩家
	QuestConditionList	mQuestConditionList;	/// 该任务的条件列表
	QuestState			mQuestState;			/// 任务的状态
	DWORD				mNumStar;				/// 数量星级
	DWORD				mRewardStar;			/// 奖励星级
	float				m_NumCoefficient;

public:
	Quest(Player* owner, const Entry_Quest* entry, DWORD numStar = 1, DWORD rewardStar = 1);
	~Quest();

public:
	static const Entry_Quest* getEntry(uint questId);
	Player* getOwner() const				{ return mOwner; }
	const Entry_Quest* getEntry() const		{ return mEntry; }
	size_t getConditionCount() const		{ return mQuestConditionList.size(); }
	QuestCondition* getCondition(int idx) const;
	QuestState getQuestState() const		{ return mQuestState; }
	DWORD	getNumStar() const
	{
		if(mNumStar==0)return 1;
		return mNumStar;
	}
	DWORD getRewardStar() const { return mRewardStar; }
	void setNumStar(DWORD star)
	{
		if(star == 0)
			return ;
		mNumStar = star;
	}

	void setCoefficient(float coefficient){m_NumCoefficient = coefficient;}

	void setRewardStar(DWORD star) { mRewardStar = star; }
public:
	bool create(uint questState,eOnLineOrAccept type);
	bool canSubmit() const;

	/// 完成任务给予玩家奖励
	static int submitReward(Player* player, int questID, bool byMail = false);

	// 任务条件改变时，重新评估任务是否完成
	void updateCondition();

	/// 用于排序
	bool operator < (const Quest& rhs) const;

	int getCurrentValue();

	int getAllNum();

	void setFinish();

private:
	void setState(QuestState state);

	/// 任务状态或者条件改变
	void onStateOrConditionChanged();
};


#endif /* QUEST_H_ */
