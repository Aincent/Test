/*
 * GameParaLoader.h
 *
 *  Created on: 2015年4月23日
 *      Author: root
 */

#ifndef GAMEPARALOADER_H_
#define GAMEPARALOADER_H_

#include "Singleton.h"
#include "define.h"
#include "Lua/LuaInterface.h"

using namespace std;
using namespace CommBaseOut;
//专门处理一些没有LUA文件管理的零碎的宏，公式等等

struct MagicParam
{
	MagicParam()
	{
		m_willBeZeroPer = 0;
		m_interDayLosePer = 0;
		m_maxInterDay = 0;
		m_canUseTempMapID = 0;
		m_magicCountDownTime = 0;
		m_SkillQuality = 0;
		m_FirstSkillQuality = 0;
		m_magicFlushItemID = 0;
		m_magicAdavnceItemID = 0;
	}
	int m_willBeZeroPer;
	int	m_interDayLosePer;
	int	m_maxInterDay;
	int	m_canUseTempMapID;
	int	m_magicCountDownTime;
	int	m_SkillQuality;
	int	m_FirstSkillQuality;
	int	m_magicFlushItemID;
	int	m_magicAdavnceItemID;
};

class GameParaLoader : public Singleton<GameParaLoader>
{
public:
	GameParaLoader():m_protectKingSceneID(0),m_digPointLength(0),m_answerActivityID(0),m_answerInstanceLimit(0),m_totalQuestions(0),m_questionNum(0),m_answerMapID(0),
	m_antiAddictionHealthTime(0),m_antiAddictionTiredTime(0),m_chatHornID(0),m_chatWorldCD(0),m_chatPrivateCD(0),m_chatGeneralCD(0),m_chatTeamCD(0),m_chatClanCD(0),m_chatCountryCD(0),
	m_chatHornCD(0),m_officialItemQualityViolet(0),m_0fficialItemQualityOrange(0),m_jobLuckyPackageSize(0),m_firstJobLuckyID(0),m_jobLuckyContriID(0),m_onceMaxAdditemCount(0),m_arrangeBagCDTime(0),m_canFinishQuestDiff(0.0f),m_luLianStone(0),
	m_baseAddExpLuLian(0),m_luLianSmallCritRate(0),m_luLianSmallCritTime(0),m_luLianBigCritRate(0),m_luLianBigCritTime(0),m_initDanLuID(0),m_escortRandomRate(0),m_escortReflushItemID(0),
	m_robTimes(0),m_reflushEscortTime(0),m_simpleEatBaseExp(0),m_horseSmallCritRate(0.0f),m_horseSmallCritTime(0),m_horseBigCritRate(0.0f),m_horseBigCritTime(0),m_highestEatCostGolden(0),
	m_maxCanLeaveEscortDis(0),m_groupMemberNum(0),m_groupMemberAddSomthigNum(0),m_outputPlayerInfoNum(0),m_opengroupModelmainLineID(0),m_baseAddExp(0),m_baseAutoEatExpRand(0.0f),m_eatHorseItemID(0),
	m_magicAdavnceItemID(0),m_magicFlushItemID(0),m_magicCountDownTime(0),m_SkillQuality(0),m_FirstSkillQuality(0),m_canUseTempMapID(0),m_maxInterDay(0),m_interDayLosePer(0),m_willBeZeroPer(0),
	m_processTime(0),m_defaultFreshMonster(0),
	m_freshMonsterArea(0),m_arena_MapID(0),m_clanWar_MapID(0),m_countryWar_MapID(0),m_treasonKing_MapID(0),m_crossArenaMapID(0),m_freshTime(0),m_first_Group_MapID(0),m_first_Challenge_MapID(0),m_lastChallengeMapID(0),m_equipMaxLv(0),m_costMoneyRate(0),m_createStoneNeedNum(0),
	m_rankingTitleIDBegin(0),m_rankingLvBegin(0),m_rankingFightBegin(0),m_rankingHorseBegin(0),m_rankingAchievementBegin(0),m_rankingClanBegin(0),m_rankingMagicWeaponBegin(0),m_rankingDanLuBegin(0),m_rankingAcupointBegin(0),
	m_rankingGirlWarBegin(0),m_rankingWorshipAmountBegin(0),m_fashionID(0),m_exchangeMoneyItemID(0),m_exchangeExpItemID(0),m_exchangeForceItemID(0),m_exchangeGoldenItemID(0),m_exchangeBindGoldenItemID(0),m_exchangeContributionItemID(0),
	m_exchangeCountryContributionItemID(0),m_exchangeCreditItemID(0),m_exchangeVipGolden(0),m_exchangeChallengeItem(0),m_pretigeMallStage1(0),m_pretigeMallStage2(0),m_pretigeMallStage3(0),m_pretigeMallStage4(0),m_pretigeMallStage5(0),
	m_dailyNeedGolden(0),m_minMonPlayerDiffLv(0),m_maxMonPlayerDiffLv(0),m_addSinValue(0),m_specialSkillIDBegin(0),m_changePlayerStateTime(0),m_reduceSinValuePer(0.0f),m_muscleDis(0),m_rangedDis(0),
	m_protectLv(0),m_decreaseExp(0.0f), m_sigNum1(0),m_sigNum2(0),m_sigNum3(0),m_sigNum4(0),m_sigNum5(0),m_oneKeyRecoveRate(0),m_recoverMoneyRate(0),m_recoverForceRate(0),m_recoverExpRate(0),m_recoverGoldenMoneyRate(0),m_recoverContriRate(0),m_vipCardLv(0),m_hpCD(0),m_mpCD(0),m_CountryMap(0),m_shoesNpc(0),
	m_costItemRevival(0),m_rose(0),m_fly(0),m_addSignInCost(0),m_dayUpdateHour(0),m_dayUpdateMinute(0),m_dayupdateSeconds(0),m_completeGiftHour(0),m_completeGiftMinute(0),m_completeGiftSeconds(0), m_arenaWarReportHour(0), m_arenaWarReportMinute(0), m_arenaWarReportSecond(0),m_WeekTask(0),m_FinishOne(0),m_GroupInstanceBuffId(0)
	{
		tlua.OpenLua();
	}
	~GameParaLoader()
	{
		tlua.CloseLua();
	}
public:
	int Init(string &path);

