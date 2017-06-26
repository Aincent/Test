/*
 * ClanLoader.cpp
 *
 *  Created on: 2014年9月30日
 *      Author: helh
 */

#include "ClanLoader.h"
#include "CSVLoader.h"
#include "Log/Logger.h"
#include "./FileLoader/UpgradeLoader.h"
#include "StrArgPkg.h"

int ClanLoader::Init(string &path)
{
	string updateFile = path + "Data/GangTypeData.csv";
	string luaFile = path + "Data/ClanBase.lua";
	string luaManorFile = path + "Data/clanManor.lua";
	string citytypeFile = path + "Data/GangcityTypeData.txt";
	string gangShop = path + "Data/GangshopTypeData.csv";
	string gangRewardItem = path + "Data/Gangwar.csv";
	string robberFile = path + "Robber.lua";
	string integralLua =  path + "Data/clanIntegral.lua";
	string rewardsPRBang = path + "Data/RewardsPRBang.csv";
	string mvpRewardsPRBang = path + "Data/MvpRewardsPRBang.csv";

	if(InitUpdate(updateFile)
			|| InitLua(luaFile)
			|| InitManorLua(luaManorFile)
			|| InitGangShop(gangShop)
			|| InitGangRewardItem(gangRewardItem)
			|| InitIntegralLua(integralLua)
			|| InitRewardsPRBang(rewardsPRBang)
			|| InitMvpRewardsPRBang(mvpRewardsPRBang))
	{
		return -1;
	}

	return 0;
}

int ClanLoader::InitUpdate(string & file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		UpdateInfo info;
		int count = 0;

		info.lv = loader.GetInt(i, count++);
		info.icon = loader.GetInt(i, count++);
		info.money = loader.GetInt64(i, count++);
		info.golden = loader.GetInt64(i, count++);
		info.person = loader.GetInt(i, count++);
		info.zhp = loader.GetInt(i, count++);
		info.zpattack = loader.GetInt(i, count++);
		info.zmattack = loader.GetInt(i, count++);
		info.zpdefence = loader.GetInt(i, count++);
		info.zmdefence = loader.GetInt(i, count++);
		info.zhrate = loader.GetInt(i, count++);
		info.zavoid = loader.GetInt(i, count++);
		info.zcrack = loader.GetInt(i, count++);
		info.zacrack = loader.GetInt(i, count++);

		info.hp = loader.GetInt(i, count++);
		info.pattack = loader.GetInt(i, count++);
		info.mattack = loader.GetInt(i, count++);
		info.pdefence = loader.GetInt(i, count++);
		info.mdefence = loader.GetInt(i, count++);
		info.hrate = loader.GetInt(i, count++);
		info.avoid = loader.GetInt(i, count++);
		info.crack = loader.GetInt(i, count++);
		info.acrack = loader.GetInt(i, count++);

		m_updateInfo.push_back(info);
	}

	return 0;
}

int ClanLoader::InitLua(string & file)
{
	if(m_lua.LoadFile(file) != 0 || m_lua.Call() != 0)
	{
		return -1;
	}

	m_lua.SetLuaFunction("GetCreateBase");

	if(m_lua.Call(0, 6))
	{
		return -1;
	}

	m_createClanItemID = m_lua.GetInt();
	m_lua.GetInt();
	m_lua.GetInt();
	m_createGolden = m_lua.GetInt();
	m_createMoney = m_lua.GetInt();
	m_openLv = m_lua.GetInt();

	return 0;
}

