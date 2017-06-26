/*
 * AttributeBase.h
 *
 *  Created on: 22/05/2015
 *      Author: root
 */

#ifndef ATTRIBUTEBASE_H_
#define ATTRIBUTEBASE_H_

#include "define.h"
#include "AttributeType.h"
#include "CharDefine.h"
#include "Smart_Ptr.h"

using namespace CommBaseOut;

#define attributeFun(attPtr,fun) \
switch (attPtr->getType()) \
{ \
case eAtt_Class_Char: \
	{ \
		return static_cast<AttributeTemplate<char>* >(attPtr)->fun; \
	} \
break; \
case eAtt_Class_UChar: \
	{ \
		return static_cast<AttributeTemplate<BYTE>* >(attPtr)->fun; \
	} \
break; \
	case eAtt_Class_Bool: \
	{ \
		return static_cast<AttributeTemplate<bool>* >(attPtr)->fun; \
	} \
break; \
	case eAtt_Class_Short: \
	{ \
		return static_cast<AttributeTemplate<short>* >(attPtr)->fun; \
	} \
break; \
	case eAtt_Class_USort: \
	{ \
		return static_cast<AttributeTemplate<WORD>* >(attPtr)->fun; \
	} \
break; \
	case eAtt_Class_Int: \
	{ \
		return static_cast<AttributeTemplate<int>* >(attPtr)->fun; \
	} \
break; \
	case eAtt_Class_UInt: \
	{ \
		return static_cast<AttributeTemplate<unsigned int>* >(attPtr)->fun; \
	} \
break; \
	case eAtt_Class_LONGLONG: \
	{ \
		return static_cast<AttributeTemplate<int64>* >(attPtr)->fun; \
	} \
break; \
	case eAtt_Class_ULONGLONG: \
	{ \
		return static_cast<AttributeTemplate<DWORD64>* >(attPtr)->fun; \
	} \
break; \
	case eAtt_Class_Float: \
	{ \
		return static_cast<AttributeTemplate<float>* >(attPtr)->fun; \
	} \
break; \
	case eAtt_Class_Double: \
	{ \
		return static_cast<AttributeTemplate<double>* >(attPtr)->fun; \
	} \
break; \
default:\
	{\
	break;\
	}\
}

class AttributeBase
{
public:
	AttributeBase(){}

	virtual ~AttributeBase(){}

public:
	virtual eATTClassType getType() = 0;
	virtual void setDefault()=0;
};

template<class T>
class AttributeTemplate : public AttributeBase
{
public:
	AttributeTemplate(T value,eATTClassType type) : AttributeBase(),m_BaseValue(value),m_AddValue(0),
	m_PercentRate(.0f),m_FinalValue(0),m_Type(type),m_DefaulValue(value)
	{
		calculateFinalValue();
	}

	virtual ~AttributeTemplate(){}

	void setDefault()
	{
		m_BaseValue = m_DefaulValue;
		m_AddValue = 0;
		m_PercentRate = 0.f;
		m_FinalValue = m_DefaulValue;
	}

public:
	//设置固定值
	template<class type>
	void setValue(type value,eAttValueType attType)
	{
		if(attType == eAttValueBase)
		{
			m_BaseValue = (T)value;
		}
		else
		{
			m_AddValue  = (T)value;
		}

		calculateFinalValue();
	}

	//增加固定值
	template<class type>
	void addValue(type value,eAttValueType attType)
	{
		if(attType == eAttValueBase)
		{
			m_BaseValue = (T)(m_BaseValue + value);
		}
		else
		{
			m_AddValue  = (T)(m_BaseValue + value);
		}

		calculateFinalValue();
	}

	//获得固定值
	template<class type>
	type getBasedValue(eAttValueType attType)
	{
		if(attType == eAttValueBase)
		{
			return (type)m_BaseValue;
		}
		else
		{
			return (type)m_AddValue;
		}
	}

	//设置百分比数据
	void setPercentValue(float value)
	{
		m_PercentRate = value;

		calculateFinalValue();
	}

	//增加百分比
	void addPercentValue(float value)
	{
		m_PercentRate += value;

		calculateFinalValue();
	}

	//获得百分比值
	float getPercentValue()
	{
		return m_PercentRate;
	}

	//获得最终值
	template<class type>
	type getValue()
	{
		return (type)m_FinalValue;
	}

	virtual eATTClassType getType(){return m_Type;}

protected:
	//计算最终值
	void calculateFinalValue()
	{
		m_FinalValue = m_BaseValue + (T)(m_BaseValue * m_PercentRate) + m_AddValue;
	}

private:
	T 			  m_BaseValue;		//基础值
	T			  m_AddValue;		//增加的值
	float		  m_PercentRate;	//百分比
	T			  m_FinalValue;		//计算后的值
	eATTClassType m_Type;			//T的类型
	T				m_DefaulValue;	//默认值
};

class AttributeList
{
public:
	AttributeList(){}

	~AttributeList(){}

	void Release()
	{
		for(int i=0; i<eCharAttrMax; ++i)
		{
			if(!m_AttList[i])
					continue;

			m_AttList[i]->setDefault();
		}
	}

	//设置固定值
	template<class T>
	void setValue(ECharAttrType att,T value,eAttValueType attType)
	{
		if(att >= eCharAttrMax || att < eCharName)
			return ;

		if(!m_AttList[att])
			return ;

		attributeFun(m_AttList[att].Get(),setValue(value,attType));
	}

	//添加固定值
	template<class T>
	void addValue(ECharAttrType att,T value,eAttValueType attType)
	{
		if(att >= eCharAttrMax || att < eCharName)
			return ;

		if(!m_AttList[att])
			return ;

		attributeFun(m_AttList[att].Get(),addValue(value,attType));
	}

	//返回最终值
	template<class T>
	T getValue(ECharAttrType att)
	{
		if(att >= eCharAttrMax || att < eCharName)
			return T();

		if(!m_AttList[att])
			return T();

		attributeFun(m_AttList[att].Get(),getValue<T>());

		return T();
	}

	//设置百分比数据
	void setPercentValue(ECharAttrType att,float value)
	{
		if(att >= eCharAttrMax || att < eCharName)
			return ;

		if(!m_AttList[att])
			return ;

		attributeFun(m_AttList[att].Get(),setPercentValue(value));
	}

	//增加百分比
	void addPercentValue(ECharAttrType att,float value)
	{
		if(att >= eCharAttrMax || att < eCharName)
			return ;

		if(!m_AttList[att])
			return ;

		attributeFun(m_AttList[att].Get(),addPercentValue(value));
	}

	//获得百分比值
	float getPercentValue(ECharAttrType att)
	{
		if(att >= eCharAttrMax || att < eCharName)
			return .0f;

		if(!m_AttList[att])
			return .0f;

		attributeFun(m_AttList[att].Get(),getPercentValue());

		return .0f;
	}

	//获得固定值
	template<class T>
	T getBasedValue(ECharAttrType att,eAttValueType attType = eAttValueBase)
	{
		if(att >= eCharAttrMax || att < eCharName)
			return T();

		if(!m_AttList[att])
			return T();

		attributeFun(m_AttList[att].Get(),getBasedValue<T>(attType));

		return T();
	}

	bool setBasePtr(AttributeBase* attBasePtr,ECharAttrType type)
	{
		if(type >= eCharAttrMax || type < eCharName)
			return false;

		if(attBasePtr == NULL)
			return false;

		m_AttList[type] = attBasePtr;

		return true;
	}

private:
	Smart_Ptr<AttributeBase> m_AttList[eCharAttrMax];
};

#endif /* ATTRIBUTEBASE_H_ */
