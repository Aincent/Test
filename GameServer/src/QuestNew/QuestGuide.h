/*
 * QuestGuide.h
 *
 *  Created on: 23/04/2015
 *      Author: root
 */

#ifndef QUESTGUIDE_H_
#define QUESTGUIDE_H_
#include "define.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageCommonRet.h"

using namespace CommBaseOut;
class Player;
class Quest;
struct sGiveQuestRewardNeedData;

//引导任务
class QuestGuide
{
public:
	QuestGuide(Player* owner);

	~QuestGuide();

public:
	void updateToClient();

	ServerRet acceptQuest();

	//玩家升级时是否要接收一个任务
	bool isUpgradeIsAcceptQuest();

	bool onWrite(PlayerInfo::QuestInfo *info);

	void onRead(const PlayerInfo::QuestInfo &info);

	DWORD getNextGuideQuestID();

private:
	Player* m_Owner;
	DWORD   m_QuestID;//玩家当前身上任务ID；已做过的最后一个任务ID(此时玩家身上没有引导任务，由等级导致)；该ID为0，表示为等级未到，还不能开启引导任务
};


#endif /* QUESTGUIDE_H_ */
