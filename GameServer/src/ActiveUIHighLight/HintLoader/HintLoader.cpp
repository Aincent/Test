/*
 * HintLoader.cpp
 *
 *  Created on: 2016年3月15日
 *      Author: root
 */

#include "HintLoader.h"
#include <CSVLoader.h>
#include <StrArgPkg.h>

using namespace CommBaseOut;

HintLoader::HintLoader() {
	// TODO Auto-generated constructor stub

}

HintLoader::~HintLoader() {
	// TODO Auto-generated destructor stub
}

int HintLoader::Init(string& path)
{
	string file = path + "Data/HintTypeData.csv";
	if (InitFile(file))
	{
		return -1;
	}

	return 0;
}
int HintLoader::InitFile(string& file)
{
	CSVLoader loader;
	if (!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	int count = 0;
	for (DWORD i = 0; i < loader.GetRowCount(); ++i)
	{
		UIHLType type;
		count = 0;

		type.first = static_cast<E_UIHL_BIGTYPE>(loader.GetInt(i, count++));
		type.second = static_cast<E_UIHL_SMALLTYPE>(loader.GetInt(i, count++));

		StrArgPkg limit("|", loader.GetString(i, count++));
		for (uint j = 0; j < limit.size(); ++j)
		{
			m_hintList[type].show.push_back(static_cast<E_HINT_SHOW>(atoi(getArg(limit, j).c_str())));
		}

		m_hintList[type].type = type;
	}

	return 0;
}


vector<E_HINT_SHOW> HintLoader::ShowWhat(UIHLType type)
{
	map<UIHLType, HintTypeInfo>::iterator it = m_hintList.find(type);
	if (m_hintList.end() != it)
	{
		return it->second.show;
	}

	return vector<E_HINT_SHOW>();
}
