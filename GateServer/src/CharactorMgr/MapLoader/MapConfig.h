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
#include "Smart_Ptr.h"

using namespace std;
using namespace CommBaseOut;

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

	sMapStaticInfo():mapid(-1),maptype(-1),lv(0),reliveMap(-1),reliveXPos(-1),reliveYPos(-1)
	{

	}
}MapStaticInfo;

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

	int LoadMapInfo(string &path);
	int LoadAllMap(string &path);
	int LoadMapStartPoint(string &path);

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance =0;
		}
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
			}
		}
	}

	//根据地图id查找世界地图信息
	void GetMapStaticByMapID(int mapid, MapStaticInfo &info)
	{
		map<int, MapStaticInfo >::iterator it = m_staticMap.find(mapid);
		if(it != m_staticMap.end())
		{
			info = it->second;
		}
	}
	MapStaticInfo* GetMapStaticInfoByMapID(int id)
	{
		map<int, MapStaticInfo >::iterator it = m_staticMap.find(id);
		if(it != m_staticMap.end())
		{
			return &it->second;
		}

		return 0;
	}

	//根据传送点id，获得传送信息
	void GetStartPointByStartID(int id, MapStartPoint &info)
	{
		map<int, MapStartPoint >::iterator it = m_allStartPoint.find(id);
		if(it != m_allStartPoint.end())
		{
			info = it->second;
		}
	}
	int GetMapIDByStartID(int id)
	{
		map<int, MapStartPoint >::iterator it = m_allStartPoint.find(id);
		if(it != m_allStartPoint.end())
		{
			return it->second.tMapID;
		}

		return -1;
	}

	MapStartPoint * GetStartPoint(int id)
	{
		map<int, MapStartPoint >::iterator it = m_allStartPoint.find(id);
		if(it != m_allStartPoint.end())
		{
			return &it->second;
		}

		return 0;
	}

	//判断传送点是否存在
	bool IsHaveStartPoint(int id)
	{
		map<int, MapStartPoint >::iterator it = m_allStartPoint.find(id);
		if(it == m_allStartPoint.end())
		{
			return false;
		}

		return true;
	}

private:
	CMapConfig();

private:

	static	CMapConfig * m_instance;

	map<int, MapStartPoint > m_allStartPoint; //传送点
	map<int, MapStaticInfo> m_staticMap;
};


#endif /* MAPCONFIG_H_ */
