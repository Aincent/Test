/*
 * GameParaLoader.cpp
 *
 *  Created on: 2015年4月23日
 *      Author: root
 */
#include "GameParaLoader.h"
#include "Log/Logger.h"
#include "../CharDefine.h"

int GameParaLoader::Init(string& path)
{
	string strLuaFile = path + "Data/gamePara.lua";

	if(InitLua(strLuaFile))
	{
		return -1;
	}

	return 0;
}

int GameParaLoader::InitLua(string& file)
{
	if(tlua.LoadFile(file) != 0 || tlua.Call() != 0)
	{
		return -1;
	}

	tlua.SetLuaFunction("GetProtectKingParam");
	if(tlua.Call(0, 2))
	{
		return -1;
	}
	m_digPointLength = tlua.GetInt();
	m_protectKingSceneID = tlua.GetInt();

	tlua.SetLuaFunction("GetAnswerParam");
	if(tlua.Call(0, 5))
	{
		return -1;
	}
	m_answerMapID = tlua.GetInt();
	m_questionNum = tlua.GetInt();
	m_totalQuestions = tlua.GetInt();
	m_answerInstanceLimit = tlua.GetInt();
	m_answerActivityID = tlua.GetInt();

	tlua.SetLuaFunction("GetAntiAddictionParam");
	if(tlua.Call(0, 2))
	{
		return -1;
	}
	m_antiAddictionTiredTime = tlua.GetInt();
	m_antiAddictionHealthTime = tlua.GetInt();

	tlua.SetLuaFunction("GetChatParam");
	if(tlua.Call(0, 7))
	{
		return -1;
	}
	m_chatCountryCD = tlua.GetInt();
	m_chatHornCD = tlua.GetInt();
	m_chatClanCD = tlua.GetInt();
	m_chatTeamCD = tlua.GetInt();
	m_chatGeneralCD = tlua.GetInt();
	m_chatPrivateCD = tlua.GetInt();
	m_chatWorldCD = tlua.GetInt();

	tlua.SetLuaFunction("GetJobLuckyParam");
	if(tlua.Call(0, 5))
	{
		return -1;
	}
	m_jobLuckyContriID = tlua.GetInt();
	m_firstJobLuckyID = tlua.GetInt();
	m_jobLuckyPackageSize = tlua.GetInt();
	m_0fficialItemQualityOrange = tlua.GetInt();
	m_officialItemQualityViolet = tlua.GetInt();

	tlua.SetLuaFunction("GetContainerParam");
	if(tlua.Call(0, 3))
	{
		return -1;
	}
	m_canFinishQuestDiff = tlua.GetFloat();
	m_arrangeBagCDTime = tlua.GetInt();
	m_onceMaxAdditemCount = tlua.GetInt();

	tlua.SetLuaFunction("GetDanLuParam");
	if(tlua.Call(0, 7))
	{
		return -1;
	}
	m_initDanLuID 	      = tlua.GetInt();
	m_luLianBigCritTime   = tlua.GetInt();
	m_luLianBigCritRate   = tlua.GetFloat();
	m_luLianSmallCritTime = tlua.GetInt();
	m_luLianSmallCritRate = tlua.GetFloat();
	m_baseAddExpLuLian 	  = tlua.GetInt();
	m_luLianStone 		  = tlua.GetInt();

	tlua.SetLuaFunction("GetEscortExParam");
	if(tlua.Call(0, 5))
	{
		return -1;
	}
	m_maxCanLeaveEscortDis = tlua.GetInt();
	m_reflushEscortTime = tlua.GetInt();
	m_robTimes = tlua.GetInt();
	m_escortReflushItemID = tlua.GetInt();
	m_escortRandomRate = tlua.GetInt();

	tlua.SetLuaFunction("GetGroupParam");
	if(tlua.Call(0, 4))
	{
		return -1;
	}
	m_opengroupModelmainLineID = tlua.GetInt();
	m_outputPlayerInfoNum = tlua.GetInt();
	m_groupMemberAddSomthigNum = tlua.GetInt();
	m_groupMemberNum = tlua.GetInt();

	tlua.SetLuaFunction("GetHorseParam");
	if(tlua.Call(0, 9))
	{
		return -1;
	}
	m_highestEatCostGolden = tlua.GetInt();
	m_simpleEatBaseExp = tlua.GetInt();
	m_eatHorseItemID = tlua.GetInt();
	m_horseBigCritTime    = tlua.GetInt();
	m_horseBigCritRate    = tlua.GetFloat();
	m_horseSmallCritTime  = tlua.GetInt();
	m_horseSmallCritRate  = tlua.GetFloat();
	m_baseAutoEatExpRand = tlua.GetFloat();
	m_baseAddExp = tlua.GetInt();

	tlua.SetLuaFunction("GetMagicWeaponParam");
	if(tlua.Call(0, 9))
	{
		return -1;
	}
	m_willBeZeroPer = tlua.GetInt();
	m_interDayLosePer = tlua.GetInt();
	m_maxInterDay = tlua.GetInt();
	m_canUseTempMapID = tlua.GetInt();
	m_magicCountDownTime = tlua.GetInt();
	m_SkillQuality = tlua.GetInt();
	m_FirstSkillQuality = tlua.GetInt();
	m_magicFlushItemID = tlua.GetInt();
	m_magicAdavnceItemID = tlua.GetInt();

	tlua.SetLuaFunction("GetArtifactMagicParam");
	if(tlua.Call(0, 9))
	{
		return -1;
	}

	m_artifactParam.m_willBeZeroPer = tlua.GetInt();
	m_artifactParam.m_interDayLosePer = tlua.GetInt();
	m_artifactParam.m_maxInterDay = tlua.GetInt();
	m_artifactParam.m_canUseTempMapID = tlua.GetInt();
	m_artifactParam.m_magicCountDownTime = tlua.GetInt();
	m_artifactParam.m_SkillQuality = tlua.GetInt();
	m_artifactParam.m_FirstSkillQuality = tlua.GetInt();
	m_artifactParam.m_magicFlushItemID = tlua.GetInt();
	m_artifactParam.m_magicAdavnceItemID = tlua.GetInt();

	tlua.SetLuaFunction("GetHeartMagicParam");
	if(tlua.Call(0, 9))
	{
		return -1;
	}

	m_heartParam.m_willBeZeroPer = tlua.GetInt();
	m_heartParam.m_interDayLosePer = tlua.GetInt();
	m_heartParam.m_maxInterDay = tlua.GetInt();
	m_heartParam.m_canUseTempMapID = tlua.GetInt();
	m_heartParam.m_magicCountDownTime = tlua.GetInt();
	m_heartParam.m_SkillQuality = tlua.GetInt();
	m_heartParam.m_FirstSkillQuality = tlua.GetInt();
	m_heartParam.m_magicFlushItemID = tlua.GetInt();
	m_heartParam.m_magicAdavnceItemID = tlua.GetInt();

	tlua.SetLuaFunction("GetMapParam");
	if(tlua.Call(0, 8))
	{
		return -1;
	}
	m_crossArenaMapID = tlua.GetInt();
	m_treasonKing_MapID = tlua.GetInt();
	m_countryWar_MapID = tlua.GetInt();
	m_clanWar_MapID = tlua.GetInt();
	m_arena_MapID = tlua.GetInt();
	m_freshMonsterArea = tlua.GetInt();
	m_defaultFreshMonster = tlua.GetInt();
	m_processTime = tlua.GetInt();

	tlua.SetLuaFunction("GetMapInstanceParam");
	if(tlua.Call(0, 4))
	{
		return -1;
	}
	m_lastChallengeMapID = tlua.GetInt();
	m_first_Challenge_MapID = tlua.GetInt();
	m_first_Group_MapID = tlua.GetInt();
	m_freshTime = tlua.GetInt();

	tlua.SetLuaFunction("GetEquipSmithPara");
	if (tlua.Call(0, 3))
	{
		return -1;
	}
	m_createStoneNeedNum = tlua.GetInt();
	m_costMoneyRate = tlua.GetInt();
	m_equipMaxLv = tlua.GetInt();

	tlua.SetLuaFunction("GetTitleIDParam");
	if(tlua.Call(0, 11))
	{
		return -1;
	}
	m_rankingWorshipAmountBegin = tlua.GetInt();
	m_rankingGirlWarBegin = tlua.GetInt();
	m_rankingAcupointBegin = tlua.GetInt();
	m_rankingDanLuBegin = tlua.GetInt();
	m_rankingMagicWeaponBegin = tlua.GetInt();
	m_rankingClanBegin = tlua.GetInt();
	m_rankingAchievementBegin = tlua.GetInt();
	m_rankingHorseBegin = tlua.GetInt();
	m_rankingFightBegin = tlua.GetInt();
	m_rankingLvBegin = tlua.GetInt();
	m_rankingTitleIDBegin = tlua.GetInt();

	tlua.SetLuaFunction("GetFashionParam");
	if (tlua.Call(0, 1))
	{
		return -1;
	}
	m_fashionID = tlua.GetInt();

	tlua.SetLuaFunction("GetItemIDParam");
	if(tlua.Call(0, 10))
	{
		return -1;
	}
	m_exchangeChallengeItem = tlua.GetInt();
	m_exchangeVipGolden = tlua.GetInt();
	m_exchangeCreditItemID = tlua.GetInt();
	m_exchangeCountryContributionItemID = tlua.GetInt();
	m_exchangeContributionItemID = tlua.GetInt();
	m_exchangeBindGoldenItemID = tlua.GetInt();
	m_exchangeGoldenItemID = tlua.GetInt();
	m_exchangeForceItemID = tlua.GetInt();
	m_exchangeExpItemID = tlua.GetInt();
	m_exchangeMoneyItemID = tlua.GetInt();

	tlua.SetLuaFunction("GetPrestigeMallPara");
	if(tlua.Call(0, 5))
	{
		return -1;
	}
	m_pretigeMallStage5 = tlua.GetInt();
	m_pretigeMallStage4 = tlua.GetInt();
	m_pretigeMallStage3 = tlua.GetInt();
	m_pretigeMallStage2 = tlua.GetInt();
	m_pretigeMallStage1 = tlua.GetInt();

	tlua.SetLuaFunction("GetDailyPara");
	if(tlua.Call(0, 1))
	{
		return -1;
	}
	m_dailyNeedGolden = tlua.GetInt();

	tlua.SetLuaFunction("GetObjectPara");
	if(tlua.Call(0, 11))
	{
		return -1;
	}

	m_decreaseExp = tlua.GetFloat();
	m_shoesNpc = tlua.GetInt();
	m_protectLv = tlua.GetInt();
	m_rangedDis = tlua.GetInt();
	m_muscleDis = tlua.GetInt();
	m_reduceSinValuePer = tlua.GetFloat();
	m_changePlayerStateTime = tlua.GetInt();
	m_specialSkillIDBegin = tlua.GetInt();
	m_addSinValue = tlua.GetInt();
	m_maxMonPlayerDiffLv = tlua.GetInt();
	m_minMonPlayerDiffLv = tlua.GetInt();

	tlua.SetLuaFunction("GetRewardHailPara");
	if(tlua.Call(0, 5))
	{
		return -1;
	}
	m_sigNum5 = tlua.GetInt();
	m_sigNum4 = tlua.GetInt();
	m_sigNum3 = tlua.GetInt();
	m_sigNum2 = tlua.GetInt();
	m_sigNum1 = tlua.GetInt();

	tlua.SetLuaFunction("GetResourceRecoverPara");
	if(tlua.Call(0, 6))
	{
		return -1;
	}
	m_recoverContriRate = tlua.GetInt();
	m_recoverGoldenMoneyRate = tlua.GetInt();
	m_recoverExpRate = tlua.GetInt();
	m_recoverForceRate = tlua.GetInt();
	m_recoverMoneyRate = tlua.GetInt();
	m_oneKeyRecoveRate = tlua.GetInt();

	tlua.SetLuaFunction("GetVipPartPara");
	if(tlua.Call(0, 1))
	{
		return -1;
	}
	m_vipCardLv = tlua.GetInt();
	
	tlua.SetLuaFunction("GetHPMPCD");
	if(tlua.Call(0,2))
	{
		return -1;
	}
	m_mpCD = tlua.GetInt();
	m_hpCD = tlua.GetInt();

	tlua.SetLuaFunction("GetCountryParam");
	if(tlua.Call(0, 1))
	{
		return -1;
	}
	m_CountryMap = tlua.GetInt();

	tlua.SetLuaFunction("GetCostItemIDParam");
	if(tlua.Call(0, 3))
	{
		return -1;
	}
	m_fly = tlua.GetInt();
	m_rose = tlua.GetInt();
	m_costItemRevival = tlua.GetInt();

	tlua.SetLuaFunction("GetSignInParam");
	if(tlua.Call(0, 1))
	{
		return -1;
	}
	m_addSignInCost = tlua.GetInt();
    //每日更新时间
	tlua.SetLuaFunction("GetDayUpdateTime");
	if(tlua.Call(0, 3))
	{
			return -1;
	}
	m_dayupdateSeconds = tlua.GetInt();
	m_dayUpdateMinute = tlua.GetInt();
	m_dayUpdateHour = tlua.GetInt();

	//每日竞技场礼包更新时间
	tlua.SetLuaFunction("GetCompleteUpdateTime");
	if(tlua.Call(0, 3))
	{
		return -1;
	}
	m_completeGiftSeconds = tlua.GetInt();
	m_completeGiftMinute  = tlua.GetInt();
	m_completeGiftHour    = tlua.GetInt();

	tlua.SetLuaFunction("GetArenaWarReportUpdateTime");
	if (tlua.Call(0, 3))
	{
		return -1;
	}
	m_arenaWarReportSecond = tlua.GetInt();
	m_arenaWarReportMinute = tlua.GetInt();
	m_arenaWarReportHour = tlua.GetInt();

	//周任务刷新消耗
	tlua.SetLuaFunction("GetWeekTask");

	if (tlua.Call(0,2))
	{
		return -1;
	}

	m_WeekTask = tlua.GetInt();
	m_FinishOne = tlua.GetInt();

	//组队副本buff加成ID
	tlua.SetLuaFunction("GetGroupInstanceBuffId");

	if (tlua.Call(0,1))
	{
		return -1;
	}

	m_GroupInstanceBuffId =  tlua.GetInt();

	return 0;
}

