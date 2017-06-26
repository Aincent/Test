/*
 * ExtraAtributeMgr.h
 *
 *  Created on: 2015年4月16日
 *      Author: root
 */

#ifndef EXTRAATRIBUTEMGR_H_
#define EXTRAATRIBUTEMGR_H_

#include "define.h"
#include <map>
#include <vector>
#include "ExtraAtributeLoader.h"
#include "EventSet/EventArgs.h"
#include "../EventSet/EventDefine.h"
#include "CharDefine.h"

using namespace std;
using namespace CommBaseOut;

class Player;

class ExtraAtributeMgr
{
public:
	ExtraAtributeMgr(Player * player);
	~ExtraAtributeMgr();

	void ReInit();
	void Release();

	void InitExtraAtribute();

	//上线计算属性
	void onlineCalculateAtt();

	//计算一次战斗力
	int calculateFight();

	//增加或减去某个档的属性
	void ChangeExtraAtribute(DWORD type,DWORD extraLv,bool isAdd);
	//计算某额外属性奖励的档数
	DWORD CalExtraAtributeLv(DWORD type);

	bool IsExtraAtribute(ExtraAtribute info);

	bool IsExtraAtributeStrength(ExtraAtribute info);

	bool IsExtraAtributeRefine(ExtraAtribute info);

	bool IsExtraAtributeEquipLv(ExtraAtribute info);

	bool IsExtraAtributeStone(ExtraAtribute info);

	bool HandlePlayerStrengthEquip(const EventArgs & e);

	bool HandlePlayerSmelt(const EventArgs & e);

	bool HandlePlayerWearEquip(const EventArgs & e);

	bool HandlePlayerTakeOffEquip(const EventArgs& e);

	bool HandlePlayerInsertStone(const EventArgs & e);

	bool HandlePlayerTakeOffStone(const EventArgs& e);

	bool HandlePlayerEquipUpgrade(const EventArgs & e);

protected:
	//根据档数是类型获得增加的属性
	void getAttByTypeAndNumber(ExtraAtributeType type,int number,map<ECharAttrType,int64>& attList);

private:
	Player*							m_owner;
	//装备强化 装备洗练 装备等级 装备宝石的档数
	DWORD m_extraStrengthLv,m_extraRefineLv,m_extraEquipLv,m_extraStoneLv;
	//装备强化
	EventConnection m_StrengthEquipEvent;
	//装备洗练
	EventConnection m_smeltEvent;
	//装备穿戴
	EventConnection m_wearEquipEvent;
	//宝石镶嵌
	EventConnection m_insertStoneEvent;
	//扣除宝石
	EventConnection m_takeOffStoneEvent;
	//装备升级
	EventConnection m_equipUpgrade;
	//脱掉装备
	EventConnection m_takeOffEquip;
};

#endif /* EXTRAATRIBUTEMGR_H_ */
