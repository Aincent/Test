/*
 * GirlWarLoader.cpp
 *
 *  Created on: 2014年12月5日
 *      Author: root
 */

#include "GirlWarLoader.h"
#include "CSVLoader.h"
#include "Log/Logger.h"
#include "StrArgPkg.h"
#include <cmath>

GirlWarLoader::GirlWarLoader():m_nCommonExp(0)//m_nPrimaryExp(0),m_nMiddleExp(0),m_nSeniorExp(0)
{
	m_mGirlWarInfo.clear();
	m_mType.clear();
	m_mGirlWarNeed.clear();
	m_mGirlWarType.clear();
	m_vGirlWarAttribute.clear();
	m_pLua.OpenLua();
}

GirlWarLoader::~GirlWarLoader()
{
	m_pLua.CloseLua();
}

int GirlWarLoader::Init(string& path)
{
	string girlWarFile = path + "Data/GirlTypeData.csv";
	string girlWarCollectionFile = path + "Data/GirlCollection.csv";
	string girlWarAttribute =  path + "Data/GirlAttribute.csv";
	string luaFile = path + "Data/GirlTypeData.lua";
	if(InitGirlWarInfo(girlWarFile) || InitGirlWarNeed(girlWarFile) || InitGirlWarStarrank(girlWarFile) || InitGirlWarQuality(girlWarFile)||InitGirlWarCollection(girlWarCollectionFile)|| InitGirlWarAttribute(girlWarAttribute) || InitLua(luaFile))
	{
		return -1;
	}

	return 0;
}

int GirlWarLoader::InitLua(string& path)
{
	if(m_pLua.LoadFile(path) != 0 || m_pLua.Call() != 0)
	{
		return -1;
	}

	try
	{
//		m_pLua.SetLuaFunction("GetGirlWarExp");
//		if (m_pLua.Call(0, 3))
//		{
//			return -1;
//		}
//
//		m_nSeniorExp = m_pLua.GetInt();
//		m_nMiddleExp = m_pLua.GetInt();
//		m_nPrimaryExp = m_pLua.GetInt();

		m_pLua.SetLuaFunction("GetCommonExp");
		if (m_pLua.Call(0, 1))
		{
			return -1;
		}

		m_nCommonExp = m_pLua.GetInt();
	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "GirlWarLoader error [errormsg=%s]", e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "GirlWarLoader unknown error");
	}
	return 0;
}

int GirlWarLoader::InitGirlWarInfo(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		sGirlWarInfo sInfo;
		int count = 0;
		count++;
		sInfo.nId = loader.GetInt(i, count++);
		sInfo.sName = loader.GetString(i,count++);
		sInfo.nModle = loader.GetInt(i,count++);
		count++;
		count++;
		sInfo.nMoveSpeed = loader.GetInt(i,count++);
		sInfo.nBasehit = loader.GetInt(i,count++);
		sInfo.nLuck = loader.GetInt(i,count++);

		count+=26;

		sInfo.nCommonSkill = loader.GetInt(i,count++);
		sInfo.nSkill1 = loader.GetInt(i,count++);
		sInfo.nSkill2 = loader.GetInt(i,count++);
		sInfo.nSkill3 = loader.GetInt(i,count++);

		sInfo.nExp1 = loader.GetInt(i,count++);
		sInfo.nExp2 = loader.GetInt(i,count++);
		sInfo.nExp3 = loader.GetInt(i,count++);

		sInfo.nMoveRange = loader.GetInt(i,count++);
		sInfo.nAckRange = loader.GetInt(i,count++);
		sInfo.nAckInterval = loader.GetInt(i,count++);
		sInfo.nLimitLv = loader.GetInt(i,count++);
		sInfo.nTriggerPro = loader.GetInt(i,count++);
		sInfo.nFlushTime = loader.GetInt(i,count++);

		m_mGirlWarInfo[sInfo.nId] = sInfo;
	}

	return 0;
}

