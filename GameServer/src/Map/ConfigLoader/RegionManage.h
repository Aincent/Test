/*
 * RegionManage.h
 *
 *  Created on: 01/12/2015
 *      Author: root
 */

#ifndef REGIONMANAGE_H_
#define REGIONMANAGE_H_

#include "Singleton.h"
#include "Lua/LuaInterface.h"

using namespace CommBaseOut;

class RegionManage : public Singleton<RegionManage>
{
public:
	RegionManage();

	~RegionManage();

public:
	int init(string& str);

	int getIntervalExp(int baseValue,int level,double multiple);

private:
	CLuaInterface m_Lua;
};


#endif /* REGIONMANAGE_H_ */
