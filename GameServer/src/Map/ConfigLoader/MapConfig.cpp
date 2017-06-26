/*
 * MapConfig.cpp
 *
 *  Created on: 2013��9��27��
 *      Author: helh
 */

#include "MapConfig.h"
#include "Log/Logger.h"
#include "CSVLoader.h"
#include "Memory/MemAllocator.h"

CMapConfig * CMapConfig::m_instance = 0;

CMapConfig::CMapConfig()
{
	m_pLua.OpenLua();
}

CMapConfig::~CMapConfig()
{
	m_pLua.CloseLua();

	map<int, FieldInfo * >::iterator it = m_fieldInfo.begin();
	for(; it!=m_fieldInfo.end(); ++it)
	{
		delete it->second;
		it->second = 0;
	}

	map<int, FieldMonster * >::iterator itMon = m_fieldMonster.begin();
	for(; itMon!=m_fieldMonster.end(); ++itMon)
	{
		delete itMon->second;
		itMon->second = 0;
	}

	map<int, FieldSpring * >::iterator itSpring = m_fieldSpring.begin();
	for(; itSpring!=m_fieldSpring.end(); ++itSpring)
	{
		delete itSpring->second;
		itSpring->second = 0;
	}
}

int CMapConfig::LoadMap(string &path)
{
	string mapPath = path + "Config/MapConfig.lua";
	string worldPath = path + "Data/MapTypeData.csv";
	string startPoint = path + "Data/PortalTypeData.csv";

	if(LoadGsMap(mapPath) || LoadAllMap(path, worldPath) || LoadMapStartPoint(startPoint))
	{
		LOG_ERROR(FILEINFO, "��ʼ����ͼ�ļ�ʧ��");

		return -1;
	}

	if(InitFieldInfo(path))
	{
		LOG_ERROR(FILEINFO, "field into load error");

		return -1;
	}

	if(InitMonsterInfo())
	{
		LOG_ERROR(FILEINFO, "monster in map load error");

		return -1;
	}

	if(InitSpringInfo())
	{
		LOG_ERROR(FILEINFO, "spring in map load error");

		return -1;
	}

	return 0;
}

int CMapConfig::LoadGsMap(string &file)
{
	if(m_pLua.LoadFile(file) != 0 || m_pLua.Call() != 0)
	{
		return -1;
	}

	m_pLua.SetLuaFunction("GetAllMap");

	if(m_pLua.Call(0, 1))
	{
		return -1;
	}

	int tableCount = m_pLua.GetTableFieldCount();
	for(int i=1; i<=tableCount; ++i)
	{
		if (m_pLua.GetFieldTable(i) != 0)
		{
			return -1;
		}

		GameSvrMap map;

		map.mapid = m_pLua.GetFieldInt("MapID");
		map.maptype = m_pLua.GetFieldInt("MapType");
		map.id = m_pLua.GetFieldInt("id");

		m_gmMap.push_back(map);
		m_pLua.CloseTable();
	}

	m_pLua.CloseTable();

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

int CMapConfig::LoadAllMap(string &path, string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
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

		m_allMap[map.mapid] = map;
	}

	return 0;
}

int CMapConfig::InitMonsterInfo()
{
	map<int, MapStaticInfo>::iterator it = m_allMap.begin();
	for(; it!=m_allMap.end(); ++it)
	{
		FieldMonster * Info = new FieldMonster(it->second.mapPath, it->second.mapid);
		if(Info->Init())
		{
			delete Info;
			Info = 0;

			return -1;
		}

		m_fieldMonster[it->second.mapid] = Info;
	}

	return 0;
}

int CMapConfig::InitSpringInfo()
{
	map<int, MapStaticInfo>::iterator it = m_allMap.begin();
	for(; it!=m_allMap.end(); ++it)
	{
		FieldSpring * info = new FieldSpring(it->second.mapPath, it->second.mapid);
		if(info->Init())
		{
			delete info;
			info = 0;

			continue;
		}

		m_fieldSpring[it->second.mapid] = info;
	}

	return 0;
}

int CMapConfig::InitFieldInfo(string &path)
{
	map<int, MapStaticInfo>::iterator it = m_allMap.begin();
	for(; it!=m_allMap.end(); ++it)
	{
		FieldInfo * info = new FieldInfo(it->second.mapPath, it->second.mapid);

		int res = info->Init(path,it->second.mapPath);
		if(res)
		{
			delete info;
			info = 0;

			return res;
		}

		m_fieldInfo[it->second.mapid] = info;
	}

	return 0;
}

bool CMapConfig::IsValidPos(int mapid, int xpos, int ypos)
{
	map<int, FieldInfo * >::iterator it = m_fieldInfo.find(mapid);
	if(it == m_fieldInfo.end())
	{
		return false;
	}

	//技能x，y都等于0，也表示错误的
	if(xpos <= 0 || ypos <= 0 || xpos >= it->second->GetXMaxNum() || ypos >= it->second->GetYMaxNum())
	{
		return false;
	}

	return true;
}

bool CMapConfig::IsCanValidPos(int mapid, int xpos, int ypos)
{
	map<int, FieldInfo * >::iterator it = m_fieldInfo.find(mapid);
	if(it == m_fieldInfo.end())
		return false;

	int mapIndex = GetMapIndex(it->second, xpos, ypos);
	if(mapIndex < 0 || mapIndex >= it->second->GetAllSectorSize())
		return false;

	if(!it->second->IsValidSector(mapIndex))
		return false;

	//技能x，y都等于0，也表示错误的
	if(xpos <= 0 || ypos <= 0 || xpos >= it->second->GetXMaxNum() || ypos >= it->second->GetYMaxNum())
		return false;

	return true;
}

int CMapConfig::GetMapIndex(FieldInfo *field_ptr, int xpos, int ypos)
{
	return (xpos + ypos * field_ptr->GetXMaxNum());
}

const MapStaticInfo* CMapConfig::getTaskDefault()
{
	map<int, MapStaticInfo>::iterator it = m_allMap.begin();
	for(;it != m_allMap.end();++it)
	{
		if(it->second.maptype == 1)	
		{
			return &it->second;
		}
	}

	return NULL;
}
