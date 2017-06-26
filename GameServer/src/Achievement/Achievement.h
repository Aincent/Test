/*
 * Achievement.h
 *
 *  Created on: 2014年6月19日
 *      Author: root
 */

#ifndef ACHIEVEMENT_H_
#define ACHIEVEMENT_H_

#include "define.h"
#include <map>
#include <vector>
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/ServerReturn3Int.pb.h"
#include "EventSet/EventArgs.h"
#include "../EventSet/EventDefine.h"
#include "AchievementLoader.h"

using namespace std;
using namespace CommBaseOut;

class Player;
class ServerThreeInt;

enum AchievementType
{
	AchievementType_1=1,		// 成长之路
	AchievementType_2,		// 江湖历练
	AchievementType_3,		// 绝世神兵
	AchievementType_4,		// 浴血沙场
	AchievementType_5,		// 最近完成
	AchievementType_s
};
//继承事件处理类
class Achievement
{
public:
	typedef std::vector<uint>	AchievementList;
	typedef std::vector<sPrestigeContent> AccumulateGetList;
	typedef std::map<int,sPrestigeContent> AchievementMap;

public:
	Achievement(Player * player);
	~Achievement();

	void InitEvent();
	void ReInit();
	void ReleaseAchievement();

public:
//
//	// 数据库相关
	void SetPrestigeToDB(PlayerInfo::PrestigeInfoList *PrestigeInfo);
	void InitPrestigeInfo(const PlayerInfo::PrestigeInfoList &info);
	void SetRetAchShopInfo(AchievementProto::RetAchShopInfo *info);
	//获取某类成就
	void GetPrestigeInfo(AchievementType type, std::vector<ServerReturn::ServerThreeInt> &list);

	//获取某个成就
	void GetPrestigeInfo(int prestigeId, PlayerInfo::PrestigeContent & content);

	//更新游戏目标完成状态
	void UpdatePrestigeInfo(int destinationtype,int param = 0);

	//更新成就值触发的称号
	void UPdatePrestigeValueTitle(int pretigeValue);

	//成就达成
	bool AddPassPrestige(int prestigeId, int param = 0);

	// 是否拥有此成就
	bool hasGetAchievement(uint achieveId);

	//领取成就奖励
	bool DrawAchievement(uint achieveId, int &errorType);

	//某类成就完成比例
	void GetAchievementTypePercent(std::vector<ServerReturn::ServerRetInt> &info);

	//统计 成就完成
//	void StatisticAchievement();

	//购买成就商品
	int BugAchShopGoods(int type,int goodsid);

	void Updata();

	// 获取所有未领取奖励
	int GetAllUnGetReward();

	bool HandlePlayerAdvanceLevel(const EventArgs & e);
	bool HandlePlayerKillMonster(const EventArgs & e);
	bool HandlePlayerSubmitTask(const EventArgs & e);
	bool HandlePlayerPassInstance(const EventArgs & e);
	bool HandlePlayerChannelUpdate(const EventArgs & e);
	bool HandlePlayerStrengthEquip(const EventArgs & e);
	bool HandlePlayerBeKilled(const EventArgs & e);
	bool HandlePlayerFightPowerChange(const EventArgs & e);
	bool HandlePlayerSkillUpgrade(const EventArgs & e);
	bool HandlePlayerSignIn(const EventArgs & e);
	bool HandlePlayerHorseUpdate(const EventArgs & e);
	bool HandlePlayerEnterClan(const EventArgs & e);
	bool HandlePlayerClanWar(const EventArgs & e);
	bool HandlePlayerInsertStone(const EventArgs & e);
	bool HandlePlayerWearEquip(const EventArgs & e);
	bool HandlePlayerMagicWeaponUpdate(const EventArgs & e);
	bool HandlePlayerRefreshMagicWeaponSkill(const EventArgs & e);
	bool HandlePlayerSmelt(const EventArgs & e);
	bool HandlePlayerClanUpdate(const EventArgs & e);
	bool HandlePlayerOneKeyComplete(const EventArgs & e);

private:
	AchievementMap				mAchievementIDMap;	// 自己获取成就的所有状态 ID为KEY
	AccumulateGetList			mRecentAchievement;
	map<int,sAchShopInfo> m_shopInfo;					//商店信息
	int64								m_time;
	Player*							m_owner;

	EventConnection m_lvEvent;
	EventConnection m_killEvent;
	EventConnection m_submitQuestEvent;
	EventConnection m_passInstanceEvent;
	EventConnection m_channelUpdateEvent;

	EventConnection m_strengthEquipEvent;
	EventConnection m_smeltEvent;
	EventConnection m_beKilledEvent;
	EventConnection m_fightPowerEvent;
	EventConnection m_skillUpgradeEvent;

	EventConnection m_signInEvent;
	EventConnection m_horseUpdateEvent;
	EventConnection m_enterClanEvent;
	EventConnection m_clanWarEvent;
	EventConnection m_insertStoneEvent;

	EventConnection m_wearEquipEvent;
	EventConnection m_magicWeaponUpdateEvent;
	EventConnection m_refreshMagicWeaponSkillEvent;
	EventConnection m_clanUpdateEvent;
	EventConnection m_oneKeyCompleteEvent;
};



#endif /* ACHIEVEMENT_H_ */
