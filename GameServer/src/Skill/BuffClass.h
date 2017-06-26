/*
 * BuffClass.h
 *
 *  Created on: 2014年2月12日
 *      Author: helh
 */

#ifndef BUFFCLASS_H_
#define BUFFCLASS_H_

#include "define.h"
#include "BuffBase.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "Timer/TimerInterface.h"

using namespace CommBaseOut;

class CreatureObj;

// 永久类BUFF
// 适用于，执行一次或多次，无时间限制的BUFF
class BuffForever : public BuffBase
{
public:
	// 间隔时长小于等于零,则只执行一次
	BuffForever(Smart_Ptr<CreatureObj>& owner, Smart_Ptr<CreatureObj>& caster, int id, DWORD onlyID);

	// 析构
	~BuffForever();

	virtual void Init(Smart_Ptr<CreatureObj> & owner, Smart_Ptr<CreatureObj> & caster, DWORD onlyID, int id, int alltime, int timer, int num);

	virtual void Release();

	// 开始
	virtual bool Start(bool isDel = true);

	//获得类型
	virtual int GetType();

	//重启BUFF时间
	virtual bool ReStart(){return true;}

	// 结束,外部结束
	virtual void End();

	virtual void SetInfo(PlayerInfo::SkillInfo *info, bool isDel);

	//获得结束时间
	virtual int64 GetEndTime(){return 0;}
protected:
	// BUFF结束了
	virtual void OnEndImp();

private:

	bool m_firstExcute; // 使否首次执行
};

// 执行次数类,次数到后，不再执行
class BuffCount : public BuffBase
{
public:
	// 如果间隔时间为-1,则只执行一次
	BuffCount(Smart_Ptr<CreatureObj>& owner, Smart_Ptr<CreatureObj>& caster, int id, DWORD onlyID, int alltime, int allCount);

	// 析构
	~BuffCount();

	virtual void Init(Smart_Ptr<CreatureObj> & owner, Smart_Ptr<CreatureObj> & caster, DWORD onlyID, int id, int alltime, int timer, int num);

	virtual void Release();

	// 开始
	virtual bool Start(bool isDel = true);
	//获得类型
	virtual int GetType();
	//重启BUFF时间
	virtual bool ReStart(){return true;}
	// 结束,外部结束
	virtual void End();

	virtual void SetInfo(PlayerInfo::SkillInfo *info, bool isDel);

	//获得结束时间
	virtual int64 GetEndTime(){return m_endTime;}
protected:

	// 循环时间
	void CycTimer(void* obj);

	// 注册循环定时器
	void RegCycTimer();

	// BUFF结束了
	virtual void OnEndImp();

private:
	float m_spaceTimer; // 间隔时间
	size_t m_allCount;  // 执行的总次数

	TimerConnection m_stime;
	int64 m_endTime; //结束时间
};


// 执行一次，到时间后删除
class BuffOnceTimer : public BuffBase
{
public:
	// 执行一次,到时间后删除
	BuffOnceTimer(Smart_Ptr<CreatureObj>& owner, Smart_Ptr<CreatureObj>& caster, int id, DWORD onlyID, int allTime);

	// 执行一次
	~BuffOnceTimer();

	virtual void Init(Smart_Ptr<CreatureObj> & owner, Smart_Ptr<CreatureObj> & caster, DWORD onlyID, int id, int alltime, int timer, int num);

	virtual void Release();

	//获得类型
	virtual int GetType();
	//重启BUFF时间
	virtual bool ReStart();
	// 开始
	virtual bool Start(bool isDel = true);

	// 结束,外部结束
	virtual void End();

	virtual void SetInfo(PlayerInfo::SkillInfo *info, bool isDel);

	//获得结束时间
	virtual int64 GetEndTime(){return m_endTime;}
protected:

	// 结束定时器
	void EndTimer(void* obj);

	// 结束了
	virtual void OnEndImp();


protected:

	int m_allTimer;   // 总时长
	TimerConnection m_stime;
	int64 m_endTime; //结束时间
};


// Buff容量 间隔一段时间执行  容量用完 buff结束     Invl interval
class BuffCapInvlTime : public BuffBase
{
public:
	//
	BuffCapInvlTime(Smart_Ptr<CreatureObj>& owner, Smart_Ptr<CreatureObj>& caster, int id, DWORD onlyID,
			int cap, int invlTime);

	~BuffCapInvlTime();

	virtual void Init(Smart_Ptr<CreatureObj> & owner, Smart_Ptr<CreatureObj> & caster, DWORD onlyID, int id, int alltime, int timer, int num);

	virtual void Release();

	//获得类型
	virtual int GetType();
	//重启BUFF时间
	virtual bool ReStart();
	// 开始
	virtual bool Start(bool isDel = true);

	// 结束,外部结束
	virtual void End();

	virtual void SetInfo(PlayerInfo::SkillInfo *info, bool isDel);

	//获得结束时间 Buff容量没用到
	virtual int64 GetEndTime(){return 0;}

	// 获得当前容量
	int GetCurCap()
	{
		return m_capacity;
	}

protected:

	// 循环定时器
	void LoopsTimer(void* obj);

	// 结束了
	virtual void OnEndImp();

	//效果  特例
	virtual void ResultEffect(Smart_Ptr<CreatureObj> target,const BuffInfo* info, vector<AttrChange> &toMyself, vector<AttrChange> &toOther);


	bool checkCondition();

protected:

	int m_intervalTime; // 间隔时间 执行
	int m_capacity; // 当前容量

	TimerConnection m_time;

};



#endif /* BUFFCLASS_H_ */
