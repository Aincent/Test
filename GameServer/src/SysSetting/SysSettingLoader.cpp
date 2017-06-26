/*
 * SysSettingLoader.cpp
 *
 *  Created on: 2015年4月8日
 *      Author: root
 */

#include "SysSettingLoader.h"
#include "Log/Logger.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"

int SysSettingLoader::Init(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		int id= loader.GetInt(i, count++, 0);
		count++;
		int lv = loader.GetInt(i, count++, 0);

		if(id == 0)continue;

		m_SysSettingInfo[id] = lv;
	}

	return 0;
}


