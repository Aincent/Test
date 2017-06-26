/*
 * AntiAddictionSystemMgr.h
 *
 *  Created on: 2014年10月28日
 *      Author: root
 */
#ifndef ANTIADDICTIONSYSTEMMGR_H_
#define ANTIADDICTIONSYSTEMMGR_H_

#include "CharDefine.h"
#include "define.h"
#include "util.h"
#include "Smart_Ptr.h"
#include "MessageStruct/AntiAddiction/AntiAddictionProto.pb.h"
#include "../Object/Player/Player.h"
#include "./FileLoader/AntiAddictionSystemLoader.h"
#include <regex.h>

using namespace std;
using namespace CommBaseOut;


class Player;

class AntiAddictionSystemMgr
{
public:
	AntiAddictionSystemMgr(Player * player);
	~AntiAddictionSystemMgr();

	void ReInit();
	void Release();

	void InitAntiAddictionSystem(int state,int64 antiAddictionTime);

	void SetAntiAddictionState(int state);

	EAndiAddictionState GetAntiAddictionState(void);
	//登陆时的累积在线时间
	void SetAntiAddictionTime(int64 antiAddictionTime);
	//获得防沉迷的收益系数
	float GetAntiAddictionFactor(void);
	//获得当前的累积在线时间
	int64 GetCurrentAntiAddictionTime(void);
	//真实姓名和身份证是否合法
	int CanBind(AntiAddictionProto::ReqBindAntiAddictionID & req);

	void InitRegex(void);

	void SetLastSaveTime(int64 lastSaveTime)
	{
		m_lastSaveTime = lastSaveTime;
	}

	int64 GetLastSaveTime(void)
	{
		return m_lastSaveTime;
	}

	int64 GetAntiAddictionTime()
	{
		return m_AntiAddictionTime;
	}
private:
	Player * m_owner;
	//绑定状态：0未绑定 1绑定但未成年 2绑定成功
	EAndiAddictionState m_AntiAddictionState;
	//防沉迷时间，从数据库获得
	int64 m_AntiAddictionTime;
	//当前反沉迷时间＝m_AntiAddictionTime＋在线时间
	int64 m_CurrentAntiAddictionTime;
	//上一次保存防沉迷的时间
	int64 m_lastSaveTime;

	regex_t regRealName; //先声明一个正则表达式体

	regex_t reg; //先声明一个正则表达式体
};

#endif /* ANTIADDICTIONSYSTEMMGR_H_ */
