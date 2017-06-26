/*
 * AnswerLoader.h
 *
 *  Created on: 2016年11月10日
 *      Author: root
 */

#ifndef ANSWERLOADER_H_
#define ANSWERLOADER_H_

#include "Singleton.h"
#include "define.h"

using namespace std;
using namespace CommBaseOut;


struct AnswerItem
{
	AnswerItem() : m_ItemID(0),m_Counts(0),m_BindType(1){}
	int  m_ItemID;
	int  m_Counts;
	WORD m_BindType;
};

struct AnswerReward
{
	AnswerReward() : m_ID(0),m_Type(0),m_Limit(0),m_Value(0){}
	int m_ID;
	int m_Type;
	int m_Limit;
	int m_Value;
	vector<AnswerItem> m_ItemList;
};


class AnswerLoader : public Singleton<AnswerLoader>
{
public:
	AnswerLoader();
	virtual ~AnswerLoader();

	int init(string& file);

	const vector<int>& GetFielData(){return m_FileDataList;}

	const AnswerReward* GetAnswerReward(int nType,int nNeeds = 0);
private:
	int initAnswerFile(string& file);

	int initRewardFile(string& file);

private:
	vector<int>   m_FileDataList;  		//类型为索引
	map<int,vector<AnswerReward> > m_RewardDataList;	    //类型为索引
};


#endif /* ANSWERLOADER_H_ */
