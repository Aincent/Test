/*
 * AttributeManage.cpp
 *
 *  Created on: 22/05/2015
 *      Author: root
 */

#include "AttributeManage.h"
#include "../Object/Player/Player.h"
#include "AttributePlayer.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Object/Player/Player.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "Lua/LuaInterface.h"

/************属性清除部份**********/
AttClearData::AttClearData()
{

}

AttClearData::~AttClearData()
{
	if(m_Time.IsValid())
		m_Time.SetNull();
}

void AttClearData::init(vector<ECharAttrType>& attList,AttTimeData& data)
{
	m_TimeData = data;
	m_AttList  = attList;

	if(m_Time.IsValid())
		m_Time.SetNull();

	switch(getTimeType(m_TimeData))
	{
	case eAttTimeMonth:
		{
			m_Time = TimerInterface::GetInstance()->RegMonthPointRepeatedTime(
					&AttClearData::time,
					this,
					NULL,
					m_TimeData.m_Day,
					m_TimeData.m_Hour,
					m_TimeData.m_Minute,
					m_TimeData.m_Second);
			break;
		}
	case eAttTimeWeek:
		{
			m_Time = TimerInterface::GetInstance()->RegWeekPointRepeatedTime(
					&AttClearData::time,
					this,
					NULL,
					m_TimeData.m_Week,
					m_TimeData.m_Hour,
					m_TimeData.m_Minute,
					m_TimeData.m_Second);
			break;
		}
	case eAttTimeDay:
		{
			m_Time = TimerInterface::GetInstance()->RegDayPointRepeatedTime(
					&AttClearData::time,
					this,
					NULL,
					m_TimeData.m_Hour,
					m_TimeData.m_Minute,
					m_TimeData.m_Second);
			break;
		}
	default:
		{
			break;
		}
	}

	return ;
}


void AttClearData::checkAtt(Player& player,DWORD64 soureTime,DWORD64 targetTime)
{
	//当前时间大过重置时间
	if(!isMoreThan(soureTime,m_TimeData))
		return ;

	//上次设置时间小于重置时间，表明玩家在线期间没有重置过
	if(isMoreThan(targetTime,m_TimeData))
		return ;

	for(size_t i = 0;i < m_AttList.size();++i)
	{
		const AttData* tempAttData = AttributeManage::GetInstance()->getAttData(m_AttList[i]);
		if(tempAttData == NULL)
			continue;

		player.getAttManage()->setValue(m_AttList[i],tempAttData->m_DefaultValue);
	}

	return ;
}

void AttClearData::operator()(Smart_Ptr<Player> player)
{
	for(size_t i = 0;i < m_AttList.size();++i)
	{
		const AttData* tempAttData = AttributeManage::GetInstance()->getAttData(m_AttList[i]);
		if(tempAttData == NULL)
			continue;

		player->getAttManage()->setValue(m_AttList[i],tempAttData->m_DefaultValue);
	}

	static_cast<AttributePlayer*>(player->getAttManage().Get())->setTime(CUtil::GetNowSecond() / 1000);
	player->SetDataFlag(eBaseInfo);

	return ;
}

void AttClearData::time(void* p)
{
	PlayerMessage::GetInstance()->foreach<AttClearData>(*this);

	return ;
}

eAttTimeType AttClearData::getTimeType(const AttTimeData& data)
{
	if(data.m_Day > 0)
		return eAttTimeMonth;

	if(data.m_Week > 0)
		return eAttTimeWeek;

	if(data.m_Hour > 0)
		return eAttTimeDay;

	return eAttTimeNUll;
}


bool AttClearData::isMoreThan(DWORD64 time,const AttTimeData& timeData)
{
	if(time == 0)
		return false;

	AttTimeData tempPamaTime;

	formatTime(time,tempPamaTime);

	correctionTime(timeData,tempPamaTime);

	if(tempPamaTime.m_Week > timeData.m_Week)
		return true;
	else if(tempPamaTime.m_Week == timeData.m_Week || timeData.m_Week == 0)
	{
		if(tempPamaTime.m_Day > timeData.m_Day)
			return true;
		else if(tempPamaTime.m_Day == timeData.m_Day || timeData.m_Day == 0)
		{
			if(tempPamaTime.m_Hour > timeData.m_Hour)
				return true;
			else if(tempPamaTime.m_Hour ==  timeData.m_Hour || timeData.m_Hour == 0)
			{
				if(tempPamaTime.m_Minute > timeData.m_Minute)
					return true;
				else if(tempPamaTime.m_Minute == timeData.m_Minute || timeData.m_Minute == 0)
				{
					if(tempPamaTime.m_Second > timeData.m_Second)
						return true;
				}
			}
		}
	}

	return false;
}

