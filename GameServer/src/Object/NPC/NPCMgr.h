/*
 * NPCMgr.h
 *
 *  Created on: 2014年8月16日
 *      Author: root
 */

#ifndef NPCMGR_H_
#define NPCMGR_H_

#include "Singleton.h"
//#include "../../EventSet/EventDefine.h"
#include "../Player/Player.h"

using namespace CommBaseOut;

class Player;

class NPCMgr: public Singleton<NPCMgr>
{
public:
	NPCMgr();
	~NPCMgr();

	int JugeIsCanGetNPCHouseInfo(Smart_Ptr<Player> &player);

};

#endif /* NPCMGR_H_ */
