/*
 * AIBrain.h
 *
 *  Created on: 2013��10��22��
 *      Author: helh
 */

#ifndef AIBRAIN_H_
#define AIBRAIN_H_

#include "OtherDefine.h"
#include "../Player/Player.h"
#include <vector>
#include <map>
#include "Smart_Ptr.h"
#include "define.h"
#include "MessageStruct/Map/SynchPath.pb.h"
#include "../../Map/ConfigLoader/FieldMonster.h"
#include "../FileLoader/AILoader.h"

using namespace std;
using namespace CommBaseOut;

class AIState;
class Monster;

enum eAIState
{
	eAIStateType_Idle   = 0, // 空闲状态
	eAIStateType_Attack = 1, // 攻击状态
	eAIStateType_Return = 2, // 返回出生位置
	eAIStateType_Dead   = 3, // 死亡状态
	eAIStateType_Max    = 4 // ���״̬����
};

enum eStrategyType
{
	eStrategyNull = 0, //�޴�����
	eStrategyHP = 1, //Ѫ���½���ĳһ�̶ֳ�
	eStrategyAtkTime = 2, //ս��ʱ�䳬��ĳ��ֵ
};

enum eChangeTarget
{
	eMinDistance=0, //�������
	eMinHP, //Ѫ�����
	eLvHP, //�ȼ����
	eMaxEnemy, //�����
	eNoChange, //������ѡ��
	eChangeMax,
};

struct sAttackBossInfo
{
	sAttackBossInfo() : vipLevel(0),playerID(0){}
	std::string name;
	int vipLevel;
	int64 playerID;
};

struct EnemyDamageInfo
{
	EnemyDamageInfo():time(0),value(0){}
	int64 time; //开始攻击时间
	int value;//伤害值
};

class AIBrain
{
public:
	AIBrain();
	~AIBrain();

	void Init(BaseAIInfo *baseAI, BetterAIInfo *betterAI);

	void Release();

	void SetOwner(Smart_Ptr<Monster> &obj);
	Smart_Ptr<Monster> GetOwner();

	//����
	void Update(DWORD dwDeltaTick);

	//��AI״̬
	bool ChangeState( eAIState eNewStateID );

	int GetCurState() {	 return m_curState; }

	//�ӳ���б�����õ�һ�������������
	Smart_Ptr<CreatureObj> GetMinLength();
	Smart_Ptr<CreatureObj> GetMinHP();
	Smart_Ptr<CreatureObj> GetMinLV();
	Smart_Ptr<CreatureObj> GetMaxEnemy();
	Smart_Ptr<CreatureObj> GetEnemy(DWORD key);
	Smart_Ptr<CreatureObj> GetRedNamePlayer();

	//��Ŀ��
	void ChangeTarget(int type, int rate);

	//��������
	bool CaseStrategy(int type, int param, int action, int rate);

	//�������ܲ���
	bool CaseStrategySkill(int type, int param, int action, int rate);

	//技能释放
	bool FireSkill(int rate);

	vector<Smart_Ptr<CreatureObj> > &  GetEnemyList();

	vector<Smart_Ptr<CreatureObj> > & getEnemyDataList(){return m_enemyList;}

	void AddEnemy(Smart_Ptr<CreatureObj> &obj);
	void DeleteEnemy(Smart_Ptr<CreatureObj> & player);
	void DeleteTargetEnemy(Smart_Ptr<CreatureObj> & player);
	bool IsExistedEnemy()
	{
		if(m_enemyList.size() == 0)
		{
			return false;
		}

		return true;
	}

	bool IsEnemy(Smart_Ptr<CreatureObj> &obj);

	//清除敌人和仇恨列表
	void ClearEnemy();

	//只清除敌人，不清楚仇恨伤害列表和首伤
	void ClearEnemyOnly();

	//更新仇恨值
	void ClearEnemyValue();
	void ClearDeadEnemy();
	void UpdateEnemyValue(int value, int64 key);

