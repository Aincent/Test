/*
 * GsCountryWarLoader.cpp
 *
 *  Created on: 2015年4月20日
 *      Author: root
 */
#include "GsCountryWarLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "util.h"
#include <math.h>
#include "./FileLoader/UpgradeLoader.h"

CountryWarLoader::CountryWarLoader()
{
		m_pLua.OpenLua();
		loadTaskConfig();
		loadWarConfig();
		loadWarMapInfo();
}

CountryWarLoader::~CountryWarLoader()
{
		m_pLua.CloseLua();
}

const CountryWarTaskConfigInfo* CountryWarLoader::getTaskConfig(int taskID)
{
	std::map<int,CountryWarTaskConfigInfo>::const_iterator it=m_taskInfo.find(taskID);
	if(it==m_taskInfo.end())
		return NULL;
	return &it->second;
}

void CountryWarLoader::getTaskList(std::vector<int>& tasks)
{
	std::map<int,CountryWarTaskConfigInfo>::const_iterator it=m_taskInfo.begin();
	for(; it!=m_taskInfo.end(); ++it)
	{
		tasks.push_back(it->first);
	}
}

int CountryWarLoader::getZonePrizeTime()
{
		return m_warConfig.zonePrizeTime;
}

int CountryWarLoader::getZoneExp(int lv,int rate)
{
	UpgradeInfo nowInfo;

	UpgradeLoader::GetInstance()->GetUpgradeByLv(lv, nowInfo);
	if(nowInfo.lv == 0)
	{
		return 0;
	}

	m_pLua.SetLuaFunction("GetZoneExp");
	m_pLua.SetFloat(nowInfo.factoryExp);
	m_pLua.SetInt(rate);
	if(m_pLua.Call(2, 1))
	{
			return 0;
	}
	return m_pLua.GetInt();
}

int CountryWarLoader::getZoneForce(int lv,int rate)
{
		m_pLua.SetLuaFunction("GetZoneForce");
		m_pLua.SetInt(lv);
		m_pLua.SetInt(rate);
		if(m_pLua.Call(2, 1))
		{
				return 0;
		}
		return m_pLua.GetInt();
}

	int CountryWarLoader::getPrizeTime()
	{
		return m_warConfig.prizeTime;
	}

	int CountryWarLoader::getScoreByPlayerNum(int num)
	{
				m_pLua.SetLuaFunction("GetScoreByPlayerNum");
				m_pLua.SetInt(num);
				if(m_pLua.Call(1, 1))
				{
						return 0;
				}
				return m_pLua.GetInt();
	}

	int CountryWarLoader::getContributeByNum(int num)
	{
			m_pLua.SetLuaFunction("GetContributeByPlayerNum");
			m_pLua.SetInt(num);
			if(m_pLua.Call(1,1))
			{
						return 0;
			}
			return m_pLua.GetInt();
	}

int CountryWarLoader::getScoreByMonster(int monID)
{
	std::map<int,CountryWarMonsterConfig>::const_iterator it=m_warConfig.monsterInfos.find(monID);
	if(it!=m_warConfig.monsterInfos.end())
		return it->second.score;
	return 0;
}

//加载任务奖励
bool CountryWarLoader::loadTaskConfig()
{
		std::string strFileFullPath;
		if(CUtil::GetModulePath(strFileFullPath) != 0)
		{
				return false;
		}
		string strFile = strFileFullPath + "Data/CampReward.csv";
		CSVLoader loader;

		if(!loader.OpenFromFile(strFile.c_str()))
		{
			return false;
		}

		//编号	目标类型	目标id	目标数量	单价阵营积分	单价个人贡献	道具奖励
		for(unsigned int i=0; i<loader.GetRowCount(); ++i)
		{
			CountryWarTaskConfigInfo info;
			int count = 0;

			info.id = loader.GetInt(i, count++);
			info.type = loader.GetInt(i, count++);
			info.targetID = loader.GetInt(i, count++);
			info.targetCount = loader.GetInt(i, count++);
			info.score = loader.GetInt(i, count++);
			info.contribute = loader.GetInt(i, count++);
			std::string strProp = loader.GetString(i,count++);
			StrArgPkg propPkg(":", strProp);
			if(propPkg.size()!=2)
				return false;

			info.propID = atoi( getArg(propPkg,0).c_str() );
			info.propCount = atoi( getArg(propPkg,1).c_str() );

			m_taskInfo[info.id] = info;
		}

		return true;
}

