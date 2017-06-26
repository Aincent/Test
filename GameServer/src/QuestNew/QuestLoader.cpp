/*
 * QuestLoader.cpp
 *
 *  Created on: 2013��10��30��
 *      Author: helh
 */

#include "QuestLoader.h"
#include "Log/Logger.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "FileLoader/UpgradeLoader.h"
#include "Probability/Probability.h"

QuestLoader::QuestLoader():m_maxMainquestID(0)
{
	tlua.OpenLua();
}

QuestLoader::~QuestLoader()
{
	tlua.CloseLua();
}

int QuestLoader::Init(string &path)
{
	string tTaskFile = path + "TaskTypeData.csv";
	string tDailyTask = path + "DailyTaskReward.csv";
	string tDailyTaskCost = path + "DailyTaskCost.csv";

	if(InitQuest(tTaskFile) || InitDailyTaskReward(tDailyTask) || InitOneFinishConsume(tDailyTaskCost))
	{
		return -1;
	}

	return 0;
}


int QuestLoader::InitQuest(string &file)
{
//	string tTaskFile = file + "TaskTypeData.csv";
//	string tWeekTaskFile = file + "WeekRewardTypeData.csv";
//	if(initWeekTask(tWeekTaskFile))
//	{
//		return -1;
//	}

	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		Entry_Quest info;
		int count = 0;

		info.m_nQuestId = loader.GetInt(i, count++);
		count++;
		info.m_nQuestChap = loader.GetInt(i, count++);
		info.m_nMainType = loader.GetInt(i, count++);
		info.m_nMinorType = loader.GetInt(i, count++);
		count++;
		info.m_nTaskTarget = loader.GetInt(i, count++);
		info.m_nTaskTargetNum = loader.GetInt(i, count++);
		info.m_nCollItemID = loader.GetInt(i, count++);
		info.m_nDropItemRate = loader.GetInt(i, count++);

		string funStrItem4 = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem4("|", funStrItem4);
		funPkgItem4.setArgs(funStrItem4);

		info.m_nTaskMap = atoi(getArg(funPkgItem4, 0).c_str());
		info.m_nTaskXpos = atoi(getArg(funPkgItem4, 1).c_str());
		info.m_nTaskYpos = atoi(getArg(funPkgItem4, 2).c_str());

		string funStrItem3 = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem3("|", funStrItem3);
		funPkgItem3.setArgs(funStrItem3);

		for(uint itemSize=0; itemSize<funPkgItem3.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem3, itemSize).c_str());
			for(unsigned int i=0; i<limitPkg.size(); ++i)
			{
				int taskItemID = atoi(getArg(limitPkg, i++).c_str());
				int taskNum = atoi(getArg(limitPkg, i).c_str());

				if(taskItemID <=0 || taskNum <=0 || taskNum>99)  //物品数量 不能 超过 99 --会影响 任务重置 删除 之前
					continue;

				info.m_nTaskItemList.push_back(taskItemID);
				info.m_nTaskItemListNum.push_back(taskNum);
			}
		}

		if(info.m_nTaskItemList.size() >1)
			continue;  //不允许 一个任务  收集 多种 物品

		info.m_nAcceptNpcId = loader.GetInt(i, count++);
		info.m_nSubmitNpcId = loader.GetInt(i, count++);
		count++;
		info.m_nrewardExp = loader.GetInt(i, count++); 														///奖励经验
		info.m_nrewardMoney = loader.GetInt(i, count++); 													///奖励金钱
		info.m_nrewardForce = loader.GetInt(i, count++); 													///奖励真气
		//奖励物品ID
		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		funPkgItem.setArgs(funStrItem);

		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			for(unsigned int i=0; i<limitPkg.size(); ++i)
			{
				info.m_vAwardItemList.push_back(atoi(getArg(limitPkg, i++).c_str()));
				info.m_vAwardItemNum.push_back(atoi(getArg(limitPkg, i).c_str()));
			}
		}

		//奖励装备
		string funStrItem1 = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem1("|", funStrItem1);
		funPkgItem1.setArgs(funStrItem1);

		for(uint itemSize=0; itemSize<funPkgItem1.size(); ++itemSize)
		{
			info.m_vEquipId.push_back(atoi(getArg(funPkgItem1, itemSize).c_str()));
			info.m_vEquipNum.push_back(1);
		}

		string funStrItem2 = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem2("|", funStrItem2);
		funPkgItem2.setArgs(funStrItem2);

		info.m_nLevelDown = atoi(getArg(funPkgItem2, 0).c_str());
		info.m_nLevelUp = atoi(getArg(funPkgItem2, 1).c_str());

		//这里条件只有一个
		info.SubmitConds[0].type = info.m_nMinorType;
		info.SubmitConds[0].par1 = info.m_nTaskTarget;
		info.SubmitConds[0].par2 = info.m_nTaskTargetNum;

		if(info.m_nMainType == QuestType_Main)
			m_maxMainquestID = info.m_nQuestId;

		m_allQuest[info.m_nQuestId] = info;
	}

	m_maxMainquestID++;  //在此基础上 加1
	return 0;
}

