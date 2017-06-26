/*
 * LucklyTurntableLoader.h
 *
 *  Created on: 2014年8月20日
 *      Author: root
 */

#ifndef LUCKLYTURNTABLELOADER_H_
#define LUCKLYTURNTABLELOADER_H_

#include <map>
#include "define.h"
#include "Singleton.h"
#include "CharDefine.h"

using namespace std;
using namespace CommBaseOut;

#define LUCKLY_REWARD_MAX_COUNTS   12    //奖励总共有12个

typedef struct sTurntableRward
{
	int  m_ID;
	WORD m_BindType;	//绑定方式，该绑定方式目前为免费次数时为绑定，非绑定时根据配表设置。
	sTurntableRward():m_ID(0),m_BindType(0){}
}TurntableRward;

struct LucklyTurntableInfo
{
	LucklyTurntableInfo():m_ID(0),m_ItemID(0),m_Counts(0),m_Type(0),m_Probability(0),m_BindType(0){}
	int 		m_ID; //ID
	int 		m_ItemID;
	int 		m_Counts;
	short 		m_Type;
	int 		m_Probability;
	WORD 		m_BindType;//绑定方式
};

typedef struct sProLucklyTurntable
{
	int id;
	int oneCost; //点击一次消耗
	int tenCost; //点击十次每次消耗
	int refuCost; //刷新消耗
	int openLv; //开放等级
	int interTm;  //刷新间隔毫秒数
	int freeRoll; //免费转的次数
	int countNum; //空间 大小
	sProLucklyTurntable():id(0),oneCost(0),tenCost(0),refuCost(0),openLv(0),interTm(0),freeRoll(0),countNum(0)
	{}
}ProLucklyTurntable;

class LucklyTurntableLoader: public Singleton<LucklyTurntableLoader>
{
public:
	LucklyTurntableLoader(){}

	~LucklyTurntableLoader(){}

	int InitDate(string &file);

	int InitLucklyTurntable(string &file);

	int InitProLuckly(string &file);

	const ProLucklyTurntable* GetProLuckly();

	const map<short,vector<LucklyTurntableInfo> >& GetFileDataList();

	const LucklyTurntableInfo* getInfoByID(int id);

	//物品数量和物品ID转成表ID，由m_tmpAward字段的修改，为了兼容数据库
	int itemIDToFileDataID(int itemID,int counts);

private:
	map<short,vector<LucklyTurntableInfo> > m_allLucklyTurntable;		//以类型做为键值
	ProLucklyTurntable 			   			m_proLuckly;
};

#endif /* LUCKLYTURNTABLELOADER_H_ */
