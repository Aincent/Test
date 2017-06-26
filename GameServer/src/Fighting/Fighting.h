/*
 * Fighting.h
 *
 *  Created on: 10/07/2015
 *      Author: root
 */

#ifndef FIGHTING_H_
#define FIGHTING_H_

#include "define.h"
#include "CharDefine.h"

using namespace std;
using namespace CommBaseOut;

class Player;

enum FightingType
{
	eFightingNull  		 = 0,
	eFightingLevel 		 = 1,
	eFightingCamp  		 = 2,
	eFightingClan  		 = 3,
	eFightingMeridian    = 4,
	eFightingTitle 		 = 5,
	eFightingJobLucky    = 6,
	eFightingEquit 		 = 7,
	eFightingExtra 		 = 8,
	eFightingFashion     = 9,
	eFightingHorse 		 = 10,
	eFightingMagicWeapon = 11,
	eFightingGirlWar 	 = 12,
	eFightingQuest 	 	 = 13,
	eFightingSkill 		 = 14,
	eFightingDanLu 		 = 15,
	eFightMoneyInstance  = 16,
	eFightingMarry		 = 17,
	eFightHero			 = 18,
	eFightSMagic		 = 19,
	eFightHeartMagic	 = 20,
	eFightingMax 		 = 21,
};


class Fighting
{
public:
	Fighting(Player* player);

	~Fighting();

	void ReInit();
	void Release();

public:
	int setFighting(FightingType type,int value);

	int addFighting(FightingType type,int value);

	int getFighting(FightingType type);

	int CalculateFighting(const map<ECharAttrType,int64>& attList);

private:
	map<FightingType,int> m_AllFighting;	//所有模块的战斗力
	Player*				  m_Owner;
};



#endif /* FIGHTING_H_ */
