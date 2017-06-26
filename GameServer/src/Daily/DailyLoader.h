/*
 * DailyLoader.h
 *
 *  Created on: 2014年12月5日
 *      Author: root
 */

#ifndef DAILYLOADER_H_
#define DAILYLOADER_H_

#include "define.h"
#include <map>
#include <vector>
#include "Singleton.h"
#include "CharDefine.h"

using namespace CommBaseOut;
using namespace std;

struct sDailyPracticeParam
{
	sDailyPracticeParam()
	{
		nId = 0;
		nLv = 0;
		nExp = 0;
	}
	int nId;
	int nLv;
	int nExp;
};

struct sRecorceSourceParam
{
	int id;
	float retrieve;
	float accerretireve;
	int golden;

	sRecorceSourceParam():id(0),retrieve(0),accerretireve(0),golden(0)
	{

	}
};

enum e_RecorceSourceEnum
{
	e_RecorceSource_exp =1,
	e_RecorceSource_force,
	e_RecorceSource_money,
	e_RecorceSource_clanfun,
	e_RecorceSource_item,
};

enum eDailyToDoType
{
	eDailyToDoType_dailyQuest = 1,
	eDailyToDoType_worldBoss,
//	eDailyToDoType_yuwenfu,
//	eDailyToDoType_mamingguang,
//	eDailyToDoType_yingxiongshilian,
//	eDailyToDoType_zhuyeling,
//	eDailyToDoType_gumuguijing,
//	eDailyToDoType_hulaoguang,
	eDailyToDoType_singleInstance,
	//eDailyToDoType_groupInstance,
	eDailyToDoType_treason,//剿灭反王
	eDailyToDoType_protectKing,
	eDailyToDoType_clanQuest,
	eDailyToDoType_moneyShop,
	eDailyToDoType_challengeInstance,
	eDailyToDoType_escort,
	eDailyToDoType_luckylyTurnTable,
	eDailyToDoType_arena,
	eDailyToDoType_clanWar,
	eDailyToDoType_answer,
};

enum eRecorceSourceParamType
{
	eRecorceSourceParamType_dailyQuest = 1,
	eRecorceSourceParamType_clanQuest,
	eRecorceSourceParamType_campQuest,
	eRecorceSourceParamType_weekQuest,
	eRecorceSourceParamType_moneyShop,
	eRecorceSourceParamType_answer,
	eRecorceSourceParamType_singleInstance,
	eRecorceSourceParamType_moneyInstance,
	eRecorceSourceParamType_expInstance,
	eRecorceSourceParamType_advancedInstance,
	eRecorceSourceParamType_arena,


	eRecorceSourceParamType_storyInstance,
	eRecorceSourceParamType_groupInstance,
	eRecorceSourceParamType_challengeInstance,
	eRecorceSourceParamType_escort,


	eRecorceSourceParamType_protectKing,
	eRecorceSourceParamType_cityReward,
	eRecorceSourceParamType_max,
};

class DailyLoader : public Singleton<DailyLoader>
{
public:
	DailyLoader();
	~DailyLoader();
public:
	int InitDaily(string &file);
	int InitDailyParam(string &file);
	int GetParcticeParamExp(int nLv);

	int InirRecorceSource(string &file);

	sRecorceSourceParam * GetRecorceSourceParamPtr(const int& id)
	{
		map<int , sRecorceSourceParam>::iterator it = m_allRecorceSource.find(id);
		if(it != m_allRecorceSource.end())
			return &it->second;
		return NULL;
	}

private:
	map<int ,sDailyPracticeParam> m_practiceParam;//闭关修炼可以获取经验的计算系数

	map<int , sRecorceSourceParam> m_allRecorceSource;
};


#endif /* DAILYLOADER_H_ */
