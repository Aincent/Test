/*
 * QuestCondition.h
 *任务的条件基类
 *  Created on: 2014年10月20日
 *      Author: root
 */

#ifndef QUESTCONDITION_H_
#define QUESTCONDITION_H_

#include "QuestDefine.h"
#include "../EventSet/EventDefine.h"

class Quest;

//用于区分上线或领取创建的任务，在设置条件时用到
enum eOnLineOrAccept
{
	eOnLineOrAcceptNull = 0,
	eQuestOnline		= 1,
	eQuestAccept		= 2,
};

class QuestCondition
{
protected:
	Quest&			mQuest;		/// 拥有该条件的任务对象
	uint			mIndex;		/// 该条件在任务对象中的索引
	int				mValue;		/// 条件的值1
	bool			mIsActive;	/// 该条件是否是激活状态
	const CondCfg& 	mCfg;		/// 条件的配置信息

public:
	QuestCondition(Quest& quest, uint index);
	virtual ~QuestCondition();

public:
	int getValue() const			{ return mValue; }
	uint getIndex() const			{ return mIndex; }

	void setActive(bool setting);
	static QuestCondition* createCondition(Quest& quest, uint index);
	void initValue(int value){mValue = value;}

public:
	virtual uint getType() const	{ return QuestCondType_Unknow; }
	virtual void setValue(int val);
	virtual bool isFinished() const	{ return (mValue == 1); }
	virtual void setFinish(){setValue(1);}
	virtual void onActive()			{}
	//初始化数据
	virtual void initCondition(){mIsActive = true;onActive();}
	virtual void onDisactive()		{}
};

