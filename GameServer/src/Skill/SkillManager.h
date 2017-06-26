/*
 * SkillManager.h
 *
 *  Created on: 2014年1月20日
 *      Author: helh
 */

#ifndef SKILLMANAGER_H_
#define SKILLMANAGER_H_

#include <set>
#include <map>
#include "../Object/Creature.h"
#include "../Object/Object.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/CharLogin/CharBaseInfo.pb.h"
#include "MessageStruct/Map/SynchPath.pb.h"
#include "SkillBase.h"
#include "../EventSet/EventDefine.h"
#include "Log/Logger.h"
#include "MessageCommonRet.h"

using namespace std;
using namespace CommBaseOut;

struct BuffInitInfo
{
	//技能类型
	int type;
	//buff的id
	int id;
	//结束时间
	int64 endtime;
	//下线时间
	int64 offtime;
	//是否已经移除
	bool isDel;

	BuffInitInfo():type(-1),id(-1),endtime(0),offtime(0),isDel(true)
	{

	}
};

struct BuffGetPublicData
{
	BuffGetPublicData() : m_Type(eNoRes){}
	ERelsultType m_Type;
};

enum PkType
{
	sPKNULL = 0,
	//和平模式
	sPeace = 1,
	//全体模式
	sAll = 2,
	//善恶模式
	sGoodEvil = 3,
	//联盟模式(把联盟模式暂时用在帮派战中)
	sAlliance = 4,
	//国家模式
	sCountry = 5,
	//组队模式
	sTeam = 6,
};

enum GameSkillType
{
	//......
	//前面的枚举为乱七八糟的东西
	ClanSkillType_Player  = 8,
	ClanSkillType_GirlWar = 9,
};


struct OneTalentSkill
{
	OneTalentSkill() : m_TalentID(0){}
	int m_TalentID;
	vector<int> m_SkillList;
};

enum eUpSkillType
{
	eUpSkillOne = 0,
	eUpSkillAll = 1,
};

class BuffBase;
class Player;
class SkillMgr
{
public:
	SkillMgr(Player * obj);
	~SkillMgr();

	void ReInit();
	void ReleaseSkill();

	void GetClanSkills(std::vector<int>& skills);

	void GetSkillInfos(std::vector<int>& skills);

	void ReSetSkillInfos(const std::vector<int>& skills);
	
	bool IsExistedSkill(int skillid);

	bool IsExistedClanSkill(int skillid);

	bool isCurTalentSkill(int skillID);

	void initBornSkill();

	//是否存在buffer ID
	bool IsExistBufferID(int BufferID);

	//是否能释放此技能
	int IsFireSkill(int skillid = 0);

	//初始化技能
	//@todo :如果以后数值恢复有问题，就把永久buff不结束，或者先初始化永久buff，后初始其他buff
	void InitSkillInfo(const PlayerInfo::SkillInfo &info);

	//上线计算属性
	void onlineCalculateAtt();

	//计算一次战斗力
	int calculateFight();

	//获得指定技能增加的永久属性
	void getDesignation(const SkillInfo& info,map<ECharAttrType,int64>& attList);

	//初始化buff
	void InitBuff();

	//保存数据
	void SetSkillInfo(PlayerInfo::SkillInfo *info, bool isDel);

	void clientGetInfo(CharLogin::skillInfoPro* info);

	//竞技场同步到世界服的技能
	void SynchSkillInfoToWS(PlayerInfo::ArenaBaseInfo *skill);

	//被动释放或回收技能
	int AttackPassive(int skillid, bool isDel = false, bool isBroadCast = true);

	//被动技能加效果
	static void ResultEffect(CreatureObj* object,vector<AttrChange> &changeVec, vector<AttrChange> &changeMyself, int type,const int *param, bool isDel = false);

	static void ResultEffect(CreatureObj* object,const SkillInfo *info, bool isDel = false, bool isBroadCast = true);

	//�ж��Ƿ���Թ���
	int IsAttacked(Smart_Ptr<CreatureObj> &obj, int xpos, int ypos, int skillid = 0);

	//�жϾ����Ƿ����㹥�����뷶Χ
	bool IsInAttackDistance(int xpos, int ypos, int skillid = 0);

	int Attack(int xpos, int ypos , int key, int skillid);

	//第一次 释放 神兵
	int Attack_FirstMagicSkill(int xpos, int ypos , int key, int skillid);

	//无条件直接执行一个技能
	int Attack(int skillid);

	//同步信息
	void SetBuffInfo(PlayerSynch::SynchPlayerBase * info);

	//加buff
	bool addBuff(int id, Smart_Ptr<CreatureObj> & obj, int num = 1);

	//清除buffer
	void ClearBuffer();

	//移出buff不用删除buff
	bool RemoveBuffByOnlyID(DWORD id);

	//移出buff并且结束buff
	bool DeleteBuffByOnlyID(DWORD id);

	bool RemoveBuffByID(DWORD id);

	void removeBuffByGroupID(DWORD groupID);

	//删除死亡需要删除的buff
	void RemoveDeadBuff();

	ServerRet upgradeOneSkill(const SkillInfo& info,int& contribute,int& exp,int& money,int& force,map<int,short>& skillBooks);

	//技能升级
	int SkillUpgrade(int id,eUpSkillType type);

	//升级帮派技能
	int ClanSkillUpGrade(int id, int& newid);

	//获得等级>=lv的技能个数
	int GetspecifiedSkillLvCount(int lv);

	void AddSkill(CSkillBase * skill);

	void SetSkillNull(uint id);

	void SetBigSkill(CSkillBase * skill)
	{
		m_bigSkill = skill;
	}

	//玩家下线，需要清掉没有释放的技能
	 bool HandlePlayerOut(const EventArgs & e);

	 //结束连斩BUFF
	 void EndContinueBeheaded();

	 //获得为参数指定特效的BUFF，目前用于霸体
	 void getBuffByEffectType(vector<DWORD>& onlyIDList,const BuffGetPublicData& data);

	 //是否有一个技能等级大过参数指定等级
	 bool isHaveSkillLevelMoreThan(int level);

	 // 是否有技能能升级
	 bool IsHaveSkillUp(ESkillInterfaceType skillType); // skillType: true 主动技能 false 被动技能


	 void getSynData(PlayerSynch::SynchPlayerBase& data);

	 const vector<int>& getClanSkillID();

private:
	 // 技能是否达到基本升级条件 基本升级条件 元宝不参与计算
	 int IsCanUpgrade(int id);

private:
	Player *  m_owner;
	int m_TalentID;
	map<int,OneTalentSkill> m_SkillList;
	map<DWORD, BuffBase *> m_buffList; //Buff链
	map<uint,CSkillBase *> m_curSkillList;	//当前技能
	vector<BuffInitInfo> m_tBuffInit; //未初始化的buff

	vector<int>	m_clanSkills;	//帮派技能

	//CSkillBase * m_curSkill; //当前方得技能
	CSkillBase * m_bigSkill; //当前放的大技能
	EventConnection m_outEvent;//玩家下线
};



#endif /* SKILLMANAGER_H_ */
