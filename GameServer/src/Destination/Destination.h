/*
 * Destination.h
 *
 *  Created on: 2014年11月27日
 *      Author: root
 */

#ifndef DESTINATION_H_
#define DESTINATION_H_

#include "define.h"
#include <map>
#include <vector>
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/ServerReturn3Int.pb.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "DestinationLoader.h"
#include "EventSet/EventArgs.h"
#include "../EventSet/EventDefine.h"

using namespace std;
using namespace CommBaseOut;

class Player;
class ServerThreeInt;

enum DestinationUseType
{
	DestinationUseType_1,		// 还没有获取
	DestinationUseType_2,		// 达成了，但还没有领取
	DestinationUseType_3,		// 领取了
	DestinationUseType_max
};

struct sDestinationContent
{
	int destinationID;
	int destinationType;
	int num;
	int isComplete;
	int hadGet;
	sDestinationContent():destinationID(0),destinationType(0),num(0),isComplete(0),hadGet(0)
	{

	}
	sDestinationContent(PlayerInfo::DestinationContent *info)
	{
		destinationID = info->destinationid();
		destinationType = info->destinationtype();
		num = info->num();
		isComplete = info->iscomplete();
		hadGet = info->hadget();
	}
	void SetProto(PlayerInfo::DestinationContent &info)
	{
		info.set_destinationid(destinationID);
		info.set_destinationtype(destinationType);
		info.set_num(num);
		info.set_iscomplete(isComplete);
		info.set_hadget(hadGet);
	}
};

//继承事件处理类
class Destination
{
public:
	Destination(Player * player);
	~Destination();

	void ReInit();
	void ReleaseDestination();

	typedef std::vector<uint>	DestinationList;
	typedef std::map<int,sDestinationContent> DestinationMap;

	// 获取未领取奖励数量
	int GetUnGetRewardNum();
	//获取某阶段游戏目标
	void GetStageDestinationInfo(int destinationStage, std::vector<ServerReturn::ServerThreeInt> &list);

	//获取可领取奖励的游戏目标IDS
	void GetHaveRewardDestination(std::vector<int> &list);

	// 获取每阶段可领取个数
	void GetPerStageCanGetAward(std::map<int, int>& get);

	//获取某类游戏目标
	void GetDestinationInfo(int destinationtype, std::vector<ServerReturn::ServerThreeInt> &list);

	//获取某个游戏目标
	void GetDestinationInfo(int destinationId, PlayerInfo::DestinationContent & content);

//	void UpdateDestinationInfo(int logicRelevant,DWORD num);
	//更新游戏目标完成状态
	void UpdateDestinationInfo(int destinationtype,int param,int param1 = 0);

	//判断游戏目标是否达成
	bool AddPassDestination(int destinationId, int param = 0,int param1 = 0);

	// 是否已达成此游戏目标
	bool hasGetDestination(uint destinationId);

	//领取游戏目标奖励
	int DrawDestination(uint destinationId);

	void SetDestinationToDB(PlayerInfo::DestinationInfoList *destinationInfo);

	void InitDestinationInfo(const PlayerInfo::DestinationInfoList &info);

	bool HandlePlayerAdvanceLevel(const EventArgs & e);
	bool HandlePlayerBuyItemFromMall(const EventArgs & e);
	bool HandlePlayerHorseUpdate(const EventArgs & e);
	bool HandlePlayerMagicWeaponUpdate(const EventArgs & e);
	bool HandlePlayerMeridianUpdate(const EventArgs & e);
	bool HandlePlayerCompose(const EventArgs & e);
	bool HandlePlayerPassInstance(const EventArgs & e);
	bool HandlePlayerKillWorldBoss(const EventArgs & e);
	bool HandlePlayerRefreshMagicWeaponSkill(const EventArgs & e);
	bool HandlePlayerStrengthEquip(const EventArgs & e);
	bool HandlePlayerFightPowerChange(const EventArgs & e);
	bool HandlePlayerExchangeJoblucky(const EventArgs & e);
	bool HandlePlayerChannelUpdate(const EventArgs & e);
	bool HandlePlayerGenenateEscort(const EventArgs & e);
	bool HandlePlayerTurnLuckTable(const EventArgs & e);
	bool HandlePlayerRankingChange(const EventArgs & e);
	bool HandlePlayerClanWar(const EventArgs & e);
	bool HandlePlayerSkillUpgrade(const EventArgs & e);
	bool HandlePlayerGetVipGift(const EventArgs & e);
	bool HandleAreaOver(const EventArgs & e);
	bool HandlePlayerSMagicWeaponUpdate(const EventArgs & e);
	bool HandlePlayerHeartMagicUpdate(const EventArgs & e);
	bool HandlePlayerInsertStone(const EventArgs & e);
	bool HandlePlayerAddGirlWar(const EventArgs & e);
	bool HandlePlayerQualityGirlWar(const EventArgs & e);
	bool HandlePlayerBeKilled(const EventArgs & e);
	bool HandlePlayerWearEquip(const EventArgs & e);
	bool HandlePlayerKillMonster(const EventArgs & e);

private:
	DestinationMap				mDestinationIDMap;	// 自己获取游戏目标的所有状态 ID为KEY
	Player*							m_owner;

	EventConnection m_lvEvent;
	EventConnection m_BuyItemFromMallEvent;
	EventConnection m_HorseUpdateEvent;
	EventConnection m_MagicWeaponUpdateEvent;
	EventConnection m_MeridianUpdateEvent;

	EventConnection m_ComposeEvent;
	EventConnection m_PassInstanceEvent;
	EventConnection m_KillWorldBossEvent;
	EventConnection m_RefreshMagicWeaponSkillEvent;
	EventConnection m_StrengthEquipEvent;

	EventConnection m_FightPowerEvent;
	EventConnection m_ExchangeJobluckyEvent;
	EventConnection m_ChannelUpdateEvent;
	EventConnection m_GenenateEscortEvent;
	EventConnection m_TurnLuckTableEvent;

	EventConnection m_RankingChangeEvent;
	EventConnection m_ClanWarEvent;
	EventConnection m_SkillUpgradeEvent;
	EventConnection m_VipEvent;
	EventConnection m_areaOverEvent;

	EventConnection m_SMagicWeaponUpdateEvent;
	EventConnection m_HeartMagicUpdateEvent;
	EventConnection m_insertStoneEvent;
	EventConnection m_AddGirlWarEvent;
	EventConnection m_QualityGirlWarEvent;

	EventConnection m_beKilledEvent;
	EventConnection m_wearEquipEvent;
};


#endif /* DESTINATION_H_ */
