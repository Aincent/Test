/*
 * NPC.cpp
 *
 *  Created on: 2013��10��21��
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

