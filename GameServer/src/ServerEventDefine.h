/*
 * ServerEventDefine.h
 *
 *  Created on: 2014年5月27日
 *      Author: helh
 */

#ifndef SERVEREVENTDEFINE_H_
#define SERVEREVENTDEFINE_H_

#include "./EventSet/EventArgs.h"
#include "Smart_Ptr.h"
#include "CharDefine.h"
#include "Object/Creature.h"
#include "Object/Player/Player.h"
#include "Object/Monster/Monster.h"
#include "Group/NewGroupMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "UIHLDefine.h"
#include "CharDefine.h"
#include "CharDefine.h"

using namespace CommBaseOut;

class MeditationEventArgs : public EventArgs
{
public:
	MeditationEventArgs(Smart_Ptr<Player> obj, int exp, int force):m_obj(obj),m_exp(exp),m_force(force)
	{}

	Smart_Ptr<Player> m_obj;
	int m_exp;
	int m_force;
};


class PlayerMapEventArgs: public EventArgs
{
public:

	PlayerMapEventArgs(Smart_Ptr<Player> obj):m_obj(obj)
	{	}

	Smart_Ptr<Player> m_obj;
};

class PlayerMoveEvent: public EventArgs
{
public:
	PlayerMoveEvent(int64 charID):m_charID(charID)
	{}
	int64   m_charID;
};

class PowerParmaEventArgs: public EventArgs
{
public:
	PowerParmaEventArgs(Smart_Ptr<CreatureObj> obj, int type):m_obj(obj),m_type(type)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int m_type;
};

class OneParmaEventArgs: public EventArgs
{
public:

	OneParmaEventArgs(Smart_Ptr<CreatureObj> obj):m_obj(obj)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
};

class TwoParmaEventArgs: public EventArgs
{
public:

	TwoParmaEventArgs(Smart_Ptr<CreatureObj> myself, Smart_Ptr<CreatureObj> other):
		m_myself(myself),m_other(other)
	{

	}

	Smart_Ptr<CreatureObj> m_myself;
	Smart_Ptr<CreatureObj> m_other;
};

// Event_PlayerLogin
class PlayerLoginEventArgs: public EventArgs
{
public:

	PlayerLoginEventArgs(Smart_Ptr<CreatureObj> obj,DWORD id):m_obj(obj),m_mapid(id)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	DWORD m_mapid;
};

// Event_PlayerLogout
class PlayerLogoutEventArgs: public EventArgs
{
public:

	PlayerLogoutEventArgs(Smart_Ptr<CreatureObj> obj):m_obj(obj)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
};

// Event_PlayerDead
class PlayerDeadEventArgs: public EventArgs
{
public:

	PlayerDeadEventArgs(Smart_Ptr<CreatureObj> obj):m_obj(obj)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
};


// Event_PlayerEnterBattleField
class PlayerEnterBattleFieldEventArgs: public EventArgs
{
public:

	PlayerEnterBattleFieldEventArgs(Smart_Ptr<CreatureObj> obj):m_obj(obj)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
};

// Event_PlayerLeaveBattleField
class PlayerLeaveBattleFieldEventArgs: public EventArgs
{
public:

	PlayerLeaveBattleFieldEventArgs(Smart_Ptr<CreatureObj> obj):m_obj(obj)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
};

// Event_PlayerEnterInstance
class PlayerEnterInstanceEventArgs: public EventArgs
{
public:

	PlayerEnterInstanceEventArgs(Smart_Ptr<CreatureObj> obj,int64 mapid):m_obj(obj),m_mapId(mapid)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int64		m_mapId;
};

// Event_PlayerLeaveInstance
class PlayerLeaveInstanceEventArgs: public EventArgs
{
public:

	PlayerLeaveInstanceEventArgs(Smart_Ptr<CreatureObj> obj,int64 mapid):m_obj(obj),m_mapId(mapid)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int64		m_mapId;
};

// Event_PlayerInstancePass
class PlayerInstancePassEventArgs: public EventArgs
{
public:

	PlayerInstancePassEventArgs(Smart_Ptr<CreatureObj> obj,int64 mapid):m_obj(obj),m_mapId(mapid)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int64		m_mapId;
};

// Event_PlayerUpgrade
class PlayerUpgradeEventArgs: public EventArgs
{
public:

	PlayerUpgradeEventArgs(Smart_Ptr<CreatureObj> obj,int lv):m_obj(obj),m_nLevel(lv)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int		m_nLevel;
};

// Event_PlayerLeaveScene
class PlayerLeaveSceneEventArgs: public EventArgs
{
public:

