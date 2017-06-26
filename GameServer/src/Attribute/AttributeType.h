/*
 * AttributeType.h
 *
 *  Created on: 26/05/2015
 *      Author: root
 */

#ifndef ATTRIBUTETYPE_H_
#define ATTRIBUTETYPE_H_

#include "define.h"

using namespace CommBaseOut;

//属性数据类型
enum eATTClassType
{
	eAtt_Class_NULL      = 0,
	eAtt_Class_Char      = 1,
	eAtt_Class_UChar     = 2,
	eAtt_Class_Bool      = 3,
	eAtt_Class_Short     = 4,
	eAtt_Class_USort     = 5,
	eAtt_Class_Int       = 6,
	eAtt_Class_UInt      = 7,
	eAtt_Class_LONGLONG  = 8,
	eAtt_Class_ULONGLONG = 9,
	eAtt_Class_Float     = 10,
	eAtt_Class_Double    = 11,
};

//定时器类型
enum eAttTimeType
{
	eAttTimeNUll  = 0,
	eAttTimeMonth = 1,
	eAttTimeWeek  = 2,
	eAttTimeDay   = 3,
};

//属性值类型
enum eAttValueType
{
	eAttValueNull   = 0,
	eAttValueBase   = 1,	//基础值
	eAttValueAdd	= 2,	//增加的值
};

struct AttValue
{
	AttValue() : m_Value(0),m_AttType(eAttValueNull){}
	int64 		  m_Value;
	eAttValueType m_AttType;
};

enum eEachModuleAttType
{
	eEachModuleAttNull 		= 0,
	eEachModuleAttLevel 	= 1,		//等级
	eEachModuleAttTitle 	= 2,		//称号
	eEachModuleAttOfficial  = 3,		//官运
	eEachModuleAttEquip		= 4,		//装备
	eEachModuleAttExtra		= 5,		//额外属性
	eEachModuleAttFashion	= 6,		//时装
	eEachModuleAttHorse		= 7,		//坐骑
	eEachModuleAttMagic		= 8,		//神兵
	eEachModuleAttGirlWar	= 9,		//战姬
	eEachModuleAttQust		= 10,		//任务
	eEachModuleAttSkill		= 11,		//技能
	eEachModuleAttMeridian	= 12,		//经脉
	eEachModuleAttDanlu		= 13,		//丹炉
	eEachModuleAttMarryRing	= 14,		//结婚
	eEachModuleAttCamp		= 15,		//阵营
	eEachModuleAttClan		= 16,		//帮派
	eEachModuleAttHero		= 17,		//英雄塔
	eEachModuleAttSMagic    = 18,		//法器
	eEachModuleAttHeartMagic = 19,		//心法
};



#endif /* ATTRIBUTETYPE_H_ */
