/*

 * Probability.h
 *
 *  Created on: 14/04/2015
 *      Author: root
 */

#ifndef PROBABILITY_H_
#define PROBABILITY_H_

#include "define.h"
#include "Smart_Ptr.h"
#include "Singleton.h"

using namespace std;
using namespace CommBaseOut;

#define INVALID_INDEX (-1)    //无效索引


template <class T>
class ProbabilityVector
{
public:
	ProbabilityVector(const vector<T>& dataList) : m_DataList(dataList){}

	~ProbabilityVector(){}

public:
	short calculation();

private:
	const vector<T>& m_DataList;
};

template <class T,class key>
class ProbabilityMap
{
public:
	ProbabilityMap(const map<key,T>& dataList) : m_DataList(dataList){}

	virtual ~ProbabilityMap(){}

	typedef typename map<key,T>::const_iterator	DataMapIt;

public:
	virtual key calculation();

private:
	const map<key,T>& m_DataList;
};





#include "Probability.hpp"



#endif /* PROBABILITY_H_ */
