/*
 * Creature.h
 *
 *  Created on: 2013��9��27��
 *      Author: helh
 */

#ifndef CREATURE_H_
#define CREATURE_H_

#include "Object.h"
#include "define.h"
#include "Smart_Ptr.h"
#include "EventSet/EventSet.h"
#include "StateMgr.h"
#include "../Attribute/AttributeType.h"


using namespace std;
using namespace CommBaseOut;

class Sector;
class CGameZone;
struct BuffGetPublicData;
class AttributeCreature;
//--------------------------------------------------------------------------
// 角色状态标识 -- 该状态改变，需要向周围玩家广播
//--------------------------------------------------------------------------
enum ECreatureState
{
	ERS_NULL			= 0x0000,
	ERS_Dead			= 0x0001,	// 死亡状态
	ERS_Fight			= 0x0002,	// 战斗状态

	ERS_Gather			= 0x0004,	// 采集状态
	ERS_Still = 0x0008, //僵直状态
	ERS_Ice = 0x0010, //冰冻状态
	ERS_Stun = 0x0020, //晕眩状态

	ERS_Meditation = 0X0040,	//打坐状态

	ERS_Shop			= 0x0100,	// 商店交易
	ERS_Exchange	= 0x0200,	// 玩家间直接交易
	ERS_Stall		= 0x0400,	// 摆摊交易
	ERS_StallSet	= 0x0800,	// 设置摆摊物品

	ERS_Escort		= 0x1000,	// 运镖中
	ERS_MONSTERESCORT_STAND = 0X2000,	//镖车停止了
	ERS_RED_NAME = 0X4000,//红名状态

	ERS_ImmunityStill = 0x8000,   //免疫僵直
	ERS_ImmunityIce   = 0x10000,  //免疫冰冻
	ERS_ImmunityStun  = 0x20000,  //免疫晕眩
	ERS_Invincible	  = 0x40000,  //无敌状态
	ERS_ImmuneHurtShield	  =	0x80000,   //免疫护盾状态
	ERS_MustBeCrit			  = 0x100000,  //必暴击状态
	ERS_Walk			  	  = 0x200000,  //行走状态
	ERS_Intonate			  = 0x400000,  //吟唱状态
	ERS_AttackBack			  = 0x800000,  //击退状态
	ERS_Horse				  = 0x1000000,  //坐骑状态
	ERS_Distortion	  = 0x2000000,  //变身状态
	ERS_Silent		  = 0x4000000,	//沉默状态
	ERS_ImmunitySilent = 0x8000000, //免疫沉默
	ERS_Sleep		   = 0x10000000, //睡眠状态
	ERS_ImmunitySleep  = 0x20000000, //免疫睡眠
	ERS_Poisoning	   = 0x40000000,		//中毒状态
	RES_ImmunityPoison = 0x80000000,		//免疫中毒
};

class CreatureObj : public Object, public EventSet
{
public:
	CreatureObj(eObjType type);
	virtual ~CreatureObj();

	void Release();

	int64 GetID()	{		return m_ID;	}
	void SetID(int64 id)	{		m_ID = id;	}

	string &GetName()	{		return m_name;	}
	void SetName(const string &name)	{		m_name = name;	}

	virtual int ChangeMoveSpeed(const int &value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);

	virtual int ChangePhyAttackMax(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);

	virtual int ChangePDefence(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);

	virtual int ChangeHRate(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);

	virtual short ChangePercentHurt(int value, bool isAdd = true);

	virtual int ChangeARate(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);

	virtual int ChangeCRate(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);

	virtual void SetHPMax(int max);
	virtual int ChangeHPMax(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);

	virtual int CurHPChange(int hp, bool flag = true);
	virtual int ChangeHPRate(int hp, bool flag = true);
	virtual void SetCurHP(int hp);

	virtual void SetMPMax(int max);
	virtual int ChangeMPMax(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);

	virtual void SetCurMP(int mp);
	virtual int ChangeCurMP(int value, bool isAdd = true);

	int GetXNewPos();

	void SetXNewPos(int xpos);

	void SetXNewPos();

	int GetYNewPos();

	void SetYNewPos(int ypos);

