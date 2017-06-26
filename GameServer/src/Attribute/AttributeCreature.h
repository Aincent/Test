/*
 * AttributeCreature.h
 *
 *  Created on: 22/05/2015
 *      Author: root
 */

#ifndef ATTRIBUTECREATURE_H_
#define ATTRIBUTECREATURE_H_

#include "AttributeBase.h"
#include "../Object/Object.h"

class AttributeCreature
{
public:
	AttributeCreature();

	virtual ~AttributeCreature();

	void Release();

public:
	void createAtt(eObjType objectType);

	virtual void init(void* data){}

public:
	template<class T>
	void setValue(ECharAttrType type,T value,eAttValueType attType = eAttValueBase)
	{
		m_AttList.setValue<T>(type,value,attType);
	}

	template<class T>
	void addValue(ECharAttrType type,T value,eAttValueType attType = eAttValueBase)
	{
		m_AttList.addValue<T>(type,value,attType);
	}

	template<class T>
	T getValue(ECharAttrType type)
	{
		return m_AttList.getValue<T>(type);
	}

	//设置百分比数据
	void setPercentValue(ECharAttrType type,float value)
	{
		m_AttList.setPercentValue(type,value);
	}

	//增加百分比
	void addPercentValue(ECharAttrType type,float value)
	{
		m_AttList.addPercentValue(type,value);
	}

	//获得百分比值
	float getPercentValue(ECharAttrType type)
	{
		return m_AttList.getPercentValue(type);
	}

	//获得固定值
	template<class T>
	T getBasedValue(ECharAttrType type,eAttValueType attType = eAttValueBase)
	{
		return m_AttList.getBasedValue<T>(type,attType);
	}

private:
	AttributeList m_AttList;
};


#endif /* ATTRIBUTECREATURE_H_ */