void AttClearData::formatTime(DWORD64 time,AttTimeData& timeData)
{
	time_t tempTime   = time;
	struct tm tempTm;
	localtime_r(&tempTime,&tempTm);

	timeData.m_Second = tempTm.tm_sec;
	timeData.m_Minute = tempTm.tm_min;
	timeData.m_Hour   = tempTm.tm_hour;
	timeData.m_Day    = tempTm.tm_mday;
	timeData.m_Week   = tempTm.tm_wday;

	return ;
}

void AttClearData::correctionTime(const AttTimeData& sourTimeData,AttTimeData& targetTimeData)
{
	switch(getTimeType(sourTimeData))
	{
	case eAttTimeMonth:
		{
			targetTimeData.m_Week = 0;
			break;
		}
	case eAttTimeWeek:
		{
			targetTimeData.m_Day = 0;
			break;
		}
	case eAttTimeDay:
		{
			targetTimeData.m_Week = 0;
			targetTimeData.m_Day  = 0;
			break;
		}
	default:
		{
			break;
		}
	}

	return ;
}



/*****************属性配置*****************/
AttributeManage::AttributeManage()
{

}

AttributeManage::~AttributeManage()
{

}

//玩家上线时，检测玩家不在线时是否重置过的属性
void AttributeManage::checkAtt(Player& player)
{
	DWORD64 tempCurrentTime = CUtil::GetNowSecond() / 1000;

	for(size_t i = 0;i < m_ClearList.size();++i)
	{
		m_ClearList[i]->checkAtt(player,tempCurrentTime,static_cast<AttributePlayer*>(player.getAttManage().Get())->getTime());
	}

	static_cast<AttributePlayer* >(player.getAttManage().Get())->setTime(tempCurrentTime);

	player.SetDataFlag(eBaseInfo);

	return ;
}

AttributeBase* AttributeManage::makeBasePtr(eATTClassType classType,int defaultValue)
{
	AttributeBase* tempPtr = NULL;

	switch(classType)
	{
	case eAtt_Class_Char:
		{
			tempPtr = new AttributeTemplate<char>(defaultValue,classType);
			break;
		}
	case eAtt_Class_UChar:
		{
			tempPtr = new AttributeTemplate<BYTE>(defaultValue,classType);
			break;
		}
	case eAtt_Class_Bool:
		{
			tempPtr = new AttributeTemplate<bool>(defaultValue,classType);
			break;
		}
	case eAtt_Class_Short:
		{
			tempPtr = new AttributeTemplate<short>(defaultValue,classType);
			break;
		}
	case eAtt_Class_USort:
		{
			tempPtr = new AttributeTemplate<WORD>(defaultValue,classType);
			break;
		}
	case eAtt_Class_Int:
		{
			tempPtr = new AttributeTemplate<int>(defaultValue,classType);
			break;
		}
	case eAtt_Class_UInt:
		{
			tempPtr = new AttributeTemplate<DWORD>(defaultValue,classType);
			break;
		}
	case eAtt_Class_LONGLONG:
		{
			tempPtr = new AttributeTemplate<int64>(defaultValue,classType);
			break;
		}
	case eAtt_Class_ULONGLONG:
		{
			tempPtr = new AttributeTemplate<DWORD64>(defaultValue,classType);
			break;
		}
	case eAtt_Class_Float:
		{
			tempPtr = new AttributeTemplate<float>(defaultValue,classType);
			break;
		}
	case eAtt_Class_Double:
		{
			tempPtr = new AttributeTemplate<double>(defaultValue,classType);
			break;
		}
	default:
		{
			break;
		}
	}

	return tempPtr;
}

bool AttributeManage::init(string& str)
{
	if(!initAttbuteFile(str) || !initLua(str) || !initModuleValueType(str))
		return false;

	return true;
}

const AttData* AttributeManage::getAttData(ECharAttrType att)
{
	map<ECharAttrType,AttData>::iterator tempIt = m_DataList.find(att);
	if(tempIt == m_DataList.end())
		return NULL;

	return &tempIt->second;
}

eAttValueType AttributeManage::getAttValueType(eEachModuleAttType moduleType)
{
	map<eEachModuleAttType,eAttValueType>::iterator tempIt = m_AttModuleMapTypeList.find(moduleType);
	if(tempIt == m_AttModuleMapTypeList.end())
	{
		return eAttValueBase;
	}

	return tempIt->second;
}

bool AttributeManage::initAttbuteFile(string& str)
{
	CSVLoader loader;
	string tempDataStr = str + "Data/AttributeData.csv";

	if(!loader.OpenFromFile(tempDataStr.c_str()))
		return false;

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;

		AttData tempData;

		tempData.m_AttType   = static_cast<ECharAttrType>(loader.GetInt(i, count++, 0));
		tempData.m_ClassType = static_cast<eATTClassType>(loader.GetInt(i, count++, 0));

		StrArgPkg tempObjectArg("|", loader.GetString(i, count++));
		for(size_t j = 0;j < tempObjectArg.size();++j)
		{
			eObjType tempType = static_cast<eObjType>(atoi(getArg(tempObjectArg,j).c_str()));
			tempData.m_HaveObject.push_back(tempType);
		}

		tempData.m_DefaultValue = loader.GetInt(i,count++,0);

		m_DataList[tempData.m_AttType] = tempData;
	}

	return true;
}

