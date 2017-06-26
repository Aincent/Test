/*
 * MonsterExpParam.cpp
 *
 *  Created on: 2015年12月29日
 *      Author: root
 */
#include "MonsterExpParam.h"
#include "CSVLoader.h"
#include "util.h"
#include <math.h>

MonsterExpParam::MonsterExpParam()
{
	LoadFile();
}

MonsterExpParam::~MonsterExpParam()
{

}

void MonsterExpParam::LoadFile()
{
	std::string strFileFullPath;
	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return;
	}

	string strFile = strFileFullPath + "Data/monsterExpAdd.csv";
	CSVLoader loader;

	if(!loader.OpenFromFile(strFile.c_str()))
	{
		return;
	}

	//ID	等级差值下限	等级差值上限	经验获得比例
	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		loader.GetInt(i, count++);

		MonsterExpData info;
		info.minlv = loader.GetInt(i, count++);
		info.maxlv = loader.GetInt(i, count++);
		info.expRate = loader.GetFloat(i, count++);
		m_param_data.push_back(info);
	}

}

int MonsterExpParam::GetExp(int monsterlv, int playerlv, int monsterExp)
{
	int temp = playerlv - monsterlv;

	for(size_t i=0; i<m_param_data.size(); ++i)
	{
		if(temp >= m_param_data[i].minlv && temp <= m_param_data[i].maxlv)
		{
			return (int)ceil(monsterExp*m_param_data[i].expRate*0.01f);
		}
	}
	return 1;
}

