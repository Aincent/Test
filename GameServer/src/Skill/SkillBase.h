/*
 * SkillBase.h
 *
 *  Created on: 2014年1月21日
 *      Author: helh
 */

#ifndef SKILLBASE_H_
#define SKILLBASE_H_

#include "../Object/Creature.h"
#include <vector>
#include <map>
#include "CharDefine.h"
#include "SkillLoader/SkillLoader.h"
#include "Timer/TimerInterface.h"
#include "MessageStruct/Map/SynchPath.pb.h"
#include "../EventSet/EventDefine.h"

using namespace std;


struct AttrChange
{
	//属性类型
	int type;
	//目标值
	int64 value;
	//目标状态
	int64 state;

	Smart_Ptr<CreatureObj> obj;

	Smart_Ptr<CreatureObj> m_Target;

	AttrChange():type(-1),value(0),state(eAttackSim)
	{

	}
};

// 技能基类
class CSkillBase
{
	friend class CSkillFactory;

protected:
	CSkillBase(int id, Smart_Ptr<CreatureObj> & owner,int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info);

	virtual ~CSkillBase();

public:
	enum
	{
		// 技能被打断
		EventBreak,
		// 技能开始产生效果
		EventBringResult,
		// 技能结束
		EventEnd,
	};

	// 得到技能的ID
	inline int GetID() const
	{
		return m_id;
	}

	int GetType()
	{
		return m_info==NULL ? 0 : m_info->sType;
	}

	void LimitTargetsNum(vector<Smart_Ptr<CreatureObj> >& targets,const unsigned int uiLimit);

	// 得到释放者
	Smart_Ptr<CreatureObj> & GetOwner()
	{
		return m_owner;
	}

	void InitInfo(int id, Smart_Ptr<CreatureObj> & owner,int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info);

	virtual void Init(int id, Smart_Ptr<CreatureObj> & owner,int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info)
	{

	}

	virtual void Release();

	// 此技能开始产生作用
	virtual void ResultEffect(int effectIndex, vector<Smart_Ptr<CreatureObj> > & targets, map<int, vector<AttrChange> >& allAttr, map<int, vector<AttrChange> >& myselfAttr);
	virtual void ResultEffectAttrChange(map<int, vector<AttrChange> >& allAttr, map<int, vector<AttrChange> >& myselfAttr);

	void ResultEffect(Smart_Ptr<CreatureObj> & targets, vector<AttrChange> &changeVec, vector<AttrChange> &changeMyself, int type, int *param,int index);

	int GetValue(Smart_Ptr<CreatureObj> & targets, int *param);

	// 得到技能的总时长
	virtual float GetTotalTime() const = 0;

	// 开始技能的释放
	virtual bool BeginStart() = 0;

	bool BeginAction();

	// 技能在释放的某个阶段被强制取消了
	virtual bool EndStart() = 0;

	//死亡事件
	bool HandleCreatureDead(const EventArgs & e);
	//离开地图事件
	bool HandleCreatureLeaveMap(const EventArgs & e);

	// 技能是否结束了
	inline bool IsEnd() const
	{
		return m_bEnd;
	}

	SkillInfo * GetSkillInfo()
	{
		return m_info;
	}

	void SetTarget(int key, int xpos, int ypos)
	{
		m_targetKey = key;
		m_xpos = xpos;
		m_ypos = ypos;
	}
	uint GetOnlyID()
	{
		return m_onlyID;
	}

	virtual bool getSynData(PlayerSynch::SynchPlayerBase& data){return false;}

protected:

	// 同步玩家释放技能
	void SynchFire();

	// 技能执行结束了
	void OnEnd();
	// 技能执行结束了,但是不销毁所有者指针
	void OnEndEx();

	// 结束
	virtual void OnEndImp()
	{

	}

	// 技能开始扣血法
	void Amerce();

	// 开始CD
	void CDTimer();

	void actionTime(void * p);

	Smart_Ptr<CreatureObj> m_owner;     // 技能的拥有者
	int m_targetKey;												//目标KEY值
	int m_xpos;
	int m_ypos;
	int      m_id;        // 链接的技能ID
	unsigned int m_onlyID; // 技能的唯一ID
	bool         m_bEnd;
	SkillInfo * m_info;
	//动作时间
	TimerConnection m_ActionTimer;
	//死亡事件
	EventConnection m_stopEvent;
	//离开场景事件
	EventConnection m_leaveEvent;
};



#endif /* SKILLBASE_H_ */
