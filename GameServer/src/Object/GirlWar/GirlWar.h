/*
 * GirlWar.h
 *
 *  Created on: 2014年12月4日
 *      Author: root
 */

#ifndef GIRLWAR_H_
#define GIRLWAR_H_

#include "../Creature.h"
#include "../Object.h"
#include <vector>
#include <map>
#include "define.h"
#include "../../Map/Sector.h"
#include "Smart_Ptr.h"
#include "util.h"
#include "../Player/Player.h"

using namespace std;
using namespace CommBaseOut;

class PetBrain;

class GirlWar : public CreatureObj
{
public:
	GirlWar(eObjType type);
	~GirlWar();

	void Realese();

	//增加战姬经验
	int AddExp(int exp);

	//设置战姬所属
	void SetOwner(Smart_Ptr<Player>& player);
	//获得战姬所属玩家
	Smart_Ptr<Player> GetOwner();
	//设置战姬AI所属
	void SetOwnerToBrain(Smart_Ptr<GirlWar> &obj);

	Smart_Ptr<GirlWar> &GetMyself() { 	return m_myself; }
	void SetMyself(const Smart_Ptr<GirlWar> &obj)
	{
		m_myself = obj;
	}
	Smart_Ptr<CreatureObj> GetCreatureSelf() { 	return m_myself; }

	//战姬重定位坐标
	virtual void RelocatePos(int xpos, int ypos, bool isSyn = false);

	void RelocatePosToAll(int xpos, int ypos, bool isSyn = false);
	//获取下个目标坐标点
	void GetNextPosition(int &xpos, int &ypos);

	//是否达到攻击距离
	bool IsAttackInLength(int distance);

	//同步战姬路径
	void SynchPath(PlayerSynch::GirlWarPathToPlayer &path);
	//清空战姬路径
	void ClearPath();

	//根据类型得到战姬属性
	int64 GetTypeValue(int type);
	//同步战姬属性
	void SynCharAttribute(vector<int> &type);
	//发送添加或者删除buff的消息
	void SendBuffState(CharBattle::BuffOperate * buf);

	virtual void Dead(Smart_Ptr<CreatureObj> &obj);
	virtual bool IsDead();
	//virtual bool IsInMap();
	virtual bool AddBuff(int id, Smart_Ptr<CreatureObj> &obj, int num=1);
	//移出buff不用删除buff
	virtual bool RemoveBuffByOnlyID(DWORD id);
	//移出buff并且结束buff
	virtual bool DeleteBuffByOnlyID(DWORD id);

	virtual void removeBuffByGroupID(DWORD groupID);

	void RemoveCanBuff();

	virtual int IsAttacked(Smart_Ptr<CreatureObj> &obj, int type);
	//被攻击了
	virtual void Attacked(Smart_Ptr<CreatureObj> &obj);
	//攻击别人
	virtual void Attack(int skillid);

	void RemoveSkillBuff(int skillid);

	bool IsViewInLength(int xpos, int ypos);

	//进入场景时同步战姬信息
	void SendGirlWarSynch();

	void SetTarget(Smart_Ptr<CreatureObj> &obj);

	Smart_Ptr<CreatureObj> & GetTargetObj()
	{
		return m_targetObj;
	}

	void UpdateEnemyList(Smart_Ptr<Player> &player);

	void ClearEnemy();

	//通知选我为目标的对象
	void ClearTargets();

	void Update(int dwTick);

	void SetDeadTime(int64 nDeadTime)
	{
		m_nDeadTime = nDeadTime;
	}

	int64 GetDeadTime()
	{
		return m_nDeadTime;
	}

	void SetModle(int nModle)
	{
		m_nModle = nModle;
	}

	int GetModle()
	{
		return m_nModle;
	}

	void SetIsWar(bool bFlag);


	bool GetIsWar()
	{
		return m_bIsWar;
	}

	//战姬复活
	void Reflush();

	void ReflushInstance();

	void BeginIdle();

	void SetPhyAttackTime(int time )
	{
		m_phyAtkTime = time;
	}

	int GetPhyAttackTime()
	{
		return m_phyAtkTime;
	}

	int GetSkill1()
	{
		return m_nSkill1;
	}

	int GetSkill2()
	{
		return m_nSkill2;
	}

	int GetSkill3()
	{
		return m_nSkill3;
	}

	void SetSkill1(int nSkill)
	{
		m_nSkill1 = nSkill;
	}

	void SetSkill2(int nSkill);

	void SetSkill3(int nSkill);

	int GetStarrank()
	{
		return m_nStarrank;
	}

	void SetStarrank(int nStarrank)
	{
		m_nStarrank = nStarrank;
	}

	int GetQuality()
	{
		return m_nQuality;
	}

	void SetQuality(int nQuality)
	{
		m_nQuality = nQuality;
	}

	Smart_Ptr<PetBrain> & GetBrain()
	{
		return m_brain;
	}

	map<int,int>& GetEquip()
	{
		return m_mEquit;
	}

	void SetEquip(int nIndex,int nEquipId)
	{
		m_mEquit[nIndex] = nEquipId;
	}

	//体战
	void Rest();

	//出战
	void Fight();

	//战姬和玩家是否在同一地图
	bool InDefMap();

	//获取最佳的目标位置(当前生物以当前点和速度去获取)
	virtual void GetPerfectLocation(int moveSpeed, int xpos, int ypos, int &txpos, int &typos);

	void onReviveTime(void* obj);

	bool IsHaveEquip(int nEquipId);

	bool IsPosEmpty(int nIndex);

	int GetSkill(int nIndex);

	void SetSkill(int nIndex,int skillId);

	void ReSkill(int nIndex,int skillId = 0);

	void ChangeOwerAttribute(bool bFlag);

	void ChangeAttribute(int nHp,int nPhyAck,int nPhydef,int nCrit,int nUng,int nDodge,bool bFlag = true);
private:
	bool m_bIsWar;
	int m_nModle;
	int m_phyAtkTime;
	int m_nSkill1;  //主动技能
	int m_nSkill2;	//被动技能
	int m_nSkill3; //被动技能
	int m_nStarrank; //星阶
	int m_nQuality; //品质
	int64 m_nDeadTime;//死亡时间
	TimerConnection m_sReviveTime;
	Smart_Ptr<PetBrain> m_brain;
	Smart_Ptr<GirlWar> m_myself;
	Smart_Ptr<Player> m_Owner;
	Smart_Ptr<CreatureObj> m_targetObj; //
	map<DWORD, BuffBase *> m_buffList; //buff连表
	map<int,int> m_mEquit;
};


#endif /* GIRLWAR_H_ */
