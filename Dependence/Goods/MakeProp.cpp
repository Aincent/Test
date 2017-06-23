/*
 * MakeProp.cpp
 *
 *  Created on: 17/07/2015
 *      Author: root
 */

#include "MakeProp.h"
#include "EquipData.h"
#include "TimeData.h"
#include "PropData.h"

Smart_Ptr<PropBase> makePropByType(int64 itemID,eGoodsType type,int strengthLv)
{
	Smart_Ptr<PropBase> tempProp = NULL;
	const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(itemID);
	if(ginfo==NULL)
	{
		return tempProp;
	}

	switch(type)
	{
	case eWeapon:
	case eArmor:
	case eJewelry:
		{
			EquipProp* tempEquip = new EquipProp(itemID);
			tempEquip->SetUpgrade(strengthLv);
			tempProp = tempEquip;

			break;
		}
	case eTime:
	{
			int64 time=0;
			if(ginfo->valideSecs>0)
			{
					time = (int64)((CUtil::GetNowSecond()+(DWORD64)ginfo->valideSecs)*0.001);
			}
			tempProp = new TimeProp(ginfo->itemID,1,time);
		 break;
	}

	default:
		{
			tempProp = new PropBase(itemID,0);
		}
	}

	return tempProp;
}

