/*
 * RegionFactory.cpp
 *
 *  Created on: 01/12/2015
 *      Author: root
 */


#include "RegionFactory.h"
#include "RegionFun.h"
#include "../../Map/ConfigLoader/FieldInfo.h"

RegionBase* RegionFactory::getRegionFun(PlayerRegion* father,const RegionData* data,BYTE index)
{
	RegionBase* tempResult = NULL;
	switch(data->m_RegionFun[index].m_FunType)
	{
	case eRegionAlterPkType:
		{
			tempResult = new RegionAlterPkMode(data,father,index);
			break;
		}
	case eRegionAlterAtt:
		{
			tempResult = new RegionAlterAtt(data,father,index);
			break;
		}
	case eRegionIntervalExp:
	case eRegionIntervalForce:
		{
			tempResult = new RegionIntervalExp(data,father,index);
			break;
		}
	default:
		{
			break;
		}
	}

	return tempResult;
}

