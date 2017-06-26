/*
 * NPC.cpp
 *
 *  Created on: 2013Äê10ÔÂ21ÈÕ
 *      Author: helh
 */

#include "NPC.h"


NPC::NPC(eObjType type):Object(type),m_npcID(-1)
{

}

NPC::~NPC()
{

}


NPCFuntion * NPCFuntion::m_instance = 0;

NPCFuntion::NPCFuntion()
{

}

NPCFuntion::~NPCFuntion()
{

}

bool NPCFuntion::IsLimit()
{
	return true;
}

