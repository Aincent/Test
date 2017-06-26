/*
 * ClanWelfareLoader.h
 *
 *  Created on: 2016年2月23日
 *      Author: root
 */

#ifndef CLANWELFARELOADER_H_
#define CLANWELFARELOADER_H_

#include "define.h"
#include "Common.h"
#include "Singleton.h"

using namespace std;
using namespace CommBaseOut;

struct ClanWelfareInfo
{
	int ID;
	int price;
	struct Items
	{
		vector<DWORD> itemID;
		vector<DWORD> itemNum;
		vector<WORD> isBind;
		void Clear()
		{
			itemID.clear();
			itemNum.clear();
			isBind.clear();
		}
	};
	Items items;

	void Clear()
	{
		ID = 0;
		price = 0;
		items.Clear();
	}
};

struct ClanWelfareBase
{
	int welfareNum; // 总福利数量
	DWORD64 overTime;
	struct Limit
	{
		int floor;
		int ceil;
	};
	Limit lv;
	Limit joinTime;
	Limit contri;
	Limit sendNum;

//	// 帮派发送福利通知内容
//	int sendWelNoticeContent;
	// 到期福利返还通知主题
	int giveDueWelTheme;
	// 到期福利返还通知内容
	int giveDueWelContent;
};

class ClanWelfareLoader : public Singleton<ClanWelfareLoader>
{
public:
	ClanWelfareLoader();
	~ClanWelfareLoader();

	int Init(string& path);
	int InitClanWelfare(string& file);
	int InitWelfareBase(string& file);

	const ClanWelfareInfo* GetClanWelfareInfo(const int ID) const;
	const ClanWelfareBase* GetClanWelfareBase() const;

private:
	map<int, ClanWelfareInfo> m_clanWelfareInfo; // 帮派福利信息
	ClanWelfareBase m_clanWelfareBase;
};

#endif /* CLANWELFARELOADER_H_ */