	PlayerLeaveSceneEventArgs(Smart_Ptr<CreatureObj> obj,int64 oldId,int tag):m_obj(obj),m_oldmapdId(oldId),m_nTag(tag)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int64		m_oldmapdId;
	int		m_nTag;
};

// Event_PlayerSceneChanged
class PlayerSceneChangedEventArgs: public EventArgs
{
public:

	PlayerSceneChangedEventArgs(Smart_Ptr<CreatureObj> obj,int64 oldId,int64 newId):m_obj(obj),m_oldmapdId(oldId),m_newmapId(newId)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int64		m_oldmapdId;
	int64		m_newmapId;
};

class GroupEventArgs: public EventArgs
{
public:

	GroupEventArgs(const Smart_Ptr<CreatureObj> obj):m_obj(obj)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
};

class PlayerAddPackSizeEventArgs: public EventArgs
{
public:

	PlayerAddPackSizeEventArgs(Smart_Ptr<CreatureObj> obj,DWORD lv):m_obj(obj),m_addLv(lv)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	DWORD		m_addLv;
};

class PlayerAcceptQuestEventArgs : public EventArgs
{
public:

	PlayerAcceptQuestEventArgs(Smart_Ptr<CreatureObj> obj,DWORD questid):m_obj(obj),m_questid(questid)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	DWORD		m_questid;
};

class MonsterKilledEventArgs : public EventArgs
{
public:

	MonsterKilledEventArgs(Smart_Ptr<Player> &killer, Smart_Ptr<Monster> &dead):m_killer(killer),m_dead(dead)
	{

	}

	Smart_Ptr<Player> m_killer;
	Smart_Ptr<Monster> m_dead;
};

class PlayerKilledEventArgs : public EventArgs
{
public:

	PlayerKilledEventArgs(Smart_Ptr<CreatureObj> &killer, Smart_Ptr<Player> &dead):m_killer(killer),m_dead(dead)
	{

	}

	Smart_Ptr<CreatureObj> m_killer;
	Smart_Ptr<Player> m_dead;
};

//生物被杀了
class CreatureKilledEventArgs: public EventArgs
{
public:

	CreatureKilledEventArgs(Smart_Ptr<CreatureObj> obj):m_obj(obj)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
};

class PlayerBeAttackArgs : public EventArgs
{
public:
	PlayerBeAttackArgs(const Smart_Ptr<Player> &self, Smart_Ptr<CreatureObj> &attack):m_self(self),m_attack(attack)
	{
	}
	Smart_Ptr<Player> m_self;
	Smart_Ptr<CreatureObj> m_attack;
};

class PlayerRemoveMemberEventArgs : public EventArgs
{
public:

	PlayerRemoveMemberEventArgs(Smart_Ptr<Player> &pObj,RemoveMethod method):m_pObj(pObj),m_eMethod(method),m_nkicker(0),m_pReason(NULL)
	{

	}

	Smart_Ptr<Player> m_pObj;
	RemoveMethod m_eMethod ;
	DWORD64 m_nkicker;
	char* m_pReason;
};

class ClearMapInstancedDataEventArgs : public EventArgs
{
public:
	ClearMapInstancedDataEventArgs(Smart_Ptr<Player> &pObj):m_pObj(pObj)
	{

	}

	Smart_Ptr<Player> m_pObj;
};

class BuyItemEventArgs : public EventArgs
{
public:
	BuyItemEventArgs(Smart_Ptr<Player> &pObj, DWORD id, DWORD num,DWORD m_type):m_pObj(pObj),m_id(id),m_num(num),m_type(m_type)
	{

	}

	Smart_Ptr<Player> m_pObj;
	DWORD m_id;
	DWORD m_num;
	DWORD m_type;//购买的物品类型
};

class UseItemEventArgs : public EventArgs
{
public:
	UseItemEventArgs(Smart_Ptr<Player> &pObj, DWORD id, DWORD num):m_pObj(pObj),m_id(id),m_num(num)
	{

	}

	Smart_Ptr<Player> m_pObj;
	DWORD m_id;
	DWORD m_num;
};


class GetPetEventArgs : public EventArgs
{
public:
	GetPetEventArgs(Smart_Ptr<Player> &pObj, int type, int value,int arg):m_pObj(pObj),m_type(type),m_value(value),m_arg(arg)
	{

	}

	Smart_Ptr<Player> m_pObj;
	int m_type;
	int m_value;
	int m_arg;
};

class ComposeArgs: public EventArgs
{
public:

	ComposeArgs(Smart_Ptr<CreatureObj> obj,int itemQuality):m_obj(obj),m_nItemQuality(itemQuality)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int m_nItemQuality;//合成后的物品品质
};

