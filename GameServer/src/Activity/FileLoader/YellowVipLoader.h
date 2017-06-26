/*
 * YellowVipLoader.h
 *
 *  Created on: 2015年6月10日
 *      Author: root
 */

#ifndef YELLOWVIPLOADER_H_
#define YELLOWVIPLOADER_H_
#include <vector>
#include "define.h"
#include "Singleton.h"
#include "Lua/LuaInterface.h"
using namespace std;
using namespace CommBaseOut;

struct DiamondTypes
{
	int nId;
	int nType;
	int nLv;
	int nProfess;
	vector<DWORD> m_totalItem;
	vector<DWORD> m_totalNum;
	vector<DWORD> m_totalLv;

	DiamondTypes():nId(0),nType(0),nLv(0),nProfess(0)
	{

	}
};


class YellowVipLoader : public Singleton<YellowVipLoader>
{
public:
	YellowVipLoader();
	~YellowVipLoader();

	int Init(string &path);
	int InitDiamondTypes(string &file);

	void GetDiamondTypes(vector<DiamondTypes>& vecTypes);
private:
	vector<DiamondTypes> vDiamon;
};

#endif /* YELLOWVIPLOADER_H_ */
