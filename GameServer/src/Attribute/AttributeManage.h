/*
 * AttributeManage.h
 *
 *  Created on: 22/05/2015
 *      Author: root
 */

#ifndef ATTRIBUTEMANAGE_H_
#define ATTRIBUTEMANAGE_H_

#include "CharDefine.h"
#include "Timer/TimerInterface.h"
#include "Singleton.h"
#include "AttributeType.h"
#include "../Object/Object.h"

class Player;
class AttributeBase;




struct AttData
{
	AttData() : m_AttType(eCharName),m_ClassType(eAtt_Class_NULL),m_DefaultValue(0){}
	ECharAttrType 	  m_AttType;
	eATTClassType 	  m_ClassType;
	vector<eObjType>  m_HaveObject;
	int				  m_DefaultValue;
};

struct AttTimeData
{
	AttTimeData() : m_Week(0),m_Day(0),m_Hour(0),m_Minute(0),m_Second(0){}
	int m_Week;
	int m_Day;
	int m_Hour;
	int m_Minute;
	int m_Second;
};

/************属性清除部份**********/
class AttClearData
{
public:
	AttClearData();

	~AttClearData();

public:
	void init(vector<ECharAttrType>& attList,AttTimeData& data);

	void checkAtt(Player& player,DWORD64 soureTime,DWORD64 targetTime);


	void operator()(Smart_Ptr<Player> player);

	void time(void* p);

	static eAttTimeType getTimeType(const AttTimeData& data);

protected:
	bool isMoreThan(DWORD64 time,const AttTimeData& timeData);

	void formatTime(DWORD64 time,AttTimeData& timeData);

	void correctionTime(const AttTimeData& sourTimeData,AttTimeData& targetTimeData);

private:
	AttTimeData 			m_TimeData;
	vector<ECharAttrType>   m_AttList;
	TimerConnection			m_Time;
};

/*****************属性配置*****************/
class AttributeManage : public Singleton<AttributeManage>
{
public:
	AttributeManage();

	~AttributeManage();

public:
	//玩家上线时，检测玩家不在线时是否重置过的属性
	void checkAtt(Player& player);

	const map<ECharAttrType,AttData>& getAttData(){return m_DataList;}

	AttributeBase* makeBasePtr(eATTClassType classType,int defaultValue);

	bool init(string& str);

	const AttData* getAttData(ECharAttrType att);

	eAttValueType getAttValueType(eEachModuleAttType moduleType);

protected:
	bool initAttbuteFile(string& str);

	bool initLua(string& str);

	bool initModuleValueType(string& str);

	AttClearData* addClearData(vector<ECharAttrType>& attList,AttTimeData& data);

private:
	map<ECharAttrType,AttData> 			  m_DataList;
	vector<Smart_Ptr<AttClearData> > 	  m_ClearList;
	map<eEachModuleAttType,eAttValueType> m_AttModuleMapTypeList;	//各模块对应
};






#endif /* ATTRIBUTEMANAGE_H_ */