int QuestLoader::InitDailyTaskReward(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		DailyTaskReward info;
		int count = 0;

		info.m_nId = loader.GetInt(i, count++);
		info.m_nType = loader.GetInt(i, count++);
		info.m_nLv = loader.GetInt(i, count++);
		info.m_nrewardMoney = loader.GetInt(i, count++);
		info.m_nrewardExp = loader.GetInt(i, count++);
		info.m_nrewardForce = loader.GetInt(i, count++);
		info.m_nClanContribution = loader.GetInt(i, count++);
		info.m_nCampContribution = loader.GetInt(i, count++);

		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			for(unsigned int i=0; i<limitPkg.size(); ++i)
			{
				info.m_nTaskItemList.push_back(atoi(getArg(limitPkg, i++).c_str()));
				info.m_nTaskItemListNum.push_back(atoi(getArg(limitPkg, i).c_str()));
			}
		}

		info.m_nrewardMoney1 = loader.GetInt(i, count++);
		info.m_nrewardExp1 = loader.GetInt(i, count++);
		info.m_nrewardForce1 = loader.GetInt(i, count++);
		info.m_nClanContribution1 = loader.GetInt(i, count++);
		info.m_nCampContribution1 = loader.GetInt(i, count++);

		funStrItem = loader.GetString(i, count++, "");
		funPkgItem.setArgs(funStrItem);
		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			for(unsigned int i=0; i<limitPkg.size(); ++i)
			{
				info.m_nTaskItemList1.push_back(atoi(getArg(limitPkg, i++).c_str()));
				info.m_nTaskItemListNum1.push_back(atoi(getArg(limitPkg, i).c_str()));
			}
		}

		m_DailyTaskReward.push_back(info);
	}

	return 0;
}


int QuestLoader::InitOneFinishConsume(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		OneFinishConsume info;
		int count = 0;
		count++;

		info.m_Type = (QuestType)loader.GetInt(i, count++);
		info.m_Golden = loader.GetInt(i, count++);

		m_OneFinishConsumeList[info.m_Type] = info;
	}

	return 0;
}

//int QuestLoader::InitTwentyReward(string &file)
//{
//	CSVLoader loader;
//
//	if(!loader.OpenFromFile(file.c_str()))
//	{
//		return -1;
//	}
//
//	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
//	{
//		sTwentyReward info;
//		int count = 1;
//
//		info.m_Level.m_UpLevel   = loader.GetInt(i, count++);
//		info.m_Level.m_DownLevel = loader.GetInt(i, count++);
//		info.m_Type = static_cast<QuestType>(loader.GetInt(i, count++));
//
//		StrArgPkg funPkgItem("|", loader.GetString(i, count++, ""));
//
//		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
//		{
//			StrArgPkg limitPkg(":",funPkgItem[itemSize].getStr());
//
//			info.m_vItemList.push_back(atoi(getArg(limitPkg,0).c_str()));
//			info.m_vItemNum.push_back(atoi(getArg(limitPkg,1).c_str()));
//			info.m_BindTypeList.push_back(1);
//		}
//
//		m_twenty[info.m_Type][info.m_Level] = info;
//	}
//
//	return 0;
//}

//int QuestLoader::InitSection(string &file)
//{
//	CSVLoader loader;
//
//	if(!loader.OpenFromFile(file.c_str()))
//	{
//		return -1;
//	}
//
//	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
//	{
//		sSectionReward info;
//		int count = 0;
//
//		info.m_nSectID = loader.GetInt(i, count++);
//		count++;
//		info.m_nHp = loader.GetInt(i, count++);
//		info.m_nMp = loader.GetInt(i, count++);
//		info.m_npAtt = loader.GetInt(i, count++);
//		info.m_nmAtt = loader.GetInt(i, count++);
//		info.m_npDefen = loader.GetInt(i, count++);
//		info.m_nmDefen = loader.GetInt(i, count++);
//
//		m_section[info.m_nSectID] = info;
//	}
//	return 0;
//}

