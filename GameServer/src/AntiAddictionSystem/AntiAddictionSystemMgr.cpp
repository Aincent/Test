/*
 * AntiAddictionSystemMgr.cpp
 *
 *  Created on: 2014年10月28日
 *      Author: root
 */

#include "AntiAddictionSystemMgr.h"
#include "MessageCommonRet.h"
#include <time.h>
#include "util.h"
#include "FileLoader/GameParaLoader.h"

AntiAddictionSystemMgr::AntiAddictionSystemMgr(Player * player)
{
	m_owner = player;
	ReInit();
	InitRegex();
}

void AntiAddictionSystemMgr::InitAntiAddictionSystem(int state,int64 antiAddictionTime)
{
	SetAntiAddictionState(state);
	SetAntiAddictionTime(antiAddictionTime);
}

AntiAddictionSystemMgr::~AntiAddictionSystemMgr()
{
	regfree(&regRealName);
	regfree(&reg);
}

void AntiAddictionSystemMgr::ReInit()
{
	m_AntiAddictionState = eAAS_NotBind;
	m_AntiAddictionTime = 0;
	m_CurrentAntiAddictionTime = 0;
	m_lastSaveTime = CUtil::GetNowSecond();
}

void AntiAddictionSystemMgr::Release()
{

}

void AntiAddictionSystemMgr::SetAntiAddictionState(int state)
{
	EAndiAddictionState antiAddictionState = (EAndiAddictionState)state;
	if(antiAddictionState < eAAS_NotBind || antiAddictionState > eAAS_BindAndAdult)
		return;
	m_AntiAddictionState = antiAddictionState;
}

void AntiAddictionSystemMgr::SetAntiAddictionTime(int64 antiAddictionTime)
{
	if(antiAddictionTime < 0)
		return;
	m_AntiAddictionTime = antiAddictionTime;
}

float AntiAddictionSystemMgr::GetAntiAddictionFactor(void)
{
	if(this->m_AntiAddictionState == eAAS_BindAndAdult)
	{
		return 1.0f;
	}

	const int ANTI_ADDICTION_HEALTH_TIME = GameParaLoader::GetInstance()->getAntiAddictionHealthTime();
	const int ANTI_ADDICTION_TIRED_TIME = GameParaLoader::GetInstance()->getAntiAddictionTiredTime();

	int totalAntiAddictionTime = GetCurrentAntiAddictionTime();
	if(totalAntiAddictionTime >=0 && totalAntiAddictionTime <= ANTI_ADDICTION_HEALTH_TIME)
	{
		return 1.0f;
	}
	else if(totalAntiAddictionTime >ANTI_ADDICTION_HEALTH_TIME && totalAntiAddictionTime <=ANTI_ADDICTION_TIRED_TIME)
	{
		return 0.5f;
	}
	else if(totalAntiAddictionTime > ANTI_ADDICTION_TIRED_TIME)
	{
		return 0.0f;
	}
	return 0.0f;
}

int64 AntiAddictionSystemMgr::GetCurrentAntiAddictionTime(void)
{
	int totalAntiAddictionTime = m_AntiAddictionTime + CUtil::GetNowSecond() - m_owner->GetLoginTime();
	return totalAntiAddictionTime;
}

EAndiAddictionState AntiAddictionSystemMgr::GetAntiAddictionState(void)
{
	return m_AntiAddictionState;
}

void AntiAddictionSystemMgr::InitRegex(void)
{
	const char * reqNameStr = "[^\u4E00-\u9FA5]";//匹配非汉字
	regcomp(&regRealName,reqNameStr,REG_EXTENDED | REG_NEWLINE); //从字符串来编译这个表达式。

	const char * reqStr = "(^([[:digit:]]{15})$)|(^([[:digit:]]{17}([[:digit:]]|X|x)$))";
	regcomp(&reg,reqStr,REG_EXTENDED | REG_NEWLINE); //从字符串来编译这个表达式。
}

int AntiAddictionSystemMgr::CanBind(AntiAddictionProto::ReqBindAntiAddictionID& req)
{
	string realName = req.realname();
	int realNameLen = realName.length();
	if(realNameLen < 4 || realNameLen > 12)
		return eBindIDRealNameError;

	int retRealName = regexec(&regRealName,realName.c_str(),0,NULL,0); //匹配字符串
	//匹配到非汉字说明名字有错
	if(retRealName == 0)
	{
		return eBindIDRealNameError;
	}
	//判断身份证是否合法
	string antiAddictionID = req.antiaddictionid();
	int idLen = antiAddictionID.length();

	int ret = regexec(&reg,antiAddictionID.c_str(),0,NULL,0); //匹配字符串
	if(ret != 0)
	{
		return eBindIDError;
	}
	//判断省市代码
	int cityCode = atoi(antiAddictionID.substr(0,6).c_str());
	bool isCityExit = AntiAddictionSystemLoader::GetInstance()->IsCityCodeExit(cityCode);
	if(isCityExit == false)
		return eBindIDError;
	//判断年龄
	int year = 0;
	int month = 0;
	int day = 0;
	if(idLen == 18)
	{
		year = atoi(antiAddictionID.substr(6,4).c_str());
		month = atoi(antiAddictionID.substr(10,2).c_str());
		day = atoi(antiAddictionID.substr(12,2).c_str());
	}
	else
	{
		//15位的身份证没有19XX
		year = atoi(antiAddictionID.substr(6,2).c_str()) + 1900;
		month = atoi(antiAddictionID.substr(8,2).c_str());
		day = atoi(antiAddictionID.substr(10,2).c_str());
	}
	if(day < 1 || day > 31)
		return eBindIDError;
	if(month < 1 || month > 12)
		return eBindIDError;

	time_t curtime=time(0);

	tm tim;
	localtime_r(&curtime,&tim);

	int curDay,curMonth,curYear;
	curDay=tim.tm_mday;//1~31
	curMonth=tim.tm_mon + 1;//当月
	curYear=tim.tm_year + 1900;//今年

	//80岁以上不给玩
	if(curYear - year > 80)
		return eBindIDError;

	if(curYear - year< 18)
		return eBindIDUnderAge;

	if(curYear - year== 18 && month < curMonth)
		return eBindIDUnderAge;

	if(curYear - year == 18 && month == curMonth && day < curDay)
		return eBindIDUnderAge;

	return 0;
}