bool AttributeManage::initLua(string& str)
{
	CLuaInterface tempLua;
	string tempDataStr = str + "Data/AttributeClear.lua";
	tempLua.OpenLua();

	if(tempLua.LoadFile(tempDataStr) != 0 || tempLua.Call() != 0)
		return false;

	tempLua.SetLuaFunction("GetAttMoth");
	if(tempLua.Call(0, 1) != 0)
		return false;

	int tempCounts = tempLua.GetTableFieldCount();

	for(int i = 1;i <= tempCounts;++i)
	{
		if (tempLua.GetFieldTable(i) != 0)
			continue;

		AttTimeData tempData;
		tempData.m_Day    = tempLua.GetFieldInt(1);
		tempData.m_Hour   = tempLua.GetFieldInt(2);
		tempData.m_Minute = tempLua.GetFieldInt(3);
		tempData.m_Second = tempLua.GetFieldInt(4);

		int tempInCounts = tempLua.GetTableFieldCount();
		vector<ECharAttrType> tempAttList;
		for(int j = 5;j <= tempInCounts;++j)
		{
			int tempAtt = tempLua.GetFieldInt(j);
			if(tempAtt >= eCharAttrMax || tempAtt < eCharName)
				continue;

			tempAttList.push_back((ECharAttrType)tempAtt);
		}

		tempLua.CloseTable();

		if(tempAttList.size() == 0)
			continue;

		addClearData(tempAttList,tempData);
	}

	tempLua.CloseTable();

	tempLua.SetLuaFunction("GetAttWeek");
	if(tempLua.Call(0, 1) != 0)
		return false;

	tempCounts = tempLua.GetTableFieldCount();

	for(int i = 1;i <= tempCounts;++i)
	{
		if (tempLua.GetFieldTable(i) != 0)
			continue;

		AttTimeData tempData;
		tempData.m_Week   = tempLua.GetFieldInt(1);
		tempData.m_Hour   = tempLua.GetFieldInt(2);
		tempData.m_Minute = tempLua.GetFieldInt(3);
		tempData.m_Second = tempLua.GetFieldInt(4);

		int tempInCounts = tempLua.GetTableFieldCount();
		vector<ECharAttrType> tempAttList;
		for(int j = 5;j <= tempInCounts;++j)
		{
			int tempAtt = tempLua.GetFieldInt(j);
			if(tempAtt >= eCharAttrMax || tempAtt < eCharName)
				continue;

			tempAttList.push_back((ECharAttrType)tempAtt);
		}

		tempLua.CloseTable();

		if(tempAttList.size() == 0)
			continue;

		addClearData(tempAttList,tempData);
	}
	tempLua.CloseTable();

	tempLua.SetLuaFunction("GetAttWeek");
	if(tempLua.Call(0, 1) != 0)
		return false;

	tempCounts = tempLua.GetTableFieldCount();

	for(int i = 1;i <= tempCounts;++i)
	{
		if (tempLua.GetFieldTable(i) != 0)
			continue;

		AttTimeData tempData;
		tempData.m_Hour   = tempLua.GetFieldInt(1);
		tempData.m_Minute = tempLua.GetFieldInt(2);
		tempData.m_Second = tempLua.GetFieldInt(3);

		int tempInCounts = tempLua.GetTableFieldCount();
		vector<ECharAttrType> tempAttList;
		for(int j = 4;j <= tempInCounts;++j)
		{
			int tempAtt = tempLua.GetFieldInt(j);
			if(tempAtt >= eCharAttrMax || tempAtt < eCharName)
				continue;

			tempAttList.push_back((ECharAttrType)tempAtt);
		}

		tempLua.CloseTable();

		if(tempAttList.size() == 0)
			continue;

		addClearData(tempAttList,tempData);
	}
	tempLua.CloseTable();

	tempLua.CloseLua();

	return true;
}

bool AttributeManage::initModuleValueType(string& str)
{
	CSVLoader loader;
	string tempDataStr = str + "Data/ModleAttType.csv";

	if(!loader.OpenFromFile(tempDataStr.c_str()))
		return false;

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		eEachModuleAttType tempModuleType = static_cast<eEachModuleAttType>(loader.GetInt(i, count++, 0));
		eAttValueType	   tempValueType  = static_cast<eAttValueType>(loader.GetInt(i, count++, 0));

		m_AttModuleMapTypeList[tempModuleType] = tempValueType;
	}

	return true;
}

AttClearData* AttributeManage::addClearData(vector<ECharAttrType>& attList,AttTimeData& data)
{
	if(AttClearData::getTimeType(data) == eAttTimeNUll)
		return NULL;

	AttClearData* tempData = new AttClearData();

	tempData->init(attList,data);
	m_ClearList.push_back(tempData);

	return tempData;
}