//int QuestLoader::InitRefreshData(string &file)
//{
//	CSVLoader loader;
//
//	if(!loader.OpenFromFile(file.c_str()))
//	{
//		return -1;
//	}
//
//	LevelData   tempLevel;
//	RefreshData tempData;
//	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
//	{
//		int count = 1;
//
//		tempData.m_Type = static_cast<QuestType>(loader.GetInt(i, count++));
//		tempData.m_Level.m_UpLevel  = loader.GetInt(i, count++);
//		tempData.m_Level.m_DownLevel = loader.GetInt(i, count++);
//
//		tempData.m_Starts = loader.GetInt(i, count++);
//
//		StrArgPkg funPkgItem("|", loader.GetString(i, count++, ""));
//
//		tempData.m_ItemList.clear();
//		tempData.m_NumList.clear();
//		tempData.m_BindTypeList.clear();
//		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
//		{
//			StrArgPkg limitPkg(":",funPkgItem[itemSize].getStr());
//
//			tempData.m_ItemList.push_back(atoi(getArg(limitPkg,0).c_str()));
//			tempData.m_NumList.push_back(atoi(getArg(limitPkg,1).c_str()));
//			tempData.m_BindTypeList.push_back(0);
//		}
//
//		tempData.m_RewardProbability = loader.GetInt(i, count++);
//		tempData.m_RewardVipLevel = loader.GetInt(i, count++);
//		tempData.m_RewardVipProBability = loader.GetInt(i, count++);
//		tempData.m_NumConsume = loader.GetFloat(i, count++);
//		tempData.m_NumProbability = loader.GetInt(i, count++);
//		tempData.m_NumVipLevel = loader.GetInt(i, count++);
//		tempData.m_NumVipProBability = loader.GetInt(i, count++);
//		tempData.m_starFactor = loader.GetFloat(i, count++);
//
//		m_RefreshDataList[tempData.m_Type][tempData.m_Starts].push_back(tempData);
//	}
//
//	return 0;
//}

//int QuestLoader::InitRefreshConsume(string &file)
//{
//	CSVLoader loader;
//
//	if(!loader.OpenFromFile(file.c_str()))
//	{
//		return -1;
//	}
//
//	RefreshConsume tempData;
//	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
//	{
//		int count = 1;
//
//		tempData.m_Starts = loader.GetInt(i, count++);
//		tempData.m_Type = static_cast<QuestType>(loader.GetInt(i, count++));
//		tempData.m_RewardMoney = loader.GetInt(i, count++);
//		tempData.m_RewardGolden = loader.GetInt(i, count++);
//		tempData.m_NumMoney = loader.GetInt(i, count++);
//		tempData.m_NumGolden = loader.GetInt(i, count++);
//
//		m_RefreshConsumeList[tempData.m_Type][tempData.m_Starts] = tempData;
//	}
//
//	return 0;
//}

//int QuestLoader::InitOneFinishConsume(string &file)
//{
//	CSVLoader loader;
//
//	if(!loader.OpenFromFile(file.c_str()))
//	{
//		return -1;
//	}
//
//	OneFinishConsume tempData;
//	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
//	{
//		int count = 1;
//
//		tempData.m_Counts = loader.GetInt(i, count++);
//		tempData.m_Type = static_cast<QuestType>(loader.GetInt(i, count++));
//		tempData.m_Money = loader.GetInt(i, count++);
//		tempData.m_Golden = loader.GetInt(i, count++);
//
//		m_OneFinishConsumeList[tempData.m_Type][tempData.m_Counts] = tempData;
//	}
//
//	return 0;
//}

//int QuestLoader::InitLua(string& file)
//{
//	if(tlua.LoadFile(file) != 0 || tlua.Call() != 0)
//	{
//		return -1;
//	}
//
//	tlua.SetLuaFunction("getCountsReward");
//	if(tlua.Call(0,2) != 0)
//	{
//		return -1;
//	}
//
//	m_WeekLua.m_AllRing 	 = tlua.GetInt();
//	m_WeekLua.m_CountsReward = tlua.GetInt();
//
//	return 0;
//}

