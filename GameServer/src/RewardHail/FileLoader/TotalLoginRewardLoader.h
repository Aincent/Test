/*
 * TotalLoginRewardLoader.h
 *
 *  Created on: 2014年9月11日
 *      Author: root
 */

#ifndef TOTALLOGINREWARDLOADER_H_
#define TOTALLOGINREWARDLOADER_H_

#include <map>
#include "define.h"
#include "Singleton.h"
#include "CharDefine.h"

using namespace std;
using namespace CommBaseOut;


typedef struct sTotalLoginAw
{
	int day;
	int zhenqi; //真气
	int money; //钱
	int bglod;  //帮定元宝
	vector<DWORD> m_totalItem;
	vector<DWORD> m_totalNum;
	vector<DWORD> m_totalLv;
	sTotalLoginAw():day(0),zhenqi(0),money(0),bglod(0)
	{}
}TotalLoginAw;

typedef struct sFirstChargeAw
{
	int profession;
	int zhenqi; //真气
	int money; //钱
	int bglod;  //帮定元宝
	vector<DWORD> m_totalItem;
	vector<DWORD> m_totalNum;
	vector<DWORD> m_totalLv;
	sFirstChargeAw():profession(0),zhenqi(0),money(0),bglod(0)
	{}
}FirstChargeAw;

class TotalLoginRewardLoader: public Singleton<TotalLoginRewardLoader>
{
public:
	TotalLoginRewardLoader();
	~TotalLoginRewardLoader();

	int Init(string &path);

	int 	InitDate(string &file);

	int InitFirstChargeData(string &file);

	TotalLoginAw * GetTotalLoginAw(int day);
	const map<int , TotalLoginAw>& getTotalLoginData(){return m_allTotalReaw;}

	FirstChargeAw * GetFirstChargeAw(int profession);

private:
	map<int , TotalLoginAw> m_allTotalReaw;
	map<int , FirstChargeAw> m_allFirstCharge;
};


#endif /* TOTALLOGINREWARDLOADER_H_ */