class StrengthEquipArgs: public EventArgs
{
public:

	StrengthEquipArgs(Smart_Ptr<CreatureObj> obj,int equipUpgrade):m_obj(obj),m_nEquipUpgrade(equipUpgrade)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int m_nEquipUpgrade;
};

class ExchangeJobluckyArgs: public EventArgs
{
public:

	ExchangeJobluckyArgs(Smart_Ptr<CreatureObj> obj,int quality,int lv):m_obj(obj),m_nQuality(quality),m_nLv(lv)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int m_nQuality;
	int m_nLv;
};

class GenenateEscortArgs: public EventArgs
{
public:

	GenenateEscortArgs(Smart_Ptr<CreatureObj> obj,int quality):m_obj(obj),m_nQuality(quality)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int m_nQuality;
};

class TurnLuckyTableArgs: public EventArgs
{
public:

	TurnLuckyTableArgs(Smart_Ptr<CreatureObj> obj,int num):m_obj(obj),m_nNum(num)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int m_nNum;
};

class ClanWarArgs: public EventArgs
{
public:

	ClanWarArgs(Smart_Ptr<CreatureObj> obj,int isWin):m_obj(obj),m_bWin(isWin)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int m_bWin;
};

class SkillUpgradeArgs: public EventArgs
{
public:

	SkillUpgradeArgs(Smart_Ptr<CreatureObj> obj,int lv,DWORD skillId):m_obj(obj),m_nLv(lv),m_nSkillId(skillId)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int m_nLv;
	DWORD m_nSkillId;
};

class WearEquipArgs: public EventArgs
{
public:

	WearEquipArgs(Smart_Ptr<CreatureObj> obj,int eQuipId):m_obj(obj),m_nEquipId(eQuipId)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int m_nEquipId;
};

class ReflushMagicWeaponSkillArgs: public EventArgs
{
public:

	ReflushMagicWeaponSkillArgs(Smart_Ptr<CreatureObj> obj,int id):m_obj(obj),m_nSkillId(id)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int m_nSkillId;
};

class SmeltArgs: public EventArgs
{
public:

	SmeltArgs(Smart_Ptr<CreatureObj> obj,int attrCount):m_obj(obj),m_nLegendAttrCount(attrCount)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int m_nLegendAttrCount;
};

class ClanUpgradeArgs: public EventArgs
{
public:

	ClanUpgradeArgs(Smart_Ptr<CreatureObj> obj,int clanLv):m_obj(obj),m_nClanLv(clanLv)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	int m_nClanLv;
};

class OneKeyCompleteArgs: public EventArgs
{
public:

	OneKeyCompleteArgs(Smart_Ptr<CreatureObj> obj,int questID,int times):m_obj(obj),m_eQuestID(questID),m_nTimes(times)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	//任务类型
	int m_eQuestID;
	//完成次数
	int m_nTimes;
};

//玩家攻击某个生物
class PlayerAttackCreature: public EventArgs
{
public:

	PlayerAttackCreature(Smart_Ptr<Player> player,Smart_Ptr<CreatureObj> creature, int key,int value):m_player(player),m_Creature(creature),m_key(key),m_value(value)
	{
	}
	Smart_Ptr<Player> m_player;
	Smart_Ptr<CreatureObj> m_Creature;
	int m_key; //受到伤害的生物的key
	int m_value;//所收到的伤害
};

class PlayerLeaveClanArgs: public EventArgs
{
public:

	PlayerLeaveClanArgs(Smart_Ptr<CreatureObj> obj,DWORD originalClanID):m_obj(obj),m_originalClanID(originalClanID)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	DWORD m_originalClanID;//原来的公会ID
};

class PlayerClanDonationArgs: public EventArgs
{
public:

	PlayerClanDonationArgs(Smart_Ptr<CreatureObj> obj,DWORD donationMoney):m_obj(obj),m_donationMoney(donationMoney)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	DWORD m_donationMoney;//捐献的金钱
};

class PlayerDialySignin: public EventArgs
{
public:

	PlayerDialySignin(Smart_Ptr<CreatureObj> obj,DWORD donationMoney):m_obj(obj),m_donationMoney(donationMoney)
	{

	}
	Smart_Ptr<CreatureObj> m_obj;
	DWORD m_donationMoney;//捐献的金钱
};

class PlayerSevenPackage : public EventArgs
{
public:
	PlayerSevenPackage(Smart_Ptr<CreatureObj> obj,DWORD id):m_obj(obj),m_ID(id){}
	Smart_Ptr<CreatureObj> m_obj;
	int 				   m_ID;
};

