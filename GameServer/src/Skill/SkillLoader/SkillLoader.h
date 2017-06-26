/*
 * SkillLoader.h
 *
 *  Created on: 2014年1月15日
 *      Author: helh
 */

#ifndef SKILLLOADER_H_
#define SKILLLOADER_H_

#include <map>
#include "define.h"
#include "CharDefine.h"
#include "../../CDTime/CDMgr.h"

using namespace std;
using namespace CommBaseOut;

struct EffectParam;


// 技能 界面显示类型
enum ESkillInterfaceType
{
	// 主动技能
	eSkillInterface_ActiveSkill = 1,
	// 被动技能
	eSkillInterface_PassiveSkill = 2,
};

typedef struct sSkillInterfaceShow
{
	ECharProf prof;
	map<ESkillInterfaceType, vector<int> > skillIDByType;

}SkillInterfaceShow;

//技能第一类型
enum SkillFirstType
{
	//单体
	eSingleSkill=1,
	//群体
	eMoreSkill,
	//辅助
	eAssistSkill,
	//被动
	ePassiveSkill,
	eSkillTypeMax
};

//技能第二类型
enum SkillSecondType
{
	//瞬发技能
	eSkillTypeBlink=1,
	//持续技能
	eSkillTypeDelay,
	//嵌套技能
	eSkillTypeNest,
	//吟唱时间
	eSkillTypeIntonate,
};

//技能可释放对象
enum SkillTargetType
{
	eSkillTargetTypeNull = 0,
	eEnemyTarget=1,//敌人
	eFriendTarget,//朋友（自己）
	eMyself,//自己
	eNoPlayer=5, //非玩家
	eTargetTypeMax,
};

//技能效果范围
enum SkillRangeType
{
	//单个
	eRangeSingle=1,
	//自身范围
	eRangeMyself,
	//点选范围
	eRangePoint,
	// 扇形
	eRangeFan,
	//直线
	eRangeLine,
	//矩形
	eRectangle,

	eRangeMax
};

//效果类型
enum SKillEffectType
{
	//直接效果
	eEffectDirect=1,
	//buff
	eEffectBuff,
	eEffectMax
};

//效果子类型
enum EffectChildType
{
	//.减少对象当前HP
	eDscHP=1,
	//增加对象当前HP
	eAddHP,
	//增加对象当前MP
	eAddMP,
	//增加永久HP上限
	eAddHPMax,
	//增加永久MP上限
	eAddMPMax,
	//增加永久物理攻击值
	eAddPhyAttack,
	//增加永久法术攻击值
	eAddMagAttack,
	//增加永久速度值
	eAddSpeed=10,
	//增加永久物理防御值
	eAddPhyDefence,
	//增加永久法术防御值
	eAddMagDefence,
	//增加永久闪避值
	eAddAvoidRate,
	//增加永久暴击值
	eAddCrackRate,
	//增加永久命中值
	eAddHitRate,
	//暂时不用，之前击退
	eAttackBackNULL,
	//暂时不用，之前击飞
	eAttackFlyNULL,
	// 位移
	eMove,
	//永久增加抗暴
	eAddAvoidCrack,
	//复活主角
	eRevive,
	//吸血
	eSuck,
	//血量回复
	eBloodRecovery,
	//增加永久暴击率
	eAddPercentCrackRate = 23 ,
	//减技能CD
	eMinusCD = 24,
	//移除某组BUFF
	eRemoveBuffByGrupID = 25,
	//加属性
	eSkillAddAtt = 26,

	eEffectChildMax
};

//效果值计算类型
enum ValueCalculateType
{
	//自己定义公式
	eUserDefineFun=1,
	//百分比计算
	ePercent,
	//自定义固定脂
	eUserDefineValue,

	eValueTypeMax
};

