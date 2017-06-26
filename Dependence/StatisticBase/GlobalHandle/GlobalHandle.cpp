/*
 * GlobalHandle.cpp
 *
 *  Created on: 2016年4月29日
 *      Author: root
 */
#include "../StatisticBase.h"
#include "GlobalHandle.h"


void GlobalStatist::Statist(const GlobalType::Main::E_TYPE& mainT, const GlobalType::Minor::E_TYPE& minorT, const StatistSaveType::Type_Data& T, StatGlobalBase* base)
{
	if (NULL == base)
	{
		return;
	}

	base->GlobalStatist(mainT, minorT, T._t, T.Get());
}

