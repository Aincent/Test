/*
 * PayLoader.cpp
 *
 *  Created on: 2015年6月17日
 *      Author: root
 */

#include "PayLoader.h"
#include "../CSVLoader.h"
#include "Log/Logger.h"
#include "../StrArgPkg.h"

CPayLoader::CPayLoader():m_n58PlayRate(0),m_nConnectingRate(0)
{
	m_pLua.OpenLua();
}

CPayLoader::~CPayLoader()
{
	m_pLua.CloseLua();
}

int CPayLoader::Init(string &path)
{
	string luaFile = path + "Data/AppSeverInfo.lua";
	string  rePriceTypeData = path + "Data/RePriceTypeData.lua";
	string tencentTaskInfo = path + "Data/STaskTypeData.csv";

	if(InitLua(luaFile) || InitRePriceTypeData(rePriceTypeData) || InitTencentTaskInfo(tencentTaskInfo))
	{
		return -1;
	}

	return 0;
}

int CPayLoader::InitLua(string & file)
{

	if(m_pLua.LoadFile(file) != 0 || m_pLua.Call() != 0)
	{
		return -1;
	}

	try
	{
		m_pLua.SetLuaFunction("GetAppInfo");
		if(m_pLua.Call(0 ,6))
		{
			return -1;
		}

		m_TencentConfig.nAppId = m_pLua.GetInt();

		char sAppKey[32] = {0};
		char sServerName[32] = {0};
		char sZoneId[32] = {0};
		char sUrlPath[64] = {0};
		char sMarkerUrlPath[64] = {0};

		m_pLua.GetString(sAppKey, 32, 0,5);
		m_pLua.GetString(sServerName, 32, 0,4);
		m_pLua.GetString(sZoneId, 32, 0,3);
		m_pLua.GetString(sUrlPath, 64, 0,2);
		m_pLua.GetString(sMarkerUrlPath, 64, 0,1);

		m_TencentConfig.sAppKey = sAppKey;
		m_TencentConfig.sServerName = sServerName;
		m_TencentConfig.sZoneId = sZoneId;
		m_TencentConfig.sUrlPath = sUrlPath;
		m_TencentConfig.sMarkerUrlPath = sMarkerUrlPath;

		m_pLua.SetLuaFunction("Get58PlayAppInfo");
		if(m_pLua.Call(0 ,2))
		{
			return -1;
		}

		m_n58PlayRate = m_pLua.GetInt();
		char s58PlayAppKey[32] = {0};
		m_pLua.GetString(s58PlayAppKey, 32, 0,1);
		m_s58PlayKey = s58PlayAppKey;

		m_pLua.SetLuaFunction("Get360PlayAppInfo");
		if(m_pLua.Call(0 ,2))
		{
			return -1;
		}

		char s360PayAppKey[64] = {0};
		m_pLua.GetString(s360PayAppKey, 64, 0,2);
		m_s360PayKey = s360PayAppKey;

		char s360LoginAppKey[64] = {0};
		m_pLua.GetString(s360LoginAppKey, 64, 0,1);
		m_s360LoginKey = s360LoginAppKey;

		m_pLua.SetLuaFunction("GetConnectingAppInfo");
		if(m_pLua.Call(0 ,2))
		{
			return -1;
		}

		m_nConnectingRate = m_pLua.GetInt();

		char sConnectingPayAppKey[64] = {0};
		m_pLua.GetString(sConnectingPayAppKey, 64, 0,1);
		m_sConnectingKey = sConnectingPayAppKey;
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "AppServerConfig error [errormsg=%s]", e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "AppServerConfig unknown error");
	}

	return 0;
}