bool CountryWarLoader::loadWarConfig()
{
			std::string strFileFullPath;
			if(CUtil::GetModulePath(strFileFullPath) != 0)
			{
				return false;
			}

			string strConfig = strFileFullPath + "Data/countryWar.lua";
			if(m_pLua.LoadFile(strConfig) != 0 || m_pLua.Call() != 0)
			{
					return false;
			}

			m_pLua.SetLuaFunction("GetScoreConfig");
			if(m_pLua.Call(0, 6))
			{
					return false;
			}

			m_warConfig.scoreRate = m_pLua.GetInt();
			m_warConfig.addscore = m_pLua.GetInt();
			m_warConfig.addscoreTime = m_pLua.GetInt();
			m_warConfig.protectime = m_pLua.GetInt();
			m_warConfig.killScore = m_pLua.GetInt();
			m_warConfig.occupyScore = m_pLua.GetInt();

			m_pLua.SetLuaFunction("GetPrize");
			if(m_pLua.Call(0,8))
			{
				m_pLua.CloseLua();
				return false;
			}
			m_warConfig.winTimesParam2 = m_pLua.GetInt();
			m_warConfig.winTimesParam1 = m_pLua.GetInt();
			m_warConfig.thirdPrizeNum = m_pLua.GetInt();
			m_warConfig.thirdPrize = m_pLua.GetInt();
			m_warConfig.secondPrizeNum = m_pLua.GetInt();
			m_warConfig.secondPrize = m_pLua.GetInt();
			m_warConfig.firstPrizeNum = m_pLua.GetInt();
			m_warConfig.firstPrize = m_pLua.GetInt();

			m_pLua.SetLuaFunction("GetNotice");
			if(m_pLua.Call(0,2))
			{
				m_pLua.CloseLua();
				return false;
			}
			m_warConfig.continueWinNoticeID = m_pLua.GetInt();
			m_warConfig.occupyAllNoticeID = m_pLua.GetInt();

			m_pLua.SetLuaFunction("GetZoneInfo");
			if(m_pLua.Call(0,5))
			{
				m_pLua.CloseLua();
				return false;
			}
			m_warConfig.noSafeZonePrizeRate=m_pLua.GetInt();
			m_warConfig.safeZonePrizeRate=m_pLua.GetInt();
			m_warConfig.bornthID = m_pLua.GetInt();
			m_warConfig.prizeTime = m_pLua.GetInt();
			m_warConfig.zonePrizeTime = m_pLua.GetInt();

			m_pLua.SetLuaFunction("GetElement");
			if(m_pLua.Call(0,1))
			{
				m_pLua.CloseLua();
				return false;
			}

			int count=m_pLua.GetTableFieldCount();
			for(int i = 1; i <= count; i++)
			{
					if(m_pLua.GetFieldTable(i) != 0)
					{
						return false;
					}

					int id = m_pLua.GetFieldInt("id");
					m_warConfig.elementID.push_back(id);
					m_pLua.CloseTable();
			}
			m_pLua.CloseTable();

			//胜利方玩家奖励物品
			m_pLua.SetLuaFunction("GetWinPrize");
			if(m_pLua.Call(0,1))
			{
				m_pLua.CloseLua();
				return false;
			}

			count=m_pLua.GetTableFieldCount();
			for(int i = 1; i <= count; i++)
			{
				if(m_pLua.GetFieldTable(i) != 0)
				{
					return false;
				}

				int id = m_pLua.GetFieldInt("id");
				int num = m_pLua.GetFieldInt("num");
				m_warConfig.winPrizeProps.push_back(make_pair(id,num));
				m_pLua.CloseTable();
			}
			m_pLua.CloseTable();

			//失败方玩家奖励物品
			m_pLua.SetLuaFunction("GetFailPrize");
			if(m_pLua.Call(0,1))
			{
				m_pLua.CloseLua();
				return false;
			}

			count=m_pLua.GetTableFieldCount();
			for(int i = 1; i <= count; i++)
			{
				if(m_pLua.GetFieldTable(i) != 0)
				{
					return false;
				}

				int id = m_pLua.GetFieldInt("id");
				int num = m_pLua.GetFieldInt("num");
				m_warConfig.failPrizeProps.push_back(make_pair(id,num));
				m_pLua.CloseTable();
			}
			m_pLua.CloseTable();

			//
			m_pLua.SetLuaFunction("GetCountryProp");
			if(m_pLua.Call(0,1))
			{
				m_pLua.CloseLua();
				return false;
			}
			count=m_pLua.GetTableFieldCount();
			for(int i=1; i<=count; i++)
			{
				if(m_pLua.GetFieldTable(i) != 0)
				{
					return false;
				}
				int id = m_pLua.GetFieldInt("id");
				int num = m_pLua.GetFieldInt("num");
				m_warConfig.countryWinprops.push_back(make_pair(id,num));
				m_pLua.CloseTable();
			}
			m_pLua.CloseTable();

			m_pLua.SetLuaFunction("GetMonster");
			if(m_pLua.Call(0,1))
			{
					m_pLua.CloseLua();
					return false;
			}

			int tableCount = m_pLua.GetTableFieldCount();
			for(int i = 1 ; i <= tableCount; i++)
			{
					if(m_pLua.GetFieldTable(i) != 0)
					{
							return false;
					}
					CountryWarMonsterConfig config;
					int id = m_pLua.GetFieldInt("monsterid");
					config.score = m_pLua.GetFieldInt("score");
					m_warConfig.monsterInfos[id] = config;
					m_pLua.CloseTable();
			}

			m_pLua.CloseTable();

			return true;
}

