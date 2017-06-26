/*
 * Monster.h
 *
 *  Created on: 2013��10��21��
 *      Author: root
 */

#ifndef MONSTER_H_
#define MONSTER_H_

#include "../Creature.h"
#include "../Object.h"
#include <vector>
#include <map>
#include "define.h"
#include "../../Map/Sector.h"
#include "Smart_Ptr.h"
#include "./MessageStruct/Map/SynchPath.pb.h"
#include "util.h"
#include "./Timer/TimerInterface.h"
#include "../../Attribute/AttributeCreature.h"

using namespace std;
using namespace CommBaseOut;

enum eAttackType
{
	eErrType=0, //����
	eActiveAtk=1, //主动攻击怪物
	ePassiveAtk, //被动攻击 怪物
	eDummyType	 // 木桩，稻草人，不动不打的怪
};

enum eMonsterQuality
{
	eCommonMonster = 1,
	eEliteMonster = 2,
	eBossMonster = 3,
	eGatherMonster = 4,			// 采集怪
	eNpcChangeToMonster = 5,		// 根据剧情刷新的怪
	eWorldMonster = 6,          // 世界BOSS
	eEscortCar	= 7,				// 镖车
	eCompeteMonster=8, //挑战怪
	eExpMonster=9, //经验怪
	eMoneyMonster=10, //铜钱怪
	eSealMonster=11, //兵符怪
	eGuardMonster = 12,//卫兵怪
	eElementMonster=13,//五行柱
	ePVPGatherMonster=14,//pvp火种
	eBoxMonster = 15, //天降宝箱
	eCountryBossMonster = 16, //阵营战boss
	eTowerMonster = 17,	//修罗塔
	eTowerNormalMonster = 18,//修罗塔小怪
	eTowerBossMonster = 19,//修罗塔BOSS
	eChairOrBed = 20,	//椅子 床
	eBoad 	= 21,				//船只
	eFestivalMonster  = 22,//节日怪物
	eBatterFlag = 23,	//战旗
	eChallengeBox = 24, //挑战宝箱
};

//竞技场怪物的角色属性
enum eCompeteMonsterAttr
{
	eCompeteProfession=0,//职业
	eCompeteSex,//性别
	eCompeteTitleID,//称号
	eCompeteMagic,//神兵
	eCompeteVipLv,//vip等级
	eCompeteWeapon,//主武器
	eCompeteAGuard,//护甲
	eCompeteFashion,//时装
	eCompeteHorse,//坐骑
	eCompeteClanJob,//帮派职务
	eCompeteEquipStreng,//强化星级
	eCompeteMax,
};

//死亡归属类型
enum eMonsterDeaBelongType
{
	eMonsterDeaBelongNull  = 0,
	eMonsterDeaBelongOnwer = 1, //拥用
	eMonsterDeaBelongOnece = 2, //打过一下
};

class BuffBase;
class Player;
class AIBrain;
struct sAttackBossInfo;

class Monster : public CreatureObj
{
public:
	Monster(eObjType type);
	~Monster();

	void Release();
	bool IsAliveTime();

	void SetDead(Smart_Ptr<CreatureObj> killer = NULL);

	void Reflush();

	void BeginIdle();

	void SetBornTime() {	 m_bornTime = CUtil::GetNowSecond(); }
	DWORD64 getBornTime() { return m_bornTime; }

	BYTE GetQuality() { 	return m_quality; }
	void SetQuality(BYTE bQua) { 	m_quality = bQua; }

	int GetFlushTime() {	 return m_flushTime; }
	void SetFlushTime(int time) { 	m_flushTime = time; }

	int GetAliveTime() { 	return m_aliveTime; }
	void SetAliveTime(int time) { 	m_aliveTime = time; }

	WORD GetRunSpeed() { 	return m_runSpeed; }
	void SetRunSpeed(WORD speed) { 	m_runSpeed = speed; }

	bool IsBetterAI();

	vector<int> GetFallID() { 	return m_fallID; }
	void SetFallID(vector<int> &id) { 	m_fallID = id; }

	int GetOriginalX() { 	return m_originalX; }
	void SetOriginalX(int pos) { 	m_originalX = pos; }

	int GetOriginalY() { 	return m_originalY; }
	void SetOriginalY(int pos) { 	m_originalY = pos; }

	int GetGatherTime() { 	return m_nGatherTime; }
	void SetGatherTime(DWORD time) { 	m_nGatherTime = time; }

	void SetOwnerToBrain(Smart_Ptr<Monster> &obj);

	void SetMoveSector(vector<Sector *> & sec) { 	m_moveSector = sec; }

	void SetStayTime(int time)
	{
		m_stayTime = time;
	}
	int GetStayTime() { 	return m_stayTime; }

	void SetPhyAttackTime(int time ) {		m_phyAtkTime = time;	}
	int GetPhyAttackTime() {	 return m_phyAtkTime; }

	bool GetEnemyList(vector<Smart_Ptr<CreatureObj> > & enemyList);

	bool IsActiveMonster();

	//木桩怪或者稻草人
	bool IsDummyMonster();

	void SetTarget(Smart_Ptr<CreatureObj> &obj);

	void UpdateEnemyList(Smart_Ptr<Player> &player);

	void ClearEnemyList();
	//更新仇恨值
	virtual void UpdateEnemyValue(int value, DWORD key);

	void Update(int dwTick);

