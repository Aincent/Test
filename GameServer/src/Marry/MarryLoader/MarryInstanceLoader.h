/*
 * MarryInstanceLoader.h
 *
 *  Created on: 2015年11月3日
 *      Author: root
 */

#ifndef MARRYINSTANCELOADER_H_
#define MARRYINSTANCELOADER_H_

#include "Singleton.h"
#include "define.h"
#include "Lua/LuaInterface.h"

using namespace std;
using namespace CommBaseOut;

struct BothPoint
{
	int nXpos;
	int nYpos;

	BothPoint():nXpos(0),nYpos(0)
	{

	}
};

class MarryInstanceLoader:public Singleton<MarryInstanceLoader>
{
public:
	MarryInstanceLoader();
	virtual ~MarryInstanceLoader();

	int initFile(string& str);

	int GetCountTime(){return m_nCountTime;}

	int GetWaitTime(){return m_nWaitTime;}

	int GetMapId(){return m_nMapId;}

	void GetBothPoint(int64& xpos,int64& ypos,bool isFirst);

	void GetBothPoint(vector<BothPoint>& vec);
protected:
	int loadSingleFileData(string& str);

private:
	int m_nCountTime;
	int m_nWaitTime;
	int m_nMapId;
	vector<BothPoint> m_bothPoint;
};


#endif /* MARRYINSTANCELOADER_H_ */
