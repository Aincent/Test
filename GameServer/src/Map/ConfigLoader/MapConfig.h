/*
 * MapConfig.h
 *
 *  Created on: 2013��9��26��
 *      Author: helh
 */

#ifndef MAPCONFIG_H_
#define MAPCONFIG_H_

#include "../../Common/include/Lua/LuaInterface.h"
#include <map>
#include <vector>
#include "FieldInfo.h"
#include "FieldMonster.h"
#include "FieldSpring.h"
#include "../SvrConfig.h"
//#include "../../Object/FileLoader/NPCLoader.h"

using namespace std;
using namespace CommBaseOut;

enum sMapPkType
{
	peace = 1, //和平模式
	neutral = 2, //中立模式
	all = 3, //全体模式
	goodEvil = 4, //善恶模式
	alliance = 5, //联盟模式(把联盟模式暂时用在帮派战中)
	country = 6, //国家模式
};

typedef struct sGameSvrMap
{
	int mapid;   //��ͼid
	int maptype;  //��ͼ����
	int id;    //相同地图的序号
}GameSvrMap;

typedef struct sMapStaticInfo
{
	int mapid;  // 地图id
	int maptype;  //地图类型
	int mapModelId;	//地图模型id
	std::string mapPath;   //地图路径
	int lv; //等级限制
	int reliveMap; //复活地图id
	int reliveXPos; //复活x坐标
	int reliveYPos; //复活y坐标
	int pktype; //pk类型

	sMapStaticInfo():mapid(-1),maptype(-1),mapModelId(-1),lv(0),reliveMap(-1),reliveXPos(-1),reliveYPos(-1),pktype(-1)
	{
		mapPath.clear();
	}
}MapStaticInfo;

//地图传送点信息
struct MapStartPoint
{
	int ID; //传送阵id
	int mapID; //地图id
	int xpos;
	int ypos;
	int tMapID; //目标传送地图id
	int tXPos;//目标传送x坐标
	int tYPos;//目标传送y坐标
	BYTE bCamp; //阵营限制

	MapStartPoint():ID(-1),mapID(-1),xpos(-1),ypos(-1),tMapID(-1),tXPos(-1),tYPos(-1),bCamp(-1)
	{

	}
};


class CMapConfig
{
public:
	~CMapConfig();

	static CMapConfig * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new CMapConfig();
		}

		return m_instance;
	}

	int LoadMap(string &path);

	int LoadGsMap(string &file);

	int LoadAllMap(string &path, string &file);

	int LoadMapStartPoint(string &path);

	int InitFieldInfo(string &path);
	int InitMonsterInfo();
	int InitSpringInfo();

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance =0;
		}
	}

	vector<GameSvrMap> &GetGameMap()
	{
		return m_gmMap;
	}

	MapStaticInfo *GetStaticInfo(int id)
	{
		map<int, MapStaticInfo>::iterator it = m_allMap.find(id);
		if(it != m_allMap.end())
		{
			return &it->second;
		}

		return 0;
	}

	MapStaticInfo *GetFistMapInfo()
	{
		map<int, MapStaticInfo>::iterator it = m_allMap.begin();
		for(;it != m_allMap.end();++it)
		{
			if(it->second.maptype == eOnlyMap)
			{
				return &it->second;
			}
		}

		return 0;
	}

	FieldInfo * GetFieldInfoByid(int mapid)
	{
		map<int, FieldInfo * >::iterator it = m_fieldInfo.find(mapid);
		if(it != m_fieldInfo.end())
		{
			return it->second;
		}

		return 0;
	}

	//获取格子大小
	int GetSectorSize(int mapid)
	{
		map<int, FieldInfo * >::iterator it = m_fieldInfo.find(mapid);
		if(it != m_fieldInfo.end())
		{
			return it->second->GetSectorSize();
		}

		return 0;
	}

	FieldMonster * GetFieldMonsterByid(int mapid)
	{
		map<int, FieldMonster * >::iterator it = m_fieldMonster.find(mapid);
		if(it != m_fieldMonster.end())
		{
			return it->second;
		}

		return 0;
	}

	FieldSpring * GetFieldSpringByid(int mapid)
	{
		map<int, FieldSpring * >::iterator it = m_fieldSpring.find(mapid);
		if(it != m_fieldSpring.end())
		{
			return it->second;
		}

		return 0;
	}

	bool IsValidPos(int mapid, int xpos, int ypos);

	//根据目的地图查找传送阵
	void GetTransferMapByTargetMapID(int mapid, MapStartPoint & info)
	{
		map<int, MapStartPoint >::iterator it = m_allStartPoint.begin();
		for(; it!=m_allStartPoint.end(); ++it)
		{
			if(it->second.tMapID == mapid)
			{
				info = it->second;

				return;
			}
		}
	}

	//根据起始地图和目标地图查找传送阵
	MapStartPoint * GetTransferMap(int bmapid, int emapid)
	{
		map<int, MapStartPoint >::iterator it = m_allStartPoint.begin();
		for(; it!=m_allStartPoint.end(); ++it)
		{
			if(it->second.tMapID == emapid && it->second.mapID == bmapid)
			{
				return &it->second;
			}
		}

		return 0;
	}

	//根据地图id，查找传送地图
	void GetTransferMapByMapID(int mapid, MapStartPoint & info)
	{
		map<int, MapStartPoint >::iterator it = m_allStartPoint.begin();
		for(; it!=m_allStartPoint.end(); ++it)
		{
			if(it->second.mapID == mapid)
			{
				info = it->second;

				return;
			}
		}
	}

	//根据传送点id，查找传送地图
	void GetTransferMapByStartPoint(int id, MapStartPoint &info)
	{
		map<int, MapStartPoint >::iterator it = m_allStartPoint.find(id);
		if(it != m_allStartPoint.end())
		{
			info = it->second;
		}
	}

	//判断是否有此地图
	bool IsHaveMap(int id)
	{
		map<int, MapStaticInfo>::iterator it = m_allMap.find(id);
		if(it != m_allMap.end())
		{
			return true;
		}

		return false;
	}

	bool IsCanValidPos(int mapid, int xpos, int ypos);

	int GetMapIndex(FieldInfo *field_ptr, int xpos, int ypos);

	const MapStaticInfo* getTaskDefault();

private:
	CMapConfig();

private:

	CLuaInterface m_pLua;
	static	CMapConfig * m_instance;

	vector<GameSvrMap> m_gmMap;   //��gameserver��������еĵ�ͼ
	map<int, MapStaticInfo> m_allMap;  // ���еĵ�ͼ��̬��Ϣ
	map<int, FieldInfo * > m_fieldInfo;  //���еĵ�����Ϣ  mapid - info

	map<int, FieldMonster * > m_fieldMonster;  // mapid - monsterinfo
	map<int, FieldSpring * > m_fieldSpring;
	map<int, MapStartPoint > m_allStartPoint; //传送点
};


#endif /* MAPCONFIG_H_ */