int ClanLoader::InitManorLua(string & file)
{
	if(m_lua.LoadFile(file) != 0 || m_lua.Call() != 0)
	{
		return -1;
	}

	//基本信息
	m_lua.SetLuaFunction("GetBaseInfo");
	if(m_lua.Call(0,6))
	{
		return -1;
	}

	m_config.winscore = m_lua.GetInt();
	m_config.killscores = m_lua.GetInt();
	m_config.occupyscores = m_lua.GetInt();
	m_config.scoretime = m_lua.GetInt();
	m_config.protectime = m_lua.GetInt();
	m_config.occypytime = m_lua.GetInt();

	//红蓝方npc的id
	m_lua.SetLuaFunction("GetNPC");
	if(m_lua.Call(0,2))
	{
		return -1;
	}
	m_config.blueCampnpcId = m_lua.GetInt();
	m_config.redCampnpcId = m_lua.GetInt();

	//宝箱信息
	m_lua.SetLuaFunction("GetBoxInfo");
	if(m_lua.Call(0,2))
	{
		return -1;
	}
	m_config.flushtime = m_lua.GetInt();
	m_config.boxsnum = m_lua.GetInt();

	//邮件id
	m_lua.SetLuaFunction("GetMailID");
	if(m_lua.Call(0,1))
	{
		return -1;
	}
	m_config.mailid = m_lua.GetInt();

	//宝箱位置信息
	m_lua.SetLuaFunction("GetBoxPos");
	if(m_lua.Call(0,1))
	{
		return -1;
	}

	int tableCount = m_lua.GetTableFieldCount();
	for(int i = 1; i <= tableCount; i++)
	{
		if(m_lua.GetFieldTable(i) != 0)
		{
			return -1;
		}
		pont pos;
		pos.xpos = m_lua.GetFieldInt("posx");
		pos.ypos = m_lua.GetFieldInt("posy");
		m_clanWarBoxsPos.push_back(pos);
		m_lua.CloseTable();
	}
	m_lua.CloseTable();

	//红方出生位置点
	m_lua.SetLuaFunction("GetRedPos");
	if(m_lua.Call(0,1))
	{
		return -1;
	}

	tableCount = m_lua.GetTableFieldCount();
	for(int i = 1; i <= tableCount; i++)
	{
		if(m_lua.GetFieldTable(i) != 0)
		{
			return -1;
		}
		pont pos;
		pos.xpos = m_lua.GetFieldInt("posx");
		pos.ypos = m_lua.GetFieldInt("posy");
		m_clanWarRedCampPos.push_back(pos);
		m_lua.CloseTable();
	}
	m_lua.CloseTable();

	//蓝方出生位置点
	m_lua.SetLuaFunction("GetBluePos");
	if(m_lua.Call(0,1))
	{
		return -1;
	}

	tableCount = m_lua.GetTableFieldCount();
	for(int i = 1; i <= tableCount; i++)
	{
		if(m_lua.GetFieldTable(i) != 0)
		{
			return -1;
		}
		pont pos;
		pos.xpos = m_lua.GetFieldInt("posx");
		pos.ypos = m_lua.GetFieldInt("posy");
		m_clanWarBlueCampPos.push_back(pos);
		m_lua.CloseTable();
	}
	m_lua.CloseTable();

	return 0;
}