typedef struct sSkillInfo
{
	int skillID;
	//职业限制
	int limitPro;
	//最低等级
	int minLv;
	//学习等级
	int learnLv;
	//学习金钱消耗
	int useMoney;
	//学习经验消耗
	int useExp;
	//学习所需真气
	int useForce;
	//学习帮贡消耗
	int useFactionCon;
	//技能当前等级
	int nowLv;
	//下一级的技能id
	int nextSkillID;
	//下一段动作技能id
	int nextActionSkill;
	//使用消耗hp
	int useHP;
	//使用消耗mp
	int useMP;
	//技能第一类型
	int fType;
	//技能第二类型
	int sType;
	//可释放目标的类型
	int targetType;
	//间隔时间
	int intervalTime;
	//技能存在时间
	int aliveTime;
	//飞行速度
	int flySpeed;
	//分段技能间隔超时时间
	int continueTime;
	 //频道类型
	int channelType;
	//冷却时间
	int coolTime;
	//释放距离
	int distance;
	//效果范围类型
	int rangeType;
	//范围大小
	int rangeLength;
	//宽度
	int rangeWidth;
	//目标数
	int targetCount;
	//效果类型
	int effectType[3];
	//效果子类型
	int effectFirstType[3];
	//释放几率
	int atkRate[3];
	//参数
	int param[3][3];
	SkillTargetType m_EffectTargetType[3];
	//技能品阶
	int quality;
	//处于控制状态，根据该配置判断是否可以释放一个技能
	vector<EAttackState> m_StatusCondition;
	//动作时间
	int m_ActionTime;
	//吟唱时间
	int m_IntonateTime;
	//技能书
	int skillBook;
	//升级数量
	int upgradeNum;
	//打断吟唱技能的状态
	vector<EAttackState> m_BreakStateList;
	//打断吟唱技能的行为
	vector<eObjectDoing> m_BreakDoingList;

	int m_TalentID;
	int m_Fight;

	sSkillInfo():skillID(-1),limitPro(0),minLv(0),learnLv(0),useMoney(0),useExp(0),useForce(0),useFactionCon(0),nowLv(0),
			nextSkillID(-1),nextActionSkill(0),useHP(0),useMP(0),fType(1),sType(0),targetType(1),intervalTime(0),aliveTime(0),
			flySpeed(0),continueTime(0),channelType(-1),coolTime(0),distance(1),rangeType(1),rangeLength(1),
			rangeWidth(0),targetCount(1),quality(0),m_ActionTime(0),m_IntonateTime(0),skillBook(0),upgradeNum(0),m_TalentID(0),m_Fight(0)
	{
		for(int i  =0; i < 3; ++i)
		{
			effectType[i] = 0;
			effectFirstType[i] = 0;
			atkRate[i] = 0;
			for(int j = 0; j < 3; ++j)
			{
				param[i][j] = 0;
			}
		}
	}
}SkillInfo;

//益害标识
enum EDamageType
{
	//无图标显示
	eNoDisplay=0,
	//有益的
	eUseful,
	//有害的
	eDamage,
};

//保存类型
enum ESaveType
{
	//不保存
	eNoSave=0,
	//冻结时间
	eIceTime,
	//不冻结时间
	eNoIceTime,
};

//效果类型
enum ERelsultType
{
	//没有效果
	eNoRes = 0,
	//改变血量
	eChangeHP,
	//比例改变血量
	eScaleChangeHP,
	//改变蓝量
	eChangeMP,
	//比例改变蓝量
	eScaleChangeMP,

	//改变hp上限
	eChangeHPMax = 11,
	//改变mp上限
	eChangeMPMax,
	//改变命中率
	eChangeHitRate,
	//改变闪避率
	eChangeAvdRate,
	//改变物理攻击
	eChangePhyAttack,
	//改变物理防御
	eChangePhyDefence,
	//改变法术攻击
	eChangeMagAttack,
	//改变物理防御
	eChangeMagDefence,
	//改变爆击
	eChangeCrack,
	//改变抗爆
	eChangeAvoidCrack,
	//打怪双倍经验
	eChangeDoubleExp,
	//改变移动速度
	eChangeMvSpeed,
	//攻击力
	eAttack  			= 23,
	//吸血
	eVampire 			= 24,
	//伤害百分比
	eSkillPercentHurt	= 25,
	//百分比护盾
	eAttPersentShield	= 26,
	//暴击率
	eEffectPercentCritRate  = 27,
	//闪避率
	eEffectPercentDodgeRate = 28,
	//百固定值护盾
	eAttFixedShield			= 29,
	//暴击回血
	eAttCritBackBlood		= 30,

	//僵直
	eStiff 				= 41,
	//冰冻
	eIce   				= 42,
	//晕眩
	eStun 				= 43,
	//免疫僵直
	eImmunityStiff 		= 44,
	//免疫冰冻
	eImmunityIce 		= 45,
	//免疫晕眩
	eImmunityStun 		= 46,
	//击退
	eRepel 				= 47,
	//无敌
	eInvincibleEffect 	= 48,
	//事件触发加Buff
	eTriggerAddBuff 	= 49,
	//免疫伤害护盾
	eImmuneHurtShield 	= 50,
	//必暴击状态
	eEffectMustBeCrit	= 51,
	//添加状态
	eAddState			= 52,
	//改变属性
	eAlterAtt			= 53,
	//反伤
	eBackHurtEffect		= 54,
	eRelTypeMax,
};

//buff类型
enum EBuffType
{
	//永久
	eBuffForever=0,
	//次数
	eBuffCount,
	//一次，时间限制
	eBuffOnce,
	// 间隔时间执行, 容量限制
	eBuffCapInvlTime,

	eBuffTypeMax
};

//效果数据源
enum eEffectDataSource
{
	eEffectNull		 = 0,	
	eEffectConst     = 1,	//常量
	eEffectSelfAtt   = 2, 	//自身属性
	eEffectTargetAtt = 3,   //目标属性
};

struct BuffParamBase
{
	BuffParamBase() : m_Type(eNoRes),m_Param(.0f){}
	virtual ~BuffParamBase(){}
	virtual	void getParams(vector<float>& params,EffectParam& param) const{}
	virtual int getFormulaID() const{return 0;}
	ERelsultType m_Type;
	float m_Param;
};

struct EffectOneParam
{
	EffectOneParam() : m_Type(eEffectConst){}
	eEffectDataSource m_Type;
    vector<float>	  m_ParamList;
};

