/*
 * OnLineAtTimeLoader.h
 *
 *  Created on: 2016年2月22日
 *      Author: root
 */

#ifndef ONLINEATTIME_H_
#define ONLINEATTIME_H_

#include "define.h"
#include "Singleton.h"

using namespace CommBaseOut;
using namespace std;

struct OnLineInfo
{
	int id;
	BYTE atTimeHour;
	BYTE atTimeMinute;
	std::vector<int64> items;
	std::vector<int> nums;
	OnLineInfo():id(0),atTimeHour(0),atTimeMinute(0)
	{

	}
};

class OnLineAtTimeLoader : public Singleton<OnLineAtTimeLoader>
{
public:
	OnLineAtTimeLoader();
	virtual ~OnLineAtTimeLoader();

	int Init(string &path);

	void initTimePoint(OnLineInfo& info,const string& str);

	void initRewardInfo(OnLineInfo& info,const string& str);

	const map<int, OnLineInfo>& getOnLineInfoList() const
	{
		return m_OnLineInfoList;
	}

private:
	map<int,OnLineInfo> m_OnLineInfoList;
};

#endif /* ONLINEATTIME_H_ */
