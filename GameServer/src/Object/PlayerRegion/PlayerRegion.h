/*
 * PlayerRegion.h
 *
 *  Created on: 30/11/2015
 *      Author: root
 */

#ifndef PLAYERREGION_H_
#define PLAYERREGION_H_

#include "define.h"
#include "Smart_Ptr.h"


using namespace CommBaseOut;
using namespace std;

class Player;
class Sector;
struct RegionData;
class RegionBase;

//------------区域键值---------------
struct RegionKey
{
	RegionKey() : m_SceneID(0),m_RegionID(0){}

	RegionKey(int64 sceneID,int regionID) : m_SceneID(sceneID),m_RegionID(regionID){}

	bool operator < (const RegionKey& other) const
	{
		return (m_SceneID < other.m_SceneID || (!(other.m_SceneID < m_SceneID) && (m_RegionID < other.m_RegionID)));
	}

	int64 m_SceneID;
	int   m_RegionID;
};

//----------------区域------------------
class PlayerRegion
{
public:
	PlayerRegion(Player* owner);
	~PlayerRegion();

	void ReInit();
	void Release();

public:
	void joinRegion(const RegionData* data,Sector* enterSector);

	void leaveRegion(const RegionData* data,Sector* leaveSector);

	Player* getOwner(){return m_Owner;}

private:
	map<RegionKey,vector<Smart_Ptr<RegionBase> > > m_DataList;
	Player* m_Owner;
};


#endif /* PLAYERREGION_H_ */