class PlayerLevelGift : public EventArgs
{
public:
	PlayerLevelGift(Smart_Ptr<CreatureObj> obj,DWORD id):m_obj(obj),m_ID(id){}
	Smart_Ptr<CreatureObj> m_obj;
	int 				   m_ID;
};

class PlayerUseTitle : public EventArgs
{
public:
	PlayerUseTitle(Smart_Ptr<CreatureObj> obj,DWORD id):m_obj(obj),m_ID(id){}
	Smart_Ptr<CreatureObj> m_obj;
	int 				   m_ID;
};

class PlayerReceiveActivity : public EventArgs
{
public:
	PlayerReceiveActivity(Smart_Ptr<CreatureObj> obj,DWORD id):m_obj(obj),m_ID(id){}
	Smart_Ptr<CreatureObj> m_obj;
	int 				   m_ID;
};

class PlayerCampContribute : public EventArgs
{
public:
	PlayerCampContribute(Smart_Ptr<CreatureObj> obj,int value):m_obj(obj),m_Value(value){}
	Smart_Ptr<CreatureObj> m_obj;
	int 				   m_Value;
};

class PlayerGirlwarUpgrade : public EventArgs
{
public:
	PlayerGirlwarUpgrade(Smart_Ptr<CreatureObj> obj):m_obj(obj){}
	Smart_Ptr<CreatureObj> m_obj;
};

class PlayerGirlwarFight : public EventArgs
{
public:
	PlayerGirlwarFight(Smart_Ptr<CreatureObj> obj):m_obj(obj){}
	Smart_Ptr<CreatureObj> m_obj;
};

class PlayerJobLuckly: public EventArgs
{
public:
	PlayerJobLuckly(Smart_Ptr<CreatureObj> obj,BYTE quality):m_obj(obj),m_Quality(quality){}
	Smart_Ptr<CreatureObj> m_obj;
	BYTE				   m_Quality;
};

class PlayerRaidsDuplicate: public EventArgs
{
public:
	PlayerRaidsDuplicate(Smart_Ptr<Player> player,int mapID):m_Player(player),m_MapID(mapID){}
	Smart_Ptr<Player> m_Player;
	int				  m_MapID;
};

class MonsterReflushArgs : public EventArgs
{
public:
	MonsterReflushArgs(Smart_Ptr<Monster> monster):m_Monster(monster)
	{

	}
	Smart_Ptr<Monster> m_Monster;
};

class MonsterBeAttackArgs : public EventArgs
{
public:
	MonsterBeAttackArgs(const Smart_Ptr<Monster> &self, Smart_Ptr<CreatureObj> &attack):m_self(self),m_attack(attack)
	{

	}
	Smart_Ptr<Monster> m_self;
	Smart_Ptr<CreatureObj> m_attack;
};

class BetAttackArgs : public EventArgs
{
public:
	BetAttackArgs(const Smart_Ptr<GirlWar> &self, Smart_Ptr<CreatureObj> &other,int value):m_Self(self),m_Other(other),m_Value(value)
	{

	}
	Smart_Ptr<GirlWar>     m_Self;
	Smart_Ptr<CreatureObj> m_Other;
	int 				   m_Value;
};

class PlayerProtectKingPassMaxArgs : public EventArgs
{
public:
	PlayerProtectKingPassMaxArgs(const Smart_Ptr<CreatureObj>& obj, int node) : m_obj(obj), m_node(node)
	{

	}

	Smart_Ptr<CreatureObj> m_obj;
	int m_node;
};
//
////通货类的日志铜钱，声望，真气等等
//class StatisticCurrencyChangeArgs : public EventArgs
//{
//public:
//	StatisticCurrencyChangeArgs(int64 charID,eStatisticMainType mainType,eLogicRelevant minorType,google::protobuf::Message *content,int64 objectID = 0):
//		m_charID(charID),m_mainType(mainType),m_minorType(minorType),m_pContent(content),m_objectID(objectID)
//	{
//
//	}
//
//	int64 m_charID;
//	eStatisticMainType m_mainType;
//	eLogicRelevant m_minorType;
//	google::protobuf::Message * m_pContent;
//	int64 m_objectID;
//};
//
////元宝统计
//class StatisticGoldenChangeArgs : public EventArgs
//{
//public:
//	StatisticGoldenChangeArgs(Smart_Ptr<CreatureObj> obj,eStatisticMainType mainType,eLogicRelevant minorType,DWORD num, DWORD cost):
//		m_obj(obj),m_mainType(mainType),m_minorType(minorType),m_num(num),m_cost(cost)
//	{
//
//	}
//
//	Smart_Ptr<CreatureObj> m_obj;
//	eStatisticMainType m_mainType;
//	eLogicRelevant m_minorType;
//	//次数
//	DWORD m_num;
//	DWORD m_cost;
//};

