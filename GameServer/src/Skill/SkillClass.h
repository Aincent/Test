/*
 * SkillClass.h
 *
 *  Created on: 2014年1月21日
 *      Author: helh
 */

#ifndef SKILLCLASS_H_
#define SKILLCLASS_H_

#include "SkillBase.h"
#include "SkillFactory.h"
#include "Common.h"
#include "Timer/TimerInterface.h"

using namespace CommBaseOut;

// 瞬发技能
class CSkillBlink : public CSkillBase
{
public:
	CSkillBlink(int id, Smart_Ptr<CreatureObj> & owner,int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info);

	~CSkillBlink();

	virtual void Init(int id, Smart_Ptr<CreatureObj> & owner,int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info);

	virtual void Release();

	// 开始技能的释放
	virtual bool BeginStart();

	// 技能在释放的某个阶段被强制取消了
	virtual bool EndStart();

	// 得到技能的总时长
	virtual float GetTotalTime() const
	{
		return m_allTimer;
	}

	// 结束
	virtual void OnEndImp();

protected:
	// 飞行定时器
	// void FlyTimer(void* obj, void * arg);
	void FlyTimer(void * arg);
	//  停止定时器
	//static void StopTimer(void* obj, void * arg);
	void StopTimer(void * arg);

	// 执行技能的结果
	void Result();

protected:
	// 释放阶段定时器
	TimerConnection m_releaseTimer;
	// 总时长
	int m_allTimer;
	//结束定时器
	TimerConnection m_stopTimer;
};

//持续技能
class CSkillDelay : public CSkillBase
{
public:
	CSkillDelay(int id, Smart_Ptr<CreatureObj> & owner, int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info);

	~CSkillDelay();

	virtual void Init(int id, Smart_Ptr<CreatureObj> & owner,int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info);
	virtual void Release();

	virtual float GetTotalTime() const;

	// 开始技能的释放
	virtual bool BeginStart();

	// 技能在释放的某个阶段被强制取消了
	virtual bool EndStart();

	// 结束
	virtual void OnEndImp();

protected:

	// 飞行定时器
	void FlyTimer(void * arg);

	//  持续定时器
	void ContinueTimer(void * arg);

	//  停止定时器
	void StopTimer(void * arg);

	// 执行技能的结果
	void Result();

protected:
	// 释放阶段定时器
	TimerConnection m_releaseTimer;
	//结束定时器
	TimerConnection m_stopTimer;
	//持续定时器
	TimerConnection m_cycleTimer;
	int m_allCount;// 技能释放次数
	int m_curCount; //技能当前辞数
};

//分段技能
class CSkillNest : public CSkillBase
{
public:
	CSkillNest(int id, Smart_Ptr<CreatureObj> & owner, int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info);

	~CSkillNest();

	virtual void Init(int id, Smart_Ptr<CreatureObj> & owner,int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info);

	virtual void Release();

	virtual float GetTotalTime() const;

	// 开始技能的释放
	virtual bool BeginStart();

	//开始执行
	bool Start();

	// 技能在释放的某个阶段被强制取消了
	virtual bool EndStart();

	// 结束
	virtual void OnEndImp();

protected:

	// 飞行定时器
	void FlyTimer(void * arg);

	//  持续定时器
	void ContinueTimer(void * arg);

	//  停止定时器
	void StopTimer(void * arg);

	// 执行技能的结果
	void Result();

protected:

	// 释放阶段定时器
	TimerConnection m_releaseTimer;
	//结束定时器
	TimerConnection m_stopTimer;
	//间隔cd定时器
	TimerConnection  m_cycleTimer;
	//下一段技能
	int m_nextSkill;
};

class CSkillIntonate : public CSkillBase
{
public:
	CSkillIntonate(int id, Smart_Ptr<CreatureObj> & owner,int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info);

	~CSkillIntonate();

public:
	virtual void Init(int id, Smart_Ptr<CreatureObj> & owner,int key, int xpos, int ypos, DWORD onlyid, SkillInfo *info);

	virtual void Release();

	// 开始技能的释放
	virtual bool BeginStart();

	// 技能在释放的某个阶段被强制取消了
	virtual bool EndStart();

	// 得到技能的总时长
	virtual float GetTotalTime() const {return 0;}

	// 结束
	virtual void OnEndImp();

	//获取同步信息
	virtual bool getSynData(PlayerSynch::SynchPlayerBase& data);

protected:
	// 飞行定时器
	void FlyTimer(void * arg);

	void intonateTimer(void* p);

	//自身死亡事件
	bool handleSelfDead(const EventArgs & e);

	//状态打断事件
	bool handleBreakState(const EventArgs & e);

	//行为打断事件
	bool handleBreakDoing(const EventArgs & e);

	void breakDealWith();

	// 执行技能的结果
	void Result();

protected:
	TimerConnection m_releaseTimer;		//飞行定时器

	TimerConnection m_IntonateTimer;	//吟唱定时器

	EventConnection m_BreakEvent;		//打断事件

	EventConnection m_DoingEvent;		//打断事件

	DWORD64 m_IntonateTime;				//吟唱的CUtil::GetNowSecond()时间
};

#endif /* SKILLCLASS_H_ */
