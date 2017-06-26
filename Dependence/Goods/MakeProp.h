/*
 * MakeProp.h
 *
 *  Created on: 17/07/2015
 *      Author: root
 */

#ifndef MAKEPROP_H_
#define MAKEPROP_H_

#include "define.h"
#include "Smart_Ptr.h"
#include "PropDefine.h"

using namespace CommBaseOut;

class PropBase;


Smart_Ptr<PropBase> makePropByType(int64 itemID,eGoodsType type,int strengthLv);

#endif /* MAKEPROP_H_ */
