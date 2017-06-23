#include "GodTreeFile.h"
#include "../CSVLoader.h"
#include "../StrArgPkg.h"
#include "Lua/LuaInterface.h"

GodTreeFile::GodTreeFile()
{

}

GodTreeFile::~GodTreeFile()
{

}

int GodTreeFile::init(const string& direct)
{
	if(initFile(direct))	
	{
		return -1;
	}

	if(initStatic(direct))
	{
		return -1;
	}

	return 0;
}

int GodTreeFile::initFile(const string& file)
{
	CSVLoader loader;
	string tStr = file + "Data/Gangtree.csv";

	if(!loader.OpenFromFile(tStr.c_str()))
	{
		return -1;
	}

	int tRowCounts = loader.GetRowCount();
	int tColCounts = 0;

	for(int i = 0;i < tRowCounts;++i)
	{
		GodTreeData tData;
		
		tColCounts = 0;

		++tColCounts;
	    tData.m_Level = loader.GetInt(i, tColCounts++);
	    tData.m_Exp = loader.GetInt(i, tColCounts++);
	
		StrArgPkg tCollectArg("|",loader.GetString(i,tColCounts++));
		vector<int> tIDList;
		for(size_t j = 0;j < tCollectArg.size();++j)
		{
			tIDList.push_back(atoi(getArg(tCollectArg,j).c_str()));
		}

		vector<CollectData> tCollects;
		StrArgPkg tRewardArg("|:",loader.GetString(i,tColCounts++));
		for(size_t j = 0;j < tRewardArg.size();++j)
		{
			CollectData tCollectReward;
			tCollectReward.m_CollectReward.m_ItemID = atoi(getArg(tRewardArg[j],0).c_str());
			tCollectReward.m_CollectReward.m_Counts = atoi(getArg(tRewardArg[j],1).c_str());
			tCollectReward.m_CollectReward.m_Bind = atoi(getArg(tRewardArg[j],2).c_str());

			tData.m_CollectItems[tIDList[j]] = tCollectReward;
		}

		++tColCounts;	

		StrArgPkg tCondition("|",loader.GetString(i,tColCounts++));
		for(size_t j = 0;j < tCondition.size();++j)
		{
			tData.m_CollectItems[tIDList[j]].m_Condition = atoi(getArg(tCondition,j).c_str());
		}

		StrArgPkg tFallReward("|:",loader.GetString(i,tColCounts++));
		for(size_t j = 0;j < tFallReward.size();++j)
		{
			GodFallReward tFallData;
			tFallData.m_Item.m_ItemID = atoi(getArg(tFallReward[j],0).c_str());
			tFallData.m_Item.m_Counts = atoi(getArg(tFallReward[j],1).c_str());
			tFallData.m_Item.m_Bind   = atoi(getArg(tFallReward[j],2).c_str());
			tFallData.m_EveryCounts   = atoi(getArg(tFallReward[j],3).c_str());
			tFallData.m_Range 		  = atoi(getArg(tFallReward[j],4 ).c_str());

			tData.m_FallReward.push_back(tFallData);
		}

		tData.m_Range = loader.GetInt(i, tColCounts++);
		++tColCounts;
		tData.m_NPCID = loader.GetInt(i, tColCounts++);

		m_Datas[tData.m_Level] = tData;
	}

	return 0;
}

int GodTreeFile::initStatic(const string& file)
{
	CLuaInterface tlua;
	tlua.OpenLua();

	string tStr = file + "Data/ClanBase.lua";
	if(tlua.LoadFile(tStr) != 0 || tlua.Call() != 0)
	{
		return -1;
	}

	tlua.SetLuaFunction("getGodTreeData");
	if(tlua.Call(0,5) != 0)
	{
		return -1;
	}
	
	m_StaticData.m_PromptTextValue			   = tlua.GetInt();
	m_StaticData.m_PromptTextID	   = tlua.GetInt();
	m_StaticData.m_InternationalID = tlua.GetInt();
	m_StaticData.m_AddExp 		   = tlua.GetInt();
	m_StaticData.m_CountdownTime   = tlua.GetInt();

	return 0;
}

const GodTreeData* GodTreeFile::getData(int level)
{	
	map<int,GodTreeData>::iterator tIt = m_Datas.find(level);
	if(tIt == m_Datas.end())
	{
		return NULL;
	}

	return &tIt->second;
}

void GodTreeFile::getLevelAndCurExp(int allExp,int& level,int& exp) 
{
	const map<int,GodTreeData>& tDatas = GodTreeFile::GetInstance()->getDatas();
	map<int,GodTreeData>::const_iterator tIt = tDatas.begin();

	for(;tIt != tDatas.end();++tIt)
	{
		level = tIt->second.m_Level;
		exp   = allExp;
		allExp = allExp - tIt->second.m_Exp;	

		//大于0再执行一次
		if(allExp < 0)
		{
			return ;
		}
	}

	return ;
}

void GodTreeFile::getCanBuckle(map<int,int>& canBuckle,const map<int,int>& haveBuckle,int level)
{
	map<int,GodTreeData>::iterator tIt =  m_Datas.find(level);
	if(tIt == m_Datas.end())
	{
		return ;
	}

	map<int,CollectData>::iterator tInIt = tIt->second.m_CollectItems.begin();
	for(;tInIt != tIt->second.m_CollectItems.end();++tInIt)
	{
		map<int,int>::const_iterator tHaveIt = haveBuckle.find(tInIt->first);
		canBuckle[tInIt->first] = tInIt->second.m_Condition;
		if(tHaveIt != haveBuckle.end())
		{
			canBuckle[tInIt->first] -= tHaveIt->second;
		}
	}

	return ;
}

void GodTreeFile::getSubmitReward(int level,map<int,int>& submits,vector<DWORD>& items,vector<DWORD>& nums,vector<WORD>& flags) 
{
	map<int,GodTreeData>::iterator tIt = m_Datas.find(level);
	if(m_Datas.end() == tIt)
	{
		return ;
	}

	map<int,int>::iterator tInIt = submits.begin();
	for(;tInIt != submits.end();++tInIt)
	{
		map<int,CollectData>::iterator tTowIt = tIt->second.m_CollectItems.find(tInIt->first);
		if(tTowIt == tIt->second.m_CollectItems.end())
		{
			continue;
		}

		items.push_back(tTowIt->second.m_CollectReward.m_ItemID);
		nums.push_back(tTowIt->second.m_CollectReward.m_Counts * tInIt->second);
		flags.push_back(tTowIt->second.m_CollectReward.m_Bind);
	}

	return ;
}

const map<int,CollectData>* GodTreeFile::getCollectData(int level)
{
	map<int,GodTreeData>::iterator tIt = m_Datas.find(level);
	if(m_Datas.end() == tIt)
	{
		return NULL;
	}

	return &tIt->second.m_CollectItems;
}