bool CountryWarLoader::loadWarMapInfo()
{
	 std::string strFileFullPath;
		if(CUtil::GetModulePath(strFileFullPath) != 0)
		{
			return false;
		}
		string strFile = strFileFullPath + "Data/CampmapZTypeData.csv";
		CSVLoader loader;
		if(!loader.OpenFromFile(strFile.c_str()))
		{
				return false;
		}

		for(unsigned int i=0; i<loader.GetRowCount(); ++i)
		{
			int d = 0;
			int a = 0;
			int count = 0;

			CountryWarMapInfo mapInfo;
			mapInfo.nId = loader.GetInt(i,count++);
			mapInfo.nXPos = loader.GetInt(i,count++);
			mapInfo.nYPos = loader.GetInt(i,count++);
			mapInfo.nRadius = loader.GetInt(i,count++);
			mapInfo.nXPos1 = loader.GetInt(i,count++);
			mapInfo.nYPos1 = loader.GetInt(i,count++);
			mapInfo.nXPos2 = loader.GetInt(i,count++);
			mapInfo.nYPos2 = loader.GetInt(i,count++);
			mapInfo.nRate  = loader.GetInt(i,count++);
			if(mapInfo.nId == 0)
			{
				continue;
			}

			if(mapInfo.nRadius > 0)
			{
				for(int i = mapInfo.nXPos - mapInfo.nRadius; i <= mapInfo.nXPos + mapInfo.nRadius; i++)
				{
					for(int j = mapInfo.nYPos - mapInfo.nRadius; j <=  mapInfo.nYPos + mapInfo.nRadius; j++)
					{
						d = (int)sqrt((i-mapInfo.nXPos)*(i-mapInfo.nXPos)+(j-mapInfo.nYPos)*(j-mapInfo.nYPos));
						if(d <= mapInfo.nRadius)
						{
							CountryBossPos pos;
							pos.nXPos = i;
							pos.nYPos = j;
							mapInfo.vPos.push_back(pos);
						}
					}
				}
			}
			else
			{
				int c = (int)sqrt((mapInfo.nXPos1 - mapInfo.nXPos)*(mapInfo.nXPos1 - mapInfo.nXPos) - (mapInfo.nYPos2 - mapInfo.nYPos) * (mapInfo.nYPos2 - mapInfo.nYPos));
				a = (mapInfo.nXPos1 - mapInfo.nXPos)*2;

				for(int i = mapInfo.nXPos - (mapInfo.nXPos1 - mapInfo.nXPos); i < mapInfo.nXPos1; i++)
				{
					for(int j = mapInfo.nYPos - mapInfo.nYPos2; j < mapInfo.nYPos + mapInfo.nYPos2; j++)
					{
						d = (int)sqrt((i-(mapInfo.nXPos - c))*(i-(mapInfo.nXPos -c))+(j-mapInfo.nYPos)*(j-mapInfo.nYPos)) + (int)sqrt((i-(mapInfo.nXPos + c))*(i-(mapInfo.nXPos + c))+(j-mapInfo.nYPos)*(j-mapInfo.nYPos));
						if(d <= a)
						{
							CountryBossPos pos;
							pos.nXPos = i;
							pos.nYPos = j;
							mapInfo.vPos.push_back(pos);
						}
					}
				}
			}

			m_warMapInfo[mapInfo.nId] = mapInfo;
		}
		return true;
}

void CountryWarLoader::GetRandomPos(int64& nXPos,int64& nYPos,int country)
{
	//目前暂时统一取点
	map<int,CountryWarMapInfo>::iterator itr = m_warMapInfo.find(m_warConfig.bornthID);
	if(itr != m_warMapInfo.end())
	{
		int temp = rand()%itr->second.vPos.size();
		nXPos = itr->second.vPos[temp].nXPos;
		nYPos = itr->second.vPos[temp].nYPos;
	}
}

bool CountryWarLoader::IsSafeZone(int64 nXPos,int64 nYPos,int country)
{
	//目前暂时统一取点
	map<int,CountryWarMapInfo>::iterator itr = m_warMapInfo.find(m_warConfig.bornthID);
	if(itr != m_warMapInfo.end())
	{
		double a = abs(itr->second.nXPos1-itr->second.nXPos);
		double b = abs(itr->second.nYPos-itr->second.nYPos2);
		double c = sqrt(a*a-b*b);
		double cx1 = (double)itr->second.nXPos-c;
		double cy1 = (double)itr->second.nYPos;
		double cx2 = (double)itr->second.nXPos+c;
		double cy2 = (double)itr->second.nYPos;

		double af1=sqrt((nXPos-cx1)*(nXPos-cx1)+(nYPos-cy1)*(nYPos-cy1));
		double af2=sqrt((nXPos-cx2)*(nXPos-cx2)+(nYPos-cy2)*(nYPos-cy2));

		double dx=af1+af2;
		if(dx<=2*a)
		{
			return true;
		}
	}
	return false;
}

void CountryWarLoader::GetWarMapInfo(map<int,CountryWarMapInfo>& infoMap)
{
	infoMap = m_warMapInfo;
}










