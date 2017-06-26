/*
 * DropItemDefine.h
 *
 *  Created on: 2014年4月18日
 *      Author: helh
 */

#ifndef DROPITEMDEFINE_H_
#define DROPITEMDEFINE_H_

#include "../../Map/Sector.h"
#include "Smart_Ptr.h"

using namespace CommBaseOut;

struct DropItem
{
	DropItem() : data(0), flag(0), m_groupId(-1),m_teamId(0), sector(0),count(0)
	{
	}
	
	Smart_Ptr<PropBase> data;  // 道具
	unsigned char flag;  // 标识
	int m_groupId;//尸体盒id
	int64 m_teamId;	//物品所属队伍ID
	Sector* sector; //所在格子
	DWORD count;
};


#endif /* DROPITEMDEFINE_H_ */