// 更新UI高亮数据
class UpdateUIHLDataArgs : public EventArgs
{
public :
	struct Content
	{
		E_UIHL_HANDLE handle;
		E_UIHL_BIGTYPE bigType;
		E_UIHL_SMALLTYPE smallType;
		int fValue;
		int sValue;
		bool flag;
		Content() : handle(eUIHLHandle_Null), bigType(eUIHLBig_Null), smallType(eUIHLSmall_Null), fValue(0), sValue(0), flag(false)
		{
		}
	};

	UpdateUIHLDataArgs(Smart_Ptr<CreatureObj> obj) : m_obj(obj)
	{
		m_content.clear();
	}
	UpdateUIHLDataArgs(Smart_Ptr<CreatureObj> obj, vector<Content>& data) : m_obj(obj), m_content(data)
	{
	}
	UpdateUIHLDataArgs(Smart_Ptr<CreatureObj> obj, vector<E_UIHL_HANDLE> handle, vector<E_UIHL_BIGTYPE> bigType,
			vector<E_UIHL_SMALLTYPE> smallType, vector<int> value, vector<bool> flag) : m_obj(obj)
	{
		if (handle.size() == bigType.size() && bigType.size() == smallType.size() && smallType.size() == value.size() &&
				value.size() == flag.size())
		{
			for (uint i = 0; i < handle.size(); ++i)
			{
				Content content;
				content.handle = handle[i];
				content.bigType =  bigType[i];
				content.smallType = smallType[i];
				content.fValue = value[i];
				content.flag = flag[i];

				m_content.push_back(content);
			}
		}
	}
	UpdateUIHLDataArgs(Smart_Ptr<CreatureObj> obj, vector<E_UIHL_HANDLE> handle, vector<E_UIHL_BIGTYPE> bigType,
			vector<E_UIHL_SMALLTYPE> smallType, vector<int> fValue, vector<bool> flag, vector<int>sValue) : m_obj(obj)
	{
		if (handle.size() == bigType.size() && bigType.size() == smallType.size() && smallType.size() == fValue.size() &&
				fValue.size() == flag.size() && flag.size() == sValue.size())
		{
			for (uint i = 0; i < handle.size(); ++i)
			{
				Content content;
				content.handle = handle[i];
				content.bigType =  bigType[i];
				content.smallType = smallType[i];
				content.fValue = fValue[i];
				content.sValue = sValue[i];
				content.flag = flag[i];

				m_content.push_back(content);
			}
		}
	}

	void AddContent(E_UIHL_HANDLE handle, E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, int value, bool flag)
	{
		Content data;
		data.handle = handle;
		data.bigType = bigType;
		data.smallType = smallType;
		data.fValue = value;
		data.flag = flag;

		m_content.push_back(data);
	}
	void AddContent(E_UIHL_HANDLE handle, E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType,
			int fValue, bool flag, int sValue)
	{
		Content data;
		data.handle = handle;
		data.bigType = bigType;
		data.smallType = smallType;
		data.fValue = fValue;
		data.sValue = sValue;
		data.flag = flag;

		m_content.push_back(data);
	}

	Smart_Ptr<CreatureObj> m_obj;
	vector<Content> m_content;
};


class HurtArgs : public EventArgs
{
public:
	HurtArgs(Smart_Ptr<CreatureObj> owner,Smart_Ptr<CreatureObj> target) : m_Owner(owner),m_Target(target){}
	Smart_Ptr<CreatureObj> m_Owner;
	Smart_Ptr<CreatureObj> m_Target;
};

class BeHurtArgs : public EventArgs
{
public:
	BeHurtArgs(Smart_Ptr<CreatureObj> owner,Smart_Ptr<CreatureObj> target) : m_Owner(owner),m_Target(target){}

	Smart_Ptr<CreatureObj> m_Owner;
	Smart_Ptr<CreatureObj> m_Target;
};


class BeforeBeHurtArgs : public EventArgs
{
public:
	BeforeBeHurtArgs(Smart_Ptr<CreatureObj> owner,Smart_Ptr<CreatureObj> target,int damageValue) : m_Owner(owner),m_Target(target),
	m_DamageValue(damageValue){}

	Smart_Ptr<CreatureObj> m_Owner;
	Smart_Ptr<CreatureObj> m_Target;
	mutable int 		   m_DamageValue;
};

class ArenaOverArgs : public EventArgs
{
public:
	ArenaOverArgs(Smart_Ptr<Player> owner, bool result) : m_Owner(owner),m_result(result){}

