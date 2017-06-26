/*
 * VipLoader.h
 *
 *  Created on: 2014年8月11日
 *      Author: root
 */

#ifndef VIPLOADER_H_
#define VIPLOADER_H_

#include "define.h"
#include <map>
#include <vector>
#include <string>
#include "../Map/ConfigLoader/FieldInfo.h"
#include "Smart_Ptr.h"
#include "../Map/ConfigLoader/FieldMonster.h"
#include "Singleton.h"

using namespace CommBaseOut;
using namespace std;
class sVipEntry;

enum E_RECHARGE_STATUS
{
	eRechargeCantMayReceive = 0, // 不可领取
	eRechargeCanMayReceive, // 可以领取
	eRechargeDrawed // 已经领取
};

struct sRecharge
{
	int id;													//id
	uint day;											//所需要天数
	vector<DWORD> m_nAward;			//奖励物品ID
	vector<DWORD> m_nAwardNum;			//对应奖励物品数量
	sRecharge():id(-1),day(0)
	{

	}
};

class VipLoader : public Singleton<VipLoader>
{
public:
	VipLoader();
	~VipLoader();
public:
	int Init(string &file);
	int InitRecharge(string &file);

	int InitAwardState(map<int,int64> &info);
	sVipEntry *GetVipEntry(DWORD id);

	DWORD GetVipLevel(DWORD addUpGold);

	sRecharge *GetRecharge(DWORD id);
	sRecharge *GetRechargeByDay(int day);

private:
	map<DWORD, sVipEntry> m_VipInfo;   // sVipEntry - info
	map<int,sRecharge> m_RechargeInfo;
	DWORD m_nRows;
};



#endif /* VIPLOADER_H_ */
