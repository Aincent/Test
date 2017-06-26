/*
 * AnswerMgr.h
 *
 *  Created on: 2016年11月10日
 *      Author: root
 */

#ifndef ANSWERMGR_H_
#define ANSWERMGR_H_

#include "define.h"
#include "util.h"
#include "MessageStruct/Answer/AnswerProto.pb.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "Log/Logger.h"
#include "MessageStruct/Daily/Daily.pb.h"

using namespace std;
using namespace CommBaseOut;

#define ANSWER_RIGHT_INDEX 1 	//正确索引
#define ANSWER_TITLE_SIZE 20

class Player;
class AnswerMgr
{
public:

	enum eTitleType
	{
		eTitleTypeNull = 0,
		eTitleTypeA    = 1,
		eTitleTypeB    = 2,
		eTitleTypeC    = 3,
	};

	enum eRewardCondition
	{
		eConditionNull       = 0,
		eConditionAnswer      = 1,
		eConditionStatistics      = 2,
	};


public:
	AnswerMgr(Player* obj);
	~AnswerMgr();

	void InitAnswerInfo(const PlayerInfo::AnswerInfo& answerInfo);
	void SetAnswerInfo(PlayerInfo::AnswerInfo *answerInfo);
	void ReleaseAnswerMgr();
	void ResetAnswerInfo();

	void GetAnswerInfo(AnswerProto::AckReqAnswerInfo& info);

	void ChoiceAnswer(int nIndex,AnswerProto::AckReqAnswerInfo& info);

	int RecoverResource(int num, Daily::RecoverResourceInfo *info);
private:
	void FlushTitle();
	void MakeOneTitle();
	void StatisticsReward();
private:
	int m_CurrentIndex; //当前第几题
	int m_CurrentTitleID;		//当前题目
	int m_RightCounts;
	int m_WrongCounts;
	Player * m_owner;
	vector<int>	m_TitleList;			//题目
};


#endif /* ANSWERMGR_H_ */
