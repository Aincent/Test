/*
 * AcceptedQuest.h
 *
 *  Created on: 2014年10月21日
 *      Author: root
 */

#ifndef ACCEPTEDQUEST_H_
#define ACCEPTEDQUEST_H_

#include "Quest.h"
#include <list>
#include "MessageStruct/CharLogin/CharBaseInfo.pb.h"
#include "MessageStruct/Quest/QuestMessage.pb.h"
#include "Smart_Ptr.h"

class QuestPart;

class AcceptedQuests
{
public:

	typedef std::list<Smart_Ptr<Quest> >	QuestList;

	AcceptedQuests(Player* owner);
	~AcceptedQuests();

	void init();
	void uninit();

	Quest* addQuest(uint questId, uint questState,eOnLineOrAccept type, DWORD numStar = 1,DWORD rewardStar = 1);
	Quest* findQuest(uint questId) const;
	void removeQuest(uint questId);
	Quest* findDailyQuest() const;
	Quest* findClanQuest() const;
	Quest* findCampQuest() const;
	DWORD findQuestByType(QuestType type);
	bool hasMainQuest() const;
	uint findMainQuest() const;
	size_t getCount() const		{ return mQuestList.size(); }

	Quest_CondStateInfo updateToClient();

	void setDirty(bool setting);
	bool isDirty() const					{ return mIsDirty; }

	//todayFlag 是否 同一天
	bool onRead(const PlayerInfo::QuestInfo &info,bool todayFlag);

	bool InitAccptedQuest(const PlayerInfo::ActiveQuest &newActive, bool todayFlag);
	bool CheckNowQuest(uint questId, uint questState, DWORD numStar,DWORD rewardStar,int cond1Value,float numCoefficient);

	bool onWrite(PlayerInfo::QuestInfo *info, const bool & flag);

	void ClearNpcFreshMonster();

	//删除等级超过的任务
//	void DeleteBranchLoseTask();

private:
	void clear();

	Player*				mOwner;
	QuestList			mQuestList;			/// 已接任务
	bool				mIsDirty;			/// 脏标志s
};




#endif /* ACCEPTEDQUEST_H_ */