	int InitLua(string& file);

	BYTE get0fficialItemQualityOrange() const {
		return m_0fficialItemQualityOrange;
	}

	DWORD getAnswerActivityId() const {
		return m_answerActivityID;
	}

	int getAnswerInstanceLimit() const {
		return m_answerInstanceLimit;
	}

	DWORD getAntiAddictionHealthTime() const {
		return m_antiAddictionHealthTime;
	}

	DWORD getAntiAddictionTiredTime() const {
		return m_antiAddictionTiredTime;
	}

	int getBaseAddExp() const {
		return m_baseAddExp;
	}

	int getBaseAddExpLuLian() const {
		return m_baseAddExpLuLian;
	}

	float getluLianSmallCritRate() const {
		return m_luLianSmallCritRate;
	}

	int getluLianSmallCritTime() const {
		return m_luLianSmallCritTime;
	}

	float getluLianBigCritRate() const {
		return m_luLianBigCritRate;
	}

	int getluLianBigCritTime() const {
		return m_luLianBigCritTime;
	}

	float getBaseAutoEatExpRand() const {
		return m_baseAutoEatExpRand;
	}

	int getCanUseTempMapId() const {
		return m_canUseTempMapID;
	}

	int getChatHornId() const {
		return m_chatHornID;
	}

	DWORD getDefaultFreshMonster() const {
		return m_defaultFreshMonster;
	}

	DWORD getDigPointLength() const {
		return m_digPointLength;
	}

	int getEatHorseItemId() const {
		return m_eatHorseItemID;
	}

	DWORD getEscortRandomRate() const {
		return m_escortRandomRate;
	}

	int getEscortReflushItemId() const {
		return m_escortReflushItemID;
	}

	DWORD getFreshMonsterArea() const {
		return m_freshMonsterArea;
	}

	DWORD getFreshTime() const {
		return m_freshTime;
	}

	uint getGroupMemberAddSomthigNum() const {
		return m_groupMemberAddSomthigNum;
	}

	uint getGroupMemberNum() const {
		return m_groupMemberNum;
	}

	int getInitDanLuId() const {
		return m_initDanLuID;
	}