int GameParaLoader::GetExchangeItemIdByAtrrType(int atrrType)
{
	switch(atrrType)
	{
	case eCharMoney:
		return m_exchangeMoneyItemID;
	case eCharExp:
		return m_exchangeExpItemID;
	case eCharForce:
		return m_exchangeForceItemID;
	case eCharGolden:
		return m_exchangeGoldenItemID;
	case eCharBindGolden:
		return m_exchangeBindGoldenItemID;
	case eFaContribution:
		return m_exchangeContributionItemID;
	case eCurCountryContribute:
		return m_exchangeCountryContributionItemID;
	case eCountryContri:
		return m_exchangeCountryContributionItemID;
	case eCredit:
		return m_exchangeCreditItemID;
	case eChallengeMoney:
		return m_exchangeChallengeItem;
	default:
		return 0;
	}

	return 0;
}

int GameParaLoader::GetAtrrTypeByExchangeItemId(int ExchangeItemId)
{
	if(m_exchangeMoneyItemID == ExchangeItemId)
		return eCharMoney;
	else if(m_exchangeExpItemID == ExchangeItemId)
		return eCharExp;
	else if(m_exchangeForceItemID == ExchangeItemId)
		return eCharForce;
	else if(m_exchangeGoldenItemID == ExchangeItemId)
		return eCharGolden;
	else if(m_exchangeBindGoldenItemID == ExchangeItemId)
		return eCharBindGolden;
	else if(m_exchangeContributionItemID == ExchangeItemId)
		return eFaContribution;
	else if(m_exchangeCountryContributionItemID == ExchangeItemId)
		return eCurCountryContribute;
	else if(m_exchangeCreditItemID == ExchangeItemId)
		return eCredit;
	else if(m_exchangeChallengeItem == ExchangeItemId)
		return eChallengeMoney;
	return 0;
}

