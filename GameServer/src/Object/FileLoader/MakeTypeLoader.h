/*
 * MakeTypeLoader.h
 *
 *  Created on: 2014年6月20日
 *      Author: root
 */

#ifndef MAKETYPELOADER_H_
#define MAKETYPELOADER_H_
#include <map>
#include "define.h"

using namespace std;
using namespace CommBaseOut;

struct HaveToDoInfo
{
	int id;
	int maxCount;
	int score;
	HaveToDoInfo():id(0),maxCount(0),score(0)
	{
	}
};
class haveToDoLoader
{
public:
	~haveToDoLoader();

	int Init(string &file);

	int InitItemTypeData(string &file);

	static haveToDoLoader * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new haveToDoLoader();
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

	void GetItemTypeDataByID(int id, HaveToDoInfo &info)
	{
		map<int, HaveToDoInfo>::iterator it = m_HaveToDoInfo.find(id);
		if(it != m_HaveToDoInfo.end())
		{
			info = it->second;
		}
	}
	int GetCount(){return m_HaveToDoInfo.size();}
private:
	haveToDoLoader();

private:
	static haveToDoLoader * m_instance;

	map<int, HaveToDoInfo> m_HaveToDoInfo;
};

#endif /* MAKETYPELOADER_H_ */