	int getLuLianStone() const {
		return m_luLianStone;
	}

	int getMagicAdavnceItemId() const {
		return m_magicAdavnceItemID;
	}

	int getMagicCountDownTime() const {
		return m_magicCountDownTime;
	}

	int getMagicSkillQuality() const {
		return m_SkillQuality;
	}

	int getMagicFirstSkillQuality() const {
		return m_FirstSkillQuality;
	}

	int getMagicFlushItemId() const {
		return m_magicFlushItemID;
	}

	int getMaxInterDay() const {
		return m_maxInterDay;
	}

	BYTE getOfficialItemQualityViolet() const {
		return m_officialItemQualityViolet;
	}

	int getOnceMaxAdditemCount() const {
		return m_onceMaxAdditemCount;
	}

	uint getOpengroupModelmainLineId() const {
		return m_opengroupModelmainLineID;
	}

	int getOutputPlayerInfoNum() const {
		return m_outputPlayerInfoNum;
	}

	DWORD getProcessTime() const {
		return m_processTime;
	}

	DWORD getProtectKingSceneId() const {
		return m_protectKingSceneID;
	}

	DWORD getTotalQuestions() const {
		return m_totalQuestions;
	}

	int getQuestionNum() const {
		return m_questionNum;
	}

	int getRankingAchievementBegin() const {
		return m_rankingAchievementBegin;
	}

	int getRankingAcupointBegin() const {
		return m_rankingAcupointBegin;
	}

	int getRankingClanBegin() const {
		return m_rankingClanBegin;
	}

	int getRankingDanLuBegin() const {
		return m_rankingDanLuBegin;
	}

	int getRankingFightBegin() const {
		return m_rankingFightBegin;
	}

	int getRankingGirlWarBegin() const {
		return m_rankingGirlWarBegin;
	}

	int getRankingHorseBegin() const {
		return m_rankingHorseBegin;
	}

	int getRankingLvBegin() const {
		return m_rankingLvBegin;
	}

	int getRankingMagicWeaponBegin() const {
		return m_rankingMagicWeaponBegin;
	}

	int getRankingTitleIdBegin() const {
		return m_rankingTitleIDBegin;
	}

	int getRankingWorshipAmountBegin() const {
		return m_rankingWorshipAmountBegin;
	}

	int getFashionId() const {
		return m_fashionID;
	}

	void setFashionId(int fashionId) {
		m_fashionID = fashionId;
	}

	int getEquipMaxLv() const {
		return m_equipMaxLv;
	}

	int getExchangeBindGoldenItemId() const {
		return m_exchangeBindGoldenItemID;
	}

	int getExchangeContributionItemId() const {
		return m_exchangeContributionItemID;
	}

	int getExchangeCountryContributionItemId() const {
		return m_exchangeCountryContributionItemID;
	}

	int getExchangeCreditItemId() const {
		return m_exchangeCreditItemID;
	}

	int getExchangeChallengeItem()const
	{
		return m_exchangeChallengeItem;
	}

	int getExchangeExpItemId() const {
		return m_exchangeExpItemID;
	}

	int getExchangeForceItemId() const {
		return m_exchangeForceItemID;
	}

	int getExchangeGoldenItemId() const {
		return m_exchangeGoldenItemID;
	}

	int getExchangeMoneyItemId() const {
		return m_exchangeMoneyItemID;
	}

	int getPretigeMallStage1() const {
		return m_pretigeMallStage1;
	}

	void setPretigeMallStage1(int pretigeMallStage1) {
		m_pretigeMallStage1 = pretigeMallStage1;
	}

	int getPretigeMallStage2() const {
		return m_pretigeMallStage2;
	}

	int getPretigeMallStage3() const {
		return m_pretigeMallStage3;
	}

	int getPretigeMallStage4() const {
		return m_pretigeMallStage4;
	}

	int getPretigeMallStage5() const {
		return m_pretigeMallStage5;
	}

	int getArrangeBagCdTime() const {
		return m_arrangeBagCDTime;
	}

	int getDailyNeedGolden() const {
		return m_dailyNeedGolden;
	}

	float getCanFinishQuestDiff() const {
		return m_canFinishQuestDiff;
	}