struct BuffDamage : public BuffParamBase 
{
	BuffDamage() : m_FormularID(0){}
    virtual void getParams(vector<float>& params,EffectParam& param) const;
	virtual int getFormulaID() const{return m_FormularID;}
	int					   m_FormularID;
	vector<EffectOneParam> m_DataList;	
};

struct BuffInfo
{
	int id;
	//组别
	int groupID;
	//等级
	int lv;
	//是否有特效
	bool isResult;
	//益害
	BYTE isDamage;
	//持续时间
	int aliveTime;
	//轮询次数
	int timer;
	//释放几率
	int fireRate;
	//效果类型
	int useType;
	//参数
	BuffParamBase* m_pParam;
	//死亡是否清除
	bool isDel;
	//保存类型
	BYTE saveType;
	//参数2 （控制连斩BUFF更新）
	float params;
	// 是否叠加
	bool isAdd;
	//条件
	int m_Condition;
	//效果目标
	SkillTargetType m_TargetType;

	BuffInfo():id(-1),groupID(0),lv(0),isResult(false),isDamage(0),aliveTime(0),timer(0),fireRate(0),useType(0),m_pParam(NULL),
			isDel(true),saveType(0),params(0),isAdd(false),m_Condition(0),m_TargetType(eMyself)
	{

	}
};

struct SingleSkillData
{
	SingleSkillData() : m_SkillCommonCD(0){}
	int m_SkillCommonCD;
};

class SkillLoader
{
public:
	~SkillLoader();

	int Init(string &file);

	int InitSkill(string &file);

	int InitBuff(string &file);

	bool loadLua(const string& str);

	BuffParamBase* initEffectParam(const string& str,ERelsultType type);
	
	int InitSkillS(string& file);

	static SkillLoader * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new SkillLoader();
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

	SkillInfo * GetSkillInfoByID(int id)
	{
		map<int, SkillInfo>::iterator it = m_skillInfo.find(id);
		if(it != m_skillInfo.end())
		{
			return &it->second;
		}

		return 0;
	}

	void GetSkillInfoByID(int id, SkillInfo &info)
	{
		map<int, SkillInfo>::iterator it = m_skillInfo.find(id);
		if(it != m_skillInfo.end())
		{
			info = it->second;
		}
	}

	const BuffInfo* GetBuffInfoPtrByID(int id)
	{
		map<int, BuffInfo>::iterator it = m_buffInfo.find(id);
		if(it == m_buffInfo.end())
		{
			return NULL;
		}

		return &it->second;
	}

	//获得连斩BUFF
	void GetBuffIdByCount(int count, int &id)
	{
		int num = 0;
		int numMax = 0;
		int tempid = -1;

		map<int, BuffInfo>::iterator it = m_buffInfo.begin();
		for(;it!=m_buffInfo.end();it++)
		{
			if(it->second.groupID == 200 && count == it->second.timer)
			{
				id = it->second.id;
				return;
			}

			if(it->second.groupID== 200)
			{
				if(num == 0)
				{
					num = (int)it->second.params;
				}
				tempid = it ->second.id;
				numMax = it->second.timer;
			}

		}

		if(count > numMax && count%num == 0)
		{
			id = tempid;
		}
	}

	//是否是同个嵌套技能
	bool IsSameSkill(int id, int nextid)
	{
		map<int, SkillInfo>::iterator it = m_skillInfo.find(id);
		if(it != m_skillInfo.end())
		{
			if(it->second.nextActionSkill <= 0)
				return false;

			if(it->second.nextActionSkill == nextid)
				return true;
			else
				return IsSameSkill(it->second.nextActionSkill, nextid);
		}

		return false;
	}

	//是否是 神兵技能
	bool IsMagicSkill(int skillID)
	{
		map<int, SkillInfo>::iterator it = m_skillInfo.find(skillID);
		if(it != m_skillInfo.end())
		{
			if(it->second.limitPro == 4)
				return true;
			else
				return false;
		}
		return false;
	}

	const vector<DWORD>& getConditionList(){return m_ConditionList;}

	vector<eChannelType>* getCDChannelByProfession(ECharProf profession);

	const SingleSkillData& getSingleData(){return m_SingleData;}

	// 获取某职业某类型的id
	 void GetIDByProfAndType(ECharProf prof, ESkillInterfaceType type, vector<int>& skillID);

	 const map<ECharProf,vector<int> >& getBornSkill(){return m_BornSkill;}

private:
	SkillLoader();

	void setCDChannel(ECharProf proffesion,eChannelType type);

private:
	static SkillLoader* m_instance;
	map<int, SkillInfo> m_skillInfo;
	map<int, BuffInfo>  m_buffInfo;
	vector<DWORD>	    m_ConditionList;
	SingleSkillData		m_SingleData;
	map<ECharProf,vector<eChannelType> >  m_CDChannelList;	//某职业对应所有技能CD频道
	map<ECharProf, SkillInterfaceShow> m_skillInterface; // 某职业的某技能类型对应的技能ID
	map<ECharProf,vector<int> >	m_BornSkill;			 //出生时间的技能
};






#endif /* SKILLLOADER_H_ */
