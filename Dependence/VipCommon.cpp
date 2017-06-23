/*
 * VipCommon.cpp
 *
 *  Created on: 08/04/2015
 *      Author: root
 */
#include "VipCommon.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"


int VipCommonLoader::InitVip(string& file,map<DWORD,sVipEntry>& data)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		int id = loader.GetInt(i, count++);
		if(id == 1)
		{
			for(unsigned int j = 0; j < 10; j++)
			{
				sVipEntry info;
				info.m_nAddUpGold = loader.GetInt(i, count++);
				info.m_nVipID = count -1;
				data[info.m_nVipID] = info;
			}
		}
		else if(id == 2)
		{
			for(unsigned int j = 0; j < 10; j++)
			{
				string funStrItem = loader.GetString(i, count++, "");
				StrArgPkg funPkgItem("|", funStrItem);

				map<DWORD,sVipEntry>::iterator itr = data.find(count-1);
				if(itr != data.end())
				{
					for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
					{
						StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
						for(unsigned int i=0; i<limitPkg.size(); ++i)
						{
							itr->second.m_nAward.push_back(atoi(getArg(limitPkg, i++).c_str()));
							itr->second.m_nAwardNum.push_back(atoi(getArg(limitPkg, i).c_str()));
						}
					}
				}
			}
		}
		else
		{
			id -= 2;
			for(unsigned int j = 0; j < 10; j++)
			{
				string str = loader.GetString(i, count++, "");
				double value = 0;
				map<DWORD,sVipEntry>::iterator itr = data.find(count-1);
				if(itr != data.end())
				{
					if (str.npos != str.find("%"))
						value = double(atof(str.c_str()) / (double)100);
					else
						value = atof(str.c_str());

					itr->second.m_nFunctions[id] = value;
				}
			}
		}
	}

//	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
//	{
//		sVipEntry info;
//		int count = 0;
//
//		info.m_nVipID = loader.GetInt(i, count++);
//		info.m_nAddUpGold = loader.GetInt(i, count++);
//
//		string funStrItem = loader.GetString(i, count++, "");
//		StrArgPkg funPkgItem("|", funStrItem);
//
//		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
//		{
//			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
//			for(unsigned int i=0; i<limitPkg.size(); ++i)
//			{
//				info.m_nAward.push_back(atoi(getArg(limitPkg, i++).c_str()));
//				info.m_nAwardNum.push_back(atoi(getArg(limitPkg, i).c_str()));
//			}
//		}
//
//		funStrItem = loader.GetString(i, count++, "");
//		funPkgItem.setArgs(funStrItem);
//
//		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
//		{
//			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
//
//			int key = 0;
//			double value = 0;
//			if (limitPkg.size() == 1)
//			{
//				key = atoi(getArg(funPkgItem, itemSize).c_str());
//				value = 0;
//			}
//			else
//			{
//				key = atoi(getArg(limitPkg, 0).c_str());
//				string str = getArg(limitPkg, 1);
//				if (str.
//		StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
//		for(unsigned int i=0; i<limitPkg.size(); ++i)
//		{
//			info.m_nAward.push_back(atoi(getArg(limitPkg, i++).c_str()));
//			info.m_nAwardNum.push_back(atoi(getArg(limitPkg, i).c_str()));
//		}
//	npos != str.find("%"))
//					value = double(atof(getArg(limitPkg, 1).c_str()) / (double)100);
//				else
//					value = atof(getArg(limitPkg, 1).c_str());
//			}
//
//			info.m_nFunctions[key] = value;
//		}
//
//		data[info.m_nVipID] = info;
//	}

	return 0;
}

