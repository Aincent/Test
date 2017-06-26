/*
 * PayLoader.h
 *
 *  Created on: 2015年6月17日
 *      Author: root
 */

#ifndef PAYLOADER_H_
#define PAYLOADER_H_

#include "define.h"
#include "Singleton.h"
#include "Lua/LuaInterface.h"

using namespace CommBaseOut;
using namespace std;

struct TencentConfig
{
	unsigned int nAppId;
	string sAppKey;
	string sServerName;
	string sZoneId;
	string sUrlPath;
	string sMarkerUrlPath;

	TencentConfig():nAppId(0)
	{
		sAppKey.clear();
		sServerName.clear();
		sZoneId.clear();
		sUrlPath.clear();
		sMarkerUrlPath.clear();
	}
};

struct RePriceType
{
	int nId;
	int nNum;
	int nPlatform;
	int nDiscount;
	int nPrice;
	int nItemId;
	string sUrl;

	RePriceType():nId(0),nNum(0),nPlatform(0),nDiscount(0),nPrice(0),nItemId(0)
	{
		sUrl.clear();
	}
};

struct TencentTaskInfo
{
	string sId;
	int nStep;
	int nTarget;
	int nNum;
	int nItemId;
	int nBindRew;

	TencentTaskInfo():nStep(0),nTarget(0),nNum(0),nItemId(0),nBindRew(0)
	{

	}
};

class CPayLoader : public Singleton<CPayLoader>
{
public:
	CPayLoader();
	~CPayLoader();

	int Init(string &path);
	int InitLua(string & file);
	int InitRePriceTypeData(string& file);
	int InitTencentTaskInfo(string& file);

	int GetPrice(int nId,int nPlatform);
	int GetNum(int nId,int nPlatform);

	const TencentConfig&  GetTencentConfig();
	void GetRePriceType(int nId,int nPlatform,RePriceType& rePriceType);

	int GetFindTaskInfo(string sId);

	int GetTaskTarget(string sId,int nStep);
	int GetTaskItemId(string sId,int nStep);
	int GetTaskItemNum(string sId,int nStep);

	string Get58PlayKey(){return m_s58PlayKey;}
	DWORD Get58PlayRate(){return m_n58PlayRate;}
	string Get360LoginKey(){return m_s360LoginKey;}
	string Get360PayKey(){return m_s360PayKey;};
	string GetConnectingKey(){return m_sConnectingKey;}
	int GetConnectingRate(){return m_nConnectingRate;}
private:
	DWORD m_n58PlayRate;
	DWORD m_nConnectingRate;
	string m_s58PlayKey;
	string m_s360PayKey;
	string m_s360LoginKey;
	string m_sConnectingKey;
	CLuaInterface m_pLua;
	TencentConfig m_TencentConfig;
	map<int,RePriceType> m_RePriceType;
	vector<TencentTaskInfo> m_TencentTaskInfo;
};


#endif /* PAYLOADER_H_ */