	int getCostMoneyRate() const {
		return m_costMoneyRate;
	}

	int getCreateStoneNeedNum() const {
		return m_createStoneNeedNum;
	}

	int getRobTimes() const {
		return m_robTimes;
	}

	int getReflushEscortTime() const {
		return m_reflushEscortTime;
	}

	int getSimpleEatBaseExp() const {
		return m_simpleEatBaseExp;
	}

	float getHorseSmallCritRate() const {
		return m_horseSmallCritRate;
	}

	int getHorseSmallCritTime() const {
		return m_horseSmallCritTime;
	}

	float getHorseBigCritRate() const {
		return m_horseBigCritRate;
	}

	int getHorseBigCritTime() const {
		return m_horseBigCritTime;
	}

	int getHighestEatCostGolden() const {
		return m_highestEatCostGolden;
	}

	int getInterDayLosePer() const {
		return m_interDayLosePer;
	}

	int getWillBeZeroPer() const {
		return m_willBeZeroPer;
	}

	int getMaxCanLeaveEscortDis() const {
		return m_maxCanLeaveEscortDis;
	}

	int getMaxMonPlayerDiffLv() const {
		return m_maxMonPlayerDiffLv;
	}

	int getMinMonPlayerDiffLv() const {
		return m_minMonPlayerDiffLv;
	}

	int getAddSinValue() const {
		return m_addSinValue;
	}

	int getSpecialSkillIdBegin() const {
		return m_specialSkillIDBegin;
	}

	int getChangePlayerStateTime() const {
		return m_changePlayerStateTime;
	}

	float getReduceSinValuePer() const {
		return m_reduceSinValuePer;
	}

	int getAnswerMapId() const {
		return m_answerMapID;
	}

	int getMuscleDis() const {
		return m_muscleDis;
	}

	int getRangedDis() const {
		return m_rangedDis;
	}

	int getProtectLv() const {
		return m_protectLv;
	}

	float getDecreaseExpRate() const {
		return m_decreaseExp;
	}

	float getDecreaseExp() const {
		return m_decreaseExp;
	}

	int getSigNum1() const {
		return m_sigNum1;
	}

	int getSigNum2() const {
		return m_sigNum2;
	}

	int getSigNum3() const {
		return m_sigNum3;
	}

	int getSigNum4() const {
		return m_sigNum4;
	}

	int getSigNum5() const {
		return m_sigNum5;
	}

	int getOneKeyRecoveRate() const {
		return m_oneKeyRecoveRate;
	}

	int getVipCardLv() const {
		return m_vipCardLv;
	}

	int getHPCD()const{
		return m_hpCD;
	}

	int getMPCD()const{
		return m_mpCD;
	}

	int getArenaMapId() const {
		return m_arena_MapID;
	}

	int getClanWarMapId() const {
		return m_clanWar_MapID;
	}

	int getCountryWarMapId() const {
		return m_countryWar_MapID;
	}

	int getFirstSkillQuality() const {
		return m_FirstSkillQuality;
	}

	int getHpCd() const {
		return m_hpCD;
	}

	int getMpCd() const {
		return m_mpCD;
	}

	int getSkillQuality() const {
		return m_SkillQuality;
	}

	int getFirstChallengeMapId() const {
		return m_first_Challenge_MapID;
	}

	int getFirstGroupMapId() const {
		return m_first_Group_MapID;
	}

	int GetLastChallengeMapID() const{
		return m_lastChallengeMapID;
	}

	int getTreasonKingMapId() const {
		return m_treasonKing_MapID;
	}

	int getCountryMap() const {
		return m_CountryMap;
	}

	int getChatGeneralCd() const {
		return m_chatGeneralCD;
	}

	int getChatPrivateCd() const {
		return m_chatPrivateCD;
	}

	int getChatWorldCd() const {
		return m_chatWorldCD;
	}

	float getLuLianBigCritRate() const {
		return m_luLianBigCritRate;
	}

	int getLuLianBigCritTime() const {
		return m_luLianBigCritTime;
	}

	float getLuLianSmallCritRate() const {
		return m_luLianSmallCritRate;
	}

	int getLuLianSmallCritTime() const {
		return m_luLianSmallCritTime;
	}

	int getExchangeVipGoldenId() const {
		return m_exchangeVipGolden;
	}