	//攻击了怪物的战姬
	void addGirlTarget(const Smart_Ptr<CreatureObj>& obj);
	void removeGirlTarget(const Smart_Ptr<CreatureObj>& obj);
	void clearGirlTarget();

	//得到最大掉落权重的玩家
	Smart_Ptr<CreatureObj> GetMaxWeight(Smart_Ptr<CreatureObj> &end);
	//获取指定数量的最大伤害值的玩家
	void GetDamageListByNum(vector<pair<int64, int> > &DamageList, int Num);
	//获取上一次死亡的时候指定数量的最大伤害值的玩家（世界BOSS）
//	void GetLastDamageListByNum(map<int64, sAttackBossInfo> &DamageMap, int Num);
	//更新路径
	void UpdatePath( int dwTick );
	void UpdatePath( const PlayerSynch::SingleMonsterPath * path );
	//行走
	void Move(  int dwTick );
	//寻路
	void FindPath( int dwTick );
	//寻路(绕过expos，eypos的点寻路)
	void FindPath( int xpos, int ypos, int expos, int eypos );
	void SetPath(std::vector<DWORD> &path);
	void SetPath(PlayerSynch::SynchMonsterBase * info);
	void ClearPathAndTime();

	void SetMonsterType(FieldMonsterType value)
	{
		m_monsterType = value;
	}
	FieldMonsterType GetMonsterType()
	{
		return m_monsterType;
	}

	void GetEnemyValue(map<int64, int>& enemyValue);

	bool GetPerfectLocation(int moveSpeed, int xpos, int ypos, int &txpos, int &typos);

	int GetAttackType()
	{
		if(!m_baseAI)
			return 0;

		return m_baseAI->atkType;
	}

	int GetViewDistance()
	{
		if(!m_baseAI)
			return 0;

		return m_baseAI->viewDistance;
	}

	int GetAttackDistance()
	{
		if(!m_baseAI)
			return 0;

		return m_baseAI->atkDistance;
	}

 int GetRunDistance()
  {
		if(!m_baseAI)
			return 0;

		return m_baseAI->runDistance;
  }

	BaseAIInfo *GetBaseAI()
	{
	 return m_baseAI;
	}

	BetterAIInfo *GetBetterAI()
	{
	 return m_betterAI;
	}

	bool IsBetterAI()
	{
		return (m_betterAI ? true : false);
	}

	//剩余刷新时间
	DWORD64 getLeftTime();

	//创建各个状态
	AIState * NewState(eAIState type, int id);

private:
	//检测伤害列表
	void CheckEnemyList(DWORD dwDeltaTick);

private:

	DWORD m_time;
	AIState * m_allState[eAIStateType_Max];
	eAIState m_curState;
	Smart_Ptr<Monster> m_owner;
	std::map<int,Smart_Ptr<CreatureObj> > m_GirlWarList;//哪些战姬选怪物为目标
	vector<Smart_Ptr<CreatureObj> > m_enemyList;
	map<int64,EnemyDamageInfo> m_enemyValue; //仇恨值,目前只存对玩家的,以后如果要怪物的仇恨就得加类型或者用新表
	const int m_clearEnemyTime; //仇恨清除时间
	DWORD m_checkEnemyTime;			 //仇恨检测时间
	int64 m_first; //首次攻击者,只针对玩家
	bool m_isPath; //是否已经请求寻路
	FieldMonsterType m_monsterType; //怪物寻路类型
	int m_pathTime; //请求寻路超时时间
	int m_curIndex; //当前第几步
	vector<DWORD> m_path; //所有的路径点
	int m_offset; //坐标偏差
	int m_curStay;//停留时间
	DWORD m_AjustTime; //调整时间
	BaseAIInfo *m_baseAI; //怪物基础ai
	BetterAIInfo *m_betterAI; //怪物高级ai
};


#endif /* AIBRAIN_H_ */
