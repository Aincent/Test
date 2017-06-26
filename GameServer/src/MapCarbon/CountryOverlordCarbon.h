/*
 * CountryOverlordCarbon.h
 *
 *  Created on: 2016年10月28日
 *      Author: root
 */

#ifndef COUNTRYOVERLORDCARBON_H_
#define COUNTRYOVERLORDCARBON_H_

#include "define.h"
#include "../Map/GameZone.h"
#include "../MapInstance/InstanceMapLoader.h"
#include <map>
#include "../EventSet/EventDefine.h"
#include "Timer/TimerInterface.h"
#include "CountryDefine.h"
#include "../Map/GameDuplicate.h"

using namespace std;
using namespace CommBaseOut;

struct LordData
{
	DWORD nKiller;
	DWORD nIntegral;
	DWORD nCounKiller;
	string sName;
	string sClanName;

	LordData():nKiller(0),nIntegral(0),nCounKiller(0)
	{
		sName.clear();
		sClanName.clear();
	}
};

class CCountryOverlordCarbon : public CGameDuplicate
{
public:
	CCountryOverlordCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params,vector<int64>& charId);
	virtual ~CCountryOverlordCarbon();

	virtual int Init();

	//更新状态,false表示要删除这个场景
	virtual bool Update();

	//加入场景
	virtual int Join(Smart_Ptr<Object> & obj);
	//加入场景前处理
	virtual bool JoinHandle(Smart_Ptr<Object> & obj);
	//场景中移动
	virtual int Move(Smart_Ptr<Object> &obj);
	//离开场景，isDel是否广播移出
	virtual void Leave(Smart_Ptr<Object> & obj, bool isDel = true, bool delFromMap = false);
	//离开场景前处理
	virtual void LeaveHandle(Smart_Ptr<Object> & obj);
	//玩家强制退出副本检验(用于判断玩家退出后副本的后续操作 )
	virtual void LeaveExame(Smart_Ptr<Player> & player);
	//此场景是否可以进入
	virtual bool IsCanEnter(Smart_Ptr<Player> & player);
	//赢了后的操作
	virtual void Win();
	//副本结束时间到了之后的 操作
	virtual void Over();
	//副本人数为零时的操作
	virtual void Count();
	virtual void CountEx();
	//输了之后的操作
	virtual void Failed();
	//奖励
	virtual void Award();
	//能否攻击
	virtual bool IsAttacked(int64 fight, int64 attacked);
	//怪物死亡事件
	bool HandleMonsterDeadEvent(const EventArgs & e);
	//玩家死亡事件
	bool HandlePlayerDeadEvent(const EventArgs & e);
protected:
	//在列表中的成员可以进入
	bool IsCanEnterList(int64 charId);
	//添加积分
	void addScore(int64 charid,int nIntegral,int nKiller = 0);
	//清空连续击杀
	int clearnCounKiller(int64 charid,int nType);
	//更新排名定时器
	void onUpdateSort(void* arg);
	//刷boss
	void FlushMonster(void* arg);
	//副本结束
	void CountryOverlordOver();
private:
	short int m_worldServerID;
	vector<int64> m_nCanEntr;
	map<int,map<int64,LordData> > m_Tang;
	map<int,map<int64,LordData> > m_Sui;

	TimerConnection m_WarPlayerSortTimer; //更新个人排名定时器
	TimerConnection m_reflushTime; //刷新怪物定时器
};



#endif /* COUNTRYOVERLORDCARBON_H_ */
