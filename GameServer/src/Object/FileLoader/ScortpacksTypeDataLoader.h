/*
 * ScortpacksTypeDataLoader.h
 *
 *  Created on: 2014年6月20日
 *      Author: root
 */

#ifndef SCORTPACKSTYPEDATALOADER_H_
#define SCORTPACKSTYPEDATALOADER_H_
#include <map>
#include "define.h"
#include <vector>
using namespace std;
using namespace CommBaseOut;

struct ScortpacksInfo
{
	int score;
	vector<int64> item;
	int copper;
	ScortpacksInfo():score(-1),copper(0)
	{
//		memset(this, 0, sizeof(*this));
	}

};

class ScortpacksLoader
{
public:
	~ScortpacksLoader();

	int Init(string &file);

	int InitItemTypeData(string &file);

	static ScortpacksLoader * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new ScortpacksLoader();
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

	void GetItemTypeDataByID(int id, ScortpacksInfo &info)
	{
		if(id < 0 || id >= (int)m_HaverToDoInfo.size())
			return;

		info = m_HaverToDoInfo[id];
	}
	int GetCount(){ return m_HaverToDoInfo.size();}
private:
	ScortpacksLoader();

private:
	static ScortpacksLoader * m_instance;

	vector<ScortpacksInfo> m_HaverToDoInfo;
};



#endif /* SCORTPACKSTYPEDATALOADER_H_ */
