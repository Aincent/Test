/*
 * ActivityBoxLoader.h
 *
 *  Created on: 2015年7月29日
 *      Author: root
 */

#ifndef ACTIVITYBOXLOADER_H_
#define ACTIVITYBOXLOADER_H_

#include <vector>
#include "define.h"
#include "Singleton.h"
#include "Lua/LuaInterface.h"
using namespace std;
using namespace CommBaseOut;

struct ChestPoint
{
	int nXPos;
	int nYPos;

	ChestPoint():nXPos(0),nYPos(0)
	{

	}
};

class ActivityBoxLoader : public Singleton<ActivityBoxLoader>
{
public:
	ActivityBoxLoader();
	virtual ~ActivityBoxLoader();

	int Init(string &path);
	int InitChestPoint(string &file);
	int InitChestInf(string &file);
	int GetMonsterId(){return m_nMonsterId;}
	int GetTimes(){return m_nTimes;}
	int GetNum(){return m_nNum;}
	int GetPertime(){return m_nPertime;}
	int GetMapId(){return m_nMapid;}
	void GetRandomPos(int64& nXPos,int64& nYPos,int mapId);
	void GetRandomPos(int mapId,vector<ChestPoint>& vec);
private:
	int m_nMonsterId;
	int m_nTimes;
	int m_nNum;
	int m_nPertime;
	int m_nMapid;
	map<int,vector<ChestPoint> > m_ChestPoint;
};

#endif /* ACTIVITYBOXLOADER_H_ */
