#ifndef QUESTWEEK_H_
#define QUESTWEEK_H_

#include "define.h"
#include "CharLogin/PlayerInfo.pb.h"
#include "MessageCommonRet.h"
#include "QuestLoader.h"
#include "MessageStruct/Daily/Daily.pb.h"

class Player;
using namespace std;
using namespace CommBaseOut;

enum eWeekTaskSendFlag
{
	eWeekTaskSendNull 	= 0,
	eWeekTaskSendMiddle = 1,
	eWeekTaskSendEnd    = 2,
};

#define MAX_WEEKTASK_NUM 10			/// 周任务

class QuestWeek
{
public:
	QuestWeek(Player& owner);

	~QuestWeek();

public:
	//数据库中设置成员
	void onRead(const PlayerInfo::QuestInfo& info);

	void resetTask();

	bool isSameWeek();

	DWORD makeQuestID(DWORD questID);

	ServerRet onSubmit(DWORD questId);

	ServerRet updateTask();

	void updateToClient();

	void onWrite(PlayerInfo::QuestInfo* info);

	void levelUp(bool send);

	ServerRet finishOneTask(DWORD questID);

//	void getAllReward(vector<DWORD>& items,vector<DWORD>& nums);

	ServerRet oneComplete(DWORD questID);

//	int getMaxRing();

	int getCurTimes(){return m_Times;}	

	bool nextIsOver();

	void GetYesterdayWeekQuest(Daily::RecoverResourceInfoList * info);
	//资源追回后的操作
	void ResetYesterdayWeekQuest(void)
	{
		m_YesterdayTimes = MAX_WEEKTASK_NUM;
	};
private:
	int GiveSubmitReward();

//	void giveReward();

//	void makeSpecifyCycleReward();

//	void makeEndReward();

private:
	Player&			 m_Owner;
	DWORD64 		 m_Time;	  //重置时间
	WORD    		 m_Times;	  //环数
	BYTE			 m_YesterdayTimes;//昨天进行到第几环
};

#endif