int CPayLoader::InitRePriceTypeData(string& file)
{
	int tableCount;
	if(m_pLua.LoadFile(file) != 0 || m_pLua.Call() != 0)
	{
		return -1;
	}

	m_pLua.SetLuaFunction("GetPriceType");

	if(m_pLua.Call(0,1))
	{
		m_pLua.CloseLua();
		return -1;
	}

	tableCount = m_pLua.GetTableFieldCount();

	for(int i = 1 ; i <= tableCount ; i++)
	{
		if (m_pLua.GetFieldTable(i) != 0)
		{
			return -1;
		}

		RePriceType info;

		info.nId = m_pLua.GetFieldInt("i_ID");
		info.nItemId = m_pLua.GetFieldInt("i_itemID");
		info.nNum = m_pLua.GetFieldInt("i_num");
		info.nPlatform = m_pLua.GetFieldInt("i_platform");
		info.nDiscount = m_pLua.GetFieldInt("i_discount");
		info.nPrice = m_pLua.GetFieldInt("i_price");
		char sUrl[256] = {0};
		m_pLua.GetFieldString(sUrl, 256, "i_url");
		info.sUrl = sUrl;

		m_RePriceType[info.nId] = info;
		m_pLua.CloseTable();
	}
	m_pLua.CloseTable();
	return 0;
}

int CPayLoader::InitTencentTaskInfo(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		TencentTaskInfo info;
		int count = 0;

		info.sId = loader.GetString(i, count++);
		count++;
		info.nStep = loader.GetInt(i, count++);
		count++;
		info.nTarget = loader.GetInt(i, count++);
		info.nNum = loader.GetInt(i, count++);
		info.nItemId = loader.GetInt(i, count++);
		info.nBindRew = loader.GetInt(i, count++);

		m_TencentTaskInfo.push_back(info);
	}

	return 0;
}

int CPayLoader::GetPrice(int nId,int nPlatform)
{
	map<int,RePriceType>::iterator itr = m_RePriceType.find(nId);
	if(itr != m_RePriceType.end() && itr->second.nPlatform == nPlatform)
	{
		return itr->second.nPrice;
	}

	return 0;
}

int CPayLoader::GetNum(int nId,int nPlatform)
{
	map<int,RePriceType>::iterator itr = m_RePriceType.find(nId);
	if(itr != m_RePriceType.end() && itr->second.nPlatform == nPlatform)
	{
		return itr->second.nPrice;
	}

	return 0;
}

const TencentConfig&  CPayLoader::GetTencentConfig()
{
	return m_TencentConfig;
}

void CPayLoader::GetRePriceType(int nId,int nPlatform,RePriceType& rePriceType)
{
	map<int,RePriceType>::iterator itr = m_RePriceType.find(nId);
	if(itr != m_RePriceType.end() && itr->second.nPlatform == nPlatform)
	{
		rePriceType = itr->second;
	}
}

int CPayLoader::GetFindTaskInfo(string sId)
{
	vector<TencentTaskInfo>::iterator itr = m_TencentTaskInfo.begin();
	for(; itr != m_TencentTaskInfo.end(); ++itr)
	{
		if(itr->sId.compare(sId) == 0)
		{
			return 0;
		}
	}

	return -1;
}

int CPayLoader::GetTaskTarget(string sId,int nStep)
{
	vector<TencentTaskInfo>::iterator itr = m_TencentTaskInfo.begin();
	for(; itr != m_TencentTaskInfo.end(); ++itr)
	{
		if(itr->sId == sId && itr->nStep == nStep)
		{
			return itr->nTarget;
		}
	}

	return -1;
}

int CPayLoader::GetTaskItemId(string sId,int nStep)
{
	vector<TencentTaskInfo>::iterator itr = m_TencentTaskInfo.begin();
	for(; itr != m_TencentTaskInfo.end(); ++itr)
	{
		if(itr->sId.compare(sId) == 0 && itr->nStep == nStep)
		{
			return itr->nItemId;
		}
	}

	return -1;
}

int CPayLoader::GetTaskItemNum(string sId,int nStep)
{
	vector<TencentTaskInfo>::iterator itr = m_TencentTaskInfo.begin();
	for(; itr != m_TencentTaskInfo.end(); ++itr)
	{
		if(itr->sId.compare(sId) == 0 && itr->nStep == nStep)
		{
			return itr->nNum;
		}
	}

	return -1;
}