	Smart_Ptr<Player> m_Owner;
	bool m_result;
};


//进入某种状态
class ObjectEnterStateArgs : public EventArgs
{
public:
	ObjectEnterStateArgs(CreatureObj& owner,ECreatureState state) : m_Owner(owner),m_State(state){}
	CreatureObj& 	m_Owner;
	ECreatureState	m_State;
};

//执行某种行为
class ObjectDoingArgs : public EventArgs
{
public:
	ObjectDoingArgs(CreatureObj& owner,eObjectDoing doing) : m_Owner(owner),m_Doing(doing){}
	CreatureObj& m_Owner;
	eObjectDoing m_Doing;
};

class MonsterBeKillBelongArgs: public EventArgs
{
public:

	MonsterBeKillBelongArgs(Smart_Ptr<CreatureObj> myself, Smart_Ptr<CreatureObj> other,eMonsterDeaBelongType type):
		m_myself(myself),m_other(other),m_Type(type)
	{

	}

	Smart_Ptr<CreatureObj> m_myself;
	Smart_Ptr<CreatureObj> m_other;
	eMonsterDeaBelongType  m_Type;
};

class AfterBeHurtArgs : public EventArgs
{
public:
	AfterBeHurtArgs(Smart_Ptr<CreatureObj> owner,Smart_Ptr<CreatureObj> target,int damageValue) : m_Owner(owner),m_Target(target),
	m_DamageValue(damageValue){}

	mutable Smart_Ptr<CreatureObj> m_Owner;
	mutable Smart_Ptr<CreatureObj> m_Target;
	int m_DamageValue;
};

class ResetAllInstArgs : public EventArgs
{
public:
	ResetAllInstArgs(Smart_Ptr<CreatureObj> __owner, int __instType) : m_owner(__owner), m_instType(__instType)
	{ }

	Smart_Ptr<CreatureObj> m_owner;
	int m_instType;
};

class ArenaAddChallArgs : public EventArgs
{
public:
	ArenaAddChallArgs(Smart_Ptr<CreatureObj> __owner, int __addNum) : m_owner(__owner), m_addNum(__addNum)
	{

	}

	Smart_Ptr<CreatureObj> m_owner;
	int m_addNum;
};

class PassTargetArgs : public EventArgs
{
public:
	PassTargetArgs(Smart_Ptr<Player> __owner, int __targetID) : m_owner(__owner), m_targetID(__targetID)
	{
	}

	Smart_Ptr<Player> m_owner;
	int m_targetID;
};

class DrawTargetArgs : public EventArgs
{
public:
	DrawTargetArgs(Smart_Ptr<Player> __owner, int __targetID) : m_owner(__owner), m_targetID(__targetID)
	{
	}

	Smart_Ptr<Player> m_owner;
	int m_targetID;
};

const int ADD_SIN_EVENT_TYPE = 1000;
const int REDUCE_SIN_EVENT_TYPE = 1001;
const int ADD_HATRED_EVENT_TYPE = 1002;
const int PLAYER_CHANGSCENCE_EVENT_TYPE = 1003;						/// 玩家切换场景
const int CREATURE_DEAD_EVENT_TYPE	= 1004;								/// 生物死亡
const int PLAYER_LOGIN_EVENT_TYPE = 1005;								/// 玩家登录
const int PLAYER_LOGOUT_EVENT_TYPE = 1006;							/// 玩家登出
const int PLAYER_ENTERBATTLEFIELD_EVENT_TYPE = 1007;				/// 玩家进入战场
const int PLAYER_LEAVEBATTLEFIELD_EVENT_TYPE = 1008;				/// 玩家退出战场
const int PLAYER_ENTERINSTANCE_EVENT_TYPE = 1009;					/// 玩家进入副本
const int PLAYER_LEAVEINSTANCE_EVENT_TYPE = 1010;					/// 玩家退出副本
const int PLAYER_ADDGROUP_EVENT_TYPE = 1011;							/// 玩家加入队伍
const int PLAYER_LEAVEGROUP_EVENT_TYPE = 1012;						/// 玩家离开队伍