Entry_Quest * QuestLoader::GetQuestInfo(int questID)
{
	map<DWORD , Entry_Quest>::iterator it = m_allQuest.find(questID);
	if(it == m_allQuest.end())
	{
		return 0;
	}
	return &it->second;
}


const DailyTaskReward* QuestLoader::GetTwentyInfo(QuestType type,int level)
{
	vector<DailyTaskReward>::iterator itr = m_DailyTaskReward.begin();
	for(; itr != m_DailyTaskReward.end(); ++itr)
	{
		if(itr->m_nType == (DWORD)type && itr->m_nLv == (DWORD)level)
		{
			return &(*itr);
		}
	}

	return NULL;
}

//sTwentyReward* QuestLoader::GetTwentyInfo(QuestType type,int level)
//{
//	map<QuestType,map<LevelData,sTwentyReward> >::iterator tempOutIt = m_twenty.find(type);
//	if(tempOutIt == m_twenty.end())
//	{
//		return NULL;
//	}
//
//	map<LevelData,sTwentyReward>::iterator tempInIt = tempOutIt->second.begin();
//	for(;tempInIt != tempOutIt->second.end();++tempInIt)
//	{
//		if(level < tempInIt->first.m_DownLevel || level > tempInIt->first.m_UpLevel)
//		{
//			continue ;
//		}
//
//		return &tempInIt->second;
//	}
//
//	return NULL;
//}

//日常任务从20001开始,假设日常任务最大1000个
void QuestLoader::GetLvDailyTaskNum(const DWORD &lv, std::vector<DWORD> &quests, const DWORD & needPassID)
{
	int i = DAILYTASK_START;

	for(;i < DAILYTASK_START + MAX_DAILYCLANTASK_NUM;i++)
	{
		map<DWORD , Entry_Quest>::iterator it = m_allQuest.find(i);
		if(it == m_allQuest.end())
			continue;

		if(it->second.m_nMainType != QuestType_Functional)
			continue; /*设定的 初始值错误*/

		if(needPassID == it->first)
			continue;  //过滤与本次相同的任务

		if(it->second.m_nLevelDown <= lv && it->second.m_nLevelUp >= lv)
		{
			quests.push_back(it->first);
		}
	}
}

//帮派任务从30001开始
void QuestLoader::GetLvClanTaskNum(const DWORD &lv, std::vector<DWORD> &quests, const DWORD & needPassID)
{
	int i = CLANTASK_START;

	for(;i < CLANTASK_START + MAX_DAILYCLANTASK_NUM;i++)
	{
		map<DWORD , Entry_Quest>::iterator it = m_allQuest.find(i);
		if(it == m_allQuest.end())
			continue;

		if(it->second.m_nMainType != QuestType_Clan)
			continue;  /*设定的 初始值错误*/

		if(needPassID == it->first)
			continue;  //过滤与本次相同的任务

		if(it->second.m_nLevelDown <= lv && it->second.m_nLevelUp >= lv)
		{
			quests.push_back(it->first);
		}
	}
}

//阵营任务从40001开始
void QuestLoader::GetLvCampaskNum(const DWORD &lv, std::vector<DWORD> &quests, const DWORD & needPassID)
{
	int i = CAMPTASK_START;

	for(;i < CAMPTASK_START + MAX_DAILYCLANTASK_NUM;i++)
	{
		map<DWORD , Entry_Quest>::iterator it = m_allQuest.find(i);
		if(it == m_allQuest.end())
			continue;

		if(it->second.m_nMainType != QuestType_Camp)
			continue;  /*设定的 初始值错误*/

		if(needPassID == it->first)
			continue;  //过滤与本次相同的任务

		if(it->second.m_nLevelDown <= lv && it->second.m_nLevelUp >= lv)
		{
			quests.push_back(it->first);
		}
	}
}

void QuestLoader::GetLvWeekTaskNum(DWORD lv, std::vector<DWORD> &quests,DWORD needPassID)
{
	int i = WEEKTASK_START;

	for(;i < MAX_DAILYCLANTASK_NUM + WEEKTASK_START;i++)
	{
		map<DWORD , Entry_Quest>::iterator it = m_allQuest.find(i);
		if(it == m_allQuest.end())
			continue;

		if(it->second.m_nMainType != QuestType_Week)
			continue;  /*设定的 初始值错误*/

		if(needPassID == it->first)
			continue;  //过滤与本次相同的任务

		if(it->second.m_nLevelDown <= lv && it->second.m_nLevelUp >= lv)
		{
			quests.push_back(it->first);
		}
	}
}

