/*
 * AchievementLoader.cpp
 *
 *  Created on: 2014年6月19日
 *      Author: root
 */

#include "AchievementLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "Log/Logger.h"
#include "MessageCommonRet.h"
#include <math.h>
#include "../QuestNew/QuestDefine.h"
#include "FileLoader/GameParaLoader.h"

AchievementLoader::AchievementLoader():m_golden(0)
{
	m_typeAndAch[1] = GameParaLoader::GetInstance()->getPretigeMallStage1();
	m_typeAndAch[2] = GameParaLoader::GetInstance()->getPretigeMallStage2();
	m_typeAndAch[3] = GameParaLoader::GetInstance()->getPretigeMallStage3();
	m_typeAndAch[4] = GameParaLoader::GetInstance()->getPretigeMallStage4();
	m_typeAndAch[5] = GameParaLoader::GetInstance()->getPretigeMallStage5();
}

AchievementLoader::~AchievementLoader()
{

}

int AchievementLoader::Init(string &file)
{
	string AchievementTypeDataPath = file + "Data/AchievementTypeData.csv";
	string TitleTypeDataPath = file + "Data/TitleTypeData.csv";
	string ShopDataPath = file + "Data/AchievementshopTypeData.csv";

	if(InitAchievement(AchievementTypeDataPath) || InitTitle(TitleTypeDataPath)||InitShop(ShopDataPath))
	{
		return -1;
	}
	return 0;
}

int AchievementLoader::InitAchievement(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		sAchievementEntry info;
		int count = 0;

		info.i_achID = loader.GetInt(i, count++);
		if(info.i_achID <=0)
			continue;

		info.s_achname = loader.GetString(i, count++);
		info.i_target = (eAchievementType)loader.GetInt(i, count++);
		info.i_targetnumber = loader.GetInt(i, count++);
		info.i_description = loader.GetString(i, count++);
		info.i_exp = loader.GetInt(i, count++);
		info.i_money = loader.GetInt(i, count++);
		info.i_zhenqi = loader.GetInt(i, count++);
		info.i_prestige = loader.GetInt(i, count++);
		info.i_classification = loader.GetInt(i, count++);
		info.i_Amount = loader.GetInt(i, count++);
		info.i_titleID = loader.GetInt(i, count++);
		info.i_BGolden = loader.GetInt(i, count++);
		m_AchievementInfo[info.i_achID] = info;

		sPrestigeContent prestigeInfo;
		prestigeInfo.achievementId = info.i_achID;							//成就ID
		prestigeInfo.achievementType = info.i_classification;	//成就类型
		prestigeInfo.isComplete = false;									//初始化还没有完成
		prestigeInfo.UseType = AchievementUseType_1;				//成就领取状态初始化
		prestigeInfo.achievementPercent = 0;											//目标完成量
		mAchievementIDMap[info.i_achID] = prestigeInfo;
	}

	return 0;
}


int AchievementLoader::InitTitle(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		sTitleEntry info;
		int count = 0;

		info.i_titleID = loader.GetInt(i, count++);
		info.i_resID = loader.GetInt(i, count++);
		info.i_titlename = loader.GetString(i, count++);
		info.i_prestige = loader.GetInt(i, count++);
		info.i_exp = loader.GetInt(i, count++);
		info.i_money = loader.GetInt(i, count++);
		info.i_zhenqi = loader.GetInt(i, count++);
		info.i_achID = loader.GetInt(i, count++);
		count++;
		info.phyack = loader.GetInt(i, count++);
		info.magack = loader.GetInt(i, count++);
		info.phydee = loader.GetInt(i, count++);
		info.magdee = loader.GetInt(i, count++);
		count++;
		info.hp = loader.GetInt(i, count++);

		if(info.i_titleID <=0 )
			continue;
		m_TitleInfo[info.i_titleID] = info;
	}
	return 0;
}

int AchievementLoader::InitShop(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		sAchShopItem info;
		sAchGoodsItem item;
		int golden;
		int count = 0;
		item.id = loader.GetInt(i, count++);
		item.goodsid = loader.GetInt(i, count++);
		count++;
		item.money = loader.GetInt(i, count++);
		info.count = loader.GetInt(i, count++);
		info.id = loader.GetInt(i, count++);
		golden = loader.GetInt(i, count++);

		if(golden != 0)
			m_golden = golden;

		map<int,sAchShopItem>::iterator it = m_AchShopInfo.find(info.id);
		if(it == m_AchShopInfo.end())
		{
			info.goodsItem[item.id] = item;
			m_AchShopInfo[info.id] = info;
		}
		else
		{
			it->second.goodsItem[item.id] = item;
		}
	}
	return 0;
}

sAchievementEntry *AchievementLoader::GetAchievementEntry(DWORD id)
{
	map<DWORD, sAchievementEntry>::iterator it =  m_AchievementInfo.find(id);

	if(it != m_AchievementInfo.end())
	{
		return &(it->second);
	}

	return NULL;
}

sTitleEntry *AchievementLoader::GetTitleEntry(DWORD id)
{
	map<DWORD, sTitleEntry>::iterator it =  m_TitleInfo.find(id);

	if(it != m_TitleInfo.end())
	{
		return &(it->second);
	}

	return NULL;
}

void AchievementLoader::GetAllAchievementID(std::vector<DWORD> & ids)
{
	map<DWORD, sAchievementEntry>::iterator it = m_AchievementInfo.begin();
	for(;it != m_AchievementInfo.end(); it++)
	{
		ids.push_back(it->first);
	}
}

void AchievementLoader::GetAchievementTypeIDs(int type,std::vector<DWORD> & ids)
{
	map<DWORD, sAchievementEntry>::iterator it = m_AchievementInfo.begin();
	for(;it != m_AchievementInfo.end(); it++)
	{
		if(it->second.i_target == type)
			ids.push_back(it->first);
	}
	std::sort(ids.begin(),ids.end());
}

void AchievementLoader::GetAllTitleID(std::vector<DWORD> & ids)
{
	map<DWORD, sTitleEntry>::iterator it = m_TitleInfo.begin();
	//称号ID大于500属于等级榜的称号
	for(;it != m_TitleInfo.end(); it++)
	{
		if(it->first >= 500)
		{
			break;
		}
		ids.push_back(it->first);
	}
}

void AchievementLoader::UpdataShop(map<int,sAchShopInfo> &info)
{
	map<int,sAchShopItem>::iterator it = m_AchShopInfo.begin();
	for(;it!=m_AchShopInfo.end();it++)
	{
		map<int,sAchShopInfo>::iterator its = info.find(it->first);
		if(its == info.end())
		{
			sAchShopInfo item;
			item.count = 0;
			item.id = it->second.id;
			it->second.RandomGoods(item.goodsid);
			info[item.id] = item;
		}
		else
		{
			it->second.RandomGoods(its->second.goodsid);
		}
	}
}
