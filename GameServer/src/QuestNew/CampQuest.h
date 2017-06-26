/*
 * CampQuest.h
 *
 *  Created on: 25/03/2015
 *      Author: root
 */

#ifndef CAMPQUEST_H_
#define CAMPQUEST_H_

#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageCommonRet.h"
#include "define.h"
#include "MessageStruct/Quest/QuestMessage.pb.h"
#include "QuestDefine.h"
#include "MessageStruct/Daily/Daily.pb.h"

using namespace CommBaseOut;

#define MAX_CAMPQUEST_NUM  20

struct Entry_Quest;
struct Quest_CondStateInfo;


class Player;

class CampQuest
{
public:
	CampQuest(Player& owner);

	~CampQuest();

public:
	//数据库中设置成员
	void onRead(const PlayerInfo::QuestInfo &info);

	// 随机生成一个任务及其星级
	void selectQuest();

	void reset();

	bool nextIsOver();

	ServerRet GiveSubmitReward(const BYTE rewardLevel);

	void setCoefficient(float coefficient){m_NumCoefficient = coefficient;}

	float getCoefficient(){return m_NumCoefficient;}

	bool canAccept(const Entry_Quest* entry, int error) const;

	//玩家提交阵营任务
	bool onSubmit(const DWORD questId);

	//更新阵营任务到客户端
	Quest_CondStateInfo updateToClient();

	bool getClientData(QuestProto::UpdateCampTask& info);

	void IncreaseDoMaxTime();

	/// 刷星, isFull表明是否全部刷满级，type即选择刷满的类型
//	ServerRet refreshStarLevel(bool isFull,int type);

	bool onWrite(PlayerInfo::QuestInfo* info);

	void GetCampQuestConditionStateInfo(Quest_CondStateInfo *ptr);

	//用保存上一次任务ID，如果清除了任务，初始化包裹后再根据该值判是否删除收集物品
	bool InitLastDailyQuest(const PlayerInfo::QuestInfo &info);
	bool InitYesterdayDailyQuest(const PlayerInfo::QuestInfo &info);

	uint GetDoLastQuestID()const {	return m_lastDoQuestID; }
	int GetDoLastCondNum() const {	return m_lastDoCondNum; }
	void SetDoLastCondNum(const int &num){ m_lastDoCondNum = num;}

public:

	void setTimes(BYTE currentTimes){m_CurrentTimes = currentTimes;}

	BYTE getTimes() const {return m_CurrentTimes;}

	void setNumStarLevel(BYTE numTimes){m_NumTimes = numTimes;}

	BYTE getNumStarLevel() const {return m_NumTimes;}

	void setRewardStarLevel(BYTE rewardStar){m_RewardStar = rewardStar;}

	BYTE getRewardStarLevel() const {return m_RewardStar;}

	void setQuestId(DWORD questID){m_QuestID = questID;}

	DWORD getQuestId() const {return m_QuestID;}

	void setMaxTimes(BYTE maxTimes){}

	BYTE getMaxTimes() const {return MAX_CAMPQUEST_NUM;}

	int GetTargetNum();

	// 获取未完成任务次数
	int GetUnCompleteNum();

	void GetYesterdayCampQuest(Daily::RecoverResourceInfoList * info);
	//资源追回后的操作
	void ResetYesterdayCampQuest(void)
	{
		m_YesterdayQuestIdAccepted = 0;
		m_YesterdayTimes = MAX_CAMPQUEST_NUM;
	};

protected:

	void selectStarLevel(eRefreshStarType type);

	int GetInitCampQuestId(void);
private:
	BYTE    m_CurrentTimes;		//当前次数
	BYTE    m_NumTimes;			//条件次数，随机生成
	BYTE    m_RewardStar;		//奖励星级
	DWORD   m_QuestID;			//任务ID
	float	m_NumCoefficient;
	Player& m_Owner;
	uint						m_YesterdayQuestIdAccepted;		//昨天的进行到的日常任务ID
	BYTE						m_YesterdayTimes;//昨天进行到第几环

	DWORD   m_lastDoQuestID;    //上一次进行的任务ID
	int     m_lastDoCondNum;    //上一次进行 的 条件完成值
};


#endif /* CAMPQUEST_H_ */