int ClanLoader::InitIntegralLua(string& file)
{
	int tableCount;

	if(m_lua.LoadFile(file) != 0 || m_lua.Call() != 0)
	{
		return -1;
	}

	m_lua.SetLuaFunction("GetRefreshTime");
	if(m_lua.Call(0,1))
	{
		return -1;
	}

	m_refreshTime = m_lua.GetInt();

	m_lua.SetLuaFunction("GetBirthPos1");
	if(m_lua.Call(0,1))
	{
		return -1;
	}

	tableCount = m_lua.GetTableFieldCount();
	for(int i = 1 ; i <= tableCount ; i++)
	{
		if (m_lua.GetFieldTable(i) != 0)
		{
			return -1;
		}

		pont pos;
		pos.xpos = m_lua.GetFieldInt(1);
		pos.ypos = m_lua.GetFieldInt(2);
		m_attVec.push_back(pos);
		m_lua.CloseTable();
	}
	m_lua.CloseTable();

	m_lua.SetLuaFunction("GetBirthPos2");
	if(m_lua.Call(0,1))
	{
		return -1;
	}

	tableCount = m_lua.GetTableFieldCount();
	for(int i = 1 ; i <= tableCount ; i++)
	{
		if (m_lua.GetFieldTable(i) != 0)
		{
			return -1;
		}
		pont pos;
		pos.xpos = m_lua.GetFieldInt(1);
		pos.ypos = m_lua.GetFieldInt(2);
		m_defVec.push_back(pos);
		m_lua.CloseTable();
	}
	m_lua.CloseTable();

	m_lua.SetLuaFunction("GetBirthBoss");
	if(m_lua.Call(0,1))
	{
		return -1;
	}

	tableCount = m_lua.GetTableFieldCount();
	for(int i = 1 ; i <= tableCount ; i++)
	{
		if (m_lua.GetFieldTable(i) != 0)
		{
			return -1;
		}
		BossInfo info;
		info.monsterId = m_lua.GetFieldInt("mosterid");
		info.pos.xpos = m_lua.GetFieldInt("xpos");
		info.	pos.ypos = m_lua.GetFieldInt("ypos");

		m_bossInfo[info.monsterId] =  info;

		m_lua.CloseTable();
	}
	m_lua.CloseTable();

	m_lua.SetLuaFunction("GetBossRate");
	if(m_lua.Call(0,1))
	{
		return -1;
	}

	tableCount = m_lua.GetTableFieldCount();
	for(int i = 1 ; i <= tableCount ; i++)
	{
		if (m_lua.GetFieldTable(i) != 0)
		{
			return -1;
		}

		int refreshNum = m_lua.GetFieldInt("refreshNum");
		int addRate = m_lua.GetFieldInt("addRate");

		m_bossRate[refreshNum] =  addRate;

		m_lua.CloseTable();
	}
	m_lua.CloseTable();

	return 0;
}

int ClanLoader::InitRewardsPRBang(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		IntegralRewards info;
		int count = 0;
		count++;

		info.nType = loader.GetInt(i, count++);
		info.nTarget = loader.GetInt(i, count++);

		string itemInclude = loader.GetString(i, count++);
		StrArgPkg includePkgItem("|", itemInclude);
		for(WORD j=0; j<includePkgItem.size(); ++j)
		{
			StrArgPkg limitPkg(":", getArg(includePkgItem, j).c_str());
			unsigned int k=0;
			info.itemList.push_back(atoi(getArg(limitPkg, k).c_str()));
			info.num.push_back(atoi(getArg(limitPkg, k+1).c_str()));
		}

		m_integralRewards.push_back(info);
	}

	return 0;
}

int ClanLoader::InitMvpRewardsPRBang(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		IntegralMVPRewards info;
		int count = 0;

		string itemInclude = loader.GetString(i, count++);
		StrArgPkg includePkgItem("|", itemInclude);
		for(WORD j=0; j<includePkgItem.size(); ++j)
		{
			info.itemList.push_back(atoi(getArg(includePkgItem, j).c_str()));
		}

		info.nLv = loader.GetInt(i, count++);

		string numInclude = loader.GetString(i, count++);
		StrArgPkg numIncludePkgItem("|", numInclude);
		for(WORD j=0; j<numIncludePkgItem.size(); ++j)
		{
			info.num.push_back(atoi(getArg(numIncludePkgItem, j).c_str()));
		}

		m_integralMVPRewards.push_back(info);
	}

	return 0;
}

int ClanLoader::InitGangShop(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 1;
		ClanShop shop;

		shop.nItemId = loader.GetInt(i, count++);
		shop.nLimitNum = loader.GetInt(i, count++);
		shop.nPrice = loader.GetInt(i, count++);
		count++;
		shop.nBindType = loader.GetInt(i, count++);

		m_clanShop[shop.nItemId] = shop;
	}

	return 0;
}

int ClanLoader::InitGangRewardItem(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		ClanWarRewardItem info;
		int count = 0;

		loader.GetInt(i, count++);
		int type = loader.GetInt(i, count++);//胜负

		//奖励物品ID
		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		funPkgItem.setArgs(funStrItem);

		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			if(limitPkg.size() != 2)
				continue;

			int id = atoi(getArg(limitPkg, 0).c_str());
			int num = atoi(getArg(limitPkg, 1).c_str());

			info.items.push_back(make_pair(id,num));
		}

		m_clanWarRewardItem[type] = info;
	}

	return 0;
}