	int getChatClanCd() const {
		return m_chatClanCD;
	}

	int getChatCountryCd()const {
		return m_chatCountryCD;
	}

	int getChatHornCd() const {
		return m_chatHornCD;
	}

	int getChatTeamCd() const {
		return m_chatTeamCD;
	}

	int getExchangeVipGolden() const {
		return m_exchangeVipGolden;
	}

	int GetExchangeItemIdByAtrrType(int atrrType);

	int GetAtrrTypeByExchangeItemId(int ExchangeItemId);

	int getRecoverExpRate() const {
		return m_recoverExpRate;
	}

	int getRecoverForceRate() const {
		return m_recoverForceRate;
	}

	int getRecoverMoneyRate() const {
		return m_recoverMoneyRate;
	}

	int getRecoverGoldenMoneyRate() const {
		return m_recoverGoldenMoneyRate;
	}

	int getShoesNpc()const{
		return m_shoesNpc;
	}

	int getCostItemRevival() const {
		return m_costItemRevival;
	}

	int getFly() const {
		return m_fly;
	}

	int getRose() const {
		return m_rose;
	}

	int getRecoverContriRate() const {
		return m_recoverContriRate;
	}

	int getAddSignInCost() const {
		return m_addSignInCost;
	}

	int getFirstJobLuckyId() const {
		return m_firstJobLuckyID;
	}

	int getJobLuckyContriId() const {
		return m_jobLuckyContriID;
	}

	int getJobLuckyPackageSize() const {
		return m_jobLuckyPackageSize;
	}
	
	void getDayUpdateTime(int &hour,int &minute,int &seconds);

	void getCompleteGiftUpdateTime(int &hour,int &minute,int &second);

	void GetArenaWarReportUpdateTime(int& hour, int& minute, int& second);

	int getArenaWarReportHour() const {
		return m_arenaWarReportHour;
	}

	int getArenaWarReportMinute() const {
		return m_arenaWarReportMinute;
	}

	int getArenaWarReportSecond() const {
		return m_arenaWarReportSecond;
	}

	int getCompleteGiftHour() const {
		return m_completeGiftHour;
	}

	int getCompleteGiftMinute() const {
		return m_completeGiftMinute;
	}

	int getCompleteGiftSeconds() const {
		return m_completeGiftSeconds;
	}

	int getCrossArenaMapId() const {
		return m_crossArenaMapID;
	}

	int getDayUpdateHour() const {
		return m_dayUpdateHour;
	}

	int getDayUpdateMinute() const {
		return m_dayUpdateMinute;
	}

	int getDayupdateSeconds() const {
		return m_dayupdateSeconds;
	}

	int getWeekTask(){return m_WeekTask;}
	int getWeekFinishone(){return m_FinishOne;}

	//获得任务资源追回的Exp
	int GetDailyQuestExp(int64 lvExp);

	//获得任务资源追回的铜钱
	int GetDailyQuestMoney(int lv);

	//获取镖车的资源追回
	int GetEscortReward(int lv,int rate1,int rate2,int rete3,int qualityExp);

	//获取竞技场资源追回经验
	int GetAreaExp(int winExp,int loseExp);

	//法器数据
	const MagicParam& GetArtifectParam(){ return m_artifactParam; }

	//心法数据
	const MagicParam& GetHeartParam(){ return m_heartParam; }

	//获取组队副本buff加成ID
	int GetGroupInstanceBuffId(){return m_GroupInstanceBuffId; }

private:
	//保护秦王副本ID
	DWORD m_protectKingSceneID;
	//挖宝的距离
	DWORD m_digPointLength;

	//答题活动的ID
	DWORD m_answerActivityID;
	int m_answerInstanceLimit;
	DWORD m_totalQuestions;
	int m_questionNum;
	//答题的副本ID
	int m_answerMapID;

	//健康时间
	DWORD m_antiAddictionHealthTime;
	//疲劳时间:大于2小时，小于5小时
	DWORD m_antiAddictionTiredTime;

