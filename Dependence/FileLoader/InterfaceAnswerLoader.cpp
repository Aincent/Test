/*
 * InterfaceAnswerLoader.cpp
 *
 *  Created on: 15/07/2015
 *      Author: root
 */

#include "InterfaceAnswerLoader.h"
#include "../CSVLoader.h"


InterfaceAnswerLoader::InterfaceAnswerLoader()
{

}

InterfaceAnswerLoader::~InterfaceAnswerLoader()
{

}

int InterfaceAnswerLoader::init(string& file)
{
	string tempAnswerStr 	 = file + "Data/ExamData.csv";
	string tempRewardStr 	 = file + "Data/AnswerReward.csv";
	string tempOtherStr  	 = file + "Data/AnsweraaInfo.csv";
	string tempProbablityStr = file + "Data/Answernum.csv";

	if(initAnswerFile(tempAnswerStr) != 0 || initRewardFile(tempRewardStr) != 0 || initOtherFile(tempOtherStr) != 0 || initProbablityFile(tempProbablityStr) != 0)
		return -1;

	return 0;
}

const AnswerFileData* InterfaceAnswerLoader::getFileDataByID(int id)
{
	map<short,vector<AnswerFileData> >::iterator tempIt = m_FileDataList.begin();
	for(;tempIt != m_FileDataList.end();++tempIt)
	{
		for(size_t i = 0;i < tempIt->second.size();++i)
		{
			if(tempIt->second[i].m_ID != id)
				continue;

			return &tempIt->second[i];
		}
	}

	return NULL;
}

const AnswerRewardData* InterfaceAnswerLoader::getRewardDataByID(int id)
{
	map<eRwardAnswerType,vector<AnswerRewardData> >::iterator tempIt = m_RewardDataList.begin();
	for(;tempIt != m_RewardDataList.end();++tempIt)
	{
		for(size_t i = 0;i < tempIt->second.size();++i)
		{
			if(tempIt->second[i].m_ID != id)
				continue;

			return &tempIt->second[i];
		}
	}

	return NULL;
}

void InterfaceAnswerLoader::getItemByItemID(int id,vector<DWORD>& itemIDList,vector<DWORD>& numList,vector<WORD>& bindTypeList)
{
	const AnswerRewardData* tempData = getRewardDataByID(id);
	if(tempData == NULL)
		return ;

	for(size_t i = 0;i < tempData->m_ItemList.size();++i)
	{
		itemIDList.push_back(tempData->m_ItemList[i].m_ItemID);
		numList.push_back(tempData->m_ItemList[i].m_Counts);
		bindTypeList.push_back(tempData->m_ItemList[i].m_BindType);
	}

	return ;
}

int InterfaceAnswerLoader::initAnswerFile(string& file)
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
		AnswerFileData tempData;

		count 		    = 0;
		tempData.m_ID   = loader.GetInt(i, count++);
		count 		   += 4;
		tempData.m_Type = loader.GetInt(i, count++);

		m_FileDataList[tempData.m_Type].push_back(tempData);
	}

	return 0;
}

int InterfaceAnswerLoader::initRewardFile(string& file)
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

		AnswerRewardData tempData;
		tempData.m_ID 		  = loader.GetInt(i, count++);
		tempData.m_Type 	  = loader.GetInt(i, count++);
		tempData.m_LowerLimit = loader.GetInt(i, count++);
		tempData.m_UpperLimit = loader.GetInt(i, count++);

		int tempSurplusCounts = loader.GetInt(i, count++);

		for(int j = 0;j < tempSurplusCounts;++j)
		{
			AnswerItemData tempItemData;

			tempItemData.m_ItemID = loader.GetInt(i, count++);
			tempItemData.m_Counts = loader.GetInt(i, count++);

			if(tempItemData.m_ItemID == 0)
				break;

			tempData.m_ItemList.push_back(tempItemData);
		}

		m_RewardDataList[(eRwardAnswerType)tempData.m_Type].push_back(tempData);
	}

	return 0;
}

int InterfaceAnswerLoader::initOtherFile(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	int count = 0;

	count++;

	m_OtherData.m_RightAnswerScore = loader.GetInt(0, count++);
	m_OtherData.m_AnswerCounts 	   = loader.GetInt(0, count++);
	m_OtherData.m_VipDoubleCounts  = loader.GetInt(0, count++);
	m_OtherData.m_VipDoubleGolden  = loader.GetInt(0, count++);
	m_OtherData.m_VIPExcludeCounts = loader.GetInt(0, count++);
	m_OtherData.m_VIPExcludeGolden = loader.GetInt(0, count++);
	m_OtherData.m_RankingCunts 	   = loader.GetInt(0, count++);
	m_OtherData.m_CountDownTime    = loader.GetInt(0, count++);
	m_OtherData.m_ChoiceTime	   = loader.GetInt(0, count++);
	m_OtherData.m_ResultTime	   = loader.GetInt(0, count++);
	m_OtherData.m_DestroyTime	   = loader.GetInt(0, count++);
	m_OtherData.m_MailID		   = loader.GetInt(0, count++);


	unsigned int tempAllColCounts = loader.GetColCount();
	tempAllColCounts -= count;
	tempAllColCounts++;
	tempAllColCounts /= 2;

	for(unsigned int i = 0;i < tempAllColCounts;++i)
	{
		short tempCounts = loader.GetInt(0, count++);
		int tempScore    = loader.GetInt(0, count++);

		m_OtherData.m_StraightList[tempCounts] = tempScore;
	}

	m_OtherData.m_dayAnswerCnt = loader.GetInt(0, count++);

	return 0;
}

int InterfaceAnswerLoader::initProbablityFile(string& file)
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
		AnswerProbability tempData;
		count = 0;
		tempData.m_ID 	  =  loader.GetInt(i, count++);
		tempData.m_Counts = loader.GetInt(i, count++);

		if(tempData.m_ID <= 0)
			return -1;

		m_ProbabilityList.push_back(tempData);
	}

	return 0;
}