const ClanWarRewardItem* ClanLoader::GetClanWarRewardItem(bool isWin)
{
	int id = isWin? 1:2;

	map<int,ClanWarRewardItem>::iterator it = m_clanWarRewardItem.find(id);
	if(it!=m_clanWarRewardItem.end())
	{
		return &(it->second);
	}

	return NULL;
}

int ClanLoader::GetScorePeople(int nNum)
{
	try
	{
		m_lua.SetLuaFunction("GetScorePeople");
		m_lua.SetInt(nNum);

		if(m_lua.Call(1, 1))
		{
			return 0;
		}

		return m_lua.GetInt();

	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "get score People error[%s]",e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "get score People error");
	}

	return 0;
}

int ClanLoader::GetScoreBoss(int nNum)
{
	try
	{
		m_lua.SetLuaFunction("GetScoreBoss");
		m_lua.SetInt(nNum);

		if(m_lua.Call(1, 1))
		{
			return 0;
		}

		return m_lua.GetInt();

	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "get score Boss error[%s]",e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "get score Boss error");
	}

	return 0;
}

int ClanLoader::GetScoreAlive(int nNum,int nTime)
{
	try
	{
		m_lua.SetLuaFunction("GetScoreAlive");
		m_lua.SetInt(nNum);
		m_lua.SetInt(nTime);

		if(m_lua.Call(2, 1))
		{
			return 0;
		}

		return m_lua.GetInt();

	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "get score Alive error[%s]",e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "get score Alive error");
	}

	return 0;
}

int ClanLoader::GetCotriReward(int nNum)
{
	try
	{
		m_lua.SetLuaFunction("GetCotriReward");
		m_lua.SetInt(nNum);

		if(m_lua.Call(1, 1))
		{
			return 0;
		}

		return m_lua.GetInt();

	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "get cotri Reward error[%s]",e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "get cotri Reward error");
	}

	return 0;
}

int ClanLoader::GetMoneyReward(int nNum)
{
	try
	{
		m_lua.SetLuaFunction("GetMoneyReward");
		m_lua.SetInt(nNum);

		if(m_lua.Call(1, 1))
		{
			return 0;
		}

		return m_lua.GetInt();

	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "get money Reward error[%s]",e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "get money Reward error");
	}

	return 0;
}

int ClanLoader::GetGasReward(int nNum)
{
	try
	{
		m_lua.SetLuaFunction("GetGasReward");
		m_lua.SetInt(nNum);

		if(m_lua.Call(1, 1))
		{
			return 0;
		}

		return m_lua.GetInt();

	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "get gas Reward error[%s]",e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "get gas Reward error");
	}

	return 0;
}

int ClanLoader::GetMoneyScore(int nNum)
{
	try
	{
		m_lua.SetLuaFunction("GetMoneyScore");
		m_lua.SetInt(nNum);

		if(m_lua.Call(1, 1))
		{
			return 0;
		}

		return m_lua.GetInt();

	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "get Money Score error[%s]",e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "get Money Score error");
	}

	return 0;
}

int ClanLoader::GetVictoryScore(int scorePeople,int scoreBoss,int scoreAlive)
{
	try
	{
		m_lua.SetLuaFunction("GetVictoryScore");
		m_lua.SetInt(scorePeople);
		m_lua.SetInt(scoreBoss);
		m_lua.SetInt(scoreAlive);

		if(m_lua.Call(3, 1))
		{
			return 0;
		}

		return m_lua.GetInt();

	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "get Victory Score error[%s]",e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "get Victory Score error");
	}

	return 0;
}

