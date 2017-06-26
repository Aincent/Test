/*
 * AnswerLoader.cpp
 *
 *  Created on: 2016年11月10日
 *      Author: root
 */

#include "AnswerLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "Log/Logger.h"
#include "MessageCommonRet.h"
#include <math.h>

AnswerLoader::AnswerLoader()
{

}

AnswerLoader::~AnswerLoader()
{

}


int AnswerLoader::init(string& file)
{
	string tempAnswerStr 	 = file + "Data/ExamData.csv";
	string tempRewardStr 	 = file + "Data/AnswerReward.csv";

	if(initAnswerFile(tempAnswerStr) != 0 || initRewardFile(tempRewardStr) != 0)
		return -1;

	return 0;
}


int AnswerLoader::initAnswerFile(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	int count       	  = 0;
	unsigned int tempRow  = loader.GetRowCount();
	for(unsigned int i = 0;i < tempRow;++i)
	{
		count 		    = 0;
		int id   = loader.GetInt(i, count++);

		m_FileDataList.push_back(id);
	}

	return 0;
}

int AnswerLoader::initRewardFile(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	int count       	  = 0;
	unsigned int tempRow  = loader.GetRowCount();
	for(unsigned int i = 0;i < tempRow;++i)
	{
		count = 0;

		AnswerReward tempData;
		tempData.m_ID 		  = loader.GetInt(i, count++);
		tempData.m_Type 	  = loader.GetInt(i, count++);
		tempData.m_Limit = loader.GetInt(i, count++);

		if(tempData.m_Type == 1)
		{
			tempData.m_Value = loader.GetInt(i, count++);
		}
		else
		{
			//限制使用物品
			string funStrItem = loader.GetString(i, count++, "");
			StrArgPkg funPkgItem("|", funStrItem);
			for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
			{

				StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
				for(unsigned int i=0; i<limitPkg.size(); ++i)
				{
					AnswerItem tempItem;
					tempItem.m_ItemID = atoi(getArg(limitPkg, i++).c_str());
					tempItem.m_Counts = atoi(getArg(limitPkg, i).c_str());

					if(tempItem.m_ItemID == 0)
						break;

					tempData.m_ItemList.push_back(tempItem);
				}
			}
		}

		m_RewardDataList[tempData.m_Type].push_back(tempData);
	}

	return 0;

}

const AnswerReward* AnswerLoader::GetAnswerReward(int nType,int nNeeds)
{
	map<int,vector<AnswerReward> >::iterator itr = m_RewardDataList.find(nType);
	if(itr != m_RewardDataList.end())
	{
		for(size_t i = 0; i < itr->second.size(); i++)
		{
			if(nType == 1)
			{
				return &((itr->second)[i]);
			}
			else
			{
				if((itr->second)[i].m_Limit ==  nNeeds)
				{
					return &((itr->second)[i]);
				}
			}
		}
	}

	return NULL;
}
