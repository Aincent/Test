/*
 * PetBrain.h
 *
 *  Created on: 2014年12月8日
 *      Author: root
 */

#ifndef PETBRAIN_H_
#define PETBRAIN_H_

#include "OtherDefine.h"
#include "../../Player/Player.h"
#include <vector>
#include <map>
#include "Smart_Ptr.h"
#include "define.h"
#include "MessageStruct/Map/SynchPath.pb.h"
#include "../../AI/AIBrain.h"

using namespace std;
using namespace CommBaseOut;

enum ePetState
{
	ePetStateType_Idle   = 0, // 空闲状态
	ePetStateType_Attack = 1, // 攻击状态
	ePetStateType_Dead   = 2, // 死亡状态
	ePetStateType_Max
};

class PetState;

class PetBrain
{
public:
	PetBrain();
	~PetBrain();

	void ReInit();
	void Release();

	void SetOwner(Smart_Ptr<GirlWar> &obj) { 	m_owner = obj; }
	Smart_Ptr<GirlWar> GetOwner() { 	return m_owner; }

	void Update(DWORD dwDeltaTick);

	bool ChangeState( ePetState eNewStateID);

	int GetCurState() {	 return m_curState; }

	//技能释放
	bool FireSkill(int rate);

	void AddEnemy(Smart_Ptr<CreatureObj> &obj);
	void DeleteEnemy(Smart_Ptr<CreatureObj> & obj);
	void ClearEnemy();

	//谁选我为目标了
	void addTarget(const Smart_Ptr<CreatureObj> &obj);
	void removeTarget(const Smart_Ptr<CreatureObj>& obj);
	void clearTarget();
	//我取消了某个目标
	void cancel(const Smart_Ptr<CreatureObj> &obj);

	bool IsExistedEnemy()
	{
		if(m_enemy)
		{
			return true;
		}

		return false;
	}

	//更新路径
	void UpdatePath( int dwTick );
	void UpdatePath( const PlayerSynch::SingleMonsterPath * path);
	//行走
	void Move(  int dwTick );
	//寻路
	void FindPath( int dwTick );
	void SetPath(PlayerSynch::GirlWarPathToPlayer &path);
	void ClearPathAndTime();

	void GetPerfectLocation(int moveSpeed, int xpos, int ypos, int &txpos, int &typos);

private:
	DWORD m_time;
	PetState* m_allState[ePetStateType_Max];
	ePetState m_curState;
	Smart_Ptr<GirlWar> m_owner;
	Smart_Ptr<CreatureObj> m_enemy;

	std::map<int,Smart_Ptr<CreatureObj> > m_targetList;//谁选我为目标了

	bool m_isPath; //是否已经请求寻路
	int m_pathTime; //请求寻路超时时间
	int m_curIndex; //当前第几步
	vector<DWORD> m_path; //所有的路径点
	int m_offset; //坐标偏差
	int m_curStay; //停留时间
};


#endif /* PETBRAIN_H_ */
