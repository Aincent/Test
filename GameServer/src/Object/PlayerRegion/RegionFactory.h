/*
 * RegionFactory.h
 *
 *  Created on: 01/12/2015
 *      Author: root
 */

#ifndef REGIONFACTORY_H_
#define REGIONFACTORY_H_

#include "Singleton.h"
#include "define.h"

using namespace CommBaseOut;

class PlayerRegion;
class RegionData;
class RegionBase;

class RegionFactory : public Singleton<RegionFactory>
{
public:
	RegionFactory(){}

	~RegionFactory(){}

public:
	RegionBase* getRegionFun(PlayerRegion* father,const RegionData* data,BYTE index);
};


#endif /* REGIONFACTORY_H_ */
