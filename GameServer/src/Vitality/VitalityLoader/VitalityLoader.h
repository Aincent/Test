/*
 * VitalityLoader.h
 *
 *  Created on: 2015年3月17日
 *      Author: root
 */

#ifndef VITALITYLOADER_H_
#define VITALITYLOADER_H_

#include <map>
#include "define.h"
#include "Singleton.h"
#include "CharDefine.h"

using namespace std;
using namespace CommBaseOut;

struct sVitality
{
	int id;
	int count;
	int score;
	sVitality():id(-1),count(0),score(0)
	{

	}
};

struct sVitalityAward
{
	int id;
	int score;
	int golden;
	int money;

	vector<DWORD> m_nAward;			//奖励物品ID
	vector<DWORD> m_nAwardNum;			//对应奖励物品数量
	sVitalityAward():id(-1),score(0),golden(0),money(0)
	{

	}
};

struct sVitalityInfo
{
	int id;
	int count;
	sVitalityInfo():id(-1),count(0)
	{

	}
};


class VitalityLoader : public Singleton<VitalityLoader>
{
public:
	VitalityLoader()
	{

	}

	~VitalityLoader()
	{

	}

	int Init(string &file);
	int InitLivenessData(string &file);
	int InitLivenessRewardData(string &file);

	sVitality * GetVitalityInfoByID(int id)
	{
		map<int, sVitality>::iterator it = m_allVitality.find(id);
		if(it != m_allVitality.end())
		{
			return &it->second;
		}

		return 0;
	}

	sVitalityAward * GetVitalityAwardInfoByID(int id)
	{
		map<int, sVitalityAward>::iterator it = m_allVitalityAward.find(id);
		if(it != m_allVitalityAward.end())
		{
			return &it->second;
		}

		return 0;
	}

	//更新数据
	void UpdataVitality(map<int, sVitalityInfo> &vitalityInfo,map<int,bool> &isGetAward);
	//可以开启多个奖励
	int GetAwardCount(int score);

	int GetAwardIDByScore(int score);
private:
	map<int, sVitality> m_allVitality;
	map<int, sVitalityAward> m_allVitalityAward;
};


#endif /* VITALITYLOADER_H_ */
