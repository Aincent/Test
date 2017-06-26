/*
 * SysSettingLoader.h
 *
 *  Created on: 2015年4月8日
 *      Author: root
 */

#ifndef SYSSETTINGLOADER_H_
#define SYSSETTINGLOADER_H_

#include <map>
#include "define.h"
#include "Singleton.h"
#include "CharDefine.h"

using namespace std;
using namespace CommBaseOut;

class SysSettingLoader : public Singleton<SysSettingLoader>
{
public:
	SysSettingLoader()
	{

	}

	~SysSettingLoader()
	{

	}

	int Init(string &file);

	int GetLvByType(int type)
	{
		int lv = 0;
		map<int,int>::iterator it = m_SysSettingInfo.find(type);
		if(it != m_SysSettingInfo.end())
		{
			lv = it->second;
		}
		return lv;
	}
private:
	map<int,int> m_SysSettingInfo;
};


#endif /* SYSSETTINGLOADER_H_ */
