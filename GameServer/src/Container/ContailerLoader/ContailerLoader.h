/*
 * ContailerLoader.h
 *
 *  Created on: 2014年12月26日
 *      Author: root
 */

#ifndef CONTAILERLOADER_H_
#define CONTAILERLOADER_H_

#include "Singleton.h"
#include "define.h"
#include <map>
#include "Lua/LuaInterface.h"
#include "../ContainerBase.h"

using namespace std;
using namespace CommBaseOut;

struct OpenCountainer
{
	int nTime;
	int nNum;
	uint nGolden;

	OpenCountainer():nTime(0),nNum(0),nGolden(0)
	{

	}
};

class ContailerLoader
{
public:
	ContailerLoader():m_nGolden(0)
	{
		m_vPackage.clear();
		m_vHouse.clear();
	}

	~ContailerLoader()
	{
		m_vPackage.clear();
		m_vHouse.clear();
	}

	static ContailerLoader* GetInstance();

	int Init(string &path);
	int InitLua(string & file);

	int InitPackage(string & file);

	int InitHouse(string & file);

//	int GetPackageNum(int nTime,int pos,bool flag);
//
//	int GetHouseNum(int nTime,int pos,bool flag);

	int GetGoldenByNum(int index ,int num , int container);

	void GetOpenPackage(int num , OpenCountainer &package)
	{
		map<int,OpenCountainer>::iterator it =  m_vPackage.find(num-35+1);
		if(it != m_vPackage.end())
		{
			package = it->second;
		}
	}

	void GetOpenHouse(int num , OpenCountainer &house)
	{
		map<int,OpenCountainer>::iterator it =  m_vHouse.find(num-28+1);
		if(it != m_vHouse.end())
		{
			house = it->second;
		}
	}
private:

	int m_nGolden; //开启格子金币
	map<int,OpenCountainer> m_vPackage;
	map<int,OpenCountainer> m_vHouse;
};

#endif /* CONTAILERLOADER_H_ */