	//ͬ�����ͻ��˵����
	void SetSynchData(PlayerSynch::SynchMonsterBase * info);
	void SetSynchData(PlayerSynch::SynchCompeteMonster * info);
	//ͬ��·�����ͻ���
	void SynchPath(PlayerSynch::MonsterPathToPlayer &path);

	//�Ƿ��ڹ�����Χ֮��
	bool IsAttackInLength(int distance);

	//�Ƿ�����Ұ��Χ֮��
	bool IsViewInLength(int xpos, int ypos);

	//�Ƿ���׷����Χ��
	bool IsInRunDistance(int distance);

	//目标位置距离自己出生点的距离
	bool IsInSelectView(int xpos, int ypos);

	//���﹥��
	void Attack(int skillid);

	//���ﱻ����
	virtual void Attacked(Smart_Ptr<CreatureObj> &obj);

	//����
	virtual void Dead(Smart_Ptr<CreatureObj> &obj);
	virtual bool IsDead();

	//处理死亡归属
	void dealWithDeadBelong(Smart_Ptr<CreatureObj> maxWeight);

	bool IsMonsterDead()
	{
		if(getAttManage()->getValue<int>(eCharHP) <= 0)
			return true;

		return false;
	}
	//当杀死 世界BOSS
	virtual void KillWorldMonsterDead();

	//���ܹ���Ŀ��
	void SkillAttack(int skillID);

	//buf
	virtual bool AddBuff(int id, Smart_Ptr<CreatureObj> &obj, int num=1);
	//移出buff不用删除buff
	virtual bool RemoveBuffByOnlyID(DWORD id);
	//移出buff并且结束buff
	virtual bool DeleteBuffByOnlyID(DWORD id);
	virtual	void removeBuffByGroupID(DWORD groupID);
	void RemoveCanBuff();

	//�ж��Ƿ���ԭ��
	bool IsOriginalPos()
	{
		if(GetXNewPos() == m_originalX && GetYNewPos() == m_originalY)
		{
			return true;
		}

		return false;
	}

	//��ȡĿ��
	Smart_Ptr<CreatureObj> & GetTargetObj()
	{
		return m_targetObj;
	}

	//获取到下一点的距离
	int  GetNextPosLen(int startx , int starty, int posx, int posy);
	//重新定向到某个位置
	virtual void RelocatePos(int xpos, int ypos, bool isSyn = false);
	int GetAIState();

	Smart_Ptr<AIBrain> & GetBrain();

	//获取属性值
	virtual int64 GetTypeValue(int type);

	//获取下个目标坐标点
	int GetNextPosition(int &xpos, int &ypos);

	//采集怪，开始采集
	bool StartGather(Smart_Ptr<Player> player);

	//采集完成
	bool EndGather(Smart_Ptr<Player> &player, int &error);

	//剧情任务的怪物
	void SetOwner(Smart_Ptr<Player> player);

	Smart_Ptr<Player> GetOwner();

	void SetOwnerName(string ownerName)
	{
		m_ownerName = ownerName;
	}

	string GetOwnerName()
	{
		return m_ownerName;
	}

	//
	 bool IsEscortCar() { return (m_quality == eEscortCar) ? true : false;}

	 //
	int GetEscortID()
	{
		if(m_quality == eEscortCar)
			return m_escortID;
		else
			return 0;
	}
	void SetEscortID(int value)
	{
		if(m_quality == eEscortCar)
			m_escortID = value;
	}

	//竞技场怪的技能
	void SetSkill(vector<int> & vec) {		m_skillList = vec; }
	vector<int> & GetSkill()
	{
		return m_skillList;
	}

	//初始化竞技场怪物的数据
	void SetCompeteData()
	{
		m_competeData.resize(eCompeteMax);
	}
	void SetCompeteData(int type, int value)
	{
		m_competeData[type] = value;
	}
	void SetClanName(string &name)
	{
		m_clanName = name;
	}

	void GetEnemyValue(map<int64, int>& enemyValue);

	void ClearEnemyValue();

	//获取最佳的目标位置(当前生物以当前点和速度去获取)
	virtual void GetPerfectLocation(int moveSpeed, int xpos, int ypos, int &txpos, int &typos);

	void ClearPathAndTime();
private:

	DWORD64 m_bornTime; // 出生时间
	BYTE m_quality; // 品质
	int m_flushTime; // 刷新时间
	int m_aliveTime; // 生存时间
	WORD m_runSpeed; // 追击速度
	vector<int> m_fallID; //掉落ID
	int m_stayTime;
	int m_phyAtkTime; // ���?�����
	int m_originalX; // 原始出生点
	int m_originalY; //原始出生点
	int m_escortID; //镖车ID

	DWORD m_nGatherTime;	//采集时间
	DWORD m_nGatherLastTime;	//采集持续时间
	Smart_Ptr<Player> m_nOwner;	//剧情任务刷新的怪属于某个玩家

	vector<Sector *> m_moveSector; // sectorid �� ԭʼ�㷶Χ�ڵ����и���

	Smart_Ptr<AIBrain> m_brain;
	Smart_Ptr<CreatureObj> m_targetObj; // Ŀ�����
	Smart_Ptr<Monster> m_myself; //�����Լ�������ָ�룬ָ���Լ�

	map<DWORD, BuffBase *> m_buffList; //怪物的buff连表
	vector<int> m_skillList; //竞技场怪的技能

	string m_ownerName; //所属着的名称

	//竞技场怪物数据
	vector<int> m_competeData;
	string m_clanName;
};


#endif /* MONSTER_H_ */
