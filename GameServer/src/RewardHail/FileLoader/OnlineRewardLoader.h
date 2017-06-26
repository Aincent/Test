/*
 * OnlineRewardLoader.h
 *
 *  Created on: 2014年6月19日
 *      Author: root
 */

#ifndef ONLINEREWARDLOADER_H_
#define ONLINEREWARDLOADER_H_

#include <map>
#include "CSVLoader.h"

using namespace CommBaseOut;
using namespace std;

typedef struct sOnlineRewardStruct
{
	int i_online;
	int i_zhenqi;
	int i_money;
	int i_bglod;
	vector<DWORD> m_nAward;			    //奖励物品ID
	vector<DWORD> m_nAwardNum;			//对应奖励物品数量
	int i_left;
}OnlineRewardStruct;

class OnlineRewardLoader
{
public:

	~OnlineRewardLoader();

	static OnlineRewardLoader * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new OnlineRewardLoader();
		}

		return m_instance;
	}

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}


	int Init(string & path);
	int InitOnlineRewardLoader(string & file);

	OnlineRewardStruct * GetOnlineRewardLoader(int id)
	{
		map<int , OnlineRewardStruct>::iterator it = m_onlineRewardStruct.find(id);
		if(it == m_onlineRewardStruct.end())
		{
			return 0;
		}
		return &it->second;
	}

	const map<int, OnlineRewardStruct>& getOnLineRewardData(){return m_onlineRewardStruct;}

private:
	OnlineRewardLoader();

private:

	static OnlineRewardLoader * m_instance;
	map<int, OnlineRewardStruct> m_onlineRewardStruct;
};

#endif /* ONLINEREWARDLOADER_H_ */
