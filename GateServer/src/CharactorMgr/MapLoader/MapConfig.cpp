/*
 * MapConfig.cpp
 *
 *  Created on: 2013��9��27��
 *      Author: helh
 */

#include "MapConfig.h"
#include "Log/Logger.h"
#include "CSVLoader.h"

CMapConfig * CMapConfig::m_instance = 0;

CMapConfig::CMapConfig()
{
}

CMapConfig::~CMapConfig()
{
}

int CMapConfig::LoadMapInfo(string &path)
{
	string startPath = path + "Data/PortalTypeData.csv";
	string mapPath = path + "Data/MapTypeData.csv";

	if(LoadAllMap(mapPath) || LoadMapStartPoint(startPath))
	{
		return -1;
	}

	return 0;
}

int CMapConfig::LoadAllMap(string &path)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(path.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		MapStaticInfo map;

		map.mapid = loader.GetInt(i, 0, 0);
		map.maptype = loader.GetInt(i, 1, 0);
		map.mapModelId = loader.GetInt(i, 2, 0);
		map.mapPath = path + "Data/Map/" + loader.GetString(i, 3, "") + "/";
		map.lv = loader.GetInt(i, 5, 0);
		map.reliveMap = loader.GetInt(i, 6, 0);
		map.reliveXPos = loader.GetInt(i,7, 0);
		map.reliveYPos = loader.GetInt(i, 8, 0);
		loader.GetString(i, 9, "");
		map.pktype = loader.GetInt(i, 10, 0);

		m_staticMap[map.mapid] = map;
	}

	return 0;
}

int CMapConfig::LoadMapStartPoint(string &path)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(path.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		MapStartPoint map;

		map.ID = loader.GetInt(i, 0, -1);
		map.mapID = loader.GetInt(i, 2, -1);
		map.xpos = loader.GetInt(i, 3, -1);
		map.ypos = loader.GetInt(i, 4, -1);
		map.tMapID = loader.GetInt(i, 5, -1);
		map.tXPos = loader.GetInt(i, 6, -1);
		map.tYPos = loader.GetInt(i, 7, -1);
		map.bCamp = loader.GetInt(i, 8, -1);

		m_allStartPoint[map.ID] = map;
	}

	return 0;
}
