/*
 * GlobalHandle.h
 *
 *  Created on: 2016年4月29日
 *      Author: root
 */

#ifndef GLOBALHANDLE_H_
#define GLOBALHANDLE_H_

#include "../StatisticDefine.h"

//class StatisticBase;
class StatGlobalBase;

#define GLOBAL_STATISTIC(MAIN_T, MINOR_T, T_D, O) GlobalStatist::Statist(MAIN_T, MINOR_T, T_D, O);

class GlobalStatist
{
public:

	static void Statist(const GlobalType::Main::E_TYPE& mainT, const GlobalType::Minor::E_TYPE& minorT, const StatistSaveType::Type_Data& T, StatGlobalBase* base);

private:
	GlobalStatist() {}
	~GlobalStatist() {}
};


#endif /* GLOBALHANDLE_H_ */
