/*
 * AntiAddictionSystemLoader.cpp
 *
 *  Created on: 2014年10月28日
 *      Author: root
 */

#include "AntiAddictionSystemLoader.h"
#include "CSVLoader.h"

AntiAddictionSystemLoader::AntiAddictionSystemLoader() {
	// TODO Auto-generated constructor stub

}

AntiAddictionSystemLoader::~AntiAddictionSystemLoader() {
	// TODO Auto-generated destructor stub
}

int AntiAddictionSystemLoader::InitAntiAddictionSystem(string& file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	m_cityCodeList.clear();

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int cityCode = loader.GetInt(i,0);
		m_cityCodeList.push_back(cityCode);
	}

	return 0;
}

bool AntiAddictionSystemLoader::IsCityCodeExit(int cityCode)
{
	if(cityCode < 110101 || cityCode > 659001)
		return false;
	for(uint i = 0;i < m_cityCodeList.size();++i)
	{
		if(m_cityCodeList[i] == cityCode)
			return true;
	}

	return false;
}