const int PLAYER_LEVEL_UP_EVENT_TYPE = 1013;  //玩家升级
const int PLAYER_UPGRADEVIPLV_EVENT_TYPE = 1014;  //玩家 VIP升级
const int MONSTER_KILLED_EVENT_TYPE = 1015;  //怪物被人杀了
const int PLAYER_CLAN_ENTERCLAN				= 1016;		//加入帮派
const int PLAYER_CLAN_LEAVECLAN				= 1017;  //离开帮派
const int PLAYER_QUEST_ACCEPTQUEST			= 1018;	//接受主线任务
const int PLAYER_QUEST_ACCEPTCLANQUEST = 1019;	//接受帮派任务
const int PLAYER_QUEST_SUBMITQUEST		= 1020;		//提交任务
const int PlAYER_LOGINOUT_CLEAR_EVENT_TYPE = 1021; //组队移除成员
const int MAPINSTANCED_CLEARMAPINSTANCEDATA_EVENT_TYPE = 1022; //清除副本信息
const int PLAYER_BUYITEM_FORMSHOP = 1023;	//从商店购买物品
const int PLAYER_BUYITEM_FROMMALL = 1024;	//从商城购买物品
const int PLAYER_ITEM_USEITEM = 1025;				//使用物品
const int PLAYER_SMELT_COMPOSE = 1026;			//合成
const int PLAYER_SMELT_INSERT = 1027;				//镶嵌
const int PLAYER_SMELT_SMELT = 1028;				//洗练
const int PLAYER_SMELT_STRONG = 1029;				//强化
const int PLAYER_CHANNEL_UPDATE = 1030;		//经脉
const int PLAYER_HORSE_UPDATE = 1031;				//坐骑
const int PLAYER_WEAPON_MAGICWEAPON = 1032;	//神兵
const int PLAYER_MERIDIAN_UPDATE = 1033;			//丹道
const int PLAYER_PACKET_ADDITEM = 1034;			//往背包添加物品
const int PLAYER_POSITION_TOPOSITION = 1035;	//到达指定位置
const int PLAYER_ENTERMAP_EVENT_TYPE = 1036;		//进入某张地图
const int PLAYER_SIGNIN_EVENT_TYPE = 1037;	//玩家签到
const int PLAYER_PASS_INSTANCE = 1038;//通关副本
const int PLAYER_REFRESH_MAGICWEAPON_SKILL = 1039;//刷新神兵技能
const int PLAYER_FIGHT_POWER_CHANGE = 1040;//战斗力变化
const int PLAYER_JOBLUCKY_EXCHANGE = 1041;//获取官印
const int PLAYER_GENENATE_ESCORT = 1042;//押镖成功
const int PLAYER_TURN_LUCKY_TABLE = 1043;//转盘
const int PLAYER_RANKING_CHANGE = 1044;//竞技场排名
const int PLAYER_WIN_CLAN_WAR = 1045;//帮派战
const int PLAYER_SKILL_UPGRADE = 1046;//技能升级
const int PLAYER_GET_VIP_GIFT = 1047;//领取VIP礼包
const int PLAYER_WEAR_EQUIP = 1048;//穿戴装备
const int PLAYER_WEAPON_MAGICWEAPON_ADVANCE = 1049;	//神兵升阶段

const int PLAYER_ENTER_MAP_EVENT_TYPE = 1050;								/// 玩家进入场景
const int PLAYER_LEAVE_MAP_EVENT_TYPE = 1051;								/// 玩家离开场景

const int PLAYER_GLOBE_PASSINSTANCE = 1052;//全局通关副本
const int PLAYER_INIT_PACKAGEINFO  = 1053;//玩家 初始化 背包 信息
const int PLAYER_CLAN_UPGRADE  = 1054;//帮派升级
const int PLAYER_ONE_KEY_COMPLETE  = 1055;//一键完成,,日常和帮派任务
const int PLAYER_ATTACK_CREATURE  = 1056;//玩家攻击某个生物

const int PLAYER_CLAN_DONATION  = 1057;//玩家帮派捐献
const int PLAYER_CLAN_HOLDPOSE  = 1058;//玩家任职某某玩家什么职位
const int PLAYER_CLAN_WEAPONLVUP = 1059;//帮主某某升级兵符至多少级，帮派上限多少人
const int PLAYER_CLAN_LOSECITY = 1060;//我帮的某某城池被某某帮占领了
const int PLAYER_CLAN_LOSEAUCTION = 1061;//我帮对某某竞价宣战失败，返回多少铜钱到帮贡仓库中
const int PLAYER_JOIN_CAMP = 1062;//加入阵营
const int PLAYER_EQUIP_UPGRADE = 1063;//装备升级
const int PLAYER_TAKE_OFF_EQUIP = 1064;//脱装备
const int PLAYER_TAKE_OFF_STONE = 1065;//从装备上扣石头

const int PLAYER_BEATTACK = 1066;				//玩家被攻击事件
const int PLAYER_MOVE = 1067;					//玩家移动事件