int GirlWarLoader::InitGirlWarNeed(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		GirlWarNeed sNeed;
		vector<int> vec;
		vector<int> vtype;

		sNeed.nId = loader.GetInt(i, 1);

		string itemInclude = loader.GetString(i, 5);
		StrArgPkg includePkgItem("|", itemInclude);

		for(WORD j=0; j<includePkgItem.size(); ++j)
		{
			StrArgPkg limitPkg(":", getArg(includePkgItem, j).c_str());
			unsigned int k=0;

			if(limitPkg.size() == 3)
			{
				sNeed.InsertData((NeedType)atoi(getArg(limitPkg, k).c_str()),atoi(getArg(limitPkg,k+1).c_str()),atoi(getArg(limitPkg, k+2).c_str()));
			}
			else if(limitPkg.size() == 2)
			{
				sNeed.InsertData((NeedType)atoi(getArg(limitPkg, k).c_str()),atoi(getArg(limitPkg, k+1).c_str()));
			}
			else if (limitPkg.size() == 4)
			{
				sNeed.InsertData((NeedType)atoi(getArg(limitPkg, k).c_str()),atoi(getArg(limitPkg, k+1).c_str()), atoi(getArg(limitPkg, k+2).c_str()),
						atoi(getArg(limitPkg, k+3).c_str()));
			}
			else
			{
				break;
			}

			map<int,vector<int> >::iterator itr = m_mType.find(atoi(getArg(limitPkg, k).c_str()));
			if(itr != m_mType.end())
			{
				itr->second.push_back(sNeed.nId);
			}
			else
			{
				vec.push_back(sNeed.nId);
				m_mType[atoi(getArg(limitPkg, k).c_str())] = vec;
			}

			vtype.push_back(atoi(getArg(limitPkg, k).c_str()));

		}
		m_mGirlWarNeed[sNeed.nId] = sNeed;
		m_mGirlWarType[sNeed.nId] = vtype;
	}
	return 0;
}

int GirlWarLoader::InitGirlWarStarrank(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		sGirlWarStarrank sInfo;
		int count = 0;
		count++;
		sInfo.nId = loader.GetInt(i, count++);
		count += 7;
		sInfo.nStarrank = loader.GetInt(i, count++);
		sInfo.nDebrisId = loader.GetInt(i, count++);
		sInfo.nDebrisNum = loader.GetInt(i, count++);
		sInfo.nResolveNum = loader.GetInt(i, count++);

		sInfo.nAddHp = loader.GetInt(i, count++);
		sInfo.nAddPhyAck = loader.GetInt(i, count++);
		sInfo.nAddPhydef = loader.GetInt(i, count++);
		sInfo.nAddCrit = loader.GetInt(i, count++);
		sInfo.nAddDodge = loader.GetInt(i, count++);
		sInfo.nAddUng = loader.GetInt(i, count++);
		count+=10;
		sInfo.nAddOwerHp = loader.GetInt(i, count++);
		sInfo.nAddOwerPhyAck = loader.GetInt(i, count++);
		sInfo.nAddOwerPhydef = loader.GetInt(i, count++);
		sInfo.nAddOwerCrit = loader.GetInt(i, count++);
		sInfo.nAddOwerDodge = loader.GetInt(i, count++);
		sInfo.nAddOwerUng = loader.GetInt(i, count++);

		m_vGirlWarStarrank.push_back(sInfo);
	}

	return 0;
}

int GirlWarLoader::InitGirlWarQuality(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		sGirlWarQuality sInfo;
		int count = 0;
		count++;
		sInfo.nId = loader.GetInt(i, count++);
		count += 17;
		sInfo.nQuality = loader.GetInt(i, count++);
		sInfo.nQualityLv = loader.GetInt(i, count++);
		sInfo.m_nEquip1 = loader.GetInt(i, count++);
		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			for(unsigned int i=0; i<limitPkg.size(); ++i)
			{
				int type = atoi(getArg(limitPkg, i).c_str());
				int value = atoi(getArg(limitPkg, ++i).c_str());
				sInfo.m_mAtt1[type] = (value);
			}
		}

		sInfo.m_nEquip2 = loader.GetInt(i, count++);
		funStrItem = loader.GetString(i, count++, "");
		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			for(unsigned int i=0; i<limitPkg.size(); ++i)
			{
				int type = atoi(getArg(limitPkg, i).c_str());
				int value = atoi(getArg(limitPkg, ++i).c_str());
				sInfo.m_mAtt2[type] = (value);
			}
		}

		sInfo.m_nEquip3 = loader.GetInt(i, count++);
		funStrItem = loader.GetString(i, count++, "");
		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			for(unsigned int i=0; i<limitPkg.size(); ++i)
			{
				int type = atoi(getArg(limitPkg, i).c_str());
				int value = atoi(getArg(limitPkg, ++i).c_str());
				sInfo.m_mAtt3[type] = (value);
			}
		}

		sInfo.m_nEquip4 = loader.GetInt(i, count++);
		funStrItem = loader.GetString(i, count++, "");
		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			for(unsigned int i=0; i<limitPkg.size(); ++i)
			{
				int type = atoi(getArg(limitPkg, i).c_str());
				int value = atoi(getArg(limitPkg, ++i).c_str());
				sInfo.m_mAtt4[type] = (value);
			}
		}

		m_vGirlWarQuality.push_back(sInfo);
	}

	return 0;
}

