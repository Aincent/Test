/*
 * ChallengeCarbon.h
 *
 *  Created on: 2016年4月25日
 *      Author: root
 */

#ifndef CHALLENGECARBON_H_
#define CHALLENGECARBON_H_

#include "define.h"
#include "../Map/GameZone.h"
#include "../MapInstance/InstanceMapLoader.h"
#include "../EventSet/EventDefine.h"
#include "Timer/TimerInterface.h"
#include "PVPDefine.h"
#include "../Map/GameDuplicate.h"

using namespace std;
using namespace CommBaseOut;

class ChallengeCarbon : public CGameDuplicate
{
public:
	ChallengeCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params);
	virtual ~ChallengeCarbon();

public:
	int64 FitchReWard(Smart_Ptr<Player> player);

protected:

	int Init();

	//更新状态,false表示要删除这个场景
	bool Update();

	//加入场景
	int Join(Smart_Ptr<Object> & obj);

	//加入场景前处理
	bool JoinHandle(Smart_Ptr<Object> & obj);

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
	void Win();

	//副本结束时间到了之后的 操作
	void Over();

	//副本人数为零时的操作
	void Count();
	void CountEx();

	//输了之后的操作
	void Failed();

	//奖励
	void Award();

	//怪物死亡事件
	bool HandleMonsterDeadEvent(const EventArgs & e);

	//玩家死亡事件
	bool HandlePlayerDeadEvent(const EventArgs & e);

	void InitTargets();

	void AddTarget(int id);

	void Result();

private:

	std::map<int,pair<int,int> > m_killMonsters;
	int m_star; //最终星级
	bool m_isFitch;
};




#endif /* CHALLENGECARBON_H_ */
