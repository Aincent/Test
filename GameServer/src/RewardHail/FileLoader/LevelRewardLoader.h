/*
 * LevelRewardLoader.h
 *
 *  Created on: 2014年6月20日
 *      Author: root
 */

#ifndef LEVELREWARDLOADER_H_
#define LEVELREWARDLOADER_H_

#include <map>
#include "CSVLoader.h"

using namespace CommBaseOut;
using namespace std;

typedef struct sLevelRewardStruct
{
	int i_zhenqi;
	int i_money;
	int i_bglod;
	int i_lv;
	vector<DWORD> m_nAward;			    //奖励物品ID
	vector<DWORD> m_nAwardNum;			//对应奖励物品数量
	vector<DWORD> m_nVipAward; // vip奖励物品ID
	vector<DWORD> m_nVipAwardNum; // 对应的奖励物品数量
}LevelRewardStruct;

class LevelRewardLoader
{
public:

	~LevelRewardLoader();

	static LevelRewardLoader * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new LevelRewardLoader();
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
	int InitLevelRewardLoader(string & file);

	LevelRewardStruct * GetLevelRewardLoader(int id)
	{
		map<int , LevelRewardStruct>::iterator it = m_levelRewardStruct.find(id);
		if(it == m_levelRewardStruct.end())
		{
			return 0;
		}
		return &it->second;
	}

	const map<int, LevelRewardStruct>& getLevelDataList(){return m_levelRewardStruct;}

private:
	LevelRewardLoader();

private:

	static LevelRewardLoader * m_instance;
	map<int, LevelRewardStruct> m_levelRewardStruct;
};


#endif /* LEVELREWARDLOADER_H_ */