int GirlWarLoader::InitGirlWarCollection(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		sGirlWarCollection sInfo;
		int count = 0;
		sInfo.nId = loader.GetInt(i, count++);
		sInfo.nStarrankDow = loader.GetInt(i, count++);
		sInfo.nStarrankUp = loader.GetInt(i, count++);
		sInfo.nAddHp = loader.GetInt(i, count++);
		sInfo.nAddPhyAck = loader.GetInt(i, count++);
		sInfo.nAddPhydef = loader.GetInt(i, count++);
		sInfo.nAddCrit = loader.GetInt(i, count++);
		sInfo.nAddUng = loader.GetInt(i, count++);
		sInfo.nAddDodge = loader.GetInt(i, count++);

		sInfo.nAddHp1 = loader.GetInt(i, count++);
		sInfo.nAddPhyAck1 = loader.GetInt(i, count++);
		sInfo.nAddPhydef1 = loader.GetInt(i, count++);
		sInfo.nAddCrit1 = loader.GetInt(i, count++);
		sInfo.nAddUng1 = loader.GetInt(i, count++);
		sInfo.nAddDodge1 = loader.GetInt(i, count++);

		sInfo.nAddHp2 = loader.GetInt(i, count++);
		sInfo.nAddPhyAck2 = loader.GetInt(i, count++);
		sInfo.nAddPhydef2 = loader.GetInt(i, count++);
		sInfo.nAddCrit2 = loader.GetInt(i, count++);
		sInfo.nAddUng2 = loader.GetInt(i, count++);
		sInfo.nAddDodge2 = loader.GetInt(i, count++);

		m_vsGirlWarCollection.push_back(sInfo);
	}

	return 0;
}

int GirlWarLoader::InitGirlWarAttribute(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		sGirlWarAttribute sInfo;
		int count = 1;
		sInfo.nId = loader.GetInt(i, count++);
		count++;
		sInfo.nLv = loader.GetInt(i, count++);
		sInfo.nAddHp = loader.GetInt(i, count++);
		sInfo.nAddPhyAck = loader.GetInt(i, count++);
		sInfo.nAddPhydef = loader.GetInt(i, count++);
		sInfo.nAddDodge = loader.GetInt(i, count++);
		sInfo.nAddCrit = loader.GetInt(i, count++);
		sInfo.nAddUng = loader.GetInt(i, count++);

		m_vGirlWarAttribute.push_back(sInfo);
	}

	return 0;
}

const sGirlWarInfo* GirlWarLoader::GetGirlWarInfo(int nId)
{
	map<int,sGirlWarInfo>::iterator itr =  m_mGirlWarInfo.find(nId);
	if(itr != m_mGirlWarInfo.end())
	{
		return &(itr->second);
	}

	return NULL;
}

const sGirlWarStarrank* GirlWarLoader::GetGirlWarStarrank(int nId, int nStar)
{
	vector<sGirlWarStarrank>::iterator itr = m_vGirlWarStarrank.begin();
	for(; itr != m_vGirlWarStarrank.end(); ++itr)
	{
		if(itr->nId == nId && itr->nStarrank == nStar)
		{
				return &(*itr);
		}
	}

	return NULL;
}

const sGirlWarQuality* GirlWarLoader::GetGirlWarQuality(int nId, int nQuality)
{
	vector<sGirlWarQuality>::iterator itr = m_vGirlWarQuality.begin();
	for(; itr != m_vGirlWarQuality.end(); ++itr)
	{
			if(itr->nId == nId && itr->nQuality == nQuality)
			{
					return &(*itr);
			}
	}

	return NULL;
}

const sGirlWarCollection* GirlWarLoader::GetGirlWarCollection(int nStar)
{
	vector<sGirlWarCollection>::iterator itr = m_vsGirlWarCollection.begin();
	for(; itr != m_vsGirlWarCollection.end(); ++itr)
	{
		if( nStar >= itr->nStarrankDow && nStar <= itr->nStarrankUp)
		{
			return &(*itr);
		}
	}

	return NULL;
}

const sGirlWarCollection* GirlWarLoader::GetGirlWarCollectionById(int nId)
{
	vector<sGirlWarCollection>::iterator itr = m_vsGirlWarCollection.begin();
	for(; itr != m_vsGirlWarCollection.end(); ++itr)
	{
		if(itr->nId == nId)
		{
			return &(*itr);
		}
	}

	return NULL;
}

void GirlWarLoader::IsGirlWarNeed(NeedType etype,int value,int arg,vector<int>& vec, int64 arg1)
{
	map<int,vector<int> >::iterator itr = m_mType.find(etype);
	if(itr != m_mType.end())
	{
		vector<int>::iterator vitrEnd = itr->second.end();
		for(vector<int>::iterator vitr = itr->second.begin(); vitr != vitrEnd; ++vitr)
		{
			map<int,GirlWarNeed>::iterator mitr =  m_mGirlWarNeed.find(*vitr);
			if(mitr != m_mGirlWarNeed.end() && mitr->second.IsEnoughCondition(etype,value,arg, arg1))
			{
				vec.push_back(mitr->first);
			}
		}
	}
}