class QC_RemoveItem
	: public QuestCondition
{
public:
	QC_RemoveItem(Quest& quest, uint index);
	~QC_RemoveItem();

	virtual uint getType() const { return QuestCondType_RemoveItem; }
	virtual void onActive();
	virtual void onDisactive();
	virtual bool isFinished() const;
	virtual void setFinish();
private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class QC_MiningItem
	: public QuestCondition
{
public:
	QC_MiningItem(Quest& quest, uint index);
	~QC_MiningItem();

	virtual uint getType() const { return QuestCondType_Mining; }
	virtual void onActive();
	virtual void onDisactive();
	virtual bool isFinished() const;
	virtual void setFinish();
private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class QC_PlayerUpgrade
	: public QuestCondition
{
public:
	QC_PlayerUpgrade(Quest& quest, uint index);
	~QC_PlayerUpgrade();

	virtual uint getType() const	{ return QuestCondType_Upgrade; }
	virtual void onActive();
	virtual void onDisactive();
private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class QC_KillMonster
	: public QuestCondition
{
public:
	QC_KillMonster(Quest& quest, uint index);
	~QC_KillMonster();

	virtual uint getType() const	{ return QuestCondType_KillMonster; }
	virtual bool isFinished() const;
	virtual void setFinish();
	virtual void onActive();
	virtual void onDisactive();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class QC_KillNpcMonster
	: public QuestCondition
{
public:
	QC_KillNpcMonster(Quest& quest, uint index);
	~QC_KillNpcMonster();

	virtual uint getType() const	{ return QuestCondType_NpcFreMonster; }
	virtual bool isFinished() const;
	virtual void setFinish();
	virtual void onActive();
	virtual void onDisactive();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class QC_OwnItem
	: public QuestCondition
{
public:
	QC_OwnItem(Quest &quest, uint index);
	~QC_OwnItem();

	virtual uint getType() const {return QuestCondType_OwnItem;}
	virtual bool isFinished() const;
	virtual void setFinish();
	virtual void onActive();
	virtual void onDisactive();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class DirectFinish
	: public QuestCondition
{
public:
	DirectFinish(Quest& quest, uint index);
	~DirectFinish();

	virtual uint getType() const	{ return QuestCondType_DirectFinish; }
	virtual bool isFinished() const	{ return true; }
	virtual void onActive();

private:
	void OnTimer();
};

/// 合成
class QC_Compose
	: public QuestCondition
{
public:
	QC_Compose(Quest& quest, uint index);
	~QC_Compose();

	virtual uint getType() const	{ return QuestCondType_Compose; }
	virtual void onActive();
	virtual void onDisactive();
private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

/// 强化一件装备
class QC_StrongEquip
	: public QuestCondition
{
public:
	QC_StrongEquip(Quest& quest, uint index);
	~QC_StrongEquip();

	virtual uint getType() const	{ return QuestCondType_Strong; }
	virtual void onActive();
	virtual void onDisactive();
	virtual bool isFinished() const;
	virtual void setFinish();
private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

/// 洗练一下装备
class QC_SmeltEquip
	: public QuestCondition
{
public:
	QC_SmeltEquip(Quest& quest, uint index);
	~QC_SmeltEquip();

	virtual uint getType() const	{ return QuestCondType_Smelt; }
	virtual void onActive();
	virtual void onDisactive();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

/// 冲脉
class QC_Channel
	: public QuestCondition
{
public:
	QC_Channel(Quest& quest, uint index);
	~QC_Channel();

	virtual uint getType() const	{ return QuestCondType_Channel; }
	virtual void onActive();
	virtual void onDisactive();
	virtual bool isFinished() const;

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

/// 镶嵌装备
class QC_Recoin
	: public QuestCondition
{
public:
	QC_Recoin(Quest& quest, uint index);
	~QC_Recoin();

	virtual uint getType() const	{ return QuestCondType_Slot; }
	virtual void onActive();
	virtual void onDisactive();
	//virtual bool isFinished() const;

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class QC_EnterScene
	: public QuestCondition
{
public:
	QC_EnterScene(Quest &quest, uint index);
	~QC_EnterScene();

	virtual uint getType() const {return QuestCondType_EnterScene;}
	virtual void onActive();
	virtual void onDisactive();

private:
	EventConnection m_stopEvent;
	EventConnection m_OtherEvent;
	bool onEvent(const EventArgs & e);
	bool onOtherEvent(const EventArgs & e);
	void executeFun(int mapid);
};

class QC_BuyMallItem
	: public QuestCondition
{
public:
	QC_BuyMallItem(Quest& quest, uint index);
	~QC_BuyMallItem();

	virtual uint getType() const	{ return QuestCondType_BuyMallItem; }
	virtual bool isFinished() const;
	virtual void setFinish();
	virtual void onActive();
	virtual void onDisactive();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class QC_UseItem
	: public QuestCondition
{
public:
	QC_UseItem(Quest& quest, uint index);
	~QC_UseItem();

	virtual uint getType() const	{ return QuestCondType_UseItem; }
	virtual bool isFinished() const;
	virtual void setFinish();
	virtual void onActive();
	virtual void onDisactive();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class QC_Check
	: public QuestCondition
{
public:
	QC_Check(Quest& quest, uint index);
	~QC_Check();

	virtual uint getType() const	{ return QuestCondType_Check; }
	//virtual void setValue(int val);
	virtual void onActive();
	virtual void onDisactive();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class QC_Horse
	: public QuestCondition
{
public:
	QC_Horse(Quest& quest, uint index);
	~QC_Horse();

	virtual uint getType() const	{ return QuestCondType_House; }
	virtual void onActive();
	virtual void onDisactive();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class QC_Horse_Update
	: public QuestCondition
{
public:
	QC_Horse_Update(Quest& quest, uint index);
	~QC_Horse_Update();

	virtual uint getType() const	{ return QuestCondType_House_Update; }
	virtual void onActive();
	virtual void onDisactive();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class QC_Danlu_Update
	: public QuestCondition
{
public:
	QC_Danlu_Update(Quest& quest, uint index);
	~QC_Danlu_Update();

	virtual uint getType() const	{ return QuestCondType_Danlu_Update; }
	virtual void onActive();
	virtual void onDisactive();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class QC_Strong_Lv
	: public QuestCondition
{
public:
	QC_Strong_Lv(Quest& quest, uint index);
	~QC_Strong_Lv();

	virtual uint getType() const	{ return QuestCondType_Strong_LV; }
	virtual void onActive();
	virtual void onDisactive();

private:
	EventConnection m_stopEvent;
	EventConnection m_changeEvent; //换装触发
	EventConnection m_offEvent;
	EventConnection m_updateEvent;
	EventConnection m_inheritEvent;
	bool onEvent(const EventArgs & e);
	virtual bool isFinished() const;
};

class QC_Arena_Success
	: public QuestCondition
{
public:
	QC_Arena_Success(Quest& quest, uint index);
	~QC_Arena_Success();

	virtual uint getType() const	{ return QuestCondType_Arena_Success; }
	virtual void onActive();
	virtual void onDisactive();
	virtual bool isFinished() const;
	virtual void setFinish();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class QC_Stone_Num
	: public QuestCondition
{
public:
	QC_Stone_Num(Quest& quest, uint index);
	~QC_Stone_Num();

	virtual uint getType() const	{ return QuestCondType_Slot_Num; }
	virtual void onActive();
	virtual void onDisactive();
	virtual bool isFinished() const;
	virtual void setFinish();

private:
	EventConnection m_stopEvent;
	EventConnection m_updateEvent;
	EventConnection m_deleteEvent;
	bool onEvent(const EventArgs & e);
};

class QC_Smelt_Num
	: public QuestCondition
{
public:
	QC_Smelt_Num(Quest& quest, uint index);
	~QC_Smelt_Num();

	virtual uint getType() const	{ return QuestCondType_Smelt_Num; }
	virtual void onActive();
	virtual void onDisactive();
	virtual bool isFinished() const;
	virtual void setFinish();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class QC_MagicWeapon
	: public QuestCondition
{
public:
	QC_MagicWeapon(Quest& quest, uint index);
	~QC_MagicWeapon();

	virtual uint getType() const	{ return QuestCondType_MagicWeapon; }
	virtual void onActive();
	virtual void onDisactive();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

/////////////////////////////////////////////////
class QC_DanLu
	: public QuestCondition
{
public:
	QC_DanLu(Quest& quest, uint index);
	~QC_DanLu();

	virtual uint getType() const	{ return QuestCondType_Danlu; }
	virtual void onActive();
	virtual void onDisactive();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

//////////////////////////////////////////////////
class QC_SendMail
	: public QuestCondition
{
public:
	QC_SendMail(Quest& quest, uint index);
	~QC_SendMail();

	virtual uint getType() const	{ return QuestCondType_SendMail; }
	virtual void onActive();
	virtual void onDisactive();
	bool isFinished() const;
	virtual void setFinish();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

/*************签到*************/
class QC_DailySign : public QuestCondition
{
public:
	QC_DailySign(Quest& quest, uint index);
	virtual ~QC_DailySign(){};

	virtual uint getType() const	{ return QuestCondType_DailySign; }
	virtual void onActive();
	//初始化数据
	virtual void initCondition();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************七日礼包*************/
class QC_SevenPackage : public QuestCondition
{
public:
	QC_SevenPackage(Quest& quest, uint index);
	virtual ~QC_SevenPackage(){};

	virtual uint getType() const	{ return QuestCondType_SevenPackage; }
	virtual void onActive();
	//初始化数据
	virtual void initCondition();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************转盘*************/
class QC_Turntable: public QuestCondition
{
public:
	QC_Turntable(Quest& quest, uint index);
	virtual ~QC_Turntable(){};

	virtual uint getType() const	{ return QuestCondType_Turntable; }
	virtual void onActive();
	//初始化数据
	virtual void initCondition();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************在线礼包领取************/
class QC_OnLineGift: public QuestCondition
{
public:
	QC_OnLineGift(Quest& quest, uint index);
	virtual ~QC_OnLineGift(){};

	virtual uint getType() const	{return QuestCondType_OnLineGift;}
	virtual void onActive();
	//初始化数据
	virtual void initCondition();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************等级礼包领取************/
class QC_LevelGift: public QuestCondition
{
public:
	QC_LevelGift(Quest& quest, uint index);
	virtual ~QC_LevelGift(){};

	virtual uint getType() const {return QuestCondType_LevelGift;}
	virtual void onActive();
	//初始化数据
	virtual void initCondition();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************称号使用************/
class QC_UseTitle: public QuestCondition
{
public:
	QC_UseTitle(Quest& quest, uint index);
	virtual ~QC_UseTitle(){};

	virtual uint getType() const {return QuestCondType_UseTitle;}
	virtual void onActive();
	//初始化数据
	virtual void initCondition();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************技能升级************/
class QC_SkillUpgrade: public QuestCondition
{
public:
	QC_SkillUpgrade(Quest& quest, uint index);
	virtual ~QC_SkillUpgrade(){};

	virtual uint getType() const {return QuestCondType_SkillUpgrade;}
	virtual void onActive();
	//初始化数据
	virtual void initCondition();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************装备商店购买物品***********/
class QC_EquipSHop: public QuestCondition
{
public:
	QC_EquipSHop(Quest& quest, uint index);
	virtual ~QC_EquipSHop(){};

	virtual uint getType() const {return QuestCondType_EquipSHop;}
	virtual void onActive();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************装备继承************/
class QC_EquipInherit: public QuestCondition
{
public:
	QC_EquipInherit(Quest& quest, uint index);
	virtual ~QC_EquipInherit(){};

	virtual uint getType() const {return QuestCondType_EquipInherit;}
	virtual void onActive();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************活跃度领取************/
class QC_Activity: public QuestCondition
{
public:
	QC_Activity(Quest& quest, uint index);
	virtual ~QC_Activity(){};

	virtual uint getType() const {return QuestCondType_Activity;}
	virtual void onActive();
	//初始化数据
	virtual void initCondition();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************阵营捐献************/
class QC_CampContribute: public QuestCondition
{
public:
	QC_CampContribute(Quest& quest, uint index);
	virtual ~QC_CampContribute(){};

	virtual uint getType() const {return QuestCondType_CampContribute;}
	virtual void onActive();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************神兵升级到指定等级************/
class QC_MagicUpgrade : public QuestCondition
{
public:
	QC_MagicUpgrade(Quest& quest, uint index);
	virtual ~QC_MagicUpgrade(){};

	virtual uint getType() const {return QuestCondType_MagicUpgrade;}
	virtual void onActive();
	//初始化数据
	virtual void initCondition();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************神兵刷新技能************/
class QC_MagicRefresh: public QuestCondition
{
public:
	QC_MagicRefresh(Quest& quest, uint index);
	virtual ~QC_MagicRefresh(){};

	virtual uint getType() const {return QuestCondType_MagicRefresh;}
	virtual void onActive();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************加入队伍************/
class QC_CreateGroup: public QuestCondition
{
public:
	QC_CreateGroup(Quest& quest, uint index);
	virtual ~QC_CreateGroup(){};

	virtual uint getType() const {return QuestCondType_CreateGroup;}
	virtual void onActive();
	//初始化数据
	void initCondition();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************装备升级************/
class QC_EquipUpgrade : public QuestCondition
{
public:
	QC_EquipUpgrade(Quest& quest, uint index);
	virtual ~QC_EquipUpgrade(){};

	virtual uint getType() const {return QuestCondType_EquipUpgrade;}
	virtual void onActive();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************钱庄领钱************/
class QC_MoneyShop: public QuestCondition
{
public:
	QC_MoneyShop(Quest& quest, uint index);
	virtual ~QC_MoneyShop(){};

	virtual uint getType() const {return QuestCondType_MoneyShop;}
	virtual void onActive();
	//初始化数据
	virtual void initCondition();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************皇纲************/
class QC_Escort: public QuestCondition
{
public:
	QC_Escort(Quest& quest, uint index);
	virtual ~QC_Escort(){};

	virtual uint getType() const {return QuestCondType_HuangGang;}
	virtual void onActive();
	//初始化数据
	virtual void initCondition();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************战姬缠绵************/
class QC_GirlWarLingering: public QuestCondition
{
public:
	QC_GirlWarLingering(Quest& quest, uint index);
	virtual ~QC_GirlWarLingering(){};

	virtual uint getType() const {return QuestCondType_GirlWarLingering;}
	virtual void onActive();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************战姬升级************/
class QC_GirlWarUpgrade: public QuestCondition
{
public:
	QC_GirlWarUpgrade(Quest& quest, uint index);
	virtual ~QC_GirlWarUpgrade(){};

	virtual uint getType() const {return QuestCondType_GirlWarUpgrade;}
	virtual void onActive();
	//初始化数据
	virtual void initCondition();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************战姬合体************/
class QC_GirlWarFit: public QuestCondition
{
public:
	QC_GirlWarFit(Quest& quest, uint index);
	virtual ~QC_GirlWarFit(){};

	virtual uint getType() const {return QuestCondType_GirlWarFit;}
	virtual void onActive();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************穿戴官印************/
class QC_JobLucky: public QuestCondition
{
public:
	QC_JobLucky(Quest& quest, uint index);
	virtual ~QC_JobLucky(){};

	virtual uint getType() const {return QuestCondType_JobLucky;}
	virtual void onActive();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************攻击世界BOSS************/
class QC_AttackWorldBoss: public QuestCondition
{
public:
	QC_AttackWorldBoss(Quest& quest, uint index);
	virtual ~QC_AttackWorldBoss(){};

	virtual uint getType() const {return QuestCondType_WorldBoss;}
	virtual void onActive();
	virtual void onDisactive();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

/*************参加竞技场***********/
class QC_ParametersArena: public QuestCondition
{
public:
	QC_ParametersArena(Quest& quest, uint index);
	virtual ~QC_ParametersArena(){};

	virtual uint getType() const {return QuestCondType_ParametersArena;}
	virtual void onActive();
	virtual void onDisactive();
	virtual bool isFinished() const;
	virtual void setFinish();

private:
	bool onEvent(const EventArgs & e);

private:
	EventConnection m_Event;
};

class QC_EquipLv_Num
	: public QuestCondition
{
public:
	QC_EquipLv_Num(Quest& quest, uint index);
	~QC_EquipLv_Num();

	virtual uint getType() const	{ return QuestCondType_EquipLv_Num; }
	virtual void onActive();
	virtual void onDisactive();
	virtual bool isFinished() const;
	virtual void setFinish();

private:
	EventConnection m_stopEvent;
	EventConnection m_updateEvent;
	EventConnection m_offEvent;
	bool onEvent(const EventArgs & e);
};

class QC_JobLucky_Num
	: public QuestCondition
{
public:
	QC_JobLucky_Num(Quest& quest, uint index);
	~QC_JobLucky_Num();

	virtual uint getType() const	{ return QuestCondType_JobLucky_Lv; }
	virtual void onActive();
	virtual void onDisactive();
	virtual bool isFinished() const;
	virtual void setFinish();

private:
	EventConnection m_stopEvent;
	EventConnection m_unwearEvent;
	bool onEvent(const EventArgs & e);
};

class QC_MoneyMap_Success
	: public QuestCondition
{
public:
	QC_MoneyMap_Success(Quest& quest, uint index);
	~QC_MoneyMap_Success();

	virtual uint getType() const	{ return QuestCondType_MoneyMap_Success; }
	virtual void onActive();
	virtual void onDisactive();

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};

class QC_Hero_Tower	: public QuestCondition
{
public:
	QC_Hero_Tower(Quest& quest, uint index);
	~QC_Hero_Tower();

	virtual uint getType() const	{ return QuestCondType_RoleTower; }
	virtual void onActive();
	virtual void onDisactive();
	virtual void initCondition();
	virtual bool isFinished() const;

private:
	EventConnection m_stopEvent;
	bool onEvent(const EventArgs & e);
};


#endif /* QUESTCONDITION_H_ */
