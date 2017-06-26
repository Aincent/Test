/*
 * Spring.h
 *
 *  Created on: 2014年10月24日
 *      Author: root
 */

#ifndef SPRING_H_
#define SPRING_H_


#include "../Object.h"

using namespace std;

class Spring : public Object
{
public:
	Spring(eObjType type):Object(type),m_range(0),m_hpRate(0)
	{

	}

	~Spring()
	{

	}

	void SetRange(int value) {	 m_range = value; }
	int GetRange() {	 return m_range; }

	void SetHPRate(int value) {	 m_hpRate = value; }
	int GetHPRate() {	 return m_hpRate; }

private:
	int m_range;
	int m_hpRate; //加血的比率
};




#endif /* SPRING_H_ */