	void SetYNewPos();

	//获取最佳的目标位置(当前生物以当前点和速度去获取)
	virtual void GetPerfectLocation(int moveSpeed, int xpos, int ypos, int &txpos, int &typos)
	{
		txpos = GetXNewPos();
		typos = GetYNewPos();
	}

	//重新定向到某个位置
	virtual void RelocatePos(int xpos, int ypos, bool isSyn = false) {}

	virtual int ChangeBaserte(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);

	//增加力量值
	virtual void AddStrength(int value){}
	//增加灵力
	virtual void AddCleverPower(int value) {}
	//增加灵气
	virtual void AddCleverAir(int value) {}
	//增加体力值
	virtual void AddPhyPower(int value){}
	//增加灵力值
	virtual void AddIntelligence(int value){}
	//增加耐力值
	virtual void AddPatience(int value){}
	//增加敏捷值
	virtual void AddAgility(int value){}
	//增加buff
	virtual bool AddBuff(int id, Smart_Ptr<CreatureObj> &obj, int num=1)
	{
		return true;
	}

	 //获得为参数指定的BUFF
	virtual void getBuffByEffectType(vector<DWORD>& onlyIDList,const BuffGetPublicData& data){}

	//移出buff不用删除buff
	virtual bool RemoveBuffByOnlyID(DWORD id)
	{
		return true;
	}
	//移出buff并且结束buff
	virtual bool DeleteBuffByOnlyID(DWORD id)
	{
		return true;
	}
	//根据组ID移除buff
   virtual void removeBuffByGroupID(DWORD groupID){}
	//����
	virtual void Dead(Smart_Ptr<CreatureObj> &obj);

	//�Ƿ�����
	virtual bool IsDead();
	virtual bool IsInMap();

	virtual void Attacked(Smart_Ptr<CreatureObj> &obj){}
	virtual void Attack(Smart_Ptr<CreatureObj> &obj){}

	//判断是否可以攻击
	virtual int IsAttacked(Smart_Ptr<CreatureObj> &obj, int type);

	bool IsHitObject(Smart_Ptr<CreatureObj>& targets);

	bool IsAttackCrack(Smart_Ptr<CreatureObj>& target);

	//获取属性值
	virtual int64 GetTypeValue(int type);
	//更新仇恨值
	virtual void UpdateEnemyValue(int value, DWORD key){}


	//------------------------------------------------------------------------------
	// 当前状态 -- 各状态间的转换，需要手动完成。即SetState()不会自动清除其他状态位。
	//------------------------------------------------------------------------------
	DWORD GetCreatureState() const { return m_CreatureState.GetState(); }

	bool IsInCreatureState(ECreatureState eState) const
	{
		return m_CreatureState.IsInState(eState);
	}

	bool IsInCreatureStateAll(DWORD dwState) const
	{
		return m_CreatureState.IsInStateAll(dwState);
	}

	bool IsInCreatureStateAny(DWORD dwState) const
	{
		return m_CreatureState.IsInStateAny(dwState);
	}

	void SetCreatureState(ECreatureState eState, bool bSendMsg=true);

	void UnsetCreatureState(ECreatureState eState, bool bSendMsg=true);

	Sector * GetSector(void)
	{
		return m_pSector;
	};
	CGameZone * GetGameZone(void)
	{
		return m_pGameZone;
	};
	void SetSector(Sector *sector);
	void SetGameZone(CGameZone *gameZone);
	void ResetSector(void);
	void ResetGameZone(void);

	Smart_Ptr<AttributeCreature> getAttManage();

public:
	typedef State<DWORD64, ECreatureState>				CreatureState;

protected:
	Smart_Ptr<AttributeCreature> m_AttManage;

private:
	//-------------------------------------------------------------------------------------
	// 玩家当前状态
	//-------------------------------------------------------------------------------------
	CreatureState			m_CreatureState;						// 玩家状态	-- 变化后，需通知周围玩家

	int64 m_ID;
	string m_name;
	//所在格子
	Sector * m_pSector;
	//所在的场景
	CGameZone * m_pGameZone;
};


#endif /* CREATURE_H_ */