DWORD QuestLoader::getNextQuestID(DWORD currentQuestID,QuestType type)
{
	map<DWORD,Entry_Quest>::iterator tempIt = m_allQuest.find(currentQuestID);
	if(tempIt == m_allQuest.end())
		return 0;

	//先++
	++tempIt;

	//取下一个任务ID
	for(;tempIt != m_allQuest.end();++tempIt)
	{
		if((DWORD)type != tempIt->second.m_nMainType)
			continue;

		return tempIt->second.m_nQuestId;
	}

	return 0;
}

//int QuestLoader::refreshStarts(int level,int currentStarts,int vipLevel,QuestType type)
//{
//	map<QuestType,map<int,vector<RefreshData> > >::iterator tempIt = m_RefreshDataList.find(type);
//	if(tempIt == m_RefreshDataList.end())
//	{
//		return 0;
//	}
//
//	int tempProbability = rand() % 10000 + 1;
//	map<int,vector<RefreshData> >::iterator tempInIt = tempIt->second.find(currentStarts);
//	int tempResultLevel = currentStarts;
//
//	++tempInIt;
//
//	while(tempInIt != tempIt->second.end())
//	{
//		for(size_t i = 0;i < tempInIt->second.size();++i)
//		{
//			if(level < tempInIt->second[i].m_Level.m_DownLevel || level > tempInIt->second[i].m_Level.m_UpLevel)
//			{
//				continue ;
//			}
//
//			int tempCurrentRate = tempInIt->second[i].m_RewardProbability;
//			if(vipLevel >= tempInIt->second[i].m_RewardVipLevel)
//			{
//				tempCurrentRate += tempInIt->second[i].m_RewardVipProBability;
//			}
//
//			if(tempCurrentRate < tempProbability)
//			{
//				return tempResultLevel;
//			}
//
//			tempResultLevel = tempInIt->second[i].m_Starts;
//
//			break;
//		}
//
//		++tempInIt;
//	}
//
//	return tempResultLevel;
//}
//
//int QuestLoader::refreshNumStarts(int level,int currentStarts,int vipLevel,QuestType type,float& consume)
//{
//	map<QuestType,map<int,vector<RefreshData> > >::iterator tempIt = m_RefreshDataList.find(type);
//	if(tempIt == m_RefreshDataList.end())
//	{
//		return 0;
//	}
//
//	int tempProbability = rand() % 10000 + 1;
//	map<int,vector<RefreshData> >::iterator tempInIt = tempIt->second.find(currentStarts);
//	int tempResultLevel = currentStarts;
//
//	++tempInIt;
//
//	while(tempInIt != tempIt->second.end())
//	{
//		for(size_t i = 0;i < tempInIt->second.size();++i)
//		{
//			if(level < tempInIt->second[i].m_Level.m_DownLevel || level > tempInIt->second[i].m_Level.m_UpLevel)
//			{
//				continue ;
//			}
//
//			int tempCurrentRate = tempInIt->second[i].m_NumProbability;
//			if(vipLevel >= tempInIt->second[i].m_NumVipLevel)
//			{
//				tempCurrentRate += tempInIt->second[i].m_NumVipProBability;
//			}
//
//			if(tempCurrentRate < tempProbability)
//			{
//				return tempResultLevel;
//			}
//
//			tempResultLevel = tempInIt->second[i].m_Starts;
//
//			consume = tempInIt->second[i].m_NumConsume;
//
//			break;
//		}
//
//		++tempInIt;
//	}
//
//	return tempResultLevel;
//}
//
//const RefreshData* QuestLoader::getOneFinishRewart(int level,int currentStarts,QuestType type)
//{
//	map<QuestType,map<int,vector<RefreshData> > >::iterator tempIt = m_RefreshDataList.find(type);
//	if(tempIt == m_RefreshDataList.end())
//	{
//		return NULL;
//	}
//
//	map<int,vector<RefreshData> >::iterator tempInIt = tempIt->second.find(currentStarts);
//
//	for(size_t i = 0;i < tempInIt->second.size();++i)
//	{
//		if(level < tempInIt->second[i].m_Level.m_DownLevel || level > tempInIt->second[i].m_Level.m_UpLevel)
//		{
//			continue ;
//		}
//
//		return &tempInIt->second[i];
//	}
//
//	return NULL;
//}
//
//sSectionReward *QuestLoader::GetSectReward(DWORD id)
//{
//	map<int , sSectionReward>::iterator it = m_section.find(id);
//
//	if(it != m_section.end())
//	{
//		return &it->second;
//	}
//
//	return NULL;
//}