	//聊天所需的道具
	int m_chatHornID;
	//世界聊天CD
	int m_chatWorldCD;
	//私聊
	int m_chatPrivateCD;
	//普通聊天
	int m_chatGeneralCD;
	//--队伍聊天
	int m_chatTeamCD;
	//--帮派聊天
	int m_chatClanCD;
	//阵营频道
	int m_chatCountryCD;
	//--喇叭聊天
	int m_chatHornCD;

	//紫色物品
	BYTE m_officialItemQualityViolet;
	//橙色物品
	BYTE m_0fficialItemQualityOrange;
	//官印背包最大格子数
	int m_jobLuckyPackageSize;
	//第一次官印给的官印ID
	int m_firstJobLuckyID;
	//功勋id
	int m_jobLuckyContriID;

	//一次最多 能够 增加的 叠加数
	int m_onceMaxAdditemCount;
	//整理背包CD时间
	int m_arrangeBagCDTime;
	//能完成任务时使用任务物品的距离
	float m_canFinishQuestDiff;

	//炉练石ID
	int m_luLianStone;
	//每次练炉增加的 经验值
	int m_baseAddExpLuLian;
	//小爆击概率
	float m_luLianSmallCritRate;
	//小爆击倍数
	int m_luLianSmallCritTime;
	//大爆击概率
	float m_luLianBigCritRate;
	//大爆击倍数
	int m_luLianBigCritTime;
	//最开始的丹炉ID
	int m_initDanLuID;

	//宝箱随机基数
	DWORD m_escortRandomRate;
	//镖车刷新令牌ID
	int m_escortReflushItemID;
	//一天能抢劫几次镖车
	int m_robTimes;
	//镖车被劫后多久刷新
	int m_reflushEscortTime;
	//普通放养增加的经验基础值
	int m_simpleEatBaseExp;
	//小爆击概率
	float m_horseSmallCritRate;
	//小爆击倍数
	int m_horseSmallCritTime;
	//大爆击概率
	float m_horseBigCritRate;
	//大爆击倍数
	int m_horseBigCritTime;
	//单次高级放养需消耗的元宝
	int m_highestEatCostGolden;
	//能离镖车的最远距离
	int m_maxCanLeaveEscortDis;

	//每个队伍成员上限
	uint m_groupMemberNum;
	//可以 增加属性 的 队伍人数
	uint m_groupMemberAddSomthigNum;
	//每页显示的 玩家 数量
	int m_outputPlayerInfoNum;
	//任务 开启 主线ID
	uint m_opengroupModelmainLineID;

	//每次增加  经验的 基础值
	int m_baseAddExp;
	float m_baseAutoEatExpRand;
	//坐骑进阶单ID
	int m_eatHorseItemID;

	//神兵升级所需道具
	int m_magicAdavnceItemID;
	//神兵刷新技能所需道具
	int m_magicFlushItemID;
	//神兵激活时间
	int m_magicCountDownTime;
	//第一次刷新技能品质
	int m_SkillQuality;
	//第一获得该技能时的品质
	int m_FirstSkillQuality;
	////能够 临时 使用 的 地图
	int m_canUseTempMapID;
	//相隔天数 上限
	int m_maxInterDay;
	//锻炼度隔一天失去的百分比
	int m_interDayLosePer;
	//低于原本的百分比直接变为0
	int m_willBeZeroPer;

	//法器数据
	MagicParam m_artifactParam;

	//心法数据
	MagicParam m_heartParam;

	//结婚过程所需时间
	DWORD m_processTime;
	//刷新怪物所需时间
	DWORD m_defaultFreshMonster;
	//刷新怪物范围
	DWORD m_freshMonsterArea;
	//表示竞技场地图ID
	int m_arena_MapID;
	//帮派战地图ID
	int m_clanWar_MapID;
	//阵营战地图
	int m_countryWar_MapID;
	//剿灭反王地图
	int m_treasonKing_MapID;
	//跨服竞技地图
	int m_crossArenaMapID;

	//副本刷新时间
	DWORD m_freshTime;
	//第一个组队副本的ID
	int m_first_Group_MapID;
	//第一个挑战副本的ID
	int m_first_Challenge_MapID;
	int m_lastChallengeMapID; // 最后一个挑战副本ID

	// 装备最高等级
	int m_equipMaxLv;
	//镶嵌石头需花费倍数(需花费铜钱=物品等级*倍数)
	int m_costMoneyRate;
	//合成宝石需要多少颗
	int m_createStoneNeedNum;

