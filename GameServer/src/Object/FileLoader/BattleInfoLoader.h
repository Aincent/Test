/*
 * BattleInfoLoader.h
 *
 *  Created on: 2013��10��26��
 *      Author: helh
 */

#ifndef BATTLEINFOLOADER_H_
#define BATTLEINFOLOADER_H_

#include <map>
#include "define.h"
#include "CharDefine.h"
#include "Lua/LuaInterface.h"

using namespace CommBaseOut;
using namespace std;

struct BuffParamBase;
struct EffectParam;

typedef struct sLvBattleAtt
{
	short int lv;
	int physicalpower;//体质
	int cleverAir;//灵气
	int cleverPower; //灵力
	int strength; //力量
	int patience;//耐久
	int agility; //敏捷
	int hitRate;//命中
	int avoidAttack;//闪避
	int lucky; //幸运值
	int crackDown;//爆击
	int avoidCrack; //抗爆
	sLvBattleAtt():lv(-1),physicalpower(0),cleverAir(0),cleverPower(0),strength(0),patience(0),agility(0),hitRate(0),avoidAttack(0),lucky(0),crackDown(0),avoidCrack(0)
	{

	}
}LvBattleAtt;

struct AttackDamageparam
{
	int aParam;
	int bParam;
	int cParam;
	int dParam;
	int eParam;
	int fParam;

	AttackDamageparam():aParam(0),bParam(0),cParam(0),dParam(0),eParam(0),fParam(0)
	{

	}
};

struct fAttrToSAttr
{
	int aParam;
	int bParam;
	int cParam;

	fAttrToSAttr():aParam(0),bParam(0),cParam(0)
	{

	}
};

//一级属性换算二级属性
enum AttrToSecondAttr
{
	eStrengthToAt=0,//力量算攻击
	eCleverPowerToMA, //灵力算法攻
	eCleverAirtoMP, //灵气算法力
	ePhyPowerToHP,//体质算HP
	ePatienceToPD,//耐久算物防
	eAgilityToMD,//敏捷算法防
	eAttrToMax,
};

//影响战斗力的属性
enum ChangeFightPower
{
	ePhyAttackToFP, //物攻
	eMagAttackToFP, //法攻
	ePhyDefenceToFP,//物防
	eMagDefenceToFP, //法防
	eHPToFP,//生命
	eMPToFP,//法力
	eHitRateToFP,//命中
	eAvoidToFP,//闪避
	eLuckyToFP,//幸运值
	eCrackToFP,//爆击
	eAvoidCrackToFP,//抗暴
	eToFPmax,
};

class BattleInfoLoader
{
public:
	~BattleInfoLoader();

	int Init(string &path);

	int InitBattleInfo(string &file);
	int InitBattleParam(string & file);

	void GetBattleByLvAndPro(int lv, int profession, LvBattleAtt &attr);
	int GetAllFight(int phyattack, int phydefence, int hp, int hit, int avoid, int crack, int avoidcrack);
	//一级书性换算的二级属性
	int GetSecondAttrByFirstAttr(int type, int value);
	//获取命中率
	float GetHitRate(int ahitrate,int bavoid);
	//获取物理攻击伤害
	int GetPhysicalDamage(int attack, int defense, int realDamage, int attackLevel,int defenseLevel,int ratio,int fightA,int fightB);
	//获取爆击律
	float GetCrackRate(int crack,int avoidcrack);

	//获得初始移动速度
	int getBaseMoveSpeed();

	static BattleInfoLoader * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new BattleInfoLoader();
		}

		return m_instance;
	}

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

	float getValueByFormula(const BuffParamBase& paramBase,EffectParam& param);

private:
	BattleInfoLoader();

private:
	static BattleInfoLoader * m_instance;
	map<int,string>	       m_Formula;
	map<WORD, LvBattleAtt> m_battleInfo[eProfessionMax-1];
	CLuaInterface m_lua;
};


#endif /* BATTLEINFOLOADER_H_ */
