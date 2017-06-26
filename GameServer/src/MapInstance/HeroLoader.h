/*
 * HeroLoader.h
 *
 *  Created on: 2016年5月18日
 *      Author: root
 */

#ifndef HEROLOADER_H_
#define HEROLOADER_H_

#include "define.h"
#include <map>
#include <vector>
#include <string>
#include "../Map/ConfigLoader/FieldInfo.h"
#include "Smart_Ptr.h"
#include "../Map/ConfigLoader/FieldMonster.h"
#include "Singleton.h"
#include "Lua/LuaInterface.h"
#include "CharDefine.h"

using namespace std;
using namespace CommBaseOut;

struct HeroLayerInfo
{
	HeroLayerInfo()
	{
		layer = 0;
		level = 0;
		monster = 0;
		hp = 0;
		attack = 0;
		defese = 0;
	  hit = 0;
		avoid = 0;
		crackDown = 0;
		avoidCrack = 0;
	}

	//层数	怪物ID	血量	攻击	防御	命中	闪避	暴击	抗暴	奖励道具	额外奖励
	int layer;
	int level;
	int monster;
	int hp;
	int attack;
	int defese;
	int hit;
	int avoid;
	int crackDown;
	int avoidCrack;

	std::vector<std::pair<int,int> > prizes;
	std::vector<std::pair<int,int> > exPrizes;
};

struct HeroConfig
{
	HeroConfig()
	{
		countTime = 0;
		posX = 0;
		posY = 0;
	}

	int countTime;
	int posX;
	int posY;
};

class HeroLoader : public Singleton<HeroLoader>
{
public:
	HeroLoader();
	~HeroLoader();

	bool InitData();

	int GetSize(){ return m_HeroLayerData.size(); }
	const HeroLayerInfo* GetHeroLayerInfo(int layer);
	const HeroConfig& GetHeroConfig(){ return m_config; }

private:
	int LoaderFile();
	int LoadLua();
private:
	CLuaInterface m_pLua;
	HeroConfig m_config;
	std::map<int,HeroLayerInfo> m_HeroLayerData;
};


#endif /* HEROLOADER_H_ */
