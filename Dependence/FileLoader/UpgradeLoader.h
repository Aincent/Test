/*
 * UpgradeLoader.h
 *
 *  Created on: 2013年12月25日
 *      Author: helh
 */

#ifndef UPGRADELOADER_H_
#define UPGRADELOADER_H_

#include <vector>
#include "define.h"

using namespace std;
using namespace CommBaseOut;

typedef struct sUpgradeInfo
{
	WORD lv;
	int64 upgradeExp; //升级经验
	int64 allExp; //累积经验
	int64 factoryExp;//计算一系列奖励的经验值系数

	sUpgradeInfo():lv(0),upgradeExp(-1),allExp(-1),factoryExp(0)
	{

	}
}UpgradeInfo;

class UpgradeLoader
{
public:
	~UpgradeLoader();

	int Init(string &file);

	static UpgradeLoader * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new UpgradeLoader();
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

	void GetUpgradeByLv(int lv, UpgradeInfo &info)
	{
		if(lv > (int)m_upgrade.size() || lv <= 0)
		{
			return ;
		}

		info = m_upgrade[lv - 1];
	}

	void GetUpgradeByExp(int64 exp, UpgradeInfo &info)
	{
		for(WORD i=0; i<m_upgrade.size(); ++i)
		{
			if(exp < m_upgrade[i].allExp)
			{
				info = m_upgrade[i];
				return;
			}
		}
		info = m_upgrade[m_upgrade.size()-1];
	}

	uint GetMaxLv(){	return m_upgrade[m_upgrade.size()-1].lv;	}

private:
	UpgradeLoader();

private:

	static UpgradeLoader * m_instance;

	vector<UpgradeInfo> m_upgrade;
};



#endif /* UPGRADELOADER_H_ */
