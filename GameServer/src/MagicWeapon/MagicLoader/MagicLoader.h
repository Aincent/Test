/*
 * MagicLoader.h
 *
 *  Created on: 2014年8月8日
 *      Author: helh
 */

#ifndef MAGICLOADER_H_
#define MAGICLOADER_H_

#include "Singleton.h"
#include "define.h"
#include "CharDefine.h"

using namespace std;
using namespace CommBaseOut;

struct MagicAttrInfo
{
	int hp;
	//攻击
	int attack;
	//物防
	int pdefence;
	//命中
	int hitrate;
	//闪避
	int avoidrate;
	//爆击
	int crackrate;
	//抗暴
	int avoidcrack;

	MagicAttrInfo():hp(0),attack(0),pdefence(0),hitrate(0),avoidrate(0),crackrate(0),avoidcrack(0)
	{

	}
};

struct PassivePorbability
{
	PassivePorbability() : m_PassiveSkillID(0),m_Probability(0)
	{

	}
	int   m_PassiveSkillID;
	int   m_Probability;
};

struct MagicWeaponInfo
{
	int id;
	//品阶
	BYTE advance;
	//星级
	BYTE lv;
	//经验
	int exp;
	//加的
	MagicAttrInfo attr[eProfessionMax-1];
	//主动技能
	int skillid[eProfessionMax-1];
	//被动技能
	vector<PassivePorbability> m_PassaveProbabilityList;
	// 升级所需数量
	int upNeedNum;
	int m_NeedMoney;
	int m_LevelLimit;
	int m_ItemID;
	int m_MinExp;
	int m_MaxExp;

	MagicWeaponInfo():id(-1),advance(1),lv(0),exp(0), upNeedNum(0),m_NeedMoney(0),m_LevelLimit(0),m_ItemID(0),
			m_MinExp(0),m_MaxExp(0)
	{

	}

	int RandSkill(int quality,bool isByQuality);

	int randSkillByQuality(int quality);

	int randSkillNotByQuality();
};

struct MagicConfigData
{
	MagicConfigData() :m_Time(0){}
	int m_Time;
};

class MagicLoader : public Singleton<MagicLoader>
{
public:
	MagicLoader();

	~MagicLoader();

	int Init(string &file);

	MagicWeaponInfo * GetMagicWeapon(int id)
	{
		if(id > (int)m_magicInfo.size() || id <= 0)
		{
			return NULL;
		}

		return &m_magicInfo[id - 1];
	}

	bool IsFinalMagic(int id)
	{
		if(id >= (int)m_magicInfo.size())
			return true;

		return false;
	}

	int getMaxID();

	bool isMagicSkill(int skillID);

	int getCountDownTime(){return m_ConfigData.m_Time;}

protected:
	void loadPassive(string& file,map<DWORD,vector<PassivePorbability> >& dataList);

	void loaLua(string& file);

private:
	vector<MagicWeaponInfo> m_magicInfo;
	MagicConfigData 		m_ConfigData;
};


#endif /* MAGICLOADER_H_ */