bool QuestLoader::IsSameChap(const uint& questID, uint *chap)
{
	Entry_Quest *curQuest =  QuestLoader::GetInstance()->GetQuestInfo(questID);
	Entry_Quest *newQuest =  QuestLoader::GetInstance()->GetQuestInfo(questID+1);

	if(curQuest && newQuest && curQuest->m_nQuestChap != newQuest->m_nQuestChap)
	{
		*chap = curQuest->m_nQuestChap;
		return false;
	}
	return true;
}

//获得指定任务之前的所有章节，不包含当任务前章节
void QuestLoader::getBeforeChapterIDList(DWORD questID,map<int,int>& chapterIDList)
{
	for(DWORD i = MAIN_TASK_START;i < questID;++i)
	{
		map<DWORD,Entry_Quest>::iterator tempIt = m_allQuest.find(i);
		if(tempIt == m_allQuest.end())
			continue;

		chapterIDList[tempIt->second.m_nQuestChap] = 0;
	}

	//移除当前任务所在章节
	map<DWORD,Entry_Quest>::iterator tempCurrentIt = m_allQuest.find(questID);
	map<DWORD,Entry_Quest>::iterator tempNextIt    = m_allQuest.find(questID + 1);
	if(tempCurrentIt != m_allQuest.end() && tempNextIt != m_allQuest.end() &&
	   tempCurrentIt->second.m_nQuestChap == tempNextIt->second.m_nQuestChap
	   )
	{
		map<int,int>::iterator tempIt  = chapterIDList.find(tempCurrentIt->second.m_nQuestChap);
		if(tempIt != chapterIDList.end())
		{
			chapterIDList.erase(tempIt);
		}
	}

	return ;
}

//const RefreshConsume* QuestLoader::GetRefleshFinishDeduction(QuestType type,int starts)
//{
//	map<QuestType,map<int,RefreshConsume> >::iterator tempOutIt = m_RefreshConsumeList.find(type);
//	if(tempOutIt == m_RefreshConsumeList.end())
//	{
//		return NULL;
//	}
//
//	const RefreshConsume* tempResultData = NULL;
//	map<int,RefreshConsume>::iterator tempInIt = tempOutIt->second.begin();
//	for(;tempInIt != tempOutIt->second.end();++tempInIt)
//	{
//		if(tempInIt->first == starts)
//		{
//			tempResultData = &tempInIt->second;
//			break;
//		}
//		if(tempInIt->first > starts)
//		{
//			break;
//		}
//
//		tempResultData = &tempInIt->second;
//	}
//
//	return tempResultData;
//}

const OneFinishConsume* QuestLoader::GetOneFinishDeduction(QuestType type)
{
	map<QuestType,OneFinishConsume>::iterator itr = m_OneFinishConsumeList.find(type);
	if(itr != m_OneFinishConsumeList.end())
	{
		return &(itr->second);
	}

	return NULL;
}

//const OneFinishConsume* QuestLoader::GetOneFinishDeduction(QuestType type,int counts)
//{
//	map<QuestType,map<int,OneFinishConsume> >::iterator tempOutIt = m_OneFinishConsumeList.find(type);
//	if(tempOutIt == m_OneFinishConsumeList.end())
//	{
//		return NULL;
//	}
//
//	const OneFinishConsume* tempResultData = NULL;
//	map<int,OneFinishConsume>::iterator tempInIt = tempOutIt->second.begin();
//	for(;tempInIt != tempOutIt->second.end();++tempInIt)
//	{
//		if(tempResultData == NULL)
//		{
//			tempResultData = &tempInIt->second;
//		}
//		if(tempInIt->first == counts)
//		{
//			tempResultData = &tempInIt->second;
//			break;
//		}
//		if(tempInIt->first > counts)
//		{
//			break;
//		}
//
//		tempResultData = &tempInIt->second;
//	}
//
//	return tempResultData;
//}

