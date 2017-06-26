/*
 * TreasonKingCarbon.h
 *
 *  Created on: 2015年6月30日
 *      Author: root
 */

#ifndef TREASONKINGCARBON_H_
#define TREASONKINGCARBON_H_

#include "define.h"
#include "../Map/GameZone.h"
#include "../MapInstance/InstanceMapLoader.h"
#include <map>
#include "../EventSet/EventDefine.h"
#include "MessageStruct/Clan/ClanBattle.pb.h"
#include "MessageStruct/BeyondServerBattle/BeyondBattle.pb.h"
#include "Timer/TimerInterface.h"
#include "CountryDefine.h"
#include "../Map/GameDuplicate.h"

using namespace std;
using namespace CommBaseOut;

class CTreasonKingCarbon : public CGameDuplicate
{
public:
	CTreasonKingCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params);
	virtual ~CTreasonKingCarbon();

	virtual int Init();

	//更新状态,false表示要删除这个场景
	virtual bool Update();

	//加入场景
	virtual int Join(Smart_Ptr<Object> & obj);
	//加入场景前处理
	virtual bool JoinHandle(Smart_Ptr<Object> & obj);
	//场景中移动
	virtual int Move(Smart_Ptr<Object> &obj);
	//怪物同步路径给所有玩家
	virtual void SendSynchPath(PlayerSynch::MonsterPathToPlayer &path, Smart_Ptr<Monster> &monster);
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
	//怪物死亡事件
	bool HandleMonsterDeadEvent(const EventArgs & e);
	//退出场景事件(如果不希望玩家下次还能进入此场景，最好在此处理)
	bool HandleLeaveMapEvent(const EventArgs & e);

	void SendAwardToPlayer(Smart_Ptr<Player> & player);
private:
};


#endif /* TREASONKINGCARBON_H_ */
