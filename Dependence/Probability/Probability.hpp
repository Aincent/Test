/*
 * Probability.hpp
 *
 *  Created on: 14/04/2015
 *      Author: root
 */


/*********vector类********/
template <class T>
short ProbabilityVector<T>::calculation()
{
	if(m_DataList.size() == 0)
		return INVALID_INDEX;

	//先算出概率总和
	int tempAllProbability = 0;
	for(size_t i = 0;i < m_DataList.size();++i)
	{
		tempAllProbability += m_DataList[i].m_Probability;
	}

	if(tempAllProbability == 0)
		return INVALID_INDEX;

	//随机一个概率
	int tempRandom = rand() % tempAllProbability + 1;
	int tempBefore = 0;

	for(size_t i = 0;i < m_DataList.size();++i)
	{
		if(m_DataList[i].m_Probability == 0)
			continue;

		if(tempRandom > tempBefore && tempRandom <= tempBefore + m_DataList[i].m_Probability)
		{
			return i;
		}

		tempBefore += m_DataList[i].m_Probability;
	}

	return INVALID_INDEX;
}

/*********map类********/
template <class T,class key>
key ProbabilityMap<T,key>::calculation()
{
	if(m_DataList.size() == 0)
		return (key)INVALID_INDEX;

	//先算出概率总和
	int tempAllProbability = 0;
	DataMapIt tempIt = m_DataList.begin();

	for(;tempIt != m_DataList.end();++tempIt)
	{
		tempAllProbability += tempIt->second.m_Probability;
	}

	if(tempAllProbability == 0)
		return (key)INVALID_INDEX;

	//随机一个概率
	int tempRandom = rand() % tempAllProbability + 1;
	int tempBefore = 0;

	tempIt = m_DataList.begin();
	for(;tempIt != m_DataList.end();++tempIt)
	{
		if(tempIt->second.m_Probability == 0)
			continue;

		if(tempRandom > tempBefore && tempRandom <= tempBefore + tempIt->second.m_Probability)
		{
			return tempIt->first;
		}

		tempBefore += tempIt->second.m_Probability;
	}

	return (key)INVALID_INDEX;
}