int QuestLoader::GetOneFinishSginleCost(QuestType type)
{
	map<QuestType,OneFinishConsume>::iterator itr = m_OneFinishConsumeList.find(type);
	if(itr != m_OneFinishConsumeList.end())
	{
		return itr->second.m_Golden;
	}

	return -1;
}

//int QuestLoader::GetOneFinishSginleCost(QuestType type)
//{
//	map<QuestType,map<int,OneFinishConsume> >::iterator it = m_OneFinishConsumeList.find(type);
//	if (it != m_OneFinishConsumeList.end())
//	{
//		// 0 是代表一键完成所有环任务 特殊值
//		map<int, OneFinishConsume>::iterator sIt = it->second.find(1);
//		if (it->second.end() != sIt)
//		{
//			return sIt->second.m_Golden;
//		}
//	}
//
//	return -1;
//}

//int QuestLoader::GetTaskExpReward(int lv,float starFactor)
//{
//	UpgradeInfo nowInfo;
//
//	UpgradeLoader::GetInstance()->GetUpgradeByLv(lv, nowInfo);
//	if(nowInfo.lv == 0)
//	{
//		LOG_ERROR(FILEINFO, "get upgrade info error and upgrade failed");
//	}
//	else
//	{
//		tlua.SetLuaFunction("GetTaskExpReward");
//		tlua.SetInt(nowInfo.factoryExp);
//		tlua.SetFloat(starFactor);
//
//		if(tlua.Call(2, 1))
//		{
//			return 0;
//		}
//		int temp = (int)(tlua.GetFloat());
//
//		return temp;
//	}
//
//	return 0;
//}
//
//int QuestLoader::GetExtraTaskExpReward(int lv)
//{
//	UpgradeInfo nowInfo;
//
//	UpgradeLoader::GetInstance()->GetUpgradeByLv(lv, nowInfo);
//	if(nowInfo.lv == 0)
//	{
//		LOG_ERROR(FILEINFO, "get upgrade info error and upgrade failed");
//	}
//	else
//	{
//		tlua.SetLuaFunction("GetExtraTaskExpReward");
//		tlua.SetFloat(nowInfo.factoryExp);
//
//		if(tlua.Call(1, 1))
//		{
//			return 0;
//		}
//		int temp = (int)(tlua.GetFloat());
//
//		return temp;
//	}
//
//	return 0;
//}
//
//int QuestLoader::GetTaskMoneyReward(QuestType type,int lv,float starFactor)
//{
//		tlua.SetLuaFunction("GetTaskMoneyReward");
//		tlua.SetInt(type);
//		tlua.SetInt(lv);
//		tlua.SetFloat(starFactor);
//
//		if(tlua.Call(3, 1))
//		{
//			return 0;
//		}
//
//		return (int)tlua.GetFloat();
//}
//
//int QuestLoader::GetTaskForceReward(QuestType type,int lv,float starFactor)
//{
//	tlua.SetLuaFunction("GetTaskForceReward");
//	tlua.SetInt(type);
//	tlua.SetInt(lv);
//	tlua.SetFloat(starFactor);
//
//	if(tlua.Call(3, 1))
//	{
//		return 0;
//	}
//
//	return (int)tlua.GetFloat();
//}
//
//bool QuestLoader::GetExtraTaskReward(QuestType type,int lv,int& nItemId, int& nItemNum)
//{
//	tlua.SetLuaFunction("GetExtraTaskReward");
//	tlua.SetInt(type);
//	tlua.SetInt(lv);
//
//	if(tlua.Call(2, 2))
//	{
//		return false;
//	}
//
//	nItemNum = (int)(tlua.GetFloat());
//	nItemId = tlua.GetInt();
//
//	return true;
//}
//
//int QuestLoader::initWeekTask(string& fileName)
//{
//	CSVLoader loader;
//
//	if(!loader.OpenFromFile(fileName.c_str()))
//	{
//		return -1;
//	}
//
//	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
//	{
//		int count = 0;
//
//		WeekTaskData info;
//
//		info.m_ID		 = loader.GetInt(i, count++);
//		info.m_Type 	 = (eWeekTaskType)loader.GetInt(i, count++);
//		info.m_LowLevel  = loader.GetInt(i, count++);
//		info.m_UpLevel   = loader.GetInt(i, count++);
//
//		WeekItemProbability tData;
//		tData.m_ItemID = 0;
//		tData.m_Probability = 0;
//		tData.m_Num = 0;
//
//		StrArgPkg tItemOne(":", loader.GetString(i, count++, ""));
//		tData.m_ItemID = atoi(getArg(tItemOne,0).c_str());
//		tData.m_Num = atoi(getArg(tItemOne,1).c_str());
//		info.m_FixReward.push_back(tData);
//
//		StrArgPkg tItemTow(":", loader.GetString(i, count++, ""));
//		tData.m_ItemID = atoi(getArg(tItemTow,0).c_str());
//		tData.m_Num = atoi(getArg(tItemTow,1).c_str());
//		info.m_FixReward.push_back(tData);
//
//		for(int z = 0;z < 2;++z)
//		{
//			StrArgPkg tItemThree(";:", loader.GetString(i, count++, ""));
//			vector<WeekItemProbability> tVector;
//			for(size_t j = 0;j < tItemThree.size();++j)
//			{
//				tData.m_ItemID = atoi(getArg(tItemThree[j],0).c_str());
//				tData.m_Probability = atoi(getArg(tItemThree[j],1).c_str());
//				tData.m_Num = atoi(getArg(tItemThree[j],2).c_str());
//				tVector.push_back(tData);
//			}
//
//			info.m_RandReward.push_back(tVector);
//		}
//
//		info.m_BindType = loader.GetInt(i, count++);
//
//		m_WeekTasks[info.m_ID] = info;
//	}
//
//	return 0;
//}
//
//void QuestLoader::getWeekTaskFixReward(int lv,WeekItems& rewards,eWeekTaskType type)
//{
//	map<int,WeekTaskData>::iterator tIt = m_WeekTasks.begin();
//	for(;tIt != m_WeekTasks.end();++tIt)
//	{
//		if(tIt->second.m_Type != type)
//		{
//			continue ;
//		}
//
//		if(lv < tIt->second.m_LowLevel || lv > tIt->second.m_UpLevel)
//		{
//			continue ;
//		}
//
//		for(size_t i = 0;i < tIt->second.m_FixReward.size();++i)
//		{
//			rewards.m_Items.push_back(tIt->second.m_FixReward[i].m_ItemID);
//			rewards.m_Num.push_back(tIt->second.m_FixReward[i].m_Num);
//			rewards.m_BindType.push_back(tIt->second.m_BindType);
//		}
//
//		break ;
//	}
//
//	return ;
//}
//
//void QuestLoader::getWeekTaskRandReward(int lv,WeekItems& rewards,eWeekTaskType type)
//{
//	map<int,WeekTaskData>::iterator tIt = m_WeekTasks.begin();
//	for(;tIt != m_WeekTasks.end();++tIt)
//	{
//		if(tIt->second.m_Type != type)
//		{
//			continue ;
//		}
//
//		if(lv < tIt->second.m_LowLevel || lv > tIt->second.m_UpLevel)
//		{
//			continue ;
//		}
//
//		for(size_t i = 0;i < tIt->second.m_RandReward.size();++i)
//		{
//			ProbabilityVector<WeekItemProbability> tHandle(tIt->second.m_RandReward[i]);
//			short tIndex = tHandle.calculation();
//			if(tIndex == INVALID_INDEX)
//				continue;
//
//			const WeekItemProbability& tData = tIt->second.m_RandReward[i][tIndex];
//
//			rewards.m_Items.push_back(tData.m_ItemID);
//			rewards.m_Num.push_back(tData.m_Num);
//			rewards.m_BindType.push_back(tIt->second.m_BindType);
//		}
//
//		break ;
//	}
//
//	return ;
//}

int QuestLoader::getMainTask(int level)
{
	int tResult = MAIN_TASK_START;
	for(int i = MAIN_TASK_START;i < MAIN_TASK_START + MAX_DAILYCLANTASK_NUM;++i)
	{
		map<DWORD,Entry_Quest>::iterator tIt = m_allQuest.find(i);
		if(tIt == m_allQuest.end())
		{
			continue;
		}

		if(tIt->second.m_nMinorType == QuestCondType_SendMail)
		{
			continue;
		}

		if(tIt->second.m_nMinorType == QuestCondType_UseItem)
		{
			continue;
		}

		tResult = tIt->second.m_nQuestId;
		if(tIt->second.m_nLevelDown > (DWORD)level)
		{
			break;	
		}
	}

	return tResult;
}