void GameParaLoader::getDayUpdateTime(int &hour,int &minute,int &seconds)
{
	hour = m_dayUpdateHour;
	minute = m_dayUpdateMinute;
	seconds = m_dayupdateSeconds;
}

void GameParaLoader::getCompleteGiftUpdateTime(int &hour,int &minute,int &second)
{
	hour = m_completeGiftHour;
	minute = m_completeGiftMinute;
	second = m_completeGiftSeconds;
}

void GameParaLoader::GetArenaWarReportUpdateTime(int& hour, int& minute, int& second)
{
	hour = m_arenaWarReportHour;
	minute = m_arenaWarReportMinute;
	second = m_arenaWarReportSecond;
}

int GameParaLoader::GetDailyQuestExp(int64 lvExp)
{
	tlua.SetLuaFunction("GetDailyQuestExp");
	tlua.SetFloat(lvExp);

	if(tlua.Call(1, 1))
	{
		return 0;
	}
	int temp = tlua.GetInt();

	return temp;
}

int GameParaLoader::GetDailyQuestMoney(int lv)
{
	tlua.SetLuaFunction("GetDailyQuestMoney");
	tlua.SetInt(lv);

	if(tlua.Call(1, 1))
	{
		return 0;
	}
	int temp = tlua.GetInt();

	return temp;
}

int GameParaLoader::GetEscortReward(int lv,int rate1,int rate2,int rete3,int qualityExp)
{
	tlua.SetLuaFunction("GetEscortReward");
	tlua.SetInt(lv);
	tlua.SetInt(rate1);
	tlua.SetInt(rate2);
	tlua.SetInt(rete3);
	tlua.SetInt(qualityExp);

	if(tlua.Call(5, 1))
	{
		return 0;
	}
	int temp = tlua.GetInt();

	return temp;
}

int GameParaLoader::GetAreaExp(int winExp,int loseExp)
{
	tlua.SetLuaFunction("GetAreaExp");
	tlua.SetInt(winExp);
	tlua.SetInt(loseExp);

	if(tlua.Call(2, 1))
	{
		return 0;
	}
	int temp = tlua.GetInt();

	return temp;
}