	//排名称号开始ID
	int m_rankingTitleIDBegin;
	//--等级排行称号开始ID
	int m_rankingLvBegin;
	//--战斗力排行称号开始ID
	int m_rankingFightBegin;
	//--坐骑排行称号开始ID
	int m_rankingHorseBegin;
	//--成就排行称号开始ID
	int m_rankingAchievementBegin;
	//--公会排行称号开始ID
	int m_rankingClanBegin;
	//--神兵排行称号开始ID
	int m_rankingMagicWeaponBegin;
	//--丹道排行称号开始ID
	int m_rankingDanLuBegin;
	//--筋脉排行称号开始ID
	int m_rankingAcupointBegin;
	//--战姬排行称号开始ID
	int m_rankingGirlWarBegin;
	//--膜拜排行称号开始ID
	int m_rankingWorshipAmountBegin;

	//时装ID
	int m_fashionID;

	int m_exchangeMoneyItemID;//金钱
	int m_exchangeExpItemID;//经验
	int m_exchangeForceItemID;//真气
	int m_exchangeGoldenItemID;//元宝
	int m_exchangeBindGoldenItemID;//帮定元宝
	int m_exchangeContributionItemID;//帮贡
	int m_exchangeCountryContributionItemID;//阵营贡献
	int m_exchangeCreditItemID;//声望
	int m_exchangeVipGolden;//vip元宝，可触发后台充值的一系列事件
	int m_exchangeChallengeItem;	//晶石货币

	//成就商品区
	int m_pretigeMallStage1;
	int m_pretigeMallStage2;
	int m_pretigeMallStage3;
	int m_pretigeMallStage4;
	int m_pretigeMallStage5;

	//每日必做
	int m_dailyNeedGolden;

	//玩家相关
	//玩家等级大于怪物5级小于10级，经验减半;大于10级经验为1
	int m_minMonPlayerDiffLv;
	int m_maxMonPlayerDiffLv;
	//杀死一个玩家增加500罪恶值
	int m_addSinValue;
	//非普通攻击技能开始ID
	int m_specialSkillIDBegin;
	//玩家状态切换时间
	int m_changePlayerStateTime;
	//每2小时减100罪恶值折合成两分钟减少1.667罪恶值
	float m_reduceSinValuePer;
	//近战的攻击距离
	int m_muscleDis;
	//远程的攻击距离
	int m_rangedDis;
	//30级不能玩家攻击也不能被玩家攻击
	int m_protectLv;
	//红名玩家被杀掉经验比率
	float m_decreaseExp;

	//签到天数
	int m_sigNum1;
	int m_sigNum2;
	int m_sigNum3;
	int m_sigNum4;
	int m_sigNum5;

	//资源追回系数
	int m_oneKeyRecoveRate;
	//铜钱系数
	int m_recoverMoneyRate;
	//真气系数
	int m_recoverForceRate;
	//经验系数
	int m_recoverExpRate;
	//追回需要的元宝跟铜钱的比例
	int m_recoverGoldenMoneyRate;
	//帮贡系数
	int m_recoverContriRate;

	//VIP体验卡使用后的VIP等级
	int m_vipCardLv;

	//血和蓝的cd id
	int m_hpCD;
	int m_mpCD;

	int m_CountryMap;

	//免飞鞋npc
	int m_shoesNpc;

	//复活
	int m_costItemRevival;
	//玫瑰花
	int m_rose;
	//小飞鞋
	int m_fly;

	//补签消耗
	int m_addSignInCost;
	//每日更新时间
	int m_dayUpdateHour;
	int m_dayUpdateMinute;
	int m_dayupdateSeconds;

	//竞技场每日礼包更新
	int m_completeGiftHour;
	int m_completeGiftMinute;
	int m_completeGiftSeconds;

	// 竞技场战报更新清理时间
	int m_arenaWarReportHour;
	int m_arenaWarReportMinute;
	int m_arenaWarReportSecond;

	int m_WeekTask;
	int m_FinishOne;

	//组队副本buff加成ID
	int m_GroupInstanceBuffId;
	CLuaInterface tlua;
};


#endif /* GAMEPARALOADER_H_ */