const GirlWarNeed* GirlWarLoader::GetGirlWarNeed(int nId)
{
	map<int,GirlWarNeed>::iterator itr = m_mGirlWarNeed.find(nId);
	if(itr != m_mGirlWarNeed.end())
	{
			return &(itr->second);
	}

	return NULL;
}

const sGirlWarAttribute* GirlWarLoader::GetGirlWarAttribute(int nId, int nLv)
{
	vector<sGirlWarAttribute>::iterator itr = m_vGirlWarAttribute.begin();
	for(; itr != m_vGirlWarAttribute.end(); ++itr)
	{
			if(itr->nId == nId && itr->nLv == nLv)
			{
					return &(*itr);
			}
	}

	return NULL;
}

int GirlWarLoader::GetFlushTime(int nId)
{
	map<int,sGirlWarInfo>::iterator itr =  m_mGirlWarInfo.find(nId);
	if(itr != m_mGirlWarInfo.end())
	{
		return itr->second.nFlushTime;
	}

	return 0;
}

int GirlWarLoader::GetMoveRange(int nId)
{
	map<int,sGirlWarInfo>::iterator itr =  m_mGirlWarInfo.find(nId);
	if(itr != m_mGirlWarInfo.end())
	{
		return itr->second.nMoveRange;
	}

	return 0;
}

int GirlWarLoader::GetAckRange(int nId)
{
	map<int,sGirlWarInfo>::iterator itr =  m_mGirlWarInfo.find(nId);
	if(itr != m_mGirlWarInfo.end())
	{
		return itr->second.nAckRange;
	}

	return 0;
}

int GirlWarLoader::GetTriggerPro(int nId)
{
	map<int,sGirlWarInfo>::iterator itr =  m_mGirlWarInfo.find(nId);
	if(itr != m_mGirlWarInfo.end())
	{
		return itr->second.nTriggerPro;
	}

	return 0;
}

int GirlWarLoader::GetCommonSkill(int nId)
{
	map<int,sGirlWarInfo>::iterator itr =  m_mGirlWarInfo.find(nId);
	if(itr != m_mGirlWarInfo.end())
	{
		return itr->second.nCommonSkill;
	}

	return 0;
}

int GirlWarLoader::GetUpdateLv(int nId,int nLv)
{
	map<int,sGirlWarInfo>::iterator itr =  m_mGirlWarInfo.find(nId);
	if(itr != m_mGirlWarInfo.end())
	{
		return (int)(std::pow((float)nLv,itr->second.nExp1/10) + nLv * itr->second.nExp2 + itr->second.nExp3);
	}

	return 0;
}

int GirlWarLoader::GetLimitLv(int nId)
{
	map<int,sGirlWarInfo>::iterator itr =  m_mGirlWarInfo.find(nId);
	if(itr != m_mGirlWarInfo.end())
	{
		return itr->second.nLimitLv;
	}

	return 0;
}

void GirlWarLoader::GetGirlWarType(int nId, vector<int>& vec)
{
	map<int,vector<int> >::iterator itr = m_mGirlWarType.find(nId);
	if(itr != m_mGirlWarType.end())
	{
		vec = itr->second;
	}
}

int GirlWarLoader::GetGirlWarNeedPropId(int nId)
{
	map<int,GirlWarNeed>::iterator itr =  m_mGirlWarNeed.find(nId);
	if(itr != m_mGirlWarNeed.end())
	{
		return itr->second.GetPropId();
	}

	return 0;
}

bool GirlWarLoader::IsHavaEquip(int nId, int nQuality,int nEquipId)
{
	const sGirlWarQuality *pQuality = GetGirlWarQuality(nId,nQuality);
	if(pQuality)
	{
		if(pQuality->m_nEquip1 == nEquipId || pQuality->m_nEquip2 == nEquipId || pQuality->m_nEquip3 == nEquipId || pQuality->m_nEquip4 == nEquipId )
		{
			return true;
		}
	}

	return false;
}

const map<int,int>* GirlWarLoader::GetEquipAtt(int nId, int nQuality,int nEquipId)
{
	const sGirlWarQuality *pQuality = GetGirlWarQuality(nId,nQuality);
	if(pQuality)
	{
		if(pQuality->m_nEquip1 == nEquipId)
		{
			return &pQuality->m_mAtt1;
		}
		else if(pQuality->m_nEquip2 == nEquipId)
		{
			return &pQuality->m_mAtt2;
		}
		else if(pQuality->m_nEquip3 == nEquipId)
		{
			return &pQuality->m_mAtt3;
		}
		else if(pQuality->m_nEquip4 == nEquipId)
		{
			return &pQuality->m_mAtt4;
		}
	}

	return NULL;
}
