/*
 * CountryRedEnvelopeLoader.h
 *
 *  Created on: 2015年4月16日
 *      Author: root
 */

#ifndef COUNTRYREDENVELOPELOADER_H_
#define COUNTRYREDENVELOPELOADER_H_

#include "Singleton.h"
#include "define.h"
#include "Lua/LuaInterface.h"
#include <map>

using namespace std;
using namespace CommBaseOut;

struct EnvelopeRewardInfo
{
	EnvelopeRewardInfo()
	{
		type = -1;
	}
	int type;
	std::vector<DWORD> itemId;
	std::vector<DWORD> num;
};

class CountryRedEnvelopeLoader : public Singleton<CountryRedEnvelopeLoader>
{
public:
	CountryRedEnvelopeLoader(): m_canJoinCountryLv(0), m_deleteEnvelopeTime(0), m_deleteEnvelopeDays(0), m_rechargeAddEnvelope(0), m_olineGetBindGoldenCount(0)
,m_offlineGetBindGoldenCount(0),m_accCountGotReward(0),m_getCountGotRewardItem_Id(0),m_accCountGotRewardId(0),m_getenvelopeCountPer10lv(0),m_getEnvelopeCountPerLvCount(0)
,m_maxCanAddEnvelopeCountLv(0)
	{
		m_lua.OpenLua();
	}
	~CountryRedEnvelopeLoader()
	{
		m_lua.CloseLua();
	}
public:
	int Init(string &path);

	int InitLua(string& file);

	//int InitCSVFile(string& file);

	int GetCanGetEnvelopeCountMax(int lv);

	int64 getDeleteEnvelopeTime() const
	{
		return m_deleteEnvelopeTime;
	}

	DWORD getAccCountGotReward() const
	{
		return m_accCountGotReward;
	}

	DWORD getAccCountGotRewardId() const
	{
		return m_accCountGotRewardId;
	}

	DWORD getCanJoinCountryLv() const
	{
		return m_canJoinCountryLv;
	}

	DWORD getDeleteEnvelopeDays() const
	{
		return m_deleteEnvelopeDays;
	}

	DWORD getGetCountGotRewardItemId() const
	{
		return m_getCountGotRewardItem_Id;
	}

	DWORD getGetenvelopeCountPer10lv() const
	{
		return m_getenvelopeCountPer10lv;
	}

	DWORD getGetEnvelopeCountPerLvCount() const
	{
		return m_getEnvelopeCountPerLvCount;
	}

	DWORD getMaxCanAddEnvelopeCountLv() const
	{
		return m_maxCanAddEnvelopeCountLv;
	}

	DWORD getOfflineGetBindGoldenCount() const
	{
		return m_offlineGetBindGoldenCount;
	}

	DWORD getOlineGetBindGoldenCount() const
	{
		return m_olineGetBindGoldenCount;
	}

	DWORD getRechargeAddEnvelope() const
	{
		return m_rechargeAddEnvelope;
	}

	void GetEnvelopeRewardByType(int type,EnvelopeRewardInfo &rewardInfo);
private:
	//20级可加入阵营
	DWORD m_canJoinCountryLv;
	//删除过期红包日期，毫秒
	int64 m_deleteEnvelopeTime;
	//每3天删除过期红包
	DWORD m_deleteEnvelopeDays;
	//30元宝增加一个可派发红包
	DWORD m_rechargeAddEnvelope;
	//在线红包能获取的元宝
	DWORD m_olineGetBindGoldenCount;
	//离线红包能获取的元宝
	DWORD m_offlineGetBindGoldenCount;
	//累积多少次可以获得累积红包
	DWORD m_accCountGotReward;
	//返利的礼包ID
	DWORD m_getCountGotRewardItem_Id;
	//累积返利的礼包ID
	DWORD m_accCountGotRewardId;
	//20级以上 60级以下 每10级增加5次可领取红包数
	DWORD m_getenvelopeCountPer10lv;
	//每10级
	DWORD m_getEnvelopeCountPerLvCount;
	//最多50级还可以怎加领取次数
	DWORD m_maxCanAddEnvelopeCountLv;
	//红包类型奖励
	std::map<int,EnvelopeRewardInfo> m_envelopeRewardType;

	CLuaInterface m_lua;
};

#endif /* COUNTRYREDENVELOPELOADER_H_ */
