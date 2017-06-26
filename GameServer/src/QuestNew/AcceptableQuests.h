/*
 * AcceptableQuests.h
 *
 *  Created on: 2014年10月21日
 *      Author: root
 */

#ifndef ACCEPTABLEQUESTS_H_
#define ACCEPTABLEQUESTS_H_

#include "define.h"
#include <list>
#include "QuestDefine.h"

using namespace std;
using namespace CommBaseOut;
/**
	将接任务的条件作为触发源，触发系统枚举出玩家的可接任务，并同步到客户端
	已知的几种触发源：
	1、等级提升；
	2、完成任务（导致主任务级别的提升）；
	3、日常任务重置；
	4、活动重置；
*/

class Player;

class AcceptableQuests
{
	typedef std::list<uint>			AcceptableQuestList;

	Player*				mOwner;

	AcceptableQuestList	mAcceptableQuestList;	/// 可接任务列表
	DWORD 				mLastMainTaskID;										/// 上一次主线任务的ID
	bool	mIsDirty;

public:
	AcceptableQuests(Player* owner);
	~AcceptableQuests();

	void init();
	void uninit();

//	void makeAcceptableQuestList(bool hasMainQuest, int &error);

	void add(uint questId);
	void remove(uint questId);

	void updateToClient();

	//前一次的主线任务
	DWORD getLastMainTaskID() { return mLastMainTaskID; }
	void  setLastMainTaskID(DWORD id) { mLastMainTaskID = id; }

private:
	void clear();
};


#endif /* ACCEPTABLEQUESTS_H_ */
