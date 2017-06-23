/*
 * InterfaceAnswerLoader.h
 *
 *  Created on: 15/07/2015
 *      Author: root
 */

#ifndef INTERFACEANSWERLOADER_H_
#define INTERFACEANSWERLOADER_H_

#include "Singleton.h"
#include "define.h"

using namespace std;
using namespace CommBaseOut;

enum eTitleType
{
	eTitleTypeNull = 0,
	eTitleTypeA    = 1,
	eTitleTypeB    = 2,
	eTitleTypeC    = 3,
};

struct AnswerFileData
{
	AnswerFileData() : m_ID(0),m_Type(0){}
	int   m_ID;
	short m_Type;
};

struct AnswerItemData
{
	AnswerItemData() : m_ItemID(0),m_Counts(0),m_BindType(1){}
	int  m_ItemID;
	int  m_Counts;
	WORD m_BindType;
};

struct AnswerRewardData
{
	AnswerRewardData() : m_ID(0),m_Type(0),m_UpperLimit(0),m_LowerLimit(0){}
	int m_ID;
	BYTE m_Type;
	int m_UpperLimit;
	int m_LowerLimit;
	vector<AnswerItemData> m_ItemList;
};

struct AnswerOtherData
{
	AnswerOtherData() :
		m_AnswerCounts(0),
		m_VipDoubleCounts(0),
		m_VipDoubleGolden(0),
		m_VIPExcludeCounts(0),
		m_VIPExcludeGolden(0),
		m_RightAnswerScore(0),
		m_RankingCunts(0),
		m_CountDownTime(0),
		m_ChoiceTime(0),
		m_ResultTime(0),
		m_DestroyTime(0),
		m_MailID(0),
		m_dayAnswerCnt(0)
	{}

	int m_AnswerCounts;				//每次答题总数
	int m_VipDoubleCounts;  		//VIP积分翻倍次数
	int m_VipDoubleGolden;
	int m_VIPExcludeCounts;     	//排错次数
	int m_VIPExcludeGolden;
	int m_RightAnswerScore;    	 	//答对一题获得分数
	int m_RankingCunts;				//排名显示个数
	int m_CountDownTime;			//倒计时时间
	int m_ChoiceTime;				//选题时间
	int m_ResultTime;				//结果显示时间
	int m_DestroyTime;				//答题结束后销毁时间
	int m_MailID;					//邮件内容ID
	map<short,int> m_StraightList;	//连胜对应积份
	int m_dayAnswerCnt; // 每天答题活动次数
};

enum eRwardAnswerType
{
	eRwardAnswerNULL  = 0,
	eRwardAnswerRank  = 1,	//排名奖励
	eRwardAnswerTitle = 2,	//题数奖励
};

struct AnswerProbability
{
	AnswerProbability() : m_ID(0),m_Counts(0){}
	int m_ID;
	int m_Counts;
};

class InterfaceAnswerLoader : public Singleton<InterfaceAnswerLoader>
{
public:
	InterfaceAnswerLoader();

	~InterfaceAnswerLoader();

	int init(string& file);

	const AnswerFileData* getFileDataByID(int id);

	const AnswerRewardData* getRewardDataByID(int id);

	const vector<AnswerProbability>& getProbabilityData(){return m_ProbabilityList;}

	const map<short,vector<AnswerFileData> >& getFielData(){return m_FileDataList;}

	const AnswerOtherData& getOtherData(){return m_OtherData;}

	const map<eRwardAnswerType,vector<AnswerRewardData> >& getRewardData(){return m_RewardDataList;}

	void getItemByItemID(int id,vector<DWORD>& itemIDList,vector<DWORD>& numList,vector<WORD>& bindTypeList);

private:
	int initAnswerFile(string& file);

	int initRewardFile(string& file);

	int initOtherFile(string& file);

	int initProbablityFile(string& file);

private:
	map<short,vector<AnswerFileData> >   			m_FileDataList;  		//类型为索引
	map<eRwardAnswerType,vector<AnswerRewardData> > m_RewardDataList;	    //类型为索引
	AnswerOtherData									m_OtherData;
	vector<AnswerProbability>						m_ProbabilityList;
};


#endif /* INTERFACEANSWERLOADER_H_ */