const int PLAYER_SEVEN_PACKAGE  		= 1068;//七日礼包领取
const int PLAYER_ONLINE_GIFT    		= 1069;//在线礼包
const int PLAYER_LEVEL_GIFT     		= 1070;//等级礼包
const int PLAYER_USE_TITLE      		= 1071;//使用称号
const int PLAYER_RECEIVE_ACTIVITY       = 1072;//领取活跃奖励
const int PLAYER_CAMP_CONTRIBUTE        = 1073;//阵营捐献
const int PLAYER_RECEIVE_MONEY_SHOP     = 1074;//钱庄领钱
const int PLAYER_START_ESCORT     		= 1075;//押镖
const int PLAYER_GIRLWAR_LINGERING   	= 1076;//战姬緾绵
const int PLAYER_GIRLWAR_UPGRADE   		= 1077;//战姬升级
const int PLAYER_GIRLWAR_FIGHT   		= 1078;//战姬出战
const int PLAYER_GIRLWAR_FIT	   		= 1079;//战姬合体
const int PLAYER_ATTACK_WORLD_BOSS	   	= 1080;//攻击世界BOSS
const int PLAYER_WARE_JOB_LUCKLY	   	= 1081;//穿戴一次官印
const int PLAYER_EQUIP_INHERIT		   	= 1082;//装备继承
const int PLAYER_RAIDS_DUPLICATE   		= 1083;//扫荡副本
const int PLAYER_SMELT_SOPHISTICATED 	= 1084;//洗练，区别于替换
const int MONSTER_REFLUSH			 	= 1085;//怪物刷新
const int MONSTER_BEATTACK 				= 1086;//怪物被攻击事件
const int PET_ATTACK	   				= 1087;//宠物伤害事件
const int PLAYER_PROTECTKING_PASSMAXNODE = 1088; // 玩家通关守护秦王最大节点

const int STATISTIC_CURRENCY_CHANGE = 1089;// 玩家通货类来源以及消耗日志
const int STATISTIC_GOLDEN_CHANGE 	= 1090;// 元宝消耗统计
const int CREATURE_HURT_EVENT			 = 1091; //伤害事件
const int CREATURE_BEHURT_EVENT		 	 = 1092; //被伤害事件
const int BEFORE_CREATURE_BEHURT_EVENT	 = 1093; //被伤害事件之前
const int CRIT_EVENT					 = 1094; //暴击事件

const int PLAYER_UIHL_UPDATEDATA = 1095; // UI高亮更新数据
const int OBJECT_ENTER_STATE	 = 1096; // 目标进入状态
const int OBJECT_EXCUTE_DOING	 = 1097; // 目标执行某种行为

const int PLAYER_ARENA_OVER = 1098; //竞技场结束，而且分出输赢
const int PLAYER_MONEYMAP_SUCCESS = 1099; //铜钱副本通关
const int PLAYER_UNWARE_JOB_LUCKLY	   	= 1100;//脱下一次官印

const int MONSTER_KILLED_EVENT_BELONG= 1101;  //怪物被杀归属，用于同队伍或被打一下的

const int PLAYER_MONEY_CHANGE = 1102; // 金钱改变
const int PLYAER_PACKAGE_CHANGE = 1103; // 背包改变   增加物品  丢弃物品  使用物品
const int PLYAER_CHANGE_FORCE = 1104; // 真气改变

const int PLAYER_MEDITATION_START = 1105;	//开始打坐
const int PLAYER_MEDITATION_INCOME = 1106; //打坐双修获得的收益
const int PLAYER_EQUIP_STRONG = 1107;				//强化行为升星和掉星

const int CREATURE_AFTER_HURT = 1108;	//对象被伤害后事件

const int BE_CRIT_EVENT	= 1109 ;//被暴击事件

const int PLAYER_RESET_PROTECTKING = 1110; // 重置守护秦王
const int PLAYER_MONEYINST_BUYCNT = 1111; // 购买铜钱副本次数
const int PLAYER_RESET_ALLINSTANCE = 1112; // 重置某类型所有副本

const int PLAYER_ADD_ARENACHALL = 1113; // 增加竞技场挑战次数

const int PLAYER_PASS_TARGET = 1114; // 玩家达到某个目标
const int PLAYER_DRAW_TARGETREWARD = 1115; // 玩家领取目标奖励

const int PLAYER_GETSIGNIN_AWARD = 1116; // 领取签到奖励

const int PLAYER_WEAPON_SMAGICWEAPON_ADVANCE = 1117;	//神器升阶段
const int PLAYER_WEAPON_HEARTMAGIC_ADVANCE = 1118;		//心法升阶段
const int PLAYER_GIRLWAR_ADD = 1119;
const int PLAYER_GIRLWAR_QUALITY= 1120; //战姬品质提升
#endif /* SERVEREVENTDEFINE_H_ */



