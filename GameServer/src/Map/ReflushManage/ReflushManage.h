#ifndef REFLUSHMANAGE_H_
#define REFLUSHMANAGE_H_

#include "define.h"
#include "Timer/TimerInterface.h"
#include "ReflushFile.h"

using namespace CommBaseOut;
using namespace std;

class ReflushManage;
class Monster;
class CGameZone;

enum eReflushStage
{
	eReflushStageNull = 0,
	eNotReflushStage  = 1,	//刷新定时器还没有被调用
	eReflushIngStage  = 2,  //已进入刷新定时器
};


struct ReflushOneData
{
	ReflushOneData() : m_Index(-1){}
	short 		m_Index;
	vector<int> m_KeyList;
};

/* (1)该类用于刷一个时间段内的怪物,时间过则清除
 * (2)该类有自我销毁功能
*/

class ReflushGroup
{
public:
	ReflushGroup(const ReflushData* pData,ReflushManage* pFather);

	~ReflushGroup();

public:
	bool monsterLeave(Monster& monster);

	eReflushStage getStage(){return m_Stage;}

protected:
	void reflushMonster();

	//刷新定时器回调,开始刷新和间隔刷共用一个定时器
	void reflushTimer(void* p);

	void endTimer(void* p);

	//注册定时器
	void registerTimer();

	void end();

	void removeAllMonster();

private:
	TimerConnection 		m_Timer;
	TimerConnection 		m_EndTimer;
	eReflushStage  			m_Stage;
	vector<ReflushOneData>  m_DataList;
	const ReflushData*		m_pFileData;
	ReflushManage*			m_pFather;
};

class ReflushManage
{
public:
	ReflushManage(CGameZone* pGameZone,const vector<ReflushData>& dataList);

	~ReflushManage();

public:
	CGameZone* getGameZone(){return m_pGameZone;}

	void monsterLeave(Monster& monster);

	void clear();

protected:
	void excute(const vector<ReflushData>& dataList);

private:
	CGameZone* m_pGameZone;
	vector<Smart_Ptr<ReflushGroup> > m_GroupList;
};

#endif