int ClanLoader::GetFailScore(int scorePeople,int scoreBoss,int scoreAlive)
{
	try
	{
		m_lua.SetLuaFunction("GetFailScore");
		m_lua.SetInt(scorePeople);
		m_lua.SetInt(scoreBoss);
		m_lua.SetInt(scoreAlive);

		if(m_lua.Call(3, 1))
		{
			return 0;
		}

		return m_lua.GetInt();

	}
	catch(exception &e)
	{
		LOG_ERROR(FILEINFO, "get Fail Score error[%s]",e.what());
	}
	catch(...)
	{
		LOG_ERROR(FILEINFO, "get Fail Score error");
	}

	return 0;
}

int ClanLoader::GetBossRate(int nNum)
{
	map<int,int>::iterator itr = m_bossRate.find(nNum);
	if(itr != m_bossRate.end())
	{
		return itr->second;
	}

	return 0;
}

const map<int,BossInfo>* ClanLoader::GetBossInfo()
{
	return &m_bossInfo;
}

void ClanLoader::GetRandomPos(int64& nXPos,int64& nYPos,bool bFlag)
{
	if(m_attVec.size() <= 0 || m_defVec.size() <= 0)
	{
		return;
	}

	if(bFlag)
	{
		int temp = rand()%m_attVec.size();
		nXPos = m_attVec[temp].xpos;
		nYPos = m_attVec[temp].ypos;
	}
	else
	{
		int temp = rand()%m_defVec.size();
		nXPos = m_defVec[temp].xpos;
		nYPos = m_defVec[temp].ypos;
	}
}

void ClanLoader::GetBirthPos(int camp, int64& posx, int64& posy)
{
	if(camp == eCamp_Red)
	{
		if(m_clanWarRedCampPos.size() == 0)
			return;

		int temp = rand()%m_clanWarRedCampPos.size();
		posx = m_clanWarRedCampPos[temp].xpos;
		posy = m_clanWarRedCampPos[temp].ypos;
	}
	else
	{
		if(m_clanWarRedCampPos.size() == 0)
			return;

		int temp = rand()%m_clanWarBlueCampPos.size();
		posx = m_clanWarBlueCampPos[temp].xpos;
		posy = m_clanWarBlueCampPos[temp].ypos;
	}

}

int ClanLoader::GetMVPRewards(int nLv)
{
	int nTemp = 0;
	int tempRandom = 0;
	int tempBefore = 0;
	const IntegralMVPRewards* pMvp = NULL;
	for(size_t i = 0; i < m_integralMVPRewards.size(); i++)
	{
		if(tempBefore < nLv && nLv <= m_integralMVPRewards[i].nLv)
		{
			pMvp = &(m_integralMVPRewards[i]);
			break;
		}

		tempBefore = m_integralMVPRewards[i].nLv;
	}

	if(pMvp)
	{
		for(size_t i = 0; i < pMvp->num.size(); i++)
		{
			nTemp += pMvp->num[i];
		}

		tempRandom = rand()%nTemp + 1;
		tempBefore = 0;

		for(size_t j = 0; j < pMvp->num.size(); j++)
		{
			if(tempRandom > tempBefore && tempRandom <= tempBefore + (int)pMvp->num[j])
			{
				return pMvp->itemList[j];
			}

			tempBefore += pMvp->num[j];
		}
	}

	return 0;
}

const IntegralRewards* ClanLoader::GetIntegralRewards(int nType,int nTarget)
{
	int tempBefore = 0;
	vector<IntegralRewards>::iterator itr = m_integralRewards.begin();
	for(; itr != m_integralRewards.end(); ++itr)
	{
		if(itr->nType == nType)
		{
			if(tempBefore <= nTarget && nTarget <= itr->nTarget)
			{
				return &(*itr);
			}

			tempBefore = itr->nTarget;
		}
	}

	vector<IntegralRewards>::iterator itrf = m_integralRewards.begin();
	for(; itrf != m_integralRewards.end(); ++itrf)
	{
		if(itrf->nType == nType && itrf->nTarget == tempBefore)
		{
			return &(*itrf);
		}
	}

	return NULL;
}
